#include "profilewindow.h"
#include "ui_profilewindow.h"
#include "settings.h"
#include "mainwindow.h"

#include <QSqlRecord>
#include <QMessageBox>

ProfileWindow::ProfileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileWindow),
    m_parent(parent)
{
    ui->setupUi(this);

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        ui->tableProfile->setModel(mw->profileModel());
        ui->tableProfile->hideColumn(4);
    }
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::on_buttonAddProfile_clicked()
{
    QString name(ui->lineEditAddProfileName->text());
    QString username(ui->lineEditAddProfileUsername->text());
    QString password(ui->lineEditAddProfilePassword->text());
    QString code(ui->lineEditAddProfileCode->text());
    bool isAdmin = ui->checkBoxAdmin->isChecked();
    if (!name.isEmpty() && !username.isEmpty() && !password.isEmpty() && !code.isEmpty())
    {
        if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
        {
            QSqlTableModel* model = mw->profileModel();
            if (!Settings::NameId(model, username) && !Settings::CodeId(model, code))
            {
                // create a record from the current model
                QSqlRecord rec = model->record();
                rec.setValue("fullName", name);
                rec.setValue("name", username);
                rec.setValue("password", password);
                rec.setValue("code", code);
                rec.setValue("admin", isAdmin ? 1 : 0);
                // insert a new record (-1)
                model->insertRecord(-1, rec);
                model->select();
            }
            else
            {
                QMessageBox::warning(this, "", Settings::NameId(model, username) ?
                    "Веќе постои корисник со внесеното корисничко име!\nОбидете се повторно." :
                    "Веќе постои корисник со внесената шифра!\nОбидете се повторно.");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "", "Сите полиња се задолжителни.");
    }

    ui->lineEditAddProfileName->clear();
    ui->lineEditAddProfileUsername->clear();
    ui->lineEditAddProfilePassword->clear();
    ui->lineEditAddProfileCode->setText("");
}

void ProfileWindow::on_buttonRemoveProfile_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сакате да го избришете селектираниот запис?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
        {
            QSqlTableModel* model = mw->profileModel();
            QModelIndexList indexes = ui->tableProfile->selectionModel()->selection().indexes();
            foreach (QModelIndex index, indexes)
            {
                model->removeRow(index.row());
            }
            model->select();
        }
    }
}
