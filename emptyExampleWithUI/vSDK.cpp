#include "vSDK.h"

#if defined(DEF_USE_QTFRAMEWORK)
#define def_Run_vSDKDllLib_Interface(_vSDKDllLib, FunName) _vSDKDllLib->resolve(FunName)
#else
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define def_Run_vSDKDllLib_Interface(_vSDKDllLib, FunName) \
    reinterpret_cast<void *>(GetProcAddress(_vSDKDllLib, FunName))
#else
#define def_Run_vSDKDllLib_Interface(_vSDKDllLib, FunName) dlsym(_vSDKDllLib, FunName)
#endif
#endif

#if defined(DEF_USE_QTFRAMEWORK)
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <codecvt>
#include <locale>
#if defined(_MSC_VER)
#else
#include <unistd.h>
#endif
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
namespace utf8local_vsdk_namespace {
std::wstring charToWString(const char *charStr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(charStr);
}
}  // namespace utf8local_vsdk_namespace
#endif
#endif

#if defined(DEF_USE_QTFRAMEWORK)
QString Get_Sys_DllAllName(const char *_appPath, const char *_cDllName) {
    QString dllPath = "";
#ifdef Q_OS_WIN
    if ((!_appPath) || (!*_appPath)) {
        dllPath = QString::fromStdString(std::string(_cDllName)) + ".dll";
    } else {
        dllPath = QString::fromStdString(std::string(_appPath)) + "/" + _cDllName + ".dll";
    }
#endif
#ifdef Q_OS_LINUX
    if (!_appPath || !*_appPath) {
        dllPath = "lib" + QString::fromStdString(std::string(_cDllName)) + ".so";
    } else {
        dllPath = QString::fromStdString(std::string(_appPath)) + "/lib" + _cDllName + ".so";
    }
#endif
#ifdef Q_OS_MAC
    if (!_appPath || !*_appPath) {
        dllPath = "lib" + QString::fromStdString(std::string(_cDllName)) + ".dylib";
    } else {
        dllPath = QString::fromStdString(std::string(_appPath)) + "/lib" + _cDllName + ".dylib";
    }
#endif
    return dllPath;
}
#else
std::string Get_Sys_DllAllName(const char *_appPath, const char *_cDllName) {
    std::string dllPath = "";
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    if ((!_appPath) || (!*_appPath)) {
        dllPath = std::string(_cDllName) + ".dll";
    } else {
        dllPath = std::string(_appPath) + "/" + _cDllName + ".dll";
    }
#elif defined(__linux__)
    if (!_appPath || !*_appPath) {
        dllPath = "lib" + std::string(_cDllName) + ".so";
    } else {
        dllPath = std::string(_appPath) + "/lib" + _cDllName + ".so";
    }
#elif defined(__APPLE__) || defined(__MACH__)
    if (!_appPath || !*_appPath) {
        dllPath = "lib" + std::string(_cDllName) + ".dylib";
    } else {
        dllPath = std::string(_appPath) + "/lib" + _cDllName + ".dylib";
    }
#endif
    return dllPath;
}
#endif

vSDK::vSDK() {
    m_vSdkDllLib = nullptr;
}

vSDK::~vSDK() {
    if (m_vSdkDllLib) {
#if defined(DEF_USE_QTFRAMEWORK)
        m_vSdkDllLib->unload();
#else
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        FreeLibrary(m_vSdkDllLib);
#else
        dlclose(m_vSdkDllLib);
#endif
#endif
    }
    /*FreeLibrary(hInst);*/
}

// VSDK_EXPORT int vSDK_Init(const char *_exe_path);
int vSDK::vSDK_Init(const char *ExePath) {
    int result = 0;
/*QString dllPath = "";
#ifdef Q_OS_WIN
    if (!ExePath || !*ExePath)
    {
        dllPath = "vSdk.dll";
    }
    else
    {
        dllPath = QString::fromStdString(std::string(ExePath)) + "/vSdk.dll";
    }
    m_vSdkDllLib = new QLibrary(dllPath);
#endif

#ifdef Q_OS_LINUX
    if (!ExePath || !*ExePath)
    {
        dllPath = "./libvSdk.so";
    }
    else
    {
        dllPath = QString::fromStdString(std::string(ExePath)) + "/libvSdk.so";
    }
    m_vSdkDllLib = new QLibrary(dllPath);
#endif

#ifdef Q_OS_MAC
    if (!ExePath || !*ExePath)
    {
        dllPath = "./libvSdk.dylib";
    }
    else
    {
        dllPath = QString::fromStdString(std::string(ExePath)) + "/libvSdk.dylib";
    }
    m_vSdkDllLib = new QLibrary(dllPath);
#endif
    if (m_vSdkDllLib->load())
    {
    typedef int (*vSDK_Init)(const char* ExePath);
    vSDK_Init vSDK_function=(vSDK_Init)def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Init");
    if(vSDK_function)
    {
        result = vSDK_function(ExePath);
    }
    }
    return result;*/
#if defined(DEF_USE_QTFRAMEWORK)
    QString dllPath = Get_Sys_DllAllName(ExePath, "vSdk");
    m_vSdkDllLib = new QLibrary(dllPath);
    if (!m_vSdkDllLib->load()) {
        // Write_RunLogFile("D:/", "Run_SDK", "m_vSdkDllLib error");
        //*err = const_cast<char*>(m_vSdkDllLib->errorString().toStdString().c_str());
        return false;
    } else {
        // Write_RunLogFile("D:/", "Run_SDK", "m_vSdkDllLib ok");
        // char *_err = nullptr;
        // FUN_DYNAMIC_InitDll(m_vSdkDllLib, "loadDll", exePath, &_err);
        //*err = const_cast<char*>(EMPTY_STRING);
        return true;
    }
#else
    std::string dllPath = Get_Sys_DllAllName(ExePath, "vSdk");
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::wstring ws_dllPath = utf8local_vsdk_namespace::charToWString(dllPath.c_str());
    m_vSdkDllLib = LoadLibraryW(ws_dllPath.c_str());
#else
    m_vSdkDllLib = dlopen(dllPath.c_str(), RTLD_LAZY);
#endif
    /*if (!m_vSdkDllLib)
    {
        return false;
    }
    return true;*/
#endif
    // if (m_vSdkDllLib->load())
    if (m_vSdkDllLib) {
        typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(const char *ExePath);
        FUN_DYNAMIC_CALL vSDK_function =
            reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Init"));
        if (vSDK_function) {
            result = vSDK_function(ExePath);
        }
    }
    return result;
}

int vSDK::vSDK_GetCurrentJob(void *&Job) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *&Job);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_GetCurrentJob"));
    if (vSDK_function) {
        result = vSDK_function(Job);
    }
    return result;
}

int vSDK::vSDK_Job_GetCurrentPcb(void *Job, void *&Pcb) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Job, void *&Pcb);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Job_GetCurrentPcb"));
    if (vSDK_function) {
        result = vSDK_function(Job, Pcb);
    }
    return result;
}

int vSDK::vSDK_Pcb_GetBoard(void *Pcb, void *&Board) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Pcb, void *&Board);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Pcb_GetBoard"));
    if (vSDK_function) {
        result = vSDK_function(Pcb, Board);
    }
    return result;
}

int vSDK::vSDK_Board_AddLayer(void *Board, const char *LayerName, void *&Layer) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, const char *LayerName, void *&Layer);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_AddLayer"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerName, Layer);
    }
    return result;
}

int vSDK::vSDK_Board_GetLayerListCount(void *Board, int &LayerCount) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int &LayerCount);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_GetLayerListCount"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerCount);
    }
    return result;
}

int vSDK::vSDK_Board_GetLayerByIndex(void *Board, int LayerIndex, void *&Layer) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerIndex, void *&Layer);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_GetLayerByIndex"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerIndex, Layer);
    }
    return result;
}

int vSDK::vSDK_Board_GetLayerByName(void *Board, const char *LayerName, void *&Layer) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, const char *LayerName, void *&Layer);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_GetLayerByName"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerName, Layer);
    }
    return result;
}

int vSDK::vSDK_Board_UpdateLayerList(void *Pcb) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Pcb);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_UpdateLayerList"));
    if (vSDK_function) {
        result = vSDK_function(Pcb);
    }
    return result;
}

int vSDK::vSDK_Board_UpdateLayerConfigFile(void *Pcb, bool BeNewProj) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Pcb, bool BeNewProj);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Board_UpdateLayerConfigFile"));
    if (vSDK_function) {
        result = vSDK_function(Pcb, BeNewProj);
    }
    return result;
}

int vSDK::vSDK_DcodeTable_FindRectangleDcodeIDBySize(void *DcodeTable,
                                                     const double CenterX,
                                                     const double CenterY,
                                                     const double Length,
                                                     const double Width,
                                                     int &DcodeID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *DcodeTable, const double CenterX, const double CenterY,
                                                const double Length, const double Width, int &DcodeID);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_DcodeTable_FindRectangleDcodeIDBySize"));
    if (vSDK_function) {
        result = vSDK_function(DcodeTable, CenterX, CenterY, Length, Width, DcodeID);
    }
    return result;
}

int vSDK::vSDK_DcodeTable_FindAndCreateRoundDcodeIDBySize(
    void *DcodeTable, double CenterX, double CenterY, double Size, int &DcodeID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *DcodeTable, double CenterX, double CenterY, double Size,
                                                int &DcodeID);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_DcodeTable_FindAndCreateRoundDcodeIDBySize"));
    if (vSDK_function) {
        result = vSDK_function(DcodeTable, CenterX, CenterY, Size, DcodeID);
    }
    return result;
}

int vSDK::vSDK_Dcode_CreateDcode(void *DcodeTable, const char *DcodeName, const char *DcodeAliasName, void *&DCode) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *DcodeTable, const char *DcodeName, const char *DcodeAliasName,
                                                void *&DCode);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Dcode_CreateDcode"));
    if (vSDK_function) {
        result = vSDK_function(DcodeTable, DcodeName, DcodeAliasName, DCode);
    }
    return result;
}

int vSDK::vSDK_Dcode_AddDcodeShape(void *DcodeTable, void *DCode, void *Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *DcodeTable, void *DCode, void *Shape);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Dcode_AddDcodeShape"));
    if (vSDK_function) {
        result = vSDK_function(DcodeTable, DCode, Shape);
    }
    return result;
}

int vSDK::vSDK_Dcode_AddDcodeEnd(void *DcodeTable, void *DCode, int &DcodeID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *DcodeTable, void *DCode, int &DcodeID);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Dcode_AddDcodeEnd"));
    if (vSDK_function) {
        result = vSDK_function(DcodeTable, DCode, DcodeID);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerID(void *Layer, int &LayerID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Layer, int &LayerID);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerID"));
    if (vSDK_function) {
        result = vSDK_function(Layer, LayerID);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerName(void *Layer, char *&LayerName) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Layer, char *&LayerName);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerName"));
    if (vSDK_function) {
        result = vSDK_function(Layer, LayerName);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerSide(void *Layer, int &iLayerSide) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Layer, int &iLayerSide);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerSide"));
    if (vSDK_function) {
        result = vSDK_function(Layer, iLayerSide);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerType(void *Layer, int &iLayerType) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Layer, int &iLayerType);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerType"));
    if (vSDK_function) {
        result = vSDK_function(Layer, iLayerType);
    }
    return result;
}

int vSDK::vSDK_Layer_GetDcodeCountByLayerId(void *Board, const int LayerId, int &DcodeCount, void *&DcodeTable) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, const int LayerId, int &DcodeCount, void *&DcodeTable);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetDcodeCountByLayerId"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, DcodeCount, DcodeTable);
    }
    return result;
}

int vSDK::vSDK_Layer_LoadLayerByLayerID(void *Board, const int LayerID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, const int LayerID);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_LoadLayerByLayerID"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectCount(void *Board, int LayerID, int &iObjectCount) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerID, int &iObjectCount);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectCount"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID, iObjectCount);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectType(void *Board, int LayerID, int IObjectIndex, int &iShapeType) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerID, int IObjectIndex, int &iShapeType);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectType"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID, IObjectIndex, iShapeType);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectShapeCount(void *Board, int LayerID, int IObjectIndex, int &iShapeCount) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerID, int IObjectIndex, int &iShapeCount);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectShapeCount"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID, IObjectIndex, iShapeCount);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectShapeByIndex(void *Board,
                                                int LayerID,
                                                int IObjectIndex,
                                                int IShapeIndex,
                                                int &Mirror,
                                                double &Angle,
                                                int &DcodeId,
                                                void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerID, int IObjectIndex, int IShapeIndex,
                                                int &Mirror, double &Angle, int &DcodeId, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectShapeByIndex"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID, IObjectIndex, IShapeIndex, Mirror, Angle, DcodeId, Shape);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectNewShapeByIndex(
    void *Board, int LayerID, int IObjectIndex, int IShapeIndex, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerID, int IObjectIndex, int IShapeIndex,
                                                void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectNewShapeByIndex"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerID, IObjectIndex, IShapeIndex, Shape);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectNetID(void *Board, int LayerId, const int LayerObjectId, int &NetID) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerId, const int LayerObjectId, int &NetID);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectNetID"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, LayerObjectId, NetID);
    }
    return result;
}

int vSDK::vSDK_Layer_AddShapeByDcode(void *Board,
                                     int LayerId,
                                     int NetID,
                                     int Mirror,
                                     double Angle,
                                     int DcodeId,
                                     bool PositiveNegative,
                                     double X,
                                     double Y,
                                     int &LayerObjectId) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerId, int NetID, int Mirror, double Angle,
                                                int DcodeId, bool PositiveNegative, double X, double Y,
                                                int &LayerObjectId);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_AddShapeByDcode"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, NetID, Mirror, Angle, DcodeId, PositiveNegative, X, Y, LayerObjectId);
    }
    return result;
}

int vSDK::vSDK_Layer_AddShapeByLine(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *_Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_AddShapeByLine"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, NetID, Shape, LayerObjectId);
    }
    return result;
}

int vSDK::vSDK_Layer_AddShapeByArc(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *_Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_AddShapeByArc"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, NetID, Shape, LayerObjectId);
    }
    return result;
}

int vSDK::vSDK_Layer_AddShapeByPolygon(
    void *Board, int LayerId, int NetID, void *GroupPolygons, void *Shape, int &LayerObjectId) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerId, int NetID, void *GroupPolygons, void *Shape,
                                                int &LayerObjectId);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_AddShapeByPolygon"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, NetID, GroupPolygons, Shape, LayerObjectId);
    }
    return result;
}

int vSDK::vSDK_Layer_AddShapeByContinuousLine(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_AddShapeByContinuousLine"));
    if (vSDK_function) {
        result = vSDK_function(Board, LayerId, NetID, Shape, LayerObjectId);
    }
    return result;
}

int vSDK::vSDK_CreateShapeConter(void *&ShapeConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *&ShapeConter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_CreateShapeConter"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter);
    }
    return result;
}

int vSDK::vSDK_Clear_ShapeConter(void *ShapeConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Clear_ShapeConter"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter);
    }
    return result;
}

int vSDK::vSDK_Delete_ShapeConter(void *ShapeConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Delete_ShapeConter"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_AddRound(void *ShapeConter, const int iID, double Dcx, double Dcy, double Diameter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int iID, double Dcx, double Dcy,
                                                double Diameter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_AddRound"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, iID, Dcx, Dcy, Diameter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_AddRectangle(
    void *ShapeConter, const int iID, double Dcx, double Dcy, double Length, double Width) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int iID, double Dcx, double Dcy, double Length,
                                                double Width);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_AddRectangle"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, iID, Dcx, Dcy, Length, Width);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_AddLine(
    void *ShapeConter, const int iID, double StartX, double StartY, double EndX, double EndY, double LineWidth) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int iID, double StartX, double StartY,
                                                double EndX, double EndY, double LineWidth);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_AddLine"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, iID, StartX, StartY, EndX, EndY, LineWidth);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_AddShape(void *ShapeConter, const int iID, void *Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int iID, void *Shape);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_AddShape"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, iID, Shape);
    }
    return result;
}

int vSDK::vSDK_Layer_GetLayerObjectToShapeConter(void *Board,
                                                 const int IMode,
                                                 const int ICurData,
                                                 const int IlayerID,
                                                 const int Iobjid,
                                                 const int Subindex,
                                                 void *ShapeConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Board, const int IMode, const int ICurData, const int IlayerID,
                                                const int Iobjid, const int Subindex, void *ShapeConter);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Layer_GetLayerObjectToShapeConter"));
    if (vSDK_function) {
        result = vSDK_function(Board, IMode, ICurData, IlayerID, Iobjid, Subindex, ShapeConter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetShapeCount(void *ShapeConter, int &ShapeCount) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, int &ShapeCount);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetShapeCount"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, ShapeCount);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetCurDataByIndex(void *ShapeConter, const int iShapeIndex, int &_iCurData) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int iShapeIndex, int &_iCurData);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetCurDataByIndex"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, iShapeIndex, _iCurData);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetShapeByIndex(void *ShapeConter, const int IIndex, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, const int IIndex, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetShapeByIndex"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, IIndex, Shape);
    }
    return result;
}

int vSDK::vSDK_Create_CALC_OBJTOOBJ_DIS_List(void *&OBJTOOBJDISList) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *&OBJTOOBJDISList);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Create_CALC_OBJTOOBJ_DIS_List"));
    if (vSDK_function) {
        result = vSDK_function(OBJTOOBJDISList);
    }
    return result;
}

int vSDK::vSDK_Delete_CALC_OBJTOOBJ_DIS_List(void *OBJTOOBJDISList) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *OBJTOOBJDISList);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Delete_CALC_OBJTOOBJ_DIS_List"));
    if (vSDK_function) {
        result = vSDK_function(OBJTOOBJDISList);
    }
    return result;
}

int vSDK::vSDK_CALC_OBJTOOBJ_DIS_List_GetValueByIndex(void *OBJTOOBJDISList,
                                                      const int IIndex,
                                                      int &aobj,
                                                      double &axmindis,
                                                      double &aymindis,
                                                      int &bobj,
                                                      double &bxmindis,
                                                      double &bymindis,
                                                      double &dis) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *OBJTOOBJDISList, const int IIndex, int &aobj, double &axmindis,
                                                double &aymindis, int &bobj, double &bxmindis, double &bymindis,
                                                double &dis);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_CALC_OBJTOOBJ_DIS_List_GetValueByIndex"));
    if (vSDK_function) {
        result = vSDK_function(OBJTOOBJDISList, IIndex, aobj, axmindis, aymindis, bobj, bxmindis, bymindis, dis);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetDistance(
    void *Shape1Conter, void *Shape2Conter, double DMin, double DMax, void *OBJTOOBJDISList, int &Num) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape1Conter, void *Shape2Conter, double DMin, double DMax,
                                                void *OBJTOOBJDISList, int &Num);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetDistance"));
    if (vSDK_function) {
        result = vSDK_function(Shape1Conter, Shape2Conter, DMin, DMax, OBJTOOBJDISList, Num);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetTouch(void *Shape1Conter, void *Shape2Conter, void *OBJTOOBJDISList, int &Num) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape1Conter, void *Shape2Conter, void *OBJTOOBJDISList,
                                                int &Num);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetTouch"));
    if (vSDK_function) {
        result = vSDK_function(Shape1Conter, Shape2Conter, OBJTOOBJDISList, Num);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetIntersection(void *Shape1Conter, void *Shape2Conter, void *ResultConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape1Conter, void *Shape2Conter, void *ResultConter);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetIntersection"));
    if (vSDK_function) {
        result = vSDK_function(Shape1Conter, Shape2Conter, ResultConter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetCUT(void *Shape1Conter, void *Shape2Conter, void *ResultConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape1Conter, void *Shape2Conter, void *ResultConter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetCUT"));
    if (vSDK_function) {
        result = vSDK_function(Shape1Conter, Shape2Conter, ResultConter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_GetConour(void *ShapeConter, void *ResultConter) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, void *ResultConter);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_GetConour"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, ResultConter);
    }
    return result;
}

int vSDK::vSDK_ShapeConter_Zoom(void *ShapeConter, double ZoomR) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ShapeConter, double ZoomR);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_ShapeConter_Zoom"));
    if (vSDK_function) {
        result = vSDK_function(ShapeConter, ZoomR);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByCircle(
    double X, double Y, double Diameter, bool PositiveNegative, bool Filled, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double X, double Y, double Diameter, bool PositiveNegative, bool Filled,
                                                void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByCircle"));
    if (vSDK_function) {
        result = vSDK_function(X, Y, Diameter, PositiveNegative, Filled, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByRectangle(
    double CenterX, double CenterY, double Length, double Width, bool PositiveNegative, bool Filled, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double CenterX, double CenterY, double Length, double Width,
                                                bool PositiveNegative, bool Filled, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByRectangle"));
    if (vSDK_function) {
        result = vSDK_function(CenterX, CenterY, Length, Width, PositiveNegative, Filled, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByLine(double StartX,
                                       double StartY,
                                       double EndX,
                                       double EndY,
                                       double LineLength,
                                       double LineWidth,
                                       bool IsRectangle,
                                       bool PositiveNegative,
                                       bool Filled,
                                       void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double StartX, double StartY, double EndX, double EndY,
                                                double LineLength, double LineWidth, bool IsRectangle,
                                                bool PositiveNegative, bool Filled, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByLine"));
    if (vSDK_function) {
        result = vSDK_function(StartX, StartY, EndX, EndY, LineLength, LineWidth, IsRectangle, PositiveNegative, Filled,
                               Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByArc(double CenterX,
                                      double CenterY,
                                      double Radius,
                                      double StartAngle,
                                      double AngleRotate,
                                      double LineLength,
                                      double LineWidth,
                                      bool IsRectangle,
                                      bool PositiveNegative,
                                      bool Filled,
                                      void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double CenterX, double CenterY, double Radius, double StartAngle,
                                                double AngleRotate, double LineLength, double LineWidth,
                                                bool IsRectangle, bool PositiveNegative, bool Filled, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByArc"));
    if (vSDK_function) {
        result = vSDK_function(CenterX, CenterY, Radius, StartAngle, AngleRotate, LineLength, LineWidth, IsRectangle,
                               PositiveNegative, Filled, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByArcThreeDrop(double StartX,
                                               double StartY,
                                               double ArcPointX,
                                               double ArcPointY,
                                               double EndX,
                                               double EndY,
                                               double LineLength,
                                               double LineWidth,
                                               bool IsRectangle,
                                               bool PositiveNegative,
                                               bool Filled,
                                               void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double StartX, double StartY, double ArcPointX, double ArcPointY,
                                                double EndX, double EndY, double LineLength, double LineWidth,
                                                bool IsRectangle, bool PositiveNegative, bool Filled, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByArcThreeDrop"));
    if (vSDK_function) {
        result = vSDK_function(StartX, StartY, ArcPointX, ArcPointY, EndX, EndY, LineLength, LineWidth, IsRectangle,
                               PositiveNegative, Filled, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateArcOrPoints(void *&ArcOrPoints) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *&ArcOrPoints);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateArcOrPoints"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints);
    }
    return result;
}

int vSDK::vSDK_Shape_DestroyArcOrPoints(void *ArcOrPoints) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_DestroyArcOrPoints"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints);
    }
    return result;
}

int vSDK::vSDK_Shape_AddArcOrPointByArc(void *ArcOrPoints,
                                        double CenterX,
                                        double CenterY,
                                        double Radius,
                                        double StartAngle,
                                        double AngleRotate,
                                        int &Count) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, double CenterX, double CenterY, double Radius,
                                                double StartAngle, double AngleRotate, int &Count);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_AddArcOrPointByArc"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, CenterX, CenterY, Radius, StartAngle, AngleRotate, Count);
    }
    return result;
}

int vSDK::vSDK_Shape_AddArcOrPointByArcThreeDrop(void *ArcOrPoints,
                                                 double StartX,
                                                 double StartY,
                                                 double ArcPointX,
                                                 double ArcPointY,
                                                 double EndX,
                                                 double EndY,
                                                 int &Count) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, double StartX, double StartY, double ArcPointX,
                                                double ArcPointY, double EndX, double EndY, int &Count);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_AddArcOrPointByArcThreeDrop"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, StartX, StartY, ArcPointX, ArcPointY, EndX, EndY, Count);
    }
    return result;
}

int vSDK::vSDK_Shape_AddArcOrPoint(void *ArcOrPoints,
                                   double ArcOrPointX,
                                   double ArcOrPointY,
                                   double ArcRadius,
                                   double ArcStartAngle,
                                   double ArcAngleRotate,
                                   int &Count) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, double ArcOrPointX, double ArcOrPointY,
                                                double ArcRadius, double ArcStartAngle, double ArcAngleRotate,
                                                int &Count);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_AddArcOrPoint"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, ArcOrPointX, ArcOrPointY, ArcRadius, ArcStartAngle, ArcAngleRotate, Count);
    }
    return result;
}

int vSDK::vSDK_Shape_GetArcOrPointCount(void *ArcOrPoints, int &Count) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, int &Count);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetArcOrPointCount"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, Count);
    }
    return result;
}

int vSDK::vSDK_Shape_GetArcOrPointByIndex(void *ArcOrPoints,
                                          const int Index,
                                          double &ArcOrPointX,
                                          double &ArcOrPointY,
                                          double &ArcOrPointRadius,
                                          double &ArcOrPointStartAngle,
                                          double &ArcOrPointAngleRotate,
                                          int &ArcOrPointType) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(
        void *ArcOrPoints, const int Index, double &ArcOrPointX, double &ArcOrPointY, double &ArcOrPointRadius,
        double &ArcOrPointStartAngle, double &ArcOrPointAngleRotate, int &ArcOrPointType);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetArcOrPointByIndex"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, Index, ArcOrPointX, ArcOrPointY, ArcOrPointRadius, ArcOrPointStartAngle,
                               ArcOrPointAngleRotate, ArcOrPointType);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByPolygon(
    void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled,
                                                void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByPolygon"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, Count, PositiveNegative, Filled, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_CreateShapeByContinuousLine(
    void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled, const double Width, void *&Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled,
                                                const double Width, void *&Shape);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreateShapeByContinuousLine"));
    if (vSDK_function) {
        result = vSDK_function(ArcOrPoints, Count, PositiveNegative, Filled, Width, Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_DestroyShape(void *Shape) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_DestroyShape"));
    if (vSDK_function) {
        result = vSDK_function(Shape);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeType(void *Shape, char *&ShapeType) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, char *&ShapeType);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeType"));
    if (vSDK_function) {
        result = vSDK_function(Shape, ShapeType);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeDataByCircle(
    void *Shape, double &X, double &Y, double &Diameter, bool &PositiveNegative, bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, double &X, double &Y, double &Diameter,
                                                bool &PositiveNegative, bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeDataByCircle"));
    if (vSDK_function) {
        result = vSDK_function(Shape, X, Y, Diameter, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeDataByRectangle(void *Shape,
                                             double &CenterX,
                                             double &CenterY,
                                             double &Length,
                                             double &Width,
                                             bool &PositiveNegative,
                                             bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, double &CenterX, double &CenterY, double &Length,
                                                double &Width, bool &PositiveNegative, bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeDataByRectangle"));
    if (vSDK_function) {
        result = vSDK_function(Shape, CenterX, CenterY, Length, Width, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeDataByLine(void *Shape,
                                        double &StartX,
                                        double &StartY,
                                        double &EndX,
                                        double &EndY,
                                        double &LineLength,
                                        double &LineWidth,
                                        bool &isRectangle,
                                        bool &PositiveNegative,
                                        bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, double &StartX, double &StartY, double &EndX, double &EndY,
                                                double &LineLength, double &LineWidth, bool &isRectangle,
                                                bool &PositiveNegative, bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeDataByLine"));
    if (vSDK_function) {
        result = vSDK_function(Shape, StartX, StartY, EndX, EndY, LineLength, LineWidth, isRectangle, PositiveNegative,
                               Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetArcOrPointsByContinuousLine(void *Shape,
                                                    void *&ArcOrPoints,
                                                    int &Count,
                                                    double &LineLength,
                                                    double &LineWidth,
                                                    bool &isRectangle,
                                                    bool &PositiveNegative,
                                                    bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, void *&ArcOrPoints, int &Count, double &LineLength,
                                                double &LineWidth, bool &isRectangle, bool &PositiveNegative,
                                                bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetArcOrPointsByContinuousLine"));
    if (vSDK_function) {
        result = vSDK_function(Shape, ArcOrPoints, Count, LineLength, LineWidth, isRectangle, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeDataByArc(void *Shape,
                                       double &CenterX,
                                       double &CenterY,
                                       double &Radius,
                                       double &StartAngle,
                                       double &AngleRotate,
                                       double &LineLength,
                                       double &LineWidth,
                                       bool &isRectangle,
                                       bool &PositiveNegative,
                                       bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(
        void *Shape, double &CenterX, double &CenterY, double &Radius, double &StartAngle, double &AngleRotate,
        double &LineLength, double &LineWidth, bool &isRectangle, bool &PositiveNegative, bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeDataByArc"));
    if (vSDK_function) {
        result = vSDK_function(Shape, CenterX, CenterY, Radius, StartAngle, AngleRotate, LineLength, LineWidth,
                               isRectangle, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeDataByArcThreeDrop(void *Shape,
                                                double &StartX,
                                                double &StartY,
                                                double &ArcPointX,
                                                double &ArcPointY,
                                                double &EndX,
                                                double &EndY,
                                                double &LineLength,
                                                double &LineWidth,
                                                bool &isRectangle,
                                                bool &PositiveNegative,
                                                bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(
        void *Shape, double &StartX, double &StartY, double &ArcPointX, double &ArcPointY, double &EndX, double &EndY,
        double &LineLength, double &LineWidth, bool &isRectangle, bool &PositiveNegative, bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeDataByArcThreeDrop"));
    if (vSDK_function) {
        result = vSDK_function(Shape, StartX, StartY, ArcPointX, ArcPointY, EndX, EndY, LineLength, LineWidth,
                               isRectangle, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_GetArcOrPointsByPolygon(
    void *Shape, void *&ArcOrPoints, int &Count, bool &PositiveNegative, bool &Filled) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, void *&ArcOrPoints, int &Count, bool &PositiveNegative,
                                                bool &Filled);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetArcOrPointsByPolygon"));
    if (vSDK_function) {
        result = vSDK_function(Shape, ArcOrPoints, Count, PositiveNegative, Filled);
    }
    return result;
}

int vSDK::vSDK_Shape_CreatePoints(void *&Points) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *&Points);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_CreatePoints"));
    if (vSDK_function) {
        result = vSDK_function(Points);
    }
    return result;
}

int vSDK::vSDK_Shape_DestroyPoints(void *Points) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Points);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_DestroyPoints"));
    if (vSDK_function) {
        result = vSDK_function(Points);
    }
    return result;
}

int vSDK::vSDK_Shape_SetPointsData(void *Points, double DX, double DY) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Points, double DX, double DY);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_SetPointsData"));
    if (vSDK_function) {
        result = vSDK_function(Points, DX, DY);
    }
    return result;
}

int vSDK::vSDK_Shape_GetPointsCount(void *Points, int &PointsCount) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Points, int &PointsCount);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetPointsCount"));
    if (vSDK_function) {
        result = vSDK_function(Points, PointsCount);
    }
    return result;
}

int vSDK::vSDK_Shape_GetPointByIndex(void *Points, const int IIndex, double &dX, double &dY) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Points, const int IIndex, double &dX, double &dY);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetPointByIndex"));
    if (vSDK_function) {
        result = vSDK_function(Points, IIndex, dX, dY);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeBox(void *Shape, double &MinX, double &MinY, double &MaxX, double &MaxY) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, double &MinX, double &MinY, double &MaxX, double &MaxY);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeBox"));
    if (vSDK_function) {
        result = vSDK_function(Shape, MinX, MinY, MaxX, MaxY);
    }
    return result;
}

int vSDK::vSDK_Shape_GetShapeArea(void *Shape, double &Area) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(void *Shape, double &Area);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Shape_GetShapeArea"));
    if (vSDK_function) {
        result = vSDK_function(Shape, Area);
    }
    return result;
}

int vSDK::vSDK_Display_SetLayer(int NLayerIdIn, bool BIsDisplayIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int NLayerIdIn, bool BIsDisplayIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_SetLayer"));
    if (vSDK_function) {
        result = vSDK_function(NLayerIdIn, BIsDisplayIn);
    }
    return result;
}

int vSDK::vSDK_Display_GetLayer(int NLayerIdIn, bool *pbIsDisplayOut) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int NLayerIdIn, bool *pbIsDisplayOut);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_GetLayer"));
    if (vSDK_function) {
        result = vSDK_function(NLayerIdIn, pbIsDisplayOut);
    }
    return result;
}

int vSDK::vSDK_Display_ReloadLayer(int NLayerIdIn, bool BIsDisplayIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int NLayerIdIn, bool BIsDisplayIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_ReloadLayer"));
    if (vSDK_function) {
        result = vSDK_function(NLayerIdIn, BIsDisplayIn);
    }
    return result;
}

int vSDK::vSDK_Display_AutoDisplayLayerByLayerName(char *playerNameIn, bool BIsShowIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(char *playerNameIn, bool BIsShowIn);
    FUN_DYNAMIC_CALL vSDK_function = reinterpret_cast<FUN_DYNAMIC_CALL>(
        def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_AutoDisplayLayerByLayerName"));
    if (vSDK_function) {
        result = vSDK_function(playerNameIn, BIsShowIn);
    }
    return result;
}

int vSDK::vSDK_Display_SetPartDisplay(bool BIsDisplayIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(bool BIsDisplayIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_SetPartDisplay"));
    if (vSDK_function) {
        result = vSDK_function(BIsDisplayIn);
    }
    return result;
}

int vSDK::vSDK_Display_GetPartDisplay(bool *pbIsDisplayOut) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(bool *pbIsDisplayOut);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_GetPartDisplay"));
    if (vSDK_function) {
        result = vSDK_function(pbIsDisplayOut);
    }
    return result;
}

int vSDK::vSDK_Display_SetObjectMode(int NObjectModeIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int NObjectModeIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_SetObjectMode"));
    if (vSDK_function) {
        result = vSDK_function(NObjectModeIn);
    }
    return result;
}

int vSDK::vSDK_Display_GetObjectMode(int *pnObjectModeIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int *pnObjectModeIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_GetObjectMode"));
    if (vSDK_function) {
        result = vSDK_function(pnObjectModeIn);
    }
    return result;
}

int vSDK::vSDK_Display_SetSelectMode(int NSelectModeIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int NSelectModeIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_SetSelectMode"));
    if (vSDK_function) {
        result = vSDK_function(NSelectModeIn);
    }
    return result;
}

int vSDK::vSDK_Display_GetSelectMode(int *pnSelectModeIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(int *pnSelectModeIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_GetSelectMode"));
    if (vSDK_function) {
        result = vSDK_function(pnSelectModeIn);
    }
    return result;
}

int vSDK::vSDK_Display_Goto(double DblPointionXIn, double DblPointYIn) {
    int result = 0;
    typedef int(/*_stdcall*/ *FUN_DYNAMIC_CALL)(double DblPointionXIn, double DblPointYIn);
    FUN_DYNAMIC_CALL vSDK_function =
        reinterpret_cast<FUN_DYNAMIC_CALL>(def_Run_vSDKDllLib_Interface(m_vSdkDllLib, "vSDK_Display_Goto"));
    if (vSDK_function) {
        result = vSDK_function(DblPointionXIn, DblPointYIn);
    }
    return result;
}
