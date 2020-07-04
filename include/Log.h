#ifndef LOG_H
#define LOG_H

#include "Constant.h"
#include <QMessageBox>

class Log{
public:
    static void info(const char *message){
        QMessageBox::information(NULL,FE_TITLE, QString("%1").arg(message), QMessageBox::Ok);
    }

    static void info(QString message){
        QMessageBox::information(NULL,FE_TITLE, message, QMessageBox::Ok);
    }
};

#endif
