#include "Loader.h"

#include <QFile>
#include <QDebug>
#include <QProcess>

Loader::Loader(QObject *parent, QString configName, QString file, QString path) : QObject(parent)
{
  m_configName = configName;
  m_file = file;
  m_path = path;
}

Loader::~Loader()
{

}

void Loader::load()
{
  if (!QFile::exists(m_file))
  {
    qDebug() << "\033[31m" << "File is not exist:" << m_file << "\033[0m";
    emit loadingFinished(false);
  }

  QProcess process;
  QStringList args;
  args << "copy" <<  m_file << m_configName + ":" + m_path;
  process.start("rclone", args);
  qDebug() << "Start loading...";
  process.waitForFinished();
  emit loadingFinished(!process.exitCode());
}