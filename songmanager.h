#ifndef SONGMANAGER_H
#define SONGMANAGER_H
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QDirIterator>

class SongManager
{
public:
    SongManager(const QString &workspace);

    QString getWorkspace() const;
    void setWorkspace(const QString &workspace);

    QString getSongsDir() const;
    void setSongsDir(const QString &songsDir);

    QMultiMap<QString, QString> getSongsWithDirs() const;
    void setSongsWithDirs(const QMultiMap<QString, QString> &songsWithDirs);

private:
    QString workspace;
    QString songsDir;
    QMultiMap<QString, QString> songsWithDirs;
    void LoadDirectoriesAndSongs();
    QStringList ListSongsPerDirectory(const QString &directory);
};

#endif // SONGMANAGER_H
