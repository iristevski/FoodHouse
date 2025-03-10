#include "reportwindow.h"
#include "ui_reportwindow.h"
#include "mainwindow.h"
#include "settings.h"

#include <QMessageBox>
#include <QSqlRecord>

ReportWindow::ReportWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportWindow),
    m_parent(parent)
{
    ui->setupUi(this);
}

ReportWindow::~ReportWindow()
{
    delete ui;
}

void ReportWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    ui->comboDDV->addItems(Settings::DDVGroups.split(", "));

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->productsModel();
        ui->comboStorn->clear();

        for (int row = 0; row < model->rowCount(); row++)
        {
            QSqlRecord record = model->record(row);
            QString name = record.value("name").toString();
            int price = record.value("price").toInt();
            ui->comboStorn->addItem(name, price);
        }
    }

    ui->lineEditQuantity->setValidator(new QIntValidator(1, 999, this));

    QDateTime now = QDateTime::currentDateTime();

    ui->dateEditBegin->setDate(now.date());
    ui->dateEditBegin->setTime(QTime(0, 0));
    ui->dateEditBegin->setMaximumDate(now.date());

    ui->dateEditEnd->setDate(now.date());
    ui->dateEditEnd->setTime(QTime(23, 59));
    ui->dateEditEnd->setMaximumDate(now.date());
}

void ReportWindow::on_buttonStorn_clicked()
{
    int currentIndex = ui->comboStorn->currentIndex();
    QString ddv = Settings::ComboDDVChar(ui->comboDDV->currentIndex());
    QString text = ui->comboStorn->itemText(currentIndex);
    QString price = QString::number(ui->comboStorn->itemData(currentIndex).toInt());

    QString data = "";

    QString loggedProfileId;
    QString loggedProfileCode;

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        loggedProfileId = QString::number(mw->loggedProfileId());
        loggedProfileCode = QString("%1").arg(mw->loggedProfileCode(), 4, 10, QChar('0'));

        // sekvenca+chr(48)+sifra_kasier+','+lozinka_na_kasierot+',1'

        data.append(QString(QChar(32)) + QString(QChar(48)) + loggedProfileId + "," + loggedProfileCode + ",1" + QString(QChar(10)));

        int row = 0;

        // sekvenca+chr(49)+naziv_prv_red+chr(10)+naziv_vtor_red+chr(9)+danocna_grupa+cena+'*'+kolicina+','+procent_na_popust  //za procenten popust
        // sekvenca+chr(49)+naziv_prv_red+chr(10)+naziv_vtor_red+chr(9)+danocna_grupa+cena+'*'+kolicina+'''+iznos_na_popust  // za popust vo iznos

        QString itemText1251 = Settings::ConvertTo1251(text);
        data.append(QString(QChar(33 + row)) + QString(QChar(49)) + itemText1251 + QString(QChar(9)) +
                    ddv + price + ".00" + "*" + ui->lineEditQuantity->text() + ".000,0.00" + QString(QChar(10)));

        data.append(QString(QChar(33 + row++)) + QString(QChar(53)) + QString(QChar(9)) + QString(QChar(10)));
        data.append(QString(QChar(33 + row)) + QString(QChar(56)) + QString(QChar(10)));
    }

    Settings::CreateFileAndPrint(data, "smetka_storn.txt");
}

void ReportWindow::on_buttonInputMoney_clicked()
{
    QString data(Settings::Sequence() + QString(QChar(70)) + ui->lineEditInputMoney->text());

    Settings::CreateFileAndPrint(data, "input_money.txt");
}

void ReportWindow::on_buttonOutputMoney_clicked()
{
    QString data(Settings::Sequence() + QString(QChar(70)) + "-" + ui->lineEditOutputMoney->text());

    Settings::CreateFileAndPrint(data, "output_money.txt");
}

void ReportWindow::on_buttonPrintPeriod_clicked()
{
    QString data(Settings::Sequence() + QString(QChar(94)) +
                 ui->dateEditBegin->dateTime().toString("ddMMyy") + "," +
                 ui->dateEditEnd->dateTime().toString("ddMMyy"));

    Settings::CreateFileAndPrint(data, "period.txt");
}

void ReportWindow::on_buttonPrintDaily_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сте сигурни?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        QString data(Settings::Sequence() + QString(QChar(69)) + "3");

        Settings::CreateFileAndPrint(data, "dneven_bez.txt");
    }
}

void ReportWindow::on_buttonPrintDailyNulling_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сте сигурни?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        QString data(Settings::Sequence() + QString(QChar(69)) + "1");

        Settings::CreateFileAndPrint(data, "dneven_so.txt");
    }
}
