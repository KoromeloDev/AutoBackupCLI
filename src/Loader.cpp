#include "Loader.h"
#include "Print.h"

#include <QFile>
#include <QProcess>

Loader::Loader(QObject *parent, QString configName, QString file, QString path) : QObject(parent)
{
  m_configName = configName;
  m_file = file;
  m_path = path;
}

Loader::~Loader() = default;

void Loader::load()
{
  if (!QFile::exists(m_file))
  {
    Print::error("File is not exist: " + m_file);
  }

  QProcess process;
  QStringList args;
  args << "copy" <<  m_file << m_configName + ":" + m_path;
  process.start("rclone", args);
  Print::system("Start loading...");
  process.waitForFinished();
  emit loadingFinished(!process.exitCode());
}