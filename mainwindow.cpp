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
