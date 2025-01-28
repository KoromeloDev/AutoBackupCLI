#include "Configurator.h"
#include "Print.h"
#include "Dir.h"

Configurator::Configurator(QObject *parent, QString configName) : QObject(parent)
{
  m_configName = configName + ".json";
}

Configurator::~Configurator()
{

}

void Configurator::config()
{
  if (QFile::exists(m_configName))
  {
    configRemove();
  }

  configCreate();
}

void Configurator::configRemove()
{
  Print::warning("File is  exist: " + m_configName);
  Print::info("Do you want to delete this configuration? (Write \"yes\" to delete it)");
  QTextStream input(stdin);

  if (const QString answer = input.readLine(); answer == "yes")
  {
    Print::info("Configuration has been deleted");
    emit configFinished(true);
  }

  Print::warning("Cancelled");
  qDebug() << "Cancelled";
  emit configFinished(true);
}

void Configurator::configCreate()
{
  QString folder = m_configName;
  folder.chop(5);
  Dir::create(folder);
  QTextStream input(stdin);
  Print::info("Creating a configuration");
  Print::info("Enter the name of the rclone configuration:");
  m_conf.info.service = input.readLine();
  Print::info("Enter the destination path for rclone configuration:");
  m_conf.info.destination = input.readLine();
  Print::info("Enter the include file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.include = input.readLine();

  if (!m_conf.info.include.isEmpty() || !QFile::exists(m_conf.info.include))
  {
    Print::warning("File is not exist: " + m_conf.info.include);
    QString path = folder + "/" + folder + ".include";
    createEmptyFile(path);
    m_conf.info.include = path;
  }

  Print::info("Enter the exclude file path for search (By default creates a file in the conifguration folder):");
  m_conf.info.exclude = input.readLine();

  if (!m_conf.info.exclude.isEmpty() || !QFile::exists(m_conf.info.exclude))
  {
    Print::warning("File is not exist: " + m_conf.info.exclude);
    QString path = folder + "/" + folder + ".exclude";
    createEmptyFile(path);
    m_conf.info.exclude = path;
  }

  Print::info("Enter the compress level for packing (By default, the maximum value is used):");
  m_conf.info.compress = input.readLine().toUInt();

  if (m_conf.info.compress > 9)
  {
    Print::warning("Unknown value: " + m_conf.info.compress);
    m_conf.info.compress = 9;
  }

  Print::info("Enter a value for crontab:");
  m_conf.info.frequency = input.readLine();
  Print::info("Enter the number of backups you want to have at the same time (Enter 0 if you do not want the backups to be deleted):");
  m_conf.info.count = input.readLine().toUInt();
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
