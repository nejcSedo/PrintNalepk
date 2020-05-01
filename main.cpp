#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString fileName("./nalepke.txt");
    QString fileNameRelease("./release/nalepke.txt");
    QFile file(fileName);
    QFile fileRelease(fileNameRelease);

    if(!QFileInfo::exists(fileName))
    {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("Seznam nalepk:\n");
        file.close();
    }

    if(!QFileInfo::exists(fileNameRelease))
    {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("Seznam nalepk:\n");
        file.close();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
};
