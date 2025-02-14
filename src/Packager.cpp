#include "Packager.h"
#include "Print.h"

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
      Print::error("File is not exist: " + file);
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
    Print::error("No required files");
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
    Print::error("File is not exist: " + path);
    return false;
  }

  if (file.remove())
  {
    Print::system("The temp file has been successfully deleted: " + path);
    return true;
  }

  Print::warning("Failed to delete the file: " + path);
  return false;
}
