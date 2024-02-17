#pragma once

#include "functionqueue.h"

namespace FlexEngine
{

  /// <summary>
  /// Specialization of FunctionQueue
  /// <para>Queues functions for freeing anything that needs to be freed</para>
  /// </summary>
  class FreeQueue : public FunctionQueue
  {
  public:
    FreeQueue() = delete;
    virtual ~FreeQueue() = default;

    /// <summary>
    /// Push a function to the queue
    /// <para>Push a freeing function to the queue when the object is created</para>
    /// <para>Use lambda functions [&#38;]() { delete object; } to delete an object</para>
    /// <para>Functions will be run when Free() is called</para>
    /// </summary>
    /// <param name="function">Function to run</param>
    static void Push(const FunctionType& function) { m_queue.PushBack(function); }
    
    /// <summary>
    /// Runs all the freeing functions in the queue
    /// </summary>
    static void Free() { m_queue.RunFront(); }

  private:
    static FunctionQueue m_queue;
  };

}