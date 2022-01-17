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
				target = yGetVar("p"+p+"characters", "attackTarget");
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("target", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
				dist = zDistanceBetweenVectors("pos", "target");
				trQuestVarSet("p"+p+"stormbonus", dist * 0.02);
				trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * (1.0 + trQuestVarGet("p"+p+"stormbonus")));
				zSetProtoUnitStat("Archer Atlantean Hero", p, 31, trQuestVarGet("p"+p+"attack"));
				if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
					trPlayerGrantResources(p, "favor", 1);
					trUnitSelectClear();
					trUnitSelectByID(target);
					trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
					dist = xsPow(trQuestVarGet("rainOfLightningRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
					amt = 2.0 * trQuestVarGet("p"+p+"attack") * trQuestVarGet("p"+p+"spellDamage");
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
		dist = trQuestVarGet("disengageRadius") * trQuestVarGet("p"+p+"spellRange");
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else if (trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Archer Atlantean Hero", dist) >0) {
				stunUnit("enemies", 2.0, p);
				trPlayerGrantResources(p, "favor", 1);
			}
		}
		trSoundPlayFN("sphinxteleportout.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeStormcutter(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Dust Large");
				target = 1 + xsMin(trQuestVarGet("disengageRange") * trQuestVarGet("p"+p+"spellRange"), 
					zDistanceBetweenVectors("pos", "p"+p+"wellPos")) / 2;
				trVectorQuestVarSet("step", zGetUnitVector("pos", "p"+p+"wellPos"));
				for(i=target; >0) {
					trQuestVarSet("posx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("stepx"));
					trQuestVarSet("posz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("stepz"));
					vectorToGrid("pos", "loc");
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					}
				}
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters");
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trMutateSelected(kbGetProtoUnitID("Siege Tower"));
				trUnitChangeProtoUnit("Archer Atlantean Hero");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters");
				trSetUnitOrientation(trVectorQuestVarGet("step"), vector(0,1,0), true);
				trMutateSelected(kbGetProtoUnitID("Archer Atlantean Hero"));
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Lightning Sparks");
			}
		}
		/* reload relics */
		equipRelicsAgain(p);
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("whirlwindCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trSoundPlayFN("sphinxspecialattack.wav","1",-1,"","");
		trVectorSetUnitPos("center", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitChangeProtoUnit("Implode Sphere Effect");
		yAddToDatabase("p"+p+"whirlwindObjects", "p"+p+"lureObject");
		yAddUpdateVar("p"+p+"whirlwindObjects", "timeout", trTimeMS() + 1000 * trQuestVarGet("p"+p+"spellRange"));
		dist = xsPow(trQuestVarGet("whirlwindRange") * trQuestVarGet("p"+p+"spellRange"), 2);
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else if (zDistanceToVectorSquared("enemies", "center") < dist) {
				launchUnit("enemies", "center");
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"whirlwindObjects") > 0) {
		yDatabaseNext("p"+p+"whirlwindObjects", true);
		amt = yGetVar("p"+p+"whirlwindObjects", "timeout") - trTimeMS();
		if (amt > 0) {
			amt = amt * 0.001;
			trSetSelectedScale(amt, 1, amt);
		} else {
			trUnitDestroy();
			yRemoveFromDatabase("p"+p+"whirlwindObjects");
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"rainOfLightning", 1);
		trQuestVarSet("p"+p+"rainOfLightningTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("rainOfLightningDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trQuestVarSet("p"+p+"nextDelay", 
			trQuestVarGet("class"+STORMCUTTER+"nextDelay") * 2.0 / (1.0 + trQuestVarGet("p"+p+"projectiles")));
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
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("whirlwindCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("rainOfLightningCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void modifyStormcutter(int eventID = -1) {
	int p = eventID - 5000 - 12 * STORMCUTTER;
	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
		trQuestVarSet("p"+p+"nextDelay", 
			trQuestVarGet("class"+STORMCUTTER+"nextDelay") * 2.0 / (1.0 + trQuestVarGet("p"+p+"projectiles")));
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