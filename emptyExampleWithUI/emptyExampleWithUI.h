#ifndef EMPTYEXAMPLEWITHUI_H
#define EMPTYEXAMPLEWITHUI_H

#include "emptyExampleWithUI_global.h"
#include <QWidget>
extern "C" EMPTYEXAMPLEWITHUISHARED_EXPORT void vSDK_Developer(QString dllpath,QWidget* parent,void* pointer);
extern "C" EMPTYEXAMPLEWITHUISHARED_EXPORT void vSDK_Close(QString dllpath,QWidget* parent);


#endif // EMPTYEXAMPLEWITHUI_H
