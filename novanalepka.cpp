#include "novanalepka.h"
#include "ui_novanalepka.h"

novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka), m_nalepkaCentimeter(75), m_qrVelikost(m_nalepkaCentimeter*1.33)
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->lineEdit_napis->setMaxLength(2500);
    ui->pushButton_natisni->setDisabled(true);
    ui->comboBox_seznamNalepk->addItem("11x8");
    ui->comboBox_seznamNalepk->addItem("10x4");
    ui->comboBox_seznamNalepk->setDisabled(true);
    ui->lineEdit_napis->setFocus();
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

void novaNalepka::on_lineEdit_napis_textChanged(const QString &arg1)
{
    if(arg1 == "")
        ui->pushButton_natisni->setDisabled(true);
    else
        ui->pushButton_natisni->setDisabled(false);

    QPixmap map(200,200);
    QPainter painter(&map);
    paintQR(painter, QSize(200,200), arg1, QColor("black"));
    ui->label_qrcode->setPixmap(map);
}

void novaNalepka::on_pushButton_natisni_clicked()
{
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A7);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins (1,1,1,1,QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setOutputFormat(QPrinter::NativeFormat);
    int sirinaNalepke(11);
    int visinaNalepke(8);
    if(ui->comboBox_seznamNalepk->currentIndex() == 1)
    {
        sirinaNalepke = 10;
        visinaNalepke = 4;
    }

    QTextDocument nalepka;
    QPainter painter(printer);
    painter.drawRect(m_nalepkaCentimeter/6,m_nalepkaCentimeter/6,sirinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/8*2),visinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/5*2));

    painter.setFont(QFont("Tahoma",9));
    if(ui->comboBox_seznamNalepk->currentIndex() == 0)
    {
        const QPointF ptHeader(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
        drawText(painter, ptHeader, Qt::AlignVCenter | Qt::AlignHCenter, "Elra   Seti   d.o.o.,   Andraž   nad   Polzelo   74/a,   3313   Polzela");
    }
    else if(ui->comboBox_seznamNalepk->currentIndex() == 1)
    {
        const QPointF ptHeader(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter)),qreal(m_nalepkaCentimeter/2));
        drawText(painter, ptHeader, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");
    }


    painter.setFont(QFont("Tahoma",35));
    if(ui->comboBox_seznamNalepk->currentIndex() == 0)
    {
        const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
        if(ui->lineEdit_napis->text().length() > 14)
            drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "PREBERI QR");
        else
            drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper());
    }
    else if(ui->comboBox_seznamNalepk->currentIndex() == 1)
    {
        const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter*2)),qreal(m_nalepkaCentimeter*2));
        if(ui->lineEdit_napis->text().length() > 14)
            drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "PREBERI QR");
        else
            drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper());

    }

    QPixmap map(m_qrVelikost,m_qrVelikost);
    QPainter painterImage(&map);
    paintQR(painterImage,QSize(m_qrVelikost,m_qrVelikost),ui->lineEdit_napis->text(), QColor("black"));

    if(ui->comboBox_seznamNalepk->currentIndex() == 0)
    {
        painter.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map);
    }
    else if(ui->comboBox_seznamNalepk->currentIndex() == 1)
    {
        painter.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter))-m_qrVelikost,m_nalepkaCentimeter,m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map);
    }

    QPrintDialog printDialog(printer, this);

    if (printDialog.exec() == QDialog::Accepted)
        nalepka.print(printer);

    painter.end();
    ui->lineEdit_napis->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->lineEdit_napis->setFocus();

}
