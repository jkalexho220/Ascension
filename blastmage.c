
const float empoweredDamage = 90;
const float empoweredRadius = 4;

const float starDuration = 10;
const float starRadius = 4;
const float starDamage = 10;

const int starfallCooldown = 8;
const float starfallRadius = 4;
const float starfallDamage = 120;

const float solarFlareDamage = 100;
const float solarFlareRange = 40;
const float solarFlareCost = 50;

const float missileDamage = 60;
const float missileAcquireRange = 12;
const int missileCooldown = 12;
const int missileLifespan = 8000;

int xStarfire = 0;


int xStarPos = 0;
int xStarTimeout = 0;

int xStarProjPos = 0;
int xStarProjTimeout = 0;

int xStarfallTimeout = 0;
int xStarfallYeehaw = 0;
int xStarfallPos = 0;

int xMissileCenter = 0;
int xMissilePos = 0;
int xMissileDir = 0;
int xMissileTarget = 0;
int xMissileTimeout = 0;
int xMissilePrev = 0;
int xMissileTargetFound = 0;

int xSolarFlarePos = 0;

void removeBlastmage(int p = 0) {
	removePlayerSpecific(p);
}

void blastmageSpell(int p = 0) {
	int db = getCharactersDB(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xSetInt(db, xStarfire, 1 + xGetInt(db, xStarfire));
		if (xGetInt(db, xStarfire) > 3) {
			xSetInt(db, xStarfire, 3);
		}
	}
}

void spawnStar(int p = 0, vector pos = vector(0,0,0)) {
	int next = trGetNextUnitScenarioNameNumber();
	int stars = trQuestVarGet("p"+p+"stars");
	int solarflare = trQuestVarGet("p"+p+"solarFlare");
	trArmyDispatch(""+p+",0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit("Spy Eye");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Outpost"));
	trSetSelectedScale(0,0,0);
	trUnitSelectClear();
	trUnitSelect(""+(next+1), true);
	trMutateSelected(kbGetProtoUnitID("Lighthouse"));
	trSetSelectedScale(0,0,0);
	xAddDatabaseBlock(stars, true);
	xSetInt(stars, xUnitName, next);
	xSetVector(stars, xStarPos, pos);
	xSetInt(stars, xStarTimeout, trTimeMS() + 1000 * starDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	xSetFloat(dPlayerData, xPlayerFavorRegen, xGetFloat(dPlayerData, xPlayerFavorRegen, p) + 0.2, p);
	if (xGetDatabaseCount(solarflare) > 0) {
		xAddDatabaseBlock(solarflare, true);
		xSetVector(solarflare, xSolarFlarePos, pos);
	}
}

void blastmageAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * BLASTMAGE;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int relics = getRelicsDB(p);
	int stars = trQuestVarGet("p"+p+"stars");
	int starfalls = trQuestVarGet("p"+p+"starfalls");
	int starproj = trQuestVarGet("p"+p+"starproj");
	int solarflare = trQuestVarGet("p"+p+"solarFlare");
	int missiles = trQuestVarGet("p"+p+"missiles");
	int missileHitbox = trQuestVarGet("p"+p+"missileHitbox");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector dest = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector prev = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeBlastmage(p);
		} else {
			hit = CheckOnHit(p);
		}
	}
	
	if (xGetDatabaseCount(stars) > 0) {
		xDatabaseNext(stars);
		
		if (trTimeMS() > xGetInt(stars, xStarTimeout)) {
			xSetFloat(dPlayerData, xPlayerFavorRegen, xGetFloat(dPlayerData, xPlayerFavorRegen) - 0.2);
			xUnitSelect(stars, xUnitName);
			trUnitChangeProtoUnit("Arkantos Boost SFX");
			trUnitSelectClear();
			trUnitSelect(""+(1+xGetInt(stars, xUnitName)));
			trUnitChangeProtoUnit("Arkantos Boost SFX");
			xFreeDatabaseBlock(stars);
		}
	}
	
	if (xGetDatabaseCount(starproj) >0) {
		xDatabaseNext(starproj);
		if (trTimeMS() > xGetInt(starproj, xStarProjTimeout)) {
			pos = vectorSnapToGrid(xGetVector(starproj, xStarProjPos));
			next = trGetNextUnitScenarioNameNumber();
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			trUnitSelectClear();
			trUnitSelect(""+next, true);
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Meteorite");
			dist = empoweredRadius * (2.0 + xGetInt(dPlayerData, xPlayerProjectiles)) / 3.0;
			dist = xsPow(dist, 2);
			amt = empoweredDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					damageEnemy(p, amt);
				}
			}
			
			gainFavor(p, 1.0);
			spawnStar(p, pos);
			xFreeDatabaseBlock(starproj);
			trQuestVarSetFromRand("sound", 1, 5, true);
			trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	
	if (yFindLatest("p"+p+"latestProj", "Priest Projectile", p) > 0) {
		pos = kbGetBlockPosition(""+1*trQuestVarGet("p"+p+"latestProj"));
		dist = 25;
		target = 0;
		for (x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			id = xGetInt(db, xUnitID);
			trUnitSelectClear();
			trUnitSelectByID(id);
			if (trUnitAlive() == false) {
				removeBlastmage(p);
			} else if (kbUnitGetAnimationActionType(id) == 12) {
				current = unitDistanceToVector(xGetInt(db, xUnitName), pos);
				if (current < dist) {
					dist = current;
					target = xGetPointer(db);
				}
			}
		}
		if (target > 0) {
			xSetPointer(db, target);
			if (xGetInt(db, xStarfire) >= 1) {
				id = xGetInt(db, xUnitID);
				xsSetContextPlayer(p);
				target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
				xsSetContextPlayer(0);
				
				xSetInt(db, xStarfire, xGetInt(db, xStarfire) - 1);
				pos = vectorSnapToGrid(kbGetBlockPosition(""+target, true));
				next = trTimeMS() + distanceBetweenVectors(kbGetBlockPosition(""+xGetInt(db, xUnitName), true), pos, false) * 33.3;
				xAddDatabaseBlock(starproj, true);
				xSetVector(starproj, xStarProjPos, pos);
				xSetInt(starproj, xStarProjTimeout, next);
				
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"latestProj", true);
				trMutateSelected(kbGetProtoUnitID("Pharaoh of Osiris XP"));
				trSetSelectedScale(0,0,0);
				trUnitOverrideAnimation(50,0,true,false,-1);
			}
		}
	}
	
	if (xGetDatabaseCount(starfalls) > 0) {
		xDatabaseNext(starfalls);
		if (xGetBool(starfalls, xStarfallYeehaw)) {
			xUnitSelect(starfalls, xUnitName);
			trMutateSelected(kbGetProtoUnitID("Lampades"));
			trSetSelectedScale(0,0,0);
			trUnitOverrideAnimation(18,0,true,false,-1);
			xSetBool(starfalls, xStarfallYeehaw, false);
		} else if (trTimeMS() > xGetInt(starfalls, xStarfallTimeout)) {
			pos = xGetVector(starfalls, xStarfallPos);
			dir = vector(1,0,0);
			dist = xsPow(starfallRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			amt = starfallDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
			hit = 0;
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					damageEnemy(p, amt, true);
					stunUnit(dEnemies, 2.0, p);
					hit = hit + 1;
				}
			}
			gainFavor(p, hit);
			trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			trArmySelect("1,0");
			trDamageUnitPercent(100);
			trUnitChangeProtoUnit("Implode Sphere Effect");
			spawnStar(p, pos);
			xFreeDatabaseBlock(starfalls);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("lapadesconvert.wav","1",-1,"","");
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		blastmageSpell(p);
		zSetProtoUnitStat("Kronny Flying", p, 1, 0.1);
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Kronny Flying",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
		trSetSelectedScale(0,3,0);
		trDamageUnitPercent(100);
		xAddDatabaseBlock(starfalls, true);
		xSetInt(starfalls, xUnitName, next);
		xSetInt(starfalls, xStarfallTimeout, trTimeMS() + 1000);
		xSetBool(starfalls, xStarfallYeehaw, true);
		xSetVector(starfalls, xStarfallPos, pos);
	}

	amt = 0.001 * (trTimeMS() - trQuestVarGet("p"+p+"missilesLast"));
	trQuestVarSet("p"+p+"missilesLast", trTimeMS());
	
	if (xGetDatabaseCount(missiles) > 0) {
		if (aiPlanGetUserVariableBool(missiles, xDirtyBit, missileHitbox)) {
			trQuestVarSet("p"+p+"missileHitbox", aiPlanGetUserVariableInt(missiles, xNextBlock, missileHitbox));
		} else {
			trQuestVarSet("p"+p+"missileHitbox", xGetPointer(missiles));
		}
		for(y=xGetDatabaseCount(missiles); >0) {
			xDatabaseNext(missiles);
			if (trTimeMS() > xGetInt(missiles, xMissileTimeout)) {
				xUnitSelect(missiles, xUnitName);
				trUnitDestroy();
				xFreeDatabaseBlock(missiles);
				missileHitbox = xGetPointer(missiles);
			} else {
				pos = xGetVector(missiles, xMissilePos);
				target = 0;
				if (xGetInt(missiles, xMissileTarget) > 0) {
					target = xGetInt(missiles, xMissileTarget);
					trUnitSelectClear();
					trUnitSelect(""+target, true);
					if (trUnitAlive()) {
						dest = kbGetBlockPosition(""+target);
						if (xGetBool(missiles, xMissileTargetFound)) {
							xSetVector(missiles, xMissileDir, xsVectorNormalize(xGetVector(missiles, xMissileDir) + getUnitVector(pos, dest, amt * 100)) * 15.0);
						} else {
							xSetVector(missiles, xMissileDir, xGetVector(missiles, xMissileDir) + getUnitVector(pos, dest, amt * 20.0));
						}
					} else {
						xSetBool(missiles, xMissileTargetFound, false);
						xSetInt(missiles, xMissileTarget, 0);
					}
				}
				pos = pos + xGetVector(missiles, xMissileDir) * amt;
				dir = (pos - xGetVector(missiles, xMissileCenter)) * 3.33;
				xUnitSelect(missiles, xUnitName);
				trSetSelectedUpVector(xsVectorGetX(dir),0.2,xsVectorGetZ(dir));
				xSetVector(missiles, xMissilePos, pos);
				if (xGetPointer(missiles) == missileHitbox) {
					if (distanceBetweenVectors(pos, xGetVector(missiles, xMissileCenter)) > 400.0) {
						next = trGetNextUnitScenarioNameNumber();
						dest = vectorSnapToGrid(pos);
						xSetVector(missiles, xMissileCenter, dest);
						trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(dest),0,xsVectorGetZ(dest),0,true);
						if (next < trGetNextUnitScenarioNameNumber()) {
							xUnitSelect(missiles, xUnitName);
							trUnitDestroy();
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Spy Eye");
							trUnitSelectClear();
							trUnitSelect(""+next, true);
							trMutateSelected(kbGetProtoUnitID("Outpost"));
							trSetSelectedScale(0,0,0);
							xSetInt(missiles, xUnitName, next);
						}
					}
					if (xGetBool(missiles, xMissileTargetFound) == false) {
						// ACQUIRING TARGETS
						dist = xsPow(missileAcquireRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
						if (xGetPointer(missiles) == missileHitbox) {
							for(x=xGetDatabaseCount(dEnemies); >0) {
								xDatabaseNext(dEnemies);
								xUnitSelectByID(dEnemies, xUnitID);
								if (trUnitAlive() == false) {
									removeEnemy();
								} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
									xSetInt(missiles, xMissileTimeout, trTimeMS() + 3000 * xGetFloat(dPlayerData, xPlayerSpellDuration));
									xSetInt(missiles, xMissileTarget, xGetInt(dEnemies, xUnitName));
									xSetBool(missiles, xMissileTargetFound, true);
									break;
								}
							}
						}
					} else {
						dir = getUnitVector(xGetVector(missiles, xMissilePrev), pos);
						dist = distanceBetweenVectors(pos, xGetVector(missiles, xMissilePrev), false) + 1.0;
						prev = xGetVector(missiles, xMissilePrev);
						xSetVector(missiles, xMissilePrev, pos);
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (rayCollision(dEnemies, prev, dir, dist, 2.0)) {
								gainFavor(p, 1.0);
								damageEnemy(p, missileDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));
								spawnStar(p, pos);
								trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								trArmySelect("1,0");
								trUnitChangeProtoUnit("Lightning Sparks");
								trQuestVarSetFromRand("sound", 1, 5, true);
								trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
								xUnitSelect(missiles, xUnitName);
								trUnitDestroy();
								xFreeDatabaseBlock(missiles);
								break;
							}
						}
					}
				}
			}
		}
	}

	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		blastmageSpell(p);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		trSoundPlayFN("lapadesconvert.wav","1",-1,"","");

		for(y=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeBlastmage();
			} else {
				pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(db, xUnitName), true));
				trQuestVarSetFromRand("angle", 0, 3.14, false);
				dir = xsVectorSet(12.0 * xsCos(trQuestVarGet("angle")),0,12.0 * xsSin(trQuestVarGet("angle")));
				for(x=3; >0) {
					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Spy Eye");
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trMutateSelected(kbGetProtoUnitID("Outpost"));
					trSetSelectedScale(0,0,0);
					xAddDatabaseBlock(missiles, true);
					xSetInt(missiles, xUnitName, next);
					xSetVector(missiles, xMissileCenter, pos);
					xSetVector(missiles, xMissilePos, pos);
					xSetVector(missiles, xMissilePrev, pos);
					xSetVector(missiles, xMissileDir, dir);
					xSetInt(missiles, xMissileTimeout, trTimeMS() + missileLifespan * xGetFloat(dPlayerData, xPlayerSpellDuration));
					xSetInt(missiles, xMissileTarget, xGetInt(db, xUnitName));

					target = target + 100;
					dir = rotationMatrix(dir, -0.5, 0.866025);
				}
			}
		}

		trQuestVarSet("p"+p+"missileHitbox", xGetNewestPointer(missiles));
	}
	
	if (xGetDatabaseCount(solarflare) > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"solarFlareNext")) {
			trQuestVarSet("p"+p+"solarFlareNext", trQuestVarGet("p"+p+"solarFlareNext") + 200);
			xDatabaseNext(solarflare);
			dest = xGetVector(solarflare, xSolarFlarePos);
			amt = solarFlareDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
			dist = solarFlareRange * xGetFloat(dPlayerData, xPlayerSpellRange);
			for(y=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(db, xUnitName), true));
				dir = getUnitVector(pos, dest);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision(dEnemies, pos, dir, dist + 2.0, 4.0)) {
						damageEnemy(p, amt);
					}
				}
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitSetStance("Passive");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(vector(0,0,0) - dir,vector(0,1,0),true);
				trUnitHighlight(10.0, false);
				xAddDatabaseBlock(dPlayerLasers, true);
				xSetInt(dPlayerLasers, xUnitName, next);
				xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 1000);
				xSetFloat(dPlayerLasers, xPlayerLaserRange, dist * 1.4);
			}
			trQuestVarSetFromRand("sound", 1, 5, true);
			trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			xFreeDatabaseBlock(solarflare);
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		spawnStar(p, xGetVector(dPlayerData, xPlayerLurePos));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		gainFavor(p, 0.0 - solarFlareCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		blastmageSpell(p);
		trQuestVarSet("p"+p+"solarFlareNext", trTimeMS());
		trSoundPlayFN("petsuchosattack.wav","1",-1,"","");
		for(x=xGetDatabaseCount(stars); >0) {
			xDatabaseNext(stars);
			xAddDatabaseBlock(solarflare, true);
			xSetVector(solarflare, xSolarFlarePos, xGetVector(stars, xStarPos));
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseBlastmage(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * BLASTMAGE;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	trQuestVarSet("p"+p+"latestProj", trGetNextUnitScenarioNameNumber() - 1);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Starfall";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Magic Missiles";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Solar Flare";
		lureIsUltimate = true;
	}
	xStarfire = xInitAddInt(db, "starfire");
	zSetProtoUnitStat("Priest Projectile", p, 8, 0.0001);
	
	xSetInt(dPlayerData,xPlayerWellCooldown, starfallCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, solarFlareCost);
	xSetInt(dPlayerData,xPlayerRainCooldown, missileCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"stars") == 0) {
		db = xInitDatabase("p"+p+"stars");
		trQuestVarSet("p"+p+"stars", db);
		xInitAddInt(db, "name");
		xStarPos = xInitAddVector(db, "pos");
		xStarTimeout = xInitAddInt(db, "timeout");
	}

	if (trQuestVarGet("p"+p+"missiles") == 0) {
		db = xInitDatabase("p"+p+"missiles");
		trQuestVarSet("p"+p+"missiles", db);
		xInitAddInt(db, "name");
		xMissileCenter = xInitAddVector(db, "center");
		xMissilePos = xInitAddVector(db, "pos");
		xMissileDir = xInitAddVector(db, "dir");
		xMissilePrev = xInitAddVector(db, "prev");
		xMissileTarget = xInitAddInt(db, "targetIndex");
		xMissileTimeout = xInitAddInt(db, "timeout");
		xMissileTargetFound = xInitAddBool(db, "targetFound", false);
	}
	
	if (trQuestVarGet("p"+p+"starproj") == 0) {
		db = xInitDatabase("p"+p+"starproj");
		trQuestVarSet("p"+p+"starproj", db);
		xStarProjPos = xInitAddVector(db, "pos");
		xStarProjTimeout = xInitAddInt(db, "timeout");
	}
	
	if (trQuestVarGet("p"+p+"starfalls") == 0) {
		db = xInitDatabase("p"+p+"starfalls");
		trQuestVarSet("p"+p+"starfalls", db);
		xInitAddInt(db, "name");
		xStarfallTimeout = xInitAddInt(db, "timeout");
		xStarfallYeehaw = xInitAddBool(db, "yeehaw");
		xStarfallPos = xInitAddVector(db, "pos");
	}
	
	if (trQuestVarGet("p"+p+"solarFlare") == 0) {
		db = xInitDatabase("p"+p+"solarFlare");
		trQuestVarSet("p"+p+"solarFlare", db);
		xSolarFlarePos = xInitAddVector(db, "pos");
	}
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
}
