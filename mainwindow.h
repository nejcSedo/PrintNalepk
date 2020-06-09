#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "novanalepka.h"
#include "methods.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public Methods//, public QMainWindow
{
    Q_OBJECT
    enum class ErrorType {
        ReadError = 0, SearchError, ProductCheckError, DeleteError, DeleteError2, DeleteError3, SaveError
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Reset() override;
    void AddRootToTreeWidget(const QString&, const QString&, QTreeWidgetItem*);
    void ReadFileAndAddToTreeWidget();
    void Search(const QString&, const QString&);
    void keyReleaseEvent(QKeyEvent*) override;
    void ProduktCheck(QString&, QString&);
    void Error(const ErrorType&);

private slots:
    void on_pushButton_shraniNalepko_clicked();
    void on_pushButton_natisni_clicked();
    void on_lineEdit_IDprodukta_textChanged(const QString&);
    void on_lineEdit_nazivProdukta_textChanged(const QString&);
    void on_treeWidget_itemClicked(QTreeWidgetItem*);
    void on_treeWidget_customContextMenuRequested(QPoint);
    void on_actionIzhod_triggered();
    void on_actionDelete_triggered();
    void on_actionShrani_nalepko_triggered();
    void on_actionPrint_triggered();
    void on_actionO_programu_triggered();
    void on_actionNov_napis_triggered();
    void on_textEdit_opombe_textChanged();

private:
    Ui::MainWindow *ui;
    bool m_count;
    bool m_TreeWidgetIsClicked;
    bool m_executable;
    QString m_id;
    QString m_naziv;
    QString m_verzija;
    QLabel *m_verzijaLabel;
    QString m_searchLine;
    QStringList m_searchList;
    short m_treeItemCount;
};
#endif // MAINWINDOW_H
