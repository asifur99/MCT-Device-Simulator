#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ctime>
#include <QMainWindow>
#include <QMessageBox>
#include <qslider.h>
#include <QString>
#include <qstring.h>
#include <QCheckBox>
#include <qcheckbox.h>
#include "allergy.h"
#include "battery.h"
#include "frequency_1hz.h"
#include "frequency_10hz.h"
#include "frequency_20hz.h"
#include "frequency_60hz.h"
#include "bioting.h"
#include "pain.h"
#include "intPain.h"

/*This is the mainWindow header file, this file contains all declarations that will be used in the QT framework
 *This header file also contains the declarations of the slots() used in the QT framework being used by the program
  */
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    allergy allergyObj;
    bioting biotingObj;
    battery batteryObj;
    pain painObj;
    intPain intPainObj;
    frequency_1hz freq_1hz_Obj;
    frequency_10hz freq_10hz_Obj;
    frequency_20hz freq_20hz_Obj;
    frequency_60hz freq_60hz_Obj;


    time_t tt;
    struct tm * ti;

    void setPageValues();
    void movePages();
    void buttonSetup();
    void setProgramTimerInterval(int);
    void resetFunc();

//Slots being used in the mainWindow.cpp file that control the QT framework
private slots:

    void nextFrame();
    void downValue();
    void upValue();
    void prevFrame();
    void powerOn();
    void backToMenu();
    void allergyFunc();
    void biotingFunc();
    void painFunc();
    void intPainFunc();
    void freq1HzFunc();
    void freq10HzFunc();
    void freq20HzFunc();
    void freq60HzFunc();
    void batteryFunc();
    void rightButtonFunc();
    void leftButtonFunc();

};

#endif // MAINWINDOW_H
