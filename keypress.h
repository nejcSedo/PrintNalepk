#ifndef KEYPRESS_H
#define KEYPRESS_H

#include <QWidget>
#include <QKeyEvent>
#include <QDebug>

class KeyPress : public QWidget
{
    Q_OBJECT
    public:
        KeyPress(QWidget *parent = nullptr);

    protected:
        void keyReleaseEvent(QKeyEvent *);
};

#endif // KEYPRESS_H
