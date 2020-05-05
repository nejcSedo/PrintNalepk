#include "novanalepka.h"
#include "ui_novanalepka.h"

novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka), m_nalepkaCentimeter(75), m_qrVelikost(m_nalepkaCentimeter*1.33), m_napis("")
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->pushButton_natisni->setDisabled(true);
    ui->comboBox_seznamNalepk->addItem("11x8");
    ui->comboBox_seznamNalepk->addItem("10x4");
    ui->comboBox_seznamNalepk->setDisabled(true);
    ui->textEdit_napis->setFocus();
}

novaNalepka::~novaNalepka()
{
    delete ui;
}

void novaNalepka::paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg)
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

void novaNalepka::drawText(QPainter & painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect = 0)
{
   const qreal size = 32767.0;
   QPointF corner(x, y - size);
   if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight) corner.rx() -= size;
   if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop) corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   painter.drawText(rect, flags, text, boundingRect);
}

void novaNalepka::drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect = {})
{
   drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}

void novaNalepka::on_pushButton_natisni_clicked()
{
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    QStringList dimenzijeNalepke(ui->comboBox_seznamNalepk->itemText(0).split('x', Qt::SkipEmptyParts));
    short sirinaNalepke(dimenzijeNalepke.at(0).toInt());
    short visinaNalepke(dimenzijeNalepke.at(1).toInt());

    QTextDocument nalepka;
    QPixmap map(m_qrVelikost,m_qrVelikost);
    QPainter painter(&map);
    paintQR(painter, QSize(m_qrVelikost, m_qrVelikost), ui->textEdit_napis->toPlainText(), QColor("black"));
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: painter.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
        case 1: painter.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter))-m_qrVelikost,m_nalepkaCentimeter,m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
        default: painter.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
    }
    map.save("image.png");

    painter.drawRect(m_nalepkaCentimeter/6,m_nalepkaCentimeter/6,sirinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/8*2),visinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/5*2));

    painter.setFont(QFont("Tahoma",9));
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: {const QPointF ptHeader(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
                drawText(painter, ptHeader, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
        case 1: {const QPointF ptHeader1(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter)),qreal(m_nalepkaCentimeter/2));
                drawText(painter, ptHeader1, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
        default:{const QPointF ptHeaderDefault(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
                drawText(painter, ptHeaderDefault, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
    }

    painter.setFont(QFont("Tahoma",35));
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: {const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
                m_napis.length() > 14 ? drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA") : drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, m_napis.toUpper());}
                break;
        case 1: {const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter*2)),qreal(m_nalepkaCentimeter*2));
                m_napis.length() > 14 ? drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA") : drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, m_napis.toUpper());}
                break;
        default:{const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
                m_napis.length() > 14 ? drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA") : drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, m_napis.toUpper());}
                break;
    }

    printer->setPageSize(QPrinter::A7);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins (1,1,1,1,QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setOutputFormat(QPrinter::NativeFormat);
    QPrintDialog printDialog(printer, this);
    if (printDialog.exec() == QDialog::Accepted)
        nalepka.print(printer);

    painter.end();
    ui->textEdit_napis->clear();
    ui->label_qrcode->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->textEdit_napis->setFocus();

}

void novaNalepka::on_textEdit_napis_textChanged()
{
    if(ui->textEdit_napis->toPlainText() == "")
    {
        ui->pushButton_natisni->setDisabled(true);
        ui->label_qrcode->clear();
    }
    else if(ui->textEdit_napis->toPlainText().length() > 100)
    {
        QString napis(ui->textEdit_napis->toPlainText());
        QTextCursor tmpCursor = ui->textEdit_napis->textCursor();
        napis.chop(1);
        QMessageBox::warning(this, "Napis predolg", "Napis lahko vsebuje največ 100 črk!");
        ui->textEdit_napis->setPlainText(napis);
        ui->textEdit_napis->setTextCursor(tmpCursor);
    }
    else
    {
        ui->pushButton_natisni->setDisabled(false);
        QPixmap map(300,300);
        QPainter painter(&map);
        paintQR(painter, QSize(300,300), ui->textEdit_napis->toPlainText(), QColor("black"));
        ui->label_qrcode->setPixmap(map);
    }
}
