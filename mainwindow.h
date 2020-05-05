#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDate>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include "QrCode.hpp"
#include "novanalepka.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddRoot(QString, QString);
    void Read();
    void Search(QString, QString);
    void drawText(QPainter & painter, qreal x, qreal y, Qt::Alignment flags, const QString & text, QRectF * boundingRect);
    void drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags, const QString & text, QRectF * boundingRect);
    void Nalepka();
    void keyReleaseEvent(QKeyEvent*);
    void ProduktCheck(QString, QString);
    void Error(int);
    void paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg);

private slots:
    void on_pushButton_shraniNalepko_clicked();
    void on_pushButton_natisni_clicked();
    void on_lineEdit_IDprodukta_textChanged(const QString &arg1);
    void on_lineEdit_nazivProdukta_textChanged(const QString &arg1);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_actionIzhod_triggered();
    void on_actionDelete_triggered();
    void on_actionShrani_nalepko_triggered();
    void on_actionPrint_triggered();
    void on_actionO_programu_triggered();
    void on_actionNov_napis_triggered();

private:
    Ui::MainWindow *ui;
    bool m_count;
    bool m_isClicked;
    QString m_id;
    QString m_naziv;
    QString m_verzija;
    QLabel *m_verzijaLabel;
    int m_nalepkaCentimeter;
    int m_qrVelikost;
};
#endif // MAINWINDOW_H
