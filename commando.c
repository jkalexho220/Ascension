void removeCommando(int p = 0) {
	for(y=3; >0) {
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "chimera"+y));
		trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
	}
	removePlayerSpecific(p);
}

void minigunOff(int p = 0) {
	trQuestVarSet("p"+p+"minigun", 0);
	trQuestVarSet("p"+p+"firstDelay", 1000);
	trQuestVarSet("p"+p+"nextDelay", 2000);
	zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, trQuestVarGet("p"+p+"RANGE"));
	if (trCurrentPlayer() == p) {
		trSetCounterDisplay("Minigun: OFF");
	}
}

void shootShotgun(int p = 0, string start = "", string dir = "", int count = 3) {
	float amt = angleOfVector(dir);
	/* arc angle starts at 33 degrees */
	float dist = 0.5 * (3.0 + count) / 7.0;
	amt = fModulo(6.283185, amt - 0.5 * dist);
	dist = dist / (count - 1);
	yAddToDatabase("p"+p+"shotgunHitboxes","next");
	yAddUpdateVar("p"+p+"shotgunHitboxes", "damage", 0.6 * trQuestVarGet("p"+p+"attack") * count);
	yAddUpdateVar("p"+p+"shotgunHitboxes", "startx", trQuestVarGet(start+"x"));
	yAddUpdateVar("p"+p+"shotgunHitboxes", "startz", trQuestVarGet(start+"Z"));
	yAddUpdateVar("p"+p+"shotgunHitboxes", "angle1", amt);
	for(x=count; >0) {
		trVectorSetFromAngle("dir", amt);
		amt = fModulo(6.283185, amt + dist);
		addGenericProj("p"+p+"pelletsIncoming",start,dir,kbGetProtoUnitID("Thor Hammer"),2,30.0,4.5,0.3, p);
		yAddUpdateVar("p"+p+"pelletsIncoming", "type", 0);
		yAddUpdateVar("p"+p+"pelletsIncoming", "startx", trQuestVarGet(start+"x"));
		yAddUpdateVar("p"+p+"pelletsIncoming", "startz", trQuestVarGet(start+"z"));
		yAddUpdateVar("p"+p+"pelletsIncoming", "start", trTimeMS());
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
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	
	if (yFindLatest("p"+p+"latestProj", "Javelin Flaming", p) > 0) {
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"latestProj", true);
		trUnitChangeProtoUnit("Rocket");
	}
	
	for(y=xsMin(3 + trQuestVarGet("p"+p+"projectiles"), yGetDatabaseCount("p"+p+"pellets")); >0) {
		yDatabaseNext("p"+p+"pellets");
		yVarToVector("p"+p+"pellets", "dir");
		yVarToVector("p"+p+"pellets", "start");
		dist = 0.03 * (trTimeMS() - yGetVar("p"+p+"pellets", "start"));
		trQuestVarSet("posx", trQuestVarGet("startx") + dist * trQuestVarGet("dirx"));
		trQuestVarSet("posz", trQuestVarGet("startz") + dist * trQuestVarGet("dirz"));
		
		vectorToGrid("pos", "loc");
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) || dist > 30.0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"pellets", true);
			trUnitChangeProtoUnit("Dust Small");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"pellets", true);
			trDamageUnitPercent(-100);
			if (dist < 30.0) {
				trQuestVarSet("minesound", 1);
			}
			yRemoveFromDatabase("p"+p+"pellets");
		} else if (yGetVar("p"+p+"pellets","type") == 1) {
			hit = 0;
			yVarToVector("p"+p+"pellets", "prev");
			current = dist - yGetVar("p"+p+"pellets", "dist");
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision("enemies", "prev", "dir", current + 2.0, 4.0)) {
					hit = yGetPointer("enemies");
					damageEnemy(p, trQuestVarGet("p"+p+"attack"), false);
				}
			}
			if (hit > 0) {
				gainFavor(p, 0.0 - trQuestVarGet("p"+p+"favorFromAttacks"));// don't get favor from minigun
				OnHit(p, hit, false);
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"pellets", true);
				trUnitChangeProtoUnit("Lightning Sparks Ground");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"pellets", true);
				trDamageUnitPercent(-100);
				yRemoveFromDatabase("p"+p+"pellets");
			} else {
				ySetVar("p"+p+"pellets", "dist", dist);
				ySetVarFromVector("p"+p+"pellets", "prev", "pos");
			}
		}
	}
	
	if (trQuestVarGet("minesound") == 1) {
		trQuestVarSet("minesound", 0);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	
	for(x=yGetDatabaseCount("p"+p+"pelletsIncoming"); >0) {
		if (processGenericProj("p"+p+"pelletsIncoming") == PROJ_BOUNCE) {
			trSetSelectedScale(0.3,0.3,-0.2);
			trUnitSetAnimationPath("3,0,0,0,0,0,0");
			yAddToDatabase("p"+p+"pellets", "p"+p+"pelletsIncoming");
			yAddUpdateVar("p"+p+"pellets", "type", yGetVar("p"+p+"pelletsIncoming", "type"));
			yAddUpdateVar("p"+p+"pellets", "dirx", yGetVar("p"+p+"pelletsIncoming", "dirx"));
			yAddUpdateVar("p"+p+"pellets", "dirz", yGetVar("p"+p+"pelletsIncoming", "dirz"));
			yAddUpdateVar("p"+p+"pellets", "prevx", yGetVar("p"+p+"pelletsIncoming", "startx"));
			yAddUpdateVar("p"+p+"pellets", "prevz", yGetVar("p"+p+"pelletsIncoming", "startz"));
			yAddUpdateVar("p"+p+"pellets", "startx", yGetVar("p"+p+"pelletsIncoming", "startx"));
			yAddUpdateVar("p"+p+"pellets", "startz", yGetVar("p"+p+"pelletsIncoming", "startz"));
			yAddUpdateVar("p"+p+"pellets", "dist", 0);
			yAddUpdateVar("p"+p+"pellets", "start", yGetVar("p"+p+"pelletsIncoming", "start"));
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
		/* at close range, enemies are hit no matter what */
		if (dist < 4.0) {
			trQuestVarSet("dir2X", 0.0 - trQuestVarGet("dir1X"));
			trQuestVarSet("dir2Z", 0.0 - trQuestVarGet("dir1Z"));
			trQuestVarSet("angleDiff", -1);
		}
		trQuestVarSet("curDist", dist);
		amt = yGetVar("p"+p+"shotgunHitboxes", "damage");
		trQuestVarSet("outer", dist * dist);
		trQuestVarSet("inner", xsPow(yGetVar("p"+p+"shotgunHitboxes", "dist"), 2));
		dist = yGetVar("p"+p+"shotgunHitboxes", "dist");
		target = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
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
							damageEnemy(p, (30.0 - current) / 30.0 * amt, false);
							target = 1;
							gainFavor(p, 1);
							OnHit(p, yGetPointer("enemies"));
						}
					}
				}
			}
		}
		
		if (trTimeMS() > yGetVar("p"+p+"shotgunHitboxes", "startTime") + 1000) {
			yRemoveFromDatabase("p"+p+"shotgunHitboxes");
		} else if (target == 1) {
			/* move the dist to halfway between */
			ySetVar("p"+p+"shotgunHitboxes", "dist", 0.5 * (trQuestVarGet("curDist") + dist));
			trQuestVarSetFromRand("sound", 1,4,true);
			trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		} else if (trQuestVarGet("curDist") > yGetVar("p"+p+"shotgunHitboxes", "dist") + 2.0) {
			ySetVar("p"+p+"shotgunHitboxes", "dist", trQuestVarGet("curDist"));
		}
	}
	
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeCommando(p);
		} else {
			hit = CheckOnHit(p, id, false);
			trVectorSetUnitPos("start", "p"+p+"characters");
			if (hit == ON_HIT_NORMAL) {
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					trVectorSetUnitPos("end", "enemies");
					trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
					trSoundPlayFN("titanfall.wav","1",-1,"","");
					if (trQuestVarGet("p"+p+"minigun") == 1) {
						gainFavor(p, 0.0 - trQuestVarGet("p"+p+"ultimateCost"));
						if (trPlayerResourceCount(p, "favor") == 0) {
							minigunOff(p);
						}
						addGenericProj("p"+p+"pelletsIncoming","start","dir",kbGetProtoUnitID("Thor Hammer"),2,30.0,4.5,0.3, p);
						yAddUpdateVar("p"+p+"pelletsIncoming", "type", 1);
						yAddUpdateVar("p"+p+"pelletsIncoming", "startx", trQuestVarGet("startx"));
						yAddUpdateVar("p"+p+"pelletsIncoming", "startz", trQuestVarGet("startz"));
						yAddUpdateVar("p"+p+"pelletsIncoming", "start", trTimeMS());
					} else {
						trSoundPlayFN("implode start.wav","1",-1,"","");
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
			trVectorQuestVarSet("dir", getBounceDir("loc","dir"));
		} else {
			ySetVarFromVector("p"+p+"shrapnelShots", "prev", "pos");
		}
		
		
		if (hit == 1) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"shrapnelShots");
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Meteorite");
			yRemoveFromDatabase("p"+p+"shrapnelShots");
			shootShotgun(p, "prev", "dir", 6 + 2 * trQuestVarGet("p"+p+"projectiles"));
			trSoundPlayFN("shockwave.wav","1",-1,"","");
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
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
	
	if (yGetDatabaseCount("p"+p+"fireharpies") > 0) {
		for (x=yGetDatabaseCount("p"+p+"fireharpies"); >0) {
			yDatabaseNext("p"+p+"fireharpies", true);
			trMutateSelected(kbGetProtoUnitID("Harpy"));
			trUnitOverrideAnimation(1,0,false,false,-1);
		}
		yClearDatabase("p"+p+"fireharpies");
	}
	
	if (yGetDatabaseCount("p"+p+"echoBombs") > 0) {
		yDatabaseNext("p"+p+"echoBombs");
		hit = 0;
		if (trQuestVarGet("p"+p+"echoBombs") > 0) {
			xsSetContextPlayer(ENEMY_PLAYER);
			target = 1*yGetVar("p"+p+"echoBombs", "unit");
			trUnitSelectClear();
			trUnitSelect(""+target);
			if (trUnitAlive() == false) {
				hit = 1;
				amt = yGetVar("p"+p+"echoBombs", "health");
			} else {
				trVectorQuestVarSet("pos", kbGetBlockPosition(""+target));
				ySetVarFromVector("p"+p+"echoBombs", "pos", "pos");
				if (PvP) {
					xsSetContextPlayer(1*yGetVarAtIndex("playerUnits", "player", 1*yGetVar("enemies", "doppelganger")));
				}
				amt = kbUnitGetCurrentHitpoints(kbGetBlockID(""+target));
				if (trTimeMS() > yGetVar("p"+p+"echoBombs", "timeout")) {
					amt = yGetVar("p"+p+"echoBombs", "health") - amt;
					hit = 1;
					if (ySetPointer("enemies", 1*yGetVar("p"+p+"echoBombs", "index"))) {
						ySetVar("enemies", "bomb", 0);
					}
				} else if (amt < yGetVar("p"+p+"echoBombs", "currenthealth")) {
					ySetVar("p"+p+"echoBombs", "size",
						yGetVar("p"+p+"echoBombs", "size") + 0.002 * (yGetVar("p"+p+"echoBombs", "currenthealth") - amt));
					ySetVar("p"+p+"echoBombs", "currenthealth", amt);
					amt = xsSqrt(yGetVar("p"+p+"echoBombs", "size"));
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"echoBombs", true);
					trSetSelectedScale(amt, amt, amt);
					trUnitHighlight(0.2, false);
				}
				xsSetContextPlayer(p);
			}
			if (hit == 1) {
				yVarToVector("p"+p+"echoBombs", "pos");
				amt = amt * trQuestVarGet("p"+p+"spellDamage");
				dist = xsPow(trQuestVarGet("echoBombRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
						damageEnemy(p, amt);
					}
				}
				
				if (amt < 1000) {
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
				} else {
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
					trCameraShake(0.5, 0.3);
				}
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"echoBombs", true);
				trUnitDestroy();
				
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				yAddToDatabase("p"+p+"fireHarpies", "next");
				trUnitSelectClear();
				trUnitSelect(""+1*trQuestVarGet("next"), true);
				trUnitChangeProtoUnit("Kronny Flying");
				trUnitSelectClear();
				trUnitSelect(""+1*trQuestVarGet("next"), true);
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trSetSelectedScale(0,-5.0,0);
				trDamageUnitPercent(100);
				
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Meteor Impact Ground");
				yRemoveFromDatabase("p"+p+"echoBombs");
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		dist = xsPow(12, 2);
		target = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = zDistanceToVectorSquared("enemies", "pos");
				if (current < dist) {
					hit = id;
					target = yGetPointer("enemies");
					dist = current;
				}
			}
		}
		if (target > 0 && ySetPointer("enemies", target) && yGetVar("enemies", "bomb") == 0) {
			trVectorSetUnitPos("pos", "enemies");
			trQuestVarSet("next", -1);
			yAddToDatabase("p"+p+"echoBombs", "next");
			yAddUpdateVar("p"+p+"echoBombs", "unit", trQuestVarGet("enemies"));
			spyEffect(1*trQuestVarGet("enemies"),kbGetProtoUnitID("Phoenix Egg"),yGetNewestName("p"+p+"echoBombs"));
			if (PvP) {
				xsSetContextPlayer(1*yGetVarAtIndex("playerUnits", "player", 1*yGetVar("enemies", "doppelganger")));
			} else {
				xsSetContextPlayer(ENEMY_PLAYER);
			}
			amt = kbUnitGetCurrentHitpoints(hit);
			yAddUpdateVar("p"+p+"echoBombs", "health", amt);
			yAddUpdateVar("p"+p+"echoBombs", "currenthealth", amt);
			yAddUpdateVar("p"+p+"echoBombs", "size", 1.0);
			yAddUpdateVar("p"+p+"echoBombs", "posx", trQuestVarGet("posx"));
			yAddUpdateVar("p"+p+"echoBombs", "posz", trQuestVarGet("posz"));
			yAddUpdateVar("p"+p+"echoBombs", "index", target);
			yAddUpdateVar("p"+p+"echoBombs", "timeout",
				trTimeMS() + 1000 * trQuestVarGet("echoBombDuration") * trQuestVarGet("p"+p+"spellDuration"));
			xsSetContextPlayer(p);
			trSoundPlayFN("siegeselect.wav", "1", -1, "","");
			trSoundPlayFN("gatherpoint.wav","1",-1,"","");
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				if (target > 0) {
					trChatSend(0, "An enemy can only have one Echo Bomb on it at a time!");
				} else {
					trChatSend(0, "You must target an enemy with this ability!");
				}
			}
			trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_COST);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"minigun", 1 - trQuestVarGet("p"+p+"minigun"));
		if (trQuestVarGet("p"+p+"minigun") == 1) {
			if (trPlayerResourceCount(p, "favor") == 0) {
				trQuestVarSet("p"+p+"minigun", 0);
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
					trChatSend(0, "Not enough favor to activate minigun!");
				}
			} else {
				trSoundPlayFN("storehouse.wav","1",-1,"","");
				trQuestVarSet("p"+p+"firstDelay", 1000 / (2.0 + trQuestVarGet("p"+p+"projectiles")));
				trQuestVarSet("p"+p+"nextDelay", trQuestVarGet("p"+p+"firstDelay"));
				zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, trQuestVarGet("p"+p+"RANGE") * 1.5);
				if (trCurrentPlayer() == p) {
					trSetCounterDisplay("Minigun: ON");
				}
			}
		} else {
			minigunOff(p);
		}
	}
	
	ySetPointer("enemies", index);
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
		rainName = "(E) Minigun";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Echo Bomb";
		lureIsUltimate = false;
		trSetCounterDisplay("Minigun: OFF");
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("shrapnelCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("echoBombCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

void modifyCommando(int eventID = -1) {
	int p = eventID - 5000 - 12 * COMMANDO;
	zSetProtoUnitStat("Javelin Cavalry Hero", p, 13, 1);
	if (trQuestVarGet("p"+p+"minigun") == 1) {
		zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, trQuestVarGet("p"+p+"RANGE") * 1.5);
	}
}

rule commando_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * COMMANDO + p, "commandoAlways");
		trEventSetHandler(1000 + 12 * COMMANDO + p, "chooseCommando");
		trEventSetHandler(5000 + 12 * COMMANDO + p, "modifyCommando");
	}
	
	trQuestVarSet("shrapnelCooldown", 10);
	
	trQuestVarSet("echoBombCooldown", 20);
	trQuestVarSet("echoBombDuration", 6);
	trQuestVarSet("echoBombRadius", 12);
	
	trQuestVarSet("minigunRange", 1.5);
}
