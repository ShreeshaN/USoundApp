#ifndef UTILS_H
#define UTILS_H
#include <QDateTime>
#endif // UTILS_H
#include<thread>

static std::string generateTimeStamp(){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return std::to_string(currentDateTime.toTime_t());
}

static void mssleep(int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
