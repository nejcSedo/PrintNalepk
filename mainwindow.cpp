#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_count(true), m_isClicked(false), m_id(""), m_naziv(""), m_verzija("v1.4.5"), m_verzijaLabel(new QLabel(this)), m_nalepkaCentimeter(75), m_qrVelikost(m_nalepkaCentimeter*1.33)
{
    ui->setupUi(this);
    QIcon icon(":icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Tiskanje nalepk");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->setRootIsDecorated(false);
    Read();
    ui->lineEdit_IDprodukta->setMaxLength(40);
    ui->lineEdit_nazivProdukta->setMaxLength(40);
    ui->lineEdit_kolicina->setMaxLength(40);
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->pushButton_natisni->setDisabled(true);
    ui->actionPrint->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    m_verzijaLabel->setText(m_verzija);
    ui->statusbar->addPermanentWidget(m_verzijaLabel);
    ui->comboBox_seznamNalepk->addItem("10x8");
    ui->comboBox_seznamNalepk->addItem("10x4");
    ui->comboBox_seznamNalepk->setDisabled(true);

    ui->lineEdit_IDprodukta->setFocus();
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
    ui->treeWidget->topLevelItemCount() % 2 == 0 ? m_count = true : m_count = false;
    m_count ? itm->setBackground(0, wcolor) : itm->setBackground(0, color);
    m_count ? itm->setBackground(1, wcolor) : itm->setBackground(1, color);
}

void MainWindow::Read()
{
    ui->treeWidget->clear();
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::ReadOnly | QFile::Text))
    {
        Error(0);
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString mText("");
    QStringList list;
    while(!in.atEnd())
    {
        mText = in.readLine();
        if(mText == "" || mText == "DELETE;DELETE" || mText == "Seznam nalepk:")
            continue;
        else
        {
            list = mText.split(';', QString::SkipEmptyParts);
            AddRoot(list.at(0), list.at(1));
        }
    }

    fileName.close();
}

void MainWindow::Search(QString id, QString naziv)
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        Error(1);
        return;
    }
    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    QString line;
    QStringList list;
    while(!out.atEnd())
    {
        line = out.readLine();
        if((line.contains(id, Qt::CaseInsensitive) && id != "") || (line.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            if(line == "Seznam nalepk:")
                continue;
            if(list.contains("DELETE"))
                continue;
            else
            {
                list = line.split(';', QString::SkipEmptyParts);
                AddRoot(list.at(0), list.at(1));
            }
        }
    }
    fileName.close();
}

void MainWindow::drawText(QPainter & painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect = 0)
{
   const qreal size = 32767.0;
   QPointF corner(x, y - size);
   if (flags & Qt::AlignHCenter)
       corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight)
       corner.rx() -= size;
   if (flags & Qt::AlignVCenter)
       corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop)
       corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   painter.drawText(rect, flags, text, boundingRect);
}

void MainWindow::drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect = {})
{
   drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}

void MainWindow::Nalepka()
{
    QString id(ui->lineEdit_IDprodukta->text().toUpper());
    QString naziv(ui->lineEdit_nazivProdukta->text().toUpper());
    QString kolicina("");
    ui->lineEdit_kolicina->text() == "" ? kolicina = "-" : kolicina = ui->lineEdit_kolicina->text().toUpper();
    QStringList dimenzijeNalepke(ui->comboBox_seznamNalepk->itemText(0).split('x', Qt::SkipEmptyParts));
    short sirinaNalepke(dimenzijeNalepke.at(0).toInt());
    short visinaNalepke(dimenzijeNalepke.at(1).toInt());
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    QTextDocument nalepka;
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height()/2);
    nalepka.setPageSize(paperSize);

    QChar newLine('\u000A');
    QString qrText("-- ELRA SETI d.o.o. --");
    qrText += newLine;
    qrText += "www.elraseti.si";
    qrText += newLine;
    qrText += "info@elraseti.si";
    qrText += newLine;
    qrText += "Opombe:";
    qrText += newLine;

    QPixmap map(m_qrVelikost,m_qrVelikost);
    QPainter painter(&map);
    paintQR(painter,QSize(m_qrVelikost,m_qrVelikost), qrText + ui->textEdit_opombe->toPlainText(), QColor("black"));
    map.save("image.png");
    painter.end();

    QPainter painterText(printer);
    painterText.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),
                           ((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),
                           m_nalepkaCentimeter*3.5,
                           m_nalepkaCentimeter*3.5, map);

    painterText.drawRect(m_nalepkaCentimeter/6,
                         m_nalepkaCentimeter/6,
                         sirinaNalepke*m_nalepkaCentimeter,
                         visinaNalepke*m_nalepkaCentimeter);

    painterText.setFont(QFont("Tahoma",9));
    const QPointF ptHeader(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter/2));
    drawText(painterText, ptHeader, Qt::AlignVCenter | Qt::AlignHCenter, "Elra   Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");

    painter.setFont(QFont("Tahoma",16));
    const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*1.2));
    drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, "LIST IZDELKA");

    painterText.setFont(QFont("Tahoma",11));
    const QPointF pt1(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*2));
    drawText(painterText, pt1, Qt::AlignVCenter | Qt::AlignHCenter, "Datum izdelave: " + QDate::currentDate().toString("d. M. yyyy"));

    const QPointF pt2(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*2.5));
    drawText(painterText, pt2, Qt::AlignVCenter | Qt::AlignHCenter, "ID izdelka: " + id);

    const QPointF pt3(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
    drawText(painterText, pt3, Qt::AlignVCenter | Qt::AlignHCenter, "Naziv izdelka: " + naziv);

    const QPointF pt4(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3.5));
    drawText(painterText, pt4, Qt::AlignVCenter | Qt::AlignHCenter, "Količina: " + kolicina);

    QPrintDialog printDialog(printer, this);
    printer->setPageSize(QPrinter::A7);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins (1,1,1,1,QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setOutputFormat(QPrinter::NativeFormat);
    painterText.end();
    if(printDialog.exec() == QDialog::Accepted)
        nalepka.print(printer);

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
        ui->textEdit_opombe->clear();
        ui->actionPrint->setDisabled(true);
        ui->actionShrani_nalepko->setDisabled(true);
        ui->pushButton_natisni->setDisabled(true);
        ui->lineEdit_IDprodukta->setFocus();
    }
}

void MainWindow::ProduktCheck(QString id, QString naziv)
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        Error(2);
        return;
    }

    QTextStream in(&fileName);
    QString line("");
    QStringList list;
    short numOfLines(0);
    in.setCodec("UTF-8");
    while(!in.atEnd())
    {
        line = in.readLine();
        numOfLines++;
        if(line == "Seznam nalepk:")
            continue;

        id.at(id.length()-1) == ' ' ? id = id.remove(id.length()-1, 1) : nullptr;
        naziv.at(naziv.length()-1) == ' ' ? naziv = naziv.remove(naziv.length()-1, 1) : nullptr;

        list = line.split(';', QString::SkipEmptyParts);

        if(id == "" || naziv == "")
        {
            ui->pushButton_natisni->setDisabled(true);
            ui->actionPrint->setDisabled(true);
        }
        else
        {
            ui->pushButton_natisni->setDisabled(false);
            ui->actionPrint->setDisabled(false);
        }

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

    if(numOfLines == 1 && id != "" && naziv != "")
    {
        ui->pushButton_shraniNalepko->setDisabled(false);
        ui->pushButton_natisni->setDisabled(false);
    }

    fileName.close();
}

void MainWindow::Error(short numError)
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QIcon icon(":icons/error.ico");
    dialog->setWindowTitle("Napaka");
    dialog->setWindowIcon(icon);
    switch (numError)
    {
        case 0: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji Read() ne obstaja!\n Znova zaženi program!"); break;
        case 1: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji Search() ne obstaja!\n Znova zaženi program!"); break;
        case 2: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji ProductCheck() ne obstaja!\n Znova zaženi program!"); break;
        case 3: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case 4: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case 5: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case 6: label->setText("ErrorNum: " + QString(QString::number(numError)) + "\nfileName v funkciji on_actionShrani_nalepko_triggered() ne obstaja!\n Znova zaženi program!"); break;
        default: label->setText("Neznana napaka!\n Znova zaženi program!"); break;
    }
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->exec();
}

void MainWindow::paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg)
{
    char *str=data.toUtf8().data();
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::HIGH);
    int sizeOf = qr.getInt();
    const int s=sizeOf>0?sizeOf:1;
    const double w=sz.width();
    const double h=sz.height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    for(int y=0; y<400; y++)
    {
        for(int x=0; x<400; x++)
        {
            QRectF r(x, y, 1, 1);
            painter.drawRects(&r,1);
        }
    }
    painter.setBrush(fg);
    for(int y=0; y<s; y++)
    {
        for(int x=0; x<s; x++)
        {
            const int color = qr.getModule(x, y);
            if(0x0!=color)
            {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
            }
        }
    }
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
        ui->textEdit_opombe->clear();
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
        ui->textEdit_opombe->clear();
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
    Error(column);
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QIcon bin(":icons/delete.ico");
    menu.addAction(ui->actionDelete);
    ui->actionDelete->setData(QVariant(pos));
    ui->actionDelete->setIcon(bin);
    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}

void MainWindow::on_actionIzhod_triggered()
{
    ui->statusbar->showMessage("Izhod", 3000);
    QApplication::quit();
}

void MainWindow::on_actionDelete_triggered()
{
    if(ui->treeWidget->currentItem() == nullptr)
        return;

    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        Error(3);
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString allText = in.readAll();
    fileName.close();

    QString produkt(ui->treeWidget->currentItem()->text(0)+";"+ui->treeWidget->currentItem()->text(1));
    allText.replace(produkt, "DELETE;DELETE");

    if(!fileName.open(QFile::WriteOnly | QFile::Truncate))
    {
        Error(4);
        return;
    }
    fileName.flush();
    fileName.close();

    if(!fileName.open(QFile::WriteOnly | QFile::Text))
    {
        Error(5);
        return;
    }
    in << allText;
    fileName.flush();
    fileName.close();

    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    ui->textEdit_opombe->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->actionPrint->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    Read();
    ui->statusbar->showMessage("Nalepka izbrisana", 3000);
}

void MainWindow::on_actionShrani_nalepko_triggered()
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::WriteOnly | QFile::Append))
    {
        Error(6);
        return;
    }

    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    m_id = ui->lineEdit_IDprodukta->text().toUpper();
    m_naziv = ui->lineEdit_nazivProdukta->text().toUpper();
    while(m_naziv.at(m_naziv.length() - 1) == ' ')
        m_naziv.remove(-1, 1);

    out << m_id << ";" << m_naziv << "\n";
    fileName.flush();
    fileName.close();

    Read();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    ui->lineEdit_kolicina->setFocus();
    ui->statusbar->showMessage("Nalepka shranjena", 3000);
}

void MainWindow::on_actionPrint_triggered()
{
    Nalepka();
    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    ui->textEdit_opombe->clear();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->pushButton_natisni->setDisabled(true);
    ui->lineEdit_IDprodukta->setFocus();
    ui->statusbar->showMessage("Nalepka se tiska", 3000);
}

void MainWindow::on_actionO_programu_triggered()
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QIcon icon(":icons/about.ico");
    dialog->setWindowTitle("O programu");
    dialog->setWindowIcon(icon);
    label->setText("Program za shranjevanje in tiskanje termo nalepk, ter Qr kode.\nTiskalnik Rollo.\n\nIzdelal: Nejc Sedovnik\nLeto: April 2020\nVerzija: " + m_verzija);
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->exec();
}

void MainWindow::on_actionNov_napis_triggered()
{
    novaNalepka *ustvariNalepko = new novaNalepka(this);
    ustvariNalepko->setModal(true);
    ustvariNalepko->exec();
}

void MainWindow::on_textEdit_opombe_textChanged()
{
    if(ui->textEdit_opombe->toPlainText().length() > 80)
    {
        QString napis(ui->textEdit_opombe->toPlainText());
        QTextCursor tmpCursor = ui->textEdit_opombe->textCursor();
        napis.chop(1);
        QMessageBox::warning(this, "Napis predolg", "Napis lahko vsebuje največ 80 črk!");
        ui->textEdit_opombe->setPlainText(napis);
        ui->textEdit_opombe->setTextCursor(tmpCursor);
    }
}
