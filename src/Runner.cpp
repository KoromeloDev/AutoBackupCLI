#include "Runner.h"
#include "Print.h"

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
    Print::error("Unknown command");
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
    Print::error("Unknown command");
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
      Print::error("No required parameter: FILES");
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
    Print::error("No required parameter: FILES");
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
      Print::info("Find: " + file);
    }

    Print::success("Done!");
  }

  Print::error("Not find!");
}

void Runner::packageFinished(bool success, QString path)
{
  if (success)
  {
    Print::info("Path: " + path);
    Print::success("Done!");
  }

  Print::error("Error packing!");
}

void Runner::loadingFinished(bool success)
{
  if (success)
  {
    Print::success("Loaded!");
  }

  Print::error("Error loading!");
}

void Runner::configFinished(bool success)
{
  if (success)
  {
    Print::success("Done!");
  }

  Print::error("Error creating configuration!");
}
