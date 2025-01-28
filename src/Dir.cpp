#include "Dir.h"
#include "Print.h"

void Dir::setPath(QString name)
{
  const QString path = QDir::homePath() + "/.config/" + PROJECT_NAME + "/" + name;

  if(!QDir(path).exists() && !QDir(path).mkdir(path))
  {
    Print::warning("Failed to create directory: " + path);
  }

  QDir::setCurrent(path);
}


void Dir::create(QString name)
{
  name = QDir::currentPath() + "/" + name;

  if(!QDir(name).exists() && !QDir(name).mkdir(name))
  {
    Print::warning("Failed to create directory: " + name);
  }
}