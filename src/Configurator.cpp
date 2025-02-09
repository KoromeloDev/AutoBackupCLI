#include "Configurator.h"
#include "Print.h"
#include "Dir.h"

#include <QCoreApplication>
#include <QProcess>

Configurator::Configurator(QObject *parent, QString configName) : QObject(parent)
{
  m_configName = configName;
}

Configurator::~Configurator()
{

}

void Configurator::config()
{
  if (QFile::exists(m_configName + "/" + m_configName + ".json"))
  {
    configRemove();
  }

  configCreate();
}

void Configurator::configRemove()
{
  Print::warning("File is exist: " + m_configName + ".json");
  Print::info("Do you want to delete this configuration? (Write \"yes\" to delete it)");
  QTextStream input(stdin);

  if (const QString answer = input.readLine(); answer != "yes")
  {
    Print::warning("Cancelled");
    qDebug() << "Cancelled";
    emit configFinished(true);
  }

  QProcess process;
  QStringList args;
  args << "-l";
  process.start("crontab", args);
  process.waitForFinished();
  QString commandAll = process.readAllStandardOutput();
  const QString path = m_configName + "/cron.job";
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly))
  {
    Print::error("Failed to create file: " + file.fileName());
  }

  const QString job = file.readAll();
  file.close();

  if (!file.open(QIODevice::WriteOnly))
  {
    Print::error("Failed to create file: " + file.fileName());
  }

  commandAll.remove(job);
  file.write(commandAll.toUtf8());
  file.close();
  args.clear();
  args << QDir::currentPath() + "/" + path;
  process.start("crontab", args);
  process.waitForFinished();

  if (process.exitStatus() != 0)
  {
    Print::error(process.errorString());
  }

  QDir dir(m_configName);
  dir.removeRecursively();
  Print::success("Configuration has been deleted");
}

void Configurator::configCreate()
{
  Dir::create(m_configName);
  QProcess::execute("clear");
  Print::system("Creating a configuration");
  QTextStream input(stdin);
  setService(input);
  setDestination(input);
  setInclude(input);
  setExclude(input);
  setCompress(input);
  setFrequency(input);
  setCount(input);
  QProcess::execute("clear");
  emit configFinished(m_conf.write(m_configName + ".json"));
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

void Configurator::setInclude(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the include file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.include = input.readLine();

  if (!m_conf.info.include.isEmpty() || !QFile::exists(m_conf.info.include))
  {
    Print::warning("File is not exist: " + m_conf.info.include);
    const QString name = m_configName + ".include";
    createEmptyFile(m_configName + "/" + name);
    m_conf.info.include = name;
  }
}

void Configurator::setExclude(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the exclude file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.exclude = input.readLine();

  if (!m_conf.info.exclude.isEmpty() || !QFile::exists(m_conf.info.exclude))
  {
    Print::warning("File is not exist: " + m_conf.info.exclude);
    const QString name = m_configName + ".exclude";
    createEmptyFile(m_configName + "/" + name);
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

void Configurator::setFrequency(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter a value for crontab:");
  m_conf.info.frequency = input.readLine();
  const QString executable = QCoreApplication::applicationDirPath() + "/" + PROJECT_NAME;
  QProcess process;
  QStringList args;
  args << "-l";
  process.start("crontab", args);
  process.waitForFinished();
  const QString command = m_conf.info.frequency + " " + executable + " run " + m_configName + "\n";
  const QString commandAll = process.readAllStandardOutput() + command;
  const QString path = m_configName + "/cron.job";
  QFile file(path);

  if (!file.open(QIODevice::WriteOnly))
  {
    Print::error("Failed to create file: " + file.fileName());
  }

  file.write(commandAll.toUtf8());
  file.close();
  args.clear();
  args << QDir::currentPath() + "/" + path;
  process.start("crontab", args);
  process.waitForFinished();

  if (process.exitStatus() != 0)
  {
    Print::error(process.errorString());
  }

  if (!file.open(QIODevice::WriteOnly))
  {
    Print::error("Failed to create file: " + file.fileName());
  }

  file.resize(0);
  file.write(command.toUtf8());
  file.close();
}

void Configurator::setCount(QTextStream &input)
{
  QProcess::execute("clear");
  Print::system("Enter the number of backups you want to have at the same time (Enter 0 if you do not want the backups to be deleted):");
  m_conf.info.count = input.readLine().toUInt();
}