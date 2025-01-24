#ifndef SEARCH_H
#define SEARCH_H

#include <QDir>

class Search : public QObject
{
Q_OBJECT

public:
    explicit Search(QObject *parent = nullptr, const QString &includeFile= nullptr, const QString &excludeFile = nullptr);
    ~Search() ;
    void search();

private:
    QFile m_includeFile;
    QFile m_excludeFile;

    QStringList filter(const QStringList &inputData);

signals:
    void searchFinished(bool success, QStringList files);
};

#endif //SEARCH_H
