#ifndef SENDDIALOG_H
#define SENDDIALOG_H

#include <QDialog>
#include <QDialog>
#include "prestaconnector.h"
#include <QProcess>
#include <QTimer>
#include <QMap>

namespace Ui {
class sendDialog;
}

class sendDialog : public QDialog
{
    Q_OBJECT
    
public:
    struct track
    {
        QString orderID;
        QString customerName;
        QString trackingNumber;
    };
    explicit sendDialog(QString host,QString login,QWidget *parent=NULL);
    ~sendDialog();
    
    void save();
private:
    Ui::sendDialog *ui;
    prestaConnector connector;
    QString m_login;
    QString m_host;
    QTimer settingTimer;
    int timerCount;
    QString currentID;
public slots:
    void on_trackingNumber_returnPressed();
    void settingTimerSlot();
signals:
    void debugOutput(QString text);
private slots:
    void on_accept_clicked();
    void on_ignore_clicked();
};
#endif // SENDDIALOG_H
