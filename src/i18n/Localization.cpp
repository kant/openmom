#include "Localization.h"

#include "UnitSpec.h"


using namespace std;


constexpr const I18 i18n::CITY_SIZE_NAMES[];

unordered_map<I18, const string> i18n::data =
{
  {I18::CITY_HAMLET, "Hamlet"},
  {I18::CITY_VILLAGE, "Village"},
  {I18::CITY_TOWN, "Town"},
  {I18::CITY_CITY, "City"},
  {I18::CITY_CAPITOL, "Capitol"},
  
  {I18::MESSAGE_NEW_BUILDING, "The %s of %s has completed the construction of a %s."},
  
  {I18::RACE_NAME_BARBARIANS, "Barbarians"},
  {I18::RACE_NAME_BEASTMEN, "Beastmen"},
  {I18::RACE_NAME_DARK_ELVES, "Dark Elves"},
  {I18::RACE_NAME_DRACONIANS, "Draconians"},
  {I18::RACE_NAME_DWARVES, "Dwarves"},
  {I18::RACE_NAME_GNOLLS, "Gnolls"},
  {I18::RACE_NAME_HALFLINGS, "Halflings"},
  {I18::RACE_NAME_HIGH_ELVES, "High Elves"},
  {I18::RACE_NAME_HIGH_MEN, "High Men"},
  {I18::RACE_NAME_KLACKONS, "Klackons"},
  {I18::RACE_NAME_LIZARDMEN, "Lizardmen"},
  {I18::RACE_NAME_NOMADS, "Nomads"},
  {I18::RACE_NAME_ORCS, "Orcs"},
  {I18::RACE_NAME_TROLLS, "Trolls"},
  
  {I18::UNIT_NAME_BARBARIANS, "Barbarian"},
  {I18::UNIT_NAME_BEASTMEN, "Beastman"},
  {I18::UNIT_NAME_DARK_ELVES, "Dark Elf"},
  {I18::UNIT_NAME_DRACONIANS, "Draconian"},
  {I18::UNIT_NAME_DWARVES, "Dwarven"},
  {I18::UNIT_NAME_GNOLLS, "Gnoll"},
  {I18::UNIT_NAME_HALFLINGS, "Halfling"},
  {I18::UNIT_NAME_HIGH_ELVES, "High Elf"},
  {I18::UNIT_NAME_HIGH_MEN, "High Man"},
  {I18::UNIT_NAME_KLACKONS, "Klackon"},
  {I18::UNIT_NAME_LIZARDMEN, "Lizardman"},
  {I18::UNIT_NAME_NOMADS, "Nomad"},
  {I18::UNIT_NAME_ORCS, "Orc"},
  {I18::UNIT_NAME_TROLLS, "Troll"},
  
  {I18::WIZARD_NAME_MERLIN, "Merlin"},
  {I18::WIZARD_NAME_RAVEN, "Raven"},
  {I18::WIZARD_NAME_SHAREE, "Sharee"},
  {I18::WIZARD_NAME_LO_PAN, "Lo Pan"},
  {I18::WIZARD_NAME_JAFAR, "Jafar"},
  {I18::WIZARD_NAME_OBERIC, "Oberic"},
  {I18::WIZARD_NAME_RJAK, "Rjak"},
  {I18::WIZARD_NAME_SSS_RA, "Sss'Ra"},
  {I18::WIZARD_NAME_TAURON, "Tauron"},
  {I18::WIZARD_NAME_FREYA, "Freya"},
  {I18::WIZARD_NAME_HORUS, "Horus"},
  {I18::WIZARD_NAME_ARIEL, "Ariel"},
  {I18::WIZARD_NAME_TLALOC, "Tlaloc"},
  {I18::WIZARD_NAME_KALI, "Kali"},
  
  {I18::BUILDING_BUILDERS_HALL,"Builders Hall"},
  {I18::BUILDING_SMITHY,"Smithy"},
  {I18::BUILDING_CITY_WALLS,"City Walls"},
  {I18::BUILDING_SHRINE,"Shrine"},
  {I18::BUILDING_TEMPLE,"Temple"},
  {I18::BUILDING_PARTHENON,"Parthenon"},
  {I18::BUILDING_CATHEDRAL,"Cathedral"},
  {I18::BUILDING_ANIMISTS_GUILD,"Animists Guild"},
  {I18::BUILDING_ORACLE,"Oracle"},
  {I18::BUILDING_ALCHEMISTS_GUILD,"Alchemists Guild"},
  {I18::BUILDING_WIZARDS_GUILD,"Wizards Guild"},
  {I18::BUILDING_STABLE,"Stable"},
  {I18::BUILDING_FANTASTIC_STABLE,"Fantastic Stable"},
  {I18::BUILDING_BARRACKS,"Barracks"},
  {I18::BUILDING_ARMORY,"Armory"},
  {I18::BUILDING_FIGHTERS_GUILD,"Fighters Guild"},
  {I18::BUILDING_ARMORERS_GUILD,"Armorers Guild"},
  {I18::BUILDING_WAR_COLLEGE,"War College"},
  {I18::BUILDING_SHIP_WRIGHTS_GUILD,"Ship Wrights Guild"},
  {I18::BUILDING_SHIP_YARD,"Ship Yard"},
  {I18::BUILDING_MARITIME_GUILD,"Maritime Guild"},
  {I18::BUILDING_MARKETPLACE,"Marketplace"},
  {I18::BUILDING_BANK,"Bank"},
  {I18::BUILDING_MERCHANTS_GUILD,"Merchants Guild"},
  {I18::BUILDING_GRANARY,"Granary"},
  {I18::BUILDING_FARMERS_MARKET,"Farmers Market"},
  {I18::BUILDING_LIBRARY,"Library"},
  {I18::BUILDING_UNIVERSITY,"University"},
  {I18::BUILDING_SAGES_GUILD,"Sages Guild"},
  {I18::BUILDING_MINERS_GUILD,"Miners Guild"},
  {I18::BUILDING_MECHANICIANS_GUILD,"Mechanicians Guild"},
  {I18::BUILDING_SAWMILL,"Sawmill"},
  {I18::BUILDING_FORESTERS_GUILD,"Forester's Guild"},
  {I18::BUILDING_TRADE_GOODS,"Trade Goods"},
  {I18::BUILDING_HOUSING,"Housing"},
  {I18::BUILDING_MAGE_FORTRESS,"Fortress"},
  {I18::BUILDING_SUMMONING_CIRCLE,"Summoning Circle"},
  
  {I18::UNIT_SPEARMEN,"Spearmen"},
  {I18::UNIT_SWORDSMEN,"Swordsmen"},
  {I18::UNIT_BOWMEN,"Bowmen"},
  {I18::UNIT_CAVALRY,"Cavalry"},
  {I18::UNIT_SHAMAN,"Shaman"},
  {I18::UNIT_HALBERDIERS,"Helberdiers"},
  {I18::UNIT_PRIESTS,"Priests"},
  {I18::UNIT_MAGICIANS,"Magicians"},
  {I18::UNIT_ENGINEERS,"Engineers"},
  {I18::UNIT_SETTLERS,"Settlers"},
  
  {I18::UNIT_BERSERKERS,"Berserkers"},
  
  {I18::HERO_DWARF,"Dward"},
  {I18::HERO_ORC_WARRIOR,"Orc Warrior"},
  
  {I18::UNIT_MAGIC_SPIRIT,"Magic Spirit"},
  {I18::UNIT_HELL_HOUNDS,"Hell Hounds"},
  {I18::UNIT_GREAT_DRAKE,"Great Drake"},
  
  {I18::SPELL_CORRUPTION, "Corruption"},
  {I18::SPELL_CHANGE_TERRAIN, "Change Terrain"},
  {I18::SPELL_RAISE_VOLCANO, "Raise Volcano"},
  
  {I18::EVENT_BAD_MOON, "Bad Moon"},
  {I18::EVENT_RED_CONJUNCTION, "Conjunction"},
  {I18::EVENT_GREEN_CONJUNCTION, "Conjunction"},
  {I18::EVENT_BLUE_CONJUNCTION, "Conjunction"},
  {I18::EVENT_DEPLETION, "Depletion"},
  {I18::EVENT_DIPLOMATIC_MARRIAGE, "Diplomatic Marriage"},
  {I18::EVENT_DISJUNCTION, "Disjunction"},
  {I18::EVENT_DONATION, "Donation"},
  {I18::EVENT_EARTHQUAKE, "Earthquake"},
  {I18::EVENT_THE_GIFT, "The Gift"},
  {I18::EVENT_GOOD_MOON, "Good Moon"},
  {I18::EVENT_GREAT_METEOR, "Great Meteor"},
  {I18::EVENT_MANA_SHORT, "Mana Short"},
  {I18::EVENT_NEW_MINERALS, "New Minerals"},
  {I18::EVENT_PIRACY, "Piracy"},
  {I18::EVENT_PLAGUE, "Plague"},
  {I18::EVENT_POPULATION_BOOM, "Population Boom"},
  {I18::EVENT_REBELLION, "Rebellion"},

  
  {I18::PLACEHOLDER, "Placeholder"},
};

unordered_map<UnitID, const string> i18n::units =
{
  {UnitID::SPEARMEN,"Spearmen"},
  {UnitID::SWORDSMEN,"Swordsmen"},
  {UnitID::BOWMEN,"Bowmen"},
  {UnitID::CAVALRY,"Cavalry"},
  {UnitID::SHAMANS,"Shaman"},
  {UnitID::HALBERDIERS,"Helberdiers"},
  {UnitID::PRIESTS,"Priests"},
  {UnitID::MAGICIANS,"Magicians"},
  {UnitID::ENGINEERS,"Engineers"},
  {UnitID::SETTLERS,"Settlers"},
  
  {UnitID::BERSERKERS,"Berserkers"},
  
  {UnitID::HERO_DWARF,"Dward"},
  {UnitID::HERO_ORC_WARRIOR,"Orc Warrior"},
  
  {UnitID::MAGIC_SPIRIT,"Magic Spirit"},
  {UnitID::HELL_HOUNDS,"Hell Hounds"},
  {UnitID::GREAT_DRAKE,"Great Drake"},
};

unordered_map<SkillBase, const string> i18n::skills =
{
  {SkillBase::HERO_AGILITY, "Agility"},
  {SkillBase::HERO_ARCANE_POWER, "Arcane Power"},
  {SkillBase::HERO_ARMS_MASTER, "Arms Master"},
  {SkillBase::HERO_BLADE_MASTER, "Blade Master"},
  {SkillBase::HERO_CHARMED, "Charmed"},
  {SkillBase::HERO_CONSTITUTION, "Constitution"},
  {SkillBase::HERO_LEADERSHIP, "Leadership"},
  {SkillBase::HERO_LEGENDARY, "Legendary"},
  {SkillBase::HERO_LUCK, "Luck"},
  {SkillBase::HERO_MIGHT, "Might"},
  {SkillBase::HERO_NOBLE, "Noble"},
  {SkillBase::HERO_PRAYER_MASTER, "Prayer Master"},
  {SkillBase::HERO_SAGE, "Sage"},
  {SkillBase::HERO_SPELL_CASTER, "Spell Caster"},
  
  {SkillBase::LARGE_SHIELD, "Large Shield"},
  {SkillBase::FIRST_STRIKE, "First Strike"},
  {SkillBase::NEGATE_FIRST_STRIKE, "Negate First Strike"},
  {SkillBase::ARMOR_PIERCING, "Armor Piercing"},
  {SkillBase::LONG_RANGE, "Long Range"},
  {SkillBase::DISPEL_EVIL, "Dispel Evil"},
  {SkillBase::CAUSE_FEAR, "Cause Fear Spell"},
  {SkillBase::WEB_SPELL, "Web Spell"},
  {SkillBase::CREATE_UNDEAD, "Create Undead"},
  {SkillBase::SUMMON_DEMONS, "Summon Demons"},
  {SkillBase::CASTER, "Caster"},
  {SkillBase::HEALING_SPELL, "Healing Spell"},
  {SkillBase::FIRE_BALL_SPELL, "Fire Ball Spell"},
  {SkillBase::DOOM_BOLT_SPELL, "Doom Bolt Spell"},
  
  {SkillBase::ILLUSIONARY_ATTACK, "Illusionary Attack"},
  {SkillBase::POISON_ATTACK, "Poison Attack"},
  {SkillBase::LIFE_STEALING, "Life Stealing"},
  {SkillBase::FIERY_BREATH, "Fiery Breath"},
  {SkillBase::LIGHTING_BREATH, "Lighting Breath"},
  {SkillBase::DOOM_GAZE, "Doom Gaze"},
  {SkillBase::DEATH_GAZE, "Death Gaze"},
  {SkillBase::STONE_GAZE, "Stoning Gaze"},
  {SkillBase::STONE_TOUCH, "Stoning Touch"},
  {SkillBase::IMMOLATION, "Immolation"},
  {SkillBase::THROW_WEAPON, "Throw Weapon"},
  
  {SkillBase::IMMUNITY_POISON, "Poison Immunity"},
  {SkillBase::IMMUNITY_FIRE, "Fire Immunity"},
  {SkillBase::IMMUNITY_STONING, "Stoning Immunity"},
  {SkillBase::IMMUNITY_WEAPON, "Weapon Immunity"},
  {SkillBase::IMMUNITY_MISSILE, "Missiles Immunity"},
  {SkillBase::IMMUNITY_ILLUSIONS, "Illusions Immunity"},
  {SkillBase::IMMUNITY_COLD, "Cold Immunity"},
  {SkillBase::IMMUNITY_MAGIC, "Magic Immunity"},
  {SkillBase::IMMUNITY_DEATH, "Death Immunity"},
  
  {SkillBase::MELD, "Meld With Node"},
  {SkillBase::PURIFY, "Purify"},
  {SkillBase::CREATE_OUTPOST, "Create Outpost"},
  {SkillBase::CREATE_ROAD, "Create Road"},
  {SkillBase::WALL_CRUSHING, "Wall Crushing"},
  
  {SkillBase::SCOUTING, "Scouting"},
  {SkillBase::TO_HIT, "To Hit"},
  {SkillBase::LUCKY, "Lucky"},
  {SkillBase::HOLY_BONUS, "Holy Bonus"},
  {SkillBase::RESISTANCE_TO_ALL, "Resistance to All"},
  {SkillBase::HEALER, "Healer"},
  
  {SkillBase::FORESTWALK, "Forester"},
  {SkillBase::FLYING, "Flight"},
  {SkillBase::UNDERGROUND, "Merging"},
  {SkillBase::MOUNTAINWALK, "Mountaineer"},
  {SkillBase::NON_CORPOREAL, "Non Corporeal"},
  {SkillBase::PATH_FINDER, "Path Finding"},
  {SkillBase::PLANAR_TRAVEL, "Plane Shift"},
  {SkillBase::TELEPORT, "Teleport"},
  {SkillBase::SWIMMING, "Waterwalking"},
  {SkillBase::WINDWALK, "Windwalking"},
  {SkillBase::SAILING, "Sailing"},
  {SkillBase::DESERTWALK, "Sandwalk"},
  {SkillBase::SWAMPWALK, "Swampwalking"},
  
  {SkillBase::SPELL_ELDRITCH_WEAPON, "Eldritch Weapon"},
  {SkillBase::SPELL_FLAME_BLADE, "Flame blade"},
  {SkillBase::SPELL_CHAOS_CHANNELS_SHIELD, "Chaos Channels"},
  {SkillBase::SPELL_CHAOS_CHANNELS_WINGS, "Chaos Channels"},
  {SkillBase::SPELL_CHAOS_CHANNELS_BREATH, "Chaos Channels"},
  
  {SkillBase::SPELL_RESIST_ELEMENTS, "Resist Elements"},
  {SkillBase::SPELL_STONE_SKIN, "Stone Skin"},
  {SkillBase::SPELL_GIANT_STRENGTH, "Giant Strength"},
  
  {SkillBase::SPELL_ELEMENTAL_ARMOR, "Elemental Armor"},
  {SkillBase::SPELL_IRON_SKIN, "Iron Skin"},
  {SkillBase::SPELL_REGENERATION, "Regeneration"},
  
  {SkillBase::SPELL_RESIST_MAGIC, "Resist Magic"},
  {SkillBase::SPELL_GUARDIAN_WIND, "Guardian Wind"},
  {SkillBase::SPELL_SPELL_LOCK, "Spell Lock"},
  {SkillBase::SPELL_INVISIBILITY, "Invisibility"},
  {SkillBase::SPELL_HASTE, "Haste"},
  
  {SkillBase::SPELL_HOLY_WEAPON, "Holy Weapon"},
  {SkillBase::SPELL_BLESS, "Bless"},
  {SkillBase::SPELL_ENDURANCE, "Endurance"},
  {SkillBase::SPELL_TRUE_SIGHT, "True Sight"},
  {SkillBase::SPELL_HEROISM, "Heroism"},
  {SkillBase::SPELL_HOLY_ARMOR, "Holy Armor"},
  {SkillBase::SPELL_LION_HEART, "Lion Heart"},
  {SkillBase::SPELL_INVULNERABILITY, "Invulnerability"},
  {SkillBase::SPELL_RIGHTEOUSNESS, "Righteousness"},
  
  {SkillBase::SPELL_UNDEAD, "Undead"},
  
  {SkillBase::SPELL_CLOAK_OF_FEAR, "Cloak of Fear"},
  {SkillBase::SPELL_BLACK_CHANNELS, "Black Channels"},
  {SkillBase::SPELL_WRAITH_FORM, "Wraith Form"},
  {SkillBase::SPELL_BERSERK, "Berserk"},
  
  {SkillBase::MITHRIL_WEAPONS, "Mithril Weapons"},
  {SkillBase::ADAMANTIUM_WEAPONS, "Adamantium Weapons"},
  
  
  {SkillBase::STATUS_WEB, "Web"},
  {SkillBase::STATUS_CONFUSION, "Confusion"},
  {SkillBase::STATUS_VERTIGO, "Vertigo"},
  {SkillBase::STATUS_HIT_PENALTY, "To Hit"},
  {SkillBase::STATUS_SHATTER, "Shatter"},
  {SkillBase::STATUS_WARP_CREATURE_DEFENSE, "Warp Creature"},
  {SkillBase::STATUS_WARP_CREATURE_ATTACK, "Warp Creature"},
  {SkillBase::STATUS_WARP_CREATURE_MAGIC_DEFENSE, "Warp Creature"},
  {SkillBase::STATUS_BLACK_SLEEP, "Black Sleep"},
  {SkillBase::STATUS_WEAKNESS, "Weakness"},
  {SkillBase::STATUS_POSESSION, "Posession"}
};

