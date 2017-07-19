#pragma once

template<typename TKVContainer, typename TVKContainer, typename TKey, typename TValue>
class _MsBlockDict
{
public:
    DINLINE _MsBlockDict(Int64 xBlockSize = 10240, Int64 xBlockCount = 200, Boolean IsIncreaseContainer = False)
        : m_PrimitiveBlockSize(xBlockSize)
        , m_PrimitiveBlockCount(xBlockCount)
        , m_IsIncreaseContainer(IsIncreaseContainer)
    {
        this->Clear();
    }

    DINLINE void Clear()
    {
        m_CurrentBlockSize = m_PrimitiveBlockSize;
        m_CurrentBlockCount = m_PrimitiveBlockCount;
        m_KVContainer.clear();
        m_VKContainer.clear();
        for (Int32 i = 0; i < m_CurrentBlockCount; i++)
        {
            m_KVContainer.push_back(TKVContainer());
        }
    }

    DINLINE Boolean Find(TKey xKey)
    {
        for (Int64 i = 0; i < m_KVContainer.size(); i++)
        {
            if (m_KVContainer[i].size() > 0)
            {
                auto it = m_KVContainer[i].end();
                --it;
                if (it->first == xKey)
                {
                    return True;
                }
                else if (it->first > xKey)
                {
                    return m_KVContainer[i].find(xKey) != m_KVContainer[i].end();
                }
            }
        }
        return False;
    }

    //DINLINE TValue& operator [](TKey xKey)
    //{
    //    TValue* xRet = nullptr;
    //    for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
    //    {
    //        if (i + 1 < (Int64)m_KVContainer.size() && (Int64)m_KVContainer[i].size() < m_CurrentBlockSize && m_KVContainer[i + 1].empty())
    //        {
    //            return m_KVContainer[i][xKey];
    //        }
    //        else if (i == (Int64)m_KVContainer.size() - 1 && (Int64)m_KVContainer[i].size() < m_CurrentBlockSize)
    //        {
    //            xRet = &(m_KVContainer[i][xKey]);
    //            if ((Int64)m_KVContainer[i].size() == m_CurrentBlockSize)
    //            {
    //                if (m_IsIncreaseContainer)
    //                {
    //                    m_KVContainer.push_back(std::map<TKey, TValue>());
    //                    m_CurrentBlockCount++;
    //                }
    //                else
    //                {
    //                    m_CurrentBlockSize *= 2;
    //                    for (Int64 j = 0; j < (Int64)m_KVContainer.size() - 1; j++)
    //                    {
    //                        for (Int64 x = j + 1; x < (Int64)m_KVContainer.size(); x++)
    //                        {
    //                            auto& xNextMap = m_KVContainer[x];
    //                            auto xMoveIt = xNextMap.begin();
    //                            while (xMoveIt != xNextMap.end())
    //                            {
    //                                m_KVContainer[j].insert(std::pair<TKey, TValue>(xMoveIt->first, xMoveIt->second));
    //                                xMoveIt = xNextMap.erase(xMoveIt);

    //                                if (m_KVContainer[j].size() == m_CurrentBlockSize)
    //                                {
    //                                    goto NEXT_MAP;
    //                                }
    //                            }
    //                        }
    //                    NEXT_MAP:
    //                        continue;
    //                    }
    //                }
    //            }
    //            return *xRet;
    //        }
    //        else
    //        {
    //            auto it2 = m_KVContainer[i].end();
    //            --it2;
    //            if (it2->first > xKey)
    //            {
    //                xRet = (&m_KVContainer[i][xKey]);

    //                for (Int64 j = i; j < (Int64)m_KVContainer.size() - 1; j++)
    //                {
    //                    if ((Int64)m_KVContainer[j].size() > m_CurrentBlockSize)
    //                    {
    //                        auto xLast = m_KVContainer[j].end();
    //                        --xLast;
    //                        m_KVContainer[j + 1].insert(std::pair<TKey, TValue>(xLast->first, xLast->second));
    //                        m_KVContainer[j].erase(xLast);
    //                    }
    //                    else
    //                    {
    //                        break;
    //                    }
    //                }
    //                return *xRet;
    //            }
    //        }
    //    }
    //    return *xRet;
    //}

    DINLINE Int64 FindRankAscending(TKey xKey)
    {
        return FindIndexAscending(xKey) + 1;
    }
    DINLINE Int64 FindIndexAscending(TKey xKey)
    {
        Int64 xIndex = 0;
        for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
        {
            if (m_KVContainer[i].size() > 0)
            {
                auto it = m_KVContainer[i].end();
                --it;
                if (it->first == xKey)
                {
                    xIndex += m_KVContainer[i].size() - 1;
                    return xIndex;
                }
                else if (it->first > xKey)
                {
                    if (m_KVContainer[i].find(xKey) != m_KVContainer[i].end())
                    {
                        for each (auto xPair in m_KVContainer[i])
                        {
                            if (xPair.first == xKey)
                            {
                                return xIndex;
                            }
                            xIndex++;
                        }
                    }
                }
                else
                {
                    xIndex += m_KVContainer[i].size();
                }
            }
        }
        return INVALID_QID;
    }

    DINLINE Int64 FindRankDescending(TKey xKey) { return this->Count() - this->FindRankAscending(xKey); }
    DINLINE Int64 FindIndexDescending(TKey xKey) { return this->Count() - this->FindIndexAscending(xKey); }

    DINLINE Int64 Count()
    {
        Int64 xCount = 0;
        for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
        {
            Int64 xTemp = m_KVContainer[i].size();
            if (xTemp == 0) { return xCount; }
            xCount += xTemp;
        }
        return xCount;
    }

    DINLINE void Arrangement()
    {
        if (m_KVContainer[m_KVContainer.size() - 1].size() == m_CurrentBlockSize)
        {
            if (m_IsIncreaseContainer)
            {
                m_KVContainer.push_back(TKVContainer());
                m_CurrentBlockCount++;
            }
            else
            {
                m_CurrentBlockSize *= 2;
                for (Int64 j = 0; j < (Int64)m_KVContainer.size() - 1; j++)
                {
                    for (Int64 x = j + 1; x < (Int64)m_KVContainer.size(); x++)
                    {
                        auto& xNextMap = m_KVContainer[x];
                        auto xMoveIt = xNextMap.begin();
                        while (xMoveIt != xNextMap.end())
                        {
                            m_KVContainer[j].insert(std::pair<TKey, TValue>(xMoveIt->first, xMoveIt->second));
                            xMoveIt = xNextMap.erase(xMoveIt);

                            if (m_KVContainer[j].size() == m_CurrentBlockSize)
                            {
                                goto NEXT_MAP;
                            }
                        }
                    }
                NEXT_MAP:
                    continue;
                }
            }
        }
    }

    DINLINE void Add(TKey xKey, TValue xValue) { return Add(std::pair<TKey, TValue>(xKey, xValue)); }
    DINLINE void Add(std::pair<TKey, TValue> xPair)
    {
        for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
        {
            if (i + 1 < (Int64)m_KVContainer.size() && (Int64)m_KVContainer[i].size() < m_CurrentBlockSize && m_KVContainer[i + 1].empty())
            {
                m_KVContainer[i].insert(xPair);
                m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                return;
            }
            else if (m_KVContainer[i].size() > 0)
            {
                auto it2 = m_KVContainer[i].end();
                --it2;
                if (it2->first > xPair.first)
                {
                    m_KVContainer[i].insert(xPair);
                    m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));

                    for (Int64 j = i; j < (Int64)m_KVContainer.size() - 1; j++)
                    {
                        if ((Int64)m_KVContainer[j].size() > m_CurrentBlockSize)
                        {
                            auto xLast = m_KVContainer[j].end();
                            --xLast;
                            m_KVContainer[j + 1].insert(std::pair<TKey, TValue>(xLast->first, xLast->second));
                            m_KVContainer[j].erase(xLast);
                        }
                        else
                        {
                            break;
                        }
                    }
                    return this->Arrangement();
                }
                else if (i == m_KVContainer.size() - 1)
                {
                    m_KVContainer[i].insert(xPair);
                    m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                    return this->Arrangement();
                }
            }
            else
            {
                m_KVContainer[i].insert(xPair);
                m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                return this->Arrangement();
            }
        }
    }

    DINLINE Int64 AddByIndexAscending(TKey xKey, TValue xValue) { return AddByIndexAscending(std::pair<TKey, TValue>(xKey, xValue)); }
    DINLINE Int64 AddByIndexAscending(std::pair<TKey, TValue> xPair)
    {
        Int64 xIndexDescendingInMap = INVALID_LID;
        Int64 xMapIndex = INVALID_LID;
        this->AddByIndexAscending(xPair, xMapIndex);
        if (xMapIndex != INVALID_LID)
        {
            auto it = m_KVContainer[xMapIndex].find(xPair.first);
            if (it != m_KVContainer[xMapIndex].end())
            {
                xIndexDescendingInMap = distance(m_KVContainer[xMapIndex].begin(), it);
            }
            for (Int32 i = 0; i < xMapIndex; i++)
            {
                xIndexDescendingInMap += m_KVContainer[i].size();
            }
        }
        return xIndexDescendingInMap;
    }
    DINLINE void AddByIndexAscending(std::pair<TKey, TValue> xPair, Int64& xMapIndex)
    {
        for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
        {
            if (i + 1 < (Int64)m_KVContainer.size() && (Int64)m_KVContainer[i].size() < m_CurrentBlockSize && m_KVContainer[i + 1].empty())
            {
                xMapIndex = i;
                m_KVContainer[i].insert(xPair);
                m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                return this->Arrangement();
            }
            else if (m_KVContainer[i].size() > 0)
            {
                auto it2 = m_KVContainer[i].end();
                --it2;
                if (it2->first > xPair.first)
                {
                    xMapIndex = i;
                    m_KVContainer[i].insert(xPair);
                    m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));

                    for (Int64 j = i; j < (Int64)m_KVContainer.size() - 1; j++)
                    {
                        if ((Int64)m_KVContainer[j].size() > m_CurrentBlockSize)
                        {
                            auto xLast = m_KVContainer[j].end();
                            --xLast;
                            m_KVContainer[j + 1].insert(std::pair<TKey, TValue>(xLast->first, xLast->second));
                            m_KVContainer[j].erase(xLast);
                        }
                        else
                        {
                            break;
                        }
                    }
                    return this->Arrangement();
                }
                else if (i == m_KVContainer.size() - 1)
                {
                    m_KVContainer[i].insert(xPair);
                    m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                    return this->Arrangement();
                }
            }
            else
            {
                m_KVContainer[i].insert(xPair);
                m_VKContainer.insert(std::pair<TValue, TKey>(xPair.second, xPair.first));
                return this->Arrangement();
            }
        }
        return this->Arrangement();
    }

    DINLINE Int64 AddByIndexDescending(TKey xKey, TValue xValue) { return this->Count() - this->AddByIndexAscending(std::pair<TKey, TValue>(xKey, xValue)); }
    DINLINE Int64 AddByIndexDescending(std::pair<TKey, TValue> xPair) { return this->Count() - this->AddByIndexAscending(xPair); }

    DINLINE void Remove(TKey xKey)
    {
        for (Int64 i = 0; i < (Int64)m_KVContainer.size(); i++)
        {
            if (m_KVContainer[i].size() > 0)
            {
                auto it = m_KVContainer[i].end();
                --it;
                if (it->first == xKey)
                {
                    m_VKContainer.erase(it->second);
                    m_KVContainer[i].erase(it);
                    return;
                }
                else if (it->first > xKey)
                {
                    it = m_KVContainer[i].find(xKey);
                    if (it != m_KVContainer[i].end())
                    {
                        m_VKContainer.erase(it->second);
                    }
                    m_KVContainer[i].erase(xKey);
                    return;
                }
            }
        }
    }

    template<typename TKVContainer>
    DINLINE Boolean MsGet(TKVContainer& xOut, Int64 xPos, Int64 xCount)
    {
        Int32 xMapIndex = INVALID_NID;
        for (Int64 i = 0; i < m_KVContainer.size(); i++)
        {
            Int64 xCurMapSize = m_KVContainer[i].size();
            if (xCurMapSize <= xPos)
            {
                xPos -= xCurMapSize;
            }
            else
            {
                xMapIndex = i;
                break;
            }
        }

        if (xMapIndex > INVALID_NID)
        {
            auto it = m_KVContainer[xMapIndex].begin();
            for (Int64 i = 0; i < xPos; ++it, i++)
            {
                ;
            }
            for (; xCount > 0 && it != m_KVContainer[xMapIndex].end(); ++it)
            {
                xOut.Add(*it);
                if (--xCount == 0)
                {
                    return True;
                }
            }

            for (Int64 i = xMapIndex + 1; i < m_KVContainer.size(); i++)
            {
                auto it = m_KVContainer[i].begin();
                for (; xCount > 0 && it != m_KVContainer[i].end(); ++it)
                {
                    xOut.Add(*it);
                    if (--xCount == 0)
                    {
                        return True;
                    }
                }
            }
        }
        return False;
    }

    template<typename TKVContainer>
    DINLINE Boolean StlGet(TKVContainer& xOut, Int64 xPos, Int64 xCount)
    {
        Int64 xMapIndex = INVALID_NID;
        for (Int64 i = 0; i < m_KVContainer.size(); i++)
        {
            Int64 xCurMapSize = m_KVContainer[i].size();
            if (xCurMapSize <= xPos)
            {
                xPos -= xCurMapSize;
            }
            else
            {
                xMapIndex = i;
                break;
            }
        }

        if (xMapIndex > INVALID_NID)
        {
            auto it = m_KVContainer[xMapIndex].begin();

            for (Int64 i = 0; i < xPos; ++it, i++);

            for (; xCount > 0 && it != m_KVContainer[xMapIndex].end(); ++it)
            {
                xOut.push_back(*it);
                if (--xCount == 0)
                {
                    return True;
                }
            }

            for (Int64 i = xMapIndex + 1; i < m_KVContainer.size(); i++)
            {
                auto it = m_KVContainer[i].begin();
                for (; xCount > 0 && it != m_KVContainer[i].end(); ++it)
                {
                    xOut.push_back(*it);
                    if (--xCount == 0)
                    {
                        return True;
                    }
                }
            }
        }
        return False;
    }

    std::vector<TKVContainer> m_KVContainer;
    TVKContainer m_VKContainer;
    Int64 m_CurrentBlockSize;
    Int64 m_CurrentBlockCount;

    Int64 m_PrimitiveBlockSize;
    Int64 m_PrimitiveBlockCount;
    Boolean m_IsIncreaseContainer;
};

template<typename TKey, typename TValue>
class MsBlockDict : public _MsBlockDict<std::map<TKey, TValue>, std::map<TValue, TKey>, TKey, TValue>
{
public:
    MsBlockDict(Int64 xBlockSize = 10240, Int64 xBlockCount = 200, Boolean IsIncreaseContainer = False)
        : _MsBlockDict(xBlockSize, xBlockCount, IsIncreaseContainer)
    {
        ;
    }

};

template<typename TKey, typename TValue>
class MsBlockMulDict : public _MsBlockDict<std::multimap<TKey, TValue>, std::multimap<TValue, TKey>, TKey, TValue>
{
public:
    MsBlockMulDict(Int64 xBlockSize = 10240, Int64 xBlockCount = 200, Boolean IsIncreaseContainer = False)
        : _MsBlockDict(xBlockSize, xBlockCount, IsIncreaseContainer)
    {
        ;
    }

};
