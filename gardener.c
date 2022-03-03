void removeGardener(int p = 0) {
	removePlayerSpecific(p);
}

void plantSeed(string next = "", int p = 0) {
	trUnitSelectClear();
	trUnitSelect(""+1*trQuestVarGet(next), true);
	trUnitChangeProtoUnit("Rock Granite Small");
	yAddToDatabase("p"+p+"seeds", next);
	yAddUpdateVar("p"+p+"seeds", "ready", trTimeMS() + 2000);
	yAddUpdateVar("p"+p+"seeds", "timeout",
		trTimeMS() + 1000 * trQuestVarGet("seedDuration") * trQuestVarGet("p"+p+"spellDuration"));
		yAddUpdateVar("p"+p+"seeds", "type", trQuestVarGet("p"+p+"natureBounty"));
		trUnitSelectClear();
		trUnitSelect(""+1*trQuestVarGet(next), true);
		if (trQuestVarGet("p"+p+"natureBounty") == 0) {
			trUnitSetAnimationPath("1,0,0,0,0");
		} else {
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelect(""+1*trQuestVarGet("next"), true);
			trMutateSelected(kbGetProtoUnitID("Berry Bush"));
		}
		if (trCurrentPlayer() == p) {
			trUnitHighlight(trQuestVarGet("seedDuration") * trQuestVarGet("p"+p+"spellDuration"), true);
		}
	}
	
	void gardenerAlways(int eventID = -1) {
		int p = eventID - 12 * GARDENER;
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
				removeGardener(p);
			} else {
				hit = CheckOnHit(p, id);
			}
		}
		
		current = trQuestVarGet("seedDuration") * trQuestVarGet("p"+p+"spellDuration");
		amt = trQuestVarGet("seedHeal");
		target = trQuestVarGet("p"+p+"seedHealPlayer") + 1;
		if (target >= ENEMY_PLAYER) {
			target = 1;
		}
		if (trQuestVarGet("p"+target+"dead") == 0) {
			for (y=yGetDatabaseCount("p"+p+"seeds"); >0) {
				yDatabaseNext("p"+p+"seeds");
				hit = 0;
				dist = yGetVar("p"+p+"seeds", "timeout") - trTimeMS();
				if (dist > 0) {
					dist = 0.001 * dist / current;
					trVectorSetUnitPos("pos", "p"+p+"seeds");
					if (zDistanceToVectorSquared("p"+target+"unit", "pos") < 1) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+target+"unit");
						if (trUnitPercentDamaged() > 0) {
							if (yGetVar("p"+p+"seeds", "type") == 1) {
								healUnit(p, 4.0 * amt, 1*trQuestVarGet("p"+target+"index"));
							} else {
								healUnit(p, amt, 1*trQuestVarGet("p"+target+"index"));
							}
							hit = 1;
						}
					}
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"seeds", true);
					if (hit == 1) {
						trQuestVarSet("eatSound", 1);
						trUnitChangeProtoUnit("Regeneration SFX");
					} else {
						dist = 0.5 + 0.5 * dist;
						trSetSelectedScale(dist, dist, dist);
					}
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"seeds", true);
					trUnitChangeProtoUnit("Dust Small");
					yRemoveFromDatabase("p"+p+"seeds");
				}
			}
		}
		
		trQuestVarSet("p"+p+"seedHealPlayer", target);
		
		if (trQuestVarGet("eatSound") == 1) {
			trQuestVarSet("eatSound", 0);
			trSoundPlayFN("colossuseat.wav","1",-1,"","");
		}
		
		if (yGetDatabaseCount("p"+p+"lifeArrows") > 0) {
			id = yDatabaseNext("p"+p+"lifeArrows", true);
			if (id == -1 || trUnitAlive() == false) {
				yVarToVector("p"+p+"lifeArrows", "pos");
				yRemoveFromDatabase("p"+p+"lifeArrows");
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				plantSeed("next", p);
			} else {
				trVectorSetUnitPos("pos", "p"+p+"lifeArrows");
				if (trQuestVarGet("posy") < worldHeight + 0.3) {
					trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
					trUnitSelectClear();
					trUnitSelect(""+1*trQuestVarGet("next"), true);
					trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"lifeArrows"));
					trUnitChangeProtoUnit("Rock Granite Small");
					
					trUnitSelectClear();
					trUnitSelect(""+1*trQuestVarGet("p"+p+"lifeArrows"), true);
					trMutateSelected(kbGetProtoUnitID("Arrow Flaming"));
					yRemoveFromDatabase("p"+p+"lifeArrows");
					
					plantSeed("next", p);
				}
			}
		}
		
		for(y=trQuestVarGet("p"+p+"projectiles"); >0) {
			if (yFindLatest("p"+p+"latestProj", "Arrow Flaming", p) > 0) {
				trVectorSetUnitPos("start", "p"+p+"latestProj");
				target = 0;
				dist = 25;
				for (x=yGetDatabaseCount("p"+p+"characters"); >0) {
					id = yDatabaseNext("p"+p+"characters", true);
					if (id == -1 || trUnitAlive() == false) {
						removeGardener(p);
					} else if (kbUnitGetAnimationActionType(id) == 12) {
						current = zDistanceToVectorSquared("p"+p+"characters", "start");
						if (current < dist) {
							dist = current;
							target = yGetPointer("p"+p+"characters");
						}
					}
				}
				if (target > 0) {
					ySetPointer("p"+p+"characters", target);
					target = trGetUnitScenarioNameNumber(1*yGetVar("p"+p+"characters", "attackTarget"));
					trVectorQuestVarSet("pos", kbGetBlockPosition(""+target));
					yAddToDatabase("p"+p+"lifeArrows", "p"+p+"latestProj");
					yAddUpdateVar("p"+p+"lifeArrows", "posx", trQuestVarGet("posx"));
					yAddUpdateVar("p"+p+"lifeArrows", "posz", trQuestVarGet("posz"));
				} else {
					break;
				}
			} else {
				break;
			}
		}
		
		if (yGetDatabaseCount("p"+p+"bloodblooms") > 0) {
			id = yDatabaseNext("p"+p+"bloodblooms", true);
			if (id == -1 || trUnitAlive() == false) {
				trUnitSelectClear();
				trUnitSelect(""+(1+trQuestVarGet("p"+p+"bloodblooms")), true);
				trDamageUnitPercent(100);
				yRemoveFromDatabase("p"+p+"bloodblooms");
			} else if (kbUnitGetAnimationActionType(id) == 6) {
				if (yGetVar("p"+p+"bloodblooms", "attacking") == 0) {
					ySetVar("p"+p+"bloodblooms", "attacking", 1);
					ySetVar("p"+p+"bloodblooms", "attackNext", trTimeMS() + 600);
				} else if (trTimeMS() > yGetVar("p"+p+"bloodblooms", "attackNext")) {
					ySetVar("p"+p+"bloodblooms", "attackNext", yGetVar("p"+p+"bloodblooms", "attackNext") + 1300);
					target = kbUnitGetTargetUnitID(id);
					for(x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == target) {
							gainFavor(p, 1);
							poisonUnit("enemies", 12.0, 12.0, p);
							trVectorSetUnitPos("pos", "enemies");
							trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
							trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
							plantSeed("next", p);
							trSoundPlayFN("farming1.wav","1",-1,"","");
							break;
						}
					}
				}
			} else if (yGetVar("p"+p+"bloodblooms", "attacking") == 1) {
				ySetVar("p"+p+"bloodblooms", "attacking", 0);
			}
		}
		
		hit = yGetDatabaseCount("p"+p+"stranglethorns");
		if (hit > 0) {
			current = 1000.0 / hit;
			if (trTimeMS() > trQuestVarGet("p"+p+"stranglethornsNext") + current) {
				trQuestVarSet("p"+p+"stranglethornsNext", trQuestVarGet("p"+p+"stranglethornsNext") + current);
				yDatabaseNext("p"+p+"stranglethorns", true);
				if (trTimeMS() > yGetVar("p"+p+"stranglethorns", "timeout")) {
					trDamageUnitPercent(100);
					yRemoveFromDatabase("p"+p+"stranglethorns");
				} else {
					yVarToVector("p"+p+"stranglethorns", "pos");
					dist = xsPow(trQuestVarGet("stranglethornsRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
					amt = trQuestVarGet("stranglethornsDamage") * trQuestVarGet("p"+p+"spellDamage");
					for (x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
							stunUnit("enemies", 1.0, p, false);
							damageEnemy(p, amt);
						}
					}
				}
			}
		} else {
			trQuestVarSet("p"+p+"stranglethornsNext", trTimeMS());
		}
		
		
		if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
			trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
			
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("p"+p+"wellPosx"),0,trQuestVarGet("p"+p+"wellPosz"),0,true);
			trArmySelect(""+p+",0");
			trUnitChangeProtoUnit("Heavenlight");
			dist = xsPow(trQuestVarGet("stranglethornsRange") * trQuestVarGet("p"+p+"spellRange"), 2);
			for(x=yGetDatabaseCount("p"+p+"seeds"); >0) {
				yDatabaseNext("p"+p+"seeds");
				if (zDistanceToVectorSquared("p"+p+"seeds", "p"+p+"wellPos") < dist) {
					gainFavor(p, 1);
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"seeds", true);
					trUnitHighlight(0.01,false);
					if (yGetVar("p"+p+"seeds", "type") == 1) {
						trUnitChangeProtoUnit("Walking Berry Bush");
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"seeds", true);
						trUnitConvert(p);
						trUnitChangeProtoUnit("Walking Berry Bush");
						activatePlayerUnit("p"+p+"seeds", p, kbGetProtoUnitID("Walking Berry Bush"), calculateDecay(p, 5.0));
						yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 5000);
					} else {
						trUnitConvert(0);
						trUnitChangeProtoUnit("Uproot 2x2");
						trQuestVarSetFromRand("angle", 0, 3.14, false);
						trVectorSetFromAngle("dir", trQuestVarGet("angle"));
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"seeds", true);
						trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
						trSetSelectedScale(trQuestVarGet("p"+p+"spellRange"),1,trQuestVarGet("p"+p+"spellRange"));
						trVectorSetUnitPos("pos", "p"+p+"seeds");
						yAddToDatabase("p"+p+"stranglethorns", "p"+p+"seeds");
						yAddUpdateVar("p"+p+"stranglethorns", "posx", trQuestVarGet("posx"));
						yAddUpdateVar("p"+p+"stranglethorns", "posZ", trQuestVarGet("posZ"));
						yAddUpdateVar("p"+p+"stranglethorns", "timeout", trTimeMS() + 1000 * trQuestVarGet("stranglethornsDuration"));
					}
					yRemoveFromDatabase("p"+p+"seeds");
				}
			}
		}
		
		if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
			trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
			trVectorSetUnitPos("pos", "p"+p+"lureObject");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"lureObject", true);
			trUnitDestroy();
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",2,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
			trUnitSelectClear();
			trUnitSelect(""+(1+trQuestVarGet("next")), true);
			trUnitChangeProtoUnit("Audrey Base");
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trUnitChangeProtoUnit("Audrey");
			yAddToDatabase("p"+p+"bloodblooms", "next");
			activatePlayerUnit("next", p, kbGetProtoUnitID("Audrey"), calculateDecay(p, 8.0));
			yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 2000);
		}
		
		if (trQuestVarGet("p"+p+"natureBounty") == 1) {
			if (trTimeMS() > trQuestVarGet("p"+p+"natureBountyTimeout")) {
				trQuestVarSet("p"+p+"natureBounty", 0);
			}
		}
		
		if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
			trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
			trSoundPlayFN("walkingwoods1.wav", "1", -1, "", "");
			trSoundPlayFN("gaiaforest.wav", "1", -1, "", "");
			gainFavor(p, 0.0 - trQuestVarGet("natureBountyCost") * trQuestVarGet("p"+p+"ultimateCost"));
			trQuestVarSet("p"+p+"natureBounty", 1);
			trQuestVarSet("p"+p+"natureBountyTimeout",
				trTimeMS() + 1000 * trQuestVarGet("natureBountyDuration") * trQuestVarGet("p"+p+"spellDuration"));
			}
			
			
			poisonKillerBonus(p);
			xsSetContextPlayer(old);
		}
		
		void chooseGardener(int eventID = -1) {
			int p = eventID - 1000 - 12 * GARDENER;
			trQuestVarSet("p"+p+"latestProj", trGetNextUnitScenarioNameNumber() - 1);
			if (trCurrentPlayer() == p) {
				map("w", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
				wellName = "(W) Stranglethorns";
				wellIsUltimate = false;
				map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
				rainName = "(E) Nature's Bounty";
				rainIsUltimate = true;
				map("q", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
				lureName = "(Q) Bloodbloom";
				lureIsUltimate = false;
			}
			trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("stranglethornsCooldown"));
			trQuestVarSet("p"+p+"wellCost", 0);
			trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("bloodbloomCooldown"));
			trQuestVarSet("p"+p+"lureCost", 0);
			trQuestVarSet("p"+p+"rainCooldown", 1);
			trQuestVarSet("p"+p+"rainCost", trQuestVarGet("natureBountyCost"));
		}
		
		void modifyGardener(int eventID = -1) {
			int p = eventID - 5000 - 12 * GARDENER;
			zSetProtoUnitStat("Walking Berry Bush", p, 0, 0.5 * trQuestVarGet("p"+p+"health"));
			zSetProtoUnitStat("Walking Berry Bush", p, 27, 0.5 * trQuestVarGet("p"+p+"attack"));
			zSetProtoUnitStat("Audrey", p, 0, trQuestVarGet("p"+p+"health"));
			zSetProtoUnitStat("Audrey", p, 27, trQuestVarGet("p"+p+"attack"));
		}
		
		
		rule gardener_init
		active
		highFrequency
		{
			xsDisableSelf();
			for(p=1; < ENEMY_PLAYER) {
				trEventSetHandler(12 * GARDENER + p, "gardenerAlways");
				trEventSetHandler(1000 + 12 * GARDENER + p, "chooseGardener");
				trEventSetHandler(5000 + 12 * GARDENER + p, "modifyGardener");
			}
			
			trQuestVarSet("seedHeal", 30);
			trQuestVarSet("seedDuration", 15);
			
			trQuestVarSet("bloodbloomCooldown", 15);
			
			trQuestVarSet("stranglethornsCooldown", 12);
			trQuestVarSet("stranglethornsRange", 6);
			trQuestVarSet("stranglethornsRadius", 3);
			trQuestVarSet("stranglethornsDuration", 3);
			trQuestVarSet("stranglethornsDamage", 20);
			
			trQuestVarSet("natureBountyCost", 60);
			trQuestVarSet("natureBountyDuration", 12);
		}
