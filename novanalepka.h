#ifndef novaNalepka_H
#define novaNalepka_H

#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QDate>
#include <QKeyEvent>
#include <QLabel>
#include "QrCode.hpp"
#include "methods.h"

QT_BEGIN_NAMESPACE
namespace Ui { class novaNalepka; }
QT_END_NAMESPACE

class novaNalepka : public QDialog
{
    Q_OBJECT

public:
    explicit novaNalepka(QWidget *parent = nullptr);
    ~novaNalepka();
    void Reset();

private slots:
    void on_pushButton_natisni_clicked();
    void on_textEdit_qrNapis_textChanged();
    void on_lineEdit_napis_textChanged(QString arg1);

private:
    Ui::novaNalepka *ui;
    Methods* m_methods;
    QPixmap m_map;
    QPixmap m_mapSaved;
    QString m_napis;
};

#endif // novaNalepka_H
