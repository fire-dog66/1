#ifndef VSDK_H
#define VSDK_H

#if defined(DEF_USE_QTFRAMEWORK)
#include <QLibrary>
#else
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <process.h>
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <string>
#include <vector>
#endif

class vSDK {
public:
#if defined(DEF_USE_QTFRAMEWORK)
    QLibrary *m_vSdkDllLib = nullptr;
#else
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || \
    defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HINSTANCE m_vSdkDllLib = nullptr;
#else
    void *m_vSdkDllLib = nullptr;
#endif
#endif
    vSDK();
    ~vSDK();
//----------------------------------------
#define def_PAD_Type_NetId 1
#define def_PAD_Type_PinId 2
#define def_PAD_Type_PartId 3
#define def_PAD_Type_SMD 4
#define def_PAD_Type_PLATED 5
#define def_PAD_Type_USAGE 6
#define def_PAD_Type_VIA_TYPE 7
#define def_PAD_Type_CADNET 8
#define def_PAD_Type_TESTPOINT 9
#define def_PAD_Type_Fiducial 10
#define def_PAD_Type_ViaID 11
//----------------------------------------
#define def_Type_SMD_NO 1
#define def_Type_SMD_YES 2
#define def_Type_SMD_NONE 4
#define def_Type_PLATED_NO 1
#define def_Type_PLATED_YES 2
#define def_Type_PLATED_NONE 4
#define def_Type_USAGE_PIN 1
#define def_Type_USAGE_VIA 2
#define def_Type_USAGE_NONE 4
#define def_Type_TESTPOINT_NO 1
#define def_Type_TESTPOINT_YES 2
#define def_Type_FIDUCIAL_NO 1
#define def_Type_FIDUCIAL_YES 2
    //---------------------------------------
    enum LayerSide {
        lsCOMMON = 0,
        lsTOP,
        lsTRANSPARENT,
        lsBOTTOM,
        lsOTHER,
        lsBOTH,
        lsALL,
        lsNOTAPPLICABLE,
        ls_ext_new_Base = 100,
        ls_ext_new_Other = lsOTHER,
        ls_ext_new_Top = lsTOP,
        ls_ext_new_Inner = lsTRANSPARENT,
        ls_ext_new_Bottom = lsBOTTOM,
        ls_ext_new_MIXED = 200
    };
    enum LayerType {
        ltELECTRICAL = 0,
        ltDOCUMENTATION,
        ltDXF,
        ltBOARD_CUTOUT,
        ltMASKING,
        ltASSEMBLY,
        ltSILKSCREEN,
        ltSOLDERPASTE,
        ltPCBBODYOUTLINE = 20,
        ltOTHER,
        lt_extLEGEND = 200,  //- (the silkscreen layer)
        lt_extGLUE,
        lt_extSOLDERMASK,
        lt_extBOARDOUTLINE,
        lt_extCOATINGCOND,
        lt_extCOATINGNONCOND,
        lt_extCONDUCTOR,
        lt_extCOURTYARD,
        lt_extDIELBASE,
        lt_extDIELCORE,
        lt_extDIELPREG,
        lt_extDIELADHV,
        lt_extSOLDERBUMP,
        lt_extPASTEMASK,
        lt_extHOLEFILL,
        lt_extPIN,
        lt_extCOMPONENT,
        lt_extRESISTIVE,
        lt_extCAPACITIVE,
        lt_extPROBE,
        lt_extREWORK,
        lt_extFIXTURE,
        lt_extGRAPHIC,
        lt_ext_new_Base = 1000,
        lt_ext_new_Signal = ltELECTRICAL,
        lt_ext_new_Soldermask = ltMASKING,
        lt_ext_new_Solderpaste = ltSOLDERPASTE,
        lt_ext_new_Silkscreen = ltSILKSCREEN,
        lt_ext_new_Drill = 1010,
        lt_ext_new_Rout,
        lt_ext_new_Profile = ltPCBBODYOUTLINE,
        lt_ext_new_POrG = 1020,
        lt_ext_new_Keepout = ltBOARD_CUTOUT,
        lt_ext_new_Text = 1030,
        lt_ext_new_BackDrill = 1031,
        lt_ext_new_THTPaste = 1035,
        lt_ext_new_PANELPROFILE = 1040,
        lt_ext_new_Pads,
        lt_ext_new_COMPONENT,
        lt_ext_new_DFMANALYSISAREA,
        lt_ext_new_DFMPANELAREA,
        lt_ext_new_PANELCOMPONENT,
        lt_ext_new_Stencil,
        lt_ext_new_Stencil_Step,
        lt_ext_new_CoolingPlate,
        lt_ext_new_PANELDESIGN,
        lt_ext_new_JetPrint = 1050,
        lt_ext_new_JetArea,
        lt_ext_new_Other = 1999,
    };

    struct TNailInfo {
        int mNailNumber;
        int mSide;  // 1-T,2-B;
        int mNet_ID;
        int mVirtual;
        int mPV_Type;
        char mType[8];  // 针点圆类型;
        char mGrid[16];
        char mNet_Name[256];
        char mPin_Via[256];
        double mX;
        double mY;
        double Dia;  // 针点圆直径;
    };

    typedef struct tagColor {
        int tag_nRed;
        int tag_nGreen;
        int tag_nBlue;
        int tag_nAlpha;
    } *PTAG_COLOR, TAG_COLOR;

    /**
     * 元件选择集*/
    typedef struct tagSelectedParts {
        int tag_nPartCount;    // board元件数量
        int *tag_pnPartIds;    // 元件选择集信息
        char *tag_strPanelId;  // board拼版Id -1为当前基板
        tagSelectedParts() {
            tag_nPartCount = 0;
            tag_pnPartIds = nullptr;
            tag_strPanelId = nullptr;
        }
        void ReleasetagSelectedParts() {
            if (1 == tag_nPartCount) {
                delete tag_pnPartIds;
            } else if (1 < tag_nPartCount) {
                delete[] tag_pnPartIds;
            }
            if (nullptr != tag_strPanelId) {
                delete tag_strPanelId;
            }
            tag_strPanelId = nullptr;
            tag_pnPartIds = nullptr;
        }
    } *PTAG_SELECTED_PARTS, TAG_SELECTED_PARTS;

    typedef struct tagSelectedBoardParts {
        int tag_nBoardCount;                // board数量
        PTAG_SELECTED_PARTS tag_pAryBoard;  // board选择集数据
        tagSelectedBoardParts() {
            tag_nBoardCount = 0;
            tag_pAryBoard = nullptr;
        }
        void ReleasetagSelectedBoardParts() {
            if (1 == tag_nBoardCount) {
                delete tag_pAryBoard;
            } else if (1 < tag_nBoardCount) {
                delete[] tag_pAryBoard;
            }
            tag_pAryBoard = nullptr;
            tag_nBoardCount = 0;
        }
    } *PTAG_SELECTED_BOARD_PARTS, TAG_SELECTED_BOARD_PARTS;

    typedef struct tagPartPins {
        int tag_nPartId;
        int tag_nPartPinsCount;
        int *tag_pAryPartPins;
        TAG_COLOR tag_colorBase;
        int tag_nIsCustom;
        tagPartPins() {
            tag_nPartId = -1;
            tag_nPartPinsCount = 0;
            tag_pAryPartPins = nullptr;
            tag_nIsCustom = -1;
        }
        void ReleasetagPartPins() {
            if (1 == tag_nPartPinsCount) {
                delete tag_pAryPartPins;
            } else if (1 < tag_nPartPinsCount) {
                delete[] tag_pAryPartPins;
            }
            tag_pAryPartPins = nullptr;
        }
    } *PTAG_PART_PINS, TAG_PART_PINS;

    /**
     * pin/via选择集*/
    typedef struct tagSelectedPartPins {
        int tag_nPartCount;              // board元件数量
        PTAG_PART_PINS tag_pAryPartIds;  // 元件选择集信息
        char *tag_strPanelId;            // board拼版Id -1为当前基板
        tagSelectedPartPins() {
            tag_nPartCount = 0;
            tag_pAryPartIds = nullptr;
            tag_strPanelId = nullptr;
        }
        void ReleasetagSelectedPartPins() {
            if (1 == tag_nPartCount) {
                delete tag_pAryPartIds;
            } else if (1 < tag_nPartCount) {
                delete[] tag_pAryPartIds;
            }
            if (nullptr != tag_strPanelId) {
                delete tag_strPanelId;
            }
            tag_strPanelId = nullptr;
            tag_pAryPartIds = nullptr;
        }
    } *PTAG_SELECTED_PART_PIN, TAG_SELECTED_PART_PIN;

    typedef struct tagSelectedBoardPartPins {
        int tag_nBoardCount;                   // board数量
        PTAG_SELECTED_PART_PIN tag_pAryBoard;  // board选择集数据
        tagSelectedBoardPartPins() {
            tag_nBoardCount = 0;
            tag_pAryBoard = nullptr;
        }
        void ReleasetagSelectedBoardPartPins() {
            if (1 == tag_nBoardCount) {
                delete tag_pAryBoard;
            } else if (1 < tag_nBoardCount) {
                delete[] tag_pAryBoard;
            }
            tag_pAryBoard = nullptr;
            tag_nBoardCount = 0;
        }
    } *PTAG_SELECTED_BOARD_PART_PIN, TAG_SELECTED_BOARD_PART_PIN;

    /**
     * 信号选择集*/
    typedef struct tagSelectedSignals {
        int tag_nSignalCount;  // board信号数量
        int *tag_pArySignals;  // 信号选择集信息
        char *tag_strPanelId;  // board拼版Id -1为当前基板
        tagSelectedSignals() {
            tag_nSignalCount = 0;
            tag_pArySignals = nullptr;
            tag_strPanelId = nullptr;
        }
        void ReleasetagSelectedSignals() {
            if (1 == tag_nSignalCount) {
                delete tag_pArySignals;
            } else if (1 < tag_nSignalCount) {
                delete[] tag_pArySignals;
            }
            if (nullptr != tag_strPanelId) {
                delete tag_strPanelId;
            }

            tag_pArySignals = nullptr;
        }
    } *PTAG_SELECTED_SIGNALS, TAG_SELECTED_SIGNALS;

    typedef struct tagSelectedBoardSignals {
        int tag_nBoardCount;                  // board数量
        PTAG_SELECTED_SIGNALS tag_pAryBoard;  // board选择集数据
        tagSelectedBoardSignals() {
            tag_nBoardCount = 0;
            tag_pAryBoard = nullptr;
        }
        void ReleasetagSelectedSignals() {
            if (1 == tag_nBoardCount) {
                delete tag_pAryBoard;
            } else if (1 < tag_nBoardCount) {
                delete[] tag_pAryBoard;
            }
            tag_pAryBoard = nullptr;
            tag_nBoardCount = 0;
        }
    } *PTAG_SELECTED_BOARD_SIGNALS, TAG_SELECTED_BOARD_SIGNALS;

    typedef struct tagSelectedLayerFeatures {
        int tag_nLayerId;
        int tag_nFeaturesCount;
        int *tag_pAryFeatures;
        tagSelectedLayerFeatures() {
            tag_nLayerId = -1;
            tag_nFeaturesCount = 0;
            tag_pAryFeatures = nullptr;
        }
        void ReleasetagSelectedLayerFeatures() {
            if (1 == tag_nFeaturesCount) {
                delete tag_pAryFeatures;
            } else if (1 < tag_nFeaturesCount) {
                delete[] tag_pAryFeatures;
            }
            tag_pAryFeatures = nullptr;
        }
    } *PTAG_SELECTED_LAYER_FEATURES, TAG_SELECTED_LAYER_FEATURES;

    /**
     * 特征选择集*/
    typedef struct tagSelectedFeatures {
        char *tag_strPanelId;                                // board拼版Id -1为当前基板
        int tag_nLayerIdCount;                               // board层数量
        PTAG_SELECTED_LAYER_FEATURES tag_pAryLayerFeatures;  // 层选择集信息
        tagSelectedFeatures() {
            tag_strPanelId = nullptr;
            tag_nLayerIdCount = 0;
            tag_pAryLayerFeatures = nullptr;
        }
        void ReleasetagSelectedFeatures() {
            if (1 == tag_nLayerIdCount && nullptr != tag_pAryLayerFeatures) {
                delete tag_pAryLayerFeatures;
            } else if (1 < tag_nLayerIdCount && nullptr != tag_pAryLayerFeatures) {
                delete[] tag_pAryLayerFeatures;
            }
            if (nullptr != tag_strPanelId) {
                delete tag_strPanelId;
            }
            tag_strPanelId = nullptr;
            tag_pAryLayerFeatures = nullptr;
        }
    } *PTAG_SELECTED_FEATURES, TAG_SELECTED_FEATURES;

    typedef struct tagSelectedBoardsFeatures {
        int tag_nBoardCount;                   // board数量
        PTAG_SELECTED_FEATURES tag_pAryBoard;  // board选择集数据
        tagSelectedBoardsFeatures() {
            tag_nBoardCount = 0;
            tag_pAryBoard = nullptr;
        }
        void ReleasetagSelectedBoardsFeatures() {
            if (1 == tag_nBoardCount) {
                delete tag_pAryBoard;
            } else if (1 < tag_nBoardCount) {
                delete[] tag_pAryBoard;
            }
            tag_pAryBoard = nullptr;
            tag_nBoardCount = 0;
        }
    } *PTAG_SELECTED_BOARD_FEATURES, TAG_SELECTED_BOARD_FEATURES;

    typedef struct tagPartSetting {
        bool tag_bFlag;
        TAG_COLOR tag_clrSelectColor;
        bool tag_bShowMainBody;
        TAG_COLOR tag_clrMainBodyColor;
        bool tag_bShowOutLine;
        TAG_COLOR tag_clrOutLine;
        bool tag_bShowPartName;
        TAG_COLOR tag_clrPartNameColor;
        bool tag_bShowPolarityPoint;
        TAG_COLOR tag_clrPolarityPointColr;

        bool tag_bShowPin;
        bool tag_bSelected;
        TAG_COLOR tag_clrPinColir;
        bool tag_bShowPinName;
        TAG_COLOR tag_clrPinNameColor;
        TAG_COLOR tag_clrFirstPinColor;
        TAG_COLOR tag_PinPolarityColor;
        TAG_COLOR tag_PinHoleColor;
        bool tag_bIsContour;
    } *PTAG_PART_SETTING, TAG_PART_SETTING;
    enum ENM_OBJECTIVE_MODE {
        OBJECTIVE_MODE_FILL = 1,         // 填充模式
        OBJECTIVE_MODE_TRANSPARENT = 2,  // 透明度 ENM_OBJECTIVE_MODE为OBJECTIVE_MODE_TRANSPARENT时，该变量会被使用。
        OBJECTVE_MODE_MIX = 3,           // 混合模式
        OBJECTIVE_MODE_LINE = 4          // 轨迹模式
    };

    struct DArcPoint  // 此处定义多边形点
    {
        double x;
        double y;
        double angle_start;  // 弧度;
        double angle_Rot;    // 弧度;
        double radius;
        DArcPoint() {
            x = 0.0;
            y = 0.0;
            angle_start = 0.0;
            radius = 0.0;
            angle_Rot = 0.0;
        }
    };

    struct DShapePoints  // 此处定义多边形点
    {
        bool bPositive;
        int nShapeId;
        int nShapeType;
        double width;
        std::vector<DArcPoint> vecPoints;
        DShapePoints() {
            bPositive = true;
            nShapeId = 0;
            nShapeType = 0;
            width = 0.0;
            vecPoints.clear();
        }
    };

    struct DShapeGroup  // 图形组
    {
        int netID;
        int pinID;
        int partID;
        int pad_trace_polygon;
        int _type;
        unsigned int unObjId;  // 层特征序号,数组下标
        int nShapeType;

        double min_x, min_y, max_x, max_y;
        bool bPositive;     // 图形组正负片
        bool bHasNegative;  // 图形组里子图形是否有负片
        std::vector<DShapePoints> vecShapes;
        DShapeGroup() {
            netID = 0;
            pinID = 0;
            partID = 0;
            pad_trace_polygon = 0;

            nShapeType = 0;

            min_x = 1e20;
            min_y = 1e20;
            max_x = -1e20;
            max_y = -1e20;

            bPositive = true;
            bHasNegative = false;
            vecShapes.clear();
        }
        bool operator==(const DShapeGroup &other) const {
            return unObjId == other.unObjId;
        }
    };
    typedef struct tagDPOINT {
        double x;
        double y;
        tagDPOINT() {
            x = 0.0;
            y = 0.0;
        }
        tagDPOINT(double _dx, double _dy) {
            x = _dx;
            y = _dy;
        }
    } DPoint;
    struct PartDrawText {
        int partID;  // 元件id
        int nPinId;
        int type;                           // 5-pinText数据;6-partText数据;
        double min_x, min_y, max_x, max_y;  // 包围盒，与DShapeGroup的类似
        double partTextAngle;               // part的角度
        DPoint centerPoint;                 // 文字中心
        std::string objName;                // 引脚数据名，元件名
    };

    typedef struct tagPreviewShapeData {
        DShapeGroup tag_shapData;
        ENM_OBJECTIVE_MODE tag_enmObjectiveMode;

    } *PTAG_PREVIEWSHAPE_DATA, TAG_PREVIEWSHAPE_DATA;

    typedef struct tagPDFLayerInfo {
        char *tag_pstrLayerName;    // PCB Layer名称
        bool tag_bIsPainted;        // 是否绘制
        int tag_nMirror;            // 镜像 0 不镜像  1X轴镜像（旋转180度） 2Y轴镜像
        double tag_dblAngle;        // 层旋转角度
        bool tag_bIsCusColorInUse;  // 是否使用自定义颜色
        int tag_nColorR;            // 颜色Red值
        int tag_nColorG;            // 颜色Green值
        int tag_nColorB;            // 颜色Blue值
        int tag_nColorA;            // 颜色透明值
        char *tag_pstrUnknown;      // 预留：附加字段
        tagPDFLayerInfo() {
            tag_pstrLayerName = nullptr;
            tag_pstrUnknown = nullptr;
        }
        void CleartagPDFLayerInfo() {
            if (nullptr != tag_pstrLayerName) {
                delete tag_pstrLayerName;
                tag_pstrLayerName = nullptr;
            }
            if (nullptr != tag_pstrUnknown) {
                delete tag_pstrUnknown;
                tag_pstrUnknown = nullptr;
            }
        }
    } *PTAG_PDF_LAYERINFO, TAG_PDF_LAYERINFO;

    typedef struct tagPDFBoardLayerInfo {
        char *tag_pstrBoardName;                    // PCB Layer名称
        int tag_nPDFLayersCount;                    // 层信息数量
        PTAG_PDF_LAYERINFO tag_pAryPDFLayerInfoes;  // 层信息数组
        tagPDFBoardLayerInfo() {
            tag_pstrBoardName = nullptr;
            tag_nPDFLayersCount = 0;
            tag_pAryPDFLayerInfoes = nullptr;
        }
        void CleartagPDFBoardLayerInfo() {
            if (nullptr != tag_pstrBoardName) {
                delete tag_pstrBoardName;
                tag_pstrBoardName = nullptr;
            }
            tag_nPDFLayersCount = 0;
            if (nullptr != tag_pAryPDFLayerInfoes) {
                delete tag_pAryPDFLayerInfoes;
                tag_pAryPDFLayerInfoes = nullptr;
            }
        }
    } *PTAG_BOARD_PDF_LAYERINFO, TAG_BOARD_PDF_LAYERINFO;

    typedef struct tagDrawWindowInfo {
        int tag_nWindowWidth;   // 屏幕绘制区域宽
        int tag_nWindowHeight;  // 屏幕绘制区域高
        double tag_dblScale;    // 当前缩放比例
        double tag_dblCenterX;  // 当前中心坐标X（物理）
        double tag_dblCenterY;  // 当前中心坐标Y
    } *PTAG_DRAW_WINDOWINFO, TAG_DRAW_WINDOWINFO;
    enum ExportDirection { D_Landscape, D_Portrait };
    typedef struct PdfSettings  // PDF导出选择的信息
    {
        int PageSize;                   // 0 : B3  1: B4  2:A3  3 :A4
        int leftMargin;                 // 左边距（单位：毫米）
        int topMargin;                  // 上边距（单位：毫米）
        int rightMargin;                // 右边距（单位：毫米）
        int bottomMargin;               // 下边距（单位：毫米）
        bool CurrentView;               // 当前视图
        bool AdaptiveView;              // 自适应
        bool ScaleView;                 // 缩放视图
        double ScaleValue;              // 缩放比例
        ExportDirection PageDirection;  // 导出方向
    } PDF_SETTING;

    typedef struct tagPDFGroupInfo {
        int tag_nPDFBoardsCount;                          // 层信息数量
        PTAG_BOARD_PDF_LAYERINFO tag_pAryPDFBoardInfoes;  // 层信息数组
        PDF_SETTING tag_pdfSetting;                       // PDF配置
        TAG_DRAW_WINDOWINFO tag_drawWindowInfo;           // 窗口绘制信息
        double tag_dblBoundStartX;                        // 包围盒起始坐标X （左下角）
        double tag_dblBoundStartY;                        // 包围盒起始坐标Y
        double tag_dblBoundEndX;                          // 包围盒结束坐标X  （右上角）
        double tag_dblBoundEndY;                          // 包围盒结束坐标U
        char *tag_pstrUnknown;                            // 预留：附加字段
        tagPDFGroupInfo() {
            tag_pstrUnknown = nullptr;
            tag_nPDFBoardsCount = 0;
            tag_pAryPDFBoardInfoes = nullptr;
        }

        void CleartagPDFGroupInfo() {
            if (nullptr == tag_pstrUnknown) {
                delete tag_pstrUnknown;
                tag_pstrUnknown = nullptr;
            }

            tag_nPDFBoardsCount = 0;
            if (nullptr == tag_pAryPDFBoardInfoes) {
                delete tag_pAryPDFBoardInfoes;
                tag_pAryPDFBoardInfoes = nullptr;
            }
        }
    } *PTAG_PDF_GOURPINFO, TAG_PDF_GOURPINFO;

    typedef struct tagLayerShowStatus {
        char *tag_pstrLayerName;
        bool tag_bShowStatus;
        tagLayerShowStatus() {
            tag_pstrLayerName = nullptr;
        }
        void CleartagLayerShowStatus() {
            if (nullptr != tag_pstrLayerName) {
                delete tag_pstrLayerName;
                tag_pstrLayerName = nullptr;
            }
        }
    } *PTAG_LAYER_SHOWSTATUS, TAG_LAYER_SHOWSTATUS;

    typedef struct tagBoardLayerShowStatus {
        char *tag_pstrBoardName;
        int tag_nLayersCount;
        PTAG_LAYER_SHOWSTATUS pAryLayerShowStatusDtas;
        tagBoardLayerShowStatus() {
            tag_pstrBoardName = nullptr;
            tag_nLayersCount = 0;
            pAryLayerShowStatusDtas = nullptr;
        }

        void CleartagBoardLayerShowStatus() {
            if (nullptr != tag_pstrBoardName) {
                delete tag_pstrBoardName;
                tag_pstrBoardName = nullptr;
            }

            tag_nLayersCount = 0;
            if (nullptr != pAryLayerShowStatusDtas) {
                delete pAryLayerShowStatusDtas;
                pAryLayerShowStatusDtas = nullptr;
            }
        }
    } *PTAG_BOARD_LAYER_SHOWSTATUS, TAG_BOARD_LAYER_SHOWSTATUS;

    typedef struct tagHighLightFeature {
        int tag_nFeatureCount;
        int *tag_arynFeatureObjectIds;
        TAG_COLOR tag_clrFeature;
    } *PTAG_HIGHLIGHT_FEATURE, TAG_HIGHLIGHT_FEATURE;

    typedef struct tagHighLightFeatureGroup {
        int tag_nLayerId;
        int tag_nGroupCount;
        PTAG_HIGHLIGHT_FEATURE tag_aryHighLightFeatures;
    } *PTAG_HIGHLIGHT_FEATURE_GROUP, TAG_HIGHLIGHT_FEATURE_GROUP;

    typedef struct tagCalcCollisionData  // 此处定义多边形点
    {
        int nGroupIndex;       // 图形组下标
        int nGroupShapeIndex;  // 图形组内下标
        int nShapeIndex;       // 图形下标
        int nShapeType;        // 图形类型 1圆 2矩形 14带宽度的弧 16多边形 24带宽度的线
        float width;           // 宽度（14带宽度的弧线 24带宽度的线 其他默认0.0）

        std::vector<DArcPoint> vecPoints;
        float fltMinX;  // 包围盒
        float fltMinY;
        float fltMaxX;
        float fltMaxY;
        tagCalcCollisionData() {
            nShapeType = 0;
            width = 0.0;
            vecPoints.clear();
            fltMinX = -1.0;
            fltMinY = -1.0;
            fltMaxX = -1.0;
            fltMaxY = -1.0;
            nGroupIndex = 0;
            nGroupShapeIndex = 0;
            nShapeIndex = 0;
        }
    } *PTAG_CALC_COLLISION_DATA, TAG_CALC_COLLISION_DATA;

    // 2025122301 vSDK_Display_SetLayerColor：改批量设置层颜色
    typedef struct tagLayerColorSet {
        int tag_nLayerId;
        TAG_COLOR tagLineColor;
        TAG_COLOR tagPadColor;
        TAG_COLOR tagPolygonColor;
    } *PTAG_LAYER_COLORSET, TAG_LAYER_COLORSET;

    // 非望友产品,其他应用程序使用时,需先调用vSDK_Init
    // const char* Exepath,应用程序目录全路径
    int vSDK_Init(const char *Exepath);

    int vSDK_GetCurrentJob(void *&Job);

    // 获取当前的Pcb
    int vSDK_Job_GetCurrentPcb(void *Job, void *&Pcb);

    int vSDK_Pcb_GetBoard(void *Pcb, void *&Board);

    // Board中添加层, const char* LayerName:层名称;返回void* &Layer
    int vSDK_Board_AddLayer(void *Board, const char *LayerName, void *&Layer);

    // 获取Board中层Layer数量
    int vSDK_Board_GetLayerListCount(void *Board, int &LayerCount);

    // 根据序号(0~n),获取Board中序号对应的层Layer
    int vSDK_Board_GetLayerByIndex(void *Board, int LayerIndex, void *&Layer);

    // 根据层名称获取层Layer
    int vSDK_Board_GetLayerByName(void *Board, const char *LayerName, void *&Layer);

    // 刷新层列表
    int vSDK_Board_UpdateLayerList(void *Pcb);

    // 更新层列表文件
    int vSDK_Board_UpdateLayerConfigFile(void *Pcb, bool BeNewProj);

    // 根据矩形大小及中心点坐标,查找单独矩形Dcode,存在获取单独矩形Dcode的DcodeID
    int vSDK_DcodeTable_FindRectangleDcodeIDBySize(void *DcodeTable,
                                                   const double CenterX,
                                                   const double CenterY,
                                                   const double Length,
                                                   const double Width,
                                                   int &DcodeID);

    int vSDK_DcodeTable_FindAndCreateRoundDcodeIDBySize(
        void *DcodeTable, double CenterX, double CenterY, double Size, int &DcodeID);

    // 在D码表DcodeTable中创建新的Dcode
    int vSDK_Dcode_CreateDcode(void *DcodeTable, const char *DcodeName, const char *DcodeAliasName, void *&DCode);

    // 在新建的Dcode中加图形(Shape),支持加多个
    int vSDK_Dcode_AddDcodeShape(void *DcodeTable, void *DCode, void *Shape);

    // 新建的Dcode图形加完后,获取Dcode的DcodeID
    int vSDK_Dcode_AddDcodeEnd(void *DcodeTable, void *DCode, int &DcodeID);

    int vSDK_Layer_GetLayerID(void *Layer, int &LayerID);

    int vSDK_Layer_GetLayerName(void *Layer, char *&LayerName);

    int vSDK_Layer_GetLayerSide(void *Layer, int &iLayerSide);

    int vSDK_Layer_GetLayerType(void *Layer, int &iLayerType);

    int vSDK_Layer_GetDcodeCountByLayerId(void *Board, const int LayerId, int &DcodeCount, void *&DcodeTable);

    // 加载层,要获取层上object,需要先加载层
    int vSDK_Layer_LoadLayerByLayerID(void *Board, const int LayerID);

    // 获取层上对象数量
    int vSDK_Layer_GetLayerObjectCount(void *Board, int LayerID, int &iObjectCount);

    int vSDK_Layer_GetLayerObjectType(void *Board, int LayerID, int IObjectIndex, int &iShapeType);

    // 循环iObjectCount,获取序号对应的Object形状数(嵌套多边形和Dcode有嵌套形状时,iShapeCount > 1)
    int vSDK_Layer_GetLayerObjectShapeCount(void *Board, int LayerID, int IObjectIndex, int &iShapeCount);

    // 循环iObjectCount和二级循环iShapeCount,获取序号对应的Object的第几个图形Shape
    // void* &Shape使用完需vSDK_Shape_DestroyShape
    int vSDK_Layer_GetLayerObjectShapeByIndex(void *Board,
                                              int LayerID,
                                              int IObjectIndex,
                                              int IShapeIndex,
                                              int &Mirror,
                                              double &Angle,
                                              int &DcodeId,
                                              void *&Shape);
    int vSDK_Layer_GetLayerObjectNewShapeByIndex(
        void *Board, int LayerID, int IObjectIndex, int IShapeIndex, void *&Shape);

    int vSDK_Layer_GetLayerObjectNetID(void *Board, int LayerId, const int kLayerObjectId, int &NetID);

    // 加Object,根据Dcode
    int vSDK_Layer_AddShapeByDcode(void *Board,
                                   int LayerId,
                                   int NetID,
                                   int Mirror,
                                   double Angle,
                                   int DcodeId,
                                   bool PositiveNegative,
                                   double X,
                                   double Y,
                                   int &LayerObjectId);

    int vSDK_Layer_AddShapeByLine(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);

    int vSDK_Layer_AddShapeByArc(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);

    // 如多边形无嵌套,void* GroupPolygons传nullptr
    int vSDK_Layer_AddShapeByPolygon(
        void *Board, int LayerId, int NetID, void *GroupPolygons, void *Shape, int &LayerObjectId);

    int vSDK_Layer_AddShapeByContinuousLine(void *Board, int LayerId, int NetID, void *Shape, int &LayerObjectId);

    // 存储计算距离结果的结构
    // 加入的数据请先转为统一单位为mm
    // 创建数据运算数据存储仓库,注意:void* &ShapeConter,使用完需用vSDK_Delete_ShapeConter释放
    int vSDK_CreateShapeConter(void *&ShapeConter);

    // 清空数据运算数据存储仓库
    int vSDK_Clear_ShapeConter(void *ShapeConter);

    // 删除数据运算数据存储仓库
    int vSDK_Delete_ShapeConter(void *ShapeConter);

    // 向数据存储仓库中加入圆形图形,iID:为编号,计算后返回对应的编号
    int vSDK_ShapeConter_AddRound(void *ShapeConter, const int iID, double Dcx, double Dcy, double Diameter);

    // 向数据存储仓库中加入矩形图形
    int vSDK_ShapeConter_AddRectangle(
        void *ShapeConter, const int iID, double Dcx, double Dcy, double Length, double Width);

    // 向数据存储仓库中加入两点线段
    int vSDK_ShapeConter_AddLine(
        void *ShapeConter, const int iID, double StartX, double StartY, double EndX, double EndY, double LineWidth);

    // 向数据存储仓库中加入图形
    int vSDK_ShapeConter_AddShape(void *ShapeConter, const int iID, void *Shape);

    // 从对应层中取对应的obj图形数据,放置到数据存储仓库中;
    // IMode != 0,内部会先裁剪去除挖孔,后面取到的图形都为正片;
    // ICurData:为编号
    // int Subindex:默认传0;
    int vSDK_Layer_GetLayerObjectToShapeConter(void *Board,
                                               const int IMode,
                                               const int ICurData,
                                               const int IlayerID,
                                               const int Iobjid,
                                               const int Subindex,
                                               void *ShapeConter);

    // 获取数据存储仓库中图形数量
    int vSDK_ShapeConter_GetShapeCount(void *ShapeConter, int &ShapeCount);

    // 获取原始设置的编号
    int vSDK_ShapeConter_GetCurDataByIndex(void *ShapeConter, const int iShapeIndex, int &_iCurData);

    // 注意:void* &Shape使用完需释放
    int vSDK_ShapeConter_GetShapeByIndex(void *ShapeConter, const int IIndex, void *&Shape);

    // 创建记录计算结果数组
    int vSDK_Create_CALC_OBJTOOBJ_DIS_List(void *&OBJTOOBJDISList);

    // 删除记录计算结果数组
    int vSDK_Delete_CALC_OBJTOOBJ_DIS_List(void *OBJTOOBJDISList);

    // 根据下标获取数组中各个值
    int vSDK_CALC_OBJTOOBJ_DIS_List_GetValueByIndex(void *OBJTOOBJDISList,
                                                    const int IIndex,
                                                    int &aobj,
                                                    double &axmindis,
                                                    double &aymindis,
                                                    int &bobj,
                                                    double &bxmindis,
                                                    double &bymindis,
                                                    double &dis);

    // 计算距离,返回满足的(DMin <= value < DMax)
    // OBJTOOBJDISList:返回的结构中aobj,bobj为加入图形的编号,不是同一个图形请编不同的号
    int vSDK_ShapeConter_GetDistance(
        void *Shape1Conter, void *Shape2Conter, double DMin, double DMax, void *OBJTOOBJDISList, int &Num);

    // 计算碰撞
    int vSDK_ShapeConter_GetTouch(void *Shape1Conter, void *Shape2Conter, void *OBJTOOBJDISList, int &Num);

    // 两组图形求交集,c=a^b
    int vSDK_ShapeConter_GetIntersection(void *Shape1Conter, void *Shape2Conter, void *ResultConter);

    // 两组图形数据求裁切,c=a-b
    int vSDK_ShapeConter_GetCUT(void *Shape1Conter, void *Shape2Conter, void *ResultConter);

    // 图形求轮廓,注意:ShapeConter(如有负片图形,请在加入时编号编-1)
    int vSDK_ShapeConter_GetConour(void *ShapeConter, void *ResultConter);

    // 图形放大,缩小;ZoomR正值:放大,负值:缩小
    int vSDK_ShapeConter_Zoom(void *ShapeConter, double ZoomR);

    // 所有的基础单位都是mm
    int vSDK_Shape_CreateShapeByCircle(
        double X, double Y, double Diameter, bool PositiveNegative, bool Filled, void *&Shape);

    int vSDK_Shape_CreateShapeByRectangle(
        double CenterX, double CenterY, double Length, double Width, bool PositiveNegative, bool Filled, void *&Shape);

    // IsRectangle:false,为圆头线段,double LineLength(矩形线头用), double LineWidth赋相同值
    int vSDK_Shape_CreateShapeByLine(double StartX,
                                     double StartY,
                                     double EndX,
                                     double EndY,
                                     double LineLength,
                                     double LineWidth,
                                     bool IsRectangle,
                                     bool PositiveNegative,
                                     bool Filled,
                                     void *&Shape);

    int vSDK_Shape_CreateShapeByArc(double CenterX,
                                    double CenterY,
                                    double Radius,
                                    double StartAngle,
                                    double AngleRotate,
                                    double LineLength,
                                    double LineWidth,
                                    bool IsRectangle,
                                    bool PositiveNegative,
                                    bool Filled,
                                    void *&Shape);

    int vSDK_Shape_CreateShapeByArcThreeDrop(double StartX,
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
                                             void *&Shape);

    int vSDK_Shape_CreateArcOrPoints(void *&ArcOrPoints);

    int vSDK_Shape_DestroyArcOrPoints(void *ArcOrPoints);

    int vSDK_Shape_AddArcOrPointByArc(void *ArcOrPoints,
                                      double CenterX,
                                      double CenterY,
                                      double Radius,
                                      double StartAngle,
                                      double AngleRotate,
                                      int &Count);

    int vSDK_Shape_AddArcOrPointByArcThreeDrop(void *ArcOrPoints,
                                               double StartX,
                                               double StartY,
                                               double ArcPointX,
                                               double ArcPointY,
                                               double EndX,
                                               double EndY,
                                               int &Count);

    int vSDK_Shape_AddArcOrPoint(void *ArcOrPoints,
                                 double ArcOrPointX,
                                 double ArcOrPointY,
                                 double ArcRadius,
                                 double ArcStartAngle,
                                 double ArcAngleRotate,
                                 int &Count);

    int vSDK_Shape_GetArcOrPointCount(void *ArcOrPoints, int &Count);

    int vSDK_Shape_GetArcOrPointByIndex(void *ArcOrPoints,
                                        const int Index,
                                        double &ArcOrPointX,
                                        double &ArcOrPointY,
                                        double &ArcOrPointRadius,
                                        double &ArcOrPointStartAngle,
                                        double &ArcOrPointAngleRotate,
                                        int &ArcOrPointType);

    int vSDK_Shape_CreateShapeByPolygon(void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled, void *&Shape);

    int vSDK_Shape_CreateShapeByContinuousLine(
        void *ArcOrPoints, int Count, bool PositiveNegative, bool Filled, const double Width, void *&Shape);

    int vSDK_Shape_DestroyShape(void *Shape);

    //"Circle","Rectangle","Line","ContinuousLine","Arc","Polygon"
    int vSDK_Shape_GetShapeType(void *Shape, char *&ShapeType);

    int vSDK_Shape_GetShapeDataByCircle(
        void *Shape, double &X, double &Y, double &Diameter, bool &PositiveNegative, bool &Filled);

    int vSDK_Shape_GetShapeDataByRectangle(void *Shape,
                                           double &CenterX,
                                           double &CenterY,
                                           double &Length,
                                           double &Width,
                                           bool &PositiveNegative,
                                           bool &Filled);

    int vSDK_Shape_GetShapeDataByLine(void *Shape,
                                      double &StartX,
                                      double &StartY,
                                      double &EndX,
                                      double &EndY,
                                      double &LineLength,
                                      double &LineWidth,
                                      bool &isRectangle,
                                      bool &PositiveNegative,
                                      bool &Filled);

    int vSDK_Shape_GetArcOrPointsByContinuousLine(void *Shape,
                                                  void *&ArcOrPoints,
                                                  int &Count,
                                                  double &LineLength,
                                                  double &LineWidth,
                                                  bool &isRectangle,
                                                  bool &PositiveNegative,
                                                  bool &Filled);

    // 圆弧起始弧度:StartAngle,起始角度除以180.0乘以PI
    // 圆弧旋转弧度:AngleRotate,旋转角度除以180.0乘以PI,正值代表逆时针,负值代表顺时针
    int vSDK_Shape_GetShapeDataByArc(void *Shape,
                                     double &CenterX,
                                     double &CenterY,
                                     double &Radius,
                                     double &StartAngle,
                                     double &AngleRotate,
                                     double &LineLength,
                                     double &LineWidth,
                                     bool &isRectangle,
                                     bool &PositiveNegative,
                                     bool &Filled);

    int vSDK_Shape_GetShapeDataByArcThreeDrop(void *Shape,
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
                                              bool &Filled);

    int vSDK_Shape_GetArcOrPointsByPolygon(
        void *Shape, void *&ArcOrPoints, int &Count, bool &PositiveNegative, bool &Filled);

    // 创建线段点集合
    int vSDK_Shape_CreatePoints(void *&Points);

    // 销毁线段点集合
    int vSDK_Shape_DestroyPoints(void *Points);

    // 加点坐标
    int vSDK_Shape_SetPointsData(void *Points, double DX, double DY);

    int vSDK_Shape_GetPointsCount(void *Points, int &PointsCount);

    int vSDK_Shape_GetPointByIndex(void *Points, const int IIndex, double &dX, double &dY);

    // 获取Shape的BOX值
    int vSDK_Shape_GetShapeBox(void *Shape, double &MinX, double &MinY, double &MaxX, double &MaxY);

    // 获取Shape的面积
    int vSDK_Shape_GetShapeArea(void *Shape, double &Area);

    // 设置层显示状态
    int vSDK_Display_SetLayer(int NLayerIdIn, bool BIsDisplayIn);

    // 获取层显示状态
    int vSDK_Display_GetLayer(int NLayerIdIn, bool *pbIsDisplayOut);

    // 刷新层显示
    int vSDK_Display_ReloadLayer(int NLayerIdIn, bool BIsDisplayIn);

    // 设置层的显示及勾选状态
    int vSDK_Display_AutoDisplayLayerByLayerName(char *playerNameIn, bool BIsShowIn);

    // 设置元件显示状态 false:不显示 true:显示
    int vSDK_Display_SetPartDisplay(bool BIsDisplayIn);

    // 获取元件显示状态 false:不显示 true:显示
    int vSDK_Display_GetPartDisplay(bool *pbIsDisplayOut);

    // 设置绘制模式
    int vSDK_Display_SetObjectMode(int NObjectModeIn);

    // 获取绘制模式
    int vSDK_Display_GetObjectMode(int *pnObjectModeIn);

    // 设置选择模式
    int vSDK_Display_SetSelectMode(int NSelectModeIn);

    // 获取选择模式
    int vSDK_Display_GetSelectMode(int *pnSelectModeIn);

    // 跳转到(板子物理坐标,非屏幕坐标)
    int vSDK_Display_Goto(double DblPointionXIn, double DblPointYIn);
};

#endif  // VSDK_H
