#pragma once

template<class TKey, class TValue>
class KeyIndexValue
{
public:
    size_t m_Index;
    TKey m_Key;
    TValue m_Value;
};

template<class TKey, class TValue>
class MsListDict
{
public:
    class Enumerator
    {
    public:
        DINLINE Boolean MoveNext()
        {
            if (m_is_new) { m_is_new = False; }
            else { m_cur++; }
            return m_cur != m_end;
        }

        DINLINE TKey GetKey() { return m_cur->first; }
        DINLINE TValue GetValue() { return m_cur->second.m_Value; }

        DINLINE TKey& Key() { return m_cur->first; }
        DINLINE TValue& Value() { return m_cur->second.m_Value; }

        DINLINE TValue SeekToBegin() { return m_cur = m_beg; }

    private:
        friend class MsListDict < TKey, TValue >;
        typename std::map<TKey, KeyIndexValue<TKey, TValue>>::iterator m_beg;
        typename std::map<TKey, KeyIndexValue<TKey, TValue>>::iterator m_cur;
        typename std::map<TKey, KeyIndexValue<TKey, TValue>>::iterator m_end;
        Boolean m_is_new;
    };

private:
    //Dictionary<TKey, KeyIndexValue<TKey, TValue>> m_Key = new Dictionary<TKey, KeyIndexValue<TKey, TValue>>();
    //Dictionary<Int32, KeyIndexValue<TKey, TValue>> m_Index = new Dictionary<Int32, KeyIndexValue<TKey, TValue>>();

    //public Int32 GetCount{ get{ return m_Key.Count; } }

    //public Boolean ContainsKey(TKey xKey) { return m_Key.ContainsKey(xKey); }

public:
    //DINLINE void Add(TKey key, TValue value) { m_Map.insert(std::pair<TKey, TValue>(key, value)); }
    size_t Add(TKey xKey, TValue xValue)
    {
        KeyIndexValue<TKey, TValue> xKeyIndexValue;
        xKeyIndexValue.m_Index = m_Index.size();
        xKeyIndexValue.m_Key = xKey;
        xKeyIndexValue.m_Value = xValue;

        m_Key.insert(std::pair<TKey, KeyIndexValue<TKey, TValue>>(xKey, xKeyIndexValue));
        m_Index.insert(std::pair<size_t, KeyIndexValue<TKey, TValue>>(xKeyIndexValue.m_Index, xKeyIndexValue));
        return xKeyIndexValue.m_Index;
    }

    //DINLINE void Clear() { m_Map.clear(); }
    void Clear()
    {
        m_Key.clear();
        m_Index.clear();
    }

    TValue& GetValueByIndex(size_t xIndex)
    {
        return m_Index[xIndex].m_Value;
    }

    TValue& GetValueByKey(TKey key)
    {
        return m_Key[key].m_Value;
    }

    DINLINE Boolean ContainsKey(TKey xKey)
    {
        std::map<TKey, KeyIndexValue<TKey, TValue>>::iterator it = m_Key.begin();
        return m_Key.find(xKey) != m_Key.end();
    }

    DINLINE void Remove(TKey key) { m_Key.erase(key); }

    DINLINE Int32 GetCount() { return (Int32)m_Key.size(); }

    //DINLINE TValue& operator [](TKey key) { return m_Map[key]; }

    //DINLINE TValue GetValue(TKey key){ return m_Map[key]; }

    DINLINE Enumerator GetEnumerator()
    {
        Enumerator xEnumerator;
        xEnumerator.m_beg = m_Key.begin();
        xEnumerator.m_cur = m_Key.begin();
        xEnumerator.m_end = m_Key.end();
        xEnumerator.m_is_new = True;
        return xEnumerator;
    }

private:
    std::map<TKey, KeyIndexValue<TKey, TValue>> m_Key;
    std::map<size_t, KeyIndexValue<TKey, TValue>> m_Index;
};
