#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCommonStyle>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set up connections between specific slots and signals and assiciate every Ui button
    //and object to a its specific function, this function contains the functinality that
    //will be implimented when this button or object is called.
    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(upValue()));
    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(downValue()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(nextFrame()));
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(prevFrame()));
    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(powerOn()));
    connect(ui->menuButton, SIGNAL(clicked()), this, SLOT(backToMenu()));
    connect(ui->rightButton, SIGNAL(clicked()), this, SLOT(rightButtonFunc()));
    connect(ui->leftButton, SIGNAL(clicked()), this, SLOT(leftButtonFunc()));
    connect(allergyObj.timer, SIGNAL(timeout()), this, SLOT(allergyFunc()));
    connect(biotingObj.timer, SIGNAL(timeout()), this, SLOT(biotingFunc()));
    connect(painObj.timer, SIGNAL(timeout()), this, SLOT(painFunc()));
    connect(intPainObj.timer, SIGNAL(timeout()), this, SLOT(intPainFunc()));
    connect(freq_1hz_Obj.timer, SIGNAL(timeout()), this, SLOT(freq1HzFunc()));
    connect(freq_10hz_Obj.timer, SIGNAL(timeout()), this, SLOT(freq10HzFunc()));
    connect(freq_20hz_Obj.timer, SIGNAL(timeout()), this, SLOT(freq20HzFunc()));
    connect(freq_60hz_Obj.timer, SIGNAL(timeout()), this, SLOT(freq60HzFunc()));
    connect(batteryObj.timer, SIGNAL(timeout()), this, SLOT(batteryFunc()));

    QCommonStyle style; //set the style for the QT GUI
    buttonSetup(); //
    //Initilizing values for power, frequency, battery, electrodes and save therapy session checkboxes in the GUI
    ui->powerSlider->setRange(0,100);
    ui->freqencySlider->setRange(1,200);
    ui->batterySlider->setValue(batteryObj.getTimer());
    ui->batteryPercentageLabel->setText(QString::number(ui->batterySlider->value()+1).rightJustified(4)+"%");
    ui->touchSensor->setChecked(1);
    ui->saveTherapy->setChecked(1);
}

//destructor for the MainWindow()
MainWindow::~MainWindow()
{
    delete ui; //delete ui upon exiting program
}


//This function is executed when the user presses "ok" button in the GUI
void MainWindow::nextFrame(){
    if(ui->stackedWidget->currentIndex() != 0){ //if the stackWidget index is not 0 (switched on)

        setPageValues(); //run the setPageValues() function (Line 687)
        movePages(); //run the movePages() function (Line 730)

        //Alert message will be displayed if a selected option is not implemented.
        if (ui->mainMenuScreen->currentRow() > 2 && ui->mainMenuScreen->currentRow() != 6){QMessageBox::about(this, "Alert", "This option is not Enabled");}
        if (ui->programsScreen->currentRow() > 4){QMessageBox::about(this, "Alert", "This option is not Enabled");}
    }
}


//This function is executed when the user presses "return" button in the GUI
void MainWindow::prevFrame(){
    if(ui->stackedWidget->currentIndex() == 1){ //prevents the user from going to the off page by pressing return button from the MainMenu
        ui->stackedWidget->setCurrentIndex(1);
    }
    //setting the row of programScreen to its maximum index and updating stackWidget display to mainMenu at index = 1
    else if(ui->stackedWidget->currentIndex() == 2){
        ui->programsScreen->setCurrentRow(ui->programsScreen->count());
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(ui->stackedWidget->currentIndex() == 3){ //return from Allergy page to ProgramScreen at index = 2
        ui->stackedWidget->setCurrentIndex(2);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save Allergy therapy to History Screen
            time (&tt); //create time object using built-in C++ time library
            ti = localtime(&tt); //save current time to variable ti
            //adding information about therapy session to therapyList
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Allergy 77hz");
            ui->therapyList->addItem(" Power Level: " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + QString::number(10 - ui->allergyTimer->text().toInt()) + " sec");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc(); //call reset function
    }
    //setting the row of freqScreen to its maximum index and updating stackWidget display to mainMenu at index = 1
    else if(ui->stackedWidget->currentIndex() == 4){
        ui->freqScreen->setCurrentRow(ui->freqScreen->count());
        ui->stackedWidget->setCurrentIndex(1);
    }

    else if(ui->stackedWidget->currentIndex() == 5){ //return from frequency 1,0 to 9.9Hz page to ProgramScreen at index = 4
        ui->stackedWidget->setCurrentIndex(4);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save frequency 1,0 to 9.9Hz therapy to History Screen
            time (&tt); //save current time to variable ti
            ti = localtime(&tt); //save current time to variable ti
            //adding information about therapy session to therapyList
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Frequency 1,0-9.9hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + ui->frequency1hz_minutes->text() + ":" +ui->frequency1hz_seconds->text());
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc(); //call reset function
    }

    else if(ui->stackedWidget->currentIndex() == 8){ //return from frequency 10hz page to ProgramScreen at index = 4
        ui->stackedWidget->setCurrentIndex(4);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save frequency 10hz therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Frequency 10hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + ui->frequency10hz_minutes->text() + ":" +ui->frequency10hz_seconds->text());
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 9){ //return from frequency 20hz page to ProgramScreen at index = 4
        ui->stackedWidget->setCurrentIndex(4);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save frequency 20hz therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Frequency 20hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + ui->frequency20hz_minutes->text() + ":" +ui->frequency20hz_seconds->text());
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 10){ //return from frequency 60hz page to ProgramScreen at index = 4
        ui->stackedWidget->setCurrentIndex(4);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save frequency 60hz therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Frequency 60hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + ui->frequency60hz_minutes->text() + ":" +ui->frequency60hz_seconds->text());
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 11){ //return from bioting to program screen at index = 2
        ui->stackedWidget->setCurrentIndex(2);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save bioting therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Bioting, 100Hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + QString::number(15 - ui->biotingTimer->text().toInt()) + " sec");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 12){ //return from pain to program screen at index = 2
        ui->stackedWidget->setCurrentIndex(2);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save pain therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Pain, 120Hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + QString::number(20 - ui->painTimer->text().toInt()) + " sec");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 13){ //return from Int. Pain to program screen at index = 2
        ui->stackedWidget->setCurrentIndex(2);
        if(ui->saveTherapy->isChecked()==true){ //if save therapy checkbox == true then save Int. Pain therapy to History Screen
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Int. Pain, 90Hz");
            ui->therapyList->addItem(" Power Level " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: " + QString::number(30 - ui->intPainTimer->text().toInt()) + " sec");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    else if(ui->stackedWidget->currentIndex() == 6){ //return from History to mainMenu Page index = 1
        ui->stackedWidget->setCurrentIndex(1);
        resetFunc(); //call reset function
    }
    else{
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1); //otherwise go back back one page
    }


}

/*This function is executed when the user presses "downValue" button in the GUI
 * when the down button is clicked iterate down the list skipping the 0th index
 * as there will be a heading on the 0th index that is not a part of the list.
 * If the user pressed down button on the last element of the list then our
 * highlighted option will appear on index 1 rather than index 0. As 0th index
 * is the heading of the list.
 */
void MainWindow::downValue(){
    if(ui->stackedWidget->currentIndex() == 1){ //condition for mainMenuScreen
        if(ui->mainMenuScreen->currentRow() <= 0){
            ui->mainMenuScreen->setCurrentRow(ui->mainMenuScreen->currentRow()+1);
        }
        if(ui->mainMenuScreen->currentRow() == (ui->mainMenuScreen->count()-1)){
            ui->mainMenuScreen->setCurrentRow(0);
        }
        ui->mainMenuScreen->setCurrentRow(ui->mainMenuScreen->currentRow()+1);
    }
    if(ui->stackedWidget->currentIndex() == 2){ //condition for programsScreen
        if(ui->programsScreen->currentRow() <= 0){
            ui->programsScreen->setCurrentRow(ui->programsScreen->currentRow()+1);
        }
        if(ui->programsScreen->currentRow() == (ui->programsScreen->count()-1)){
            ui->programsScreen->setCurrentRow(0);
        }
        ui->programsScreen->setCurrentRow(ui->programsScreen->currentRow()+1);
    }
    if(ui->stackedWidget->currentIndex() == 4){ //condition for freqScreen
        if(ui->freqScreen->currentRow() <= 0){
            ui->freqScreen->setCurrentRow(ui->freqScreen->currentRow()+1);
        }
        if(ui->freqScreen->currentRow() == (ui->freqScreen->count()-1)){
            ui->freqScreen->setCurrentRow(0);
        }
        ui->freqScreen->setCurrentRow(ui->freqScreen->currentRow()+1);
    }
    if(ui->stackedWidget->currentIndex() == 6){ //condition for historyList
        if(ui->historyList->currentRow() <= 0){
            ui->historyList->setCurrentRow(ui->historyList->currentRow()+1);
        }
        if(ui->historyList->currentRow() == 2){
            ui->historyList->setCurrentRow(0);
        }

        ui->historyList->setCurrentRow(ui->historyList->currentRow()+1);
    }
    if(ui->stackedWidget->currentIndex() == 7){ //condition for therapyList
        if(ui->therapyList->currentRow() == (ui->therapyList->count()-5)){
            ui->therapyList->setCurrentRow(0);
        }else{
            ui->therapyList->setCurrentRow(ui->therapyList->currentRow()+5);
        }
    }
}


/*This function is executed when the user presses "upValue" button in the GUI
 * when the up button is clicked iterate up the list skipping the 0th index
 * as there will be a heading on the 0th index that is not a part of the list.
 * If the user pressed up button on the first element of the list then our
 * highlighted option will appear on last index = ui->someList->count()-1 rather
 * than index = ui->someList->count(). As this index does not exists and without
 * this condition the highligher will dissapear of one iteration on the list.
 */
void MainWindow::upValue(){
    if(ui->stackedWidget->currentIndex() == 1){ //condition for mainMenuScreen
        if(ui->mainMenuScreen->currentRow() <= 1){
            //!ui->mainMenuScreen->setCurrentRow(6);
            ui->mainMenuScreen->setCurrentRow(ui->mainMenuScreen->count()-1);
        }else{
            ui->mainMenuScreen->setCurrentRow(ui->mainMenuScreen->currentRow()-1);
        }
    }
    if(ui->stackedWidget->currentIndex() == 2){ //condition for programsScreen
        if(ui->programsScreen->currentRow() <= 1){
            //!ui->programsScreen->setCurrentRow(6);
            ui->programsScreen->setCurrentRow(ui->programsScreen->count()-1);
        }else{
            ui->programsScreen->setCurrentRow(ui->programsScreen->currentRow()-1);
        }
    }
    if(ui->stackedWidget->currentIndex() == 4){ //condition for freqScreen
        if(ui->freqScreen->currentRow() <= 1){
            //!ui->programsScreen->setCurrentRow(6);
            ui->freqScreen->setCurrentRow(ui->freqScreen->count()-1);
        }else{
            ui->freqScreen->setCurrentRow(ui->freqScreen->currentRow()-1);
        }
    }
    if(ui->stackedWidget->currentIndex() == 6){ //condition for historyList
        if(ui->historyList->currentRow() <= 1){
            ui->historyList->setCurrentRow(ui->historyList->count()-1);
        }else{
            ui->historyList->setCurrentRow(ui->historyList->currentRow()-1);
        }
    }
    if(ui->stackedWidget->currentIndex() == 7){ //condition for therapyList
        if(ui->therapyList->currentRow() <= 1){
            ui->therapyList->setCurrentRow(ui->therapyList->count()-5);
        }else{
            ui->therapyList->setCurrentRow(ui->therapyList->currentRow()-5);
        }
    }
}

/*This function is executed when the power on/off button is clicked on the GUI
 * If the simulator is turned off the simulator will turn on and the battery
 * timer will start. The batterySlider will be set to its maximum value,
 * the stackWidget is also set to index = 1 (mainMenuScreen). The battery
 * percentage lable is set to the current position of the battery slider.
 * If the battery becomes 0, then the simulator will not turn on.
 * When the simulator is turned off all the values are reset using resetFunc();
 * stackWidget current index is set to 0 (the poweroff Screen)
 */
void MainWindow::powerOn(){

    if(ui->stackedWidget->currentIndex() == 0 && ui->batterySlider->value() != 0){

        batteryObj.timer->start(15000); //start the timer
        batteryObj.setTimerSlider(ui->batterySlider->value()+1);

        ui->stackedWidget->setCurrentIndex(1);
        ui->mainMenuScreen->setCurrentRow(1);
        ui->programsScreen->setCurrentRow(ui->programsScreen->count());

        ui->batteryPercentageLabel->setText(QString::number(ui->batterySlider->value()+1).rightJustified(4)+"%");

    }
    else if(ui->stackedWidget->currentIndex() == 0 && ui->batterySlider->value() == 0){
        ui->batteryPercentageLabel->setText(QString::number(0).rightJustified(4,' ')+"%");
    }
    else{
       batteryObj.timer->stop();
       resetFunc();
       ui->stackedWidget->setCurrentIndex(0);
    }


}

//This function is executed when the menuButton is pressed by the user on the GUI
//It will automatically set the stackedWidget index = 1 (mainMenuScreen)
//The resetFunc() will be called.
void MainWindow::backToMenu(){
    if (ui->stackedWidget->currentIndex() != 0){
         ui->stackedWidget->setCurrentIndex(1);
         resetFunc();
    }
}

//The batteryFunc() takes care of the battery reduction implimentation.
//The value of the batteryLable and batterySlider is updated/decremented
//according to the time interval set by the batteryObj Qtimer.
//When the battery level becomes 0 the powerOn(); function will be called,
//powering off the simulator.
void MainWindow::batteryFunc(){
    batteryObj.setTimer();

    ui->batteryPercentageLabel->setText(QString::number(batteryObj.getTimer()).rightJustified(4)+"%");
    ui->batterySlider->setValue(batteryObj.getTimer());

    if(ui->batterySlider->value() == 5){
        QMessageBox::about(this, "Alert", "Low Battery... Please charge now!");
    }

    if (batteryObj.getTimer() == 0){
        powerOn();
    }
}


/*This function runs everytime the allergyObj.timer decrements by 1
 * the allergyTimer_Label is updated everytime the function executes
 * if the touchSensor checkbox==1, else an alert meessage will be
 * displayed until the touchSensor is checked. When the allergyTimer_Label
 * becomes 00:00 and touchSensor == 1 then we will return to the programsScreen,
 * the resetFunc(); will called. If the saveTherapy checkBox == 1, we will
 * save this therapy session information by adding items to the therapyList
 */
void MainWindow::allergyFunc(){
    QString text = ui->allergyTimer->text();   //text is the STRING 10
    if(text.toInt() <= 0 && ui->touchSensor->isChecked() == 1){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(2);
        batteryObj.timer->setInterval(15000);
        if(ui->saveTherapy->isChecked()==true){ //save Alllergy therapy
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Allergy 77hz");
            ui->therapyList->addItem(" Power Level: " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: 10");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        allergyObj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }

    if(text.toInt() > 0 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() - 1); //4 , 3, 2 , 1
        if(text.toInt() <= 10){
            ui->allergyTimer->setText("0"+s);
        }else{
            ui->allergyTimer->setText(s);
        }
    }
}

/*This function runs everytime the biotingObj.timer decrements by 1
 * the biotingTimer is updated everytime the function executes
 * if the touchSensor checkbox==1, else an alert meessage will be
 * displayed until the touchSensor is checked. When the biotingTimer
 * becomes 00:00 and touchSensor == 1 then we will return to the programsScreen,
 * the resetFunc(); will called. If the saveTherapy checkBox == 1, we will
 * save this therapy session information by adding items to the therapyList
 */
void MainWindow::biotingFunc(){
    QString text = ui->biotingTimer->text();   //text is the STRING 15
    if(text.toInt() <= 0 && ui->touchSensor->isChecked() == 1){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(2);
        batteryObj.timer->setInterval(15000);
        if(ui->saveTherapy->isChecked()==true){ //save bioting therapy
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Bioting, 100Hz");
            ui->therapyList->addItem(" Power Level: " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: 15");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        biotingObj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }

    if(text.toInt() > 0 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() - 1); //4 , 3, 2 , 1
        if(text.toInt() <= 10){
            ui->biotingTimer->setText("0"+s);
        }else{
            ui->biotingTimer->setText(s);
        }
    }
}

/*This function runs everytime the painObj.timer decrements by 1
 * the painTimer is updated everytime the function executes
 * if the touchSensor checkbox==1, else an alert meessage will be
 * displayed until the touchSensor is checked. When the painTimer
 * becomes 00:00 and touchSensor == 1 then we will return to the programsScreen,
 * the resetFunc(); will called. If the saveTherapy checkBox == 1, we will
 * save this therapy session information by adding items to the therapyList
 */
void MainWindow::painFunc(){
    QString text = ui->painTimer->text();   //text is the STRING 20
    if(text.toInt() <= 0 && ui->touchSensor->isChecked() == 1){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(2);
        batteryObj.timer->setInterval(15000);
        if(ui->saveTherapy->isChecked()==true){ //save Pain therapy
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Bioting, 100Hz");
            ui->therapyList->addItem(" Power Level: " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: 20");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        painObj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }

    if(text.toInt() > 0 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() - 1); //4 , 3, 2 , 1
        if(text.toInt() <= 10){
            ui->painTimer->setText("0"+s);
        }else{
            ui->painTimer->setText(s);
        }
    }
}

/*This function runs everytime the intPainObj.timer decrements by 1
 * the intPainTimer is updated everytime the function executes
 * if the touchSensor checkbox==1, else an alert meessage will be
 * displayed until the touchSensor is checked. When the intPainTimer
 * becomes 00:00 and touchSensor == 1 then we will return to the programsScreen,
 * the resetFunc(); will called. If the saveTherapy checkBox == 1, we will
 * save this therapy session information by adding items to the therapyList
 */
void MainWindow::intPainFunc(){
    QString text = ui->intPainTimer->text();   //text is the STRING 30
    if(text.toInt() <= 0 && ui->touchSensor->isChecked() == 1){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(2);
        batteryObj.timer->setInterval(15000);
        if(ui->saveTherapy->isChecked()==true){ //save intPain therapy
            time (&tt);
            ti = localtime(&tt);
            ui->therapyList->addItem(asctime(ti));
            ui->therapyList->addItem(" Treatment: Bioting, 100Hz");
            ui->therapyList->addItem(" Power Level: " + ui->powerLevelLabel->text());
            ui->therapyList->addItem(" Duration: 30");
            ui->therapyList->addItem("--------------------------------------------");
        }
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        intPainObj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }

    if(text.toInt() > 0 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() - 1); //4 , 3, 2 , 1
        if(text.toInt() <= 10){
            ui->intPainTimer->setText("0"+s);
        }else{
            ui->intPainTimer->setText(s);
        }
    }
}


/*This function runs everytime the freq_1hz_Obj.timer increments by 1
 * the frequency1hz_seconds is updated everytime the function executes
 * and if frequency1hz_seconds reaches 59 the frequency1hz_minutes is
 * incremented by 1 and frequency1hz_seconds is set to 00, if the touchSensor
 * checkbox==1, else an alert message will be displayed until the touchSensor
 * is checked. When the frequency1hz_minutes becomes 60:00 and touchSensor == 1
 * then we will return to the programsScreen, the resetFunc(); will called. If
 * the saveTherapy checkBox == 1, we will save this therapy session information
 * by adding items to the therapyList
 */
void MainWindow::freq1HzFunc(){
    QString text = ui->frequency1hz_seconds->text();   //text is the STRING 00:00
    QString text2 = ui->frequency1hz_minutes->text();
    if(text2.toInt() >= 60){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(4);
        batteryObj.timer->setInterval(15000);
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        freq_1hz_Obj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }
    if(text.toInt() <= 59 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() + 1); //4 , 3, 2 , 1
        if(text.toInt() < 9){
            ui->frequency1hz_seconds->setText("0"+s);
        }else{
            ui->frequency1hz_seconds->setText(s);
        }
    }
    if(text.toInt() == 59){
        QString s2 = QString::number(text2.toInt() + 1);
        if(text2.toInt() < 9){
            ui->frequency1hz_minutes->setText("0"+s2);
        }else{
            ui->frequency1hz_minutes->setText(s2);
        }
        ui->frequency1hz_seconds->setText("00");
    }
}

/*This function runs everytime the freq_10hz_Obj.timer increments by 1
 * the frequency10hz_seconds is updated everytime the function executes
 * and if frequency10hz_seconds reaches 59 the frequency10hz_minutes is
 * incremented by 1 and frequency10hz_seconds is set to 00, if the touchSensor
 * checkbox==1, else an alert message will be displayed until the touchSensor
 * is checked. When the frequency10hz_minutes becomes 60:00 and touchSensor == 1
 * then we will return to the programsScreen, the resetFunc(); will called. If
 * the saveTherapy checkBox == 1, we will save this therapy session information
 * by adding items to the therapyList
 */
void MainWindow::freq10HzFunc(){
    QString text = ui->frequency10hz_seconds->text();   //text is the STRING 5
    QString text2 = ui->frequency10hz_minutes->text();
    if(text2.toInt() >= 60){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(4);
        batteryObj.timer->setInterval(15000);
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        freq_10hz_Obj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }
    if(text.toInt() <= 59 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() + 1); //4 , 3, 2 , 1
        if(text.toInt() < 9){
            ui->frequency10hz_seconds->setText("0"+s);
        }else{
            ui->frequency10hz_seconds->setText(s);
        }
    }
    if(text.toInt() == 59){
        QString s2 = QString::number(text2.toInt() + 1);
        if(text2.toInt() < 9){
            ui->frequency10hz_minutes->setText("0"+s2);
        }else{
            ui->frequency10hz_minutes->setText(s2);
        }
        ui->frequency10hz_seconds->setText("00");
    }
}

/*This function runs everytime the freq_20hz_Obj.timer increments by 1
 * the frequency20hz_seconds is updated everytime the function executes
 * and if frequency20hz_seconds reaches 59 the frequency20hz_minutes is
 * incremented by 1 and frequency20hz_seconds is set to 00, if the touchSensor
 * checkbox==1, else an alert message will be displayed until the touchSensor
 * is checked. When the frequency20hz_minutes becomes 60:00 and touchSensor == 1
 * then we will return to the programsScreen, the resetFunc(); will called. If
 * the saveTherapy checkBox == 1, we will save this therapy session information
 * by adding items to the therapyList
 */
void MainWindow::freq20HzFunc(){
    QString text = ui->frequency20hz_seconds->text();   //text is the STRING 5
    QString text2 = ui->frequency20hz_minutes->text();
    if(text2.toInt() >= 60){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(4);
        batteryObj.timer->setInterval(15000);
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        freq_10hz_Obj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }
    if(text.toInt() <= 59 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() + 1); //4 , 3, 2 , 1
        if(text.toInt() < 9){
            ui->frequency20hz_seconds->setText("0"+s);
        }else{
            ui->frequency20hz_seconds->setText(s);
        }
    }
    if(text.toInt() == 59){
        QString s2 = QString::number(text2.toInt() + 1);
        if(text2.toInt() < 9){
            ui->frequency20hz_minutes->setText("0"+s2);
        }else{
            ui->frequency20hz_minutes->setText(s2);
        }
        ui->frequency20hz_seconds->setText("00");
    }
}

/*This function runs everytime the freq_60hz_Obj.timer increments by 1
 * the frequency60hz_seconds is updated everytime the function executes
 * and if frequency60hz_seconds reaches 59 the frequency60hz_minutes is
 * incremented by 1 and frequency10hz_seconds is set to 00, if the touchSensor
 * checkbox==1, else an alert message will be displayed until the touchSensor
 * is checked. When the frequency60hz_minutes becomes 60:00 and touchSensor == 1
 * then we will return to the programsScreen, the resetFunc(); will called. If
 * the saveTherapy checkBox == 1, we will save this therapy session information
 * by adding items to the therapyList
 */
void MainWindow::freq60HzFunc(){
    QString text = ui->frequency60hz_seconds->text();   //text is the STRING 5
    QString text2 = ui->frequency60hz_minutes->text();
    if(text2.toInt() >= 60){  //parse to comapre to 0
        ui->stackedWidget->setCurrentIndex(4);
        batteryObj.timer->setInterval(15000);
        resetFunc();
    }
    if (ui->touchSensor->isChecked() == 0){
        freq_10hz_Obj.timer->stop();
        QMessageBox::about(this, "Alert", "Please place Electrode on skin and Press OK to continue!");
    }
    if(text.toInt() <= 59 && ui->touchSensor->isChecked() == 1){
        QString s = QString::number(text.toInt() + 1); //4 , 3, 2 , 1
        if(text.toInt() < 9){
            ui->frequency60hz_seconds->setText("0"+s);
        }else{
            ui->frequency60hz_seconds->setText(s);
        }
    }
    if(text.toInt() == 59){
        QString s2 = QString::number(text2.toInt() + 1);
        if(text2.toInt() < 9){
            ui->frequency60hz_minutes->setText("0"+s2);
        }else{
            ui->frequency60hz_minutes->setText(s2);
        }
        ui->frequency60hz_seconds->setText("00");
    }
}

//This function increments the powerSlider value by 10 if and only if the user if on any of the freqency or program pages.
//It also updated the powerLevelLabel for all the freqency and program pages according to the powerSlider value that is
//updated by the user
void MainWindow::rightButtonFunc(){
    if(ui->powerSlider->value() < 99 && (ui->stackedWidget->currentIndex() == 3 || ui->stackedWidget->currentIndex() == 5
                                         || (ui->stackedWidget->currentIndex() >=8 && ui->stackedWidget->currentIndex() <= 13)
                                         /*|| ui->stackedWidget->currentIndex() == 8 || ui->stackedWidget->currentIndex() == 9
                                         || ui->stackedWidget->currentIndex() == 10|| ui->stackedWidget->currentIndex() == 11
                                         || ui->stackedWidget->currentIndex() == 12|| ui->stackedWidget->currentIndex() == 13*/)){
        ui->powerSlider->setValue(ui->powerSlider->value() + 10);
        ui->powerLevelLabel->setText(QString::number((ui->powerSlider->value()))); //Edit By Abdullah
        ui->powerLevelLabel_Bioting->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel_Pain->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel_intPain->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_10hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_20hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_60hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));

    }

}

//This function decrements the powerSlider value by 10 if and only if the user if on any of the freqency or program pages.
//It also updated the powerLevelLabel for all the freqency and program pages according to the powerSlider value that is
//updated by the user
void MainWindow::leftButtonFunc(){
   if(ui->powerSlider->value() > 0 && (ui->stackedWidget->currentIndex() == 3 || ui->stackedWidget->currentIndex() == 5
                                        || (ui->stackedWidget->currentIndex() >=8 && ui->stackedWidget->currentIndex() <= 13)
                                       /* || ui->stackedWidget->currentIndex() == 8 || ui->stackedWidget->currentIndex() == 9
                                        || ui->stackedWidget->currentIndex() == 10|| ui->stackedWidget->currentIndex() == 11
                                        || ui->stackedWidget->currentIndex() == 12|| ui->stackedWidget->currentIndex() == 13*/)){
        ui->powerSlider->setValue(ui->powerSlider->value() - 10);
        ui->powerLevelLabel->setText(QString::number((ui->powerSlider->value()))); //Edit By Abdullah
        ui->powerLevelLabel_Bioting->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel_Pain->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel_intPain->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_10hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_20hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));
        ui->powerLevelLabel2_60hz->setText("Power Level: " + QString::number((ui->powerSlider->value())));

    }
}


//This function changes the rate of decrementation of the batteryLevel with respect to the
//value of the powerSlider which is passed in as a pararment in the function.
//The higher the value of the powerSlider the faster the battery will drain out.
void MainWindow::setProgramTimerInterval(int i){
    if(i > 90 && i <=100){ batteryObj.timer->setInterval(1000); }
    if(i > 80 && i <= 90){ batteryObj.timer->setInterval(2000); }
    if(i > 70 && i <= 80){ batteryObj.timer->setInterval(3000); }
    if(i > 60 && i <= 70){ batteryObj.timer->setInterval(4000); }
    if(i > 50 && i <= 60){ batteryObj.timer->setInterval(5000); }
    if(i > 40 && i <= 50){ batteryObj.timer->setInterval(6000); }
    if(i > 30 && i <= 40){ batteryObj.timer->setInterval(7000); }
    if(i > 20 && i <= 30){ batteryObj.timer->setInterval(8000); }
    if(i > 10 && i <= 20){ batteryObj.timer->setInterval(9000); }
    if(i >  0 && i <= 10){ batteryObj.timer->setInterval(15000); }
    if(i == 0){ batteryObj.timer->setInterval(15000); }
}

//The reset function resets the values for all the programs and freqencies everytime
//it is called.
void MainWindow::resetFunc(){
    batteryObj.timer->setInterval(15000);

    allergyObj.timer->stop();
    biotingObj.timer->stop();
    painObj.timer->stop();
    intPainObj.timer->stop();

    ui->allergyTimer->setText(QString::number(10));
    ui->biotingTimer->setText(QString::number(15));
    ui->painTimer->setText(QString::number(20));
    ui->intPainTimer->setText(QString::number(30));

    ui->powerSlider->setValue(0);
    ui->freqencySlider->setValue(1);
    ui->powerLevelLabel->setText("0");
    ui->powerLevelLabel_Bioting->setText("Power Level: 0");
    ui->powerLevelLabel_Pain->setText("Power Level: 0");
    ui->powerLevelLabel_intPain->setText("Power Level: 0");
    ui->powerLevelLabel2->setText("Power Level: 0");
    ui->powerLevelLabel2_10hz->setText("Power Level: 0");
    ui->powerLevelLabel2_20hz->setText("Power Level: 0");


    freq_1hz_Obj.timer->stop();  freq_10hz_Obj.timer->stop();
    freq_20hz_Obj.timer->stop(); freq_60hz_Obj.timer->stop();

    ui->frequency1hz_seconds->setText("00");  ui->frequency1hz_minutes->setText("00");
    ui->frequency10hz_seconds->setText("00"); ui->frequency10hz_minutes->setText("00");
    ui->frequency20hz_seconds->setText("00"); ui->frequency20hz_minutes->setText("00");
    ui->frequency60hz_seconds->setText("00"); ui->frequency60hz_minutes->setText("00");
}

//This function initilized all the buttons that are being used in the GUI.
void MainWindow::buttonSetup(){
    // Setting icons for buttons by passing in a path
     ui->upButton->setIcon(QIcon("/home/student/Desktop/src/button/up.PNG"));
    ui->downButton->setIcon(QIcon("/home/student/Desktop/src/button/down.PNG"));
    ui->rightButton->setIcon(QIcon("/home/student/Desktop/src/button/right.PNG"));
    ui->leftButton->setIcon(QIcon("/home/student/Desktop/src/button/left.PNG"));
    ui->returnButton->setIcon(QIcon("/home/student/Desktop/src/button/return.PNG"));
    ui->powerButton->setIcon(QIcon("/home/student/Desktop/src/button/power.PNG"));
    ui->menuButton->setIcon(QIcon("/home/student/Desktop/src/button/menu.PNG"));
    ui->okButton->setIcon(QIcon("/home/student/Desktop/src/button/ok.PNG"));

    //Setting Sizes for pictures imported for Icons to fill the button with Image
    const QSize iconSize(25,25);

    ui->upButton->setIconSize(iconSize);
    ui->downButton->setIconSize(iconSize);
    ui->rightButton->setIconSize(iconSize);
    ui->leftButton->setIconSize(iconSize);
    ui->returnButton->setIconSize(iconSize);
    ui->powerButton->setIconSize(iconSize);
    ui->menuButton->setIconSize(iconSize);
    ui->okButton->setIconSize(iconSize);
}

//This function sets the frequencies and starts the timer for every
//program and frequency therapries. It also re-initilized the interval
//for the battery according to the powerSlider value the user selected
//by calling setProgramTimerInterval(); function.
void MainWindow::setPageValues(){
    if(ui->stackedWidget->currentIndex() == 3){ //condition for allergy page
        ui->freqencySlider->setValue(20);
        setProgramTimerInterval(ui->powerSlider->value());
        allergyObj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 5){ //condition for freqency 1-0,9.9hz page
        ui->freqencySlider->setValue(5);
        setProgramTimerInterval(ui->powerSlider->value());
        freq_1hz_Obj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 8){ //condition for freqency 10hz page
        ui->freqencySlider->setValue(10);
        setProgramTimerInterval(ui->powerSlider->value());
        freq_10hz_Obj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 9){ //condition for freqency 20hz page
        ui->freqencySlider->setValue(20);
        setProgramTimerInterval(ui->powerSlider->value());
        freq_20hz_Obj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 10){ //condition for freqency 60hz page
        ui->freqencySlider->setValue(60);
        setProgramTimerInterval(ui->powerSlider->value());
        freq_60hz_Obj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 11){ //condition for bioting page
        ui->freqencySlider->setValue(100);
        setProgramTimerInterval(ui->powerSlider->value());
        biotingObj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 12){ //condition for pain page
        ui->freqencySlider->setValue(120);
        setProgramTimerInterval(ui->powerSlider->value());
        painObj.timer->start(1000);
    }
    if(ui->stackedWidget->currentIndex() == 13){ //condition for intPain page
        ui->freqencySlider->setValue(90);
        setProgramTimerInterval(ui->powerSlider->value());
        intPainObj.timer->start(1000);
    }
}

//This function is used to navigate throught the different stackWidget screens by,
//incrementing and decrementing the stackWidget indicies accordingly
void MainWindow::movePages(){
    if (ui->mainMenuScreen->currentRow() == 1){
       if(ui->stackedWidget->currentIndex() == 1){
           ui->programsScreen->setCurrentRow(1);
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
       }else if(ui->stackedWidget->currentIndex() == 2 && ui->programsScreen->currentRow() == 1){
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
           resetFunc();
       }else if(ui->stackedWidget->currentIndex() == 2 && ui->programsScreen->currentRow() == 2){
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+9);
           resetFunc();
       }else if(ui->stackedWidget->currentIndex() == 2 && ui->programsScreen->currentRow() == 3){
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+10);
           resetFunc();
       }else if(ui->stackedWidget->currentIndex() == 2 && ui->programsScreen->currentRow() == 4){
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+11);
           resetFunc();
       }

    }
    if (ui->mainMenuScreen->currentRow() == 2){
        if(ui->stackedWidget->currentIndex() == 1){
            ui->freqScreen->setCurrentRow(1);
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+3);
        }else if(ui->stackedWidget->currentIndex() == 4 && ui->freqScreen->currentRow() == 1){
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
            resetFunc();
        }else if(ui->stackedWidget->currentIndex() == 4 && ui->freqScreen->currentRow() == 2){
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+4);
            resetFunc();
        }else if(ui->stackedWidget->currentIndex() == 4 && ui->freqScreen->currentRow() == 3){
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+5);
            resetFunc();
        }else if(ui->stackedWidget->currentIndex() == 4 && ui->freqScreen->currentRow() == 4){
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+6);
            resetFunc();
        }else if(ui->freqScreen->currentRow() > 4 && ui->freqScreen->currentRow() <= ui->freqScreen->count()){
            QMessageBox::about(this, "Alert", "This option is not Enabled");
        }
    }
    if (ui->mainMenuScreen->currentRow() == 6){
        if(ui->stackedWidget->currentIndex() == 1){
            ui->historyList->setCurrentRow(1);
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+5);
        }else if(ui->stackedWidget->currentIndex() == 6 && ui->historyList->currentRow() == 1){
            ui->therapyList->setCurrentRow(0);
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
            resetFunc();
        }else if(ui->stackedWidget->currentIndex() == 6 && ui->historyList->currentRow() == 2){
            ui->therapyList->clear();
            QMessageBox::about(this, "Alert", "History deleted!");
        }
    }
}
