#include "printpreview.h"

#include <QTextDocument>
#include <QDebug>

PrintPreview::PrintPreview()
    : m_htmlTable(QString()),
      m_htmlData(QString())
{
    m_printer.setPageSize(QPageSize::A7);
    m_printer.setPageMargins(QMarginsF(1, 1, 1, 1), QPageLayout::Millimeter);
}

void PrintPreview::setHtmlRow(QStringList fields)
{
    QString row;

    row.append("<tr>");
    foreach (QString field, fields)
    {
        row.append(htmlItem(field));
    }
    row.append("</tr>");

    m_htmlTable.append(row);
}

QString PrintPreview::htmlItem(QString item)
{
    QString returnValue;

    returnValue.append("<td><p align=\"left\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">");
    returnValue.append(item);
    returnValue.append("</span></p></td>");

    return returnValue;
}

QString PrintPreview::htmlTable(QString rows)
{
    QString returnValue = "";
    returnValue.append("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\"><tbody>");
    returnValue.append(rows);
    returnValue.append("</tbody></table>");
    return returnValue ;
}

QString PrintPreview::htmlClientPage(QString table)
{
    QString r = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
            "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><meta name=\"qrichtext\" content=\"1\">"
            "<title>QTextEdit Example</title>"
            "<style type=\"text/css\">p, li { white-space: pre-wrap; }"
            "</style></head><body style=\" font-family:'Arial'; font-size:16pt; font-weight:100; font-"
            "style:normal;\">";
    r.append(table);
    r.append("<br><hr><br></body></html>");
    return r;
}

void PrintPreview::setHtmlData(QString data)
{
    m_htmlData = data;
}

QString PrintPreview::getHtmlData()
{
    return m_htmlData;
}

void PrintPreview::print()
{
    setHtmlData(htmlClientPage(htmlTable(m_htmlTable)));

    print(&m_printer);
}

void PrintPreview::print(QPrinter *printer)
{
    QTextDocument* doc = new QTextDocument();
    doc->setPageSize(QSizeF(m_printer.pageRect(QPrinter::Point).size()));
    doc->setHtml(m_htmlData);
    doc->print(printer);
}
