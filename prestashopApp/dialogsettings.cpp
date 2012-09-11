#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include <QPrinterInfo>

DialogSettings::DialogSettings(QSettings * settings,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    m_settings=settings;
    ui->autoPrinter->clear();
    ui->regPrinter->clear();
    ui->coliPrinter->clear();

    ui->zbar->setText(m_settings->value("zbar","").toString());

    foreach(QPrinterInfo info,QPrinterInfo::availablePrinters())
    {
        ui->autoPrinter->addItem(info.printerName());
        ui->regPrinter->addItem(info.printerName());
        ui->coliPrinter->addItem(info.printerName());
    }

    ui->autoBaseX->setValue(m_settings->value("autoBaseX",200).toInt());
    ui->autoBaseY->setValue(m_settings->value("autoBaseY",180).toInt());
    ui->autoFontSize->setValue(m_settings->value("autoFontSize",15).toInt());
    ui->autoPrintA4->setChecked(m_settings->value("autoPrintA4",false).toBool());
    ui->autoPrinter->setCurrentIndex(ui->autoPrinter->findText(m_settings->value("autoPrinter","none").toString()));

    ui->regBaseX->setValue(m_settings->value("regBaseX",370).toInt());
    ui->regBaseY->setValue(m_settings->value("regBaseY",840).toInt());
    ui->regFontSize->setValue(m_settings->value("regFontSize",15).toInt());
    ui->regPrinter->setCurrentIndex(ui->regPrinter->findText(m_settings->value("regPrinter","none").toString()));

    ui->coliCustomersFile->setText(m_settings->value("coliCustomersFile","").toString());
    ui->coliOrdersFile->setText(m_settings->value("coliOrdersFile","").toString());
    ui->coliOutputFile->setText(m_settings->value("coliOutputFile","").toString());
    ui->coliPath->setText(m_settings->value("coliPath","").toString());    
    ui->countriesFilename->setText(m_settings->value("coliCountriesFile","").toString());
    ui->taxesFilename->setText(m_settings->value("coliTaxesFile","").toString());
    ui->coliDB->setText(m_settings->value("coliDB","").toString());
    ui->coliLicense->setText(m_settings->value("coliLicense","").toString());

    ui->coliPrinter->setCurrentIndex(ui->coliPrinter->findText(m_settings->value("coliPrinter","none").toString()));

    ui->fontComboBox->setCurrentIndex(ui->fontComboBox->findText(m_settings->value("font","Arial").toString()));
    ui->fontBaseSize->setValue(m_settings->value("fontSize",15).toInt());
    ui->leHost->setText(m_settings->value("host","").toString());
    ui->leLogin->setText(m_settings->value("login","").toString());
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_accepted()
{
    m_settings->setValue("autoBaseX",ui->autoBaseX->value());
    m_settings->setValue("autoBaseY",ui->autoBaseY->value());
    m_settings->setValue("autoFontSize",ui->autoFontSize->value());
    m_settings->setValue("autoPrintA4",ui->autoPrintA4->isChecked());
    m_settings->setValue("autoPrinter",ui->autoPrinter->currentText());
    m_settings->setValue("zbar",ui->zbar->text());

    m_settings->setValue("regBaseX",ui->regBaseX->value());
    m_settings->setValue("regBaseY",ui->regBaseY->value());
    m_settings->setValue("regFontSize",ui->regFontSize->value());
    m_settings->setValue("regPrinter",ui->regPrinter->currentText());

    m_settings->setValue("font",ui->fontComboBox->currentText());
    m_settings->setValue("fontSize",ui->fontBaseSize->value());
    m_settings->setValue("host",ui->leHost->text());
    m_settings->setValue("login",ui->leLogin->text());

    m_settings->setValue("coliCustomersFile",ui->coliCustomersFile->text());
    m_settings->setValue("coliOrdersFile",ui->coliOrdersFile->text());
    m_settings->setValue("coliOutputFile",ui->coliOutputFile->text());
    m_settings->setValue("coliCountriesFile",ui->countriesFilename->text());
    m_settings->setValue("coliTaxesFile",ui->taxesFilename->text());

    m_settings->setValue("coliDB",ui->coliDB->text());
    m_settings->setValue("coliLicense",ui->coliLicense->text());

    m_settings->setValue("coliPath",ui->coliPath->text());
    m_settings->setValue("coliPrinter",ui->coliPrinter->currentText());


}
