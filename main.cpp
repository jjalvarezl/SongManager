#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QDebug>
#include "choosedirectorydialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Loading screen initiation
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/Images/SongManager.png"));
    splash->show();

    //Verifies that workspace exists
    //Loading window settings
    QSettings settings("jjalvarezl","SongManager");
    settings.beginGroup("MainWindow");

    //Verifing workspace existence
    if (!settings.contains("workspace") || !QDir(settings.value("workspace").toString()).exists()){

        //if it does not exists, then open choose directory dialog
        ChooseDirectoryDialog cdd;
        cdd.setModal(true);

        //Exec It stops execution of main until opened dialog closes
        cdd.exec();
    }

    //Loading main screen
    MainWindow w;

    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));
    //splash->close();
    //w.show();

    return a.exec();
}
