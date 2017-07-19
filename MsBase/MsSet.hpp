#pragma once

template<typename TKey>
class MsSet
{
public:
    MsSet() { }
    MsSet(google::protobuf::RepeatedField<TKey>* xRepeatedValue) { this->LoadFromProto(xRepeatedValue); m_It = m_Container.begin(); }
    MsSet(google::protobuf::RepeatedPtrField<TKey>* xRepeatedValue) { this->LoadFromProto(xRepeatedValue); m_It = m_Container.begin(); }

    DINLINE void SaveToProto(google::protobuf::RepeatedPtrField<TKey>* xRepeatedValue)
    {
        xRepeatedValue->Clear();
        for (auto it = m_Container.begin(); it != m_Container.end(); ++it)
        {
            xRepeatedValue->Add()->CopyFrom(*it);
        }
    }

    DINLINE void SaveToProto(google::protobuf::RepeatedField<TKey>* xRepeatedValue)
    {
        xRepeatedValue->Clear();
        for (auto it = m_Container.begin(); it != m_Container.end(); ++it)
        {
            xRepeatedValue->Add(*it);
        }
    }

    DINLINE void LoadFromProto(google::protobuf::RepeatedPtrField<TKey>* xRepeatedValue)
    {
        m_Container.clear();
        for (int i = 0; i < xRepeatedValue->size(); i++)
        {
            m_Container.insert(xRepeatedValue->Get(i));
        }
    }

    DINLINE void LoadFromProto(google::protobuf::RepeatedField<TKey>* xRepeatedValue)
    {
        m_Container.clear();
        for (int i = 0; i < xRepeatedValue->size(); i++)
        {
            m_Container.insert(xRepeatedValue->Get(i));
        }
    }

    DINLINE Boolean Contains(TKey key) { return m_Container.find(key) != m_Container.end(); }
    DINLINE Int32 GetCount() { return (Int32)m_Container.size(); }
    DINLINE Int64 GetBigCount() { return (Int64)m_Container.size(); }

    DINLINE auto GetFirst() { return m_Container.begin(); }
    DINLINE auto GetLast() { return m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); }
    DINLINE TKey GetKey() { return *m_It; }
    DINLINE TKey Key() { return *m_It; }
    DINLINE const TKey& Value() { return (*m_It); }
    DINLINE TKey& ValueToFirst() { this->ToFirst(); return Value(); }

    DINLINE auto& ToFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ForFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ToLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ForLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ToEnd() { m_It = m_Container.end(); return (*this); }

    DINLINE Boolean Empty() { return m_Container.empty(); }
    DINLINE Boolean IsValid() { return m_It != m_Container.end(); }
    DINLINE Boolean IsBegin() { return m_It == m_Container.begin(); }
    DINLINE Boolean IsEnd() { return m_It == m_Container.end(); }

    DINLINE void Add(TKey key)
    {
        if (m_Container.size() == 0)
        {
            m_Container.insert(key);
            m_It = m_Container.begin();
        }
        else
        {
            m_Container.insert(key);
        }
    }
    DINLINE void RemoveCur() { if (m_Container.size() == 0) { return; } if (m_It != m_Container.end()) { m_It = m_Container.erase(m_It); } }
    DINLINE void Remove(TKey xKey) { if (m_Container.size() == 0) { return; } if (m_It != m_Container.end() && (*m_It) == xKey) { m_It = m_Container.erase(m_It); } else { m_Container.erase(xKey); } }
    DINLINE void Clear() { m_It = m_Container.end(); m_Container.clear(); }

    DINLINE Boolean MoveUp() { return --m_It != m_Container.end(); }
    DINLINE Boolean MoveNext() { return ++m_It != m_Container.end(); }

    DINLINE TKey& ValueDirectMoveUp() { --m_It; return Value(); }
    DINLINE void DirectMoveUp() { --m_It; }
    DINLINE void DirectMoveNext() { ++m_It; }
    DINLINE void SafeMoveUp() { if (m_Container.size() > 0 && m_It != m_Container.begin() && m_It != m_Container.end()) { --m_It; } }
    DINLINE void SafeMoveNext() { if (m_Container.size() > 0 && m_It != m_Container.end()) { ++m_It; } }
public:
    std::set<TKey> m_Container;
    typename std::set<TKey>::iterator m_It;
};
