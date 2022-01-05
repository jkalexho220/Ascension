void removeMoonblade(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "crescentCount");
	yRemoveUpdateVar("p"+p+"characters", "crescentDamage");
	yRemoveUpdateVar("p"+p+"characters", "crescentTimeout");
	yRemoveUpdateVar("p"+p+"characters", "crescentSFX");
	yRemoveUpdateVar("p"+p+"characters", "crescentOn");
}

void moonbladeAlways(int eventID = -1) {
	int p = eventID - 12 * MOONBLADE;
	int id = 0;
	int hit = 0;
	int target = 0;
	float angle = 0;
	float posX = 0;
	float posZ = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeMoonblade(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit > ON_HIT_NONE) {
				if (yGetVar("p"+p+"characters", "crescentCount") > 0) {
					ySetVar("p"+p+"characters", "crescentCount", yGetVar("p"+p+"characters", "crescentCount") - 1);
					ySetVar("p"+p+"characters", "crescentTimeout", trTimeMS() + 5000);
					target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
					trVectorQuestVarSet("pos", kbGetBlockPosition(""+target));
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning Sparks Ground");
					trPlayerGrantResources(p, "favor", 3);
					for(x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies") == target) {
							trUnitSelectClear();
							trUnitSelectByQV("enemies", true);
							damageEnemy(p, 50*trQuestVarGet("p"+p+"spellDamage"), false);
							stunUnit("enemies", 1.5, p);
							break;
						}
					}
				}
				angle = trQuestVarGet("p"+p+"health") * trQuestVarGet("p"+p+"spellDamage") * 0.01;
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					yDatabaseNext("playerUnits", true);
					healUnit(p, angle);
				}
				if (hit == ON_HIT_SPECIAL) {
					if (trCurrentPlayer() == p) {
						trSoundPlayFN("mythcreate.wav","1",-1,"","");
					}
					trVectorSetUnitPos("pos", "p"+p+"characters");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					yAddToDatabase("playerUnits", "next");
					yAddUpdateVar("playerUnits", "player", p);
					yAddUpdateVar("playerUnits", "decay", calculateDecay(p, 5));
					yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 1000);
					yAddToDatabase("p"+p+"wolves", "next");
					trArmyDispatch(""+p+",0","Wolf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				}
			}
			if ((yGetVar("p"+p+"characters", "crescentCount") > 0) && 
				(trTimeMS() > yGetVar("p"+p+"characters", "crescentTimeout"))) {
				ySetVar("p"+p+"characters", "crescentTimeout", trTimeMS() + 5000);
				ySetVar("p"+p+"characters", "crescentCount", yGetVar("p"+p+"characters", "crescentCount") - 1);
			}

			if ((yGetVar("p"+p+"characters", "crescentOn") == 1) && (yGetVar("p"+p+"characters", "crescentCount") == 0)) {
				ySetVar("p"+p+"characters", "crescentOn", 0);
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "crescentSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("eclipsebirth.wav","1",-1,"","");
		vectorSnapToGrid("p"+p+"wellPos");
		posX = trQuestVarGet("p"+p+"wellPosx");
		posZ = trQuestVarGet("p"+p+"wellPosz");
		yAddToDatabase("p"+p+"moonbeams", "next");
		yAddUpdateVar("p"+p+"moonbeams", "radius", xsPow(trQuestVarGet("moonbeamRadius")*trQuestVarGet("p"+p+"spellRange"),2));
		yAddUpdateVar("p"+p+"moonbeams", "damage", trQuestVarGet("moonbeamDamage") * trQuestVarGet("p"+p+"spellDamage"));
		yAddUpdateVar("p"+p+"moonbeams", "damageBoost", 
			trQuestVarGet("moonbeamDamageBoost") * trQuestVarGet("p"+p+"spellDamage"));
		yAddUpdateVar("p"+p+"moonbeams", "timeout", 
			trTimeMS() + 1000 * trQuestVarGet("moonbeamDuration") * trQuestVarGet("p"+p+"spellDuration"));
		yAddUpdateVar("p"+p+"moonbeams", "start", trGetNextUnitScenarioNameNumber());
		yAddUpdateVar("p"+p+"moonbeams", "posx", posX);
		yAddUpdateVar("p"+p+"moonbeams", "posz", posZ);
		yAddUpdateVar("p"+p+"moonbeams", "next", trTimeMS());
		for(x=0; <16) {
			trVectorSetFromAngle("dir", angle);
			angle = angle + 6.283185 / 16;
			trQuestVarSet("dirx", 
				trQuestVarGet("dirx") * trQuestVarGet("moonbeamRadius") * trQuestVarGet("p"+p+"spellRange") + posX);
			trQuestVarSet("dirz", 
				trQuestVarGet("dirz") * trQuestVarGet("moonbeamRadius") * trQuestVarGet("p"+p+"spellRange") + posZ);
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("dirx"),0,trQuestVarGet("dirz"),0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trMutateSelected(kbGetProtoUnitID("Healing SFX"));
		}
		yAddUpdateVar("p"+p+"moonbeams", "end", trGetNextUnitScenarioNameNumber());
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"crescentStarted", 1);
		trSoundPlayFN("olympustemplesfx.wav","1",-1,"","");
		for(x=yGetDatabaseCount("p"+p+"Characters"); >0) {
			yDatabaseNext("p"+p+"Characters");
			ySetVar("p"+p+"Characters", "crescentCount", yGetVar("p"+p+"Characters", "crescentCount") + 3);
			ySetVar("p"+p+"Characters", "crescentDamage", trQuestVarGet("crescentDamage") * trQuestVarGet("p"+p+"spellDamage"));
			ySetVar("p"+p+"Characters", "crescentTimeout", trTimeMS() + 5000);
			ySetVar("p"+p+"characters", "crescentOn", 1);
			if ((yGetVar("p"+p+"Characters", "crescentSFX") == 0) ||
				(kbGetBlockID(""+1*yGetVar("p"+p+"characters", "crescentSFX")) == -1)) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"characters", true);
				spyEffect(1*trQuestVarGet("p"+p+"Characters"), 
					kbGetProtoUnitID("Rocket"), yGetVarName("p"+p+"characters", "crescentSFX"));
			}
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"protection", 1 - trQuestVarGet("p"+p+"protection"));
		if (trQuestVarGet("p"+p+"protection") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"protection", 0);
			} else {
				trQuestVarSet("protectionCount", trQuestVarGet("protectionCount") + 1);
				trQuestVarSet("p"+p+"protectionNext", 
					trTimeMS() + trQuestVarGet("protectionDelay") * trQuestVarGet("p"+p+"ultimateCost"));
				trSoundPlayFN("bronzebirth.wav","1",-1,"","");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					id = yDatabaseNext("playerUnits", true);
					xsSetContextPlayer(1*yGetVar("playerUnits", "player"));
					ySetVar("playerUnits", "currentHealth", kbUnitGetCurrentHitpoints(id));
				}
			}
		} else {
			trQuestVarSet("protectionCount", trQuestVarGet("protectionCount") - 1);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}

	if ((trQuestVarGet("p"+p+"crescentStarted") == 1) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			if (yGetVar("p"+p+"characters", "crescentSFX") < 0) {
				ySetVar("p"+p+"characters", "crescentSFX", trQuestVarGet("spyEye"+(0-yGetVar("p"+p+"characters", "crescentSFX"))));
			}
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"characters", "crescentSFX"), true);
			trMutateSelected(kbGetProtoUnitID("Outpost"));
			trSetSelectedScale(0.0,0.0,0.0);
		}
		trQuestVarSet("p"+p+"crescentStarted", 0);
	}

	if (yGetDatabaseCount("p"+p+"moonbeams") > 0) {
		target = 0;
		yDatabaseNext("p"+p+"moonbeams");
		if (trTimeMS() > yGetVar("p"+p+"moonbeams", "next")) {
			ySetVar("p"+p+"moonbeams", "next", trTimeMS() + 500);
			trQuestVarSet("centerX", yGetVar("p"+p+"moonbeams", "posX"));
			trQuestVarSet("centerZ", yGetVar("p"+p+"moonbeams", "posZ"));
			angle = yGetVar("p"+p+"moonbeams", "radius");
			for(x=yGetDatabaseCount("playerUnits"); >0) {
				yDatabaseNext("playerUnits");
				if (zDistanceToVectorSquared("playerUnits", "center") < angle) {
					target = target + 1;
				}
			}
			posX = yGetVar("p"+p+"moonbeams", "damage") + target * yGetVar("p"+p+"moonbeams", "damageBoost");
			posX = posX * 0.5;
			for(x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "center") < angle) {
					damageEnemy(p, posX);
				}
			}
			if (trTimeMS() > yGetVar("p"+p+"moonbeams", "timeout")) {
				for(x=yGetVar("p"+p+"moonbeams", "start"); < yGetVar("p"+p+"moonbeams", "end")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trUnitDestroy();
				}
				yRemoveFromDatabase("p"+p+"moonbeams");
				yRemoveUpdateVar("p"+p+"moonbeams", "radius");
				yRemoveUpdateVar("p"+p+"moonbeams", "damage");
				yRemoveUpdateVar("p"+p+"moonbeams", "damageBoost");
				yRemoveUpdateVar("p"+p+"moonbeams", "timeout");
				yRemoveUpdateVar("p"+p+"moonbeams", "posX");
				yRemoveUpdateVar("p"+p+"moonbeams", "posZ");
				yRemoveUpdateVar("p"+p+"moonbeams", "next");
				yRemoveUpdateVar("p"+p+"moonbeams", "start");
				yRemoveUpdateVar("p"+p+"moonbeams", "end");
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"wolves") > 0) {
		yDatabaseNext("p"+p+"wolves", true);
		if (trUnitAlive() == false) {
			trUnitChangeProtoUnit("Conversion SFX");
			yRemoveFromDatabase("p"+p+"wolves");
		}
	}

	if (trQuestVarGet("p"+p+"protection") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"protectionNext")) {
			trQuestVarSet("p"+p+"protectionNext", 
				trQuestVarGet("p"+p+"protectionNext") + trQuestVarGet("protectionDelay") * trQuestVarGet("p"+p+"ultimateCost"));
			trPlayerGrantResources(p, "favor", 0 - 1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"protection", 0);
				trQuestVarSet("protectionCount", trQuestVarGet("protectionCount") - 1);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}

	xsSetContextPlayer(old);
}

void chooseMoonblade(int eventID = -1) {
	int p = eventID - 1000 - 12 * MOONBLADE;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Moonbeam";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Protection";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Crescent Strikes";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("moonbeamCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("crescentCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

rule moonblade_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * MOONBLADE + p, "moonbladeAlways");
		trEventSetHandler(1000 + 12 * MOONBLADE + p, "chooseMoonblade");
	}
	trQuestVarSet("moonbeamCooldown", 18);
	trQuestVarSet("moonbeamDamage", 30);
	trQuestVarSet("moonbeamDamageBoost", 15);
	trQuestVarSet("moonbeamDuration", 6);
	trQuestVarSet("moonbeamRadius", 6);

	trQuestVarSet("crescentCooldown", 12);
	trQuestVarSet("crescentCount", 3);
	trQuestVarSet("crescentDamage", 50);

	trQuestVarSet("protectionCost", 12);
	trQuestVarSet("protectionDelay", 1000 / 12);
}