#include "Print.h"

void Print::info(const QString &text)
{
  qInfo() << text;
}

void Print::success(const QString &text)
{
  qInfo() << "\033[32m" << text << "\033[0m";
  exit(0);
}

void Print::warning(const QString &text)
{
  qWarning() << "\033[33m" << text << "\033[0m";
}

void Print::error(const QString &text)
{
  qCritical() << "\033[31m" << text << "\033[0m";
  exit(1);
}
