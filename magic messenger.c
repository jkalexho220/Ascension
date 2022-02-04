void removeMagicMessenger(int p = 0) {
	for(x=3; >0) {
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x), true);
		trUnitChangeProtoUnit("Rockt");
	}
	removePlayerSpecific(p);
	/* the star units */
	yRemoveUpdateVar("p"+p+"characters", "star1");
	yRemoveUpdateVar("p"+p+"characters", "star2");
	yRemoveUpdateVar("p"+p+"characters", "star3");
	/* the last hit angle of the star */
	yRemoveUpdateVar("p"+p+"characters", "last1");
	yRemoveUpdateVar("p"+p+"characters", "last2");
	yRemoveUpdateVar("p"+p+"characters", "last3");
	yRemoveUpdateVar("p"+p+"characters", "targetRadius");
	yRemoveUpdateVar("p"+p+"characters", "currentRadius");
	yRemoveUpdateVar("p"+p+"characters", "angle");
	yRemoveUpdateVar("p"+p+"characters", "hitbox");
	yRemoveUpdateVar("p"+p+"characters", "last");
}

void magicMessengerAlways(int eventID = -1) {
	int p = eventID - 12 * MAGICMESSENGER;
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

			if (trQuestVarGet("p"+p+"starsAlign") == 0) {
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
							if (trQuestVarGet("angleDiff") < 0.939692) {
								ySetVar("p"+p+"characters", "last"+x, fModulo(6.283185, current + 0.349065));
								trVectorSetFromAngle("prevPos", yGetVar("p"+p+"characters", "last"+x));
								trQuestVarSet("angleDiff", 0.939692);
							}
						}
						trVectorScale("dir", 3.33 * yGetVar("p"+p+"characters", "currentRadius"));
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x));
						if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Cinematic Block")) {
							trMutateSelected(kbGetProtoUnitID("Outpost"));
							trSetSelectedScale(0,0,0);
						}
						trSetSelectedUpVector(trQuestVarGet("dirX"),0.2,trQuestVarGet("dirZ"));
					}
					current = fModulo(6.283185, current + 2.094395);
				}

				/* collision detection for one star */
				
				trQuestVarSet("outer", xsPow(yGetVar("p"+p+"characters", "currentRadius") + 1.5, 2));
				trQuestVarSet("inner", xsPow(yGetVar("p"+p+"characters", "currentRadius") - 1.5, 2));
				trVectorSetUnitPos("center", "p"+p+"characters");

				amt = trQuestVarGet("p"+p+"attack") * trQuestVarGet("p"+p+"spellDamage");
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
	}


	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"revolution") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"revolutionNext")) {
			trQuestVarSet("p"+p+"revolutionNext", trQuestVarGet("p"+p+"revolutionNext") + trQuestVarGet("p"+p+"revolutionDelay"));
			trPlayerGrantResources(p, "favor", -1);
			if (trPlayerResourceCount(p, "favor") == 0) {
				trQuestVarSet("p"+p+"revolution", 0);
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					yDatabaseNext("p"+p+"characters");
					ySetVar("p"+p+"characters", "targetRadius", amt);
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
				trQuestVarGet("starAngularVelocity") * (3.0 + trQuestVarGet("p"+p+"projectiles")) / 4.0);
			trQuestVarSet("p"+p+"revolutionDelay", trQuestVarGet("revolutionDelay") / trQuestVarGet("p"+p+"ultimateCost"));
			trQuestVarSet("p"+p+"revolutionNext", trTimeMS());
			amt = trQuestVarGet("revolutionRadius") * trQuestVarGet("p"+p+"spellRange");
		} else {
			trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
		}
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			ySetVar("p"+p+"characters", "targetRadius", amt);
		}
		trSoundPlayFN("suckup3.wav","1",-1,"","");
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trSoundPlayFN("godpower.wav","1",-1,"","");
		trVectorSetUnitPos("target", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"starsAlign", 1);
		trQuestVarSet("p"+p+"soundNext", trTimeMS() + 1000);
		trQuestVarSet("p"+p+"soundCount", 3);
		trSoundPlayFN("suckup1.wav","1",-1,"","");
		for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			trVectorSetUnitPos("pos", "p"+p+"characters");
			ySetVar("p"+p+"characters", "alignAngle", angleBetweenVectors("pos", "target"));
			current = yGetVar("p"+p+"characters", "angle");
			for(x=1; <=3) {
				ySetVar("p"+p+"characters", "radius"+x, yGetVar("p"+p+"characters", "currentRadius"));
				ySetVar("p"+p+"characters", "last"+x, current);
				current = fModulo(6.283185, current + 2.094395);
			}
		}
	}

	ySetPointer("enemies", index);
	xsSetContextPlayer(old);
}

void chooseMagicMessenger(int eventID = -1) {
	int p = eventID - 1000 - 12 * MAGICMESSENGER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Flashbang";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Revolution";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Escape";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("realignCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("escapeCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

void magicMessengerModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * MAGICMESSENGER;
	/* no attack for u */
	if (trQuestVarGet("p"+p+"revolution") == 1) {
		trQuestVarSet("p"+p+"starAngularVelocity", 
			trQuestVarGet("starAngularVelocity") * (3.0 + trQuestVarGet("p"+p+"projectiles")) / 4.0);
	} else {
		trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
	}
}

rule magicMessenger_init
active
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("starAngularVelocity", 6.283185 / 4.0);
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * MAGICMESSENGER + p, "magicMessengerAlways");
		trEventSetHandler(1000 + 12 * MAGICMESSENGER + p, "chooseMagicMessenger");
		trEventSetHandler(5000 + 12 * MAGICMESSENGER + p, "magicMessengerModify");
		trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
	}

	trQuestVarSet("realignCooldown", 5);
	trQuestVarSet("realignRadius", 15);

	trQuestVarSet("journeyCooldown", 15);
	trQuestVarSet("journeyRadius", 15);
	trQuestVarSet("journeyDuration", 4);

	trQuestVarSet("revolutionCost", 5);
	trQuestVarSet("revolutionDelay", 1000.0 / trQuestVarGet("revolutionCost"));
	trQuestVarSet("revolutionDamage", 50);
	trQuestVarSet("revolutionRadius", 12);
}