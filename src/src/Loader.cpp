#include "Loader.h"

#include <QFile>
#include <QDebug>
#include <QProcess>

Loader::Loader(QObject *parent, QString configName, QString file, QString path) : QObject(parent)
{
  if (!QFile::exists(file))
  {
    qDebug() << "\033[31m" << "File is not exist:" << file << "\033[0m";
    emit loadingFinished(false);
  }

  m_configName = configName;
  m_file = file;
  m_path = path;
}

Loader::~Loader()
{

}

void Loader::load()
{
  QProcess process;
  QStringList args;
  args << "sync" << m_file << m_configName + ":" + m_path;
  process.start("rclone", args);
  qDebug() << "Start loading...";
  quint8 exitCode = process.exitCode();
  process.waitForFinished();
  emit loadingFinished(!exitCode);
}
