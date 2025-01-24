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

  const QString command = QCoreApplication::arguments().at(1);

  if (command == "search")
  {
    search();
  }
  else if (command == "pack")
  {
    pack();
  }
  else
  {
    qDebug() << "\033[31m" << "Unknown command:" << command << "\033[0m";
    exit(1);
  }
}

void Runner::search()
{
  const QString includeFile(QCoreApplication::arguments().at(2));
  QString excludeFile = nullptr;

  if (QCoreApplication::arguments().count() >= 4)
  {
    excludeFile = QCoreApplication::arguments().at(3);
  }

  m_search = m_search.create(this, includeFile, excludeFile);
  connect(m_search.get(), &Search::searchFinished, this, &Runner::searchFinished);
  m_search->search();
}

void Runner::pack()
{

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
