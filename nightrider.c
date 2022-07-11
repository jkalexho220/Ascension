const float abductRange = 12;
const int abductCooldown = 15;

const float deathSentenceDamage = 6;
const float deathSentenceDuration = 6;
const float deathSentenceRadius = 8;

const int ariseCooldown = 16;

const float nightfallCost = 80;
const float nightfallRadius = 15;
const float nightfallDuration = 12;
const float nightfallDecay = 12;

int xDeathSentenceIndex = 0;
int xDeathSentenceTimeout = 0;
int xDeathSentenceHealth = 0;
int xDeathSentenceSFX = 0;
int xDeathSentenceProto = 0;

int xAbductDest = 0;
int xAbductTimeout = 0;
int xAbductCurse = 0;

int xNightfallSFX = 0;

void removeNightrider(int p = 0) {
	xUnitSelect(getCharactersDB(p), xNightfallSFX);
	trUnitDestroy();
	removePlayerSpecific(p);
}

void spawnMinion(int p = 0, vector pos = vector(0,0,0), float heading = 0) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch(""+p+",0","Minion",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),heading,true);
	activatePlayerUnit(next, p, kbGetProtoUnitID("Minion"), calculateDecay(p, 5.0));
}

/*
assume pointer is already on the death sentence target
also make sure target does not have death sentence on them already
*/
void castDeathSentence(int p = 0) {
	int sentences = trQuestVarGet("p"+p+"sentences");
	trSoundPlayFN("shadeofhadesbirth.wav","1",-1,"","");
	
	xSetBool(dEnemies, xDeathSentence, true);
	
	poisonUnit(dEnemies, deathSentenceDuration, deathSentenceDamage, p);
	silenceUnit(dEnemies,deathSentenceDuration, p);
	trQuestVarSet("poisonSound", 0);
	xAddDatabaseBlock(sentences, true);
	xSetInt(sentences, xUnitName, xGetInt(dEnemies, xUnitName));
	xSetInt(sentences, xDeathSentenceIndex, xGetPointer(dEnemies));
	xSetInt(sentences, xDeathSentenceProto, xGetInt(dEnemies, xUnitProto));
	xSetInt(sentences, xUnitID, xGetInt(dEnemies, xUnitID));
	xSetInt(sentences, xDeathSentenceTimeout,
		trTimeMS() + 1000 * deathSentenceDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	
	int id = xGetInt(dEnemies, xUnitID);
	if (PvP) {
		xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner, xGetInt(dEnemies, xDoppelganger)));
	} else {
		xsSetContextPlayer(ENEMY_PLAYER);
	}
	float health = kbUnitGetCurrentHitpoints(id);
	xsSetContextPlayer(0);
	xSetFloat(sentences, xDeathSentenceHealth, health);

	if ((xGetInt(dEnemies, xUnitName) == bossUnit) && (trQuestVarGet("stage") >= 9)) {
		xSetInt(sentences, xDeathSentenceSFX, aiPlanGetUserVariableInt(ARRAYS,bossInts,2));
		xUnitSelect(sentences, xDeathSentenceSFX);
		trMutateSelected(kbGetProtoUnitID("Shade"));
	} else {
		spyEffect(xGetInt(dEnemies, xUnitName),kbGetProtoUnitID("Shade"),
			xsVectorSet(sentences, xDeathSentenceSFX, xGetNewestPointer(sentences)));
	}
}

void nightriderAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * NIGHTRIDER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int sentences = trQuestVarGet("p"+p+"sentences");
	int abducts = trQuestVarGet("p"+p+"abducts");
	int clouds = trQuestVarGet("p"+p+"nightfallClouds");
	float amt = 0;
	float dist = 0;
	float current = 0;
	bool special = false;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dest = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeNightrider(p);
		} else {
			hit = CheckOnHit(p);
			if (hit >= ON_HIT_NORMAL) {
				if (xGetBool(dEnemies, xDeathSentence, xGetInt(db, xCharAttackTargetIndex))) {
					if (xGetInt(dPlayerData, xPlayerRainCooldownStatus) == ABILITY_COOLDOWN) {
						xSetInt(dPlayerData, xPlayerRainReadyTime, xGetInt(dPlayerData, xPlayerRainReadyTime) - 1000);
						amt = xGetInt(dPlayerData, xPlayerRainReadyTime) - trTimeMS();
						if (amt > 0) {
							if (trCurrentPlayer() == p) {
								trCounterAbort("rain");
								trCounterAddTime("rain", amt * 0.001, 0, rainName);
							}
						}
					}
				}
				if (hit == ON_HIT_SPECIAL) {
					if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
						if (xGetBool(dEnemies, xDeathSentence) == false) {
							castDeathSentence(p);
						} else { // summon a minion instead
							pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
							trQuestVarSetFromRand("heading", 1, 360, true);
							spawnMinion(p, pos, trQuestVarGet("heading"));
						}
					}
				}
			}
		}
	}
	
	if (xGetDatabaseCount(sentences) > 0) {
		xDatabaseNext(sentences);
		xUnitSelectByID(sentences, xUnitID);
		if (trUnitAlive() == false) {
			pos = kbGetBlockPosition(""+xGetInt(sentences, xUnitName));
			
			if (trQuestVarGet("p"+p+"nightfallActive") == 1) {
				spawnPlayerUnit(p, xGetInt(sentences, xDeathSentenceProto), pos, calculateDecay(p, nightfallDecay));
			} else {
				trQuestVarSetFromRand("heading", 1, 360, true);
				spawnMinion(p, pos, trQuestVarGet("heading"));
			}

			hit = 0;
			dist = xsPow(deathSentenceRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			xUnitSelect(sentences, xDeathSentenceSFX);
			if ((xGetInt(sentences, xUnitName) == bossUnit) && (trQuestVarGet("stage") >= 9)) {
				trSetSelectedScale(1,1,1);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			} else {
				trUnitDestroy();
			}
			xFreeDatabaseBlock(sentences);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (xGetBool(dEnemies, xDeathSentence) == false) {
					if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						castDeathSentence(p);
						break;
					}
				}
			}
		} else {
			hit = xGetPointer(dEnemies);
			if (xSetPointer(dEnemies, xGetInt(sentences, xDeathSentenceIndex))) {
				if (trTimeMS() > xGetInt(sentences, xDeathSentenceTimeout)) {
					xUnitSelect(sentences, xDeathSentenceSFX);
					trUnitDestroy();
					xSetBool(dEnemies, xDeathSentence, false);
					xFreeDatabaseBlock(sentences);
				} else {
					id = xGetInt(dEnemies, xUnitID);
					if (PvP) {
						xsSetContextPlayer(kbUnitGetOwner(id));
					} else {
						xsSetContextPlayer(ENEMY_PLAYER);
					}
					amt = kbUnitGetCurrentHitpoints(id);
					xsSetContextPlayer(0);
					dist = xGetFloat(sentences, xDeathSentenceHealth) - amt;
					if (dist > 0) {
						xUnitSelectByID(dEnemies, xUnitID);
						trDamageUnit(dist);
						xSetFloat(sentences, xDeathSentenceHealth, amt - dist);
					} else {
						xSetFloat(sentences, xDeathSentenceHealth, amt);
					}
				}
				xSetPointer(dEnemies, hit);
			}
		}
	}
	
	for (x=xGetDatabaseCount(abducts); > 0) {
		if (processGenericProj(abducts) == PROJ_FALLING) {
			prev = xGetVector(abducts, xProjPrev);
			dir = xGetVector(abducts, xProjDir);
			pos = kbGetBlockPosition(""+xGetInt(abducts, xUnitName), true);
			dist = distanceBetweenVectors(pos, prev, false) + 3.0;
			if (trTimeMS() > xGetInt(abducts, xAbductTimeout)) {
				xUnitSelectByID(abducts, xUnitID);
				trUnitChangeProtoUnit("Kronny Birth SFX");
				xFreeDatabaseBlock(abducts);
			} else {
				xSetVector(abducts, xProjPrev, pos);
				for(y=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if ((xGetBool(dEnemies, xLaunched) == false) || (xGetInt(dEnemies, xUnitName) == bossUnit)) {
						pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
						current = distanceBetweenVectors(prev, pos, false);
						if (current < dist) {
							dest = xsVectorSet(xsVectorGetX(prev) + current * xsVectorGetX(dir), 0,
								xsVectorGetZ(prev) + current * xsVectorGetZ(dir)); // the hitbox
							
							if (distanceBetweenVectors(pos, dest) < 9.0) {
								gainFavor(p, 1);
								dest = xsVectorSet(xsVectorGetX(pos) - xsVectorGetX(dest) + xsVectorGetX(xGetVector(abducts,xAbductDest)),0,
									xsVectorGetZ(pos) - xsVectorGetZ(dest) + xsVectorGetZ(xGetVector(abducts,xAbductDest)));
								if (xGetBool(abducts, xAbductCurse) &&
									xGetBool(dEnemies, xDeathSentence) == false) {
									castDeathSentence(p);
									xSetBool(abducts, xAbductCurse, false);
								}
								launchUnit(dEnemies, dest);
							}
						}
					}
				}
			}
		}
	}
	
	dist = abductRange * xGetFloat(dPlayerData, xPlayerSpellRange);
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeNightrider(p);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dir = getUnitVector(xGetVector(dPlayerData, xPlayerWellPos), pos);
				prev = xsVectorSet(xsVectorGetX(pos) - dist * xsVectorGetX(dir), 0,
					xsVectorGetZ(pos) - dist * xsVectorGetZ(dir));
				addGenericProj(abducts,prev,dir);
				xSetVector(abducts, xProjPrev, prev);
				xSetVector(abducts, xAbductDest, pos);
				xSetInt(abducts, xAbductTimeout, trTimeMS() + dist / 0.015);
			}
		}
		trSoundPlayFN("changeunit.wav","1",-1,"","");
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"ariseCount", trQuestVarGet("p"+p+"ariseCount") + 3 + xGetDatabaseCount(sentences));
		trQuestVarSetFromRand("p"+p+"ariseAngle", 0, 3.14, false);
		trQuestVarSet("p"+p+"ariseDist", 2);
		trQuestVarSet("p"+p+"ariseNext", trTimeMS());
		trSoundPlayFN("ancestorsbirth.wav", "1", -1, "","");
	}
	
	if (trQuestVarGet("p"+p+"ariseCount") > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"ariseNext")) {
			amt = trQuestVarGet("p"+p+"ariseAngle");
			dir = xsVectorSet(xsSin(amt), 0, xsCos(amt));
			amt = trQuestVarGet("p"+p+"ariseAngle") * 57.295;
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeNightrider(p);
				} else {
					pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					pos = xsVectorSet(xsVectorGetX(pos) + trQuestVarGet("p"+p+"ariseDist") * xsVectorGetX(dir), 0,
						xsVectorGetZ(pos) + trQuestVarGet("p"+p+"ariseDist") * xsVectorGetZ(dir));
					spawnMinion(p, pos, amt);
				}
			}
			trQuestVarSet("p"+p+"ariseDist", 0.5 + trQuestVarGet("p"+p+"ariseDist"));
			trQuestVarSet("p"+p+"ariseAngle", 2.43 + trQuestVarGet("p"+p+"ariseAngle"));
			trQuestVarSet("p"+p+"ariseCount", trQuestVarGet("p"+p+"ariseCount") - 1);
			trQuestVarSet("p"+p+"ariseNext", trQuestVarGet("p"+p+"ariseNext") + 200);
		}
	}

	if (trQuestVarGet("p"+p+"nightfallSpy") == 1) {
		if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xNightfallSFX);
				trMutateSelected(kbGetProtoUnitID("Tartarian Gate"));
				trUnitOverrideAnimation(6,0,true,false,-1);
			}
			trQuestVarSet("p"+p+"nightfallSpy", 0);
		}
	}

	if (trQuestVarGet("p"+p+"nightfallActive") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"nightfallTimeout")) {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xNightfallSFX);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
			trQuestVarSet("p"+p+"nightfallActive", 0);
		}
	}
	
	switch(1*trQuestVarGet("p"+p+"nightfall"))
	{
		case 1:
		{
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"nightfallCloud");
			trMutateSelected(kbGetProtoUnitID("Kronny Birth SFX"));
			trUnitOverrideAnimation(2,0,true,false,-1);
			trQuestVarSet("p"+p+"nightfall", 2);
			trQuestVarSet("p"+p+"nightfallNext", trTimeMS() + 1000);
		}
		case 3:
		{
			for(x=xGetDatabaseCount(clouds); >0) {
				hit = processGenericProj(clouds);
			}
			if (hit == PROJ_BOUNCE) {
				trQuestVarSet("p"+p+"nightfall", 4);
				trQuestVarSet("p"+p+"nightfallDist", 0);
				trQuestVarSet("p"+p+"nightfallNext", trTimeMS());
			}
		}
		case 2:
		{
			if (trTimeMS() > trQuestVarGet("p"+p+"nightfallNext")) {
				xSetBool(dPlayerData, xPlayerLaunched, false);
				trQuestVarSet("p"+p+"nightfall", 3);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				pos = trVectorQuestVarGet("p"+p+"nightfallCenter");
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeNightrider(p);
					} else {
						trMutateSelected(kbGetProtoUnitID("Dwarf"));
						trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
						trUnitChangeProtoUnit("Hero Greek Achilles");
						if (PvP) {
							xSetInt(db, xCharIndex,activatePlayerUnit(xGetInt(db, xUnitName),p,kbGetProtoUnitID("Hero Greek Achilles")));
							xSetBool(dPlayerUnits, xIsHero, true);
							xSetFloat(dPlayerUnits, xPhysicalResist, xGetFloat(dPlayerData, xPlayerPhysicalResist, p));
							xSetFloat(dPlayerUnits, xMagicResist, xGetFloat(dPlayerData, xPlayerMagicResist, p));
							if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
								xSetInt(dPlayerData, xPlayerIndex, xGetInt(db, xCharIndex));
							}
						} else if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex)) == false) {
							debugLog("Nightrider " + p + ": Unable to restore database block");
						}
					}
				}
				equipRelicsAgain(p);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				if (trUnitVisToPlayer()) {
					trCameraShake(0.5, 0.5);
				}
				trUnitChangeProtoUnit("Kronny Birth SFX");
				
				/* spread the love */
				dir = vector(1,0,0);
				for(x=16; >0) {
					addGenericProj(clouds,pos,dir);
					dir = rotationMatrix(dir, 0.923879, 0.382683);
				}
				trSoundPlayFN("tartariangateselect.wav","1",-1,"","");
				trSoundPlayFN("changeunit.wav","1",-1,"","");
			}
		}
		case 4:
		{
			dist = trTimeMS() - trQuestVarGet("p"+p+"nightfallNext");
			dist = dist * 0.015;
			if (dist > nightfallRadius * xGetFloat(dPlayerData, xPlayerSpellRange)) {
				trQuestVarSet("p"+p+"nightfall", 0);
				for(x=xGetDatabaseCount(clouds); >0) {
					xDatabaseNext(clouds);
					xUnitSelectByID(clouds, xUnitID);
					trUnitDestroy();
				}
				xClearDatabase(clouds);

				trQuestVarSet("p"+p+"nightfallActive", 1);
				trQuestVarSet("p"+p+"nightfallTimeout", trTimeMS() + 1000 * nightfallDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeNightrider(p);
					} else if (xGetInt(db, xNightfallSFX) < 0) {
						spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db,xNightfallSFX,xGetPointer(db)),vector(0,0,0));
						trQuestVarSet("p"+p+"nightfallSpy", 1);
					} else {
						xUnitSelect(db, xNightfallSFX);
						trMutateSelected(kbGetProtoUnitID("Tartarian Gate"));
					}
				}
			}
			pos = trVectorQuestVarGet("p"+p+"nightfallCenter");
			dist = xsPow(dist, 2);
			amt = 0;
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (xGetBool(dEnemies, xDeathSentence) == false) {
					current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
					if (current < dist && current > trQuestVarGet("p"+p+"nightfallDist")) {
						if (current > amt) {
							amt = current;
						}
						castDeathSentence(p);
						stunUnit(dEnemies, deathSentenceDuration, p);
					}
				}
			}
			trQuestVarSet("p"+p+"nightfallDist", amt);
		}
		case 0:
		{
			if (xGetBool(dPlayerData, xPlayerLureActivated)) {
				xSetBool(dPlayerData, xPlayerLureActivated, false);
				gainFavor(p, 0.0 - nightfallCost * xGetFloat(dPlayerData, xPlayerUltimateCost));

				/* find the closest player unit and draw a line to the destination. stop on walls */
				pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p), true); // default to player unit
				dest = xGetVector(dPlayerData, xPlayerLurePos);
				dist = distanceBetweenVectors(pos, dest);
				for(i=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						prev = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
						current = distanceBetweenVectors(prev, dest);
						if (current < dist) {
							pos = prev;
							dist = current;
						}
					}
				}

				dist = xsSqrt(dist) / 2;
				dir = getUnitVector(pos, dest, 2.0);
				prev = pos;
				for(i = dist; >0) {
					pos = pos + dir;
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					} else {
						prev = pos;
					}
				}

				trVectorQuestVarSet("p"+p+"nightfallCenter", vectorSnapToGrid(prev));
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"lureObject");
				trUnitDestroy();
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trQuestVarSet("p"+p+"nightfall", 1);
				xSetBool(dPlayerData, xPlayerLaunched, true);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeNightrider(p);
					} else {
						trMutateSelected(kbGetProtoUnitID("Victory Marker"));
						if (PvP) {
							xSetPointer(dPlayerUnits, xGetInt(db, xCharIndex));
							xRestoreDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
							xFreeDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
							xFreeDatabaseBlock(dPlayerUnits);
						} else {
							xDetachDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex));
						}
					}
				}
				
				pos = trVectorQuestVarGet("p"+p+"nightfallCenter");
				trUnitSelectClear();
				trQuestVarSet("p"+p+"nightfallCloud", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Kronny Flying",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trDamageUnitPercent(100);
				trSetSelectedScale(0,3,0);
				
				trSoundPlayFN("cinematics\32_out\kronosbehinddorrshort.mp3","1",-1,"","");
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseNightrider(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * NIGHTRIDER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Abduct";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Arise";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Nightfall";
		lureIsUltimate = true;
	}
	xNightfallSFX = xInitAddInt(db, "nightfallSFX", -1);
	
	xSetInt(dPlayerData,xPlayerWellCooldown, abductCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, nightfallCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,ariseCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"sentences") == 0) {
		db = xInitDatabase("p"+p+"sentences");
		trQuestVarSet("p"+p+"sentences", db);
		xInitAddInt(db, "name");
		xDeathSentenceIndex = xInitAddInt(db, "index");
		xInitAddInt(db, "id");
		xDeathSentenceTimeout = xInitAddInt(db, "timeout");
		xDeathSentenceHealth = xInitAddFloat(db, "health");
		xDeathSentenceSFX = xInitAddInt(db, "sfx");
		xDeathSentenceProto = xInitAddInt(db, "proto");
	}
	
	if (trQuestVarGet("p"+p+"abducts") == 0) {
		db = initGenericProj("p"+p+"abducts",kbGetProtoUnitID("Kronny Birth SFX"),2,15.0,4.5,0,p,true);
		trQuestVarSet("p"+p+"abducts", db);
		xAbductDest = xInitAddVector(db, "dest");
		xAbductTimeout = xInitAddInt(db, "timeout");
		xAbductCurse = xInitAddBool(db, "curse", true);
	}
	
	if (trQuestVarGet("p"+p+"nightfallClouds") == 0) {
		db = initGenericProj("p"+p+"nightfallClouds", kbGetProtoUnitID("Kronny Birth SFX"),2,15.0,4.5,0,p);
		trQuestVarSet("p"+p+"nightfallClouds", db);
	}
}

void nightriderModify(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * NIGHTRIDER;
	zSetProtoUnitStat("Minion", p, 0, 0.2 * xGetFloat(dPlayerData, xPlayerHealth, p));
	zSetProtoUnitStat("Minion", p, 27, 0.2 * xGetFloat(dPlayerData, xPlayerAttack, p));
	zSetProtoUnitStat("Hero Greek Achilles", p, 27, xGetFloat(dPlayerData, xPlayerAttack, p));
}

rule nightrider_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * NIGHTRIDER + p, "nightriderAlways");
		trEventSetHandler(1000 + 12 * NIGHTRIDER + p, "chooseNightrider");
		trEventSetHandler(5000 + 12 * NIGHTRIDER + p, "nightriderModify");
	}
}
