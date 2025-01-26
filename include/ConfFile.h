#ifndef CONFFILE_H
#define CONFFILE_H

#include <QJsonObject>
#include <QJsonDocument>

class ConfFile
{
public:
  struct Info
  {
    QString service;
    QString destination = "/";
    QString include;
    QString exclude;
    quint8 compress = 9;
    QString frequency;
    quint8 count = 0;
  } info;

  bool read(QString configName);
  bool write(QString configName);
};

#endif //CONFFILE_H
