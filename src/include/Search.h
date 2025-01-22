#ifndef SEARCH_H
#define SEARCH_H

#include <QDir>

class Search
{
public:
    explicit Search(const QString &includeFile= nullptr, const QString &excludeFile = nullptr);
    void search();

private:
    QFile m_includeFile;
    QFile m_excludeFile;

    QStringList filter(const QStringList inputData);
};

#endif //SEARCH_H
