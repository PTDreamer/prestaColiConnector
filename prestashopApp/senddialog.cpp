#include "senddialog.h"
#include "ui_senddialog.h"

sendDialog::sendDialog(QString host,QString login,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sendDialog),m_host(host),m_login(login)
{
    ui->setupUi(this);
    this->setFocus();
    ui->trackingNumber->setFocus();
    connect(&settingTimer,SIGNAL(timeout()),this,SLOT(settingTimerSlot()));
}

sendDialog::~sendDialog()
{
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

    if(orders.values().length()>1 || orders.values().length()==0)
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
