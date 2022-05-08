int sunlightCooldown = 18;
float sunlightRadius = 6;
float sunlightDuration = 6;
float sunlightPower = 60;

int healingRaysCooldown = 12;
float healingRaysPower = 100;
float healingRaysDuration = 6;

float searingDelay = 200; // 1000 / 5

int xSearingSFX = 0;

int xSunlightRadius = 0;
int xSunlightDamage = 0;
int xSunlightTimeout = 0;
int xSunlightStart = 0;
int xSunlightEnd = 0;
int xSunlightPos = 0;
int xSunlightNext = 0;

void removeSunbow(int p = 0) {
	removePlayerSpecific(p);
}

void sunbowAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * SUNBOW;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int sunlights = trQuestVarGet("p"+p+"sunlights");
	float amt = 0;
	float dist = 0;
	xSetPointer(dPlayerData, p);
	
	vector end = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeSunbow(p);
		} else {
			hit = CheckOnHit(p);
			if (hit == ON_HIT_NORMAL) {
				if (trQuestVarGet("p"+p+"healingRays") == 1) {
					hit = 0;
					target = trGetUnitScenarioNameNumber(xGetInt(db, xCharAttackTarget));
					end = kbGetBlockPosition(""+target, true);
					pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dir = getUnitVector(pos, end);
					
					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trUnitHighlight(1.0, false);
					trSetUnitOrientation(xsVectorSet(0.0 - xsVectorGetX(dir),0, 0.0 - xsVectorGetZ(dir)), vector(0,1,0), true);
					xAddDatabaseBlock(dPlayerLasers, true);
					xSetInt(dPlayerLasers, xUnitName, next);
					xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 500);
					xSetFloat(dPlayerLasers, xPlayerLaserRange, xGetFloat(dPlayerData, xPlayerRange) * 1.4);
					amt = healingRaysPower * xGetFloat(dPlayerData, xPlayerSpellDamage);
					dist = xGetFloat(dPlayerData, xPlayerRange) + 3.0;
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits, pos, dir, dist, 9.0)) {
							healUnit(p, amt);
							hit = hit + 1;
						}
					}
					if (trQuestVarGet("p"+p+"searing") == 1) {
						amt = amt * xGetFloat(dPlayerData, xPlayerHealBoost);
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (rayCollision(dEnemies, pos, dir, dist, 9.0)) {
								hit = hit + 1;
								damageEnemy(p, amt, true);
								OnHit(p, xGetPointer(dEnemies), true);
							}
						}
					}
					gainFavor(p, hit);
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("restorationbirth.wav","1",-1,"","");
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		xAddDatabaseBlock(sunlights, true);
		xSetFloat(sunlights, xSunlightRadius, xsPow(sunlightRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2));
		xSetFloat(sunlights, xSunlightDamage, sunlightPower * xGetFloat(dPlayerData, xPlayerSpellDamage));
		xSetInt(sunlights, xSunlightTimeout,
			trTimeMS() + 1000 * sunlightDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		xSetInt(sunlights, xSunlightStart, trGetNextUnitScenarioNameNumber());
		xSetVector(sunlights, xSunlightPos, pos);
		xSetInt(sunlights, xSunlightNext, trTimeMS());
		dir = xsVectorSet(sunlightRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 0, 0);
		for(x=0; <16) {
			next = trGetNextUnitScenarioNameNumber();
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(dir)+xsVectorGetX(pos),0,xsVectorGetZ(dir)+xsVectorGetZ(pos),0,true);
			trArmySelect(""+p+",0");
			trUnitChangeProtoUnit("Relic");
			trUnitSelectClear();
			trUnitSelect(""+next, true);
			trMutateSelected(kbGetProtoUnitID("Hero Birth"));
			dir = rotationMatrix(dir, 0.92388, 0.382683); // rotate 22.5 degrees
		}
		xSetInt(sunlights, xSunlightEnd, trGetNextUnitScenarioNameNumber());
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"healingrays", 1);
		trQuestVarSet("p"+p+"healingRaysTimeout",
			trTimeMS() + 1000 * healingRaysDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		trSoundPlayFN("skypassagein.wav","1",-1,"","");
		xSetInt(dPlayerData, xPlayerNextDelay,
			xGetInt(dClass, xClassNextDelay, SUNBOW) * 2.0 / (1.0 + xGetInt(dPlayerData, xPlayerProjectiles)));
	}
	
	if ((trQuestVarGet("p"+p+"healingRays") == 1) && (trTimeMS() > trQuestVarGet("p"+p+"healingRaysTimeout"))) {
		trQuestVarSet("p"+p+"healingRays", 0);
		trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
		xSetInt(dPlayerData, xPlayerNextDelay, xGetInt(dClass, xClassNextDelay, SUNBOW));
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"searing", 1 - trQuestVarGet("p"+p+"searing"));
		if (trQuestVarGet("p"+p+"searing") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"searing", 0);
			} else {
				trQuestVarSet("p"+p+"searingNext",
					trTimeMS() + searingDelay / xGetFloat(dPlayerData, xPlayerUltimateCost));
				trSoundPlayFN("forestfirebirth.wav","1",-1,"","");
			}
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
		
		if (trQuestVarGet("p"+p+"searing") == 1) {
			target = kbGetProtoUnitID("Ball of Fire Impact");
		} else {
			target = kbGetProtoUnitID("Rocket");
		}
		
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			if ((xGetInt(db, xSearingSFX) == 0) ||
				(kbGetBlockID(""+xGetInt(db, xSearingSFX)) == -1)) {
				xUnitSelectByID(db, xUnitID);
				spyEffect(xGetInt(db, xUnitName),target, xsVectorSet(db, xSearingSFX, xGetPointer(db)));
			} else {
				xUnitSelect(db, xSearingSFX);
				trMutateSelected(target);
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"searing") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"searingNext")) {
			trQuestVarSet("p"+p+"searingNext",
				trQuestVarGet("p"+p+"searingNext") + searingDelay / xGetFloat(dPlayerData, xPlayerUltimateCost));
			gainFavor(p, 0 - 1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"searing", 0);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelect(db, xSearingSFX);
					trMutateSelected(kbGetProtoUnitID("Rocket"));
				}
			}
		}
	}
	
	if (xGetDatabaseCount(sunlights) > 0) {
		target = 0;
		xDatabaseNext(sunlights);
		if (trTimeMS() > xGetInt(sunlights, xSunlightNext)) {
			gainFavor(p, 1);
			xSetInt(sunlights, xSunlightNext, xGetInt(sunlights, xSunlightNext) + 500);
			pos = xGetVector(sunlights, xSunlightPos);
			amt = xGetFloat(sunlights, xSunlightDamage);
			dist = xGetFloat(sunlights, xSunlightRadius);
			
			
			for (x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
					healUnit(p, amt);
				}
			}
			
			amt = amt * xGetFloat(dPlayerData, xPlayerHealBoost);
			if (trQuestVarGet("p"+p+"searing") == 1) {
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(dEnemies, pos) < dist) {
						damageEnemy(p, amt);
					}
				}
			}
			
			if (trTimeMS() > xGetInt(sunlights, xSunlightTimeout)) {
				for(x=xGetInt(sunlights, xSunlightStart); < xGetInt(sunlights, xSunlightEnd)) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Rocket"));
					trUnitDestroy();
				}
				xFreeDatabaseBlock(sunlights);
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseSunbow(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * SUNBOW;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Sunlight";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Searing Light";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Healing Rays";
		lureIsUltimate = false;
	}
	xSetInt(dPlayerData,xPlayerWellCooldown, sunlightCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, healingRaysCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost,0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	xSearingSFX = xInitAddInt(db, "searingSFX");
	
	if (trQuestVarGet("p"+p+"sunlights") == 0) {
		db = xInitDatabase("p"+p+"sunlights");
		trQuestVarSet("p"+p+"sunlights", db);
		xSunlightRadius = xInitAddFloat(db, "radius");
		xSunlightDamage = xInitAddFloat(db, "damage");
		xSunlightTimeout = xInitAddInt(db, "timeout");
		xSunlightStart = xInitAddInt(db, "start");
		xSunlightEnd = xInitAddInt(db, "end");
		xSunlightPos = xInitAddVector(db, "pos");
		xSunlightNext = xInitAddInt(db, "next");
	}
}

void modifySunbow(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * SUNBOW;
	if (trQuestVarGet("p"+p+"healingRays") == 1) {
		xSetInt(dPlayerData, xPlayerNextDelay,
			xGetInt(dClass, xClassNextDelay, SUNBOW) * 2.0 / (1.0 + xGetInt(dPlayerData, xPlayerProjectiles)));
	}
}

rule sunbow_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SUNBOW + p, "sunbowAlways");
		trEventSetHandler(1000 + 12 * SUNBOW + p, "chooseSunbow");
		trEventSetHandler(5000 + 12 * SUNBOW + p, "modifySunbow");
	}
}
