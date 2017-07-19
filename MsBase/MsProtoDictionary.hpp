#pragma once

template<class TKey, class TValue>
class MsProtoDictionary
{
public:
    MsProtoDictionary(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedField<TValue>* xRepeatedValue)
    {
        this->LoadFromProto(xRepeatedKey, xRepeatedValue);
    }

    MsProtoDictionary(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto = nullptr)
    {
        if (xRepeatedProto)
        {
            this->LoadFromProto(xRepeatedProto);
        }
    }
public:
    DINLINE void SaveToProto(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>* xRepeatedValue)
    {
        xRepeatedKey->Clear();
        xRepeatedValue->Clear();
        for (auto it = m_Container.begin(); it != m_Container.end(); ++it)
        {
            xRepeatedKey->Add(it->first);
            xRepeatedValue->Add()->CopyFrom(it->second);
        }
    }
    DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto)
    {
        return SaveToProto(*xRepeatedProto);
    }
    DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TValue>& xRepeatedProto)
    {
        xRepeatedProto.Clear();
        for (auto it = m_Container.begin(); it != m_Container.end(); ++it)
        {
            xRepeatedProto.Add()->CopyFrom(it->second);
        }
    }

    DINLINE void LoadFromProto(google::protobuf::RepeatedField<TKey>* xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>* xRepeatedValue)
    {
        m_Container.clear();
        for (int i = 0; i < xRepeatedValue->size(); i++)
        {
            const TKey& xKey = xRepeatedKey->Get(i);
            const TValue& xValue = xRepeatedValue->Get(i);
            m_Container[xKey] = xValue;
        }
    }
    DINLINE void LoadFromProto(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto)
    {
        return LoadFromProto(*xRepeatedProto);
    }
    DINLINE void LoadFromProto(google::protobuf::RepeatedPtrField<TValue>& xRepeatedProto)
    {
        m_Container.clear();
        for (int i = 0; i < xRepeatedProto.size(); i++)
        {
            const TValue& xValue = xRepeatedProto.Get(i);
            m_Container[(TKey)i].CopyFrom(xValue);
        }
    }
public:

    DINLINE Boolean ContainsKey(TKey key) { return m_Container.find(key) != m_Container.end(); }
    DINLINE Int32 GetCount() { return (Int32)m_Container.size(); }
    DINLINE Int64 GetBigCount() { return (Int64)m_Container.size(); }

    DINLINE auto GetFirst() { return m_Container.begin(); }
    DINLINE auto GetLast() { return m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); }
    DINLINE TKey GetKey() { return m_It->first; }
    DINLINE TValue GetValue() { return m_It->second; }
    DINLINE TValue GetValue(TKey key) { std::map<TKey, TValue>::iterator it = m_Container.find(key); if (it != m_Container.end()) { return it->second; } else { return NULL; } }
    DINLINE TValue& operator [](TKey key) 
    {
        if (m_Container.size() == 0)
        {
            TValue& xRet = m_Container[key];
            m_It = m_Container.begin();
            return xRet;
        }
        else
        {
            return m_Container[key];
        }
    }
    DINLINE TKey Key() { return m_It->first; }
    DINLINE TValue& Value() { return m_It->second; }
    DINLINE void ToFirst() { m_It = m_Container.begin(); }
    DINLINE auto& ForFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE TValue& ValueToFirst() { this->ToFirst(); return Value(); }
    DINLINE void ToLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); }
    DINLINE auto& ForLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE void ToEnd() { m_It = m_Container.end(); }

    DINLINE Boolean Empty() { return m_Container.empty(); }
    DINLINE Boolean IsValid() { return m_It != m_Container.end(); }
    DINLINE Boolean IsBegin() { return m_It == m_Container.begin(); }
    DINLINE Boolean IsEnd() { return m_It == m_Container.end(); }

    DINLINE void Add(TKey key, TValue value) 
    {
        if (m_Container.size() == 0)
        {
            m_Container.insert(std::pair<TKey, TValue>(key, value));
            m_It = m_Container.begin();
        }
        else
        {
            m_Container.insert(std::pair<TKey, TValue>(key, value));
        }
    }
    DINLINE void RemoveCur() { if (m_It != m_Container.end()) { m_It = m_Container.erase(m_It); } }
    DINLINE void Remove(TKey xKey) { if (m_It != m_Container.end() && m_It->first == xKey) { m_It = m_Container.erase(m_It); } else { m_Container.erase(xKey); } }
    DINLINE void Clear() { m_It = m_Container.end(); m_Container.clear(); }

    DINLINE Boolean MoveUp() { return --m_It != m_Container.end(); }
    DINLINE Boolean MoveNext() { return ++m_It != m_Container.end(); }

    DINLINE TValue& ValueDirectMoveUp() { --m_It; return Value(); }
    DINLINE void DirectMoveUp() { --m_It; }
    DINLINE void DirectMoveNext() { ++m_It; }
    DINLINE void SafeMoveUp() { if (m_Container.size() > 0 && m_It != m_Container.begin() && m_It != m_Container.end()) { --m_It; } }
    DINLINE void SafeMoveNext() { if (m_Container.size() > 0 && m_It != m_Container.end()) { ++m_It; } }

public:
    std::map<TKey, TValue> m_Container;
    typename std::map<TKey, TValue>::iterator m_It;
};













template<class TKey, class TValue>
class MsProtoPtrDictionary
{
public:
    MsProtoPtrDictionary()
    {
        ;
    }

    MsProtoPtrDictionary(google::protobuf::RepeatedPtrField<TKey>* xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>* xRepeatedValue)
    {
        this->LoadFromProto(xRepeatedKey, xRepeatedValue);
    }

    //MsProtoPtrDictionary(google::protobuf::RepeatedPtrField<TValue>* xRepeatedProto = nullptr)
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
            if (m_Container.size() == 0) { return False; }
            else if (m_is_new) { m_is_new = False; }
            else { m_cur++; }
            return m_cur != m_end;
        }

        DINLINE TKey GetKey() { return m_cur->first; }

        DINLINE TValue GetValue() { return m_cur->second; }

        DINLINE TKey Key() { return m_It->first; }

        DINLINE TValue& Value() { return m_cur->second; }

        DINLINE TValue SeekToBegin() { return m_cur = m_beg; }

    private:
        friend class MsProtoPtrDictionary <TKey, TValue>;
        typename std::map<TKey, TValue>::iterator m_beg;
        typename std::map<TKey, TValue>::iterator m_cur;
        typename std::map<TKey, TValue>::iterator m_end;
        Boolean m_is_new;
    };

public:
    DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TKey>* xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>* xRepeatedValue)
    {
        xRepeatedKey->Clear();
        xRepeatedValue->Clear();
        for (auto it = m_Container.begin(); it != m_Container.end(); ++it)
        {
            (*xRepeatedKey->Add()) = it->first;
            xRepeatedValue->Add()->CopyFrom(it->second);
        }
    }

    DINLINE void LoadFromProto(google::protobuf::RepeatedPtrField<TKey>* xRepeatedKey, google::protobuf::RepeatedPtrField<TValue>* xRepeatedValue)
    {
        m_Container.clear();
        for (int i = 0; i < xRepeatedValue->size(); i++)
        {
            const TKey& xKey = xRepeatedKey->Get(i);
            const TValue& xValue = xRepeatedValue->Get(i);
            m_Container[xKey] = xValue;
        }
    }

public:

    DINLINE Boolean ContainsKey(TKey key)
    {
        return m_Container.find(key) != m_Container.end();
    }

    DINLINE void Add(TKey key, TValue value) { m_Container.insert(std::pair<TKey, TValue>(key, value)); }

    DINLINE void Remove(TKey key) { m_Container.erase(key); }

    DINLINE void Clear() { m_Container.clear(); }

    DINLINE Int32 GetCount() { return (Int32)m_Container.size(); }

    DINLINE TValue& operator [](TKey key) { return m_Container[key]; }

    DINLINE TValue GetValue(TKey key)
    {
        std::map<TKey, TValue>::iterator it = m_Container.find(key);
        if (it != m_Container.end())
        {
            return it->second;
        }
        else { return NULL; }
    }

    DINLINE TKey GetLastKey() { return (--(m_Container.end()))->first; }
    DINLINE TValue GetLastValue() { return (--(m_Container.end()))->second; }

    DINLINE Enumerator GetEnumerator()
    {
        Enumerator xEnumerator;
        xEnumerator.m_beg = m_Container.begin();
        xEnumerator.m_cur = m_Container.begin();
        xEnumerator.m_end = m_Container.end();
        xEnumerator.m_is_new = True;
        return xEnumerator;
    }

private:
    std::map<TKey, TValue> m_Container;
};
