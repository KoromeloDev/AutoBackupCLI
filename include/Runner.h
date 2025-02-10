#ifndef RUNNER_H
#define RUNNER_H

#include "Search.h"
#include "Packager.h"
#include "Loader.h"
#include "Configurator.h"
#include "ConfFile.h"

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
  QSharedPointer<ConfFile> m_config;
  bool m_isWaiting = false;
  QString m_removePath;

  void search(QString includeFile = nullptr, QString excludeFile = nullptr);
  void pack(QStringList files = {}, quint8 level = 0);
  void load(QString configName = nullptr, QString file = nullptr, QString path = nullptr);
  void config();
  void run();

private slots:
  void searchFinished(bool success, QStringList files);
  void packageFinished(bool success, QString path);
  void loadingFinished(bool success);
  void configFinished(bool success);
  void runFinished(bool success);
  void afterSearch(QStringList files);
  void afterPacking(QString path);
  void afterLoading();
};

#endif //RUNNER_H
