#include "Packager.h"

#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QFile>

Packager::Packager(QObject *parent, quint8 level, QStringList files) : QObject(parent)
{
  m_level = level == 0 ? 1 : level > 9 ? 9 : level;

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

Packager::~Packager()
{

}

void Packager::pack(QString name)
{
  if (m_files.isEmpty())
  {
    qDebug() << "\033[31m" << "No required files" << "\033[0m";
    emit packageFinished(false, "");
  }

  const QDateTime currentDate = QDateTime::currentDateTime();
  const QString currentDateString = currentDate.toString("dd.MM.yyyy HH-mm");
  QString archiveName = name + currentDateString + ".tar";
  QProcess process;
  QStringList args;
  args << "-czf" << archiveName << m_files;
  process.start("tar", args);
  process.waitForFinished();
  const QString command = QString("gzip -%1 \"%2\"").arg(QString::number(m_level), archiveName);
  process.startCommand(command);
  process.waitForFinished();
  emit packageFinished(!process.exitCode(), archiveName + ".gz");
}

bool Packager::remove(QString path)
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
