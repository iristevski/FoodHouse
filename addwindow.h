#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QWidget>

namespace Ui {
class AddWindow;
}

class AddWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = 0);
    ~AddWindow();

private slots:
    void on_buttonAddProduct_clicked();
    void on_buttonAddGroup_clicked();
    void on_buttonRemoveProduct_clicked();
    void on_buttonRemoveGroup_clicked();
    void on_toolButton_clicked();

private:
    void updateGroups();

private:
    Ui::AddWindow *ui;
    QWidget* m_parent;

};

#endif // ADDWINDOW_H
