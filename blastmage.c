
void removeBlastmage(int p = 0) {
	removePlayerSpecific(p);
}

void blastmageSpell(int p = 0) {
	for(x=yGetDatabaseCount("P"+p+"characters"); >0) {
		yDatabaseNext("p"+p+"characters");
		ySetVar("p"+p+"characters", "starfire", 1 + yGetVar("p"+p+"characters", "starfire"));
		if (yGetVar("p"+p+"characters", "starfire") > 3) {
			ySetVar("p"+p+"characters", "starfire", 3);
		}
	}
}

void spawnStar(int p = 0, string pos = "") {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch(""+p+",0","Dwarf",2,trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit("Spy Eye");
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	trMutateSelected(kbGetProtoUnitID("Outpost"));
	trSetSelectedScale(0,0,0);
	trUnitSelectClear();
	trUnitSelect(""+(1+trQuestVarGet("next")), true);
	trMutateSelected(kbGetProtoUnitID("Lighthouse"));
	trSetSelectedScale(0,0,0);
	yAddToDatabase("p"+p+"stars", "next");
	yAddUpdateVar("p"+p+"stars", "posx", trQuestVarGet(pos+"x"));
	yAddUpdateVar("p"+p+"stars", "posz", trQuestVarGet(pos+"z"));
	yAddUpdateVar("p"+p+"stars", "timeout",
		trTimeMS() + 1000 * trQuestVarGet("starDuration") * trQuestVarGet("p"+p+"spellDuration"));
	yAddUpdateVar("p"+p+"stars", "last", trTimeMS());
}

void blastmageAlways(int eventID = -1) {
	int p = eventID - 12 * BLASTMAGE;
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
			removeBlastmage(p);
		} else {
			hit = CheckOnHit(p, id);
		}
	}
	
	if (yGetDatabaseCount("p"+p+"stars") > 0) {
		yDatabaseNext("p"+p+"stars");
		amt = 0.001 * (trTimeMS() - yGetVar("p"+p+"stars", "last"));
		
		if (amt >= 1) {
			amt = amt * trQuestVarGet("starDamage") * trQuestVarGet("p"+p+"spellDamage");
			ySetVar("p"+p+"stars", "last", trTimeMS());
			yVarToVector("p"+p+"stars", "pos");
			dist = trQuestVarGet("starRadius") * trQuestVarGet("p"+p+"spellRange");
			dist = xsPow(dist, 2);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				yDatabaseNext("enemies", true);
				if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					damageEnemy(p, amt);
				}
			}
		}
		
		if (trTimeMS() > yGetVar("p"+p+"stars", "timeout")) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"stars", true);
			trUnitChangeProtoUnit("Arkantos Boost SFX");
			trUnitSelectClear();
			trUnitSelect(""+(1+trQuestVarGet("p"+p+"stars")));
			trUnitChangeProtoUnit("Arkantos Boost SFX");
			yRemoveFromDatabase("p"+p+"stars");
		}
	}
	
	if (yGetDatabaseCount("p"+p+"starProj") >0) {
		if (trTimeMS() > yDatabaseNext("p"+p+"starProj")) {
			yVarToVector("p"+p+"starProj", "pos");
			vectorSnapToGrid("pos");
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Meteorite");
			dist = trQuestVarGet("empoweredRadius") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0;
			dist = xsPow(dist, 2);
			amt = trQuestVarGet("empoweredDamage") * trQuestVarGet("p"+p+"spellDamage");
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					damageEnemy(p, amt);
				}
			}
			
			gainFavor(p, 3);
			spawnStar(p, "pos");
			yRemoveFromDatabase("p"+p+"starProj");
			trQuestVarSetFromRand("sound", 1, 5, true);
			trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	
	if (yFindLatest("p"+p+"latestProj", "Priest Projectile", p) > 0) {
		trVectorSetUnitPos("center", "p"+p+"latestProj");
		dist = 25;
		target = 0;
		for (x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeBlastmage(p);
			} else if (kbUnitGetAnimationActionType(id) == 12) {
				current = zDistanceToVectorSquared("p"+p+"characters", "center");
				if (current < dist) {
					dist = current;
					target = yGetPointer("p"+p+"characters");
				}
			}
		}
		if (target > 0) {
			ySetPointer("p"+p+"characters", target);
			if (yGetVar("p"+p+"characters", "starfire") >= 1) {
				id = kbGetBlockID(""+1*trQuestVarGet("p"+p+"characters"));
				target = kbUnitGetTargetUnitID(id);
				trQuestVarSet("target", trGetUnitScenarioNameNumber(target));
				ySetVar("p"+p+"characters", "starfire", yGetVar("p"+p+"characters", "starfire") - 1);
				trVectorSetUnitPos("start", "p"+p+"characters");
				trVectorSetUnitPos("target", "target");
				vectorSnapToGrid("target");
				trQuestVarSet("next", trTimeMS() + zDistanceBetweenVectors("start", "target") * 33.3);
				yAddToDatabase("p"+p+"starProj", "next");
				yAddUpdateVar("p"+p+"starProj", "posx", trQuestVarGet("targetx"));
				yAddUpdateVar("p"+p+"starProj", "posz", trQuestVarGet("targetz"));
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"latestProj", true);
				trMutateSelected(kbGetProtoUnitID("Pharaoh of Osiris XP"));
				trSetSelectedScale(0,0,0);
				trUnitOverrideAnimation(50,0,true,false,-1);
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"starfalls") > 0) {
		yDatabaseNext("p"+p+"starfalls", true);
		if (yGetVar("p"+p+"starfalls", "yeehaw") == 1) {
			trMutateSelected(kbGetProtoUnitID("Lampades"));
			trSetSelectedScale(0,0,0);
			trUnitOverrideAnimation(18,0,true,false,-1);
			ySetVar("p"+p+"starfalls", "yeehaw", 0);
		} else if (trTimeMS() > yGetVar("p"+p+"starfalls", "timeout")) {
			yVarToVector("p"+p+"starfalls", "pos");
			trVectorQuestVarSet("dir", vector(1,0,0));
			dist = xsPow(trQuestVarGet("starfallRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
			current = xsPow(trQuestVarGet("starfallStunRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
			amt = trQuestVarGet("starfallDamage") * trQuestVarGet("p"+p+"spellDamage");
			hit = 0;
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					damageEnemy(p, amt, true);
					ySetVar("enemies", "magicResist", yGetVar("enemies", "magicResist") - 0.1);
					hit = hit + 1;
				}
			}
			gainFavor(p, hit);
			trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
			trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
			trArmySelect("1,0");
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Implode Sphere Effect");
			spawnStar(p, "pos");
			yRemoveFromDatabase("p"+p+"starfalls");
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("lapadesconvert.wav","1",-1,"","");
		blastmageSpell(p);
		zSetProtoUnitStat("Kronny Flying", p, 1, 0.1);
		vectorSnapToGrid("p"+p+"wellPos");
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Kronny Flying",1,trQuestVarGet("p"+p+"wellPosX"),0,trQuestVarGet("p"+p+"wellPosz"),0,true);
		trUnitSelectClear();
		trUnitSelectByQV("next", true);
		trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
		trSetSelectedScale(0,3,0);
		trDamageUnitPercent(100);
		yAddToDatabase("p"+p+"starfalls", "next");
		yAddUpdateVar("p"+p+"starfalls", "timeout", trTimeMS() + 1000);
		yAddUpdateVar("p"+p+"starfalls", "yeehaw", 1);
		yAddUpdateVar("p"+p+"starfalls", "posX", trQuestVarGet("p"+p+"wellposX"));
		yAddUpdateVar("p"+p+"starfalls", "posZ", trQuestVarGet("p"+p+"wellposZ"));
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trVectorSetUnitPos("end", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		blastmageSpell(p);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		trSoundPlayFN("sphinxteleportout.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeBlastmage(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				spawnStar(p, "pos");
				target = 1 + xsMin(trQuestVarGet("warpRange") * trQuestVarGet("p"+p+"spellRange"),
					zDistanceBetweenVectors("pos", "end")) / 2;
				trVectorQuestVarSet("step", zGetUnitVector("pos", "end"));
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
				trUnitChangeProtoUnit("Priest");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters");
				trSetUnitOrientation(trVectorQuestVarGet("step"), vector(0,1,0), true);
				trMutateSelected(kbGetProtoUnitID("Priest"));
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Lightning Sparks");
			}
		}
		/* reload relics */
		equipRelicsAgain(p);
	}
	
	if (yGetDatabaseCount("p"+p+"solarFlare") > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"solarFlareNext")) {
			/*
			*
			gainFavor(p, 1);
			/**/
			trQuestVarSet("p"+p+"solarFlareNext", trQuestVarGet("p"+p+"solarFlareNext") + 200);
			yDatabaseNext("p"+p+"solarFlare");
			yVarToVector("p"+p+"solarFlare", "pos");
			amt = trQuestVarGet("solarFlareDamage") * trQuestVarGet("p"+p+"spellDamage");
			dist = trQuestVarGet("solarFlareRange") * trQuestVarGet("p"+p+"spellRange");
			for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
				yDatabaseNext("p"+p+"characters");
				trVectorSetUnitPos("start", "p"+p+"characters");
				vectorSnapToGrid("start");
				trVectorQuestVarSet("dir", zGetUnitVector("start", "pos"));
				for(x=xGetDatabaseCount(dEnemies); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision("enemies", "start", "dir", dist + 2.0, 4.0)) {
						damageEnemy(p, amt);
					}
				}
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("startX"),0,trQuestVarGet("startZ"),0,true);
				trArmySelect("1,0");
				trUnitSetStance("Passive");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trQuestVarSet("dirx", 0.0 - trQuestVarGet("dirx"));
				trQuestVarSet("dirz", 0.0 - trQuestVarGet("dirz"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trUnitHighlight(10.0, false);
				yAddToDatabase("playerLasers", "next");
				yAddUpdateVar("playerLasers", "timeout", trTimeMS() + 1000);
				yAddUpdateVar("playerLasers", "range", dist * 1.4);
			}
			trQuestVarSetFromRand("sound", 1, 5, true);
			trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			yRemoveFromDatabase("p"+p+"solarFlare");
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		gainFavor(p, 0.0 - trQuestVarGet("solarFlareCost") * trQuestVarGet("p"+p+"ultimateCost"));
		blastmageSpell(p);
		trQuestVarSet("p"+p+"solarFlareNext", trTimeMS());
		trSoundPlayFN("petsuchosattack.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"stars"); >0) {
			yDatabaseNext("p"+p+"stars");
			yVarToVector("p"+p+"stars", "pos");
			yAddToDatabase("p"+p+"solarFlare", "p"+p+"stars");
			yAddUpdateVar("p"+p+"solarFlare", "posx", trQuestVarGet("posx"));
			yAddUpdateVar("p"+p+"solarFlare", "posz", trQuestVarGet("posz"));
		}
	}
	
	
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseBlastmage(int eventID = -1) {
	int p = eventID - 1000 - 12 * BLASTMAGE;
	trQuestVarSet("p"+p+"latestProj", trGetNextUnitScenarioNameNumber() - 1);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Starfall";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Solar Flare";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Warp";
		lureIsUltimate = false;
	}
	zSetProtoUnitStat("Priest Projectile", p, 8, 0.0001);
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("starfallCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("warpCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", trQuestVarGet("solarFlareCost"));
}

void modifyBlastmage(int eventID = -1) {
	int p = eventID - 5000 - 12 * BLASTMAGE;
	zSetProtoUnitStat("Priest", p, 13, 1);
}

rule blastmage_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * BLASTMAGE + p, "blastmageAlways");
		trEventSetHandler(1000 + 12 * BLASTMAGE + p, "chooseBlastmage");
		trEventSetHandler(5000 + 12 * BLASTMAGE + p, "modifyBlastmage");
	}
	
	trQuestVarSet("empoweredDamage", 60);
	trQuestVarSet("empoweredRadius", 4);
	
	trQuestVarSet("starDuration", 10);
	trQuestVarSet("starRadius", 4);
	trQuestVarSet("starDamage", 10);
	
	trQuestVarSet("starfallCooldown", 8);
	trQuestVarSet("starfallRadius", 5);
	trQuestVarSet("starfallDamage", 120);
	
	trQuestVarSet("solarFlareCooldown", 20);
	trQuestVarSet("solarFlareDamage", 90);
	trQuestVarSet("solarFlareRange", 40);
	trQuestVarSet("solarFlareCost", 40);
	
	trQuestVarSet("warpCost", 0);
	trQuestVarSet("warpRange", 8);
	trQuestVarSet("warpCooldown", 6);
}
