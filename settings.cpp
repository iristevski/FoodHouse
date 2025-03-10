#include "settings.h"
#include <QApplication>
#include <QSerialPort>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>
#include <QProcess>

QString Settings::DBLocation = "";
int Settings::License = 1;
QString Settings::DDVGroups = "18% македонски производи, 5% македонски производи, 0% македонски производи, "
                              "0%  македонски производи за фирми кои не се ДДВ обврзници, "
                              "18% странски производи, 5% странски производи, 0% странски производи, "
                              "0% странски производи за фирми кои не се ДДВ обврзници";

QString Settings::PortName = "COM1";
int Settings::BaudRate = 2400;
int Settings::DataBits = QSerialPort::Data8;
int Settings::Parity = QSerialPort::NoParity;
int Settings::StopBits = QSerialPort::OneStop;
int Settings::FlowControl = QSerialPort::NoFlowControl;

Settings::Settings(QObject *parent) :
    QSettings("HAS", "FoodHouse", parent)
{
    DBLocation = QApplication::applicationDirPath();
}

void Settings::init()
{
    QString keyDBLocation = "DBLocation";
    QString keyLicense = "License";
    QString keyPortName = "PortName";
    QString keyBaudRate = "BaudRate";
    QString keyDataBits = "DataBits";
    QString keyParity = "Parity";
    QString keyStopBits = "StopBits";
    QString keyFlowControl = "FlowControl";

    if (!contains(keyDBLocation))       setValue(keyDBLocation, DBLocation);
    if (!contains(keyLicense))          setValue(keyLicense, License);
    if (!contains(keyPortName))         setValue(keyPortName, PortName);
    if (!contains(keyBaudRate))         setValue(keyBaudRate, BaudRate);
    if (!contains(keyDataBits))         setValue(keyDataBits, DataBits);
    if (!contains(keyParity))           setValue(keyParity, Parity);
    if (!contains(keyStopBits))         setValue(keyStopBits, StopBits);
    if (!contains(keyFlowControl))      setValue(keyFlowControl, FlowControl);

    DBLocation =        value(keyDBLocation).toString();
    License =           value(keyLicense).toInt();
    PortName =          value(keyPortName).toString();
    BaudRate =          value(keyBaudRate).toInt();
    DataBits =          value(keyDataBits).toInt();
    Parity =            value(keyParity).toInt();
    StopBits =          value(keyStopBits).toInt();
    FlowControl =       value(keyFlowControl).toInt();
}

QString Settings::GroupsStylesheet()
{
    return QString("QListWidget { font: 12pt \"Segoe UI\"; color: rgb(64, 64, 64); outline: 0; }"
        "QListWidget::item { color: rgb(64, 64, 64); border: 0px solid white; height: 55px; }"
        "QListWidget::item:selected:!active { color: rgb(64, 64, 64); background: rgb(120, 180, 230); }"
        "QListWidget::item:selected:active { color: rgb(64, 64, 64); background: rgb(120, 180, 230); }"
        "QListWidget::item:hover { color: rgb(64, 64, 64); background: rgb(210, 220, 240); }");
}

QString Settings::ProductsStylesheet()
{
    return QString("QListWidget { font: 10pt \"Segoe UI\"; color: rgb(64, 64, 64); outline: 0; }"
        "QListWidget::item { color: rgb(64, 64, 64); border: 0px solid white; height: }"
        "QListWidget::item:selected:!active { color: rgb(64, 64, 64); background: rgb(120, 180, 230); }"
        "QListWidget::item:selected:active { color: rgb(64, 64, 64); background: rgb(120, 180, 230); }"
        "QListWidget::item:hover { color: rgb(64, 64, 64); background: rgb(210, 220, 240); }");
}

// returns 0 for not existing name
int Settings::NameId(const QSqlTableModel* model, const QString name)
{
    int returnValue = 0;

    for (int row = 0; row < model->rowCount(); row++)
    {
        QSqlRecord record = model->record(row);
        if (name == record.value("name").toString())
        {
            returnValue = record.value("id").toInt();
        }
    }

    return returnValue;
}

// returns 0 for not existing name
int Settings::CodeId(const QSqlTableModel* model, const QString code)
{
    int returnValue = 0;

    for (int row = 0; row < model->rowCount(); row++)
    {
        QSqlRecord record = model->record(row);
        if (code == record.value("code").toString())
        {
            returnValue = record.value("id").toInt();
        }
    }

    return returnValue;
}

QString Settings::Sequence()
{
    static bool bSeq = false;

    QString returnValue = bSeq ? QString(QChar(33)) : QString(QChar(34));
    bSeq = !bSeq;

    return returnValue;
}

void Settings::CreateFileAndPrint(QString data, QString fileName)
{
    QString filePath = QApplication::applicationDirPath() + "/" + fileName;
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << data;
    file.close();

    QString command("cmd.exe /C start " +
    QApplication::applicationDirPath() + "/DAVID32.EXE" + " " +
    QApplication::applicationDirPath() + "/" + fileName);
    QProcess::execute(command);
}

QString Settings::ComboDDVChar(int index)
{
    // Ima 8 danocni grupi:
    // chr(192) = А = 18% македонски производи
    // chr(193) = Б = 5%  македонски производи
    // chr(194) = В = 0%  македонски производи
    // chr(195) = Г = 0%  македонски производи за фирми кои не се ДДВ обврзници
    // chr(65) = A = 18%  странски производи
    // chr(66) = B = 5%   странски
    // chr(67) = C = 0%   странски
    // chr(68) = D = 0% за фирми кои не се ДДВ обврзници

    QString returnValue;

    switch (index)
    {
    case 0:
        returnValue = QString(QChar(192));
        break;
    case 1:
        returnValue = QString(QChar(193));
        break;
    case 2:
        returnValue = QString(QChar(194));
        break;
    case 3:
        returnValue = QString(QChar(195));
        break;
    case 4:
        returnValue = QString(QChar(65));
        break;
    case 5:
        returnValue = QString(QChar(66));
        break;
    case 6:
        returnValue = QString(QChar(67));
        break;
    case 7:
        returnValue = QString(QChar(68));
        break;
    default:
        returnValue = QString(QChar(192));
        break;
    }

    return returnValue;
}

QString Settings::ConvertTo1251(QString string)
{
    QString returnValue;

    for (QString::const_iterator iter(string.begin()); iter != string.end(); ++iter)
    {
        if(iter->toUpper() == 'A') returnValue.append(QChar(192));
        else if(iter->toUpper() == 'B') returnValue.append(QChar(193));
        else if(iter->toUpper() == 'V') returnValue.append(QChar(194));
        else if(iter->toUpper() == 'G') returnValue.append(QChar(195));
        else if(iter->toUpper() == 'D') returnValue.append(QChar(196));
        else if(iter->toUpper() == '\\') returnValue.append(QChar(129));
        else if(iter->toUpper() == 'E') returnValue.append(QChar(197));
        else if(iter->toUpper() == '@') returnValue.append(QChar(198));
        else if(iter->toUpper() == 'Z') returnValue.append(QChar(199));
        else if(iter->toUpper() == 'Y') returnValue.append(QChar(189));
        else if(iter->toUpper() == 'I') returnValue.append(QChar(200));
        else if(iter->toUpper() == 'J') returnValue.append(QChar(163));
        else if(iter->toUpper() == 'K') returnValue.append(QChar(202));
        else if(iter->toUpper() == 'L') returnValue.append(QChar(203));
        else if(iter->toUpper() == 'Q') returnValue.append(QChar(138));
        else if(iter->toUpper() == 'M') returnValue.append(QChar(204));
        else if(iter->toUpper() == 'N') returnValue.append(QChar(205));
        else if(iter->toUpper() == 'W') returnValue.append(QChar(140));
        else if(iter->toUpper() == 'O') returnValue.append(QChar(206));
        else if(iter->toUpper() == 'P') returnValue.append(QChar(207));
        else if(iter->toUpper() == 'R') returnValue.append(QChar(208));
        else if(iter->toUpper() == 'S') returnValue.append(QChar(209));
        else if(iter->toUpper() == 'T') returnValue.append(QChar(210));
        else if(iter->toUpper() == ']') returnValue.append(QChar(141));
        else if(iter->toUpper() == 'U') returnValue.append(QChar(211));
        else if(iter->toUpper() == 'F') returnValue.append(QChar(212));
        else if(iter->toUpper() == 'H') returnValue.append(QChar(213));
        else if(iter->toUpper() == 'C') returnValue.append(QChar(214));
        else if(iter->toUpper() == '^') returnValue.append(QChar(215));
        else if(iter->toUpper() == 'X') returnValue.append(QChar(143));
        else if(iter->toUpper() == '[') returnValue.append(QChar(216));
        else returnValue.append(iter->toUpper());
    }

    return returnValue;
}
