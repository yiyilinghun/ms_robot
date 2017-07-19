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

// ��ͼͷ
struct MapHeader_1_0
{
    Int32 Width;
    Int32 Height;
};
struct MapHeader_3_0
{
    DWORD   CellDataAddr;   // ��Ԫ��ʼ��ַ
    DWORD   Width;          // ��ͼʵ�ʵĿ��
    DWORD   Height;         // ��ͼʵ�ʵĸ߶�
    WORD    PointWidth;     // ����Ŀ��(Ĭ�ϣ�20px)
    WORD    PointHeight;    // ����ĸ߶�(Ĭ�ϣ�20px)
    WORD    SubWidth;       // С��ͼ�Ŀ��(Ĭ�ϣ�400px)
    WORD    SubHeight;      // С��ͼ�ĸ߶�(Ĭ�ϣ�320px)
    DWORD   UnitIndexOffset;// ��Ԫ������λ��
    DWORD   UnitIndexNum;   // ��Ԫ����������
    DWORD   IndexOffset;    // ������λ��(δ֪����)
    DWORD   IndexNum;       // ����������(δ֪����)
};

// ��ͼ����Ϣ
struct BlockUnitInfo { DWORD m_Flag; Int32 m_Size; };

struct BlockUnitData
{
    ~BlockUnitData() { SAFE_DELETE_ARRAY(m_Data); }

    DWORD  m_Flag = 0;
    Int32  m_Size = 0;
    LPBYTE m_Data = nullptr;
};

// �赲������Ϣ
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

// ��ͼ����ͷ
struct MaskHead
{
    DWORD m_Unknown;
    Int32 m_MaskNum;
};

// JPEG����ͷ
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

#define READ_FILE_ASSIST(file, type, name)  DWORD dwReadSize##type = sizeof(type); Boolean xRet##type = ::ReadFile(file, &type, dwReadSize##type, &dwReadSize##type, nullptr); Assert(AL_NORMAL, xRet##type, "��ȡ["#name"]ʧ��!");
#define ARRAY_READ_FILE_ASSIST(file, buffaddr, len, name)  DWORD dwReadSize##buffaddr = len; Boolean xRet##buffaddr = ::ReadFile(file, buffaddr, dwReadSize##buffaddr, &dwReadSize##buffaddr, nullptr); Assert(AL_NORMAL, xRet##buffaddr, "��ȡ["#name"]ʧ��!");

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
        AssertLog("�趨�α�ʧ��!");\
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
        AssertLog("��ȡ[%s]ʧ��!", name);\
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
        AssertLog("��ȡ[%s]ʧ��!", name);\
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

//#define READ_FILE_ASSIST(file, type, name)                  DWORD dwReadSize##type = sizeof(type); Boolean xRet##type = ::ReadFile(file, &type, dwReadSize##type, &dwReadSize##type, nullptr); Assert(AL_NORMAL, xRet##type, "��ȡ["#name"]ʧ��!");
//#define ARRAY_READ_FILE_ASSIST(file, buffaddr, len, name)   DWORD dwReadSize##buffaddr = len; Boolean xRet##buffaddr = ::ReadFile(file, buffaddr, dwReadSize##buffaddr, &dwReadSize##buffaddr, nullptr); Assert(AL_NORMAL, xRet##buffaddr, "��ȡ["#name"]ʧ��!");

#define MAX_ROLE_LEVEL 160

#define BASE_SPEED (0.008f)     // �ƶ������ٶ�

#define DEFAULT_SPEED (1.3f)    // Ĭ���ƶ��ٶ�

#define MAX_TEAM_OBJECT_COUNT   5

// �˺���С����
#define USERNAME_MIN_LEN    4
#define USERNAME_MAX_LEN    50

// �ǳ���С����
#define NICKNAME_MIN_LEN    2
#define NICKNAME_MAX_LEN    6

// ��װ�̳�һ����������
#define SUIT_MALL_GOODS_ONCE_COUNT          4

// �Ǵ��۱����̳�һ����������
#define FACE_MALL_GOODS_ONCE_COUNT          7

// ���۱����̳�һ����������
#define DISCOUNT_FACE_MALL_GOODS_ONCE_COUNT 3

#define PLAYER_DISPLACEMENT_KICK_STR "��ɫ�Ѿ��������豸��¼!"


// ��װ�̵�ˢ�¼��.(��)
#define SUIT_SHOP_UPDATE_INTERVAL 60

// �����̵�ˢ�¼��.(��)
#define FACE_SHOP_UPDATE_INTERVAL 60

#define SUIT_FOREVER_DURATION_DEF -1
#define FACE_FOREVER_DURATION_DEF -1

// ս�����ƶ��ٶ�
#define BATTLE_SPEED 8.0f

// ����Ŀ������
enum SKILL_TARGET_TYPE
{
    INVALID,        // ��Ч��
    ALL,            // ȫ��
    ENEMY,          // ����
    FRIENDLY,       // ����
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
//    EST_��Ч,
//
//    EST_�����Ը�,
//    EST_���߲�˿,
//    EST_���޵���,
//    EST_�����Ϲ�,
//    EST_�������,
//
//    EST_������,
//    EST_�˯��,
//    EST_�����,
//    EST_�Ի���,
//    EST_������,
//
//    EST_��Ы����,
//    EST_׷������,
//    EST_�ϳ���ɢ,
//    EST_�׶����,
//    EST_�򶾹���,
//
//    EST_����֮��,
//    EST_��������,
//    EST_�������,
//    EST_�赶ɱ��,
//    EST_ʧ�Ŀ���,
//
//    EST_��������,
//    EST_׷������,
//    EST_ħ������,
//    EST_����ʴ��,
//    EST_����׷��,
//
//    EST_��֮ħ��,
//    EST_������,
//    EST_ʨ��֮ŭ,
//    EST_��������,
//    EST_ħ����,
//
//    EST_��������,
//    EST_��������,
//    EST_��������,
//    EST_ħ����,
//    EST_��������,
//
//    EST_ħ֮�ɲ�,
//    EST_����֮ħ,
//    EST_ħ�����,
//    EST_�����ħ,
//    EST_Ǭ������,
//
//    EST_��������,
//    EST_���չ⻪,
//    EST_����ŭ��,
//    EST_��������,
//    EST_�������,
//
//    EST_�������,
//    EST_����ˮ��,
//    EST_��Х����,
//    EST_��������,
//    EST_��������,
//
//    EST_�����һ�,
//    EST_����ŭ��,
//    EST_��ζ���,
//    EST_�һ���,
//    EST_��������,
//
//    EST_��ɰ��ʯ,
//    EST_�˷�����,
//    EST_̫������,
//    EST_����ӿ��,
//    EST_����Ǭ��,
//};
//
//inline E_SKILL_TYPE GetSkillTypeByStr(LPCSTR xSkillName)
//{
//    if (strcmp(xSkillName, "�����Ը�") == 0) { return EST_�����Ը�; }
//    else if (strcmp(xSkillName, "���߲�˿") == 0) { return EST_���߲�˿; }
//    else if (strcmp(xSkillName, "���޵���") == 0) { return EST_���޵���; }
//    else if (strcmp(xSkillName, "�����Ϲ�") == 0) { return EST_�����Ϲ�; }
//    else if (strcmp(xSkillName, "�������") == 0) { return EST_�������; }
//
//    else if (strcmp(xSkillName, "������") == 0) { return EST_������; }
//    else if (strcmp(xSkillName, "�˯��") == 0) { return EST_�˯��; }
//    else if (strcmp(xSkillName, "�����") == 0) { return EST_�����; }
//    else if (strcmp(xSkillName, "�Ի���") == 0) { return EST_�Ի���; }
//    else if (strcmp(xSkillName, "������") == 0) { return EST_������; }
//
//    else if (strcmp(xSkillName, "��Ы����") == 0) { return EST_��Ы����; }
//    else if (strcmp(xSkillName, "׷������") == 0) { return EST_׷������; }
//    else if (strcmp(xSkillName, "�ϳ���ɢ") == 0) { return EST_�ϳ���ɢ; }
//    else if (strcmp(xSkillName, "�׶����") == 0) { return EST_�׶����; }
//    else if (strcmp(xSkillName, "�򶾹���") == 0) { return EST_�򶾹���; }
//
//    else if (strcmp(xSkillName, "����֮��") == 0) { return EST_����֮��; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "�������") == 0) { return EST_�������; }
//    else if (strcmp(xSkillName, "�赶ɱ��") == 0) { return EST_�赶ɱ��; }
//    else if (strcmp(xSkillName, "ʧ�Ŀ���") == 0) { return EST_ʧ�Ŀ���; }
//
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "׷������") == 0) { return EST_׷������; }
//    else if (strcmp(xSkillName, "ħ������") == 0) { return EST_ħ������; }
//    else if (strcmp(xSkillName, "����ʴ��") == 0) { return EST_����ʴ��; }
//    else if (strcmp(xSkillName, "����׷��") == 0) { return EST_����׷��; }
//
//    else if (strcmp(xSkillName, "��֮ħ��") == 0) { return EST_��֮ħ��; }
//    else if (strcmp(xSkillName, "������") == 0) { return EST_������; }
//    else if (strcmp(xSkillName, "ʨ��֮ŭ") == 0) { return EST_ʨ��֮ŭ; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "ħ����") == 0) { return EST_ħ����; }
//
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "ħ����") == 0) { return EST_ħ����; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//
//    else if (strcmp(xSkillName, "ħ֮�ɲ�") == 0) { return EST_ħ֮�ɲ�; }
//    else if (strcmp(xSkillName, "����֮ħ") == 0) { return EST_����֮ħ; }
//    else if (strcmp(xSkillName, "ħ�����") == 0) { return EST_ħ�����; }
//    else if (strcmp(xSkillName, "�����ħ") == 0) { return EST_�����ħ; }
//    else if (strcmp(xSkillName, "Ǭ������") == 0) { return EST_Ǭ������; }
//
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "���չ⻪") == 0) { return EST_���չ⻪; }
//    else if (strcmp(xSkillName, "����ŭ��") == 0) { return EST_����ŭ��; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "�������") == 0) { return EST_�������; }
//
//    else if (strcmp(xSkillName, "�������") == 0) { return EST_�������; }
//    else if (strcmp(xSkillName, "����ˮ��") == 0) { return EST_����ˮ��; }
//    else if (strcmp(xSkillName, "��Х����") == 0) { return EST_��Х����; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//
//    else if (strcmp(xSkillName, "�����һ�") == 0) { return EST_�����һ�; }
//    else if (strcmp(xSkillName, "����ŭ��") == 0) { return EST_����ŭ��; }
//    else if (strcmp(xSkillName, "��ζ���") == 0) { return EST_��ζ���; }
//    else if (strcmp(xSkillName, "�һ���") == 0) { return EST_�һ���; }
//    else if (strcmp(xSkillName, "��������") == 0) { return EST_��������; }
//
//    else if (strcmp(xSkillName, "��ɰ��ʯ") == 0) { return EST_��ɰ��ʯ; }
//    else if (strcmp(xSkillName, "�˷�����") == 0) { return EST_�˷�����; }
//    else if (strcmp(xSkillName, "̫������") == 0) { return EST_̫������; }
//    else if (strcmp(xSkillName, "����ӿ��") == 0) { return EST_����ӿ��; }
//    else if (strcmp(xSkillName, "����Ǭ��") == 0) { return EST_����Ǭ��; }
//
//    return EST_��Ч;
//}


enum SKILL_TYPE
{
    SGT_��Ч,

    SGT_����,
    SGT_��ӡ,
    SGT_��˯,
    SGT_�ж�,

    SGT_����,
    SGT_��ţ,
    SGT_����,
    SGT_����,

    SGT_�編,
    SGT_ˮ��,
    SGT_�׷�,
    SGT_��,

    SGT_����,
    SGT_���,
    SGT_����,
    SGT_����,
};


enum SKILL_RACIAL_GROUP_TYPE
{
    SRGT_��Ч,
    SRGT_��,
    SRGT_ħ,
    SRGT_��,
    SRGT_��,
};

enum BUFF_TYPE
{
    BT_INVALID,
    BT_CONTROL,     // ����
    BT_GAIN,        // ����
    BT_DEBUFFS      // ����
};

enum BUFF_ID
{
    BI_ICE,         // ��ӡ
    BI_POISON,      // �ж�
    BI_MIX,         // ����
    BI_SLEEP,       // ��˯

    BI_DISC,        // ��˿
    BI_CATTLE,      // ��ţ
    BI_FIRE,        // ����
    BI_WATER,       // ʪˮ
    BI_SPEEDUP,     // ����
    BI_SPEEDDOWN,   // ����
};

//typedef role_info       role_info;
//typedef role_info*   role_info*;


enum E_GAME_CONTROL_TYPE
{
    EGCT_��Ч = 0,
    ͼƬ,
    ��ť,
    ѡ���,
    �����,
    �ı���,
    �����,
    �������,
    �������,
    ����,
};

enum E_GAME_OBJECT_ACTION
{
    EGOA_��Ч = 0,

    EGOA_����,
    EGOA_����,
    EGOA_����,
    EGOA_�ܲ�,

    EGOA_ս��,
    EGOA_����,
    EGOA_�ܻ�,
    EGOA_����,
    EGOA_����,

    EGOA_ʩ��,

    EGOA_δ֪,
    EGOA_���,
};

// ö��˳���ϵ���洢��ȡ˳��,�����޸�
enum E_GAME_OBJECT_DIRECTION
{
    EGOD_��Ч = 0,
    ����,
    ����,
    ����,
    ����,
    ��,
    ��,
    ��,
    ��,
    ��,
    �������� = �� - 1,
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

        E_GAME_OBJECT_DIRECTION xDirX = E_GAME_OBJECT_DIRECTION::��;
        if (xDir.X < 0)
        {
            xDirX = E_GAME_OBJECT_DIRECTION::��;
        }
        else if (xDir.X > 0)
        {
            xDirX = E_GAME_OBJECT_DIRECTION::��;
        }

        E_GAME_OBJECT_DIRECTION xDirY = E_GAME_OBJECT_DIRECTION::��;
        if (xDir.Y < 0)
        {
            xDirY = E_GAME_OBJECT_DIRECTION::��;
        }
        else if (xDir.Y > 0)
        {
            xDirY = E_GAME_OBJECT_DIRECTION::��;
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

        if (xDirX == E_GAME_OBJECT_DIRECTION::��)
        {
            return xDirY;
        }
        else if (xDirY == E_GAME_OBJECT_DIRECTION::��)
        {
            return xDirX;
        }
        else if (xDirX == E_GAME_OBJECT_DIRECTION::��)
        {
            if (xDirY == E_GAME_OBJECT_DIRECTION::��)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            else
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
        }
        else if (xDirX == E_GAME_OBJECT_DIRECTION::��)
        {
            if (xDirY == E_GAME_OBJECT_DIRECTION::��)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            else
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
        }
        else
        {
            return E_GAME_OBJECT_DIRECTION::��;
        }
    }

    DINLINE static E_GAME_OBJECT_DIRECTION GetDirByPos(const Position xPos)
    {
        if (xPos.X > 0)
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            return E_GAME_OBJECT_DIRECTION::��;
        }
        else if (xPos.X < 0)
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::����;
            }
            return E_GAME_OBJECT_DIRECTION::��;
        }
        else
        {
            if (xPos.Y < 0)
            {
                return E_GAME_OBJECT_DIRECTION::��;
            }
            else if (xPos.Y > 0)
            {
                return E_GAME_OBJECT_DIRECTION::��;
            }
            return E_GAME_OBJECT_DIRECTION::��;
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

    // pos1��pos2�ķ�������
    DINLINE static Position GetDir(Position& src, Position& des)
    {
        return Position(des.X - src.X, des.Y - src.Y);
    }

    // pos1��pos2�ľ����Ƿ�С��xFollowKeepDistance
    DINLINE static Boolean SamePosition(Position& pos1, Position& pos2, Single xFollowKeepDistance)
    {
        Single xCurrentDistance = GetDistance(pos1, pos2);
        return xCurrentDistance <= xFollowKeepDistance;
    }

    // pos1��pos2�ľ���
    DINLINE static Single GetDistance(Position pos1, Position pos2)
    {
        return sqrt((pos1.X - pos2.X) * (pos1.X - pos2.X) + (pos1.Y - pos2.Y) * (pos1.Y - pos2.Y));
    }

    // pos1��pos2�ƶ����վ��뻹ʣxResidualDistance��Ӧ���ڵ�posλ��
    DINLINE static Position GetResidualPosition(Position pos1, Position pos2, Single xResidualDistance)
    {
        Single xCurrentDistance = GetDistance(pos1, pos2);
        return GetPursuitPosition(pos1, pos2, xCurrentDistance - xResidualDistance);
    }

    // pos1��pos2���ƶ�xPursuitDistance������λ��Pos
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

// �д�ȷ�ϵȴ�ʱ��
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
//    //ECOT_WILD_SCENE,        // Ұ�ⳡ��
//    //ECOT_CITY_SCENE,        // ���г���
//    //ECOT_BATTLE_SCENE,      // ս������
//    //ECOT_LEADERBOARD_SCENE, // ���а񳡾�
//
//    //ECOT_ALL_SCENE,         // ���г���
//
//
//
//
//    //ECOT_LOGIN_PLAYER,          // �������
//    //ECOT_SCENE_PLAYER,          // �������
//
//
//    //ECOT_FRIENDS_MANGER,        // ���ѹ�����
//
//};

enum CALL_BACK_TYPE
{
    // ���
    //WEB_DB_CB_ACCOUNT_REGISTER = 0x20000000,// ע���˺�
    //WEB_DB_CB_ACCOUNT_CHECK,                // ����˺�
    //WEB_DB_CB_ACCOUNT_LOGIN,                // ��¼�˺�

    //WEB_DB_CB_ROLE_SELECT_COUNT,    // ��ɫ����
    //WEB_DB_CB_CLEAR_ROLE,           // ��ս�ɫ
    //WEB_DB_CB_ROLE_CREATE,          // ������ɫ
    //WEB_DB_CB_ROLE_SELECT_INFO,     // ��ѯ��Ϣ

    //WEB_CB_ROLE_LEAVE_LOGIN,        // ��ɫ�뿪��¼
    //WEB_CB_ROLE_ENTER_SCENE,        // ��ɫ���볡��

    // TIME
    TIMER_CB_SERVER_TICK = 0x30000000,// ����1��֡
    //TIMER_CB_SERVER_TICK_,// ����1��֡

    //TIMER_CB_WAIT_OPERATION_SURPLUS_SECOND, // ������ʣ�����ʱ��

    //TIMER_CB_WAIT_ACTIONDONE_SURPLUS_SECOND,// �ͻ��˲��Ŷ���ʣ��ʱ��


    TIMER_CB_SCENEPLAYER_TEST,

    CUSTOM_CB_SCENE_SAVE_PLAYER,

    // �رյ�¼������
    LOGIN_CB_CLOSE_ALL_CLIENT = 0x40000000,

    SCENE_CB_CLOSE,

    // ����ϵͳ
    CUSTOM_CB_FRIEND_MANAGER_CLOSE = 0x60000000,

    // ��ʱ, �����������ȷ��
    TIMER_CB_REQUEST_FRIEND_PRESCRIPTION_CHECK,

    // ��ʱ, ˢ���Ƽ����ѷֶ���Ϣ
    TIMER_CB_UPDATE_FRIEND_FRAGMENT_INFO,


    // ���г����д��ڹ㲥����
    TIMER_CB_CITY_SCENE_NPC_ARTILLERY_BROADCAST_RECORD_QUEUE,

    // ���ûָ������㲥����
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
