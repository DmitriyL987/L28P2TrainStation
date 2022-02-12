#include "Railwaystation.h"

#include <utility>

void RailwayStation::dispatcher() {
    int countDepartedTrain = 0;

    while (countDepartedTrain < 3 ) // Главный цикл
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        for (int i = 0; i < (*MSG_p).size(); ++i )
        {

            if ((*MSG_p)[i].target == "station" ) {
                if((*MSG_p)[i].msg == "DEPARTED"){
                    std::cout << "***Поезд " << listTrain[i]->getName() << " отбыл.\n";
                    if (unRegisteredTrain(listTrain[i])) ++countDepartedTrain;
                    for (int j = 0; j < (*MSG_p).size(); ++j)
                    {
                        senders(j, "MOVE");
                    }
                    break;

                } else if ((*MSG_p)[i].msg == "ON_STATION"){
                    for (int j = 0; j < (*MSG_p).size(); ++j) if (j != i) {
                        senders(j, "WAIT");
                    }
                    std::string answer = "";
                    stationLock.lock();
                    while(answer != "depart")
                    {
                        std::cout << "-> ";
                        std::cin >> answer;
                    }
                    stationLock.unlock();
                    senders(i, "DEPART");

                } else if ((*MSG_p)[i].msg == "ARRIVAL"){
                    stationLock.lock();
                    std::cout << "(*MSG_p)[i].msg " << (*MSG_p)[i].msg << std::endl;
                    stationLock.unlock();
                    senders(i, "ARRIVED");
                }
            }
        }

        /*for (int i = 0; i < listTrain.size(); ++i)
        {
            if (listTrain[i]->getStat() == "DEPARTED"){
                std::cout << "Поезд " << listTrain[i]->getName() << " отбыл.\n";
                if (unRegisteredTrain(listTrain[i])) ++countDepartedTrain;
                for (int j = 0; j < MSG_p->size(); ++j)
                {
                    stationLock.lock();
                    sender(j, "MOVE");
                    stationLock.unlock();
                }
                break;
            } else if (listTrain[i]->getStat() == "ON_STATION"){
                for (int j = 0; j < (*MSG_p).size(); ++j) if (j != i) sender(j, "WAIT");
                stationLock.lock();
                std::string answer = "";
                while(answer != "depart")
                {
                    std::cout << "-> ";
                    std::cin >> answer;
                }
                sender(i, "DEPART");
                stationLock.unlock();
            } else if (listTrain[i]->getStat() == "ARRIVED"){
                stationLock.lock();
                sender(i, "ARRIVED");
                stationLock.unlock();
            } else if (listTrain[i]->getStat() == "WAITING" || listTrain[i]->getStat() == "MOVING") {
                stationLock.lock();
                sender(i, "REPORT");
                stationLock.unlock();
            }
        }*/
    }
}
//-----------------------------

void RailwayStation::registerTrain(Train* train) {
    if (train == nullptr) return;
    stationLock.lock();
    listTrain.push_back(train);
    stationLock.unlock();
}
//-----------------------------

bool RailwayStation::unRegisteredTrain(Train *train) {
    stationLock.lock();
    //std::cout << "исключение из списка поездов\n" << train->getName() << std::endl;
    if(find(listTrain, train->getName()) != -1) {
        int index;
        index = find(listTrain, train->getName());
        std::vector<Train*>::iterator train_it = listTrain.begin();
        listTrain.erase(train_it+index);
        //std::cout << "исключение из очереди сообщений\n" << (*MSG_p)[index].source << std::endl;
        (*MSG_p).erase(((*MSG_p).begin()+index));
        for(int i = 0; i < (*MSG_p).size(); ++i) listTrain[i]->setMsgPtr(&(*MSG_p)[i]);
        stationLock.unlock();
        return true;
    }
    stationLock.unlock();
    return false;
}
//-----------------------------

int RailwayStation::find(std::vector<Train*> &vec, std::string &in_name){
    int i = 0;
    for(; i < vec.size(); ++i){
        if (vec[i]->getName() == in_name) return i;
    }
    return  -1;
}

Train* RailwayStation::getTrain(int index){
    return listTrain[index];
}

void RailwayStation::senders(int index, std::string in_msg){
    stationLock.lock();
    (*MSG_p)[index].target = listTrain[index]->getName();
    (*MSG_p)[index].msg = in_msg;
    stationLock.unlock();
}

void RailwayStation::sendMSG(int i) {
    stationLock.lock();
    if (listTrain[i]->getStat() == "MOVING"){
        std::cout << "Поезд \"" << listTrain[i]->getName() << "\" продолжает движение\n"
        << "прибытие через " << listTrain[i]->getMoveTime() << " сек." << std::endl;
    } else if (listTrain[i]->getStat() == "WAITING") {
        std::cout << "Поезд \"" << listTrain[i]->getName() << "\" ожидает в течении " << listTrain[i]->getWaitTime() << " секунд." << std::endl;

    } else if(listTrain[i]->getStat() == "ARRIVED") {
        std::cout << "Поезд \"" << listTrain[i]->getName() << "\" прибыл" << std::endl;

    } else if (listTrain[i]->getStat() == "ON_STATION"){
        std::cout << "Поезд \"" << listTrain[i]->getName() << "\" находится на станции и ждёт команду к отправке" << std::endl;

    } else if(listTrain[i]->getStat() == "DEPARTED"){
        std::cout << "Поезд " << listTrain[i]->getName() << " отбыл" << std::endl;

    }
    stationLock.unlock();
}