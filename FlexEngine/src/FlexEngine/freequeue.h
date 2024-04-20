#pragma once

#include "functionqueue.h"

namespace FlexEngine
{

  // Specialization of FunctionQueue
  // Queues functions for freeing anything that needs to be freed
  class __FLX_API FreeQueue
  {
    static FunctionQueue m_queue;

  public:
    // static class
    FreeQueue() = delete;
    ~FreeQueue() = delete;
    FreeQueue(const FreeQueue&) = delete;
    FreeQueue(FreeQueue&&) = delete;
    FreeQueue& operator=(const FreeQueue&) = delete;
    FreeQueue& operator=(FreeQueue&&) = delete;

    // Push a function to the queue
    // Push a freeing function to the queue when the object is created
    // Use lambda functions [&]() { delete object; } to delete an object
    // Functions will be run when Free() is called
    static void Push(const std::function<void()>& function) { m_queue.Insert({ function, -static_cast<int>(m_queue.size()) }); }
    
    // Runs all the freeing functions in the queue
    static void Free() { m_queue.Flush(); }

#ifdef _DEBUG
    // passthrough
    static void Dump() { m_queue.Dump(); }
#endif
  };

}