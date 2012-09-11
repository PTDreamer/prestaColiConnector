#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSettings(QSettings * settings, QWidget *parent=0);
    ~DialogSettings();
    int getAutoBaseX(){return m_settings->value("autoBaseX",200).toInt();}
    int getAutoBaseY(){return m_settings->value("autoBaseY",180).toInt();}
    int getAutoFontSize(){return m_settings->value("autoFontSize",15).toInt();}
    bool getAutoPrintA4(){return m_settings->value("autoPrintA4",false).toBool();}
    QString getAutoPrinter(){return m_settings->value("autoPrinter","none").toString();}
    QString getZBarPath(){return m_settings->value("zbar","").toString();}

    int getRegBaseX(){return m_settings->value("regBaseX",370).toInt();}
    int getRegBaseY(){return m_settings->value("regBaseY",840).toInt();}
    int getRegFontSize(){return m_settings->value("regFontSize",15).toInt();}
    QString getRegPrinter(){return m_settings->value("regPrinter","none").toString();}

    QString getColiCustomersFile(){return m_settings->value("coliCustomersFile","").toString();}
    QString getColiOrdersFile(){return m_settings->value("coliOrdersFile","").toString();}
    QString getColiOutputFile(){return m_settings->value("coliOutputFile","").toString();}
    QString getColiCountriesFile(){return m_settings->value("coliCountriesFile","").toString();}
    QString getColiTaxesFile(){return m_settings->value("coliTaxesFile","").toString();}
    QString getColiDB(){return m_settings->value("coliDB","").toString();}
    QString getColiLicense(){return m_settings->value("coliLicense","").toString();}

    QString getColiPath(){return m_settings->value("coliPath","").toString();}
    QString getColiPrinter(){return m_settings->value("coliPrinter","none").toString();}

    QString getFont(){return m_settings->value("font","Arial").toString();}
    int getFontSize(){return m_settings->value("fontSize",15).toInt();}
    QString getHost(){return m_settings->value("host","").toString();}
    QString getLogin(){return m_settings->value("login","").toString();}
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogSettings *ui;
    QSettings * m_settings;
};

#endif // DIALOGSETTINGS_H
