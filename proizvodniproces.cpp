#include "proizvodniproces.h"
#include "ui_proizvodniproces.h"

ProizvodniProces::ProizvodniProces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProizvodniProces), m_searchLine(""), m_searchList(), m_green(0,170,0),
    m_white(255,255,255), m_grey(220,220,220), m_opis(176,224,230), m_darkGrey(255,255,130),
    m_picFolder("C:\\Users\\sedov\\Documents\\Qt projekti\\build-PrintNalepk-Desktop_Qt_5_15_1_MinGW_64_bit-Release\\images\\"),
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
    QFile file("ShraniBarve.txt");

    if(!file.open(QFile::WriteOnly | QFile::Truncate))
        qDebug() << "Errorrrrrrr";
    file.flush();
    file.close();

    ShraniBackgroundColor();
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

void ProizvodniProces::AddRootToTreeWidget(const QStringList& list, QTreeWidgetItem* itm, QString& naziv, QString& http, bool naziv_bool)
{
    if(!naziv_bool)
    {
        itm->setText(4, naziv);
        itm->setTextAlignment(4, Qt::AlignHCenter | Qt::AlignVCenter);
        itm->setBackground(4, QBrush(m_opis));
        itm->setText(6, http);
        itm->setForeground(6, QColor(255,255,255,255));

        QUrl link("http://elra.chickenkiller.com/" + itm->text(6) + ".pdf");
        bool fileExist(false);
        QTcpSocket socket;
        socket.connectToHost(link.host(), 80);
        if (socket.waitForConnected()) {
            socket.write("HEAD " + link.path().toUtf8() + " HTTP/1.1\r\n"
            "Host: " + link.host().toUtf8() + "\r\n"
            "\r\n");
            if(socket.waitForReadyRead())
            {
                QByteArray bytes = socket.readAll();
                if (bytes.contains("200 OK"))
                    fileExist = true;
            }
        }

        if(fileExist)
        {
            itm->setText(5, "Tehnični načrt");
            itm->setTextAlignment(5, Qt::AlignHCenter | Qt::AlignVCenter);
            itm->setBackground(5, QBrush(QColor(51,153,255)));
        }
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

    else if(list.at(0) != "-*-")
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

    else
    {
        itm->setBackground(0, QBrush(QColor(100,100,100)));
        itm->setBackground(1, QBrush(QColor(100,100,100)));
        itm->setBackground(2, QBrush(QColor(100,100,100)));
        itm->setBackground(3, QBrush(QColor(100,100,100)));
        itm->setBackground(4, QBrush(QColor(100,100,100)));
        itm->setBackground(5, QBrush(QColor(100,100,100)));
        itm->setBackground(6, QBrush(QColor(100,100,100)));
        itm->setBackground(7, QBrush(QColor(100,100,100)));
        itm->setBackground(8, QBrush(QColor(100,100,100)));
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

void ProizvodniProces::TrenutniSeznam(QString& naziv)
{
    QFile file("trenutniSeznam.txt");

    if(!file.open(QFile::WriteOnly | QFile::Append))
        qDebug() << "ERORRRRR";

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << naziv << "\n";
    file.flush();
    file.close();
}

void ProizvodniProces::ShraniBackgroundColor()
{
    QFile file("ShraniBarve.txt");

    if(!file.open(QFile::WriteOnly | QFile::Append))
        qDebug() << "ERORRRRR";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    QString line("");
    for(int row(0); row < ui->treeWidget->topLevelItemCount(); row++)
    {
        for(int column(0); column < 9; column++)
        {
            QColor color(ui->treeWidget->topLevelItem(row)->backgroundColor(column));
            if(color.name() == "#000000")
                color.setRgb(255,255,255);
            line += (color.name() + ";");
        }
    }
    out << line;
    file.flush();
    file.close();
}

void ProizvodniProces::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(column == 5 && item->text(5) == "Tehnični načrt")
    {
        QUrl link("http://elra.chickenkiller.com/" + item->text(6) + ".pdf");
        QDesktopServices::openUrl(QUrl(link));
    }

    if(item->background(4) == QBrush(m_opis))
    {
        ClearWidgets(ui->verticalLayout_image);

        QString productText(item->text(4));
        QStringList productList = productText.split(" -- ", Qt::SkipEmptyParts);
        QString imageName(productList.at(1));
        QString productName(productList.at(0));
        productName.remove("Vodnik\n");
        productName.remove("Kabel\n");
        productName.remove("Veriga\n");
        productName.remove("Snop\n");
        productName.remove("Drugo\n");
        QDirIterator it(m_picFolder + productName + "\\", QStringList() << imageName + "*", QDir::Files, QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            m_numOfImages++;
            it.next();
        }

        for(int i(1); i <= m_numOfImages; i++)
        {
            QPixmap image(m_picFolder + productName + "\\" + imageName + "_" + QString::number(i) + ".jpg");
            QLabel* label = new QLabel();
            label->setPixmap(image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->verticalLayout_image->addWidget(label);
        }
        ui->verticalLayout_image->addStretch(1);
    }        

    if(item->background(4) == QBrush(m_darkGrey))
    {
        QString productText(item->text(4));
        QStringList productList = productText.split(" -- ", Qt::SkipEmptyParts);
        QString imageName(productList.at(1));
        QString productName(productList.at(0));

        QPixmap image(m_picFolder + productName + "\\" + imageName + ".jpg");
        image = image.scaled(this->width(), this->height(), Qt::AspectRatioMode::KeepAspectRatio);
        QLabel* label = new QLabel();
        QHBoxLayout* layout = new QHBoxLayout(this);
        label->setPixmap(image);
        layout->addWidget(label);
        QDialog* imageDialog = new QDialog(this);
        imageDialog->setWindowTitle(item->text(4));
        imageDialog->setLayout(layout);
        imageDialog->showMaximized();
        imageDialog->show();
        imageDialog->exec();
    }

    if(item->background(column) == QBrush(m_green))
    {
        if(column % 2 == 1)
            item->setBackground(column, QBrush(m_grey));
        else if(column % 2 == 0)
            item->setBackground(column, QBrush(m_white));
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

    else if(item->text(3) == "--------------------------")
    {
        item->setBackground(0, QBrush(m_white));
        item->setBackground(1, QBrush(m_white));
        item->setBackground(2, QBrush(m_white));
        item->setBackground(3, QBrush(m_white));
        item->setBackground(4, QBrush(m_darkGrey));
        item->setBackground(5, QBrush(m_white));
        item->setBackground(6, QBrush(m_white));
        item->setBackground(7, QBrush(m_white));
        item->setBackground(8, QBrush(m_white));
    }

    else
        item->setBackground(4, QBrush(m_white));

    if(item->background(0) == QBrush(QColor(100,100,100)))
    {
        item->setBackground(0, QBrush(QColor(100,100,100)));
        item->setBackground(1, QBrush(QColor(100,100,100)));
        item->setBackground(2, QBrush(QColor(100,100,100)));
        item->setBackground(3, QBrush(QColor(100,100,100)));
        item->setBackground(4, QBrush(QColor(100,100,100)));
        item->setBackground(5, QBrush(QColor(100,100,100)));
        item->setBackground(6, QBrush(QColor(100,100,100)));
        item->setBackground(7, QBrush(QColor(100,100,100)));
        item->setBackground(8, QBrush(QColor(100,100,100)));
    }

    else if(item->text(0) == "" || item->text(1) == "" || item->text(2) == "" || item->text(3) == "" || item->text(5) == "" || item->text(6) == "" || item->text(7) == "" || item->text(8) == "")
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

    if(item->text(5) == "Tehnični načrt")
        item->setBackground(5, QBrush(QColor(51,153,255)));
}

void ProizvodniProces::on_pushButton_pocisti_clicked()
{
    QFile file("trenutniSeznam.txt");

    if(!file.open(QFile::WriteOnly | QFile::Truncate))
        qDebug() << "Errorrrrrrr";
    file.flush();
    file.close();

    QFile file1("ShraniBarve.txt");

    if(!file1.open(QFile::WriteOnly | QFile::Truncate))
        qDebug() << "Errorrrrrrr";
    file1.flush();
    file1.close();

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
                if(m_searchList.at(0).at(0) == "V")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        QString http(m_searchList.at(0));
                        http.remove(0,1);
                        TrenutniSeznam(m_searchLine);
                        QStringList partList = m_searchLine.split(";", QString::SkipEmptyParts);
                        naziv = "Vodnik\n" + naziv;
                        m_naziv_bool = false;
                        QTreeWidgetItem* itm = new QTreeWidgetItem();
                        if(partList.length() > 9)
                        {
                            QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                            AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                            m_naziv_bool = true;
                        }
                        OpisProdukta(partList);
                        AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                        QTreeWidgetItem* itm2 = new QTreeWidgetItem();
                        partList.clear();
                        partList.append("-*-");
                        AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
                    }
                }
                else if(m_searchList.at(0).at(0) == "K")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        QString http(m_searchList.at(0));
                        http.remove(0,1);
                        TrenutniSeznam(m_searchLine);
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
                                AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                        QTreeWidgetItem* itm2 = new QTreeWidgetItem();
                        partList.clear();
                        partList.append("-*-");
                        AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
                    }
                }
                else if(m_searchList.at(0).at(1) == "R")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        QString http(m_searchList.at(0));
                        http.remove(0,2);
                        TrenutniSeznam(m_searchLine);
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
                                AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                        QTreeWidgetItem* itm2 = new QTreeWidgetItem();
                        partList.clear();
                        partList.append("-*-");
                        AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
                    }
                }
                else if(m_searchList.at(0).at(0) == "S")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        QString http(m_searchList.at(0));
                        http.remove(0,1);
                        TrenutniSeznam(m_searchLine);
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
                                AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                        QTreeWidgetItem* itm2 = new QTreeWidgetItem();
                        partList.clear();
                        partList.append("-*-");
                        AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
                    }
                }
                else if(m_searchList.at(0).at(0) == "N")
                {
                    if(m_searchList.at(1).toUpper() == naziv)
                    {
                        QString http(m_searchList.at(0));
                        http.remove(0,1);
                        TrenutniSeznam(m_searchLine);
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
                                AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                                m_naziv_bool = true;
                            }
                            OpisProdukta(partList);
                            AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                            index++;
                            tmp = m_searchList.at(index);
                            partList = tmp.split(";", QString::SkipEmptyParts);
                        }
                        QTreeWidgetItem* itm2 = new QTreeWidgetItem();
                        partList.clear();
                        partList.append("-*-");
                        AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
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

void ProizvodniProces::on_pushButton_clicked()
{
    QFile file("trenutniSeznam.txt");

    if(!file.open(QFile::Text | QFile::ReadOnly))
        qDebug() << "Errorrrrrrr";

    QTextStream out(&file);
    out.setCodec("UTF-8");
    QString naziv("");
    while(!out.atEnd())
    {
        m_searchLine = out.readLine();
        m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
        if(m_searchList.at(0).at(0) == "V")
        {
            QString http(m_searchList.at(0));
            http.remove(0,1);
            QStringList partList = m_searchLine.split(";", QString::SkipEmptyParts);
            naziv = "Vodnik\n" + partList.at(1);
            m_naziv_bool = false;
            QTreeWidgetItem* itm = new QTreeWidgetItem();
            if(partList.length() > 9)
            {
                QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                m_naziv_bool = true;
            }
            OpisProdukta(partList);
            AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
            QTreeWidgetItem* itm2 = new QTreeWidgetItem();
            partList.clear();
            partList.append("-*-");
            AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
        }
        else if(m_searchList.at(0).at(0) == "K")
        {
            QString http(m_searchList.at(0));
            http.remove(0,1);
            int index(0);
            m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
            QStringList partList;
            QString tmp = m_searchList.at(index);
            partList = tmp.split(";", QString::SkipEmptyParts);
            naziv = "Kabel\n" + partList.at(1);
            m_naziv_bool = false;
            while(tmp != "***")
            {
                QTreeWidgetItem* itm = new QTreeWidgetItem();
                if(partList.length() > 9)
                {
                    QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                    AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                    m_naziv_bool = true;
                }
                OpisProdukta(partList);
                AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                index++;
                tmp = m_searchList.at(index);
                partList = tmp.split(";", QString::SkipEmptyParts);
            }
            QTreeWidgetItem* itm2 = new QTreeWidgetItem();
            partList.clear();
            partList.append("-*-");
            AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
        }
        else if(m_searchList.at(0).at(1) == "R")
        {
            QString http(m_searchList.at(0));
            http.remove(0,2);
            int index(0);
            m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
            QStringList partList;
            QString tmp = m_searchList.at(index);
            partList = tmp.split(";", QString::SkipEmptyParts);
            naziv = "Veriga\n" + partList.at(1);
            m_naziv_bool = false;
            while(tmp != "***")
            {
                QTreeWidgetItem* itm = new QTreeWidgetItem();
                if(partList.length() > 9)
                {
                    QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                    AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                    m_naziv_bool = true;
                }
                OpisProdukta(partList);
                AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                index++;
                tmp = m_searchList.at(index);
                partList = tmp.split(";", QString::SkipEmptyParts);
            }
            QTreeWidgetItem* itm2 = new QTreeWidgetItem();
            partList.clear();
            partList.append("-*-");
            AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
        }
        else if(m_searchList.at(0).at(0) == "S")
        {
            QString http(m_searchList.at(0));
            http.remove(0,1);
            int index(0);
            m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
            QStringList partList;
            QString tmp = m_searchList.at(index);
            partList = tmp.split(";", QString::SkipEmptyParts);
            naziv = "Snop\n" + partList.at(1);
            m_naziv_bool = false;
            while(tmp != "***")
            {
                QTreeWidgetItem* itm = new QTreeWidgetItem();
                if(partList.length() > 9)
                {
                    QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                    AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                    m_naziv_bool = true;
                }
                OpisProdukta(partList);
                AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                index++;
                tmp = m_searchList.at(index);
                partList = tmp.split(";", QString::SkipEmptyParts);
            }
            QTreeWidgetItem* itm2 = new QTreeWidgetItem();
            partList.clear();
            partList.append("-*-");
            AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
        }
        else if(m_searchList.at(0).at(0) == "N")
        {
            QString http(m_searchList.at(0));
            http.remove(0,1);
            int index(0);
            m_searchList = m_searchLine.split("&", QString::SkipEmptyParts);
            QStringList partList;
            QString tmp = m_searchList.at(index);
            partList = tmp.split(";", QString::SkipEmptyParts);
            naziv = "Drugo\n" + partList.at(1);
            m_naziv_bool = false;
            while(tmp != "***")
            {
                QTreeWidgetItem* itm = new QTreeWidgetItem();
                if(partList.length() > 9)
                {
                    QTreeWidgetItem* itm1 = new QTreeWidgetItem();
                    AddRootToTreeWidget(partList, itm1, naziv, http, m_naziv_bool);
                    m_naziv_bool = true;
                }
                OpisProdukta(partList);
                AddRootToTreeWidget(partList, itm, naziv, http, m_naziv_bool);
                index++;
                tmp = m_searchList.at(index);
                partList = tmp.split(";", QString::SkipEmptyParts);
            }
            QTreeWidgetItem* itm2 = new QTreeWidgetItem();
            partList.clear();
            partList.append("-*-");
            AddRootToTreeWidget(partList, itm2, naziv, http, m_naziv_bool);
        }
    }
    file.close();


    QFile fileColor("ShraniBarve.txt");

    if(!fileColor.open(QFile::Text | QFile::ReadOnly))
        qDebug() << "ERORRRRR";

    QTextStream inColor(&fileColor);
    inColor.setCodec("UTF-8");
    QString colorLine = inColor.readLine();
    fileColor.close();
    QStringList colorList = colorLine.split(";", Qt::SkipEmptyParts);
    int index(0);
    for(int row(0); row < ui->treeWidget->topLevelItemCount(); row++)
    {
        for(int column(0); column < 9; column++)
        {
            QColor color(colorList.at(index));
            index++;
            ui->treeWidget->topLevelItem(row)->setBackground(column, color);
        }
    }
}
