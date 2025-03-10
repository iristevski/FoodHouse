#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QSqlTableModel>

class Settings : public QSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    void init();

    static QString GroupsStylesheet();
    static QString ProductsStylesheet();
    static int NameId(const QSqlTableModel* model, const QString name);
    static int CodeId(const QSqlTableModel* model, const QString code);
    static QString Sequence();
    static void CreateFileAndPrint(QString data, QString fileName);
    static QString ComboDDVChar(int index);
    static QString ConvertTo1251(QString string);

public:
    static QString DBLocation;
    static int License;
    static QString DDVGroups;

    static QString PortName;
    static int BaudRate;
    static int DataBits;
    static int Parity;
    static int StopBits;
    static int FlowControl;

};

#endif // SETTINGS_H
