#ifndef LoginDialog_H
#define LoginDialog_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    int id();
    QString username();
    QString name();
    int code();

    bool isAdmin();

public slots:
    int exec();

private slots:
    void on_buttonOK_clicked();

private:
    Ui::LoginDialog *ui;
    QWidget* m_parent;

    int m_id;
    QString m_name;
    QString m_fullName;
    int m_code;

    int m_returnValue;
};

#endif // LoginDialog_H
