void removeFrostKnight(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "blizzardSFX");
}

void frostknightAlways(int eventID = -1) {
	int p = eventID - 12 * FROSTKNIGHT;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeFrostKnight(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_SPECIAL) {
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					stunUnit("enemies", 1.5, p);
				}
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"icicles") > 0) {
		yDatabaseNext("p"+p+"icicles", true);
		trQuestVarSet("scale", (yGetVar("p"+p+"icicles", "timeout") - trTimeMS()) / trQuestVarGet("icicleDuration") / 1000);
		trQuestVarSet("scale", 1.0 + 3.0 * trQuestVarGet("scale"));
		if (trQuestVarGet("scale") <= 1.0) {
			trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardSFX"), true);
			trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardBlock"), true);
			trUnitDestroy();
			yRemoveFromDatabase("p"+p+"icicles");
		} else {
			trSetSelectedScale(yGetVar("p"+p+"icicles", "radius"),trQuestVarGet("scale"),yGetVar("p"+p+"icicles", "radius"));
		}
	}

	if (trQuestVarGet("p"+p+"blizzard") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"blizzardNext")) {
			trQuestVarSet("p"+p+"blizzardNext", trQuestVarGet("p"+p+"blizzardNext") + 1000);
			for (x=yGetDatabaseCount("enemies"); >0) {
				id = yDatabaseNext("enemies", true);
				if (id == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else {
					hit = trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Victory Marker",
						trQuestVarGet("p"+p+"spellRange") * trQuestVarGet("blizzardRadius"));
					hit = hit + trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Frost Giant",
						trQuestVarGet("p"+p+"spellRange") * trQuestVarGet("blizzardRadius"));
					hit = hit + trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Hero Greek Achilles",
						trQuestVarGet("p"+p+"spellRange") * trQuestVarGet("blizzardRadius"));
					damageEnemy(p, trQuestVarGet("blizzardDamage") * trQuestVarGet("p"+p+"spellDamage") * hit, true);
				}
			}
		}
		if (trTimeMS() > trQuestVarGet("p"+p+"blizzardTimeout")) {
			trQuestVarSet("p"+p+"blizzard", 0);
			for(x=yGetDatabaseCount("p"+p+"icicles"); >0) {
				yDatabaseNext("p"+p+"icicles");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardBlock"), true);
				trUnitChangeProtoUnit("Cinematic Block");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardSFX"), true);
				trUnitChangeProtoUnit("Cinematic Block");
			}
			for(x=yGetDatabaseCount("p"+p+"frostGiants"); >0) {
				yDatabaseNext("p"+p+"frostGiants");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"frostGiants", "blizzardSFX"), true);
				trUnitChangeProtoUnit("Cinematic Block");
			}
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				yDatabaseNext("p"+p+"characters");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"characters", "blizzardSFX"), true);
				trUnitChangeProtoUnit("Cinematic Block");
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trQuestVarSetFromRand("heading",1,360,true);
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trQuestVarSet("marker", trQuestVarGet("next") + 1);
		trQuestVarSet("blizzardSFX", trQuestVarGet("next") + 2);
		trArmyDispatch(""+p+",0","Dwarf",3,
			trQuestVarGet("p"+p+"wellPosx"),0,trQuestVarGet("p"+p+"wellPosz"),trQuestVarGet("heading"),true);
		

		yAddToDatabase("p"+p+"icicles", "next");
		yAddUpdateVar("p"+p+"icicles", "blizzardBlock", trQuestVarGet("marker"));
		yAddUpdateVar("p"+p+"icicles", "blizzardSFX", trQuestVarGet("blizzardSFX"));
		yAddUpdateVar("p"+p+"icicles", "timeout", 
			trTimeMS() + 1000 * trQuestVarGet("icicleDuration") * trQuestVarGet("p"+p+"spellDuration"));
		yAddUpdateVar("p"+p+"icicles", "radius", trQuestVarGet("icicleRadius") * trQuestVarGet("p"+p+"spellRange"));

		if (trQuestVarGet("p"+p+"blizzard") == 1) {
			trUnitSelectClear();
			trUnitSelectByQV("blizzardSFX", true);
			trUnitChangeProtoUnit("Frost Drift");
			trUnitSelectClear();
			trUnitSelectByQV("marker", true);
			trUnitChangeProtoUnit("Victory Marker");
			trUnitSelectClear();
			trUnitSelectByQV("blizzardSFX", true);
			trUnitOverrideAnimation(2,0,1,1,-1);
			trSetSelectedScale(1.25 * trQuestVarGet("p"+p+"spellRange"),1,1.6*trQuestVarGet("p"+p+"spellRange"));
		} else {
			trUnitSelectClear();
			trUnitSelectByQV("marker", true);
			trUnitChangeProtoUnit("Cinematic Block");
			trUnitSelectClear();
			trUnitSelectByQV("blizzardSFX", true);
			trUnitChangeProtoUnit("Cinematic Block");
		}

		trUnitSelectClear();
		trUnitSelectByQV("next", true);
		trMutateSelected(kbGetProtoUnitID("Ice Block"));
		trUnitSetAnimationPath("0,0,0,0,0,0,0");
		trSetSelectedScale(trQuestVarGet("icicleRadius") * trQuestVarGet("p"+p+"spellRange"),
			3,trQuestVarGet("icicleRadius") * trQuestVarGet("p"+p+"spellRange"));
		trUnitHighlight(trQuestVarGet("icicleDuration") * trQuestVarGet("p"+p+"spellDuration"), false);

		trSoundPlayFN("piercemetal3.wav", "1", -1, "", "");
		trSoundPlayFN("egg2.wav", "1", -1, "", "");
		trSoundPlayFN("icemono.wav", "1", -1, "", "");

		dist = xsPow(trQuestVarGet("p"+p+"spellRange") * trQuestVarGet("icicleRadius"), 2);
		hit = 0;
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else if (zDistanceToVectorSquared("enemies", "p"+p+"wellPos") < dist) {
				target = yGetPointer("enemies");
				hit = hit + 1;
				trPlayerGrantResources(p, "favor", 1);
				damageEnemy(p, trQuestVarGet("icicleDamage") * trQuestVarGet("p"+p+"spellDamage"), true);
			}
		}
		if (hit == 1) {
			ySetPointer("enemies", target);
			stunUnit("enemies", 1.5, p);
		}
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trSoundPlayFN("recreation.wav","1",-1,"","");
		trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
		trQuestVarSet("p"+p+"blizzard", 1);
		trQuestVarSet("p"+p+"blizzardNext", trTimeMS());
		trQuestVarSet("p"+p+"blizzardTimeout", 
			trTimeMS() + 1000 * trQuestVarGet("blizzardDuration") * trQuestVarGet("p"+p+"spellDuration"));
		for(x=yGetDatabaseCount("p"+p+"icicles"); >0) {
			yDatabaseNext("p"+p+"icicles");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardSFX"), true);
			trUnitChangeProtoUnit("Frost Drift");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardBlock"), true);
			trUnitChangeProtoUnit("Victory Marker");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardSFX"), true);
			trUnitOverrideAnimation(2,0,1,1,-1);
			trSetSelectedScale(1.25 * trQuestVarGet("p"+p+"spellRange"),1,1.6*trQuestVarGet("p"+p+"spellRange"));
		}

		for(x=yGetDatabaseCount("p"+p+"frostGiants"); >0) {
			id = yDatabaseNext("p"+p+"frostGiants", true);
			if (id == -1 || trUnitAlive() == false) {
				yRemoveFromDatabase("p"+p+"frostGiants");
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"frostGiants", "blizzardSFX"), true);
				trUnitChangeProtoUnit("Frost Drift");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"frostGiants", "blizzardSFX"), true);
				trUnitOverrideAnimation(2,0,1,1,-1);
				trSetSelectedScale(1.25 * trQuestVarGet("p"+p+"spellRange"),1,1.6*trQuestVarGet("p"+p+"spellRange"));
			}
		}

		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeFrostKnight();
			} else {
				if (yGetVar("p"+p+"characters", "blizzardSFX") == 0) {
					spyEffect(1*trQuestVarGet("p"+p+"characters"), 
						kbGetProtoUnitID("Frost Drift"), yGetVarName("p"+p+"characters", "blizzardSFX"));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "blizzardSFX"), true);
					trUnitChangeProtoUnit("Frost Drift");
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "blizzardSFX"), true);
					trUnitOverrideAnimation(2,0,1,1,-1);
					trSetSelectedScale(1.25 * trQuestVarGet("p"+p+"spellRange"),1,1.6*trQuestVarGet("p"+p+"spellRange"));
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("frostGiantCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitChangeProtoUnit("Frost Giant");
		trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("frostGiantCost") * trQuestVarGet("p"+p+"ultimateCost"));
		yAddToDatabase("p"+p+"frostGiants", "p"+p+"lureObject");
		yAddUpdateVar("p"+p+"frostGiants", "specialnext", trTimeMS());
		yAddUpdateVar("p"+p+"frostGiants", "step", 0);
		yAddUpdateVar("p"+p+"frostGiants", "index", yAddToDatabase("playerUnits", "p"+p+"lureObject"));
		yAddUpdateVar("playerUnits", "player", p);
		yAddUpdateVar("playerUnits", "decay", calculateDecay(p, trQuestVarGet("frostGiantDecay")));
		yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 1000);
		ySetPointer("p"+p+"frostGiants", yGetNewestPointer("p"+p+"frostGiants"));
		if (trQuestVarGet("p"+p+"blizzard") == 0) {
			spyEffect(1*trQuestVarGet("p"+p+"lureObject"),
				kbGetProtoUnitID("Cinematic Block"),yGetVarName("p"+p+"frostGiants", "blizzardSFX"));
		} else {
			spyEffect(1*trQuestVarGet("p"+p+"lureObject"),
				kbGetProtoUnitID("Frost Drift"),yGetVarName("p"+p+"frostGiants", "blizzardSFX"));
		}
	}

	if(yGetDatabaseCount("p"+p+"frostGiants") >0) {
		id = yDatabaseNext("p"+p+"frostGiants", true);
		if (id == -1 || trUnitAlive() == false) {
            trUnitChangeProtoUnit("Frost Giant");
			yRemoveFromDatabase("p"+p+"frostGiants");
			yRemoveUpdateVar("p"+p+"frostGiants", "step");
		} else if (trTimeMS() > yGetVar("p"+p+"frostGiants", "specialnext")) {
			switch(1*yGetVar("p"+p+"frostGiants", "step"))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						target = kbUnitGetTargetUnitID(id);
						ySetVar("p"+p+"frostGiants", "target", trGetUnitScenarioNameNumber(target));
						ySetVar("p"+p+"frostGiants", "step", 1);
						ySetVar("p"+p+"frostGiants", "specialnext", trTimeMS() + 1400);
						trUnitOverrideAnimation(40,0,false,false,-1);
					}
				}
				case 1:
				{
					hit = 0;
					for (x=yGetDatabaseCount("enemies"); >0) {
						if (yGetVar("p"+p+"frostGiants", "target") == yDatabaseNext("enemies")) {
							trUnitSelectClear();
							trUnitSelectByQV("enemies");
							stunUnit("enemies", 2.0, p);
							hit = 1;
							break;
						}
					}
					ySetVar("p"+p+"frostGiants", "step", 2);
					ySetVar("p"+p+"frostGiants", "specialnext", yGetVar("p"+p+"frostGiants", "specialnext") + 600);
					if (hit == 0) {
						ySetVar("p"+p+"frostGiants", "target", -1);
					}
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					ySetVar("p"+p+"frostGiants", "step", 0);
					if (yGetVar("p"+p+"frostGiants", "target") == -1) {
						ySetVar("p"+p+"frostGiants", "specialnext", trTimeMS());
					} else {
						ySetVar("p"+p+"frostGiants", "specialnext", trTimeMS() + 15000 * trQuestVarGet("p"+p+"cooldownReduction"));
					}
				}
			}
		}
	}

	xsSetContextPlayer(old);
}

void chooseFrostKnight(int eventID = -1) {
	int p = eventID - 1000 - 12 * FROSTKNIGHT;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Icicle";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Blizzard";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Frost Giant";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("icicleCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("frostGiantCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("blizzardCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

void frostknightModify(int eventID = -1) {
	int p = eventID - 5000 - 12 * FROSTKNIGHT;
	zSetProtoUnitStat("Frost Giant", p, 0, trQuestVarGet("p"+p+"health"));
	zSetProtoUnitStat("Frost Giant", p, 27, trQuestVarGet("p"+p+"baseAttack"));
}

rule frostknight_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * FROSTKNIGHT + p, "frostknightAlways");
		trEventSetHandler(1000 + 12 * FROSTKNIGHT + p, "chooseFrostKnight");
		trEventSetHandler(5000 + 12 * FROSTKNIGHT + p, "frostknightModify");
	}

	trQuestVarSet("icicleCooldown", 4);
	trQuestVarSet("icicleDamage", 80);
	trQuestVarSet("icicleDuration", 6);
	trQuestVarSet("icicleRadius", 1.5);

	trQuestVarSet("blizzardCooldown", 16);
	trQuestVarSet("blizzardDuration", 6);
	trQuestVarSet("blizzardDamage", 20);
	trQuestVarSet("blizzardRadius", 4);

	trQuestVarSet("frostGiantDecay", 3);
	trQuestVarSet("frostGiantCost", 60);
}