/* Start Header
*****************************************************************/
/*!
\file      moves.cpp
\author    [90%] Rocky Sutarius
\par			 rocky.sutarius\@digipen.edu
\co-author [10%] Ho Jin Jie Donovan, h.jinjiedonovan, 2301233
\par       h.jinjiedonovan\@digipen.edu
\date      03 October 2024
\brief     This file contains the definition of all the moves'
					 functionalities along with the registry of these
					 moves stored into an unordered map

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#include "CharacterMoves/moves.h"
#include "Components/battlecomponents.h"
namespace ChronoShift 
{
	// MoveData: Functions for all the moves
	// GearData: Functions for all the chrono gears
	// Create passive system
	// - Each Character has a passive (passive has a unique passive name)
	// - Create a function that check whether character passive is in effect
	// - Struct Passive
	// - Passive Function (Inclusive of if check)
	#pragma region MoveData
	
	void Move_DealDamage(std::vector<FlexECS::Entity> targets, int value) {
		// Example of Damage
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= value;
		}
	}

	void Move_Recover(std::vector<FlexECS::Entity> targets, int value) {
		// Example of Healing
		for (FlexECS::Entity target : targets)
		{
			if (target.GetComponent<Character>()->current_health + value > target.GetComponent<Character>()->base_health) {
				target.GetComponent<Character>()->current_health = target.GetComponent<Character>()->base_health;
			}
			else target.GetComponent<Character>()->current_health += value;
		}
	}
	#pragma endregion;

	//declaration of statics
	std::unordered_map<int, MoveEffect> MoveRegistry::s_move_function_registry;

	// Should i add the chrono gears with the moves list or should i create a seperate list?
	void MoveRegistry::RegisterMoves()
	{
		FlexEngine::Log::Debug("Registering Moves:");
		s_move_function_registry[REDUCE_HP] = Move_DealDamage;
		s_move_function_registry[INCREASE_HP] = Move_Recover;
		FlexEngine::Log::Debug("Registering Moves finished");
	}

	// Access Move Data from File
	
	Move MoveRegistry::GetMove(std::string name)
	{
		Asset::FlxData& move = FLX_ASSET_GET(Asset::FlxData, "/data/" + name + ".flxdata");
		Move result;
		result.name = move.GetString("name", "");
		result.description = move.GetString("description", "");
		result.cost = move.GetInt("speed_cost", 0);
		result.is_target_enemy = move.GetBool("is_target_enemy", false);
		result.target_type = move.GetInt("target_type", 0);
		result.damage = move.GetInt("damage", 0);
		result.effect = s_move_function_registry[move.GetInt("Function", 0)];
		return result;
	}


	MoveRegistry::MoveRegistry() {
	}

	MoveRegistry::~MoveRegistry() {
	}

}