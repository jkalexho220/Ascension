
float empoweredDamage = 60;
float empoweredRadius = 4;

float starDuration = 10;
float starRadius = 4;
float starDamage = 10;

int starfallCooldown = 8;
float starfallRadius = 5;
float starfallDamage = 120;

float solarFlareDamage = 90;
float solarFlareRange = 40;
float solarFlareCost = 40;

float warpRange = 8;
int warpCooldown = 6;

int xStarfire = 0;


int xStarPos = 0;
int xStarTimeout = 0;
int xStarLast = 0;

int xStarProjPos = 0;
int xStarProjTimeout = 0;

int xStarfallTimeout = 0;
int xStarfallYeehaw = 0;
int xStarfallPos = 0;

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
	xSetInt(stars, xStarLast, trTimeMS());
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
	int stars = trQuestVarGet("p"+p+"stars");
	int starfalls = trQuestVarGet("p"+p+"starfalls");
	int starproj = trQuestVarGet("p"+p+"starproj");
	int solarflare = trQuestVarGet("p"+p+"solarFlare");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector dest = vector(0,0,0);
	vector dir = vector(0,0,0);
	
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
		amt = 0.001 * (trTimeMS() - xGetInt(stars, xStarLast));
		
		if (amt >= 1) {
			amt = amt * starDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
			xSetInt(stars, xStarLast, trTimeMS());
			pos = xGetVector(stars, xStarPos);
			
			dist = starRadius * xGetFloat(dPlayerData, xPlayerSpellRange);
			dist = xsPow(dist, 2);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					xUnitSelectByID(dEnemies, xUnitID);
					damageEnemy(p, amt);
				}
			}
		}
		
		if (trTimeMS() > xGetInt(stars, xStarTimeout)) {
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
			
			gainFavor(p, 3);
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
					xSetFloat(dEnemies, xMagicResist, xGetFloat(dEnemies, xMagicResist) - 0.1);
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
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		blastmageSpell(p);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		trSoundPlayFN("sphinxteleportout.wav","1",-1,"","");
		dest = xGetVector(dPlayerData, xPlayerLurePos);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeBlastmage(p);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				spawnStar(p, pos);
				target = 1 + xsMin(warpRange * xGetFloat(dPlayerData, xPlayerSpellRange),
					distanceBetweenVectors(pos, dest, false)) / 2;
				dir = getUnitVector(pos, dest);
				for(i=target; >0) {
					pos = pos + (dir * 2.0);
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					}
				}
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				xUnitSelectByID(db, xUnitID);
				trImmediateUnitGarrison(""+next);
				trMutateSelected(kbGetProtoUnitID("Siege Tower"));
				trUnitChangeProtoUnit("Priest");
				xUnitSelectByID(db, xUnitID);
				trSetUnitOrientation(dir, vector(0,1,0), true);
				trMutateSelected(kbGetProtoUnitID("Priest"));
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trUnitChangeProtoUnit("Lightning Sparks");
			}
		}
		/* reload relics */
		equipRelicsAgain(p);
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
	
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
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
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Solar Flare";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Warp";
		lureIsUltimate = false;
	}
	xStarfire = xInitAddInt(db, "starfire");
	zSetProtoUnitStat("Priest Projectile", p, 8, 0.0001);
	
	xSetInt(dPlayerData,xPlayerWellCooldown, starfallCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, warpCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, solarFlareCost);
	
	if (trQuestVarGet("p"+p+"stars") == 0) {
		db = xInitDatabase("p"+p+"stars");
		trQuestVarSet("p"+p+"stars", db);
		xInitAddInt(db, "name");
		xStarPos = xInitAddVector(db, "pos");
		xStarTimeout = xInitAddInt(db, "timeout");
		xStarLast = xInitAddInt(db, "last");
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
