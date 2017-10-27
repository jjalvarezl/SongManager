#include "choosedirectorydialog.h"
#include "ui_choosedirectorydialog.h"

ChooseDirectoryDialog::ChooseDirectoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDirectoryDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Elija un directorio de trabajo");

    //Change ok and cancel text.
    ui->buttonBoxAcceptReject->button(QDialogButtonBox::Apply)->setText("Aceptar");
    ui->buttonBoxAcceptReject->button(QDialogButtonBox::Cancel)->setText("Cancelar");
}

ChooseDirectoryDialog::~ChooseDirectoryDialog()
{
    delete ui;
}

void ChooseDirectoryDialog::on_pushButtonSearchDir_clicked()
{
    ui->lineEditDirPath->setText(
        QFileDialog::getExistingDirectory(this, tr("Elija el directorio de trabajo"), ui->lineEditDirPath->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)
    );
}

void ChooseDirectoryDialog::on_buttonBoxAcceptReject_clicked(QAbstractButton *button)
{
    qDebug() << button->text();

    if(ui->buttonBoxAcceptReject->standardButton(button) == QDialogButtonBox::Apply){
        // Apply button event handler:
        if (!QDir(ui->lineEditDirPath->text()).exists() || (ui->lineEditDirPath->text() == "")) {
            QMessageBox errorMessageBox;
            errorMessageBox.critical(0, "Error", "La cadena ingresada no es una ruta a un directorio, por favor elija de nuevo");
            errorMessageBox.setFixedSize(500,200);
        } else {
            //Saving window settings
            QSettings setting("jjalvarezl","SongManager");
            setting.beginGroup("MainWindow");
            setting.setValue("workspace", ui->lineEditDirPath->text());
            setting.endGroup();
            this->close();
        }
    }
}
