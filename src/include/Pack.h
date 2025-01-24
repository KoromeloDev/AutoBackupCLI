#ifndef PACK_H
#define PACK_H

#include <QObject>

class Pack : public QObject
{
Q_OBJECT

public:
  explicit Pack(QObject *parent = nullptr);
  ~Pack();

};

#endif //PACK_H
