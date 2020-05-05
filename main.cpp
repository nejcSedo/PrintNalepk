#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if(!QFileInfo::exists("./nalepke.txt"))
    {
        QFile file("./nalepke.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("Seznam nalepk:\n");
        file.flush();
        file.close();
    }
    if(!QFileInfo::exists("./release/nalepke.txt"))
    {
        QFile file("./release/nalepke.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("Seznam nalepk:\n");
        file.flush();
        file.close();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
};
