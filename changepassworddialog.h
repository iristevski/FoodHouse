#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = 0);
    ~ChangePasswordDialog();

private slots:
    void on_buttonOK_clicked();

private:
    Ui::ChangePasswordDialog *ui;
    QWidget* m_parent;

};

#endif // CHANGEPASSWORDDIALOG_H
