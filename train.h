#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>

struct Message{
    std::string target = "";
    std::string msg ="MOVE";
};

//-------------------------------

class Train {

    std::string name = "";
    int moveTime = 0;
    int waitTime = 0;
    enum Status{
        WAITING,
        MOVING,
        ON_STATION,
        DEPARTED,
        ARRIVED,
        };
    Status status = MOVING;
    Message* MSG_ptr = nullptr;
    std::mutex trainLock;
  public:

    Train(int i, int in_moveTime, Message *M_ptr){
        name.resize(1, static_cast<char>(('A'+ i)));
        moveTime = in_moveTime;
        MSG_ptr = M_ptr;
    }

    std::string getStat();

    int getMoveTime(){return moveTime;}

    int getWaitTime(){return waitTime;}

    auto getMSGptr(){return MSG_ptr;}

    void setMsgPtr(Message* M_ptr){MSG_ptr = M_ptr;}

    std::string& getName(){return (std::string&)name;}

    void trainDriver();

    bool selector();

    void sendMSG();

    void timeMoveControl();

    void sender(std::string const com);
};