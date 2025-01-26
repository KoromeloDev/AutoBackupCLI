#include "Configurator.h"

#include <QFile>
#include <QDebug>

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
  qDebug() << "\033[33m" << "File is  exist:" << m_configName << "\033[0m";
  qDebug() << "Do you want to delete this configuration? (Write \"yes\" to delete it)";
  QTextStream input(stdin);

  if (const QString answer = input.readLine(); answer == "yes")
  {
    qDebug() << "\033[31m" << "Configuration has been deleted" << "\033[0m";
    emit configFinished(true);
  }

  qDebug() << "Cancelled";
  emit configFinished(true);
}

void Configurator::configCreate()
{
  QTextStream input(stdin);
  qInfo() << "Creating a configuration";
  qInfo() << "Enter the name of the rclone configuration:";
  m_conf.info.service = input.readLine();
  qInfo() << "Enter the destination path for rclone configuration:";
  m_conf.info.destination = input.readLine();
  qInfo() << "Enter the include file path for search (By default creates a file in the conifguration folder):";
  m_conf.info.include = input.readLine();

  if (!m_conf.info.include.isEmpty() && !QFile::exists(m_conf.info.include))
  {
    qWarning() << "\033[33m" << "File is  exist:" << m_conf.info.include << "\033[0m";
  }

  qInfo() << "Enter the exclude file path for search (By default creates a file in the conifguration folder):";
  m_conf.info.exclude = input.readLine();

  if (!m_conf.info.exclude.isEmpty() && !QFile::exists(m_conf.info.exclude))
  {
    qWarning() << "\033[33m" << "File is  exist:" << m_conf.info.exclude << "\033[0m";
  }

  qInfo() << "Enter the compress level for packing (By default, the maximum value is used):";
  m_conf.info.compress = input.readLine().toUInt();

  if (m_conf.info.compress > 9)
  {
    qWarning() << "\033[33m" << "Unknown value" << m_conf.info.compress << "\033[0m";
    m_conf.info.compress = 9;
  }

  qInfo() << "Enter a value for crontab:";
  m_conf.info.frequency = input.readLine();
  qInfo() << "Enter the number of backups you want to have at the same time (Enter 0 if you do not want the backups to be deleted):";
  m_conf.info.count = input.readLine().toUInt();
  emit configFinished(m_conf.write(m_configName));
}
