#include "senddialog.h"
#include "ui_senddialog.h"

sendDialog::sendDialog(QString zbarPath,QString host,QString login,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sendDialog),m_host(host),m_login(login)
{
    ui->setupUi(this);
    QStringList arguments;
   // arguments << "-style";//TODO
    zbar = new QProcess(parent);
    connect(zbar,SIGNAL(started()),this,SLOT(zbarStarted()));
    zbar->setReadChannel(QProcess::StandardOutput);
    zbar->start(zbarPath, arguments);
    connect(zbar,SIGNAL(readyRead()),SLOT(zbarReadyRead()));
    this->setFocus();
    ui->trackingNumber->setFocus();
    connect(&settingTimer,SIGNAL(timeout()),this,SLOT(settingTimerSlot()));
}

sendDialog::~sendDialog()
{
    zbar->terminate();
    delete ui;
}

void sendDialog::settingTimerSlot()
{
    timerCount--;
    if(timerCount==0)
    {
        settingTimer.stop();
        ui->saveTimer->setText("SAVED");
        save();
    }
    else
    {
        ui->saveTimer->setText(QString::number(timerCount));
    }
}
void sendDialog::save()
{
    emit debugOutput(QString("sendDialog:order with ID:%0 was set to shipped status").arg(currentID));
    connector.setOrderField(m_login,m_host,currentID,"current_state","1");
}

void sendDialog::zbarStarted()
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

void sendDialog::zbarReadyRead()
{
    QByteArray temp=zbar->readAllStandardOutput();
    QString s(temp);

    qDebug()<<"SCANNED:"<<s;
    ui->trackingNumber->setText(s);
    on_trackingNumber_returnPressed();
}

void sendDialog::on_trackingNumber_returnPressed()
{
    if(settingTimer.isActive())
    {
        settingTimer.stop();
        ui->saveTimer->setText("SAVED");
        save();
    }
    QMap<QString,prestaConnector::order> orders;
    QMap<QString,prestaConnector::adress> adresses;
    QString filter=QString("&filter[shipping_number]=[%1]").arg(ui->trackingNumber->text());
    QByteArray array=connector.getOrders(m_login,m_host,QStringList(),false,filter);
    connector.ordersToStruct(array,orders);

    if(orders.values().length()>1)
    {
        ui->customerName->setText("NOT FOUND");
    }
    else
    {
        prestaConnector::order order=orders.values().at(0);
        QString temp=order.address_delivery;
        QStringList list;
        list.append(temp);
        array=connector.getAdresses(m_login,m_host,list);
        connector.addressesToStruct(array,adresses);
        prestaConnector::adress adr=adresses.values().at(0);
        ui->customerName->setText(QString(adr.firstName+" "+adr.lastName));
        currentID=order.ID;
        timerCount=6;
        settingTimer.start(1000);
    }
}

void sendDialog::on_accept_clicked()
{
    if(settingTimer.isActive())
    {
        settingTimer.stop();
        ui->saveTimer->setText("SAVED");
        save();
    }
}

void sendDialog::on_ignore_clicked()
{
    if(settingTimer.isActive())
    {
        settingTimer.stop();
        ui->saveTimer->setText("IGNORED");
    }
}
