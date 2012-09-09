#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFontMetrics>
#include <QPrinterInfo>

static const char *PASSWORD = "";
#define STICKERS_PRINTED "14"
#define PAYMENT_ACCEPTED "2"
#define SHIPPED "4"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings=new QSettings("JB Tech","prestashop app",this);
    dialog=new DialogSettings(settings,this);
    connect(ui->doNothing,SIGNAL(toggled(bool)),this,SLOT(setMode()));
    connect(ui->autoFetchPrint,SIGNAL(toggled(bool)),this,SLOT(setMode()));
    connect(ui->checkOnly,SIGNAL(toggled(bool)),this,SLOT(setMode()));
   // processColibriOutput("/home/jb/test");
    connector.loadCountriesAndStates(dialog->getLogin(),dialog->getHost());
    connect(&connector,SIGNAL(debuggingInfo(QString)),this,SLOT(addDebugInfo(QString)));
/*
    prestaConnector con;
    QStringList list;
    QMap<QString,QString> map;
    list<<"0"<<"1"<<"2";
    QByteArray temp=con.getAdresses("NAAONB50QA18PX2QJT72UJYJMBMMD5SX","prestashop.localhost",list);
    con.arrayToFile(temp,QString("adresses"));
    temp=con.getOrders("NAAONB50QA18PX2QJT72UJYJMBMMD5SX","prestashop.localhost");
    con.arrayToFile(temp,QString("orders"));
    temp=con.retrieveCountries("NAAONB50QA18PX2QJT72UJYJMBMMD5SX","prestashop.localhost",map);
    con.arrayToFile(temp,"countries");
    temp=con.retrieveTaxes("NAAONB50QA18PX2QJT72UJYJMBMMD5SX","prestashop.localhost");
    con.arrayToFile(temp,QString("taxes"));
    ui->lcdPending->display(100);
    return;
    ui->lcdPending->display(100);
    processColibriOutput("~/test");
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOptions_triggered()
{
    dialog->exec();
}

void MainWindow::setMode()
{
    if(ui->doNothing->isChecked())
        automode=MainWindow::none;
    else if(ui->checkOnly->isChecked())
        automode=MainWindow::autofetch;
    else if(ui->autoFetchPrint->isChecked())
        automode=MainWindow::fullauto;
}
void MainWindow::on_pushButton_clicked()//Manual:FetchOrders
{
    orderMap.clear();
    adrMap.clear();
    filteredOrders.clear();
    adrArray.clear();

    QByteArray allOrders=connector.getOrders(dialog->getLogin(),dialog->getHost());
    QStringList orders;
    QStringList adresses;
    connector.filterOrders(PAYMENT_ACCEPTED,allOrders,adresses,orders);
    if(orders.length()>0)
        filteredOrders=connector.getOrders(dialog->getLogin(),dialog->getHost(),orders);
    adrArray=connector.getAdresses(dialog->getLogin(),dialog->getHost(),adresses);

    connector.addressesToStruct(adrArray,adrMap);
    connector.ordersToStruct(filteredOrders,orderMap);
    refreshTable();

}

void MainWindow::on_tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if(orderMap.values().length()<1)
        return;
    QList<prestaConnector::product> pro=orderMap.value(ui->tableWidget->item(current->row(),5)->text()).products;
    ui->tableWidget_2->setRowCount(pro.length());
    int row=0;
    foreach(prestaConnector::product p,pro)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(p.name));
        ui->tableWidget_2->setItem(row, 0, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(p.quantity));
        ui->tableWidget_2->setItem(row, 1, newItem);
        ++row;
    }
}

void MainWindow::on_pushButton_3_clicked()//Manual:Print Auto on A4
{
    if(adrMap.values().length()<1)
    {
        QMessageBox::critical(this,"Error","Não existem encomenda a processar");
    }
    else
    {
        QPrinter printer( QPrinter::HighResolution );
        printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
        QPrintPreviewDialog  preview( &printer, this );
        connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(a4Render(QPrinter*)) );
        preview.exec();
    }
}

void MainWindow::a4Render(QPrinter *printer)
{
    QList<prestaConnector::adress> list=adrMap.values();
    connector.renderAutoA4Preview(dialog->getFont(),dialog->getAutoFontSize(),dialog->getAutoBaseX(),dialog->getAutoBaseY(),ui->autoA4Offset->text().toInt(),false,list,printer);

}
void MainWindow::brotherRender(QPrinter *printer)
{
    prestaConnector::adress adr;
    adr.firstName="José";
    adr.lastName="Barros";
    adr.postalCode="2825-439";
    adr.adress1="Rua José Maria Pedroto Nº12,1ºE";
    adr.adress2="Santo António";
    adr.country="Portugal";
    adr.city="Costa de Caparica";
    adr.company="JB Tech";
    QList<prestaConnector::adress> list=adrMap.values();
    connector.renderAutoBrotherPreview(dialog->getFont(),dialog->getAutoFontSize(),dialog->getAutoBaseX(),dialog->getAutoBaseY(),adr,list,printer);

}
void MainWindow::registosRender(QPrinter *printer)
{
    prestaConnector::adress adr;
    adr.firstName="José";
    adr.lastName="Barros";
    adr.postalCode="2825-439";
    adr.adress1="Rua José Maria Pedroto Nº12,1ºE";
    adr.adress2="Santo António";
    adr.country="Portugal";
    adr.city="Costa de Caparica";
    adr.company="JB Tech";
    adr.state="";
    QList<prestaConnector::adress> list=adrMap.values();
    connector.renderRegistosPreview(dialog->getFont(),dialog->getRegFontSize(),dialog->getRegBaseX(),dialog->getRegBaseY(),adr,list,printer);

}
void MainWindow::on_pushButton_5_clicked()//Manual:Print Registos de envio
{
    if(adrMap.values().length()<1)
    {
        QMessageBox::critical(this,"Error","NÃ£o existem encomenda a processar");
    }
    else
    {
        QPrinterInfo infoBro;
        foreach(QPrinterInfo info,QPrinterInfo::availablePrinters())
        {
            if(info.printerName()=="Brother DCP-145C Printer")
                infoBro=info;
            qDebug()<<info.printerName();
        }
        QPrinter printer(infoBro, QPrinter::HighResolution );
        printer.setPageSize(QPrinter::A5);
        printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
        QPrintPreviewDialog  preview( &printer, this );
        connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(registosRender(QPrinter*)) );
        preview.exec();
    }
}

void MainWindow::on_pushButton_4_clicked()//Manual:Print Auto Brother
{
    if(adrMap.values().length()<1)
    {
        QMessageBox::critical(this,"Error","NÃ£o existem encomenda a processar");
    }
    else
    {
        QPrinter printer( QPrinter::HighResolution );
        printer.setPaperSize(QSizeF(62,100),QPrinter::Millimeter);
        printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
        QPrintPreviewDialog  preview( &printer, this );
        connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(brotherRender(QPrinter*)) );
        preview.exec();
    }
}

void MainWindow::on_actionImprimir_minha_morada_para_A4_triggered()
{
    QPrinter printer( QPrinter::HighResolution );
    printer.setPaperSize(QPrinter::A4);
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    QPrintPreviewDialog  preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(a4SenderRender(QPrinter*)) );
    preview.exec();
}

void MainWindow::a4SenderRender(QPrinter *printer)
{
    prestaConnector::adress adr;
    adr.firstName="José Barros";
    adr.lastName="JB Tech";
    adr.postalCode="2825-439";
    adr.adress1="Rua José Maria Pedroto Nº12,1ºE";
    adr.adress2="Santo António";
    adr.country="Portugal";
    adr.city="Costa de Caparica";
    adr.company="JB Tech";
    adr.state="";
    QList<prestaConnector::adress> list;
    for(int x=0;x<16;++x)
    {
        list.append(adr);
    }
    connector.renderAutoA4Preview(dialog->getFont(),dialog->getAutoFontSize(),dialog->getAutoBaseX(),dialog->getAutoBaseY(),ui->autoA4Offset->text().toInt(),false,list,printer);

}

void MainWindow::on_pushButton_6_clicked()//Manual:Print faturas
{
    QMap<QString,QString> tempmap;
    QByteArray temp=connector.retrieveCountries(dialog->getLogin(),dialog->getHost(),tempmap);
    connector.arrayToFile(temp,dialog->getColiCountriesFile());
    connector.arrayToFile(adrArray,dialog->getColiCustomersFile());
    connector.arrayToFile(filteredOrders,dialog->getColiOrdersFile());
    temp=connector.retrieveTaxes(dialog->getLogin(),dialog->getHost());
    connector.arrayToFile(temp,dialog->getColiTaxesFile());
    QStringList coliParameters;
    coliParameters<<"konqueror"<<"file:/home/thomas";
    QProcess myProcess(this);
    connect(&myProcess,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(colibriFinished (int, QProcess::ExitStatus)));
    myProcess.start(dialog->getColiPath(),coliParameters);
}

void MainWindow::colibriFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    processColibriOutput(dialog->getColiOutputFile());
}

void MainWindow::on_addByOrderIDAct_clicked()
{
    QStringList temp;
    QMap<QString,prestaConnector::order> tempOrderMap;
    temp<<ui->addByOrderID->text();
    QByteArray temparray=connector.getOrders(dialog->getLogin(),dialog->getHost(),temp,false);
    connector.ordersToStruct(temparray,tempOrderMap);
    QStringList adrlist;
    foreach(prestaConnector::order order,tempOrderMap.values())
    {
        adrlist.append(order.address_delivery);
        orderMap.insert(order.ID,order);
    }
    temparray=connector.getAdresses(dialog->getLogin(),dialog->getHost(),adrlist);
    connector.addressesToStruct(temparray,adrMap);
    refreshTable();
}
void MainWindow::refreshTable()
{
    int row=0;
    ui->tableWidget->setRowCount(orderMap.count());
    foreach(prestaConnector::order ord,orderMap.values())
    {
        prestaConnector::adress adr=adrMap.value(ord.address_delivery);//TODO PASSAR PARA INVOICE ADR
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(adr.ID));
        ui->tableWidget->setItem(row, 0, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(adr.firstName+" "+adr.lastName));
        ui->tableWidget->setItem(row, 1, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(adr.adress1));
        ui->tableWidget->setItem(row, 2, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(adr.country));
        ui->tableWidget->setItem(row, 3, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(ord.total_paid));
        ui->tableWidget->setItem(row, 4, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(ord.ID));
        ui->tableWidget->setItem(row, 5, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(ord.tracking));
        ui->tableWidget->setItem(row, 6, newItem);
        row++;
    }
    ui->tableWidget->setCurrentCell(0,0);
    ui->lcdOrdersToProcess->display(orderMap.count());
}

void MainWindow::on_addByCustomerIDAct_clicked()
{
    QStringList adrlist;
    adrlist<<ui->addByCustomerID->text();
    QByteArray temparray=connector.getAdresses(dialog->getLogin(),dialog->getHost(),adrlist);
    connector.addressesToStruct(temparray,adrMap);
    prestaConnector::order fakeOrder;
    fakeOrder.address_delivery=ui->addByCustomerID->text();
    fakeOrder.address_invoice=ui->addByCustomerID->text();
    fakeOrder.ID="-1";
    orderMap.insert(fakeOrder.ID,fakeOrder);
    refreshTable();
}

void MainWindow::on_addByCustomerNameAct_clicked()
{
    QStringList adrlist;
    QMap<QString,prestaConnector::adress> adrMapTemp;
    adrlist<<ui->addByCustomerID->text();
    QByteArray temparray=connector.getAdressesByName(dialog->getLogin(),dialog->getHost(),ui->addByCustomerName->text());
    connector.addressesToStruct(temparray,adrMapTemp);
    prestaConnector::adress adrTemp=adrMapTemp.values().at(0);
    adrMap.insert(adrTemp.ID,adrTemp);
    prestaConnector::order fakeOrder;
    fakeOrder.address_delivery=adrTemp.ID;
    fakeOrder.address_invoice=adrTemp.ID;
    fakeOrder.ID="-1";
    orderMap.insert(fakeOrder.ID,fakeOrder);
    refreshTable();
}

void MainWindow::on_deleteAllAct_clicked()
{
    adrMap.clear();
    orderMap.clear();
    refreshTable();
}


void MainWindow::on_deleteAct_clicked()
{
    orderMap.remove(ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text());
    refreshTable();
}

void MainWindow::on_pushButton_7_clicked()//Manual:Set Tracking number
{
    QMap<QString,TrackingNumberDialog::track> * list=new QMap<QString,TrackingNumberDialog::track>();
    foreach(prestaConnector::order order,orderMap.values())
    {
        TrackingNumberDialog::track t;
        t.orderID=order.ID;
        t.customerName=adrMap.value(order.address_delivery).firstName+" "+adrMap.value(order.address_delivery).lastName;
        list->insert(t.orderID,t);
    }
    if(list->values().length()<1)
        return;
    TrackingNumberDialog dia(dialog->getZBarPath(),list);
    if(dia.exec()==QDialog::Accepted)
    {
        foreach(TrackingNumberDialog::track track,list->values())
        connector.setTracking(dialog->getLogin(),dialog->getHost(),track.orderID,track.trackingNumber);
    }
}
int MainWindow::processColibriOutput(QString file)
{
    QStringList ordersEnvoiced;
    int ret=0;
    QFile mfile(file);

    if (!mfile.open(QFile::ReadOnly | QFile::Text))
    {
           return -1;
    }
    QByteArray data = mfile.readAll();
    qDebug()<<data;
    QDomDocument doc;
    doc.setContent(data);
    QDomNodeList nodes = doc.elementsByTagName("Adress");
    ui->log->append(QString("Colibri processed %0 adresses").arg(nodes.size()));
    for(int x=0;x<nodes.size();++x)
    {
        QDomElement element=nodes.at(x).firstChildElement("presta_id");
        QDomElement element2=nodes.at(x).firstChildElement("new");
        ui->log->append(QString("Adress with ID %0 new=%1").arg(element.text()).arg(element2.text()));
    }
    nodes = doc.elementsByTagName("Order");
    ui->log->append(QString("Colibri processed %0 orders").arg(nodes.size()));
    for(int x=0;x<nodes.size();++x)
    {
        QDomElement element=nodes.at(x).firstChildElement("presta_id");
        QDomElement element2=nodes.at(x).firstChildElement("colibri_id");
        QDomElement element3=nodes.at(x).firstChildElement("price_compare");
        ui->log->append(QString("Oreder with ID %0 was assigned colibri ID %1 and was processed %2").arg(element.text()).arg(element2.text()).arg(element3.text()));
        if(element3.text()!="OK")
            ret=-1;
        else
            ordersEnvoiced<<element.text();
    }
    foreach(QString str,ordersEnvoiced)
    {
        connector.setOrderField(dialog->getLogin(),dialog->getHost(),str,"current_state",STICKERS_PRINTED);
    }
    return ret;
}

void MainWindow::on_pushButton_2_clicked()
{
    sendDialog * dia=new sendDialog(dialog->getZBarPath(),dialog->getHost(),dialog->getLogin());
    dia->exec();
    delete dia;
}


void MainWindow::on_pushButton_8_clicked()
{
    connector.checkDelivery(dialog->getLogin(),dialog->getHost());
}
void MainWindow::addDebugInfo(QString text)
{
    ui->log->append(text);
}
