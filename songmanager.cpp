#include "songmanager.h"

SongManager::SongManager(const QString &workspace)
{
    setWorkspace(workspace);
    setSongsDir("/Songs");
    LoadDirectoriesAndSongs();
}

QString SongManager::getWorkspace() const
{
    return workspace;
}

void SongManager::setWorkspace(const QString &workspace)
{
    this->workspace = workspace;
}

QString SongManager::getSongsDir() const
{
    return songsDir;
}

void SongManager::setSongsDir(const QString &songsDir)
{
    this->songsDir = songsDir;
}

QMultiMap<QString, QString> SongManager::getSongsWithDirs() const
{
    return songsWithDirs;
}

void SongManager::setSongsWithDirs(const QMultiMap<QString, QString> &songsWithDirs)
{
    this->songsWithDirs = songsWithDirs;
}

void SongManager::LoadDirectoriesAndSongs(){
    QStringList allDirs;
    allDirs << songsDir;

    bool noFilesOnDir = true;

    //Load all files on songsDir directory
    QStringList allFiles = QDir(workspace+songsDir).entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsFirst);
    for(QStringList::iterator it = allFiles.begin(); it != allFiles.end(); ++it){
        QString file = *it;
        songsWithDirs.insert(workspace+songsDir, file);
        noFilesOnDir=false;
    }
    //If there are no songs, add the directory without any song
    if (noFilesOnDir){
        songsWithDirs.insert(workspace+songsDir, "");
    }

    //Load all files and subdirectories on songsDir directory
    QDirIterator directories(workspace+songsDir, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(directories.hasNext()){
        directories.next();
        noFilesOnDir=true;
        allFiles = QDir(directories.filePath()).entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsFirst);
        for(QStringList::iterator it = allFiles.begin(); it != allFiles.end(); ++it){
            QString file = *it;
            songsWithDirs.insert(directories.filePath(), file);
            noFilesOnDir=false;
        }
        if (noFilesOnDir){
            songsWithDirs.insert(directories.filePath(), "");
        }
    }

    qDebug()<<songsWithDirs<<"KEEEEEEEEYYYYYYSSSS"<<this->ListSongsPerDirectory("/home/jjalvarezl/Proyectos/SongManager/Documentos OpenSong/Songs/Servicio/Corderos");
    //exit(0);
}

QStringList SongManager::ListSongsPerDirectory(const QString &directory){
    return songsWithDirs.values(directory);
}
