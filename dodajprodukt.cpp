#include "dodajprodukt.h"
#include "ui_dodajprodukt.h"

DodajProdukt::DodajProdukt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DodajProdukt), m_steviloZil(0)
{
    ui->setupUi(this);
    this->setWindowTitle("Dodaj produkt");
    this->showMaximized();
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Interactive);
    ui->comboBox_stevilozil->insertItem(0, "1");
    ui->comboBox_stevilozil->setDisabled(true);
    m_steviloZil = 1;
    ui->lineEdit->setFocus();
}

DodajProdukt::~DodajProdukt()
{
    delete ui;
}

void DodajProdukt::on_pushButton_clicked()
{
    QFile file("proizvodniproces.txt");

    if(!file.open(QFile::WriteOnly | QFile::Append))
        qDebug() << "ERORRRRR";

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QString naziv("");
    if(ui->comboBox_vrstaProdukta->currentIndex() == 0)
    {
        m_steviloZil = 1;
        naziv = "V;";
    }
    else if(ui->comboBox_vrstaProdukta->currentIndex() == 1)
        naziv = "K;";
    else if(ui->comboBox_vrstaProdukta->currentIndex() == 2)
        naziv = "VR;";
    else if(ui->comboBox_vrstaProdukta->currentIndex() == 3)
        naziv = "S;";
    else
        naziv = "N;";
    naziv += ui->lineEdit->text().toUpper();
    while(naziv.at(naziv.length() - 1) == ' ')
        naziv.remove(-1, 1);
    naziv += ";";
    for(int i(0); i < m_steviloZil; i++)
    {
        for(int j(0); j < 9; j++)
        {
            if(j == 1 || j == 4 || j == 7)
            {
                QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,j));
                QStringList nazivList;
                nazivList << myCB->currentText();
                if(nazivList.at(0) != "")
                    naziv += nazivList.at(0) + ";";
                else
                    naziv += "/;";
            }
            else
            {
                if(ui->tableWidget->item(i, j) && ui->tableWidget->item(i, j)->text() != "")
                    naziv += ui->tableWidget->item(i, j)->text() + ";";
                else
                    naziv += "/;";
            }
        }
        naziv += "&;";
    }
    if(ui->comboBox_vrstaProdukta->currentIndex() == 1 || ui->comboBox_vrstaProdukta->currentIndex() == 2 || ui->comboBox_vrstaProdukta->currentIndex() == 3 || ui->comboBox_vrstaProdukta->currentIndex() == 4)
    {
        naziv.remove(-1,1);
        naziv += "***&;";
    }
    out << naziv << "\n";
    file.flush();
    file.close();
    ui->lineEdit->clear();
    ui->comboBox_vrstaProdukta->setCurrentIndex(0);
    ui->comboBox_stevilozil->setCurrentIndex(0);
    ui->tableWidget->clear();
    ui->lineEdit->setFocus();
}

void DodajProdukt::on_comboBox_vrstaProdukta_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->comboBox_stevilozil->setDisabled(true);
        for(int i(m_steviloZil); i >= 0; i--)
            ui->tableWidget->removeRow(i);
        ui->tableWidget->insertRow(0);
        m_steviloZil = 1;
    }
    else
    {
        ui->comboBox_stevilozil->setDisabled(false);
        ui->tableWidget->removeRow(0);
        ui->comboBox_stevilozil->clear();
        for(int i(0); i < 200; i++)
            ui->comboBox_stevilozil->insertItem(i, QString ::number(i + 1));
    }
}

void DodajProdukt::on_comboBox_stevilozil_currentIndexChanged(int index)
{
    for(int i(m_steviloZil); i >= 0; i--)
        ui->tableWidget->removeRow(i);

    m_steviloZil = 0;
    for(int i(0); i <= index; i++)
    {
        ui->tableWidget->insertRow(i);
        m_steviloZil++;
    }
}

void DodajProdukt::on_tableWidget_cellClicked(int row, int column)
{
    if(column == 1 || column == 7)
    {
        QFile file("orodja.txt");

        if(!file.open(QFile::Text | QFile::ReadOnly))
            qDebug() << "Errorrrrrrr";

        QTextStream out(&file);
        out.setCodec("UTF-8");
        QStringList searchList;
        while(!out.atEnd())
        {
            QString searchLine = out.readAll();
            searchList = searchLine.split('\n', QString::SkipEmptyParts);
        }
        QComboBox* combo = new QComboBox();
        combo->addItems(searchList);
        ui->tableWidget->setCellWidget(row, column, combo);
    }
    if(column == 4)
    {
        QFile file("barve.txt");

        if(!file.open(QFile::Text | QFile::ReadOnly))
            qDebug() << "Errorrrrrrr";

        QTextStream out(&file);
        out.setCodec("UTF-8");
        QStringList searchList;
        while(!out.atEnd())
        {
            QString searchLine = out.readAll();
            searchList = searchLine.split('\n', QString::SkipEmptyParts);
        }
        QComboBox* combo = new QComboBox();
        combo->addItems(searchList);
        ui->tableWidget->setCellWidget(row, column, combo);
    }
}
