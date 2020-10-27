#ifndef PROIZVODNIPROCES_H
#define PROIZVODNIPROCES_H

#include <QDialog>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QFile>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDirIterator>

#include <QDebug>

namespace Ui {
class ProizvodniProces;
}

class ProizvodniProces : public QDialog
{
    Q_OBJECT

public:
    explicit ProizvodniProces(QWidget *parent = nullptr);
    ~ProizvodniProces();
    void Search(const QString&);
    void AddRootToTreeWidget(const QStringList&, QTreeWidgetItem*, QString&, bool);
    void OpisProdukta(QStringList&);
    void ClearWidgets(QLayout*);

private slots:
    void on_lineEdit_isciProdukt_textChanged(const QString &arg1);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_pocisti_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_skrijSliko_clicked();

private:
    Ui::ProizvodniProces *ui;
    QString m_searchLine;
    QStringList m_searchList;
    QColor m_green;
    QColor m_white;
    QColor m_grey;
    QColor m_opis;
    QString m_picFolder;
    bool m_naziv_bool;
    int m_numOfImages;
};

#endif // PROIZVODNIPROCES_H
