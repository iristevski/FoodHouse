#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QWidget>

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWindow(QWidget *parent = 0);
    ~ReportWindow();

protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_buttonStorn_clicked();
    void on_buttonInputMoney_clicked();
    void on_buttonOutputMoney_clicked();
    void on_buttonPrintPeriod_clicked();
    void on_buttonPrintDaily_clicked();
    void on_buttonPrintDailyNulling_clicked();

private:
    Ui::ReportWindow *ui;
    QWidget* m_parent;
};

#endif // REPORTWINDOW_H
