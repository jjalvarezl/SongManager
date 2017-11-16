#include "songmanager.h"

SongManager::SongManager(const QString &workspace)
{
    setWorkspace(workspace);
    setSongsDir("/Songs");
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



QMultiMap<QString, QString> SongManager::getCurrentSongData() const
{
    return currentSongData;
}

void SongManager::setCurrentSongData(const QMultiMap<QString, QString> &value)
{
    currentSongData = value;
}

//The following comented code works, but has been deprecated due to there is another and better way to load songs on GUI:
/*

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
}*/

/**
 * @brief SongManager::loadSong
 * @param filePath, absolute path to the file
 * @return true if can be loaded, else false.
 *
 * Reads the xml file and return the information on a
 * QMultiMap called songData that mainwindow can access.
 */
bool SongManager::loadSong (const QString &filePath){
    //Need to clear current song data to avoid load another songs information
    this->clearCurrentSongData();

    QFile file(filePath);
    if(!file.open(QFile::ReadWrite | QFile::Text)){
        return false;
    }

    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement()) {
        if (reader.name() == "song"){
            while(reader.readNextStartElement()){
                currentSongData.insert(reader.name().toString(), reader.readElementText());
            }
        }
        else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool SongManager::writeSong(const QString &filePathWithName){
    QFile file(filePathWithName);
    if(!file.open(QFile::ReadWrite | QFile::Text)){
        return false;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    //Document header
    writer.writeStartDocument();
        //write starting song tag
        writer.writeStartElement("song");
           //Write key - value pairs as xml element
            QMapIterator<QString, QString> i(currentSongData);
            while (i.hasNext()) {
                i.next();
                writer.writeTextElement(i.key(), i.value());
            }
        //write ending song tag
        writer.writeEndElement();

    writer.writeEndDocument();

    return true;
}

void SongManager::addCurrentSongData (const QString &key, const QString &value){
    currentSongData.insert(key, value);
}

void SongManager::clearCurrentSongData(){
    currentSongData.clear();
}
