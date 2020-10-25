#ifndef DODAJPRODUKT_H
#define DODAJPRODUKT_H

#include <QDialog>
#include <QFile>
#include <QTableWidgetItem>
#include <QDebug>

namespace Ui {
class DodajProdukt;
}

class DodajProdukt : public QDialog
{
    Q_OBJECT

public:
    explicit DodajProdukt(QWidget *parent = nullptr);
    ~DodajProdukt();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_vrstaProdukta_currentIndexChanged(int index);
    void on_comboBox_stevilozil_currentIndexChanged(int index);
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::DodajProdukt *ui;
    int m_steviloZil;
};

#endif // DODAJPRODUKT_H
