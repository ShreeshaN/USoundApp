#ifndef UTILS_H
#define UTILS_H
#include <QDateTime>
#endif // UTILS_H

static std::string generateTimeStamp(){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return std::to_string(currentDateTime.toTime_t());
}

