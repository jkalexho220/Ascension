void removeThroneShield(int p = 0) {
	removePlayerSpecific(p);
	if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
		int queen = trQuestVarGet("p"+p+"queen");
		if (queen > 0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+queen+"tether");
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		trQuestVarSet("p"+queen+"simp", 0);
		trQuestVarSet("p"+p+"queen", 0);
	}
	yRemoveUpdateVar("p"+p+"characters", "shieldSFX");
}

void throneShieldAlways(int eventID = -1) {
	int p = eventID - 12 * THRONESHIELD;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		target = 0;
		dist = 100;
		for(x=1; < ENEMY_PLAYER) {
			if (x == p) {
				continue;
			} else if (trQuestVarGet("p"+x+"dead") == 0) {
				current = zDistanceToVectorSquared("p"+x+"unit", "p"+p+"wellPos");
				if (current < dist) {
					target = x;
					dist = current;
				}
			}
		}
		if (target > 0) {
			if (trQuestVarGet("p"+target+"simp") > 0) {
				target = 0;
				if (trCurrentPlayer() == p) {
					trChatSend(0, "That player already has a partner!");
				}
			} else if (trQuestVarGet("p"+target+"class") == THRONESHIELD) {
				target = 0;
				if (trCurrentPlayer() == p) {
					trChatSend(0, "Cannot target another Throne Shield!");
				}
			} else {
				/* first we renounce our love for our first queen */
				if (trQuestVarGet("p"+p+"queen") > 0) {
					hit = trQuestVarGet("p"+p+"queen");
					trUnitSelectClear();
					trUnitSelectByQV("p"+hit+"tether");
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					trQuestVarSet("p"+hit+"simp", 0);
				}
				trQuestVarSet("p"+target+"simp", p);
				trQuestVarSet("p"+p+"queen", target);
				if (trCurrentPlayer() == p) {
					trChatSend(0, "<color=1,1,1>You have given your Knight's Vow to " + trStringQuestVarGet("p"+target+"name") + "!");
				}
				if (trCurrentPlayer() == target) {
					trChatSend(0, "<color=1,1,1>" + trStringQuestVarGet("p"+p+"name") + " has given you a Knight's Vow!");
				}
				xsSetContextPlayer(target);
				trQuestVarSet("p"+target+"currentHitpoints",
					kbUnitGetCurrentHitpoints(kbGetBlockID(""+1*trQuestVarGet("p"+target+"unit"))));
				xsSetContextPlayer(p);
				trSoundPlayFN("militarycreate.wav","1",-1,"","");
				trSoundPlayFN("herocreation.wav","1",-1,"","");
				if (kbGetBlockID(""+1*trQuestVarGet("p"+p+"tether")) == -1) {
					spyEffect(1*trQuestVarGet("p"+p+"unit"),kbGetProtoUnitID("Vortex Finish Linked"),"p"+p+"tether");
					trQuestVarSet("p"+p+"tetherReady", 0);
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"tether", true);
					trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+p+"tetherReady", 1);
				}
				if (kbGetBlockID(""+1*trQuestVarGet("p"+target+"tether")) == -1) {
					spyEffect(1*trQuestVarGet("p"+target+"unit"),kbGetProtoUnitID("Vortex Finish Linked"),"p"+target+"tether");
					trQuestVarSet("p"+target+"tetherReady", 0);
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("p"+target+"tether", true);
					trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					trUnitSetAnimationPath("0,1,0,0,0,0,0");
					trQuestVarSet("p"+target+"tetherReady", 1);
				}
			}
		} else if (trCurrentPlayer() == p) {
			trChatSend(0, "You must target an ally!");
		}
		if (target == 0) {
			if (trCurrentPlayer() == p) {
				trCounterAbort("well");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
			}
			trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COST);
		}
	}
	
	/* OMG MY QUEEN HEART-EYES EMOJI */
	if (trQuestVarGet("p"+p+"queen") > 0) {
		target = trQuestVarGet("p"+p+"queen");
		trVectorSetUnitPos("start", "p"+target+"unit");
		trVectorSetUnitPos("end", "p"+p+"unit");
		trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
		if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
			if (kbGetBlockID(""+1*trQuestVarGet("p"+p+"tether")) == -1) {
				spyEffect(1*trQuestVarGet("p"+p+"unit"),kbGetProtoUnitID("Vortex Finish Linked"),"p"+p+"tether");
				trQuestVarSet("p"+p+"tetherReady", 0);
			} else if (trQuestVarGet("p"+p+"tetherReady") == 0) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"tether", true);
				trUnitSetAnimationPath("0,1,0,0,0,0,0");
				trQuestVarSet("p"+p+"tetherReady", 1);
			}
			if (kbGetBlockID(""+1*trQuestVarGet("p"+target+"tether")) == -1) {
				spyEffect(1*trQuestVarGet("p"+target+"unit"),kbGetProtoUnitID("Vortex Finish Linked"),"p"+target+"tether");
				trQuestVarSet("p"+target+"tetherReady", 0);
			} else if (trQuestVarGet("p"+target+"tetherReady") == 0) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+target+"tether", true);
				trUnitSetAnimationPath("0,1,0,0,0,0,0");
				trQuestVarSet("p"+target+"tetherReady", 1);
			}
		}
		/* point the tethers towards each other */
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"tether", true);
		trSetSelectedUpVector(0.0 - trQuestVarGet("dirX"),-1,0.0 - trQuestVarGet("dirZ"));
		trUnitSelectClear();
		trUnitSelectByQV("p"+target+"tether", true);
		trSetSelectedUpVector(trQuestVarGet("dirX"),-1,trQuestVarGet("dirZ"));
		/* health damage reduction */
		xsSetContextPlayer(target);
		amt = kbUnitGetCurrentHitpoints(kbGetBlockID(""+1*trQuestVarGet("p"+target+"unit")));
		dist = 0.5 * (trQuestVarGet("p"+target+"currentHitpoints") - amt);
		if (dist > 0 && trQuestVarGet("p"+target+"currentHitpoints") > 1.0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+target+"unit");
			trDamageUnit(0.0 - dist);
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"unit");
			trDamageUnit(dist);
			amt = amt + dist;
		}
		trQuestVarSet("p"+target+"currentHitpoints", amt);
		xsSetContextPlayer(p);
	}
	
	
	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		trQuestVarSet("p"+p+"shieldOfLight", 1);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trSoundPlayFN("gaiasparkle2.wav","1",-1,"","");
		trQuestVarSet("p"+p+"shieldOfLightTimeout",
			trTimeMS() + 1000 * trQuestVarGet("shieldOfLightDuration") * trQuestVarGet("p"+p+"spellDuration") - 3000);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "shieldSFX")) == -1) {
				spyEffect(1*trQuestVarGet("p"+p+"characters"),
					kbGetProtoUnitID("Increase Prosperity Small"),yGetVarName("p"+p+"characters","shieldSFX"));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "shieldSFX"), true);
				trUnitChangeProtoUnit("Increase Prosperity Small");
			}
		}
		gainFavor(p, 0.0 - trQuestVarGet("shieldOfLightCost") * trQuestVarGet("p"+p+"ultimateCost"));
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
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					yDatabaseNext("p"+p+"characters");
					if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
						if (trCurrentPlayer() == p) {
							trChatSend(0, "<color=1,1,1>Shield of Light finished. Total damage: " + 1*yGetVar("p"+p+"characters", "absorbed"));
						}
					}
					ySetVar("p"+p+"characters", "laserStep", 1);
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "shieldSFX"));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
			trQuestVarSet("p"+p+"shieldOfLight", hit);
		}
	}
	
	
	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		xsSetContextPlayer(ENEMY_PLAYER);
		yClearDatabase("justice");
		dist = trQuestVarGet("justiceRadius") * trQuestVarGet("p"+p+"spellRange");
		dist = dist * dist;
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			target = yDatabaseNext("p"+p+"characters", true);
			trVectorSetUnitPos("pos", "p"+p+"characters");
			for(y=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
					hit = kbUnitGetTargetUnitID(id);
					if (hit == target) {
						silenceEnemy(p, 6.0);
					} else {
						stunUnit("enemies", 2.0, p);
					}
					gainFavor(p, 1);
				}
			}
			trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
			trArmySelect("1,0");
			trUnitChangeProtoUnit("Olympus Temple SFX");
		}
	}
	
	xsSetContextPlayer(p);
	/* for security reasons, this needs to run every loop */
	for (y=yGetDatabaseCount("p"+p+"characters"); > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeThroneShield(p);
		} else {
			/* damage reduction */
			amt = kbUnitGetCurrentHitpoints(id);
			dist = yGetVar("p"+p+"characters", "currentHitpoints") - amt;
			if (dist > 0) {
				if (trQuestVarGet("p"+p+"shieldOfLight") >= 1) {
					ySetVar("p"+p+"characters", "absorbed", yGetVar("p"+p+"characters", "absorbed") + dist);
					current = 1.0 + xsSqrt(yGetVar("p"+p+"characters", "absorbed")) * 0.02;
					trSetSelectedScale(current,current,current);
					trUnitHighlight(0.2, false);
				} else if (yGetVar("p"+p+"characters", "currentHitpoints") <= 1) {
					dist = 0;
				} else {
					dist = dist * trQuestVarGet("p"+p+"damageReduction");
				}
				trDamageUnit(0.0 - dist);
				amt = amt + dist;
			}
			ySetVar("p"+p+"characters", "currentHitpoints", amt);
			
			hit = CheckOnHit(p, id);
			if (hit >= ON_HIT_NORMAL) {
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					if (yGetVar("p"+p+"characters", "laserStep") == 1) {
						trVectorSetUnitPos("start", "p"+p+"characters");
						trVectorSetUnitPos("end", "enemies");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
						trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
						if (trUnitVisToPlayer()) {
							trCameraShake(1.0, 1.5);
						}
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
						dist = xsPow(trQuestVarGet("shieldOfLightWidth") * trQuestVarGet("p"+p+"spellRange"), 2);
						amt = yGetVar("p"+p+"characters", "absorbed") * trQuestVarGet("p"+p+"spellDamage");
						for(x=yGetDatabaseCount("enemies"); >0) {
							id = yDatabaseNext("enemies", true);
							if (id == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else {
								current = zDistanceToVector("enemies", "start");
								trQuestVarSet("hitboxX", trQuestVarGet("startX") + current * trQuestVarGet("dirX"));
								trQuestVarSet("hitboxZ", trQuestVarGet("startZ") + current * trQuestVarGet("dirZ"));
								if (zDistanceToVectorSquared("enemies", "hitbox") < dist) {
									damageEnemy(p, amt, true);
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
						zSetProtoUnitStat("Meteorite",p,1,100.0);
						trMutateSelected(kbGetProtoUnitID("Meteorite"));
						trUnitOverrideAnimation(6,0,true,false,-1);
						trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"));
						ySetVar("p"+p+"characters", "absorbed", 0);
						ySetVar("p"+p+"characters", "laserStep", 0);
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"characters");
						trSetSelectedScale(1,1,1);
					} else if (hit == ON_HIT_SPECIAL) {
						gainFavor(p, 3);
						stunUnit("enemies", 2.0, p);
						amt = 0.05 * trQuestVarGet("p"+p+"health");
						for(x=yGetDatabaseCount("playerCharacters"); >0) {
							if (yDatabaseNext("playerCharacters", true) == -1 || trUnitAlive() == false) {
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
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseThroneShield(int eventID = -1) {
	int p = eventID - 1000 - 12 * THRONESHIELD;
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
	trQuestVarSet("p"+p+"damageReduction", 0.02 * trQuestVarGet("p"+p+"health") / 100);
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("vowCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("shieldOfLightCooldown"));
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("shieldOfLightCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("justiceCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void modifyThroneShield(int eventID = -1) {
	int p = eventID - 5000 - 12 * THRONESHIELD;
	trQuestVarSet("p"+p+"damageReduction", 0.02 * trQuestVarGet("p"+p+"health") / 100);
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
	
	trQuestVarSet("vowCooldown", 10);
	
	trQuestVarSet("justiceCooldown", 10);
	trQuestVarSet("justiceRadius", 8);
	
	trQuestVarSet("shieldOfLightCost", 60);
	trQuestVarSet("shieldOfLightCooldown", 20);
	trQuestVarSet("shieldOfLightDuration", 6);
	trQuestVarSet("shieldOfLightWidth", 3);
}
