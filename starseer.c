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
			amt = trQuestVarGet("starAngularVelocity") * 0.001 * (trTimeMS() - yGetVar("p"+p+"characters", "last"));
			ySetVar("p"+p+"characters", "last", trTimeMS());
			ySetVar("p"+p+"characters", "angle", fModulo(6.283185, yGetVar("p"+p+"characters", "angle") - amt));

			if (yGetVar("p"+p+"characters", "targetRadius") == 0) {
				ySetVar("p"+p+"characters", "targetRadius", 10);
				ySetVar("p"+p+"characters", "currentRadius", 10);
			}

			hit = 1 + yGetVar("p"+p+"characters", "hitbox");
			if (hit > 3) {
				hit = 1;
			}
			ySetVar("p"+p+"characters", "hitbox", hit);

			current = yGetVar("p"+p+"characters", "angle");
			for(x=1; <=3) {
				id = kbGetBlockID(""+1*yGetVar("p"+p+"characters", "star"+x));
				if (id == -1) {
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

			amt = (trQuestVarGet("starBaseDamage") + trQuestVarGet("p"+p+"attack")) * trQuestVarGet("p"+p+"spellDamage");
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
				trQuestVarSetFromRand("sound", 1, 3, true);
				if (trQuestVarGet("sound") == 1) {
					trSoundPlayFN("automatonspcbirth.wav","1",-1,"","");
				} else {
					trSoundPlayFN("automatonspcbirth"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
			}

			/*
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_SPECIAL) {
				
			}
			*/
		}
	}


	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		
	}

	xsSetContextPlayer(old);
}

void chooseStarseer(int eventID = -1) {
	int p = eventID - 1000 - 12 * STARSEER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Realignment";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Solar Flare";
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

rule starseer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * STARSEER + p, "starseerAlways");
		trEventSetHandler(1000 + 12 * STARSEER + p, "chooseStarseer");
	}

	trQuestVarSet("starBaseDamage", 30);

	trQuestVarSet("starAngularVelocity", 6.283185 / 4.0);

	trQuestVarSet("realignCooldown", 5);
	trQuestVarSet("realignRadius", 15);

	trQuestVarSet("solarFlareCooldown", 15);
	trQuestVarSet("solarFlareDamage", 50);
	trQuestVarSet("solarFlareRadius", 3);

	trQuestVarSet("starsAlignDamage", 10);
	trQuestVarSet("starsAlignWidth", 3);
	trQuestVarSet("starsAlignCost", 50);
	trQuestVarSet("starsAlignCooldown", 10);
}