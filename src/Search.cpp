#include "Search.h"
#include "Print.h"

#include <QDirIterator>
#include <QRegularExpressionMatchIterator>

Search::Search(QObject *parent, const QString &includeFile, const QString &excludeFile) : QObject(parent)
{
  if (excludeFile != nullptr)
  {
    m_excludeFile.setFileName(excludeFile);
  }

  m_includeFile.setFileName(includeFile);
}

Search::~Search()
{

}

void Search::search()
{
  if (!m_includeFile.exists() || !m_includeFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    Print::error("Include file does not exist or not readable: " + m_includeFile.fileName());
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
    Print::info("Include path: " + path);
    QDirIterator it(path, QDir::AllEntries | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
      if (const QString name = it.next(); !name.isEmpty())
      {
        files << name;
      }
    }
  }

  files = filter(files);

  emit searchFinished(!files.isEmpty(), files);
}

QStringList Search::filter(const QStringList &inputData)
{
  QString ignorePattern;

  if (m_excludeFile.exists() && m_excludeFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    while (!m_excludeFile.atEnd())
    {
      auto ignore = m_excludeFile.readLine();
      ignore = ignore.trimmed();

      if (!ignore.isEmpty())
      {
        ignorePattern += ignore + "|";
        Print::info("Exclude path: " + QString::fromUtf8(ignore));
      }
    }

    m_excludeFile.close();

    if (ignorePattern.isEmpty())
    {
      Print::warning("Ignore pattern is empty!");
      return inputData;
    }

    ignorePattern.chop(1);
  }
  else
  {
    Print::warning("Ignore pattern is not recognize!");
    return inputData;
  }

  QStringList filteredList;

  for (auto &line : inputData)
  {
    static QRegularExpression re(ignorePattern);

    if (QRegularExpressionMatchIterator i = re.globalMatch(line); !i.hasNext())
    {
      filteredList.append(line);
    }
  }

  return filteredList;
}
