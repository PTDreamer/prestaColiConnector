#include "trackingnumberdialog.h"
#include "ui_trackingnumberdialog.h"

TrackingNumberDialog::TrackingNumberDialog(QMap<QString, track> * map,QWidget *parent) :
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
    this->setFocus();
    ui->trackingNumber->setFocus();
}

TrackingNumberDialog::~TrackingNumberDialog()
{
    delete ui;
}

void TrackingNumberDialog::load(int order)
{
    if(order<mappointer->values().length() && order>=0)
    {
        track t=mappointer->values().at(order);
        ui->customerName->setText(t.customerName);
        ui->trackingNumber->setText("");
    }
}

void TrackingNumberDialog::on_pbnext_clicked()
{
    ++index;
    if(!(index<mappointer->values().length() && index>=0))
    {
        --index;
    }
    load(index);
    ui->trackingNumber->setFocus();
}

void TrackingNumberDialog::on_pbOK_clicked()
{
    static bool closing=false;
    if(!closing)
    {
    track t=mappointer->values().at(index);
    t.trackingNumber=ui->trackingNumber->text();
    mappointer->remove(t.orderID);
    mappointer->insert(t.orderID,t);
    ++index;
    if(!(index<mappointer->values().length() && index>=0))
    {
        closing=true;
        --index;
        this->done(QDialog::Accepted);
    }
    load(index);
    }
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
