#include <QCoreApplication>
#include <QDir>

#include "Search.h"

//Creates folders in the current directory
void createPath(const QVector<QString> &pathList)
{
  for (QString path : pathList)
  {
    path = QDir::currentPath() + "/" + path;

    if(!QDir(path).exists() && !QDir(path).mkdir(path))
    {
      qDebug() << "Failed to create directory: " << path;
    }
  }
}

//Creating a folder with configuration files
void setPath()
{
  QString path;

  path.append(QDir::homePath());
  path.append("/.config/");
  path.append(PROJECT_NAME);

  if(!QDir(path).exists() && !QDir(path).mkdir(path))
  {
    qDebug() << "Failed to create directory: " << path;
  }

  QDir::setCurrent(path);
}

void startCommand()
{
  if (QCoreApplication::arguments().count() < 3)
  {
    return;
  }

  const QString command = QCoreApplication::arguments().at(1);

  if (command == "search")
  {
    const QString includeFile(QCoreApplication::arguments().at(2));
    const QString excludeFile(QCoreApplication::arguments().at(3));
    Search search(includeFile, excludeFile);
    search.search();
  }
  else
  {
    qDebug() << "Unknown command";
  }


}

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  QCoreApplication::setApplicationVersion(APP_VERSION "-beta");
  setPath();
  startCommand();

  return QCoreApplication::exec();
}
