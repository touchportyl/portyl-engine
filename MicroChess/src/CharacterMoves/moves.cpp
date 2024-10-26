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
	
	void Move_Empty(std::vector<FlexECS::Entity> targets) {

	}

	void Move_DualStrike(std::vector<FlexECS::Entity> targets) {
		// Example of Two Enemy Damage
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 1;
		}
	}

	void Move_Whirlwind(std::vector<FlexECS::Entity> targets) {
		// Example of Three Enemy Damage
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 2;
		}
		//modify speed buff/debuff here
	}

	void Move_Block(std::vector<FlexECS::Entity> targets) {
		//add block modifier here

	}

	void Move_RiftSurge(std::vector<FlexECS::Entity> targets) {
		// Example of Five Enemy Damage
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 3;
		}
	}

	void Move_Flashbang(std::vector<FlexECS::Entity> targets) {
		// Example of Single Enemy Damage
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 2;
			//Add dizzy
		}
	}

	void Move_Heal(std::vector<FlexECS::Entity> targets) {
		// Example of Ally and Self Heal
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health += 2;
			//Add dizzy
		}
	}

	void Move_FullRecovery(std::vector<FlexECS::Entity> targets) {
		// Example of Team Heal
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health += 2;
			//Add dizzy
		}
	}

	void Move_TemporalWave(std::vector<FlexECS::Entity> targets) {
		// Example of 
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 2;
		}
		/*for (FlexECS::Entity target : targets)
		{
			target.GetComponent<StatusEffects>()->speed.value -= 1;
			target.GetComponent<StatusEffects>()->speed.duration = 5;
		}*/
	}

	void Move_Poke(std::vector<FlexECS::Entity> targets) {
		for (FlexECS::Entity target : targets)
		{
			target.GetComponent<Character>()->current_health -= 1;
		}
	}
	#pragma endregion;

	#pragma region GearData

	#pragma endregion

	#pragma region PassiveData
	#pragma endregion

	//declaration of statics
	std::unordered_map<std::string, Move> MoveRegistry::s_move_registry;
	std::unordered_map<int, MoveEffect> MoveRegistry::s_move_function_registry;

	enum : int
	{
		
	};
	// Should i add the chrono gears with the moves list or should i create a seperate list?
	void MoveRegistry::RegisterMoves()
	{
		/*
		Asset::FlxData& f = FLX_ASSET_GET(Asset::FlxData, "/data/flxer.flxdata");
                if (f.GetBool("test", false) == false) Log::Warning("Failed to get bool");
                if (f.GetString("tester", "") != "Hello2 World!") Log::Warning("Failed to get string");
                if (f.GetInt("testint", 0) != 1243) Log::Warning("Failed to get int");
		//Asset::FlxData flex("/data/flxer.flxdata");
    //flex.SetBool("test", true);
    //flex.SetString("tester", "Hello2 World!");
    //flex.SetInt("testint", 1243);
    //flex.Save();
		*/
		/*Asset::FlxData move("/data/Dual Strike.flxdata");
		move.SetString("name", "Dual Strike");
		move.SetString("description", "Hit target 2 times for 1 damage");
		move.SetInt("speed_cost", 4);
		move.SetBool("is_target_enemy", true);
		move.SetInt("target_type", MOVE_TARGET_DOUBLE);
		move.SetInt("damage", 2);
		move.SetInt("Function", 0);
		move.Save();*/
		s_move_function_registry[REDUCE_HP] = Move_DualStrike;

		FlexEngine::Log::Debug("Registering Moves:");
		//Move EMPTY{ "", "YO", 0, MOVE_TARGET_NONE, 0, Move_Empty };
		Move dual_strike{ "Dual Strike", "Hit target 2 times for 1 damage", 4, true, MOVE_TARGET_DOUBLE, 2, Move_DualStrike };
		Move whirlwind{ "Whirlwind", "Hits 2 Targets for 2 damage. Increases self speed by 1.", 7, true, MOVE_TARGET_TRIPLE, 2, Move_Whirlwind };
		Move block{ "Block", "Takes no damage from moves.", 5, false, MOVE_TARGET_SELF, 0, Move_Block };
		Move rift_surge{ "Rift Surge", "Hits 2 targets for 3 damage", 7, true, MOVE_TARGET_TRIPLE, 2, Move_RiftSurge };
		Move flashbang{ "Flashbang", "Hit 1 target for 2 damage and adds dizzy.", 5, true, MOVE_TARGET_SINGLE, 1, Move_Flashbang };
		Move temporal_wave{ "Temporal Wave", "Increase all drifter’s speed by 1.", 7, true, MOVE_TARGET_ALL, 0, Move_TemporalWave };
		Move full_recovery{ "Full Recovery", "Heals Entire team to max HP", 7, false, MOVE_TARGET_ALL, 0, Move_FullRecovery };
		Move poke{ "Poke", "Owie.", 7, false, MOVE_TARGET_SINGLE, 1, Move_Poke };
		Move heal{ "Heal", "Heal 1 Ally for 5 HP.", 5, false, MOVE_TARGET_SINGLE, 1, Move_Heal };

		//s_move_registry[EMPTY.name] = EMPTY;
		s_move_registry[dual_strike.name] = dual_strike;
		s_move_registry[whirlwind.name] = whirlwind;
		s_move_registry[block.name] = block;
		s_move_registry[rift_surge.name] = rift_surge;
		s_move_registry[flashbang.name] = flashbang;
		s_move_registry[temporal_wave.name] = temporal_wave;
		s_move_registry[poke.name] = poke;
		s_move_registry[full_recovery.name] = full_recovery;
		s_move_registry[heal.name] = heal;
		FlexEngine::Log::Debug("Registering Moves finished");
	}

	// Register Chrono Gears Functions
	
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
		//return Instance().s_move_registry[name];
		return result;
	}


	MoveRegistry::MoveRegistry()
	{
		//FlexEngine::Log::Flow("Registering Moves:");
		//Move EMPTY{ "", "YO", 0, MOVE_TARGET_NONE, 0, Move_Empty };
		//Move dual_strike{ "Dual Strike", "Hit target 2 times for 1 damage", 4, MOVE_TARGET_SINGLE, 2, Move_DualStrike };
		//Move whirlwind{ "Whirlwind", "Hits 2 Targets for 2 damage. Increases self speed by 1.", 7, MOVE_TARGET_SINGLE, 2, Move_Whirlwind };
		//Move block{ "Block", "Takes no damage from moves.", 5, MOVE_TARGET_SELF, 0, Move_Block };
		//Move rift_surge{ "Rift Surge", "Hits 2 targets for 3 damage", 7, MOVE_TARGET_SINGLE, 2, Move_RiftSurge };
		//Move flashbang{ "Flashbang", "Hit 1 target for 2 damage and adds dizzy.", 5, MOVE_TARGET_SINGLE, 1, Move_Flashbang };
		//Move temporal_wave{ "Temporal Wave", "Increase all drifter’s speed by 1.", 7, MOVE_TARGET_ALL_PLAYER, 0, Move_TemporalWave };
		//Move poke{ "Poke", "Owie.", 7, MOVE_TARGET_SINGLE, 1, Move_Poke};

		//m_move_registry[EMPTY.name]= EMPTY;
		//m_move_registry[dual_strike.name]= dual_strike;
		//m_move_registry[whirlwind.name]= whirlwind;
		//m_move_registry[block.name]= block;
		//m_move_registry[rift_surge.name]= rift_surge;
		//m_move_registry[flashbang.name]= flashbang;
		//m_move_registry[temporal_wave.name]= temporal_wave;
		//m_move_registry[poke.name]= poke;

		//MoveRegistry::Instance().m_move_registry.emplace(EMPTY.name, EMPTY);
		//MoveRegistry::Instance().m_move_registry.emplace(dual_strike.name, dual_strike);
		//MoveRegistry::Instance().m_move_registry.emplace(whirlwind.name, whirlwind);
		//MoveRegistry::Instance().m_move_registry.emplace(block.name, block);
		//MoveRegistry::Instance().m_move_registry.emplace(rift_surge.name, rift_surge);
		//MoveRegistry::Instance().m_move_registry.emplace(flashbang.name, flashbang);
		//MoveRegistry::Instance().m_move_registry.emplace(temporal_wave.name, temporal_wave);
		//MoveRegistry::Instance().m_move_registry.emplace(poke.name, poke);
		//FlexEngine::Log::Flow("Registering Moves finished");
	}

	MoveRegistry::~MoveRegistry()
	{
	}

}