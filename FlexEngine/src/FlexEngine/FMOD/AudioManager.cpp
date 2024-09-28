/** WLVerse
 * \file AudioManager.h
 *
 * \brief
 *   Audio wrapper to be used to play all audio in the game.
 *   This system uses FMOD studio as a basis for playing music
 *
 * \authors
 *   Yew Chong (yewchong.k\@digipen.edu)
 *
 * \link <https://portylstudios.notion.site/WLVerse-GAM150-Plan-d3460d6a24db417ca86f9ed54deaf365?pvs=4>
 *   WLVerse GAM150 Plan
 * \endlink
 *
 * \par All content (c) 2024 DigiPen Institute of Technology Singapore. All rights reserved.
 */

#include "AudioManager.h"
#include "fmod_errors.h"
#include <string>
#include <cassert>

namespace FlexEngine
{

// Most of the FMOD functions return an FMOD_RESULT, we want to watch for this with FMOD assert macro to crash fast for debugging
#define FMOD_ASSERT(FUNCTION) \
          AudioManager::result = FUNCTION;  \
          assert(AudioManager::result == FMOD_OK && FMOD_ErrorString(AudioManager::result))

// Static initialization
FMOD::System* AudioManager::fmod_system = NULL;
FMOD_RESULT AudioManager::result;
std::map<std::string, FMOD::Channel*> AudioManager::Core::channels;

/*!
  \brief Constructor for the AudioManager. Handles the creation of the FMOD system.
*/
void AudioManager::Load()
{
  FMOD_ASSERT(FMOD::System_Create(&fmod_system));
  FMOD_ASSERT(fmod_system->init(32, FMOD_INIT_NORMAL, 0));
}

/*!
  \brief Destructor for the AudioManager. Handles close and release of the FMOD system.
*/
void AudioManager::Unload()
{
  fmod_system->close();
  fmod_system->release();
}

/*!
  \brief Updates the FMOD instance
*/
void AudioManager::Update()
{
  fmod_system->update();
}

/*!
  \brief Plays the sound.
  \param identifier The identifier of the sound for controlling
  \param sound The sound to play
*/
void AudioManager::Core::PlaySound(std::string const& identifier, Asset::Sound const& asset)
{
  // Play the sound given a sound handle and a channel...
  if (channels.find(identifier) == channels.end()) // not already used identifier
  {
    FMOD::Channel* channel;
    FMOD_ASSERT(fmod_system->playSound(asset.sound, 0, false, &channel));
    channels[identifier] = channel;
  }
  else Log::Warning("Channel already exists for identifier: " + identifier);
}

void AudioManager::Core::StopSound(std::string const& identifier)
{
  if (channels.find(identifier) != channels.end())
  {
    channels[identifier]->stop();
    channels.erase(identifier);
  }
  else Log::Warning("Tried to stop channel that does not exist for identifier: " + identifier);
}

}// namespace FlexEngine

  //// Sample code to get if the channel is playing...
  //bool b;
  //channel->isPlaying(&b);

  //// Stop the channel
  //channel->stop();

  //// Channel represents an instance of sound being played...
  //channel->setVolume(0.5f);

  //// Channel groups are like a mixer, you can control the volume of a group of sounds
  //FMOD::ChannelGroup* musicGroup = nullptr;
  //fmod_system->createChannelGroup("Music", &musicGroup);

  //// Volume controls group wise
  //musicGroup->setVolume(0.7f);  // Set volume for the entire group
  //musicGroup->stop();