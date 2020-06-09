#ifndef METHODS_H
#define METHODS_H

#include <QMainWindow>
#include <QPainter>
#include <QDate>
#include <QPrinter>
#include <QPdfWriter>
#include "QrCode.hpp"

class Methods : public QMainWindow
{
    friend class novaNalepka;
    Q_OBJECT
public:
    enum class NacinTiska {
        Standard, Napis
    };

public:
    explicit Methods(QWidget *parent = nullptr);
    ~Methods();
    virtual void Reset();
    void drawText(QPainter&, const QPointF&, Qt::Alignment, const QString&, QRectF*);
    void drawQr(QPixmap&, QString&, QString&, QString&, bool);
    void NalepkaPrint(QString, QString, QString, QString, int, NacinTiska);

signals:

protected:
    QString m_nalepkaText;
    short m_nalepkaCentimeter;
    short m_qrVelikost;
    short m_sirinaNalepke;
    short m_visinaNalepke;
};

#endif // METHODS_H
