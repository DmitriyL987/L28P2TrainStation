#include "train.h"

void Train::trainDriver() {
    //sendMSG();
    bool loop;
    do{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timeMoveControl();
        loop = selector();
    }while(loop);
}

//-----------------------------
bool Train::selector() {

    if (MSG_ptr->target == name)
    {
        trainLock.lock();
        std::cout << "Проверяем сообщения от станции!\n";
        std::cout << "MSG_ptr->target - " << MSG_ptr->target << std::endl;
        std::cout << "MSG_ptr->msg - " << MSG_ptr->msg << std::endl;


        if ("WAIT" == MSG_ptr->msg && status != WAITING) {
            std::cout << "Поезд \"" << name << "\" получено сообщение = " << MSG_ptr->msg << std::endl;
            status = WAITING;
            //sendMSG();
            sender("WAITING");
        } else if (MSG_ptr->msg == "MOVE" && status != MOVING) {
            status = MOVING;
            std::cout << "Поезд \"" << name << "\" получено сообщение = " << MSG_ptr->msg << std::endl;
            //sendMSG();
            sender("MOVING");
        } else if (MSG_ptr->msg == "DEPART" && status != DEPARTED){
            std::cout << "Поезд \"" << name << "\" получено сообщение = " << MSG_ptr->msg << std::endl;
            status = DEPARTED;
            //sendMSG();
            sender("DEPARTED");
            return false;
        } else if (MSG_ptr->msg == "ARRIVED" && status != ARRIVED){
            std::cout << "Поезд \"" << name << "\" получено сообщение = " << MSG_ptr->msg << std::endl;
            status = ARRIVED;
            //sendMSG();
            sender("ON_STATION");
        } else if (MSG_ptr->msg == "ON_STATION" && status != ON_STATION){
            status = ON_STATION;
            //sendMSG();
            //sender("ON_STATION");
        } //else if (MSG_ptr->msg == "REPORT") sendMSG();
    }
    trainLock.unlock();
    return true;
}
//-----------------------------

/*void Train::sendMSG() {
    trainLock.lock();
    if (status == MOVING){
        std::cout << "Поезд \"" << name << "\" продолжает движение\n"
        << "прибытие через " << moveTime << " сек." << std::endl;
        trainLock.unlock();
    } else if (status == WAITING) {
        std::cout << "Поезд \"" << name << "\" ожидает в течении " << waitTime << " секунд." << std::endl;
        trainLock.unlock();
    } else if(status == ARRIVED) {
        std::cout << "Поезд \"" << name << "\" прибыл" << std::endl;
        trainLock.unlock();
    } else if (status == ON_STATION){
        std::cout << "Поезд \"" << name << "\" находится на станции и ждёт команду к отправке" << std::endl;
        trainLock.unlock();
    } else if(status == DEPARTED){
        std::cout << "Поезд " << name << " отбыл" << std::endl;
        trainLock.unlock();
    }
}*/
//-----------------------------


void Train::timeMoveControl() {
    if (status == MOVING && moveTime > 0) {
        --moveTime;
    } else if (status == MOVING && moveTime == 0){
        //status = ARRIVED;
        sender("ARRIVAL");
    } else if (status == WAITING) ++waitTime;
    if (status != WAITING) waitTime = 0;
}
//--------------------------------
void Train::sender(std::string const com) {
    trainLock.lock();
    MSG_ptr->target = "station";
    MSG_ptr->msg = com;
    trainLock.unlock();
}
//-----------------------------
std::string Train::getStat(){
    std::string str;
    switch (status)
    {
        case WAITING:
            str = "WAITING";
            break;
        case MOVING:
            str = "MOVING";
            break;
        case ON_STATION:
            str = "ON_STATION";
            break;
        case DEPARTED:
            str = "DEPARTED";
            break;
        case ARRIVED:
            str = "ARRIVED";
            break;
    }
    return str;
}