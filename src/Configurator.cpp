#include "Configurator.h"

#include <iostream>
#include <QFile>
#include <QJsonObject>

Configurator::Configurator(QObject *parent, QString configName) : QObject(parent)
{
  m_configName = configName + ".conf";
}

Configurator::~Configurator()
{

}

void Configurator::config()
{
  if (QFile::exists(m_configName))
  {
    qDebug() << "\033[33m" << "File is  exist:" << m_configName << "\033[0m";
    qDebug() << "Do you want to delete this configuration? (Write \"yes\" to delete it)";
    QTextStream input(stdin);
    const QString answer = input.readLine();

    if (answer == "yes")
    {
      qDebug() << "\033[31m" << "Configuration has been deleted" << "\033[0m";
      emit configFinished(true);
    }

    qDebug() << "Cancelled";
    emit configFinished(true);
  }

  QTextStream input(stdin);
  qDebug() << "Creating a configuration";
  qDebug() << "Enter the name of the rclone configuration:";
  const QString service = input.readLine();
  qDebug() << "Enter the destination path for rclone configuration:";
  const QString destination = input.readLine();
  qDebug() << "Enter the include file path for search (By default creates a file in the conifguration folder):";
  const QString include = input.readLine();

  if (!include.isEmpty() && !QFile::exists(include))
  {
    qDebug() << "\033[33m" << "File is  exist:" << include << "\033[0m";
  }

  qDebug() << "Enter the exclude file path for search (By default creates a file in the conifguration folder):";
  const QString exclude = input.readLine();

  if (!exclude.isEmpty() && !QFile::exists(exclude))
  {
    qDebug() << "\033[33m" << "File is  exist:" << exclude << "\033[0m";
  }

  qDebug() << "Enter the compress level for packing (By default, the maximum value is used):";
  quint8 compress = input.readLine().toUInt();

  if (compress > 9)
  {
    qDebug() << "\033[33m" << "Unknown value" << compress << "\033[0m";
    compress = 9;
  }

  qDebug() << "Enter a value for crontab:";
  const QString frequency = input.readLine();
  qDebug() << "Enter the number of backups you want to have at the same time (Enter 0 if you do not want the backups to be deleted):";
  const quint8 count = input.readLine().toUInt();





  emit configFinished(false);
}
