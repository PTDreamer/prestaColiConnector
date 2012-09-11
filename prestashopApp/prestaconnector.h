#ifndef PRESTACONNECTOR_H
#define PRESTACONNECTOR_H
#include <QString>
#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QStringList>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QFile>
#include <QDebug>

class prestaConnector:public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *nreply;
    QMap<QString,QString> countries;
    QMap<QString,QString> states;
public:
    struct adress
    {
        QString VAT;
        QString ID;
        QString firstName;
        QString lastName;
        QString adress1;
        QString adress2;
        QString postalCode;
        QString country;
        QString state;
        QString city;
        QString company;
    };
    struct product
    {
        QString name;
        QString quantity;
    };
    struct order
    {
        QString ID;
        QString address_delivery;
        QString address_invoice;
        QString total_paid;
        QString total_shipping;
        QList<product> products;
        QString tracking;
    };

    prestaConnector(QObject * parent=0);
    void loadCountriesAndStates(QString user,QString host);
    QByteArray retrieveCountries(QString user, QString host,QMap<QString, QString> &countries);
    QByteArray retrieveStates(QString user, QString host, QMap<QString, QString> &states);
    QByteArray getOrders(QString user, QString host, QStringList ids=QStringList(), bool validOnly=true,QString customFilter=QString());
    QByteArray getAdresses(QString user, QString host, QStringList ids, bool all=false);
    void arrayToFile(QByteArray & array,QString file);
    void addressesToStruct(QByteArray array, QMap<QString, prestaConnector::adress> &adressList);
    void setTracking(QString user, QString host, QString orderID, QString trackingNumber);
    void setOrderField(QString user, QString host, QString orderID, QString field, QString value);
    void renderAutoA4Preview(QString fontName, int fontSize, int baseX, int baseY,int offSet, bool sender, QList<prestaConnector::adress> &adresses, QPrinter *printer);
    void renderAutoBrotherPreview(QString fontName, int fontSize, int baseX, int baseY, prestaConnector::adress sender, QList<prestaConnector::adress> &adresses, QPrinter *printer);
    void renderRegistosPreview(QString fontName, int fontSize, int baseX, int baseY, prestaConnector::adress sender, QList<prestaConnector::adress> &adresses, QPrinter *printer);
    QByteArray retrieveTaxes(QString user, QString host);
    QByteArray saveAllAdressesToFile(QString user, QString host, QString filename);
    void filterOrders(QString currentState, QByteArray &data, QStringList &adressList,QStringList & invoiceAdressList, QStringList &ordersList);
    void ordersToStruct(QByteArray array, QMap<QString, prestaConnector::order> &orderList);
    QByteArray getAdressesByName(QString user, QString host, QString customerName);
    void checkDelivery(QString user, QString host);
signals:
    void debuggingInfo(QString);
};

#endif // PRESTACONNECTOR_H
