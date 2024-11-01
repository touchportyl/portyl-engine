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
	// 
	//declaration of statics
	std::unordered_map<std::string, MoveFunction> MoveRegistry::s_move_function_registry;
	std::unordered_map<std::string, EffectFunction> MoveRegistry::s_status_function_registry;

	#pragma region MoveData
	
	void Move_DealDamage(std::vector<FlexECS::Entity> targets, int value) {
		// Example of Damage
		for (FlexECS::Entity target : targets)
		{
			if (target.HasComponent<Immunity>()) continue;
			else target.GetComponent<Character>()->current_health -= value;
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

	void Move_Immunity(std::vector<FlexECS::Entity> targets, int status_duration, int block_damage) {
		UNREFERENCED_PARAMETER(block_damage);
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Immunity>()) t.GetComponent<Immunity>()->remaining_turns += status_duration;
			else t.AddComponent<Immunity>({ status_duration }); // remaining turns
		}
	}

	void Move_Stun(std::vector<FlexECS::Entity> targets, int status_duration, int stun_value) {
		UNREFERENCED_PARAMETER(stun_value);
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Stun>()) t.GetComponent<Stun>()->remaining_turns += status_duration;
			else t.AddComponent<Stun>({ status_duration }); // remaining turns
		}
	}

	void Move_Shear(std::vector<FlexECS::Entity> targets, int status_duration, int shear_value) {
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Shear>()) {
				// to double check whether the damage value to add on or how
				t.GetComponent<Shear>()->remaining_turns += status_duration;
			}
			else t.AddComponent<Shear>({ status_duration , shear_value }); // remaining turns, damage
		}
	}

	void Move_Burn(std::vector<FlexECS::Entity> targets, int status_duration, int burn_value) {
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Burn>()) {
				// to double check whether the damage value to add on or how
				t.GetComponent<Burn>()->remaining_turns += status_duration;
			}
			else t.AddComponent<Burn>({ status_duration , burn_value }); // remaining turns, damage
		}
	}

	void Move_Shock(std::vector<FlexECS::Entity> targets, int status_duration, int shock_value) {
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Shock>()) {
				// to double check whether the damage value to add on or how
				t.GetComponent<Shock>()->remaining_turns += status_duration;
			}
			else t.AddComponent<Shock>({ status_duration , shock_value }); // remaining turns, damage
		}
	}

	void Move_Recovery(std::vector<FlexECS::Entity> targets, int status_duration, int heal_value) {
		for (FlexECS::Entity& t : targets) {
			if (t.HasComponent<Recovery>()) {
				// to double check whether the damage value to add on or how
				t.GetComponent<Recovery>()->remaining_turns += status_duration;
			}
			else t.AddComponent<Recovery>({ status_duration , heal_value }); // remaining turns, heal
		}
	}
	#pragma endregion;


	// Should i add the chrono gears with the moves list or should i create a seperate list?
	void MoveRegistry::RegisterMoveFunctions()
	{
		FlexEngine::Log::Debug("Registering Move Functions:");
		s_move_function_registry["DAMAGE"] = Move_DealDamage;
		s_move_function_registry["HEAL"] = Move_Recover;
		FlexEngine::Log::Debug("Move Functions Registered");
	}

	void MoveRegistry::RegisterStatusFunctions() {
		FlexEngine::Log::Debug("Registering Status Functions:");
		s_status_function_registry["RECOVERY"] = Move_Recovery;
		s_status_function_registry["BURN"] = Move_Burn;
		s_status_function_registry["SHEAR"] = Move_Shear;
		s_status_function_registry["SHOCK"] = Move_Shock;
		s_status_function_registry["IMMUNITY"] = Move_Immunity;
		s_status_function_registry["STUN"] = Move_Stun;
		FlexEngine::Log::Debug("Status Functions Registered");

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
		result.target_type = break_down_numerical_string[move.GetString("target_type", "")];
		result.move_value = move.GetInt("move_value", 0);
		result.move_function = s_move_function_registry[move.GetString("move_function", "")];
		result.effect_value = move.GetInt("effect_value", 0);
		result.effect_duration = move.GetInt("effect_duration", 0);
		if (move.GetString("effect_function", "") != "") {
			result.effect_function = s_status_function_registry[move.GetString("effect_function", "")];
		}
		
		return result;
	}


	MoveRegistry::MoveRegistry() {
	}

	MoveRegistry::~MoveRegistry() {
	}

}