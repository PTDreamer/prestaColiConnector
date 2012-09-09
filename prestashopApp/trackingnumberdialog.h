#ifndef TRACKINGNUMBERDIALOG_H
#define TRACKINGNUMBERDIALOG_H

#include <QDialog>
#include "prestaconnector.h"
#include <QProcess>
#include <QTimer>

namespace Ui {
class TrackingNumberDialog;
}
class TrackingNumberDialog : public QDialog
{

    Q_OBJECT
    
public:

    struct track
    {
        QString orderID;
        QString customerName;
        QString trackingNumber;
    };
    explicit TrackingNumberDialog(QString zbarPath,QMap<QString,track> * map,QWidget *parent = 0);
    ~TrackingNumberDialog();
    void load(int order);
    void hideNavButtons(bool show);

public slots:
    void zbarStarted();
    void on_pbnext_clicked();
    void on_pbOK_clicked();
    void zbarReadyRead();

private slots:
    void on_pbprevious_clicked();

    void on_trackingNumber_returnPressed();

private:
    QProcess *zbar;
    QMap<QString, track> * mappointer;
    int index;
    Ui::TrackingNumberDialog *ui;
};

#endif // TRACKINGNUMBERDIALOG_H
