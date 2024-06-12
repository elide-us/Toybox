#pragma once

#include "pch.h"
/*
class BaseCharacterSheet
{
// Public (will be Property) that provides the calculated value from the stack of base + level + item + buff - debuff
public:
	float Str{};
	float Dex{};
	float Int{};
	float Sta{};

	float DamagePhysical{}; // Depending on class, can be derived from statistic + weapon item + buff - debuff
	float DamageMagical{}; // Depending on class, can be derived from statistic + spell item + buff - debuff
	float DamageTrue{};

	float ResistPhysical{}; // Derived from Items, ability to absorb (mitigate) damage
	float ResistDodge{}; // Derived from Dex, ability to avoid damage
	float ResistBlock{}; // Derived from Str, ability to absorb (mitigate) damage
	float ResistMagical{}; // aka "Magic Resist", can be derived from statistics + equipment items + buffs - debuffs

	float PenetrationArmor{};
	float PenetrationMagical{};

	float AttackSpeed{}; // Derived from Items
	float HitChancePhysical{}; // Derived from Dex
	float HitChanceMagical{}; // Derived from Int

	float MoveSpeed{}; // Base statistic + equipment items + buffs - debuffs
	int CharacterLevel{}; // Base statistic, starts at 0 + 1st level
	BaseCharacterEnums::CharacterRace CharacterRace{}; // Can have interactions with magic types, eg. undead vs holy
	BaseCharacterEnums::CharacterLanguage CharacterLanguage{};
	BaseCharacterEnums::CharacterClass CharacterClass{}; // Index for now, but will be a champion type
	BaseCharacterEnums::CharacterAlignment CharacterAlignment{};
	//BaseCharacterEnums::CharacterDiety CharacterDiety{};

private:
	// Base statistics
	float m_strBase; // Physical damage
	float m_dexBase; // Attack and defense skill
	float m_intBase; // Magic skill, mana and magic damage
	float m_staBase; // Health
					 
	// levelBuffs stack<LevelBuff>
	// levelDebuffs stack<LevelDebuff>
	// itemBuffs stack<ItemBuff>
	// itemDebuffs stack<ItemDebuff>
	// spellBuffs stack<SpellBuff>
	// spellDebuffs stack<SpellDebuff>
};

class BaseCharacterItems
{
public:
	float BonusStr;
	float BonusDex;
	float BonusInt;
	float BonusCha;
private:
	int itemHelm;
	int itemShoulders;
	int itemArms;
	int itemHands;

	int itemChest;
	int itemBelt;
	int itemLegs;
	int itemBoots;

	int itemLeftRing;
	int itemRightRing;
	int itemNeck;
	int itemBack;

	int itemAmmunition;
	int itemPrimary;
	int itemSecondary;
};

namespace BaseCharacterEnums
{
	enum CharacterRace
	{
		Human,
		// Erudite,
		// Oriental,
		// Amazonian,
		// Aboriginal,
		HighElf,
		WoodElf,
		DarkElf,
		HalfElf,
		Dwarf,
		Halfling,
		Gnome,
		Ogre,
		Cyclopse, // ogre
		Goblin,
		Hobgoblin, // goblin
		Orc,
		Kobold, // orcish
		Angel,
		Demon,
		Undead, // common
		Werewolf, // common
		Vampire,
		Centaur,
		Hippogryph, // angelic
		Minotaur, // demonic
		Wyvern, // dragon
		Dragon,
		Treant, // entish
		Diety,
		Lizardman,
		Frogloc,
		Elementals // entish
	};
	
	enum CharacterClass
	{
		Warrior,
		Knight,
		DeathKnight,
		Monk,
		Hoplite,
		Brawler,
		Rogue,
		Assassin, // Ninja
		Swashbuckler,
		Tinkerer,
		Psionicist,
		Archer,
		Beastmaster, // pets
		Wizard,
		Enchanter,
		Mage, //pets
		Necromancer,
		Priest,
		Cleric,
		Shaman, // Good alignment
		Druid,
		Headhunter // Evil alignment shaman (voodoo)
	};

	enum CharacterAlignment
	{
		LawfulGood,
		LawfulNeutral,
		LawfulEvil,
		NeutralGood,
		TrueNeutral,
		NeutralEvil,
		ChaoticGood,
		ChaoticNeutral,
		ChaoticEvil
	};

	enum CharacterLanguage
	{
		Common,
		HighElvish,
		WoodElvish,
		DarkElvish,
		Dwarvish,
		Orcish,
		Goblin,
		Angelic,
		Demonic,
		Centaurian,
		Gnomish,
		Vampirish,
		Ogre, // ?
		Dragon,
		Entish
	};
}
*/