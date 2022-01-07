void removeStarseer(int p = 0) {
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

void starseerAlways(int eventID = -1) {
	int p = eventID - 12 * STARSEER;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	for (y=yGetDatabaseCount("p"+p+"characters"); > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeStarseer(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_SPECIAL) {
				target = kbUnitGetTargetUnitID(id);
				trVectorSetUnitPos("pos", "p"+p+"characters");
				for(x=yGetDatabaseCount("enemies"); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (id == target) {
						trVectorSetUnitPos("dir", "enemies");
						trVectorQuestVarSet("dest", zGetUnitVector("pos", "dir", 15.0 * trQuestVarGet("p"+p+"spellRange")));
						trQuestVarSet("destx", trQuestVarGet("destx") + trQuestVarGet("posx"));
						trQuestVarSet("destZ", trQuestVarGet("destZ") + trQuestVarGet("posZ"));
						launchUnit("enemies", "dest");
						trSoundPlayFN("shockwave.wav","1",-1,"","");
						break;
					}
				}
			}
			
			current = trTimeMS() - yGetVar("p"+p+"characters", "last");
			ySetVar("p"+p+"characters", "last", trTimeMS());

			if (trQuestVarGet("p"+p+"starsAlign") == 0) {
				amt = trQuestVarGet("p"+p+"starAngularVelocity") * 0.001 * current;
				ySetVar("p"+p+"characters", "angle", fModulo(6.283185, yGetVar("p"+p+"characters", "angle") - amt));
				if (yGetVar("p"+p+"characters", "targetRadius") == 0) {
					ySetVar("p"+p+"characters", "targetRadius", 10);
					ySetVar("p"+p+"characters", "currentRadius", 10);
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

				amt = trQuestVarGet("starBaseDamage") * trQuestVarGet("p"+p+"spellDamage");
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
			} else if (trQuestVarGet("p"+p+"starsAlign") == 1) {
				trVectorSetFromAngle("target", yGetVar("p"+p+"characters", "alignAngle"));
				trVectorQuestVarSet("center", vector(0,0,0));
				hit = 0;
				for(x=1; <=3) {
					trVectorSetFromAngle("pos", yGetVar("p"+p+"characters", "last"+x));
					trVectorScale("pos", yGetVar("p"+p+"characters", "radius"+x));
					trQuestVarSet("destx", trQuestVarGet("targetx") * 3.0 * x);
					trQuestVarSet("destz", trQuestVarGet("targetz") * 3.0 * x);
					
					dist = zDistanceBetweenVectors("dest", "pos");
					if (dist > 0.3) {
						dist = 3.0 * dist * 0.001 * current;
						trVectorQuestVarSet("dir", zGetUnitVector("pos", "dest", dist));
						trQuestVarSet("posX", trQuestVarGet("posX") + trQuestVarGet("dirX"));
						trQuestVarSet("posZ", trQuestVarGet("posZ") + trQuestVarGet("dirZ"));
					} else {
						trQuestVarSet("posX", trQuestVarGet("destX"));
						trQuestVarSet("posZ", trQuestVarGet("destZ"));
						hit = hit + 1;
					}
					
					ySetVar("p"+p+"characters", "last"+x, angleBetweenVectors("center", "pos"));
					ySetVar("p"+p+"characters", "radius"+x, zDistanceBetweenVectors("pos", "center"));
					trVectorScale("pos", 3.33);
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x));
					trSetSelectedUpVector(trQuestVarGet("posX"),0.2,trQuestVarGet("posZ"));
				}
				if (hit == 3) {
					trQuestVarSet("p"+p+"starsAlign", 2);
					trQuestVarSet("p"+p+"drainLast", trTimeMS());
					trQuestVarSet("p"+p+"raligunDamage", 0);
				}
			} else if (trQuestVarGet("p"+p+"starsAlign") == 3) {
				if (trUnitVisToPlayer()) {
					trCameraShake(1.0, 1.5);
				}
				trVectorSetUnitPos("start", "p"+p+"characters");
				trVectorSetFromAngle("dir", yGetVar("p"+p+"characters", "alignAngle"));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trUnitHighlight(5.0, false);
				trSetUnitOrientation(xsVectorSet(0.0 - trQuestVarGet("dirx"),0, 0.0 - trQuestVarGet("dirz")), vector(0,1,0), true);
				yAddToDatabase("playerLasers", "next");
				yAddUpdateVar("playerLasers", "timeout", trTimeMS() + 2500);
				yAddUpdateVar("playerLasers", "range", 50);
				amt = xsPow(trQuestVarGet("starsAlignWidth") * trQuestVarGet("p"+p+"spellRange"), 2);
				for(x=yGetDatabaseCount("enemies"); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else {
						dist = zDistanceToVector("enemies", "start");
						trQuestVarSet("hitboxX", trQuestVarGet("startX") + dist * trQuestVarGet("dirX"));
						trQuestVarSet("hitboxZ", trQuestVarGet("startZ") + dist * trQuestVarGet("dirZ"));
						if (zDistanceToVectorSquared("enemies", "hitbox") < amt) {
							damageEnemy(p, trQuestVarGet("p"+p+"railgunDamage"), true);
						}
					}
				}
				trQuestVarSet("endx", trQuestVarGet("startx") + trQuestVarGet("dirx"));
				trQuestVarSet("endz", trQuestVarGet("startz") + trQuestVarGet("dirz"));
				vectorSetAsTargetVector("target", "start", "end", 300.0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Lampades Bolt");
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				trUnitOverrideAnimation(6,0,true,false,-1);
				trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"));

				current = yGetVar("p"+p+"characters", "angle");
				for(x=1; <= 3) {
					ySetVar("p"+p+"characters", "last"+x, fModulo(6.283185, current + 0.349065));
					current = fModulo(6.283185, current + 2.094395);
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"starsAlign") == 3) {
		trQuestVarSet("p"+p+"starsAlign", 0);
		trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
	} else if (trQuestVarGet("p"+p+"starsAlign") == 2) {
		if (trTimeMS() > trQuestVarGet("p"+p+"drainLast")) {
			trQuestVarSet("p"+p+"drainLast", trQuestVarGet("p"+p+"drainLast") + 100.0 / trQuestVarGet("p"+p+"ultimateCost"));
			trPlayerGrantResources(p, "favor", -3);
			trQuestVarSet("p"+p+"railgunDamage", trQuestVarGet("p"+p+"railgunDamage") + 90);
			if (trPlayerResourceCount(p, "favor") == 0) {
				trQuestVarSet("p"+p+"starsAlign", 3);
				trQuestVarSet("p"+p+"railgunDamage", trQuestVarGet("p"+p+"railgunDamage") * trQuestVarGet("p"+p+"spellDamage"));
			}
		}
	}

	if (trQuestVarGet("p"+p+"starsAlign") > 0 && trQuestVarGet("p"+p+"starsAlign") < 3) {
		if (trTimeMS() > trQuestVarGet("p"+p+"soundNext")) {
			trQuestVarSetFromRand("sound", 1, 3, true);
			trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			trQuestVarSet("p"+p+"soundCount", 1 + trQuestVarGet("p"+p+"soundCount"));
			trQuestVarSet("p"+p+"soundNext", trTimeMS() + 3000 / trQuestVarGet("p"+p+"soundCount"));
		}
	}


	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			amt = xsMin(trQuestVarGet("realignRadius") * trQuestVarGet("p"+p+"spellRange"),
				zDistanceToVector("p"+p+"characters", "p"+p+"wellPos"));
			amt = xsMax(2, amt);
			ySetVar("p"+p+"characters", "targetRadius", amt);
		}
		trSoundPlayFN("suckup3.wav","1",-1,"","");
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trSoundPlayFN("arkantosarrive.wav","1",-1,"","");
		trSoundPlayFN("ui\thunder1.wav","1",-1,"","");
		for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			trVectorSetUnitPos("center", "p"+p+"characters");
			amt = yGetVar("p"+p+"characters", "angle");
			dist = xsPow(trQuestVarGet("solarFlareRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
			for(x=3; >0) {
				trVectorSetFromAngle("dir", amt);
				amt = fModulo(6.283185, amt + 2.094395);
				trVectorScale("dir", yGetVar("p"+p+"characters", "currentRadius"));
				trQuestVarSet("posX", trQuestVarGet("centerX") + trQuestVarGet("dirX"));
				trQuestVarSet("posZ", trQuestVarGet("centerZ") + trQuestVarGet("dirZ"));
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect(""+p+",0");
				trDamageUnitPercent(100);
				trUnitChangeProtoUnit("Implode Sphere Effect");
				for(i=yGetDatabaseCount("enemies"); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
						damageEnemy(p, trQuestVarGet("solarFlareDamage") * trQuestVarGet("p"+p+"spellDamage"));
						if (trUnitAlive()) {
							stunUnit("enemies", 2.0, p);
						}
					}
				}
			}
		}
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

	xsSetContextPlayer(old);
}

void chooseStarseer(int eventID = -1) {
	int p = eventID - 1000 - 12 * STARSEER;
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Realignment";
		wellIsUltimate = false;
		map("q", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(Q) Solar Flare";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) The Stars Align";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("realignCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("starsAlignCooldown"));
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("starsAlignCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("solarFlareCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void starseerModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * STARSEER;
	/* no attack for u */
	zSetProtoUnitStat("Oracle Hero", p, 27, 0);
	trQuestVarSet("p"+p+"starAngularVelocity", 
		trQuestVarGet("starAngularVelocity") * (3.0 + trQuestVarGet("p"+p+"projectiles")) / 4.0);
}

rule starseer_init
active
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("starAngularVelocity", 6.283185 / 4.0);
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STARSEER + p, "starseerAlways");
		trEventSetHandler(1000 + 12 * STARSEER + p, "chooseStarseer");
		trEventSetHandler(5000 + 12 * STARSEER + p, "starseerModify");
		trQuestVarSet("p"+p+"starAngularVelocity", trQuestVarGet("starAngularVelocity"));
	}

	trQuestVarSet("starBaseDamage", 30);


	trQuestVarSet("realignCooldown", 5);
	trQuestVarSet("realignRadius", 15);

	trQuestVarSet("solarFlareCooldown", 15);
	trQuestVarSet("solarFlareDamage", 50);
	trQuestVarSet("solarFlareRadius", 5);

	trQuestVarSet("starsAlignDamage", 10);
	trQuestVarSet("starsAlignWidth", 3);
	trQuestVarSet("starsAlignCost", 50);
	trQuestVarSet("starsAlignCooldown", 10);
}