int sunlightCooldown = 18;
float sunlightRadius = 6;
float sunlightDuration = 6;
float sunlightPower = 60;

int smitingRaysCooldown = 12;
float smitingRaysPower = 60;
float smitingRaysDuration = 9;
float smitingRaysRange = 16;

int xSunlightRadius = 0;
int xSunlightDamage = 0;
int xSunlightTimeout = 0;
int xSunlightStart = 0;
int xSunlightEnd = 0;
int xSunlightPos = 0;
int xSunlightNext = 0;

int xSmitingLast = 0;
int xSmitingTimeout = 0;
int xSmitingHeal = 0;
int xSmitingDB = 0;
int xSmitingIndex = 0;
int xSmitingSFX = 0;

int lightwingDamage = 240;
int lightwingDuration = 15;
int lightwingCost = 4;

void removeSunbow(int p = 0) {
	if (trQuestVarGet("p"+p+"lightwing") == 1) {
		int db = getCharactersDB(p);
		if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex)) == false) {
			debugLog("Cannot restore p" + p + " sunbow");
		}
	}
	removePlayerSpecific(p);
}

void lightwingOff(int p = 0) {
	int db = getCharactersDB(p);
	int sunlights = trQuestVarGet("p"+p+"sunlights");
	trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
	xSetBool(dPlayerData, xPlayerLaunched, false);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeSunbow(p);
		} else {
			xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex));
			trUnitChangeProtoUnit("Hero Greek Hippolyta");
		}
	}
	trQuestVarSet("p"+p+"lightwing",0);
	equipRelicsAgain(p);
	xSetInt(dPlayerData, xPlayerFirstDelay, xGetInt(dClass, xClassFirstDelay, SUNBOW));
	xSetInt(dPlayerData, xPlayerNextDelay, xGetInt(dClass, xClassNextDelay, SUNBOW));

	for(x=xGetDatabaseCount(sunlights); >0) {
		xDatabaseNext(sunlights);
		for(y=xGetInt(sunlights, xSunlightStart); < xGetInt(sunlights, xSunlightEnd)) {
			trUnitSelectClear();
			trUnitSelect(""+y, true);
			trUnitChangeProtoUnit("Relic");
			trUnitSelectClear();
			trUnitSelect(""+y, true);
			trMutateSelected(kbGetProtoUnitID("Hero Birth"));
		}
	}
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
	int relics = getRelicsDB(p);
	int sunlights = trQuestVarGet("p"+p+"sunlights");
	int smiting = trQuestVarGet("p"+p+"smitingTargets");
	float amt = 0;
	float dist = 0;
	float current = 0;
	bool alternate = true;
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
			if (hit >= ON_HIT_NORMAL) {
				if (trQuestVarGet("p"+p+"lightwing") == 1) {
					amt = lightwingDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
					dist = 5.0 * xGetFloat(dPlayerData, xPlayerSpellRange);
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName, xGetInt(db, xCharAttackTargetIndex)));
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive()) {
							if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
								damageEnemy(p, amt);
							}
						}
					}
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		if (trQuestVarGet("p"+p+"lightwing") == 0) {
			trSoundPlayFN("restorationbirth.wav","1",-1,"","");
		} else {
			trSoundPlayFN("forestfirebirth.wav","1",-1,"","");
		}
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
			if ((trQuestVarGet("p"+p+"lightwing") == 1) && alternate) {
				trMutateSelected(kbGetProtoUnitID("Ball of Fire Impact"));
			} else {
				trMutateSelected(kbGetProtoUnitID("Hero Birth"));
			}
			alternate = (alternate == false);
			dir = rotationMatrix(dir, 0.92388, 0.382683); // rotate 22.5 degrees
		}
		xSetInt(sunlights, xSunlightEnd, trGetNextUnitScenarioNameNumber());
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		pos = xGetVector(dPlayerData, xPlayerLurePos);

		dist = 25;
		target = 0;
		if (trQuestVarGet("p"+p+"lightwing") == 1) {
			hit = dEnemies;
		} else {
			hit = dPlayerCharacters;
		}
		for(x=xGetDatabaseCount(hit); >0) {
			xDatabaseNext(hit);
			xUnitSelectByID(hit, xUnitID);
			if (trUnitAlive()) {
				current = unitDistanceToVector(xGetInt(hit, xUnitName), pos);
				if (current < dist) {
					dist = current;
					target = xGetPointer(hit);
					id = xGetInt(hit, xUnitName);
				}
			}
		}
		if (target == 0) {
			xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST);
			if (trCurrentPlayer() == p) {
				trCounterAbort("lure");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				if (trQuestVarGet("p"+p+"lightwing") == 1) {
					trChatSend(0, "You must target an enemy with this ability when in Lightwing form!");
				} else {
					trChatSend(0, "You must target an allied player with this ability!");
				}
			}
		} else {
			trSoundPlayFN("firegiantdie.wav","1",-1,"","");
			xAddDatabaseBlock(smiting, true);
			xSetInt(smiting, xUnitName, id);
			xSetInt(smiting, xSmitingLast, trTimeMS());
			xSetInt(smiting, xSmitingTimeout, trTimeMS() + 1000 * smitingRaysDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
			xSetFloat(smiting, xSmitingHeal, smitingRaysPower * xGetFloat(dPlayerData, xPlayerSpellDamage));
			if (trQuestVarGet("p"+p+"lightwing") == 1) {
				xSetInt(smiting, xSmitingDB, dEnemies);
				xSetInt(smiting, xSmitingIndex, target);
				spyEffect(id,kbGetProtoUnitID("Ball of Fire Impact"),xsVectorSet(smiting,xSmitingSFX,xGetNewestPointer(smiting)));
			} else {
				spyEffect(id,kbGetProtoUnitID("Curse SFX"),xsVectorSet(smiting,xSmitingSFX,xGetNewestPointer(smiting)));
				next = xGetInt(dPlayerCharacters, xPlayerOwner, target);
				hit = getCharactersDB(next);
				xSetInt(smiting, xSmitingDB, hit);
				for(x=xGetDatabaseCount(hit); >0) {
					xDatabaseNext(hit);
					if (xGetInt(hit, xUnitName) == id) {
						xSetInt(smiting, xSmitingIndex, xGetPointer(hit));
						xSetFloat(hit, xCharSmiteDamage, xGetFloat(hit, xCharSmiteDamage) + xGetFloat(smiting, xSmitingHeal));
						break;
					}
				}
				if (trCurrentPlayer() == next) {
					trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has granted you laser attacks!");
				}
			}
		}
	}

	if (xGetDatabaseCount(smiting) > 0) {
		xDatabaseNext(smiting);
		if (trTimeMS() > xGetInt(smiting, xSmitingLast)) {
			xSetInt(smiting, xSmitingLast, xGetInt(smiting, xSmitingLast) + 500);
			xUnitSelect(smiting, xUnitName);
			if (trUnitAlive() == false) {
				xFreeDatabaseBlock(smiting);
			} else {
				if (xGetInt(smiting, xSmitingDB) == dEnemies) {
					xSetPointer(dEnemies, xGetInt(smiting, xSmitingIndex));
					amt = xGetFloat(smiting, xSmitingHeal) * xGetFloat(dPlayerData, xPlayerHealBoost) * 0.5;
					if (trQuestVarGet("p"+p+"lightwing") == 1) {
						hit = 1;
						dist = xsPow(xGetFloat(dPlayerData, xPlayerRange), 2);
						pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName)));
						for(x=xGetDatabaseCount(db); >0) {
							xDatabaseNext(db);
							xUnitSelectByID(db, xUnitID);
							if (trUnitAlive() == false) {
								removeSunbow(p);
							} else if (unitDistanceToVector(xGetInt(db, xUnitName), pos) < dist) {
								hit = hit + 1;
								end = kbGetBlockPosition(""+xGetInt(db, xUnitName)) + vector(0,1,0);
								dir = getUnitVector3d(end, pos);
								next = trGetNextUnitScenarioNameNumber();
								trArmyDispatch("1,0", "Dwarf", 1, xsVectorGetX(pos), 0, xsVectorGetZ(pos), 0, true);
								trArmySelect("1,0");
								trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
								trSetUnitOrientation(dir, xsVectorNormalize(xsVectorSet(xsVectorGetZ(dir),0,0.0 - xsVectorGetX(dir))),true);
								trUnitHighlight(2.0, false);
								xAddDatabaseBlock(dPlayerLasers, true);
								xSetInt(dPlayerLasers, xUnitName, next);
								xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 500);
								xSetFloat(dPlayerLasers, xPlayerLaserRange, distanceBetweenVectors3d(pos, end, false) * 1.4);
							}
						}
						xUnitSelect(dEnemies, xUnitName);
						damageEnemy(p, amt * hit);
						if (hit > 1) {
							if (trUnitVisToPlayer()) {
								trSoundPlayFN("pegasusflap.wav","1",-1,"","");
							}
							for(i=hit; >1) {
								OnHit(p, xGetInt(smiting, xSmitingIndex), true);
							}
						}
					}
				} else {
					xSetPointer(dPlayerUnits, xGetInt(smiting, xSmitingIndex));
					healUnit(p, xGetFloat(smiting, xSmitingHeal) * 0.5);
				}
				if (trTimeMS() > xGetInt(smiting, xSmitingTimeout)) {
					xUnitSelect(smiting, xSmitingSFX);
					trUnitDestroy();
					hit = xGetInt(smiting, xSmitingDB);
					if (hit != dEnemies) {
						xSetFloat(hit,xCharSmiteDamage, xGetFloat(hit,xCharSmiteDamage) - xGetFloat(smiting, xSmitingHeal));
					}
					xFreeDatabaseBlock(smiting);
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"lightwing") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"lightwingNext")) {
			gainFavor(p, -1.0);
			amt = 1000.0 / trQuestVarGet("p"+p+"lightwingCost");
			trQuestVarSet("p"+p+"lightwingNext", trQuestVarGet("p"+p+"lightwingNext") + amt);
			// lightwing cost increases by 0.5 favor per second
			trQuestVarSet("p"+p+"lightwingCost", trQuestVarGet("p"+p+"lightwingCost") + 0.0005 * amt * xGetFloat(dPlayerData, xPlayerUltimateCost));
			if (trPlayerResourceCount(p, "favor") == 0) {
				lightwingOff(p);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"lightwing", 1 - trQuestVarGet("p"+p+"lightwing"));
		if (trQuestVarGet("p"+p+"lightwing") == 1) {
			trSoundPlayFN("cinematics\32_out\doorseal.mp3","1",-1,"","");
			trQuestVarSet("p"+p+"lightwingCost", lightwingCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
			xSetBool(dPlayerData, xPlayerLaunched, true);
			for(x=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				xUnitSelect(relics, xUnitName);
				trUnitChangeProtoUnit("Cinematic Block");
			}
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeSunbow(p);
				} else {
					trMutateSelected(kbGetProtoUnitID("Phoenix From Egg"));
					target = xGetInt(db, xCharIndex);

					xSetInt(dPlayerUnits, xPoisonStatus, 0);
					xSetInt(dPlayerUnits, xSilenceStatus, 0);
					if (xGetInt(dPlayerUnits, xStunStatus, target) > 0) {
						xFreeDatabaseBlock(dStunnedUnits, xGetInt(dPlayerUnits, xStunStatus, target));
						xSetInt(dPlayerUnits, xStunStatus, 0);
					}

					if (xGetBool(dPlayerUnits, xLaunched, target)) {
						for(y=xGetDatabaseCount(dLaunchedUnits); >0) {
							xDatabaseNext(dLaunchedUnits);
							if (xGetInt(dLaunchedUnits, xUnitName) == xGetInt(dPlayerUnits, xUnitName, target)) {
								xFreeDatabaseBlock(dLaunchedUnits);
							}
						}
						xSetBool(dPlayerUnits, xLaunched, false, target);
					}
					
					for(i=xStunSFX; < xSilenceSFX) {
						trUnitSelectClear();
						trUnitSelect(""+xGetInt(dPlayerUnits, i), true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
					xDetachDatabaseBlock(dPlayerUnits, target);
				}
			}

			for(x=xGetDatabaseCount(sunlights); >0) {
				xDatabaseNext(sunlights);
				for(y=xGetInt(sunlights, xSunlightStart); < xGetInt(sunlights, xSunlightEnd)) {
					if (kbGetBlockID(""+y) >= 0) {
						if (alternate) {
							trUnitSelectClear();
							trUnitSelect(""+y, true);
							trMutateSelected(kbGetProtoUnitID("Ball of Fire Impact"));
						}
						alternate = (alternate == false);
					}
				}
			}

			xSetInt(dPlayerData, xPlayerFirstDelay, 2000);
			xSetInt(dPlayerData, xPlayerNextDelay, 2700);
			trQuestVarSet("p"+p+"lightwingNext", trTimeMS());
		} else {
			lightwingOff(p);
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
			
			if (trQuestVarGet("p"+p+"lightwing") == 1) {
				amt = amt * xGetFloat(dPlayerData, xPlayerHealBoost);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						damageEnemy(p, amt);
					}
				}
			} else {
				for (x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
						healUnit(p, amt);
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
		rainName = "(E) Lightwing";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Smiting Rays";
		lureIsUltimate = false;
	}
	xSetInt(dPlayerData,xPlayerWellCooldown, sunlightCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, smitingRaysCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost,0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, lightwingCost);
	
	
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

	if (trQuestVarGet("p"+p+"smitingTargets") == 0) {
		db = xInitDatabase("p"+p+"smitingTargets");
		trQuestVarSet("p"+p+"smitingTargets", db);
		xInitAddInt(db, "name"); // name of the target
		xSmitingLast = xInitAddInt(db, "last");
		xSmitingTimeout = xInitAddInt(db, "timeout");
		xSmitingHeal = xInitAddFloat(db, "heal");
		xSmitingDB = xInitAddInt(db, "database");
		xSmitingIndex = xInitAddInt(db, "index");
		xSmitingSFX = xInitAddInt(db, "sfx");
	}
}

void modifySunbow(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * SUNBOW;
	zSetProtoUnitStat("Phoenix From Egg", p, 1, xGetFloat(dPlayerData, xPlayerSpeed) + 2.0);
	zSetProtoUnitStat("Phoenix From Egg", p, 0, xGetFloat(dPlayerData, xPlayerHealth));
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
