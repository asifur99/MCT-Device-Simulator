#ifndef BATTERY_H
#define BATTERY_H
#include <QTimer>
#include <QDateTime>

/*header file for battery class, this class contains the following:
 * a battery class contructor
 * a setTimer() function which sets the value of the QT timer, which is a private data member batteryTimer
 * a getTimer() function which gets the value of the QT timer, which is a private data member batteryTimer
 * a setTimerSlider function which takens in an int, this function sets the value of the QTBattery Slider
 */

class battery{

    public:
        battery();
        void setTimer();
        int getTimer();
        void setTimerSlider(int);
        QTimer *timer;


    private:
        int batteryTimer;

};
#endif // BATTERY_H
