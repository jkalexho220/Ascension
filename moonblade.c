const int moonbeamCooldown = 18;
const float moonbeamDamage = 30;
const float moonbeamDuration = 6;
const float moonbeamRadius = 6;

const int crescentCooldown = 12;
const float crescentCount = 3;
const float crescentDamage = 50;

const float protectionDelay = 83.33;

int xCrescentCount = 0;
int xCrescentTimeout = 0;
int xCrescentSFX = 0;
int xCrescentOn = 0;

/* moonbeams */
int xMoonbeamRadius = 0;
int xMoonbeamDamage = 0;
int xMoonbeamTimeout = 0;
int xMoonbeamStart = 0;
int xMoonbeamPos = 0;
int xMoonbeamNext = 0;
int xMoonbeamEnd = 0;

void removeMoonblade(int p = 0) {
	removePlayerSpecific(p);
}

void moonbladeAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * MOONBLADE;
	int db = trQuestVarGet("p"+p+"characters");
	int moonbeams = trQuestVarGet("p"+p+"moonbeams");
	xSetPointer(dPlayerData, p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	float dist = 0;
	float amt = 0;
	
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeMoonblade(p);
		} else {
			hit = CheckOnHit(p);
			if (hit > ON_HIT_ATTACKING) {
				if (xGetInt(db, xCrescentCount) > 0) {
					xSetInt(db, xCrescentCount, xGetInt(db, xCrescentCount) - 1);
					xSetInt(db, xCrescentTimeout, trTimeMS() + 5000);
					target = xGetPointer(dEnemies);
					if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
						stunUnit(dEnemies, 2.0, p);
						xUnitSelectByID(dEnemies, xUnitID);
						damageEnemy(p, crescentDamage * xGetFloat(dPlayerData,xPlayerSpellDamage), false);
						pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName));
						
						trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("Lightning Sparks Ground");
						gainFavor(p, 3.0);
					}
				}
				amt = xGetFloat(dPlayerData, xPlayerHealth) * xGetFloat(dPlayerData, xPlayerSpellDamage) * 0.03;
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					healUnit(p, amt);
				}
				if (hit == ON_HIT_SPECIAL) {
					if (trCurrentPlayer() == p) {
						trSoundPlayFN("mythcreate.wav","1",-1,"","");
					}
					pos = kbGetBlockPosition(""+xGetInt(db, xUnitName));
					xAddDatabaseBlock(dPlayerWolves, true);
					xSetInt(dPlayerData, xUnitName, trGetNextUnitScenarioNameNumber());
					spawnPlayerUnit(p, kbGetProtoUnitID("Wolf"), pos, calculateDecay(p, 5.0));
				}
			}
			if ((xGetInt(db, xCrescentCount) > 0) &&
				(trTimeMS() > xGetInt(db, xCrescentTimeout))) {
				xSetInt(db, xCrescentTimeout, trTimeMS() + 5000);
				xSetInt(db, xCrescentCount, xGetInt(db, xCrescentCount) - 1);
			}
			
			if (xGetBool(db, xCrescentOn) && (xGetInt(db, xCrescentCount) <= 0)) {
				xSetBool(db, xCrescentOn, false);
				xUnitSelect(db, xCrescentSFX);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("eclipsebirth.wav","1",-1,"","");
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		xAddDatabaseBlock(moonbeams, true);
		xSetFloat(moonbeams, xMoonbeamRadius, xsPow(moonbeamRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2));
		xSetFloat(moonbeams, xMoonbeamDamage, moonbeamDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));
		xSetInt(moonbeams,xMoonbeamTimeout, trTimeMS() + 1000 * moonbeamDuration * xGetFloat(dPlayerData,xPlayerSpellDuration));
		xSetInt(moonbeams, xMoonbeamStart, trGetNextUnitScenarioNameNumber());
		xSetVector(moonbeams, xMoonbeamPos, pos);
		xSetInt(moonbeams, xMoonbeamNext, trTimeMS());
		dir = xsVectorSet(moonbeamRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 0, 0);
		trChatSend(0, "pos: " + xGetVector(dPlayerData, xPlayerWellPos));
		trChatSend(0, "pos: " + pos);
		trChatSend(0, "dir: " + dir);
		for(x=0; <16) {
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(dir)+xsVectorGetX(pos),0,xsVectorGetZ(dir)+xsVectorGetZ(pos),0,true);
			trArmySelect(""+p+",0");
			trMutateSelected(kbGetProtoUnitID("Healing SFX"));
			dir = rotationMatrix(dir, 0.92388, 0.382683); // rotate 22.5 degrees
		}
		xSetInt(moonbeams, xMoonbeamEnd, trGetNextUnitScenarioNameNumber());
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"crescentStarted", 1);
		trSoundPlayFN("olympustemplesfx.wav","1",-1,"","");
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xSetInt(db, xCrescentCount, xGetInt(db, xCrescentCount) + 3);
			xSetInt(db, xCrescentTimeout, trTimeMS() + 5000);
			xSetBool(db, xCrescentOn, true);
			if ((xGetInt(db, xCrescentSFX) == 0) ||
				(kbGetBlockID(""+xGetInt(db, xCrescentSFX)) == -1)) {
				spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Rocket"), xsVectorSet(db,xCrescentSFX,xGetPointer(db)));
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"protection", 1 - trQuestVarGet("p"+p+"protection"));
		if (trQuestVarGet("p"+p+"protection") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"protection", 0);
			} else {
				trQuestVarSet("protectionCount", 1 + trQuestVarGet("protectionCount"));
				trQuestVarSet("p"+p+"protectionNext",
					trTimeMS() + protectionDelay / trQuestVarGet("p"+p+"ultimateCost"));
				trSoundPlayFN("bronzebirth.wav","1",-1,"","");
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					id = xGetInt(dPlayerUnits, xUnitID);
					trUnitSelectClear();
					trUnitSelectByID(id);
					xsSetContextPlayer(xGetInt(dPlayerUnits,xPlayerOwner));
					amt = kbUnitGetCurrentHitpoints(id);
					xsSetContextPlayer(0);
					xSetFloat(dPlayerUnits, xCurrentHealth, amt);
				}
			}
		} else {
			trQuestVarSet("protectionCount", trQuestVarGet("protectionCount") - 1);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}
	
	if (PvP && (trQuestVarGet("p"+p+"protection") == 1)) {
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			id = xGetInt(dPlayerUnits, xUnitID);
			trUnitSelectClear();
			trUnitSelectByID(id);
			trUnitHighlight(0.2, false);
			xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner));
			amt = kbUnitGetCurrentHitpoints(id);
			xsSetContextPlayer(0);
			if (amt > xGetFloat(dPlayerUnits, xCurrentHealth)) {
				xSetFloat(dPlayerUnits, xCurrentHealth, amt);
			} else {
				trDamageUnit(amt - xGetFloat(dPlayerUnits, xCurrentHealth));
			}
		}
	}
	
	if ((trQuestVarGet("p"+p+"crescentStarted") == 1) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelect(db, xCrescentSFX);
			trMutateSelected(kbGetProtoUnitID("Outpost"));
			trSetSelectedScale(0.0,0.0,0.0);
		}
		trQuestVarSet("p"+p+"crescentStarted", 0);
	}
	
	if (xGetDatabaseCount(moonbeams) > 0) {
		target = 0;
		xDatabaseNext(moonbeams);
		if (trTimeMS() > xGetInt(moonbeams, xMoonbeamNext)) {
			xSetInt(moonbeams, xMoonbeamNext, xGetInt(moonbeams, xMoonbeamNext) + 500);
			pos = xGetVector(moonbeams, xMoonbeamPos);
			dist = xGetFloat(moonbeams, xMoonbeamRadius);
			
			amt = xGetFloat(moonbeams, xMoonbeamDamage);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(dEnemies, pos) < dist) {
					silenceUnit(dEnemies,1.0,p);
					damageEnemy(p, amt);
				}
			}
			if (trTimeMS() > xGetInt(moonbeams, xMoonbeamTimeout)) {
				for(x=xGetInt(moonbeams, xMoonbeamStart); < xGetInt(moonbeams, xMoonbeamEnd)) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trUnitDestroy();
				}
				xFreeDatabaseBlock(moonbeams);
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"protection") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"protectionNext")) {
			trQuestVarSet("p"+p+"protectionNext",
				trQuestVarGet("p"+p+"protectionNext") + protectionDelay / trQuestVarGet("p"+p+"ultimateCost"));
			gainFavor(p, 0.0 - 1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"protection", 0);
				trQuestVarSet("protectionCount", trQuestVarGet("protectionCount") - 1);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
}

void chooseMoonblade(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * MOONBLADE;
	int db = trQuestVarGet("p"+p+"characters");
	xPrintAll(db);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData,p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Moonbeam";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Protection";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Crescent Strikes";
		lureIsUltimate = false;
	}
	xSetInt(dPlayerData,xPlayerWellCooldown, moonbeamCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, crescentCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost,0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	xCrescentCount = xInitAddInt(db, "crescentCount");
	xCrescentTimeout = xInitAddInt(db, "crescentTimeout");
	xCrescentSFX = xInitAddInt(db, "crescentSFX");
	xCrescentOn = xInitAddBool(db, "crescentOn");
	
	xPrintAll(db);
	
	if (trQuestVarGet("p"+p+"moonbeams") == 0) {
		db = xInitDatabase("p"+p+"moonbeams");
		trQuestVarSet("p"+p+"moonbeams", db);
		xMoonbeamRadius = xInitAddFloat(db, "radius");
		xMoonbeamDamage = xInitAddFloat(db, "damage");
		xMoonbeamTimeout = xInitAddInt(db, "timeout");
		xMoonbeamStart = xInitAddInt(db, "start");
		xMoonbeamEnd = xInitAddInt(db, "end");
		xMoonbeamPos = xInitAddVector(db, "pos");
		xMoonbeamNext = xInitAddInt(db, "next");
	}
}

void moonbladeModify(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * MOONBLADE;
	zSetProtoUnitStat("Wolf", p, 0, 0.2 * trQuestVarGet("p"+p+"health"));
	zSetProtoUnitStat("Wolf", p, 27, 0.2 * trQuestVarGet("p"+p+"baseAttack"));
}

rule moonblade_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * MOONBLADE + p, "moonbladeAlways");
		trEventSetHandler(1000 + 12 * MOONBLADE + p, "chooseMoonblade");
		trEventSetHandler(5000 + 12 * MOONBLADE + p, "moonbladeModify");
	}
}
