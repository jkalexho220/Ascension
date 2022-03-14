void removeFireKnight(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "phoenix");
	yRemoveUpdateVar("p"+p+"characters", "charging");
}

void fireknightAlways(int eventID = -1) {
	int p = eventID - 12 * FIREKNIGHT;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"unit");
	if (trUnitAlive()) {
		amt = 0.01 * trUnitPercentDamaged();
	}
	
	trQuestVarSet("p"+p+"Lifesteal",
		trQuestVarGet("p"+p+"Lifesteal") + amt - trQuestVarGet("p"+p+"fireknightBonus"));
	trQuestVarSet("p"+p+"fireknightBonus", amt);
	
	if (yGetDatabaseCount("p"+p+"fireharpies") > 0) {
		for (x=yGetDatabaseCount("p"+p+"fireharpies"); >0) {
			yDatabaseNext("p"+p+"fireharpies", true);
			trMutateSelected(kbGetProtoUnitID("Harpy"));
			trUnitOverrideAnimation(1,0,false,false,-1);
		}
		yClearDatabase("p"+p+"fireharpies");
	}
	
	if (yGetDatabaseCount("p"+p+"fireCharges") > 0) {
		yDatabaseNext("p"+p+"fireCharges");
		trVectorSetUnitPos("pos", "p"+p+"fireCharges");
		yVarToVector("p"+p+"fireCharges", "dest");
		if (zDistanceBetweenVectorsSquared("pos", "dest") < 4 || trTimeMS() > yGetVar("p"+p+"fireCharges", "timeout")) {
			if (ySetPointer("p"+p+"characters", 1*yGetVar("p"+p+"fireCharges", "index"))) {
				ySetVar("p"+p+"characters", "charging", 0);
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters");
				trUnitChangeProtoUnit("Lancer Hero");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters");
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				
				if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
					equipRelicsAgain(p);
				}
				
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"fireCharges", "sfx"), true);
				trUnitChangeProtoUnit("Kronny Flying");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"fireCharges", "sfx"), true);
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trSetSelectedScale(0,-5.0,0);
				trDamageUnitPercent(100);
				trQuestVarSet("next", yGetVar("p"+p+"fireCharges", "sfx"));
				yAddToDatabase("p"+p+"fireHarpies", "next");
				
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"fireCharges");
				trDamageUnitPercent(100);
				trUnitChangeProtoUnit("Meteorite");
				yRemoveFromDatabase("p"+p+"fireCharges");
				amt = trQuestVarGet("flamingImpactDamage") * trQuestVarGet("p"+p+"spellDamage");
				dist = xsPow(trQuestVarGet("flamingImpactRange") * trQuestVarGet("p"+p+"spellRange") * 2, 2);
				for(x=yGetDatabaseCount("enemies"); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
						damageEnemy(p, amt, true);
						gainFavor(p, 2);
					}
				}
				trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
			}
		} else {
			dist = xsPow(trQuestVarGet("flamingImpactRange") * trQuestVarGet("p"+p+"spellRange"), 2);
			for(x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist &&
					yGetVar("enemies", "launched") == 0) {
					trVectorSetUnitPos("end", "enemies");
					trQuestVarSet("endX", trQuestVarGet("endX") - trQuestVarGet("posX") + trQuestVarGet("destX"));
					trQuestVarSet("endZ", trQuestVarGet("endZ") - trQuestVarGet("posZ") + trQuestVarGet("destZ"));
					launchUnit("enemies", "end");
				}
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("nidhoggflame1.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeFireKnight();
			} else if (yGetVar("p"+p+"characters", "charging") == 0) {
				ySetVar("p"+p+"characters", "charging", 1);
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("pos", "p"+p+"wellPos"));
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"characters"));
				trUnitChangeProtoUnit("Dwarf");
				
				trModifyProtounit("Hero Greek Achilles",p,5,2);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
				
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters", true);
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				trUnitOverrideAnimation(15,0,true,false,-1);
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				
				trQuestVarSet("sfx", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("sfx", true);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				trUnitOverrideAnimation(6,0,true,false,-1);
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				
				ySetVarAtIndex("playerUnits", "launched", 1, 1*yGetVar("p"+p+"characters", "index"));
				
				dist = zDistanceBetweenVectors("pos", "p"+p+"wellpos");
				for(y=0; < dist / 2) {
					trQuestVarSet("nextx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
					trQuestVarSet("nextz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
					vectorToGrid("next", "loc");
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					} else {
						trQuestVarSet("posx", trQuestVarGet("nextx"));
						trQuestVarSet("posz", trQuestVarGet("nextz"));
					}
				}
				
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
				trUnitMoveToPoint(trQuestVarGet("posx"),0,trQuestVarGet("posz"),-1,false);
				trModifyProtounit("Hero Greek Achilles",p,5,-2);
				
				yAddToDatabase("p"+p+"fireCharges", "next");
				yAddUpdateVar("p"+p+"fireCharges", "index", yGetPointer("p"+p+"characters"));
				yAddUpdateVar("p"+p+"fireCharges", "timeout", trTimeMS() + 2000 * trQuestVarGet("p"+p+"spellDuration"));
				yAddUpdateVar("p"+p+"fireCharges", "sfx", trQuestVarGet("sfx"));
				yAddUpdateVar("p"+p+"fireCharges", "destX", trQuestVarGet("posx"));
				yAddUpdateVar("p"+p+"fireCharges", "destZ", trQuestVarGet("posz"));
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trSoundPlayFN("firegiantdie.wav","1",-1,"","");
		trQuestVarSet("p"+p+"overheat", 1);
		trQuestVarSet("p"+p+"overheatTimeout",
			trTimeMS() + 1000 * trQuestVarGet("overheatDuration") * trQuestVarGet("p"+p+"spellDuration"));
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeFireKnight();
			} else {
				if (yGetVar("p"+p+"characters", "phoenix") == 0) {
					spyEffect(1*trQuestVarGet("p"+p+"characters"),
						kbGetProtoUnitID("Phoenix"), yGetVarName("p"+p+"characters", "phoenix"));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "phoenix"));
					trMutateSelected(kbGetProtoUnitID("Phoenix"));
				}
				ySetVar("p"+p+"characters", "overheatNext", trTimeMS());
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		gainFavor(p, 0.0 - trQuestVarGet("infernoCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trSoundPlayFN("forestfirebirth.wav","1",-1,"","");
		trSoundPlayFN("flamingweapons.wav","1",-1,"","");
		zSetProtoUnitStat("Ball of Fire Impact", p, 8, trQuestVarGet("p"+p+"spellDuration") * trQuestVarGet("infernoDuration"));
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeFireKnight();
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				yAddToDatabase("p"+p+"inferno", "p"+p+"characters");
				yAddUpdateVar("p"+p+"inferno", "posx", trQuestVarGet("posX"));
				yAddUpdateVar("p"+p+"inferno", "posz", trQuestVarGet("posZ"));
				yAddUpdateVar("p"+p+"inferno", "radius", 0);
				yAddUpdateVar("p"+p+"inferno", "damage", trQuestVarGet("infernoDamage") * trQuestVarGet("p"+p+"spellDamage"));
				yAddUpdateVar("p"+p+"inferno", "maxradius", trQuestVarGet("infernoRange") * trQuestVarGet("p"+p+"spellRange"));
				yAddUpdateVar("p"+p+"inferno", "next", trTimeMS());
				yAddUpdateVar("p"+p+"inferno", "timeout",
					trTimeMS() + 1000 * trQuestVarGet("infernoDuration") * trQuestVarGet("p"+p+"spellDuration"));
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"overheat") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"overheatTimeout")) {
			trQuestVarSet("p"+p+"overheat", 0);
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				id = yDatabaseNext("p"+p+"characters", true);
				if (id == -1 || trUnitAlive() == false) {
					removeFireKnight();
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "phoenix"));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeFireKnight(p);
		} else {
			amt = 0;
			trVectorSetUnitPos("pos", "p"+p+"characters");
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_SPECIAL) {
				damagePlayerUnit(50.0 * trQuestVarGet("p"+p+"spellDamage"), 1*yGetVar("p"+p+"characters", "index"));
				
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trDamageUnitPercent(100);
				trUnitChangeProtoUnit("Meteorite");
				amt = 50.0 * trQuestVarGet("p"+p+"spellDamage");
				trSoundPlayFN("fireball fall 2.wav","1",-1,"","");
			}
			current = kbUnitGetCurrentHitpoints(id);
			if (trQuestVarGet("p"+p+"overheat") == 1) {
				if (trTimeMS() > yGetVar("p"+p+"characters", "overheatNext")) {
					ySetVar("p"+p+"characters", "overheatNext", yGetVar("p"+p+"characters", "overheatNext") + 500);
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					damagePlayerUnit(trQuestVarGet("overheatDamage") * trQuestVarGet("p"+p+"spellDamage") * 0.5,
						1*yGetVar("p"+p+"characters","index"));
					current = kbUnitGetCurrentHitpoints(id);
				}
				amt = amt + xsMax(0, yGetVar("p"+p+"characters", "health") - current);
			}
			ySetVar("p"+p+"characters", "health", current);
			dist = xsPow(trQuestVarGet("overheatRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
			for(x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					damageEnemy(p, amt * trQuestVarGet("p"+p+"spellDamage"), true);
				}
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"inferno") >0) {
		yDatabaseNext("p"+p+"inferno");
		if (trTimeMS() > yGetVar("p"+p+"inferno", "next")) {
			ySetVar("p"+p+"inferno", "next", yGetVar("p"+p+"inferno", "next") + 300);
			yVarToVector("p"+p+"inferno", "pos");
			if (yGetVar("p"+p+"inferno", "radius") < yGetVar("p"+p+"inferno", "maxradius")) {
				ySetVar("p"+p+"inferno", "radius", 2 + yGetVar("p"+p+"inferno", "radius"));
				trQuestVarSetFromRand("angle", 0, 3.141592, false);
				hit = 1.5 * yGetVar("p"+p+"inferno", "radius");
				amt = 6.283185 / hit;
				for(x=hit; >0) {
					trQuestVarSet("angle", fModulo(6.283185, trQuestVarGet("angle") + amt));
					trVectorSetFromAngle("dir", trQuestVarGet("angle"));
					trQuestVarSet("dirX", yGetVar("p"+p+"inferno","radius") * trQuestVarGet("dirX") + trQuestVarGet("posX"));
					trQuestVarSet("dirZ", yGetVar("p"+p+"inferno","radius") * trQuestVarGet("dirZ") + trQuestVarGet("posZ"));
					trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("dirX"),0,trQuestVarGet("dirZ"),0,true);
					trArmySelect(""+p+",0");
					trUnitChangeProtoUnit("Ball of Fire Impact");
				}
			}
			amt = yGetVar("p"+p+"inferno", "damage") * 0.3;
			dist = xsPow(yGetVar("p"+p+"inferno", "radius"), 2);
			for(x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					damageEnemy(p, amt);
				}
			}
			for(x=yGetDatabaseCount("playerUnits"); >0) {
				id = yDatabaseNext("playerUnits", true);
				if (id == -1 || trUnitAlive() == false) {
					removePlayerUnit();
				} else if (zDistanceToVectorSquared("playerUnits", "pos") < dist) {
					healUnit(p, amt * 0.5);
				}
			}
		}
		if (trTimeMS() > yGetVar("p"+p+"inferno", "timeout")) {
			yRemoveFromDatabase("p"+p+"inferno");
		}
	}
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseFireKnight(int eventID = -1) {
	int p = eventID - 1000 - 12 * FIREKNIGHT;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Flaming Impact";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Overheat";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Inferno";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("flamingImpactCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("infernoCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("overheatCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

rule fireknight_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * FIREKNIGHT + p, "fireknightAlways");
		trEventSetHandler(1000 + 12 * FIREKNIGHT + p, "chooseFireKnight");
	}
	
	trQuestVarSet("flamingImpactCooldown", 12);
	trQuestVarSet("flamingImpactDamage", 90);
	trQuestVarSet("flamingImpactRange", 4);
	
	trQuestVarSet("overheatCooldown", 18);
	trQuestVarSet("overheatDuration", 6);
	trQuestVarSet("overheatDamage", 40);
	trQuestVarSet("overheatRadius", 4);
	
	trQuestVarSet("infernoDuration", 8);
	trQuestVarSet("infernoRange", 12);
	trQuestVarSet("infernoDamage", 120);
	trQuestVarSet("infernoHeal", 60);
	trQuestVarSet("infernoCost", 70);
}
