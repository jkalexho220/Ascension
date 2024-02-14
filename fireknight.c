
const int flamingImpactCooldown = 12;
const float flamingImpactDamage = 90;
const float flamingImpactRange = 4;

const int overheatCooldown = 18;
const float overheatDuration = 6;
const float overheatDamage = 40;
const float overheatRadius = 4;

const float infernoDuration = 8;
const float infernoRange = 12;
const float infernoDamage = 120;
const float infernoHeal = 60;
const float infernoCost = 70;

int xFireKnightPhoenix = 0;
int xFireKnightCharging = 0;
int xFireKnightOverheatNext = 0;

int xFireChargeTimeout = 0;
int xFireChargeDir = 0;
int xFireChargeSFX = 0;
int xFireChargeDest = 0;

int xInfernoPos = 0;
int xInfernoDamage = 0;
int xInfernoRadius = 0;
int xInfernoMaxRadius = 0;
int xInfernoNext = 0;
int xInfernoTimeout = 0;

void removeFireKnight(int p = 0) {
	removePlayerSpecific(p);
}

void fireknightAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * FIREKNIGHT;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int count = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int infernos = trQuestVarGet("p"+p+"inferno");
	int charges = trQuestVarGet("p"+p+"fireCharges");
	int harpies = trQuestVarGet("p"+p+"fireHarpies");
	int next = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	float mCos = 0;
	float mSin = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector dest = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	xUnitSelect(dPlayerData, xPlayerUnit);
	if (trUnitAlive()) {
		amt = 0.01 * trUnitPercentDamaged();
	}
	
	xSetFloat(dPlayerData, xPlayerLifesteal,
		xGetFloat(dPlayerData, xPlayerLifesteal) + amt - trQuestVarGet("p"+p+"fireknightBonus"));
	trQuestVarSet("p"+p+"fireknightBonus", amt);
	
	if (xGetDatabaseCount(harpies) > 0) {
		for (x=xGetDatabaseCount(harpies); >0) {
			xDatabaseNext(harpies);
			xUnitSelect(harpies, xUnitName);
			trMutateSelected(kbGetProtoUnitID("Harpy"));
			trUnitOverrideAnimation(1,0,false,false,-1);
		}
		xClearDatabase(harpies);
	}
	
	if (xGetDatabaseCount(charges) > 0) {
		xDatabaseNext(charges);
		pos = kbGetBlockPosition(""+xGetInt(charges, xUnitName), true);
		dest = xGetVector(charges, xFireChargeDest);
		if (distanceBetweenVectors(pos, dest) < 4 || trTimeMS() > xGetInt(charges, xFireChargeTimeout)) {
			if (xSetPointer(db, xGetInt(charges, xDatabaseIndex))) {
				xSetBool(db, xFireKnightCharging, false);
				xSetBool(dPlayerUnits, xLaunched, false, xGetInt(db, xCharIndex));
				xUnitSelectByID(db, xUnitID);
				trUnitChangeProtoUnit("Lancer Hero");
				xUnitSelectByID(db, xUnitID);
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				
				if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
					equipRelicsAgain(p);
				}
				
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				xUnitSelect(charges, xFireChargeSFX);
				trUnitChangeProtoUnit("Kronny Flying");
				xUnitSelect(charges, xFireChargeSFX);
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trSetSelectedScale(0,-5.0,0);
				trDamageUnitPercent(100);
				next = xGetInt(charges, xFireChargeSFX);
				xAddDatabaseBlock(harpies, true);
				xSetInt(harpies, xUnitName, next);
				
				xUnitSelect(charges, xUnitName);
				trDamageUnitPercent(100);
				trUnitChangeProtoUnit("Meteorite");
				xFreeDatabaseBlock(charges);
				amt = flamingImpactDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
				dist = xsPow(flamingImpactRange * xGetFloat(dPlayerData, xPlayerSpellRange) * 2, 2);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						damageEnemy(p, amt, true);
						gainFavor(p, 2);
					}
				}
				trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
			}
		} else {
			dir = xGetVector(charges, xFireChargeDir);
			dest = dest + dir * 4.0;
			dir = rotationMatrix(dir, 0, 1.0);
			dist = xsPow(flamingImpactRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist &&
					xGetBool(dEnemies, xLaunched) == false) {
					launchUnit(dEnemies, dir * dotProduct(kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true) - pos, dir) + dest);
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("nidhoggflame1.wav","1",-1,"","");
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeFireKnight();
			} else if (xGetBool(db, xFireKnightCharging) == false) {
				xSetBool(db, xFireKnightCharging, true);
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerWellPos));
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trImmediateUnitGarrison(""+xGetInt(db, xUnitName));
				trUnitChangeProtoUnit("Dwarf");
				
				trModifyProtounit("Hero Greek Achilles",p,5,2);
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
				trSetUnitOrientation(dir, vector(0,1,0), true);
				
				xUnitSelectByID(db, xUnitID);
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				trUnitOverrideAnimation(15,0,true,false,-1);
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+next);
				trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
				
				target = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelect(""+target, true);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				trUnitOverrideAnimation(6,0,true,false,-1);
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+next);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				
				xSetBool(dPlayerUnits, xLaunched, true, xGetInt(db, xCharIndex));
				
				// Charge duration is 2, and speed is 15. Therefore it is 30 x spell duration for maximum range
				dist = xsMin(distanceBetweenVectors(pos, xGetVector(dPlayerData, xPlayerWellPos), false), 30.0 * xGetFloat(dPlayerData, xPlayerSpellDuration));
				for(y=0; < dist / 2) {
					dest = pos + (dir * 2.0);
					if (terrainIsType(vectorToGrid(dest), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					} else {
						pos = dest;
					}
				}
				
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
				trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,false);
				trModifyProtounit("Hero Greek Achilles",p,5,-2);
				
				xAddDatabaseBlock(charges, true);
				xSetInt(charges, xUnitName, next);
				xSetInt(charges, xDatabaseIndex, xGetPointer(db));
				xSetInt(charges, xFireChargeSFX, target);
				xSetInt(charges, xFireChargeTimeout, trTimeMS() + 1000 + 2000 * xGetFloat(dPlayerData, xPlayerSpellDuration));
				xSetVector(charges, xFireChargeDir, dir);
				xSetVector(charges, xFireChargeDest, pos);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trSoundPlayFN("firegiantdie.wav","1",-1,"","");
		trQuestVarSet("p"+p+"overheat", 1);
		trQuestVarSet("p"+p+"overheatTimeout",
			trTimeMS() + 1000 * overheatDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeFireKnight();
			} else {
				if (xGetInt(db, xFireKnightPhoenix) <= 0) {
					spyEffect(xGetInt(db, xUnitName),
						kbGetProtoUnitID("Phoenix"), xsVectorSet(db, xFireKnightPhoenix, xGetPointer(db)));
				} else {
					xUnitSelect(db, xFireKnightPhoenix);
					trMutateSelected(kbGetProtoUnitID("Phoenix"));
				}
				xSetInt(db, xFireKnightOverheatNext, trTimeMS());
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		gainFavor(p, 0.0 - infernoCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trSoundPlayFN("forestfirebirth.wav","1",-1,"","");
		trSoundPlayFN("flamingweapons.wav","1",-1,"","");
		zSetProtoUnitStat("Ball of Fire Impact", p, 8, xGetFloat(dPlayerData, xPlayerSpellDuration) * infernoDuration);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (id == -1 || trUnitAlive() == false) {
				removeFireKnight();
			} else {
				xAddDatabaseBlock(infernos, true);
				xSetVector(infernos, xInfernoPos, kbGetBlockPosition(""+xGetInt(db, xUnitName), true));
				xSetFloat(infernos, xInfernoDamage, infernoDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));
				xSetFloat(infernos, xInfernoMaxRadius, infernoRange * xGetFloat(dPlayerData, xPlayerSpellRange));
				xSetInt(infernos, xInfernoNext, trTimeMS());
				xSetInt(infernos, xInfernoTimeout,
					trTimeMS() + 1000 * infernoDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"overheat") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"overheatTimeout")) {
			trQuestVarSet("p"+p+"overheat", 0);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeFireKnight();
				} else {
					xUnitSelect(db, xFireKnightPhoenix);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}
	}
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeFireKnight(p);
		} else {
			amt = 0;
			pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			hit = CheckOnHit(p);
			if (hit == ON_HIT_SPECIAL) {
				count = 1;
				if (playerHasSymphony(p, SYMPHONY_DOUBLE_SPECIAL)) {
					count = 2;
				}
				for(i=count; >0) {
					xUnitSelectByID(db, xUnitID);
					trDamageUnit(50.0 * xGetFloat(dPlayerData, xPlayerSpellDamage));
					
					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trDamageUnitPercent(100);
					trUnitChangeProtoUnit("Meteorite");
					amt = amt + 50.0 * xGetFloat(dPlayerData, xPlayerSpellDamage);
				}

				trSoundPlayFN("fireball fall 2.wav","1",-1,"","");
			}
			xsSetContextPlayer(p);
			current = kbUnitGetCurrentHitpoints(id);
			xsSetContextPlayer(0);
			if (trQuestVarGet("p"+p+"overheat") == 1) {
				if (trTimeMS() > xGetInt(db, xFireKnightOverheatNext)) {
					xSetInt(db, xFireKnightOverheatNext, xGetInt(db, xFireKnightOverheatNext) + 500);
					xUnitSelectByID(db, xUnitID);
					trDamageUnit(overheatDamage * xGetFloat(dPlayerData, xPlayerSpellDamage) * 0.5);
					xsSetContextPlayer(p);
					current = kbUnitGetCurrentHitpoints(id);
					xsSetContextPlayer(0);
				}
				amt = amt + xsMax(0, xGetFloat(dPlayerUnits, xCurrentHealth, xGetInt(db, xCharIndex)) - current);
			}
			xSetFloat(dPlayerUnits, xCurrentHealth, current, xGetInt(db, xCharIndex));
			if (amt > 0) {
				dist = xsPow(overheatRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						damageEnemy(p, amt * xGetFloat(dPlayerData, xPlayerSpellDamage), true);
					}
				}
			}
		}
	}
	
	if (xGetDatabaseCount(infernos) >0) {
		xDatabaseNext(infernos);
		if (trTimeMS() > xGetInt(infernos, xInfernoNext)) {
			xSetInt(infernos, xInfernoNext, xGetInt(infernos, xInfernoNext) + 250);
			pos = xGetVector(infernos, xInfernoPos);
			if (xGetFloat(infernos, xInfernoRadius) < xGetFloat(infernos, xInfernoMaxRadius)) {
				xSetFloat(infernos, xInfernoRadius, 2.0 + xGetFloat(infernos, xInfernoRadius));
				hit = 1.5 * xGetFloat(infernos, xInfernoRadius);
				amt = 6.283185 / hit;
				mCos = xsCos(amt);
				mSin = xsSin(amt);
				
				trQuestVarSetFromRand("angle", 0, 3.141592, false);
				current = trQuestVarGet("angle");
				dir = xsVectorSet(xsCos(current),0,xsSin(current)) * xGetFloat(infernos, xInfernoRadius);
				for(x=hit; >0) {
					dest = pos + dir;
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(dest),0,xsVectorGetZ(dest),0,true);
					trArmySelect(""+p+",0");
					trUnitChangeProtoUnit("Ball of Fire Impact");
					dir = rotationMatrix(dir, mCos, mSin);
				}
			}
			amt = xGetFloat(infernos, xInfernoDamage) * 0.25;
			dist = xsPow(xGetFloat(infernos, xInfernoRadius), 2);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					damageEnemy(p, amt);
				}
			}
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
					healUnit(p, amt * 0.5);
				}
			}
		}
		if (trTimeMS() > xGetInt(infernos, xInfernoTimeout)) {
			xFreeDatabaseBlock(infernos);
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseFireKnight(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * FIREKNIGHT;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
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
	xFireKnightPhoenix = xInitAddInt(db, "phoenix");
	xFireKnightCharging = xInitAddBool(db, "charging");
	xFireKnightOverheatNext = xInitAddInt(db, "overheatNext");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, flamingImpactCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, infernoCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,overheatCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"fireharpies") == 0) {
		db = xInitDatabase("p"+p+"fireharpies");
		trQuestVarSet("p"+p+"fireharpies", db);
		xInitAddInt(db, "name");
	}
	
	if (trQuestVarGet("p"+p+"fireCharges") == 0) {
		db = xInitDatabase("p"+p+"fireCharges");
		trQuestVarSet("p"+p+"fireCharges", db);
		xInitAddInt(db, "name");
		xInitAddInt(db, "index");
		xFireChargeTimeout = xInitAddInt(db, "timeout");
		xFireChargeSFX = xInitAddInt(db, "sfx");
		xFireChargeDir = xInitAddVector(db, "dir");
		xFireChargeDest = xInitAddVector(db, "dest");
	}
	
	if (trQuestVarGet("p"+p+"inferno") == 0) {
		db = xInitDatabase("p"+p+"inferno");
		trQuestVarSet("p"+p+"inferno", db);
		xInfernoPos = xInitAddVector(db, "pos");
		xInfernoDamage = xInitAddFloat(db, "damage");
		xInfernoRadius = xInitAddFloat(db, "radius");
		xInfernoMaxRadius = xInitAddFloat(db, "maxRadius");
		xInfernoNext = xInitAddInt(db, "next");
		xInfernoTimeout = xInitAddInt(db, "timeout");
	}
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
}
