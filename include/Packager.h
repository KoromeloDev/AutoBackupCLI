#ifndef PACKAGER_H
#define PACKAGER_H

#include <QObject>

class Packager : public QObject
{
Q_OBJECT

public:
  explicit Packager(QObject *parent = nullptr, quint8 level = 9, QStringList files = {});
  ~Packager();

  void pack(QString name);
  bool remove(QString path);

private:
  quint8 m_level;
  QStringList m_files;

signals:
  void packageFinished(bool success, QString path);
};

#endif //PACKAGER_H