//
//  CombatMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CombatMechanics.h"

#include <limits>

#include "Combat.h"
#include "CombatMap.h"
#include "common/Util.h"

#define COMBAT_LOG(y, ...) LOGG("[combat]", "%s", fmt::sprintf(y, __VA_ARGS__).c_str())

using namespace combat;

#pragma mark Movement Mechanics

u16 CombatMechanics::movementCostForTile(const CombatUnit* unit, const CombatTile* tile, Dir from)
{
  bool isLongMove = from == Dir::EAST || from == Dir::WEST || from == Dir::SOUTH || from == Dir::NORTH;
  
  if (unit->isFlying())
    return isLongMove ? 3 : 2;
  else if (tile->road != RoadType::NONE)
    return isLongMove ? 2 : 1;
  else if (tile->prop == TileProp::TREE)
    return isLongMove ? 4 : 3;
  else if (tile->prop == TileProp::ROCK)
    return isLongMove ? 4 : 3;
  else if (tile->prop == TileProp::TREES)
    return isLongMove ? 5 : 4;
  else if (tile->type == TileType::HILLS)
    return isLongMove ? 5 : 4;
  else
    return isLongMove ? 3 : 2;
}

CombatPosition CombatMechanics::positionForDeployedUnit(CombatMap* map, const CombatUnit* unit, u16 index)
{
  static const CombatCoord attackerBase = {4,13};//{7,16};
  static const Dir attackerRowDirection = Dir::SOUTH_EAST;
  
  static const CombatCoord defenderBase = {4,10};
  static const Dir defenderRowDirection = Dir::NORTH_WEST;
  
  static const CoordJump jumps[] = { { Dir::SOUTH_WEST }, { Dir::NORTH_EAST, 2 }, { Dir::SOUTH_WEST, 3 } };
  
  const auto base = unit->side() == Side::DEFENDER ? defenderBase : attackerBase;
  const auto dir = unit->side() == Side::DEFENDER ? defenderRowDirection : attackerRowDirection;
  const auto facing = unit->side() == Side::DEFENDER ? Dir::SOUTH_EAST : Dir::NORTH_WEST;
  
  auto rowStart = base;
  auto current = rowStart;
  for (u16 i = 0, r = 1; i < index; ++r)
  {
    if (r == 3)
    {
      rowStart = rowStart.neighbour(dir);
      current = rowStart;
      r = 0;
    }
    else
    {
      for (u16 c = 0; c < jumps[r-1].length; ++c)
        current = current.neighbour(jumps[r-1].direction);
    }
    
    
    const CombatTile* tile = map->tileAt(current.x, current.y);
    
    if (!isTileBlocked(tile, unit))
      ++i;
    else
      continue;
  }

  return CombatPosition(current, facing);
}

CombatCoord CombatMechanics::defaultPositionForObject(CombatObject object)
{
  switch (object)
  {
    case CombatObject::MAIN_BUILDING: return { 3, 8 };
    case CombatObject::WALL: return { 3, 6 };
    default: assert(false); return { 0, 0 };
  }
}

bool CombatMechanics::isTileBlocked(const CombatTile* tile, const CombatUnit* unit)
{
  assert(unit && tile);
  const auto building = tile->building;
  const auto stoneWall = tile->stoneWall;
  const auto isWallDestroyed = tile->isStoneWallDestroyed;
  
  /* if tile has a building which is not an house then movement is blocked */
  bool hasBuilding = building != TileBuilding::HOUSE && building != TileBuilding::NONE;
  /* if tile has a stone wall tower then it is blocked */
  bool isBlockedByStoneTower = !isWallDestroyed && (stoneWall == WallType::EAST_CORNER || stoneWall == WallType::WEST_CORNER || stoneWall == WallType::SOUTH_CORNER || stoneWall == WallType::NORTH_CORNER);
  
  /* if tile has a non broken stone wall and unit doesn't have flying/non corporeal of teleport*/
  const auto* skills = unit->skills();
  bool isBlockedByStoneWall = unit->isAttacker() && stoneWall != WallType::NO_WALL && !isWallDestroyed && !(skills->has(MovementType::FLYING) || skills->has(MovementType::NON_CORPOREAL) || skills->has(MovementType::TELEPORT));
  
  return hasBuilding || isBlockedByStoneTower || isBlockedByStoneWall;
}

combat_pathfind_info CombatMechanics::reachableTiles(const Combat* combat, const CombatUnit* unit, s16 movement)
{
  static constexpr size_t DIRECTIONS = 8;
  
  const auto& map = combat->_map;
  
  struct node
  {
    const CombatTile* tile;
    value_t cost;
    Dir from;
    
    node() : node(nullptr, std::numeric_limits<s16>::max(), Dir::INVALID) { }
    node(const CombatTile* tile, value_t cost, Dir from) : tile(tile), cost(cost), from(from) { }
  };

  std::unordered_set<const CombatTile*> closedSet;
  std::list<node> openSet;
  std::unordered_map<const CombatTile*, node> closedMap;
  
  
  LOGD3("[combat][pathfind] computing reachable tiles from (%d,%d)", unit->x(), unit->y())
  
  openSet.push_back({ map->tileAt(unit->position), 0, Dir::INVALID });
  
  while (!openSet.empty())
  {
    const auto node = *openSet.begin();
    openSet.erase(openSet.begin());
    
    if (closedSet.find(node.tile) != closedSet.end())
      continue;
    
    closedSet.insert(node.tile);
    
    for (size_t i = 0; i < DIRECTIONS; ++i)
    {
      const Dir dir = static_cast<Dir>(i);
      const auto* neighbour = node.tile->neighbour(dir);
      
      bool alreadyExplored = closedSet.find(neighbour) != closedSet.end();
      bool blockedByEnvironment = !neighbour || isTileBlocked(neighbour, unit);
      bool blockedByOtherUnit = neighbour && combat->unitAtTile(neighbour->coords) != nullptr;
      
      /* tile is available to be reached */
      if (!alreadyExplored && neighbour && !blockedByEnvironment && !blockedByOtherUnit)
      {
        auto& entry = closedMap[neighbour];
        value_t cost = node.cost + movementCostForTile(unit, neighbour, dir);
        
        /* the new path found to reach neighbour is cheaper that previous one 
           then this route path should be used instead
         */
        if (cost < entry.cost)
          entry = { neighbour, cost, dir };
        
        if (cost < movement)
          openSet.push_back(entry);
      }
    }
  }
  
  combat_pathfind_info::set_t reachable;

  std::transform(closedMap.begin(), closedMap.end(), std::inserter(reachable, reachable.end()), [](const decltype(closedMap)::value_type& entry) {
    return CombatPosition(entry.first->x(), entry.first->y(), entry.second.from);
  });
  
  return combat_pathfind_info(unit->position, reachable);
}

void CombatMechanics::castCombatInstant(const SpellCast<CombatSpell>& cast, const CombatUnit *unit)
{
  
}

#pragma mark Combat Mechanics

bool CombatMechanics::canMeleeAttack(const Combat* combat, const CombatUnit* attacker, const CombatUnit* defender)
{
  const auto& attackerSkills = attacker->skills();
  const auto& defenderSkills = defender->skills();

  if (defenderSkills->has(MovementType::FLYING))
    return attackerSkills->has(MovementType::FLYING) || attackerSkills->hasSimpleEffect(SimpleEffect::Type::ALLOW_MELEE_ATTACKS_AGAINST_FLYING);
  else
    return true;
}


#pragma mark CombatFormulas

/*
  http://masterofmagic.wikia.com/wiki/Defense#Order_of_Operations_for_Resolving_Defense_and_Immunities
  http://masterofmagic.wikia.com/wiki/Resistance
  http://masterofmagic.wikia.com/wiki/Conventional_Damage
  http://masterofmagic.wikia.com/wiki/Attack_Strength#Table_Of_Attack_Strength_Modifiers
*/

template<typename... Args> 
void CombatFormulas::log(const std::string& format, Args&&... args)
{
  if (logEnabled)
    COMBAT_LOG(format.c_str(), args...);
}

value_t CombatFormulas::passingRollsf(value_t count, float chance)
{
  value_t passed = 0;
  for (value_t i = 0; i < count; ++i)
    if (Math::chance(chance)) ++passed;
  return passed;
}

value_t CombatFormulas::passingRolls(value_t count, value_t chance)
{
  u32 passed = 0;
  for (u32 i = 0; i < count; ++i)
    if (Math::chance(chance)) ++passed;
  return passed;
}

damage_value CombatFormulas::computePhysicalDamage(value_t toHit, value_t strength, const HitPoints& hitPoints, value_t toDefend, value_t defense)
{
  /* compute passing attack rolls */
  value_t registered_hits = passingRolls(strength, toHit);

  /* build the set of hits for each living figure */
  unit_figure_value defender_hp = hitPoints;

  /* total amount of hits that should be handled*/
  value_t totalPossibleHits = registered_hits;
  value_t effectiveHits = 0;
  auto currentDefender = defender_hp.begin();
  
  
  log("computing physical damage of magnitude %d against %d figures", strength, hitPoints.size());
  log("  attacking with %d%% hit, defending with %d shields and %d%% chance", toHit, defense, toDefend);
  log("  possible hits: %d", totalPossibleHits);
  
  /* physical damage for multi figure units will make a separate defense roll for each unblocked damage on the lead figure */
  /* while there's still damage to be applied and there are still defenders */
  while (totalPossibleHits > 0 && currentDefender != defender_hp.end())
  {
    value_t startingHits = totalPossibleHits;
    /* figure makes defense rolls which are removed from potential hits */
    value_t defendedHitsByFigure = passingRolls(defense, toDefend);
    /* remove defended hits but don't go < 0 */
    totalPossibleHits = std::max(totalPossibleHits - defendedHitsByFigure, 0);
    /* apply to the figure at most its hp as damage */
    value_t hitsToFigure = std::min(*currentDefender, totalPossibleHits);
    
    /* update values */
    totalPossibleHits -= hitsToFigure;
    *currentDefender -= hitsToFigure;
    effectiveHits += hitsToFigure;
    
    log("    lead figure blocked %d of %d hits, received %d damage and %s", defendedHitsByFigure, startingHits, hitsToFigure, *currentDefender == 0 ? "died" : "survived");

    assert(totalPossibleHits == 0 || *currentDefender == 0);

    /* we go to next unit */
    ++currentDefender;
  }
  
  log("  total damage: %d", effectiveHits);
  
  return effectiveHits;
}


damage_value CombatFormulas::computeAreaDamage(value_t toHit, value_t strength, count_t figures, value_t hitPoints, value_t toDefend, value_t defense)
{
  /* to hit on each figure, to defend on each figure, each damage applied separately up to max figure HP, 
     then damage is summed altogether, will be applied as a whole
  */

  /* make attack rolls for each alive figure */
  unit_figure_value registered_hits = unit_figure_value(figures, [strength, toHit] (size_t index) { return passingRolls(strength, toHit); });
  
  /* make defend rolls for each alive figure */
  unit_figure_value defended_hits = unit_figure_value(figures, [defense, toDefend] (size_t index) { return passingRolls(defense, toDefend); });
  
  /* remove blocked damage */
  unit_figure_value effective_hits = registered_hits - defended_hits;
  effective_hits.clampNegativeValuesToZero();
  /* clamp values, damage per figure is limited by maximum hp per figure */
  effective_hits.forEach([hitPoints](value_t& v) { v = std::min(v, hitPoints); });

  value_t totalDamage = effective_hits.sum();

  log("computing area damage of magnitude %d against %d figures", strength, figures);
  log("  attacking with %d%% hit, defending with %d shields and %d%% chance", toHit, defense, toDefend);
  for (count_t i = 0; i < figures; ++i)
    log("    damage for figure %d: %2d - %2d = %2d (%2d)", i+1, registered_hits[i], defended_hits[i], registered_hits[i] - defended_hits[i], effective_hits[i]);
  log("  total damage: %d", totalDamage);

  return totalDamage;
}

unit_figure_value CombatFormulas::computeGazeDamage(damage_amount strength, count_t figures, value_t resistance, value_t bonus)
{
  /* one resistance roll per figure, if passed then damage is resisted */
  resistance = std::max(0, resistance + bonus);
  unit_figure_value result = unit_figure_value(figures, [resistance, strength](size_t) {
    return Math::chanceByTenths(resistance) ? 0 : strength.toDamage();
  });

  log("computing gaze damage of strength %d against %d figures with resistance %d (+%d)", strength.toDamage(), figures, resistance, bonus);
  log("  %d figures affected", result.countPositive());

  return result;
}

damage_value CombatFormulas::computePoisonDamage(damage_amount strength, value_t resistance, value_t bonus)
{
  /* for each point of damage we do a resistance roll: failure is 1 point of damage */
  auto passed = passingRolls(strength.toDamage(), (resistance + bonus) * 10);
  return strength.toDamage() - passed;
}
