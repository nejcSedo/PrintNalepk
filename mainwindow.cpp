#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

// KONSTRUKOR
MainWindow::MainWindow(QWidget *parent)
    : Methods(parent),
      ui(new Ui::MainWindow),
      m_count(true),
      m_TreeWidgetIsClicked(false),
      m_executable(true),
      m_id(""), m_naziv(""),
      m_verzija("v1.7"),
      m_verzijaLabel(new QLabel(this)),
      m_searchLine(";"),
      m_searchList(m_searchLine.split(';', QString::SkipEmptyParts)),
      m_treeItemCount(0)
{
    ui->setupUi(this);
    QIcon icon(":icons/icon.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle("Tiskanje nalepk");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidget->header()->setStretchLastSection(false);
    ui->treeWidget->setRootIsDecorated(false);
    ui->lineEdit_IDprodukta->setMaxLength(40);
    ui->lineEdit_nazivProdukta->setMaxLength(40);
    ui->lineEdit_kolicina->setMaxLength(40);
    m_verzijaLabel->setAlignment(Qt::AlignLeft);
    m_verzijaLabel->setText(m_verzija);
    ui->statusbar->addPermanentWidget(m_verzijaLabel);
    Reset();
}

// DESTRUKTOR
MainWindow::~MainWindow() {
    delete ui;
}

// OVERRIDE RESET
void MainWindow::Reset() {
    m_treeItemCount = 0;
    m_TreeWidgetIsClicked = false;
    ui->spinBox_kopijePrint->setValue(1);
    ui->lineEdit_IDprodukta->clear();
    ui->lineEdit_nazivProdukta->clear();
    ui->lineEdit_kolicina->clear();
    ui->textEdit_opombe->clear();
    ui->pushButton_shraniNalepko->setDisabled(true);
    ui->pushButton_natisni->setDisabled(true);
    ui->actionPrint->setDisabled(true);
    ui->actionShrani_nalepko->setDisabled(true);
    ReadFileAndAddToTreeWidget();
    ui->lineEdit_IDprodukta->setFocus();
}

// ITEM V TREEWIDGET
void MainWindow::AddRootToTreeWidget(const QString& id, const QString& naziv, QTreeWidgetItem* itm) {
    itm->setText(0, id);
    itm->setTextAlignment(0, Qt::AlignHCenter);
    itm->setText(1, naziv);
    itm->setTextAlignment(1, Qt::AlignHCenter);
    ui->treeWidget->addTopLevelItem(itm);
    QColor color(220,220,220);
    QColor wcolor(250,250,250);
    ui->treeWidget->topLevelItemCount() % 2 == 0 ? m_count = true : m_count = false;
    m_count ? itm->setBackground(0, wcolor) : itm->setBackground(0, color);
    m_count ? itm->setBackground(1, wcolor) : itm->setBackground(1, color);
}

// PREBERE FILE Z SHRANJENIMI NALEPKAMI
void MainWindow::ReadFileAndAddToTreeWidget()
{
    ui->treeWidget->clear();
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::ReadOnly | QFile::Text)) {
        Error(MainWindow::ErrorType::ReadError);
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");

    while(!in.atEnd()) {
        m_searchLine = in.readLine();
        if(m_searchLine == "" || m_searchLine == "DELETE;DELETE" || m_searchLine == "Seznam nalepk:")
            continue;
        else {
            QTreeWidgetItem* itm = new QTreeWidgetItem();
            m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
            AddRootToTreeWidget(m_searchList.at(0), m_searchList.at(1), itm);
        }
    }

    fileName.close();
}

// ISKALNIK
void MainWindow::Search(const QString& id, const QString& naziv)
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly)) {
        Error(MainWindow::ErrorType::SearchError);
        return;
    }

    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    while(!out.atEnd())
    {
        m_searchLine = out.readLine();
        if((m_searchLine.contains(id, Qt::CaseInsensitive) && id != "") || (m_searchLine.contains(naziv, Qt::CaseInsensitive) && naziv != ""))
        {
            if(m_searchLine == "Seznam nalepk:")
                continue;
            if(m_searchLine.contains("DELETE"))
                continue;
            else
            {
                QTreeWidgetItem* itm = new QTreeWidgetItem();
                m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);
                AddRootToTreeWidget(m_searchList.at(0), m_searchList.at(1), itm);
            }
        }
    }
    fileName.close();
}

// UJAME PRITISK NA TIPKO
void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Insert || event->key() == Qt::Key_Return) {
        m_executable = false;
        if(ui->treeWidget->topLevelItem(0) == nullptr) {
            Reset();
        }
        else if(m_treeItemCount != 0) {
            ui->lineEdit_IDprodukta->setText(ui->treeWidget->selectedItems().at(0)->text(0));
            ui->lineEdit_nazivProdukta->setText(ui->treeWidget->selectedItems().at(0)->text(1));
            ui->lineEdit_kolicina->setFocus();
            m_treeItemCount = 0;
        }
        else {
            ui->lineEdit_IDprodukta->setText(ui->treeWidget->topLevelItem(0)->text(0));
            ui->lineEdit_nazivProdukta->setText(ui->treeWidget->topLevelItem(0)->text(1));
            ui->lineEdit_kolicina->setFocus();
            m_treeItemCount = 0;
        }
    }

    if(event->key() == Qt::Key_Delete) {
        m_executable = false;
        Reset();
    }

    if(event->key() == Qt::Key_Up) {
        m_executable = false;
        m_treeItemCount == 0 ? m_treeItemCount = 0 : m_treeItemCount-- ;

        if(m_treeItemCount == ui->treeWidget->topLevelItemCount() - 1) {
            ui->treeWidget->topLevelItem(m_treeItemCount)->setSelected(false);
        }
        else {
            ui->treeWidget->topLevelItem(m_treeItemCount + 1)->setSelected(false);
        }

        ui->treeWidget->topLevelItem(m_treeItemCount)->setSelected(true);
    }

    if(event->key() == Qt::Key_Down) {
        m_executable = false;
        m_treeItemCount == ui->treeWidget->topLevelItemCount() - 1 ? m_treeItemCount = ui->treeWidget->topLevelItemCount() - 1 : m_treeItemCount++ ;

        if(m_treeItemCount == 0) {
            ui->treeWidget->topLevelItem(m_treeItemCount)->setSelected(false);
        }
        else {
            ui->treeWidget->topLevelItem(m_treeItemCount - 1)->setSelected(false);
        }

        ui->treeWidget->topLevelItem(m_treeItemCount)->setSelected(true);
    }
    m_executable = true;
}

// PREVERI ALI JE NALEPKA ZE SHRANJENA
void MainWindow::ProduktCheck(QString& id, QString& naziv)
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        Error(MainWindow::ErrorType::ProductCheckError);
        return;
    }

    QTextStream in(&fileName);
    short numOfLines(0);
    in.setCodec("UTF-8");
    while(!in.atEnd())
    {
        m_searchLine = in.readLine();
        numOfLines++;
        if(m_searchLine == "Seznam nalepk:")
            continue;

        id.at(id.length()-1) == ' ' ? id = id.remove(id.length() - 1, 1) : nullptr;
        naziv.at(naziv.length()-1) == ' ' ? naziv = naziv.remove(naziv.length() - 1, 1) : nullptr;

        m_searchList = m_searchLine.split(';', QString::SkipEmptyParts);

        if(id == "" || naziv == "")
        {
            ui->pushButton_natisni->setDisabled(true);
            ui->actionPrint->setDisabled(true);
        }
        else
        {
            ui->pushButton_natisni->setDisabled(false);
            ui->actionPrint->setDisabled(false);
        }

        if(m_searchList.at(0) == id || m_searchList.at(1) == naziv || id == "" || naziv == "")
        {
            ui->pushButton_shraniNalepko->setDisabled(true);
            ui->actionShrani_nalepko->setDisabled(true);
            break;
        }
        else
        {
            ui->pushButton_shraniNalepko->setDisabled(false);
            ui->actionShrani_nalepko->setDisabled(false);
        }
    }

    if(numOfLines == 1 && id != "" && naziv != "")
    {
        ui->pushButton_shraniNalepko->setDisabled(false);
        ui->pushButton_natisni->setDisabled(false);
    }

    fileName.close();
}

// ERROR
void MainWindow::Error(const ErrorType& numError)
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QIcon icon(":icons/error.ico");
    dialog->setWindowTitle("Napaka");
    dialog->setWindowIcon(icon);
    switch (numError)
    {
        case MainWindow::ErrorType::ReadError: label->setText("Error:\nfileName v funkciji Read() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::SearchError: label->setText("Error:\nfileName v funkciji Search() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::ProductCheckError: label->setText("Error:\nfileName v funkciji ProductCheck() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::DeleteError: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::DeleteError2: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::DeleteError3: label->setText("Error:\nfileName v funkciji on_actionDelete_triggered() ne obstaja!\n Znova zaženi program!"); break;
        case MainWindow::ErrorType::SaveError: label->setText("Error:\nfileName v funkciji on_actionShrani_nalepko_triggered() ne obstaja!\n Znova zaženi program!"); break;
        default: label->setText("Neznana napaka!\n Znova zaženi program!"); break;
    }
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->exec();
}

// SHRANI NALEPKO
void MainWindow::on_pushButton_shraniNalepko_clicked()
{
    MainWindow::on_actionShrani_nalepko_triggered();
}

// NATISNI NALEPKO
void MainWindow::on_pushButton_natisni_clicked()
{
    MainWindow::on_actionPrint_triggered();
}

// ID SE SPREMENI
void MainWindow::on_lineEdit_IDprodukta_textChanged(const QString& arg1)
{
    if(m_executable) {
        m_treeItemCount = 0;
        if(m_TreeWidgetIsClicked)
        {
            m_TreeWidgetIsClicked = false;
            return;
        }

        if(arg1 == "")
        {
            ReadFileAndAddToTreeWidget();
            ui->pushButton_shraniNalepko->setDisabled(true);
            return;
        }

        if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ')
            ui->lineEdit_IDprodukta->backspace();
        else
        {
            m_id = arg1.toUpper();
            m_naziv = ui->lineEdit_nazivProdukta->text().toUpper();
            ui->treeWidget->clear();
            Search(m_id, m_naziv);
            ProduktCheck(m_id, m_naziv);
        }
    }
}

// NAZIV SE SPREMENI
void MainWindow::on_lineEdit_nazivProdukta_textChanged(const QString& arg1)
{
    if(m_executable) {
        m_treeItemCount = 0;
        if(m_TreeWidgetIsClicked)
        {
            m_TreeWidgetIsClicked = false;
            return;
        }

        if(arg1 == "")
        {
            ReadFileAndAddToTreeWidget();
            ui->pushButton_shraniNalepko->setDisabled(true);
            return;
        }

        if(arg1.at(arg1.length()-2) == ' ' && arg1.at(arg1.length()-1) == ' ')
            ui->lineEdit_nazivProdukta->backspace();
        else
        {
            m_naziv = arg1.toUpper();
            m_id = ui->lineEdit_IDprodukta->text().toUpper();
            ui->treeWidget->clear();
            Search(m_id, m_naziv);
            ProduktCheck(m_id, m_naziv);
        }
    }
}

// OPOMBA SPREMEMBA
void MainWindow::on_textEdit_opombe_textChanged()
{
    if(ui->textEdit_opombe->toPlainText().length() > 500)
    {
        QString napis(ui->textEdit_opombe->toPlainText());
        QTextCursor tmpCursor = ui->textEdit_opombe->textCursor();
        napis.chop(1);
        QMessageBox::warning(this, "Napis predolg", "Napis lahko vsebuje največ 500 črk!");
        ui->textEdit_opombe->setPlainText(napis);
        ui->textEdit_opombe->setTextCursor(tmpCursor);
    }
}

// CE KLIKNES NA NALEPKO V TREEWIDGETU
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item)
{
    m_TreeWidgetIsClicked = true;
    ui->lineEdit_IDprodukta->setText(item->text(0));
    ui->lineEdit_nazivProdukta->setText(item->text(1));
    ui->lineEdit_kolicina->setFocus();
}

// DESNA TIPKA NA TREEWIDGET DA ZBRISES NALEPKO
void MainWindow::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QMenu menu(this);
    QIcon bin(":icons/delete.ico");
    menu.addAction(ui->actionDelete);
    ui->actionDelete->setData(QVariant(pos));
    ui->actionDelete->setIcon(bin);
    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}

// IZHOD
void MainWindow::on_actionIzhod_triggered()
{
    ui->statusbar->showMessage("Izhod", 3000);
    QApplication::quit();
}

// IZBRISI NALEPKO
void MainWindow::on_actionDelete_triggered()
{
    if(ui->treeWidget->currentItem() == nullptr)
        return;

    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::Text | QFile::ReadOnly))
    {
        Error(MainWindow::ErrorType::DeleteError);
        return;
    }

    QTextStream in(&fileName);
    in.setCodec("UTF-8");
    QString allText = in.readAll();
    fileName.close();

    QString produkt(ui->treeWidget->currentItem()->text(0)+";"+ui->treeWidget->currentItem()->text(1));
    allText.replace(produkt, "DELETE;DELETE");

    if(!fileName.open(QFile::WriteOnly | QFile::Truncate))
    {
        Error(MainWindow::ErrorType::DeleteError2);
        return;
    }
    fileName.flush();
    fileName.close();

    if(!fileName.open(QFile::WriteOnly | QFile::Text))
    {
        Error(MainWindow::ErrorType::DeleteError3);
        return;
    }
    in << allText;
    fileName.flush();
    fileName.close();

    Reset();
    ui->statusbar->showMessage("Nalepka izbrisana", 3000);
}

// SHRANI NALEPKO
void MainWindow::on_actionShrani_nalepko_triggered()
{
    QFile fileName("nalepke.txt");
    if(!fileName.open(QFile::WriteOnly | QFile::Append))
    {
        Error(MainWindow::ErrorType::SaveError);
        return;
    }

    QTextStream out(&fileName);
    out.setCodec("UTF-8");
    m_id = ui->lineEdit_IDprodukta->text().toUpper();
    m_naziv = ui->lineEdit_nazivProdukta->text().toUpper();
    while(m_naziv.at(m_naziv.length() - 1) == ' ')
        m_naziv.remove(-1, 1);

    out << m_id << ";" << m_naziv << "\n";
    fileName.flush();
    fileName.close();

    Reset();
    ui->statusbar->showMessage("Nalepka shranjena", 3000);
}

// PRINT
void MainWindow::on_actionPrint_triggered() {
    NalepkaPrint(ui->lineEdit_IDprodukta->text(), ui->lineEdit_nazivProdukta->text(),
                            ui->lineEdit_kolicina->text(), ui->textEdit_opombe->toPlainText(),
                            ui->spinBox_kopijePrint->value(), Methods::NacinTiska::Standard);

    Reset();
    ui->statusbar->showMessage("Nalepka se tiska", 3000);
}

// O PROGRAMU
void MainWindow::on_actionO_programu_triggered()
{
    QDialog *dialog = new QDialog(this);
    QLabel *label = new QLabel(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QIcon icon(":icons/about.ico");
    dialog->setWindowTitle("O programu");
    dialog->setWindowIcon(icon);
    label->setText("Program za shranjevanje in tiskanje termo nalepk, ter Qr kode.\nTiskalnik Rollo.\n\nIzdelal: Nejc Sedovnik\nLeto: April 2020\nVerzija: " + m_verzija);
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->exec();
}

// QR KODA ALI NAPIS
void MainWindow::on_actionNov_napis_triggered()
{
    novaNalepka* ustvariNalepko = new novaNalepka(this);
    ustvariNalepko->setModal(true);
    ustvariNalepko->exec();
    delete ustvariNalepko;
}
