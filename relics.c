/* regular drop pool */
const int RELIC_HEALTH = 1;
const int RELIC_SPEED = 2;
const int RELIC_ATTACK_RANGE = 3;
const int RELIC_SPELL_RANGE = 4;
const int RELIC_SPELL_DURATION = 5;
const int RELIC_ATTACK_LIFESTEAL = 6;
const int RELIC_SPELL_LIFESTEAL = 7;
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
const int RELIC_STUN_KILLER = 19;
const int RELIC_ALL = 20;

/* nickonhawk's shop */

/* key relics */
const int RELIC_KEY_GREEK = 33;
const int RELIC_KEY_NORSE = 34;
const int RELIC_KEY_ATLANTEAN = 35;
const int RELIC_KEY_CHINA = 36;
const int RELIC_KEY_EGYPT = 37;

const int RELIC_COUNT = 20;

string relicName(int relic = 0) {
	string msg = "WTF That's not a relic!";
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
			msg = "Spell Range +0.2x";
		}
		case RELIC_SPELL_DURATION:
		{
			msg = "Spell Duration +0.2x";
		}
		case RELIC_ATTACK_LIFESTEAL:
		{
			msg = "Attack Lifesteal +0.2";
		}
		case RELIC_SPELL_LIFESTEAL:
		{
			msg = "Spell Lifesteal +0.1";
		}
		case RELIC_ATTACK_DAMAGE:
		{
			msg = "Attack +12";
		}
		case RELIC_SPELL_POWER:
		{
			msg = "Spell Power +0.3x";
		}
		case RELIC_HEAL_BOOST:
		{
			msg = "Healing Effects +0.3x";
		}
		case RELIC_POISON_RESISTANCE:
		{
			msg = "Poison resistance x0.3";
		}
		case RELIC_STUN_RESISTANCE:
		{
			msg = "Stun resistance x0.3";
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			msg = "Silence resistance x0.3";
		}
		case RELIC_POISON_KILLER:
		{
			msg = "+0.3x damage to poisoned enemies.";
		}
		case RELIC_STUN_KILLER:
		{
			msg = "+0.3x damage to stunned enemies.";
		}
		case RELIC_ALL:
		{
			msg = "Spell Power, Duration, Range +0.1";
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
			msg = "Cooldown Reduction x0.1";
		}
	}
	if (relic >= RELIC_KEY_GREEK) {
		msg = "A key that opens something";
	}
	return(msg);
}

string relicIcon(int relic = 0) {
	string icon = "icons\infantry g hoplite icon 64";
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
		case RELIC_ATTACK_LIFESTEAL:
		{
			icon = "icons\special n troll icon 64";
		}
		case RELIC_SPELL_LIFESTEAL:
		{
			icon = "icons\special e wadjet icon 64";
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
			icon = "icons\special e priest icon 64";
		}
		case RELIC_POISON_RESISTANCE:
		{
			icon = "icons\god power dryad icons 64";
		}
		case RELIC_STUN_RESISTANCE:
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
		case RELIC_STUN_KILLER:
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
	}
	return(icon);
}

void relicDescription(int relic = 0) {
	string icon = relicIcon(relic);
	string msg = relicName(relic);
	trShowImageDialog(icon, msg);
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
		}
		case RELIC_SPELL_RANGE:
		{
			trQuestVarSet("p"+p+"spellRange", trQuestVarGet("p"+p+"spellRange") + 0.2 * m);
		}
		case RELIC_SPELL_DURATION:
		{
			trQuestVarSet("p"+p+"spellDuration", trQuestVarGet("p"+p+"spellDuration") + 0.2 * m);
		}
		case RELIC_ATTACK_LIFESTEAL:
		{
			trQuestVarSet("p"+p+"attackLifesteal", trQuestVarGet("p"+p+"attackLifesteal") + 0.2 * m);
		}
		case RELIC_SPELL_LIFESTEAL:
		{
			trQuestVarSet("p"+p+"spellLifesteal", trQuestVarGet("p"+p+"spellLifesteal") + 0.1 * m);
		}
		case RELIC_ATTACK_DAMAGE:
		{
			trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("p"+p+"baseAttack") + 12.0 * m);
			zSetProtoUnitStat(proto, p, 27, trQuestVarGet("p"+p+"baseAttack"));
			zSetProtoUnitStat(proto, p, 31, trQuestVarGet("p"+p+"baseAttack"));
		}
		case RELIC_SPELL_POWER:
		{
			trQuestVarSet("p"+p+"spellDamage", trQuestVarGet("p"+p+"spellDamage") + 0.3 * m);
		}
		case RELIC_HEAL_BOOST:
		{
			trQuestVarSet("p"+p+"healBoost", trQuestVarGet("p"+p+"healBoost") + 0.3 * m);
		}
		case RELIC_POISON_RESISTANCE:
		{
			trQuestVarSet("p"+p+"poisonResistanceCount", trQuestVarGet("p"+p+"poisonResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"poisonResistance", xsPow(0.7, 1*trQuestVarGet("p"+p+"poisonResistanceCount")));
		}
		case RELIC_STUN_RESISTANCE:
		{
			trQuestVarSet("p"+p+"stunResistanceCount", trQuestVarGet("p"+p+"stunResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"stunResistance", xsPow(0.7, 1*trQuestVarGet("p"+p+"stunResistanceCount")));
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			trQuestVarSet("p"+p+"silenceResistanceCount", trQuestVarGet("p"+p+"silenceResistanceCount") + 1.0 * m);
			trQuestVarSet("p"+p+"silenceResistance", xsPow(0.7, 1*trQuestVarGet("p"+p+"silenceResistanceCount")));
		}
		case RELIC_POISON_KILLER:
		{
			trQuestVarSet("p"+p+"poisonKiller", trQuestVarGet("p"+p+"poisonKiller") + 0.3 * m);
		}
		case RELIC_STUN_KILLER:
		{
			trQuestVarSet("p"+p+"stunKiller", trQuestVarGet("p"+p+"stunKiller") + 0.3 * m);
		}
		case RELIC_ALL:
		{
			trQuestVarSet("p"+p+"spellDamage", trQuestVarGet("p"+p+"spellDamage") + 0.1 * m);
			trQuestVarSet("p"+p+"spellDuration", trQuestVarGet("p"+p+"spellDuration") + 0.1 * m);
			trQuestVarSet("p"+p+"spellRange", trQuestVarGet("p"+p+"spellRange") + 0.1 * m);
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
			trQuestVarSet("p"+p+"cooldownReduction", xsPow(0.9, 1*trQuestVarGet("p"+p+"cooldownReductionCount")));
		}
	}
	if ((relic >= RELIC_KEY_GREEK) && (trCurrentPlayer() == p) && equip) {
		trChatSend(0, "You have picked up a key. <icon=(20)("+relicIcon(relic)+")>");
	} else {
		/* certain classes have special interactions with stats */
		trEventFire(5000 + 12 * trQuestVarGet("p"+p+"class") + p);
	}
}

int relicProto(int relic = 0) {
	int proto = 0;
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
		case RELIC_ATTACK_LIFESTEAL:
		{
			proto = kbGetProtoUnitID("Troll");
		}
		case RELIC_SPELL_LIFESTEAL:
		{
			proto = kbGetProtoUnitID("Wadjet");
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
			proto = kbGetProtoUnitID("Priest");
		}
		case RELIC_POISON_RESISTANCE:
		{
			proto = kbGetProtoUnitID("Dryad");
		}
		case RELIC_STUN_RESISTANCE:
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
		case RELIC_STUN_KILLER:
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