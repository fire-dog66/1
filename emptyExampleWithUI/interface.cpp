#include "interface.h"
#include "ui_interface.h"
#include <QMouseEvent>
#include <QSettings>
#include <QTextCodec>

Interface::Interface(QWidget *parent, QString dllPath)
    : QWidget(parent)
    , m_strDllPath(dllPath)
    , ui(new Ui::Interface)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); // 取消标题栏
    this->setAttribute(Qt::WA_StyledBackground);
    this->setAttribute(Qt::WA_DeleteOnClose); // modify by sq 20241111

    // 获取样式表
    GetSystemStyle(usedSkin);
    // 修改样式表
    ChangeStyleSheet(usedSkin);

    thre = new ChangeStyleThread(usedSkin);
    connect(thre,SIGNAL(finished()),thre,SLOT(quit()));
    connect(thre,SIGNAL(finished()),thre,SLOT(deleteLater()));
    connect(thre,SIGNAL(ModifySkin(QString)),this,SLOT(ModifyCurentSkin(QString)));
    thre->start();

    // 关联按钮的槽函数
    connect(ui->Close, &QToolButton::clicked, this, &Interface::CloseButton);

    // 初始化logic
    logic.Init();

    // 修改翻译
    onLanguageChange();
}

Interface::~Interface()
{
    delete ui;
}

void Interface::GetSystemStyle(QString &str)
{
    QString sIniFile = QCoreApplication::applicationDirPath() + QString("/system.ini");
    QSettings settings(sIniFile, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF_8"));
    settings.beginGroup("Skin");
    QString strSkin = settings.value("UseSkin").toString();
    settings.endGroup();
    str = strSkin;
}

void Interface::ChangeStyleSheet(QString str)
{
    if(str == "BlackGold")
    {
        QFile sty(":/new/prefix1/QSS/Gold_SE.txt");
        sty.open(QFile::ReadOnly | QFile::Text);
        QString stylesheet = QString::fromLatin1(sty.readAll());
        this->setStyleSheet(stylesheet);
    }
    else if(str == "Deongaree")
    {
        QFile sty(":/new/prefix1/QSS/Blue_SE.txt");
        sty.open(QFile::ReadOnly | QFile::Text);
        QString stylesheet = QString::fromLatin1(sty.readAll());
        this->setStyleSheet(stylesheet);
    }
    else if(str == "Pink")
    {
        QFile sty(":/new/prefix1/QSS/Pink_SE.txt");
        sty.open(QFile::ReadOnly | QFile::Text);
        QString stylesheet = QString::fromLatin1(sty.readAll());
        this->setStyleSheet(stylesheet);
    }
    else if(str == "Plain")
    {
        QFile sty(":/new/prefix1/QSS/White_SE.txt");
        sty.open(QFile::ReadOnly | QFile::Text);
        QString stylesheet = QString::fromLatin1(sty.readAll());
        this->setStyleSheet(stylesheet);
    }
}

void Interface::onLanguageChange()
{
    QString sIniFile = QCoreApplication::applicationDirPath() + QString("/system.ini");
    QSettings settings(sIniFile, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF_8"));
    settings.beginGroup("Language");
    QString strLan = settings.value("UseLanguage").toString();
    settings.endGroup();
    if(m_strLan == strLan)
    {
        return;
    }
    QString lanStr = "emptyExampleWithUI";
    QString qmFile = LanguageFilePath(qApp->applicationDirPath(), lanStr);
    if(QFile(qmFile).exists())
    {
        if(g_translator == nullptr)
        {
            g_translator=new QTranslator();
        }
        g_translator->load(qmFile);
        qApp->installTranslator(g_translator);
    }
    m_strLan = strLan;
}

QString Interface::LanguageFilePath(QString appDirPath, QString moudleName)
{
    QString language = getCurrentLanguage(appDirPath);
    QString sLangIniFile = appDirPath + "/Language/languages.ini";
    QSettings* pLangConfigIni = new QSettings(sLangIniFile, QSettings::IniFormat);
    QString key = "LanguageName/" + language;
    QString langFileFlag = pLangConfigIni->value(key).toString();
    QString langfile = appDirPath + "/Language/" + moudleName + "_" + langFileFlag + ".qm";
    return langfile;
}

QString Interface::getCurrentLanguage(QString appDirPath)
{
    QString sIniFile = appDirPath + "/System.ini";
    QSettings* pConfigIni = new QSettings(sIniFile, QSettings::IniFormat);
    return pConfigIni->value("Language/UseLanguage").toString();
}

void Interface::mousePressEvent(QMouseEvent* e)
{
    QRect F(ui->TitleDecoration_W->geometry());
    if(F.contains(e->pos()))
    {
        isPressedWidget = true;
        if(e->buttons() == Qt::LeftButton)
        {
            if(F.contains(e->pos()))
            {
                MoveP = e->globalPos() - frameGeometry().topLeft();
                e->accept();
            }
        }
    }
}

void Interface::mouseMoveEvent(QMouseEvent* e)
{
    if(e->buttons() == Qt::LeftButton)
    {
        if(isPressedWidget)
        {
            move(e->globalPos() - MoveP);
            e->accept();
        }
    }
}

void Interface::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    isPressedWidget = false;
}

void Interface::changeEvent(QEvent* e)
{
    switch(e->type())
    {
        case QEvent::LanguageChange:
            onLanguageChange();
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void Interface::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    // 存入界面的坐标
    QString appPath = QCoreApplication::applicationDirPath();
    // 设置ini文件路径
    QString str = QString("%1\\Tools\\ExampleProgram/InterfaceLocation.ini").arg(appPath);
    QSettings* ini = new QSettings(str, QSettings::IniFormat);
    ini->setValue("emptyExampleWithUI/PosX", this->pos().x());
    ini->setValue("emptyExampleWithUI/PosY", this->pos().y());
    if(ini != nullptr)
    {
        delete ini;
    }
}

void Interface::ModifyCurentSkin(QString str)
{
    ChangeStyleSheet(str);
}

void Interface::CloseButton()
{
    // 用户界面关闭按钮
    RunCallbackFunction(m_strDllPath, 0); // modify by sq 20241111
    // 关闭此页面
    this->close();
}
