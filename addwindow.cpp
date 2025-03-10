#include "addwindow.h"
#include "ui_addwindow.h"
#include "settings.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSqlRecord>
#include <QFileDialog>

AddWindow::AddWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddWindow),
    m_parent(parent)
{
    ui->setupUi(this);

    updateGroups();

    ui->comboDDV->addItems(Settings::DDVGroups.split(", "));

    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        ui->tableProduct->setModel(mw->productsModel());
        ui->tableGroup->setModel(mw->groupsModel());
    }
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::updateGroups()
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->groupsModel();
        ui->comboAddGroup->clear();

        for (int row = 0; row < model->rowCount(); row++)
        {
            QSqlRecord record = model->record(row);
            QString name = record.value("name").toString();

            ui->comboAddGroup->addItem(name);
        }
    }
}

void AddWindow::on_buttonAddProduct_clicked()
{
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->productsModel();
        // create a record from the current model
        QSqlRecord rec = model->record();
        rec.setValue("groups", Settings::NameId(mw->groupsModel(), ui->comboAddGroup->currentText()));
        rec.setValue("name", ui->lineEditAddProduct->text());
        rec.setValue("price", ui->lineEditAddPrice->text());
        rec.setValue("ddv", Settings::ComboDDVChar(ui->comboDDV->currentIndex()));
        rec.setValue("icon", ui->lineEditIcon->text());
        // insert a new record (-1)
        model->insertRecord(-1, rec);
        model->select();
    }

    ui->lineEditAddProduct->clear();
    ui->lineEditAddPrice->clear();
    ui->lineEditIcon->clear();
}

void AddWindow::on_buttonAddGroup_clicked()
{
    QString name(ui->lineEditAddGroup->text());
    if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
    {
        QSqlTableModel* model = mw->groupsModel();
        if (!Settings::NameId(model, name))
        {
            QSqlRecord rec = model->record();
            rec.setValue("name", name);
            model->insertRecord(-1, rec);
            model->select();
        }
        else
        {
            QMessageBox::warning(this, "", "Веќе постои група со внесеното име!\n"
                                           "Обидете се повторно.");
        }

        updateGroups();
    }

    ui->lineEditAddGroup->clear();
}

void AddWindow::on_buttonRemoveProduct_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сакате да ги избришете селектираните записи?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
        {
            QSqlTableModel* model = mw->productsModel();
            QModelIndexList indexes = ui->tableProduct->selectionModel()->selection().indexes();
            foreach (QModelIndex index, indexes)
            {
                model->removeRow(index.row());
            }
            model->select();
        }
    }
}

void AddWindow::on_buttonRemoveGroup_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Дали сакате да ги избришете селектираните записи?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (msgBox.exec() == QMessageBox::Ok)
    {
        if (MainWindow* mw = dynamic_cast<MainWindow*>(m_parent))
        {
            QSqlTableModel* model = mw->groupsModel();
            QModelIndexList indexes = ui->tableGroup->selectionModel()->selection().indexes();
            foreach (QModelIndex index, indexes)
            {
                model->removeRow(index.row());
            }
            model->select();

            updateGroups();
        }
    }
}

void AddWindow::on_toolButton_clicked()
{
    ui->lineEditIcon->setText(QFileDialog::getOpenFileName(
        this, tr("Open Icon"), QApplication::applicationDirPath().append("/icons"), tr("Image Files (*.png *.jpg *.bmp)")));
}
