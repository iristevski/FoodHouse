#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QWidget>

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWindow(QWidget *parent = 0);
    ~ProfileWindow();

private slots:
    void on_buttonAddProfile_clicked();
    void on_buttonRemoveProfile_clicked();

private:
    Ui::ProfileWindow *ui;
    QWidget* m_parent;

};

#endif // PROFILEWINDOW_H
