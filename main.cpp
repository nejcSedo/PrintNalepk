#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString fileName("./nalepke.txt");
    QFile file(fileName);
    if(!QFileInfo::exists(fileName))
    {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("DELETE;DELETE\n");
        file.close();
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
