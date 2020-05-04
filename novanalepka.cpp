#include "novanalepka.h"
#include "ui_novanalepka.h"

novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka)
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->lineEdit_napis->setMaxLength(2500);
    ui->pushButton_natisni->setDisabled(true);
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

    QPainter painter(printer);
    const QPointF pt(qreal(820/2),qreal(200));
    painter.drawRect(5,5,820,570);
    painter.setFont(QFont("Tahoma",9));
    painter.drawText(50,80,"Elra   Seti   d.o.o.,   Andraž   nad   Polzelo   74/a,   3313   Polzela");
    painter.setFont(QFont("Tahoma",35));

    if(ui->lineEdit_napis->text().length() > 14)
        drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, "PREBERI QR");
    else
        drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper());

    QPixmap map(100,100);
    QPainter painterImage(&map);
    paintQR(painterImage,QSize(100,100),ui->lineEdit_napis->text(), QColor("black"));
    map.save("image.png");
    painter.drawPixmap(270,270,300,300, map);
    painter.end();
    QTextDocument nalepka;
    QPrintDialog printDialog(printer, this);

    if (printDialog.exec() == QDialog::Accepted)
    {
        nalepka.print(printer);
    }

    ui->lineEdit_napis->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->lineEdit_napis->setFocus();
    }
