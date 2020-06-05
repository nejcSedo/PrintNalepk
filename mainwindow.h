#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "novanalepka.h"
#include "methods.h"

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
    void drawText(QPainter &, qreal, qreal, Qt::Alignment, const QString &, QRectF *);
    void drawText(QPainter &, const QPointF &, Qt::Alignment, const QString &, QRectF *);
    void drawQr(QPixmap &, QString &, QString &, bool);
    void Nalepka();
    void keyReleaseEvent(QKeyEvent*);
    void ProduktCheck(QString, QString);
    void Error(short);
    void paintQR(QPainter &, const QSize, const QString &, QColor);

private slots:
    void on_pushButton_shraniNalepko_clicked();
    void on_pushButton_natisni_clicked();
    void on_lineEdit_IDprodukta_textChanged(const QString &);
    void on_lineEdit_nazivProdukta_textChanged(const QString &);
    void on_treeWidget_itemClicked(QTreeWidgetItem *, int);
    void on_treeWidget_customContextMenuRequested(const QPoint &);
    void on_actionIzhod_triggered();
    void on_actionDelete_triggered();
    void on_actionShrani_nalepko_triggered();
    void on_actionPrint_triggered();
    void on_actionO_programu_triggered();
    void on_actionNov_napis_triggered();
    void on_textEdit_opombe_textChanged();

private:
    Ui::MainWindow *ui;
    Methods* m_methods;
    bool m_count;
    bool m_isClicked;
    QString m_id;
    QString m_naziv;
    QString m_verzija;
    QLabel *m_verzijaLabel;
    int m_nalepkaCentimeterPrint;
    int m_nalepkaCentimeterPdf;
    int m_qrVelikostPrint;
    int m_qrVelikostPdf;
};
#endif // MAINWINDOW_H
