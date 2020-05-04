#ifndef NOVANALEPKA_H
#define NOVANALEPKA_H

#include <QDialog>
#include <QPainter>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDesktopServices>
#include <QDir>
#include <QSizeF>
#include "QrCode.hpp"

namespace Ui {
class novaNalepka;
}

class novaNalepka : public QDialog
{
    Q_OBJECT

public:
    explicit novaNalepka(QWidget *parent = nullptr);
    ~novaNalepka();
    void paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg);
    void drawText(QPainter & painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect);
    void drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect);

private slots:
    void on_lineEdit_napis_textChanged(const QString &arg1);
    void on_pushButton_natisni_clicked();

private:
    Ui::novaNalepka *ui;
    int m_nalepkaCentimeter;
};

#endif // NOVANALEPKA_H
