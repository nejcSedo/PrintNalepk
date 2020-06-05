#ifndef METHODS_H
#define METHODS_H

#include <QMainWindow>

class Methods : public QMainWindow
{
    Q_OBJECT
public:
    explicit Methods(QWidget *parent = nullptr);
    void drawText(QPainter&, const QPointF&, Qt::Alignment, const QString&, QRectF*);
    //void drawText(QPainter&, qreal, qreal, Qt::Alignment, const QString&, QRectF*);
    //void drawText(QPainter&, const QPointF&, Qt::Alignment, const QString&, QRectF*);

signals:

};

#endif // METHODS_H
