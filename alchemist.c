const int POTION_STUN = 0;
const int POTION_POISON = 1;
const int POTION_HEAL = 2;

void removeAlchemist(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "potion");
}

void alchemistAlways(int eventID = -1) {
	int p = eventID - 12 * ALCHEMIST;
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
			removeAlchemist(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_NORMAL) {
				target = yGetVar("p"+p+"characters", "attackTarget");
				ySetVar("p"+p+"characters", "potion", 1+yGetVar("p"+p+"characters", "potion"));
				if (yGetVar("p"+p+"characters", "potion") == 3) {
					trPlayerGrantResources(p, "favor", trQuestVarGet("p"+p+"projectiles"));
					ySetVar("p"+p+"characters", "potion", 0);
					if (trQuestVarGet("p"+p+"potion") == POTION_HEAL) {
						amt = trQuestVarGet("potionHeal") * trQuestVarGet("p"+p+"spellDamage") * trQuestVarGet("p"+p+"projectiles");
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							id = yDatabaseNext("playerUnits", true);
							if (id == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else {
								xsSetContextPlayer(1*yGetVar("playerUnits", "player"));
								healUnit(p, amt * kbUnitGetCurrentHitpoints(id) * 0.01);
							}
						}
						xsSetContextPlayer(p);
					} else {
						yAddToDatabase("p"+p+"potions", yGetVarName("p"+p+"characters", "attackTarget"));
						yAddUpdateVar("p"+p+"potions", "type", trQuestVarGet("p"+p+"potion"));
						yAddUpdateVar("p"+p+"potions", "target", trGetUnitScenarioNameNumber(target));
						trVectorSetUnitPos("start", "p"+p+"characters");
						trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
						yAddUpdateVar("p"+p+"potions", "timeout", trTimeMS() + zDistanceBetweenVectors("start", "end") * 32);
						yAddUpdateVar("p"+p+"potions", "posx", trQuestVarGet("endx"));
						yAddUpdateVar("p"+p+"potions", "posz", trQuestVarGet("endz"));
					}
				}
				
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"potions") >0) {
		yDatabaseNext("p"+p+"potions");
		if (trTimeMS() > yGetVar("p"+p+"potions", "timeout")) {
			yVarToVector("p"+p+"potions", "pos");
			switch(1*yGetVar("p"+p+"potions", "type"))
			{
				case POTION_STUN:
				{
					hit = trQuestVarGet("p"+p+"projectiles") - 1;
					for (x=yGetDatabaseCount("enemies"); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (id == 1*trQuestVarGet("p"+p+"potions")) {
							stunUnit("enemies", 2.0, p);
							break;
						}
					}
					if (hit > 0) {
						for (x=yGetDatabaseCount("enemies") - 1; >0) {
							id = yDatabaseNext("enemies", true);
							if (id == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else if (zDistanceToVectorSquared("enemies", "pos") < 16) {
								stunUnit("enemies", 2.0, p);
								hit = hit - 1;
								if (hit == 0) {
									break;
								}
							}
						}
					}
				}
				case POTION_POISON:
				{
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lampades Blood");
					for(x=yGetDatabaseCount("enemies"); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "pos") < 16) {
							poisonUnit("enemies", 12,12 * trQuestVarGet("p"+p+"projectiles"), p);
						}
					}
				}
			}
			yRemoveFromDatabase("p"+p+"potions");
		}
	}

	for(y=trQuestVarGet("p"+p+"projectiles"); >0) {
		if (yFindLatest("p"+p+"latestProj", "Priest Projectile", p) > 0) {
			for (x=yGetDatabaseCount("p"+p+"characters"); >0) {
				id = yDatabaseNext("p"+p+"characters", true);
				if (id == -1 || trUnitAlive() == false) {
					removeAlchemist(p);
				} else if (kbUnitGetAnimationActionType(id) == 12) {
					if (yGetVar("p"+p+"characters", "potion") == 2) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"latestProj", true);
						switch(1*trQuestVarGet("p"+p+"potion"))
						{
							case POTION_STUN:
							{
								trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
							}
							case POTION_POISON:
							{
								trMutateSelected(kbGetProtoUnitID("Poison SFX"));
							}
							case POTION_HEAL:
							{
								trMutateSelected(kbGetProtoUnitID("Curse SFX"));
							}
						}
					}
					break;
				}
			}
		} else {
			break;
		}
	}


	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		dist = 2500;
		hit = -1;
		for(x=yGetDatabaseCount("playerUnits"); >0) {
			id = yDatabaseNext("playerUnits", true);
			if (id == -1 || trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				amt = zDistanceToVectorSquared("playerUnits", "p"+p+"wellPos");
				if (amt < dist) {
					dist = amt;
					hit = yGetPointer("playerUnits");
				}
			}
		}
		if (hit > 0) {
			ySetPointer("playerUnits", hit);
			ySetVar("playerUnits", "poisonTimeout", 0);
			ySetVar("playerUnits", "stunTimeout", 0);
			if (trQuestVarGet("playerUnits") == trQuestVarGet("p"+1*yGetVar("playerUnits", "player")+"unit")) {
				trQuestVarSet("p"+1*yGetVar("playerUnits", "player")+"silencedTimeout", 0);
			}
			trSoundPlayFN("recreation.wav","1",-1,"","");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetUnitAtIndex("playerUnits", hit));
			healUnit(p, trQuestVarGet("elixirHeal") * trQuestVarGet("p"+p+"spellDamage"), hit);
			trVectorSetUnitPos("pos", "playerUnits");
			trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
			trArmySelect("1,0");
			if (Multiplayer) {
				trUnitChangeProtoUnit("Recreation");
			} else {
				trUnitChangeProtoUnit("Hero Birth");
			}
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = 81;
		hit = -1;
		for(x=yGetDatabaseCount("playerCharacters"); >0) {
			id = yDatabaseNext("playerCharacters", true);
			if (id == -1 || trUnitAlive() == false) {
				removePlayerCharacter();
			} else {
				amt = zDistanceToVectorSquared("playerCharacters", "pos");
				if (amt < dist) {
					dist = amt;
					hit = yGetPointer("playerCharacters");
				}
			}
		}
		if (hit > 0) {
			trPlayerGrantResources(p, "favor", 0 - trQuestVarGet("duplicateCost") * trQuestVarGet("p"+p+"ultimateCost"));
			ySetPointer("playerCharacters", hit);
			trVectorSetUnitPos("pos", "playerCharacters");
			spawnPlayerClone(1*yGetVar("playerCharacters", "player"), "pos");
			yAddToDatabase("p"+p+"duplicates", "next");
			yAddUpdateVar("p"+p+"duplicates", "index", yGetNewestPointer("playerUnits"));
			yAddUpdateVar("p"+p+"duplicates", "decay", 0);
			yAddUpdateVar("p"+p+"duplicates", "decayNext", trTime());
			trSoundPlayFN("changeunit.wav","1",-1,"","");
			trSoundPlayFN("sonofosirisbirth.wav","1",-1,"","");
			if (trCurrentPlayer() == 1*yGetVar("playerCharacters", "player")) {
				trMessageSetText(trStringQuestVarGet("p"+p+"name") + " has summoned a copy of you!");
			}
		} else if (trCurrentPlayer() == p) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
		}
	}


	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		trQuestVarSet("p"+p+"potion", 1 + trQuestVarGet("p"+p+"potion"));
		if (trQuestVarGet("p"+p+"potion") > 2) {
			trQuestVarSet("p"+p+"potion", 0);
		}
		if (trCurrentPlayer() == p) {
			switch(1*trQuestVarGet("p"+p+"potion"))
			{
				case POTION_POISON:
				{
					trSetCounterDisplay("Potion: POISON");
					trSoundPlayFN("lampadesblood.wav","1",-1,"","");
				}
				case POTION_STUN:
				{
					trSetCounterDisplay("Potion: FREEZE");
					trSoundPlayFN("icestereo.wav","1",-1,"","");
				}
				case POTION_HEAL:
				{
					trSetCounterDisplay("Potion: HEAL");
					trSoundPlayFN("heal.wav","1",-1,"","");
				}
			}
		}
	}

	if (yGetDatabaseCount("p"+p+"duplicates") > 0) {
		id = yDatabaseNext("p"+p+"duplicates", true);
		if (id == -1 || trUnitAlive() == false) {
			yRemoveFromDatabase("p"+p+"duplicates");
		} else if (trTime() > yGetVar("p"+p+"duplicates", "decayNext")) {
			ySetVar("p"+p+"duplicates", "decayNext", 1 + yGetVar("p"+p+"duplicates", "decayNext"));
			ySetVar("p"+p+"duplicates", "decay", yGetVar("p"+p+"duplicates", "decay") + calculateDecay(p, 1.0));
			damagePlayerUnit(trQuestVarGet("p"+p+"health") * 0.01 * yGetVar("p"+p+"duplicates", "decay"), 
				1*yGetVar("p"+p+"duplicates", "index"));
		}
	}

	xsSetContextPlayer(old);
}

void chooseAlchemist(int eventID = -1) {
	int p = eventID - 1000 - 12 * ALCHEMIST;
	trQuestVarSet("p"+p+"latestProj", trGetNextUnitScenarioNameNumber() - 1);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Elixir";
		wellIsUltimate = false;
		map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(W) Swap Potion";
		rainIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(E) Duplicate";
		lureIsUltimate = true;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("elixirCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", 1);
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("duplicateCost"));
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}


rule alchemist_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * ALCHEMIST + p, "alchemistAlways");
		trEventSetHandler(1000 + 12 * ALCHEMIST + p, "chooseAlchemist");
	}

	trQuestVarSet("potionHeal", 5);

	trQuestVarSet("elixirCooldown", 12);
	trQuestVarSet("elixirHeal", 200);

	trQuestVarSet("duplicateCost", 50);
}