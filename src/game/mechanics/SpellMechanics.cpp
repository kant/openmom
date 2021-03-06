#include "SpellMechanics.h"

#include "Spells.h"
#include "SpellBook.h"
#include "Player.h"
#include "Tile.h"
#include "Unit.h"
#include "Game.h"
#include "World.h"

#include <list>

using namespace std;

count_t SpellMechanics::guaranteedSpellAmountForRarity(SpellRarity rarity, School school, count_t books)
{
  // no benefits from more than 11 books
  if (books > 11)
    books = 11;
  
  if (rarity == SpellRarity::COMMON)
    return books - 1;
  else if (rarity == SpellRarity::UNCOMMON && books == 11)
    return 2;
  else if (rarity == SpellRarity::RARE && books == 11)
    return 1;
  else
    return 0;
}

spell_rarity_map<s32> SpellMechanics::guaranteedSpells(School school, u32 books)
{
  return spell_rarity_map<s32>({
    { SpellRarity::COMMON, guaranteedSpellAmountForRarity(SpellRarity::COMMON, School::NO_SCHOOL, books) },
    { SpellRarity::UNCOMMON, guaranteedSpellAmountForRarity(SpellRarity::UNCOMMON, School::NO_SCHOOL, books) },
    { SpellRarity::RARE, guaranteedSpellAmountForRarity(SpellRarity::RARE, School::NO_SCHOOL, books) },
    { SpellRarity::VERY_RARE, guaranteedSpellAmountForRarity(SpellRarity::VERY_RARE, School::NO_SCHOOL, books) }
  });
}

u32 SpellMechanics::researchableSpellAmountForRarity(SpellRarity rarity, School school, count_t books)
{
  static enum_simple_map<SpellRarity, std::array<u8, 12>, 4> data = {
    { SpellRarity::COMMON,    { {0, 3, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10} } },
    { SpellRarity::UNCOMMON,  { {0, 1, 2, 3, 4, 5, 6,  8, 10, 10, 10, 10} } },
    { SpellRarity::RARE,      { {0, 0, 1, 2, 3, 4, 5,  6,  7,  9, 10, 10} } },
    { SpellRarity::VERY_RARE, { {0, 0, 0, 1, 2, 3, 4,  5,  6,  7, 10, 10} } },
  };
  
  // no benefits from more than 11 books
  if (books > 11)
    books = 11;
  
  return data[rarity][books];
}

bool SpellMechanics::isGlobalAllowed(const Player *player, const GlobalSpell *spell)
{
  bool hasAlready = std::find(player->getSpells().begin(), player->getSpells().end(), spell) != player->getSpells().end();

  return !hasAlready;
}

bool SpellMechanics::isCityAllowed(const Player *player, const CitySpell *spell, const City *city)
{
  // TODO
  return true;
}

bool SpellMechanics::isTileAllowed(const Player *player, const Spell *spell, const Tile *tile)
{
  if (!player->fog()->get(tile->position))
    return false;
  
  if (spell == Spells::CORRUPTION)
  {
    if (tile->type == TileType::OCEAN || tile->node())
      return false;
  }
  else if (spell == Spells::CHANGE_TERRAIN)
  {
    if (tile->node()) return false;
    
    switch (tile->type) {
      case TileType::DESERT:
      case TileType::FOREST:
      case TileType::HILL:
      case TileType::SWAMP:
      case TileType::GRASS:
      case TileType::VOLCANO:
      case TileType::MOUNTAIN:
        return true;
      default:
        return false;
    }
  }
  else if (spell == Spells::RAISE_VOLCANO)
  {
    return !tile->node() && tile->type != TileType::HILL && tile->type != TileType::MOUNTAIN && tile->type != TileType::RIVER;
  }
  
  return true;
}

bool SpellMechanics::isUnitAllowed(const Player *player, const UnitSpell *spell, const Unit *unit)
{
  return !unit->skills()->hasSpell(spell);
}

bool SpellMechanics::applyTileSpell(const SpellCast<SpecialSpell>& cast, Tile *tile)
{
  if (cast.spell() == Spells::CHANGE_TERRAIN)
  {
    TileType newType;
    
    switch (tile->type) {
      case TileType::DESERT:
      case TileType::FOREST:
      case TileType::HILL:
      case TileType::SWAMP:
        newType = TileType::GRASS; break;
      case TileType::GRASS:
        newType = TileType::FOREST; break;
      case TileType::VOLCANO:
        newType = TileType::MOUNTAIN; break;
      case TileType::MOUNTAIN:
        newType = TileType::HILL; break;
      default:
        newType = TileType::GRASS;
    }
    
    tile->type = newType;
    
    g->world->calcSubTile(tile->position.x, tile->position.y, tile->position.plane);
    //TODO: also on neighbors
  }
  else if (cast.spell() == Spells::RAISE_VOLCANO)
  {
    tile->resource = Resource::NONE;
    tile->type = TileType::VOLCANO;
  }
  else
  {
    tile->addSpell(cast);
  }
  
  return true;
}

value_t SpellMechanics::actualManaCost(Player *player, const Spell *spell, bool combat)
{
  return combat ? spell->mana.combatManaCost : spell->mana.manaCost;
}

value_t SpellMechanics::actualResearchGain(const Player *player, const Spell *spell)
{
  value_t research = player->baseResearchPoints();
  
  //TODO: check how bonus is calculated, ceil floor?
  // 20% bonus by Sage Master
  if (player->hasRetort("sage_master"))
    research += std::floor(research * 0.20f);
  
  if (spell)
  {
    // 25% bonus by Conjurer if Summoning spell
    if (player->hasRetort("conjurer") && spell->kind == SpellKind::SUMMONING)
      research += std::floor(research * 0.25f);
    
    // 15% for misc School mastery if spell of same school
    if (player->hasMastery(spell->school))
      research += std::floor(research * 0.15f);
    
    // 10% of discount for each book starting from 8 books
    int bookCount = player->book()->booksForSchool(spell->school);
    if (bookCount >= 8)
      research += std::floor(research * (0.10f * bookCount - 7));
  }
  
  return research;
}

float SpellMechanics::computeDispelChance(const Cast& cast, const Cast& dispelCast, float dispelMultiplier)
{
  value_t dispelMana = dispelCast.totalMana();
  value_t castMana = cast.totalMana();

  dispelMana *= dispelMultiplier;
  
  /* runemaster helps dispelling */
  if (dispelCast.caster().isWizard() && dispelCast.caster().player()->hasRetort("runemaster"))
    dispelMana *= 2.0f;
  
  if (cast.caster().isWizard())
  {
    /* archmage prevents dispelling */
    if (cast.caster().player()->hasRetort("archmage"))
      castMana += cast.totalMana();
    
    /* school mastery prevents dispelling */
    if (cast.caster().player()->hasMastery(cast.spell()->school))
      castMana += cast.totalMana();

  }
  
  return dispelMana / (float)(dispelMana + castMana);
}



