

void removeNightrider(int p = 0) {
	removePlayerSpecific(p);
}

void spawnMinion(int p = 0, string pos = "", float heading = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch(""+p+",0","Minion",1,trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),heading,true);
	activatePlayerUnit("next", p, kbGetProtoUnitID("Minion"), calculateDecay(p, 5.0));
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
	silenceUnit("enemies",trQuestVarGet("deathSentenceDuration"),p);
	trQuestVarSet("poisonSound", 0);
	yAddToDatabase("p"+p+"Sentences", "enemies");
	yAddUpdateVar("p"+p+"Sentences", "index", yGetPointer("enemies"));
	yAddUpdateVar("p"+p+"sentences", "posx", trQuestVarGet("posx"));
	yAddUpdateVar("p"+p+"sentences", "posz", trQuestVarGet("posz"));
	yAddUpdateVar("p"+p+"sentences", "timeout",
		trTimeMS() + 1000 * trQuestVarGet("deathSentenceDuration") * trQuestVarGet("p"+p+"spellDuration"));
	
	if (PvP) {
		xsSetContextPlayer(1*yGetVarAtIndex("playerUnits", "player", 1*yGetVar("enemies", "doppelganger")));
	} else {
		xsSetContextPlayer(ENEMY_PLAYER);
	}
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
			if (hit >= ON_HIT_NORMAL) {
				if (yGetVarAtIndex("enemies", "deathSentence", 1*yGetVar("p"+p+"characters", "attackTargetIndex")) > 0) {
					if (trQuestVarGet("p"+p+"rainCooldownStatus") == ABILITY_COOLDOWN) {
						trQuestVarSet("p"+p+"rainReadyTime", trQuestVarGet("p"+p+"rainReadyTime") - 1000);
						amt = trQuestVarGet("p"+p+"rainReadyTime") - trTimeMS();
						if (amt > 0) {
							if (trCurrentPlayer() == p) {
								trCounterAbort("rain");
								trCounterAddTime("rain", amt * 0.001, 0, rainName);
							}
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
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"sentences") > 0) {
		xsSetContextPlayer(ENEMY_PLAYER);
		if (yDatabaseNext("p"+p+"Sentences", true) == -1 || trUnitAlive() == false) {
			yVarToVector("p"+p+"Sentences", "pos");
			hit = 0;
			dist = 100;
			for(x=xGetDatabaseCount(dEnemies); >0) {
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
			trQuestVarSetFromRand("heading",1,360,false);
			spawnMinion(p, "pos", trQuestVarGet("heading"));
			if (hit > 0) {
				ySetPointer("enemies", hit);
				castDeathSentence(p);
			}
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
					if (PvP) {
						xsSetContextPlayer(1*yGetVarAtIndex("playerUnits", "player", 1*yGetVar("enemies", "doppelganger")));
					}
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
			if (trTimeMS() > yGetVar("p"+p+"abducts", "timeout")) {
				trUnitChangeProtoUnit("Kronny Birth SFX");
				yRemoveFromDatabase("p"+p+"abducts");
			} else {
				for(y=xGetDatabaseCount(dEnemies); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (yGetVar("enemies", "launched") == 0) {
						trVectorSetUnitPos("target", "enemies");
						current = zDistanceBetweenVectors("target", "prev");
						if (current < dist) {
							trQuestVarSet("hitboxX", trQuestVarGet("prevX") + current * trQuestVarGet("dirX"));
							trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + current * trQuestVarGet("dirZ"));
							if (zDistanceBetweenVectorsSquared("target", "hitbox") < 9.0) {
								gainFavor(p, 1);
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
				addGenericProj("p"+p+"abducts","start","dir",kbGetProtoUnitID("Kronny Birth SFX"),2,15.0,4.5,0,p);
				yAddUpdateVar("p"+p+"abducts", "prevx", trQuestVarGet("startx"));
				yAddUpdateVar("p"+p+"abducts", "prevz", trQuestVarGet("startz"));
				yAddUpdateVar("p"+p+"abducts", "destx", trQuestVarGet("posx"));
				yAddUpdateVar("p"+p+"abducts", "destz", trQuestVarGet("posz"));
				yAddUpdateVar("p"+p+"abducts", "timeout", trTimeMS() + dist / 0.015);
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
	
	switch(1*trQuestVarGet("p"+p+"nightfall"))
	{
		case 1:
		{
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"nightfallCloud");
			trMutateSelected(kbGetProtoUnitID("Kronny Birth SFX"));
			trUnitOverrideAnimation(2,0,true,false,-1);
			trQuestVarSet("p"+p+"nightfall", 2);
			trQuestVarSet("p"+p+"nightfallNext", trTimeMS() + 1000);
		}
		case 3:
		{
			for(x=yGetDatabaseCount("p"+p+"nightfallClouds"); >0) {
				hit = processGenericProj("p"+p+"nightfallClouds");
			}
			if (hit == PROJ_BOUNCE) {
				trQuestVarSet("p"+p+"nightfall", 4);
				trQuestVarSet("p"+p+"nightfallDist", 0);
				trQuestVarSet("p"+p+"nightfallNext", trTimeMS());
			}
		}
		case 2:
		{
			if (trTimeMS() > trQuestVarGet("p"+p+"nightfallNext")) {
				trQuestVarSet("p"+p+"launched", 0);
				trQuestVarSet("p"+p+"nightfall", 3);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,
					trQuestVarGet("p"+p+"nightfallCenterx"),0,trQuestVarGet("p"+p+"nightfallCenterz"),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
						removeNightrider(p);
					} else {
						trMutateSelected(kbGetProtoUnitID("Dwarf"));
						trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
						trUnitChangeProtoUnit("Hero Greek Achilles");
						ySetVar("p"+p+"characters", "index", activatePlayerUnit("p"+p+"characters"));
						yAddUpdateVar("playerUnits", "hero", 1);
						yAddUpdateVar("playerUnits", "magicResist", trQuestVarGet("p"+p+"magicResist"));
						yAddUpdateVar("playerUnits", "physicalResist", trQuestVarGet("p"+p+"physicalResist"));
						if (trQuestVarGet("p"+p+"unit") == trQuestVarGet("p"+p+"characters")) {
							trQuestVarSet("p"+p+"index", yGetNewestPointer("playerUnits"));
						}
					}
				}
				equipRelicsAgain(p);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				if (trUnitVisToPlayer()) {
					trCameraShake(0.5, 0.5);
				}
				trUnitChangeProtoUnit("Kronny Birth SFX");
				
				/* spread the love */
				trVectorQuestVarSet("dir", vector(1,0,0));
				for(x=16; >0) {
					addGenericProj("p"+p+"nightfallClouds","p"+p+"nightfallCenter","dir",
						kbGetProtoUnitID("Kronny Birth SFX"),2,15.0,4.5,0,p);
					trVectorQuestVarSet("dir", rotationMatrix("dir", 0.923879, 0.382683));
				}
				trSoundPlayFN("tartariangateselect.wav","1",-1,"","");
				trSoundPlayFN("changeunit.wav","1",-1,"","");
			}
		}
		case 4:
		{
			dist = trTimeMS() - trQuestVarGet("p"+p+"nightfallNext");
			dist = dist * 0.015;
			if (dist > trQuestVarGet("nightfallRadius") * trQuestVarGet("p"+p+"spellRange")) {
				trQuestVarSet("p"+p+"nightfall", 0);
				for(x=yGetDatabaseCount("p"+p+"nightfallClouds"); >0) {
					yDatabaseNext("p"+p+"nightfallClouds", true);
					trUnitDestroy();
				}
				yClearDatabase("p"+p+"nightfallClouds");
			}
			dist = xsPow(dist, 2);
			amt = 0;
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (yGetVar("enemies", "deathSentence") == 0) {
					current = zDistanceToVectorSquared("enemies", "p"+p+"nightfallCenter");
					if (current < dist && current > trQuestVarGet("p"+p+"nightfallDist")) {
						if (current > amt) {
							amt = current;
						}
						castDeathSentence(p);
						stunUnit("enemies", trQuestVarGet("deathSentenceDuration"), p);
					}
				}
			}
			trQuestVarSet("p"+p+"nightfallDist", amt);
		}
		case 0:
		{
			if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
				trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
				gainFavor(p, 0.0 - trQuestVarGet("nightfallCost") * trQuestVarGet("p"+p+"ultimateCost"));
				trVectorSetUnitPos("p"+p+"nightfallCenter", "p"+p+"lureObject");
				vectorSnapToGrid("p"+p+"nightfallCenter");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"lureObject");
				trUnitDestroy();
				zSetProtoUnitStat("Kronny Flying", p, 1, 0.01);
				trQuestVarSet("p"+p+"nightfall", 1);
				trQuestVarSet("p"+p+"launched", 1);
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					if (yDatabaseNext("p"+p+"characters", true) == -1 || trUnitAlive() == false) {
						removeNightrider(p);
					} else {
						ySetPointer("playerUnits", 1*yGetVar("p"+p+"characters", "index"));
						yRemoveFromDatabase("playerUnits");
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
				
				trUnitSelectClear();
				trQuestVarSet("p"+p+"nightfallCloud", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Kronny Flying",1,
					trQuestVarGet("p"+p+"nightfallCenterx"),0,trQuestVarGet("p"+p+"nightfallCenterz"),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
				trDamageUnitPercent(100);
				trSetSelectedScale(0,3,0);
				
				trSoundPlayFN("cinematics\32_out\kronosbehinddorrshort.mp3","1",-1,"","");
			}
		}
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
		lureName = "(E) Nightfall";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("abductCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("nightfallCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("ariseCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void nightriderModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * NIGHTRIDER;
	zSetProtoUnitStat("Minion", p, 0, 0.2 * trQuestVarGet("p"+p+"health"));
	trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack"));
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
	
	trQuestVarSet("nightfallCost", 75);
	trQuestVarSet("nightfallRadius", 15);
}
