void removeStormcutter(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "rainSFX");
}

void stormcutterAlways(int eventID = -1) {
	int p = eventID - 12 * STORMCUTTER;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
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
					gainFavor(p, 1);
					trChatSetStatus(false);
					trDelayedRuleActivation("enable_chat");
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
							damageEnemy(p, amt, false, 1.0);
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
				gainFavor(p, 1.0);
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
	
	if (yGetDatabaseCount("p"+p+"spearedUnits") > 0) {
		yDatabaseNext("p"+p+"spearedUnits");
		if (yGetVarAtIndex("enemies", "launched", 1*yGetVar("p"+p+"spearedUnits", "index")) == 0) {
			hit = 1*yGetVarAtIndex("enemies", "doppelganger", 1*yGetVar("p"+p+"spearedUnits", "index"));
			if ((PvP == false) || (yGetVarAtIndex("playerUnits", "launched", hit) == 0)) {
				if (yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("p"+p+"spearedUnits", "index")) > 0) {
					if (ySetPointer("enemies", 1*yGetVar("p"+p+"spearedUnits", "index"))) {
						trUnitSelectClear();
						trUnitSelectByQV("enemies");
						damageEnemy(p, 100.0 * trQuestVarGet("p"+p+"spellDamage") * (1.0 + 0.02 * yGetVar("p"+p+"spearedUnits", "dist")));
					}
				}
				yRemoveFromDatabase("p"+p+"spearedUnits");
			}
		} else {
			yVarToVector("p"+p+"spearedUnits", "prev");
			trVectorSetUnitPos("pos", "p"+p+"spearedUnits");
			dist = zDistanceBetweenVectors("pos", "prev");
			if (dist > 2.0) {
				ySetVar("p"+p+"spearedUnits", "dist", yGetVar("p"+p+"spearedUnits", "dist") + dist);
				yVarToVector("p"+p+"spearedUnits", "dir");
				for(x=dist / 2; >0) {
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("prevx"),0,trQuestVarGet("prevz"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning Sparks");
					trQuestVarSet("prevx", trQuestVarGet("prevx") + 2.0 * trQuestVarGet("dirx"));
					trQuestVarSet("prevz", trQuestVarGet("prevz") + 2.0 * trQuestVarGet("dirz"));
				}
				ySetVarFromVector("p"+p+"spearedUnits", "prev", "pos");
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"shockArrows") > 0) {
		yDatabaseNext("p"+p+"shockArrows");
		trVectorSetUnitPos("pos", "p"+p+"shockArrows");
		yVarToVector("p"+p+"shockArrows", "prev");
		yVarToVector("p"+p+"shockArrows", "dir");
		yVarToVector("p"+p+"shockArrows", "dest");
		dist = zDistanceBetweenVectors("pos", "prev");
		hit = 0;
		if (dist > 2.0) {
			ySetVar("p"+p+"shockArrows", "dist", yGetVar("p"+p+"shockArrows", "dist") + dist);
			if (yGetVar("p"+p+"shockArrows", "timeout") == -1) {
				ySetVar("p"+p+"shockArrows", "timeout", trTimeMS() + 1000 * trQuestVarGet("p"+p+"spellRange"));
			}
			for(x=yGetDatabaseCount("enemies"); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision("enemies", "prev", "dir", dist + 1.0, 2.0)) {
					hit = 1;
					gainFavor(p, 5.0);
					trQuestVarSetFromRand("sound", 1, 2, true);
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					}
					damageEnemy(p, 100.0 * trQuestVarGet("p"+p+"spellDamage") * (1.0 + 0.02 * yGetVar("p"+p+"shockArrows", "dist")));
					if (trUnitAlive()) {
						yAddToDatabase("p"+p+"spearedUnits", "enemies");
						yAddUpdateVar("p"+p+"spearedUnits", "index", yGetPointer("enemies"));
						yAddUpdateVar("p"+p+"spearedUnits", "prevx", trQuestVarGet("posx"));
						yAddUpdateVar("p"+p+"spearedUnits", "prevz", trQuestVarGet("posz"));
						yAddUpdateVar("p"+p+"spearedUnits", "dirx", trQuestVarGet("dirx"));
						yAddUpdateVar("p"+p+"spearedUnits", "dirz", trQuestVarGet("dirz"));
						yAddUpdateVar("p"+p+"spearedUnits", "dist", yGetVar("p"+p+"shockArrows", "dist"));
						launchUnit("enemies", "dest");
					}
					
					break;
				}
			}
			for (x=dist/2; >0) {
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("prevx"),0,trQuestVarGet("prevz"),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Lightning Sparks");
				trQuestVarSet("prevx", trQuestVarGet("prevx") + 2.0 * trQuestVarGet("dirx"));
				trQuestVarSet("prevz", trQuestVarGet("prevz") + 2.0 * trQuestVarGet("dirz"));
			}
			ySetVarFromVector("p"+p+"shockArrows", "prev", "pos");
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hit = 2;
			}
		} else if ((trTimeMS() > yGetVar("p"+p+"shockArrows", "timeout")) && (yGetVar("p"+p+"shockArrows", "timeout") > 0)) {
			hit = 1;
		} else if (zDistanceToVectorSquared("pos", "dest") < 1.0) {
			hit = 1;
		}
		
		if (hit >= 1) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"shockArrows");
			trUnitChangeProtoUnit("Dust Small");
			yRemoveFromDatabase("p"+p+"shockArrows");
			if (hit == 2) {
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trSoundPlayFN("ui\lightning4.wav","1",-1,"","");
		trVectorSetUnitPos("target", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject");
		trUnitDestroy();
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters") == -1 || trUnitAlive() == false) {
				removeStormcutter(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("pos", "target"));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Ballista Shot"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				vectorSetAsTargetVector("dest","pos","target",30.0 * trQuestVarGet("p"+p+"spellRange"));
				trUnitMoveToPoint(trQuestVarGet("destx"),0,trQuestVarGet("destz"));
				yAddToDatabase("p"+p+"shockArrows", "next");
				yAddUpdateVar("p"+p+"shockArrows", "prevx", trQuestVarGet("posx"));
				yAddUpdateVar("p"+p+"shockArrows", "prevz", trQuestVarGet("posz"));
				yAddUpdateVar("p"+p+"shockArrows", "destx", trQuestVarGet("destx"));
				yAddUpdateVar("p"+p+"shockArrows", "destz", trQuestVarGet("destz"));
				yAddUpdateVar("p"+p+"shockArrows", "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar("p"+p+"shockArrows", "dirz", trQuestVarGet("dirz"));
				yAddUpdateVar("p"+p+"shockArrows", "timeout", -1);
				yAddUpdateVar("p"+p+"shockArrows", "dist", 0);
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"rainOfLightning") == 2) {
		trQuestVarSet("p"+p+"rainOfLightning", 1);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
				removeStormcutter(p);
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "rainSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Arkantos God"));
				trSetSelectedScale(0,0,0);
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		gainFavor(p, 0.0 - trQuestVarGet("rainOfLightningCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trSoundPlayFN("lightningbirth.wav","1",-1,"","");
		trQuestVarSet("p"+p+"rainOfLightning", 2);
		trQuestVarSet("p"+p+"rainOfLightningTimeout",
			trTimeMS() + 1000 * trQuestVarGet("rainOfLightningDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trQuestVarSet("p"+p+"nextDelay",
			trQuestVarGet("class"+STORMCUTTER+"nextDelay") * 2.0 / (1.0 + trQuestVarGet("p"+p+"projectiles")));
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
		trQuestVarSet("p"+p+"arrowNext", trGetNextUnitScenarioNameNumber() - 1);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
				removeStormcutter(p);
			} else if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "rainSFX")) == -1) {
				spyEffect(1*trQuestVarGet("p"+p+"characters"),kbGetProtoUnitID("Cinematic Block"),
					yGetVarName("p"+p+"characters", "rainSFX"));
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"rainOfLightningTimeout")) {
			trQuestVarSet("p"+p+"rainOfLightning", 0);
			trQuestVarSet("p"+p+"nextDelay", trQuestVarGet("class"+STORMCUTTER+"nextDelay"));
			zSetProtoUnitStat("Archer Atlantean Hero", p, 13, trQuestVarGet("p"+p+"projectiles"));
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
					removeStormcutter(p);
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "rainSFX"), true);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}
		id = yFindLatest("p"+p+"arrowNext", "Arrow Flaming", p);
		if (id > 0) {
			trUnitDestroy();
		}
	}
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseStormcutter(int eventID = -1) {
	int p = eventID - 1000 - 12 * STORMCUTTER;
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Disengage";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Rain of Lightning";
		rainIsUltimate = true;
		map("q", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(Q) Shock Arrow";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("disengageCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("shockArrowCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", trQuestVarGet("rainOfLightningCost"));
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
	
	trQuestVarSet("rainOfLightningCost", 50);
	trQuestVarSet("rainOfLightningDuration", 10);
	trQuestVarSet("rainOfLightningRadius", 5);
	
	trQuestVarSet("disengageCooldown", 8);
	trQuestVarSet("disengageRadius", 4);
	trQuestVarSet("disengageRange", 12);
	
	trQuestVarSet("shockArrowCooldown", 10);
	trQuestVarSet("shockArrowRange", 30);
	trQuestVarSet("shockArrowDamage", 50);
}
