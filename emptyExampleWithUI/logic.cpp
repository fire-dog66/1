#include "logic.h"

logic::logic(QObject *parent) : QObject(parent)
{

}

void logic::Init()
{
    // 初始化vSDK
    QString exePath = QApplication::applicationDirPath();
    vSDK.vSDK_Init(exePath.toStdString().c_str());
    // 获取Job指针
    vSDK.vSDK_Get_CurrentJob(Job);
    // 获取Pcb指针
    vSDK.vSDK_Job_GetCurrentPcb(Job, Pcb);
    // 获取Board指针
    vSDK.vSDK_Pcb_GetBoard(Pcb, Board);
}
