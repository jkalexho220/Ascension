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

/* nottud's shop */
const int RELIC_CURSED_RANGE = 21;
const int RELIC_CURSED_DURATION = 22;
const int RELIC_CURSED_POWER = 23;
const int RELIC_FAVOR_FROM_ATTACKS = 24;
const int RELIC_POISON_FASTER = 25;

const int RELIC_YEEBAAGOOON = 26;

const int RELIC_NOTTUD = 28;

const int RELIC_CURSED_COOLDOWNS = 29;
const int RELIC_ZENOPHOBIA = 30;

const int NORMAL_RELICS = 30;
/* key relics */

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
				msg = "Spell Power, Duration, Range +0.15";
			}
			case RELIC_ULTIMATE_COST:
			{
				msg = "Ultimate Cost Reduction x0.1";
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
				msg = "When you attack, gain 1 favor. Ultimate Cost Increased x0.1";
			}
			case RELIC_POISON_FASTER:
			{
				msg = "Poison damage doubled but duration halved";
			}
			case RELIC_YEEBAAGOOON:
			{
				msg = "Regenerate 0.3 favor per second";
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
				msg = "Dream Goggles. What do they do?";
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
	float m = 1.0;
	if (equip == false) {
		m = -1.0;
	}
	int class = trQuestVarGet("p"+p+"class");
	string proto = kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto"));
	switch(relic)
	{
		case RELIC_HEALTH:
		{
			trQuestVarSet("p"+p+"health", trQuestVarGet("p"+p+"health") + 300.0 * m);
			zSetProtoUnitStat(proto, p, 0, trQuestVarGet("p"+p+"health"));
		}
		case RELIC_SPEED:
		{
			trQuestVarSet("p"+p+"speed", trQuestVarGet("p"+p+"speed") + 1.0 * m);
			zSetProtoUnitStat(proto, p, 1, trQuestVarGet("p"+p+"speed"));
		}
		case RELIC_ATTACK_RANGE:
		{
			trQuestVarSet("p"+p+"range", trQuestVarGet("p"+p+"range") + 5.0 * m);
			zSetProtoUnitStat(proto, p, 11, trQuestVarGet("p"+p+"range"));
			trQuestVarSet("p"+p+"los", trQuestVarGet("p"+p+"los") + 5.0 * m);
			zSetProtoUnitStat(proto, p, 2, trQuestVarGet("p"+p+"los"));
			zSetProtoUnitStat("Revealer to Player", p, 2, trQuestVarGet("p"+p+"los"));
		}
		case RELIC_SPELL_RANGE:
		{
			trQuestVarSet("p"+p+"spellRangeTrue", trQuestVarGet("p"+p+"spellRangeTrue") + 0.3 * m);
		}
		case RELIC_SPELL_DURATION:
		{
			trQuestVarSet("p"+p+"spellDurationTrue", trQuestVarGet("p"+p+"spellDurationTrue") + 0.3 * m);
		}
		case RELIC_LIFESTEAL:
		{
			trQuestVarSet("p"+p+"Lifesteal", trQuestVarGet("p"+p+"Lifesteal") + 0.2 * m);
		}
		case RELIC_ARMOR:
		{
			if (m == 1) {
				trQuestVarSet("p"+p+"magicResist", calculateArmor(trQuestVarGet("p"+p+"magicResist"), 0.2));
				trQuestVarSet("p"+p+"physicalResist", calculateArmor(trQuestVarGet("p"+p+"physicalResist"), 0.2));
			} else {
				trQuestVarSet("p"+p+"magicResist", calculateArmor(trQuestVarGet("p"+p+"magicResist"), -0.25));
				trQuestVarSet("p"+p+"physicalResist", calculateArmor(trQuestVarGet("p"+p+"physicalResist"), -0.25));
			}
			trModifyProtounit(proto, p, 24, -1);
			trModifyProtounit(proto, p, 25, -1);
			trModifyProtounit(proto, p, 26, -1);
			trModifyProtounit(proto, p, 24, trQuestVarGet("p"+p+"physicalResist"));
			trModifyProtounit(proto, p, 25, trQuestVarGet("p"+p+"physicalResist"));
			trModifyProtounit(proto, p, 26, trQuestVarGet("p"+p+"physicalResist"));
			for(x=yGetDatabaseCount("playerUnits"); >0) {
				yDatabaseNext("playerUnits");
				if ((yGetVar("playerUnits", "player") == p) && (yGetVar("playerUnits", "hero") == 1)) {
					ySetVar("playerUnits", "physicalResist", trQuestVarGet("p"+p+"physicalResist"));
					ySetVar("playerUnits", "magicResist", trQuestVarGet("p"+p+"magicResist"));
				}
			}
		}
		case RELIC_ATTACK_DAMAGE:
		{
			trQuestVarSet("p"+p+"baseAttackTrue", trQuestVarGet("p"+p+"baseAttackTrue") + 15.0 * m);
			trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("p"+p+"baseAttackTrue"));
			trQuestVarSet("p"+p+"Attack", trQuestVarGet("p"+p+"baseAttack"));
			zSetProtoUnitStat(proto, p, 27, trQuestVarGet("p"+p+"baseAttack"));
			zSetProtoUnitStat(proto, p, 31, trQuestVarGet("p"+p+"baseAttack"));
		}
		case RELIC_SPELL_POWER:
		{
			trQuestVarSet("p"+p+"spellDamageTrue", trQuestVarGet("p"+p+"spellDamageTrue") + 0.3 * m);
		}
		case RELIC_HEAL_BOOST:
		{
			trQuestVarSet("p"+p+"healBoost", trQuestVarGet("p"+p+"healBoost") + 0.5 * m);
		}
		case RELIC_POISON_RESISTANCE:
		{
			trQuestVarSet("p"+p+"poisonResistanceCount", trQuestVarGet("p"+p+"poisonResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"poisonResistance", xsPow(0.5, 1*trQuestVarGet("p"+p+"poisonResistanceCount")));
		}
		case RELIC_STUN_RESISTANCE:
		{
			trQuestVarSet("p"+p+"stunResistanceCount", trQuestVarGet("p"+p+"stunResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"stunResistance", xsPow(0.5, 1*trQuestVarGet("p"+p+"stunResistanceCount")));
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			trQuestVarSet("p"+p+"silenceResistanceCount", trQuestVarGet("p"+p+"silenceResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"silenceResistance", xsPow(0.5, 1*trQuestVarGet("p"+p+"silenceResistanceCount")));
		}
		case RELIC_POISON_KILLER:
		{
			trQuestVarSet("p"+p+"poisonKiller", trQuestVarGet("p"+p+"poisonKiller") + 0.5 * m);
		}
		case RELIC_WEIGHTED_BLOWS:
		{
			trQuestVarSet("p"+p+"stunDamage", trQuestVarGet("p"+p+"stunDamage") + 0.1 * m);
		}
		case RELIC_ALL:
		{
			trQuestVarSet("p"+p+"spellDamage", trQuestVarGet("p"+p+"spellDamage") + 0.15 * m);
			trQuestVarSet("p"+p+"spellDuration", trQuestVarGet("p"+p+"spellDuration") + 0.15 * m);
			trQuestVarSet("p"+p+"spellRange", trQuestVarGet("p"+p+"spellRange") + 0.15 * m);
		}
		case RELIC_ULTIMATE_COST:
		{
			trQuestVarSet("p"+p+"ultimateCostCount", trQuestVarGet("p"+p+"ultimateCostCount") + 1.0 * m);
			trQuestVarSet("p"+p+"ultimateCost", xsPow(0.9, trQuestVarGet("p"+p+"ultimateCostCount")));
		}
		case RELIC_PROJECTILES:
		{
			trQuestVarSet("p"+p+"projectiles", trQuestVarGet("p"+p+"projectiles") + 1.0 * m);
			zSetProtoUnitStat(proto, p, 13, trQuestVarGet("p"+p+"projectiles"));
		}
		case RELIC_SPECIAL_ATTACK:
		{
			trQuestVarSet("p"+p+"specialAttackCooldown", trQuestVarGet("p"+p+"specialAttackCooldown") - 1.0 * m);
		}
		case RELIC_COOLDOWN_REDUCTION:
		{
			trQuestVarSet("p"+p+"cooldownReductionCount", trQuestVarGet("p"+p+"cooldownReductionCount") + 1.0 * m);
			trQuestVarSet("p"+p+"cooldownReduction", xsPow(0.85, 1*trQuestVarGet("p"+p+"cooldownReductionCount")));
		}
		case RELIC_CURSED_RANGE:
		{
			trQuestVarSet("p"+p+"spellRangeTrue", trQuestVarGet("p"+p+"spellRangeTrue") + 0.5 * m);
			trQuestVarSet("p"+p+"spellDamageTrue", trQuestVarGet("p"+p+"spellDamageTrue") - 0.3 * m);
		}
		case RELIC_CURSED_DURATION:
		{
			trQuestVarSet("p"+p+"spellDurationTrue", trQuestVarGet("p"+p+"spellDurationTrue") + 0.5 * m);
			trQuestVarSet("p"+p+"spellRangeTrue", trQuestVarGet("p"+p+"spellRangeTrue") - 0.3 * m);
		}
		case RELIC_CURSED_POWER:
		{
			trQuestVarSet("p"+p+"spellDamageTrue", trQuestVarGet("p"+p+"spellDamageTrue") + 0.5 * m);
			trQuestVarSet("p"+p+"spellDurationTrue", trQuestVarGet("p"+p+"spellDurationTrue") - 0.3 * m);
		}
		case RELIC_FAVOR_FROM_ATTACKS:
		{
			trQuestVarSet("p"+p+"favorFromAttacks", trQuestVarGet("p"+p+"favorFromAttacks") + m);
			trQuestVarSet("p"+p+"ultimateCostCount", trQuestVarGet("p"+p+"ultimateCostCount") - 1.0 * m);
			trQuestVarSet("p"+p+"ultimateCost", xsPow(0.9, trQuestVarGet("p"+p+"ultimateCostCount")));
		}
		case RELIC_POISON_FASTER:
		{
			trQuestVarSet("p"+p+"poisonSpeed", trQuestVarGet("p"+p+"poisonSpeed") + 1.0 * m);
		}
		case RELIC_YEEBAAGOOON:
		{
			trQuestVarSet("p"+p+"regenerateFavorLast", trTimeMS());
			trQuestVarSet("p"+p+"favorRegen", trQuestVarGet("p"+p+"favorRegen") + 0.3 * m);
		}
		case RELIC_MAGIC_DETECTOR:
		{
			trQuestVarSet("p"+p+"favorRegen", trQuestVarGet("p"+p+"favorRegen") - m);
			trQuestVarSet("p"+p+"regenerateFavorLast", trTimeMS());
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
				ySetPointer("p"+p+"relics", yGetNewestPointer("p"+p+"relics"));
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"relics", true);
				trUnitChangeProtoUnit("Relic");
				yAddToDatabase("freeRelics", "p"+p+"relics");
				yAddUpdateVar("freeRelics", "type", RELIC_NICKONHAWK);
				yRemoveFromDatabase("p"+p+"relics");
			} else if (trQuestVarGet("p"+p+"nickQuestProgress") == 0) {
				trQuestVarSet("p"+p+"nickQuestProgress", 1);
			}
		}
		case RELIC_NICKONHAWK_TICKET:
		{
			trQuestVarSet("p"+p+"equippedGoggles", trQuestVarGet("p"+p+"equippedGoggles") + m);
			if (trQuestVarGet("p"+p+"equippedGoggles") > 0) {
				trSetLighting("eclipse", 0.1);
			} else {
				trSetLighting("default", 0.1);
			}
		}
	}
	if ((relic >= RELIC_KEY_GREEK) && (relic <= RELIC_KEY_EGYPT) && (trCurrentPlayer() == p) && equip) {
		trChatSend(0, "You have picked up a key. <icon=(20)("+relicIcon(relic)+")>");
	} else {
		/* certain classes have special interactions with stats */
		trEventFire(5000 + 12 * trQuestVarGet("p"+p+"class") + p);
	}
	zSetProtoUnitStat("Cinematic Block", p, 0, trQuestVarGet("p"+p+"health"));
	trQuestVarSet("p"+p+"spellDamage", xsMax(0.1, trQuestVarGet("p"+p+"spellDamageTrue")));
	trQuestVarSet("p"+p+"spellDuration", xsMax(0.1, trQuestVarGet("p"+p+"spellDurationTrue")));
	trQuestVarSet("p"+p+"spellRange", xsMax(0.1, trQuestVarGet("p"+p+"spellRangeTrue")));
	if (trQuestVarGet("p"+p+"godBoon") == BOON_SPELL_ATTACK) {
		trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("p"+p+"spellDamage") * trQuestVarGet("p"+p+"baseAttackTrue"));
		trQuestVarSet("p"+p+"Attack", trQuestVarGet("p"+p+"baseAttack"));
		zSetProtoUnitStat(proto, p, 27, trQuestVarGet("p"+p+"baseAttack"));
		zSetProtoUnitStat(proto, p, 31, trQuestVarGet("p"+p+"baseAttack"));
	}
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
				proto = kbGetProtoUnitID("Walking Berry Bush");
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

void spawnRelicSpecific(float x = 0, float z = 0, int val = 1) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,x,0,z,0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	if (trUnitVisToPlayer()) {
		trSoundPlayFN("relicselect.wav","1",-1,"","");
	}
	trUnitChangeProtoUnit("Relic");
	yAddToDatabase("freeRelics", "next");
	yAddUpdateVar("freeRelics", "type", val);
}

void spawnRelic(float x = 0, float z = 0, int maxval = 10) {
	spawnRelicSpecific(x, z, randomStageClosest(maxval));
}

void spawnRelicClosest(float x = 0, float z = 0, int target = 0) {
	trQuestVarSetFromRand("rand", 0, 10, true);
	trQuestVarSetFromRand("rand",
		xsMax(1, target - trQuestVarGet("rand")), xsMin(10, target + trQuestVarGet("rand")), true);
	spawnRelicSpecific(x, z, 1*trQuestVarGet("rand"));
}
