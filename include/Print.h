#ifndef PRINT_H
#define PRINT_H

#include <QTextStream>

class Print
{
public:
  static void info(const QString &text);
  static void system(const QString &text);
  static void success(const QString &text);
  static void warning(const QString &text);
  static void error(const QString &text);
};

#endif //PRINT_H
