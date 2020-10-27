#include "mainwindow.h"

#include <QApplication>

// FUNCTIONS
void MakeFile(const QString&);

// MAIN
int main(int argc, char *argv[])
{
    QDir dir("./nalepke");
    if (!dir.exists())
        dir.mkpath(".");

    QString fileName("nalepke.txt");
    if(!QFileInfo::exists("./" + fileName))
        MakeFile(fileName);

    if(!QFileInfo::exists("./release/" + fileName)) {
        fileName = "release/" + fileName;
        MakeFile(fileName);
    }

    QString fileNameProces("proizvodniproces.txt");
    if(!QFileInfo::exists("./" + fileNameProces))
        MakeFile(fileNameProces);

    if(!QFileInfo::exists("./release/" + fileNameProces)) {
        fileNameProces = "release/" + fileNameProces;
        MakeFile(fileNameProces);
    }

    QString fileNameOrodje("orodja.txt");
    if(!QFileInfo::exists("./" + fileNameOrodje))
        MakeFile(fileNameOrodje);

    if(!QFileInfo::exists("./release/" + fileNameOrodje)) {
        fileNameOrodje = "release/" + fileNameOrodje;
        MakeFile(fileNameOrodje);
    }

    QString fileNameBarve("barve.txt");
    if(!QFileInfo::exists("./" + fileNameBarve))
        MakeFile(fileNameBarve);

    if(!QFileInfo::exists("./release/" + fileNameBarve)) {
        fileNameBarve = "release/" + fileNameBarve;
        MakeFile(fileNameBarve);
    }


    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
};

// DECLARATION
void MakeFile(const QString& fileName)
{
    QFile file("./" + fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.flush();
    file.close();
}
