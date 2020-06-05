#include "methods.h"

Methods::Methods(QWidget *parent) : QMainWindow(parent)
{

}

void Methods::drawText(QPainter& painter,  const QPointF& point, Qt::Alignment flags, const QString& text, QRectF* boundingRect = 0) {
   const qreal size = 32767.0;
   QPointF corner(point.x(), point.y() - size);
   if (flags & Qt::AlignHCenter)
       corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight)
       corner.rx() -= size;
   if (flags & Qt::AlignVCenter)
       corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop)
       corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   //painter.drawText(rect, flags, text, boundingRect);
}

/*
void Methods::drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect = {}) {
   drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}
*/
