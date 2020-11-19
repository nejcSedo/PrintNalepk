#include "mainwindow.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

// FUNCTIONS
void CheckFiles();
void MakeFile(const QString&);

// MAIN
int main(int argc, char *argv[])
{
    QApplication  app(argc, argv);
    MainWindow window;

    // Preveri ce obstajajo vse datoteke
    CheckFiles();

    // Iz serverja potegne najnovejse datoteke
    QNetworkAccessManager man;
    QNetworkRequest reqProcesi(QUrl("http://elra.chickenkiller.com/Proces/proizvodniproces.txt"));
    QString ua("HttpRequestDemo/0.1 (Win64) Qt/5.7");
    reqProcesi.setHeader(QNetworkRequest::UserAgentHeader, QVariant(ua));
    QNetworkReply* replyProcesi = man.get(reqProcesi);
    QObject::connect(replyProcesi, &QNetworkReply::finished, [&]() {
        QByteArray read = replyProcesi->readAll();

        QFile file("proizvodniproces.txt");
        if(!file.open(QFile::WriteOnly | QFile::Truncate))
            qDebug() << "Errorrrrrrr";

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << read << "\n";
        file.flush();
        file.close();

        replyProcesi->close();
        replyProcesi->deleteLater();
    });

    QNetworkRequest reqBarve(QUrl("http://elra.chickenkiller.com/Proces/barve.txt"));
    reqBarve.setHeader(QNetworkRequest::UserAgentHeader, QVariant(ua));
    QNetworkReply* replyBarve = man.get(reqBarve);
    QObject::connect(replyBarve, &QNetworkReply::finished, [&]() {
        QByteArray read = replyBarve->readAll();

        QFile file("barve.txt");
        if(!file.open(QFile::WriteOnly | QFile::Truncate))
            qDebug() << "Errorrrrrrr";

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << read << "\n";
        file.flush();
        file.close();

        replyBarve->close();
        replyBarve->deleteLater();
    });

    QNetworkRequest reqOrodja(QUrl("http://elra.chickenkiller.com/Proces/orodja.txt"));
    reqOrodja.setHeader(QNetworkRequest::UserAgentHeader, QVariant(ua));
    QNetworkReply* replyOrodja = man.get(reqOrodja);
    QObject::connect(replyOrodja, &QNetworkReply::finished, [&]() {
        QByteArray read = replyOrodja->readAll();

        QFile file("orodja.txt");
        if(!file.open(QFile::WriteOnly | QFile::Truncate))
            qDebug() << "Errorrrrrrr";

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << read << "\n";
        file.flush();
        file.close();

        replyOrodja->close();
        replyOrodja->deleteLater();
    });

    window.show();
    return app.exec();
};

// DECLARATION
void CheckFiles()
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
}

void MakeFile(const QString& fileName)
{
    QFile file("./" + fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.flush();
    file.close();
}
