#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QFileSystemModel>
#include "choosedirectorydialog.h"
#include "songmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCambiarEspacioDeTrabajo_triggered();

private:
    Ui::MainWindow *ui;

    void SaveSettings();
    void LoadSettings();
    void LoadDirectories();
    void LoadSongs();
};

#endif // MAINWINDOW_H
