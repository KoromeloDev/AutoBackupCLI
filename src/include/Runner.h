#ifndef RUNNER_H
#define RUNNER_H

#include "Search.h"

#include <QObject>
#include <QSharedPointer>

class Runner : public QObject
{
Q_OBJECT

public:
  explicit Runner(QObject *parent = nullptr);
  ~Runner();
  void start();

private:
  QSharedPointer<Search> m_search;

  void search();
  void pack();

public slots:
  static void searchFinished(bool success, QStringList files);
};

#endif //RUNNER_H
