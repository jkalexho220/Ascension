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
	int index = yGetPointer("enemies");
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
					gainFavor(p, trQuestVarGet("p"+p+"projectiles"));
					ySetVar("p"+p+"characters", "potion", 0);
					if (trQuestVarGet("p"+p+"potion") == POTION_HEAL) {
						amt = trQuestVarGet("potionHeal") * trQuestVarGet("p"+p+"spellDamage") * trQuestVarGet("p"+p+"projectiles");
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							id = yDatabaseNext("playerUnits", true);
							if (id == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else if (yGetVar("playerUnits", "poisonStatus") == 0) {
								trDamageUnitPercent(0.0 - amt * trQuestVarGet("p"+p+"spellDamage") * trQuestVarGet("p"+p+"healBoost"));
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
					for (x=xGetDatabaseCount(dEnemies); >0) {
						id = yDatabaseNext("enemies", true);
						if (id == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (id == 1*trQuestVarGet("p"+p+"potions")) {
							stunUnit("enemies", 2.0, p);
							break;
						}
					}
					if (hit > 0) {
						for (x=xGetDatabaseCount(dEnemies) - 1; >0) {
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
					for(x=xGetDatabaseCount(dEnemies); >0) {
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
	
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		dist = 100;
		hit = -1;
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
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
			target = yGetVar("playerUnits", "player");
			ySetVar("playerUnits", "poisonTimeout", 0);
			ySetVar("playerUnits", "stunTimeout", 0);
			if (trQuestVarGet("playerUnits") == trQuestVarGet("p"+target+"unit")) {
				trQuestVarSet("p"+target+"silenceTimeout", 0);
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
		} else {
			if (trCurrentPlayer() == p) {
				trCounterAbort("well");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				trChatSend(0, "You must target an ally!");
			}
			trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COST);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
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
			gainFavor(p, 0.0 - trQuestVarGet("duplicateCost") * trQuestVarGet("p"+p+"ultimateCost"));
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
	
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
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
			trDamageUnitPercent(yGetVar("p"+p+"duplicates", "decay"));
		}
	}
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
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
		trSetCounterDisplay("Potion: FREEZE");
	}
	trQuestVarSet("p"+p+"potion", POTION_STUN);
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
