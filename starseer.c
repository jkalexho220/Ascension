const float starBaseDamage = 50;

const int realignCooldown = 5;
const float realignRadius = 15;

const int RepelCooldown = 15;
const float RepelRange = 15;

const float cleansingCost = 6;
const float cleansingHeal = 20;

const float starAngularVelocity = 1.570796; // 6.283185 / 4.0

int xStarseerStar = 0;
int xStarseerMeteorite = 0;
int xStarseerTargetRadius = 0;
int xStarseerCurrentRadius = 0;
int xStarseerAngle = 0;
int xStarseerHitbox = 0;
int xStarseerLast = 0;
int xStarseerDir = 0;


void removeStarseer(int p = 0) {
	int db = getCharactersDB(p);
	for(x=0; <3) {
		xUnitSelect(db, xStarseerStar + x);
		trUnitChangeProtoUnit("Rocket");
		xUnitSelect(db, xStarseerMeteorite + x);
		trUnitChangeProtoUnit("Rocket");
	}
	removePlayerSpecific(p);
}

void cleansingOff(int p = 0) {
	trQuestVarSet("p"+p+"cleansing", 0);
	trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
	int db = getCharactersDB(p);
	for(i=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeStarseer(p);
		} else {
			for(j=0; < 3) {
				xUnitSelect(db, xStarseerMeteorite + j);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
}

void starseerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * STARSEER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	float amt = 0;
	float dist = 0;
	float current = 0;
	float angleDiff = 0;
	float outer = 0;
	float inner = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector cur = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dest = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector center = vector(0,0,0);
	
	for (y=xGetDatabaseCount(db); > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeStarseer(p);
		} else {
			
			current = trTimeMS() - xGetInt(db, xStarseerLast);
			xSetInt(db, xStarseerLast, trTimeMS());
			
			amt = trQuestVarGet("p"+p+"starAngularVelocity") * 0.001 * current;
			xSetFloat(db, xStarseerAngle, fModulo(6.283185, xGetFloat(db, xStarseerAngle) - amt));
			
			dist = xGetFloat(db, xStarseerTargetRadius) - xGetFloat(db, xStarseerCurrentRadius);
			if (xsAbs(dist) > 0.1) {
				dist = 2.0 * dist * 0.001 * current;
				xSetFloat(db, xStarseerCurrentRadius, xGetFloat(db, xStarseerCurrentRadius) + dist);
			} else if (xsAbs(dist) > 0) {
				xSetFloat(db, xStarseerCurrentRadius, xGetFloat(db, xStarseerTargetRadius));
			}
			
			/*
			Adjust player speed
			*/
			if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
				amt = realignRadius * xGetFloat(dPlayerData, xPlayerSpellRange) - 2.0;
				amt = (amt - xGetFloat(db, xStarseerCurrentRadius) + 2.0) / amt;
				zSetProtoUnitStat("Oracle Hero", p, 1, xGetFloat(dPlayerData, xPlayerSpeed) * (0.5 + amt));
			}
			
			hit = 1 + xGetInt(db, xStarseerHitbox);
			if (hit >= 3) {
				hit = 0;
			}
			xSetInt(db, xStarseerHitbox, hit);
			
			current = xGetFloat(db, xStarseerAngle);
			dir = xsVectorSet(xsSin(current), 0, xsCos(current));
			for(x=0; < 3) {
				if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
					if (xGetInt(db, xStarseerMeteorite + x) <= 0) {
						spyEffect(xGetInt(db, xUnitName),
							kbGetProtoUnitID("Cinematic Block"),xsVectorSet(db, xStarseerMeteorite + x, xGetPointer(db)));
					}
					if (xGetInt(db, xStarseerStar + x) <= 0) {
						spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Outpost"),
							xsVectorSet(db, xStarseerStar + x, xGetPointer(db)),vector(0,0,0));
					}
				}
				if (x == hit) {
					/* this is the hitbox for this iteration */
					cur = dir;
					prev = xGetVector(db, xStarseerDir + x);
					if (xsVectorGetX(prev) + xsVectorGetZ(prev) == 0) {
						prev = vector(1,0,0);
					}
					angleDiff = dotProduct(cur, prev);
				}
				pos = dir * xGetFloat(db, xStarseerCurrentRadius);
				xUnitSelect(db, xStarseerStar + x);
				trSetSelectedUpVector(3.33 * xsVectorGetX(pos),0.2,3.33 * xsVectorGetZ(pos));
				if (trQuestVarGet("p"+p+"cleansing") == 1) {
					xUnitSelect(db, xStarseerMeteorite + x);
					trSetSelectedUpVector(0.5 * xsVectorGetX(pos),-1,0.5 * xsVectorGetZ(pos));
				}
				
				dir = rotationMatrix(dir, -0.5, -0.866025);
			}
			
			/* collision detection for one star */
			outer = xsPow(xGetFloat(db, xStarseerCurrentRadius) + 1.5, 2);
			inner = xsPow(xGetFloat(db, xStarseerCurrentRadius) - 1.5, 2);
			center = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			
			amt = (starBaseDamage + 2.0 * xGetFloat(db, xStarseerCurrentRadius)) * xGetFloat(dPlayerData, xPlayerSpellDamage);
			target = 0;
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else {
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
					dist = distanceBetweenVectors(pos, center);
					if (dist < outer && dist > inner) {
						/* enemy is within the donut */
						dir = getUnitVector(center, pos);
						/* if enemy is in between the old and new positions, that's a hit */
						if (dotProduct(dir, cur) > angleDiff) {
							if (dotProduct(dir, prev) > angleDiff) {
								/* HIT */
								gainFavor(p, 1);
								trUnitHighlight(0.2, false);
								damageEnemy(p, amt, true);
								target = 1;
							}
						}
					}
				}
			}
			
			/* the star hit something so we update the prev position to be the current position */
			if (target == 1) {
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				xSetVector(db, xStarseerDir + hit, cur);
			} else if (angleDiff < trQuestVarGet("p"+p+"starCosine")) {
				xSetVector(db, xStarseerDir + hit, cur);
			}
		}
	}
	
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		pos = xGetVector(dPlayerData, xPlayerWellPos);
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db, true);
			trUnitSetStance("Passive");
			amt = xsMin(realignRadius * xGetFloat(dPlayerData, xPlayerSpellRange),
				unitDistanceToVector(xGetInt(db, xUnitName), pos, false));
			amt = xsMax(2, amt);
			xSetFloat(db, xStarseerTargetRadius, amt);
			if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
				trQuestVarSet("p"+p+"starseerRadius", amt);
			}
		}
		trSoundPlayFN("suckup3.wav","1",-1,"","");
	}
	
	if (trQuestVarGet("p"+p+"Repel") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"RepelTimeout")) {
			trQuestVarSet("p"+p+"RepelTimeout", trQuestVarGet("p"+p+"RepelTimeout") + 1000);
			trQuestVarSet("p"+p+"Repel", 2);
			hit = 0;
			dist = xsPow(RepelRange * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeStarseer(p);
				} else {
					pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Tremor");
					for(y=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
							dir = getUnitVector(pos, kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true),
								RepelRange * xGetFloat(dPlayerData, xPlayerSpellRange));
							launchUnit(dEnemies, pos + dir);
							hit = hit + 1;
						}
					}
				}
			}
			gainFavor(p, hit);
		}
	} else if (trQuestVarGet("p"+p+"Repel") == 2) {
		if (trTimeMS() > trQuestVarGet("p"+p+"RepelTimeout")) {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeStarseer(p);
				} else {
					trUnitOverrideAnimation(-1, 0, false, true, -1);
				}
			}
			trQuestVarSet("p"+p+"Repel", 0);
		}
	}

	if (trQuestVarGet("p"+p+"cleansing") == 1) {
		amt = (0.0 + trTimeMS() - trQuestVarGet("p"+p+"cleansingNext")) / 1000.0;
		trQuestVarSet("p"+p+"cleansingNext", trTimeMS());
		for(i=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelect(db, xUnitName);
			if (trUnitAlive() == false) {
				removeStarseer(p);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dist = xsPow(xGetFloat(db, xStarseerCurrentRadius), 2);
				for(j=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
						xSetInt(dPlayerUnits, xPoisonTimeout, 0);
						xSetInt(dPlayerUnits, xStunTimeout, 0);
						xSetInt(dPlayerUnits, xSilenceTimeout, 0);
						healUnit(p, amt * cleansingHeal * xGetFloat(dPlayerData, xPlayerSpellDamage));
					}
				}
			}
		}

		gainFavor(p, 0.0 - cleansingCost * amt * xGetFloat(dPlayerData, xPlayerUltimateCost));
		if (trPlayerResourceCount(p, "favor") == 0) {
			cleansingOff(p);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"cleansing", 1 - trQuestVarGet("p"+p+"cleansing"));
		if (trQuestVarGet("p"+p+"cleansing") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"cleansing", 0);
			} else {
				trQuestVarSet("p"+p+"cleansingNext", trTimeMS());
				trSoundPlayFN("converting.wav","1",-1,"","");
				for(i=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					if (trUnitAlive() == false) {
						removeStarseer(p);
					} else {
						for(j=0; < 3) {
							xUnitSelect(db, xStarseerMeteorite + j);
							trMutateSelected(kbGetProtoUnitID("Valkyrie"));
							trSetSelectedScale(0,0,0);
							trUnitSetAnimationPath("1,0,0,0,0,0,0");
							trUnitOverrideAnimation(15,0,true,false,-1);
						}
					}
				}
			}
		} else {
			cleansingOff(p);
		}
	}
	
	if (trQuestVarGet("p"+p+"massTeleport") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"RepelTimeout")) {
			trSoundPlayFN("vortexstart.wav");
			dest = trVectorQuestVarGet("p"+p+"massTeleportDestination");
			index = trGetNextUnitScenarioNameNumber();
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(dest),0,xsVectorGetZ(dest),0,true);
			trArmySelect(""+p+",0");
			trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeStarseer();
				} else {
					center = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dist = xsPow(xGetFloat(db, xStarseerCurrentRadius), 2);
					for(i=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if ((kbUnitGetProtoUnitID(xGetInt(dPlayerUnits, xUnitID)) != kbGetProtoUnitID("Oracle Hero")) || (trUnitIsOwnedBy(p) == false)) {
							pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
							if (distanceBetweenVectors(pos, center, true) < dist) {
								dir = getUnitVector(center, pos);
								cur = trVectorQuestVarGet("p"+p+"massTeleportDestination") + pos - center;
								next = trGetNextUnitScenarioNameNumber();
								target = xGetInt(dPlayerUnits, xPlayerOwner);
								trArmyDispatch("" + target +",0", "Dwarf",1,xsVectorGetX(cur),0,xsVectorGetZ(cur),0,true);
								trUnitSelectClear();
								trUnitSelect(""+next);
								trSetUnitOrientation(dir, vector(0,1,0),true);
								trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));

								xUnitSelectByID(dPlayerUnits, xUnitID);
								trImmediateUnitGarrison(""+next);
								trUnitChangeProtoUnit(kbGetProtoUnitName(kbGetUnitBaseTypeID(xGetInt(dPlayerUnits, xUnitID))));

								trUnitSelectClear();
								trUnitSelect(""+next);
								trUnitChangeProtoUnit("Vortex Finish Linked");

								// re-equip relics
								if (xGetInt(dPlayerData, xPlayerUnit, target) == xGetInt(dPlayerUnits, xUnitName)) {
									equipRelicsAgain(target);
									if (trCurrentPlayer() == target) {
										lookAt(xsVectorGetX(dest),xsVectorGetZ(dest));
									}
								}
							}
						}
					}

					xUnitSelectByID(db, xUnitID);
					trImmediateUnitGarrison(""+index);
					trUnitChangeProtoUnit("Oracle Hero");

					if (trCurrentPlayer() == p) {
						lookAt(xsVectorGetX(dest),xsVectorGetZ(dest));
					}
				}
			}

			trUnitSelectClear();
			trUnitSelect("" + index, true);
			trUnitChangeProtoUnit("Vision SFX");

			equipRelicsAgain(p);
			trQuestVarSet("p"+p+"massTeleport", 0);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		if (distanceBetweenVectors(xGetVector(dPlayerData, xPlayerLurePos), kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit), true)) > xsPow(trQuestVarGet("p"+p+"starseerRadius"), 2)) {
			trQuestVarSet("p"+p+"massTeleport", 1);
			trQuestVarSet("p"+p+"RepelTimeout", trTimeMS() + 2000);
			trSoundPlayFN("cinematics\24_in\magic.mp3");
			trVectorQuestVarSet("p"+p+"massTeleportDestination", xGetVector(dPlayerData, xPlayerLurePos));
		} else {
			trQuestVarSet("p"+p+"RepelTimeout", trTimeMS() + 1200);
			trQuestVarSet("p"+p+"Repel", 1);
			trSoundPlayFN("oracledone.wav","1",-1,"","");
			trSoundPlayFN("implode reverse.wav","1",-1,"","");
		}
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStarseer(p);
			} else {
				trMutateSelected(kbGetProtoUnitID("Oracle Hero"));
				trUnitSetStance("Passive");
				trUnitOverrideAnimation(52, 0, false, false, -1);
				if (trQuestVarGet("p"+p+"massTeleport") == 1) {
					center = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dir = getUnitVector(center, xGetVector(dPlayerData, xPlayerLurePos));
					trSetUnitOrientation(dir, vector(0,1,0), true);
					cur = xsVectorSet(1,0,0) * xGetFloat(db, xStarseerCurrentRadius);
					for(i=8; >0) {
						pos = center + cur;
						cur = rotationMatrix(cur, 0.707107, 0.707107);
						next = trGetNextUnitScenarioNameNumber();
						trArmyDispatch("1,0", "Dwarf", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("Vortex Start Linked");
						trUnitSelectClear();
						trUnitSelect("" + next);
						trSetSelectedUpVector(xsVectorGetX(dir),-1,xsVectorGetZ(dir));
						trUnitSetAnimationPath("0,0,1,0,0,0,0");
					}
				}
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	pvpReattachPlayer();
}

void chooseStarseer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * STARSEER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Realignment";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Celestial Cleansing";
		rainIsUltimate = false;
		map("q", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(Q) Repel/Mass Teleport";
		lureIsUltimate = false;
	}
	xStarseerStar = xInitAddInt(db, "star0");
	xInitAddInt(db, "star1");
	xInitAddInt(db, "star2");
	xStarseerMeteorite = xInitAddInt(db, "meteorite0");
	xInitAddInt(db, "meteorite1");
	xInitAddInt(db, "meteorite2");
	xStarseerTargetRadius = xInitAddFloat(db, "targetRadius", 2.0);
	xStarseerCurrentRadius = xInitAddFloat(db, "currentRadius", 2.0);
	xStarseerAngle = xInitAddFloat(db, "angle");
	xStarseerHitbox = xInitAddInt(db, "hitbox");
	xStarseerLast = xInitAddInt(db, "last");
	xStarseerDir = xInitAddVector(db, "dir0");
	xInitAddVector(db, "dir1");
	xInitAddVector(db, "dir2");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, realignCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, RepelCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
}

void starseerModify(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 5000 - 12 * STARSEER;
	xSetPointer(dPlayerData, p);
	/* no attack for u */
	zSetProtoUnitStat("Oracle Hero", p, 27, 0);
	trQuestVarSet("p"+p+"starAngularVelocity",
		starAngularVelocity * (2.0 + xGetInt(dPlayerData, xPlayerProjectiles)) / 3.0);
	if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
		trQuestVarSet("p"+p+"starAngularVelocity", 2.0 * trQuestVarGet("p"+p+"starAngularVelocity"));
	}
	trQuestVarSet("p"+p+"starCosine", xsCos(trQuestVarGet("p"+p+"starAngularVelocity") * 0.3));
}

rule starseer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STARSEER + p, "starseerAlways");
		trEventSetHandler(1000 + 12 * STARSEER + p, "chooseStarseer");
		trEventSetHandler(5000 + 12 * STARSEER + p, "starseerModify");
		trQuestVarSet("p"+p+"starAngularVelocity", starAngularVelocity);
		trQuestVarSet("p"+p+"starCosine", xsCos(trQuestVarGet("p"+p+"starAngularVelocity") * 0.3));
	}
}
