#include "pch.h"

#include "functionqueue.h"

namespace FlexEngine
{

  void FunctionQueue::Insert(FunctionQueueData data)
  {
    // guard
    if (!data.m_function) return;

    m_queue.push_back(data);
  }

  void FunctionQueue::Flush()
  {
    // guard
    if (m_queue.empty()) return;

    // sort by priority
    Sort();
    for (auto& data : m_queue) data();
    m_queue.clear();
  }

#ifdef _DEBUG
  void FunctionQueue::Dump()
  {
    Log::Debug(__FUNCTION__);

    // guard
    if (m_queue.empty())
    {
      Log::Debug("Queue is empty.");
      return;
    }

    // sort by priority
    Sort();
    for (auto& data : m_queue)
    {
      // print the function address
      Log::Debug("[" + std::to_string(data.GetPriority()) + "] Function: " + data.m_function.target_type().name());
    }
    Log::Debug("End of dump.");
  }
#endif
}
