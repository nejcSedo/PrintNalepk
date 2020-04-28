#include "keypress.h"

KeyPress::KeyPress(QWidget *parent) :
    QWidget(parent)
{

}

void KeyPress::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        qDebug() << "You released ESC";
    }
}
