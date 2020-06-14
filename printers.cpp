#include "printers.h"
#include "ui_printers.h"

Printers::Printers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Printers),
    m_count(true),
    m_newMethod(new Methods())
{
    ui->setupUi(this);
    AddRootToTreeWidget();
}

Printers::~Printers()
{
    delete m_newMethod;
    delete ui;
}

void Printers::AddRootToTreeWidget()
{
    for(short i(0); i < m_newMethod->m_printerList.length(); i++)
    {
        QTreeWidgetItem* itm = new QTreeWidgetItem();
        itm->setText(0, m_newMethod->m_printerList.at(i).printerName());
        itm->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignVCenter);
        ui->treeWidget->addTopLevelItem(itm);

        QColor color(220,220,220);
        QColor wcolor(250,250,250);

        ui->treeWidget->topLevelItemCount() % 2 == 0 ?
                    m_count = true :
                    m_count = false;

        m_count ? itm->setBackground(0, wcolor) :
                    itm->setBackground(0, color);

        m_count ? itm->setBackground(1, wcolor) :
                    itm->setBackground(1, color);
    }
}

void Printers::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    m_newMethod->m_printerSelected = item->text(column);
}
