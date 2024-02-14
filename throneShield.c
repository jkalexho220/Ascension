
const int vowCooldown = 10;

const int justiceCooldown = 10;
const float justiceRadius = 8;

const float shieldOfLightCost = 60;
const int shieldOfLightCooldown = 20;
const float shieldOfLightDuration = 9;
const float shieldOfLightWidth = 3;
const float shieldOfLightRadius = 20;

int xThroneShieldSFX = 0;
int xThroneShieldAbsorbed = 0;
int xThroneShieldLaser = 0;

void removeThroneShield(int p = 0) {
	int db = getCharactersDB(p);
	if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit, p)) {
		int queen = xGetInt(dPlayerData, xPlayerQueen);
		if (queen > 0) {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, queen), true);
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		xSetInt(dPlayerData, xPlayerTether, 0, p);
		xSetInt(dPlayerData, xPlayerSimp, 0, queen);
		xSetInt(dPlayerData, xPlayerQueen, 0, p);
	}
	removePlayerSpecific(p);
}

void throneShieldAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * THRONESHIELD;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int count = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector end = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		start = xGetVector(dPlayerData, xPlayerWellPos);
		target = 0;
		if (xGetInt(dPlayerData, xPlayerQueen) > 0) {
			if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, xGetInt(dPlayerData, xPlayerQueen)), start) < 16) {
				target = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),225,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeThroneShield();
					} else {
						trImmediateUnitGarrison(""+target);
						trUnitChangeProtoUnit("Trident Soldier Hero");
					}
				}
				equipRelicsAgain(p);
				trUnitSelectClear();
				trUnitSelect(""+target, true);
				trUnitChangeProtoUnit("Fimbulwinter SFX");
				trSoundPlayFN("skypassageout.wav","1",-1,"","");
				target = -1;
			}
		}
		if (target == 0) {
			dist = 100;
			for(x=1; < ENEMY_PLAYER) {
				if (x == p) {
					continue;
				} else if (xGetInt(dPlayerData, xPlayerDead, x) == 0) {
					current = unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, x), start);
					if (current < dist) {
						target = x;
						dist = current;
					}
				}
			}
		}
		if (target > 0) {
			if (xGetInt(dPlayerData, xPlayerSimp, target) > 0) {
				target = 0;
				if (trCurrentPlayer() == p) {
					if (xGetInt(dPlayerData, xPlayerSimp, target) == p) {
						trChatSend(0, "You must cast closer to your ally in order to teleport to them.");
					} else {
						trChatSend(0, "That player already has a partner!");
					}
				}
			} else if (xGetInt(dPlayerData, xPlayerClass, target) == THRONESHIELD) {
				target = 0;
				if (trCurrentPlayer() == p) {
					trChatSend(0, "Cannot target another Throne Shield!");
				}
			} else {
				/* first we renounce our love for our first queen */
				if (xGetInt(dPlayerData, xPlayerQueen) > 0) {
					hit = xGetInt(dPlayerData, xPlayerQueen);
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, hit), true);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					xSetInt(dPlayerData, xPlayerSimp, 0, hit);
				}
				xSetInt(dPlayerData, xPlayerSimp, p, target);
				xSetInt(dPlayerData, xPlayerQueen, target, p);
				if (trCurrentPlayer() == p) {
					trChatSend(0, "<color=1,1,1>You have given your Knight's Vow to " + trStringQuestVarGet("p"+target+"name") + "!");
				}
				if (trCurrentPlayer() == target) {
					trChatSend(0, "<color=1,1,1>" + trStringQuestVarGet("p"+p+"name") + " has given you a Knight's Vow!");
				}
				id = xGetInt(dPlayerUnits, xUnitID, xGetInt(dPlayerData, xPlayerIndex, target));
				xsSetContextPlayer(target);
				amt = kbUnitGetCurrentHitpoints(id);
				xsSetContextPlayer(0);
				xSetFloat(dPlayerUnits, xCurrentHealth, amt, xGetInt(dPlayerData, xPlayerIndex, target));
				trSoundPlayFN("militarycreate.wav","1",-1,"","");
				trSoundPlayFN("herocreation.wav","1",-1,"","");
				if (xGetInt(dPlayerData, xPlayerTether) <= 0) {
					spyEffect(xGetInt(dPlayerData, xPlayerUnit, p),
						kbGetProtoUnitID("Vortex Finish Linked"),xsVectorSet(dPlayerData,xPlayerTether,p));
					trQuestVarSet("p"+p+"tetherReady", 0);
				} else {
					xUnitSelect(dPlayerData, xPlayerTether);
					trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+p+"tetherReady", 1);
				}
				if (xGetInt(dPlayerData, xPlayerTether, target) == -1) {
					spyEffect(xGetInt(dPlayerData, xPlayerUnit, target),
						kbGetProtoUnitID("Vortex Finish Linked"),xsVectorSet(dPlayerData, xPlayerTether, target));
					trQuestVarSet("p"+target+"tetherReady", 0);
				} else {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, target), true);
					trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+target+"tetherReady", 1);
				}
			}
		}
		if (target == 0) {
			if (trCurrentPlayer() == p) {
				trChatSend(0, "You must target an ally!");
				trCounterAbort("well");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
			}
			xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COST);
		}
	}
	
	/* OMG MY QUEEN HEART-EYES EMOJI */
	if (xGetInt(dPlayerData, xPlayerQueen) > 0) {
		target = xGetInt(dPlayerData, xPlayerQueen);
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, target), true);
		if (trUnitAlive()) {
			start = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, target), true);
			end = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
			dir = getUnitVector(start, end);
			if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
				if (xGetInt(dPlayerData, xPlayerTether) <= 0) {
					spyEffect(xGetInt(dPlayerData, xPlayerUnit),
						kbGetProtoUnitID("Vortex Finish Linked"),xsVectorSet(dPlayerData, xPlayerTether, p));
					trQuestVarSet("p"+p+"tetherReady", 0);
				} else if (trQuestVarGet("p"+p+"tetherReady") == 0) {
					xUnitSelect(dPlayerData, xPlayerTether);
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+p+"tetherReady", 1);
				}
				if (xGetInt(dPlayerData, xPlayerTether, target) <= 0) {
					spyEffect(xGetInt(dPlayerData, xPlayerUnit, target),
						kbGetProtoUnitID("Vortex Finish Linked"),xsVectorSet(dPlayerData, xPlayerTether, target));
					trQuestVarSet("p"+target+"tetherReady", 0);
				} else if (trQuestVarGet("p"+target+"tetherReady") == 0) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, target), true);
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+target+"tetherReady", 1);
				}
			}
			/* point the tethers towards each other */
			xUnitSelect(dPlayerData, xPlayerTether);
			trSetSelectedUpVector(0.0 - xsVectorGetX(dir),-1,0.0 - xsVectorGetZ(dir));
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, target), true);
			trSetSelectedUpVector(xsVectorGetX(dir),-1,xsVectorGetZ(dir));
			/* health damage reduction */
			id = xGetInt(dPlayerUnits, xUnitID, xGetInt(dPlayerData, xPlayerIndex, target));
			xsSetContextPlayer(target);
			amt = kbUnitGetCurrentHitpoints(id);
			xsSetContextPlayer(0);
			current = xGetFloat(dPlayerUnits, xCurrentHealth, xGetInt(dPlayerData, xPlayerIndex, target));
			dist = 0.5 * (current - amt);
			if (dist > 0 && current > 1.0) {
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, target), true);
				trDamageUnit(0.0 - dist);
				xUnitSelect(dPlayerData, xPlayerUnit);
				trDamageUnit(dist);
				amt = amt + dist;
			}
			xSetFloat(dPlayerUnits, xCurrentHealth, amt, xGetInt(dPlayerData, xPlayerIndex, target));
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		trQuestVarSet("p"+p+"shieldOfLight", 1);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trSoundPlayFN("gaiasparkle2.wav","1",-1,"","");
		trQuestVarSet("p"+p+"shieldOfLightTimeout",
			trTimeMS() + 1000 * shieldOfLightDuration * xGetFloat(dPlayerData, xPlayerSpellDuration) - 3000);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			if (xGetInt(db, xThroneShieldSFX) == 0) {
				spyEffect(xGetInt(db, xUnitName),
					kbGetProtoUnitID("Increase Prosperity Small"),xsVectorSet(db, xThroneShieldSFX, xGetPointer(db)));
			} else {
				xUnitSelect(db, xThroneShieldSFX);
				trUnitChangeProtoUnit("Increase Prosperity Small");
			}
		}

		start = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit), true);
		dist = xsPow(shieldOfLightRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), start) < dist) {
				trUnitDoWorkOnUnit(""+xGetInt(dPlayerData, xPlayerUnit, p));
			}
		}
		gainFavor(p, 0.0 - shieldOfLightCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trQuestVarSet("p"+p+"shieldOfLight", 4);
	}
	
	if (trQuestVarGet("p"+p+"shieldOfLight") >= 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"shieldOfLightTimeout")) {
			hit = trQuestVarGet("p"+p+"shieldOfLight") - 1;
			if (hit >=1) {
				trQuestVarSet("p"+p+"shieldOfLightTimeout", 1000 + trQuestVarGet("p"+p+"shieldOfLightTimeout"));
				if (trCurrentPlayer() == p) {
					trChatSend(0, "<color=1,1,1>Shield of Light ends in " + hit + "...");
					trSoundPlayFN("hitpointsmax.wav","1",-1,"","");
				}
			} else {
				trQuestVarSet("p"+p+"shieldOfLight", 0);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
						if (trCurrentPlayer() == p) {
							trChatSend(0,"<color=1,1,1>Shield of Light finished. Total damage: " + 1*xGetFloat(db, xThroneShieldAbsorbed));
						}
					}
					xSetBool(db, xThroneShieldLaser, true);
					xUnitSelect(db, xThroneShieldSFX);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
			trQuestVarSet("p"+p+"shieldOfLight", hit);
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		dist = xsPow(justiceRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			target = xGetInt(db, xUnitID);
			start = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			for(y=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				id = xGetInt(dEnemies, xUnitID);
				trUnitSelectClear();
				trUnitSelectByID(id);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), start) < dist) {
					if (PvP) {
						xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner, xGetInt(dEnemies, xDoppelganger)));
					} else {
						xsSetContextPlayer(ENEMY_PLAYER);
					}
					hit = kbUnitGetTargetUnitID(id);
					xsSetContextPlayer(0);
					if (hit == target) {
						silenceUnit(dEnemies,6.0, p);
					} else {
						stunUnit(dEnemies, 3.0, p);
					}
					gainFavor(p, 1);
				}
			}
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
			trArmySelect("1,0");
			trUnitChangeProtoUnit("Olympus Temple SFX");
		}
	}
	
	/* for security reasons, this needs to run every loop */
	for (y=xGetDatabaseCount(db); > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeThroneShield(p);
		} else {
			/* damage reduction */
			xsSetContextPlayer(p);
			amt = kbUnitGetCurrentHitpoints(id);
			xsSetContextPlayer(0);
			dist = xGetFloat(dPlayerUnits, xCurrentHealth, xGetInt(db, xCharIndex)) - amt;
			if (dist > 0) {
				if (trQuestVarGet("p"+p+"shieldOfLight") >= 1) {
					xSetFloat(db, xThroneShieldAbsorbed, xGetFloat(db, xThroneShieldAbsorbed) + dist);
					current = 1.0 + xsSqrt(xGetFloat(db, xThroneShieldAbsorbed)) * 0.02;
					trSetSelectedScale(current,current,current);
					trUnitHighlight(0.2, false);
				} else if (xGetFloat(dPlayerUnits, xCurrentHealth, xGetInt(db, xCharIndex)) <= 1) {
					dist = 0;
				} else {
					dist = dist * xsMin(0.9, trQuestVarGet("p"+p+"damageReduction"));
				}
				trDamageUnit(0.0 - dist);
				amt = amt + dist;
			}
			xSetFloat(dPlayerUnits, xCurrentHealth, amt, xGetInt(db, xCharIndex));
			
			hit = CheckOnHit(p);
			if (hit >= ON_HIT_NORMAL) {
				if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
					if (xGetBool(db, xThroneShieldLaser)) {
						start = kbGetBlockPosition(""+xGetInt(db,xUnitName), true);
						end = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
						dir = getUnitVector(start, end);
						trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
						if (trUnitVisToPlayer()) {
							trCameraShake(1.0, 1.5);
						}
						target = trGetNextUnitScenarioNameNumber();
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
						trUnitSelectClear();
						trUnitSelect(""+target, true);
						trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
						trUnitHighlight(5.0, false);
						trSetUnitOrientation(xsVectorSet(0.0 - xsVectorGetX(dir),0, 0.0 - xsVectorGetZ(dir)), vector(0,1,0), true);
						xAddDatabaseBlock(dPlayerLasers, true);
						xSetInt(dPlayerLasers, xUnitName, target);
						xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 2500);
						xSetFloat(dPlayerLasers, xPlayerLaserRange, 50.0);
						dist = xsPow(shieldOfLightWidth * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
						amt = xGetFloat(db, xThroneShieldAbsorbed) * xGetFloat(dPlayerData, xPlayerSpellDamage);
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (rayCollision(dEnemies,start,dir,999.0,dist)) {
								damageEnemy(p, amt, true);
							}
						}
						end = vectorSetAsTargetVector(start, start + dir, 300.0);
						target = trGetNextUnitScenarioNameNumber();
						trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
						trUnitSelectClear();
						trUnitSelect(""+target, true);
						trUnitChangeProtoUnit("Lampades Bolt");
						trUnitSelectClear();
						trUnitSelect(""+target, true);
						zSetProtoUnitStat("Meteorite",p,1,100.0);
						trMutateSelected(kbGetProtoUnitID("Meteorite"));
						trUnitOverrideAnimation(6,0,true,false,-1);
						trUnitMoveToPoint(xsVectorGetX(end),0,xsVectorGetZ(end));
						xSetFloat(db, xThroneShieldAbsorbed, 0);
						xSetBool(db, xThroneShieldLaser, false);
						xUnitSelectByID(db, xUnitID);
						trSetSelectedScale(1,1,1);
					} else if (hit == ON_HIT_SPECIAL) {
						count = 1;
						if (playerHasSymphony(p, SYMPHONY_DOUBLE_SPECIAL)) {
							count = 2;
						}
						for(i=count; >0) {
							gainFavor(p, 3);
							stunUnit(dEnemies, 3.0, p);
							amt = 0.1 * xGetFloat(dPlayerData, xPlayerHealth);
							for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
								xDatabaseNext(dPlayerCharacters);
								xUnitSelectByID(dPlayerCharacters, xUnitID);
								if (trUnitAlive() == false) {
									removePlayerCharacter();
								} else {
									healUnit(p, amt);
								}
							}
						}
					}
				}
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseThroneShield(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * THRONESHIELD;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Knight's Vow";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Justice";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Shield of Light";
		lureIsUltimate = true;
	}
	xThroneShieldSFX = xInitAddInt(db, "shieldSfx");
	xThroneShieldAbsorbed = xInitAddFloat(db, "absorbed");
	xThroneShieldLaser = xInitAddBool(db, "laser");
	trQuestVarSet("p"+p+"damageReduction", 0.02 * xGetFloat(dPlayerData, xPlayerHealth) / 100);
	xSetInt(dPlayerData,xPlayerWellCooldown, vowCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, shieldOfLightCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, shieldOfLightCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,justiceCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
}

void modifyThroneShield(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * THRONESHIELD;
	trQuestVarSet("p"+p+"damageReduction", 0.02 * xGetFloat(dPlayerData, xPlayerHealth, p) / 100);
}

rule throneShield_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * THRONESHIELD + p, "throneShieldAlways");
		trEventSetHandler(1000 + 12 * THRONESHIELD + p, "chooseThroneShield");
		trEventSetHandler(5000 + 12 * THRONESHIELD + p, "modifyThroneShield");
	}
}
