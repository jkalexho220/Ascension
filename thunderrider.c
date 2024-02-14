const int blitzCooldown = 8;
const float blitzRange = 12;
const float blitzDamage = 80;

const int rechargeCooldown = 22;

const float rideLightningDamage = 100;
const float rideLightningRange = 5;
const float rideLightningCost = 5;

int xThunderRiderPrev = 0;
int xThunderRiderIndex = 0;
int xThunderRiderRechargeSFX = 0;

int xLightningBallDir = 0;
int xLightningBallPrev = 0;
int xLightningBallLast = 0;
int xLightningBallStart = 0;
int xLightningBallDamage = 0;
int xLightningBallYeehaw = 0;

int xShockDamage = 0;
int xShockPos = 0;
int xShockNext = 0;

void removeThunderRider(int p = 0) {
	int balls = trQuestVarGet("p"+p+"lightningBalls");
	int db = getCharactersDB(p);
	if (xSetPointer(balls, xGetInt(db, xThunderRiderIndex))) {
		xFreeDatabaseBlock(balls);
	}
	if (trQuestVarGet("p"+p+"rideLightning") == 1) {
		if (PvP == false) {
			if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex)) == false) {
				debugLog("Unable to restore database block for player " + p);
			}
		}
	}
	removePlayerSpecific(p);
}

void rideLightningOff(int p = 0) {
	zSetProtoUnitStat("Attack Revealer", p, 2, 4.0);
	int index = 0;
	int next = 0;
	int relics = getRelicsDB(p);
	int db = getCharactersDB(p);
	int balls = trQuestVarGet("p"+p+"lightningBalls");
	float dist = 0;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	for(x=xGetDatabaseCount(relics); >0) {
		xDatabaseNext(relics);
		xUnitSelect(relics, xUnitName);
		trUnitChangeProtoUnit("Relic");
	}
	
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		index = xGetInt(db, xThunderRiderIndex);
		xSetPointer(balls, index);
		dir = xGetVector(balls,xLightningBallDir);
		dist = distanceTraveled(xGetInt(balls, xLightningBallLast), 2.0 * xGetFloat(dPlayerData, xPlayerSpeed));
		pos = vectorSetAsCurrentPosition(xGetVector(balls, xLightningBallPrev),dir,dist);
		pos = vectorSnapToGrid(pos);
		xSetVector(db, xThunderRiderPrev, pos);
		
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trSetUnitOrientation(dir,vector(0,1,0),true);
		trUnitChangeProtoUnit("Transport Ship Greek");
		
		xUnitSelectByID(db, xUnitID);
		trMutateSelected(kbGetProtoUnitID("Dwarf"));
		trImmediateUnitGarrison(""+next);
		
		trUnitChangeProtoUnit("Hero Greek Atalanta");
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trUnitChangeProtoUnit("Arkantos God Out");
		
		xUnitSelect(balls, xUnitName);
		trUnitDestroy();
		xUnitSelectByID(db, xUnitID);
		if (PvP) {
			xSetInt(db, xCharIndex,activatePlayerUnit(xGetInt(db, xUnitName),p,kbGetProtoUnitID("Hero Greek Atalanta")));
			xSetBool(dPlayerUnits, xIsHero, true);
			xSetFloat(dPlayerUnits, xPhysicalResist, xGetFloat(dPlayerData, xPlayerPhysicalResist, p));
			xSetFloat(dPlayerUnits, xMagicResist, xGetFloat(dPlayerData, xPlayerMagicResist, p));
			if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
				xSetInt(dPlayerData, xPlayerIndex, xGetInt(db, xCharIndex));
			}
		} else if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex)) == false) {
			debugLog("Thunderstepper " + p + ": Unable to restore database block");
		}
		healUnit(p, trQuestVarGet("p"+p+"rideLightningHeal"), xGetInt(db, xCharIndex));
	}
	
	trQuestVarSet("p"+p+"rideLightningHeal", 0);
	
	equipRelicsAgain(p);
	xClearDatabase(balls);
	xClearDatabase(1*trQuestVarGet("p"+p+"rideLightningTargets"));
	
	if (trCurrentPlayer() == p) {
		reselectMyself();
	}
	trUnitSelectClear();
	xSetBool(dPlayerData, xPlayerLaunched, false);
}

void refreshRideLightningTargets(int p = 0) {
	int targets = trQuestVarGet("p"+p+"rideLightningTargets");
	xClearDatabase(targets);
	for(x=xGetDatabaseCount(dEnemies); >0) {
		xDatabaseNext(dEnemies);
		xAddDatabaseBlock(targets, true);
		xSetInt(targets, xUnitName, xGetInt(dEnemies, xUnitName));
		xSetInt(targets, xDatabaseIndex, xGetPointer(dEnemies));
	}
}


void lightningBallBounce(int p = 0, vector pos = vector(0,0,0)) {
	int balls = trQuestVarGet("p"+p+"lightningBalls");
	zSetProtoUnitStat("Kronny Flying", p, 1, 2.0 * xGetFloat(dPlayerData, xPlayerSpeed));
	xUnitSelect(balls, xUnitName);
	trUnitDestroy();
	
	vectorSnapToGrid(pos);
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch(""+p+",0","Kronny Flying",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	trQuestVarSetFromRand("sound", 1, 3, true);
	if (trUnitVisToPlayer()) {
		trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	trSetUnitOrientation(xGetVector(balls, xLightningBallDir), vector(0,1,0), true);
	trSetSelectedScale(0,-4.7,0);
	trDamageUnitPercent(100);
	
	xSetInt(balls, xUnitName, next);
	xSetVector(balls, xLightningBallPrev, pos);
	xSetInt(balls, xLightningBallStart, trTimeMS());
	
	xSetInt(balls, xLightningBallYeehaw, 1);
}

void thunderRiderAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * THUNDERRIDER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int relics = getRelicsDB(p);
	int balls = trQuestVarGet("p"+p+"lightningBalls");
	int targets = trQuestVarGet("p"+p+"rideLightningTargets");
	int shocks = trQuestVarGet("p"+p+"thunderShocks");
	int shockTargets = trQuestVarGet("p"+p+"thunderShockTargets");
	int next = 0;
	int count = 0;
	float amt = 0;
	float dist = 0;
	
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector loc = vector(0,0,0);
	
	xSetPointer(dPlayerData, p);
	
	if (trQuestVarGet("p"+p+"blitz") == 1) {
		trQuestVarSet("p"+p+"blitz", 0);
		/* stun enemies */
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else if (trCountUnitsInArea(""+xGetInt(dEnemies, xUnitName), p, "Victory Marker", 3) > 0) {
				gainFavor(p, 3);
				damageEnemy(p, blitzDamage * xGetFloat(dPlayerData, xPlayerSpellDamage), true);
				if (trUnitAlive()) {
					stunUnit(dEnemies, 3.0, p);
				}
			}
		}
		for(x=trQuestVarGet("p"+p+"blitzStart"); < trQuestVarGet("p"+p+"blitzEnd")) {
			trUnitSelectClear();
			trUnitSelect(""+x, true);
			trUnitChangeProtoUnit("Lightning Sparks");
		}
	}
	
	if (trQuestVarGet("p"+p+"rideLightning") == 1) {
		for (i=xGetDatabaseCount(balls); > 0) {
			xDatabaseNext(balls);
			xUnitSelect(balls, xUnitName);
			if (xGetInt(balls, xLightningBallYeehaw) == 1) {
				trMutateSelected(kbGetProtoUnitID("Implode Sphere Effect"));
				trUnitSetAnimationPath("0,1,1,0,0,0,0");
				trUnitOverrideAnimation(2,0,true,false,-1);
				trSetSelectedScale(0,0,0);
				trSetSelectedUpVector(0,50.0,0);
				xSetInt(balls, xLightningBallYeehaw, 0);
			} else {
				hit = 0;
				if (trUnitVisToPlayer()) {
					hit = 1;
				}
				prev = xGetVector(balls, xLightningBallPrev);
				dir = xGetVector(balls, xLightningBallDir);
				
				dist = distanceTraveled(xGetInt(balls, xLightningBallLast), 2.0 * xGetFloat(dPlayerData, xPlayerSpeed));
				pos = vectorSetAsCurrentPosition(prev,dir,dist);
				trQuestVarSet("p"+p+"thunderRiderBonus",
					trQuestVarGet("p"+p+"thunderRiderBonus") + dist * 0.1 * xGetFloat(dPlayerData, xPlayerBaseAttack));
				xSetInt(balls, xLightningBallLast, trTimeMS());

				if (cameraLockOnSelf) {
					if (trQuestVarGet("p"+p+"rideLightningIndex") == xGetPointer(balls)) {
						lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
					}
				}
				
				amt = rideLightningRange * xGetFloat(dPlayerData, xPlayerSpellRange);
				dist = dist + amt;
				amt = amt * amt;
				for(x=xGetDatabaseCount(targets); >0) {
					xDatabaseNext(targets);
					xUnitSelect(targets, xUnitName);
					if (trUnitAlive() == false) {
						xFreeDatabaseBlock(targets);
					} else if (rayCollision(targets,prev,dir,dist,amt)) {
						hit = hit * 2;
						if (xSetPointer(dEnemies, xGetInt(targets, xDatabaseIndex))) {
							trUnitHighlight(0.5, false);
							damageEnemy(p, xGetFloat(balls, xLightningBallDamage), true);
						}
						xFreeDatabaseBlock(targets);
					}
				}
				
				trQuestVarSetFromRand("sound", 1, 5, true);
				if (hit > 1) {
					trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
				
				prev = pos;
				pos = pos + (dir * 2.0);
				loc = vectorToGrid(pos);
				if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("implodehit"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					xSetVector(balls, xLightningBallDir, getBounceDir(pos, loc, dir));
					
					amt = rideLightningDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
					xSetFloat(balls, xLightningBallDamage, amt + xGetFloat(balls, xLightningBallDamage));
					lightningBallBounce(p, prev);
					refreshRideLightningTargets(p);
				} else if (trTimeMS() - xGetInt(balls, xLightningBallStart) > 1000) {
					lightningBallBounce(p, prev);
				} else {
					xSetVector(balls, xLightningBallPrev, prev);
				}
			}
		}
		if (trTimeMS() > trQuestVarGet("p"+p+"rideLightningNext")) {
			amt = 1000.0 / trQuestVarGet("p"+p+"rideLightningCost");
			trQuestVarSet("p"+p+"rideLightningNext", trQuestVarGet("p"+p+"rideLightningNext") + amt);
			trQuestVarSet("p"+p+"rideLightningCost", trQuestVarGet("p"+p+"rideLightningCost") + 0.0005 * amt * xGetFloat(dPlayerData, xPlayerUltimateCost));
			gainFavor(p, -1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"rideLightning", 0);
				xSetBool(dPlayerData, xPlayerLaunched, false);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
				rideLightningOff(p);
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		trSoundPlayFN("lightningstrike3.wav", "1", -1, "","");
		if (trQuestVarGet("p"+p+"rideLightning") == 0) {
			trQuestVarSet("p"+p+"blitz", 1);
			trQuestVarSet("p"+p+"blitzStart", trGetNextUnitScenarioNameNumber());
			for(x=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				xUnitSelect(relics, xUnitName);
				trUnitChangeProtoUnit("Relic");
			}
			/* dash */
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeThunderRider(p);
				} else {
					prev = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					
					target = 1 + xsMin(blitzRange * xGetFloat(dPlayerData, xPlayerSpellRange),
						distanceBetweenVectors(prev, pos, false)) / 2;
					dir = getUnitVector(prev, pos);
					for(i=target; >0) {
						trArmyDispatch(""+p+",0", "Dwarf", 1, xsVectorGetX(prev), 0, xsVectorGetZ(prev), 0, true);
						trArmySelect(""+p+",0");
						trUnitChangeProtoUnit("Victory Marker");
						prev = prev + (dir * 2.0);
						if (terrainIsType(vectorToGrid(prev), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							break;
						}
					}
					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, xsVectorGetX(prev),0,xsVectorGetZ(prev),0,true);
					xUnitSelectByID(db, xUnitID);
					trImmediateUnitGarrison(""+next);
					trMutateSelected(kbGetProtoUnitID("Siege Tower"));
					trUnitChangeProtoUnit("Hero Greek Atalanta");
					xUnitSelectByID(db, xUnitID);
					trSetUnitOrientation(dir, vector(0,1,0), true);
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trUnitChangeProtoUnit("Victory Marker");
				}
			}
			trQuestVarSet("p"+p+"blitzEnd", trGetNextUnitScenarioNameNumber());
			/* reload relics */
			equipRelicsAgain(p);
		} else {
			/* ride the lightning direction change */
			for(x=xGetDatabaseCount(balls); >0) {
				xDatabaseNext(balls);
				dist = distanceTraveled(xGetInt(balls, xLightningBallLast), 2.0 * xGetFloat(dPlayerData, xPlayerSpeed));
				prev = vectorSetAsCurrentPosition(xGetVector(balls, xLightningBallPrev),xGetVector(balls, xLightningBallDir),dist);
				dir = getUnitVector(prev, pos);
				xSetVector(balls, xLightningBallDir, dir);
				lightningBallBounce(p, prev);
			}
			refreshRideLightningTargets(p);
		}
	}

	if (trQuestVarGet("p"+p+"rechargeSFX") == 1) {
		if (trQuestVarGet("spyFind") == trQuestVarGet("spyFound")) {
			for(i=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xThunderRiderRechargeSFX);
				trUnitChangeProtoUnit("Spy Eye");
				xUnitSelect(db, xThunderRiderRechargeSFX);
				trMutateSelected(kbGetProtoUnitID("Flying Medic"));
				trUnitOverrideAnimation(18,0,false,false,-1);
				trSetSelectedScale(0,0,0);
			}
			trQuestVarSet("p"+p+"rechargeSFX", 0);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trSoundPlayFN("suckup1.wav","1",-1,"","");
		if (trQuestVarGet("p"+p+"rideLightning") == 1) {
			trQuestVarSet("p"+p+"rideLightningHeal",
				trQuestVarGet("p"+p+"rideLightningHeal") + xGetFloat(dPlayerData, xPlayerAttack));
		} else {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				trUnitHighlight(0.5, false);
				healUnit(p, xGetFloat(dPlayerData, xPlayerAttack), xGetInt(db, xCharIndex));
				if (xGetInt(db, xThunderRiderRechargeSFX) == 0) {
					spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db,xThunderRiderRechargeSFX,xGetPointer(db)));
				}
			}
			trQuestVarSet("p"+p+"rechargeSFX", 1);
		}
		gainFavor(p, 0.1 * xGetFloat(dPlayerData, xPlayerAttack));
		trQuestVarSet("p"+p+"thunderRiderBonus", 0);
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"rideLightning", 1 - trQuestVarGet("p"+p+"rideLightning"));
		if (trQuestVarGet("p"+p+"rideLightning") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"rideLightning", 0);
			} else {
				trQuestVarSet("p"+p+"rideLightningCost", rideLightningCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
				xSetBool(dPlayerData, xPlayerLaunched, true);
				refreshRideLightningTargets(p);
				trQuestVarSet("p"+p+"rideLightningNext", trTimeMS());
				trSoundPlayFN("lightningbirth.wav","1",-1,"","");
				zSetProtoUnitStat("Attack Revealer", p, 2, xGetFloat(dPlayerData, xPlayerLos));
				zSetProtoUnitStat("Kronny Flying", p, 1, 2.0 * xGetFloat(dPlayerData, xPlayerSpeed));
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeThunderRider(p);
					} else {
						prev = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
						dir = getUnitVector(prev, pos);
						
						trQuestVarSet("heading", 180.0 / 3.141592 * angleBetweenVectors(prev, pos));
						next = trGetNextUnitScenarioNameNumber();
						xSetInt(db, xThunderRiderIndex, xAddDatabaseBlock(balls, true));
						xSetInt(balls, xUnitName, next);
						xSetVector(balls, xLightningBallDir, dir);
						xSetVector(balls, xLightningBallPrev, prev);
						xSetInt(balls, xLightningBallLast, trTimeMS());
						xSetInt(balls, xLightningBallStart, trTimeMS());
						xSetFloat(balls, xLightningBallDamage, rideLightningDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));

						if (xGetInt(dPlayerData, xPlayerUnit) == xGetInt(db, xUnitName)) {
							trQuestVarSet("p"+p+"rideLightningIndex", xGetNewestPointer(balls));
						}
						
						
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(prev),0,xsVectorGetZ(prev),0,true);
						trUnitSelectClear();
						trUnitSelect(""+next, true);
						trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
						trSetUnitOrientation(dir, vector(0,1,0), true);
						trSetSelectedScale(0, -4.7, 0);
						trDamageUnitPercent(100);
						
						xUnitSelectByID(db, xUnitID);
						trMutateSelected(kbGetProtoUnitID("Victory Marker"));
						
						if (xSetPointer(dPlayerUnits, xGetInt(db, xCharIndex))) {
							xUnitSelect(dPlayerUnits, xStunSFX);
							trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
							xUnitSelect(dPlayerUnits, xPoisonSFX);
							trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
							if (xGetInt(dPlayerUnits, xStunStatus) > 0) {
								if (xSetPointer(dStunnedUnits, xGetInt(dPlayerUnits, xStunStatus))) {
									xFreeDatabaseBlock(dStunnedUnits);
								}
							}
							if (xGetBool(dPlayerUnits, xLaunched)) {
								for(y=xGetDatabaseCount(dLaunchedUnits); >0) {
									xDatabaseNext(dLaunchedUnits);
									if (xGetInt(dLaunchedUnits, xUnitName) == xGetInt(dPlayerUnits, xUnitName)) {
										xFreeDatabaseBlock(dLaunchedUnits);
									}
								}
								xSetBool(dPlayerUnits, xLaunched, false);
							}
							if (PvP) {
								xRestoreDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
								xFreeDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
								xFreeDatabaseBlock(dPlayerUnits);
							} else if (xDetachDatabaseBlock(dPlayerUnits) == false) {
								debugLog("Unable to detach Thunderrider");
							}
						} else {
							debugLog("Could not set Thunderrider pointer");
						}
					}
				}
			}
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
			rideLightningOff(p);
		}
	}
	
	/* passive attack boost */
	if (xGetDatabaseCount(db) > 0) {
		/* percentage of a second that has passed */
		amt = 0.001 * (trTimeMS() - trQuestVarGet("p"+p+"thunderRiderBonusLast"));
		amt = xsMax(amt, trQuestVarGet("p"+p+"thunderRiderBonus") * amt * 0.1);
		trQuestVarSet("p"+p+"thunderRiderBonusLast", trTimeMS());
		trQuestVarSet("p"+p+"thunderRiderBonus", trQuestVarGet("p"+p+"thunderRiderBonus") - amt);
		if (trQuestVarGet("p"+p+"rideLightning") == 0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeThunderRider(p);
			} else {
				hit = CheckOnHit(p);
				if (hit == ON_HIT_SPECIAL) {
					xClearDatabase(shocks);
					target = trGetUnitScenarioNameNumber(xGetInt(db, xCharAttackTarget));
					pos = kbGetBlockPosition(""+target, true);
					count = 1;
					if (playerHasSymphony(p, SYMPHONY_DOUBLE_SPECIAL)) {
						count = 2;
					}
					for(i=count; >0) {
						xAddDatabaseBlock(shocks, true);
						xSetFloat(shocks, xShockDamage, xGetFloat(dPlayerData, xPlayerAttack));
						xSetInt(shocks, xShockNext, trTimeMS() + 100);
						xSetVector(shocks, xShockPos, pos);
					}
					
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect(""+p+",0");
					trUnitChangeProtoUnit("Lightning Sparks");
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (xGetInt(dEnemies, xUnitName) != target){
							xAddDatabaseBlock(shockTargets, true);
							xSetInt(shockTargets, xUnitName, xGetInt(dEnemies, xUnitName));
							xSetInt(shockTargets, xDatabaseIndex, xGetPointer(dEnemies));
						}
					}
					trQuestVarSetFromRand("sound", 1, 4, true);
					trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
				prev = xGetVector(db, xThunderRiderPrev);
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				if (xsVectorGetX(prev) + xsVectorGetZ(prev) > 0) {
					dist = distanceBetweenVectors(prev, pos, false);
					if (dist == 0) {
						trQuestVarSet("p"+p+"thunderRiderBonus", trQuestVarGet("p"+p+"thunderRiderBonus") - amt);
					} else {
						trQuestVarSet("p"+p+"thunderRiderBonus",
							trQuestVarGet("p"+p+"thunderRiderBonus") + dist * 0.1 * xGetFloat(dPlayerData, xPlayerBaseAttack));
						xSetVector(db, xThunderRiderPrev, pos);
					}
				} else {
					xSetVector(db, xThunderRiderPrev, pos);
				}
			}
		}
		trQuestVarSet("p"+p+"thunderRiderBonus", xsMax(0, trQuestVarGet("p"+p+"thunderRiderBonus")));
		xSetFloat(dPlayerData, xPlayerAttack,
			xGetFloat(dPlayerData, xPlayerBaseAttack) + trQuestVarGet("p"+p+"thunderRiderBonus"));
		zSetProtoUnitStat("Hero Greek Atalanta", p, 27, xGetFloat(dPlayerData, xPlayerAttack));
	}
	
	if (xGetDatabaseCount(shocks) > 0) {
		xDatabaseNext(shocks);
		if (trTimeMS() > xGetInt(shocks, xShockNext)) {
			xSetInt(shocks, xShockNext, 100 + xGetInt(shocks, xShockNext));
			pos = xGetVector(shocks, xShockPos);
			dist = 25;
			target = -1;
			for(x=xGetDatabaseCount(shockTargets); >0) {
				xDatabaseNext(shockTargets);
				xUnitSelect(shockTargets, xUnitName);
				if (trUnitAlive() == false) {
					xFreeDatabaseBlock(shockTargets);
				} else {
					amt = unitDistanceToVector(xGetInt(shockTargets, xUnitName), pos);
					if (amt < dist) {
						target = xGetPointer(shockTargets);
					}
				}
			}
			if (target == -1) {
				xFreeDatabaseBlock(shocks);
			} else {
				xSetPointer(shockTargets, target);
				
				if (xSetPointer(dEnemies, xGetInt(shockTargets, xDatabaseIndex))) {
					xUnitSelectByID(dEnemies, xUnitID);
					trUnitHighlight(0.2, false);
					damageEnemy(p, xGetFloat(shocks, xShockDamage), false);
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect(""+p+",0");
					trUnitChangeProtoUnit("Lightning Sparks Ground");
					xSetVector(shocks, xShockPos, pos);
					OnHit(p, xGetPointer(dEnemies));
				}
				xFreeDatabaseBlock(shockTargets);
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseThunderRider(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * THUNDERRIDER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Blitz";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Recharge";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Ride The Lightning";
		lureIsUltimate = false;
	}
	xThunderRiderPrev = xInitAddVector(db, "prev");
	xThunderRiderIndex = xInitAddInt(db, "index");
	xThunderRiderRechargeSFX = xInitAddInt(db, "rechargeSFX");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, blitzCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,rechargeCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"lightningBalls") == 0) {
		db = xInitDatabase("p"+p+"lightningBalls");
		trQuestVarSet("p"+p+"lightningBalls", db);
		xInitAddInt(db, "name");
		xLightningBallDir = xInitAddVector(db, "dir");
		xLightningBallPrev = xInitAddVector(db, "prev");
		xLightningBallLast = xInitAddInt(db, "last");
		xLightningBallStart = xInitAddInt(db, "start");
		xLightningBallDamage = xInitAddFloat(db, "damage");
		xLightningBallYeehaw = xInitAddInt(db, "yeehaw", 1);
	}
	
	if (trQuestVarGet("p"+p+"rideLightningTargets") == 0) {
		db = xInitDatabase("p"+p+"rideLightningTargets");
		trQuestVarSet("p"+p+"rideLightningTargets", db);
		xInitAddInt(db, "name");
		xInitAddInt(db, "index");
	}
	
	if (trQuestVarGet("p"+p+"thunderShockTargets") == 0) {
		db = xInitDatabase("p"+p+"thunderShockTargets");
		trQuestVarSet("p"+p+"thunderShockTargets", db);
		xInitAddInt(db, "name");
		xInitAddInt(db, "index");
	}
	
	if (trQuestVarGet("p"+p+"thunderShocks") == 0) {
		db = xInitDatabase("p"+p+"thunderShocks");
		trQuestVarSet("p"+p+"thunderShocks", db);
		xShockDamage = xInitAddFloat(db, "damage");
		xShockPos = xInitAddVector(db, "pos");
		xShockNext = xInitAddInt(db, "next");
	}
}

rule thunderRider_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * THUNDERRIDER + p, "thunderRiderAlways");
		trEventSetHandler(1000 + 12 * THUNDERRIDER + p, "chooseThunderRider");
	}
}
