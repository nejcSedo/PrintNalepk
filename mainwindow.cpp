#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_count(true), m_isClicked(false), m_id(""), m_naziv("")
{
    ui->setupUi(this);
    QIcon icon(":icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Tiskanje nalepk");
    ui->lineEdit_IDprodukta->setFocus();
    MainWindow::setWindowIcon(icon);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->setRootIsDecorated(false);
    ui->lineEdit_IDprodukta->setMaxLength(40);
    ui->lineEdit_nazivProdukta->setMaxLength(40);
    ui->lineEdit_kolicina->setMaxLength(30);
    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    Read();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddRoot(QString id, QString naziv)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);

    itm->setText(0, id.toUpper());
    itm->setTextAlignment(0, Qt::AlignHCenter);
    itm->setText(1, naziv.toUpper());
    itm->setTextAlignment(1, Qt::AlignHCenter);

    ui->treeWidget->addTopLevelItem(itm);

    QColor color(220,220,220);
    QColor wcolor(250,250,250);

    if(ui->treeWidget->topLevelItemCount() % 2 == 0)
        m_count = true;
    else
        m_count = false;

    if(m_count)
    {
        itm->setBackground(0, wcolor);
        itm->setBackground(1, wcolor);
    } else {
        itm->setBackground(0, color);
        itm->setBackground(1, color);
    }
}

void MainWindow::Read()
{
    ui->treeWidget->clear();
    QString produkti = "produkti.txt";

    QFile fileName(produkti);

    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening file for reading in Read()";
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString mText("");
    QRegExp rx("[;]");
    QStringList list;

    while(!in.atEnd())
    {
        mText = in.readLine();
        list = mText.split(rx, QString::SkipEmptyParts);

        if(mText == "" || mText == "DELETE" || mText == "DELETE;DELETE")
            continue;
        else
            AddRoot(list.at(0), list.at(1));
    }

    fileName.close();
}

void MainWindow::Search(QString id, QString naziv)
{
    QString produkti = "produkti.txt";
    QFile mFile(produkti);

    if(!mFile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in Search";
        return;
    }
    QTextStream out(&mFile);
    out.setCodec("UTF-8");
    QString line;
    QRegExp rx("[;]");
    QStringList list;
    while(!out.atEnd())
    {
        line = out.readLine();
        if((line.contains(id, Qt::CaseInsensitive) && id != "") || (line.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            list = line.split(rx, QString::SkipEmptyParts);
            if(list.at(0) == "DELETE")
                continue;
            else
                AddRoot(list.at(0), list.at(1));
        }
    }
    mFile.close();
}

void MainWindow::Nalepka()
{
    QString id("");
    QString naziv("");
    QString kolicina("");
    if(ui->lineEdit_kolicina->text() == "")
        kolicina = "/";
    else
        kolicina = ui->lineEdit_kolicina->text().toUpper();

    if(ui->lineEdit_IDprodukta->text() == "")
        id = "/";
    else
        id = ui->lineEdit_IDprodukta->text().toUpper();

    if(ui->lineEdit_nazivProdukta->text() == "")
        naziv = "/";
    else
        naziv = ui->lineEdit_nazivProdukta->text().toUpper();

    QString text_nalepka =
        /**************************** NALEPKA *********************************/
        "<font color='#000000'>"
        "<table align='center' width='100%' height='100%'>"
            "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<font size='4'>Elra   Seti   d.o.o.,   Andraž   nad   Polzelo   74/a,   3313   Polzela"
                    "</th>"
                "</tr>"
            "</div>"
            "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<font size=6><b>LIST IZDELKA</b>"
                    "</th>"
                "</tr>"
            "</div>"
        "</table>"
        "<table border=1 align='center' width='100%' height='100%'>"
            "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<p align='center'>"
                            "<font size='4'><b>Datum izdelave:</b>"
                        "</p>"
                    "</th>"
                    "<th>"
                        "<font size='4'>" + QDate::currentDate().toString("d. M. yyyy") + ""
                    "</th>"
                "</tr>"
            "</div>"
            "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<p align='center'>"
                            "<font size='4'><b>ID produkta:</b>"
                        "</p>"
                    "</th>"
                    "<th>"
                        "<font size='4'>" + id + ""
                    "</th>"
                "</tr>"
            "</div>"
            "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<p align='center'>"
                            "<font size='4'><b>Naziv produkta:</b>"
                        "</p>"
                    "</th>"
                    "<th>"
                        "<font size='4'>" + naziv + ""
                    "</th>"
                "</tr>"
             "</div>"
             "<div align='center'>"
                 "<tr>"
                     "<th>"
                         "<p align='center'>"
                             "<font size='4'><b>Število kosov:</b>"
                         "</p>"
                     "</th>"
                     "<th>"
                         "<font size='4' text-align=left>" + kolicina + ""
                     "</th>"
                 "</tr>"
             "</div>"
           "</table>"
           "<table align='center' width='100%' height='100%'>"
             "<div align='center'>"
                "<tr>"
                    "<th>"
                        "<p align='center'>"
                            "<img src=:icons/elraseti.png width=400 height=200>"
                        "</p>"
                    "</th>"
                "</tr>"
             "</div>"
         "</table>";

    QTextDocument nalepka;
    QTextCursor cursor(&nalepka);
    cursor.insertHtml(text_nalepka);

    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setPageSize(QPrinter::A6);
    printer->setOrientation(QPrinter::Portrait);
    printer->setFullPage(true);

    QSizeF paperSize;
    qreal left = 0, right = -5, top =-2, bottom = 10, width=1500, height=1100;
    paperSize.setWidth(width);
    paperSize.setHeight(height);

    nalepka.setPageSize(paperSize);
    printer->setPageMargins(left, top, right, bottom, QPrinter::Millimeter);

    QFont f(nalepka.defaultFont());
    f.setPointSize(18);
    nalepka.setDefaultFont(f);

    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == QDialog::Accepted)
    {
        nalepka.print(printer);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Insert)
    {
        if(ui->treeWidget->topLevelItem(0) == nullptr)
        {
            ui->lineEdit_IDprodukta->setText("");
            ui->lineEdit_nazivProdukta->setText("");
            ui->lineEdit_kolicina->setText("");
            ui->pushButton_shraniNalepko->setDisabled(true);
        }
        else
        {
            ui->lineEdit_IDprodukta->setText(ui->treeWidget->topLevelItem(0)->text(0));
            ui->lineEdit_nazivProdukta->setText(ui->treeWidget->topLevelItem(0)->text(1));
            ui->lineEdit_kolicina->setFocus();
        }
    }

    if(event->key() == Qt::Key_Delete)
    {
        m_isClicked = false;
        ui->lineEdit_IDprodukta->clear();
        ui->lineEdit_nazivProdukta->clear();
        ui->lineEdit_kolicina->clear();
        ui->lineEdit_IDprodukta->setFocus();
    }
}

void MainWindow::ProduktCheck(QString id, QString naziv)
{
    QString produkti = "produkti.txt";
    QString line("");
    QStringList list;
    QRegExp rx("[;]");
    QFile fileName(produkti);

    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in on_pushButton_odstraniProdukt_clicked";
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    line = in.readLine();
    while(!in.atEnd())
    {
        line = in.readLine();

        if(id.at(id.length()-1) == ' ')
            id = id.remove(id.length()-1, 1);

        if(naziv.at(naziv.length()-1) == ' ')
            naziv = naziv.remove(naziv.length()-1, 1);

        list = line.split(rx, QString::SkipEmptyParts);

        if(list.at(0) == id || list.at(1) == naziv || id == "" || naziv == "")
        {
            ui->pushButton_shraniNalepko->setDisabled(true);
            ui->actionShrani_nalepko->setDisabled(true);
            break;
        }
        else
        {
            ui->pushButton_shraniNalepko->setDisabled(false);
            ui->actionShrani_nalepko->setDisabled(false);
        }
    }
    fileName.close();
}

void MainWindow::on_pushButton_shraniNalepko_clicked()
{
    MainWindow::on_actionShrani_nalepko_triggered();
}

void MainWindow::on_pushButton_natisni_clicked()
{
    MainWindow::on_actionPrint_triggered();
}

void MainWindow::on_lineEdit_IDprodukta_textChanged(const QString &arg1)
{
    if(m_isClicked)
    {
        m_isClicked = false;
        return;
    }
    if(arg1 == "")
    {
        Read();
        ui->pushButton_shraniNalepko->setDisabled(true);
        return;
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ')
        ui->lineEdit_IDprodukta->backspace();
    else
    {
        ui->treeWidget->clear();
        ui->lineEdit_kolicina->clear();
        Search(arg1.toUpper(), ui->lineEdit_nazivProdukta->text().toUpper());
        ProduktCheck(arg1.toUpper(), ui->lineEdit_nazivProdukta->text().toUpper());
    }
}

void MainWindow::on_lineEdit_nazivProdukta_textChanged(const QString &arg1)
{
    if(m_isClicked)
    {
        m_isClicked = false;
        return;
    }
    if(arg1 == "")
    {
        Read();
        ui->pushButton_shraniNalepko->setDisabled(true);
        return;
    }
    if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ')
        ui->lineEdit_nazivProdukta->backspace();
    else
    {
        ui->treeWidget->clear();
        ui->lineEdit_kolicina->clear();
        Search(ui->lineEdit_IDprodukta->text().toUpper(), arg1.toUpper());
        ProduktCheck(ui->lineEdit_IDprodukta->text().toUpper(), arg1.toUpper());
    }
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    m_isClicked = true;
    ui->lineEdit_IDprodukta->setText(item->text(0));
    ui->lineEdit_nazivProdukta->setText(item->text(1));
    ui->lineEdit_kolicina->setFocus();
    ui->treeWidget->clear();
    Read();
    return;
    qDebug() << column;
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QIcon bin(":icons/bin.ico");
    menu.addAction(ui->actionDelete);

    ui->actionDelete->setData(QVariant(pos));
    ui->actionDelete->setIcon(bin);
    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}

void MainWindow::on_actionIzhod_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionDelete_triggered()
{
    if(ui->treeWidget->currentItem() == nullptr)
    {
        return;
    }

    QString produkti = "produkti.txt";

    QFile fileName(produkti);
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error opening mFile for reading in on_pushButton_odstraniProdukt_clicked";
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString allText = in.readAll();
    fileName.close();

    QString produkt(ui->treeWidget->currentItem()->text(0)+";"+ui->treeWidget->currentItem()->text(1));
    allText.replace(produkt, "DELETE");
    if(!fileName.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Error opening mFile for truncate in on_pushButton_odstraniProdukt_clicked";
        return;
    }
    fileName.flush();
    fileName.close();

    if(!fileName.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error opening mFile for writing in on_pushButton_odstraniProdukt_clicked";
        return;
    }
    in << allText;
    fileName.flush();
    fileName.close();

    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    Read();
}

void MainWindow::on_actionShrani_nalepko_triggered()
{
    QString produkti = "produkti.txt";
    QFile fileName(produkti);

    if(!fileName.open(QFile::WriteOnly | QFile::Append))
    {
        qDebug() << "Error opening fileName for writing in on_pushButton_shraniNalepko_clicked";
        return;
    }

    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    m_id = ui->lineEdit_IDprodukta->text().toUpper();
    m_naziv = ui->lineEdit_nazivProdukta->text().toUpper();

    while(m_naziv.at(m_naziv.length() - 1) == ' ')
    {
        m_naziv.remove(-1, 1);
    }

    out << m_id << ";" << m_naziv << "\n";
    fileName.flush();
    fileName.close();
    Read();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    ui->lineEdit_kolicina->setFocus();
}

void MainWindow::on_actionPrint_triggered()
{
    Nalepka();
    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->lineEdit_IDprodukta->setFocus();
}

void MainWindow::on_actionO_programu_triggered()
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QIcon icon(":icons/about.ico");
    dialog->setWindowTitle("O programu");
    dialog->setWindowIcon(icon);
    label->setText("Program za shranjevanje in tiskanje nalepk.\nTiskalnik Rollo.\n\nIzdelal: Nejc Sedovnik\nLeto: April 2020\nVerzija: 1.0");
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->exec();
}
