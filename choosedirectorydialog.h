#ifndef CHOOSEDIRECTORYDIALOG_H
#define CHOOSEDIRECTORYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class ChooseDirectoryDialog;
}

class ChooseDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseDirectoryDialog(QWidget *parent = 0);
    ~ChooseDirectoryDialog();

private slots:
    void on_pushButtonSearchDir_clicked();
    void on_buttonBoxAcceptReject_clicked(QAbstractButton *button);

private:
    Ui::ChooseDirectoryDialog *ui;
    void OpenFileChooserDialog();
};

#endif // CHOOSEDIRECTORYDIALOG_H
