#include <QCoreApplication>
#include <QDir>

#include "Runner.h"

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

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  QCoreApplication::setApplicationVersion(APP_VERSION "-beta");
  setPath();
  Runner runner;
  runner.start();
  return QCoreApplication::exec();
}
