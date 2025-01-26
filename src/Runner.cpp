#include "Runner.h"

#include <QCoreApplication>

Runner::Runner(QObject *parent) : QObject(parent)
{

}

Runner::~Runner()
{

}

void Runner::start()
{
  if (QCoreApplication::arguments().count() < 3)
  {
    qDebug() << "\033[31m" << "Unknown command" << "\033[0m";
    exit(1);
  }

  if (const QString command = QCoreApplication::arguments().at(1); command == "search")
  {
    search();
  }
  else if (command == "pack")
  {
    pack();
  }
  else if (command == "load")
  {
    load();
  }
  else if (command == "config")
  {
    config();
  }
  else
  {
    qDebug() << "\033[31m" << "Unknown command:" << command << "\033[0m";
    exit(1);
  }
}

void Runner::search()
{
  QString includeFile = QCoreApplication::arguments().at(2);
  QString excludeFile = nullptr;

  if (QCoreApplication::arguments().count() >= 4)
  {
    excludeFile = QCoreApplication::arguments().at(3);
  }

  m_search = m_search.create(this, includeFile, excludeFile);
  connect(m_search.get(), &Search::searchFinished, this, &Runner::searchFinished);
  m_search->search();
}

void Runner::pack(QStringList files)
{
  quint8 level = QCoreApplication::arguments().at(2).toUInt();

  if (files.isEmpty())
  {
    if (QCoreApplication::arguments().count() >= 4)
    {
      for (quint8 i = 3; i < QCoreApplication::arguments().count(); ++i)
      {
        files += QCoreApplication::arguments().at(i);
      }
    }
    else
    {
      qDebug() << "\033[31m" << "No required parameter: FILES" << "\033[0m";
      exit(1);
    }
  }

  m_packager = m_packager.create(this, level, files);
  connect(m_packager.get(), &Packager::packageFinished, this, &Runner::packageFinished);
  m_packager->pack("Config ");
}

void Runner::load()
{
  if (QCoreApplication::arguments().count() == 3)
  {
    qDebug() << "\033[31m" << "No required parameter: FILE" << "\033[0m";
    exit(1);
  }

  const QString configName = QCoreApplication::arguments().at(2);
  const QString file = QCoreApplication::arguments().at(3);
  QString path = "/";

  if (QCoreApplication::arguments().count() >= 5)
  {
    path = QCoreApplication::arguments().at(4);
  }

  m_loader = m_loader.create(this, configName, file , path);
  connect(m_loader.get(), &Loader::loadingFinished, this, &Runner::loadingFinished);
  m_loader->load();
}

void Runner::config()
{
  QString configName = QCoreApplication::arguments().at(2);
  m_configurator = m_configurator.create(this, configName);
  connect(m_configurator.get(), &Configurator::configFinished, this, &Runner::configFinished);
  m_configurator->config();
}

void Runner::searchFinished(bool success, QStringList files)
{
  if (success)
  {
    for (const auto &file : files)
    {
      qDebug() << "Find:" << file;
    }

    exit(0);
  }

  qDebug() << "\033[31m" << "Not find!" << "\033[0m";
  exit(1);
}

void Runner::packageFinished(bool success, QString path)
{
  if (success)
  {
    qDebug() << "Path:" << path;
    exit(0);
  }

  qDebug() << "\033[31m" << "Error packing!" << "\033[0m";
  exit(1);
}

void Runner::loadingFinished(bool success)
{
  if (success)
  {
    qDebug() << "\033[32m" << "Loaded" << "\033[0m";
    exit(0);
  }

  qDebug() << "\033[31m" << "Error loading!" << "\033[0m";
  exit(1);
}

void Runner::configFinished(bool success)
{
  if (!success)
  {
    qDebug() << "\033[31m" << "Error creating configuration!" << "\033[0m";
  }

  exit(!success);
}
