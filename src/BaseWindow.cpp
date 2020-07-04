#include "BaseWindow.h"

#include <QGuiApplication>
#include <QScreen>

BaseWindow::BaseWindow(QWidget *parent) : QMainWindow(parent)
{
    int wSize = 640;
    int hSize = 480;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect =  screen->availableVirtualGeometry();
//    resize(screenRect.width(),screenRect.height());

    setMinimumSize(wSize,hSize);
//    setWindowFlags(Qt::FramelessWindowHint);
    setBaseSize(wSize,hSize);
    resize(wSize,hSize);

    int mWidth = (screenRect.width()/2 - width() / 2);
    int mHeight = (screenRect.height()/2 - height() / 2);

    move(mWidth, mHeight);
    setWindowTitle(FE_TITLE);
}
