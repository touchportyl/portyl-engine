#pragma once
#include <FlexEngine.h>

using namespace FlexEngine;
namespace ChronoShift
{
  class ChronoGearStatsRegistry {
  public:
    static void createStats(std::string stat_name);
  private:
    static std::unordered_map<std::string, FlexECS::Entity> cg_stats_registry;
  };
}