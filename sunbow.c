void removeSunbow(int p = 0) {
	removePlayerCharacter(p);
	yRemoveUpdateVar("p"+p+"characters", "searingSFX");
}

void sunbowAlways(int eventID = -1) {
	int p = eventID - 12 * SUNBOW;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float posX = 0;
	float posZ = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeSunbow(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_NORMAL) {
				if (trQuestVarGet("p"+p+"healingRays") == 1) {
					trPlayerGrantResources(p, "favor", 1);
					target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
					trVectorQuestVarSet("end", kbGetBlockPosition(""+target));
					trVectorSetUnitPos("start", "p"+p+"characters");
					trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trUnitHighlight(1.0, false);
					trSetUnitOrientation(xsVectorSet(0.0 - trQuestVarGet("dirx"),0, 0.0 - trQuestVarGet("dirz")), vector(0,1,0), true);
					yAddToDatabase("playerLasers", "next");
					yAddUpdateVar("playerLasers", "timeout", trTimeMS() + 500);
					yAddUpdateVar("playerLasers", "range", trQuestVarGet("p"+p+"range") * 1.3);
					amt = trQuestVarGet("p"+p+"attack");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						yDatabaseNext("playerUnits");
						dist = zDistanceToVector("playerUnits", "start");
						if (dist < trQuestVarGet("p"+p+"range") + 3) {
							trQuestVarSet("hitboxX", trQuestVarGet("startX") + dist * trQuestVarGet("dirx"));
							trQuestVarSet("hitboxZ", trQuestVarGet("startZ") + dist * trQuestVarGet("dirz"));
							if (zDistanceToVectorSquared("playerUnits", "hitbox") < 9) {
								trUnitSelectClear();
								trUnitSelectByQV("playerUnits", true);
								healUnit(p, amt);
							}
						}
						
					}
					if (trQuestVarGet("p"+p+"searing") == 1) {
						for(x=yGetDatabaseCount("enemies"); >0) {
							yDatabaseNext("enemies");
							dist = zDistanceToVector("enemies", "start");
							if (dist < trQuestVarGet("p"+p+"range") + 3) {
								trQuestVarSet("hitboxX", trQuestVarGet("startX") + dist * trQuestVarGet("dirx"));
								trQuestVarSet("hitboxZ", trQuestVarGet("startZ") + dist * trQuestVarGet("dirz"));
								if (zDistanceToVectorSquared("enemies", "hitbox") < 9) {
									trUnitSelectClear();
									trUnitSelectByQV("enemies", true);
									damageEnemy(p, amt * trQuestVarGet("p"+p+"healBoost"));
								}
							}
						}
					}
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("restorationbirth.wav","1",-1,"","");
		vectorSnapToGrid("p"+p+"wellPos");
		posX = trQuestVarGet("p"+p+"wellPosx");
		posZ = trQuestVarGet("p"+p+"wellPosz");
		yAddToDatabase("p"+p+"sunlights", "next");
		yAddUpdateVar("p"+p+"sunlights", "radius", xsPow(trQuestVarGet("sunlightRadius")*trQuestVarGet("p"+p+"spellRange"),2));
		yAddUpdateVar("p"+p+"sunlights", "power", trQuestVarGet("sunlightPower") * trQuestVarGet("p"+p+"spellDamage"));
		yAddUpdateVar("p"+p+"sunlights", "timeout", 
			trTimeMS() + 1000 * trQuestVarGet("sunlightDuration") * trQuestVarGet("p"+p+"spellDuration"));
		yAddUpdateVar("p"+p+"sunlights", "start", trGetNextUnitScenarioNameNumber());
		yAddUpdateVar("p"+p+"sunlights", "posx", posX);
		yAddUpdateVar("p"+p+"sunlights", "posz", posZ);
		yAddUpdateVar("p"+p+"sunlights", "next", trTimeMS());
		for(x=0; <16) {
			trVectorSetFromAngle("dir", amt);
			amt = amt + 6.283185 / 16;
			trQuestVarSet("dirx", 
				trQuestVarGet("dirx") * trQuestVarGet("sunlightRadius") * trQuestVarGet("p"+p+"spellRange") + posX);
			trQuestVarSet("dirz", 
				trQuestVarGet("dirz") * trQuestVarGet("sunlightRadius") * trQuestVarGet("p"+p+"spellRange") + posZ);
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("dirx"),0,trQuestVarGet("dirz"),0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trUnitChangeProtoUnit("Relic");
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trMutateSelected(kbGetProtoUnitID("Hero Birth"));
		}
		yAddUpdateVar("p"+p+"sunlights", "end", trGetNextUnitScenarioNameNumber());
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		trQuestVarSet("p"+p+"healingrays", 1);
		trQuestVarSet("p"+p+"healingRaysTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("healingRaysDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trSoundPlayFN("skypassagein.wav","1",-1,"","");
		trQuestVarSet("p"+p+"nextDelay", 
			trQuestVarGet("class"+SUNBOW+"nextDelay") * 2.0 / (1.0 + trQuestVarGet("p"+p+"projectiles")));
	}

	if ((trQuestVarGet("p"+p+"healingRays") == 1) && (trTimeMS() > trQuestVarGet("p"+p+"healingRaysTimeout"))) {
		trQuestVarSet("p"+p+"healingRays", 0);
		trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
		trQuestVarSet("p"+p+"nextDelay", trQuestVarGet("class"+SUNBOW+"nextDelay"));
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"searingStarted", 1);
		trQuestVarSet("p"+p+"searing", 1 - trQuestVarGet("p"+p+"searing"));
		if (trQuestVarGet("p"+p+"searing") == 1) {
			if (trPlayerResourceCount(p, "favor") < 1) {
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
				}
				trQuestVarSet("p"+p+"searing", 0);
			} else {
				trQuestVarSet("p"+p+"searingNext", 
					trTimeMS() + trQuestVarGet("searingDelay") * trQuestVarGet("p"+p+"ultimateCost"));
				trSoundPlayFN("flamingweapons.wav","1",-1,"","");
				for(x=yGetDatabaseCount("p"+p+"Characters"); >0) {
					yDatabaseNext("p"+p+"Characters");
					if ((yGetVar("p"+p+"Characters", "searingSFX") == 0) ||
						(kbGetBlockID(""+1*yGetVar("p"+p+"characters", "searingSFX")) == -1)) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"characters", true);
						spyEffect(1*trQuestVarGet("p"+p+"Characters"), 
							kbGetProtoUnitID("Rocket"), yGetVarName("p"+p+"characters", "searingSFX"));
					}
				}
			}
		} else {
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}

	if (trQuestVarGet("p"+p+"searing") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"searingNext")) {
			trQuestVarSet("p"+p+"searingNext", 
				trQuestVarGet("p"+p+"searingNext") + trQuestVarGet("searingDelay") * trQuestVarGet("p"+p+"ultimateCost"));
			trPlayerGrantResources(p, "favor", 0 - 1);
			if (trPlayerResourceCount(p, "favor") < 1) {
				trQuestVarSet("p"+p+"searing", 0);
				trQuestVarSet("p"+p+"searingStarted", 1);
				trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			}
		}
	}

	if ((trQuestVarGet("p"+p+"searingStarted") == 1) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			if (yGetVar("p"+p+"characters", "searingSFX") < 0) {
				ySetVar("p"+p+"characters", "searingSFX", trQuestVarGet("spyEye"+(0-yGetVar("p"+p+"characters", "searingSFX"))));
			}
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"characters", "searingSFX"), true);
			if (trQuestVarGet("p"+p+"searing") == 1) {
				trMutateSelected(kbGetProtoUnitID("Ball of Fire Impact"));
			} else {
				trMutateSelected(kbGetProtoUnitID("Rocket"));
			}
		}
		trQuestVarSet("p"+p+"searingStarted", 0);
	}

	if (yGetDatabaseCount("p"+p+"sunlights") > 0) {
		target = 0;
		yDatabaseNext("p"+p+"sunlights");
		if (trTimeMS() > yGetVar("p"+p+"sunlights", "next")) {
			ySetVar("p"+p+"sunlights", "next", trTimeMS() + 500);
			trQuestVarSet("centerX", yGetVar("p"+p+"sunlights", "posX"));
			trQuestVarSet("centerZ", yGetVar("p"+p+"sunlights", "posZ"));
			amt = yGetVar("p"+p+"sunlights", "radius");
			posX = yGetVar("p"+p+"sunlights", "power");
			posX = posX * 0.5;

			for (x=yGetDatabaseCount("playerUnits"); >0) {
				id = yDatabaseNext("playerUnits", true);
				if (id == -1 || trUnitAlive() == false) {
					removePlayerUnit();
				} else if (zDistanceToVectorSquared("playerUnits", "center") < amt) {
					healUnit(p, posX);
				}
			}

			if (trQuestVarGet("p"+p+"searing") == 1) {
				for(x=yGetDatabaseCount("enemies"); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "center") < amt) {
						damageEnemy(p, posX * trQuestVarGet("p"+p+"healBoost"));
					}
				}
			}
			
			if (trTimeMS() > yGetVar("p"+p+"sunlights", "timeout")) {
				for(x=yGetVar("p"+p+"sunlights", "start"); < yGetVar("p"+p+"sunlights", "end")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Rocket"));
					trUnitDestroy();
				}
				yRemoveFromDatabase("p"+p+"sunlights");
				yRemoveUpdateVar("p"+p+"sunlights", "radius");
				yRemoveUpdateVar("p"+p+"sunlights", "power");
				yRemoveUpdateVar("p"+p+"sunlights", "timeout");
				yRemoveUpdateVar("p"+p+"sunlights", "posX");
				yRemoveUpdateVar("p"+p+"sunlights", "posZ");
				yRemoveUpdateVar("p"+p+"sunlights", "next");
				yRemoveUpdateVar("p"+p+"sunlights", "start");
				yRemoveUpdateVar("p"+p+"sunlights", "end");
			}
		}
	}

	if (trTime() > trQuestVarGet("p"+p+"sunbowPassiveNext")) {
		trQuestVarSet("p"+p+"sunbowPassiveNext", trTime());
		for(x=yGetDatabaseCount("playerUnits"); >0) {
			id = yDatabaseNext("playerUnits", true);
			if ((id == -1) || trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				healUnit(1, 1);
			}
		}
	}

	xsSetContextPlayer(old);
}

void chooseSunbow(int eventID = -1) {
	int p = eventID - 1000 - 12 * SUNBOW;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Sunlight";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Searing Light";
		rainIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Healing Rays";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("sunlightCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("healingRaysCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

rule sunbow_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SUNBOW + p, "sunbowAlways");
		trEventSetHandler(1000 + 12 * SUNBOW + p, "chooseSunbow");
	}

	trQuestVarSet("sunlightCooldown", 18);
	trQuestVarSet("sunlightRadius", 6);
	trQuestVarSet("sunlightDuration", 6);
	trQuestVarSet("sunlightPower", 60);

	trQuestVarSet("healingRaysCooldown", 12);
	trQuestVarSet("healingRaysPower", 100);
	trQuestVarSet("healingRaysDuration", 5);

	trQuestVarSet("searingCost", 5);
	trQuestVarSet("searingDelay", 1000 / trQuestVarGet("searingCost"));
}