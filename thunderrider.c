void removeThunderRider(int p = 0) {
	removePlayerCharacter(p);
	yRemoveUpdateVar("p"+p+"characters", "prevX");
	yRemoveUpdateVar("p"+p+"characters", "prevZ");
	yRemoveUpdateVar("p"+p+"characters", "lightningIndex");
}

void removeLightningBall(int p = 0) {
	yRemoveFromDatabase("p"+p+"lightningBalls");
	yRemoveUpdateVar("p"+p+"lightningBalls", "dirX");
	yRemoveUpdateVar("p"+p+"lightningBalls", "dirZ");
	yRemoveUpdateVar("p"+p+"lightningBalls", "prevX");
	yRemoveUpdateVar("p"+p+"lightningBalls", "prevZ");
	yRemoveUpdateVar("p"+p+"lightningBalls", "damage");
	yRemoveUpdateVar("p"+p+"lightningBalls", "yeehaw");
	yRemoveUpdateVar("p"+p+"lightningBalls", "inWall");
	yRemoveUpdateVar("p"+p+"lightningBalls", "extra");
	yRemoveUpdateVar("p"+p+"lightningBalls", "lastGoodX");
	yRemoveUpdateVar("p"+p+"lightningBalls", "lastGoodZ");
	yRemoveUpdateVar("p"+p+"lightningBalls", "inWallTimeout");
}

void rideLightningOff(int p = 0) {
	trModifyProtounit("Attack Revealer", p, 2, -10);
	int index = 0;
	for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
		yDatabaseNext("p"+p+"characters");
		index = yGetVar("p"+p+"characters", "lightningIndex");
		ySetPointer("p"+p+"lightningBalls", index);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lightningBalls", true);
		trUnitChangeProtoUnit("Transport Ship Greek");
		trDamageUnitPercent(-100);

		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"characters");
		trMutateSelected(kbGetProtoUnitID("Dwarf"));
		if (kbGetBlockID(""+1*trQuestVarGet("p"+p+"lightningBalls")) >= 0) {
			trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"lightningBalls"));
		}
		trUnitChangeProtoUnit("Hero Greek Atalanta");

		trUnitSelectClear();
		trUnitSelect(""+1*yGetVar("p"+p+"lightningBalls", "extra"), true);
		trUnitChangeProtoUnit("Implode Sphere Effect");

		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lightningBalls", true);
		trUnitChangeProtoUnit("Arkantos God Out");
	}

	for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
		yDatabaseNext("p"+p+"relics", true);
		trUnitChangeProtoUnit("Relic");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"relics");
		trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
		trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
		if (yGetVar("p"+p+"relics", "type") < RELIC_KEY_GREEK) {
			trSetSelectedScale(0,0,0);
			trUnitSetAnimationPath("1,0,1,1,0,0,0");
		}
	}
	yClearDatabase("p"+p+"lightningBalls");
	yClearDatabase("p"+p+"rideLightningTargets");

	if (trCurrentPlayer() == p) {
		uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"unit"));
	}
}

void refreshRideLightningTargets(int p = 0) {
	yClearDatabase("p"+p+"rideLightningTargets");
	for(x=yGetDatabaseCount("enemies"); >0) {
		yDatabaseNext("enemies");
		yAddToDatabase("p"+p+"rideLightningTargets", "enemies");
		yAddUpdateVar("p"+p+"rideLightningTargets", "index", yGetPointer("enemies"));
	}
}

void lightningBallBounce(int p = 0) {
	yVarToVector("p"+p+"lightningBalls", "dir");
	zSetProtoUnitStat("Kronny Flying", p, 1, 2.0 * trQuestVarGet("p"+p+"speed"));
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"lightningBalls");
	trUnitChangeProtoUnit("Kronny Flying");
	trUnitSelectClear();
	trUnitSelect(""+1*yGetVar("p"+p+"lightningBalls", "extra"));
	trUnitChangeProtoUnit("Kronny Flying");
	
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"lightningBalls");
	trDamageUnitPercent(-100);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
	trSetSelectedScale(0,-4.5,0);
	trDamageUnitPercent(100);

	trUnitSelectClear();
	trUnitSelect(""+1*yGetVar("p"+p+"lightningBalls", "extra"));
	trDamageUnitPercent(-100);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
	trSetSelectedScale(0,-4.5,0);
	trDamageUnitPercent(100);

	ySetVar("p"+p+"lightningBalls", "yeehaw", 1);
}

void thunderRiderAlways(int eventID = -1) {
	int p = eventID - 12 * THUNDERRIDER;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float posX = 0;
	float posZ = 0;

	if (yGetDatabaseCount("p"+p+"blitzSFX") > 0) {
		/* stun enemies */
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else if (trCountUnitsInArea(""+1*trQuestVarGet("enemies"), p, "Victory Marker", 3) > 0) {
				trPlayerGrantResources(p, "favor", 3);
				damageEnemy(p, trQuestVarGet("blitzDamage") * trQuestVarGet("p"+p+"spellDamage"), true);
				if (trUnitAlive()) {
					stunUnit("enemies", 1.5, p);
				}
			}
		}
		for(x=yGetDatabaseCount("p"+p+"blitzSFX"); >0) {
			yDatabaseNext("p"+p+"blitzSFX", true);
			trUnitChangeProtoUnit("Lightning Sparks");
		}
		yClearDatabase("p"+p+"blitzSFX");
	}

	if (trQuestVarGet("p"+p+"rideLightning") == 1) {
		if (yGetDatabaseCount("p"+p+"lightningBalls") > 0) {
			yDatabaseNext("p"+p+"lightningBalls", true);
			if (yGetVar("p"+p+"lightningBalls", "yeehaw") == 1) {
				trMutateSelected(kbGetProtoUnitID("Arkantos God"));
				trUnitOverrideAnimation(26,0,true,true,-1);
				trSetSelectedScale(0,0,0);
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"lightningBalls", "extra"));
				trMutateSelected(kbGetProtoUnitID("Vortex Finish linked"));
				trUnitOverrideAnimation(2,0,true,true,-1);
				trSetSelectedScale(1,1,1);
				ySetVar("p"+p+"lightningBalls", "yeehaw", 0);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"lightningBalls");
				yVarToVector("p"+p+"lightningBalls", "prev");
				dist = zDistanceBetweenVectors("pos", "prev");

				trQuestVarSet("p"+p+"thunderRiderBonus", 
					trQuestVarGet("p"+p+"thunderRiderBonus") + dist * 0.1 * trQuestVarGet("p"+p+"baseAttack"));
				ySetVarFromVector("p"+p+"lightningBalls", "prev", "pos");

				vectorToGrid("pos", "loc");
				if (yGetVar("p"+p+"lightningBalls", "inWall") == 0) {
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						yVarToVector("p"+p+"lightningBalls", "dir");
						trVectorQuestVarSet("dir", getBounceDir("pos", "dir"));
						trQuestVarSetFromRand("sound", 1, 2, true);
						trSoundPlayFN("implodehit"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
						ySetVarFromVector("p"+p+"lightningBalls", "dir", "dir");
						ySetVar("p"+p+"lightningBalls", "inWall", 1);
						ySetVar("p"+p+"lightningBalls", "inWallTimeout", trTimeMS() + 300);
						amt = 0.5 * trQuestVarGet("rideLightningDamage") * trQuestVarGet("p"+p+"spellDamage");
						ySetVar("p"+p+"lightningBalls", "damage", amt + yGetVar("p"+p+"lightningBalls", "damage"));
						lightningBallBounce(p);
						refreshRideLightningTargets(p);
					} else {
						ySetVarFromVector("p"+p+"lightningBalls", "lastGood", "pos");
					}
				} else if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
					ySetVar("p"+p+"lightningBalls", "inWall", 0);
				} else if (trTimeMS() > yGetVar("p"+p+"lightningBalls", "inWallTimeout")) {
					/* we've been in a wall for too long something's not right */
					ySetVar("p"+p+"lightningBalls", "inWallTimeout", trTimeMS() + 300);
					yVarToVector("p"+p+"lightningBalls", "lastGood");
					trVectorQuestVarSet("dir", zGetUnitVector("pos", "lastGood"));
					ySetVarFromVector("p"+p+"lightningBalls", "dir", "dir");
					lightningBallBounce(p);
				}
				if (trQuestVarGet("posY") < 1.0 && yGetVar("p"+p+"lightningBalls", "yeehaw") == 0) {
					lightningBallBounce(p);
				}

				dist = xsPow(trQuestVarGet("p"+p+"rideLightningRange"), 2);
				for(x=yGetDatabaseCount("p"+p+"rideLightningTargets"); >0) {
					id = yDatabaseNext("p"+p+"rideLightningTargets", true);
					if (id == -1 || trUnitAlive() == false) {
						yRemoveFromDatabase("p"+p+"rideLightningTargets");
					} else if (zDistanceToVectorSquared("p"+p+"rideLightningTargets", "pos") < dist) {
						ySetPointer("enemies", 1*yGetVar("p"+p+"rideLightningTargets", "index"));
						damageEnemy(p, yGetVar("p"+p+"lightningBalls", "damage"), true);
						trUnitHighlight(0.1, false);
						yRemoveFromDatabase("p"+p+"rideLightningTargets");
					}
				}
			}
		}
		if (trTimeMS() > trQuestVarGet("p"+p+"rideLightningNext")) {
			trQuestVarSet("p"+p+"rideLightningNext", 
				trQuestVarGet("p"+p+"rideLightningNext") + trQuestVarGet("rideLightningDelay") * trQuestVarGet("p"+p+"ultimateCost"));
			trPlayerGrantResources(p, "favor", -1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"rideLightning", 0);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
				rideLightningOff(p);
			}
		}
	}
	

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		vectorSnapToGrid("p"+p+"wellPos");
		posX = trQuestVarGet("p"+p+"wellPosx");
		posZ = trQuestVarGet("p"+p+"wellPosz");
		trSoundPlayFN("lightningstrike3.wav", "1", -1, "","");
		if (trQuestVarGet("p"+p+"rideLightning") == 0) {
			/* dash */
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				id = yDatabaseNext("p"+p+"characters", true);
				if (id == -1 || trUnitAlive() == false) {
					removeThunderRider(p);
				} else {
					trVectorSetUnitPos("pos", "p"+p+"characters");
					target = 1 + xsMin(trQuestVarGet("blitzRange") * trQuestVarGet("p"+p+"spellRange"), 
						zDistanceBetweenVectors("pos", "p"+p+"wellPos")) / 2;
					trVectorQuestVarSet("step", zGetUnitVector("pos", "p"+p+"wellPos"));
					for(i=target; >0) {
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						yAddToDatabase("p"+p+"blitzSFX", "next");
						trArmyDispatch(""+p+",0", "Dwarf", 1, trQuestVarGet("posX"), 0, trQuestVarGet("posZ"), 0, true);
						trArmySelect(""+p+",0");
						trUnitChangeProtoUnit("Victory Marker");
						trQuestVarSet("posx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("stepx"));
						trQuestVarSet("posz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("stepz"));
						vectorToGrid("pos", "loc");
						if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							break;
						}
					}
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trMutateSelected(kbGetProtoUnitID("Siege Tower"));
					trUnitChangeProtoUnit("Hero Greek Atalanta");
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trSetUnitOrientation(trVectorQuestVarGet("step"), vector(0,1,0), true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitChangeProtoUnit("Lightning Sparks");
				}
			}
			/* reload relics */
			for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
				yDatabaseNext("p"+p+"relics", true);
				trUnitChangeProtoUnit("Relic");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"relics");
				trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
				trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
				if (yGetVar("p"+p+"relics", "type") < RELIC_KEY_GREEK) {
					trSetSelectedScale(0,0,0);
					trUnitSetAnimationPath("1,0,1,1,0,0,0");
				}
			}
		} else {
			/* ride the lightning direction change */
			for(x=yGetDatabaseCount("p"+p+"lightningBalls"); >0) {
				yDatabaseNext("p"+p+"lightningBalls");
				trVectorSetUnitPos("pos", "p"+p+"lightningBalls");
				trVectorQuestVarSet("dir", zGetUnitVector("pos", "p"+p+"wellPos"));
				ySetVarFromVector("p"+p+"lightningBalls", "dir", "dir");
				lightningBallBounce(p);
			}
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trSoundPlayFN("suckup1.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters", true);
			trUnitHighlight(0.5, false);
			healUnit(p, trQuestVarGet("p"+p+"thunderRiderBonus"));
			trVectorSetUnitPos("pos", "p"+p+"characters");
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
			trArmySelect(""+p+",0");
			trUnitChangeProtoUnit("Regeneration SFX");
		}
		trPlayerGrantResources(p, "favor", 0.1 * trQuestVarGet("p"+p+"thunderRiderBonus"));
		trQuestVarSet("p"+p+"thunderRiderBonus", 0);
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("end", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"rideLightning", 1 - trQuestVarGet("p"+p+"rideLightning"));
		if (trQuestVarGet("p"+p+"rideLightning") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"rideLightning", 0);
			} else {
				refreshRideLightningTargets(p);
				trQuestVarSet("p"+p+"rideLightningNext", 
					trTimeMS() + trQuestVarGet("rideLightningDelay") * trQuestVarGet("p"+p+"ultimateCost"));
				trSoundPlayFN("lightningbirth.wav","1",-1,"","");
				trModifyProtounit("Attack Revealer", p, 2, 10);
				zSetProtoUnitStat("Kronny Flying", p, 1, 2.0 * trQuestVarGet("p"+p+"speed"));
				trQuestVarSet("p"+p+"rideLightningRange", 
					trQuestVarGet("rideLightningRange") * trQuestVarGet("p"+p+"spellRange"));
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					id = yDatabaseNext("p"+p+"characters");
					if (id == -1 || trUnitAlive() == false) {
						removeThunderRider(p);
					} else {
						trVectorSetUnitPos("start", "p"+p+"characters");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
						trQuestVarSet("heading", 180.0 / 3.141592 * angleBetweenVectors("start", "end"));
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						ySetVar("p"+p+"characters", "lightningIndex", yAddToDatabase("p"+p+"lightningBalls", "next"));
						yAddUpdateVar("p"+p+"lightningBalls", "dirX", trQuestVarGet("dirX"));
						yAddUpdateVar("p"+p+"lightningBalls", "dirZ", trQuestVarGet("dirZ"));
						yAddUpdateVar("p"+p+"lightningBalls", "damage", 
							trQuestVarGet("rideLightningDamage") * trQuestVarGet("p"+p+"spellDamage"));
						yAddUpdateVar("p"+p+"lightningBalls", "yeehaw", 1);
						yAddUpdateVar("p"+p+"lightningBalls", "inWall", 0);

						trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
						trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
						trSetSelectedScale(0, -4.5, 0);
						trDamageUnitPercent(100);

						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						yAddUpdateVar("p"+p+"lightningBalls", "extra", trQuestVarGet("next"));
						trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
						trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
						trSetSelectedScale(0, -4.5, 0);
						trDamageUnitPercent(100);

						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"characters", true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
			}
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
			rideLightningOff(p);
		}
	}

	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	/* passive attack boost */
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		/* percentage of a second that has passed */
		amt = 0.001 * (trTimeMS() - trQuestVarGet("p"+p+"thunderRiderBonusLast"));
		amt = xsMax(amt, trQuestVarGet("p"+p+"thunderRiderBonus") * amt * 0.1);
		trQuestVarSet("p"+p+"thunderRiderBonusLast", trTimeMS());
		trQuestVarSet("p"+p+"thunderRiderBonus", trQuestVarGet("p"+p+"thunderRiderBonus") - amt);
		if (trQuestVarGet("p"+p+"rideLightning") == 0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeThunderRider(p);
			} else {
				hit = CheckOnHit(p, id);
				if (hit == ON_HIT_SPECIAL) {
					yClearDatabase("p"+p+"thunderShockTargets");
					target = kbUnitGetTargetUnitID(id);
					trQuestVarSet("target", trGetUnitScenarioNameNumber(target));
					yAddToDatabase("p"+p+"thunderShocks", "target");
					yAddUpdateVar("p"+p+"thunderShocks", "damage", trQuestVarGet("p"+p+"attack"));
					yAddUpdateVar("p"+p+"thunderShocks", "next", trTimeMS() + 100);
					trVectorSetUnitPos("pos", "target");
					yAddUpdateVar("p"+p+"thunderShocks", "posX", trQuestVarGet("posX"));
					yAddUpdateVar("p"+p+"thunderShocks", "posZ", trQuestVarGet("posZ"));
					trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					trArmySelect(""+p+",0");
					trUnitChangeProtoUnit("Lightning Sparks");
					for(x=yGetDatabaseCount("enemies"); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else {
							if ((id == target) == false) {
								yAddToDatabase("p"+p+"thunderShockTargets", "enemies");
								yAddUpdateVar("p"+p+"thunderShockTargets", "index", yGetPointer("enemies"));
							}
						}
					}
					trQuestVarSetFromRand("sound", 1, 4, true);
					trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
				yVarToVector("p"+p+"characters", "prev");
				trVectorSetUnitPos("pos", "p"+p+"characters");
				dist = zDistanceBetweenVectors("pos", "prev");
				if (dist == 0) {
					trQuestVarSet("p"+p+"thunderRiderBonus", trQuestVarGet("p"+p+"thunderRiderBonus") - amt);
				} else {
					trQuestVarSet("p"+p+"thunderRiderBonus", 
						trQuestVarGet("p"+p+"thunderRiderBonus") + dist * 0.1 * trQuestVarGet("p"+p+"baseAttack"));
					ySetVarFromVector("p"+p+"characters", "prev", "pos");
				}
			}
		}
		trQuestVarSet("p"+p+"thunderRiderBonus", xsMax(0, trQuestVarGet("p"+p+"thunderRiderBonus")));
		trQuestVarSet("p"+p+"attack", trQuestVarGet("p"+p+"baseAttack") + trQuestVarGet("p"+p+"thunderRiderBonus"));
		zSetProtoUnitStat("Hero Greek Atalanta", p, 27, trQuestVarGet("p"+p+"attack"));
	}

	if (yGetDatabaseCount("p"+p+"thunderShocks") > 0) {
		yDatabaseNext("p"+p+"thunderShocks");
		if (trTimeMS() > yGetVar("p"+p+"thunderShocks", "next")) {
			ySetVar("p"+p+"thunderShocks", "next", 100 + yGetVar("p"+p+"thunderShocks", "next"));
			yVarToVector("p"+p+"thunderShocks", "pos");
			dist = 16;
			trQuestVarSet("temp", -1);
			for(x=yGetDatabaseCount("p"+p+"thunderShockTargets"); >0) {
				id = yDatabaseNext("p"+p+"thunderShockTargets", true);
				if (id == -1 || trUnitAlive() == false) {
					yRemoveFromDatabase("p"+p+"thunderShockTargets");
				} else {
					amt = zDistanceToVectorSquared("p"+p+"thunderShockTargets", "pos");
					if (amt < dist) {
						trQuestVarSet("temp", yGetPointer("p"+p+"thunderShockTargets"));
					}
				}
			}
			if (trQuestVarGet("temp") == -1) {
				yRemoveFromDatabase("p"+p+"thunderShocks");
			} else {
				ySetPointer("p"+p+"thunderShockTargets", 1*trQuestVarGet("temp"));
				/* this is the new thundershock center */
				ySetUnit("p"+p+"thunderShocks", trQuestVarGet("p"+p+"thunderShockTargets"));
				
				ySetPointer("enemies", 1*yGetVar("p"+p+"thunderShockTargets", "index"));
				yRemoveFromDatabase("p"+p+"thunderShockTargets");

				trVectorSetUnitPos("pos", "enemies");
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect(""+p+",0");
				trUnitChangeProtoUnit("Lightning Sparks Ground");
				ySetVarFromVector("p"+p+"thunderShocks", "pos", "pos");
				trUnitSelectClear();
				trUnitSelectByQV("enemies");
				trUnitHighlight(0.2, false);
				damageEnemy(p, yGetVar("p"+p+"thunderShocks", "damage"), false);
			}
		}
	}

	xsSetContextPlayer(old);
}

void chooseThunderRider(int eventID = -1) {
	int p = eventID - 1000 - 12 * THUNDERRIDER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Blitz";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Recharge";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Ride The Lightning";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("blitzCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("rechargeCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);

	trSetCivilizationNameOverride(p, "Thunder Rider | " + (1+trQuestVarGet("p"+p+"level")));
}

rule thunderRider_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * THUNDERRIDER + p, "thunderRiderAlways");
		trEventSetHandler(1000 + 12 * THUNDERRIDER + p, "chooseThunderRider");
	}

	trQuestVarSet("blitzCooldown", 8);
	trQuestVarSet("blitzRange", 12);
	trQuestVarSet("blitzDamage", 50);

	trQuestVarSet("rechargeCooldown", 18);

	trQuestVarSet("rideLightningDamage", 100);
	trQuestVarSet("rideLightningRange", 4);
	trQuestVarSet("rideLightningCost", 8);
	trQuestVarSet("rideLightningDelay", 1000 / trQuestVarGet("rideLightningCost"));
}