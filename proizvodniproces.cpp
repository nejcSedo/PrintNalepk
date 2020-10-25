#include "proizvodniproces.h"
#include "ui_proizvodniproces.h"

ProizvodniProces::ProizvodniProces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProizvodniProces), m_searchLine(""), m_searchList(), m_green(0,170,0), m_white(255,255,255), m_grey(220,220,220), m_opis(176,224,230), m_picFolder("C:\\Users\\sedov\\Desktop\\")
{
    ui->setupUi(this);
    this->setWindowTitle("Proizvodni proces");
    this->showMaximized();
    QStringList glava;
    glava.append("Opomba");
    glava.append("Številka orodja\nnastavitev glave\n(zgornja / spodnja)");
    glava.append("Dolžina");
    glava.append("Oznaka");
    glava.append("Barva vodnika");
    glava.append("Oznaka");
    glava.append("Dolžina");
    glava.append("Številka orodja\nnastavitev glave\n(zgornja / spodnja)");
    glava.append("Opomba");
    ui->treeWidget->setHeaderLabels(glava);
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(6, QHeaderView::Stretch);
    QFont font("Arial", 20, QFont::Bold);
    ui->lineEdit_isciProdukt->setFocus();
}

ProizvodniProces::~ProizvodniProces()
{
    delete ui;
}

// ISKALNIK
void ProizvodniProces::Search(const QString& naziv)
{
    QFile file("proizvodniproces.txt");

    if(!file.open(QFile::Text | QFile::ReadOnly))
        qDebug() << "Errorrrrrrr";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    while(!out.atEnd())
    {
        m_searchLine = out.readLine();
        if((m_searchLine.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            if(m_searchLine == "Proizvodni proces:")
                continue;
            if(m_searchLine.contains("DELETE"))
                continue;
            else
            {
                m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
                if(m_searchList.at(1).toUpper().contains(naziv.toUpper(), Qt::CaseInsensitive))
                    ui->listWidget->addItem(m_searchList.at(1).toUpper());
            }
        }
    }
    file.close();
}

// ITEM V TREEWIDGET
void ProizvodniProces::AddRootToTreeWidget(const QStringList& list, QTreeWidgetItem* itm, QString& naziv, bool naziv_bool)
{
    if(!naziv_bool)
    {
        itm->setText(4, naziv);
        itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(4, QBrush(m_opis));
    }
    else
    {
        itm->setText(0, list.at(0));
        itm->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(0) == "/" || list.at(0) == "")
            itm->setBackground(0, QBrush(m_green));

        itm->setText(1, list.at(1));
        itm->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(1) == "/" || list.at(1) == "")
            itm->setBackground(1, QBrush(m_green));
        else
            itm->setBackground(1, QBrush(m_grey));

        itm->setText(2, list.at(2));
        itm->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(2) == "/" || list.at(2) == "")
            itm->setBackground(2, QBrush(m_green));

        itm->setText(3, list.at(3));
        itm->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(3) == "/" || list.at(3) == "")
            itm->setBackground(3, QBrush(m_green));
        else
            itm->setBackground(3, QBrush(m_grey));

        itm->setText(4, list.at(4));
        itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(4) == "/" || list.at(4) == "")
            itm->setBackground(4, QBrush(m_green));

        itm->setText(5, list.at(5));
        itm->setTextAlignment(5, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(5) == "/" || list.at(5) == "")
            itm->setBackground(5, QBrush(m_green));
        else
            itm->setBackground(5, QBrush(m_grey));

        itm->setText(6, list.at(6));
        itm->setTextAlignment(6, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(6) == "/" || list.at(6) == "")
            itm->setBackground(6, QBrush(m_green));

        itm->setText(7, list.at(7));
        itm->setTextAlignment(7, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(7) == "/" || list.at(7) == "")
            itm->setBackground(7, QBrush(m_green));
        else
            itm->setBackground(7, QBrush(m_grey));

        itm->setText(8, list.at(8));
        itm->setTextAlignment(8, Qt::AlignHCenter | Qt::AlignVCenter);
        if(list.at(8) == "/" || list.at(8) == "")
            itm->setBackground(8, QBrush(m_green));
    }

    ui->treeWidget->addTopLevelItem(itm);
}

void ProizvodniProces::on_lineEdit_isciProdukt_textChanged(const QString &arg1)
{
    ui->listWidget->clear();
    Search(arg1);
}

void ProizvodniProces::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->background(4) == QBrush(m_opis))
    {
        ui->label_image->clear();
        QString imageName(item->text(4));
        imageName.remove("Vodnik\n");
        imageName.remove("Kabel\n");
        imageName.remove("Veriga\n");
        imageName.remove("Snop\n");
        imageName.remove("Drugo\n");
        QPixmap image(m_picFolder + imageName + ".jpg");
        ui->label_image->setPixmap(image);
    }

    if(item->background(column) == QBrush(m_green))
    {
        if(column % 2 == 1)
            item->setBackground(column, QBrush(m_grey));
        else if(column % 2 == 0)
            item->setBackground(column, QBrush(m_white));
    }
    else
        item->setBackground(column, QBrush(m_green));

    if(item->background(0) == QBrush(m_green) && item->background(1) == QBrush(m_green) && item->background(2) == QBrush(m_green) && item->background(3) == QBrush(m_green) &&
       item->background(5) == QBrush(m_green) && item->background(6) == QBrush(m_green) && item->background(7) == QBrush(m_green) && item->background(8) == QBrush(m_green))
        item->setBackground(4, QBrush(m_green));
    else if(item->text(4).contains("Vodnik") || item->text(4).contains("Kabel") || item->text(4).contains("Veriga") || item->text(4).contains("Snop") || item->text(4).contains("Drugo"))
        item->setBackground(4, QBrush(m_opis));
    else
        item->setBackground(4, QBrush(m_white));
}

void ProizvodniProces::on_pushButton_pocisti_clicked()
{
    ui->label_image->clear();
    ui->lineEdit_isciProdukt->clear();
    ui->listWidget->clear();
    ui->treeWidget->clear();
}

void ProizvodniProces::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString naziv = item->text();
    QFile file("proizvodniproces.txt");

    if(!file.open(QFile::Text | QFile::ReadOnly))
        qDebug() << "Errorrrrrrr";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    while(!out.atEnd())
    {
        m_searchLine = out.readLine();
        if((m_searchLine.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            if(m_searchLine == "Proizvodni proces:")
                continue;
            if(m_searchLine.contains("DELETE"))
                continue;
            else
            {
                m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
                if(m_searchList.at(0) == "V")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        bool naziv_bool(false);
                        for(int i(0); i < 2; i++)
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            m_searchList = m_searchLine.split(";", QString::SkipEmptyParts);
                            m_searchList.removeAt(0);
                            m_searchList.removeAt(0);
                            naziv = "Vodnik\n" + naziv;
                            AddRootToTreeWidget(m_searchList, itm, naziv, naziv_bool);
                            naziv_bool = true;
                        }
                    }
                }
                else if(m_searchList.at(0) == "K")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        int index(0);
                        m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
                        QStringList partList;
                        QString tmp = m_searchList.at(index);
                        partList = tmp.split(";", QString::SkipEmptyParts);
                        naziv = "Kabel\n" + naziv;
                        bool naziv_bool(false);
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                partList.removeAt(0);
                                partList.removeAt(0);
                                AddRootToTreeWidget(partList, itm1, naziv, naziv_bool);
                                naziv_bool = true;
                            }
                            AddRootToTreeWidget(partList, itm, naziv, naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                    }
                }
                else if(m_searchList.at(0) == "VR")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        int index(0);
                        m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
                        QStringList partList;
                        QString tmp = m_searchList.at(index);
                        partList = tmp.split(";", QString::SkipEmptyParts);
                        naziv = "Veriga\n" + naziv;
                        bool naziv_bool(false);
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                partList.removeAt(0);
                                partList.removeAt(0);
                                AddRootToTreeWidget(partList, itm1, naziv, naziv_bool);
                                naziv_bool = true;
                            }
                            AddRootToTreeWidget(partList, itm, naziv, naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                    }
                }
                else if(m_searchList.at(0) == "S")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        int index(0);
                        m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
                        QStringList partList;
                        QString tmp = m_searchList.at(index);
                        partList = tmp.split(";", QString::SkipEmptyParts);
                        naziv = "Snop\n" + naziv;
                        bool naziv_bool(false);
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                partList.removeAt(0);
                                partList.removeAt(0);
                                AddRootToTreeWidget(partList, itm1, naziv, naziv_bool);
                                naziv_bool = true;
                            }
                            AddRootToTreeWidget(partList, itm, naziv, naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                    }
                }
                else if(m_searchList.at(0) == "N")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        int index(0);
                        m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
                        QStringList partList;
                        QString tmp = m_searchList.at(index);
                        partList = tmp.split(";", QString::SkipEmptyParts);
                        naziv = "Drugo\n" + naziv;
                        bool naziv_bool(false);
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                partList.removeAt(0);
                                partList.removeAt(0);
                                AddRootToTreeWidget(partList, itm1, naziv, naziv_bool);
                                naziv_bool = true;
                            }
                            AddRootToTreeWidget(partList, itm, naziv, naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                    }
                }
            }
        }
    }
    file.close();
}

void ProizvodniProces::on_pushButton_skrijSliko_clicked()
{
    ui->label_image->clear();
}
