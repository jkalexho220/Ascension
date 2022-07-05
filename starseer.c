float starBaseDamage = 50;

int realignCooldown = 5;
float realignRadius = 15;

int RepelCooldown = 15;
float RepelRange = 15;

float eventHorizonCost = 60;
float eventHorizonDuration = 10;

float starAngularVelocity = 1.570796; // 6.283185 / 4.0

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

void starseerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * STARSEER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
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
					angleDiff = dotProduct(cur, prev);
				}
				pos = dir * xGetFloat(db, xStarseerCurrentRadius);
				xUnitSelect(db, xStarseerStar + x);
				trSetSelectedUpVector(3.33 * xsVectorGetX(pos),0.2,3.33 * xsVectorGetZ(pos));
				if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
					xUnitSelect(db, xStarseerMeteorite + x);
					trSetSelectedUpVector(1.0 * xsVectorGetX(pos),0,1.0 * xsVectorGetZ(pos));
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
								if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
									stunUnit(dEnemies, 1.5, p);
								} else {
									gainFavor(p, 1);
								}
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
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"Repel", 1);
		trQuestVarSet("p"+p+"RepelTimeout", trTimeMS() + 1200);
		trSoundPlayFN("oracledone.wav","1",-1,"","");
		trSoundPlayFN("implode reverse.wav","1",-1,"","");
		for(x=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeStarseer(p);
			} else {
				trMutateSelected(kbGetProtoUnitID("Oracle Hero"));
				trUnitSetStance("Passive");
				trUnitOverrideAnimation(52, 0, false, false, -1);
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"eventHorizonTimeout")) {
			trQuestVarSet("p"+p+"eventHorizon", 0);
			for(y=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				for(x=0; < 3) {
					xUnitSelect(db, xStarseerMeteorite + x);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
			trQuestVarSet("p"+p+"starAngularVelocity",
				starAngularVelocity * (2.0 + xGetInt(dPlayerData, xPlayerProjectiles)) / 3.0);
			trQuestVarSet("p"+p+"starCosine", xsCos(trQuestVarGet("p"+p+"starAngularVelocity") * 0.3));
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		gainFavor(p, 0.0 - eventHorizonCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trQuestVarSet("p"+p+"eventHorizon", 1);
		trQuestVarSet("p"+p+"eventHorizonTimeout",
			trTimeMS() + 1000 * eventHorizonDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		trQuestVarSet("p"+p+"starAngularVelocity",
			2.0 * starAngularVelocity * (2.0 + xGetInt(dPlayerData, xPlayerProjectiles)) / 3.0);
		trQuestVarSet("p"+p+"starCosine", xsCos(trQuestVarGet("p"+p+"starAngularVelocity") * 0.3));
		for(y=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			for(x=0; < 3) {
				xUnitSelect(db, xStarseerMeteorite + x);
				trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
				trSetSelectedScale(0,0,0);
				trUnitSetAnimationPath("0,0,1,0,0,0,0");
			}
		}
		trSoundPlayFN("cinematics\24_in\magic.mp3", "5", -1, "","");
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
		map("q", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(Q) Repel";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Event Horizon";
		lureIsUltimate = true;
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
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, eventHorizonCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,RepelCooldown);
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
