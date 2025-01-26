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

signals:
  void configFinished(bool success);

private slots:
  void configRemove();
  void configCreate();
};

#endif //CONFIGURATOR_H
