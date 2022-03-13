void removeSpellstealer(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "cloakSFX");
}

void spellstealerAlways(int eventID = -1) {
	int p = eventID - 12 * SPELLSTEALER;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	int stunned = 0;
	int poisoned = 0;
	int silenced = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeSpellstealer(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit >= ON_HIT_NORMAL) {
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
					amt = 1;
					if (yGetVar("enemies", "poisonStatus") > 0) {
						amt = amt * 2;
					}
					if (yGetVar("enemies", "stunStatus") > 0) {
						amt = amt * 2;
					}
					if (yGetVar("enemies", "silencestatus") > 0) {
						amt = amt * 2;
					}
					amt = amt - 1;
					trUnitSelectClear();
					trUnitSelectByQV("enemies", true);
					damageEnemy(p, amt * trQuestVarGet("p"+p+"baseAttack"), false);
					if (hit == ON_HIT_SPECIAL) {
						trVectorSetUnitPos("pos", "enemies");
						dist = xsPow(trQuestVarGet("spellstealerPassiveRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("Lampades Blood");
						for(x=yGetDatabaseCount("enemies"); >0) {
							id = yDatabaseNext("enemies", true);
							if (id == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
								poisonUnit("enemies", 12.0, 12.0, p);
							}
						}
					}
				}
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		dist = xsPow(trQuestVarGet("bladeDanceRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
		amt = dist;
		target = -1;
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id == -1 || trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = zDistanceToVectorSquared("enemies", "pos");
				if (current < dist) {
					yAddToDatabase("p"+p+"bladeDanceTargets", "enemies");
					yAddUpdateVar("p"+p+"bladeDanceTargets", "index", yGetPointer("enemies"));
					if (current < amt) {
						amt = current;
						target = yGetNewestPointer("p"+p+"bladeDanceTargets");
					}
				}
			}
		}
		if (target > 0) {
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				id = yDatabaseNext("p"+p+"characters", true);
				if (id == -1 || trUnitAlive() == false) {
					removeSpellstealer(p);
				} else {
					trMutateSelected(kbGetProtoUnitID("Revealer to Player"));
				}
			}
			trQuestVarSet("p"+p+"launched", 1);
			
			amt = trQuestVarGet("bladeDanceCost") * trQuestVarGet("p"+p+"ultimateCost");
			
			ySetPointer("p"+p+"bladeDanceTargets", target);
			ySetVar("p"+p+"bladeDanceTargets", "status", trQuestVarGet("p"+p+"spellstealStatus"));
			if (ySetPointer("enemies", 1*yGetVar("p"+p+"bladeDanceTargets", "index"))) {
				stunned = yGetVar("enemies", "stunStatus");
				poisoned = yGetVar("enemies", "poisonStatus");
				silenced = yGetVar("enemies", "silenceStatus");
			}
			
			for(x=yGetDatabaseCount("p"+p+"bladeDanceTargets"); >0) {
				yDatabaseNext("p"+p+"bladeDanceTargets");
				if (target != yGetPointer("p"+p+"bladeDanceTargets")) {
					id = yGetVar("p"+p+"bladeDanceTargets", "index");
					hit = yGetVarAtIndex("enemies", "stunStatus", id) * stunned;
					hit = hit + yGetVarAtIndex("enemies", "poisonStatus", id) * poisoned;
					if ((hit == 0) || (trPlayerResourceCount(p, "favor") < 2.0 * amt)) {
						yRemoveFromDatabase("p"+p+"bladeDanceTargets");
					} else {
						ySetVar("p"+p+"bladeDanceTargets", "status", trQuestVarGet("p"+p+"spellstealStatus"));
					}
				}
			}
			trQuestVarSet("p"+p+"spellstealStatus", 0);
			trQuestVarSet("p"+p+"bladeDanceNext", trTimeMS() - 1);
			ySetPointer("p"+p+"bladeDanceTargets", target);
			
			if (yGetDatabaseCount("p"+p+"bladeDanceTargets") > 1) {
				amt = amt * 2;
			}
			gainFavor(p, 0.0 - amt);
		} else {
			yClearDatabase("p"+p+"bladeDanceTargets");
			trQuestVarSet("p"+p+"lureReadyTime", 0);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				trChatSend(0, "There are no enemies near your cursor!");
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"bladeDanceTargets") > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"bladeDanceNext")) {
			trQuestVarSet("p"+p+"bladeDanceNext", trQuestVarGet("p"+p+"bladeDanceNext") + 300);
			for(x=yGetDatabaseCount("p"+p+"bladeDanceTargets"); >0) {
				yDatabaseNext("p"+p+"bladeDanceTargets", true);
				if (trUnitAlive() == false) {
					yRemoveFromDatabase("p"+p+"bladeDanceTargets");
				} else if (ySetPointer("enemies", 1*yGetVar("p"+p+"bladeDanceTargets", "index"))) {
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					hit = yGetVar("p"+p+"bladeDanceTargets", "status");
					amt = 1;
					if (hit >= xsPow(2, STATUS_SILENCE)) {
						hit = hit - xsPow(2, STATUS_SILENCE);
						silenceUnit("enemies", 9.0, p);
					}
					if (hit >= xsPow(2, STATUS_POISON)) {
						hit = hit - xsPow(2, STATUS_POISON);
						poisonUnit("enemies", 12.0, 12.0, p);
					}
					if (hit >= xsPow(2, STATUS_STUN)) {
						hit = hit - xsPow(2, STATUS_STUN);
						stunUnit("enemies", 2.0, p);
					}
					if (yGetVar("enemies", "stunstatus") > 0) {
						amt = amt * 2;
					}
					if (yGetVar("enemies", "poisonstatus") > 0) {
						amt = amt * 2;
					}
					if (yGetVar("enemies", "silencestatus") > 0) {
						amt = amt * 2;
					}
					amt = amt * trQuestVarGet("p"+p+"baseAttack") * trQuestVarGet("p"+p+"spellDamage");
					trVectorSetUnitPos("pos", "enemies");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0", "Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"bladeDanceTargets");
					damageEnemy(p, amt, false, 1.0);
					OnHit(p, 1*yGetVar("p"+p+"bladeDanceTargets", "index"), false);
					yRemoveFromDatabase("p"+p+"bladeDanceTargets");
					break;
				}
			}
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			if (yGetDatabaseCount("p"+p+"bladeDanceTargets") == 0) {
				trUnitChangeProtoUnit("Transport Ship Greek");
				for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
					yDatabaseNext("p"+p+"characters", true);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Swordsman Hero");
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"characters");
					trMutateSelected(kbGetProtoUnitID("Swordsman Hero"));
				}
				trQuestVarSet("p"+p+"launched", 0);
				equipRelicsAgain(p);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Kronny Birth SFX");
			} else {
				trUnitChangeProtoUnit("Kronny Birth SFX");
			}
		}
	}
	
	for(y=xsMin(2, yGetDatabaseCount("p"+p+"spellblades")); >0) {
		hit = 0;
		yDatabaseNext("p"+p+"spellblades");
		yVarToVector("p"+p+"spellblades", "prev");
		yVarToVector("p"+p+"spellblades", "dir");
		dist = zDistanceToVector("p"+p+"spellblades", "prev");
		for(x=yGetDatabaseCount("enemies"); >0) {
			if ((yDatabaseNext("enemies", true) == -1) || trUnitAlive() == false) {
				removeEnemy();
			} else {
				trVectorSetUnitPos("pos", "enemies");
				current = zDistanceBetweenVectors("pos", "prev");
				if (current < dist + 3.0) {
					trQuestVarSet("hitboxX", trQuestVarGet("prevX") + current * trQuestVarGet("dirX"));
					trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + current * trQuestVarGet("dirZ"));
					if (zDistanceBetweenVectorsSquared("pos", "hitbox") < 9.0) {
						trQuestVarSet("spellsound", 2);
						amt = 0.5;
						hit = yGetVar("p"+p+"spellblades", "status");
						if (hit >= xsPow(2, STATUS_SILENCE)) {
							hit = hit - xsPow(2, STATUS_SILENCE);
							silenceUnit("enemies", 9.0, p);
						}
						if (hit >= xsPow(2, STATUS_POISON)) {
							hit = hit - xsPow(2, STATUS_POISON);
							poisonUnit("enemies", 12.0, 12.0, p);
						}
						if (hit >= xsPow(2, STATUS_STUN)) {
							hit = hit - xsPow(2, STATUS_STUN);
							stunUnit("enemies", 2.0, p);
						}
						if (yGetVar("enemies", "stunstatus") > 0) {
							amt = amt * 2;
						}
						if (yGetVar("enemies", "poisonstatus") > 0) {
							amt = amt * 2;
						}
						if (yGetVar("enemies", "silencestatus") > 0) {
							amt = amt * 2;
						}
						amt = amt * trQuestVarGet("p"+p+"baseAttack") * trQuestVarGet("p"+p+"spellDamage");
						damageEnemy(p, amt, true);
						hit = 1;
					}
				}
			}
		}
		if (hit == 0) {
			trVectorSetUnitPos("pos", "p"+p+"spellblades");
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hit = 1;
				trQuestVarSet("spellsound", 1);
			} else {
				ySetVarFromVector("p"+p+"spellblades", "prev", "pos");
			}
		}
		
		if (hit == 1) {
			gainFavor(p, 1);
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"spellblades");
			trUnitChangeProtoUnit("Lightning Sparks Ground");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"spellblades");
			trDamageUnitPercent(-100);
			yRemoveFromDatabase("p"+p+"spellblades");
		}
	}
	
	if (trQuestVarGet("spellsound") == 1) {
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	} else if (trQuestVarGet("spellsound") == 2) {
		trQuestVarSetFromRand("sound", 1,4,true);
		trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	trQuestVarSet("spellsound", 0);
	
	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		trSoundPlayFN("manticorespecialattack.wav","1",-1,"","");
		for(y=yGetDatabaseCount("p"+p+"characters"); >0) {
			id = yDatabaseNext("p"+p+"characters", true);
			if (id == -1 || trUnitAlive() == false) {
				removeSpellstealer(p);
			} else {
				trVectorSetUnitPos("start", "p"+p+"characters");
				hit = trQuestVarGet("spellbladesCount") + 2 * trQuestVarGet("p"+p+"projectiles");
				amt = angleBetweenVectors("start", "p"+p+"wellPos");
				amt = fModulo(6.283185, amt - 0.196349 * (hit - 1) * 0.5);
				for(x=hit; >0) {
					trVectorSetFromAngle("dir", amt);
					trQuestVarSet("endx", trQuestVarGet("startx") + trQuestVarGet("dirx"));
					trQuestVarSet("endz", trQuestVarGet("startz") + trQuestVarGet("dirz"));
					vectorSetAsTargetVector("target","start","end",300.0);
					amt = fModulo(6.283185, amt + 0.196349);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trMutateSelected(kbGetProtoUnitID("Stymph Bird Feather"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"));
					yAddToDatabase("p"+p+"spellblades","next");
					yAddUpdateVar("p"+p+"spellblades","prevX", trQuestVarGet("startX"));
					yAddUpdateVar("p"+p+"spellblades","prevZ", trQuestVarGet("startZ"));
					yAddUpdateVar("p"+p+"spellblades","dirX", trQuestVarGet("dirX"));
					yAddUpdateVar("p"+p+"spellblades","dirZ", trQuestVarGet("dirZ"));
					yAddUpdateVar("p"+p+"spellblades","status", trQuestVarGet("p"+p+"spellstealStatus"));
				}
			}
		}
		trQuestVarSet("p"+p+"spellstealStatus", 0);
	}
	
	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"negationCloak", 1);
		trQuestVarSet("p"+p+"negationCloakTimeout",
			trTimeMS() + 1000 * trQuestVarGet("negationCloakDuration") * trQuestVarGet("p"+p+"spellDuration"));
		trSoundPlayFN("spybirth.wav","1",-1,"","");
	}
	
	if (trQuestVarGet("p"+p+"negationCloak") == 1) {
		if (trTime() > trQuestVarGet("p"+p+"negationCloakNext")) {
			trQuestVarSet("p"+p+"negationCloakNext", trTime());
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				id = yDatabaseNext("p"+p+"characters", true);
				if (id == -1 || trUnitAlive() == false) {
					removeSpellstealer(p);
				} else {
					if (kbGetBlockID(""+1*yGetVar("p"+p+"characters", "cloakSFX")) == -1) {
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
							kbGetProtoUnitID("Kronny Birth SFX"), yGetVarName("p"+p+"characters", "cloakSFX"));
					} else {
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"characters", "cloakSFX"), true);
						trUnitChangeProtoUnit("Kronny Birth SFX");
					}
				}
			}
		}
		if (trTimeMS() > trQuestVarGet("p"+p+"negationCloakTimeout")) {
			trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			trQuestVarSet("p"+p+"negationCloak", 0);
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if ((yDatabaseNext("p"+p+"characters", true) == -1) || (trUnitAlive() == false)) {
					removeSpellstealer(p);
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar("p"+p+"characters", "cloakSFX"), true);
					trUnitChangeProtoUnit("Cinematic Block");
				}
			}
		}
	}
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseSpellstealer(int eventID = -1) {
	int p = eventID - 1000 - 12 * SPELLSTEALER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Spellblades";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Negation Cloak";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Blade Dance";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("spellbladesCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("bladeDanceCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("negationCloakCooldown"));
	trQuestVarSet("p"+p+"rainCost", 0);
}


rule spellstealer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SPELLSTEALER + p, "spellstealerAlways");
		trEventSetHandler(1000 + 12 * SPELLSTEALER + p, "chooseSpellstealer");
	}
	trQuestVarSet("spellstealerPassiveRadius", 6);
	
	trQuestVarSet("bladeDanceCost", 10);
	trQuestVarSet("bladeDanceRadius", 15);
	
	trQuestVarSet("negationCloakDuration", 3);
	trQuestVarSet("negationCloakCooldown", 12);
	
	trQuestVarSet("spellbladesCooldown", 9);
	trQuestVarSet("spellbladesCount", 3);
	
}
