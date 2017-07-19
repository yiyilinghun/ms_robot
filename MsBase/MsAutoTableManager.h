#pragma once

enum SQL_TYPE;
// �����Զ������Ϣ�ṹ��
typedef struct ATA_STRUCT
{
    SQL_TYPE*   m_pAta;         // ���������
    UINT        m_uItemCount;   // ��������
    UINT        m_uLineCount;   // ����и���
}ATAC;

typedef struct ATA_ITEM 
{
    SQL_TYPE    m_pAta;     // ���������
    LPVOID      m_lpItem;   // �����ͷָ��
}AI;

typedef struct TCHAR_ARRAY 
{
    TCHAR   m_strText[MAX_PATH];
}TCA;

class CAutoTableManager
{
public: // ���졢����
    CAutoTableManager(ATAC* pAtac);     // �����������캯��
    virtual ~CAutoTableManager(VOID);   // ����������������

public: // �ӿ�
    BOOL                Init(VOID);     // ��ʼ����������
    BOOL                IsInit(VOID);   // ����Ƿ��Ѿ���ʼ����������
    BOOL                Release(VOID);  // �ͷű�������

    VOID                SetTablePointer(UINT uItemIndex, UINT uLineIndex);
    VOID                GetTablePointer(UINT& uItemIndex, UINT& uLineIndex);

    VOID                SetValidLine(UINT uCount);
    INT                 GetValidLine(VOID);

    VOID                GetTableSize(UINT& uItemCount, UINT& uLineCount);

    SQL_TYPE    GetTableItemType(UINT uItemIndex);
    INT         GetTableItemSize(UINT uItemIndex, UINT uLineIndex, BOOL IsUsable);

    BOOL AddTableItemPointer(VOID);
    BOOL SubTableItemPointer(VOID);
    BOOL AddTableLinePointer(VOID);
    BOOL SubTableLinePointer(VOID);
    BOOL operator ++ (VOID);
    BOOL operator ++ (INT);
    BOOL operator -- (VOID);
    BOOL operator -- (INT);

    // ��ָ����Ŀָ���з����׵�ַ
    LPVOID              GetTableItemInLine(UINT uItemIndex, UINT uLineIndex);
    INT*                GetTableItemInLineAsINT(UINT uItemIndex, UINT uLineIndex);
    TCA*                GetTableItemInLineAsTCHAR(UINT uItemIndex, UINT uLineIndex);
    FLOAT*              GetTableItemInLineAsFLOAT(UINT uItemIndex, UINT uLineIndex);
    TIMESTAMP_STRUCT*   GetTableItemInLineAsDATETIME(UINT uItemIndex, UINT uLineIndex);

private:// ˽��
    AI*                 m_pAi;          // �����Ϣָ��
    INT*                m_pInt;
    TCA*                m_pTchar;
    FLOAT*              m_pFloat;
    TIMESTAMP_STRUCT*   m_pDataTime;
    BOOL                m_IsInit;       // ��ǰ�����Ƿ��Ѿ���ʼ�����
    UINT                m_uItemCount;   // ��������
    UINT                m_uLineCount;   // ����и���
    UINT                m_uItemIndex;   // ��ǰ��λ��
    UINT                m_uLineIndex;   // ��ǰ��λ��
    UINT                m_uValidLine;   // ��Ч��¼������
};
