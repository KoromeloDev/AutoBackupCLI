#include <QCoreApplication>

#include "Print.h"
#include "Runner.h"
#include "Dir.h"

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  QCoreApplication::setApplicationVersion(APP_VERSION "-beta");
  Dir::setPath();
  Runner runner;
  runner.start();
  return QCoreApplication::exec();
}
