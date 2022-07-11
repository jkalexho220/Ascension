const float rainOfLightningCost = 50;
const float rainOfLightningDuration = 10;
const float rainOfLightningRadius = 5;

const int disengageCooldown = 10;
const float disengageRadius = 4;
const float disengageRange = 12;

const int shockArrowCooldown = 8;
const float shockArrowRange = 30;
const float shockArrowDamage = 100;

int xRainStormSFX = 0;

int xSpearedIndex = 0;
int xSpearedPrev = 0;
int xSpearedDir = 0;
int xSpearedDist = 0;


int xShockArrowPrev = 0;
int xShockArrowDest = 0;
int xShockArrowDir = 0;
int xShockArrowTimeout = 0;
int xShockArrowDist = 0;

void removeStormpiercer(int p = 0) {
	removePlayerSpecific(p);
}

void stormpiercerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * STORMPIERCER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int relics = getRelicsDB(p);
	int spearedUnits = trQuestVarGet("p"+p+"spearedUnits");
	int shockArrows = trQuestVarGet("p"+p+"shockArrows");
	float amt = 0;
	float dist = 0;
	float current = 0;
	vector pos = vector(0,0,0);
	vector start = vector(0,0,0);
	vector end = vector(0,0,0);
	vector dir = vector(0,0,0);
	xSetPointer(dPlayerData, p);
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeStormpiercer(p);
		} else {
			hit = CheckOnHit(p);
			if (hit == ON_HIT_NORMAL) {
				target = xGetInt(db, xCharAttackTarget);
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				end = kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target));
				dist = distanceBetweenVectors(pos, end, false);
				trQuestVarSet("p"+p+"stormbonus", dist * 0.04);
				xSetFloat(dPlayerData, xPlayerAttack,
					xGetFloat(dPlayerData, xPlayerBaseAttack) * (1.0 + trQuestVarGet("p"+p+"stormbonus")));
				zSetProtoUnitStat("Archer Atlantean Hero", p, 31, xGetFloat(dPlayerData, xPlayerAttack));
				if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
					trChatSetStatus(false);
					trDelayedRuleActivation("enable_chat");
					trUnitSelectClear();
					trUnitSelectByID(target);
					trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
					dist = xsPow(rainOfLightningRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
					amt = xGetFloat(dPlayerData, xPlayerAttack) * xGetFloat(dPlayerData, xPlayerSpellDamage);
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (unitDistanceToVector(xGetInt(dEnemies,xUnitName), end) < dist) {
							trUnitHighlight(0.2, false);
							damageEnemy(p, amt, false, 1.0);
						}
					}
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		end = xGetVector(dPlayerData, xPlayerWellPos);
		dist = disengageRadius * xGetFloat(dPlayerData, xPlayerSpellRange);
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else if (trCountUnitsInArea(""+xGetInt(dEnemies, xUnitName),p,"Archer Atlantean Hero", dist) >0) {
				stunUnit(dEnemies, 2.0, p);
				gainFavor(p, 1.0);
			}
		}
		trSoundPlayFN("sphinxteleportout.wav","1",-1,"","");
		for(x=xGetDatabaseCount(relics); >0) {
			xDatabaseNext(relics);
			xUnitSelect(relics, xUnitName);
			trUnitChangeProtoUnit("Relic");
		}
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStormpiercer(p);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Dust Large");
				target = 1 + xsMin(disengageRange * xGetFloat(dPlayerData, xPlayerSpellRange),
					unitDistanceToVector(xGetInt(db, xUnitName), end)) / 2;
				dir = getUnitVector(pos, end);
				for(i=target; >0) {
					pos = xsVectorSet(xsVectorGetX(pos) + 2.0 * xsVectorGetX(dir), 0, xsVectorGetZ(pos) + 2.0 * xsVectorGetZ(dir));
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					}
				}
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				xUnitSelectByID(db, xUnitID);
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trMutateSelected(kbGetProtoUnitID("Siege Tower"));
				trUnitChangeProtoUnit("Archer Atlantean Hero");
				xUnitSelectByID(db, xUnitID);
				trSetUnitOrientation(dir, vector(0,1,0), true);
				trMutateSelected(kbGetProtoUnitID("Archer Atlantean Hero"));
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Lightning Sparks");
			}
		}
		/* reload relics */
		equipRelicsAgain(p);
	}
	
	if (xGetDatabaseCount(spearedUnits) > 0) {
		xDatabaseNext(spearedUnits);
		bool launched = false;
		if (PvP) {
			hit = xGetInt(dEnemies, xDoppelganger, xGetInt(spearedUnits, xSpearedIndex));
			launched = xGetBool(dPlayerUnits, xLaunched, hit);
		} else {
			xUnitSelect(spearedUnits, xUnitName);
			launched = trUnitAlive() && xGetBool(dEnemies, xLaunched, xGetInt(spearedUnits, xSpearedIndex));
			if ((xGetInt(dEnemies, xUnitName, xGetInt(spearedUnits, xSpearedIndex)) == bossUnit) && trQuestVarGet("stage") > 5) {
				launched = false;
			}
		}
		if (launched == false) {
			if (xGetInt(dEnemies, xStunStatus, xGetInt(spearedUnits, xSpearedIndex)) > 0) { // if they hit a wall, damage them
				if (xSetPointer(dEnemies, xGetInt(spearedUnits, xSpearedIndex))) {
					xUnitSelectByID(dEnemies, xUnitID);
					damageEnemy(p,
						shockArrowDamage * xGetFloat(dPlayerData, xPlayerSpellDamage) * (1.0 + 0.04 * xGetFloat(spearedUnits, xSpearedDist)));
				}
			}
			xFreeDatabaseBlock(spearedUnits);
		} else {
			pos = xGetVector(spearedUnits, xSpearedPrev);
			end = kbGetBlockPosition(""+xGetInt(spearedUnits, xUnitName));
			
			dist = distanceBetweenVectors(pos, end, false);
			if (dist > 2.0) {
				xSetFloat(spearedUnits, xSpearedDist, xGetFloat(spearedUnits, xSpearedDist) + dist);
				dir = xGetVector(spearedUnits, xSpearedDir);
				dir = dir + dir;
				for(x=dist / 2; >0) {
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning Sparks");
					pos = pos + dir;
				}
				xSetVector(spearedUnits, xSpearedPrev, end);
			}
		}
	}
	
	if (xGetDatabaseCount(shockArrows) > 0) {
		xDatabaseNext(shockArrows);
		start = xGetVector(shockArrows, xShockArrowPrev);
		pos = kbGetBlockPosition(""+xGetInt(shockArrows, xUnitName), true);
		dist = distanceBetweenVectors(start, pos, false);
		dir = xGetVector(shockArrows, xShockArrowDir);
		end = xGetVector(shockArrows, xShockArrowDest);
		hit = 0;
		if (dist > 2.0) {
			xSetFloat(shockArrows, xShockArrowDist, xGetFloat(shockArrows, xShockArrowDist) + dist);
			if (xGetInt(shockArrows, xShockArrowTimeout) == -1) { // start the timer when the arrow starts moving
				xSetInt(shockArrows, xShockArrowTimeout, trTimeMS() + 1000 * xGetFloat(dPlayerData, xPlayerSpellRange));
			}
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision(dEnemies, start, dir, dist + 1.0, 2.0)) {
					hit = 1;
					gainFavor(p, 5.0);
					trQuestVarSetFromRand("sound", 1, 2, true);
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					}
					damageEnemy(p,
						shockArrowDamage * xGetFloat(dPlayerData, xPlayerSpellDamage) * (1.0 + 0.04 * xGetFloat(shockArrows, xShockArrowDist)));
					if (trUnitAlive() && (xGetBool(dEnemies, xLaunched) == false)) {
						xAddDatabaseBlock(spearedUnits, true);
						xSetInt(spearedUnits, xUnitName, xGetInt(dEnemies, xUnitName));
						xSetInt(spearedUnits, xSpearedIndex, xGetPointer(dEnemies));
						xSetVector(spearedUnits, xSpearedPrev, start);
						xSetVector(spearedUnits, xSpearedDir, dir);
						xSetFloat(spearedUnits, xSpearedDist, xGetFloat(shockArrows, xShockArrowDist));
						launchUnit(dEnemies, end);
					}
					
					break;
				}
			}
			dir = dir + dir;
			for (x=dist/2; >0) {
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Lightning Sparks");
				start = start + dir;
			}
			xSetVector(shockArrows, xShockArrowPrev, pos);
			if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hit = 2;
			}
		} else if ((trTimeMS() > xGetInt(shockArrows, xShockArrowTimeout)) && (xGetInt(shockArrows, xShockArrowTimeout) > 0)) {
			hit = 1;
		} else if (distanceBetweenVectors(pos, end) < 1.0) {
			hit = 1;
		}
		
		if (hit >= 1) {
			xUnitSelect(shockArrows, xUnitName);
			trUnitChangeProtoUnit("Dust Small");
			xFreeDatabaseBlock(shockArrows);
			if (hit == 2) {
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trSoundPlayFN("ui\lightning4.wav","1",-1,"","");
		end = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject");
		trUnitDestroy();
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStormpiercer(p);
			} else {
				start = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dir = getUnitVector(start, end);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Ballista Shot"));
				trSetUnitOrientation(dir,vector(0,1,0),true);
				pos = vectorSetAsTargetVector(start,end,30.0 * xGetFloat(dPlayerData, xPlayerSpellRange));
				trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos));
				xAddDatabaseBlock(shockArrows, true);
				xSetInt(shockArrows, xUnitName, 1*trQuestVarGet("next"));
				xSetVector(shockArrows, xShockArrowPrev, start);
				xSetVector(shockArrows, xShockArrowDest, pos);
				xSetVector(shockArrows, xShockArrowDir, dir);
			}
		}
	}
	
	if ((trQuestVarGet("p"+p+"rainOfLightning") == 2) && (trQuestVarGet("spyfind") == trQuestVarGet("spyfound"))) {
		trQuestVarSet("p"+p+"rainOfLightning", 1);
		for(x=xGetDatabaseCount(db); >0) {
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStormpiercer(p);
			} else {
				xUnitSelect(db, xRainStormSFX);
				trMutateSelected(kbGetProtoUnitID("Arkantos God"));
				trSetSelectedScale(0,0,0);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		gainFavor(p, 0.0 - rainOfLightningCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trSoundPlayFN("lightningbirth.wav","1",-1,"","");
		trQuestVarSet("p"+p+"rainOfLightning", 2);
		trQuestVarSet("p"+p+"rainOfLightningTimeout",
			trTimeMS() + 1000 * rainOfLightningDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		xSetInt(dPlayerData, xPlayerNextDelay,
			xGetInt(dClass, xClassNextDelay, STORMPIERCER) * 2.0 / (1.0 + xGetInt(dPlayerData, xPlayerProjectiles)));
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
		trQuestVarSet("p"+p+"arrowNext", trGetNextUnitScenarioNameNumber() - 1);
		for(x=xGetDatabaseCount(db); >0) {
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStormpiercer(p);
			} else if (kbGetBlockID(""+xGetInt(db, xRainStormSFX)) == -1) {
				spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db, xRainStormSFX, xGetPointer(db)));
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"rainOfLightningTimeout")) {
			trQuestVarSet("p"+p+"rainOfLightning", 0);
			xSetInt(dPlayerData, xPlayerNextDelay, xGetInt(dClass, xClassNextDelay, STORMPIERCER));
			zSetProtoUnitStat("Archer Atlantean Hero", p, 13, xGetInt(dPlayerData, xPlayerProjectiles));
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeStormpiercer(p);
				} else {
					xUnitSelect(db, xRainStormSFX);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}
		id = yFindLatest("p"+p+"arrowNext", "Arrow Flaming", p);
		if (id > 0) {
			trUnitDestroy();
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseStormpiercer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * STORMPIERCER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Disengage";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Rain of Lightning";
		rainIsUltimate = true;
		map("q", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(Q) Stormbolt";
		lureIsUltimate = false;
	}
	xRainStormSFX = xInitAddInt(db, "rainSFX");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, disengageCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, shockArrowCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost,0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, rainOfLightningCost);
	
	if (trQuestVarGet("p"+p+"spearedUnits") == 0) {
		db = xInitDatabase("p"+p+"spearedUnits");
		trQuestVarSet("p"+p+"spearedUnits", db);
		xInitAddInt(db, "name");
		xSpearedIndex = xInitAddInt(db, "index");
		xSpearedPrev = xInitAddVector(db, "prev");
		xSpearedDir = xInitAddVector(db, "dir");
		xSpearedDist = xInitAddFloat(db, "dist");
	}
	
	if (trQuestVarGet("p"+p+"shockArrows") == 0) {
		db = xInitDatabase("p"+p+"shockArrows");
		trQuestVarSet("p"+p+"shockArrows", db);
		xInitAddInt(db, "name");
		xShockArrowPrev = xInitAddVector(db, "prev");
		xShockArrowDest = xInitAddVector(db, "dest");
		xShockArrowDir = xInitAddVector(db, "dir");
		xShockArrowTimeout = xInitAddInt(db, "timeout", -1);
		xShockArrowDist = xInitAddFloat(db, "dist");
	}
}

void modifyStormpiercer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * STORMPIERCER;
	if (trQuestVarGet("p"+p+"rainOfLightning") == 1) {
		zSetProtoUnitStat("Archer Atlantean Hero", p, 13, 1);
		xSetInt(dPlayerData, xPlayerNextDelay,
			xGetInt(dClass, xClassNextDelay, STORMPIERCER) * 2.0 / (1.0 + xGetInt(dPlayerData, xPlayerProjectiles)));
	}
}

rule stormpiercer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STORMPIERCER + p, "stormpiercerAlways");
		trEventSetHandler(1000 + 12 * STORMPIERCER + p, "chooseStormpiercer");
		trEventSetHandler(5000 + 12 * STORMPIERCER + p, "modifyStormpiercer");
	}
}
