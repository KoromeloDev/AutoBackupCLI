#include "Print.h"

void Print::info(const QString &text)
{
  QTextStream cout(stdout);
  cout << "\033[37m" << text << "\033[0m" << Qt::endl;
}

void Print::system(const QString &text)
{
  QTextStream cout(stdout);
  cout << "\033[94m" << text << "\033[0m" << Qt::endl;
}

void Print::success(const QString &text)
{
  QTextStream cout(stdout);
  cout << "\033[32m" << text << "\033[0m" << Qt::endl;
  exit(0);
}

void Print::warning(const QString &text)
{
  QTextStream cout(stdout);
  cout << "\033[33m" << text << "\033[0m" << Qt::endl;
}

void Print::error(const QString &text)
{
  QTextStream cout(stdout);
  cout << "\033[31m" << text << "\033[0m" << Qt::endl;
  exit(1);
}
