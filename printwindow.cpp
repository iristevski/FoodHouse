#include "printwindow.h"
#include "ui_printwindow.h"
#include "settings.h"
#include "mainwindow.h"
#include "printpreview.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QPushButton>
#include <QListView>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QProcess>

PrintWindow::PrintWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintWindow),
    m_parent(parent)
{
    ui->setupUi(this);

    ui->listWidgetGroups->setStyleSheet(Settings::GroupsStylesheet());
    ui->listWidgetProducts->setStyleSheet(Settings::ProductsStylesheet());

    QStringList headerLabels;
    headerLabels << "#" << "Име" << "Кол." << "Цена (ден.)" << "Вкупно (ден.)";
    ui->tablePrint->setHorizontalHeaderLabels(headerLabels);
    ui->tablePrint->horizontalHeader()->resizeSection(0, 20);
    ui->tablePrint->hideColumn(5);

    ui->lineEditSum->setText("0");
}

PrintWindow::~PrintWindow()
{
    delete ui;
}

void PrintWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    updateGroups();
    ui->listWidgetGroups->setCurrentRow(0);
}

void PrintWindow::updateGroups()
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->groupsModel();
        ui->listWidgetGroups->clear();

        for (int row = 0; row < model->rowCount(); row++)
        {
            QSqlRecord record = model->record(row);
            QString name = record.value("name").toString();

            QListWidgetItem* item = new QListWidgetItem(name);
            ui->listWidgetGroups->addItem(item);
        }

        on_listWidgetGroups_currentTextChanged("");
    }
}

void PrintWindow::updateProducts(int currentGroup/* = 1*/)
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->productsModel();
        ui->listWidgetProducts->clear();

        for (int row = 0; row < model->rowCount(); row++)
        {
            QSqlRecord record = model->record(row);
            int group = record.value("groups").toInt();
            if (group == currentGroup)
            {
                QString icon = record.value("icon").toString();
                QString name = record.value("name").toString();
                int id = record.value("id").toInt();
                int price = record.value("price").toInt();
                QString ddv = record.value("ddv").toString();

                int pWidth = 128;
                int pHeight = 80;
                QPixmap pixmap(pWidth, pHeight);
                pixmap.fill(Qt::gray);

                if (!icon.isEmpty())
                {
                    pixmap = QIcon(icon).pixmap(pWidth, pHeight);
                    pixmap = pixmap.scaled(pWidth, pHeight, Qt::KeepAspectRatio);
                }

                QIcon iconFinal;
                iconFinal.addPixmap(pixmap, QIcon::Normal);
                iconFinal.addPixmap(pixmap, QIcon::Selected);

                QListWidgetItem* item = new QListWidgetItem(iconFinal, name);
                item->setSizeHint(QSize(pWidth, pWidth));

                // userrole is the id of the item
                item->setData(Qt::UserRole, id);
                // tooltip role is the price of the item
                item->setData(Qt::ToolTipRole, price);
                // userrole + 1 is the ddv group of the item
                item->setData(Qt::UserRole + 1, ddv);

                ui->listWidgetProducts->addItem(item);
            }
        }
    }
}

void PrintWindow::updateSum()
{
    int sum = 0;
    for (int row = 0; row < ui->tablePrint->rowCount(); row++)
    {
        sum += ui->tablePrint->item(row, 4)->text().toInt();
    }

    ui->lineEditSum->setText(QString::number(sum));
}

void PrintWindow::on_listWidgetProducts_itemClicked(QListWidgetItem *item)
{
    int row = ui->tablePrint->rowCount();
    int col = 0;

    // userrole is the id of the item
    int intItemId = item->data(Qt::UserRole).toInt();
    int intTableItemRowIfExists = -1;

    for (int i = 0; i < row; i++)
    {
        int intTableItemId = ui->tablePrint->item(i, 0)->text().toInt();
        if (intItemId == intTableItemId)
        {
            intTableItemRowIfExists = ui->tablePrint->item(i, 0)->row();
            break;
        }
    }

    // item not exist int the print table
    if (intTableItemRowIfExists == -1)
    {
        ui->tablePrint->insertRow(row);

        // id column 0
        QTableWidgetItem *tableItemId = new QTableWidgetItem(item->data(Qt::UserRole).toString());
        ui->tablePrint->setItem(row, col++, tableItemId);

        // name column 1
        QTableWidgetItem *tableItemName = new QTableWidgetItem(item->text());
        ui->tablePrint->setItem(row, col++, tableItemName);

        // quantity column 2
        QTableWidgetItem *tableItemQuantity = new QTableWidgetItem(QString::number(1));
        ui->tablePrint->setItem(row, col++, tableItemQuantity);

        // price column 3
        QTableWidgetItem *tableItemPrice = new QTableWidgetItem(item->data(Qt::ToolTipRole).toString());
        ui->tablePrint->setItem(row, col++, tableItemPrice);

        // sum column 4
        QTableWidgetItem *tableItemSum = new QTableWidgetItem(item->data(Qt::ToolTipRole).toString());
        ui->tablePrint->setItem(row, col++, tableItemSum);

        // ddv column 5
        QTableWidgetItem *tableItemDDV = new QTableWidgetItem(item->data(Qt::UserRole + 1).toString());
        ui->tablePrint->setItem(row, col++, tableItemDDV);
    }
    else
    {
        int intTableItemQuantity = ui->tablePrint->item(intTableItemRowIfExists, 2)->text().toInt();
        int intTableItemPrice = ui->tablePrint->item(intTableItemRowIfExists, 3)->text().toInt();
        int intTableItemSum = ui->tablePrint->item(intTableItemRowIfExists, 4)->text().toInt();

        // quantity column 2
        QTableWidgetItem *tableItemQuantity = new QTableWidgetItem(QString::number(++intTableItemQuantity));
        ui->tablePrint->setItem(intTableItemRowIfExists, 2, tableItemQuantity);

        // sum column 4
        QTableWidgetItem *tableItemSum = new QTableWidgetItem(QString::number(intTableItemPrice + intTableItemSum));
        ui->tablePrint->setItem(intTableItemRowIfExists, 4, tableItemSum);
    }

    updateSum();
}

void PrintWindow::on_tablePrint_itemClicked(QTableWidgetItem *item)
{
    int itemRow = item->row();
    int intItemQuantity = ui->tablePrint->item(itemRow, 2)->text().toInt();
    int intItemPrice = ui->tablePrint->item(itemRow, 3)->text().toInt();
    int intItemSum = ui->tablePrint->item(itemRow, 4)->text().toInt();

    if (intItemQuantity == 1)
    {
        ui->tablePrint->removeRow(itemRow);
    }
    else
    {
        // quantity column 2
        QTableWidgetItem *itemQuantity = new QTableWidgetItem(QString::number(--intItemQuantity));
        ui->tablePrint->setItem(itemRow, 2, itemQuantity);

        // sum column 4
        QTableWidgetItem *itemSum = new QTableWidgetItem(QString::number(intItemSum - intItemPrice));
        ui->tablePrint->setItem(itemRow, 4, itemSum);
    }

    updateSum();
}

void PrintWindow::on_listWidgetGroups_currentTextChanged(const QString &currentText)
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        updateProducts(Settings::NameId(mw->groupsModel(), currentText));
    }
}

void PrintWindow::on_buttonPrint_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сте сигурни?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        if (ui->checkBoxFiscal->isChecked())
        {
            printFiscal();
        }
        if (ui->checkBoxTermal->isChecked())
        {
            printTermal();
        }

        ui->tablePrint->model()->removeRows(0, ui->tablePrint->rowCount());
        ui->lineEditSum->setText("0");
    }
}

void PrintWindow::printFiscal()
{
    m_data = "";

    QString loggedProfileId;
    QString loggedProfileCode;

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        loggedProfileId = QString::number(mw->loggedProfileId());
        loggedProfileCode = QString("%1").arg(mw->loggedProfileCode(), 4, 10, QChar('0'));

        int tableRowCount = ui->tablePrint->rowCount();
        if (tableRowCount > 0)
        {
            // sekvenca+chr(48)+sifra_kasier+','+lozinka_na_kasierot+',1'

            m_data.append(QString(QChar(32)) + QString(QChar(48)) + loggedProfileId + "," + loggedProfileCode + ",1" + QString(QChar(10)));

            int row = 0;
            for ( ; row < tableRowCount; row++)
            {
                QTableWidgetItem* itemText = ui->tablePrint->item(row, 1);
                QTableWidgetItem* itemQuantity = ui->tablePrint->item(row, 2);
                QTableWidgetItem* itemPrice = ui->tablePrint->item(row, 3);
                QTableWidgetItem* itemDDV = ui->tablePrint->item(row, 5);

                // sekvenca+chr(49)+naziv_prv_red+chr(10)+naziv_vtor_red+chr(9)+danocna_grupa+cena+'*'+kolicina+','+procent_na_popust  //za procenten popust
                // sekvenca+chr(49)+naziv_prv_red+chr(10)+naziv_vtor_red+chr(9)+danocna_grupa+cena+'*'+kolicina+'''+iznos_na_popust  // za popust vo iznos

                QString itemText1251 = Settings::ConvertTo1251(itemText->text());
                m_data.append(QString(QChar(33 + row)) + QString(QChar(49)) + itemText1251 + QString(QChar(9)) +
                            itemDDV->text() + itemPrice->text() + ".00" + "*" + itemQuantity->text() + ".000,0.00" + QString(QChar(10)));

            }

            m_data.append(QString(QChar(33 + row++)) + QString(QChar(53)) + QString(QChar(9)) + QString(QChar(10)));
            m_data.append(QString(QChar(33 + row)) + QString(QChar(56)) + QString(QChar(10)));
        }
    }

    Settings::CreateFileAndPrint(m_data, "smetka.txt");
}

void PrintWindow::printTermal()
{
    int tableRowCount = ui->tablePrint->rowCount();
    if (tableRowCount > 0)
    {
        PrintPreview pp;

        for (int row = 0; row < tableRowCount; row++)
        {
            QTableWidgetItem* itemText = ui->tablePrint->item(row, 1);
            QTableWidgetItem* itemQuantity = ui->tablePrint->item(row, 2);

            pp.setHtmlRow(QStringList() <<
                          itemText->text() <<
                          itemQuantity->text());
        }

        pp.print();
    }
}
