#include "methods.h"

// KONSTRUKTOR
Methods::Methods(QWidget *parent) :
    QMainWindow(parent),
    m_nalepkaText(""),
    m_alignment(Qt::AlignVCenter | Qt::AlignHCenter),
    m_nalepkaCentimeter(10),
    m_qrVelikost(m_nalepkaCentimeter * 10),
    m_sirinaNalepke(90),
    m_visinaNalepke(70),
    m_ofSet_Header(m_nalepkaCentimeter / 3),
    m_ofSet_ListIzdelka_FirstLine(m_nalepkaCentimeter * 0.8),
    m_ofSet_Id_OneLine(m_nalepkaCentimeter * 1.4),
    m_ofSet_Naziv_SecondLine(m_nalepkaCentimeter * 1.9),
    m_ofSet_Kolicina(m_nalepkaCentimeter * 2.4),
    m_XofSet((m_sirinaNalepke - m_nalepkaCentimeter) / 2),
    m_ofSet_PDF(52),
    m_headerText("Elra Seti d.o.o., Andraž nad Polzelo 74/a, 3313 Polzela"),
    m_bigFontStandard("Tahoma",20),
    m_mediumFontStandard("Tahoma",11),
    m_smallFontHeader("Tahoma",9),
    m_bigFontNapis("Tahoma",50),
    m_mediumFontNapis("Tahoma",35),
    m_printerSelected("Rollo Printer"),
    m_printerList(QPrinterInfo::availablePrinters())
{

}

// DESTRUKTOR
Methods::~Methods()
{

}

// VIRTUAL ADDROOT
void Methods::AddRootToTreeWidget(const QString &, const QString &, QTreeWidgetItem *)
{

}

// VIRTUAL RESET
void Methods::Reset()
{

}

// VIRTUAL KEYRELESEEVENT
void Methods::keyReleaseEvent(QKeyEvent*)
{

}

// POZICIONIRANJE TEXTA NA SREDINO NALEPKE
void Methods::drawText(QPainter& painter, QPointF& point,
                       Qt::Alignment& flags, const QString& text,
                       QRectF* boundingRect)
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
    char *str = qrText.toUtf8().data();
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::HIGH);
    int sizeOf = qr.getInt();
    const int s = sizeOf > 0 ? sizeOf : 1;
    const double w = QSize(m_qrVelikost, m_qrVelikost).width();
    const double h = QSize(m_qrVelikost, m_qrVelikost).height();
    const double aspect = w / h;
    const double size = ((aspect > 1.0) ? h : w);
    const double scale = size / (s + 2);

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
            if(0x0 != color)
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
    printer.setPageSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(m_visinaNalepke, m_sirinaNalepke),QPrinter::Millimeter);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(false);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPrinterName(m_printerSelected);
    printer.setNumCopies(numCopies);

    // QR SLIKA
    QPixmap map(m_qrVelikost, m_qrVelikost);
    drawQr(map, id, naziv, opombe);

    // SHRANJEVANJE V PDF
    m_nalepkaText = QDate::currentDate().toString("d_M_yyyy") + "-" + id + ".pdf";
    const QString nalepkaFile("./nalepke/" + m_nalepkaText);
    const QPageSize pageSize(QSizeF(m_visinaNalepke, m_sirinaNalepke), QPageSize::Millimeter);
    const QPageLayout pageLayout(pageSize, QPageLayout::Landscape,QMargins(0, 0, 0, 0));

    QPdfWriter nalepkaPdf(nalepkaFile);
    nalepkaPdf.setPageSize(pageSize);
    nalepkaPdf.setPageLayout(pageLayout);

    // PAINTER
    QPainter painterTextNalepkePdf(&nalepkaPdf);
    QPainter painterTextNalepkePrint(&printer);

    // PDF okvir in Qr
    painterTextNalepkePdf.drawRect((m_nalepkaCentimeter * (m_ofSet_PDF / 2)) / 2,
                                   (m_nalepkaCentimeter * m_ofSet_PDF) / 2,
                                   (m_sirinaNalepke * m_ofSet_PDF) - ((m_nalepkaCentimeter * 1.5) * m_ofSet_PDF),
                                   (m_visinaNalepke * m_ofSet_PDF) - ((m_nalepkaCentimeter * 1.5) * m_ofSet_PDF));

    painterTextNalepkePdf.drawPixmap((((m_sirinaNalepke * m_ofSet_PDF) - (m_nalepkaCentimeter * m_ofSet_PDF * 4)) / 2),
                                      ((m_visinaNalepke * m_ofSet_PDF) - (m_nalepkaCentimeter * m_ofSet_PDF * 1.5) / 2),
                                       (m_nalepkaCentimeter * 3) * m_ofSet_PDF,
                                       (m_nalepkaCentimeter * 3) * m_ofSet_PDF, map);

    // PRINT okvir in Qr
    painterTextNalepkePrint.drawRect(m_nalepkaCentimeter / 2,
                                     m_nalepkaCentimeter / 2,
                                     m_sirinaNalepke - (m_nalepkaCentimeter * 2),
                                     m_visinaNalepke - (m_nalepkaCentimeter * 2));

    painterTextNalepkePrint.drawPixmap(((m_sirinaNalepke - (m_nalepkaCentimeter * 4)) / 2),
                                         m_visinaNalepke - (m_qrVelikost + (m_nalepkaCentimeter / 2)),
                                         m_nalepkaCentimeter * 3,
                                         m_nalepkaCentimeter * 3, map);

    QRectF* boundingRect = {0};
    QPointF textPos;
    qreal position(m_XofSet * m_ofSet_PDF);

    // STANDARD NACIN (KO GRE ZA NALEPKO Z ID, NAZIVOM, KOLICINO IN OPOMBO
    if(nacin == NacinTiska::Standard) {
        // PDF id, naziv, kolicina
        painterTextNalepkePdf.setFont(m_smallFontHeader);
        textPos.setX(position);
        position = m_ofSet_Header * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, m_headerText, boundingRect);

        painterTextNalepkePdf.setFont(m_bigFontStandard);
        position = m_ofSet_ListIzdelka_FirstLine * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePdf.setFont(m_mediumFontStandard);
        position = m_ofSet_Id_OneLine * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, "ID izdelka: " + id, boundingRect);

        position = m_ofSet_Naziv_SecondLine * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        position = m_ofSet_Kolicina * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, "Količina: " + kolicina, boundingRect);

        // PRINT id, naziv, kolicina

        painterTextNalepkePrint.setFont(m_smallFontHeader);
        position = m_XofSet;
        textPos.setX(position);
        position = m_ofSet_Header;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, m_headerText, boundingRect);

        painterTextNalepkePrint.setFont(m_bigFontStandard);
        position = m_ofSet_ListIzdelka_FirstLine;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, "LIST IZDELKA", boundingRect);

        painterTextNalepkePrint.setFont(m_mediumFontStandard);
        position = m_ofSet_Id_OneLine;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, "ID izdelka: " + id, boundingRect);

        position = m_ofSet_Naziv_SecondLine;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, "Naziv izdelka: " + naziv, boundingRect);

        position = m_ofSet_Kolicina;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, "Količina: " + kolicina, boundingRect);
    }
    // NAPIS (KO GRE SAMO ZA NAPIS ALI QR KODO
    else if(nacin == NacinTiska::Napis) {
        // PDF qr napis ali kratek text
        position = m_XofSet * m_ofSet_PDF;
        textPos.setX(position);
        painterTextNalepkePdf.setFont(m_smallFontHeader);
        position = m_ofSet_Header * m_ofSet_PDF;
        textPos.setY(position);
        drawText(painterTextNalepkePdf, textPos, m_alignment, m_headerText, boundingRect);

        QStringList list = naziv.split(";", QString::SkipEmptyParts);

        switch(list.length())
        {
            case 1: { painterTextNalepkePdf.setFont(m_bigFontNapis);
                      position = m_ofSet_Id_OneLine * m_ofSet_PDF;
                      textPos.setY(position);
                      drawText(painterTextNalepkePdf, textPos, m_alignment, list.at(0), boundingRect);
                      break; }
            case 2: { painterTextNalepkePdf.setFont(m_mediumFontNapis);
                      position = m_ofSet_ListIzdelka_FirstLine * m_ofSet_PDF;
                      textPos.setY(position);
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);

                      position = m_ofSet_Naziv_SecondLine * m_ofSet_PDF;
                      textPos.setY(position);
                      drawText(painterTextNalepkePdf, textPos, m_alignment, list.at(1), boundingRect);
                      break; }
            default: {painterTextNalepkePdf.setFont(m_bigFontNapis);
                      position = m_ofSet_Id_OneLine * m_ofSet_PDF;
                      textPos.setY(position);
                      drawText(painterTextNalepkePdf, textPos, m_alignment, "QR KODA", boundingRect);
                      break; }
         }

        // PRINT qr napis ali kratek text
        painterTextNalepkePrint.setFont(m_smallFontHeader);
        textPos.setX(m_XofSet);
        position = m_ofSet_Header;
        textPos.setY(position);
        drawText(painterTextNalepkePrint, textPos, m_alignment, m_headerText, boundingRect);

        switch(list.length())
        {
            case 1: { painterTextNalepkePrint.setFont(m_bigFontNapis);
                      position = m_ofSet_Id_OneLine;
                      textPos.setY(position);
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);
                      break; }
            case 2: { painterTextNalepkePrint.setFont(m_mediumFontNapis);
                      position = m_ofSet_ListIzdelka_FirstLine;
                      textPos.setY(position);
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(0), boundingRect);

                      position = m_ofSet_Naziv_SecondLine;
                      textPos.setY(position);
                      drawText(painterTextNalepkePrint, textPos, m_alignment, list.at(1), boundingRect);
                      break; }
            default: {painterTextNalepkePrint.setFont(m_bigFontNapis);
                      position = m_ofSet_Id_OneLine;
                      textPos.setY(position);
                      drawText(painterTextNalepkePrint, textPos, m_alignment, "QR KODA", boundingRect);
                      break; }
         }
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

// PRINTERS
void Methods::AllPrinters()
{
    foreach (QPrinterInfo printerInfo, m_printerList)
        printerInfo.printerName();
}
