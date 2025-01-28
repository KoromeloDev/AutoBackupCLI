#include "ConfFile.h"

#include <QFile>

bool ConfFile::read(QString configName)
{
  QFile file(configName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QByteArray jsonData = file.readAll();
  file.close();
  QJsonDocument document = QJsonDocument::fromJson(jsonData);
  QJsonObject config = document.object();

  if (config.isEmpty())
  {
    return false;
  }

  info.service = config["service"].toString();
  info.destination = config["destination"].toString();
  info.include = config["include"].toString();
  info.exclude = config["exclude"].toString();
  info.compress = config["compress"].toInt();
  info.frequency = config["frequency"].toString();
  info.count = config["count"].toInt();
  return true;
}

bool ConfFile::write(QString configName)
{
  QString folder = configName;
  folder.chop(5);
  folder += "/";
  QFile file(folder + configName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return false;
  }

  QJsonObject config;
  config["service"] = info.service;
  config["destination"] = info.destination;
  config["include"] = info.include;
  config["exclude"] = info.exclude;
  config["compress"] = info.compress;
  config["frequency"] = info.frequency;
  config["count"] = info.count;
  QJsonDocument document(config);
  file.write(document.toJson());
  file.close();
  return true;
}
