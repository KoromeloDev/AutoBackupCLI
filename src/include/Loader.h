#ifndef LOADER_H
#define LOADER_H

#include <QObject>

class Loader : public QObject
{
Q_OBJECT

public:
  explicit Loader(QObject *parent = nullptr, QString configName = {}, QString file = {}, QString path = "/");
  ~Loader();

  void load();

private:
  QString m_configName;
  QString m_file;
  QString m_path;

signals:
  void loadingFinished(bool success);

};

#endif //LOADER_H
