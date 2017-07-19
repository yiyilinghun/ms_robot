#pragma once

template<typename TValue>
class MsQueue
{
public:
    // 进队
    DINLINE void Enqueue(TValue value)
    {
        m_Container.push(value);
    }

    // 出队
    DINLINE TValue& Dequeue()
    {
        TValue& xTValue = m_Container.front();
        m_Container.pop();
        return xTValue;
    }

    DINLINE void Clear()
    {
        while (!m_Container.empty()) { m_Container.pop(); }
    }

    DINLINE Boolean Empty() { return m_Container.empty(); }

    DINLINE Int32 GetCount()
    {
        return (Int32)m_Container.size();
    }

    DINLINE Int64 GetBigCount()
    {
        return (Int64)m_Container.size();
    }

public:
    std::queue<TValue> m_Container;
};
