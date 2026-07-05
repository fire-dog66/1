#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QTranslator>

#include "logic.h"
#include "changestylethread.h"

namespace Ui {
class Interface;
}

class Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Interface(QWidget* parent = nullptr, QString dllpath = "");
    ~Interface();
    // style
    void GetSystemStyle(QString &str);
    void ChangeStyleSheet(QString str);
    // Language
    void onLanguageChange();
    QString LanguageFilePath(QString appDirPath, QString moudleName);
    QString getCurrentLanguage(QString appDirPath);
    void SetCallbackPointer(void* callbackPointer)
    {
        m_pPoniter = callbackPointer;
    }
    void RunCallbackFunction(QString, int status)
    {
        void (*Funcc)(QString, int);
        Funcc = (void (*)(QString, int)) m_pPoniter;
        Funcc(m_strDllPath,status);
    }
    void* m_pPoniter = nullptr; // modify by sq 20241111
    typedef void (*Funcc)(QString dllpath, int status); // modify by sq 20241111
    QString m_strDllPath; // modify by sq 20241111
    // Translate
    QString m_strLan;
    QTranslator* g_translator = nullptr;

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void changeEvent(QEvent* e);
    void closeEvent(QCloseEvent* e);

private slots:
    void ModifyCurentSkin(QString str);
    // 关闭页面按钮
    void CloseButton();

private:
    Ui::Interface* ui;
    logic logic;
    // 窗口移动相关
    QPoint MoveP; // 窗口拖动用变量
    bool isPressedWidget = false;
    // 换肤相关
    QString usedSkin;
    ChangeStyleThread* thre;
};

#endif // INTERFACE_H
