#ifndef CHANGESTYLETHREAD_H
#define CHANGESTYLETHREAD_H

#include <QThread>
#include <QObject>

class NetParameterEditor;

class ChangeStyleThread : public QThread
{
    Q_OBJECT

public:
    ChangeStyleThread(QString skin);
    void run();
    void SetInterFace(NetParameterEditor* INInterFace);
    void stoop();
    NetParameterEditor* m_InterFace;
    QString usedskin;
    void GetSystemStyle(QString &str);
    bool onWorking;

signals:
    void ModifySkin(QString str);

private:
    QString m_strSkin;
};

#endif // CHANGESTYLETHREAD_H
