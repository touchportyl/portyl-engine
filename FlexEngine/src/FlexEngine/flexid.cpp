#include "flexid.h"

#include <vector>

namespace FlexEngine
{
  namespace ID
  {

    #pragma region Internal Functions

    static void Internal_IncrementGeneration(uint64_t& id)
    {
      id += (static_cast<uint64_t>(1) << SHIFT_GENERATION);
    }

    static void Internal_ResetFlags(uint64_t& id)
    {
      id &= ~(static_cast<uint64_t>(MASK_FLAGS) << SHIFT_FLAGS);
    }

    #pragma endregion

    #pragma region Constructors

    __FLX_API uint64_t Create(const uint8_t flags, uint64_t& next_id, std::vector<uint64_t>& unused_ids)
    {
      // If there are any unused IDs, use them first.
      if (!unused_ids.empty())
      {
        uint64_t id = unused_ids.back();
        unused_ids.pop_back();

        // Increment the generation and reset the flags.
        Internal_IncrementGeneration(id);
        Internal_ResetFlags(id);

        return id;
      }

      // Otherwise, create a new ID.
      uint64_t id = 0;
      id |= (next_id << SHIFT_ID);
      Internal_IncrementGeneration(id);
      SetFlags(id, flags);

      next_id++;
      return id;
    }

    __FLX_API uint64_t Create(
      const bool __FLX_ECS_ID_FLAG_ONE_,
      const bool __FLX_ECS_ID_FLAG_TWO_,
      const bool __FLX_ECS_ID_FLAG_THREE_,
      const bool __FLX_ECS_ID_FLAG_FOUR_,
      uint64_t& next_id,
      std::vector<uint64_t>& unused_ids
    )
    {
      return Create(
        (__FLX_ECS_ID_FLAG_ONE_   ? FLAG(__FLX_ECS_ID_FLAG_ONE)   : FLAG(None)) |
        (__FLX_ECS_ID_FLAG_TWO_   ? FLAG(__FLX_ECS_ID_FLAG_TWO)   : FLAG(None)) |
        (__FLX_ECS_ID_FLAG_THREE_ ? FLAG(__FLX_ECS_ID_FLAG_THREE) : FLAG(None)) |
        (__FLX_ECS_ID_FLAG_FOUR_  ? FLAG(__FLX_ECS_ID_FLAG_FOUR)  : FLAG(None)),
        next_id, unused_ids
      );
    }

    __FLX_API void Destroy(uint64_t& id, std::vector<uint64_t>& unused_ids)
    {
      unused_ids.push_back(id);
      id = 0ULL;
    }

    #pragma endregion

    #pragma region Getters

    __FLX_API void Decompose(const uint64_t id, uint32_t* out_id, uint32_t* out_generation, uint8_t* out_flags)
    {
      if (out_id) *out_id = GetID(id);
      if (out_generation) *out_generation = GetGeneration(id);
      if (out_flags) *out_flags = GetFlags(id);
    }

    __FLX_API uint32_t GetID(const uint64_t id)
    {
      return (id & MASK_ID) >> SHIFT_ID;
    }

    __FLX_API uint32_t GetGeneration(const uint64_t id)
    {
      return (id & MASK_GENERATION) >> SHIFT_GENERATION;
    }

    __FLX_API uint8_t GetFlags(const uint64_t id)
    {
      return (id & MASK_FLAGS) >> SHIFT_FLAGS;
    }

    #pragma endregion

    #pragma region Setters

    __FLX_API void SetFlags(uint64_t& id, const uint8_t value)
    {
      Internal_ResetFlags(id);
      id |= (static_cast<uint64_t>(value) << SHIFT_FLAGS);
    }

    __FLX_API void SetFlags(
      uint64_t& id,
      const bool __FLX_ECS_ID_FLAG_ONE_,
      const bool __FLX_ECS_ID_FLAG_TWO_,
      const bool __FLX_ECS_ID_FLAG_THREE_,
      const bool __FLX_ECS_ID_FLAG_FOUR_)
    {
      SetFlags(
        id,
        static_cast<uint8_t>(
          (__FLX_ECS_ID_FLAG_ONE_   ? FLAG(__FLX_ECS_ID_FLAG_ONE)   : FLAG(None)) |
          (__FLX_ECS_ID_FLAG_TWO_   ? FLAG(__FLX_ECS_ID_FLAG_TWO)   : FLAG(None)) |
          (__FLX_ECS_ID_FLAG_THREE_ ? FLAG(__FLX_ECS_ID_FLAG_THREE) : FLAG(None)) |
          (__FLX_ECS_ID_FLAG_FOUR_  ? FLAG(__FLX_ECS_ID_FLAG_FOUR)  : FLAG(None))
        )
      );
    }

    #pragma endregion

    #pragma region Flag Checkers

    __FLX_API bool __FLX_ECS_ID_FLAG_ONE    (uint64_t id) { return (id & FLAG(__FLX_ECS_ID_FLAG_ONE   )) != 0; }
    __FLX_API bool __FLX_ECS_ID_FLAG_TWO    (uint64_t id) { return (id & FLAG(__FLX_ECS_ID_FLAG_TWO   )) != 0; }
    __FLX_API bool __FLX_ECS_ID_FLAG_THREE  (uint64_t id) { return (id & FLAG(__FLX_ECS_ID_FLAG_THREE )) != 0; }
    __FLX_API bool __FLX_ECS_ID_FLAG_FOUR   (uint64_t id) { return (id & FLAG(__FLX_ECS_ID_FLAG_FOUR  )) != 0; }

    #pragma endregion

  }
}
