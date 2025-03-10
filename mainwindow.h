#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int loggedProfileId();
    QString loggedProfile();
    int loggedProfileCode();

    QSerialPort* serialPort();

    QSqlTableModel* groupsModel();
    QSqlTableModel* productsModel();
    QSqlTableModel* profileModel();

protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_buttonChangePassword_clicked();
    void on_buttonSignOut_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    void initSettings();
    void setupDB();

private:
    Ui::MainWindow *ui;

    QSerialPort* m_serial;

    QString m_dBLocation;
    QSqlDatabase* m_database;
    QSqlTableModel* m_groupsModel;
    QSqlTableModel* m_productsModel;
    QSqlTableModel* m_profileModel;
    LoginDialog* m_loginDialog;
};

#endif // MAINWINDOW_H
