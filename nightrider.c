

void removeNightrider(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "doomsdaySFX");
}

void spawnMinion(int p = 0, string pos = "", float heading = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch(""+p+",0","Minion",1,trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),heading,true);
	yAddToDatabase("playerUnits", "next");
	yAddUpdateVar("playerUnits", "player", p);
	yAddUpdateVar("playerUnits", "decay", calculateDecay(p, 5.0));
	yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 1000);
}

/* 
assume pointer is already on the death sentence target 
also make sure target does not have death sentence on them already
*/
void castDeathSentence(int p = 0) {
	trSoundPlayFN("shadeofhadesbirth.wav","1",-1,"","");
	trVectorSetUnitPos("pos", "enemies");
	
	ySetVar("enemies", "deathSentence", 1);
	poisonUnit("enemies", trQuestVarGet("deathSentenceDuration"), trQuestVarGet("deathSentenceDamage"), p);
	trQuestVarSet("poisonSound", 0);
	yAddToDatabase("p"+p+"Sentences", "enemies");
	yAddUpdateVar("p"+p+"Sentences", "index", yGetPointer("enemies"));
	yAddUpdateVar("p"+p+"sentences", "posx", trQuestVarGet("posx"));
	yAddUpdateVar("p"+p+"sentences", "posz", trQuestVarGet("posz"));
	yAddUpdateVar("p"+p+"sentences", "timeout", 
		trTimeMS() + 1000 * trQuestVarGet("deathSentenceDuration") * trQuestVarGet("p"+p+"spellDuration"));

	xsSetContextPlayer(ENEMY_PLAYER);
	yAddUpdateVar("p"+p+"sentences", "health", kbUnitGetCurrentHitpoints(kbGetBlockID(""+1*trQuestVarGet("enemies"))));
	xsSetContextPlayer(p);

	spyEffect(1*trQuestVarGet("enemies"),kbGetProtoUnitID("Shade"),yGetNewVarName("p"+p+"sentences", "SFX"));
}

void nightriderAlways(int eventID = -1) {
	int p = eventID - 12 * NIGHTRIDER;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeNightrider(p);
		} else {
			hit = CheckOnHit(p, id);

			if (trQuestVarGet("p"+p+"doomsday") == 1) {
				amt = xsSqrt(trQuestVarGet("p"+p+"doomsdayBonus"));
				trSetSelectedScale(amt, amt, amt);
				if (hit >= ON_HIT_NORMAL) {
					if (trQuestVarGet("p"+p+"doomsday") == 1) {
						trQuestVarSet("p"+p+"lifestealTotal", trQuestVarGet("p"+p+"lifestealTotal") + 0.03 * trQuestVarGet("p"+p+"health"));
						trVectorSetUnitPos("pos", "p"+p+"characters");
						trQuestVarSetFromRand("heading", 1, 360, true);
						spawnMinion(p, "pos", trQuestVarGet("heading"));
						trQuestVarSet("p"+p+"doomsdayBonus", trQuestVarGet("p"+p+"doomsdayBonus") + 0.1);
						trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * trQuestVarGet("p"+p+"doomsdayBonus"));
						zSetProtoUnitStat("Minion", p, 27, (0.2 + trQuestVarGet("p"+p+"doomsdayBonus")) * trQuestVarGet("p"+p+"baseAttack"));
						zSetProtoUnitStat("Hero Greek Achilles", p, 27, trQuestVarGet("p"+p+"Attack"));
					}
				}
			}
			
			
			if (hit == ON_HIT_SPECIAL) {
				target = yGetPointer("enemies");
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					if (yGetVar("enemies", "deathSentence") == 0) {
						castDeathSentence(p);
					} else {
						/* don't consume special attack */
						ySetVar("p"+p+"characters", "specialAttack", 0);
					}
					ySetPointer("enemies", target);
				}
			}
			if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "doomsdaySFX")) == -1) {
				spyEffect(1*trQuestVarGet("p"+p+"characters"),
					kbGetProtoUnitID("Cinematic Block"),yGetVarName("p"+p+"characters","doomsdaySFX"));
			}
			
		}
	}

	if (yGetDatabaseCount("p"+p+"sentences") > 0) {
		xsSetContextPlayer(ENEMY_PLAYER);
		if (yDatabaseNext("p"+p+"Sentences", true) == -1 || trUnitAlive() == false) {
			yVarToVector("p"+p+"Sentences", "pos");
			hit = 0;
			dist = 100;
			for(x=yGetDatabaseCount("enemies"); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (yGetVar("enemies", "deathSentence") == 0) {
					current = zDistanceToVectorSquared("enemies", "Pos");
					if (current < dist) {
						dist = current;
						hit = yGetPointer("enemies");
					}
				}
			}
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"sentences", "SFX"), true);
			trUnitChangeProtoUnit("Kronny Birth SFX");
			yRemoveFromDatabase("p"+p+"sentences");
			if (hit > 0) {
				ySetPointer("enemies", hit);
				castDeathSentence(p);
			}
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trQuestVarSetFromRand("heading",1,360,false);
			spawnMinion(p, "pos", trQuestVarGet("heading"));
		} else {
			hit = yGetPointer("enemies");
			if (ySetPointer("enemies", 1*yGetVar("p"+p+"sentences", "index"))) {
				if (trTimeMS() > yGetVar("p"+p+"sentences", "timeout")) {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"sentences", "SFX"));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					ySetVar("enemies", "deathSentence", 0);
					yRemoveFromDatabase("p"+p+"sentences");
				} else {
					trVectorSetUnitPos("pos", "enemies");
					ySetVarFromVector("p"+p+"sentences", "pos", "pos");
					amt = kbUnitGetCurrentHitpoints(kbGetBlockID(""+1*trQuestVarGet("enemies")));
					dist = yGetVar("p"+p+"sentences", "health") - amt;
					if (dist > 0) {
						trUnitSelectClear();
						trUnitSelectByQV("enemies");
						trDamageUnit(dist);
						ySetVar("p"+p+"sentences", "health", amt - dist);
					} else {
						ySetVar("p"+p+"sentences", "health", amt);
					}
				}
				ySetPointer("enemies", hit);
			}
		}
		xsSetContextPlayer(p);
	}
	
	for (x=yGetDatabaseCount("p"+p+"abducts"); > 0) {
		if (processGenericProj("p"+p+"abducts") == PROJ_FALLING) {
			yVarToVector("p"+p+"abducts", "prev");
			yVarToVector("p"+p+"abducts", "dir");
			yVarToVector("p"+p+"abducts", "dest");
			dist = zDistanceBetweenVectors("pos", "prev") + 3.0;
			if (zDistanceBetweenVectorsSquared("pos", "dest") < dist &&
				zDistanceBetweenVectorsSquared("prev", "dest") < dist) {
				trUnitChangeProtoUnit("Kronny Birth SFX");
				yRemoveFromDatabase("p"+p+"abducts");
			} else {
				for(y=yGetDatabaseCount("enemies"); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (yGetVar("enemies", "launched") == 0) {
						trVectorSetUnitPos("target", "enemies");
						current = zDistanceBetweenVectors("target", "prev");
						if (current < dist) {
							trQuestVarSet("hitboxX", trQuestVarGet("prevX") + current * trQuestVarGet("dirX"));
							trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + current * trQuestVarGet("dirZ"));
							if (zDistanceBetweenVectorsSquared("target", "hitbox") < 9.0) {
								trPlayerGrantResources(p, "favor", 1);
								trQuestVarSet("destX", trQuestVarGet("destX") + trQuestVarGet("targetX") - trQuestVarGet("hitboxX"));
								trQuestVarSet("destZ", trQuestVarGet("destZ") + trQuestVarGet("targetZ") - trQuestVarGet("hitboxZ"));
								if ((yGetVar("p"+p+"abducts", "curse") == 1) &&
									yGetVar("enemies", "deathSentence") == 0) {
									castDeathSentence(p);
									ySetVar("p"+p+"abducts", "curse", 0);
								}
								launchUnit("enemies", "dest");
							}
						}
					}
				}
				ySetVarFromVector("p"+p+"abducts", "prev", "pos");
			}
		}
	}
	
	dist = trQuestVarGet("abductRange") * trQuestVarGet("p"+p+"spellRange");
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
				removeNightrider(p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("p"+p+"wellPos", "pos"));
				trQuestVarSet("startx", trQuestVarGet("posx") - dist * trQuestVarGet("dirx"));
				trQuestVarSet("startz", trQuestVarGet("posz") - dist * trQuestVarGet("dirz"));
				addGenericProj("p"+p+"abducts","start","dir",kbGetProtoUnitID("Kronny Birth SFX"),2,15.0,4.5);
				yAddUpdateVar("p"+p+"abducts", "prevx", trQuestVarGet("startx"));
				yAddUpdateVar("p"+p+"abducts", "prevz", trQuestVarGet("startz"));
				yAddUpdateVar("p"+p+"abducts", "destx", trQuestVarGet("posx"));
				yAddUpdateVar("p"+p+"abducts", "destz", trQuestVarGet("posz"));
				yAddUpdateVar("p"+p+"abducts", "curse", 1);
			}
		}
		trSoundPlayFN("changeunit.wav","1",-1,"","");
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"ariseCount", trQuestVarGet("p"+p+"ariseCount") + 3 + yGetDatabaseCount("p"+p+"sentences"));
		trQuestVarSetFromRand("p"+p+"ariseAngle", 0, 3.14, false);
		trQuestVarSet("p"+p+"ariseDist", 2);
		trQuestVarSet("p"+p+"ariseNext", trTimeMS());
		trSoundPlayFN("ancestorsbirth.wav", "1", -1, "","");
	}

	if (trQuestVarGet("p"+p+"ariseCount") > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"ariseNext")) {
			trVectorSetFromAngle("dir", trQuestVarGet("p"+p+"ariseAngle"));
			amt = trQuestVarGet("p"+p+"ariseAngle") * 57.295;
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
					removeNightrider(p);
				} else {
					trVectorSetUnitPos("pos", "p"+p+"characters");
					trQuestVarSet("posx", trQuestVarGet("posx") + trQuestVarGet("p"+p+"ariseDist") * trQuestVarGet("dirX"));
					trQuestVarSet("posz", trQuestVarGet("posz") + trQuestVarGet("p"+p+"ariseDist") * trQuestVarGet("dirZ"));
					spawnMinion(p, "pos", amt);
				}
			}
			trQuestVarSet("p"+p+"ariseDist", 0.5 + trQuestVarGet("p"+p+"ariseDist"));
			trQuestVarSet("p"+p+"ariseAngle", 2.43 + trQuestVarGet("p"+p+"ariseAngle"));
			trQuestVarSet("p"+p+"ariseCount", trQuestVarGet("p"+p+"ariseCount") - 1);
			trQuestVarSet("p"+p+"ariseNext", trQuestVarGet("p"+p+"ariseNext") + 200);
		}
	}

	if (trQuestVarGet("p"+p+"doomsday") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"doomsdayTimeout")) {
			trQuestVarSet("p"+p+"doomsday", 0);
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				yDatabaseNext("p"+p+"characters", true);
				trSetSelectedScale(1,1,1);
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "doomsdaySFX"));
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
			trModifyProtounit("Minion", p, 27, 4.25);
			trModifyProtounit("Minion", p, 1, 1);
			trTechSetStatus(p, 377, 0);
			zSetProtoUnitStat("Minion", p, 27, 0.2 * trQuestVarGet("p"+p+"baseAttack"));
			zSetProtoUnitStat("Hero Greek Achilles", p, 27, trQuestVarGet("p"+p+"baseAttack"));
			trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack"));
			trQuestVarSet("p"+p+"doomsdayBonus", 0);
			trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("doomsdayCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trQuestVarSet("p"+p+"doomsday", 1);
		trQuestVarSet("p"+p+"doomsdayBonus", 1);
		trQuestVarSet("p"+p+"doomsdayTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("doomsdayDuration") * trQuestVarGet("p"+p+"spellDuration"));
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"characters", "doomsdaySFX"));
			trMutateSelected(kbGetProtoUnitID("Tartarian Gate"));
			trSetSelectedScale(0,0,0);
			trUnitOverrideAnimation(6,0,false,true,-1);
		}
		trModifyProtounit("Minion", p, 27, -4.25);
		trModifyProtounit("Minion", p, 1, -1);
		trTechSetStatus(p, 377, 4);
		trSoundPlayFN("tartariangateselect.wav","1",-1,"","");
	}

	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseNightrider(int eventID = -1) {
	int p = eventID - 1000 - 12 * NIGHTRIDER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Abduct";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Arise";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Doomsday";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("abductCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("doomsdayCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("ariseCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void nightriderModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * NIGHTRIDER;
	zSetProtoUnitStat("Minion", p, 0, 0.2 * trQuestVarGet("p"+p+"health"));
	trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack"));
	if (trQuestVarGet("p"+p+"doomsday") == 1) {
		trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") * trQuestVarGet("p"+p+"doomsdayBonus"));
	}
	zSetProtoUnitStat("Minion", p, 27, 0.2 * trQuestVarGet("p"+p+"Attack"));
	zSetProtoUnitStat("Hero Greek Achilles", p, 27, trQuestVarGet("p"+p+"Attack"));
}

rule nightrider_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * NIGHTRIDER + p, "nightriderAlways");
		trEventSetHandler(1000 + 12 * NIGHTRIDER + p, "chooseNightrider");
		trEventSetHandler(5000 + 12 * NIGHTRIDER + p, "nightriderModify");
	}

	trQuestVarSet("abductRange", 12);
	trQuestVarSet("abductCooldown", 15);

	trQuestVarSet("deathSentenceDamage", 6);
	trQuestVarSet("deathSentenceDuration", 6);
	trQuestVarSet("deathSentenceRadius", 8);

	trQuestVarSet("ariseCooldown", 16);

	trQuestVarSet("doomsdayCost", 80);
	trQuestVarSet("doomsdayDuration", 12);
}