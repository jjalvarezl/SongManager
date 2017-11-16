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

    //Disabling everithing of gui except TreeView interaction
    this->disableDirAndSongsOfGUI();

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

    QString dirPath;
    if(QFileInfo(model->filePath(index)).isDir()){
        dirPath = model->filePath(index);
    } else {
        dirPath = QFileInfo(model->filePath(index)).path();
    }

    if(!QDir(dirPath).mkdir(dirPath+"/"+dirName)){
        QMessageBox::critical(this,"Error", "No se puede hacer la carpeta, verifique si la ruta existe o faltan permisos de carpeta", QMessageBox::Close);
    }
    this->clearGUI();
    this->disableDirAndSongsOfGUI();
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
        QMessageBox::critical(this,"Error", "No se puede hacer el archivo, verifique si la ruta existe o faltan permisos de carpeta", QMessageBox::Close);
    }
}

void MainWindow::on_treeViewSongsDirectories_clicked(const QModelIndex &index)
{
    this->clearGUI();
    //Knowing if selected index is dir or file
    if(QFileInfo(model->filePath(index)).isDir()){
        //dirPath = model->filePath(index);
        this->enableGUIOptions(true);
    } else {
        //dirPath = QFileInfo(model->filePath(index)).path();
        //Open file as fileInfo
        QString filePath = model->filePath(index);
        QFileInfo fileInfo = QFileInfo(filePath);

        //Loading song data
        if (fileInfo.isFile()){
            this->enableGUIOptions(false);
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
            } else {
                QMessageBox::critical(this,"Error", "El archivo está corrupto o dañado, no se puede leer el contenido del archivo", QMessageBox::Close);
            }
        }
    }

}

void MainWindow::on_pushButtonEditSongName_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid() && QFileInfo(model->filePath(index)).isDir()) return;

    bool ok;
    QString newFileName =  QInputDialog::getText(this, "Renombrando la canción \""+model->fileName(index)+"\"",
                                             tr("Ingrese el nuevo nombre del archivo:"), QLineEdit::Normal,
                                             "", &ok);
    if (!ok || newFileName.isEmpty()) return;

    //qDebug()<<model->filePath(index)+"/"+model->fileName(index);

    QFileInfo fileInfo(model->filePath(index));
    QFile file (model->filePath(index));
    if(!file.rename(fileInfo.path()+"/"+newFileName)){
        QMessageBox::critical(this, "Error", "El archivo de la canción no ha podido ser renombrada", QMessageBox::Close);
        return;
    }
    this->clearGUI();
    this->disableDirAndSongsOfGUI();
}

void MainWindow::on_pushButtonEditDirName_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid() && QFileInfo(model->filePath(index)).isDir()) return;

    bool ok;
    QString newDirName =  QInputDialog::getText(this, "Renombrando la carpeta \""+model->fileName(index)+"\"",
                                             tr("Ingrese el nuevo nombre de la carpeta:"), QLineEdit::Normal,
                                             "", &ok);
    if (!ok || newDirName.isEmpty()) return;

    QDir dir (model->filePath(index));
    if(!dir.cdUp() || !dir.rename(model->filePath(index), dir.absolutePath()+"/"+newDirName)){
        QMessageBox::critical(this, "Error", "La carpeta de la canción no pudo ser renombrada", QMessageBox::Close);
        return;
    }
    this->clearGUI();
    this->disableDirAndSongsOfGUI();
}


void MainWindow::on_pushButtonDeleteDir_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid() && !QFileInfo(model->filePath(index)).isDir()) return;

    if(QMessageBox::question(this,"Eliminar Carpeta","¿Esta seguro de eliminar la carpeta \""+model->fileName(index)+"\"?", QMessageBox::Accepted, QMessageBox::Cancel)){
        if (model->rmdir(index)) {
            this->clearGUI();
            this->disableDirAndSongsOfGUI();
        } else {
            QMessageBox::critical(this,"Error", "No es posible eliminar la carpeta\""+model->fileName(index)+"\"", QMessageBox::Close);
        }
    }
    this->clearGUI();
    this->disableDirAndSongsOfGUI();
}

void MainWindow::on_pushButtonSaveSong_clicked()
{

}

void MainWindow::on_pushButtonDeleteSong_clicked()
{
    QModelIndex index = ui->treeViewSongsDirectories->currentIndex();
    if (!index.isValid() && QFileInfo(model->filePath(index)).isDir()) return;
    if(QMessageBox::question(this,"Eliminar Carpeta","¿Esta seguro de eliminar la canción \""+model->fileName(index)+"\"?", QMessageBox::Accepted, QMessageBox::Cancel)){
        QFile file (model->filePath(index));
        if(file.remove()){
            this->clearGUI();
            this->disableDirAndSongsOfGUI();
        } else {
            QMessageBox::critical(this,"Error", "No es posible eliminar la canción \""+model->fileName(index)+"\"", QMessageBox::Close);
            return;
        }
    }
    this->clearGUI();
    this->disableDirAndSongsOfGUI();
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
    ui->lineEditAuthor->setText("");
    ui->lineEditOrder->setText("");
    ui->lineEditSongKey->setText("");
    ui->lineEditTitle->setText("");
    ui->plainTextEditChordsLyrics->document()->setPlainText("");
}



void MainWindow::enableSongOptionsGUI (const bool &enable){
    ui->pushButtonAddChords->setEnabled(enable);
    ui->pushButtonAcceptTranspose->setEnabled(enable);
    ui->pushButtonAddColumn->setEnabled(enable);
    ui->pushButtonAddMultipleVerses->setEnabled(enable);
    ui->pushButtonAddPage->setEnabled(enable);
    ui->pushButtonAddSection->setEnabled(enable);
    ui->pushButtonComment->setEnabled(enable);
    ui->pushButtonDeleteSong->setEnabled(enable);
    ui->pushButtonEditSongName->setEnabled(enable);
    ui->pushButtonMakeSong->setEnabled(true);
    ui->pushButtonPrintSong->setEnabled(enable);
    ui->pushButtonSaveSong->setEnabled(enable);
    ui->lineEditAuthor->setEnabled(enable);
    ui->lineEditOrder->setEnabled(enable);
    ui->lineEditSongKey->setEnabled(enable);
    ui->lineEditTitle->setEnabled(enable);
    ui->comboBoxSemitonesNumber->setEnabled(enable);
    ui->comboBoxTransposeType->setEnabled(enable);
    ui->radioButtonChordsAndLyrics->setEnabled(enable);
    ui->radioButtonOnlyChords->setEnabled(enable);
}

void MainWindow::enableDirOptionsGUI (const bool &enable){
    ui->pushButtonDeleteDir->setEnabled(enable);
    ui->pushButtonEditDirName->setEnabled(enable);
    ui->pushButtonMakeDir->setEnabled(true);
}

/**
 * @brief MainWindow::enableGUIOptions
 * @param dirOptionsGUIEnabled, true if you want to enable Dir GUI options, false if you want to enable songs GUI options.
 */
void MainWindow::enableGUIOptions(const bool &dirOptionsGUIEnabled){
    // Enabling/Disabling song options
    this->enableDirOptionsGUI(dirOptionsGUIEnabled);

    // Disabling/Enabling directory options
    this->enableSongOptionsGUI(!dirOptionsGUIEnabled);
}

void MainWindow::disableDirAndSongsOfGUI() {
    // Disabling song options
    this->enableDirOptionsGUI(false);

    // Disabling directory options
    this->enableSongOptionsGUI(false);
    ui->pushButtonMakeDir->setEnabled(false);
    ui->pushButtonMakeSong->setEnabled(false);
    ui->treeViewSongsDirectories->selectionModel()->clearSelection();
}
