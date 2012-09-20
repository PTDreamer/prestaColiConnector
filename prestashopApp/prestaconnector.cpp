#include "prestaconnector.h"
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>
#include <QDomElement>
#include <QDomDocument>
#include <QFileInfo>

#define STICKERS_PRINTED "14"
#define PAYMENT_ACCEPTED "2"
#define SHIPPED "4"
#define PASSWORD ""
#define DELIVERED "5"

prestaConnector::prestaConnector(QObject *parent):QObject(parent)
{
    m_network = new QNetworkAccessManager(this);
}

void prestaConnector::loadCountriesAndStates(QString user, QString host)
{
    retrieveCountries(user,host,countries);
    retrieveStates(user,host,states);
}
QByteArray prestaConnector::retrieveCountries(QString user,QString host, QMap<QString, QString> &countries)
{
    QByteArray ret;
    countries.clear();
    emit debuggingInfo("Retrieving countries");
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl("http://"+host+"/api/countries/?display=full"));
    QEventLoop loop;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &loop, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    loop.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting countries");
        tT.stop();
        return ret;
    }
    else
    {
           emit debuggingInfo("Getting countries done");
           tT.stop();
           if (nreply->error() > 0) {
               emit debuggingInfo(QString("GET FAILED Error number = %0").arg(nreply->errorString()));
           }
           else {
               ret = nreply->readAll();
               QDomDocument doc;
               doc.setContent(ret);
               QDomNodeList nodes = doc.elementsByTagName("country");
               if (nodes.size() > 0) {
                   QString str;
                   for(int x=0;x<nodes.size();++x)
                   {
                       str=nodes.at(x).firstChildElement("name").firstChildElement("language").text();
                       countries.insert(nodes.at(x).firstChildElement("id").text(),str);
                   }
               }
           }

    }
    return ret;
}
QByteArray prestaConnector::retrieveStates(QString user,QString host,QMap <QString,QString> & states)
{
    QByteArray ret;
    states.clear();
    emit debuggingInfo("Retrieving states");
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl("http://"+host+"/api/states/?display=full"));
    QEventLoop loop;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &loop, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    loop.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting states");
        tT.stop();
        return ret;
    }
    else
    {
           emit debuggingInfo("Getting states done");
           tT.stop();
           if (nreply->error() > 0) {
               emit debuggingInfo("GET FAILED Error number = " + nreply->errorString());
           }
           else {
               ret = nreply->readAll();
               QDomDocument doc;
               doc.setContent(ret);
               QDomNodeList nodes = doc.elementsByTagName("state");
               if (nodes.size() > 0) {
                   for(int x=0;x<nodes.size();++x)
                   {
                       states.insert(nodes.at(x).firstChildElement("id").text(),nodes.at(x).firstChildElement("name").text());
                   }
               }
           }
    }
    return ret;
}
QByteArray prestaConnector::retrieveTaxes(QString user,QString host)
{
    QByteArray ret;
    emit debuggingInfo("Retrieving Taxes");
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl("http://"+host+"/api/tax_rules/?display=full"));
    QEventLoop loop;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &loop, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    loop.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting taxes");
        tT.stop();
        return ret;
    }
    else
    {
           emit debuggingInfo("Getting taxes done");
           tT.stop();
           if (nreply->error() > 0) {
               emit debuggingInfo(QString("GET FAILED Error number = %0").arg(nreply->errorString()));
           }
           else {
               ret = nreply->readAll();
           }

    }
    return ret;
}
QByteArray prestaConnector::getOrders(QString user,QString host,QStringList ids,bool validOnly,QString customFilter)
{
    QByteArray array;
    QString filter;
    foreach(QString str,ids)
    {
        filter.append(str);
        filter.append("|");
    }
    filter.chop(1);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    QString requestStr="http://"+host+"/api/orders/?display=full";
    if(ids.length()>0)
    {
        requestStr=requestStr+QString("&filter[id]=[%0]").arg(filter);
    }
    if(!customFilter.isEmpty())
        requestStr=requestStr+customFilter;
    if(validOnly)
        requestStr=requestStr+"&filter[valid]=[1]";
    request.setUrl(QUrl(requestStr));
    QEventLoop q;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &q, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    q.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting orders");
        tT.stop();
        return array;
    }
    else
    {
           emit debuggingInfo("Getting orders done");
           tT.stop();
           if (nreply->error() > 0) {
               emit debuggingInfo("GET Orders FAILED Error number = " + nreply->errorString());
               return array;
           }
           else {
               array = nreply->readAll();
               return array;
           }

    }
}
QByteArray prestaConnector::getAdresses(QString user,QString host,QStringList ids,bool all)
{
    QByteArray array;
    QString filter;
    foreach(QString str,ids)
    {
        filter.append(str);
        filter.append("|");
    }
    filter.chop(1);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    if(all)
        request.setUrl(QUrl(QString("http://%1/api/addresses/?display=full").arg(host)));
    else
        request.setUrl(QUrl(QString("http://%1/api/addresses/?display=full&filter[id]=[%2]").arg(host).arg(filter)));
    QEventLoop q;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &q, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    q.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting adresses");
        tT.stop();
        return array;
    }
    else
    {
           emit debuggingInfo("Getting adresses done");
           if (nreply->error() > 0) {
               qDebug()<<nreply->errorString();
               emit debuggingInfo("GET adresses FAILED Error number = " + nreply->errorString());
               return array;
           }
           else {
               array = nreply->readAll();
               return array;
           }
    }
}
QByteArray prestaConnector::getAdressesByName(QString user,QString host,QString customerName)
{
    QByteArray array;

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl(QString("http://%1/api/addresses/?display=full&filter[lastname]=[%2]").arg(host).arg(customerName)));
    QEventLoop q;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &q, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    q.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting adresses");
        tT.stop();
        return array;
    }
    else
    {
           emit debuggingInfo("Getting adresses done");
           if (nreply->error() > 0) {
               emit debuggingInfo("GET adresses FAILED Error number = " + nreply->errorString());
               return array;
           }
           else {
               array = nreply->readAll();
               return array;
           }
    }
}
QByteArray prestaConnector::saveAllAdressesToFile(QString user,QString host,QString filename)
{
    QByteArray array;
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl(QString("http://%1/api/addresses/?display=full").arg(host)));
    QEventLoop q;
    QTimer tT;
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &q, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    q.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error getting adresses");
        tT.stop();
        return array;
    }
    else
    {
           emit debuggingInfo("Getting adresses done");
           if (nreply->error() > 0) {
               emit debuggingInfo("GET adresses FAILED Error number = " + nreply->errorString());
               return array;
           }
           else {
               array = nreply->readAll();
               arrayToFile(array,filename);
               return array;
           }
    }
}
void prestaConnector::arrayToFile(QByteArray &array, QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Could not open file";
        return;
    }
    qDebug()<<QFileInfo(file).absolutePath();
    QTextStream stream( &file ); // we will serialize the data into the file
    stream << array;
    file.close();
}

void prestaConnector::addressesToStruct(QByteArray array,QMap<QString,prestaConnector::adress> & adressList)
{
    QDomDocument doc;
    doc.setContent(array);
    QDomNodeList nodes = doc.elementsByTagName("address");
    for(int x=0;x<nodes.size();++x)
    {
        adress adr;
        adr.ID=nodes.at(x).firstChildElement("id").text();
        adr.VAT=nodes.at(x).firstChildElement("vat_number").text();
        adr.firstName=nodes.at(x).firstChildElement("firstname").text();
        adr.lastName=nodes.at(x).firstChildElement("lastname").text();
        adr.postalCode=nodes.at(x).firstChildElement("postcode").text();
        adr.adress1=nodes.at(x).firstChildElement("address1").text();
        adr.adress2=nodes.at(x).firstChildElement("address2").text();
        adr.country=countries.value(nodes.at(x).firstChildElement("id_country").text(),"Country unknown");
        adr.state=states.value(nodes.at(x).firstChildElement("id_state").text(),"State unknown");
        adr.city=nodes.at(x).firstChildElement("city").text();
        adr.company=nodes.at(x).firstChildElement("company").text();
        adressList.insert(adr.ID,adr);
    }
}
void prestaConnector::ordersToStruct(QByteArray array,QMap<QString,prestaConnector::order> & orderList)
{
    QDomDocument doc;
    doc.setContent(array);
    QDomNodeList nodes = doc.elementsByTagName("order");
    for(int x=0;x<nodes.size();++x)
    {
        order ord;
        ord.ID=nodes.at(x).firstChildElement("id").text();
        ord.address_delivery=nodes.at(x).firstChildElement("id_address_delivery").text();
        ord.address_invoice=nodes.at(x).firstChildElement("id_address_invoice").text();
        ord.total_paid=nodes.at(x).firstChildElement("total_paid_real").text();
        ord.total_shipping=nodes.at(x).firstChildElement("total_shipping").text();
        ord.tracking=nodes.at(x).firstChildElement("shipping_number").text();
        QDomNodeList ordernodes = nodes.at(x).firstChildElement("associations").firstChildElement("order_rows").elementsByTagName("order_row");
        for(int xx=0;xx<ordernodes.size();++xx)
        {
            product pro;
            pro.name=ordernodes.at(xx).firstChildElement("product_name").text();
            pro.quantity=ordernodes.at(xx).firstChildElement("product_quantity").text();
            ord.products.append(pro);
        }
        orderList.insert(ord.ID,ord);
    }
}
void prestaConnector::setTracking(QString user,QString host,QString orderID,QString trackingNumber)
{
    setOrderField(user,host,orderID,"shipping_number",trackingNumber);
}
void prestaConnector::setOrderField(QString user,QString host,QString orderID,QString field,QString value)
{
   // field="gift";
   // value="1";
    QEventLoop loop;
    QTimer tT;
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Basic " +
                         QByteArray(QString("%1:%2").arg(user).arg(PASSWORD).toAscii()).toBase64());
    request.setUrl(QUrl(QString("http://%0/api/orders/%1").arg(host).arg(orderID)));
    tT.setSingleShot(true);
    connect(m_network, SIGNAL(finished(QNetworkReply*)),
            &loop, SLOT(quit()));
    connect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
    nreply = m_network->get(request);
    tT.start(30000);
    loop.exec();
    if(!tT.isActive()){
        emit debuggingInfo("Error setting order field");
        tT.stop();
        return;
    }
    tT.stop();
    if (nreply->error() > 0) {
        emit debuggingInfo("GET FAILED Error number = " + nreply->errorString());
    }
    else {
        QByteArray data = nreply->readAll();
        QDomDocument doc;
        doc.setContent(data);
        QDomNodeList nodes = doc.elementsByTagName("order");
        for(int x=0;x<nodes.size();++x)
        {
            if(nodes.at(x).firstChildElement("id").text().contains(orderID))
            {
                QDomElement element=nodes.at(x).firstChildElement(field);

                QDomElement newNodeTag = doc.createElement(QString(field));
                QDomText newNodeText = doc.createTextNode(value);
                newNodeTag.appendChild(newNodeText);

                // replace existing node with new node
               // nodes.at(x).removeChild(element);
                nodes.at(x).replaceChild(newNodeTag,element);

            }
        }

        QByteArray array=doc.toByteArray();
        qDebug()<<array;
        QEventLoop loop2;
        QTimer timer;
        connect(nreply, SIGNAL(finished()),
                &loop2, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &loop2, SLOT(quit()));
        nreply=m_network->put(request,array);
        timer.start(1000);
        loop2.exec();
        qDebug()<<"RESPONSE:"<<nreply->readAll();
        if(!tT.isActive()){
            emit debuggingInfo("No response, should mean OK");
            tT.stop();
            return;
        }
        tT.stop();
    }
}
void prestaConnector::renderAutoA4Preview(QString fontName,int fontSize,int baseX,int baseY,int offSet,bool sender,QList<prestaConnector::adress> & adresses,QPrinter *printer)
{
    QPainter painter;
    int height=printer->paperRect().height();
    int witdh=printer->paperRect().width();
    QFont serifFont(fontName, fontSize, QFont::Bold);
    painter.begin(printer);
    painter.setFont(serifFont);
    int vcount=0;
    int hcount=0;
    if (offSet>15) offSet=0;
    int saltohoriz=witdh/2;
    int saltoregisto=height/8;
    int xbasestatic=baseX;
    int ybasestatic=baseY;
    int x=xbasestatic;
    if(offSet>7)
    {
        x=xbasestatic+saltohoriz;
        hcount=1;
        offSet=offSet-8;
    }
    vcount=offSet;
    int ybase=ybasestatic+saltoregisto*offSet;
    bool newPage=false;
    foreach(prestaConnector::adress adr,adresses)
    {
        if(newPage)
        {
            printer->newPage();
            newPage=false;
        }
        int y=ybase;
        serifFont=QFont(fontName, fontSize, QFont::Bold);
        painter.setFont(serifFont);
        QFontMetrics fm(serifFont,printer);
        QString toFrom;
        if(sender)
        {
            toFrom="FROM:";
        }
        else
        {
            toFrom="TO:";
        }
        painter.drawText(x,y,toFrom);
        serifFont=QFont(fontName, fontSize, QFont::Normal);
        painter.setFont(serifFont);
        painter.drawText(x+fm.width(toFrom),y,adr.lastName+", "+adr.firstName);
        fm=QFontMetrics(serifFont,printer);
        if(!adr.company.isEmpty())
        {
            y=y+fm.ascent();
            painter.drawText(x,y,adr.company);
        }
        int xx=fm.width(adr.adress1);
        if(xx>witdh/2)
        {
            serifFont.setPointSize(fontSize-3);
            painter.setFont(serifFont);
        }
        fm=QFontMetrics(serifFont,printer);
        y=y+fm.ascent();
        painter.drawText(x,y,adr.adress1);
        serifFont.setPointSize(fontSize);
        painter.setFont(serifFont);
        y=y+fm.ascent();
        if(!adr.adress2.isEmpty())
        {
            painter.drawText(x,y,adr.adress2);
            fm=QFontMetrics(serifFont,printer);
            y=y+fm.ascent();
        }
        painter.drawText(x,y,adr.postalCode+" "+adr.city);
        fm=QFontMetrics(serifFont,printer);
        y=y+fm.ascent();
        painter.drawText(x,y,QString(adr.state+" "+adr.country).trimmed());
        ybase+=saltoregisto;
        ++vcount;
        if(vcount==8)
        {
            if(hcount==1)
            {
                newPage=true;
                ybase=ybasestatic;
                x=xbasestatic;
                hcount=0;
                vcount=0;
            }
            else
            {
                hcount=1;
                ybase=ybasestatic;
                x=xbasestatic+saltohoriz;
                vcount=0;
            }
        }
    }
    painter.end();
}
void prestaConnector::renderAutoBrotherPreview(QString fontName,int fontSize,int baseX,int baseY, prestaConnector::adress sender,QList<prestaConnector::adress> & adresses,QPrinter *printer)
{
    QPainter painter;
    int height=printer->paperRect().height();
    int witdh=printer->paperRect().width();
    painter.begin(printer);

    prestaConnector::adress currentAdr;
    bool isFirst=true;

    foreach(prestaConnector::adress adr,adresses)
    {
        int receiverOffsetX=120;// TESTAR
        int receiverOffsetY=100;
        int mbaseX=baseX;
        if(!isFirst)
            printer->newPage();
        else
            isFirst=false;
        QFont serifFont(fontName, fontSize, QFont::Bold);//size
        painter.setFont(serifFont);
        QFontMetrics fm=QFontMetrics(serifFont,printer);
        int m_fontsize;
        int comulativeY=baseY;
        int comulativeX=mbaseX;
        int xOffSet=0;
        for (int x=0;x<2;++x)
        {
            if(x==0)
            {
                m_fontsize=fontSize-5;
                serifFont.setPointSize(m_fontsize);
                painter.setFont(serifFont);
                fm=QFontMetrics(serifFont,printer);
                xOffSet=0;
                currentAdr=sender;
                painter.drawText(comulativeX,comulativeY,"FROM:");
                comulativeX=comulativeX+fm.width("FROM:");
            }
            else
            {
                m_fontsize=fontSize;
                mbaseX=0;
                serifFont.setPointSize(m_fontsize);
                painter.setFont(serifFont);
                fm=QFontMetrics(serifFont,printer);
                xOffSet=receiverOffsetX;
                comulativeX=0;
                comulativeY=comulativeY+receiverOffsetY;
                currentAdr=adr;
                serifFont.setBold(true);
                painter.setFont(serifFont);
                painter.drawText(xOffSet+comulativeX,comulativeY,"TO:");
                comulativeX=comulativeX+fm.width("TO:");
            }
            serifFont.setBold(false);
            painter.setFont(serifFont);
            fm=QFontMetrics(serifFont,printer);
            painter.drawText(xOffSet+comulativeX,comulativeY,currentAdr.lastName+", "+currentAdr.firstName);
            comulativeY=comulativeY+fm.ascent();
            if(!currentAdr.company.isEmpty())
            {
                painter.drawText(xOffSet+mbaseX,comulativeY,currentAdr.company);
                comulativeY=comulativeY+fm.ascent();
            }

            if(fm.width(adr.adress1)>witdh)
            {
                serifFont.setPointSize(m_fontsize-3);//size
                painter.setFont(serifFont);
                fm=QFontMetrics(serifFont,printer);
            }

            painter.drawText(xOffSet+mbaseX,comulativeY,currentAdr.adress1);
            comulativeY=comulativeY+fm.ascent();
            serifFont.setPointSize(m_fontsize);
            painter.setFont(serifFont);
            fm=QFontMetrics(serifFont,printer);

            if(!currentAdr.adress2.isEmpty())
            {
                painter.drawText(xOffSet+mbaseX,comulativeY,currentAdr.adress2);
                comulativeY=comulativeY+fm.ascent();
            }
            painter.drawText(xOffSet+mbaseX,comulativeY,currentAdr.postalCode+" "+currentAdr.city);
            comulativeY=comulativeY+fm.ascent();
            if(currentAdr.state!="State unknown")
                painter.drawText(xOffSet+mbaseX,comulativeY,QString(currentAdr.state+" "+currentAdr.country).trimmed());
            else
                painter.drawText(xOffSet+mbaseX,comulativeY,QString(currentAdr.country).trimmed());
        }
    }
    painter.end();
}
void prestaConnector::renderRegistosPreview(QString fontName,int fontSize,int baseX,int baseY, prestaConnector::adress sender,QList<prestaConnector::adress> & adresses,QPrinter *printer)
{
    QPainter painter;
    int height=printer->paperRect().height();
    int witdh=printer->paperRect().width();
    QFont serifFont(fontName, fontSize-5, QFont::Normal);
    QFontMetrics fm=QFontMetrics(serifFont,printer);
    int xxOffset=baseX;
    int yyOffset=baseY+170;
    int nameYPos=200;
    int adrYPos=245;
    int postalCodePos=310;
    int charSpace=23;
    int postalCodeDiv=10;
    int lineOffset=fm.height()+1;
    painter.begin(printer);
    painter.setFont(serifFont);
    bool newPage=false;
    foreach(adress adr,adresses)
    {
        if(newPage)
        {
            printer->newPage();
            newPage=false;
        }
        painter.drawText(baseX,nameYPos+baseY,adr.lastName+", "+adr.firstName);
        painter.drawText(baseX,adrYPos+baseY,adr.adress1);
        painter.drawText(baseX,adrYPos+baseY+lineOffset,adr.adress2);
        int pc=0;
        int xxx=0;
        painter.drawText(pc+baseX+200,postalCodePos+baseY,adr.postalCode);
        QString compose;
        compose=adr.city;
        if(!adr.state.isEmpty() && !adr.state.contains("State unknown"))
            compose=compose+", "+adr.state;
        compose=compose+", "+adr.country;
        painter.drawText(baseX+200+fm.width(adr.postalCode)+5,postalCodePos+baseY,compose);
        painter.drawText(xxOffset,nameYPos+yyOffset,sender.lastName+", "+sender.firstName);
        painter.drawText(xxOffset,adrYPos+yyOffset,sender.adress1);
        painter.drawText(xxOffset,adrYPos+yyOffset+lineOffset,sender.adress2);
        pc=0;
        xxx=0;
        for(int xx=0;xx<sender.postalCode.length();++xx)
        {
            if(xx>3)
                pc=postalCodeDiv;
            if(sender.postalCode[xx]!='-')
            {
                painter.drawText(pc+baseX+charSpace*xxx+12,postalCodePos+yyOffset,QString(sender.postalCode[xx]));
                ++xxx;
            }
        }
        compose=sender.city;
        if(!sender.state.isEmpty())
            compose=compose+", "+sender.state;
        compose=compose+", "+sender.country;
        painter.drawText(baseX+180,postalCodePos+yyOffset,compose);
        painter.drawText(483,524,"X");
        newPage=true;
    }
    painter.end();
}
//void prestaConnector::renderRegistosPreview(QString fontName,int fontSize,int baseX,int baseY, prestaConnector::adress sender,QList<prestaConnector::adress> & adresses,QPrinter *printer)
//{
//    QPainter painter;
//    int height=printer->paperRect().height();
//    int witdh=printer->paperRect().width();
//    QFont serifFont(fontName, fontSize-5, QFont::Normal);
//    QFontMetrics fm=QFontMetrics(serifFont,printer);
//    qDebug()<<"f"<<fm.height();
//    int xxOffset=baseX;
//    int yyOffset=baseY+850;
//    int nameYPos=250;
//    int adrYPos=480;
//    int postalCodePos=780;
//    int charSpace=120;
//    int postalCodeDiv=40;
//    int lineOffset=80;
//    painter.begin(printer);
//    painter.setFont(serifFont);
//    bool newPage=false;
//    foreach(adress adr,adresses)
//    {
//        if(newPage)
//        {
//            printer->newPage();
//            newPage=false;
//        }
//        painter.drawText(baseX,nameYPos+baseY,adr.lastName+", "+adr.firstName);
//        painter.drawText(baseX,adrYPos+baseY,adr.adress1);
//        painter.drawText(baseX,adrYPos+baseY+lineOffset,adr.adress2);
//        int pc=0;
//        int xxx=0;
//        painter.drawText(pc+baseX+900,postalCodePos+baseY,adr.postalCode);
//        QString compose;
//        compose=adr.city;
//        if(!adr.state.isEmpty() && !adr.state.contains("State unknown"))
//            compose=compose+", "+adr.state;
//        compose=compose+", "+adr.country;
//        painter.drawText(baseX+1300,postalCodePos+baseY,compose);
//        painter.drawText(xxOffset,nameYPos+yyOffset,sender.lastName+", "+sender.firstName);
//        painter.drawText(xxOffset,adrYPos+yyOffset,sender.adress1);
//        painter.drawText(xxOffset,adrYPos+yyOffset+lineOffset,sender.adress2);
//        pc=0;
//        xxx=0;
//        for(int xx=0;xx<sender.postalCode.length();++xx)
//        {
//            if(xx>3)
//                pc=postalCodeDiv;
//            if(sender.postalCode[xx]!='-')
//            {
//                painter.drawText(pc+baseX+charSpace*xxx-50,postalCodePos+yyOffset,QString(sender.postalCode[xx]));
//                ++xxx;
//            }
//        }
//        compose=sender.city;
//        if(!sender.state.isEmpty())
//            compose=compose+", "+sender.state;
//        compose=compose+", "+sender.country;
//        painter.drawText(baseX+800,postalCodePos+yyOffset,compose);
//        newPage=true;
//    }
//    painter.end();
//}
void prestaConnector::filterOrders(QString currentState,QByteArray & data,QStringList & deliveryAdressList,QStringList & invoiceAdressList,QStringList & ordersList)
{
        QDomDocument doc;
        doc.setContent(data);
        QDomNodeList nodes = doc.elementsByTagName("current_state");
        if (nodes.size() > 0) {
            for(int x=0;x<nodes.size();++x)
            {
                if(nodes.at(x).toElement().text()==currentState)//CURRENT_STATE
                {
                    deliveryAdressList.append(nodes.at(x).parentNode().firstChildElement("id_address_delivery").text());
                    invoiceAdressList.append(nodes.at(x).parentNode().firstChildElement("id_address_invoice").text());
                    ordersList.append(nodes.at(x).parentNode().firstChildElement("id").text());
                }
            }
            emit debuggingInfo(QString("Orders to process:%0 IDs:%1").arg(deliveryAdressList.count()).arg(ordersList.join(", ")));
    }
}
void prestaConnector::checkDelivery(QString user,QString host)
{
    QStringList adressList;
    QStringList invoiceList;
    QStringList orderList;
    QMap<QString,order> mapOrder;
    QByteArray array=getOrders(user,host,QStringList(),true);
    filterOrders(SHIPPED,array,adressList,invoiceList,orderList);
    ordersToStruct(array,mapOrder);
    foreach(QString ord,orderList)
    {
        order order=mapOrder.value(ord);
        QString tracking=order.tracking;
        emit debuggingInfo("Checking delivery status of order ID:"+ord);
        QNetworkRequest request;
        request.setUrl(QUrl(QString("http://www.ctt.pt/feapl_2/app/open/objectSearch/ctt_feapl_132col-cttObjectSearch.jspx?objects=&showResults=true&pesqObjecto.objectoId=%0").arg(tracking)));
        QEventLoop loop;
        QTimer tT;
        tT.setSingleShot(true);
        connect(m_network, SIGNAL(finished(QNetworkReply*)),
                &loop, SLOT(quit()));
        connect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
        tT.start(500);
        loop.exec();
        nreply = m_network->get(request);
        tT.start(30000);
        loop.exec();
        disconnect(m_network, SIGNAL(finished(QNetworkReply*)),
                &loop, SLOT(quit()));
        disconnect(&tT, SIGNAL(timeout()), &loop, SLOT(quit()));
        if(!tT.isActive()){
            emit debuggingInfo("Error checking tracking status");
            tT.stop();
        }
        else
        {
               emit debuggingInfo("checking tracking status");
               tT.stop();
               if (nreply->error() > 0) {
                   emit debuggingInfo(QString("GET FAILED Error number = %0").arg(nreply->errorString()));
               }
               else {
                   QByteArray ret = nreply->readAll();
                   QString string(ret);
                   if(string.contains("ENTREGA CONSEGUIDA"))
                   {
                       if(true)//QMessageBox::Yes==QMessageBox::question(NULL,"Confirm delivery set",QString("Order %0 was delivered").arg(ord),QMessageBox::Yes,QMessageBox::No))
                       {
                            emit debuggingInfo("Tracking page confirmed delivery, setting order now");
                            setOrderField(user,host,ord,"current_state",DELIVERED);
                       }
                   }
                   else
                   {
                       emit debuggingInfo("Tracking page did NOT confirm delivery");
                   }
               }
        }
    }
}
