#pragma once

#include <deque>
#include <functional>

using FunctionType = std::function<void()>;

namespace FlexEngine
{

  /// <summary>
  /// Queues functions for execution later
  /// <para>Functions will be run when Run() is called</para>
  /// </summary>
  class FunctionQueue
  {
  public:
    FunctionQueue() = default;
    virtual ~FunctionQueue() = default;

    // disable copy and move constructors
    FunctionQueue(const FunctionQueue&) = delete;
    FunctionQueue(FunctionQueue&&) = delete;
    FunctionQueue& operator=(const FunctionQueue&) = delete;
    FunctionQueue& operator=(FunctionQueue&&) = delete;

    /// <summary>
    /// Push a function to the queue
    /// <para>Function will be run when Run() is called</para>
    /// </summary>
    inline void PushFront(const FunctionType& function) { m_queue.push_front(function); }

    /// <summary>
    /// Push a function to the queue
    /// <para>Function will be run when Run() is called</para>
    /// </summary>
    inline void PushBack(const FunctionType& function) { m_queue.push_back(function); }

    /// <summary>
    /// Pop a function from the queue
    /// </summary>
    inline void PopFront() { m_queue.pop_front(); }

    /// <summary>
    /// Pop a function from the queue
    /// </summary>
    inline void PopBack() { m_queue.pop_back(); }

    /// <summary>
    /// Run all functions in the queue
    /// <para>Functions will be run in the order they were added</para>
    /// </summary>
    void RunFront();

    /// <summary>
    /// Run all functions in the queue
    /// <para>Functions will be run in the reverse order they were added</para>
    /// </summary>
    void RunBack();

    /// <summary>
    /// Clear the function queue
    /// </summary>
    void Clear();

    /// <summary>
    /// Check if the queue is empty
    /// </summary>
    inline bool IsEmpty() const { return m_queue.empty(); }

  private:
    std::deque<FunctionType> m_queue;
  };

}