void removeThunderRider(int p = 0) {
	removePlayerCharacter(p);
	yRemoveUpdateVar("p"+p+"characters", "prevX");
	yRemoveUpdateVar("p"+p+"characters", "prevZ");
	yRemoveUpdateVar("p"+p+"characters", "lightning");
}

void removeLightningBall(int p = 0) {
	yRemoveFromDatabase("p"+p+"lightningBalls");
	yRemoveUpdateVar("p"+p+"lightningBalls", "dirX");
	yRemoveUpdateVar("p"+p+"lightningBalls", "dirZ");
	yRemoveUpdateVar("p"+p+"lightningBalls", "damage");
}

void rideLightningOff(int p = 0) {
	trModifyProtounit("Attack Revealer", p, 2, -10);
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
						trChatSend(0, "we're going to mars baby!");
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						yAddToDatabase("p"+p+"blitzSFX", "next");
						trArmyDispatch(""+p+",0", "Dwarf", 1, trQuestVarGet("posX"), 0, trQuestVarGet("posZ"), 0, true);
						trArmySelect(""+p+",0");
						trUnitChangeProtoUnit("Victory Marker");
						trQuestVarSet("posx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("stepx"));
						trQuestVarSet("posz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("stepz"));
						vectorToGrid("pos", "loc");
						if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							trChatSend(0, "oh shi-!");
							break;
						}
					}
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0", "Transport Ship Greek", 1, trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trMutateSelected(kbGetProtoUnitID("Siege Tower"));
					trUnitChangeProtoUnit("Dwarf");
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trMutateSelected(kbGetProtoUnitID("Hero Greek Atalanta"));
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
				if (yGetVar("p"+p+"relics", "type") < RELIC_KEY_GREEK) {
					trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
					trSetSelectedScale(0,0,0);
					trUnitSetAnimationPath("1,0,1,1,0,0,0");
				}
			}
		} else {
			/* ride the lightning direction change */
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
				trQuestVarSet("p"+p+"rideLightningNext", 
					trTimeMS() + trQuestVarGet("rideLightningDelay") * trQuestVarGet("p"+p+"ultimateCost"));
				trSoundPlayFN("lightningbirth.wav","1",-1,"","");
				trModifyProtounit("Attack Revealer", p, 2, 10);
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					id = yDatabaseNext("p"+p+"characters");
					if (id == -1 || trUnitAlive() == false) {
						removeThunderRider(p);
					} else {
						trVectorSetUnitPos("start", "p"+p+"characters");
						trVectorQuestVarSet("dir", xsVectorNormalize(trVectorQuestVarGet("end") - trVectorQuestVarGet("start")));
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						ySetVar("p"+p+"characters", "lightning", trQuestVarGet("next"));
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

	if (trQuestVarGet("p"+p+"rideLightning") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"rideLightningNext")) {
			trQuestVarSet("p"+p+"rideLightningNext", 
				trQuestVarGet("p"+p+"rideLightningNext") + trQuestVarGet("rideLightningDelay") * trQuestVarGet("p"+p+"ultimateCost"));
			trPlayerGrantResources(p, "favor", 0 - 1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"rideLightning", 0);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
				rideLightningOff(p);
			}
		}
		if (trCurrentPlayer() == p) {
			uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"unit"));
		}
	}

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

	trSetCivilizationNameOverride(p, "Thunder Rider | " + (1+trQuestVarGet("p"+p+"progress")));
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

	trQuestVarSet("rideLightningCost", 8);
	trQuestVarSet("rideLightningDelay", 1000 / trQuestVarGet("rideLightningCost"));
}