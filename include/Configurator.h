#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include "ConfFile.h"

#include <QObject>

class Configurator : public QObject
{
Q_OBJECT

public:
  explicit Configurator(QObject *parent = nullptr, QString configName = "");
  ~Configurator();

  void config();

private:
  QString m_configName;
  ConfFile m_conf;

  void configRemove();
  void configCreate();
  void createEmptyFile(QString path);
  void setService(QTextStream &input);
  void setDestination(QTextStream &input);
  void setInclude(QTextStream &input);
  void setExclude(QTextStream &input);
  void setCompress(QTextStream &input);
  void setFrequency(QTextStream &input);
  void setCount(QTextStream &input);

signals:
  void configFinished(bool success);
};

#endif //CONFIGURATOR_H
