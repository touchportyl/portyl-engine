#pragma once

#include "fmod_studio.hpp"
#include "fmod_errors.h"

// assert function wrapper for fmod
#define FLX_FMOD_ASSERT(FUNCTION)            \
FlexEngine::FmodStudio::m_result = FUNCTION; \
FLX_ASSERT(FlexEngine::FmodStudio::m_result == FMOD_OK, FMOD_ErrorString(FlexEngine::FmodStudio::m_result))

namespace FlexEngine
{

  /// <summary>
  /// Wrapper for FMOD::Studio::System
  /// <para>Handles initialization and release of FMOD Studio</para>
  /// </summary>
  class FmodStudio
  {
  public:
    ~FmodStudio();

    /// <returns>FmodStudio instance</returns>
    static FmodStudio* GetInstance();

    /// <summary>
    /// FMOD Studio update function
    /// <para>Call this function every frame</para>
    /// </summary>
    static void Update() { FLX_FMOD_ASSERT(FmodStudio::GetInstance()->GetSystem()->update()); }

    // first bank is always the master bank
    void GetBanks(std::vector<std::string> paths);
    void GetEventDescriptions();

    FMOD::Studio::System* GetSystem() const { return m_system; }
    FMOD::System* GetCoreSystem() const { return m_core_system; }

    std::vector<FMOD::Studio::EventDescription*> m_event_descriptions;

    static FMOD_RESULT m_result;

  protected:
    FMOD::Studio::System* m_system;
    FMOD::System* m_core_system;

    std::vector<FMOD::Studio::Bank*> m_banks;
    FMOD::Studio::Bank* m_master_bank = nullptr;

  private:
    FmodStudio();

    static FmodStudio* m_instance;
  };

  /// <summary>
  /// Wrapper for FMOD::Studio::EventInstance
  /// Handles creation and release of event instances
  /// </summary>
  struct FmodEventInstance
  {
    FmodEventInstance() = delete;

    /// <summary>
    /// Create an event instance from an event description
    /// </summary>
    FmodEventInstance(FMOD::Studio::EventDescription* event)
    {
      FLX_FMOD_ASSERT(event->createInstance(&m_event_instance));
    }

    ~FmodEventInstance()
    {
      FLX_FMOD_ASSERT(m_event_instance->release());
    }

    FMOD::Studio::EventInstance* m_event_instance;
  };

}