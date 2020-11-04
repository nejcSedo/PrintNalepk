#include "proizvodniproces.h"
#include "ui_proizvodniproces.h"

ProizvodniProces::ProizvodniProces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProizvodniProces), m_searchLine(""), m_searchList(), m_green(0,170,0),
    m_white(255,255,255), m_grey(220,220,220), m_opis(176,224,230), m_darkGrey(255,255,130),
    m_picFolder("C:\\Users\\Admin\\Documents\\dev\\build-PrintNalepk-Desktop_Qt_5_14_2_MinGW_64_bit-Release\\images\\"),
    m_naziv_bool(false), m_numOfImages(0)
{
    ui->setupUi(this);
    this->setWindowTitle("Proizvodni proces");
    this->showMaximized();
    QStringList glava;
    glava.append("Opomba");
    glava.append("Št. orodja  :\n(zgornja gl. - spodnja gl.)");
    glava.append("Dolžina");
    glava.append("Oznaka");
    glava.append("Barva vodnika");
    glava.append("Oznaka");
    glava.append("Dolžina");
    glava.append("Št. orodja  :\n(zgornja gl. - spodnja gl.)");
    glava.append("Opomba");
    ui->treeWidget->setHeaderLabels(glava);
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->treeWidget->setRootIsDecorated(false);
    ui->label_A->setFixedWidth(ui->treeWidget->columnWidth(0) + ui->treeWidget->columnWidth(1) + ui->treeWidget->columnWidth(2) + ui->treeWidget->columnWidth(3) + ui->treeWidget->columnWidth(4));
    ui->label_B->setFixedWidth(ui->treeWidget->columnWidth(4) + ui->treeWidget->columnWidth(5) + ui->treeWidget->columnWidth(6) + ui->treeWidget->columnWidth(7) + ui->treeWidget->columnWidth(8));
    QFont font("Arial", 20, QFont::Bold);
    ui->lineEdit_isciProdukt->setFocus();
}

ProizvodniProces::~ProizvodniProces()
{
    delete ui;
}

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

void ProizvodniProces::AddRootToTreeWidget(const QStringList& list, QTreeWidgetItem* itm, QString& naziv, bool naziv_bool)
{
    if(!naziv_bool)
    {
        itm->setText(4, naziv);
        itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(4, QBrush(m_opis));
    }
    else if(list.at(0) == "/" && list.at(1) == "/" && list.at(2) == "/" && list.at(3) == "/" &&
            list.at(5) == "/" && list.at(6) == "/" && list.at(7) == "/" && list.at(8) == "/")
    {
        QString presledek("--------------------------");
        itm->setText(0, presledek);
        itm->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(0, QBrush(m_white));

        itm->setText(1, presledek);
        itm->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(1, QBrush(m_white));

        itm->setText(2, presledek);
        itm->setTextAlignment(2, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(2, QBrush(m_white));

        itm->setText(3, presledek);
        itm->setTextAlignment(3, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(3, QBrush(m_white));

        itm->setText(4, list.at(4));
        itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(4, QBrush(m_darkGrey));

        itm->setText(5, presledek);
        itm->setTextAlignment(5, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(5, QBrush(m_white));

        itm->setText(6, presledek);
        itm->setTextAlignment(6, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(6, QBrush(m_white));

        itm->setText(7, presledek);
        itm->setTextAlignment(7, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(7, QBrush(m_white));

        itm->setText(8, presledek);
        itm->setTextAlignment(8, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(8, QBrush(m_white));
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
        if(list.at(2) == "/" || list.at(2) == "" || naziv.contains("Vodnik", Qt::CaseInsensitive))
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
        if(list.at(6) == "/" || list.at(6) == "" || naziv.contains("Vodnik", Qt::CaseInsensitive))
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

void ProizvodniProces::OpisProdukta(QStringList& partList)
{
    if(partList.length() > 9)
    {
        partList.removeAt(0);
        partList.removeAt(0);
    }
    if(m_naziv_bool && partList.at(1) != "/")
    {
        QFile file("orodja.txt");

        if(!file.open(QFile::Text | QFile::ReadOnly))
            qDebug() << "Error orodja";

        QTextStream out(&file);
        out.setCodec("UTF-8");

        QStringList searchList;
        QString removeToolHeader = partList.at(1);
        searchList = removeToolHeader.split('*', QString::SkipEmptyParts);
        QString tmp = searchList.at(1);
        tmp.remove(" ");
        searchList = removeToolHeader.split(':', QString::SkipEmptyParts);
        searchList.replace(1, tmp);

        while(!out.atEnd())
        {
            QString searchLine = out.readLine();

            QStringList searchList3 = searchLine.split('*', QString::SkipEmptyParts);
            QString tmp2 = searchList3.at(1);
            tmp2.remove(" ");
            searchList3 = searchLine.split(':', QString::SkipEmptyParts);
            searchList3.replace(1, tmp2);
            if(searchList3.at(0) == searchList.at(0) && searchList3.at(1) == searchList.at(1))
            {
                searchLine.replace(":", "  : ");
                searchList = searchLine.split('*', QString::SkipEmptyParts);
                partList.replace(1, searchList.at(0));

            }
        }
    }

    if(m_naziv_bool && partList.at(7) != "/")
    {
        QFile file("orodja.txt");

        if(!file.open(QFile::Text | QFile::ReadOnly))
            qDebug() << "Error orodja";

        QTextStream out(&file);
        out.setCodec("UTF-8");

        QString removeToolHeader = partList.at(7);
        QStringList searchList2 = removeToolHeader.split('*', QString::SkipEmptyParts);
        QString tmp = searchList2.at(1);
        tmp.remove(" ");
        searchList2 = removeToolHeader.split(':', QString::SkipEmptyParts);
        searchList2.replace(1, tmp);
        while(!out.atEnd())
        {
            QString searchLine = out.readLine();

            QStringList searchList3 = searchLine.split('*', QString::SkipEmptyParts);
            QString tmp2 = searchList3.at(1);
            tmp2.remove(" ");
            searchList3 = searchLine.split(':', QString::SkipEmptyParts);
            searchList3.replace(1, tmp2);

            if(searchList3.at(0) == searchList2.at(0) && searchList3.at(1) == searchList2.at(1))
            {
                searchLine.replace(":", "  : ");
                searchList2 = searchLine.split('*', QString::SkipEmptyParts);
                partList.replace(7, searchList2.at(0));
            }
        }
    }

    if(m_naziv_bool && partList.at(4).contains("*"))
    {
        QString tmp = partList.at(4);
        tmp.remove("*");
        partList.replace(4, tmp);
    }
}

void ProizvodniProces::on_lineEdit_isciProdukt_textChanged(const QString &arg1)
{
    ui->listWidget->clear();
    Search(arg1);
}

void ProizvodniProces::ClearWidgets(QLayout* layout)
{
    if (!layout)
        return;
    while (auto item = layout->takeAt(0))
    {
        delete item->widget();
        ClearWidgets(item->layout());
        m_numOfImages = 0;
    }
}

void ProizvodniProces::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->background(4) == QBrush(m_opis))
    {
        ClearWidgets(ui->verticalLayout_image);
        QString imageName(item->text(4));
        imageName.remove("Vodnik\n");
        imageName.remove("Kabel\n");
        imageName.remove("Veriga\n");
        imageName.remove("Snop\n");
        imageName.remove("Drugo\n");
        QDirIterator it(m_picFolder, QStringList() << imageName + "*", QDir::Files, QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            m_numOfImages++;
            it.next();
        }

        for(int i(1); i <= m_numOfImages; i++)
        {
            QPixmap image(m_picFolder + imageName + "_" + QString::number(i) + ".jpg");
            QLabel* label = new QLabel();
            label->setPixmap(image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->verticalLayout_image->addWidget(label);
        }
        ui->verticalLayout_image->addStretch(1);
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
    if(item->text(0) == "" || item->text(1) == "" || item->text(2) == "" || item->text(3) == "" || item->text(5) == "" || item->text(6) == "" || item->text(7) == "" || item->text(8) == "")
    {
        item->setBackground(0, QBrush(m_white));
        item->setBackground(1, QBrush(m_white));
        item->setBackground(2, QBrush(m_white));
        item->setBackground(3, QBrush(m_white));
        item->setBackground(5, QBrush(m_white));
        item->setBackground(6, QBrush(m_white));
        item->setBackground(7, QBrush(m_white));
        item->setBackground(8, QBrush(m_white));
    }
}

void ProizvodniProces::on_pushButton_pocisti_clicked()
{
    ClearWidgets(ui->verticalLayout_image);
    ui->lineEdit_isciProdukt->clear();
    ui->listWidget->clear();
    ui->treeWidget->clear();
    ui->label_opis->clear();
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
                        QStringList partList = m_searchLine.split(";", QString::SkipEmptyParts);
                        naziv = "Vodnik\n" + naziv;
                        m_naziv_bool = false;
                        QTreeWidgetItem* itm = new QTreeWidgetItem();
                        if(partList.length() > 9)
                        {
                            QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                            AddRootToTreeWidget(partList, itm1, naziv, m_naziv_bool);
                            m_naziv_bool = true;
                        }
                        OpisProdukta(partList);
                        AddRootToTreeWidget(partList, itm, naziv, m_naziv_bool);
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
                        m_naziv_bool = false;
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                AddRootToTreeWidget(partList, itm1, naziv, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, m_naziv_bool);
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
                        m_naziv_bool = false;
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                AddRootToTreeWidget(partList, itm1, naziv, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, m_naziv_bool);
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
                        m_naziv_bool = false;
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                AddRootToTreeWidget(partList, itm1, naziv, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, m_naziv_bool);
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
                        m_naziv_bool = false;
                        while(tmp != "***")
                        {
                            QTreeWidgetItem* itm = new QTreeWidgetItem();
                            if(partList.length() > 9)
                            {
                                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                                AddRootToTreeWidget(partList, itm1, naziv, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            qDebug() << "error";
                            OpisProdukta(partList);
                            qDebug() << partList;
                            AddRootToTreeWidget(partList, itm, naziv, m_naziv_bool);
                            qDebug() << "error";
                            index++;
                            qDebug() << "error";
                            tmp = m_searchList.at(index);
                            qDebug() << "error";
                            partList = tmp.split(";", QString::SkipEmptyParts);
                            qDebug() << "error";
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
    ClearWidgets(ui->verticalLayout_image);
}

void ProizvodniProces::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->label_opis->setText(item->text(column));
}
