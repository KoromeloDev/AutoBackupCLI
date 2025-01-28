#ifndef DIR_H
#define DIR_H

#include <QDir>

class Dir
{
public:
  static void setPath(QString name = {});
  static void create(QString name);

};

#endif //DIR_H
