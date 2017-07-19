#pragma once

template<typename TKey, typename TValue>
class MsDictionary
{
public:
    MsDictionary() { }
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
    DINLINE TValue& ValueToFirst() { this->ToFirst(); return Value(); }

    DINLINE auto& ToFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ForFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ToLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ForLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ToEnd() { m_It = m_Container.end(); return (*this); }

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
    DINLINE void RemoveCur() { if (m_Container.size() == 0) { return; } if (m_It != m_Container.end()) { m_It = m_Container.erase(m_It); } }
    DINLINE void Remove(TKey xKey) { if (m_Container.size() == 0) { return; } if (m_It != m_Container.end() && m_It->first == xKey) { m_It = m_Container.erase(m_It); } else { m_Container.erase(xKey); } }
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

//
//template<typename TValue>
//struct Id_Index_Dict
//{
//    DINLINE Boolean ContainsId(Int32 xId)
//    {
//        return m_Dict_Id.ContainsKey(xId);
//    }
//
//    DINLINE Boolean ContainsIndex(Int64 xIndex)
//    {
//        return m_Dict_Index.ContainsKey(xIndex);
//    }
//
//    DINLINE Boolean ContainsKey(Int32 xId, Int64 xIndex)
//    {
//        return m_Dict_Id.ContainsKey(xId) && m_Dict_Index.ContainsKey(xIndex);
//    }
//
//    DINLINE Boolean Add(Int32 xId, Int64 xIndex, TValue value)
//    {
//        if (!this->ContainsKey(xId, xIndex))
//        {
//            m_Dict_Id.Add(xId, value);
//            m_Dict_Index.Add(xIndex, &(m_Dict_Id[xId]));
//            return True;
//        }
//        return False;
//    }
//
//    DINLINE Boolean Remove(Int32 xId, Int64 xIndex)
//    {
//        if (this->ContainsKey(xId, xIndex))
//        {
//            m_Dict_Id.Remove(xId);
//            m_Dict_Index.Remove(xIndex);
//            return True;
//        }
//        return False;
//    }
//
//    DINLINE void Clear() { m_Dict_Id.Clear(); m_Dict_Index.Clear(); }
//
//    DINLINE Int32 GetCount() { return (Int32)m_Dict_Id.size(); }
//
//    DINLINE TValue& operator [](Int32 xId) { return m_Dict_Id[xId]; }
//    DINLINE TValue& operator [](Int64 xIndex) { return *(m_Dict_Index[xIndex]); }
//
//    DINLINE TValue GetValue(Int32 xId) { if (this->ContainsId(xId)) { return m_Dict_Id[xId]; } else { return NULL; } }
//    DINLINE TValue GetValue(Int64 xIndex) { if (this->ContainsIndex(xIndex)) { return *(m_Dict_Index[xIndex]); } else { return NULL; } }
//
//    typename DINLINE MsDictionary<Int32, TValue>::Enumerator GetEnumerator()
//    {
//        return m_Dict_Id.GetEnumerator();
//    }
//
//public:
//    MsDictionary<Int32, TValue> m_Dict_Id;
//    MsDictionary<Int64, TValue*> m_Dict_Index;
//};
//
