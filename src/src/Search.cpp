#include "Search.h"

#include <QDirIterator>
#include <QRegularExpressionMatchIterator>

Search::Search(const QString &includeFile, const QString &excludeFile)
{
  if (includeFile == nullptr)
  {
    qDebug() << "include file is null";
    return;
  }

  if (excludeFile != nullptr)
  {
    m_excludeFile.setFileName(excludeFile);
  }

  m_includeFile.setFileName(includeFile);

}

void Search::search()
{
  if (!m_includeFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Couldn't open the file:" << m_includeFile.fileName();
    return;
  }

  QStringList pathList;
  QStringList files;

  while (!m_includeFile.atEnd())
  {
    QString path = m_includeFile.readLine();
    path = path.trimmed();

    if (!path.isEmpty())
    {
      pathList += path;
    }
  }

  m_includeFile.close();

  for (const auto &path : pathList)
  {
    qDebug() << "\033[34m" << "Include path:" << path << "\033[0m";
    QDirIterator it(path, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
      if (const QString name = it.next(); !name.isEmpty())
      {
        files << name;
      }
    }
  }

  files = filter(files);

  if (files.isEmpty())
  {
    qDebug() << "\033[31m" << "Not find!" << "\033[0m";
    exit(1);
  }

  for (const auto &file : files)
  {
    qDebug() << "Find:" << file;
  }

  exit(0);
}

QStringList Search::filter(const QStringList inputData)
{
  QString ignorePattern;

  if (m_excludeFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    while (!m_excludeFile.atEnd())
    {
      auto ignore = m_excludeFile.readLine();
      ignore = ignore.trimmed();

      if (!ignore.isEmpty())
      {
        ignorePattern += "(" + ignore + ")|";
        qDebug() << "\033[33m" <<"Exclude path:" << QString::fromUtf8(ignore) << "\033[0m";
      }
    }

    m_excludeFile.close();

    if (ignorePattern.isEmpty())
    {
      return inputData;
    }

    ignorePattern.chop(1);
  }
  else
  {
    return inputData;
  }

  QStringList filteredList;

  for (auto &line : inputData)
  {
    static QRegularExpression re(ignorePattern);
    QRegularExpressionMatchIterator i = re.globalMatch(line);

    if (!i.hasNext())
    {
      filteredList.append(line);
    }
  }

  return filteredList;
}
