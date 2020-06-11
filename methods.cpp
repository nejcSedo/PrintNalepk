#include "methods.h"
#include <QDebug>

// KONSTRUKTOR
Methods::Methods(QWidget *parent) :
    QMainWindow(parent),
    m_nalepkaText(""),
    m_alignment(Qt::AlignVCenter | Qt::AlignHCenter),
    m_nalepkaCentimeter(100),
    m_qrVelikost(m_nalepkaCentimeter*5),
    m_sirinaNalepke(9),
    m_visinaNalepke(7) {

}

// DESTRUKTOR
Methods::~Methods()
{
}

// VIRTUALNI RESET
void Methods::Reset()
{

}

// VIRTUAL KEYRELESEEVENT
void Methods::keyReleaseEvent(QKeyEvent*)
{

}

// POZICIONIRANJE TEXTA NA SREDINO NALEPKE
void Methods::drawText(QPainter& painter, const QPointF& point,
                       Qt::Alignment& flags, const QString& text,
                       QRectF* boundingRect = 0)
{
   const qreal size(32767.0);
   QPointF corner(point.x(), point.y() - size);
   if (flags & Qt::AlignHCenter)
       corner.rx() -= size / 2.0;
   else if (flags & Qt::AlignRight)
       corner.rx() -= size;
   if (flags & Qt::AlignVCenter)
       corner.ry() += size / 2.0;
   else if (flags & Qt::AlignTop)
       corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   painter.drawText(rect, flags, text, boundingRect);
}

// QR GENERATOR
void Methods::drawQr(QPixmap& map, const QString& id,
                     const QString& naziv, const QString& opombe)
{
    // Qr text
    QChar newLine('\u000A');
    QString qrText("www.elraseti.si");
    qrText += newLine;
    qrText += id + " ; " + naziv + " ; " + QDate::currentDate().toString("d.M.yyyy");
    qrText += newLine;
    qrText += "Opombe: ";
    qrText += opombe;
    if(naziv == "")
        qrText = opombe;
    else if(opombe == "")
        qrText = naziv;
    else {
        qrText = "www.elraseti.si";
        qrText += newLine;
        qrText += id + " ; " + naziv + " ; " + QDate::currentDate().toString("d.M.yyyy");
        qrText += newLine;
        qrText += "Opombe: ";
        qrText += opombe;
    }

    // Painter
    QPainter painterQr(&map);
    char *str=qrText.toUtf8().data();
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::HIGH);
    int sizeOf = qr.getInt();
    const int s=sizeOf>0?sizeOf:1;
    const double w=QSize(m_qrVelikost,m_qrVelikost).width();
    const double h=QSize(m_qrVelikost,m_qrVelikost).height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);

    painterQr.setPen(Qt::NoPen);
    painterQr.setBrush(Qt::white);
    for(int y=0; y<2000; y++)
    {
        for(int x=0; x<2000; x++)
        {
            QRectF r(x, y, 1, 1);
            painterQr.drawRects(&r,1);
        }
    }
    painterQr.setBrush(QColor("black"));
    for(int y=0; y<s; y++)
    {
        for(int x=0; x<s; x++)
        {
            const int color = qr.getModule(x, y);
            if(0x0!=color)
            {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painterQr.drawRects(&r,1);
            }
        }
    }

    map.save("qrNalepkePrint.png");

    painterQr.end();
}

// PRINT METODA
void Methods::NalepkaPrint(const QString& id, const QString& naziv,
                           const QString& kolicina, const QString& opombe,
                           const int& numCopies, const NacinTiska& nacin)
{
    // PRINTER SETUP
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A7);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageMargins(-59,-24,0,0,QPrinter::Millimeter);
    printer.setFullPage(true);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setNumCopies(numCopies);

    // QR SLIKA
    QPixmap map(m_qrVelikost,m_qrVelikost);

    drawQr(map, id, naziv, opombe);

    QRectF* boundingRect = {};

    // SHRANJEVANJE V PDF
    m_nalepkaText = QDate::currentDate().toString("d_M_yyyy") + "-" + id + ".pdf";
    const QString nalepkaFile("./nalepke/" + m_nalepkaText);
    const QPageSize pageSize(QPageSize::A7);
    const QPageLayout pageLayout(pageSize, QPageLayout::Landscape,QMargins(0,0,0,0));

    QPdfWriter nalepkaPdf(nalepkaFile);
    nalepkaPdf.setPageSize(pageSize);
    nalepkaPdf.setPageLayout(pageLayout);

    // PAINTER
    QPainter painterTextNalepkePdf(&nalepkaPdf);
    QPainter painterTextNalepkePrint(&printer);

    // PDF okvir in Qr
    {
    painterTextNalepkePdf.drawRect(m_nalepkaCentimeter,
                                   m_nalepkaCentimeter * 3,
                                   (m_sirinaNalepke * m_nalepkaCentimeter * 5.35) - (m_nalepkaCentimeter),
                                   (m_visinaNalepke * m_nalepkaCentimeter * 4.6) - (m_nalepkaCentimeter));

    painterTextNalepkePdf.drawPixmap(((m_sirinaNalepke * m_nalepkaCentimeter) * 2.2) - m_qrVelikost,
                                     ((m_visinaNalepke * m_nalepkaCentimeter) * 2.7) - m_qrVelikost,
                                       m_nalepkaCentimeter * 20,
                                       m_nalepkaCentimeter * 20, map);
    }
    // PRINT okvir in Qr
    {
    painterTextNalepkePrint.drawRect(m_nalepkaCentimeter/2,
                                     m_nalepkaCentimeter/1.5,
                                     (m_sirinaNalepke * m_nalepkaCentimeter) - (m_nalepkaCentimeter * 1.2),
                                     (m_visinaNalepke * m_nalepkaCentimeter) - (m_nalepkaCentimeter * 1.9));

    painterTextNalepkePrint.drawPixmap(m_sirinaNalepke * m_nalepkaCentimeter - (m_qrVelikost * 1.2),
                                       m_visinaNalepke * m_nalepkaCentimeter - (m_qrVelikost / 1.15),
                                       m_nalepkaCentimeter * 3,
                                       m_nalepkaCentimeter * 3, map);
    }

    // STANDARD NACIN (KO GRE ZA NALEPKO Z ID, NAZIVOM, KOLICINO IN OPOMBO
    if(nacin == NacinTiska::Standard) {
        // PDF id, naziv, kolicina
        painterTextNalepkePdf.setFont(QFont("Tahoma",9));
        const QPointF headerPositionPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 1.5));
        drawText(painterTextNalepkePdf, headerPositionPdf, m_alignment, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        painterTextNalepkePdf.setFont(QFont("Tahoma",20));
        const QPointF listPositionPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 5));
        drawText(painterTextNalepkePdf, listPositionPdf, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePdf.setFont(QFont("Tahoma",11));
        const QPointF idPositionPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 8));
        drawText(painterTextNalepkePdf, idPositionPdf, m_alignment, "ID izdelka: " + id, boundingRect);

        const QPointF nazivPositionPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 10));
        drawText(painterTextNalepkePdf, nazivPositionPdf, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        const QPointF kolicinaPositionPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 12));
        drawText(painterTextNalepkePdf, kolicinaPositionPdf, m_alignment, "Količina: " + kolicina, boundingRect);

        // PRINT id, naziv, kolicina
        const QPointF headerPositionPrint(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2), qreal(m_nalepkaCentimeter / 2.2));
        painterTextNalepkePrint.setFont(QFont("Tahoma",9));
        drawText(painterTextNalepkePrint, headerPositionPrint, m_alignment, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        const QPointF listPositionPrint(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2), qreal(m_nalepkaCentimeter * 1));
        painterTextNalepkePrint.setFont(QFont("Tahoma",20));
        drawText(painterTextNalepkePrint, listPositionPrint, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePrint.setFont(QFont("Tahoma",11));
        const QPointF idPositionPrint(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2), qreal(m_nalepkaCentimeter * 1.5));
        drawText(painterTextNalepkePrint, idPositionPrint, m_alignment, "ID izdelka: " + id, boundingRect);

        const QPointF nazivPositionPrint(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2), qreal(m_nalepkaCentimeter * 2));
        drawText(painterTextNalepkePrint, nazivPositionPrint, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        const QPointF kolicinaPositionPrint(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2),qreal(m_nalepkaCentimeter * 2.5));
        drawText(painterTextNalepkePrint, kolicinaPositionPrint, m_alignment, "Količina: " + kolicina, boundingRect);
    }

    // NAPIS (KO GRE SAMO ZA NAPIS ALI QR KODO
    else if(nacin == NacinTiska::Napis) {
        // PDF qr napis ali kratek text
        painterTextNalepkePdf.setFont(QFont("Tahoma",9));
        const QPointF ptHeaderDefaultPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 1.5));
        drawText(painterTextNalepkePdf, ptHeaderDefaultPdf, m_alignment, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        QStringList list = naziv.split(";", QString::SkipEmptyParts);
        qDebug() << list;

        painterTextNalepkePdf.setFont(QFont("Tahoma",35));
        const QPointF napisPdf(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 5));
        list.at(0) == "" ? drawText(painterTextNalepkePdf, napisPdf, m_alignment, "QR KODA", boundingRect)
                    : drawText(painterTextNalepkePdf, napisPdf, m_alignment, list.at(0), boundingRect);

        const QPointF napisPdf2(qreal((m_sirinaNalepke * m_nalepkaCentimeter * 5.5) / 2), qreal(m_nalepkaCentimeter * 10.5));
        list.at(1) == "" ? drawText(painterTextNalepkePdf, napisPdf2, m_alignment, "", boundingRect)
                         : drawText(painterTextNalepkePdf, napisPdf2, m_alignment, list.at(1), boundingRect);

        // PRINT qr napis ali kratek text
        painterTextNalepkePrint.setFont(QFont("Tahoma",9));
        const QPointF ptHeaderDefault(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2),qreal(m_nalepkaCentimeter / 2.2));
        drawText(painterTextNalepkePrint, ptHeaderDefault, m_alignment, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        painterTextNalepkePrint.setFont(QFont("Tahoma",35));
        const QPointF pt(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2),qreal(m_nalepkaCentimeter * 1.1));

        list.at(0) == "" ? drawText(painterTextNalepkePrint, pt, m_alignment, "QR KODA", boundingRect)
                    : drawText(painterTextNalepkePrint, pt, m_alignment, list.at(0), boundingRect);

        const QPointF pt2(qreal((m_sirinaNalepke*m_nalepkaCentimeter) / 2), qreal(m_nalepkaCentimeter * 2.05));
        list.at(1) == "" ? drawText(painterTextNalepkePrint, pt2, m_alignment, "", boundingRect)
                         : drawText(painterTextNalepkePrint, pt2, m_alignment, list.at(1), boundingRect);
    }

    painterTextNalepkePdf.end();
    painterTextNalepkePrint.end();
}

// ERROR CALL
void Methods::ErrorCall(QFile& fileName, const ErrorType& errorType)
{
    if(!fileName.open(QFile::ReadOnly | QFile::Text)) {
        QDialog *dialog = new QDialog(this);
        QLabel *label = new QLabel(this);
        QHBoxLayout *layout = new QHBoxLayout();
        QIcon icon(":icons/error.ico");
        dialog->setWindowTitle("Napaka");
        dialog->setWindowIcon(icon);
        switch (errorType) {
            case ErrorType::ReadError: label->setText("Error:\nfileName v funkciji Read() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::SearchError: label->setText("Error:\nfileName v funkciji Search() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::ProductCheckError: label->setText("Error:\nfileName v funkciji ProductCheck() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_ReadFile: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_DeleteFileText: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DeleteError_InsertNewText: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::SaveError: label->setText("Error:\nfileName v funkciji on_actionShrani_nalepko_triggered() ne obstaja!\n Znova zaženi program!"); break;
            case ErrorType::DefoultError: label->setText("Error:\nNeznana napaka\n Znova zaženi program!"); break;
            default: label->setText("Neznana napaka!\n Znova zaženi program!"); break;
        }
        layout->addWidget(label);
        dialog->setLayout(layout);
        dialog->exec();
    }
}
