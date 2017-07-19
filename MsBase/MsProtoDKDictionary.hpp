#pragma once

template<class TKey, class TValue>
class MsProtoDKDictionary
{
public:
    MsProtoDKDictionary()
    {
        ;
    }

    MsProtoDKDictionary(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedField<TValue>* xRepeatedValue)
    {
        this->LoadFromProto(xRepeatedKey, xRepeatedValue);
    }

    //MsProtoDKDictionary(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto = nullptr)
    //{
    //    if (xRepeatedProto)
    //    {
    //        this->LoadFromProto(xRepeatedProto);
    //    }
    //}

    class Enumerator
    {
    public:
        DINLINE Boolean MoveNext()
        {
            if (m_is_new) { m_is_new = False; }
            else { m_cur++; }
            return m_cur != m_end;
        }

        DINLINE void ToBegin()
        {
            m_beg = m_MapA.begin();
            m_end = m_MapA.end();
            m_cur = m_MapA.begin();
            m_is_new = True;
        }

        DINLINE void ToEnd()
        {
            m_beg = m_MapA.begin();
            m_end = m_MapA.end();
            m_cur = m_MapA.end();
            m_is_new = True;
        }

        DINLINE TKey GetKey() { return m_cur->first; }

        DINLINE TValue GetValue() { return m_cur->second; }

        DINLINE TValue& Value() { return m_cur->second; }

        DINLINE TValue SeekToBegin() { return m_cur = m_beg; }

    private:
        friend class MsProtoDKDictionary <TKey, TValue>;
        typename std::map<TKey, TValue>::iterator m_beg;
        typename std::map<TKey, TValue>::iterator m_cur;
        typename std::map<TKey, TValue>::iterator m_end;
        Boolean m_is_new;
    };

public:
    //DINLINE void SaveToProto(BaseProto* xProto)
    //{
    //    return SaveToProto(*xProto);
    //}
    //DINLINE void SaveToProto(BaseProto& xProto)
    //{
    //    xProto.clear_keys();
    //    xProto.clear_values();
    //    for (auto it = m_MapA.begin(); it != m_MapA.end(); ++it)
    //    {
    //        xProto.add_keys(it->first);
    //        xProto.add_values(it->second);
    //    }
    //}

    DINLINE void SaveToProto(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedField<TValue>* xRepeatedValue)
    {
        xRepeatedKey->Clear();
        xRepeatedValue->Clear();
        for (auto it = m_MapA.begin(); it != m_MapA.end(); ++it)
        {
            xRepeatedKey->Add(it->first);
            xRepeatedValue->Add(it->second);
        }
    }

    //DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TKey>& xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>& xRepeatedValue)
    //{
    //    xRepeatedKey.Clear();
    //    xRepeatedValue.Clear();
    //    for (auto it = m_MapA.begin(); it != m_MapA.end(); ++it)
    //    {
    //        xRepeatedKey.Add()->CopyFrom(it->first);
    //        xRepeatedValue.Add()->CopyFrom(it->second);
    //    }
    //}
    //DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto)
    //{
    //    return SaveToProto(*xRepeatedProto);
    //}
    //DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TValue>& xRepeatedProto)
    //{
    //    xRepeatedProto.Clear();
    //    for (auto it = m_MapA.begin(); it != m_MapA.end(); ++it)
    //    {
    //        xRepeatedProto.Add()->CopyFrom(it->second);
    //    }
    //}

    //DINLINE void LoadFromProto(BaseProto* xProto)
    //{
    //    return LoadFromProto(*xProto);
    //}
    //DINLINE void LoadFromProto(BaseProto& xProto)
    //{
    //    m_MapA.clear();
    //    for (int i = 0; i < xProto.keys_size(); i++)
    //    {
    //        const TKey& xKey = xProto.keys(i);
    //        const TValue& xValue = xProto.values(i);
    //        m_MapA[xRepeatedKey].CopyFrom(xValue);
    //    }

    //    Int32 xCount = xProto.keys_size();
    //    for (Int32 i = 0; i < xCount; i++)
    //    {
    //        m_MapA[xProto.keys(i)] = xProto.values(i);
    //    }
    //}
    DINLINE void LoadFromProto(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedField<TValue>* xRepeatedValue)
    {
        m_MapA.clear();
        for (int i = 0; i < xRepeatedValue->size(); i++)
        {
            const TKey& xKey = xRepeatedKey->Get(i);
            const TValue& xValue = xRepeatedValue->Get(i);
            m_MapA[xKey] = xValue;
            m_MapB[xValue] = xKey;
        }
    }
    //DINLINE void LoadFromProto(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto)
    //{
    //    return LoadFromProto(*xRepeatedProto);
    //}
    //DINLINE void LoadFromProto(const google::protobuf::RepeatedPtrField<TValue>& xRepeatedProto)
    //{
    //    m_MapA.clear();
    //    for (int i = 0; i < xRepeatedProto.size(); i++)
    //    {
    //        const TValue& xValue = xRepeatedProto.Get(i);
    //        m_MapA[xValue.index_key()].CopyFrom(xValue);
    //    }
    //}
public:

    DINLINE Boolean ContainsKey(TKey key)
    {
        return m_MapA.find(key) != m_MapA.end();
    }

    DINLINE Boolean ContainsValue(TValue value)
    {
        return m_MapB.find(value) != m_MapB.end();
    }

    DINLINE void Add(TKey key, TValue value) { m_MapA.insert(std::pair<TKey, TValue>(key, value)); }

    DINLINE void Remove(TKey key)
    {
        if (ContainsKey(key))
        {
            m_MapB.erase(m_MapA[key]);
        }
        m_MapA.erase(key);
    }

    DINLINE void Clear() { m_MapA.clear(); }

    DINLINE Boolean Empty() { return m_MapA.empty(); }

    DINLINE Int32 GetCount() { return (Int32)m_MapA.size(); }

    DINLINE Int64 GetBigCount() { return (Int64)m_MapA.size(); }

    DINLINE TValue& operator [](TKey key) { return m_MapA[key]; }

    DINLINE TValue GetValue(TKey key)
    {
        std::map<TKey, TValue>::iterator it = m_MapA.find(key);
        if (it != m_MapA.end())
        {
            return it->second;
        }
        else { return NULL; }
    }

    DINLINE TKey GetKey(TValue value)
    {
        std::map<TValue, TKey>::iterator it = m_MapB.find(value);
        if (it != m_MapB.end())
        {
            return it->second;
        }
        else { return NULL; }
    }

    DINLINE TKey GetLastKey() { return (--(m_MapA.end()))->first; }
    DINLINE TValue GetLastValue() { return (--(m_MapA.end()))->second; }

    DINLINE Enumerator GetEnumerator()
    {
        Enumerator xEnumerator;
        xEnumerator.m_beg = m_MapA.begin();
        xEnumerator.m_cur = m_MapA.begin();
        xEnumerator.m_end = m_MapA.end();
        xEnumerator.m_is_new = True;
        return xEnumerator;
    }

public:
    std::map<TKey, TValue> m_MapA;
    std::map<TValue, TKey> m_MapB;
};
