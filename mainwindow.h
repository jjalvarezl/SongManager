#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QFileSystemModel>
#include <QInputDialog>
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

    SongManager *getSongManager() const;
    void setSongManager(SongManager *value);

private slots:
    void on_actionCambiarEspacioDeTrabajo_triggered();

    void on_pushButtonMakeDir_clicked();

    void on_treeViewSongsDirectories_clicked(const QModelIndex &index);

    void on_pushButtonMakeSong_clicked();

    void on_pushButtonEditSongName_clicked();

    void on_pushButtonDeleteDir_clicked();

    void on_pushButton_clicked();

    void on_pushButtonSaveSong_clicked();

    void on_pushButtonDeleteSong_clicked();

    void on_pushButtonPrintSong_clicked();

    void on_pushButtonEditDirName_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    SongManager *sm;

    void SaveSettings();
    void LoadSettings();
    void LoadDirectoriesAndSongsInTreeView();
    void clearGUI();
    void enableGUIOptions(const bool &dirOptionsGUIEnabled);
    void enableDirOptionsGUI (const bool &enable);
    void enableSongOptionsGUI (const bool &enable);
    void disableDirAndSongsOfGUI();
};

#endif // MAINWINDOW_H
