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
const int RELIC_STUN_RESISTANCE = 12;
const int RELIC_SILENCE_RESISTANCE = 13;
const int RELIC_POISON_KILLER = 14;
const int RELIC_STUN_KILLER = 15;
const int RELIC_HALF_KILLER = 16;
const int RELIC_ULTIMATE_COST = 17;
const int RELIC_PROJECTILES = 18;
const int RELIC_SPECIAL_ATTACK = 19;
const int RELIC_COOLDOWN_REDUCTION = 20;

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
			msg = "Attack Range +6";
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
			msg = "You recover from poison twice as fast.";
		}
		case RELIC_STUN_RESISTANCE:
		{
			msg = "You recover from stuns twice as fast.";
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			msg = "You are immune to silences.";
		}
		case RELIC_POISON_KILLER:
		{
			msg = "Deal 0.3x bonus damage to poisoned enemies.";
		}
		case RELIC_STUN_KILLER:
		{
			msg = "Deal 0.3x bonus damage to stunned enemies.";
		}
		case RELIC_HALF_KILLER:
		{
			msg = "Deal 0.3x bonus damage to enemies below half health.";
		}
		case RELIC_ULTIMATE_COST:
		{
			msg = "Ultimate Cost -5. (No effect on toggled ultimates)";
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
	return(msg);
}

void relicDescription(int relic = 0) {
	string icon = "icons\infantry g hoplite icon 64";
	string msg = "WTF That's not a relic!";
	switch(relic)
	{
		case RELIC_HEALTH:
		{
			icon = "icons\special g sea turtle icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPEED:
		{
			icon = "icons\special g pegasus icon 64";
			msg = relicName(relic);
		}
		case RELIC_ATTACK_RANGE:
		{
			icon = "icons\cavalry e chariot archer icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPELL_RANGE:
		{
			icon = "icons\special e petosuchus icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPELL_DURATION:
		{
			icon = "icons\special n frost giant icon 64";
			msg = relicName(relic);
		}
		case RELIC_ATTACK_LIFESTEAL:
		{
			icon = "icons\special n troll icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPELL_LIFESTEAL:
		{
			icon = "icons\special e wadjet icon 64";
			msg = relicName(relic);
		}
		case RELIC_ATTACK_DAMAGE:
		{
			icon = "icons\siege n ballista icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPELL_POWER:
		{
			icon = "icons\special n fire giant icon 64";
			msg = relicName(relic);
		}
		case RELIC_HEAL_BOOST:
		{
			icon = "icons\special e priest icon 64";
			msg = relicName(relic);
		}
		case RELIC_POISON_RESISTANCE:
		{
			icon = "icons\special x behemoth icons 64";
			msg = relicName(relic);
		}
		case RELIC_STUN_RESISTANCE:
		{
			icon = "icons\hero g ajax icon 64";
			msg = relicName(relic);
		}
		case RELIC_SILENCE_RESISTANCE:
		{
			icon = "icons\special e sphinx icon 64";
			msg = relicName(relic);
		}
		case RELIC_POISON_KILLER:
		{
			icon = "icons\special e mummy icon 64";
			msg = relicName(relic);
		}
		case RELIC_STUN_KILLER:
		{
			icon = "icons\special g medusa icon 64";
			msg = relicName(relic);
		}
		case RELIC_HALF_KILLER:
		{
			icon = "icons\special g cyclops icon 64";
			msg = relicName(relic);
		}
		case RELIC_ULTIMATE_COST:
		{
			icon = "icons\special g arkantos icon 64";
			msg = relicName(relic);
		}
		case RELIC_PROJECTILES:
		{
			icon = "icons\special g manticore icon 64";
			msg = relicName(relic);
		}
		case RELIC_SPECIAL_ATTACK:
		{
			icon = "icons\special e avenger icon 64";
			msg = relicName(relic);
		}
		case RELIC_COOLDOWN_REDUCTION:
		{
			icon = "icons\special x argus icons 64";
			msg = relicName(relic);
		}
	}
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
			trQuestVarSet("p"+p+"range", trQuestVarGet("p"+p+"range") + 6.0 * m);
			zSetProtoUnitStat(proto, p, 11, trQuestVarGet("p"+p+"range"));
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
			trQuestVarSet("p"+p+"poisonKiller", trQuestVarGet("p"+p+"poisonKiller") + 0.3 * m);
		}
		case RELIC_STUN_KILLER:
		{
			trQuestVarSet("p"+p+"stunKiller", trQuestVarGet("p"+p+"stunKiller") + 0.3 * m);
		}
		case RELIC_HALF_KILLER:
		{
			trQuestVarSet("p"+p+"halfKiller", trQuestVarGet("p"+p+"halfKiller") + 0.3 * m);
		}
		case RELIC_ULTIMATE_COST:
		{
			trQuestVarSet("p"+p+"ultimateCost", trQuestVarGet("p"+p+"ultimateCost") + 5.0 * m);
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
		case RELIC_HALF_KILLER:
		{
			proto = kbGetProtoUnitID("Cyclops");
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
	}
	return(proto);
}

int relicRandom(bool basic = true) {
	trQuestVarSetFromRand("relicrand1",1,10, true);
	trQuestVarSetFromRand("relicrand2",1,10, true);
	float firstDiff = xsAbs(trQuestVarGet("relicrand1") - trQuestVarGet("stage"));
	float secondDiff = xsAbs(trQuestVarGet("relicrand2") - trQuestVarGet("stage"));
	if (secondDiff < firstDiff) {
		trQuestVarSet("relicrand1", trQuestVarGet("relicrand2"));
	}
	if (basic == false) {
		trQuestVarSet("relicrand1", 10 + trQuestVarGet("relicrand1"));
	}
	return(1*trQuestVarGet("relicrand1"));
}

void spawnRelic(float x = 0, float z = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,x,0,z,0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	if (trUnitVisToPlayer()) {
		trSoundPlayFN("relicselect.wav","1",-1,"","");
	}
	trUnitChangeProtoUnit("Relic");
	yAddToDatabase("freeRelics", "next");
	yAddUpdateVar("freeRelics", "type", relicRandom());
}