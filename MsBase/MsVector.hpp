#pragma once

template<typename TValue>
class MsVector
{
public:
    MsVector() {}
    MsVector(std::list<TValue>& xList) { m_Container = xList; m_It = m_Container.begin(); }
    MsVector(std::vector<TValue>& xList) { m_Container = xList; m_It = m_Container.begin(); }
    MsVector(const MsList<TValue>& xMsList) { m_Container = xMsList.m_Container; m_It = m_Container.begin(); }
    MsVector(const MsVector<TValue>& xMsList) { m_Container = xMsList.m_Container; m_It = m_Container.begin(); }
    void operator=(std::list<TValue>& xList) { m_Container = xList; m_It = m_Container.begin(); }
    void operator=(std::vector<TValue>& xList) { m_Container = xList; m_It = m_Container.begin(); }
    void operator=(MsList<TValue>& xMsList) { m_Container = xMsList.m_Container; m_It = m_Container.begin(); }
    void operator=(MsVector<TValue>& xMsList) { m_Container = xMsList.m_Container; m_It = m_Container.begin(); }
    DINLINE TValue& operator [](Int32 xIndex) { return m_Container[xIndex]; }

    DINLINE Int32 IndexOf(TValue value)
    {
        Int32 xIndex = 0;
        for (std::vector<TValue>::iterator it = m_Container.begin(); it != m_Container.end(); ++it, xIndex++) { if (*it == value) { return xIndex; } }
        return -1;
    }
    DINLINE Int32 GetCount() { return (Int32)m_Container.size(); }
    DINLINE Int64 GetBigCount() { return (Int64)m_Container.size(); }

    DINLINE auto GetFirst() { return m_Container.begin(); }
    DINLINE auto GetLast() { return m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); }
    DINLINE TValue GetValue() { return *m_It; }
    DINLINE TValue& Value() { return *m_It; }
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

    DINLINE void Add(TValue value) { this->AddToLast(value); }
    DINLINE void AddToLast(TValue value)
    {
        m_Container.emplace_back(value);
        m_It = m_Container.begin();
    }
    DINLINE void AddToFirst(TValue value)
    {
        m_Container.emplace_front(value);
        m_It = m_Container.begin();
    }

    DINLINE void RemoveCur() { if (m_Container.size() == 0) { return; } if (m_It != m_Container.end()) { m_It = m_Container.erase(m_It); } }
    DINLINE void Remove(TValue value)
    {
        if (m_Container.size() == 0) { return; }
        if (m_It != m_Container.end() && *m_It == value) { m_It = m_Container.erase(m_It); }
        else { m_Container.erase(value); }
    }

    DINLINE TValue RemoveGetFirst() { TValue xFirst = *(m_Container.begin()); RemoveAtFirst(); return xFirst; }
    DINLINE void RemoveAtFirst()
    {
        if (m_It == m_Container.begin()) { m_It = m_Container.erase(m_It); }
        else { m_Container.erase(m_Container.begin()); }
    }
    DINLINE TValue RemoveGetLast() { TValue xLast = *(m_Container.begin()); RemoveAtLast(); return xLast; }
    DINLINE void RemoveAtLast()
    {
        auto it = m_Container.end(); it--;
        if (m_It == it) { m_It = m_Container.erase(m_It); }
        else { m_Container.erase(it); }
    }
    DINLINE void RemoveAt(Int32 xIndex)
    {
        auto xIt = m_Container.begin();
        for (Int32 i = 0; i < xIndex; i++) { ++xIt; }
        m_Container.erase(xIt);
    }

    DINLINE void Clear() { m_It = m_Container.end(); m_Container.clear(); }

    DINLINE Boolean MoveUp() { return --m_It != m_Container.end(); }
    DINLINE Boolean MoveNext() { return ++m_It != m_Container.end(); }

    DINLINE TValue& ValueDirectMoveUp() { --m_It; return Value(); }
    DINLINE void DirectMoveUp() { --m_It; }
    DINLINE void DirectMoveNext() { ++m_It; }
    DINLINE void SafeMoveUp() { if (m_Container.size() > 0 && m_It != m_Container.begin() && m_It != m_Container.end()) { --m_It; } }
    DINLINE void SafeMoveNext() { if (m_Container.size() > 0 && m_It != m_Container.end()) { ++m_It; } }
public:
    std::vector<TValue> m_Container;
    typename std::vector<TValue>::iterator m_It;
};
