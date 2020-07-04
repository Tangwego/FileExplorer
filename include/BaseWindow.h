#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include "Constant.h"

class BaseWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BaseWindow(QWidget *parent = nullptr);

signals:

};

#endif // BASEWINDOW_H
