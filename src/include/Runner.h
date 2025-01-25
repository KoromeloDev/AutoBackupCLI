#ifndef RUNNER_H
#define RUNNER_H

#include "Search.h"
#include "Pack.h"
#include "Loader.h"

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
  QSharedPointer<Pack> m_pack;
  QSharedPointer<Loader> m_loader;

  void search();
  void pack(QStringList files = {});
  void load();

public slots:
  static void searchFinished(bool success, QStringList files);
  static void packageFinished(bool success, QString path);
  static void loadingFinished(bool success);
};

#endif //RUNNER_H
