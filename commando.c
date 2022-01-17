void removeCommando(int p = 0) {
	removePlayerSpecific(p);
}

void shootShotgun(int p = 0, string start = "", string dir = "", int count = 3) {
	float amt = angleOfVector(dir);
	/* arc angle starts at 33 degrees */
	float dist = 0.5 * (3.0 + count) / 7.0;
	amt = fModulo(6.283185, amt - 0.5 * dist);
	dist = dist / (count - 1);
	yAddToDatabase("p"+p+"shotgunHitboxes","next");
	yAddUpdateVar("p"+p+"shotgunHitboxes", "damage", 0.5 * trQuestVarGet("p"+p+"attack") * count);
	yAddUpdateVar("p"+p+"shotgunHitboxes", "startx", trQuestVarGet(start+"x"));
	yAddUpdateVar("p"+p+"shotgunHitboxes", "startz", trQuestVarGet(start+"Z"));
	yAddUpdateVar("p"+p+"shotgunHitboxes", "angle1", amt);
	for(x=count; >0) {
		trVectorSetFromAngle("dir", amt);
		amt = fModulo(6.283185, amt + dist);
		addGenericProj("p"+p+"pelletsIncoming",start,dir,kbGetProtoUnitID("Petosuchus Projectile"),2,30.0,4.5,1.5);
	}
	yAddUpdateVar("p"+p+"shotgunHitboxes", "angle2", amt);
	yAddUpdateVar("p"+p+"shotgunHitboxes", "dist", 0);
	yAddUpdateVar("p"+p+"shotgunHitboxes", "startTime", trTimeMS());
}

void commandoAlways(int eventID = -1) {
	int p = eventID - 12 * COMMANDO;
	int id = 0;
	int hit = 0;
	int target = 0;
	int stunned = 0;
	int poisoned = 0;
	int silenced = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	for(x=xsMin(3, yGetDatabaseCount("p"+p+"pellets")); >0) {
		yDatabaseNext("p"+p+"pellets");
		trVectorSetUnitPos("pos", "p"+p+"pellets");
		if (trQuestVarGet("posY") < 0.5 + worldHeight) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"pellets", true);
			trUnitChangeProtoUnit("Dust Small");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"pellets", true);
			trDamageUnitPercent(-100);
			yRemoveFromDatabase("p"+p+"pellets");
		} else {
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"pellets", true);
				trUnitChangeProtoUnit("Dust Small");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"pellets", true);
				trDamageUnitPercent(-100);
				trQuestVarSet("minesound", 1);
				yRemoveFromDatabase("p"+p+"pellets");
			}
		}
	}

	if (trQuestVarGet("minesound") == 1) {
		trQuestVarSet("minesound", 0);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}

	for(x=xsMin(3, yGetDatabaseCount("p"+p+"pelletsIncoming")); >0) {
		if (processGenericProj("p"+p+"pelletsIncoming") == PROJ_BOUNCE) {
			trUnitHighlight(3.0, false);
			yAddToDatabase("p"+p+"pellets", "p"+p+"pelletsIncoming");
			yRemoveFromDatabase("p"+p+"pelletsIncoming");
		}
	}

	if (yGetDatabaseCount("p"+p+"shotgunHitboxes") > 0) {
		yDatabaseNext("p"+p+"shotgunHitboxes");
		yVarToVector("p"+p+"shotgunHitboxes", "start");
		trVectorSetFromAngle("dir1", yGetVar("p"+p+"shotgunHitboxes", "angle1"));
		trVectorSetFromAngle("dir2", yGetVar("p"+p+"shotgunHitboxes", "angle2"));
		trQuestVarSet("angleDiff", dotProduct("dir1", "dir2"));
		/* projectiles move at 30 units per second */
		dist = (trTimeMS() - yGetVar("p"+p+"shotgunHitboxes", "startTime")) * 0.03;
		trQuestVarSet("curDist", dist);
		amt = yGetVar("p"+p+"shotgunHitboxes", "damage");
		trQuestVarSet("outer", dist * dist);
		trQuestVarSet("inner", xsPow(yGetVar("p"+p+"shotgunHitboxes", "dist"), 2));
		dist = yGetVar("p"+p+"shotgunHitboxes", "dist");
		target = 0;
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = zDistanceToVectorSquared("enemies", "start");
				if (current < trQuestVarGet("outer") && current > trQuestVarGet("inner")) {
					/* enemy is within the donut */
					trVectorSetUnitPos("pos", "enemies");
					trVectorQuestVarSet("dir", zGetUnitVector("start", "pos"));
					/* if enemy is in between the old and new positions, that's a hit */
					trQuestVarSet("curDiff", dotProduct("dir", "dir1"));
					if (trQuestVarGet("curDiff") > trQuestVarGet("angleDiff")) {
						if (dotProduct("dir", "dir2") > trQuestVarGet("angleDiff")) {
							/* HIT */
							current = xsSqrt(current);
							dist = xsMax(current, dist);
							/* the farther they are, the lower the damage */
							damageEnemy(p, (30.0 - current) / current * amt, true);
							target = 1;
							trPlayerGrantResources(p, "favor", 1);
						}
					}
				}
			}
		}

		if (trTimeMS() > yGetVar("p"+p+"shotgunHitboxes", "startTime") + 1000) {
			yRemoveFromDatabase("p"+p+"shotgunHitboxes");
		} else if (target == 1) {
			ySetVar("p"+p+"shotgunHitboxes", "dist", dist);
			trQuestVarSetFromRand("sound", 1,4,true);
			trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			ySetVar("p"+p+"shotgunHitboxes", "dist", dist);
		} else if (trQuestVarGet("curDist") > yGetVar("p"+p+"shotgunHitboxes", "dist") + 2.0) {
			ySetVar("p"+p+"shotgunHitboxes", "dist", trQuestVarGet("curDist"));
		}
	}

	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeCommando(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit >= ON_HIT_NORMAL) {
				trPlayerGrantResources(p, "favor", 3);
				if (hit == ON_HIT_SPECIAL) {
					trVectorSetUnitPos("start", "p"+p+"characters");
					if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
						trSoundPlayFN("titanfall.wav","1",-1,"","");
						trSoundPlayFN("implode start.wav","1",-1,"","");
						trVectorSetUnitPos("end", "enemies");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
						shootShotgun(p, "start", "dir", 3 + trQuestVarGet("p"+p+"projectiles"));
					}
				}
			}
		}
	}

	
	if (yGetDatabaseCount("p"+p+"shrapnelShots") > 0) {
		hit = 0;
		yDatabaseNext("p"+p+"shrapnelShots");
		yVarToVector("p"+p+"shrapnelShots", "prev");
		yVarToVector("p"+p+"shrapnelShots", "dir");
		
		trVectorSetUnitPos("pos", "p"+p+"shrapnelShots");
		vectorToGrid("pos", "loc");
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			hit = 1;
			trVectorQuestVarSet("dir", getBounceDir("pos","dir"));
		} else {
			ySetVarFromVector("p"+p+"shrapnelShots", "prev", "pos");
		}
	

		if (hit == 1) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"shrapnelShots");
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Meteorite");
			yRemoveFromDatabase("p"+p+"shrapnelShots");
			shootShotgun(p, "prev", "dir", 5 + 2 * trQuestVarGet("p"+p+"projectiles"));
			trSoundPlayFN("shockwave.wav","1",-1,"","");
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("catapultattack.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeCommando(p);
			} else {
				trVectorSetUnitPos("start", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("start", "p"+p+"wellpos"));
				vectorSetAsTargetVector("target", "start", "p"+p+"wellPos", 300.0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Ball of Fire"));
				trUnitOverrideAnimation(18,0,true,false,-1);
				trSetSelectedScale(0.5,0.5,0.5);
				trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"),-1, false);
				yAddToDatabase("p"+p+"shrapnelShots","next");
				yAddUpdateVar("p"+p+"shrapnelShots", "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar("p"+p+"shrapnelShots", "dirz", trQuestVarGet("dirz"));
				yAddUpdateVar("p"+p+"shrapnelShots", "prevx", trQuestVarGet("startx"));
				yAddUpdateVar("p"+p+"shrapnelShots", "prevz", trQuestVarGet("startz"));
				yAddUpdateVar("p"+p+"shrapnelShots", "timeout", trTimeMS() + 3000);
			}
		}
	}
	

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		
	}


	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseCommando(int eventID = -1) {
	int p = eventID - 1000 - 12 * COMMANDO;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Shrapnel Shot";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Flamethrower";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Echo Bomb";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("shrapnelCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("echoBombCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 2);
	trQuestVarSet("p"+p+"rainCost", 1);
}

rule commando_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * COMMANDO + p, "commandoAlways");
		trEventSetHandler(1000 + 12 * COMMANDO + p, "chooseCommando");
	}

	trQuestVarSet("shotgunCount", 4);

	trQuestVarSet("shrapnelCooldown", 10);
	trQuestVarSet("shrapnelCount", 8);

	trQuestVarSet("echoBombCooldown", 24);
	trQuestVarSet("echoBombDuration", 6);
	trQuestVarSet("echoBombRadius", 12);

	trQuestVarSet("flamethrowerCost", 8);
	trQuestVarSet("flamethrowerDelay", 1000 / trQuestVarGet("flamethrowerCost"));
	trQuestVarSet("flamethrowerRange", 7);
}