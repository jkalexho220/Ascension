void removeSparkWitch(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "attacking");
}

void sparkWitchAlways(int eventID = -1) {
	int p = eventID - 12 * SPARKWITCH;
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
			removeSparkWitch(p);
		} else {
			if (kbUnitGetAnimationActionType(id) == 59) {
				ySetVar("p"+p+"characters", "attacking", 1);
				ySetVar("p"+p+"characters", "attackTarget", trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id)));
				ySetVar("p"+p+"characters", "attackNext", trTimeMS() + trQuestVarGet("p"+p+"firstDelay"));
				trUnitOverrideAnimation(19,0,false,false,-1);
			}
			switch(1*yGetVar("p"+p+"characters", "attacking"))
			{
				case 1:
				{
					if (trTimeMS() > yGetVar("p"+p+"characters", "attackNext")) {
						ySetVar("p"+p+"characters", "attacking", 2);
						ySetVar("p"+p+"characters", "attackNext", 
							yGetVar("p"+p+"characters", "attackNext") + trQuestVarGet("p"+p+"firstDelay"));

						trVectorSetUnitPos("pos", "p"+p+"characters");
						trQuestVarSet("next", yGetVar("p"+p+"characters", "attackTarget"));
						trUnitSelectClear();
						trUnitSelectByQV("next");
						if (trUnitAlive()) {
							trVectorSetUnitPos("end", "next");
							yAddToDatabase("p"+p+"zaps", "next");
							yAddUpdateVar("p"+p+"zaps", "startx", trQuestVarGet("posx"));
							yAddUpdateVar("p"+p+"zaps", "startz", trQuestVarGet("posz"));
							yAddUpdateVar("p"+p+"zaps", "endx", trQuestVarGet("endx"));
							yAddUpdateVar("p"+p+"zaps", "endz", trQuestVarGet("endz"));
							yAddUpdateVar("p"+p+"zaps", "startTime", trTimeMS());
							yAddUpdateVar("p"+p+"zaps", "next", 0);
							yAddUpdateVar("p"+p+"zaps", "bounces", 2 + trQuestVarGet("p"+p+"projectiles"));
							for(x=yGetDatabaseCount("enemies"); >0) {
								if (yDatabaseNext("enemies") == trQuestVarGet("next")) {
									yAddUpdateVar("p"+p+"zaps", "index", yGetPointer("enemies"));
									break;
								}
							}
						}
					} else if (kbUnitGetAnimationActionType(id) != 16) {
						ySetVar("p"+p+"characters", "attacking", 0);
					}
				}
				case 2:
				{
					if (trTimeMS() > yGetVar("p"+p+"characters", "attackNext")) {
						trUnitOverrideAnimation(-1,0,false,true,-1);
						ySetVar("p"+p+"characters", "attacking", 0);
					}
				}
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"zaps") > 0) {
		yDatabaseNext("p"+p+"zaps");
		current = trTimeMS() - yGetVar("p"+p+"zaps", "startTime");
		if (current > yGetVar("p"+p+"zaps", "next")) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"zaps");
			if (trUnitAlive()) {
				trVectorSetUnitPos("end", "p"+p+"zaps");
				ySetVarFromVector("p"+p+"zaps", "end", "end");
			} else {
				yVarToVector("p"+p+"zaps", "end");
			}
			yVarToVector("p"+p+"zaps", "start");
			trQuestVarSet("dirx", trQuestVarGet("endx") - trQuestVarGet("startx"));
			trQuestVarSet("dirz", trQuestVarGet("endz") - trQuestVarGet("startz"));
			for(y=5; >0) {
				if (current > yGetVar("p"+p+"zaps", "next")) {
					ySetVar("p"+p+"zaps", "next", yGetVar("p"+p+"zaps", "next") + 100);
					amt = yGetVar("p"+p+"zaps", "next") * 0.002;
					trQuestVarSet("posx", trQuestVarGet("startx") + trQuestVarGet("dirx") * amt);
					trQuestVarSet("posz", trQuestVarGet("startz") + trQuestVarGet("dirz") * amt);
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning Sparks Ground");
				} else {
					break;
				}
			}
			if (yGetVar("p"+p+"zaps", "next") >= 500) {
				trQuestVarSetFromRand("sound", 1, 5, true);
				trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"zaps");
				if (trUnitAlive()) {
					trUnitHighlight(0.2, false);
					if ((1*yGetVar("p"+p+"zaps", "index") > 0) && ySetPointer("enemies", 1*yGetVar("p"+p+"zaps", "index"))) {
						damageEnemy(p, 100.0 * trQuestVarGet("p"+p+"spellDamage"));
					}
				}
				ySetVar("p"+p+"zaps", "bounces", yGetVar("p"+p+"zaps", "bounces") - 1);
				if (yGetVar("p"+p+"zaps", "bounces") > 0) {
					dist = xsPow(trQuestVarGet("p"+p+"Range") - 5, 2);
					hit = 0;
					for(x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (trQuestVarGet("p"+p+"zaps") == trQuestVarGet("enemies")) {
							continue;
						} else if (zDistanceToVectorSquared("enemies", "end") < dist) {
							trVectorSetUnitPos("pos", "enemies");
							hit = 1;
							break;
						}
					}
					if (hit == 1) {
						ySetUnit("p"+p+"zaps", trQuestVarGet("enemies"));
						ySetVar("p"+p+"zaps", "index", yGetPointer("enemies"));
						ySetVar("p"+p+"zaps", "next", 0);
						ySetVar("p"+p+"zaps", "startTime", trTimeMS());
						ySetVarFromVector("p"+p+"zaps", "start", "end");
						ySetVarFromVector("p"+p+"zaps", "end", "pos");
					} else if (zDistanceToVectorSquared("p"+p+"unit", "end") < dist) {
						ySetUnit("p"+p+"zaps", trQuestVarGet("p"+p+"unit"));
						ySetVar("p"+p+"zaps", "index", -1);
						ySetVar("p"+p+"zaps", "next", 0);
						ySetVar("p"+p+"zaps", "startTime", trTimeMS());
						ySetVarFromVector("p"+p+"zaps", "start", "end");
						trVectorSetUnitPos("pos", "p"+p+"unit");
						ySetVarFromVector("p"+p+"zaps", "end", "pos");
					} else {
						yRemoveFromDatabase("p"+p+"zaps");
					}
				} else {
					yRemoveFromDatabase("p"+p+"zaps");
				}
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		gainFavor(p, 0.0 - trQuestVarGet("vortexCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trVectorSetUnitPos("center", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = trQuestVarGet("vortexRange") * trQuestVarGet("p"+p+"spellRange");
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Dwarf",8,trQuestVarGet("centerx"),0,trQuestVarGet("centerz"),0,true);
	}

	if (yGetDatabaseCount("p"+p+"vortexObjects") > 0) {
		yDatabaseNext("p"+p+"vortexObjects");
		amt = yGetVar("p"+p+"vortexObjects", "timeout") - trTimeMS();
		if (amt > 0) {
			amt = amt * 0.001;
		} else {
			trUnitDestroy();
			yRemoveFromDatabase("p"+p+"vortexObjects");
		}
	}


	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		
	}

	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseSparkWitch(int eventID = -1) {
	int p = eventID - 1000 - 12 * SPARKWITCH;
	if (trCurrentPlayer() == p) {
		map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(W) Hex";
		wellIsUltimate = false;
		map("q", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(Q) Electrify";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Vortex";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("hexCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("vortexRadius"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("electrifyCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}

rule sparkWitch_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SPARKWITCH + p, "sparkWitchAlways");
		trEventSetHandler(1000 + 12 * SPARKWITCH + p, "chooseSparkWitch");
	}

	trQuestVarSet("electrifyCooldown", 18);
	trQuestVarSet("electrifyRadius", 4);
	trQuestVarSet("electrifyDuration", 5);
	trQuestVarSet("electrifyDamage", 30);

	trQuestVarSet("hexCooldown", 12);
	trQuestVarSet("hexDuration", 6);

	trQuestVarSet("vortexCost", 80);
	trQuestVarSet("vortexRadius", 18);
}