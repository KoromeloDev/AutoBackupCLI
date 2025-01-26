#ifndef RUNNER_H
#define RUNNER_H

#include "Search.h"
#include "Packager.h"
#include "Loader.h"
#include "Configurator.h"

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
  QSharedPointer<Packager> m_packager;
  QSharedPointer<Loader> m_loader;
  QSharedPointer<Configurator> m_configurator;

  void search();
  void pack(QStringList files = {});
  void load();
  void config();

private slots:
  static void searchFinished(bool success, QStringList files);
  static void packageFinished(bool success, QString path);
  static void loadingFinished(bool success);
  static void configFinished(bool success);
};

#endif //RUNNER_H
