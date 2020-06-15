#ifndef PRINTERS_H
#define PRINTERS_H

#include <QDialog>

#include "methods.h"

namespace Ui {
class Printers;
}

class Printers : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit Printers(QWidget *parent = nullptr);
    ~Printers();
    void AddRootToTreeWidget();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    QString on_buttonBox_clicked();

private:
    Ui::Printers *ui;
    bool m_count;
    Methods* m_newMethod;
    QString m_printer;
};

#endif // PRINTERS_H
