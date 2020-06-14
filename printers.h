#ifndef PRINTERS_H
#define PRINTERS_H

#include <QDialog>

#include "methods.h"

namespace Ui {
class Printers;
}

class Printers : public QDialog
{
    Q_OBJECT

public:
    explicit Printers(QWidget *parent = nullptr);
    ~Printers();
    void AddRootToTreeWidget();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::Printers *ui;
    bool m_count;
    Methods* m_newMethod;
};

#endif // PRINTERS_H
