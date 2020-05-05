#ifndef NOVANALEPKA_H
#define NOVANALEPKA_H

#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QDate>
#include <QKeyEvent>
#include <QLabel>
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
    void paintQR(QPainter &, const QSize, const QString &, QColor);
    void drawText(QPainter &, qreal, qreal, Qt::Alignment, const QString &, QRectF *);
    void drawText(QPainter &, const QPointF &, Qt::Alignment, const QString &, QRectF *);

private slots:
    void on_pushButton_natisni_clicked();
    void on_textEdit_napis_textChanged();

private:
    Ui::novaNalepka *ui;
    int m_nalepkaCentimeter;
    int m_qrVelikost;
    QString m_napis;
};

#endif // NOVANALEPKA_H
