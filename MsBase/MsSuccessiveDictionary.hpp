#pragma once

template<class TKey, class TValue>
class MsSuccessiveDictionary
{
    struct BTKeyTValue
    {
        Boolean IsUse;
        TKey Key;
        TValue Value;
    };

public:
    class Enumerator
    {
    public:
        DINLINE Boolean MoveNext(){ if (m_is_new){ m_is_new = False; } else{ m_cur++; }return m_cur != m_end; }
        DINLINE TKey GetKey(){ return m_cur->first; }
        DINLINE TKey& Key(){ return m_cur->first; }
        DINLINE TValue GetValue(){ return m_lpData[m_cur->second].Value; }
        DINLINE TValue& Value(){ return m_lpData[m_cur->second].Value; }
        DINLINE TValue SeekToBegin(){ return m_cur = m_beg; }

    private:
        friend class MsSuccessiveDictionary < TKey, TValue > ;
        typename std::map<TKey, Int32>::iterator m_beg;
        typename std::map<TKey, Int32>::iterator m_cur;
        typename std::map<TKey, Int32>::iterator m_end;
        Boolean m_is_new;
        BTKeyTValue* m_lpData;
    };

public:
    MsSuccessiveDictionary(MsSuccessiveDictionary& xMsSuccessiveDictionary) : m_lpData(nullptr), m_MaxCount(xMsSuccessiveDictionary.m_MaxCount)
    {
        m_lpData = NEW BTKeyTValue[m_MaxCount];
        memcpy(m_lpData, xMsSuccessiveDictionary.m_lpData, sizeof(BTKeyTValue) * m_MaxCount);
        for (Int32 i = 0; i < m_MaxCount; i++)
        {
            if (m_lpData[i].IsUse == True)
            {
                m_Map.insert(std::pair<TKey, Int32>(m_lpData[i].Key, i));
            }
            else
            {
                m_FreeOffset.Enqueue(i);
            }
        }
    }

    MsSuccessiveDictionary(LPBYTE lpAddr) : m_lpData(nullptr), m_MaxCount(*((Int32*)lpAddr))
    {
        m_lpData = NEW BTKeyTValue[m_MaxCount];
        memcpy(m_lpData, lpAddr + sizeof(m_MaxCount), sizeof(BTKeyTValue) * m_MaxCount);
        for (Int32 i = 0; i < m_MaxCount; i++)
        {
            if (m_lpData[i].IsUse == True)
            {
                m_Map.insert(std::pair<TKey, Int32>(m_lpData[i].Key, i));
            }
            else
            {
                m_FreeOffset.Enqueue(i);
            }
        }
    }

    MsSuccessiveDictionary(Int32 xMaxCount = 1024) : m_lpData(nullptr), m_MaxCount(xMaxCount)
    {
        m_lpData = NEW BTKeyTValue[m_MaxCount];
        for (Int32 i = 0; i < m_MaxCount; i++){ m_FreeOffset.Enqueue(i); }
    }

    ~MsSuccessiveDictionary(){ SAFE_DELETE_ARRAY(m_lpData); }

    DINLINE Boolean ContainsKey(TKey xKey)
    {
        std::map<TKey, Int32>::iterator it = m_Map.begin();
        return m_Map.find(xKey) != m_Map.end();
    }

    DINLINE Boolean Add(TKey key, TValue value)
    {
        if (GetCount() < m_MaxCount && m_FreeOffset.GetCount() > 0 && !ContainsKey(key))
        {
            Int32 xOffset = m_FreeOffset.Dequeue();
            m_lpData[xOffset].Value = value;
            m_lpData[xOffset].Key = key;
            m_lpData[xOffset].IsUse = True;
            m_Map.insert(std::pair<TKey, Int32>(key, xOffset));
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE void Remove(TKey key)
    {
        if (ContainsKey(key))
        {
            Int32 xOffset = m_Map[key];
            m_lpData[xOffset].IsUse = False;
            m_Map.erase(key);
            m_FreeOffset.Enqueue(xOffset);
        }
    }

    DINLINE void Clear(){ m_Map.clear(); m_FreeOffset.Clear(); for (Int32 i = 0; i < m_MaxCount; i++){ m_lpData[i].IsUse = False; m_FreeOffset.Enqueue(i); } }

    DINLINE Boolean Empty() { return m_Map.empty(); }

    DINLINE Int32 GetCount() { return (Int32)m_Map.size(); }

    DINLINE Int64 GetBigCount() { return (Int64)m_Map.size(); }

    DINLINE TValue& operator [](TKey key){ return m_lpData[m_Map[key]].Value; }

    DINLINE TValue GetValue(TKey key){ return m_lpData[m_Map[key]].Value; }

    DINLINE Int32 GetByteSize(){ return (sizeof(BTKeyTValue) * m_MaxCount) + sizeof(m_MaxCount); }

    DINLINE Int32 GetMaxSize(){ return m_MaxCount; }

    DINLINE void SaveTo(LPBYTE lpAddr){ *((Int32*)lpAddr) = m_MaxCount; memcpy(lpAddr + sizeof(m_MaxCount), m_lpData, sizeof(BTKeyTValue) * m_MaxCount); }

    DINLINE Enumerator GetEnumerator()
    {
        Enumerator xEnumerator;
        xEnumerator.m_beg = m_Map.begin();
        xEnumerator.m_cur = m_Map.begin();
        xEnumerator.m_end = m_Map.end();
        xEnumerator.m_is_new = True;
        xEnumerator.m_lpData = m_lpData;
        return xEnumerator;
    }

public:
    Int32 m_MaxCount;
    BTKeyTValue* m_lpData;

    MsQueue<Int32> m_FreeOffset;
    std::map<TKey, Int32> m_Map;
};
