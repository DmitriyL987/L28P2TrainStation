#pragma once
#include "train.h"


class RailwayStation{

    std::mutex stationLock;

    std::vector <Train*> listTrain;

    std::vector<Message>* MSG_p;

    Train* curTrain = nullptr;
  public:

    RailwayStation(std::vector<Message>* in_vec){MSG_p = in_vec;}

    auto getMSGptr(){return MSG_p;}

    void registerTrain(Train*);

    bool unRegisteredTrain(Train*);

    void dispatcher();

    void senders(int index, std::string );

    Train* getTrain(int);

    int find(std::vector<Train*>&, std::string& );

    void sendMSG(int);

};
