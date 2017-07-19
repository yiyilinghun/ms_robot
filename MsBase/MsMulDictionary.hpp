#pragma once

template<typename TKey, typename TValue>
class MsMulDictionary
{
public:
    MsMulDictionary() { }

    DINLINE Boolean ContainsKey(TKey key) { return m_Container.find(key) != m_Container.end(); }

    DINLINE auto GetFirst() { return m_Container.begin(); }
    DINLINE auto GetLast() { return m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); }
    DINLINE TKey GetKey() { return m_It->first; }

    DINLINE auto GetIt(TKey key, TValue value)
    {
        for (auto xPair = m_Container.equal_range(key); xPair.first != m_Container.end() && xPair.first != xPair.second; ++(xPair.first))
        {
            if (xPair.first->second == value)
            {
                return xPair.first;
            }
        }
        return m_Container.end();
    }
    DINLINE void Remove(TKey key)
    {
        if (m_Container.size() == 0) { return; }
        if (m_It != m_Container.end() && m_It->first == key) { m_It = m_Container.erase(m_It); }
        else { m_Container.erase(key); }
    }
    DINLINE void Remove(TKey key, TValue value)
    {
        if (m_Container.size() == 0) { return; }
        if (m_It != m_Container.end() && m_It->first == key && m_It->second == value) { m_It = m_Container.erase(m_It); }
        else { auto it = this->GetIt(key, value); if (it != m_Container.end()) { m_Container.erase(it); } }
    }
    DINLINE std::pair<std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const TKey, TValue> > > >,
        std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const TKey, TValue> > > >> GetValues(TKey key)
    {
        return m_Container.equal_range(key);
    }

    DINLINE TKey Key() { return m_It->first; }
    DINLINE TValue& Value() { return m_It->second; }
    DINLINE TValue& ValueToFirst() { this->ToFirst(); return Value(); }

    DINLINE auto& ToFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ForFirst() { m_It = m_Container.begin(); return (*this); }
    DINLINE auto& ToLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ForLast() { m_It = m_Container.size() > 0 ? m_Container.end() - 1 : m_Container.end(); return (*this); }
    DINLINE auto& ToEnd() { m_It = m_Container.end(); return (*this); }

    DINLINE Int32 GetCount() { return (Int32)m_Container.size(); }
    DINLINE Int64 GetBigCount() { return (Int64)m_Container.size(); }

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
    DINLINE void Clear() { m_It = m_Container.end(); m_Container.clear(); }

    DINLINE Boolean MoveUp() { return --m_It != m_Container.end(); }
    DINLINE Boolean MoveNext() { return ++m_It != m_Container.end(); }

    DINLINE TValue& ValueDirectMoveUp() { --m_It; return Value(); }
    DINLINE void DirectMoveUp() { --m_It; }
    DINLINE void DirectMoveNext() { ++m_It; }
    DINLINE void SafeMoveUp() { if (m_Container.size() > 0 && m_It != m_Container.begin() && m_It != m_Container.end()) { --m_It; } }
    DINLINE void SafeMoveNext() { if (m_Container.size() > 0 && m_It != m_Container.end()) { ++m_It; } }

public:
    std::multimap<TKey, TValue> m_Container;
    typename std::multimap<TKey, TValue>::iterator m_It;
};

