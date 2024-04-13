#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace FlexEngine
{

  struct FunctorQueueData
  {
    std::function<void()> m_function = nullptr;
    int m_priority = 0;

    FunctorQueueData(std::function<void()> function, int priority = 0)
      : m_function(function), m_priority(priority)
    {
    }

    // setter

    void SetFunction(std::function<void()> function) { m_function = function; }
    void SetPriority(int p) { m_priority = p; }

    // getter

    //std::function<void()> GetFunction() const { return m_function; }
    int GetPriority() const { return m_priority; }

    void Run() const { m_function(); }

    // make it a functor
    void operator()() const { Run(); }

    // overload < operator for sorting
    bool operator<(const FunctorQueueData& rhs) const { return m_priority < rhs.m_priority; }
  };

  class FunctionQueue
  {
    static std::vector<FunctorQueueData> m_queue;

    static void Sort() { std::sort(m_queue.begin(), m_queue.end()); }

  public:

    // passthrough

    FunctorQueueData& operator[](size_t index) { return m_queue[index]; }
    const FunctorQueueData& operator[](size_t index) const { return m_queue[index]; }

    FunctorQueueData& at(size_t index) { return m_queue.at(index); }
    const FunctorQueueData& at(size_t index) const { return m_queue.at(index); }

    size_t size() const { return m_queue.size(); }
    bool empty() const { return m_queue.empty(); }

    std::vector<FunctorQueueData>::iterator begin() { return m_queue.begin(); }
    std::vector<FunctorQueueData>::iterator end() { return m_queue.end(); }
    std::vector<FunctorQueueData>::reverse_iterator rbegin() { return m_queue.rbegin(); }
    std::vector<FunctorQueueData>::reverse_iterator rend() { return m_queue.rend(); }
                
    std::vector<FunctorQueueData>::const_iterator begin() const { return m_queue.begin(); }
    std::vector<FunctorQueueData>::const_iterator end() const { return m_queue.end(); }
    std::vector<FunctorQueueData>::const_reverse_iterator rbegin() const { return m_queue.rbegin(); }
    std::vector<FunctorQueueData>::const_reverse_iterator rend() const { return m_queue.rend(); }
                
    std::vector<FunctorQueueData>::const_iterator cbegin() const { return m_queue.cbegin(); }
    std::vector<FunctorQueueData>::const_iterator cend()	const { return m_queue.cend(); }
    std::vector<FunctorQueueData>::const_reverse_iterator crbegin() const { return m_queue.crbegin(); }
    std::vector<FunctorQueueData>::const_reverse_iterator crend() const { return m_queue.crend(); }
    
    // Add a function to the queue
    // Lowest priority will be run first
    // Bind Usage: FunctionQueue::Insert({ std::bind(&GameObject::Draw, gameobject), gameobject.GetZIndex() });
    // Lambda Usage: FunctionQueue::Insert({ [&gameobject]() { gameobject.Draw(); }, 0 });
    void Insert(FunctorQueueData data);

    // Run all the functions in the queue
    // Functions will be run in order of priority from lowest to highest
    // Functions with the same priority will be run in the order they were added
    void Flush();

#ifdef _DEBUG
    // For debugging
    // Logs all the function addresses in the queue
    void Dump();
#endif

  };
}