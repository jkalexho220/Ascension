const int quickdrawCooldown = 10;
const float quickdrawRadius = 3.0;
const int quickdrawDuration = 3000;

const int echoBombCooldown = 20;
const float echoBombDuration = 6;
const float echoBombRadius = 12;

const float bulletStormRange = 8;

const float commandoPadStepRadius = 1.5;
const float commandoPadExplodeRadius = 12.0;

int xCommandoBulletStormSFX = 0;
int xCommandoQuickdrawSFX = 0;
int xCommandoBulletStormTargetIndex = 0;
int xCommandoBulletStormShoot = 0;

int xCommandoPadID = 0;
int xCommandoPadPos = 0;
int xCommandoPadSourcePos = 0;
int xCommandoPadTimeout = 0;

int xQuickdrawIndex = 0;

int xEchoBombStartingHealth = 0;
int xEchoBombCurrentHealth = 0;
int xEchoBombSize = 0;
int xEchoBombPos = 0;
int xEchoBombIndex = 0;
int xEchoBombTimeout = 0;
int xEchoBombUnit = 0;

void removeCommando(int p = 0) {
	int db = getCharactersDB(p);
	xUnitSelect(db, xCommandoBulletStormSFX);
	trUnitDestroy();
	xUnitSelect(db, xCommandoQuickdrawSFX);
	trUnitDestroy();
	removePlayerSpecific(p);
}

float commandoCritMultiplier(int p = 0) {
	float amt = 1.0;
	trQuestVarSetFromRand("rand", 0, 1.0, false);
	if (trQuestVarGet("rand") < xGetFloat(dPlayerData, xPlayerPhysicalResist)) {
		amt = amt * 2;
		gainFavor(p, 1.0);
	}
	return(amt);
}


void shootBullet(int p = 0, vector start = vector(0,0,0), vector end = vector(0,0,0)) {
	float dist = distanceBetweenVectors(start, end, false);
	vector dir = getUnitVector(start, end);
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
	trArmySelect("1,0");
	trUnitSetStance("Passive");
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	trSetUnitOrientation(vector(0,0,0) - dir,vector(0,1,0),true);
	trUnitHighlight(10.0, false);
	xAddDatabaseBlock(dPlayerLasers, true);
	xSetInt(dPlayerLasers, xUnitName, next);
	xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 1000);
	xSetFloat(dPlayerLasers, xPlayerLaserScale, 0.5);
	xSetFloat(dPlayerLasers, xPlayerLaserRange, dist * 1.4);
}


void minigunOff(int p = 0) {
	trModifyProtounit("Royal Guard Hero", p, 55, 1);
	trQuestVarSet("p"+p+"bulletStormActive", 0);
	int db = getCharactersDB(p);
	for(i=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xUnitSelect(db, xCommandoBulletStormSFX);
		trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
	}
}

void placeCommandoPad(int p = 0, vector pos = vector(0,0,0)) {
	int commandoPads = trQuestVarGet("p"+p+"commandoPads");
	int next = 0;
	int count = 1;
	if (playerHasSymphony(p, SYMPHONY_DOUBLE_SPECIAL)) {
		count = 2;
	}
	for(i=count; >0) {
		next = trGetNextUnitScenarioNameNumber();
		trQuestVarSetFromRand("rand", 0, 6.283185, false);
		pos = pos + xsVectorSet(xsCos(trQuestVarGet("rand")),0,xsSin(trQuestVarGet("rand")));
		vector snap = vectorSnapToGrid(pos);
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(snap),0,xsVectorGetZ(snap),0,true);
		int id = kbGetBlockID(""+next, true);
		xAddDatabaseBlock(commandoPads, true);
		xSetInt(commandoPads, xCommandoPadID, id);
		xSetVector(commandoPads, xCommandoPadSourcePos, snap);
		xSetVector(commandoPads, xCommandoPadPos, pos);
		xSetInt(commandoPads, xCommandoPadTimeout, trTimeMS() + 20000 * xGetFloat(dPlayerData, xPlayerSpellDuration));

		xUnitSelectByID(commandoPads, xCommandoPadID);
		trUnitChangeProtoUnit("UI Range Indicator Norse SFX");
		xUnitSelectByID(commandoPads, xCommandoPadID);
		if (trCurrentPlayer() == p) {
			trSetSelectedUpVector(xsVectorGetX(pos) - xsVectorGetX(snap),0,xsVectorGetZ(pos) - xsVectorGetZ(snap));
		} else {
			trSetSelectedUpVector(0,-10,0);
		}
	}
}

/*
void plantEchoBomb(int p = 0) {
	float amt = 0;
	int id = xGetInt(dEnemies, xUnitID);
	int echoBombs = trQuestVarGet("p"+p+"echoBombs");
	xSetBool(dEnemies, xEchoBomb, true);
	xAddDatabaseBlock(echoBombs, true);
	xSetInt(echoBombs, xEchoBombUnit, xGetInt(dEnemies, xUnitName));
	if ((xGetInt(dEnemies, xUnitName) == bossUnit) && (trQuestVarGet("stage") >= 9)) {
		// can't cast spy on the titan atlantean
		xSetInt(echoBombs, xUnitName, aiPlanGetUserVariableInt(ARRAYS, bossInts, 2));
		xUnitSelect(echoBombs, xUnitName);
		trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
	} else {
		spyEffect(xGetInt(dEnemies, xUnitName), kbGetProtoUnitID("Phoenix Egg"),
			xsVectorSet(echoBombs, xUnitName, xGetPointer(echoBombs)));
	}
	if (PvP) {
		xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner, xGetInt(dEnemies, xDoppelganger)));
	} else {
		xsSetContextPlayer(ENEMY_PLAYER);
	}
	amt = kbUnitGetCurrentHitpoints(id);
	xsSetContextPlayer(0);
	xSetFloat(echoBombs, xEchoBombStartingHealth, amt);
	xSetFloat(echoBombs, xEchoBombCurrentHealth, amt);
	xSetFloat(echoBombs, xEchoBombSize, 1.0);
	xSetVector(echoBombs, xEchoBombPos, xGetVector(dEnemies, xUnitPos));
	xSetInt(echoBombs, xEchoBombIndex, xGetPointer(dEnemies));
	xSetInt(echoBombs, xEchoBombTimeout,
		trTimeMS() + 1000 * echoBombDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	trSoundPlayFN("siegeselect.wav", "1", -1, "","");
	trSoundPlayFN("gatherpoint.wav","1",-1,"","");
}
*/

void commandoSmite(int p = 0) {
	int db = getCharactersDB(p);
	if (xGetFloat(db, xCharSmiteDamage) + trQuestVarGet("p"+p+"laserAttacks") > 0) {
		vector pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(db, xUnitName)));
		vector dir = getUnitVector(pos, kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName)));
		float dist = xsMax(16.0, xGetFloat(dPlayerData, xPlayerRange, p) + 4.0);
		int next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("1,0");
		trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
		trUnitHighlight(2.0, false);
		trSetUnitOrientation(vector(0,0,0) - dir, vector(0,1,0), true);
		xAddDatabaseBlock(dPlayerLasers, true);
		xSetInt(dPlayerLasers, xUnitName, next);
		xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 500);
		xSetFloat(dPlayerLasers, xPlayerLaserRange, dist * 1.4);
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive()) {
				if (rayCollision(dEnemies, pos, dir, dist, 3.0)) {
					damageEnemy(p, xGetFloat(db, xCharSmiteDamage) + trQuestVarGet("p"+p+"laserAttacks"));
				}
			}
		}
	}
}

void commandoAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * COMMANDO;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int harpies = trQuestVarGet("p"+p+"fireharpies");
	int echoBombs = trQuestVarGet("p"+p+"echoBombs");
	int quickdrawIncoming = trQuestVarGet("p"+p+"quickdrawTargetsIncoming");
	int quickdrawTargets = trQuestVarGet("p"+p+"quickdrawTargets");
	int commandoPads = trQuestVarGet("p"+p+"commandoPads");
	float amt = 0;
	float dist = 0;
	float current = 0;
	float angleDiff = 0;
	float curDiff = 0;
	float curDist = 0;
	float outer = 0;
	float inner = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector end = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector left = vector(0,0,0);
	vector right = vector(0,0,0);
	
	// basic loop
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeCommando(p);
		} else {
			hit = CheckOnHit(p);
			start = kbGetBlockPosition(""+xGetInt(db, xUnitName));
			if (hit >= ON_HIT_NORMAL) {
				id = xGetPointer(dEnemies);
				xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex));
				xUnitSelectByID(dEnemies, xUnitID);
				damageEnemy(p, (commandoCritMultiplier(p) - 1.0) * xGetFloat(dPlayerData, xPlayerAttack), false);
				if (hit == ON_HIT_SPECIAL) {
					xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex));
					placeCommandoPad(p, kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true));
				}
				xSetPointer(dEnemies, id);
			}

			pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			amt = xsPow(commandoPadStepRadius, 2);
			dist = xsPow(commandoPadExplodeRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			for(i=xGetDatabaseCount(commandoPads); >0) {
				xDatabaseNext(commandoPads);
				if (distanceBetweenVectors(pos, xGetVector(commandoPads, xCommandoPadPos)) < amt) {
					xUnitSelectByID(commandoPads, xCommandoPadID);
					trUnitDestroy();

					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trUnitChangeProtoUnit("Deconstruct Unit");
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trUnitOverrideAnimation(18, 0, false, true, -1);

					if (trCurrentPlayer() == p) {
						trSoundPlayFN("storehouse.wav");
					}
					xUnitSelect(db, xUnitName);
					gainFavor(p, 3.0);

					trQuestVarSet("p"+p+"commandoBoost", 0.1 * xGetFloat(dPlayerData, xPlayerBaseAttack) + trQuestVarGet("p"+p+"commandoBoost"));
					xFreeDatabaseBlock(commandoPads);
					break;
				}
			}
		}
	}
	amt = (0.0 + trTimeMS() - trQuestVarGet("p"+p+"commandoBoostLast")) * 0.001;
	trQuestVarSet("p"+p+"commandoBoostLast", trTimeMS());

	if (trQuestVarGet("p"+p+"commandoBoost") > 0.0) {
		trQuestVarSet("p"+p+"commandoBoost", xsMax(0.0, trQuestVarGet("p"+p+"commandoBoost") - xsMax(amt, 0.05 * amt * trQuestVarGet("p"+p+"commandoBoost"))));
	}

	xSetFloat(dPlayerData, xPlayerAttack, xGetFloat(dPlayerData, xPlayerBaseAttack) + trQuestVarGet("p"+p+"commandoBoost"));
	zSetProtoUnitStat("Royal Guard Hero", p, 27, xGetFloat(dPlayerData, xPlayerAttack));

	if (xGetDatabaseCount(commandoPads) > 0) {
		xDatabaseNext(commandoPads);
		if (trTimeMS() > xGetInt(commandoPads, xCommandoPadTimeout)) {
			xUnitSelectByID(commandoPads, xCommandoPadID);
			trUnitDestroy();
			xFreeDatabaseBlock(commandoPads);
		}
	}

	
	if (trQuestVarGet("p"+p+"quickdrawSFX") == 1) {
		if (trQuestVarGet("spyFind") == trQuestVarGet("spyFound")) {
			for(i=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xCommandoQuickdrawSFX);
				trMutateSelected(kbGetProtoUnitID("Heka Shockwave SFX"));
			}
			trQuestVarSet("p"+p+"quickdrawSFX", 0);
		}
	}

	if (trQuestVarGet("p"+p+"quickdraw") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"quickdrawTimeout")) {
			trSoundPlayFN("storehouse.wav");
			trQuestVarSet("p"+p+"quickdraw", 0);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				if (trUnitAlive() == false) {
					removeCommando(p);
				} else if (xGetInt(db, xCommandoQuickdrawSFX) != 0) {
					xUnitSelect(db, xCommandoQuickdrawSFX);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
			trModifyProtounit("Royal Guard Hero", p, 1, 0.0 - trQuestVarGet("p"+p+"quickdrawBoost"));
		} else {
			hit = xGetPointer(dEnemies);
			xDatabaseNext(db);
			pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			dist = xsPow(quickdrawRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			for(x=xGetDatabaseCount(quickdrawIncoming); >0) {
				xDatabaseNext(quickdrawIncoming);
				xUnitSelect(quickdrawIncoming, xUnitName);
				if (trUnitAlive()) {
					if (distanceBetweenVectors(kbGetBlockPosition(""+xGetInt(quickdrawIncoming, xUnitName), true), pos) < dist) {
						if (trCurrentPlayer() == p) {
							trUnitHighlight(20.0, false);
						}
						xSetPointer(dEnemies, xGetInt(quickdrawIncoming, xQuickdrawIndex));
						silenceUnit(dEnemies, 6.0, p);
						xAddDatabaseBlock(quickdrawTargets, true);
						xSetInt(quickdrawTargets, xUnitName, xGetInt(quickdrawIncoming, xUnitName));
						xSetInt(quickdrawTargets, xQuickdrawIndex, xGetInt(quickdrawIncoming, xQuickdrawIndex));
						xFreeDatabaseBlock(quickdrawIncoming);
					}
				} else {
					xFreeDatabaseBlock(quickdrawIncoming);
				}
			}
			xSetPointer(dEnemies, hit);

			start = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit), true);
			amt = 0.001 * (0.0 + trTimeMS() - trQuestVarGet("p"+p+"quickdrawLast"));
			trQuestVarSet("p"+p+"quickdrawLast", trTimeMS());
			for(i=xGetDatabaseCount(commandoPads); >0) {
				xDatabaseNext(commandoPads);
				pos = xGetVector(commandoPads, xCommandoPadPos);
				dist = distanceBetweenVectors(start, pos, false);
				if (dist < 10.0) {
					xSetVector(commandoPads, xCommandoPadPos, (start - pos) * xsMax(dist, xGetFloat(dPlayerData, xPlayerSpeed)) * amt + pos);
				}
				xUnitSelectByID(commandoPads, xCommandoPadID);
				dir = xGetVector(commandoPads, xCommandoPadPos) - xGetVector(commandoPads, xCommandoPadSourcePos);
				if (trCurrentPlayer() == p) {
					trSetSelectedUpVector(xsVectorGetX(dir),0,xsVectorGetZ(dir));
				}
			}
		}
	} else if (xGetDatabaseCount(quickdrawTargets) > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"quickdrawTimeout")) {
			gainFavor(p, 1.0);
			trQuestVarSet("p"+p+"quickdrawTimeout", trQuestVarGet("p"+p+"quickdrawTimeout") + 100);
			trQuestVarSet("bulletSound", 1);
			xUnitSelect(quickdrawTargets, xUnitName);
			if (trUnitAlive()) {
				if (trCurrentPlayer() == p) {
					trUnitHighlight(0.1, true);
				}
				amt = commandoCritMultiplier(p);
				target = xGetPointer(dEnemies);
				xSetPointer(dEnemies, xGetInt(quickdrawTargets, xQuickdrawIndex));
				damageEnemy(p, amt * xGetFloat(dPlayerData, xPlayerAttack) * xGetDatabaseCount(db), false);
				OnHit(p, xGetPointer(dEnemies));
				for(i=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeCommando(p);
					} else {
						shootBullet(p, kbGetBlockPosition(""+xGetInt(db, xUnitName), true), kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true));
						xSetInt(db, xCharSpecialAttack, xGetInt(db, xCharSpecialAttack) - 1);
						if (xGetInt(db, xCharSpecialAttack) <= 0) {
							xSetInt(db, xCharSpecialAttack, xsMax(xGetInt(dPlayerData,xPlayerSpecialAttackCooldown,p), xGetInt(dClass, xClassSpecialAttackCooldown, xGetInt(dPlayerData, xPlayerClass)) / 2));
							placeCommandoPad(p, kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true));
						}
						commandoSmite(p);
					}
				}
				
				xSetPointer(dEnemies, target);
			}
			xFreeDatabaseBlock(quickdrawTargets);
		}
	}
	
	// QUICKDRAW BABY
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("recreation.wav","1",-1,"","");
		trSoundPlayFN("rainofarrows.wav");

		if ((trQuestVarGet("p"+p+"quickdrawTimeout") < trTimeMS()) || (xGetDatabaseCount(quickdrawTargets) > 0)) {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeCommando(p);
				} else if (xGetInt(db, xCommandoQuickdrawSFX) == 0) {
					spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db,xCommandoQuickdrawSFX,xGetPointer(db)));
				}
			}
			trQuestVarSet("p"+p+"quickdrawSFX", 1);

			trQuestVarSet("p"+p+"quickdraw", 1);
			trQuestVarSet("p"+p+"quickdrawBoost", xGetFloat(dPlayerData, xPlayerSpeed) * 2);
			trModifyProtounit("Royal Guard Hero", p, 1, trQuestVarGet("p"+p+"quickdrawBoost"));
			trQuestVarSet("p"+p+"quickdrawLast", trTimeMS());
		}

		for(x=xGetDatabaseCount(dEnemies); > 0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				xAddDatabaseBlock(quickdrawIncoming, true);
				xSetInt(quickdrawIncoming, xUnitName, xGetInt(dEnemies, xUnitName));
				xSetInt(quickdrawIncoming, xQuickdrawIndex, xGetPointer(dEnemies));
			}
		}

		trQuestVarSet("p"+p+"quickdrawTimeout", trTimeMS() + quickdrawDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
	}
	
	if (xGetDatabaseCount(harpies) > 0) {
		for (x=xGetDatabaseCount(harpies); >0) {
			xDatabaseNext(harpies);
			xUnitSelect(harpies, xUnitName);
			trMutateSelected(kbGetProtoUnitID("Harpy"));
			trUnitOverrideAnimation(1,0,false,false,-1);
		}
		xClearDatabase(harpies);
	}
	
	if (xGetDatabaseCount(echoBombs) > 0) {
		xDatabaseNext(echoBombs);
		hit = 0;
		if (xGetInt(echoBombs, xUnitName) > 0) {
			target = xGetInt(echoBombs, xEchoBombUnit);
			xUnitSelect(echoBombs, xEchoBombUnit);
			if (trUnitAlive() == false) {
				hit = 1;
				amt = xGetFloat(echoBombs, xEchoBombStartingHealth);
			} else {
				pos = kbGetBlockPosition(""+target);
				xSetVector(echoBombs, xEchoBombPos, pos);
				if (PvP) {
					xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner,
							xGetInt(dEnemies, xDoppelganger, xGetInt(echoBombs, xEchoBombIndex))));
				} else {
					xsSetContextPlayer(ENEMY_PLAYER);
				}
				amt = kbUnitGetCurrentHitpoints(kbGetBlockID(""+target));
				xsSetContextPlayer(0);
				if (trTimeMS() > xGetInt(echoBombs, xEchoBombTimeout)) {
					amt = xGetFloat(echoBombs, xEchoBombStartingHealth) - amt;
					hit = 1;
					if (xSetPointer(dEnemies, xGetInt(echoBombs, xEchoBombIndex))) {
						xSetBool(dEnemies, xEchoBomb, false);
					}
				} else if (amt < xGetFloat(echoBombs, xEchoBombCurrentHealth)) {
					xSetFloat(echoBombs, xEchoBombSize,
						xGetFloat(echoBombs, xEchoBombSize) + 0.002 * (xGetFloat(echoBombs, xEchoBombCurrentHealth) - amt));
					xSetFloat(echoBombs, xEchoBombCurrentHealth, amt);
					amt = xsSqrt(xGetFloat(echoBombs, xEchoBombSize));
					xUnitSelect(echoBombs, xUnitName);
					trSetSelectedScale(amt, amt, amt);
					trUnitHighlight(0.2, false);
				}
			}
			if (hit == 1) {
				pos = xGetVector(echoBombs, xEchoBombPos);
				amt = amt * xGetFloat(dPlayerData, xPlayerSpellDamage);
				dist = xsPow(echoBombRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						damageEnemy(p, amt);
					}
				}
				
				if (amt < 1000) {
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
				} else {
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
					trCameraShake(0.5, 0.3);
				}
				xUnitSelect(echoBombs, xUnitName);
				if ((xGetInt(echoBombs, xEchoBombUnit) == bossUnit) && (trQuestVarGet("stage") >= 9)) {
					trSetSelectedScale(1,1,1);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				} else {
					trUnitDestroy();
				}
				
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				xAddDatabaseBlock(harpies, true);
				xSetInt(harpies, xUnitName, 1*trQuestVarGet("next"));
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitChangeProtoUnit("Kronny Flying");
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trSetSelectedScale(0,-5.0,0);
				trDamageUnitPercent(100);
				
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Meteor Impact Ground");
				xFreeDatabaseBlock(echoBombs);
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = xsPow(12, 2);
		target = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			id = xGetInt(dEnemies, xUnitID);
			trUnitSelectClear();
			trUnitSelectByID(id);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
				if (current < dist) {
					hit = id;
					target = xGetPointer(dEnemies);
					dist = current;
				}
			}
		}
		if (target > 0 && xSetPointer(dEnemies, target) && (xGetBool(dEnemies, xEchoBomb) == false)) {
			
			xSetBool(dEnemies, xEchoBomb, true);
			xAddDatabaseBlock(echoBombs, true);
			xSetInt(echoBombs, xEchoBombUnit, xGetInt(dEnemies, xUnitName));
			if ((xGetInt(dEnemies, xUnitName) == bossUnit) && (trQuestVarGet("stage") >= 9)) {
				// can't cast spy on the titan atlantean
				xSetInt(echoBombs, xUnitName, aiPlanGetUserVariableInt(ARRAYS, bossInts, 2));
				xUnitSelect(echoBombs, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
			} else {
				spyEffect(xGetInt(dEnemies, xUnitName), kbGetProtoUnitID("Phoenix Egg"),
					xsVectorSet(echoBombs, xUnitName, xGetPointer(echoBombs)));
			}
			if (PvP) {
				xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner, xGetInt(dEnemies, xDoppelganger)));
			} else {
				xsSetContextPlayer(ENEMY_PLAYER);
			}
			amt = kbUnitGetCurrentHitpoints(hit);
			xsSetContextPlayer(0);
			xSetFloat(echoBombs, xEchoBombStartingHealth, amt);
			xSetFloat(echoBombs, xEchoBombCurrentHealth, amt);
			xSetFloat(echoBombs, xEchoBombSize, 1.0);
			xSetVector(echoBombs, xEchoBombPos, xGetVector(dEnemies, xUnitPos));
			xSetInt(echoBombs, xEchoBombIndex, target);
			xSetInt(echoBombs, xEchoBombTimeout,
				trTimeMS() + 1000 * echoBombDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
			trSoundPlayFN("siegeselect.wav", "1", -1, "","");
			trSoundPlayFN("gatherpoint.wav","1",-1,"","");
			
			//plantEchoBomb(p);
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				if (target > 0) {
					trChatSend(0, "An enemy can only have one Echo Bomb on it at a time!");
				} else {
					trChatSend(0, "You must target an enemy with this ability!");
				}
				trCounterAbort("lure");
			}
			xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST);
		}
	}

	if (trQuestVarGet("p"+p+"bulletStormStart") == 1) {
		if (trQuestVarGet("spyFind") == trQuestVarGet("spyFound")) {
			for(i=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xCommandoBulletStormSFX);
				trMutateSelected(kbGetProtoUnitID("Sphinx"));
				trUnitOverrideAnimation(39,0,true,false,-1);
			}
			trQuestVarSet("p"+p+"bulletStormStart", 0);
		}
	}

	if (trQuestVarGet("p"+p+"bulletStormActive") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"bulletStormNext")) {
			//trQuestVarSet("p"+p+"bulletStormShoot", 1 - trQuestVarGet("p"+p+"bulletStormShoot"));
			
			trQuestVarSet("p"+p+"bulletStormNext", trQuestVarGet("p"+p+"bulletStormNext") + 100);
			//if (trQuestVarGet("p"+p+"bulletStormShoot") == 1) {
				gainFavor(p, 0.0 - 1.0 * xGetFloat(dPlayerData, xPlayerUltimateCost));
			//}
			dir = rotationMatrix(trVectorQuestVarGet("p"+p+"bulletStormDir"), -0.740544, -0.672008);
			dist = xsPow(bulletStormRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);

			hit = xGetPointer(dEnemies);
			for(i=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeCommando(p);
				} else {
					trSetUnitOrientation(dir, vector(0,1,0), true);
					pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					shootBullet(p, pos, pos + dir * bulletStormRange * xGetFloat(dPlayerData, xPlayerSpellRange));
					if (xGetBool(db, xCommandoBulletStormShoot)) {
						xSetInt(db, xCharSpecialAttack, xGetInt(db, xCharSpecialAttack) - 1);
						xSetPointer(dEnemies, xGetInt(db, xCommandoBulletStormTargetIndex));
						for(j=xGetDatabaseCount(dEnemies); > 0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else {
								end = kbGetBlockPosition("" + xGetInt(dEnemies, xUnitName), true);
								if (distanceBetweenVectors(pos, end, true) < dist) {
									damageEnemy(p, commandoCritMultiplier(p) * xGetFloat(dPlayerData, xPlayerAttack), false);
									OnHit(p, xGetPointer(dEnemies));
									commandoSmite(p);
									if (xGetInt(db, xCharSpecialAttack) <= 0) {
										placeCommandoPad(p, kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true));
										xSetInt(db, xCharSpecialAttack, xsMax(xGetInt(dPlayerData,xPlayerSpecialAttackCooldown,p), xGetInt(dClass, xClassSpecialAttackCooldown, xGetInt(dPlayerData, xPlayerClass)) / 2));
									}
									trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(end),0,xsVectorGetZ(end),0,true);
									trArmySelect("1,0");
									trUnitChangeProtoUnit("Dust Small");
									if (xGetInt(db, xCommandoBulletStormTargetIndex) == xGetPointer(dEnemies)) {
										xSetBool(db, xCommandoBulletStormShoot, false); // when there's only one enemy, halve the number of hits.
									} else {
										xSetInt(db, xCommandoBulletStormTargetIndex, xGetPointer(dEnemies));
									}
									break;
								}
							}
						}
					} else {
						xSetBool(db, xCommandoBulletStormShoot, true);
					}
				}
			}
			xSetPointer(dEnemies, hit);
			trQuestVarSet("bulletSound", 1);
			trVectorQuestVarSet("p"+p+"bulletStormDir", dir);
			if ((trPlayerResourceCount(p, "favor") * xGetDatabaseCount(db)) == 0) {
				minigunOff(p);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"bulletStormActive", 1 - trQuestVarGet("p"+p+"bulletStormActive"));
		if (trQuestVarGet("p"+p+"bulletStormActive") == 1) {
			if (trPlayerResourceCount(p, "favor") == 0) {
				trQuestVarSet("p"+p+"bulletStormActive", 0);
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
					trChatSend(0, "Not enough favor to activate Bullet Storm!");
				}
			} else {
				trSoundPlayFN("storehouse.wav","1",-1,"","");
				trQuestVarSet("p"+p+"bulletStormStart", 1);
				trModifyProtounit("Royal Guard Hero", p, 55, 2);
				trQuestVarSet("p"+p+"bulletStormNext", trTimeMS() + 100);
				trQuestVarSet("p"+p+"bulletStormShoot", 1);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeCommando(p);
					} else if (xGetInt(db, xCommandoBulletStormSFX) == 0) {
						trMutateSelected(kbGetProtoUnitID("Royal Guard Hero"));
						spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db,xCommandoBulletStormSFX,xGetPointer(db)));
					}
				}
			}
		} else {
			minigunOff(p);
		}
	}

	if (trQuestVarGet("bulletSound") == 1) {
		trQuestVarSet("bulletSound", 0);
		trSoundPlayFN("titanfall.wav");
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseCommando(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * COMMANDO;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Quickdraw";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Shooting Gallery";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Echo Bomb";
		lureIsUltimate = false;
	}
	
	xCommandoBulletStormSFX = xInitAddInt(db, "bulletStormSFX");
	xCommandoQuickdrawSFX = xInitAddInt(db, "quickdrawSFX");
	xCommandoBulletStormTargetIndex = xInitAddInt(db, "bulletStormTargetIndex");
	xCommandoBulletStormShoot = xInitAddBool(db, "shootingTime", true);

	xSetInt(dPlayerData,xPlayerWellCooldown, quickdrawCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, echoBombCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"fireharpies") == 0) {
		db = xInitDatabase("p"+p+"fireharpies");
		trQuestVarSet("p"+p+"fireharpies", db);
		xInitAddInt(db, "name");
	}
	
	if (trQuestVarGet("p"+p+"echoBombs") == 0) {
		db = xInitDatabase("p"+p+"echoBombs");
		trQuestVarSet("p"+p+"echoBombs", db);
		xInitAddInt(db, "name");
		xEchoBombUnit = xInitAddInt(db, "unit");
		xEchoBombStartingHealth = xInitAddFloat(db, "startingHealth");
		xEchoBombCurrentHealth = xInitAddFloat(db, "currentHealth");
		xEchoBombSize = xInitAddFloat(db, "size");
		xEchoBombPos = xInitAddVector(db, "pos");
		xEchoBombIndex = xInitAddInt(db, "index");
		xEchoBombTimeout = xInitAddInt(db, "timeout");
	}

	if (trQuestVarGet("p"+p+"quickdrawTargets") == 0) {
		db = xInitDatabase("p"+p+"quickdrawTargets");
		trQuestVarSet("p"+p+"quickdrawTargets", db);
		xInitAddInt(db, "name");
		xQuickdrawIndex = xInitAddInt(db, "index");
	}

	if (trQuestVarGet("p"+p+"quickdrawTargetsIncoming") == 0) {
		db = xInitDatabase("p"+p+"quickdrawTargetsIncoming");
		trQuestVarSet("p"+p+"quickdrawTargetsIncoming", db);
		xInitAddInt(db, "name");
		xQuickdrawIndex = xInitAddInt(db, "index");
	}

	if (trQuestVarGet("p"+p+"commandoPads") == 0) {
		db = xInitDatabase("p"+p+"commandoPads");
		trQuestVarSet("p"+p+"commandoPads", db);
		xCommandoPadID = xInitAddInt(db, "name");
		xCommandoPadPos = xInitAddVector(db, "pos");
		xCommandoPadSourcePos = xInitAddVector(db, "sourcePos");
		xCommandoPadTimeout = xInitAddInt(db, "timeout");
	}

	trVectorQuestVarSet("p"+p+"bulletStormDir", vector(1,0,0));
}


rule commando_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * COMMANDO + p, "commandoAlways");
		trEventSetHandler(1000 + 12 * COMMANDO + p, "chooseCommando");
	}
}
