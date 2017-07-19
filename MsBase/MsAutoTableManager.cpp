#include "Precompiled.h"
#include "MsSqlServerManager.h"

// �����������캯��
CAutoTableManager::CAutoTableManager(ATAC* pAtac)
    : m_IsInit(FALSE) // ��ǰ���󴴽�ʱ��δ��ʼ��
    , m_uItemCount(pAtac->m_uItemCount)
    , m_uLineCount(pAtac->m_uLineCount)
    , m_pAi(NULL)
    , m_uItemIndex(0)
    , m_uLineIndex(0)
    , m_uValidLine(0)
{
    m_pAi = NEW AI[m_uItemCount];
    //AssertEx(AL_NORMAL, m_pAi, _T("m_pAiΪ��,�ڴ治��"));
    ZeroMemory(m_pAi, sizeof(AI) * m_uItemCount);

    for (UINT i = 0; i < m_uItemCount; i++)
    {
        m_pAi[i].m_pAta = pAtac->m_pAta[i];
    }
}

// ����������������
CAutoTableManager::~CAutoTableManager(VOID)
{


    this->Release();
    SAFE_DELETE_ARRAY(m_pAi);


}

// ��ʼ����������
BOOL CAutoTableManager::Init(VOID)
{


    for (UINT i = 0; i < m_uItemCount; i++)
    {
        switch (m_pAi[i].m_pAta)
        {
            case MS_SQL_TYPE_INT:
            {
                m_pInt = NEW INT[m_uLineCount];
                //AssertEx(AL_NORMAL, m_pInt, _T("m_pIntΪ��,�ڴ治��"));
                ZeroMemory(m_pInt, sizeof(INT) * m_uLineCount);
                m_pAi[i].m_lpItem = m_pInt;
            }break;
            case MS_SQL_TYPE_TCHAR:
            {
                m_pTchar = NEW TCA[m_uLineCount];
                //AssertEx(AL_NORMAL, m_pTchar, _T("m_pIntΪ��,�ڴ治��"));
                ZeroMemory(m_pTchar, sizeof(TCA) * m_uLineCount);
                m_pAi[i].m_lpItem = m_pTchar;
            }break;
            case MS_SQL_TYPE_FLOAT:
            {
                m_pFloat = NEW FLOAT[m_uLineCount];
                //AssertEx(AL_NORMAL, m_pFloat, _T("m_pIntΪ��,�ڴ治��"));
                ZeroMemory(m_pFloat, sizeof(FLOAT) * m_uLineCount);
                m_pAi[i].m_lpItem = m_pFloat;
            }break;
            case MS_SQL_TYPE_DATETIME:
            {
                m_pDataTime = NEW TIMESTAMP_STRUCT[m_uLineCount];
                //AssertEx(AL_NORMAL, m_pDataTime, _T("m_pIntΪ��,�ڴ治��"));
                ZeroMemory(m_pDataTime, sizeof(TIMESTAMP_STRUCT) * m_uLineCount);
                m_pAi[i].m_lpItem = m_pDataTime;
            }break;
            default:
            {
                //AssertEx(AL_NORMAL, FALSE, _T("��Ч����,�����߼�"));
            }
        }
    }
    m_uValidLine = 0;
    m_IsInit = TRUE;
    // ���سɹ�
    return TRUE;
}

// ����Ƿ��Ѿ���ʼ����������
BOOL CAutoTableManager::IsInit(VOID)
{
    return m_IsInit;
}

// �ͷű�������
BOOL CAutoTableManager::Release(VOID)
{


    if (!m_IsInit)
    {
        return TRUE;
    }
    for (UINT i = 0; i < m_uItemCount; i++)
    {
        switch (m_pAi[i].m_pAta)
        {
            case MS_SQL_TYPE_INT:
            {
                m_pInt = (INT*)m_pAi[i].m_lpItem;
                SAFE_DELETE_ARRAY(m_pInt);
                m_pAi[i].m_lpItem = m_pInt;
            }break;
            case MS_SQL_TYPE_TCHAR:
            {
                m_pTchar = (TCA*)m_pAi[i].m_lpItem;
                SAFE_DELETE_ARRAY(m_pTchar);
                m_pAi[i].m_lpItem = m_pTchar;
            }break;
            case MS_SQL_TYPE_FLOAT:
            {
                m_pFloat = (FLOAT*)m_pAi[i].m_lpItem;
                SAFE_DELETE_ARRAY(m_pFloat);
                m_pAi[i].m_lpItem = m_pFloat;
            }break;
            case MS_SQL_TYPE_DATETIME:
            {
                m_pDataTime = (TIMESTAMP_STRUCT*)m_pAi[i].m_lpItem;
                SAFE_DELETE_ARRAY(m_pDataTime);
                m_pAi[i].m_lpItem = m_pDataTime;
            }break;
            default:
            {
                //AssertEx(AL_NORMAL, FALSE, _T("��Ч����,�����߼�"));
            }
        }
    }
    m_uValidLine = 0;
    m_IsInit = FALSE;

    // ���سɹ�
    return TRUE;
}

LPVOID CAutoTableManager::GetTableItemInLine(UINT uItemIndex, UINT uLineIndex)
{


    //AssertEx(AL_NORMAL, (uItemIndex < m_uItemCount), _T("uItemIndex����Խ��,���󴫲�"));
    //AssertEx(AL_NORMAL, (uLineIndex < m_uLineCount), _T("uLineIndex����Խ��,���󴫲�"));

    switch (m_pAi[uItemIndex].m_pAta)
    {
        case MS_SQL_TYPE_INT:
        {
            m_pInt = (INT*)m_pAi[uItemIndex].m_lpItem;
            return &m_pInt[uLineIndex];
        }break;

        case MS_SQL_TYPE_TCHAR:
        {
            m_pTchar = (TCA*)m_pAi[uItemIndex].m_lpItem;
            return m_pTchar[uLineIndex].m_strText;
        }break;

        case MS_SQL_TYPE_FLOAT:
        {
            m_pFloat = (FLOAT*)m_pAi[uItemIndex].m_lpItem;
            return &m_pFloat[uLineIndex];
        }break;

        case MS_SQL_TYPE_DATETIME:
        {
            m_pDataTime = (TIMESTAMP_STRUCT*)m_pAi[uItemIndex].m_lpItem;
            return &m_pDataTime[uLineIndex];
        }break;

        default:
        {
            //AssertEx(AL_NORMAL, FALSE, _T("��Ч����,�����߼�"));
            return NULL;
        }break;
    }
}

INT* CAutoTableManager::GetTableItemInLineAsINT(UINT uItemIndex, UINT uLineIndex)
{
    m_pInt = (INT*)this->GetTableItemInLine(uItemIndex, uLineIndex);
    //AssertEx(AL_NORMAL, m_pInt, _T("m_pIntΪ��,����ʧ��"));
    //AssertEx(AL_NORMAL, (m_pAi[uItemIndex].m_pAta == MS_SQL_TYPE_INT), _T("���Ͳ�ƥ��,���󴫲�"));
    return m_pInt;
}

TCA* CAutoTableManager::GetTableItemInLineAsTCHAR(UINT uItemIndex, UINT uLineIndex)
{
    m_pTchar = (TCA*)this->GetTableItemInLine(uItemIndex, uLineIndex);
    //AssertEx(AL_NORMAL, m_pTchar, _T("m_pTcharΪ��,����ʧ��"));
    //AssertEx(AL_NORMAL, (m_pAi[uItemIndex].m_pAta == MS_SQL_TYPE_TCHAR), _T("���Ͳ�ƥ��,���󴫲�"));
    return m_pTchar;
}

FLOAT* CAutoTableManager::GetTableItemInLineAsFLOAT(UINT uItemIndex, UINT uLineIndex)
{
    m_pFloat = (FLOAT*)this->GetTableItemInLine(uItemIndex, uLineIndex);
    //AssertEx(AL_NORMAL, m_pFloat, _T("m_pFloatΪ��,����ʧ��"));
    //AssertEx(AL_NORMAL, (m_pAi[uItemIndex].m_pAta == MS_SQL_TYPE_FLOAT), _T("���Ͳ�ƥ��,���󴫲�"));
    return m_pFloat;
}

TIMESTAMP_STRUCT* CAutoTableManager::GetTableItemInLineAsDATETIME(UINT uItemIndex, UINT uLineIndex)
{
    m_pDataTime = (TIMESTAMP_STRUCT*)this->GetTableItemInLine(uItemIndex, uLineIndex);
    //AssertEx(AL_NORMAL, m_pDataTime, _T("m_pDataTimeΪ��,����ʧ��"));
    //AssertEx(AL_NORMAL, (m_pAi[uItemIndex].m_pAta == MS_SQL_TYPE_DATETIME), _T("���Ͳ�ƥ��,���󴫲�"));
    return m_pDataTime;
}

VOID CAutoTableManager::SetTablePointer(UINT uItemIndex, UINT uLineIndex)
{
    //AssertEx(AL_NORMAL, (uItemIndex < m_uItemCount), _T("uItemIndex����Խ��,���󴫲�"));
    //AssertEx(AL_NORMAL, (uLineIndex < m_uLineCount), _T("uLineIndex����Խ��,���󴫲�"));
    m_uItemIndex = uItemIndex;
    m_uLineIndex = uLineIndex;
}

VOID CAutoTableManager::GetTablePointer(UINT& uItemIndex, UINT& uLineIndex)
{
    //AssertEx(AL_NORMAL, (uItemIndex < m_uItemCount), _T("uItemIndex����Խ��,���󴫲�"));
    //AssertEx(AL_NORMAL, (uLineIndex < m_uLineCount), _T("uLineIndex����Խ��,���󴫲�"));
    uItemIndex = m_uItemIndex;
    uLineIndex = m_uLineIndex;
}


VOID CAutoTableManager::SetValidLine(UINT uCount)
{
    //AssertEx(AL_NORMAL, (uCount <= m_uLineCount), _T("nCount����Խ��,���󴫲�"));
    m_uValidLine = uCount;
}

INT CAutoTableManager::GetValidLine(VOID)
{
    return m_uValidLine;
}

BOOL CAutoTableManager::AddTableItemPointer(VOID)
{
    if (m_uItemIndex + 1 < m_uItemCount)
    {
        m_uItemIndex++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CAutoTableManager::SubTableItemPointer(VOID)
{
    if (m_uItemIndex > 0)
    {
        m_uItemIndex--;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CAutoTableManager::AddTableLinePointer(VOID)
{
    if (m_uLineIndex + 1 < m_uLineCount)
    {
        m_uLineIndex++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CAutoTableManager::SubTableLinePointer(VOID)
{
    if (m_uLineIndex > 0)
    {
        m_uLineIndex--;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CAutoTableManager::operator ++ (VOID)
{
    return this->AddTableItemPointer();
}

BOOL CAutoTableManager::operator ++ (INT)
{
    return this->AddTableLinePointer();
}

BOOL CAutoTableManager::operator -- (VOID)
{
    return this->SubTableItemPointer();
}

BOOL CAutoTableManager::operator -- (INT)
{
    return this->SubTableLinePointer();
}

VOID CAutoTableManager::GetTableSize(UINT& uItemCount, UINT& uLineCount)
{
    uItemCount = m_uItemCount;
    uLineCount = m_uLineCount;
}

SQL_TYPE CAutoTableManager::GetTableItemType(UINT uItemIndex)
{
    //AssertEx(AL_NORMAL, (uItemIndex < m_uItemCount), _T("uItemIndex����Խ��,���󴫲�"));
    return m_pAi[uItemIndex].m_pAta;
}

INT CAutoTableManager::GetTableItemSize(UINT uItemIndex, UINT uLineIndex, BOOL IsUsable)
{

    //AssertEx(AL_NORMAL, (uItemIndex < m_uItemCount), _T("uItemIndex����Խ��,���󴫲�"));
    switch (m_pAi[uItemIndex].m_pAta)
    {
        case MS_SQL_TYPE_INT:
        {
            return sizeof(INT);
        }break;
        case MS_SQL_TYPE_TCHAR:
        {
            if (IsUsable)
            {
                return sizeof(TCA);
            }
            else
            {
                m_pTchar = (TCA*)m_pAi[uItemIndex].m_lpItem;
                return INT((_tcslen(m_pTchar[uLineIndex].m_strText) + 1) * sizeof(TCHAR));
            }
        }break;
        case MS_SQL_TYPE_FLOAT:
        {
            return sizeof(FLOAT);
        }break;
        case MS_SQL_TYPE_DATETIME:
        {
            return sizeof(TIMESTAMP_STRUCT);
        }break;
        default:
        {
            return 0;
        }break;
    }
}
