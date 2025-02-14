#include "Runner.h"
#include "Print.h"

#include <QCoreApplication>
#include <QProcess>
#include <QJsonArray>

#include "Dir.h"

Runner::Runner(QObject *parent) : QObject(parent)
{

}

Runner::~Runner()
{

}

void Runner::start()
{
  if (QCoreApplication::arguments().count() < 3)
  {
    Print::error("Unknown command");
  }

  if (const QString command = QCoreApplication::arguments().at(1); command == "search")
  {
    search();
  }
  else if (command == "pack")
  {
    pack();
  }
  else if (command == "load")
  {
    load();
  }
  else if (command == "config")
  {
    config();
  }
  else if (command == "run")
  {
    run();
  }
  else
  {
    Print::error("Unknown command");
  }
}

void Runner::search(QString includeFile, QString excludeFile)
{
  if (includeFile == nullptr)
  {
    includeFile = QCoreApplication::arguments().at(2);
  }

  if (excludeFile == nullptr && QCoreApplication::arguments().count() >= 4)
  {
    excludeFile = QCoreApplication::arguments().at(3);
  }

  m_search = m_search.create(this, includeFile, excludeFile);
  connect(m_search.get(), &Search::searchFinished, this, &Runner::searchFinished);
  m_search->search();
}

void Runner::pack(QStringList files, quint8 level)
{
  if (level == 0)
  {
    level = QCoreApplication::arguments().at(2).toUInt();
  }

  if (files.isEmpty())
  {
    if (QCoreApplication::arguments().count() < 4)
    {
      Print::error("No required parameter: FILES");
    }

    for (quint8 i = 3; i < QCoreApplication::arguments().count(); ++i)
    {
      files += QCoreApplication::arguments().at(i);
    }
  }

  m_packager = m_packager.create(this, level, files);
  connect(m_packager.get(), &Packager::packageFinished, this, &Runner::packageFinished);
  QString name = "Config ";

  if (m_isWaiting)
  {
    name = m_configName + " ";
  }

  m_packager->pack(name);
}

void Runner::load(QString configName, QString file, QString path)
{
  if (configName == nullptr)
  {
    configName = QCoreApplication::arguments().at(2);
  }

  if (file == nullptr && QCoreApplication::arguments().count() == 3)
  {
    Print::error("No required parameter: FILES");
  }

  if (file == nullptr)
  {
    file = QCoreApplication::arguments().at(3);
  }

  if (path == nullptr)
  {
    path = "/";

    if (QCoreApplication::arguments().count() >= 5)
    {
      path = QCoreApplication::arguments().at(4);
    }
  }

  m_loader = m_loader.create(this, configName, file, path);
  connect(m_loader.get(), &Loader::loadingFinished, this, &Runner::loadingFinished);
  m_loader->load();
}

void Runner::config()
{
  QString configName = QCoreApplication::arguments().at(2);
  m_configurator = m_configurator.create(this, configName);
  connect(m_configurator.get(), &Configurator::configFinished, this, &Runner::configFinished);
  m_configurator->config();
}

void Runner::run()
{
  m_configName = QCoreApplication::arguments().at(2);

  if(!QDir(m_configName).exists())
  {
    Print::error("The folder doesn't exist: " + m_configName);
  }

  if (!QFile::exists(m_configName + "/" + m_configName + ".json"))
  {
    Print::error("The config doesn't exist: " + m_configName);
  }

  Dir::setPath(m_configName);
  m_config = m_config.create();
  m_config->read(m_configName + ".json");
  m_isWaiting = true;
  search(m_config->info.include, m_config->info.exclude);
}

void Runner::searchFinished(bool success, QStringList files)
{
  if (success)
  {
    for (const auto &file : files)
    {
      Print::info("Find: " + file);
    }

    if (m_isWaiting)
    {
      Print::info("Search is done!");
      afterSearch(files);
    }

    Print::success("Search is done!");
  }

  Print::error("Not find!");
}

void Runner::packageFinished(bool success, QString path)
{
  if (success)
  {
    Print::info("Path: " + path);

    if (m_isWaiting)
    {
      Print::system("Package is done!");
      afterPacking(path);
    }

    Print::success("Packing is done!");
  }

  Print::error("Error packing!");
}

void Runner::loadingFinished(bool success)
{
  if (success)
  {
    if (m_isWaiting)
    {
      Print::system("Loaded!");
      afterLoading();
    }

    Print::success("Loaded!");
  }

  Print::error("Error loading!");
}

void Runner::configFinished(bool success)
{
  if (success)
  {
    Print::success("Config created!");
  }

  Print::error("Error creating configuration!");
}

void Runner::runFinished(bool success)
{
  if (success)
  {
    Print::success("All task done!");
  }

  Print::error("Error!");
}

void Runner::afterSearch(QStringList files)
{
  pack(files, m_config->info.compress);
}

void Runner::afterPacking(QString path)
{
  m_removePath = path;
  load(m_config->info.service, path, m_config->info.destination);
}

void Runner::afterLoading()
{
  m_packager->remove(m_removePath);
  const quint8 count = m_config->info.count;
  QProcess process;
  QStringList args;
  QString path = m_config->info.service + ":" + m_config->info.destination;
  args << "lsjson" << path;
  process.start("rclone", args);
  process.waitForFinished();
  const QByteArray jsonData = process.readAllStandardOutput();
  const QJsonDocument document = QJsonDocument::fromJson(jsonData);
  const QJsonArray array = document.array();
  struct FileInfo
  {
    QString name;
    QDate date;
  };
  QVector<FileInfo> files;

  for (const QJsonValue &value : array)
  {
    const QJsonObject object = value.toObject();
    const QString mimeType = object["MimeType"].toString();

    if (mimeType != "application/gzip")
    {
      continue;
    }

    FileInfo file;
    file.name = object["Name"].toString();
    file.date = QDate::fromString(object["ModTime"].toString().first(10), Qt::ISODate);
    files.append(file);
  }

  if (count >= files.count() || count == 0)
  {
    Print::system("Nothing to delete");
    runFinished(true);
  }

  const quint8 needToDelete = files.count() - count;
  std::ranges::sort(files, [](const FileInfo &a, const FileInfo &b)
  {
    return a.date < b.date;
  });

  for (quint8 i = 0; i < needToDelete; ++i)
  {
    Print::info("Deleting: " + files[i].name);
    args.clear();
    args << "delete" << path + "/" + files[i].name;
    process.start("rclone", args);
    process.waitForFinished();

    if (process.exitCode())
    {
      runFinished(false);
    }
  }

  runFinished(true);
}