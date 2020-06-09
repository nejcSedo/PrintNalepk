#include "novanalepka.h"
#include "ui_novanalepka.h"

// KONSTRUKTOR
novaNalepka::novaNalepka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::novaNalepka), m_methods(new Methods()), m_map(m_methods->m_qrVelikost*5, m_methods->m_qrVelikost*5), m_mapSaved("qrNalepkePrint.png"), m_napis("")
{
    ui->setupUi(this);
    this->setWindowTitle("Qr koda");
    ui->pushButton_natisni->setDisabled(true);
    ui->lineEdit_napis->setMaxLength(14);
    ui->spinBox_kopijePrint->setValue(1);
    ui->lineEdit_napis->setFocus();
}

// DESTRUKTOR
novaNalepka::~novaNalepka() {
    delete m_methods;
    delete ui;
}

// OVERRIDE RESET
void novaNalepka::Reset() {
    ui->lineEdit_napis->setDisabled(false);
    ui->pushButton_natisni->setDisabled(false);
    ui->spinBox_kopijePrint->setValue(1);
    ui->lineEdit_napis->clear();
    ui->textEdit_qrNapis->clear();
    ui->lineEdit_shrani->clear();
    ui->pushButton_natisni->setDisabled(true);
    ui->textEdit_qrNapis->setFocus();
}

// NATISNI NALEPKO
void novaNalepka::on_pushButton_natisni_clicked() {
    m_methods->NalepkaPrint(ui->lineEdit_shrani->text(), ui->lineEdit_napis->text(),
                            "save", ui->textEdit_qrNapis->toPlainText(),
                            ui->spinBox_kopijePrint->value(), Methods::NacinTiska::Napis);
    Reset();
}

// TEKST V QR KODI
void novaNalepka::on_textEdit_qrNapis_textChanged()
{
    if(ui->textEdit_qrNapis->toPlainText() == "") {
        ui->lineEdit_napis->setDisabled(false);
        ui->pushButton_natisni->setDisabled(true);
    }
    else if(ui->textEdit_qrNapis->toPlainText().length() > 500) {
        m_napis = ui->textEdit_qrNapis->toPlainText();
        m_napis.chop(1);
        QMessageBox::warning(this, "Qr koda predolga", "Qr koda lahko vsebuje največ 500 črk!");
        ui->textEdit_qrNapis->setPlainText(m_napis);
        QTextCursor tmpCursor = ui->textEdit_qrNapis->textCursor();
        ui->textEdit_qrNapis->setTextCursor(tmpCursor);
    }
    else {
        ui->lineEdit_napis->setDisabled(true);
        ui->pushButton_natisni->setDisabled(false);
        m_napis = ui->textEdit_qrNapis->toPlainText();
    }
}

// NAPIS V NAPISU
void novaNalepka::on_lineEdit_napis_textChanged(QString arg1)
{
    if(arg1 == "") {
        ui->textEdit_qrNapis->setDisabled(false);
        ui->pushButton_natisni->setDisabled(true);
    }
    else if(arg1.length() > 14) {
        m_napis = arg1;
        m_napis.chop(1);
        QMessageBox::warning(this, "Napis predolg", "Napis lahko vsebuje največ 14 črk!");
        ui->lineEdit_napis->setText(m_napis);
    }
    else {
        ui->textEdit_qrNapis->setDisabled(true);
        ui->pushButton_natisni->setDisabled(false);
        m_napis = arg1;
    }
}
