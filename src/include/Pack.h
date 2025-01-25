#ifndef PACK_H
#define PACK_H

#include <QObject>

class Pack : public QObject
{
Q_OBJECT

public:
  explicit Pack(QObject *parent = nullptr, quint8 level = 9, QStringList files = {});
  ~Pack();

  void pack(QString name);
  bool remove(QString path);

private:
  quint8 m_level;
  QStringList m_files;

signals:
  void packageFinished(bool success, QString path);

};

#endif //PACK_H
