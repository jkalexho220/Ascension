float seedHeal = 30;
float seedDuration = 15;

int bloodbloomCooldown = 15;

int stranglethornsCooldown = 12;
float stranglethornsRange = 6;
float stranglethornsRadius = 3;
float stranglethornsDuration = 3;
float stranglethornsDamage = 20;

float natureBountyCost = 60;
float natureBountyDuration = 12;

int xSeedReadyTime = 0;
int xSeedTimeout = 0;
int xSeedType = 0;

int xLifeArrowPos = 0;

int xBloodbloomAttacking = 0;
int xBloodbloomAttackNext = 0;

int xStranglethornTimeout = 0;
int xStranglethornPos = 0;

void removeGardener(int p = 0) {
	removePlayerSpecific(p);
}

void plantSeed(int next = 0, int p = 0) {
	int seeds = trQuestVarGet("p"+p+"seeds");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trUnitChangeProtoUnit("Rock Granite Small");
	xAddDatabaseBlock(seeds, true);
	xSetInt(seeds, xUnitName, next);
	xSetInt(seeds, xSeedReadyTime, trTimeMS() + 2000);
	xSetInt(seeds, xSeedTimeout, trTimeMS() + 1000 * seedDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	xSetInt(seeds, xSeedType, 1*trQuestVarGet("p"+p+"natureBounty"));
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	if (trQuestVarGet("p"+p+"natureBounty") == 0) {
		trUnitSetAnimationPath("1,0,0,0,0");
	} else {
		trUnitChangeProtoUnit("Spy Eye");
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Berry Bush"));
	}
	if (trCurrentPlayer() == p) {
		trUnitHighlight(seedDuration * xGetFloat(dPlayerData, xPlayerSpellDuration), true);
	}
}

void gardenerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * GARDENER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int seeds = trQuestVarGet("p"+p+"seeds");
	int lifeArrows = trQuestVarGet("p"+p+"lifeArrows");
	int bloodblooms = trQuestVarGet("p"+p+"bloodblooms");
	int stranglethorns = trQuestVarGet("p"+p+"stranglethorns");
	float amt = 0;
	float dist = 0;
	float current = 0;
	
	vector pos = vector(0,0,0);
	vector start = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	xSetPointer(dPlayerData, p);
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeGardener(p);
		} else {
			hit = CheckOnHit(p);
		}
	}
	
	current = seedDuration * xGetFloat(dPlayerData, xPlayerSpellDuration);
	amt = seedHeal;
	target = trQuestVarGet("p"+p+"seedHealPlayer") + 1;
	if (target >= ENEMY_PLAYER) {
		target = 1;
	}
	if (xGetInt(dPlayerData, xPlayerDead, target) == 0) {
		for (y=xGetDatabaseCount(seeds); >0) {
			xDatabaseNext(seeds);
			hit = 0;
			dist = xGetInt(seeds, xSeedTimeout) - trTimeMS();
			if (dist > 0) {
				dist = 0.001 * dist / current;
				pos = kbGetBlockPosition(""+xGetInt(seeds, xUnitName));
				if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, target), pos) < 1) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, target), true);
					if (trUnitPercentDamaged() > 0) {
						if (xGetInt(seeds, xSeedType) == 1) {
							healUnit(p, 4.0 * amt, xGetInt(dPlayerData, xPlayerIndex, target));
						} else {
							healUnit(p, amt, xGetInt(dPlayerData, xPlayerIndex, target));
						}
						hit = 1;
					}
				}
				xUnitSelect(seeds, xUnitName);
				if (hit == 1) {
					trQuestVarSet("eatSound", 1);
					trUnitChangeProtoUnit("Regeneration SFX");
				} else {
					dist = 0.5 + 0.5 * dist;
					trSetSelectedScale(dist, dist, dist);
				}
			} else {
				xUnitSelect(seeds, xUnitName);
				trUnitChangeProtoUnit("Dust Small");
				xFreeDatabaseBlock(seeds);
			}
		}
	}
	
	trQuestVarSet("p"+p+"seedHealPlayer", target);
	
	if (trQuestVarGet("eatSound") == 1) {
		trQuestVarSet("eatSound", 0);
		trSoundPlayFN("colossuseat.wav","1",-1,"","");
	}
	
	if (xGetDatabaseCount(lifeArrows) > 0) {
		xDatabaseNext(lifeArrows);
		xUnitSelect(lifeArrows, xUnitName);
		if (trUnitAlive() == false) {
			pos = xGetVector(lifeArrows, xLifeArrowPos);
			
			xFreeDatabaseBlock(lifeArrows);
			target = trGetNextUnitScenarioNameNumber();
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			plantSeed(target, p);
		} else {
			pos = kbGetBlockPosition(""+xGetInt(lifeArrows, xUnitName), true);
			if (xsVectorGetY(pos) < worldHeight + 0.3) {
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				target = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelect(""+target, true);
				trImmediateUnitGarrison(""+xGetInt(lifeArrows, xUnitName));
				trUnitChangeProtoUnit("Rock Granite Small");
				
				xUnitSelect(lifeArrows, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Arrow Flaming"));
				xFreeDatabaseBlock(lifeArrows);
				
				plantSeed(target, p);
			}
		}
	}
	
	for(y=xGetInt(dPlayerData, xPlayerProjectiles); >0) {
		if (yFindLatest("p"+p+"latestProj", "Arrow Flaming", p) > 0) {
			start = kbGetBlockPosition(""+1*trQuestVarGet("p"+p+"latestProj"));
			target = 0;
			dist = 25;
			for (x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				id = xGetInt(db, xUnitID);
				trUnitSelectClear();
				trUnitSelectByID(id);
				if (trUnitAlive() == false) {
					removeGardener(p);
				} else if (kbUnitGetAnimationActionType(id) == 12) {
					current = unitDistanceToVector(xGetInt(db, xUnitName), start);
					if (current < dist) {
						dist = current;
						target = xGetPointer(db);
					}
				}
			}
			if (target > 0) {
				xSetPointer(db, target);
				target = trGetUnitScenarioNameNumber(xGetInt(db, xCharAttackTarget));
				xAddDatabaseBlock(lifeArrows, true);
				xSetInt(lifeArrows, xUnitName, 1*trQuestVarGet("p"+p+"latestProj"));
				xSetVector(lifeArrows, xLifeArrowPos, kbGetBlockPosition(""+target));
			} else {
				break;
			}
		} else {
			break;
		}
	}
	
	if (xGetDatabaseCount(bloodblooms) > 0) {
		xDatabaseNext(bloodblooms);
		id = xGetInt(bloodblooms, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			trUnitSelectClear();
			trUnitSelect(""+(1+xGetInt(bloodblooms, xUnitName)), true);
			trDamageUnitPercent(100);
			xFreeDatabaseBlock(bloodblooms);
		} else if (kbUnitGetAnimationActionType(id) == 6) {
			if (xGetBool(bloodblooms, xBloodbloomAttacking) == false) {
				xSetBool(bloodblooms, xBloodbloomAttacking, true);
				xSetInt(bloodblooms, xBloodbloomAttackNext, trTimeMS() + 600);
			} else if (trTimeMS() > xGetInt(bloodblooms, xBloodbloomAttackNext)) {
				xSetInt(bloodblooms, xBloodbloomAttackNext, xGetInt(bloodblooms, xBloodbloomAttackNext) + 1300);
				xsSetContextPlayer(p);
				target = kbUnitGetTargetUnitID(id);
				xsSetContextPlayer(0);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					if (xGetInt(dEnemies, xUnitID) == target) {
						gainFavor(p, 1);
						poisonUnit(dEnemies, 12.0, 12.0, p);
						pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
						target = trGetNextUnitScenarioNameNumber();
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						plantSeed(target, p);
						trSoundPlayFN("farming1.wav","1",-1,"","");
						break;
					}
				}
			}
		} else if (xGetBool(bloodblooms, xBloodbloomAttacking)) {
			xSetBool(bloodblooms, xBloodbloomAttacking, false);
		}
	}
	
	hit = xGetDatabaseCount(stranglethorns);
	if (hit > 0) {
		current = 1000.0 / hit;
		if (trTimeMS() > trQuestVarGet("p"+p+"stranglethornsNext") + current) {
			trQuestVarSet("p"+p+"stranglethornsNext", trQuestVarGet("p"+p+"stranglethornsNext") + current);
			xDatabaseNext(stranglethorns);
			xUnitSelect(stranglethorns, xUnitName);
			if (trTimeMS() > xGetInt(stranglethorns, xStranglethornTimeout)) {
				trDamageUnitPercent(100);
				xFreeDatabaseBlock(stranglethorns);
			} else {
				pos = xGetVector(stranglethorns, xStranglethornPos);
				dist = xsPow(stranglethornsRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				amt = stranglethornsDamage * xGetFloat(dPlayerData, xPlayerSpellDuration);
				for (x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						stunUnit(dEnemies, 1.5, p, false);
						damageEnemy(p, amt);
					}
				}
			}
		}
	} else {
		trQuestVarSet("p"+p+"stranglethornsNext", trTimeMS());
	}
	
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		pos = xGetVector(dPlayerData, xPlayerWellPos);
		target = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect(""+p+",0");
		trUnitChangeProtoUnit("Heavenlight");
		dist = xsPow(stranglethornsRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		for(x=xGetDatabaseCount(seeds); >0) {
			xDatabaseNext(seeds);
			if (unitDistanceToVector(xGetInt(seeds, xUnitName), pos) < dist) {
				gainFavor(p, 1);
				xUnitSelect(seeds, xUnitName);
				trUnitHighlight(0.01,false);
				if (xGetInt(seeds, xSeedType) == 1) {
					trUnitChangeProtoUnit("Walking Berry Bush");
					xUnitSelect(seeds, xUnitName);
					trUnitConvert(p);
					trUnitChangeProtoUnit("Walking Berry Bush");
					activatePlayerUnit(xGetInt(seeds, xUnitName), p, kbGetProtoUnitID("Walking Berry Bush"), calculateDecay(p, 5.0));
					xSetInt(dPlayerUnits, xDecayNext, trTimeMS() + 5000); // berry bush birth animation is five seconds long wtf
				} else {
					trUnitConvert(0);
					trUnitChangeProtoUnit("Uproot 2x2");
					trQuestVarSetFromRand("angle", 0, 3.14, false);
					dir = xsVectorSet(xsCos(trQuestVarGet("angle")),0,xsSin(trQuestVarGet("angle")));
					xUnitSelect(seeds, xUnitName);
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trSetSelectedScale(xGetFloat(dPlayerData, xPlayerSpellRange),1,xGetFloat(dPlayerData, xPlayerSpellRange));
					xAddDatabaseBlock(stranglethorns, true);
					xSetInt(stranglethorns, xUnitName, xGetInt(seeds, xUnitName));
					xSetVector(stranglethorns, xStranglethornPos, kbGetBlockPosition(""+xGetInt(seeds, xUnitName),true));
					xSetInt(stranglethorns, xStranglethornTimeout, trTimeMS() + 1000 * stranglethornsDuration);
				}
				xFreeDatabaseBlock(seeds);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		target = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trUnitSelectClear();
		trUnitSelect(""+(1+target), true);
		trUnitChangeProtoUnit("Audrey Base");
		trUnitSelectClear();
		trUnitSelect(""+target, true);
		trUnitChangeProtoUnit("Audrey");
		xAddDatabaseBlock(bloodblooms, true);
		xSetInt(bloodblooms, xUnitName, target);
		xSetInt(bloodblooms, xUnitID, kbGetBlockID(""+target, true));
		
		activatePlayerUnit(target, p, kbGetProtoUnitID("Audrey"), calculateDecay(p, 8.0));
		xSetInt(dPlayerUnits, xDecayNext, trTimeMS() + 2000);
	}
	
	if (trQuestVarGet("p"+p+"natureBounty") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"natureBountyTimeout")) {
			trQuestVarSet("p"+p+"natureBounty", 0);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trSoundPlayFN("walkingwoods1.wav", "1", -1, "", "");
		trSoundPlayFN("gaiaforest.wav", "1", -1, "", "");
		gainFavor(p, 0.0 - natureBountyCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trQuestVarSet("p"+p+"natureBounty", 1);
		trQuestVarSet("p"+p+"natureBountyTimeout",
			trTimeMS() + 1000 * natureBountyDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseGardener(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * GARDENER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	trQuestVarSet("p"+p+"latestProj", trGetNextUnitScenarioNameNumber() - 1);
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Stranglethorns";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Nature's Bounty";
		rainIsUltimate = true;
		map("q", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(Q) Bloodbloom";
		lureIsUltimate = false;
	}
	
	xSetInt(dPlayerData,xPlayerWellCooldown, stranglethornsCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, bloodbloomCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, natureBountyCost);
	
	if (trQuestVarGet("p"+p+"seeds") == 0) {
		db = xInitDatabase("p"+p+"seeds");
		trQuestVarSet("p"+p+"seeds", db);
		xInitAddInt(db, "name");
		xSeedReadyTime = xInitAddInt(db, "ready");
		xSeedTimeout = xInitAddInt(db, "timeout");
		xSeedType = xInitAddInt(db, "type");
	}
	
	if (trQuestVarGet("p"+p+"lifeArrows") == 0) {
		db = xInitDatabase("p"+p+"lifeArrows");
		trQuestVarSet("p"+p+"lifeArrows", db);
		xInitAddInt(db, "name");
		xLifeArrowPos = xInitAddVector(db, "pos");
	}
	
	if (trQuestVarGet("p"+p+"bloodblooms") == 0) {
		db = xInitDatabase("p"+p+"bloodblooms");
		trQuestVarSet("p"+p+"bloodblooms", db);
		xInitAddInt(db, "name");
		xBloodbloomAttacking = xInitAddBool(db, "attacking");
		xInitAddInt(db, "id");
		xBloodbloomAttackNext = xInitAddInt(db, "attackNext");
	}
	
	if (trQuestVarGet("p"+p+"stranglethorns") == 0) {
		db = xInitDatabase("p"+p+"stranglethorns");
		trQuestVarSet("p"+p+"stranglethorns", db);
		xInitAddInt(db, "name");
		xStranglethornTimeout = xInitAddInt(db, "timeout");
		xStranglethornPos = xInitAddVector(db, "pos");
	}
}

void modifyGardener(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * GARDENER;
	zSetProtoUnitStat("Walking Berry Bush", p, 0, 0.5 * xGetFloat(dPlayerData, xPlayerHealth, p));
	zSetProtoUnitStat("Walking Berry Bush", p, 27, 0.5 * xGetFloat(dPlayerData, xPlayerAttack, p));
	zSetProtoUnitStat("Audrey", p, 0, xGetFloat(dPlayerData, xPlayerHealth, p));
	zSetProtoUnitStat("Audrey", p, 27, xGetFloat(dPlayerData, xPlayerAttack, p));
}


rule gardener_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * GARDENER + p, "gardenerAlways");
		trEventSetHandler(1000 + 12 * GARDENER + p, "chooseGardener");
		trEventSetHandler(5000 + 12 * GARDENER + p, "modifyGardener");
	}
}
