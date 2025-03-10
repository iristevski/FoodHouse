#ifndef PRINTWINDOW_H
#define PRINTWINDOW_H

#include <QWidget>

#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QSerialPort>

namespace Ui {
class PrintWindow;
}

class PrintWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWindow(QWidget *parent = 0);
    ~PrintWindow();

protected:
    void showEvent(QShowEvent* event);

private slots:
    void on_listWidgetProducts_itemClicked(QListWidgetItem *item);
    void on_tablePrint_itemClicked(QTableWidgetItem *item);
    void on_listWidgetGroups_currentTextChanged(const QString &currentText);

    void on_buttonPrint_clicked();

private:
    void updateGroups();
    void updateProducts(int currentGroup = 1);
    void updateSum();

    void printFiscal();
    void printTermal();

private:
    Ui::PrintWindow *ui;
    QWidget* m_parent;
    QSerialPort* m_serial;
    QString m_data;
};

#endif // PRINTWINDOW_H
