#include "Configurator.h"
#include "Print.h"
#include "Dir.h"

#include <QCoreApplication>
#include <QProcess>

Configurator::Configurator(QObject *parent, QString configName) : QObject(parent)
{
  m_configName = configName + ".json";
}

Configurator::~Configurator()
{

}

void Configurator::config()
{
  QString folder = m_configName;
  folder.chop(5);

  if (QFile::exists(folder + "/" + m_configName))
  {
    configRemove(folder);
  }

  configCreate();
}

void Configurator::configRemove(QString folder)
{
  Print::warning("File is exist: " + m_configName);
  Print::info("Do you want to delete this configuration? (Write \"yes\" to delete it)");
  QTextStream input(stdin);

  if (const QString answer = input.readLine(); answer == "yes")
  {
    QDir dir(folder);
    dir.removeRecursively();
    Print::info("Configuration has been deleted");
    emit configFinished(true);
  }

  Print::warning("Cancelled");
  qDebug() << "Cancelled";
  emit configFinished(true);
}

void Configurator::configCreate()
{
  QProcess::execute("clear");
  Print::system("Creating a configuration");
  QString folder = m_configName;
  folder.chop(5);
  Dir::create(folder);
  QTextStream input(stdin);
  setService(input);
  setDestination(input);
  setInclude(input, folder);
  setExclude(input, folder);
  setCompress(input);
  setFrequency(input, folder);
  setCount(input);
  QProcess::execute("clear");
  emit configFinished(m_conf.write(m_configName));
}

void Configurator::createEmptyFile(QString path)
{
  QFile file(path);

  if (!file.open(QIODevice::WriteOnly))
  {
    Print::warning("Failed to create file: " + path);
  }

  file.close();
}

void Configurator::setService(QTextStream &input)
{
  Print::system("Enter the name of the rclone configuration:");
  m_conf.info.service = input.readLine();
}

void Configurator::setDestination(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the destination path for rclone configuration:");
  m_conf.info.destination = input.readLine();
}

void Configurator::setInclude(QTextStream &input, QString &folder)
{
  QProcess::execute("clear");
  Print::system("Enter the include file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.include = input.readLine();

  if (!m_conf.info.include.isEmpty() || !QFile::exists(m_conf.info.include))
  {
    Print::warning("File is not exist: " + m_conf.info.include);
    const QString name = folder + ".include";
    createEmptyFile(folder + "/" + name);
    m_conf.info.include = name;
  }
}

void Configurator::setExclude(QTextStream &input, QString &folder)
{
  QProcess::execute("clear");
  Print::system("Enter the exclude file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.exclude = input.readLine();

  if (!m_conf.info.exclude.isEmpty() || !QFile::exists(m_conf.info.exclude))
  {
    Print::warning("File is not exist: " + m_conf.info.exclude);
    const QString name = folder + ".exclude";
    createEmptyFile(folder + "/" + name);
    m_conf.info.exclude = name;
  }
}

void Configurator::setCompress(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the compress level for packing (By default, the maximum value is used):");
  m_conf.info.compress = input.readLine().toUInt();

  if (m_conf.info.compress > 9)
  {
    Print::warning("Unknown value: " + m_conf.info.compress);
    m_conf.info.compress = 9;
  }
}

void Configurator::setFrequency(QTextStream &input, QString &folder)
{
  QProcess::execute("clear");
  Print::system("Enter a value for crontab:");
  m_conf.info.frequency = input.readLine();
  const QString executable = QCoreApplication::applicationDirPath() + "/" + PROJECT_NAME;
  QProcess process;
  QString command = m_conf.info.frequency + " " + executable + " run " + folder + "\n";
  const QString path = folder + "/cron.job";
  QFile file(path);

  if (!file.open(QIODevice::WriteOnly))
  {
    Print::error("Failed to create file: " + file.fileName());
  }

  file.write(command.toUtf8());
  file.close();
  QStringList args;
  args << QDir::currentPath() + "/" + path;
  process.start("crontab", args);
  process.waitForFinished();

  if (process.exitStatus() != 0)
  {
    Print::error(process.errorString());
  }
}

void Configurator::setCount(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the number of backups you want to have at the same time (Enter 0 if you do not want the backups to be deleted):");
  m_conf.info.count = input.readLine().toUInt();
}