#include "logindialog.h"
#include "ui_logindialog.h"
#include "settings.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QSqlRecord>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_parent(parent),
    m_name(QString()),
    m_fullName(QString()),
    m_returnValue(QDialog::Rejected)
{
    ui->setupUi(this);
    setModal(true);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

int LoginDialog::id()
{
    return m_id;
}

QString LoginDialog::username()
{
    return m_name;
}

QString LoginDialog::name()
{
    return m_fullName;
}

int LoginDialog::code()
{
    return m_code;
}

bool LoginDialog::isAdmin()
{
    bool returnValue = false;

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->profileModel();
        for (int row = 0; row < model->rowCount(); ++row)
        {
            QSqlRecord record = model->record(row);
            QString username = ui->lineEditUsername->text();
            if (record.value("name").toString() == username)
            {
                returnValue = record.value("admin").toInt() == 1;
                break;
            }
        }
    }

    return returnValue;
}

int LoginDialog::exec()
{
    QDialog::exec();
    return m_returnValue;
}

void LoginDialog::on_buttonOK_clicked()
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->profileModel();
        for (int row = 0; row < model->rowCount(); ++row)
        {
            QSqlRecord record = model->record(row);
            QString username = ui->lineEditUsername->text();
            if (record.value("name").toString() == username)
            {
                QString password = ui->lineEditPassword->text();
                if (record.value("password").toString() == password)
                {
                    m_id = record.value("id").toInt();
                    m_fullName = record.value("fullName").toString();
                    m_name = username;
                    m_code = record.value("code").toInt();
                    m_returnValue = QDialog::Accepted;
                }
            }
        }
    }

    if (m_returnValue == QDialog::Rejected)
    {
        QMessageBox::warning(this, "", "Погрешно корисничко име или лозинка!\nОбидете се повторно.");

        ui->lineEditUsername->clear();
        ui->lineEditPassword->clear();
    }
    else
    {
        close();
    }
}
