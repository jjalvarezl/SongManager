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
    SongManager * sm = new SongManager(setting.value("workspace").toString());

    //Loading directories on QTreeView
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(sm->getWorkspace()+sm->getSongsDir());
    ui->treeViewSongsDirectories->setModel(model);
    const QModelIndex rootIndex = model->index(QDir::cleanPath(sm->getWorkspace()+sm->getSongsDir()));
    if (rootIndex.isValid())
        ui->treeViewSongsDirectories->setRootIndex(rootIndex);
        ui->treeViewSongsDirectories->setRowHidden(1, rootIndex, true);
    ui->treeViewSongsDirectories->show();

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

void MainWindow::on_actionCambiarEspacioDeTrabajo_triggered()
{
    qDebug()<<"Cambiar espacio de trabajo activado";
    ChooseDirectoryDialog cdd;
    //cdd.setModal(true);

    //Exec It stops execution of main until opened dialog closes
    cdd.exec();
}

void MainWindow::LoadDirectories(){

}

void MainWindow::LoadSongs(){

}
