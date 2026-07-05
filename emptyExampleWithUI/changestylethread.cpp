#include "changestylethread.h"
#include <QSettings>
#include <QTextCodec>
#include <QCoreApplication>
#include <QDebug>

ChangeStyleThread::ChangeStyleThread(QString skin)
{
    m_strSkin = skin;
    onWorking = true;
}

void ChangeStyleThread::run()
{
    while(onWorking)
    {
        QString NowStr;
        GetSystemStyle(NowStr);
        if(m_strSkin != NowStr)
        {
            m_strSkin = NowStr;
            emit ModifySkin(m_strSkin);
            QThread::msleep(10);
        }
        QThread::msleep(200);
    }
}

void ChangeStyleThread::GetSystemStyle(QString& str)
{
    QString sIniFile = QCoreApplication::applicationDirPath() + QString("/system.ini");
    QSettings settings(sIniFile, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF_8"));
    settings.beginGroup("Skin");
    QString strSkin = settings.value("UseSkin").toString();
    settings.endGroup();
    str = strSkin;
}
