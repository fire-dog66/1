#ifndef LOGIC_H
#define LOGIC_H

#include <QApplication>
#include <QDebug>

#include "vSDK.h"

class logic : public QObject
{
    Q_OBJECT

public:
    logic(QObject* parent = nullptr);
    // 初始化
    void Init();

private:
    void* Job = nullptr; // Job指针
    void* Pcb = nullptr; // Pcb指针
    void* Board = nullptr; // Board指针
    vSDK vSDK; // vSDK工具类
};

#endif // LOGIC_H
