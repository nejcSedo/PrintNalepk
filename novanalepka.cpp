#include "novanalepka.h"
#include "ui_novanalepka.h"
#include <QDebug>

novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka), m_nalepkaCentimeter(75), m_qrVelikost(m_nalepkaCentimeter*1.33)
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->pushButton_natisni->setDisabled(true);
    ui->comboBox_seznamNalepk->addItem("11x8");
    ui->comboBox_seznamNalepk->addItem("10x4");
    ui->comboBox_seznamNalepk->setDisabled(true);
    ui->lineEdit_napis->setMaxLength(14);
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

void novaNalepka::on_pushButton_natisni_clicked()
{
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A7);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins (1,1,1,1,QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setOutputFormat(QPrinter::NativeFormat);

    QStringList dimenzijeNalepke(ui->comboBox_seznamNalepk->itemText(0).split('x', Qt::SkipEmptyParts));
    short sirinaNalepke(dimenzijeNalepke.at(0).toInt());
    short visinaNalepke(dimenzijeNalepke.at(1).toInt());

    QTextDocument nalepka;
    QPixmap map(m_qrVelikost,m_qrVelikost);
    QPainter painter(&map);

    QString napis("");
    ui->lineEdit_napis->text() = "" ? napis = "NI PODATKA" : napis = ui->lineEdit_napis->text();
    ui->textEdit_qrNapis->toPlainText() == "" ? paintQR(painter, QSize(m_qrVelikost, m_qrVelikost), napis, QColor("black")) :
                                                paintQR(painter, QSize(m_qrVelikost, m_qrVelikost), ui->textEdit_qrNapis->toPlainText(), QColor("black")) ;
    painter.end();

    QPainter painterText(printer);
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: painterText.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),
                                   ((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),
                                   m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
        case 1: painterText.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter))-m_qrVelikost,m_nalepkaCentimeter,
                                   m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
        default: painterText.drawPixmap(((sirinaNalepke*m_nalepkaCentimeter)/2)-(m_qrVelikost*1.25),
                                    ((visinaNalepke*m_nalepkaCentimeter))-(m_qrVelikost+(m_nalepkaCentimeter*2.7)),
                                    m_nalepkaCentimeter*3.5,m_nalepkaCentimeter*3.5, map); break;
    }
    map.save("image.png");

    painterText.drawRect(m_nalepkaCentimeter/4,
                     m_nalepkaCentimeter/4,
                     sirinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/5*4),
                     visinaNalepke*m_nalepkaCentimeter-(m_nalepkaCentimeter/5*4));

    painterText.setFont(QFont("Tahoma",9));
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: {const QPointF ptHeader(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
                drawText(painterText, ptHeader, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
        case 1: {const QPointF ptHeader1(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter)),qreal(m_nalepkaCentimeter/2));
                drawText(painterText, ptHeader1, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
        default:{const QPointF ptHeaderDefault(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
                drawText(painterText, ptHeaderDefault, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela");}
                break;
    }

    painterText.setFont(QFont("Tahoma",35));
    switch(ui->comboBox_seznamNalepk->currentIndex())
    {
        case 0: {const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
                ui->lineEdit_napis->text() != "" ? drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper()) : drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA");}
                break;
        case 1: {const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2 - (m_nalepkaCentimeter*2)),qreal(m_nalepkaCentimeter*2));
                ui->lineEdit_napis->text() != "" ? drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper()) : drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA");}
                break;
        default:{const QPointF pt(qreal((sirinaNalepke*m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
                ui->lineEdit_napis->text() != "" ? drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, ui->lineEdit_napis->text().toUpper()) : drawText(painterText, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA");}
                break;
    }

    painterText.end();

    ui->lineEdit_napis->clear();
    ui->textEdit_qrNapis->clear();
    ui->label_qrcode->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->textEdit_qrNapis->setFocus();

}

void novaNalepka::on_textEdit_qrNapis_textChanged()
{
    if(ui->textEdit_qrNapis->toPlainText() == "")
    {
        ui->pushButton_natisni->setDisabled(true);
        ui->label_qrcode->clear();
    }
    else if(ui->textEdit_qrNapis->toPlainText().length() > 100)
    {
        QString napis(ui->textEdit_qrNapis->toPlainText());
        QTextCursor tmpCursor = ui->textEdit_qrNapis->textCursor();
        napis.chop(1);
        QMessageBox::warning(this, "Qr koda predolga", "Qr koda lahko vsebuje največ 100 črk!");
        ui->textEdit_qrNapis->setPlainText(napis);
        ui->textEdit_qrNapis->setTextCursor(tmpCursor);
    }
    else
    {
        ui->pushButton_natisni->setDisabled(false);
        QPixmap map(300,300);
        QPainter painter(&map);
        paintQR(painter, QSize(300,300), ui->textEdit_qrNapis->toPlainText(), QColor("black"));
        ui->label_qrcode->setPixmap(map);
    }
}

void novaNalepka::on_lineEdit_napis_textChanged(const QString &arg1)
{
    if(arg1 == "")
    {
        ui->pushButton_natisni->setDisabled(true);
        ui->label_qrcode->clear();
    }
    else if(arg1.length() > 14)
    {
        QString napis(arg1);
        napis.chop(1);
        QMessageBox::warning(this, "Napis predolg", "Napis lahko vsebuje največ 14 črk!");
        ui->lineEdit_napis->setText(napis);
    }
    else
    {
        ui->pushButton_natisni->setDisabled(false);
        QPixmap map(300,300);
        QPainter painter(&map);
        paintQR(painter, QSize(300,300), arg1, QColor("black"));
        ui->label_qrcode->setPixmap(map);
    }
}
