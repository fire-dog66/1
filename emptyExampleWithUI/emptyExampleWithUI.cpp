#include <QCoreApplication>
#include <QDir>
#include <QApplication>
#include <Qdebug.h>
#include <QMainWindow>
#include <QSettings>
#include "emptyExampleWithUI.h"
#include "interface.h"

#if defined Q_OS_WIN32   //for win
#include <windows.h>
bool checkOnly()
{
    //  创建互斥量
    HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"Mark");
    //  检查错误代码
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //  如果已有互斥量存在则释放句柄并复位互斥量
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
        //  程序退出
        return false;
    }
    else
    {
        return true;
    }
}
#endif

#if defined Q_OS_LINUX   //for linux
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
bool checkOnly()
{
    const char filename[]  = "/tmp/lockfile";
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    int flock = lockf(fd, F_TLOCK, 0);
    if(fd == -1)
    {
        perror("open lockfile/n");
        return false;
    }
    //给文件加锁
    if(flock == -1)
    {
        perror("lock file error/n");
        return false;
    }
    //程序退出后，文件自动解锁
    return true;
}
#endif

void vSDK_Developer(QString dllpath,QWidget* parent,void* pointer)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString str = QString("%1\\Tools\\ExampleProgram/InterfaceLocation.ini").arg(appPath);// 设置ini文件路径
    QSettings* ini = new QSettings(str, QSettings::IniFormat);
    QStringList sections = ini->childGroups();
    QPoint pos;
    if(sections.contains("emptyExampleWithUI"))
    {
        pos.rx() = ini->value("emptyExampleWithUI/PosX").toInt();
        pos.ry() = ini->value("emptyExampleWithUI/PosY").toInt();
    }
    else
    {
        QMainWindow* mainWnd = reinterpret_cast<QMainWindow*>(parent);
        pos = mainWnd->mapToGlobal(mainWnd->centralWidget()->pos());
    }
    QFileInfo fileInfo(dllpath);
    QString WindowsoOjectName = fileInfo.fileName();//不同应用需要修改此名字，与dll一致
    // qDebug()<< WindowsoOjectName;
    QList<QWidget*> listwidgets = QApplication::topLevelWidgets();//遍历窗体,禁止非模态窗体重复打开
    for(int i = 0; i < listwidgets.size(); i++)
    {
        QWidget* oneWidget = listwidgets.at(i);
        if(i != listwidgets.size() - 1)
        {
            // QString WindowsoOjectNameTemp = oneWidget->objectName();
            // qDebug()<< WindowsoOjectNameTemp << "!";
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                break;
            }
        }
        else
        {
            // QString WindowsoOjectNameTemp = oneWidget->objectName();
            // qDebug()<< WindowsoOjectNameTemp << "?";
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                break;
            }
            else
            {
                Interface* formWnd = new Interface(parent,dllpath);
                formWnd->setObjectName(WindowsoOjectName);
                formWnd->SetCallbackPointer(pointer); //modify by sq 20241111

                if(false)
                {
                    //设置模态窗体true或非模态false
                    formWnd->setWindowModality(Qt::ApplicationModal);
                }
                else
                {
                    // formWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);//设置窗口顶置: 一直在最前面
                    formWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
                }
                formWnd->move(pos.x(), pos.y());
                formWnd->show();
                formWnd->RunCallbackFunction(dllpath,1); //modify by sq 20241111

                formWnd->setAttribute(Qt::WA_DeleteOnClose);
                formWnd->setAttribute(Qt::WA_StyledBackground);
            }
        }
    }
    if(ini != nullptr)
    {
        delete ini;
    }
}
void vSDK_Developerold(QString dllpath, QWidget* parent)
{
    Q_UNUSED(dllpath);
    if(checkOnly()==false)
    {
        return;
    }
    else
    {
        Interface* formWnd = new Interface(parent);
        if(true)
        {
            //设置模态窗体true或非模态false
            formWnd->setWindowModality(Qt::ApplicationModal);
        }
        else
        {
            formWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);//设置窗口顶置: 一直在最前面
        }
        formWnd->show();
        formWnd->setAttribute(Qt::WA_DeleteOnClose);
    }
}

Interface* getWndPointer(QString dllpath)
{
    Interface* wnd = nullptr;
    QFileInfo fileInfo(dllpath);
    QString  WindowsoOjectName = fileInfo.fileName();//不同应用需要修改此名字，与dll一致
    QList<QWidget*> listwidgets = QApplication::topLevelWidgets();//遍历窗体,禁止非模态窗体重复打开
    for(int i = 0; i < listwidgets.size(); i++)
    {
        QWidget* oneWidget = listwidgets.at(i);
        if(i != listwidgets.size() - 1)
        {
            // QString WindowsoOjectNameTemp = oneWidget->objectName();
            // qDebug()<< WindowsoOjectNameTemp << "!";
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                wnd = (Interface*)oneWidget;
            }
        }
        else
        {
            // QString WindowsoOjectNameTemp = oneWidget->objectName();
            // qDebug()<< WindowsoOjectNameTemp << "?";
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                wnd = (Interface*)oneWidget;
            }
        }
    }
    return wnd;
}

void vSDK_Developershortcut(QString dllshortcut, int index, QString dllpath, QWidget* parent)
{
    Q_UNUSED(dllshortcut);
    // qDebug()<< "vSDK_Developershortcut is enter!!!!";
    Interface* wnd = getWndPointer(dllpath);
    // qDebug()<< "vSDK_Developershortcut Form* wnd" << wnd;

    if(index == 0)
    {
        if(wnd == nullptr)
        {
           // vSDK_Developer(dllpath, parent);
        }
        // qDebug()<< "Ctrl+2 !!!";
    }
    if(index == 1)
    {
        if(wnd == nullptr)
        {
            //vSDK_Developer(dllpath, parent);
        }
        // qDebug()<< "Ctrl+3 !!!";
    }
}

void vSDK_Close(QString dllpath,QWidget* parent)
{
    QFileInfo fileInfo(dllpath);
    QString  WindowsoOjectName = fileInfo.fileName();//不同应用需要修改此名字，与dll一致
    QList<QWidget*> listwidgets = QApplication::topLevelWidgets();//遍历窗体,禁止非模态窗体重复打开
    for(int i=0; i<listwidgets.size();i++)
    {
        QWidget* oneWidget = listwidgets.at(i);
        if(i !=listwidgets.size()-1 )
        {
            QString  WindowsoOjectNameTemp =oneWidget->objectName();
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                Interface* formWnd = static_cast<Interface*>(oneWidget);
                formWnd->RunCallbackFunction(dllpath,0); //modify by sq 20241111
                formWnd->close();
            }
        }
        else
        {
            QString  WindowsoOjectNameTemp =oneWidget->objectName();
            //qDebug()<<WindowsoOjectNameTemp<<"？";
            if(oneWidget->objectName() == WindowsoOjectName)
            {
                Interface* formWnd = static_cast<Interface*>(oneWidget);
                formWnd->RunCallbackFunction(dllpath,0); //modify by sq 20241111
                formWnd->close();
            }
        }
    }
}
