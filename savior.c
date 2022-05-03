int guardianAngelCooldown = 8;
float guardianAngelHeal = 50;
float guardianAngelRange = 10;

int unityCooldown = 20;
float unityRadius = 10;
float unityDuration = 8;
float unityBonus = 0.5;
float unityHeal = 0.5;

float interventionCost = 100;
int interventionCooldown = 10;

int xJumping = 0;

void removeSavior(int p = 0) {
	removePlayerSpecific(p);
	
}

void saviorAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * SAVIOR;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		
		dist = xsPow(2 * guardianAngelRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		hit = -1;
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			xUnitSelectByID(dPlayerUnits, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				amt = unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), xGetVector(dPlayerData, xPlayerWellPos));
				if (amt < dist) {
					dist = amt;
					hit = xGetPointer(dPlayerUnits);
				}
			}
		}
		
		if (hit > 0) {
			gainFavor(p, 5);
			xSetPointer(dPlayerUnits, hit);
			pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
			dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerWellPos), 2.0);
			dist = xsMin(guardianAngelRange * xGetFloat(dPlayerData, xPlayerSpellRange),
				distanceBetweenVectors(xGetVector(dPlayerData, xPlayerWellPos), pos, false)) / 2;
			for(x=dist; >0) {
				start = pos + dir;
				if (terrainIsType(vectorToGrid(start), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					break;
				} else {
					pos = start;
				}
			}
			amt = 0;
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),180,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeSavior(p);
				} else {
					start = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dir = getUnitVector(start, pos);
					amt = amt + distanceBetweenVectors(start, pos, false);
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Hero Greek Bellerophon");
					xUnitSelectByID(db, xUnitID);
					trSetUnitOrientation(dir,vector(0,1,0),true);
				}
			}
			equipRelicsAgain(p);
			
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trUnitChangeProtoUnit("Arkantos God Out");
			trSoundPlayFN("arkantosarrive.wav","1",-1,"","");
			
			amt = guardianAngelHeal * (1.0 + 0.1 * amt);
			dist = xsPow(guardianAngelRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
					xUnitSelectByID(dPlayerUnits, xUnitID);
					healUnit(p, amt * xGetFloat(dPlayerData, xPlayerSpellDamage));
				}
			}
		} else {
			xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COST);
			if (trCurrentPlayer() == p) {
				trCounterAbort("well");
				trChatSend(0, "You must choose a unit to teleport to!");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		if (trQuestVarGet("p"+p+"unity") == 1) {
			for(x=trQuestVarGet("p"+p+"unityNext"); < trQuestVarGet("p"+p+"unityend")) {
				trUnitSelectClear();
				trUnitSelect(""+x, true);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trUnitDestroy();
			}
		}
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerLurePos));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",8,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Spy Eye");
		for(x=trQuestVarGet("next"); < trGetNextUnitScenarioNameNumber()) {
			trUnitSelectClear();
			trUnitSelect(""+x, true);
			trMutateSelected(kbGetProtoUnitID("Valkyrie"));
			trSetSelectedScale(0,0,0);
			trUnitSetAnimationPath("1,0,0,0,0,0,0");
			trUnitOverrideAnimation(15,0,true,false,-1);
		}
		trSoundPlayFN("gaiasparkle3.wav","1",-1,"","");
		trQuestVarSet("p"+p+"unity", 1);
		trQuestVarSet("p"+p+"unityNext", trQuestVarGet("next"));
		trQuestVarSet("p"+p+"unityEnd", trGetNextUnitScenarioNameNumber());
		trQuestVarSet("p"+p+"unityTimeout",
			trTimeMS() + 1000 * unityDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		trQuestVarSet("p"+p+"unityLast", trTimeMS());
		trQuestVarSet("p"+p+"unityradius", unityRadius * xGetFloat(dPlayerData, xPlayerSpellRange));
		trQuestVarSet("p"+p+"unityangle", 0);
		trVectorQuestVarSet("p"+p+"unityPos", pos);
		trQuestVarSet("p"+p+"unityLast", trTimeMS());
		trQuestVarSet("p"+p+"unitySearch", p);
	}
	
	if (trQuestVarGet("p"+p+"unity") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"unitytimeout")) {
			for(x=trQuestVarGet("p"+p+"unityNext"); < trQuestVarGet("p"+p+"unityend")) {
				trUnitSelectClear();
				trUnitSelect(""+x, true);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trUnitDestroy();
			}
			zSetProtoUnitStat("Hero Greek Bellerophon", p, 27, xGetFloat(dPlayerData, xPlayerBaseAttack));
			xSetFloat(dPlayerData, xPlayerAttack, xGetFloat(dPlayerData, xPlayerBaseAttack));
			trQuestVarSet("p"+p+"unity", 0);
			for (x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				if (getBit(p, xGetInt(dPlayerUnits, xUnity)) == true) {
					xSetInt(dPlayerUnits, xUnity, xGetInt(dPlayerUnits, xUnity) - xsPow(2, p));
					xSetFloat(dPlayerUnits, xMagicResist, calculateArmor(xGetFloat(dPlayerUnits, xMagicResist), -1.0));
				}
			}
		} else {
			dist = trTimeMS() - trQuestVarGet("p"+p+"unitylast");
			amt = trQuestVarGet("p"+p+"unityangle");
			amt = fModulo(6.283185, amt + dist * 0.001);
			trQuestVarSet("p"+p+"unitylast", trTimeMS());
			trQuestVarSet("p"+p+"unityangle", amt);
			dir = xsVectorSet(xsSin(amt),0,xsCos(amt));
			
			amt = trQuestVarGet("p"+p+"unityradius");
			dist = amt * amt;
			amt = 0.5 * amt;
			for(x=0; <8) {
				trUnitSelectClear();
				trUnitSelect(""+(x+trQuestVarGet("p"+p+"unityNext")), true);
				trSetSelectedUpVector(amt * xsVectorGetX(dir),0,amt * xsVectorGetZ(dir));
				dir = rotationMatrix(dir, 0.707107, 0.707107);
			}
			if (trTime() > trQuestVarGet("p"+p+"unityTime")) {
				pos = trVectorQuestVarGet("p"+p+"unitypos");
				trQuestVarSet("p"+p+"unityTime", trTime());
				hit = 0;
				for (x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
						hit = hit + 1;
						if (getBit(p, xGetInt(dPlayerUnits, xUnity)) == false) {
							xSetInt(dPlayerUnits, xUnity, xGetInt(dPlayerUnits, xUnity) + xsPow(2, p));
							xSetFloat(dPlayerUnits, xMagicResist, calculateArmor(xGetFloat(dPlayerUnits, xMagicResist), 0.5));
						}
					} else if (getBit(p, xGetInt(dPlayerUnits, xUnity)) == true) {
						xSetInt(dPlayerUnits, xUnity, xGetInt(dPlayerUnits, xUnity) - xsPow(2, p));
						xSetFloat(dPlayerUnits, xMagicResist, calculateArmor(xGetFloat(dPlayerUnits, xMagicResist), -1.0));
					}
				}
				
				trQuestVarSet("p"+p+"unityBuff", unityBonus * hit);
				xSetFloat(dPlayerData, xPlayerAttack,
					xGetFloat(dPlayerData, xPlayerBaseAttack) * (1.0 + trQuestVarGet("p"+p+"unityBuff")));
				zSetProtoUnitStat("Hero Greek Bellerophon", p, 27, xGetFloat(dPlayerData, xPlayerAttack));
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"intervention", 1);
	}
	
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeSavior(p);
		} else {
			hit = CheckOnHit(p);
			if (hit == ON_HIT_JUMP) {
				if (xGetBool(db, xJumping) == false) {
					xSetBool(db, xJumping, true);
					xsSetContextPlayer(p);
					target = kbUnitGetTargetUnitID(id);
					xsSetContextPlayer(0);
					xSetInt(db, xCharAttackTarget, target);
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						if (xGetInt(dEnemies, xUnitID) == target) {
							xSetInt(db, xCharAttackTargetIndex, xGetPointer(dEnemies));
							xSetInt(dPlayerData, xPlayerPoisonKillerActive, xGetInt(dEnemies, xPoisonStatus));
							break;
						}
					}
				}
			} else if (xGetBool(db, xJumping)) {
				xSetBool(db, xJumping, false);
				if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
					stunUnit(dEnemies, 3.0, p);
				}
			} else if (hit >= ON_HIT_NORMAL) {
				target = 1;
				if (hit == ON_HIT_SPECIAL) {
					if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
						start = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
						pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
						dir = getUnitVector(start, pos);
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
						trUnitHighlight(1.0, false);
						trSetUnitOrientation(xsVectorSet(0.0 - xsVectorGetX(dir),0, 0.0 - xsVectorGetZ(dir)), vector(0,1,0), true);
						dist = 10.0 + xGetFloat(dPlayerData, xPlayerRange);
						xAddDatabaseBlock(dPlayerLasers, true);
						xSetInt(dPlayerLasers, xUnitName, 1*trQuestVarGet("next"));
						xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 500);
						xSetFloat(dPlayerLasers, xPlayerLaserRange, dist * 1.3);
						amt = xGetFloat(dPlayerData, xPlayerAttack);
						for(x=xGetDatabaseCount(dEnemies); >1) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (rayCollision(dEnemies,start,dir,current, 9.0)) {
								damageEnemy(p, amt, false);
								target = 1 + target;
								OnHit(p, xGetPointer(dEnemies), false);
							}
						}
					}
				}
				if (trQuestVarGet("p"+p+"unity") == 1) {
					pos = trVectorQuestVarGet("p"+p+"unityPos");
					amt = xGetFloat(dPlayerData, xPlayerAttack) * unityHeal * target;
					dist = xsPow(unityRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
					gainFavor(p, 2.0);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(dPlayerUnits, pos) < dist) {
							healUnit(p, amt);
							if (xGetBool(dPlayerUnits, xIsHero)) {
								gainFavor(xGetInt(dPlayerUnits, xPlayerOwner), 2.0);
							}
						}
					}
				}
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"dead") > 0 &&
		trPlayerResourceCount(p, "favor") >= interventionCost * xGetFloat(dPlayerData, xPlayerUltimateCost)) {
		trQuestVarSet("p"+p+"intervention", 1);
	}
	
	if (trQuestVarGet("p"+p+"intervention") == 1) {
		for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
			xDatabaseNext(dPlayerCharacters);
			xUnitSelectByID(dPlayerCharacters, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerCharacter();
			}
		}
		for(x=1; < ENEMY_PLAYER) {
			if (xGetInt(dPlayerData, xPlayerDead, x) > 0) {
				revivePlayer(x);
				xSetInt(dPlayerData, xPlayerDead, x, 0);
			}
		}
		gainFavor(p, 0.0 - interventionCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		xSetInt(dPlayerData, xPlayerRainCooldownStatus, ABILITY_COOLDOWN);
		trChatSend(0, "<color=1,1,1>Intervention!</color>");
		trSoundPlayFN("restorationbirth.wav","1",-1,"","");
		trSoundPlayFN("herobirth3.wav","1",-1,"","");
		trQuestVarSet("p"+p+"intervention", 0);
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseSavior(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * SAVIOR;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Guardian Angel";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Intervention";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Unity";
		lureIsUltimate = false;
	}
	
	xJumping = xInitAddBool(db, "jumping");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, guardianAngelCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, unityCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,interventionCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, interventionCost);
}


rule savior_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SAVIOR + p, "saviorAlways");
		trEventSetHandler(1000 + 12 * SAVIOR + p, "chooseSavior");
	}
}
