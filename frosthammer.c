int icicleCooldown = 5;
float icicleDuration = 8;
float icicleDamage = 80;
float icicleRadius = 2;
float icicleHealRange = 5;
float icicleRange = 4;

int xIciclePos = 0;
int xIcicleTimeout = 0;
int xIcicleLast = 0;
int xIcicleStep = 0;
int xIcicleRadius = 0;

int titanicImpactCooldown = 15;
float smashRange = 6;
float smashWidth = 2;

int xSmashAngle = 0;
int xSmashVelocity = 0;
int xSmashScale = 0;
int xSmashDir = 0;
int xSmashTimeout = 0;

int blizzardCost = 80;
float blizzardDuration = 8;
float blizzardRadius = 10;

int xBlizzardSFX = 0;

void removeFrostHammer(int p = 0) {
	removePlayerSpecific(p);
	
}

void frostHammerGrow(int p = 0) {
	float amt = 10;
	if (xGetInt(dPlayerData, xPlayerGodBoon, p) == BOON_SPELL_ATTACK) {
		amt = amt * xGetFloat(dPlayerData, xPlayerSpellDamage, p);
	}
	trQuestVarSet("p"+p+"frostHammerBonus", trQuestVarGet("p"+p+"frostHammerBonus") + amt);
}

void frosthammerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * FROSTHAMMER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int icicles = trQuestVarGet("p"+p+"icicles");
	int smashes = trQuestVarGet("p"+p+"titanicSmashes");
	float amt = 0;
	float dist = 0;
	float current = 0;
	float radius = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);

	float scale = xsSqrt(1.0 + 0.03 * trQuestVarGet("p"+p+"frostHammerBonus"));

	float tdiff = 0.001 * (trTimeMS() - trQuestVarGet("p"+p+"frostHammerLast"));
	trQuestVarSet("p"+p+"frostHammerLast", trTimeMS());

	trQuestVarSet("p"+p+"frostHammerBonus", trQuestVarGet("p"+p+"frostHammerBonus") - tdiff * xsMax(1.0, 0.03 * trQuestVarGet("p"+p+"frostHammerBonus")));
	if (trQuestVarGet("p"+p+"frostHammerBonus") < 0) {
		trQuestVarSet("p"+p+"frostHammerBonus", 0);
	}
	xSetFloat(dPlayerData, xPlayerAttack, xGetFloat(dPlayerData, xPlayerBaseAttack) + trQuestVarGet("p"+p+"frostHammerBonus"));
	zSetProtoUnitStat("Hero Norse", p, 27, xGetFloat(dPlayerData, xPlayerAttack));

	for (x=xsMin(3, xGetDatabaseCount(icicles)); > 0) {
		xDatabaseNext(icicles);
		if (xGetBool(icicles, xIcicleStep)) {
			amt = (1.0 * xGetInt(icicles, xIcicleTimeout) - trTimeMS()) / icicleDuration / 1000;
			amt = 1.0 + 3.0 * amt;
			xUnitSelect(icicles, xUnitName);
			if (amt <= 1.0) {
				trUnitSelect(""+(xGetInt(icicles, xUnitName) + 1), true);
				trUnitDestroy();
				xFreeDatabaseBlock(icicles);
			} else {
				trSetSelectedScale(xGetFloat(icicles, xIcicleRadius),amt,xGetFloat(icicles, xIcicleRadius));
				amt = trTimeMS() - xGetInt(icicles, xIcicleLast);
				if (amt > 500) {
					xSetInt(icicles, xIcicleLast, trTimeMS());
					amt = amt * 0.00001 * xGetFloat(dPlayerData, xPlayerHealth);
					dist = icicleHealRange * xGetFloat(icicles, xIcicleRadius);
					pos = xGetVector(icicles, xIciclePos);
					for(y=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db, xUnitID);
						if (trUnitAlive() == false) {
							removeFrostHammer();
						} else if (unitDistanceToVector(xGetInt(db, xUnitName), pos) < dist) {
							healUnit(p, amt);
						}
					}
				}
			}
		} else {
			xUnitSelect(icicles, xUnitName);
			amt = (150.0 - xGetInt(icicles, xIcicleTimeout) + trTimeMS()) / 150.0;
			if (amt >= 1.0) {
				xSetBool(icicles, xIcicleStep, true);
				xSetInt(icicles, xIcicleTimeout, xGetInt(icicles, xIcicleTimeout) + icicleDuration * xGetFloat(dPlayerData, xPlayerSpellDuration) * 1000);
				xSetInt(icicles, xIcicleLast, trTimeMS());
				amt = 1.0;
			}
			amt = 1.0 + 3.0 * amt;
			trSetSelectedScale(xGetFloat(icicles, xIcicleRadius),amt,xGetFloat(icicles, xIcicleRadius));
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("piercemetal"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		trQuestVarSetFromRand("heading", 0, 360, true);
		next = trGetNextUnitScenarioNameNumber();
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		trArmyDispatch(""+p+",0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
		trUnitSelectClear();
		trUnitSelect(""+(next+1),true);
		trMutateSelected(kbGetProtoUnitID("Frost Drift"));
		trUnitOverrideAnimation(2,0,true,true,-1);
		trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));

		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Ice Block"));
		trUnitSetAnimationPath("0,0,0,0,0,0,0");
		trSetSelectedScale(icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange),0,icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange));

		dist = xsPow(icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		amt = dist;
		hit = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
				if (current < dist) {
					hit = hit + 1;
					damageEnemy(p, icicleDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));
					if (current < amt) {
						amt = current;
						target = xGetPointer(dEnemies);
					}
				}
			}
		}
		if (hit >= 1) {
			gainFavor(p, hit);
			xSetPointer(dEnemies, target);
			stunUnit(dEnemies, 2, p);
			frostHammerGrow(p);
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}

		xAddDatabaseBlock(icicles, true);
		xSetInt(icicles, xUnitName, next);
		xSetInt(icicles, xIcicleTimeout, trTimeMS() + 150);
		xSetFloat(icicles, xIcicleRadius, xGetFloat(dPlayerData, xPlayerSpellRange));
		xSetVector(icicles, xIciclePos, pos);
	}

	for(x=xGetDatabaseCount(smashes); >0) {
		xDatabaseNext(smashes);
		xUnitSelect(smashes, xUnitName);
		if (xGetInt(smashes, xSmashTimeout) == 0) {
			xSetFloat(smashes, xSmashVelocity, xGetFloat(smashes, xSmashVelocity) - tdiff * 5.0);
			xSetFloat(smashes, xSmashAngle, xGetFloat(smashes, xSmashAngle) + xGetFloat(smashes, xSmashVelocity) * tdiff);
			dir = xGetVector(smashes, xSmashDir);
			amt = xGetFloat(smashes, xSmashAngle);
			trSetUnitOrientation(dir * xsSin(amt) - xsVectorSet(0,xsCos(amt),0), dir * xsCos(amt) + xsVectorSet(0,xsSin(amt),0), true);
			if (xGetFloat(smashes, xSmashAngle) < 0.2) {
				dir = xGetVector(smashes, xSmashDir);
				start = kbGetBlockPosition(""+xGetInt(smashes, xUnitName));
				xSetInt(smashes, xSmashTimeout, trTimeMS() + 1000);
				trUnitHighlight(0.2, false);
				amt = xGetFloat(smashes, xSmashScale);
				dist = amt * smashRange;
				current = xsPow(amt * smashWidth, 2);
				amt = amt * 0.003 * xGetFloat(dPlayerData, xPlayerHealth) * xGetFloat(dPlayerData, xPlayerAttack);
				for(y=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision(dEnemies,start,dir,dist,current)) {
						damageEnemy(p, amt);
						gainFavor(p, 1.0);
					}
				}
				radius = xsPow(icicleHealRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				trQuestVarSet("sound", 0);
				for(y=xGetDatabaseCount(icicles); >0) {
					xDatabaseNext(icicles);
					if (rayCollision(icicles,start,dir,dist,current)) {
						trQuestVarSetFromRand("sound", 1, 3, true);
						trUnitSelectClear();
						trUnitSelect(""+(1+xGetInt(icicles, xUnitName)), true);
						trUnitDestroy();
						xUnitSelect(icicles, xUnitName);
						trDamageUnitPercent(100);
						trUnitChangeProtoUnit("Implode Sphere Effect");
						pos = xGetVector(icicles, xIciclePos);
						for(z=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < radius) {
								damageEnemy(p, amt * 0.2);
							}
						}
						xFreeDatabaseBlock(icicles);
					}
				}
				if (trQuestVarGet("sound") > 0) {
					trSoundPlayFN("crushmetal"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
				for(y=1 + xGetFloat(smashes, xSmashScale) * smashRange / 2; >0) {
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Dust Large");
					start = start + dir * 2.0;
				}
				amt = xGetFloat(smashes, xSmashScale);
				trCameraShake(0.5, amt * 0.25);
				if (amt > 2.0) {
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
				} else if (amt > 1.5) {
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
				} else {
					trSoundPlayFN("meteorsmallhit.wav","1",-1,"","");
				}
			}
		} else if (trTimeMS() > xGetInt(smashes, xSmashTimeout)) {
			trUnitDestroy();
			xFreeDatabaseBlock(smashes);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		pos = xGetVector(dPlayerData, xPlayerLurePos);

		trSoundPlayFN("cinematics\32_out\hammerconnect.mp3","1",-1,"","");

		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeFrostHammer();
			} else {
				start = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(db, xUnitName), true));
				dir = getUnitVector(start, pos);
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trUnitChangeProtoUnit("Spy Eye");
				trUnitSelectClear();
				trUnitSelect(""+next, true);
				trMutateSelected(kbGetProtoUnitID("Thor Hammer"));
				trUnitSetAnimationPath("0,0,0,0,0,0,0");
				trSetUnitOrientation(dir, vector(0,1,0), true);
				trSetSelectedScale(scale, scale, scale);
				xAddDatabaseBlock(smashes, true);
				xSetInt(smashes, xUnitName, next);
				xSetFloat(smashes, xSmashScale, scale);
				xSetVector(smashes, xSmashDir, dir);
			}
		}
	}

	if (trQuestVarGet("p"+p+"blizzard") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"blizzardNext")) {
			trQuestVarSet("p"+p+"blizzardNext", trQuestVarGet("p"+p+"blizzardNext") + 500 / xGetDatabaseCount(db));
			pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			if (xGetDatabaseCount(dEnemies) > 0) {
				dist = xsPow(blizzardRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				bool stunActive = true;
				xSetPointer(dEnemies, 1 * trQuestVarGet("p"+p+"blizzardPointer"));
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						silenceUnit(dEnemies, 1.0, p);
						if (stunActive) {
							trQuestVarSet("p"+p+"blizzardPointer", xGetPointer(dEnemies));
							stunUnit(dEnemies, 3.0, p);
							frostHammerGrow(p);
							stunActive = false;
						}
					}
				}
			}
			if (trTimeMS() > trQuestVarGet("p"+p+"blizzardTimeout")) {
				trQuestVarSet("p"+p+"blizzard", 0);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeFrostHammer();
					} else {
						xUnitSelect(db, xBlizzardSFX);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		gainFavor(p, 0.0 - blizzardCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trQuestVarSet("p"+p+"blizzard", 1);
		trQuestVarSet("p"+p+"blizzardNext", trTimeMS() + 1000);
		trQuestVarSet("p"+p+"blizzardTimeout", trTimeMS() + 1000 * blizzardDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeFrostHammer();
			} else if (kbGetBlockID(""+xGetInt(db, xBlizzardSFX)) == -1) {
				spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Ice Sheet"),xsVectorSet(db,xBlizzardSFX,xGetPointer(db)));
			} else {
				xUnitSelect(db, xBlizzardSFX);
				trMutateSelected(kbGetProtoUnitID("Ice Sheet"));
			}
		}
		trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
	}
	
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeFrostHammer(p);
		} else {
			trSetSelectedScale(scale, scale, scale);
			hit = CheckOnHit(p);
			if (hit == ON_HIT_SPECIAL) {
				xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex));
				stunUnit(dEnemies, 3.0, p);
				frostHammerGrow(p);
			}
		}
	}
	
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseFrostHammer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * FROSTHAMMER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Icicle";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Blizzard";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Titanic Impact";
		lureIsUltimate = false;
	}
	xBlizzardSFX = xInitAddInt(db, "blizzardSFX", -1);
	
	xSetInt(dPlayerData,xPlayerWellCooldown, icicleCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, titanicImpactCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, blizzardCost);

	if (trQuestVarGet("p"+p+"icicles") == 0) {
		db = xInitDatabase("p"+p+"icicles");
		trQuestVarSet("p"+p+"icicles", db);
		xInitAddInt(db, "name");
		xIcicleTimeout = xInitAddInt(db, "timeout");
		xIcicleLast = xInitAddInt(db, "last");
		xIciclePos = xInitAddVector(db, "pos");
		xIcicleStep = xInitAddBool(db, "step", false);
		xIcicleRadius = xInitAddFloat(db, "radius");
	}

	if (trQuestVarGet("p"+p+"titanicSmashes") == 0) {
		db = xInitDatabase("p"+p+"titanicSmashes");
		trQuestVarSet("p"+p+"titanicSmashes", db);
		xInitAddInt(db, "name");
		xSmashAngle = xInitAddFloat(db, "angle", 1.5708);
		xSmashVelocity = xInitAddFloat(db, "velocity", 2.0);
		xSmashScale = xInitAddFloat(db, "scale");
		xSmashDir = xInitAddVector(db, "dir");
		xSmashTimeout = xInitAddInt(db, "timeout");
	}
}


rule frosthammer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * FROSTHAMMER + p, "frosthammerAlways");
		trEventSetHandler(1000 + 12 * FROSTHAMMER + p, "chooseFrostHammer");
	}
}
