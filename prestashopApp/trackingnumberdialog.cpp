#include "trackingnumberdialog.h"
#include "ui_trackingnumberdialog.h"

TrackingNumberDialog::TrackingNumberDialog(QString zbarPath,QMap<QString, track> * map,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrackingNumberDialog),index(0)
{
    mappointer=map;
    ui->setupUi(this);
    load(0);
    connect(this,SIGNAL(accepted()),this,SLOT(on_pbOK_clicked()));
    if(map->values().length()<2)
    {
        hideNavButtons(false);
    }
    QStringList arguments;
   // arguments << "-style";//TODO
    zbar = new QProcess(parent);
    connect(zbar,SIGNAL(started()),this,SLOT(zbarStarted()));
    zbar->setReadChannel(QProcess::StandardOutput);
    zbar->start(zbarPath, arguments);
    connect(zbar,SIGNAL(readyRead()),SLOT(zbarReadyRead()));
    this->setFocus();
    ui->trackingNumber->setFocus();
}

TrackingNumberDialog::~TrackingNumberDialog()
{
    zbar->terminate();
    delete ui;
}

void TrackingNumberDialog::load(int order)
{
    if(order<mappointer->values().length() && order>=0)
    {
        track t=mappointer->values().at(order);
        ui->customerName->setText(t.customerName);
    }
}

void TrackingNumberDialog::on_pbnext_clicked()
{
    ++index;
    if(!(index<mappointer->values().length() && index>=0))
        --index;
    load(index);
    ui->trackingNumber->setFocus();
}

void TrackingNumberDialog::on_pbOK_clicked()
{
    track t=mappointer->values().at(index);
    t.trackingNumber=ui->trackingNumber->text();
    mappointer->remove(t.orderID);
    mappointer->insert(t.orderID,t);
    ++index;
    if(!(index<mappointer->values().length() && index>=0))
        --index;
    load(index);
}

void TrackingNumberDialog::on_pbprevious_clicked()
{
    --index;
    if(!(index<mappointer->values().length() && index>=0))
        ++index;
    load(index);
}
void TrackingNumberDialog::hideNavButtons(bool show)
{
    ui->pbnext->setVisible(show);
    ui->pbOK->setVisible(show);
    ui->pbprevious->setVisible(show);
}
void TrackingNumberDialog::zbarReadyRead()
{
    QByteArray temp=zbar->readAllStandardOutput();
    QString s(temp);

    qDebug()<<"SCANNED:"<<s;
    ui->trackingNumber->setText(s);
    if(ui->pbnext->isVisible())
        on_pbnext_clicked();
    else
        on_pbOK_clicked();
}
void TrackingNumberDialog::zbarStarted()
{
    static bool flag=false;
    if(!flag)
    {
        QTimer::singleShot(1000,this,SLOT(zbarStarted()));
        flag=true;
    }
    qDebug()<<"FORCE";
    this->activateWindow();
    this->setFocus();
    ui->trackingNumber->setFocus();
}

void TrackingNumberDialog::on_trackingNumber_returnPressed()
{
    if(ui->pbnext->isVisible())
        on_pbnext_clicked();
    else
        on_pbOK_clicked();
}
