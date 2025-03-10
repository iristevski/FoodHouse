#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"
#include "settings.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QSqlRecord>

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog),
    m_parent(parent)
{
    ui->setupUi(this);
    setModal(true);

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditUsername->setText(mw->loggedProfile());
    }
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::on_buttonOK_clicked()
{
    QString oldPassword = ui->lineEditOldPassword->text();
    QString newPassword1 = ui->lineEditNewPassword1->text();
    QString newPassword2 = ui->lineEditNewPassword2->text();

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->profileModel();
        for (int row = 0; row < model->rowCount(); ++row)
        {
            QSqlRecord record = model->record(row);

            if (oldPassword == record.value("password").toString())
            {
                if (newPassword1 == newPassword2)
                {
                    record.setValue("password", newPassword1);
                    model->setRecord(row, record);
                    model->select();
                    close();
                }
                else
                {
                    QMessageBox::warning(this, "", "Новата лозинка не се совпаѓа!\nОбидете се повторно.");

                    ui->lineEditNewPassword1->clear();
                    ui->lineEditNewPassword2->clear();
                    break;
                }
            }
            else if (row == model->rowCount() - 1)
            {
                QMessageBox::warning(this, "", "Погрешна лозинка!\nОбидете се повторно.");

                ui->lineEditOldPassword->clear();
                ui->lineEditNewPassword1->clear();
                ui->lineEditNewPassword2->clear();
                break;
            }
        }
    }
}
