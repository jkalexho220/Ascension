/* regular drop pool */
const int RELIC_HEALTH = 1;
const int RELIC_SPEED = 2;
const int RELIC_ATTACK_RANGE = 3;
const int RELIC_SPELL_RANGE = 4;
const int RELIC_SPELL_DURATION = 5;
const int RELIC_LIFESTEAL = 6;
const int RELIC_ARMOR = 7;
const int RELIC_ATTACK_DAMAGE = 8;
const int RELIC_SPELL_POWER = 9;
const int RELIC_HEAL_BOOST = 10;

/* boss drop pool */
const int RELIC_POISON_RESISTANCE = 11;
const int RELIC_ULTIMATE_COST = 12;
const int RELIC_SILENCE_RESISTANCE = 13;
const int RELIC_PROJECTILES = 14;
const int RELIC_STUN_RESISTANCE = 15;
const int RELIC_SPECIAL_ATTACK = 16;
const int RELIC_POISON_KILLER = 17;
const int RELIC_COOLDOWN_REDUCTION = 18;
const int RELIC_WEIGHTED_BLOWS = 19;
const int RELIC_ALL = 20;

/*
auras
pet companion?
*/
const int RELIC_FAVOR_SPECIAL = 21; // when your special attack triggers, all allies gain 1 favor
const int RELIC_PET_DOG = 22; // Bella fights for you
const int RELIC_NOTTUD = 23; // +0.2x area damage on attacks. (Radius 4)
const int RELIC_ZENOPHOBIA = 24; // +0.2 magic penetration
const int RELIC_YEEBAAGOOON = 25; // Regen 0.3 favor per second


/* nottud's shop */
const int RELIC_CURSED_RANGE = 26;
const int RELIC_CURSED_DURATION = 27;
const int RELIC_CURSED_POWER = 28;
const int RELIC_FAVOR_FROM_ATTACKS = 29;
const int RELIC_POISON_FASTER = 30;

const int NORMAL_RELICS = 30;
/* key relics */

const int RELIC_NOTTUD_TICKET = 35;

const int RELIC_NICKONHAWK = 36;
const int RELIC_NICKONHAWK_TICKET = 37;

const int RELIC_GHOST_PICTURE = 38;

const int RELIC_MATH_PROBLEM = 40;
/*
reserved to 60
*/
const int RELIC_MATH_PROBLEM_END = 60;

const int RELIC_LITERAL_FECES = 61;
const int RELIC_BERRY_BUSH = 62;
const int RELIC_A_FUCKING_CORPSE = 63;

const int RELIC_POISON_BUCKET = 64;

const int RELIC_WORTHLESS_JUNK = 65;
const int RELIC_MAGIC_DETECTOR = 66;

const int RELIC_BINARY_POISON = 67;

const int KEY_RELICS = 100;
const int RELIC_KEY_GREEK = 101;
const int RELIC_KEY_NORSE = 102;
const int RELIC_KEY_ATLANTEAN = 103;
const int RELIC_KEY_CHINA = 104;
const int RELIC_KEY_EGYPT = 105;

string relicName(int relic = 0) {
	string msg = "WTF That's not a relic!";
	if ((relic >= RELIC_KEY_GREEK) && (relic <= RELIC_KEY_EGYPT)) {
		msg = "A key that opens something";
	} else if ((relic >= RELIC_MATH_PROBLEM) && (relic <= RELIC_MATH_PROBLEM_END)) {
		msg = "" + (relic - RELIC_MATH_PROBLEM);
	} else {
		switch(relic)
		{
			case RELIC_HEALTH:
			{
				msg = "Health +300";
			}
			case RELIC_SPEED:
			{
				msg = "Speed +1";
			}
			case RELIC_ATTACK_RANGE:
			{
				msg = "Attack Range and LOS +5";
			}
			case RELIC_SPELL_RANGE:
			{
				msg = "Spell Range +0.3x";
			}
			case RELIC_SPELL_DURATION:
			{
				msg = "Spell Duration +0.3x";
			}
			case RELIC_LIFESTEAL:
			{
				msg = "Lifesteal +0.2 (Half for spells)";
			}
			case RELIC_ARMOR:
			{
				msg = "Armor +0.2x";
			}
			case RELIC_ATTACK_DAMAGE:
			{
				msg = "Attack +15";
			}
			case RELIC_SPELL_POWER:
			{
				msg = "Spell Power +0.3x";
			}
			case RELIC_HEAL_BOOST:
			{
				msg = "Healing Effects +0.5x";
			}
			case RELIC_POISON_RESISTANCE:
			{
				msg = "Poison resistance x0.5";
			}
			case RELIC_STUN_RESISTANCE:
			{
				msg = "Stun resistance x0.5";
			}
			case RELIC_SILENCE_RESISTANCE:
			{
				msg = "Silence resistance x0.5";
			}
			case RELIC_POISON_KILLER:
			{
				msg = "+0.5x damage to poisoned enemies.";
			}
			case RELIC_WEIGHTED_BLOWS:
			{
				msg = "Your stuns deal damage equal to 10 percent of your health.";
			}
			case RELIC_ALL:
			{
				msg = "Spell Power, Duration, Range +0.12";
			}
			case RELIC_ULTIMATE_COST:
			{
				msg = "Ultimate Cost Reduction x0.12";
			}
			case RELIC_PROJECTILES:
			{
				msg = "Projectiles +1";
			}
			case RELIC_SPECIAL_ATTACK:
			{
				msg = "Special Attack Cooldown -1";
			}
			case RELIC_COOLDOWN_REDUCTION:
			{
				msg = "Cooldown Reduction x0.15";
			}
			case RELIC_CURSED_RANGE:
			{
				msg = "Spell Range +0.5 | Spell Power -0.3";
			}
			case RELIC_CURSED_DURATION:
			{
				msg = "Spell Duration +0.5 | Spell Range -0.3";
			}
			case RELIC_CURSED_POWER:
			{
				msg = "Spell Power +0.5 | Spell Duration -0.3";
			}
			case RELIC_FAVOR_FROM_ATTACKS:
			{
				msg = "When you attack, gain 1 favor. Ultimate Cost Increased x0.12";
			}
			case RELIC_POISON_FASTER:
			{
				msg = "Poison damage doubled but duration halved";
			}
			case RELIC_YEEBAAGOOON:
			{
				msg = "Regenerate 0.3 favor per second";
			}
			case RELIC_ZENOPHOBIA:
			{
				msg = "+0.2 ignore enemy magic resist";
			}
			case RELIC_NOTTUD:
			{
				msg = "+0.3x area damage on attacks. (Radius 4)";
			}
			case RELIC_PET_DOG:
			{
				msg = "A pet dog fights for you. (Respawn = 30 seconds)";
			}
			case RELIC_FAVOR_SPECIAL:
			{
				msg = "When your special attack activates, all allies gain 1 favor";
			}
			case RELIC_GHOST_PICTURE:
			{
				msg = "A picture of a young man.";
			}
			case RELIC_NICKONHAWK:
			{
				msg = "Help! Someone help me!";
			}
			case RELIC_NICKONHAWK_TICKET:
			{
				msg = "Golden Ticket";
			}
			case RELIC_NOTTUD_TICKET:
			{
				msg = "A signed copy of Gladiator Worlds";
			}
			
			case RELIC_LITERAL_FECES:
			{
				msg = "Literal feces";
			}
			
			case RELIC_A_FUCKING_CORPSE:
			{
				msg = "A fucking corpse";
			}
			
			case RELIC_BERRY_BUSH:
			{
				msg = "A berry bush";
			}
			
			case RELIC_POISON_BUCKET:
			{
				msg = "A bucket for holding a liquid";
			}
			
			case RELIC_WORTHLESS_JUNK:
			{
				msg = "Worthless junk";
			}
			case RELIC_MAGIC_DETECTOR:
			{
				msg = "Magic Detector";
			}
			case RELIC_BINARY_POISON:
			{
				msg = "Mysterious goblet";
			}
		}
	}
	return(msg);
}

string relicIcon(int relic = 0) {
	string icon = "icons\infantry g hoplite icon 64";
	if ((relic >= RELIC_MATH_PROBLEM) && (relic <= RELIC_MATH_PROBLEM_END)) {
		icon = "icons\god power audrey icons 64";
	} else {
		switch(relic)
		{
			case RELIC_HEALTH:
			{
				icon = "icons\special g sea turtle icon 64";
			}
			case RELIC_SPEED:
			{
				icon = "icons\special g pegasus icon 64";
			}
			case RELIC_ATTACK_RANGE:
			{
				icon = "icons\cavalry e chariot archer icon 64";
			}
			case RELIC_SPELL_RANGE:
			{
				icon = "icons\special e petosuchus icon 64";
			}
			case RELIC_SPELL_DURATION:
			{
				icon = "icons\special n frost giant icon 64";
			}
			case RELIC_LIFESTEAL:
			{
				icon = "icons\special n troll icon 64";
			}
			case RELIC_STUN_RESISTANCE:
			{
				icon = "icons\special e Scarab icon 64";
			}
			case RELIC_ATTACK_DAMAGE:
			{
				icon = "icons\siege n ballista icon 64";
			}
			case RELIC_SPELL_POWER:
			{
				icon = "icons\special n fire giant icon 64";
			}
			case RELIC_HEAL_BOOST:
			{
				icon = "icons\special x flying medic icons 64";
			}
			case RELIC_POISON_RESISTANCE:
			{
				icon = "icons\god power dryad icons 64";
			}
			case RELIC_ARMOR:
			{
				icon = "icons\special x behemoth icons 64";
			}
			case RELIC_SILENCE_RESISTANCE:
			{
				icon = "icons\special e sphinx icon 64";
			}
			case RELIC_POISON_KILLER:
			{
				icon = "icons\special e mummy icon 64";
			}
			case RELIC_WEIGHTED_BLOWS:
			{
				icon = "icons\special g medusa icon 64";
			}
			case RELIC_ALL:
			{
				icon = "icons\special g chimera icon 64";
			}
			case RELIC_ULTIMATE_COST:
			{
				icon = "icons\special g arkantos icon 64";
			}
			case RELIC_PROJECTILES:
			{
				icon = "icons\special g manticore icon 64";
			}
			case RELIC_SPECIAL_ATTACK:
			{
				icon = "icons\special e avenger icon 64";
			}
			case RELIC_COOLDOWN_REDUCTION:
			{
				icon = "icons\special x argus icons 64";
			}
			case RELIC_CURSED_RANGE:
			{
				icon = "icons\siege g petrobolos icon 64";
			}
			case RELIC_CURSED_DURATION:
			{
				icon = "icons\scenario x folstag icons 64";
			}
			case RELIC_CURSED_POWER:
			{
				icon = "icons\special e phoenix icon 64";
			}
			case RELIC_FAVOR_FROM_ATTACKS:
			{
				icon = "icons\special x servant icons 64";
			}
			case RELIC_POISON_FASTER:
			{
				icon = "icons\special e scorpionman icon 64";
			}
			case RELIC_YEEBAAGOOON:
			{
				icon = "icons\special e son of osiris icon 64";
			}
			case RELIC_KEY_GREEK:
			{
				icon = "ui range indicator greek";
			}
			case RELIC_KEY_NORSE:
			{
				icon = "ui range indicator norse";
			}
			case RELIC_KEY_ATLANTEAN:
			{
				icon = "ui range indicator atlantean";
			}
			case RELIC_KEY_CHINA:
			{
				icon = "ui range indicator chinese";
			}
			case RELIC_KEY_EGYPT:
			{
				icon = "ui range indicator egypt";
			}
			case RELIC_GHOST_PICTURE:
			{
				icon = "icons\special g kastor icon 64";
			}
			case RELIC_LITERAL_FECES:
			{
				icon = "icons\special x promethean icons 64";
			}
			case RELIC_A_FUCKING_CORPSE:
			{
				icon = "icons\special e minion icon 64";
			}
			case RELIC_BERRY_BUSH:
			{
				icon = "icons\world berry bush icon 64";
			}
			case RELIC_POISON_BUCKET:
			{
				icon = "icons\special c jiangshi icon";
			}
			case RELIC_WORTHLESS_JUNK:
			{
				icon = "icons\boat c junk icon";
			}
			case RELIC_MAGIC_DETECTOR:
			{
				icon = "icons\siege g helepolis icon 64";
			}
			case RELIC_NICKONHAWK:
			{
				icon = "icons\trade x caravan icons 64";
			}
			case RELIC_NICKONHAWK_TICKET:
			{
				icon = "icons\hero g odysseus icon 64";
			}
			case RELIC_NOTTUD_TICKET:
			{
				icon = "icons\infantry x oracle hero icons 64";
			}
			case RELIC_PET_DOG:
			{
				icon = "icons\Animal Wolf icon 64";
			}
			case RELIC_NOTTUD:
			{
				icon = "icons\special g minotaur icon 64";
			}
			case RELIC_FAVOR_SPECIAL:
			{
				icon = "icons\special x lampades icons 64";
			}
			case RELIC_BINARY_POISON:
			{
				icon = "icons\improvement life drain icon";
			}
		}
	}
	return(icon);
}

void relicDescription(int relic = 0) {
	string icon = relicIcon(relic);
	string msg = relicName(relic);
	trShowImageDialog(icon, msg);
}

float calculateArmor(float start = 0, float armor = 0) {
	float remaining = 1.0 - start;
	float reduction = remaining * (1.0 - armor);
	return(start + remaining - reduction);
}

void relicEffect(int relic = 0, int p = 0, bool equip = true) {
	int db = 0;
	int n = 0;
	float m = 1.0;
	if (equip == false) {
		m = -1.0;
	}
	int index = xGetPointer(dPlayerData);
	xSetPointer(dPlayerData,p);
	int class = xGetInt(dPlayerData,xPlayerClass);
	string proto = kbGetProtoUnitName(xGetInt(dClass,xClassProto,class));
	switch(relic)
	{
		case RELIC_HEALTH:
		{
			xSetFloat(dPlayerData,xPlayerHealth,xGetFloat(dPlayerData,xPlayerHealth) + 300.0 * m);
			zSetProtoUnitStat(proto, p, 0, xGetFloat(dPlayerData,xPlayerHealth));
		}
		case RELIC_SPEED:
		{
			xSetFloat(dPlayerData,xPlayerSpeed,xGetFloat(dPlayerData,xPlayerSpeed) + 1.0 * m);
			zSetProtoUnitStat(proto, p, 1, xGetFloat(dPlayerData,xPlayerSpeed));
		}
		case RELIC_ATTACK_RANGE:
		{
			xSetFloat(dPlayerData,xPlayerRange,xGetFloat(dPlayerData,xPlayerRange) + 5.0 * m);
			xSetFloat(dPlayerData,xPlayerLos,xGetFloat(dPlayerData,xPlayerLos) + 5.0 * m);
			zSetProtoUnitStat(proto, p, 11, xGetFloat(dPlayerData, xPlayerRange));
			zSetProtoUnitStat(proto, p, 2, xGetFloat(dPlayerData, xPlayerLos));
			zSetProtoUnitStat("Revealer to Player", p, 2, xGetFloat(dPlayerData,xPlayerLos));
		}
		case RELIC_SPELL_RANGE:
		{
			xSetFloat(dPlayerData,xPlayerSpellRangeTrue,xGetFloat(dPlayerData,xPlayerSpellRangeTrue) + 0.3 * m);
		}
		case RELIC_SPELL_DURATION:
		{
			xSetFloat(dPlayerData,xPlayerSpellDurationTrue,xGetFloat(dPlayerData,xPlayerSpellDurationTrue) + 0.3 * m);
		}
		case RELIC_LIFESTEAL:
		{
			xSetFloat(dPlayerData,xPlayerLifesteal,xGetFloat(dPlayerData,xPlayerLifesteal) + 0.2 * m);
		}
		case RELIC_ARMOR:
		{
			if (m == 1) {
				xSetFloat(dPlayerData,xPlayerMagicResist,calculateArmor(xGetFloat(dPlayerData,xPlayerMagicResist),0.2));
				xSetFloat(dPlayerData,xPlayerPhysicalResist,calculateArmor(xGetFloat(dPlayerData,xPlayerPhysicalResist),0.2));
			} else {
				xSetFloat(dPlayerData,xPlayerMagicResist,calculateArmor(xGetFloat(dPlayerData,xPlayerMagicResist),-0.25));
				xSetFloat(dPlayerData,xPlayerPhysicalResist,calculateArmor(xGetFloat(dPlayerData,xPlayerPhysicalResist),-0.25));
			}
			trModifyProtounit(proto, p, 24, -1);
			trModifyProtounit(proto, p, 25, -1);
			trModifyProtounit(proto, p, 26, -1);
			trModifyProtounit(proto, p, 24, xGetFloat(dPlayerData,xPlayerPhysicalResist));
			trModifyProtounit(proto, p, 25, xGetFloat(dPlayerData,xPlayerPhysicalResist));
			trModifyProtounit(proto, p, 26, xGetFloat(dPlayerData,xPlayerPhysicalResist));
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				if ((xGetInt(dPlayerUnits,xPlayerOwner) == p) && xGetBool(dPlayerUnits,xIsHero)) {
					xSetFloat(dPlayerUnits,xPhysicalResist,xGetFloat(dPlayerData,xPlayerPhysicalResist));
					xSetFloat(dPlayerUnits,xMagicResist,xGetFloat(dPlayerData,xPlayerMagicResist));
				}
			}
		}
		case RELIC_ATTACK_DAMAGE:
		{
			xSetFloat(dPlayerData,xPlayerBaseAttackTrue,xGetFloat(dPlayerData,xPlayerBaseAttackTrue) + 15.0 * m);
			xSetFloat(dPlayerData,xPlayerBaseAttack,xGetFloat(dPlayerData,xPlayerBaseAttackTrue));
			xSetFloat(dPlayerData,xPlayerAttack,xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 27, xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 31, xGetFloat(dPlayerData,xPlayerBaseAttack));
		}
		case RELIC_SPELL_POWER:
		{
			xSetFloat(dPlayerData,xPlayerSpellDamageTrue,xGetFloat(dPlayerData,xPlayerSpellDamageTrue) + 0.3 * m);
		}
		case RELIC_HEAL_BOOST:
		{
			xSetFloat(dPlayerData,xPlayerHealBoost,xGetFloat(dPlayerData,xPlayerHealBoost) + 0.5 * m);
		}
		case RELIC_POISON_RESISTANCE:
		{
			xSetInt(dPlayerData,xPlayerPoisonResistanceCount,xGetInt(dPlayerData,xPlayerPoisonResistanceCount) + 1 * m);
			xSetFloat(dPlayerData,xPlayerPoisonResistance,xsPow(0.5, xGetInt(dPlayerData,xPlayerPoisonResistanceCount)));
		}
		case RELIC_STUN_RESISTANCE:
		{
			xSetInt(dPlayerData,xPlayerStunResistanceCount,xGetInt(dPlayerData,xPlayerStunResistanceCount) + 1 * m);
			xSetFloat(dPlayerData,xPlayerStunResistance,xsPow(0.5, xGetInt(dPlayerData,xPlayerStunResistanceCount)));
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			xSetInt(dPlayerData,xPlayerSilenceResistanceCount,xGetInt(dPlayerData,xPlayerSilenceResistanceCount) + 1 * m);
			xSetFloat(dPlayerData,xPlayerSilenceResistance,xsPow(0.5, xGetInt(dPlayerData,xPlayerSilenceResistanceCount)));
		}
		case RELIC_POISON_KILLER:
		{
			xSetFloat(dPlayerData,xPlayerPoisonKiller,xGetFloat(dPlayerData,xPlayerPoisonKiller) + 0.5 * m);
		}
		case RELIC_WEIGHTED_BLOWS:
		{
			xSetFloat(dPlayerData,xPlayerStunDamage,xGetFloat(dPlayerData,xPlayerStunDamage) + 0.1 * m);
		}
		case RELIC_ALL:
		{
			xSetFloat(dPlayerData,xPlayerSpellRangeTrue,xGetFloat(dPlayerData,xPlayerSpellRangeTrue) + 0.12 * m);
			xSetFloat(dPlayerData,xPlayerSpellDamageTrue,xGetFloat(dPlayerData,xPlayerSpellDamageTrue) + 0.12 * m);
			xSetFloat(dPlayerData,xPlayerSpellDurationTrue,xGetFloat(dPlayerData,xPlayerSpellDurationTrue) + 0.12 * m);
		}
		case RELIC_ULTIMATE_COST:
		{
			xSetInt(dPlayerData,xPlayerUltimateCostCount,xGetInt(dPlayerData,xPlayerUltimateCostCount) + 1 * m);
			xSetFloat(dPlayerData,xPlayerUltimateCost, xsPow(0.88, xGetInt(dPlayerData,xPlayerUltimateCostCount)));
		}
		case RELIC_PROJECTILES:
		{
			xSetInt(dPlayerData,xPlayerProjectiles,xGetInt(dPlayerData,xPlayerProjectiles) + 1 * m);
			zSetProtoUnitStat(proto, p, 13, xGetInt(dPlayerData,xPlayerProjectiles));
		}
		case RELIC_SPECIAL_ATTACK:
		{
			xSetInt(dPlayerData,xPlayerSpecialAttackCooldown,xGetInt(dPlayerData,xPlayerSpecialAttackCooldown) - 1 * m);
		}
		case RELIC_COOLDOWN_REDUCTION:
		{
			xSetInt(dPlayerData,xPlayerCooldownReductionCount,xGetInt(dPlayerData,xPlayerCooldownReductionCount) + 1 * m);
			xSetFloat(dPlayerData,xPlayerCooldownReduction,xsPow(0.85, xGetInt(dPlayerData,xPlayerCooldownReductionCount)));
		}
		case RELIC_CURSED_RANGE:
		{
			xSetFloat(dPlayerData,xPlayerSpellRangeTrue,xGetFloat(dPlayerData,xPlayerSpellRangeTrue) + 0.5 * m);
			xSetFloat(dPlayerData,xPlayerSpellDamageTrue,xGetFloat(dPlayerData,xPlayerSpellDamageTrue) - 0.3 * m);
		}
		case RELIC_CURSED_DURATION:
		{
			xSetFloat(dPlayerData,xPlayerSpellDurationTrue,xGetFloat(dPlayerData,xPlayerSpellDurationTrue) + 0.5 * m);
			xSetFloat(dPlayerData,xPlayerSpellRangeTrue,xGetFloat(dPlayerData,xPlayerSpellRangeTrue) - 0.3 * m);
		}
		case RELIC_CURSED_POWER:
		{
			xSetFloat(dPlayerData,xPlayerSpellDamageTrue,xGetFloat(dPlayerData,xPlayerSpellDamageTrue) + 0.5 * m);
			xSetFloat(dPlayerData,xPlayerSpellDurationTrue,xGetFloat(dPlayerData,xPlayerSpellDurationTrue) - 0.3 * m);
		}
		case RELIC_FAVOR_FROM_ATTACKS:
		{
			xSetInt(dPlayerData,xPlayerFavorFromAttacks,xGetInt(dPlayerData,xPlayerFavorFromAttacks) + m);
			xSetInt(dPlayerData,xPlayerUltimateCostCount,xGetInt(dPlayerData,xPlayerUltimateCostCount) - 1 * m);
			xSetFloat(dPlayerData,xPlayerUltimateCost,xsPow(0.88, xGetInt(dPlayerData,xPlayerUltimateCostCount)));
		}
		case RELIC_POISON_FASTER:
		{
			xSetInt(dPlayerData,xPlayerPoisonSpeed,xGetInt(dPlayerData,xPlayerPoisonSpeed) + 1 * m);
		}
		case RELIC_YEEBAAGOOON:
		{
			xSetInt(dPlayerData,xPlayerRegenerateFavorLast,trTimeMS());
			xSetFloat(dPlayerData,xPlayerFavorRegen,xGetFloat(dPlayerData,xPlayerFavorRegen) + 0.3 * m);
		}
		case RELIC_MAGIC_DETECTOR:
		{
			xSetInt(dPlayerData,xPlayerRegenerateFavorLast,trTimeMS());
			xSetFloat(dPlayerData,xPlayerFavorRegen,xGetFloat(dPlayerData,xPlayerFavorRegen) - m);
			if (trQuestVarGet("questActive") > 0) {
				if (m > 0) {
					trQuestVarSet("magicDetectorHolder", p);
				} else {
					trQuestVarSet("magicDetectorHolder", 0);
				}
			}
		}
		case RELIC_POISON_BUCKET:
		{
			trQuestVarSet("p"+p+"poisonBucket", m);
			if (m > 0) {
				trQuestVarSet("poisonBucketHolder", p);
			} else {
				trQuestVarSet("poisonBucketHolder", 0);
			}
		}
		case RELIC_NICKONHAWK:
		{
			trQuestVarSet("p"+p+"nickEquipped", trQuestVarGet("p"+p+"nickEquipped") + m);
			if ((trQuestVarGet("p"+p+"nickEquipped") > 1) ||
				(trQuestVarGet("p"+p+"nickQuestProgress") * trQuestVarGet("p"+p+"nickEquipped") >= 5)) {
				/* No duplicates */
				trQuestVarSet("p"+p+"nickEquipped", trQuestVarGet("p"+p+"nickEquipped") - 1);
				db = getRelicsDB(p);
				xSetPointer(db, xGetNewestPointer(db));
				for(x=xGetDatabaseCount(db); > 0) {
					if (xGetInt(db, xRelicType) == RELIC_NICKONHAWK) {
						trUnitSelectClear();
						trUnitSelect(""+xGetInt(db, xUnitName),true);
						trUnitChangeProtoUnit("Relic");
						xAddDatabaseBlock(dFreeRelics);
						xSetPointer(dFreeRelics,xGetNewestPointer(dFreeRelics));
						xSetInt(dFreeRelics,xRelicName,xGetInt(db,xRelicName));
						xSetInt(dFreeRelics,xRelicType,RELIC_NICKONHAWK);
						xFreeDatabaseBlock(db);
						break;
					} else {
						xDatabaseNext(db);
					}
				}
			} else if (trQuestVarGet("p"+p+"nickQuestProgress") == 0) {
				trQuestVarSet("p"+p+"nickQuestProgress", 1);
			}
		}
		case RELIC_NICKONHAWK_TICKET:
		{
			trQuestVarSet("p"+p+"equippedGoggles", trQuestVarGet("p"+p+"equippedGoggles") + m);
		}
		case RELIC_NOTTUD_TICKET:
		{
			trQuestVarSet("p"+p+"gladiatorWorlds", trQuestVarGet("p"+p+"gladiatorWorlds") + m);
		}
		case RELIC_PET_DOG:
		{
			xSetInt(dPlayerData,xPlayerPetDogs,xGetInt(dPlayerData,xPlayerPetDogs) + m);
		}
		case RELIC_ZENOPHOBIA:
		{
			xSetFloat(dPlayerData,xPlayerMagicPen,xGetFloat(dPlayerData,xPlayerMagicPen) + 0.2 * m);
		}
		case RELIC_NOTTUD:
		{
			xSetFloat(dPlayerData,xPlayerCleave,xGetFloat(dPlayerData,xPlayerCleave) + 0.3 * m);
		}
		case RELIC_FAVOR_SPECIAL:
		{
			xSetFloat(dPlayerData,xPlayerFavorSpecial,xGetFloat(dPlayerData,xPlayerFavorSpecial) + m);
		}
		case RELIC_BINARY_POISON:
		{
			if (equip) {
				aiPlanSetUserVariableInt(ARRAYS,1*trQuestVarGet("doomedPlayers"),1*trQuestVarGet("doomedPlayerCount"),p);
				trQuestVarSet("doomedPlayerCount", 1 + trQuestVarGet("doomedPlayerCount"));
			}
		}
	}
	if ((relic >= RELIC_KEY_GREEK) && (relic <= RELIC_KEY_EGYPT) && (trCurrentPlayer() == p) && equip) {
		trChatSend(0, "You have picked up a key. <icon=(20)("+relicIcon(relic)+")>");
	} else {
		/* certain classes have special interactions with stats */
		trEventFire(5000 + 12 * xGetInt(dPlayerData,xPlayerClass) + p);
	}
	zSetProtoUnitStat("Victory Marker", p, 0, xGetFloat(dPlayerData,xPlayerHealth));
	xSetFloat(dPlayerData,xPlayerSpellRange,xsMax(0.1,xGetFloat(dPlayerData,xPlayerSpellRangeTrue)));
	xSetFloat(dPlayerData,xPlayerSpellDamage,xsMax(0.1,xGetFloat(dPlayerData,xPlayerSpellDamageTrue)));
	xSetFloat(dPlayerData,xPlayerSpellDuration,xsMax(0.1,xGetFloat(dPlayerData,xPlayerSpellDurationTrue)));
	
	switch(xGetInt(dPlayerData,xPlayerGodBoon))
	{
		case BOON_SPELL_ATTACK:
		{
			xSetFloat(dPlayerData,xPlayerBaseAttack,
				xGetFloat(dPlayerData,xPlayerSpellDamage) * xGetFloat(dPlayerData,xPlayerBaseAttackTrue));
			xSetFloat(dPlayerData,xPlayerAttack,xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 27, xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 31, xGetFloat(dPlayerData,xPlayerBaseAttack));
		}
		case BOON_HEALTH_ATTACK:
		{
			xSetFloat(dPlayerData, xPlayerBaseAttack, xGetFloat(dPlayerData,xPlayerBaseAttackTrue) + 0.03 * xGetFloat(dPlayerData,xPlayerHealth));
			xSetFloat(dPlayerData,xPlayerAttack,xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 27, xGetFloat(dPlayerData,xPlayerBaseAttack));
			zSetProtoUnitStat(proto, p, 31, xGetFloat(dPlayerData,xPlayerBaseAttack));
		}
	}

	xSetPointer(dPlayerData,index);
}

int relicProto(int relic = 0) {
	int proto = kbGetProtoUnitID("Hoplite");
	if ((relic >= RELIC_MATH_PROBLEM) && (relic <= RELIC_MATH_PROBLEM_END)) {
		proto = kbGetProtoUnitID("Audrey");
	} else {
		switch(relic)
		{
			case RELIC_HEALTH:
			{
				proto = kbGetProtoUnitID("Sea Turtle");
			}
			case RELIC_SPEED:
			{
				proto = kbGetProtoUnitID("Pegasus");
			}
			case RELIC_ATTACK_RANGE:
			{
				proto = kbGetProtoUnitID("Chariot Archer");
			}
			case RELIC_SPELL_RANGE:
			{
				proto = kbGetProtoUnitID("Petsuchos");
			}
			case RELIC_SPELL_DURATION:
			{
				proto = kbGetProtoUnitID("Frost Giant");
			}
			case RELIC_LIFESTEAL:
			{
				proto = kbGetProtoUnitID("Troll");
			}
			case RELIC_STUN_RESISTANCE:
			{
				proto = kbGetProtoUnitID("Scarab");
			}
			case RELIC_ATTACK_DAMAGE:
			{
				proto = kbGetProtoUnitID("Ballista");
			}
			case RELIC_SPELL_POWER:
			{
				proto = kbGetProtoUnitID("Fire Giant");
			}
			case RELIC_HEAL_BOOST:
			{
				proto = kbGetProtoUnitID("Flying Medic");
			}
			case RELIC_POISON_RESISTANCE:
			{
				proto = kbGetProtoUnitID("Dryad");
			}
			case RELIC_ARMOR:
			{
				proto = kbGetProtoUnitID("Behemoth");
			}
			case RELIC_SILENCE_RESISTANCE:
			{
				proto = kbGetProtoUnitID("Sphinx");
			}
			case RELIC_POISON_KILLER:
			{
				proto = kbGetProtoUnitID("Mummy");
			}
			case RELIC_WEIGHTED_BLOWS:
			{
				proto = kbGetProtoUnitID("Medusa");
			}
			case RELIC_ALL:
			{
				proto = kbGetProtoUnitID("Chimera");
			}
			case RELIC_ULTIMATE_COST:
			{
				proto = kbGetProtoUnitID("Arkantos");
			}
			case RELIC_PROJECTILES:
			{
				proto = kbGetProtoUnitID("Manticore");
			}
			case RELIC_SPECIAL_ATTACK:
			{
				proto = kbGetProtoUnitID("Avenger");
			}
			case RELIC_COOLDOWN_REDUCTION:
			{
				proto = kbGetProtoUnitID("Argus");
			}
			case RELIC_KEY_GREEK:
			{
				proto = kbGetProtoUnitID("UI Range Indicator Greek SFX");
			}
			case RELIC_KEY_NORSE:
			{
				proto = kbGetProtoUnitID("UI Range Indicator Norse SFX");
			}
			case RELIC_KEY_ATLANTEAN:
			{
				proto = kbGetProtoUnitID("UI Range Indicator Atlantean SFX");
			}
			case RELIC_KEY_CHINA:
			{
				proto = kbGetProtoUnitID("UI Range Indicator Chinese SFX");
			}
			case RELIC_KEY_EGYPT:
			{
				proto = kbGetProtoUnitID("UI Range Indicator Egypt SFX");
			}
			case RELIC_CURSED_RANGE:
			{
				proto = kbGetProtoUnitID("Petrobolos");
			}
			case RELIC_CURSED_DURATION:
			{
				proto = kbGetProtoUnitID("King Folstag");
			}
			case RELIC_CURSED_POWER:
			{
				proto = kbGetProtoUnitID("Phoenix");
			}
			case RELIC_FAVOR_FROM_ATTACKS:
			{
				proto = kbGetProtoUnitID("Servant");
			}
			case RELIC_POISON_FASTER:
			{
				proto = kbGetProtoUnitID("Scorpion Man");
			}
			case RELIC_YEEBAAGOOON:
			{
				proto = kbGetProtoUnitID("Pharaoh of Osiris");
			}
			case RELIC_GHOST_PICTURE:
			{
				proto = kbGetProtoUnitID("Kastor");
			}
			case RELIC_LITERAL_FECES:
			{
				proto = kbGetProtoUnitID("Promethean");
			}
			case RELIC_A_FUCKING_CORPSE:
			{
				proto = kbGetProtoUnitID("Minion");
			}
			case RELIC_BERRY_BUSH:
			{
				proto = kbGetProtoUnitID("Berry Bush");
			}
			case RELIC_POISON_BUCKET:
			{
				proto = kbGetProtoUnitID("Jiangshi");
			}
			case RELIC_WORTHLESS_JUNK:
			{
				proto = kbGetProtoUnitID("Junk");
			}
			case RELIC_MAGIC_DETECTOR:
			{
				proto = kbGetProtoUnitID("Helepolis");
			}
			case RELIC_NICKONHAWK:
			{
				proto = kbGetProtoUnitID("Caravan Atlantean");
			}
			case RELIC_NICKONHAWK_TICKET:
			{
				proto = kbGetProtoUnitID("Hero Greek Odysseus");
			}
			case RELIC_NOTTUD_TICKET:
			{
				proto = kbGetProtoUnitID("Oracle Scout");
			}
			case RELIC_PET_DOG:
			{
				proto = kbGetProtoUnitID("Dog");
			}
			case RELIC_NOTTUD:
			{
				proto = kbGetProtoUnitID("Minotaur");
			}
			case RELIC_FAVOR_SPECIAL:
			{
				proto = kbGetProtoUnitID("Lampades");
			}
			case RELIC_BINARY_POISON:
			{
				proto = kbGetProtoUnitID("Jiangshi");
			}
		}
	}
	return(proto);
}

int randomLow(int maxval = 10) {
	trQuestVarSetFromRand("relicrand1",1,maxval, true);
	trQuestVarSetFromRand("relicrand2",1,maxval, true);
	if (trQuestVarGet("relicrand2") < trQuestVarGet("relicrand1")) {
		trQuestVarSet("relicrand1", trQuestVarGet("relicrand2"));
	}
	return(1*trQuestVarGet("relicrand1"));
}

int randomStageClosest(int maxval = 10) {
	trQuestVarSetFromRand("relicrand1",1,maxval, true);
	trQuestVarSetFromRand("relicrand2",1,maxval, true);
	float firstDiff = xsAbs(trQuestVarGet("relicrand1") - trQuestVarGet("stage"));
	float secondDiff = xsAbs(trQuestVarGet("relicrand2") - trQuestVarGet("stage"));
	if (secondDiff < firstDiff) {
		trQuestVarSet("relicrand1", trQuestVarGet("relicrand2"));
	}
	return(1*trQuestVarGet("relicrand1"));
}

void spawnRelicSpecific(vector v = vector(0,0,0), int val = 1) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(v),0,xsVectorGetZ(v),0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	if (trUnitVisToPlayer()) {
		trSoundPlayFN("relicselect.wav","1",-1,"","");
	}
	trUnitChangeProtoUnit("Relic");
	xSetPointer(dFreeRelics, xAddDatabaseBlock(dFreeRelics));
	xSetInt(dFreeRelics,xRelicName,1*trQuestVarGet("next"));
	xSetInt(dFreeRelics,xRelicType,val);
}

void spawnRelic(vector v = vector(0,0,0), int maxval = 10) {
	spawnRelicSpecific(v, randomStageClosest(maxval));
}

void spawnRelicClosest(vector v = vector(0,0,0), int target = 0) {
	trQuestVarSetFromRand("rand", 0, 10, true);
	trQuestVarSetFromRand("rand",
		xsMax(1, target - trQuestVarGet("rand")), xsMin(10, target + trQuestVarGet("rand")), true);
	spawnRelicSpecific(v, 1*trQuestVarGet("rand"));
}
