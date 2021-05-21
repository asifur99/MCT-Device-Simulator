#include "battery.h"
#include <QDebug>

//Contructor for the allergy class
battery::battery(){
    batteryTimer = 100;
    timer = new QTimer(); //initilization of QT timer

}

//setTimer() function.
void battery::setTimer(){
    batteryTimer--;
}

//getTimer() function.
int battery::getTimer(){
    return batteryTimer;
}

//setTimerSlider
void battery::setTimerSlider(int b){
    batteryTimer = b;
}



