#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "printwindow.h"
#include "reportwindow.h"
#include "addwindow.h"
#include "profilewindow.h"
#include "logindialog.h"
#include "changepassworddialog.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serial(new QSerialPort(this))
{
    initSettings();
    setupDB();

    m_loginDialog = new LoginDialog(this);
    if (m_loginDialog->exec() == QDialog::Rejected)
    {
        m_database->close();
        exit(0);
    }

    ui->setupUi(this);

    ui->labelProfile->setText(m_loginDialog->name());
    ui->tabWidget->setCurrentIndex(0);

    // serial port configuration from registry
    m_serial->setPortName(Settings::PortName);
    m_serial->setBaudRate(Settings::BaudRate);
    m_serial->setDataBits(QSerialPort::DataBits(Settings::DataBits));
    m_serial->setParity(QSerialPort::Parity(Settings::Parity));
    m_serial->setStopBits(QSerialPort::StopBits(Settings::StopBits));
    m_serial->setFlowControl(QSerialPort::FlowControl(Settings::FlowControl));

    PrintWindow *printWindow = new PrintWindow(this);
    ui->tabPrint->layout()->addWidget(printWindow);

    ReportWindow *reportWindow = new ReportWindow(this);
    ui->tabReport->layout()->addWidget(reportWindow);

    AddWindow *addWindow = new AddWindow(this);
    ui->tabAdd->layout()->addWidget(addWindow);

    ProfileWindow *profileWindow = new ProfileWindow(this);
    ui->tabProfile->layout()->addWidget(profileWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    showMaximized();
}

void MainWindow::initSettings()
{
    Settings settings;
    settings.init();

    if (settings.License == 0)
    {
        exit(0);
    }
}

void MainWindow::setupDB()
{
    m_database = new QSqlDatabase();

    // Set database driver to QSQLITE
    *m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database->setDatabaseName(Settings::DBLocation + "/products.db");

    if(!m_database->open())
    {
        QMessageBox::warning(0, "Грешка", "Не може да се отвори базата на податоци");
        exit(0);
    }

    // Try to locate the Products database.
    // If it is not available create it.
    bool found = false;
    foreach (QString table, m_database->tables())
    {
        if(table == "Products")
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        QSqlQuery query(*m_database);
        query.exec("CREATE TABLE Products (id INTEGER PRIMARY KEY, name TEXT, price NUMERIC, icon TEXT)");
    }

    m_groupsModel = new QSqlTableModel(this, *m_database);
    m_groupsModel->setTable("Groups");
    m_groupsModel->select();

    m_productsModel = new QSqlTableModel(this, *m_database);
    m_productsModel->setTable("Products");
    m_productsModel->select();

    m_profileModel = new QSqlTableModel(this, *m_database);
    m_profileModel->setTable("Profile");
    m_profileModel->select();
}

int MainWindow::loggedProfileId()
{
    return m_loginDialog->id();
}

QString MainWindow::loggedProfile()
{
    return m_loginDialog->username();
}

int MainWindow::loggedProfileCode()
{
    return m_loginDialog->code();
}

QSerialPort* MainWindow::serialPort()
{
    return m_serial;
}

QSqlTableModel* MainWindow::groupsModel()
{
    return m_groupsModel;
}

QSqlTableModel* MainWindow::productsModel()
{
    return m_productsModel;
}

QSqlTableModel* MainWindow::profileModel()
{
    return m_profileModel;
}

void MainWindow::on_buttonChangePassword_clicked()
{
    ChangePasswordDialog* cpd = new ChangePasswordDialog(this);
    cpd->exec();
}

void MainWindow::on_buttonSignOut_clicked()
{
    exit(0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index != 0 && !m_loginDialog->isAdmin())
    {
        ui->tabWidget->setCurrentIndex(0);
    }
}
