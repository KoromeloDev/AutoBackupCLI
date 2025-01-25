#include "Pack.h"

#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QFile>

Pack::Pack(QObject *parent, quint8 level, QStringList files) : QObject(parent)
{
  m_level = level > 9 ? 9 : level;

  if (files.isEmpty())
  {
    qDebug() << "\033[31m" << "No required files" << "\033[0m";
    emit packageFinished(false, "");
  }

  for (auto &file : files)
  {
    if (QFile::exists(file))
    {
      m_files += file;
    }
    else
    {
      qDebug() << "\033[31m" << "Non-existing file:" << file << "\033[0m";
    }
  }
}

Pack::~Pack()
{

}

void Pack::pack(QString name)
{
  const QDateTime currentDate = QDateTime::currentDateTime();
  const QString currentDateString = currentDate.toString("dd.MM.yyyy HH-mm");
  QString archiveName = name + currentDateString + ".tar";

  for (int i = 0; i < archiveName.length(); ++i)
  {
    if (archiveName[i] == ' ')
    {
      archiveName[i] = '_';
    }
  }

  QProcess process;
  QStringList argument;
  argument << "-czf" << archiveName << m_files;
  process.start("tar", argument);
  process.waitForFinished();
  QString command = QString("gzip -%1 %2").arg(QString::number(m_level), archiveName);
  process.startCommand(command);
  quint8 exitCode = process.exitCode();
  process.waitForFinished();
  emit packageFinished(!exitCode, archiveName + ".gz");
}

bool Pack::remove(QString path)
{
  QFile file(path);

  if (!file.exists())
  {
    qDebug() << "\033[31m" << "File not exist:" << path << "\033[0m";
    return false;
  }

  if (file.remove())
  {
    qDebug() << "\033[32m" << "The file has been successfully deleted:" << path << "\033[0m";
    return true;
  }

  qDebug() << "\033[31m" << "Failed to delete the file:" << path << "\033[0m";
  return false;

}
