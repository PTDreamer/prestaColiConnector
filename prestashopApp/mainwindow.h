#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDebug>
#include <QMap>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewWidget>
#include <QPrintPreviewDialog>
#include <QTimer>
#include <QSettings>
#include "dialogsettings.h"
#include "prestaconnector.h"
#include <QTableWidget>
#include <QProcess>
#include <QInputDialog>
#include "trackingnumberdialog.h"
#include "senddialog.h"
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum mode{none,autofetch,fullauto};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void cleanRecords();
    void refreshTable();
    int processColibriOutput(QString file);
    bool coliBackup();
    void takeCareOffStuff();
    void registosRender(QPrinter *printer);
private slots:
    void coliReadyRead();
    void coliStarted();
    void addDebugInfo(QString text);
    void on_actionOptions_triggered();
    void setMode();
    void on_pushButton_clicked();
    void on_tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);
    void on_pushButton_3_clicked();
    void a4Render(QPrinter * printer);
    void brotherRender(QPrinter *printer);
    void a4SenderRender(QPrinter *printer);
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_actionImprimir_minha_morada_para_A4_triggered();
    void on_pushButton_6_clicked();
    void colibriFinished (int exitCode, QProcess::ExitStatus exitStatus );
    void on_addByOrderIDAct_clicked();
    void on_addByCustomerIDAct_clicked();
    void on_addByCustomerNameAct_clicked();
    void on_deleteAllAct_clicked();
    void on_deleteAct_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_8_clicked();
    void automationTimerSlot();
    void on_pushButton_9_clicked();

private:
    QProcess * coliProcess;
    QMap<QString,prestaConnector::adress> adrMap;
    QMap<QString,prestaConnector::order> orderMap;
    QByteArray filteredOrders;
    QByteArray deliveryAdrArray;
    QByteArray invoiceAdrArray;
    Ui::MainWindow *ui;
    QSettings * settings;
    mode automode;
    prestaConnector connector;
    DialogSettings * dialog;
    QTimer automationTimer;
};

#endif // MAINWINDOW_H
