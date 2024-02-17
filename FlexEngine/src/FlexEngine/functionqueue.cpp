#include "functionqueue.h"

namespace FlexEngine
{

  void FunctionQueue::RunFront()
  {
    while (!IsEmpty())
    {
      FunctionType fn = m_queue.front();
      fn();
      PopFront();
    }
  }
  void FunctionQueue::RunBack()
  {
    while (!IsEmpty())
    {
      FunctionType fn = m_queue.back();
      fn();
      PopBack();
    }
  }

  void FunctionQueue::Clear() {
    while (!IsEmpty())
    {
      PopBack();
    }
  }

}