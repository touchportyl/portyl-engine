#include "pch.h"

#include "fmodstudio.h"

namespace FlexEngine
{
  // static member initialization
  FMOD_RESULT FmodStudio::m_result;
  FmodStudio* FmodStudio::m_instance = nullptr;

  FmodStudio::FmodStudio()
  {
    FLX_FMOD_ASSERT(FMOD::Studio::System::create(&m_system));
    FLX_FMOD_ASSERT(m_system->getCoreSystem(&m_core_system));
    FLX_FMOD_ASSERT(m_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
  }

  FmodStudio::~FmodStudio()
  {
    for (auto& bank : m_banks)
    {
      FLX_FMOD_ASSERT(bank->unload());
    }
    FLX_FMOD_ASSERT(m_system->release());
  }

  FmodStudio* FmodStudio::GetInstance()
  {
    if (m_instance == nullptr)
    {
      m_instance = new FmodStudio();
    }
    return m_instance;
  }

  void FmodStudio::GetBanks(std::vector<std::string> paths)
  {
    for (auto& path : paths)
    {
      FMOD::Studio::Bank* bank;
      FLX_FMOD_ASSERT(m_system->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
      FLX_ASSERT(bank->isValid(), "Bank is not valid.");
      m_banks.push_back(bank);
    }
    m_master_bank = m_banks[0];
  }

  void FmodStudio::GetEventDescriptions()
  {
    FMOD::Studio::EventDescription* event_descriptions[256]; int num_events = 0;
    FLX_FMOD_ASSERT(m_master_bank->getEventList(event_descriptions, 256, &num_events));
    for (int i = 0; i < num_events && i < 256; i++)
    {
      m_event_descriptions.push_back(event_descriptions[i]);
    }
  }

}