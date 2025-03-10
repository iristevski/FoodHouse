#ifndef PRINTPREVIEW_H
#define PRINTPREVIEW_H

#include <QPrinter>

class PrintPreview
{
public:
    PrintPreview();

    void setHtmlRow(QStringList fields);
    void print();

public slots:
    void print(QPrinter *printer);

private:
    QString htmlItem(QString item);
    QString htmlTable(QString rows);
    QString htmlClientPage(QString table);

    void setHtmlData(QString data);
    QString getHtmlData();

private:
    QPrinter m_printer;
    QString m_htmlTable;
    QString m_htmlData;

};

#endif // PRINTPREVIEW_H
