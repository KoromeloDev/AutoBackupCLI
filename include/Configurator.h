#ifndef CONFIGURATOR_H
  #define CONFIGURATOR_H

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

signals:
  void configFinished(bool success);
};

#endif //CONFIGURATOR_H
