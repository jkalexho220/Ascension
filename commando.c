int shrapnelCooldown = 10;

int echoBombCooldown = 20;
float echoBombDuration = 6;
float echoBombRadius = 12;

float minigunRange = 1.5;

int xShotgunHitboxStartPos = 0;
int xShotgunHitboxStartTime = 0;
int xShotgunHitboxDamage = 0;
int xShotgunHitboxAngle1 = 0;
int xShotgunHitboxAngle2 = 0;
int xShotgunHitboxDist = 0;

int xPelletIncomingStartPos = 0;
int xPelletIncomingStartTime = 0;
int xPelletIncomingType = 0;

int xShotgunPelletStartTime = 0;
int xShotgunPelletStartPos = 0;
int xShotgunPelletPrev = 0;
int xShotgunPelletDir = 0;
int xShotgunPelletType = 0;
int xShotgunPelletDist = 0;

int xShrapnelShotDir = 0;
int xShrapnelShotPrev = 0;
int xShrapnelShotTimeout = 0;

int xEchoBombStartingHealth = 0;
int xEchoBombCurrentHealth = 0;
int xEchoBombSize = 0;
int xEchoBombPos = 0;
int xEchoBombIndex = 0;
int xEchoBombTimeout = 0;
int xEchoBombUnit = 0;

void removeCommando(int p = 0) {
	removePlayerSpecific(p);
}

void minigunOff(int p = 0) {
	trQuestVarSet("p"+p+"minigun", 0);
	xSetInt(dPlayerData, xPlayerFirstDelay, 1000);
	xSetInt(dPlayerData, xPlayerNextDelay, 2000);
	zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, xGetFloat(dPlayerData, xPlayerRange));
	if (trCurrentPlayer() == p) {
		trSetCounterDisplay("Minigun: OFF");
	}
}

void shootShotgun(int p = 0, vector start = vector(0,0,0), vector dir = vector(0,0,0), int count = 3) {
	int shotgunHitboxes = trQuestVarGet("p"+p+"shotgunHitboxes");
	int pelletsIncoming = trQuestVarGet("p"+p+"pelletsIncoming");
	float amt = angleOfVector(dir);
	/* arc angle starts at 33 degrees */
	float dist = 0.5 * (3.0 + count) / 7.0;
	amt = fModulo(6.283185, amt - 0.5 * dist);
	dist = dist / (count - 1);
	xAddDatabaseBlock(shotgunHitboxes, true);
	xSetFloat(shotgunHitboxes, xShotgunHitboxDamage, 0.6 * xGetFloat(dPlayerData, xPlayerAttack) * count);
	xSetVector(shotgunHitboxes, xShotgunHitboxStartPos, start);
	xSetFloat(shotgunHitboxes, xShotgunHitboxAngle1, amt);
	for(x=count; >0) {
		dir = xsVectorSet(xsSin(amt),0,xsCos(amt));
		amt = fModulo(6.283185, amt + dist);
		addGenericProj(pelletsIncoming,start,dir);
		xSetInt(pelletsIncoming, xPelletIncomingType, 0);
		xSetVector(pelletsIncoming, xPelletIncomingStartPos, start);
		xSetInt(pelletsIncoming, xPelletIncomingStartTime, trTimeMS());
	}
	xSetFloat(shotgunHitboxes, xShotgunHitboxAngle2, amt);
	xSetInt(shotgunHitboxes, xShotgunHitboxStartTime, trTimeMS());
}

void commandoAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * COMMANDO;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int shotgunHitboxes = trQuestVarGet("p"+p+"shotgunHitboxes");
	int pelletsIncoming = trQuestVarGet("p"+p+"pelletsIncoming");
	int pellets = trQuestVarGet("p"+p+"pellets");
	int shrapnelShots = trQuestVarGet("p"+p+"shrapnelShots");
	int harpies = trQuestVarGet("p"+p+"fireharpies");
	int echoBombs = trQuestVarGet("p"+p+"echoBombs");
	float amt = 0;
	float dist = 0;
	float current = 0;
	float angleDiff = 0;
	float curDiff = 0;
	float curDist = 0;
	float outer = 0;
	float inner = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector end = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector left = vector(0,0,0);
	vector right = vector(0,0,0);
	
	if (yFindLatest("p"+p+"latestProj", "Javelin Flaming", p) > 0) {
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"latestProj", true);
		trUnitChangeProtoUnit("Rocket");
	}
	
	for(y=xsMin(3 + xGetInt(dPlayerData, xPlayerProjectiles), xGetDatabaseCount(pellets)); >0) {
		xDatabaseNext(pellets);
		dir = xGetVector(pellets, xShotgunPelletDir);
		start = xGetVector(pellets, xShotgunPelletStartPos);
		dist = 0.03 * (trTimeMS() - xGetInt(pellets, xShotgunPelletStartTime));
		pos = xsVectorSet(xsVectorGetX(start) + dist * xsVectorGetX(dir), 0, xsVectorGetZ(start) + dist * xsVectorGetZ(dir));
		
		if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL) || dist > 30.0) {
			xUnitSelect(pellets, xUnitName);
			trUnitChangeProtoUnit("Dust Small");
			xUnitSelect(pellets, xUnitName);
			trDamageUnitPercent(-100);
			if (dist < 30.0) {
				trQuestVarSet("minesound", 1);
			}
			xFreeDatabaseBlock(pellets);
		} else if (xGetInt(pellets,xShotgunPelletType) == 1) {
			hit = 0;
			prev = xGetVector(pellets, xShotgunPelletPrev);
			current = dist - xGetFloat(pellets, xShotgunPelletDist);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision(dEnemies, prev, dir, current + 2.0, 4.0)) {
					hit = xGetPointer(dEnemies);
					damageEnemy(p, xGetFloat(dPlayerData, xPlayerAttack), false);
				}
			}
			if (hit > 0) {
				gainFavor(p, 0.0 - xGetInt(dPlayerData, xPlayerFavorFromAttacks));// don't get favor from minigun
				OnHit(p, hit, false);
				xUnitSelect(pellets, xUnitName);
				trUnitChangeProtoUnit("Lightning Sparks Ground");
				xUnitSelect(pellets, xUnitName);
				trDamageUnitPercent(-100);
				xFreeDatabaseBlock(pellets);
			} else {
				xSetFloat(pellets, xShotgunPelletDist, dist);
				xSetVector(pellets, xShotgunPelletPrev, pos);
			}
		}
	}
	
	if (trQuestVarGet("minesound") == 1) {
		trQuestVarSet("minesound", 0);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	
	for(x=xGetDatabaseCount(pelletsIncoming); >0) {
		if (processGenericProj(pelletsIncoming) == PROJ_BOUNCE) {
			trSetSelectedScale(0.3,0.3,-0.2);
			trUnitSetAnimationPath("3,0,0,0,0,0,0");
			xAddDatabaseBlock(pellets, true);
			xSetInt(pellets, xUnitName, xGetInt(pelletsIncoming, xUnitName));
			xSetInt(pellets, xShotgunPelletType, xGetInt(pelletsIncoming, xPelletIncomingType));
			xSetVector(pellets, xShotgunPelletDir, xGetVector(pelletsIncoming, xProjDir));
			xSetVector(pellets, xShotgunPelletStartPos, xGetVector(pelletsIncoming, xPelletIncomingStartPos));
			xSetVector(pellets, xShotgunPelletPrev, xGetVector(pelletsIncoming, xPelletIncomingStartPos));
			xSetInt(pellets, xShotgunPelletStartTime, xGetInt(pelletsIncoming, xPelletIncomingStartTime));
			xFreeDatabaseBlock(pelletsIncoming);
		}
	}
	
	if (xGetDatabaseCount(shotgunHitboxes) > 0) {
		xDatabaseNext(shotgunHitboxes);
		start = xGetVector(shotgunHitboxes, xShotgunHitboxStartPos);
		amt = xGetFloat(shotgunHitboxes, xShotgunHitboxAngle1);
		left = xsVectorSet(xsSin(amt),0,xsCos(amt));
		amt = xGetFloat(shotgunHitboxes, xShotgunHitboxAngle2);
		right = xsVectorSet(xsSin(amt),0,xsCos(amt));
		
		angleDiff = dotProduct(left, right);
		
		/* projectiles move at 30 units per second */
		dist = (trTimeMS() - xGetInt(shotgunHitboxes, xShotgunHitboxStartTime)) * 0.03;
		/* at close range, enemies are hit no matter what so the angles are 180 degrees apart */
		if (dist < 4.0) {
			left = xsVectorSet(0.0 - xsVectorGetX(right),0, 0.0 - xsVectorGetZ(right));
			angleDiff = -1;
		}
		curDist = dist;
		curDiff = 0;
		amt = xGetFloat(shotgunHitboxes, xShotgunHitboxDamage);
		outer = dist * dist;
		inner = xsPow(xGetFloat(shotgunHitboxes, xShotgunHitboxDist), 2);
		dist = xGetFloat(shotgunHitboxes, xShotgunHitboxDist);
		target = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), start);
				if (current < outer && current > inner) {
					/* enemy is within the donut */
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName));
					dir = getUnitVector(start, pos);
					/* if enemy is in between the old and new positions, that's a hit */
					curDiff = dotProduct(dir, left);
					if (curDiff > angleDiff) {
						if (dotProduct(dir, right) > angleDiff) {
							/* HIT */
							current = xsSqrt(current);
							dist = xsMax(current, dist);
							/* the farther they are, the lower the damage */
							damageEnemy(p, (30.0 - current) / 30.0 * amt, false);
							target = 1;
							gainFavor(p, 1);
							OnHit(p, xGetPointer(dEnemies), false);
						}
					}
				}
			}
		}
		
		if (trTimeMS() > xGetInt(shotgunHitboxes, xShotgunHitboxStartTime) + 1000) {
			xFreeDatabaseBlock(shotgunHitboxes);
		} else if (target == 1) {
			/* move the dist to halfway between */
			xSetFloat(shotgunHitboxes, xShotgunHitboxDist, 0.5 * (curDist + dist));
			trQuestVarSetFromRand("sound", 1,4,true);
			trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		} else if (curDist > xGetFloat(shotgunHitboxes, xShotgunHitboxDist) + 2.0) {
			xSetFloat(shotgunHitboxes, xShotgunHitboxDist, curDist); // catch-up hitbox
		}
	}
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelect(db, xUnitName);
		if (trUnitAlive() == false) {
			removeCommando(p);
		} else {
			hit = CheckOnHit(p,false);
			start = kbGetBlockPosition(""+xGetInt(db, xUnitName));
			if (hit == ON_HIT_NORMAL) {
				if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
					dir = getUnitVector(start, pos);
					trSoundPlayFN("titanfall.wav","1",-1,"","");
					if (trQuestVarGet("p"+p+"minigun") == 1) {
						gainFavor(p, 0.0 - trQuestVarGet("p"+p+"ultimateCost"));
						if (trPlayerResourceCount(p, "favor") == 0) {
							minigunOff(p);
						}
						addGenericProj(pelletsIncoming,start,dir);
						xSetInt(pelletsIncoming, xPelletIncomingType, 1);
						xSetInt(pelletsIncoming, xPelletIncomingStartTime, trTimeMS());
						xSetVector(pelletsIncoming, xPelletIncomingStartPos, start);
					} else {
						trSoundPlayFN("implode start.wav","1",-1,"","");
						shootShotgun(p, start, dir, 3 + xGetInt(dPlayerData, xPlayerProjectiles));
					}
				}
			}
		}
	}
	
	
	if (xGetDatabaseCount(shrapnelShots) > 0) {
		hit = 0;
		xDatabaseNext(shrapnelShots);
		prev = xGetVector(shrapnelShots, xShrapnelShotPrev);
		dir = xGetVector(shrapnelShots, xShrapnelShotDir);
		
		pos = kbGetBlockPosition(""+xGetInt(shrapnelShots, xUnitName));
		if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			hit = 1;
			dir = getBounceDir(pos, vectorToGrid(pos), dir);
		} else {
			xSetVector(shrapnelShots, xShrapnelShotPrev, pos);
		}
		
		
		if (hit == 1) {
			xUnitSelect(shrapnelShots, xUnitName);
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Meteorite");
			xFreeDatabaseBlock(shrapnelShots);
			shootShotgun(p, prev, dir, 6 + 2 * xGetInt(dPlayerData, xPlayerProjectiles));
			trSoundPlayFN("shockwave.wav","1",-1,"","");
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("catapultattack.wav","1",-1,"","");
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			if (trUnitAlive() == false) {
				removeCommando(p);
			} else {
				start = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dir = getUnitVector(start, xGetVector(dPlayerData, xPlayerWellPos));
				end = vectorSetAsTargetVector(start, xGetVector(dPlayerData, xPlayerWellPos), 300.0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Ball of Fire"));
				trUnitOverrideAnimation(18,0,true,false,-1);
				trSetSelectedScale(0.5,0.5,0.5);
				trUnitMoveToPoint(xsVectorGetX(end),0,xsVectorGetZ(end),-1, false);
				xAddDatabaseBlock(shrapnelShots, true);
				xSetInt(shrapnelShots, xUnitName, 1*trQuestVarGet("next"));
				xSetVector(shrapnelShots, xShrapnelShotDir, dir);
				xSetVector(shrapnelShots, xShrapnelShotPrev, start);
				xSetInt(shrapnelShots, xShrapnelShotTimeout, trTimeMS() + 3000);
			}
		}
	}
	
	if (xGetDatabaseCount(harpies) > 0) {
		for (x=xGetDatabaseCount(harpies); >0) {
			xDatabaseNext(harpies);
			xUnitSelect(harpies, xUnitName);
			trMutateSelected(kbGetProtoUnitID("Harpy"));
			trUnitOverrideAnimation(1,0,false,false,-1);
		}
		xClearDatabase(harpies);
	}
	
	if (xGetDatabaseCount(echoBombs) > 0) {
		xDatabaseNext(echoBombs);
		hit = 0;
		if (xGetInt(echoBombs, xUnitName) > 0) {
			target = xGetInt(echoBombs, xEchoBombUnit);
			xUnitSelect(echoBombs, xEchoBombUnit);
			if (trUnitAlive() == false) {
				hit = 1;
				amt = xGetFloat(echoBombs, xEchoBombStartingHealth);
			} else {
				pos = kbGetBlockPosition(""+target);
				xSetVector(echoBombs, xEchoBombPos, pos);
				if (PvP) {
					xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner,
							xGetInt(dEnemies, xDoppelganger, xGetInt(echoBombs, xEchoBombIndex))));
				} else {
					xsSetContextPlayer(ENEMY_PLAYER);
				}
				amt = kbUnitGetCurrentHitpoints(kbGetBlockID(""+target));
				xsSetContextPlayer(0);
				if (trTimeMS() > xGetInt(echoBombs, xEchoBombTimeout)) {
					amt = xGetFloat(echoBombs, xEchoBombStartingHealth) - amt;
					hit = 1;
					if (xSetPointer(dEnemies, xGetInt(echoBombs, xEchoBombIndex))) {
						xSetBool(dEnemies, xEchoBomb, false);
					}
				} else if (amt < xGetFloat(echoBombs, xEchoBombCurrentHealth)) {
					xSetFloat(echoBombs, xEchoBombSize,
						xGetFloat(echoBombs, xEchoBombSize) + 0.002 * (xGetFloat(echoBombs, xEchoBombCurrentHealth) - amt));
					xSetFloat(echoBombs, xEchoBombCurrentHealth, amt);
					amt = xsSqrt(xGetFloat(echoBombs, xEchoBombSize));
					xUnitSelect(echoBombs, xUnitName);
					trSetSelectedScale(amt, amt, amt);
					trUnitHighlight(0.2, false);
				}
			}
			if (hit == 1) {
				pos = xGetVector(echoBombs, xEchoBombPos);
				amt = amt * xGetFloat(dPlayerData, xPlayerSpellDamage);
				dist = xsPow(echoBombRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						damageEnemy(p, amt);
					}
				}
				
				if (amt < 1000) {
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
				} else {
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
					trCameraShake(0.5, 0.3);
				}
				xUnitSelect(echoBombs, xUnitName);
				trUnitDestroy();
				
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				xAddDatabaseBlock(harpies, true);
				xSetInt(harpies, xUnitName, 1*trQuestVarGet("next"));
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitChangeProtoUnit("Kronny Flying");
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trSetSelectedScale(0,-5.0,0);
				trDamageUnitPercent(100);
				
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Meteor Impact Ground");
				xFreeDatabaseBlock(echoBombs);
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = xsPow(12, 2);
		target = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			id = xGetInt(dEnemies, xUnitID);
			trUnitSelectClear();
			trUnitSelectByID(id);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
				if (current < dist) {
					hit = id;
					target = xGetPointer(dEnemies);
					dist = current;
				}
			}
		}
		if (target > 0 && xSetPointer(dEnemies, target) && (xGetBool(dEnemies, xEchoBomb) == false)) {
			xSetBool(dEnemies, xEchoBomb, true);
			xAddDatabaseBlock(echoBombs, true);
			xSetInt(echoBombs, xEchoBombUnit, xGetInt(dEnemies, xUnitName));
			spyEffect(xGetInt(dEnemies, xUnitName), kbGetProtoUnitID("Phoenix Egg"),
				xsVectorSet(echoBombs, xUnitName, xGetPointer(echoBombs)));
			if (PvP) {
				xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner, xGetInt(dEnemies, xDoppelganger)));
			} else {
				xsSetContextPlayer(ENEMY_PLAYER);
			}
			amt = kbUnitGetCurrentHitpoints(hit);
			xsSetContextPlayer(0);
			xSetFloat(echoBombs, xEchoBombStartingHealth, amt);
			xSetFloat(echoBombs, xEchoBombCurrentHealth, amt);
			xSetFloat(echoBombs, xEchoBombSize, 1.0);
			xSetVector(echoBombs, xEchoBombPos, xGetVector(dEnemies, xUnitPos));
			xSetInt(echoBombs, xEchoBombIndex, target);
			xSetInt(echoBombs, xEchoBombTimeout,
				trTimeMS() + 1000 * echoBombDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
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
			xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST);
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
				xSetInt(dPlayerData, xPlayerFirstDelay, 1000 / (2.0 + xGetInt(dPlayerData, xPlayerProjectiles)));
				xSetInt(dPlayerData, xPlayerNextDelay, xGetInt(dPlayerData, xPlayerFirstDelay));
				zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, xGetFloat(dPlayerData, xPlayerRange) * 1.5);
				if (trCurrentPlayer() == p) {
					trSetCounterDisplay("Minigun: ON");
				}
			}
		} else {
			minigunOff(p);
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
}

void chooseCommando(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * COMMANDO;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
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
	
	xSetInt(dPlayerData,xPlayerWellCooldown, shrapnelCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, echoBombCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"shotgunHitboxes") == 0) {
		db = xInitDatabase("p"+p+"shotgunHitboxes");
		trQuestVarSet("p"+p+"shotgunHitboxes", db);
		
		xShotgunHitboxStartPos = xInitAddVector(db, "startPos");
		xShotgunHitboxStartTime = xInitAddInt(db, "startTime");
		xShotgunHitboxDamage = xInitAddFloat(db, "damage");
		xShotgunHitboxAngle1 = xInitAddFloat(db, "angle1");
		xShotgunHitboxAngle2 = xInitAddFloat(db, "angle2");
		xShotgunHitboxDist = xInitAddFloat(db, "dist");
	}
	
	if (trQuestVarGet("p"+p+"pelletsIncoming") == 0) {
		db = initGenericProj("p"+p+"pelletsIncoming", kbGetProtoUnitID("Thor Hammer"),2,30.0,4.5,0.3, p);
		trQuestVarSet("p"+p+"pelletsIncoming", db);
		xPelletIncomingStartPos = xInitAddVector(db, "startPos");
		xPelletIncomingStartTime = xInitAddInt(db, "startTime");
		xPelletIncomingType = xInitAddInt(db, "type");
	}
	
	if (trQuestVarGet("p"+p+"pellets") == 0) {
		db = xInitDatabase("p"+p+"pellets");
		trQuestVarSet("p"+p+"pellets", db);
		xInitAddInt(db, "name");
		xShotgunPelletStartTime = xInitAddInt(db, "startTime");
		xShotgunPelletStartPos = xInitAddVector(db, "startPos");
		xShotgunPelletPrev = xInitAddVector(db, "prev");
		xShotgunPelletDir = xInitAddVector(db, "dir");
		xShotgunPelletType = xInitAddInt(db, "type");
		xShotgunPelletDist = xInitAddFloat(db, "dist");
	}
	
	if (trQuestVarGet("p"+p+"shrapnelShots") == 0) {
		db = xInitDatabase("p"+p+"shrapnelShots");
		trQuestVarSet("p"+p+"shrapnelShots", db);
		xInitAddInt(db, "name");
		xShrapnelShotDir = xInitAddVector(db, "dir");
		xShrapnelShotPrev = xInitAddVector(db, "prev");
		xShrapnelShotTimeout = xInitAddInt(db, "timeout");
	}
	
	if (trQuestVarGet("p"+p+"fireharpies") == 0) {
		db = xInitDatabase("p"+p+"fireharpies");
		trQuestVarSet("p"+p+"fireharpies", db);
		xInitAddInt(db, "name");
	}
	
	if (trQuestVarGet("p"+p+"echoBombs") == 0) {
		db = xInitDatabase("p"+p+"echoBombs");
		trQuestVarSet("p"+p+"echoBombs", db);
		xInitAddInt(db, "name");
		xEchoBombUnit = xInitAddInt(db, "unit");
		xEchoBombStartingHealth = xInitAddFloat(db, "startingHealth");
		xEchoBombCurrentHealth = xInitAddFloat(db, "currentHealth");
		xEchoBombSize = xInitAddFloat(db, "size");
		xEchoBombPos = xInitAddVector(db, "pos");
		xEchoBombIndex = xInitAddInt(db, "index");
		xEchoBombTimeout = xInitAddInt(db, "timeout");
	}
}

void modifyCommando(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * COMMANDO;
	zSetProtoUnitStat("Javelin Cavalry Hero", p, 13, 1);
	if (trQuestVarGet("p"+p+"minigun") == 1) {
		zSetProtoUnitStat("Javelin Cavalry Hero", p, 11, xGetFloat(dPlayerData, xPlayerRange, p) * 1.5);
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
}
