void removeFrostKnight(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "blizzardSFX");
}

void castIcicle(int p = 0, string pos = "") {
	float dist = 0;
	float current = 0;
	float amt = 0;
	int hit = 0;
	int target = 0;
	trQuestVarSetFromRand("heading",1,360,true);
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trQuestVarSet("marker", trQuestVarGet("next") + 1);
	trQuestVarSet("blizzardSFX", trQuestVarGet("next") + 2);
	trArmyDispatch(""+p+",0","Dwarf",3,
		trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),trQuestVarGet("heading"),true);
		
		
		yAddToDatabase("p"+p+"icicles", "next");
		yAddUpdateVar("p"+p+"icicles", "blizzardBlock", trQuestVarGet("marker"));
		yAddUpdateVar("p"+p+"icicles", "blizzardSFX", trQuestVarGet("blizzardSFX"));
		yAddUpdateVar("p"+p+"icicles", "step", 0);
		yAddUpdateVar("p"+p+"icicles", "timeout", trTimeMS() + 150);
		yAddUpdateVar("p"+p+"icicles", "radius", trQuestVarGet("icicleRadius") * xGetFloat(dPlayerData, xPlayerSpellRange));
		
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
			trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));
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
		trSetSelectedScale(trQuestVarGet("icicleRadius") * xGetFloat(dPlayerData, xPlayerSpellRange),
		0,trQuestVarGet("icicleRadius") * xGetFloat(dPlayerData, xPlayerSpellRange));
		trUnitHighlight(trQuestVarGet("icicleDuration") * xGetFloat(dPlayerData, xPlayerSpellDuration), false);
		
		dist = xsPow(xGetFloat(dPlayerData, xPlayerSpellRange) * trQuestVarGet("icicleRadius"), 2);
		amt = dist;
		hit = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = zDistanceToVectorSquared("enemies", pos);
				if (current < dist) {
					hit = hit + 1;
					damageEnemy(p, trQuestVarGet("icicleDamage") * xGetFloat(dPlayerData, xPlayerSpellDamage), true);
					if (current < amt) {
						amt = current;
						target = xGetPointer(dEnemies);
					}
				}
			}
		}
		if (hit >= 1) {
			gainFavor(p, hit);
			ySetPointer("enemies", target);
			stunUnit("enemies", 1.5, p);
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	
	void frostknightAlways(int eventID = -1) {
		int p = eventID - 12 * FROSTKNIGHT;
		int id = 0;
		int hit = 0;
		int target = 0;
		int index = xGetPointer(dEnemies);
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
						trVectorSetUnitPos("pos", "enemies");
						castIcicle(p, "pos");
					}
				}
			}
		}
		
		for (x=xsMin(3, yGetDatabaseCount("p"+p+"icicles")); > 0) {
			yDatabaseNext("p"+p+"icicles", true);
			if (yGetVar("p"+p+"icicles", "step") == 2) {
				amt = (yGetVar("p"+p+"icicles", "timeout") - trTimeMS()) / trQuestVarGet("icicleDuration") / 1000;
				amt = 1.0 + 3.0 * amt;
				if (amt <= 1.0) {
					trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardSFX"), true);
					trUnitSelect(""+1*yGetVar("p"+p+"icicles", "blizzardBlock"), true);
					trUnitDestroy();
					yRemoveFromDatabase("p"+p+"icicles");
				} else {
					trSetSelectedScale(yGetVar("p"+p+"icicles", "radius"),amt,yGetVar("p"+p+"icicles", "radius"));
				}
			} else {
				amt = (150.0 - yGetVar("p"+p+"icicles", "timeout") + trTimeMS()) / 150.0;
				if (amt >= 1.0) {
					ySetVar("p"+p+"icicles", "step", 2);
					ySetVar("p"+p+"icicles", "timeout",
						yGetVar("p"+p+"icicles", "timeout") + trQuestVarGet("icicleDuration") * xGetFloat(dPlayerData, xPlayerSpellDuration) * 1000.0);
						amt = 1.0;
					}
					amt = 1.0 + 3.0 * amt;
					trSetSelectedScale(yGetVar("p"+p+"icicles", "radius"),amt,yGetVar("p"+p+"icicles", "radius"));
				}
			}
			
			if (trQuestVarGet("p"+p+"blizzard") == 1) {
				if (trTimeMS() > trQuestVarGet("p"+p+"blizzardNext")) {
					trQuestVarSet("p"+p+"blizzardNext", trQuestVarGet("p"+p+"blizzardNext") + 1000);
					for (x=xGetDatabaseCount(dEnemies); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else {
							hit = trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Victory Marker",
							xGetFloat(dPlayerData, xPlayerSpellRange) * trQuestVarGet("blizzardRadius"));
							hit = hit + trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Frost Giant",
							xGetFloat(dPlayerData, xPlayerSpellRange) * trQuestVarGet("blizzardRadius"));
							hit = hit + trCountUnitsInArea(""+1*trQuestVarGet("enemies"),p,"Hero Greek Achilles",
							xGetFloat(dPlayerData, xPlayerSpellRange) * trQuestVarGet("blizzardRadius"));
							damageEnemy(p, trQuestVarGet("blizzardDamage") * xGetFloat(dPlayerData, xPlayerSpellDamage) * hit, true);
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
			
			if (xGetBool(dPlayerData, xPlayerWellActivated)) {
				xSetBool(dPlayerData, xPlayerWellActivated, false);
				castIcicle(p, "p"+p+"wellPos");
			}
			
			if (xGetBool(dPlayerData, xPlayerRainActivated)) {
				xSetBool(dPlayerData, xPlayerRainActivated, false);
				trSoundPlayFN("recreation.wav","1",-1,"","");
				trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
				trQuestVarSet("p"+p+"blizzard", 1);
				trQuestVarSet("p"+p+"blizzardNext", trTimeMS());
				trQuestVarSet("p"+p+"blizzardTimeout",
					trTimeMS() + 1000 * trQuestVarGet("blizzardDuration") * xGetFloat(dPlayerData, xPlayerSpellDuration));
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
						trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));
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
							trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));
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
								trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));
							}
						}
					}
				}
				
				if (xGetBool(dPlayerData, xPlayerLureActivated)) {
					xSetBool(dPlayerData, xPlayerLureActivated, false);
					gainFavor(p, 0 - trQuestVarGet("frostGiantCost") * xGetFloat(dPlayerData, xPlayerUltimateCost));
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"lureObject", true);
					trUnitChangeProtoUnit("Frost Giant");
					gainFavor(p, 0 - trQuestVarGet("frostGiantCost") * xGetFloat(dPlayerData, xPlayerUltimateCost));
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
								for (x=xGetDatabaseCount(dEnemies); >0) {
									if (yGetVar("p"+p+"frostGiants", "target") == xDatabaseNext(dEnemies)) {
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
				xSetPointer(dEnemies, index);
				poisonKillerBonus(p);
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
				zSetProtoUnitStat("Frost Giant", p, 0, xGetFloat(dPlayerData, xPlayerHealth));
				zSetProtoUnitStat("Frost Giant", p, 27, xGetFloat(dPlayerData, xPlayerBaseAttack));
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
				trQuestVarSet("blizzardDuration", 8);
				trQuestVarSet("blizzardDamage", 30);
				trQuestVarSet("blizzardRadius", 4);
				
				trQuestVarSet("frostGiantDecay", 3);
				trQuestVarSet("frostGiantCost", 60);
			}
