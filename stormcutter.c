void removeStormcutter(int p = 0) {
	removePlayerSpecific(p);
}

void stormcutterAlways(int eventID = -1) {
	int p = eventID - 12 * STORMCUTTER;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeStormcutter(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_NORMAL) {
				target = kbUnitGetTargetUnitID(id);
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("target", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
				dist = zDistanceBetweenVectors("pos", "target");
				trQuestVarSet("p"+p+"stormbonus", dist * 0.02);
				trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * (1.0 + trQuestVarGet("p"+p+"stormbonus")));
				zSetProtoUnitStat("Archer Atlantean Hero", p, 31, trQuestVarGet("p"+p+"attack"));
				if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
					trUnitSelectClear();
					trUnitSelectByID(target);
					trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
					dist = xsPow(trQuestVarGet("rainOfLightningRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
					amt = trQuestVarGet("p"+p+"attack") * trQuestVarGet("p"+p+"spellDamage");
					for(x=yGetDatabaseCount("enemies"); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "target") < dist) {
							trUnitHighlight(0.2, false);
							damageEnemy(p, amt, false);
						}
					}
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"rainOfLightning", 1);
		trQuestVarSet("p"+p+"rainOfLightningTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("rainOfLightningDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trQuestVarSet("p"+p+"nextDelay", 
			trQuestVarGet("class"+STORMCUTTER+"nextDelay") * 3.0 / (2.0 + trQuestVarGet("p"+p+"projectiles")));
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
		trQuestVarSet("p"+p+"arrowNext", trGetNextUnitScenarioNameNumber() - 1);
	}

	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"rainOfLightningTimeout")) {
			trQuestVarSet("p"+p+"rainOfLightning", 0);
			trQuestVarSet("p"+p+"nextDelay", trQuestVarGet("class"+STORMCUTTER+"nextDelay"));
			zSetProtoUnitStat("Archer Atlantean Hero", p, 13, trQuestVarGet("p"+p+"projectiles"));
		}
		id = yFindLatest("p"+p+"arrowNext", "Arrow Flaming", p);
		if (id > 0) {
			trUnitDestroy();
		}
	}

	xsSetContextPlayer(old);
}

void chooseStormcutter(int eventID = -1) {
	int p = eventID - 1000 - 12 * STORMCUTTER;
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Disengage";
		wellIsUltimate = false;
		map("q", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(Q) Rain of Lightning";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Whirlwind";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("disengageCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("whirlwindCooldown"));
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("whirlwindCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("rainOfLightningCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void modifyStormcutter(int eventID = -1) {
	int p = eventID - 5000 - 12 * STORMCUTTER;
	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
	}
}

rule stormcutter_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STORMCUTTER + p, "stormcutterAlways");
		trEventSetHandler(1000 + 12 * STORMCUTTER + p, "chooseStormcutter");
		trEventSetHandler(5000 + 12 * STORMCUTTER + p, "modifyStormcutter");
	}

	trQuestVarSet("rainOfLightningCooldown", 12);
	trQuestVarSet("rainOfLightningDuration", 4);
	trQuestVarSet("rainOfLightningRadius", 3);

	trQuestVarSet("disengageCooldown", 8);
	trQuestVarSet("disengageRadius", 4);
	trQuestVarSet("disengageRange", 12);

	trQuestVarSet("whirlwindCost", 70);
	trQuestVarSet("whirlwindRange", 18);
	trQuestVarSet("whirlwindCooldown", 5);
}