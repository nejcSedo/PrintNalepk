#include "methods.h"

// KONSTRUKTOR
Methods::Methods(QWidget *parent) : QMainWindow(parent), m_nalepkaText(""), m_nalepkaCentimeter(83),
    m_qrVelikost(m_nalepkaCentimeter*5), m_sirinaNalepke(10), m_visinaNalepke(8) {

}

// DESTRUKTOR
Methods::~Methods() {
}

// VIRTUALNI RESET
void Methods::Reset() {

}

// POZICIONIRANJE TEXTA NA SREDINO NALEPKE
void Methods::drawText(QPainter& painter,  const QPointF& point, Qt::Alignment flags, const QString& text, QRectF* boundingRect = 0) {
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
void Methods::drawQr(QPixmap& map, QString& id, QString& naziv, QString& opombe, bool print) {
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

    // PAINTER
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
    for(int y=0; y<5000; y++)
    {
        for(int x=0; x<5000; x++)
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

    print ? map.save("qrNalepkePrint.png") : map.save("qrNalepkePdf.png");

    painterQr.end();
}

// PRINT METODA
void Methods::NalepkaPrint(QString id, QString naziv, QString kolicina, QString opombe, int numCopies, NacinTiska nacin) {
    // PRINTER SETUP
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A7);
    printer->setOrientation(QPrinter::Landscape);
    printer->setPageMargins(-59,-24,0,0,QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setNumCopies(numCopies);

    // QR SLIKA
    QPixmap map(m_qrVelikost,m_qrVelikost);

    drawQr(map, id, naziv, opombe, false);
    drawQr(map, id, naziv, opombe, true);

    QRectF* boundingRect = {};

    // SHRANJEVANJE V PDF
    m_nalepkaText = id + "-" + QDate::currentDate().toString("d_M_yyyy") + ".pdf";
    const QString nalepkaFile("./nalepke/" + m_nalepkaText);
    const QPageSize pageSize(QPageSize::A7);
    const QPageLayout pageLayout(pageSize, QPageLayout::Landscape,QMargins(0,0,0,0));

    QPdfWriter nalepkaPdf(nalepkaFile);
    nalepkaPdf.setPageSize(pageSize);
    nalepkaPdf.setPageLayout(pageLayout);

    // PAINTER
    QPainter painterTextNalepkePdf(&nalepkaPdf);
    QPainter painterTextNalepkePrint(printer);

    // PDF
    painterTextNalepkePdf.drawRect(m_nalepkaCentimeter * 3,
                                    m_nalepkaCentimeter * 3,
                                    m_sirinaNalepke*(m_nalepkaCentimeter * 5.6) - m_nalepkaCentimeter * 3,
                                    m_visinaNalepke*(m_nalepkaCentimeter * 5) - m_nalepkaCentimeter * 3);

    painterTextNalepkePdf.drawPixmap(m_sirinaNalepke*m_nalepkaCentimeter * 2.35 - m_qrVelikost,
                                       m_visinaNalepke*m_nalepkaCentimeter * 2.8 - m_qrVelikost,
                                       m_nalepkaCentimeter * 20,
                                       m_nalepkaCentimeter * 20, map);

    // PRINT
    painterTextNalepkePrint.drawRect(m_nalepkaCentimeter / 3,
                                     m_nalepkaCentimeter - (m_nalepkaCentimeter / 7),
                                     m_sirinaNalepke - (m_nalepkaCentimeter / 2),
                                     m_visinaNalepke - (m_nalepkaCentimeter * 2));

    painterTextNalepkePrint.drawPixmap((m_sirinaNalepke / 2) - m_qrVelikost,
                                        m_visinaNalepke - m_qrVelikost,
                                        m_nalepkaCentimeter * 3.5,
                                        m_nalepkaCentimeter * 3.5, map);

    // STANDARD NACIN (KO GRE ZA NALEPKO Z ID, NAZIVOM, KOLICINO IN OPOMBO
    if(nacin == NacinTiska::Standard)  {
        // PDF
        painterTextNalepkePdf.setFont(QFont("Tahoma",9));
        const QPointF headerPositionPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*1.5));
        drawText(painterTextNalepkePdf, headerPositionPdf, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        painterTextNalepkePdf.setFont(QFont("Tahoma",20));
        const QPointF listPositionPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*5));
        drawText(painterTextNalepkePdf, listPositionPdf, Qt::AlignVCenter | Qt::AlignHCenter, "LIST IZDELKA", boundingRect);

        painterTextNalepkePdf.setFont(QFont("Tahoma",11));
        const QPointF idPositionPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*9));
        drawText(painterTextNalepkePdf, idPositionPdf, Qt::AlignVCenter | Qt::AlignHCenter, "ID izdelka: " + id, boundingRect);

        const QPointF nazivPositionPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*13));
        drawText(painterTextNalepkePdf, nazivPositionPdf, Qt::AlignVCenter | Qt::AlignHCenter, "Naziv izdelka: " + naziv, boundingRect);

        const QPointF kolicinaPositionPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*17));
        drawText(painterTextNalepkePdf, kolicinaPositionPdf, Qt::AlignVCenter | Qt::AlignHCenter, "Količina: " + kolicina, boundingRect);

        // PRINT
        const QPointF headerPositionPrint(qreal(m_sirinaNalepke/2), qreal(m_nalepkaCentimeter/1.9));
        painterTextNalepkePrint.setFont(QFont("Tahoma",9));
        drawText(painterTextNalepkePrint, headerPositionPrint, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        const QPointF listPositionPrint(qreal(m_sirinaNalepke/2), qreal(m_nalepkaCentimeter*1.2));
        painterTextNalepkePrint.setFont(QFont("Tahoma",20));
        drawText(painterTextNalepkePrint, listPositionPrint, Qt::AlignVCenter | Qt::AlignHCenter, "LIST IZDELKA", boundingRect);

        painterTextNalepkePrint.setFont(QFont("Tahoma",11));
        const QPointF idPositionPrint(qreal(m_sirinaNalepke/2), qreal(m_nalepkaCentimeter*2.1));
        drawText(painterTextNalepkePrint, idPositionPrint, Qt::AlignVCenter | Qt::AlignHCenter, "ID izdelka: " + id, boundingRect);

        const QPointF nazivPositionPrint(qreal(m_sirinaNalepke/2), qreal(m_nalepkaCentimeter*2.6));
        drawText(painterTextNalepkePrint, nazivPositionPrint, Qt::AlignVCenter | Qt::AlignHCenter, "Naziv izdelka: " + naziv, boundingRect);

        const QPointF kolicinaPositionPrint(qreal(m_sirinaNalepke/2),qreal(m_nalepkaCentimeter*3.1));
        drawText(painterTextNalepkePrint, kolicinaPositionPrint, Qt::AlignVCenter | Qt::AlignHCenter, "Količina: " + kolicina, boundingRect);
    }

    // NAPIS (KO GRE SAMO ZA NAPIS ALI QR KODO
    else if(nacin == NacinTiska::Napis) {
        // PDF
        painterTextNalepkePdf.setFont(QFont("Tahoma",9));
        const QPointF ptHeaderDefaultPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*1.5));
        drawText(painterTextNalepkePdf, ptHeaderDefaultPdf, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        painterTextNalepkePdf.setFont(QFont("Tahoma",35));
        const QPointF napisPdf(qreal(m_sirinaNalepke*(m_nalepkaCentimeter*3)), qreal(m_nalepkaCentimeter*7));
        opombe == "" ? drawText(painterTextNalepkePdf, napisPdf, Qt::AlignVCenter | Qt::AlignHCenter, naziv, boundingRect)
                    : drawText(painterTextNalepkePdf, napisPdf, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA", boundingRect);

        m_nalepkaText = opombe;

        // PRINT
        painterTextNalepkePrint.setFont(QFont("Tahoma",9));
        const QPointF ptHeaderDefault(qreal((m_sirinaNalepke * m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter));
        drawText(painterTextNalepkePrint, ptHeaderDefault, Qt::AlignVCenter | Qt::AlignHCenter, "Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela", boundingRect);

        painterTextNalepkePrint.setFont(QFont("Tahoma",35));
        const QPointF pt(qreal((m_sirinaNalepke * m_nalepkaCentimeter)/2),qreal(m_nalepkaCentimeter*3));
        naziv != "" ? drawText(painterTextNalepkePrint, pt, Qt::AlignVCenter | Qt::AlignHCenter, opombe, boundingRect)
                    : drawText(painterTextNalepkePrint, pt, Qt::AlignVCenter | Qt::AlignHCenter, "QR KODA", boundingRect);
    }

    painterTextNalepkePdf.end();
    painterTextNalepkePrint.end();
}
