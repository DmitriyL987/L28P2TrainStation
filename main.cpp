#include "train.h"
#include "Railwaystation.h"

int main() {
    std::cout << "+++ Railway station +++" << std::endl;
    std::vector <Message> MSG(3);
    RailwayStation station(&MSG);

    for (int i = 0; i < 3; ++i){
        double arrivalTime;
        do{
            std::cout << "Введите время до прибытия поезда № " << i+1 << " на станцию (в секундах): ";
            std::cin >> arrivalTime;
        }while (arrivalTime <= 0);
        station.registerTrain(new Train(i, arrivalTime, &MSG[i]));
    }

    std::cout << "address vector MSG: " << &MSG << std::endl;
    std::cout << "size vector MSG: " << MSG.size() << std::endl;
    for (int i = 0; i < MSG.size(); ++i){
        std::cout << "address MSG[i]: " << &MSG[i] << std::endl;
        std::cout << "station.getTrain(i)->getMSGptr()" << station.getTrain(i)->getMSGptr() << std::endl;
        std::cout << "&(*station.getMSGptr())[i]" << &(*station.getMSGptr())[i] << std::endl;
    }

    std::thread thread_Train[3];
    for (int i = 0; i < 3; ++i){
        thread_Train[i] = std::thread(&Train::trainDriver, station.getTrain(i));
    }
    std::thread thread_Station = std::thread(&RailwayStation::dispatcher, &station);
    for (auto & i : thread_Train) if (i.joinable())i.join();
    thread_Station.join();

    std::cout << "Все поезда отбыли от станции\nХотите выйти из программы? Y/N" << std::endl;
    char a;
    std::cin >> a;
    return 0;
}