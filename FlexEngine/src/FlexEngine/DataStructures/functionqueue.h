#pragma once

#include "flx_api.h"

#include <vector>
#include <functional>
#include <algorithm>

namespace FlexEngine
{

  struct __FLX_API FunctionQueueData
  {
    std::function<void()> m_function = nullptr;
    int m_priority = 0;

    FunctionQueueData(std::function<void()> function, int priority = 0)
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
    bool operator<(const FunctionQueueData& rhs) const { return m_priority < rhs.m_priority; }
  };

  class __FLX_API FunctionQueue
  {
    std::vector<FunctionQueueData> m_queue;

  public:

    #pragma region Passthrough Functions

    FunctionQueueData& operator[](size_t index) { return m_queue[index]; }
    const FunctionQueueData& operator[](size_t index) const { return m_queue[index]; }

    FunctionQueueData& at(size_t index) { return m_queue.at(index); }
    const FunctionQueueData& at(size_t index) const { return m_queue.at(index); }

    size_t size() const { return m_queue.size(); }
    bool empty() const { return m_queue.empty(); }

    std::vector<FunctionQueueData>::iterator begin() { return m_queue.begin(); }
    std::vector<FunctionQueueData>::iterator end() { return m_queue.end(); }
    std::vector<FunctionQueueData>::reverse_iterator rbegin() { return m_queue.rbegin(); }
    std::vector<FunctionQueueData>::reverse_iterator rend() { return m_queue.rend(); }
                
    std::vector<FunctionQueueData>::const_iterator begin() const { return m_queue.begin(); }
    std::vector<FunctionQueueData>::const_iterator end() const { return m_queue.end(); }
    std::vector<FunctionQueueData>::const_reverse_iterator rbegin() const { return m_queue.rbegin(); }
    std::vector<FunctionQueueData>::const_reverse_iterator rend() const { return m_queue.rend(); }
                
    std::vector<FunctionQueueData>::const_iterator cbegin() const { return m_queue.cbegin(); }
    std::vector<FunctionQueueData>::const_iterator cend()	const { return m_queue.cend(); }
    std::vector<FunctionQueueData>::const_reverse_iterator crbegin() const { return m_queue.crbegin(); }
    std::vector<FunctionQueueData>::const_reverse_iterator crend() const { return m_queue.crend(); }

    #pragma endregion

    // Add a function to the back of the queue.
    // Bind Usage: FunctionQueue::Push({ std::bind(&GameObject::Draw, gameobject) });
    // Lambda Usage: FunctionQueue::Push({ [&gameobject]() { gameobject.Draw(); } });
    void Push(FunctionQueueData data) { m_queue.push_back(data); }
    
    // Add a function to the queue.
    // Lowest priority will be run first.
    // Bind Usage: FunctionQueue::Insert({ std::bind(&GameObject::Draw, gameobject), gameobject.GetZIndex() });
    // Lambda Usage: FunctionQueue::Insert({ [&gameobject]() { gameobject.Draw(); }, 0 });
    void Insert(FunctionQueueData data);

    // Run all the functions in the queue
    // Functions will be run in order of priority from lowest to highest
    // Functions with the same priority will be run in the order they were added
    void Flush();

  private:
    void Sort() { std::sort(m_queue.begin(), m_queue.end()); }

#ifdef _DEBUG
  public:
    // For debugging
    // Logs all the function addresses in the queue
    void Dump();
#endif

  };
}