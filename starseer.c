void removeStarseer(int p = 0) {
	for(x=3; >0) {
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "star"+x), true);
		trUnitChangeProtoUnit("Rocket");
		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"characters", "Meteorite"+x), true);
		trUnitChangeProtoUnit("Rocket");
	}
	removePlayerSpecific(p);
	/* the star units */
	for(x=3; >0) {
		yRemoveUpdateVar("p"+p+"characters", "star"+x);
		yRemoveUpdateVar("p"+p+"characters", "Meteorite"+x);
		yRemoveUpdateVar("p"+p+"characters", "last"+x);
	}
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
	int index = yGetPointer("enemies");
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
			
			current = trTimeMS() - yGetVar("p"+p+"characters", "last");
			ySetVar("p"+p+"characters", "last", trTimeMS());
			
			amt = trQuestVarGet("p"+p+"starAngularVelocity") * 0.001 * current;
			ySetVar("p"+p+"characters", "angle", fModulo(6.283185, yGetVar("p"+p+"characters", "angle") - amt));
			if (yGetVar("p"+p+"characters", "targetRadius") == 0) {
				ySetVar("p"+p+"characters", "targetRadius", 2);
				ySetVar("p"+p+"characters", "currentRadius", 2);
			} else {
				dist = yGetVar("p"+p+"characters", "targetRadius") - yGetVar("p"+p+"characters", "currentRadius");
				if (xsAbs(dist) > 0.1) {
					dist = 2.0 * dist * 0.001 * current;
					ySetVar("p"+p+"characters", "currentRadius", yGetVar("p"+p+"characters", "currentRadius") + dist);
				} else if (xsAbs(dist) > 0) {
					ySetVar("p"+p+"characters", "currentRadius", yGetVar("p"+p+"characters", "targetRadius"));
				}
			}
			
			if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
				amt = trQuestVarGet("realignRadius") * trQuestVarGet("p"+p+"spellRange") - 2.0;
				amt = (amt - yGetVar("p"+p+"characters", "currentRadius") + 2.0) / amt;
				zSetProtoUnitStat("Oracle Hero", p, 1, trQuestVarGet("p"+p+"speed") * (0.5 + amt));
			}
			
			hit = 1 + yGetVar("p"+p+"characters", "hitbox");
			if (hit > 3) {
				hit = 1;
			}
			ySetVar("p"+p+"characters", "hitbox", hit);
			
			current = yGetVar("p"+p+"characters", "angle");
			for(x=1; <=3) {
				if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
					id = kbGetBlockID(""+1*yGetVar("p"+p+"characters", "Meteorite"+x));
					if (id == -1 || yGetVar("p"+p+"characters", "Meteorite"+x) == 0) {
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
						kbGetProtoUnitID("Cinematic Block"),yGetVarName("p"+p+"characters", "Meteorite"+x));
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
					if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "Meteorite"+x), true);
						//trSetSelectedUpVector(0.2 * trQuestVarGet("dirX"), 0, 0.2 * trQuestVarGet("dirZ"));
						trSetSelectedUpVector(1.0 * trQuestVarGet("dirX"),0,1.0 * trQuestVarGet("dirZ"));
					}
				}
				current = fModulo(6.283185, current + 2.094395);
			}
			
			/* collision detection for one star */
			
			trQuestVarSet("outer", xsPow(yGetVar("p"+p+"characters", "currentRadius") + 1.5, 2));
			trQuestVarSet("inner", xsPow(yGetVar("p"+p+"characters", "currentRadius") - 1.5, 2));
			trVectorSetUnitPos("center", "p"+p+"characters");
			
			amt = trQuestVarGet("starbaseDamage") * trQuestVarGet("p"+p+"spellDamage");
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
								if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
									stunUnit("enemies", 1.5, p);
								}
								if (trQuestVarGet("p"+p+"eventHorizon") == 0) {
									gainFavor(p, 1);
								}
								trUnitHighlight(0.2, false);
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
				trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
	}
	
	
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters", true);
			trUnitSetStance("Passive");
			amt = xsMin(trQuestVarGet("realignRadius") * trQuestVarGet("p"+p+"spellRange"),
			zDistanceToVector("p"+p+"characters", "p"+p+"wellPos"));
			amt = xsMax(2, amt);
			ySetVar("p"+p+"characters", "targetRadius", amt);
		}
		trSoundPlayFN("suckup3.wav","1",-1,"","");
	}
	
	if (trQuestVarGet("p"+p+"Repel") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"RepelTimeout")) {
			trQuestVarSet("p"+p+"RepelTimeout", trQuestVarGet("p"+p+"RepelTimeout") + 1000);
			trQuestVarSet("p"+p+"Repel", 2);
			hit = 0;
			dist = xsPow(trQuestVarGet("RepelRange") * trQuestVarGet("p"+p+"spellRange"), 2);
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
					removeStarseer(p);
				} else {
					trVectorSetUnitPos("pos", "p"+p+"characters");
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Tremor");
					for(y=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
							trVectorSetUnitPos("target", "enemies");
							trVectorQuestVarSet("dir",
								zGetUnitVector("pos", "target", trQuestVarGet("RepelRange") * trQuestVarGet("p"+p+"spellRange")));
								trQuestVarSet("destx", trQuestVarGet("posx") + trQuestVarGet("dirx"));
								trQuestVarSet("destz", trQuestVarGet("posz") + trQuestVarGet("dirz"));
								launchUnit("enemies", "dest");
								hit = hit + 1;
							}
						}
					}
				}
				gainFavor(p, hit);
			}
		} else if (trQuestVarGet("p"+p+"Repel") == 2) {
			if (trTimeMS() > trQuestVarGet("p"+p+"RepelTimeout")) {
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
						removeStarseer(p);
					} else {
						trVectorSetUnitPos("pos", "p"+p+"characters");
						trUnitOverrideAnimation(-1, 0, false, true, -1);
					}
				}
				trQuestVarSet("p"+p+"Repel", 0);
			}
		}
		
		if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
			trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
			trQuestVarSet("p"+p+"Repel", 1);
			trQuestVarSet("p"+p+"RepelTimeout", trTimeMS() + 1200);
			trSoundPlayFN("oracledone.wav","1",-1,"","");
			trSoundPlayFN("implode reverse.wav","1",-1,"","");
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
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
				for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
					yDatabaseNext("p"+p+"characters");
					for(x=3; >0) {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "Meteorite"+x), true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
				trQuestVarSet("p"+p+"starAngularVelocity",
					trQuestVarGet("starAngularVelocity") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0);
				}
			}
			
			if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
				trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
				gainFavor(p, 0.0 - trQuestVarGet("eventHorizonCost") * trQuestVarGet("p"+p+"ultimateCost"));
				trQuestVarSet("p"+p+"eventHorizon", 1);
				trQuestVarSet("p"+p+"eventHorizonTimeout",
					trTimeMS() + 1000 * trQuestVarGet("eventHorizonDuration") * trQuestVarGet("p"+p+"spellDuration"));
					trQuestVarSet("p"+p+"starAngularVelocity",
						2.0 * trQuestVarGet("starAngularVelocity") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0);
						for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
							yDatabaseNext("p"+p+"characters");
							for(x=3; >0) {
								trUnitSelectClear();
								trUnitSelect(""+1*yGetVar("p"+p+"characters", "Meteorite"+x), true);
								trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
								trSetSelectedScale(0,0,0);
								trUnitSetAnimationPath("0,0,1,0,0,0,0");
							}
						}
						trSoundPlayFN("cinematics\24_in\magic.mp3", "5", -1, "","");
					}
					
					ySetPointer("enemies", index);
					xsSetContextPlayer(old);
				}
				
				void chooseStarseer(int eventID = -1) {
					int p = eventID - 1000 - 12 * STARSEER;
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
					trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("realignCooldown"));
					trQuestVarSet("p"+p+"wellCost", 0);
					trQuestVarSet("p"+p+"lureCooldown", 1);
					trQuestVarSet("p"+p+"lureCost", trQuestVarGet("eventHorizonCost"));
					trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("RepelCooldown"));
					trQuestVarSet("p"+p+"rainCost", 0);
				}
				
				void starseerModify(int eventID = -1) {
					int p = eventID - 5000 - 12 * STARSEER;
					/* no attack for u */
					zSetProtoUnitStat("Oracle Hero", p, 27, 0);
					trQuestVarSet("p"+p+"starAngularVelocity",
						trQuestVarGet("starAngularVelocity") * (2.0 + trQuestVarGet("p"+p+"projectiles")) / 3.0);
						if (trQuestVarGet("p"+p+"eventHorizon") == 1) {
							trQuestVarSet("p"+p+"starAngularVelocity", 2.0 * trQuestVarGet("p"+p+"starAngularVelocity"));
						}
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
						
						trQuestVarSet("starBaseDamage", 50);
						
						
						trQuestVarSet("realignCooldown", 5);
						trQuestVarSet("realignRadius", 15);
						
						trQuestVarSet("RepelCooldown", 15);
						trQuestVarSet("RepelRange", 15);
						
						trQuestVarSet("eventHorizonCost", 60);
						trQuestVarSet("eventHorizonDuration", 10);
					}
