const int POTION_STUN = 0;
const int POTION_POISON = 1;
const int POTION_HEAL = 2;

int xPotionTimeout = 0;
int xPotionPos = 0;
int xPotionType = 0;

int xDuplicateIndex = 0;
int xDuplicateDecay = 0;
int xDuplicateDecayNext = 0;

int xNextPotion = 0;


float potionHeal = 5;

int elixirCooldown = 12;
float elixirHeal = 200;

float duplicateCost = 50;

void removeAlchemist(int p = 0) {
	removePlayerSpecific(p);
}

void alchemistAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * ALCHEMIST;
	int id = 0;
	int hit = 0;
	int target = 0;
	int db = getCharactersDB(p);
	int index = xGetPointer(dEnemies);
	int potions = trQuestVarGet("p"+p+"potions");
	int duplicates = trQuestVarGet("p"+p+"duplicates");
	float amt = 0;
	float dist = 0;
	float current = 0;
	vector pos = vector(0,0,0);
	vector end = vector(0,0,0);
	xSetPointer(dPlayerData, p);
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeAlchemist(p);
		} else {
			hit = CheckOnHit(p);
			if (hit == ON_HIT_NORMAL) {
				target = xGetInt(db, xCharAttackTarget);
				xSetInt(db, xNextPotion, 1 + xGetInt(db, xNextPotion));
				if (xGetInt(db, xNextPotion) == 3) {
					gainFavor(p, xGetInt(dPlayerData, xPlayerProjectiles));
					xSetInt(db, xNextPotion, 0);
					if (trQuestVarGet("p"+p+"potion") == POTION_HEAL) {
						amt = potionHeal * xGetFloat(dPlayerData, xPlayerSpellDamage) * xGetFloat(dPlayerData, xPlayerHealBoost);
						amt = amt * xGetInt(dPlayerData, xPlayerProjectiles);
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else if (xGetInt(dPlayerUnits, xPoisonStatus) == 0) {
								trDamageUnitPercent(0.0 - amt);
							}
						}
					} else {
						xAddDatabaseBlock(potions, true);
						xSetInt(potions, xUnitName, trGetUnitScenarioNameNumber(target));
						xSetInt(potions, xPotionType, 1*trQuestVarGet("p"+p+"potion"));
						pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
						end = kbGetBlockPosition(""+xGetInt(potions, xUnitName), true);
						xSetInt(potions, xPotionTimeout, trTimeMS() + 33 * distanceBetweenVectors(pos, end, false));
						xSetVector(potions, xPotionPos, end);
					}
				}
				
			}
		}
	}
	
	if (xGetDatabaseCount(potions) >0) {
		xDatabaseNext(potions);
		if (trTimeMS() > xGetInt(potions, xPotionTimeout)) {
			pos = xGetVector(potions, xPotionPos);
			switch(xGetInt(potions, xPotionType))
			{
				case POTION_STUN:
				{
					hit = xGetInt(dPlayerData, xPlayerProjectiles) - 1;
					for (x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (xGetInt(dEnemies, xUnitName) == xGetInt(potions, xUnitName)) {
							stunUnit(dEnemies, 2.0, p);
							break;
						}
					}
					if (hit > 0) {
						for (x=xGetDatabaseCount(dEnemies) - 1; >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < 25.0) {
								stunUnit(dEnemies, 2.0, p);
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
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lampades Blood");
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < 25.0) {
							poisonUnit(dEnemies, 12.0,12.0 * xGetInt(dPlayerData, xPlayerProjectiles), p);
						}
					}
				}
			}
			xFreeDatabaseBlock(potions);
		}
	}
	
	for(y=xGetInt(dPlayerData, xPlayerProjectiles); >0) {
		if (yFindLatest("p"+p+"latestProj", "Priest Projectile", p) > 0) {
			for (x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				id = xGetInt(db, xUnitID);
				trUnitSelectClear();
				trUnitSelectByID(id);
				if (trUnitAlive() == false) {
					removeAlchemist(p);
				} else if (kbUnitGetAnimationActionType(id) == 12) {
					if (xGetInt(db, xNextPotion) == 2) {
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
		pos = xGetVector(dPlayerData, xPlayerWellPos);
		dist = 100;
		hit = -1;
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			xUnitSelectByID(dPlayerUnits, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				amt = unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos);
				if (amt < dist) {
					dist = amt;
					hit = xGetPointer(dPlayerUnits);
				}
			}
		}
		if (hit > 0) {
			xSetPointer(dPlayerUnits, hit);
			xSetInt(dPlayerUnits, xPoisonTimeout, 0);
			xSetInt(dPlayerUnits, xStunTimeout, 0);
			xSetInt(dPlayerUnits, xSilenceTimeout, 0);
			trSoundPlayFN("recreation.wav","1",-1,"","");
			xUnitSelectByID(dPlayerUnits, xUnitID);
			healUnit(p, elixirHeal * xGetFloat(dPlayerData, xPlayerSpellDamage));
			pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
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
			xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COST);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = 81;
		hit = -1;
		for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
			xDatabaseNext(dPlayerCharacters);
			xUnitSelectByID(dPlayerCharacters, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerCharacter();
			} else {
				amt = unitDistanceToVector(xGetInt(dPlayerCharacters, xUnitName), pos);
				if (amt < dist) {
					dist = amt;
					hit = xGetPointer(dPlayerCharacters);
				}
			}
		}
		if (hit > 0) {
			gainFavor(p, 0.0 - duplicateCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
			xSetPointer(dPlayerCharacters, hit);
			pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName));
			xAddDatabaseBlock(duplicates, true);
			xSetInt(duplicates, xUnitName, trGetNextUnitScenarioNameNumber());
			
			xSetInt(duplicates, xDuplicateIndex, spawnPlayerClone(xGetInt(dPlayerCharacters, xPlayerOwner), pos));
			xSetInt(duplicates, xDuplicateDecayNext, trTime());
			trSoundPlayFN("changeunit.wav","1",-1,"","");
			trSoundPlayFN("sonofosirisbirth.wav","1",-1,"","");
			if (trCurrentPlayer() == xGetInt(dPlayerCharacters, xPlayerOwner)) {
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
	
	if (xGetDatabaseCount(duplicates) > 0) {
		xDatabaseNext(duplicates);
		xUnitSelect(duplicates, xUnitName);
		if (trUnitAlive() == false) {
			xFreeDatabaseBlock(duplicates);
		} else if (trTime() > xGetInt(duplicates, xDuplicateDecayNext)) {
			xSetInt(duplicates, xDuplicateDecayNext, 1 + xGetInt(duplicates, xDuplicateDecayNext));
			xSetFloat(duplicates, xDuplicateDecay, xGetFloat(duplicates, xDuplicateDecay) + calculateDecay(p, 1.0));
			trDamageUnitPercent(xGetFloat(duplicates, xDuplicateDecay));
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
}

void chooseAlchemist(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * ALCHEMIST;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
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
	
	xNextPotion = xInitAddInt(db, "nextPotion");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, elixirCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 0);
	xSetFloat(dPlayerData,xPlayerLureCost, duplicateCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"potions") == 0) {
		db = xInitDatabase("p"+p+"potions");
		trQuestVarSet("p"+p+"potions", db);
		xInitAddInt(db, "name");
		xPotionTimeout = xInitAddInt(db, "timeout");
		xPotionPos = xInitAddVector(db, "pos");
		xPotionType = xInitAddInt(db, "type");
	}
	
	if (trQuestVarGet("p"+p+"duplicates") == 0) {
		db = xInitDatabase("p"+p+"duplicates");
		trQuestVarSet("p"+p+"duplicates", db);
		xInitAddInt(db, "name");
		xDuplicateIndex = xInitAddInt(db, "index");
		xDuplicateDecay = xInitAddFloat(db, "decay");
		xDuplicateDecayNext = xInitAddInt(db, "next");
	}
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
}
