#ifndef SONGMANAGER_H
#define SONGMANAGER_H
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QDirIterator>
#include <QXmlStreamReader>

class SongManager
{
public:
    SongManager(const QString &workspace);

    QString getWorkspace() const;
    void setWorkspace(const QString &workspace);

    QString getSongsDir() const;
    void setSongsDir(const QString &songsDir);

    /*QMultiMap<QString, QString> getSongsWithDirs() const;
    void setSongsWithDirs(const QMultiMap<QString, QString> &songsWithDirs);*/

    QMultiMap<QString, QString> getCurrentSongData() const;
    void setCurrentSongData(const QMultiMap<QString, QString> &value);

    bool loadSong (const QString &filePath);
    bool writeSong (const QString &filePathWithName);
    void addCurrentSongData (const QString &key, const QString &value);
    void clearCurrentSongData ();

private:
    QString workspace;
    QString songsDir;
    //QMultiMap<QString, QString> songsWithDirs;
    QMultiMap<QString, QString> currentSongData;

    /*void LoadDirectoriesAndSongs();
    QStringList ListSongsPerDirectory(const QString &directory);*/
};

#endif // SONGMANAGER_H
