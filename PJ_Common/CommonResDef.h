#pragma once

//#define PJ_DEFAULT_SERVER_PORT          7001
//#define PJ_DEFAULT_SERVER_PORT_COUNT    4
#define PJ_FIND_WAY_RADIUS 50.0f


enum ContentAlignment
{
    TopLeft = 1,
    TopCenter = 2,
    TopRight = 4,
    MiddleLeft = 16,
    MiddleCenter = 32,
    MiddleRight = 64,
    BottomLeft = 256,
    BottomCenter = 512,
    BottomRight = 1024
};

mstr GetBaseGamePath();
mstr GetServerAddr();
WORD GetServerPort();
DWORD GetServerSum();
void SaveCurrentConfig(RenderManager* xRenderManager);

// 地图头
struct MapHeader_1_0
{
    Int32 Width;
    Int32 Height;
};
struct MapHeader_3_0
{
    DWORD   CellDataAddr;   // 单元起始地址
    DWORD   Width;          // 地图实际的宽度
    DWORD   Height;         // 地图实际的高度
    WORD    PointWidth;     // 坐标的宽度(默认：20px)
    WORD    PointHeight;    // 坐标的高度(默认：20px)
    WORD    SubWidth;       // 小地图的宽度(默认：400px)
    WORD    SubHeight;      // 小地图的高度(默认：320px)
    DWORD   UnitIndexOffset;// 单元引索的位置
    DWORD   UnitIndexNum;   // 单元引索的数量
    DWORD   IndexOffset;    // 引索的位置(未知部分)
    DWORD   IndexNum;       // 引索的数量(未知部分)
};

// 地图块信息
struct BlockUnitInfo { DWORD m_Flag; Int32 m_Size; };

struct BlockUnitData
{
    ~BlockUnitData() { SAFE_DELETE_ARRAY(m_Data); }

    DWORD  m_Flag = 0;
    Int32  m_Size = 0;
    LPBYTE m_Data = nullptr;
};

// 阻挡数据信息
struct BarrierDataInfo
{
    Int32 m_BarrierBytesNum;
    LPBYTE m_Data;
};

class MapUnit
{
public:
    ~MapUnit() { SAFE_DELETE_ARRAY(m_ArrayUnitAddr); }
    LPDWORD m_ArrayUnitAddr = nullptr;
    Int32 m_UnitCount = 0;
};

// 地图遮罩头
struct MaskHead
{
    DWORD m_Unknown;
    Int32 m_MaskNum;
};

// JPEG共享头
struct JPEGCommonHead
{
    DWORD m_Flag;
    Int32 m_Size;
    LPBYTE m_Data;
};

#define MAP_MAPX_HEADER_FLAG    0x4D415058
#define MAP_M1_0_HEADER_FLAG    0x4D312E30
#define MAP_M3_0_HEADER_FLAG    0x4D332E30

#define MAP_JPGH_FLAG           0x4A504748
#define MAP_GEPJ_FLAG           0x4A504547
#define MAP_CELL_FLAG           0x43454C4C
#define MAP_BLOK_FLAG           0x424C4F4B
#define MAP_MASK_FLAG           0x4D41534B
#define MAP_IMAG_FLAG           0x494D4147
#define MAP_MSK2_FLAG           0x4D534B32
#define MAP_ROL0_FLAG           0x524F4C30
#define MAP_BRIG_FLAG           0x42524947
#define MAP_LIGT_FLAG           0x4C494754
#define MAP_DNE__FLAG           0x454E4420



inline Int32 _MAP_BLOCK_WIDTH(Int32 xHeader)
{
    switch (xHeader)
    {
        case 0x4D415058:
        case 0x4D312E30: { return 320; }

        case 0x4D322E35:
        case 0x4D332E30:
        case 0x524F4C30: { return 400; }
    }
    return INVALID_NID;
}
#define MAP_BLOCK_WIDTH (_MAP_BLOCK_WIDTH(m_MapHeaderFlag))

inline Int32 _MAP_BLOCK_HEIGHT(Int32 xHeader)
{
    switch (xHeader)
    {
        case 0x4D415058:
        case 0x4D312E30: { return 240; }

        case 0x4D322E35:
        case 0x4D332E30:
        case 0x524F4C30: { return 320; }
    }
    return INVALID_NID;
}
#define MAP_BLOCK_HEIGHT (_MAP_BLOCK_HEIGHT(m_MapHeaderFlag))

//#define MAP_BLOCK_WIDTH         320
//#define MAP_BLOCK_HEIGHT        240
inline Int32 _MAP_BARRIER_LATTICE_PIXEL_WIDTH_SIZE(Int32 xHeader, MapHeader_1_0& xMapHeader_1_0, MapHeader_3_0& xMapHeader_3_0)
{
    switch (xHeader)
    {
        case 0x4D322E35:
        case 0x4D332E30:
        case 0x524F4C30: { return xMapHeader_3_0.PointWidth; }
    }
    return 20;
}
inline Int32 _MAP_BARRIER_LATTICE_PIXEL_HEIGHT_SIZE(Int32 xHeader, MapHeader_1_0& xMapHeader_1_0, MapHeader_3_0& xMapHeader_3_0)
{
    switch (xHeader)
    {
        case 0x4D322E35:
        case 0x4D332E30:
        case 0x524F4C30: { return xMapHeader_3_0.PointHeight; }
    }
    return 20;
}

#define MAP_BARRIER_LATTICE_PIXEL_WIDTH_SIZE    20
#define MAP_BARRIER_LATTICE_PIXEL_HEIGHT_SIZE   20

//#define MAP_BARRIER_LATTICE_PIXEL_WIDTH_SIZE    (_MAP_BARRIER_LATTICE_PIXEL_WIDTH_SIZE(m_MapHeaderFlag,m_MapHeader_1_0,m_MapHeader_3_0))
//#define MAP_BARRIER_LATTICE_PIXEL_HEIGHT_SIZE   (_MAP_BARRIER_LATTICE_PIXEL_HEIGHT_SIZE(m_MapHeaderFlag,m_MapHeader_1_0,m_MapHeader_3_0))

inline Int32 _MAP_BARRIER_WIDTH(Int32 xHeader, MapHeader_1_0& xMapHeader_1_0, MapHeader_3_0& xMapHeader_3_0) { return _MAP_BLOCK_WIDTH(xHeader) / _MAP_BARRIER_LATTICE_PIXEL_WIDTH_SIZE(xHeader, xMapHeader_1_0, xMapHeader_3_0); }
#define MAP_BARRIER_WIDTH (_MAP_BARRIER_WIDTH(m_MapHeaderFlag,m_MapHeader_1_0,m_MapHeader_3_0))
inline Int32 _MAP_BARRIER_HEIGHT(Int32 xHeader, MapHeader_1_0& xMapHeader_1_0, MapHeader_3_0& xMapHeader_3_0) { return _MAP_BLOCK_HEIGHT(xHeader) / _MAP_BARRIER_LATTICE_PIXEL_HEIGHT_SIZE(xHeader, xMapHeader_1_0, xMapHeader_3_0); }
#define MAP_BARRIER_HEIGHT (_MAP_BARRIER_HEIGHT(m_MapHeaderFlag,m_MapHeader_1_0,m_MapHeader_3_0))


//#define MAP_BARRIER_WIDTH         (MAP_BLOCK_WIDTH / MAP_BARRIER_LATTICE_PIXEL_SIZE)
//#define MAP_BARRIER_HEIGHT        (MAP_BLOCK_HEIGHT / MAP_BARRIER_LATTICE_PIXEL_SIZE)


#define READ_MEMORY_ASSIST(memory, type, name)  memcpy(&type, memory, sizeof(type)); memory += sizeof(type);
#define ARRAY_READ_MEMORY_ASSIST(memory, buffaddr, len, name)  memcpy(buffaddr, memory, len); memory += len;
#define ADD_MEMORY_POS(memory, pos) memory += pos;
#define SUB_MEMORY_POS(memory, pos) memory -= pos;

#define READ_FILE_ASSIST(file, type, name)  DWORD dwReadSize##type = sizeof(type); Boolean xRet##type = ::ReadFile(file, &type, dwReadSize##type, &dwReadSize##type, nullptr); Assert(AL_NORMAL, xRet##type, "读取["#name"]失败!");
#define ARRAY_READ_FILE_ASSIST(file, buffaddr, len, name)  DWORD dwReadSize##buffaddr = len; Boolean xRet##buffaddr = ::ReadFile(file, buffaddr, dwReadSize##buffaddr, &dwReadSize##buffaddr, nullptr); Assert(AL_NORMAL, xRet##buffaddr, "读取["#name"]失败!");

#define ADD_FILE_POS(file, pos) ::SetFilePointer(file, pos, 0, FILE_CURRENT);
#define SUB_FILE_POS(file, pos) ::SetFilePointer(file, -pos, 0, FILE_CURRENT);

#define AUTO_SET_POINTER_ASSIST(file, basememory, memory, pos)\
{\
    if (file != INVALID_HANDLE_VALUE)\
    {\
        ::SetFilePointer(file, pos, 0, FILE_BEGIN);\
    }\
    else if (memory)\
    {\
        memory = basememory + pos;\
    }\
    else\
    {\
        AssertLog("设定游标失败!");\
    }\
}

#define AUTO_READ_LEN_ASSIST(file, memory, addr, len, name)\
{\
    if (file != INVALID_HANDLE_VALUE)\
    {\
        ARRAY_READ_FILE_ASSIST(file, addr, len, name);\
    }\
    else if (memory)\
    {\
        ARRAY_READ_MEMORY_ASSIST(memory, addr, len, name);\
    }\
    else\
    {\
        AssertLog("读取[%s]失败!", name);\
    }\
}

#define AUTO_READ_ASSIST(file, memory, type, name)\
{\
    if (file != INVALID_HANDLE_VALUE)\
    {\
        READ_FILE_ASSIST(file, type, name);\
    }\
    else if (memory)\
    {\
        READ_MEMORY_ASSIST(memory, type, name);\
    }\
    else\
    {\
        AssertLog("读取[%s]失败!", name);\
    }\
}

//#define MAP_NEW_HEADER_FLAG     0x4D312E30
//#define MAP_OLD_HEADER_FLAG     0x4D415058
//#define MAP_OLD_JPEG_HEAD_FLAG  0x4A504748
//
//#define MAP_GEPJ_FLAG           0x4A504547
//#define MAP_CELL_FLAG           0x43454C4C

//#define MAP_BLOCK_WIDTH         320
//#define MAP_BLOCK_HEIGHT        240

//#define MAP_BARRIER_LATTICE_PIXEL_SIZE 20
//#define MAP_BARRIER_WIDTH                                   (MAP_BLOCK_WIDTH / MAP_BARRIER_LATTICE_PIXEL_SIZE)
//#define MAP_BARRIER_HEIGHT                                  (MAP_BLOCK_HEIGHT / MAP_BARRIER_LATTICE_PIXEL_SIZE)

//#define READ_FILE_ASSIST(file, type, name)                  DWORD dwReadSize##type = sizeof(type); Boolean xRet##type = ::ReadFile(file, &type, dwReadSize##type, &dwReadSize##type, nullptr); Assert(AL_NORMAL, xRet##type, "读取["#name"]失败!");
//#define ARRAY_READ_FILE_ASSIST(file, buffaddr, len, name)   DWORD dwReadSize##buffaddr = len; Boolean xRet##buffaddr = ::ReadFile(file, buffaddr, dwReadSize##buffaddr, &dwReadSize##buffaddr, nullptr); Assert(AL_NORMAL, xRet##buffaddr, "读取["#name"]失败!");

#define MAX_ROLE_LEVEL 160

#define BASE_SPEED (0.008f)     // 移动基础速度

#define DEFAULT_SPEED (1.3f)    // 默认移动速度

#define MAX_TEAM_OBJECT_COUNT   5

// 账号最小长度
#define USERNAME_MIN_LEN    4
#define USERNAME_MAX_LEN    50

// 昵称最小长度
#define NICKNAME_MIN_LEN    2
#define NICKNAME_MAX_LEN    6

// 套装商城一次销售数量
#define SUIT_MALL_GOODS_ONCE_COUNT          4

// 非打折表情商城一次销售数量
#define FACE_MALL_GOODS_ONCE_COUNT          7

// 打折表情商城一次销售数量
#define DISCOUNT_FACE_MALL_GOODS_ONCE_COUNT 3

#define PLAYER_DISPLACEMENT_KICK_STR "角色已经在其他设备登录!"


// 套装商店刷新间隔.(秒)
#define SUIT_SHOP_UPDATE_INTERVAL 60

// 表情商店刷新间隔.(秒)
#define FACE_SHOP_UPDATE_INTERVAL 60

#define SUIT_FOREVER_DURATION_DEF -1
#define FACE_FOREVER_DURATION_DEF -1

// 战斗中移动速度
#define BATTLE_SPEED 8.0f

// 技能目标类型
enum SKILL_TARGET_TYPE
{
    INVALID,        // 无效的
    ALL,            // 全部
    ENEMY,          // 敌人
    FRIENDLY,       // 队友
};

enum E_COLLISION_RELATION
{
    IRRELEVANT,
    COLLISION,
    NO_COLLISION
};

//enum E_OBJECT_TYPE
//{
//    EOT_MONSTER,
//    EOT_NPC,
//    EOT_PLAYER,
//    EOT_PET,
//};

//enum E_SKILL_TYPE
//{
//    EST_无效,
//
//    EST_作茧自缚,
//    EST_金蛇缠丝,
//    EST_天罗地网,
//    EST_作壁上观,
//    EST_四面楚歌,
//
//    EST_催眠咒,
//    EST_瞌睡咒,
//    EST_离魂咒,
//    EST_迷魂醉,
//    EST_百日眠,
//
//    EST_蛇蝎美人,
//    EST_追魂迷香,
//    EST_断肠烈散,
//    EST_鹤顶红粉,
//    EST_万毒攻心,
//
//    EST_反间之计,
//    EST_情真意切,
//    EST_谗言相加,
//    EST_借刀杀人,
//    EST_失心狂乱,
//
//    EST_夺命勾魂,
//    EST_追神摄魄,
//    EST_魔音摄心,
//    EST_销魂蚀骨,
//    EST_阎罗追命,
//
//    EST_妖之魔力,
//    EST_力神复苏,
//    EST_狮王之怒,
//    EST_兽王神力,
//    EST_魔神附身,
//
//    EST_红袖添香,
//    EST_莲步轻舞,
//    EST_楚楚可怜,
//    EST_魔神护体,
//    EST_含情脉脉,
//
//    EST_魔之飞步,
//    EST_急速之魔,
//    EST_魔神飞舞,
//    EST_天外飞魔,
//    EST_乾坤借速,
//
//    EST_雷霆霹雳,
//    EST_日照光华,
//    EST_雷神怒击,
//    EST_电闪雷鸣,
//    EST_天诛地灭,
//
//    EST_龙卷雨击,
//    EST_龙腾水溅,
//    EST_龙啸九天,
//    EST_蛟龙出海,
//    EST_九龙冰封,
//
//    EST_地狱烈火,
//    EST_天雷怒火,
//    EST_三味真火,
//    EST_烈火骄阳,
//    EST_九阴纯火,
//
//    EST_飞砂走石,
//    EST_乘风破浪,
//    EST_太乙生风,
//    EST_风雷涌动,
//    EST_袖里乾坤,
//};
//
//inline E_SKILL_TYPE GetSkillTypeByStr(LPCSTR xSkillName)
//{
//    if (strcmp(xSkillName, "作茧自缚") == 0) { return EST_作茧自缚; }
//    else if (strcmp(xSkillName, "金蛇缠丝") == 0) { return EST_金蛇缠丝; }
//    else if (strcmp(xSkillName, "天罗地网") == 0) { return EST_天罗地网; }
//    else if (strcmp(xSkillName, "作壁上观") == 0) { return EST_作壁上观; }
//    else if (strcmp(xSkillName, "四面楚歌") == 0) { return EST_四面楚歌; }
//
//    else if (strcmp(xSkillName, "催眠咒") == 0) { return EST_催眠咒; }
//    else if (strcmp(xSkillName, "瞌睡咒") == 0) { return EST_瞌睡咒; }
//    else if (strcmp(xSkillName, "离魂咒") == 0) { return EST_离魂咒; }
//    else if (strcmp(xSkillName, "迷魂醉") == 0) { return EST_迷魂醉; }
//    else if (strcmp(xSkillName, "百日眠") == 0) { return EST_百日眠; }
//
//    else if (strcmp(xSkillName, "蛇蝎美人") == 0) { return EST_蛇蝎美人; }
//    else if (strcmp(xSkillName, "追魂迷香") == 0) { return EST_追魂迷香; }
//    else if (strcmp(xSkillName, "断肠烈散") == 0) { return EST_断肠烈散; }
//    else if (strcmp(xSkillName, "鹤顶红粉") == 0) { return EST_鹤顶红粉; }
//    else if (strcmp(xSkillName, "万毒攻心") == 0) { return EST_万毒攻心; }
//
//    else if (strcmp(xSkillName, "反间之计") == 0) { return EST_反间之计; }
//    else if (strcmp(xSkillName, "情真意切") == 0) { return EST_情真意切; }
//    else if (strcmp(xSkillName, "谗言相加") == 0) { return EST_谗言相加; }
//    else if (strcmp(xSkillName, "借刀杀人") == 0) { return EST_借刀杀人; }
//    else if (strcmp(xSkillName, "失心狂乱") == 0) { return EST_失心狂乱; }
//
//    else if (strcmp(xSkillName, "夺命勾魂") == 0) { return EST_夺命勾魂; }
//    else if (strcmp(xSkillName, "追神摄魄") == 0) { return EST_追神摄魄; }
//    else if (strcmp(xSkillName, "魔音摄心") == 0) { return EST_魔音摄心; }
//    else if (strcmp(xSkillName, "销魂蚀骨") == 0) { return EST_销魂蚀骨; }
//    else if (strcmp(xSkillName, "阎罗追命") == 0) { return EST_阎罗追命; }
//
//    else if (strcmp(xSkillName, "妖之魔力") == 0) { return EST_妖之魔力; }
//    else if (strcmp(xSkillName, "力神复苏") == 0) { return EST_力神复苏; }
//    else if (strcmp(xSkillName, "狮王之怒") == 0) { return EST_狮王之怒; }
//    else if (strcmp(xSkillName, "兽王神力") == 0) { return EST_兽王神力; }
//    else if (strcmp(xSkillName, "魔神附身") == 0) { return EST_魔神附身; }
//
//    else if (strcmp(xSkillName, "红袖添香") == 0) { return EST_红袖添香; }
//    else if (strcmp(xSkillName, "莲步轻舞") == 0) { return EST_莲步轻舞; }
//    else if (strcmp(xSkillName, "楚楚可怜") == 0) { return EST_楚楚可怜; }
//    else if (strcmp(xSkillName, "魔神护体") == 0) { return EST_魔神护体; }
//    else if (strcmp(xSkillName, "含情脉脉") == 0) { return EST_含情脉脉; }
//
//    else if (strcmp(xSkillName, "魔之飞步") == 0) { return EST_魔之飞步; }
//    else if (strcmp(xSkillName, "急速之魔") == 0) { return EST_急速之魔; }
//    else if (strcmp(xSkillName, "魔神飞舞") == 0) { return EST_魔神飞舞; }
//    else if (strcmp(xSkillName, "天外飞魔") == 0) { return EST_天外飞魔; }
//    else if (strcmp(xSkillName, "乾坤借速") == 0) { return EST_乾坤借速; }
//
//    else if (strcmp(xSkillName, "雷霆霹雳") == 0) { return EST_雷霆霹雳; }
//    else if (strcmp(xSkillName, "日照光华") == 0) { return EST_日照光华; }
//    else if (strcmp(xSkillName, "雷神怒击") == 0) { return EST_雷神怒击; }
//    else if (strcmp(xSkillName, "电闪雷鸣") == 0) { return EST_电闪雷鸣; }
//    else if (strcmp(xSkillName, "天诛地灭") == 0) { return EST_天诛地灭; }
//
//    else if (strcmp(xSkillName, "龙卷雨击") == 0) { return EST_龙卷雨击; }
//    else if (strcmp(xSkillName, "龙腾水溅") == 0) { return EST_龙腾水溅; }
//    else if (strcmp(xSkillName, "龙啸九天") == 0) { return EST_龙啸九天; }
//    else if (strcmp(xSkillName, "蛟龙出海") == 0) { return EST_蛟龙出海; }
//    else if (strcmp(xSkillName, "九龙冰封") == 0) { return EST_九龙冰封; }
//
//    else if (strcmp(xSkillName, "地狱烈火") == 0) { return EST_地狱烈火; }
//    else if (strcmp(xSkillName, "天雷怒火") == 0) { return EST_天雷怒火; }
//    else if (strcmp(xSkillName, "三味真火") == 0) { return EST_三味真火; }
//    else if (strcmp(xSkillName, "烈火骄阳") == 0) { return EST_烈火骄阳; }
//    else if (strcmp(xSkillName, "九阴纯火") == 0) { return EST_九阴纯火; }
//
//    else if (strcmp(xSkillName, "飞砂走石") == 0) { return EST_飞砂走石; }
//    else if (strcmp(xSkillName, "乘风破浪") == 0) { return EST_乘风破浪; }
//    else if (strcmp(xSkillName, "太乙生风") == 0) { return EST_太乙生风; }
//    else if (strcmp(xSkillName, "风雷涌动") == 0) { return EST_风雷涌动; }
//    else if (strcmp(xSkillName, "袖里乾坤") == 0) { return EST_袖里乾坤; }
//
//    return EST_无效;
//}


enum SKILL_TYPE
{
    SGT_无效,

    SGT_混乱,
    SGT_封印,
    SGT_昏睡,
    SGT_中毒,

    SGT_提速,
    SGT_加牛,
    SGT_震摄,
    SGT_伤盘,

    SGT_风法,
    SGT_水法,
    SGT_雷法,
    SGT_火法,

    SGT_降速,
    SGT_鬼火,
    SGT_治疗,
    SGT_控盘,
};


enum SKILL_RACIAL_GROUP_TYPE
{
    SRGT_无效,
    SRGT_人,
    SRGT_魔,
    SRGT_仙,
    SRGT_鬼,
};

enum BUFF_TYPE
{
    BT_INVALID,
    BT_CONTROL,     // 控制
    BT_GAIN,        // 增益
    BT_DEBUFFS      // 减益
};

enum BUFF_ID
{
    BI_ICE,         // 封印
    BI_POISON,      // 中毒
    BI_MIX,         // 混乱
    BI_SLEEP,       // 昏睡

    BI_DISC,        // 盘丝
    BI_CATTLE,      // 加牛
    BI_FIRE,        // 火种
    BI_WATER,       // 湿水
    BI_SPEEDUP,     // 提速
    BI_SPEEDDOWN,   // 降速
};

//typedef role_info       role_info;
//typedef role_info*   role_info*;


enum E_GAME_CONTROL_TYPE
{
    EGCT_无效 = 0,
    图片,
    按钮,
    选择框,
    标题框,
    文本框,
    密码框,
    横向滚动,
    纵向滚动,
    总数,
};

enum E_GAME_OBJECT_ACTION
{
    EGOA_无效 = 0,

    EGOA_待机,
    EGOA_休闲,
    EGOA_行走,
    EGOA_跑步,

    EGOA_战待,
    EGOA_攻击,
    EGOA_受击,
    EGOA_防御,
    EGOA_死亡,

    EGOA_施法,

    EGOA_未知,
    EGOA_最大,
};

// 枚举顺序关系到存储读取顺序,不可修改
enum E_GAME_OBJECT_DIRECTION
{
    EGOD_无效 = 0,
    右下,
    左下,
    左上,
    右上,
    下,
    左,
    上,
    右,
    无,
    方向总数 = 无 - 1,
};

struct TEAM_POS_INFO
{
    Int32 m_TeamPos;
    Boolean m_TeamMaster;
};

class PJBase
{
public:

    static void GetTeamPosInfoByTeamPosIndex(Int32& xTeamPos, Boolean& xTeamMaster, Int32 xTeamPosIndex)
    {
        if (xTeamPosIndex >= 5)
        {
            xTeamMaster = True;
            xTeamPos = xTeamPosIndex - 4;
        }
        else
        {
            xTeamMaster = False;
            xTeamPos = xTeamPosIndex + 1;
        }
    }

    static Int32 GetTeamPosIndexByTeamPosInfo(Int32 xTeamPos, Boolean xTeamMaster)
    {
        if (xTeamMaster)
        {
            return xTeamPos + 4;
        }
        else
        {
            return xTeamPos - 1;
        }
    }

    static E_GOODS_TYPE GetUnitTypeById(DWORD dwId)
    {
        if (dwId >= E_ITEM_DEF::ITEM_MIN_ID && dwId <= E_ITEM_DEF::ITEM_MAX_ID)
        {
            return EGT_ITEM;
        }

        if (dwId >= E_ITEM_DEF::EQUIP_MIN_ID && dwId <= E_ITEM_DEF::EQUIP_MAX_ID)
        {
            return EGT_EQUIP;
        }
        return EGT_INVALID;
    }
};

class Position
{
public:
    Position() { X = 0.f; Y = 0.f; }
    bool operator==(Position xPosition) { return X == xPosition.X && Y == xPosition.Y; }
    bool operator!=(Position xPosition) { return X != xPosition.X || Y != xPosition.Y; }
    Position(Single _X, Single _Y) :X(_X), Y(_Y) {}
    Single X;
    Single Y;

public:
    DINLINE static E_GAME_OBJECT_DIRECTION GetDirectionByPoint(Position& xBegin, Position& xEnd, Boolean IsAccurate)
    {
        //if (Position::SamePosition(xBegin, xEnd, 1.0f))
        //{
        //    IsAccurate = True;
        //}
        //Single xDistance = Position::GetDistance(xBegin, xEnd);

        //if (abs(xDistance) < 8.0f) { IsAccurate = True; }

        Position xDir;
        xDir.X = xEnd.X - xBegin.X;
        xDir.Y = xEnd.Y - xBegin.Y;

        E_GAME_OBJECT_DIRECTION xDirX = E_GAME_OBJECT_DIRECTION::无;
        if (xDir.X < 0)
        {
            xDirX = E_GAME_OBJECT_DIRECTION::左;
        }
        else if (xDir.X > 0)
        {
            xDirX = E_GAME_OBJECT_DIRECTION::右;
        }

        E_GAME_OBJECT_DIRECTION xDirY = E_GAME_OBJECT_DIRECTION::无;
        if (xDir.Y < 0)
        {
            xDirY = E_GAME_OBJECT_DIRECTION::上;
        }
        else if (xDir.Y > 0)
        {
            xDirY = E_GAME_OBJECT_DIRECTION::下;
        }

        if (!IsAccurate)
        {
            if (abs(xDir.Y) < 0.1f)
            {
                xDir.Y = 0.0f;
            }
            if (abs(xDir.X) < 0.1f)
            {
                xDir.X = 0.0f;
            }

            if (abs(xDir.Y) < 5.0f && abs(xDir.X) > (abs(xDir.Y) * 3.0f))
            {
                xDir.Y = 0.0f;
            }

            if (abs(xDir.X) < 5.0f && abs(xDir.Y) > (abs(xDir.X) * 3.0f))
            {
                xDir.X = 0.0f;
            }

            if (xDir.Y == 0.0f && xDir.X != 0.0f)
            {
                return xDirX;
            }
            else if (xDir.Y != 0.0f && xDir.X == 0.0f)
            {
                return xDirY;
            }

            //if (abs(xDir.X) < 3 && abs(abs(xDir.X) - abs(xDir.Y)) > 2)
            //{
            //    return xDirY;
            //}

            //if (abs(xDir.Y) < 3 && abs(abs(xDir.X) - abs(xDir.Y)) > 2)
            //{
            //    return xDirX;
            //}
        }

        if (xDirX == E_GAME_OBJECT_DIRECTION::无)
        {
            return xDirY;
        }
        else if (xDirY == E_GAME_OBJECT_DIRECTION::无)
        {
            return xDirX;
        }
        else if (xDirX == E_GAME_OBJECT_DIRECTION::左)
        {
            if (xDirY == E_GAME_OBJECT_DIRECTION::上)
            {
                return E_GAME_OBJECT_DIRECTION::左上;
            }
            else
            {
                return E_GAME_OBJECT_DIRECTION::左下;
            }
        }
        else if (xDirX == E_GAME_OBJECT_DIRECTION::右)
        {
            if (xDirY == E_GAME_OBJECT_DIRECTION::上)
            {
                return E_GAME_OBJECT_DIRECTION::右上;
            }
            else
            {
                return E_GAME_OBJECT_DIRECTION::右下;
            }
        }
        else
        {
            return E_GAME_OBJECT_DIRECTION::无;
        }
    }

    DINLINE static E_GAME_OBJECT_DIRECTION GetDirByPos(const Position xPos)
    {
        if (xPos.X > 0)
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::右上;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::右下;
            }
            return E_GAME_OBJECT_DIRECTION::右;
        }
        else if (xPos.X < 0)
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::左上;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::左下;
            }
            return E_GAME_OBJECT_DIRECTION::左;
        }
        else
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::上;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::下;
            }
            return E_GAME_OBJECT_DIRECTION::无;
        }
    }

    DINLINE static void TranslateVector(Position& vector, Position& change)
    {
        vector.X += change.X;
        vector.Y += change.Y;
    }

    DINLINE static Single SqrMagnitudeVector(Position& vector)
    {
        return vector.X * vector.X + vector.Y * vector.Y;
    }

    DINLINE static Position NormalizeVector(Position& vector)
    {
        Position xNormalized(vector.X, vector.Y);

        Single xSqeMagnitude = Position::SqrMagnitudeVector(xNormalized);
        if (xSqeMagnitude > 0.0f)
        {
            Single inv_sqr = 1.0f / sqrt(xSqeMagnitude);
            xNormalized.X *= inv_sqr;
            xNormalized.Y *= inv_sqr;
        }
        else
        {
            xNormalized.X = 0.0f;
            xNormalized.Y = 0.0f;
        }
        return xNormalized;
    }

    // pos1到pos2的方向向量
    DINLINE static Position GetDir(Position& src, Position& des)
    {
        return Position(des.X - src.X, des.Y - src.Y);
    }

    // pos1到pos2的距离是否小于xFollowKeepDistance
    DINLINE static Boolean SamePosition(Position& pos1, Position& pos2, Single xFollowKeepDistance)
    {
        Single xCurrentDistance = GetDistance(pos1, pos2);
        return xCurrentDistance <= xFollowKeepDistance;
    }

    // pos1到pos2的距离
    DINLINE static Single GetDistance(Position pos1, Position pos2)
    {
        return sqrt((pos1.X - pos2.X) * (pos1.X - pos2.X) + (pos1.Y - pos2.Y) * (pos1.Y - pos2.Y));
    }

    // pos1向pos2移动最终距离还剩xResidualDistance后应该在的pos位置
    DINLINE static Position GetResidualPosition(Position pos1, Position pos2, Single xResidualDistance)
    {
        Single xCurrentDistance = GetDistance(pos1, pos2);
        return GetPursuitPosition(pos1, pos2, xCurrentDistance - xResidualDistance);
    }

    // pos1向pos2点移动xPursuitDistance距离后的位置Pos
    DINLINE static Position GetPursuitPosition(Position pos1, Position pos2, Single xPursuitDistance)
    {
        if (GetDistance(pos1, pos2) < xPursuitDistance)
        {
            return pos2;
        }

        D3DXVECTOR3 xVector1(pos1.X, pos1.Y, 0.0f);
        D3DXVECTOR3 xVector2(pos2.X, pos2.Y, 0.0f);

        D3DXVECTOR3 xTempVector1;
        D3DXVec3Subtract(&xTempVector1, &xVector2, &xVector1);

        D3DXVECTOR3 xTempVector2;
        D3DXVec3Normalize(&xTempVector2, &xTempVector1);

        D3DXVECTOR3 xTempVector3;
        D3DXVec3Scale(&xTempVector3, &xTempVector2, xPursuitDistance);

        D3DXVECTOR3 xTempVector4;
        D3DXVec3Add(&xTempVector4, &xVector1, &xTempVector3);

        return Position(xTempVector4.x, xTempVector4.y);
    }

    DINLINE static Boolean MoveTo(Position& src, Position& des, Int64 xTimeElapse, Single xSpeed, Single xFollowKeepDistance)
    {
        if (Position::SamePosition(src, des, xFollowKeepDistance))
        {
            return True;
        }

        Position xDir = Position::GetDir(src, des);
        Position xNormalized = Position::NormalizeVector(xDir);

        xNormalized.X = xNormalized.X * xTimeElapse * xSpeed * BASE_SPEED;
        xNormalized.Y = xNormalized.Y * xTimeElapse * xSpeed * BASE_SPEED;

        if (Position::SqrMagnitudeVector(xNormalized) > Position::SqrMagnitudeVector(xDir))
        {
            Position::TranslateVector(src, xDir);
        }
        else
        {
            Position::TranslateVector(src, xNormalized);
        }

        return False;
    }
};

CONST Position GC_BATTLETEAM_1_POS_MASTER[] = { { 11, 16 }, { 15, 12 }, {  7, 20 }, { 19,  8 }, {  3, 24 }, { 4, 13 } };
CONST Position GC_BATTLETEAM_1_POS_SUMMON[] = { { 15, 20 }, { 19, 16 }, { 11, 24 }, { 23, 12 }, {  7, 28 }, { 8,  9 } };

CONST Position GC_BATTLETEAM_2_POS_MASTER[] = { { 39, 28 }, { 35, 32 }, { 43, 24 }, { 47, 20 }, { 31, 36 }, { 42, 35 } };
CONST Position GC_BATTLETEAM_2_POS_SUMMON[] = { { 35, 24 }, { 31, 28 }, { 39, 20 }, { 43, 16 }, { 27, 32 }, { 46, 31 } };

#pragma pack(push) 
#pragma pack(1)
struct UnitFileInfo
{
    DWORD m_Key;
    DWORD m_Addr;
    DWORD m_Len;
    DWORD m_UnKnow;
};
struct WdfUnit
{
    WdfUnit() { ; }
    WdfUnit(UnitFileInfo& xUnitFileInfo) : m_UnitFileInfo(xUnitFileInfo), m_Data(nullptr) { ; }
    UnitFileInfo m_UnitFileInfo;
    LPBYTE m_Data;
};
#pragma pack(pop)

enum E_UNIT_TYPE
{
    EUT_UNKNOW,
    EUT_WAS,
    EUT_BMP,
    EUT_RIFF,
    EUT_TGA,
    EUT_MP3,
    EUT_WAV,
};

#pragma pack(push) 
#pragma pack(1)
struct GameObjectActionInfo
{
    Int32 m_ActionEnum;
    DWORD m_Key;
};

enum E_POS_TYPE
{
    EPT_NULL,
    EPT_CENTER,
    EPT_LEFTTOP,
    EPT_LEFTBOTTOM,
    EPT_RIGHTTOP,
    EPT_RIGHTBOTTOM,
};

// 切磋确认等待时间
#define QIECUO_CONFIRM_WAIT_TIME (5 * 1000 * 10000)

struct QiecuoConfirmInfo
{
    Int64 src_role_index;
    Int64 tar_role_index;
    Int64 end_time;
};

struct QiecuoConfirmState
{
    Boolean Contains(Int64 src_role_index)
    {
        return m_Dict_Qiecuo_Ask.ContainsKey(src_role_index);
    }

    void AddState(Int64 src_role_index, Int64 tar_role_index, Int64 ask_time)
    {
        m_Dict_Qiecuo_Ask.Add(src_role_index, { src_role_index,  tar_role_index,  ask_time });
    }

    void RemoveState(Int64 src_role_index)
    {
        m_Dict_Qiecuo_Ask.Remove(src_role_index);
    }
    MsUnorderedDictionary<Int64, QiecuoConfirmInfo> m_Dict_Qiecuo_Ask;
};

struct GameBaseFormInfo
{
    mstr        m_szName;
    mstr        m_ResFile;
    DWORD       m_ResKey;
    DWORD       m_ZLevel;
    Int32       m_Width;
    Int32       m_Height;
    Boolean     m_CanMove;
    Boolean     m_CanOutofBounds;
    Boolean     m_IsFastClose;
    E_POS_TYPE  m_PosType;
    //Int32       m_ControlCount;
};
#pragma pack(pop)


struct account_save_info
{
    Int64   m_GuidKey;
    mstr    m_Username;
    mstr    m_Password;
};

//enum E_CLUSTER_OBJ_TYPE
//{
//    //ECOT_WILD_SCENE,        // 野外场景
//    //ECOT_CITY_SCENE,        // 城市场景
//    //ECOT_BATTLE_SCENE,      // 战斗场景
//    //ECOT_LEADERBOARD_SCENE, // 排行榜场景
//
//    //ECOT_ALL_SCENE,         // 所有场景
//
//
//
//
//    //ECOT_LOGIN_PLAYER,          // 网关玩家
//    //ECOT_SCENE_PLAYER,          // 场景玩家
//
//
//    //ECOT_FRIENDS_MANGER,        // 好友管理器
//
//};

enum CALL_BACK_TYPE
{
    // 玩家
    //WEB_DB_CB_ACCOUNT_REGISTER = 0x20000000,// 注册账号
    //WEB_DB_CB_ACCOUNT_CHECK,                // 检查账号
    //WEB_DB_CB_ACCOUNT_LOGIN,                // 登录账号

    //WEB_DB_CB_ROLE_SELECT_COUNT,    // 角色数量
    //WEB_DB_CB_CLEAR_ROLE,           // 清空角色
    //WEB_DB_CB_ROLE_CREATE,          // 创建角色
    //WEB_DB_CB_ROLE_SELECT_INFO,     // 查询信息

    //WEB_CB_ROLE_LEAVE_LOGIN,        // 角色离开登录
    //WEB_CB_ROLE_ENTER_SCENE,        // 角色进入场景

    // TIME
    TIMER_CB_SERVER_TICK = 0x30000000,// 服务1器帧
    //TIMER_CB_SERVER_TICK_,// 服务1器帧

    //TIMER_CB_WAIT_OPERATION_SURPLUS_SECOND, // 操作者剩余操作时间

    //TIMER_CB_WAIT_ACTIONDONE_SURPLUS_SECOND,// 客户端播放动画剩余时间


    TIMER_CB_SCENEPLAYER_TEST,

    CUSTOM_CB_SCENE_SAVE_PLAYER,

    // 关闭登录管理器
    LOGIN_CB_CLOSE_ALL_CLIENT = 0x40000000,

    SCENE_CB_CLOSE,

    // 好友系统
    CUSTOM_CB_FRIEND_MANAGER_CLOSE = 0x60000000,

    // 定时, 处理请求好友确认
    TIMER_CB_REQUEST_FRIEND_PRESCRIPTION_CHECK,

    // 定时, 刷新推荐好友分段信息
    TIMER_CB_UPDATE_FRIEND_FRAGMENT_INFO,


    // 城市场景中大炮广播队列
    TIMER_CB_CITY_SCENE_NPC_ARTILLERY_BROADCAST_RECORD_QUEUE,

    // 澡堂恢复体力广播队列
    TIMER_CB_BATHHOURSE_RECOVER_STAMINA_BROADCAST,
};
//
//class PJTeamInfo
//{
//public:
//    Int32 FindInTeamPos(LPVOID lpPlayer)
//    {
//        if (lpPlayer > 0)
//        {
//            for (Int32 i = 0; i < MAX_TEAM_OBJECT_COUNT; i++)
//            {
//                if (m_PlayerInPos[i] == lpPlayer) { return i + 1; }
//            }
//        }
//        return INVALID_NID;
//    }
//
//    Boolean LeaveTeam(LPVOID lpPlayer) { return (lpPlayer, FindInTeamPos(lpPlayer)); }
//    Boolean LeaveTeam(LPVOID lpPlayer, Int32 xPos)
//    {
//        if (lpPlayer > 0 && xPos > 0)
//        {
//            switch (xPos)
//            {
//                case 1: { if (lpPlayer == m_PlayerInPos[0]) { m_PlayerInPos[0] = nullptr; return True; } }break;
//                case 2: { if (lpPlayer == m_PlayerInPos[1]) { m_PlayerInPos[1] = nullptr; return True; } }break;
//                case 3: { if (lpPlayer == m_PlayerInPos[2]) { m_PlayerInPos[2] = nullptr; return True; } }break;
//                case 4: { if (lpPlayer == m_PlayerInPos[3]) { m_PlayerInPos[3] = nullptr; return True; } }break;
//                case 5: { if (lpPlayer == m_PlayerInPos[4]) { m_PlayerInPos[4] = nullptr; return True; } }break;
//            }
//        }
//        return False;
//    }
//
//    void ClearApplyList()
//    {
//        m_ListApply.Clear();
//    }
//
//    Boolean CheckInApplyList(LPVOID lpPlayer)
//    {
//        return (m_ListApply.IndexOf(lpPlayer) == INVALID_NID);
//    }
//
//    Boolean EnterApplyList(LPVOID lpPlayer)
//    {
//        if (!CheckInApplyList(lpPlayer))
//        {
//            m_ListApply.Add(lpPlayer);
//            return True;
//        }
//        return False;
//    }
//
//    void CreateTeamInfo(LPVOID lpPlayer)
//    {
//        m_ListApply.Clear();
//        ZeroMemoryArray(m_PlayerInPos);
//    }
//
//    MsList<LPVOID> m_ListApply;
//    LPVOID m_PlayerInPos[MAX_TEAM_OBJECT_COUNT];
//};
//
