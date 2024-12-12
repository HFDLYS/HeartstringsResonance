#include "graphicsthread.h"

#include <iostream>

GraphicsThread::GraphicsThread(QObject *parent) : QThread(parent) {}

GraphicsThread::~GraphicsThread() {
    wait();
    quit();
}

void GraphicsThread::run() {
    // int i = 0;
    while (true) {
        // std::cerr << "signal: " << i++ << std::endl;
        timeout();
        QThread::msleep(10);
    }
}
