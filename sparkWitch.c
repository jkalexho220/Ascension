const int thunderstrikeCooldown = 12;
const float thunderstrikeRadius = 6;
const float thunderstrikeDamage = 60;

const int hexboltCooldown = 18;
const float hexboltDuration = 6;
const float hexboltRadius = 3;
const float hexboltDamage = 30;

const float vortexCost = 80;
const float vortexRadius = 12;
const float vortexMoveTime = 800; // 1000 * vortexRadius / 15

int xHex = 0;
int xBounces = 0;

int xZapStart = 0;
int xZapEnd = 0;
int xZapStartTime = 0;
int xZapNext = 0;
int xZapHex = 0;
int xZapBounces = 0;
int xZapIndex = 0;

int xWitchPigIndex = 0;
int xWitchPigProto = 0;
int xWitchPigTimeout = 0;

int xHexOrbPos = 0;
int xHexOrbNext = 0;
int xHexOrbTimeout = 0;

int xVortexLast = 0;
int xVortexRadius = 0;
int xVortexCenter = 0;
int xVortexDir = 0;
int xVortexStep = 0;
int xVortexTimeout = 0;

void removeSparkWitch(int p = 0) {
	removePlayerSpecific(p);
}

void sparkWitchAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * SPARKWITCH;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int zaps = trQuestVarGet("p"+p+"zaps");
	int pigs = trQuestVarGet("p"+p+"witchPigs");
	int orbs = trQuestVarGet("p"+p+"hexOrbs");
	int vortexObjects = trQuestVarGet("p"+p+"vortexObjects");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector end = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	for (y=xGetDatabaseCount(db); > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeSparkWitch(p);
		} else {
			if (kbUnitGetAnimationActionType(id) == 59) {
				xsSetContextPlayer(p);
				target = kbUnitGetTargetUnitID(id);
				xsSetContextPlayer(0);
				xSetBool(db, xCharAttacking, true);
				xSetInt(db, xCharSpecialAttack, 1);
				xSetInt(db, xCharAttackTarget, trGetUnitScenarioNameNumber(target));
				xSetInt(db, xCharAttackNext, trTimeMS() + xGetInt(dPlayerData, xPlayerFirstDelay));
				trUnitOverrideAnimation(19,0,false,false,-1);
			}
			switch(xGetInt(db, xCharSpecialAttack))
			{
				case 1:
				{
					if (trTimeMS() > xGetInt(db, xCharAttackNext)) {
						xSetInt(db, xCharSpecialAttack, 2);
						xSetInt(db, xCharAttackNext, xGetInt(db, xCharAttackNext) + xGetInt(dPlayerData, xPlayerFirstDelay));
						
						pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
						next = xGetInt(db, xCharAttackTarget);
						trUnitSelectClear();
						trUnitSelect(""+next, true);
						if (trUnitAlive()) {
							end = kbGetBlockPosition(""+next, true);
							xAddDatabaseBlock(zaps, true);
							xSetInt(zaps, xUnitName, next);
							xSetVector(zaps, xZapStart, pos);
							xSetVector(zaps, xZapEnd, end);
							xSetInt(zaps, xZapStartTime, trTimeMS());
							xSetInt(zaps, xZapHex, xGetInt(db, xHex));
							xSetInt(zaps, xZapBounces, xGetInt(dPlayerData, xPlayerProjectiles) + xGetInt(db, xBounces));
							if (xGetInt(db, xHex) == 2) {
								trSoundPlayFN("pigpower.wav","1",-1,"","");
							} else if (xGetInt(db, xHex) == 1) {
								trSoundPlayFN("lightningbirth.wav","1",-1,"","");
							}
							xSetInt(db, xBounces, 0);
							xSetInt(db, xHex, 0);
							for(x=xGetDatabaseCount(dEnemies); >0) {
								xDatabaseNext(dEnemies);
								if (xGetInt(dEnemies, xUnitName) == next) {
									xSetInt(zaps, xZapIndex, xGetPointer(dEnemies));
									break;
								}
							}
							if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
								if (trCurrentPlayer() == p) {
									trClearCounterDisplay();
								}
							}
						}
					} else if (kbUnitGetAnimationActionType(id) != 16) {
						xSetBool(db, xCharAttacking, false);
						xSetInt(db, xCharSpecialAttack, 0);
					}
				}
				case 2:
				{
					if (trTimeMS() > xGetInt(db, xCharAttackNext)) {
						trUnitOverrideAnimation(-1,0,false,true,-1);
						xSetBool(db, xCharAttacking, false);
						xSetInt(db, xCharSpecialAttack, 0);
					}
				}
			}
		}
	}
	
	if (xGetDatabaseCount(pigs) > 0) {
		xDatabaseNext(pigs);
		xUnitSelect(pigs, xUnitName);
		if (trUnitAlive() == false) {
			trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(pigs, xWitchPigProto)));
			xFreeDatabaseBlock(pigs);
		} else if (trTimeMS() > xGetInt(pigs, xWitchPigTimeout)) {
			trMutateSelected(xGetInt(pigs, xWitchPigProto));
			if (xSetPointer(dEnemies, xGetInt(pigs, xWitchPigTimeout))) {
				xSetFloat(dEnemies, xPhysicalResist, xGetFloat(pigs, xPhysicalResist));
				xSetFloat(dEnemies, xMagicResist, xGetFloat(pigs, xMagicResist));
				if (xGetInt(dEnemies, xStunStatus) > 0) {
					xSetInt(dStunnedUnits, xStunnedProto, xGetInt(pigs, xWitchPigProto), xGetInt(dEnemies, xStunStatus));
				}
			}
			xFreeDatabaseBlock(pigs);
		}
	}
	
	if(xGetDatabaseCount(orbs) >0) {
		xDatabaseNext(orbs);
		amt = trTimeMS() - xGetInt(orbs, xHexOrbNext);
		if (amt > 500) {
			amt = 0.001 * amt * hexboltDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
			pos = xGetVector(orbs, xHexOrbPos);
			dist = xsPow(hexboltRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					damageEnemy(p, amt);
				}
			}
			
			if (trTimeMS() > xGetInt(orbs, xHexOrbTimeout)) {
				xUnitSelect(orbs, xUnitName);
				trUnitDestroy();
				xFreeDatabaseBlock(orbs);
			} else {
				xSetInt(orbs, xHexOrbNext, trTimeMS());
			}
		}
	}
	
	if (xGetDatabaseCount(zaps) > 0) {
		xDatabaseNext(zaps);
		current = trTimeMS() - xGetInt(zaps, xZapStartTime);
		if (current > xGetInt(zaps, xZapNext)) {
			xUnitSelect(zaps, xUnitName);
			if (trUnitAlive()) {
				end = kbGetBlockPosition(""+xGetInt(zaps, xUnitName), true);
				xSetVector(zaps, xZapEnd, end);
			} else {
				end = xGetVector(zaps, xZapEnd);
			}
			pos = xGetVector(zaps, xZapStart);
			dir = end - pos;
			for(y=5; >0) {
				if (current > xGetInt(zaps, xZapNext)) {
					xSetInt(zaps, xZapNext, 100 + xGetInt(zaps, xZapNext));
					amt = 0.002 * xGetInt(zaps, xZapNext);
					pos = pos + (dir * amt);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					if (xGetInt(zaps, xZapHex) == 0) {
						trUnitChangeProtoUnit("Lightning Sparks Ground");
					} else {
						trUnitChangeProtoUnit("Lightning Sparks");
					}
				} else {
					break;
				}
			}
			if (xGetInt(zaps, xZapNext) >= 500) {
				gainFavor(p, 1.0);
				trQuestVarSetFromRand("sound", 1, 5, true);
				trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				xUnitSelect(zaps, xUnitName);
				if (trUnitAlive()) {
					trUnitHighlight(0.2, false);
					if (xGetInt(zaps, xZapIndex) > 0) { // if it targeted an enemy
						if (xSetPointer(dEnemies, xGetInt(zaps, xZapIndex))) {
							damageEnemy(p, 100.0 * xGetFloat(dPlayerData, xPlayerSpellDamage));
							if (xGetInt(zaps, xZapHex) > 0) {
								pos = vectorSnapToGrid(end);
								next = trGetNextUnitScenarioNameNumber();
								trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								if (xGetInt(zaps, xZapHex) == 2) {
									xAddDatabaseBlock(pigs, true);
									xSetInt(pigs, xUnitName, xGetInt(zaps, xUnitName));
									xSetInt(pigs, xWitchPigProto, xGetInt(dEnemies, xUnitProto));
									xSetFloat(pigs, xPhysicalResist, xGetFloat(dEnemies, xPhysicalResist));
									xSetFloat(pigs, xMagicResist, xGetFloat(dEnemies, xMagicResist));
									xSetInt(pigs, xWitchPigIndex, xGetPointer(dEnemies));
									xSetInt(pigs, xWitchPigTimeout, trTimeMS() + 1000 * hexboltDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
									xSetFloat(dEnemies, xPhysicalResist, 0);
									xSetFloat(dEnemies, xMagicResist, 0);
									if (xGetInt(zaps, xUnitName) == bossUnit) {
										trMutateSelected(kbGetProtoUnitID("Hero Boar 2"));
									} else {
										trMutateSelected(kbGetProtoUnitID("Hero Boar"));
									}
									xSetInt(dEnemies, xSilenceStatus, 1); // prevent UI range indicator from appearing
									silenceUnit(dEnemies,hexboltDuration,p);
									trUnitSelectClear();
									trUnitSelect(""+next, true);
									trUnitChangeProtoUnit("Curse SFX");
								} else if (xGetInt(zaps, xZapHex) == 1) {
									trUnitSelectClear();
									trUnitSelect(""+next, true);
									trUnitChangeProtoUnit("Spy Eye");
									trUnitSelectClear();
									trUnitSelect(""+next, true);
									trMutateSelected(kbGetProtoUnitID("Arkantos God"));
									trSetSelectedScale(0,0,0);
									trUnitOverrideAnimation(26,0,true,false,-1);
									xAddDatabaseBlock(orbs, true);
									xSetInt(orbs, xUnitName, next);
									xSetInt(orbs, xHexOrbTimeout,
										trTimeMS() + 1000 * hexboltDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
									xSetInt(orbs, xHexOrbNext, trTimeMS());
									xSetVector(orbs, xHexOrbPos, pos);
								}
							}
							OnHit(p, xGetInt(zaps, xZapIndex), true);
						}
					}
				}
				xSetInt(zaps, xZapBounces, xGetInt(zaps, xZapBounces) - 1);
				if (xGetInt(zaps, xZapBounces) > 0) {
					dist = xsPow(xGetFloat(dPlayerData, xPlayerRange) - 5, 2);
					hit = 0;
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (xGetInt(zaps, xUnitName) == xGetInt(dEnemies, xUnitName)) {
							continue;
						} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), end) < dist) {
							hit = 1;
							break;
						}
					}
					if (hit == 1) {
						xSetInt(zaps, xUnitName, xGetInt(dEnemies, xUnitName));
						xSetInt(zaps, xZapIndex, xGetPointer(dEnemies));
						xSetInt(zaps, xZapStartTime, trTimeMS());
						xSetInt(zaps, xZapNext, 0);
						xSetVector(zaps, xZapStart, end);
						xSetVector(zaps, xZapEnd, xGetVector(dEnemies, xUnitPos));
					} else if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit), end) < dist) {
						xSetInt(zaps, xUnitName, xGetInt(dPlayerData, xPlayerUnit));
						xSetInt(zaps, xZapIndex, -1);
						xSetInt(zaps, xZapNext, 0);
						xSetInt(zaps, xZapStartTime, trTimeMS());
						xSetVector(zaps, xZapStart, end);
						xSetVector(zaps, xZapEnd, kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit)));
					} else {
						xFreeDatabaseBlock(zaps);
					}
				} else {
					xFreeDatabaseBlock(zaps);
				}
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"thunderstrikeActive") == 1) {
		trQuestVarSet("p"+p+"thunderstrikeActive", 0);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"thunderstrike");
		trUnitChangeProtoUnit("Tremor");
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xSetInt(db, xBounces, 1 + xGetInt(db, xBounces));
		}
		pos = xGetVector(dPlayerData, xPlayerWellPos);
		trQuestVarSetFromRand("sound", 1, 5, true);
		trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		trChatSetStatus(false);
		trDelayedRuleActivation("enable_chat");
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Militia"));
		trSetSelectedScale(0,0,0);
		trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
		trQuestVarSet("p"+p+"thunderstrikeActive", 1);
		trQuestVarSet("p"+p+"thunderstrike", next);
		dist = xsPow(thunderstrikeRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		amt = thunderstrikeDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
		hit = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
				hit = hit + 1;
				damageEnemy(p, amt);
			}
		}
		gainFavor(p, hit);
		if (hit > 0) {
			xSetInt(dPlayerData, xPlayerWellReadyTime, xGetInt(dPlayerData, xPlayerWellReadyTime) - 1000 * hit);
			if (xGetInt(dPlayerData, xPlayerWellReadyTime) < trTimeMS() + 500) {
				xSetInt(dPlayerData, xPlayerWellReadyTime, trTimeMS() + 500);
			}
			if (trCurrentPlayer() == p) {
				trCounterAbort("well");
				trCounterAddTime("well",
					xsMax(xGetInt(dPlayerData, xPlayerWellCooldown) * xGetFloat(dPlayerData, xPlayerCooldownReduction) - hit, 1),
					0, wellName);
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xSetInt(db, xBounces, 1 + xGetInt(db, xBounces));
		}
		gainFavor(p, 0.0 - vortexCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		dist = vortexRadius * xGetFloat(dPlayerData, xPlayerSpellRange);
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Dwarf",8,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect(""+p+",0");
		trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
		trSetSelectedScale(0,0,0);
		trUnitSetAnimationPath("0,0,1,0,0,0,0");
		xAddDatabaseBlock(vortexObjects, true);
		xSetInt(vortexObjects, xUnitName, next);
		xSetInt(vortexObjects, xVortexLast, trTimeMS());
		xSetFloat(vortexObjects, xVortexRadius, vortexRadius * xGetFloat(dPlayerData, xPlayerSpellRange));
		xSetVector(vortexObjects, xVortexCenter, pos);
		xSetInt(vortexObjects, xVortexTimeout, trTimeMS() + 1000);
		trSoundPlayFN("cinematics\32_out\doorseal.mp3","1",-1,"","");
	}
	
	if (xGetDatabaseCount(vortexObjects) > 0) {
		xDatabaseNext(vortexObjects);
		if (xGetInt(vortexObjects, xVortexStep) == 1) {
			dist = 0.015 * (xGetInt(vortexObjects, xVortexTimeout) - trTimeMS());
		} else {
			dist = xGetFloat(vortexObjects, xVortexRadius);
		}
		amt = trTimeMS() - xGetInt(vortexObjects, xVortexLast);
		amt = amt * 0.002;
		xSetInt(vortexObjects, xVortexLast, trTimeMS());
		
		dir = rotationMatrix(xGetVector(vortexObjects, xVortexDir), xsCos(amt), xsSin(amt));
		xSetVector(vortexObjects, xVortexDir, dir);
		dir = dir * dist;
		for(x=0; < 8) {
			trUnitSelectClear();
			trUnitSelect(""+(x + xGetInt(vortexObjects, xUnitName)), true);
			trSetSelectedUpVector(xsVectorGetX(dir),0,xsVectorGetZ(dir));
			dir = rotationMatrix(dir, 0.707107, 0.707107);
		}
		if (trTimeMS() > xGetInt(vortexObjects, xVortexTimeout)) {
			if (xGetInt(vortexObjects, xVortexStep) == 0) {
				xSetInt(vortexObjects, xVortexStep, 1);
				xSetInt(vortexObjects, xVortexTimeout,
					trTimeMS() + vortexMoveTime * xGetFloat(dPlayerData, xPlayerSpellRange));
				trSoundPlayFN("suckup1.wav","1",-1,"","");
				trSoundPlayFN("vortexstart.wav","1",-1,"","");
				dist = xsPow(vortexRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
				pos = xGetVector(vortexObjects, xVortexCenter);
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
						launchUnit(dEnemies, pos);
						xSetBool(dLaunchedUnits, xLaunchedStun, true);
					}
				}
			} else {
				for(x=0; < 8) {
					trUnitSelectClear();
					trUnitSelect(""+(x + xGetInt(vortexObjects, xUnitName)), true);
					trUnitChangeProtoUnit("Arkantos Boost SFX");
				}
				xFreeDatabaseBlock(vortexObjects);
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"hex", 1 - trQuestVarGet("p"+p+"hex"));
		if (trCurrentPlayer() == p) {
			trSoundPlayFN("skypassageout.wav","1",-1,"","");
			switch(1*trQuestVarGet("p"+p+"hex"))
			{
				case 0:
				{
					trSetCounterDisplay("Next Attack: ELECTRIFY");
					trSoundPlayFN("lapadesconvert.wav","1",-1,"","");
				}
				case 1:
				{
					trSetCounterDisplay("Next Attack: POLYMORPH");
					trSoundPlayFN("pigambient.wav","1",-1,"","");
				}
			}
		}
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xSetInt(db, xBounces, 1 + xGetInt(db, xBounces));
			xSetInt(db, xHex, 1 + trQuestVarGet("p"+p+"hex"));
		}
	}
	
	xSetPointer(dEnemies, index);
	pvpReattachPlayer();
}

void chooseSparkWitch(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * SPARKWITCH;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Thunderstrike";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Hexbolt";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Vortex";
		lureIsUltimate = true;
	}
	xHex = xInitAddInt(db, "hex");
	xBounces = xInitAddInt(db, "bounces");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, thunderstrikeCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, vortexCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,hexboltCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"zaps") == 0) {
		db = xInitDatabase("p"+p+"zaps");
		trQuestVarSet("p"+p+"zaps", db);
		xInitAddInt(db, "name");
		xZapIndex = xInitAddInt(db, "index");
		xZapStart = xInitAddVector(db, "start");
		xZapEnd = xInitAddVector(db, "end");
		xZapStartTime = xInitAddInt(db, "startTime");
		xZapNext = xInitAddInt(db, "next");
		xZapHex = xInitAddInt(db, "hex");
		xZapBounces = xInitAddInt(db, "bounces");
	}
	
	if (trQuestVarGet("p"+p+"witchPigs") == 0) {
		db = xInitDatabase("p"+p+"witchPigs");
		trQuestVarSet("p"+p+"witchPigs", db);
		xInitAddInt(db, "name");
		xWitchPigIndex = xInitAddInt(db, "index");
		xWitchPigProto = xInitAddInt(db, "proto");
		xInitAddFloat(db, "physicalResist");
		xInitAddFloat(db, "magicResist");
		xWitchPigTimeout = xInitAddInt(db, "timeout");
	}
	
	if (trQuestVarGet("p"+p+"hexOrbs") == 0) {
		db = xInitDatabase("p"+p+"hexOrbs");
		trQuestVarSet("p"+p+"hexOrbs", db);
		xInitAddInt(db, "name");
		xHexOrbPos = xInitAddVector(db, "pos");
		xHexOrbNext = xInitAddInt(db, "next");
		xHexOrbTimeout = xInitAddInt(db, "timeout");
	}
	
	if (trQuestVarGet("p"+p+"vortexObjects") == 0) {
		db = xInitDatabase("p"+p+"vortexObjects");
		trQuestVarSet("p"+p+"vortexObjects", db);
		xInitAddInt(db, "name");
		xVortexLast = xInitAddInt(db, "last");
		xVortexRadius = xInitAddFloat(db, "radius");
		xVortexCenter = xInitAddVector(db, "center");
		xVortexDir = xInitAddVector(db, "dir", vector(1,0,0));
		xVortexStep = xInitAddInt(db, "step");
		xVortexTimeout = xInitAddInt(db, "timeout");
	}
}

void sparkwitchModify(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * SPARKWITCH;
	xSetPointer(dPlayerData, p);
	xSetFloat(dPlayerData, xPlayerBaseAttack, 100.0 * xGetFloat(dPlayerData, xPlayerSpellDamage));
	xSetFloat(dPlayerData, xPlayerAttack, xGetFloat(dPlayerData, xPlayerBaseAttack));
}

rule sparkWitch_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SPARKWITCH + p, "sparkWitchAlways");
		trEventSetHandler(1000 + 12 * SPARKWITCH + p, "chooseSparkWitch");
		trEventSetHandler(5000 + 12 * SPARKWITCH + p, "sparkwitchModify");
	}
}
