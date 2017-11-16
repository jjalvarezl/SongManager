#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoadSettings();
}

MainWindow::~MainWindow()
{
    SaveSettings();
    delete ui;
}

void MainWindow::LoadSettings(){

    //Loading window settings
    QSettings setting("jjalvarezl","SongManager");
    setting.beginGroup("MainWindow");

    //If workspace is not contained just exit from system
    if (!setting.contains("workspace")){
        exit(0);
    }

    //Loading directories and songs
    this->setSongManager(new SongManager(setting.value("workspace").toString()));

    this->LoadDirectoriesAndSongsInTreeView();


    //Position MainWindow setting
    if (setting.contains("position")){
        QRect myRect = setting.value("position").toRect();
        setGeometry(myRect);
    } else {
        QRect myRect = frameGeometry();
        myRect.moveCenter(QDesktopWidget().availableGeometry().center());
        setGeometry(myRect);
    }


    //Open a file in the current operative system
    QDesktopServices::openUrl(QUrl::fromLocalFile("/home/jjalvarezl/Escritorio/audioclip-1489093033000-223155.mp4"));

    setting.endGroup();

    //Show saved when debugger
    qDebug() << "Loaded";
}

void MainWindow::SaveSettings(){

    //Saving window settings
    QSettings setting("jjalvarezl","SongManager");
    setting.beginGroup("MainWindow");
    setting.setValue("position", this->geometry());
    setting.endGroup();

    //Show saved when debugger
    qDebug() << "Saved";
}

void MainWindow::LoadDirectoriesAndSongsInTreeView(){
    model = new QFileSystemModel;
    model->setRootPath(this->getSongManager()->getWorkspace()+this->getSongManager()->getSongsDir());
    ui->treeViewSongsDirectories->setModel(model);
    const QModelIndex rootIndex = model->index(QDir::cleanPath(this->getSongManager()->getWorkspace()+this->getSongManager()->getSongsDir()));
    ui->treeViewSongsDirectories->setRootIndex(rootIndex);
    ui->treeViewSongsDirectories->show();
}

//EVENTS
void MainWindow::on_actionCambiarEspacioDeTrabajo_triggered()
{
    qDebug()<<"Cambiar espacio de trabajo activado";
    ChooseDirectoryDialog cdd;
    //cdd.setModal(true);

    //Exec It stops execution of main until opened dialog closes
    cdd.exec();
}


void MainWindow::on_pushButtonMakeDir_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid()) return;
    bool ok;
    QString dirName =  QInputDialog::getText(this, tr("Nombre del nuevo directorio"),
                                             tr("Ingrese el nombre del nuevo directorio a crear:"), QLineEdit::Normal,
                                             "", &ok);
    if (!ok || dirName.isEmpty()) return;
    model->mkdir(index,dirName);
}

void MainWindow::on_pushButtonMakeSong_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid()) return;
    bool ok;
    QString fileName =  QInputDialog::getText(this, tr("Nombre de la canción"),
                                             tr("Ingrese el nombre de la canción nueva:"), QLineEdit::Normal,
                                             "", &ok);
    if (!ok || fileName.isEmpty()) return;
    QString dirPath;
    if(QFileInfo(model->filePath(index)).isDir()){
        dirPath = model->filePath(index);
    } else {
        dirPath = QFileInfo(model->filePath(index)).path();
    }

    sm->addCurrentSongData("title", fileName);

    if(!sm->writeSong(dirPath+"/"+fileName)){
        QMessageBox::critical(this,"Error", "No se puede escribir el archivo, verifique si la ruta existe o faltan permisos de carpeta", QMessageBox::Close);
    }
}

void MainWindow::on_treeViewSongsDirectories_clicked(const QModelIndex &index)
{
    this->clearGUI();
    //Knowing if selected index is dir or file
    if(QFileInfo(model->filePath(index)).isDir()){
        //dirPath = model->filePath(index);
        this->enableDirGUIOptions();
    } else {
        //dirPath = QFileInfo(model->filePath(index)).path();
        //Open file as fileInfo
        QString filePath = model->filePath(index);
        QFileInfo fileInfo = QFileInfo(filePath);

        //Loading song data
        if (fileInfo.isFile()){
            if(this->getSongManager()->loadSong(filePath)){
                //Refect song data on ui

                //lyrics
                ui->plainTextEditChordsLyrics->document()->setPlainText(
                    this->getSongManager()->getCurrentSongData().value("lyrics")
                );
                //title
                ui->lineEditTitle->setText(
                    this->getSongManager()->getCurrentSongData().value("title")
                );
                //author
                ui->lineEditAuthor->setText(
                    this->getSongManager()->getCurrentSongData().value("author")
                );
                //key
                ui->lineEditSongKey->setText(
                    this->getSongManager()->getCurrentSongData().value("key")
                );
                //print order
                ui->lineEditOrder->setText(
                    this->getSongManager()->getCurrentSongData().value("order")
                );
                this->enableFileGUIOptions();
            } else {
                QMessageBox::critical(this,"Error", "El archivo está corrupto o dañado, no se puede leer el contenido del archivo", QMessageBox::Close);
            }
        }
    }

}

void MainWindow::on_pushButtonEditSongName_clicked()
{

}

void MainWindow::on_pushButtonDeleteDir_clicked()
{

}

void MainWindow::on_pushButtonSaveSong_clicked()
{

}

void MainWindow::on_pushButtonDeleteSong_clicked()
{

}

void MainWindow::on_pushButtonPrintSong_clicked()
{

}


//GETTERS AND SETTERS

SongManager *MainWindow::getSongManager() const
{
    return sm;
}

void MainWindow::setSongManager(SongManager *value)
{
    sm = value;
}


//AUXILIAR OPERATIONS


void MainWindow::clearGUI() {

}

void MainWindow::enableDirGUIOptions(){

}

void MainWindow::enableFileGUIOptions(){

}


