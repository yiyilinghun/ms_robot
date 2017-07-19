#pragma once
//#define BLOCK_SIZE 0xFFFFFFFFFFFFFFFF
#define BLOCK_SIZE 0xFFFF

template<typename TKey, typename TValue>
struct MsUnitBucket
{
    DINLINE void Clear()
    {
        return m_Container.clear();
    }

    TKey IndexOf(TKey xKey)
    {
        auto it = m_Container.find(xKey);
        while (it != m_Container.end())
        {
            return distance(m_Container.begin(), it);
        }
        return INVALID_LID;
    }
    std::multimap<TKey, TValue> m_Container;
};

template<typename TKey, typename TValue>
struct MsBucketLayer2
{
    DINLINE void Clear()
    {
        return m_Container.clear();
    }

    DINLINE Boolean Find(TKey xKey, TKey xSubKey)
    {
        return m_Container[xSubKey].m_Container.find(xKey) != m_Container[xSubKey].m_Container.end();
    }

    DINLINE Boolean Add(TKey xKey, TKey xSubKey, TValue xValue)
    {
        if (m_Container[xSubKey].m_Container.find(xKey) != m_Container[xSubKey].m_Container.end())
        {
            m_Container[xSubKey].m_Container.insert(std::pair<TKey, TValue>(xKey, xValue));
            m_Count++;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE Boolean Remove(TKey xKey, TKey xSubKey)
    {
        if (m_Container[xSubKey].m_Container.find(xKey) != m_Container[xSubKey].m_Container.end())
        {
            m_Container[xSubKey].m_Container.erase(xKey);
            m_Count--;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE QWORD Count()
    {
        return m_Count;
        //QWORD xCount = 0;
        //for (QWORD i = 0; i < m_Container.size(); i++)
        //{
        //    xCount += m_Container[i].m_Container.size();
        //}
        //return xCount;
    }

    TKey IndexOf(TKey xKey, TKey xSubKey)
    {
        TKey xIndex = m_Container[xSubKey].IndexOf(xKey);
        if (xIndex != INVALID_LID)
        {
            for (TKey i = 0; i < xSubKey; i++)
            {
                xIndex += m_Container[i].m_Container.size();
            }
        }
        return xIndex;
    }

    TKey m_Count = 0;
    std::map<TKey, MsUnitBucket<TKey, TValue>> m_Container;
};

template<typename TKey, typename TValue>
struct MsLayerBucket3
{
    DINLINE void Clear()
    {
        return m_Container.clear();
    }

    DINLINE Boolean Find(TKey xKey, TKey xSubKey)
    {
        return m_Container[xSubKey >> 16].Find(xKey, xSubKey & 0x000000000000FFFF);
    }

    DINLINE Boolean Add(TKey xKey, TKey xSubKey, TValue xValue)
    {
        if (m_Container[xSubKey >> 16].Add(xKey, xSubKey & 0x000000000000FFFF, xValue))
        {
            m_Count++;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE Boolean Remove(TKey xKey, TKey xSubKey)
    {
        if (m_Container[xSubKey].Remove(xKey, xSubKey / BLOCK_SIZE))
        {
            m_Count--;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE QWORD Count()
    {
        return m_Count;
    }

    TKey IndexOf(TKey xKey, TKey xSubKey)
    {
        TKey xTempIndex = xSubKey >> 16;
        TKey xIndex = m_Container[xTempIndex].IndexOf(xKey, xSubKey & 0x000000000000FFFF);
        if (xIndex != INVALID_LID)
        {
            for (TKey i = 0; i < xTempIndex; i++)
            {
                xIndex += m_Container[i].Count();
            }
        }
        return xIndex;
    }

    TKey m_Count = 0;
    std::map<TKey, MsBucketLayer2<TKey, TValue>> m_Container;
};

template<typename TKey, typename TValue>
struct MsBucketLayer4
{
    DINLINE void Clear()
    {
        return m_Container.clear();
    }

    DINLINE Boolean Find(TKey xKey, TKey xSubKey)
    {
        return m_Container[xSubKey >> 32].Find(xKey, xSubKey & 0x00000000FFFFFFFF);
    }

    DINLINE Boolean Add(TKey xKey, TKey xSubKey, TValue xValue)
    {
        if (m_Container[xSubKey >> 32].Add(xKey, xSubKey & 0x00000000FFFFFFFF, xValue))
        {
            m_Count++;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE Boolean Remove(TKey xKey, TKey xSubKey)
    {
        if (m_Container[xSubKey >> 32].Remove(xKey, xSubKey & 0x00000000FFFFFFFF))
        {
            m_Count--;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE QWORD Count()
    {
        QWORD xCount = 0;
        for (QWORD i = 0; i < m_Container.size(); i++)
        {
            xCount += m_Container[i].Count();
        }
        return xCount;
    }

    TKey IndexOf(TKey xKey, TKey xSubKey)
    {
        TKey xTempIndex = xSubKey >> 32;
        TKey xIndex = m_Container[xTempIndex].IndexOf(xKey, xSubKey & 0x00000000FFFFFFFF);
        if (xIndex != INVALID_LID)
        {
            for (TKey i = 0; i < xTempIndex; i++)
            {
                xIndex += m_Container[i].Count();
            }
        }
        return xIndex;
    }

    TKey m_Count = 0;
    std::map<TKey, MsLayerBucket3<TKey, TValue>> m_Container;
};

template<typename TKey, typename TValue>
struct MsOuterBucketLayer
{
    DINLINE void Clear()
    {
        return m_Container.clear();
    }

    DINLINE Boolean Find(TKey xKey)
    {
        return m_Container[xKey >> 48].Find(xKey, xKey & 0x0000FFFFFFFFFFFF);
    }

    DINLINE Boolean Add(TKey xKey, TValue xValue)
    {
        if (m_Container[xKey >> 48].Add(xKey, xKey & 0x0000FFFFFFFFFFFF, xValue))
        {
            m_Count++;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE Boolean Remove(TKey xKey)
    {
        if (m_Container[xKey >> 48].Remove(xKey, xKey & 0x0000FFFFFFFFFFFF))
        {
            m_Count--;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE QWORD Count()
    {
        return m_Count;
    }

    TKey IndexOf(TKey xKey)
    {
        TKey xTempIndex = xKey >> 48;
        TKey xIndex = m_Container[xTempIndex].IndexOf(xKey, xKey & 0x0000FFFFFFFFFFFF);
        if (xIndex != INVALID_LID)
        {
            for (TKey i = 0; i < xTempIndex; i++)
            {
                xIndex += m_Container[i].Count();
            }
        }
        return xIndex;
    }
    TKey m_Count = 0;
    std::map<TKey, MsBucketLayer4<TKey, TValue>> m_Container;
};

template<typename TKey, typename TValue>
class MsBlockTestDict
{
public:
    DINLINE void Clear()
    {
        return m_Container.Clear();
    }

    DINLINE Boolean Find(TKey xKey)
    {
        return m_Container.Find(xKey);
    }

    DINLINE Boolean Add(TKey xKey, TValue xValue)
    {
        if (m_Container.Add(xKey, xValue))
        {
            m_Count++;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE Boolean Remove(TKey xKey)
    {
        if (m_Container.Remove(xKey))
        {
            m_Count--;
            return True;
        }
        else
        {
            return False;
        }
    }

    DINLINE QWORD Count()
    {
        return m_Container.Count();
    }

    TKey IndexOf(TKey xKey)
    {
        return m_Container.IndexOf(xKey);
    }

    TKey m_Count = 0;
    MsOuterBucketLayer<TKey, TValue> m_Container;
};
