void removeMagicMessenger(int p = 0) {
	for(x=3; >0) {
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x), true);
		trUnitChangeProtoUnit("Rocket");
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "valkyrie"+x), true);
		trUnitChangeProtoUnit("Rocket");
	}
	removePlayerSpecific(p);
	/* the star units */
	for(x=3; >0) {
		yRemoveUpdateVar("p"+p+"characters", "star"+x);
		yRemoveUpdateVar("p"+p+"characters", "valkyrie"+x);
		yRemoveUpdateVar("p"+p+"characters", "last"+x);
	}
	yRemoveUpdateVar("p"+p+"characters", "targetRadius");
	yRemoveUpdateVar("p"+p+"characters", "currentRadius");
	yRemoveUpdateVar("p"+p+"characters", "angle");
	yRemoveUpdateVar("p"+p+"characters", "hitbox");
	yRemoveUpdateVar("p"+p+"characters", "last");
	yRemoveUpdateVar("p"+p+"characters", "slipstreamSFX");
}

void magicMessengerAlways(int eventID = -1) {
	int p = eventID - 12 * STARSEER;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	for (y=yGetDatabaseCount("p"+p+"characters"); > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeMagicMessenger(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_NORMAL) {
				trPlayerGrantResources(p, "favor", 5);
			}
			
			current = trTimeMS() - yGetVar("p"+p+"characters", "last");
			ySetVar("p"+p+"characters", "last", trTimeMS());

			amt = trQuestVarGet("p"+p+"starAngularVelocity") * 0.001 * current;
			ySetVar("p"+p+"characters", "angle", fModulo(6.283185, yGetVar("p"+p+"characters", "angle") - amt));
			if (yGetVar("p"+p+"characters", "targetRadius") == 0) {
				ySetVar("p"+p+"characters", "targetRadius", 3);
				ySetVar("p"+p+"characters", "currentRadius", 3);
			} else {
				dist = yGetVar("p"+p+"characters", "targetRadius") - yGetVar("p"+p+"characters", "currentRadius");
				if (xsAbs(dist) > 0.1) {
					dist = 2.0 * dist * 0.001 * current;
					ySetVar("p"+p+"characters", "currentRadius", yGetVar("p"+p+"characters", "currentRadius") + dist);
				} else if (xsAbs(dist) > 0) {
					ySetVar("p"+p+"characters", "currentRadius", yGetVar("p"+p+"characters", "targetRadius"));
				}
			}

			hit = 1 + yGetVar("p"+p+"characters", "hitbox");
			if (hit > 3) {
				hit = 1;
			}
			ySetVar("p"+p+"characters", "hitbox", hit);

			current = yGetVar("p"+p+"characters", "angle");
			for(x=1; <=3) {
				if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
					id = kbGetBlockID(""+1*yGetVar("p"+p+"characters", "valkyrie"+x));
					if (id == -1 || yGetVar("p"+p+"characters", "valkyrie"+x) == 0) {
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
								kbGetProtoUnitID("Cinematic Block"),yGetVarName("p"+p+"characters", "valkyrie"+x));
					}
				}
				id = kbGetBlockID(""+1*yGetVar("p"+p+"characters", "star"+x));
				if (id == -1 || yGetVar("p"+p+"characters", "star"+x) == 0) {
					if (trQuestVarGet("spyFound") == trQuestVarGet("spyFind")) {
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
							kbGetProtoUnitID("Cinematic Block"),yGetVarName("p"+p+"characters", "star"+x));
					}
				} else {
					trVectorSetFromAngle("dir", current);
					if (x == hit) {
						/* this is the hitbox for this iteration */
						trQuestVarSet("currentAngle", current);
						trQuestVarSet("curPosx", trQuestVarGet("dirx"));
						trQuestVarSet("curPosz", trQuestVarGet("dirz"));
						trVectorSetFromAngle("prevPos", yGetVar("p"+p+"characters", "last"+x));
						trQuestVarSet("angleDiff", dotProduct("curPos", "prevPos"));
						/*
						cos(20) is 0.939692 
						if the previous hitbox is too far back, we reel it back in
						*/
						amt = xsCos(trQuestVarGet("p"+p+"starAngularVelocity") * 0.2);
						if (trQuestVarGet("angleDiff") < amt) {
							ySetVar("p"+p+"characters", "last"+x, fModulo(6.283185, current + trQuestVarGet("p"+p+"starAngularVelocity") * 0.2));
							trVectorSetFromAngle("prevPos", yGetVar("p"+p+"characters", "last"+x));
							trQuestVarSet("angleDiff", amt);
						}
					}
					trVectorScale("dir", yGetVar("p"+p+"characters", "currentRadius"));
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x));
					if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Cinematic Block")) {
						trMutateSelected(kbGetProtoUnitID("Outpost"));
						trSetSelectedScale(0,0,0);
					}
					trSetSelectedUpVector(3.33 * trQuestVarGet("dirX"),0.2,3.33 * trQuestVarGet("dirZ"));
					if (trQuestVarGet("p"+p+"revolution") == 1) {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "valkyrie"+x), true);
						trSetSelectedUpVector(0.5 * trQuestVarGet("dirX"), 0, 0.5 * trQuestVarGet("dirZ"));
					}
				}
				current = fModulo(6.283185, current + 2.094395);
			}

			/* collision detection for one star */
			
			trQuestVarSet("outer", xsPow(yGetVar("p"+p+"characters", "currentRadius") + 1.5, 2));
			trQuestVarSet("inner", xsPow(yGetVar("p"+p+"characters", "currentRadius") - 1.5, 2));
			trVectorSetUnitPos("center", "p"+p+"characters");

			amt = trQuestVarGet("revolutionDamage") * trQuestVarGet("p"+p+"spellDamage");
			target = 0;
			for(x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else {
					dist = zDistanceToVectorSquared("enemies", "center");
					if (dist < trQuestVarGet("outer") && dist > trQuestVarGet("inner")) {
						/* enemy is within the donut */
						trVectorSetUnitPos("pos", "enemies");
						trVectorQuestVarSet("dir", zGetUnitVector("center", "pos"));
						/* if enemy is in between the old and new positions, that's a hit */
						trQuestVarSet("curDiff", dotProduct("dir", "curPos"));
						if (trQuestVarGet("curDiff") > trQuestVarGet("angleDiff")) {
							if (dotProduct("dir", "prevPos") > trQuestVarGet("angleDiff")) {
								/* HIT */
								trUnitHighlight(0.2, false);
								trPlayerGrantResources(p, "favor", 1);
								damageEnemy(p, amt, true);
								if (trQuestVarGet("curDiff") > trQuestVarGet("angleDiff")) {
									target = 1;
								}
							}
						}
					}
				}
			}

			/* the star hit something so we update the prev position to be the current position */
			if (target == 1) {
				ySetVar("p"+p+"characters", "last"+hit, trQuestVarGet("currentAngle"));
				trQuestVarSetFromRand("sound", 1, 4, true);
				trSoundPlayFN("swordonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
			
		}
	}
	
	if (yGetDatabaseCount("p"+p+"frostbolts") >0) {
		yDatabaseNext("p"+p+"frostbolts");
		trVectorSetUnitPos("pos", "p"+p+"frostbolts");
		yVarToVector("p"+p+"frostbolts", "prev");
		yVarToVector("p"+p+"frostbolts", "dir");
		hit = 0;
		vectorToGrid("pos", "loc");
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			hit = 1;
		} else {
			dist = zDistanceBetweenVectors("pos", "prev");
			for (x=yGetDatabaseCount("enemies"); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision("enemies", "prev", "dir", dist + 2.0, 4.0)) {
					hit = 1;
					break;
				}
			}
		}
		
		if (hit == 1) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"frostbolts", true);
			trUnitChangeProtoUnit("Lampades Bolt");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"frostbolts", true);
			trMutateSelected(kbGetProtoUnitID("Frost Drift"));
			trSetSelectedScale(1.25 * trQuestVarGet("p"+p+"spellRange"),1,1.6*trQuestVarGet("p"+p+"spellRange"));
			dist = xsPow(trQuestVarGet("frostboltRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
			amt = trQuestVarGet("frostboltDamage") * trQuestVarGet("p"+p+"spellDamage");
			for (x=yGetDatabaseCount("enemies"); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					hit = hit + 1;
					damageEnemy(p, amt);
					stunUnit("enemies", 2.0 * trQuestVarGet("p"+p+"spellDuration"), p);
				}
			}
			trPlayerGrantResources(p, "favor", hit);
			trSoundPlayFN("icemono.wav","1",-1,"","");
			yRemoveFromDatabase("p"+p+"frostbolts");
		} else {
			ySetVarFromVector("p"+p+"frostbolts", "prev", "pos");
		}
		
	}

	
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("lampadesshoot.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
				removeMagicMessenger(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("pos", "p"+p+"wellPos"));
				vectorSetAsTargetVector("dest", "pos", "p"+p+"wellpos", 300.0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
				trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
				trUnitMoveToPoint(trQuestVarGet("destx"),0,trQuestVarGet("destz"), -1, false);
				yAddToDatabase("p"+p+"frostbolts", "next");
				yAddUpdateVar("p"+p+"frostbolts", "timeout", trTimeMS() + 5000);
				yAddUpdateVar("p"+p+"frostbolts", "prevx", trQuestVarGet("posx"));
				yAddUpdateVar("p"+p+"frostbolts", "prevz", trQuestVarGet("posz"));
				yAddUpdateVar("p"+p+"frostbolts", "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar("p"+p+"frostbolts", "dirz", trQuestVarGet("dirz"));
			}
		}
	}
	

	if (trQuestVarGet("p"+p+"revolution") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"revolutionNext")) {
			trQuestVarSet("p"+p+"revolutionNext", trQuestVarGet("p"+p+"revolutionNext") + trQuestVarGet("p"+p+"revolutionDelay"));
			trPlayerGrantResources(p, "favor", -1);
			if (trPlayerResourceCount(p, "favor") == 0) {
				trQuestVarSet("p"+p+"revolution", 0);
				for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
					yDatabaseNext("p"+p+"characters");
					ySetVar("p"+p+"characters", "targetRadius", 3);
					for(x=3; >0) {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "valkyrie"+x), true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
				trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
			}
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"revolution", 1 - trQuestVarGet("p"+p+"revolution"));
		amt = 3;
		if (trQuestVarGet("p"+p+"revolution") == 1) {
			trQuestVarSet("p"+p+"starAngularVelocity", 
				trQuestVarGet("starAngularVelocity") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0);
			trQuestVarSet("p"+p+"revolutionDelay", trQuestVarGet("revolutionDelay") / trQuestVarGet("p"+p+"ultimateCost"));
			trQuestVarSet("p"+p+"revolutionNext", trTimeMS());
			amt = trQuestVarGet("revolutionRadius") * trQuestVarGet("p"+p+"spellRange");
		} else {
			trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
		}
		for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			ySetVar("p"+p+"characters", "targetRadius", amt);
			for(x=3; >0) {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "valkyrie"+x), true);
				if (trQuestVarGet("p"+p+"revolution") == 1) {
					trMutateSelected(kbGetProtoUnitID("Valkyrie"));
					trSetSelectedScale(0,0,0);
					trUnitSetAnimationPath("1,0,0,0,0,0,0");
					trUnitOverrideAnimation(15,0,true,false,-1);
				} else {
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		}
		trSoundPlayFN("suckup3.wav","1",-1,"","");
	}

	if (trQuestVarGet("p"+p+"slipstream") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"slipstreamTimeout")) {
			trQuestVarSet("p"+p+"slipstream", 0);
			zSetProtoUnitStat("Javelin Cavalry Hero", p, 1, trQuestVarGet("p"+p+"speed"));
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
					removeMagicMessenger(p);
				} else {
					if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "slipstreamSFX")) >= 0) {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "slipstreamSFX"), true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trSoundPlayFN("meteorsplash.wav","1",-1,"","");
		zSetProtoUnitStat("Javelin Cavalry Hero", p, 1, trQuestVarGet("p"+p+"speed") + trQuestVarGet("slipstreamBoost"));
		trQuestVarSet("p"+p+"slipstream", 1);
		trQuestVarSet("p"+p+"slipstreamTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("slipstreamDuration") * trQuestVarGet("p"+p+"spellDuration"));
		hit = 0;
		dist = xsPow(trQuestVarGet("slipstreamRange") * trQuestVarGet("p"+p+"spellRange"), 2);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
				removeMagicMessenger(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "slipstreamSFX")) >= 0) {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "slipstreamSFX"), true);
					trMutateSelected(kbGetProtoUnitID("Valkyrie"));
					trSetSelectedScale(0,0,0);
					trUnitSetAnimationPath("1,0,0,0,0,0,0");
					trUnitOverrideAnimation(15,0,true,false,-1);
				} else {
					debugLog("valkyrie missing!");
					spyEffect(1*trQuestVarGet("p"+p+"characters"), 
						kbGetProtoUnitID("Cinematic Block"), yGetVarName("p"+p+"characters", "slipstreamSFX"));
				}
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Meteor Impact Water");
				for(y=yGetDatabaseCount("enemies"); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
						trVectorSetUnitPos("target", "enemies");
						trVectorQuestVarSet("dir", 
							zGetUnitVector("pos", "target", trQuestVarGet("slipstreamRange") * trQuestVarGet("p"+p+"spellRange")));
						trQuestVarSet("destx", trQuestVarGet("posx") + trQuestVarGet("dirx"));
						trQuestVarSet("destz", trQuestVarGet("posz") + trQuestVarGet("dirz"));
						launchUnit("enemies", "dest");
						hit = hit + 1;
					}
				}
			}
		}
		trPlayerGrantResources(p, "favor", hit);
	}

	ySetPointer("enemies", index);
	xsSetContextPlayer(old);
}

void chooseMagicMessenger(int eventID = -1) {
	int p = eventID - 1000 - 12 * STARSEER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Frostbolt";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Revolution";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Slipstream";
		lureIsUltimate = false;
	}
	zSetProtoUnitStat("Lampades Bolt", p, 1, 10);
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("frostboltCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("slipstreamCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

void magicMessengerModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * STARSEER;
	/* no attack for u */
	if (trQuestVarGet("p"+p+"revolution") == 1) {
		trQuestVarSet("p"+p+"starAngularVelocity", 
			trQuestVarGet("starAngularVelocity") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0);
	} else {
		trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
	}

	if (trQuestVarGet("p"+p+"slipstream") == 1) {
		zSetProtoUnitStat("Javelin Cavalry Hero", p, 1, trQuestVarGet("p"+p+"speed") + trQuestVarGet("slipstreamBoost"));
	}
}

rule magicMessenger_init
active
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("starAngularVelocity", 6.283185 / 4.0);
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STARSEER + p, "magicMessengerAlways");
		trEventSetHandler(1000 + 12 * STARSEER + p, "chooseMagicMessenger");
		trEventSetHandler(5000 + 12 * STARSEER + p, "magicMessengerModify");
		trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
	}
	trQuestVarSet("frostboltCooldown", 8);
	trQuestVarSet("frostboltRadius", 4);
	trQuestVarSet("frostboltDamage", 40);

	trQuestVarSet("slipstreamCooldown", 15);
	trQuestVarSet("slipstreamRange", 15);
	trQuestVarSet("slipstreamDuration", 4);
	trQuestVarSet("slipstreamBoost", 3);

	trQuestVarSet("revolutionCost", 5);
	trQuestVarSet("revolutionDelay", 1000.0 / trQuestVarGet("revolutionCost"));
	trQuestVarSet("revolutionDamage", 50);
	trQuestVarSet("revolutionRadius", 12);
}