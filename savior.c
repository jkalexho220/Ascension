void removeSavior(int p = 0) {
	removePlayerSpecific(p);
	
}

void saviorAlways(int eventID = -1) {
	int p = eventID - 12 * SAVIOR;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);

		dist = xsPow(2 * trQuestVarGet("guardianAngelRange") * trQuestVarGet("p"+p+"spellRange"), 2);
		hit = -1;
		for(x=yGetDatabaseCount("playerUnits"); >0) {
			id = yDatabaseNext("playerUnits", true);
			if (id == -1 || trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				amt = zDistanceToVectorSquared("playerUnits", "p"+p+"wellPos");
				if (amt < dist) {
					dist = amt;
					hit = yGetPointer("playerUnits");
				}
			}
		}
		
		if (hit > 0) {
			trPlayerGrantResources(p, "favor", 5);
			ySetPointer("playerUnits", hit);
			trVectorSetUnitPos("pos", "playerUnits");
			trVectorQuestVarSet("dir", zGetUnitVector("pos", "p"+p+"wellPos", 2.0));
			dist = xsMin(trQuestVarGet("guardianAngelRange") * trQuestVarGet("p"+p+"spellRange"), 
				zDistanceBetweenVectors("p"+p+"wellPos", "pos")) / 2;
			for(x=dist; >0) {
				trQuestVarSet("tempx", trQuestVarGet("posX") + trQuestVarGet("dirX"));
				trQuestVarSet("tempZ", trQuestVarGet("posZ") + trQuestVarGet("dirZ"));
				vectorToGrid("temp", "loc");
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					break;
				} else {
					trQuestVarSet("posX", trQuestVarGet("tempX"));
					trQuestVarSet("posZ", trQuestVarGet("tempZ"));
				}
			}
			amt = 0;
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),180,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
					removeSavior(p);
				} else {
					trVectorSetUnitPos("start", "p"+p+"characters");
					trVectorQuestVarSet("dir", zGetUnitVector("start", "pos"));
					amt = amt + zDistanceToVector("p"+p+"characters", "pos");
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Hero Greek Bellerophon");
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				}
			}
			equipRelicsAgain(p);

			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trUnitChangeProtoUnit("Arkantos God Out");
			trSoundPlayFN("arkantosarrive.wav","1",-1,"","");
			
			amt = trQuestVarGet("guardianAngelHeal") * (1.0 + 0.1 * amt);
			dist = xsPow(trQuestVarGet("guardianAngelRange") * trQuestVarGet("p"+p+"spellRange"), 2);

			for(x=yGetDatabaseCount("playerUnits"); >0) {
				yDatabaseNext("playerUnits");
				if (zDistanceToVectorSquared("playerUnits", "pos") < dist) {
					trUnitSelectClear();
					trUnitSelectByQV("playerUnits", true);
					healUnit(p, amt * trQuestVarGet("p"+p+"spellDamage"));
				}
			}
		} else {
			trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COST);
			if (trCurrentPlayer() == p) {
				trCounterAbort("well");
				trChatSend(0, "You must choose a unit to teleport to!");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
			}
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		if (trQuestVarGet("p"+p+"unity") == 1) {
			for(x=trQuestVarGet("p"+p+"unityNext"); < trQuestVarGet("p"+p+"unityend")) {
				trUnitSelectClear();
				trUnitSelect(""+x, true);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trUnitDestroy();
			}
		}
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		vectorSnapToGrid("pos");
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",8,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Spy Eye");
		for(x=trQuestVarGet("next"); < trGetNextUnitScenarioNameNumber()) {
			trUnitSelectClear();
			trUnitSelect(""+x, true);
			trMutateSelected(kbGetProtoUnitID("Valkyrie"));
			trSetSelectedScale(0,0,0);
			trUnitSetAnimationPath("1,0,0,0,0,0,0");
			trUnitOverrideAnimation(15,0,true,false,-1);
		}
		trSoundPlayFN("gaiasparkle3.wav","1",-1,"","");
		trQuestVarSet("p"+p+"unity", 1);
		trQuestVarSet("p"+p+"unityNext", trQuestVarGet("next"));
		trQuestVarSet("p"+p+"unityEnd", trGetNextUnitScenarioNameNumber());
		trQuestVarSet("p"+p+"unityTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("unityDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trQuestVarSet("p"+p+"unityLast", trTimeMS());
		trQuestVarSet("p"+p+"unityradius", trQuestVarGet("unityRadius") * trQuestVarGet("p"+p+"spellRange"));
		trQuestVarSet("p"+p+"unityangle", 0);
		trQuestVarSet("p"+p+"unityposX", trQuestVarGet("posX"));
		trQuestVarSet("p"+p+"unityposZ", trQuestVarGet("posZ"));
		trQuestVarSet("p"+p+"unityLast", trTimeMS());
		trQuestVarSet("p"+p+"unitySearch", p);
	}

	if (trQuestVarGet("p"+p+"unity") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"unitytimeout")) {
			for(x=trQuestVarGet("p"+p+"unityNext"); < trQuestVarGet("p"+p+"unityend")) {
				trUnitSelectClear();
				trUnitSelect(""+x, true);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trUnitDestroy();
			}
			zSetProtoUnitStat("Hero Greek Bellerophon", p, 27, trQuestVarGet("p"+p+"baseAttack"));
			trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack"));
			trQuestVarSet("p"+p+"unity", 0);
			/*
			for(x=1; < ENEMY_PLAYER) {
				if (getBit(p, 1*trQuestVarGet("p"+x+"unityCount")) == true) {
					trQuestVarSet("p"+x+"unityCount", trQuestVarGet("p"+x+"unityCount") - xsPow(2, p));
				}
			}
			*/
		} else {
			dist = trTimeMS() - trQuestVarGet("p"+p+"unitylast");
			amt = trQuestVarGet("p"+p+"unityangle");
			amt = fModulo(6.283185, amt + dist * 0.001);
			trQuestVarSet("p"+p+"unitylast", trTimeMS());
			trQuestVarSet("p"+p+"unityangle", amt);
			trVectorSetFromAngle("dir1", amt);
			trQuestVarSet("dir2x", 0.707107 * (trQuestVarGet("dir1x") - trQuestVarGet("dir1z")));
			trQuestVarSet("dir2z", 0.707107 * (trQuestVarGet("dir1x") + trQuestVarGet("dir1z")));
			amt = trQuestVarGet("p"+p+"unityradius");
			dist = amt * amt;
			amt = 0.5 * amt;
			for(x=0; <4) {
				trUnitSelectClear();
				trUnitSelect(""+(x+trQuestVarGet("p"+p+"unityNext")), true);
				trSetSelectedUpVector(amt * trQuestVarGet("dir1x"),0,amt * trQuestVarGet("dir1z"));
				trUnitSelectClear();
				trUnitSelect(""+(4+x+trQuestVarGet("p"+p+"unityNext")), true);
				trSetSelectedUpVector(amt * trQuestVarGet("dir2x"),0,amt * trQuestVarGet("dir2z"));
				vectorRotate90Deg("dir1");
				vectorRotate90Deg("dir2");
			}
			hit = 0;
			for (x=yGetDatabaseCount("playerUnits"); >0) {
				yDatabaseNext("playerUnits");
				if (zDistanceToVectorSquared("playerUnits", "p"+p+"unitypos") < dist) {
					hit = hit + 1;
				}
			}
			
			trQuestVarSet("p"+p+"unityBuff", trQuestVarGet("unityBonus") * hit);
			trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * (1.0 + trQuestVarGet("p"+p+"unityBuff")));
			zSetProtoUnitStat("Hero Greek Bellerophon", p, 27, trQuestVarGet("p"+p+"attack"));

			/*
			for(x = xsMin(3, ENEMY_PLAYER); >0) {
				hit = 1 + trQuestVarGet("p"+p+"unitySearch");
				if (hit >= ENEMY_PLAYER) {
					hit = 1;
				}
				trQuestVarSet("p"+p+"unitySearch", hit);
				if (trQuestVarGet("p"+hit+"dead") == 0) {
					if (zDistanceToVectorSquared("p"+hit+"unit", "p"+p+"unityPos") < dist) {
						trQuestVarSet("p"+hit+"silenceTimeout", 0);
						if (getBit(p, 1*trQuestVarGet("p"+hit+"unityCount")) == false) {
							trQuestVarSet("p"+hit+"unityCount", trQuestVarGet("p"+hit+"unityCount") + xsPow(2, p));
						}
					} else if (getBit(p, 1*trQuestVarGet("p"+hit+"unityCount")) == true) {
						trQuestVarSet("p"+hit+"unityCount", trQuestVarGet("p"+hit+"unityCount") - xsPow(2, p));
					}
				}
			}
			*/
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"intervention", 1);
	}


	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeSavior(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_JUMP) {
				if (yGetVar("p"+p+"characters", "jumping") == 0) {
					ySetVar("p"+p+"characters", "jumping", 1);
					ySetVar("p"+p+"characters", "attackTarget", kbUnitGetTargetUnitID(id));
					for(x=yGetDatabaseCount("enemies"); >0) {
                		yDatabaseNext("enemies");
                		if (kbGetBlockID(""+1*trQuestVarGet("enemies")) == yGetVar("p"+p+"characters", "attackTarget")) {
                			ySetVar("p"+p+"characters", "attackTargetIndex", yGetPointer("enemies"));
                			trQuestVarSet("p"+p+"poisonKillerActive", yGetVar("enemies", "poisonStatus"));
                			break;
                		}
                	}
				}
			} else if (yGetVar("p"+p+"characters", "jumping") == 1) {
				ySetVar("p"+p+"characters", "jumping", 0);
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					stunUnit("enemies", 1.5, p);
				} else {
				}
			} else if (hit >= ON_HIT_NORMAL) {
				target = 1;
				if (hit == ON_HIT_SPECIAL) {
					if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
						trVectorSetUnitPos("start", "p"+p+"characters");
						trVectorSetUnitPos("end", "enemies");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
						trUnitHighlight(1.0, false);
						trSetUnitOrientation(xsVectorSet(0.0 - trQuestVarGet("dirx"),0, 0.0 - trQuestVarGet("dirz")), vector(0,1,0), true);
						dist = 10.0 + trQuestVarGet("p"+p+"attackRange");
						yAddToDatabase("playerLasers", "next");
						yAddUpdateVar("playerLasers", "timeout", trTimeMS() + 500);
						yAddUpdateVar("playerLasers", "range", dist * 1.3);
						amt = trQuestVarGet("p"+p+"attack");
						for(x=yGetDatabaseCount("enemies"); >1) {
							if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else {
								current = zDistanceToVector("enemies", "start");
								if (current < dist) {
									trQuestVarSet("hitboxX", trQuestVarGet("startX") + current * trQuestVarGet("dirx"));
									trQuestVarSet("hitboxZ", trQuestVarGet("startZ") + current * trQuestVarGet("dirz"));
									if (zDistanceToVectorSquared("enemies", "hitbox") < 9) {
										damageEnemy(p, amt, false);
										target = 1 + target;
									}
								}
							}
						}
					}
				}
				if (trQuestVarGet("p"+p+"unity") == 1) {
					amt = trQuestVarGet("p"+p+"attack") * trQuestVarGet("unityHeal") * target;
					dist = xsPow(trQuestVarGet("unityRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
					trPlayerGrantResources(p, "favor", 1);
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (zDistanceToVectorSquared("playerUnits", "p"+p+"unityPos") < dist) {
							healUnit(p, amt);
							if (yGetVar("playerUnits", "hero") == 1) {
								trPlayerGrantResources(1*yGetVar("playerUnits","player"), "favor", target);
							}
						}
					}
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"dead") > 0 && 
		trPlayerResourceCount(p, "favor") >= trQuestVarGet("interventionCost") * trQuestVarGet("p"+p+"ultimateCost")) {
		trQuestVarSet("p"+p+"intervention", 1);
	}

	if (trQuestVarGet("p"+p+"intervention") == 1) {
		for(x=yGetDatabaseCount("playerCharacters"); >0) {
			if (yDatabaseNext("playerCharacters", true) == -1 || trUnitAlive() == false) {
				removePlayerCharacter();
			}
		}
		for(x=1; < ENEMY_PLAYER) {
			if (trQuestVarGet("p"+x+"dead") > 0) {
				revivePlayer(x);
				trQuestVarSet("p"+x+"dead", 0);
			}
		}
		trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("interventionCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trQuestVarSet("p"+p+"rainCooldownStatus", ABILITY_COOLDOWN);
		trChatSend(0, "<color=1,1,1>Intervention!</color>");
		trSoundPlayFN("restorationbirth.wav","1",-1,"","");
		trSoundPlayFN("herobirth3.wav","1",-1,"","");
		trQuestVarSet("p"+p+"intervention", 0);
	}

	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseSavior(int eventID = -1) {
	int p = eventID - 1000 - 12 * SAVIOR;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Guardian Angel";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Intervention";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Unity";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("guardianAngelCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("unityCooldown"));
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("unityCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("interventionCooldown"));
	trQuestVarSet("p"+p+"rainCost", trQuestVarGet("interventionCost"));
}

void modifySavior(int eventID = -1) {
	int p = eventID - 5000 - 12 * SAVIOR;
	if (yGetDatabaseCount("p"+p+"unityAuras") >0) {
		trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * (1.0 + trQuestVarGet("p"+p+"unityBuff")));
		zSetProtoUnitStat("Hero Greek Bellerophon", p, 27, trQuestVarGet("p"+p+"attack"));
	}
}

rule savior_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SAVIOR + p, "saviorAlways");
		trEventSetHandler(1000 + 12 * SAVIOR + p, "chooseSavior");
		trEventSetHandler(5000 + 12 * SAVIOR + p, "modifySavior");
	}

	trQuestVarSet("guardianAngelCooldown", 8);
	trQuestVarSet("guardianAngelHeal", 50);
	trQuestVarSet("guardianAngelRange", 10);

	trQuestVarSet("unityCooldown", 20);
	trQuestVarSet("unityRadius", 10);
	trQuestVarSet("unityDuration", 8);
	trQuestVarSet("unityBonus", 0.5);
	trQuestVarSet("unityHeal", 0.5);

	trQuestVarSet("interventionCost", 100);
	trQuestVarSet("interventionCooldown", 10);
}