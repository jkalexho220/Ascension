
float spellstealerPassiveRadius = 6;

float bladeDanceCost = 10;
float bladeDanceRadius = 15;

float negationCloakDuration = 3;
int negationCloakCooldown = 12;

int spellbladesCooldown = 9;
int spellbladesCount = 3;

int xBladeDanceIndex = 0;
int xBladeDanceStatus = 0;

int xSpellbladePrev = 0;
int xSpellbladeDir = 0;
int xSpellbladeStatus = 0;

int xCloakSFX = 0;

void removeSpellstealer(int p = 0) {
	removePlayerSpecific(p);
}

void spellstealerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * SPELLSTEALER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int db = getCharactersDB(p);
	int bladeDanceTargets = trQuestVarGet("p"+p+"bladeDanceTargets");
	int spellblades = trQuestVarGet("p"+p+"spellblades");
	int index = xGetPointer(dEnemies);
	int stunned = 0;
	int poisoned = 0;
	int silenced = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector end = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
		if (trUnitAlive() == false) {
			removeSpellstealer(p);
		} else {
			hit = CheckOnHit(p);
			if (hit >= ON_HIT_NORMAL) {
				if (xSetPointer(dEnemies, xGetInt(db, xCharAttackTargetIndex))) {
					amt = 1;
					if (xGetInt(dEnemies, xStunStatus) > 0) {
						amt = amt * 2;
					}
					amt = amt * xsPow(2, xGetInt(dEnemies, xPoisonStatus) + xGetInt(dEnemies, xSilenceStatus)) - 1; // -1 because character attack
					xUnitSelectByID(dEnemies, xUnitID);
					damageEnemy(p, amt * xGetFloat(dPlayerData, xPlayerAttack), false);
					if (hit == ON_HIT_SPECIAL) {
						pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
						dist = xsPow(spellstealerPassiveRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
						trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("Lampades Blood");
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
								poisonUnit(dEnemies, 12.0, 12.0, p);
							}
						}
					}
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trUnitDestroy();
		dist = xsPow(bladeDanceRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		amt = dist;
		target = -1;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
				if (current < dist) {
					xAddDatabaseBlock(bladeDanceTargets, true);
					xSetInt(bladeDanceTargets, xUnitName, xGetInt(dEnemies, xUnitName));
					xSetInt(bladeDanceTargets, xBladeDanceIndex, xGetPointer(dEnemies));
					if (current < amt) {
						amt = current;
						target = xGetNewestPointer(bladeDanceTargets);
					}
				}
			}
		}
		if (target > 0) {
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeSpellstealer(p);
				} else {
					trMutateSelected(kbGetProtoUnitID("Revealer to Player"));
					if (PvP) {
						xSetPointer(dPlayerUnits, xGetInt(db, xCharIndex));
						xRestoreDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
						xFreeDatabaseBlock(dEnemies, xGetInt(dPlayerUnits, xDoppelganger));
						xFreeDatabaseBlock(dPlayerUnits);
					} else {
						xDetachDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex));
					}
				}
			}
			xSetBool(dPlayerData, xPlayerLaunched, true);
			
			amt = bladeDanceCost * xGetFloat(dPlayerData, xPlayerUltimateCost);
			
			xSetPointer(bladeDanceTargets, target);
			xSetInt(bladeDanceTargets, xBladeDanceStatus, 1*trQuestVarGet("p"+p+"spellStealStatus"));
			if (xSetPointer(dEnemies, xGetInt(bladeDanceTargets, xBladeDanceIndex))) {
				stunned = xGetInt(dEnemies, xStunStatus);
				poisoned = xGetInt(dEnemies, xPoisonStatus);
				silenced = xGetInt(dEnemies, xSilenceStatus);
			}
			
			for(x=xGetDatabaseCount(bladeDanceTargets); >0) {
				xDatabaseNext(bladeDanceTargets);
				if (target != xGetPointer(bladeDanceTargets)) {
					id = xGetInt(bladeDanceTargets, xBladeDanceIndex);
					hit = xGetInt(dEnemies, xStunStatus, id) * stunned;
					hit = hit + xGetInt(dEnemies, xPoisonStatus, id) * poisoned;
					hit = hit + xGetInt(dEnemies, xSilenceStatus, id) * silenced;
					if ((hit == 0) || (trPlayerResourceCount(p, "favor") < 2.0 * amt)) {
						xFreeDatabaseBlock(bladeDanceTargets);
					} else {
						xSetInt(bladeDanceTargets, xBladeDanceStatus, trQuestVarGet("p"+p+"spellStealStatus"));
					}
				}
			}
			trQuestVarSet("p"+p+"spellstealStatus", 0);
			trQuestVarSet("p"+p+"bladeDanceNext", trTimeMS() - 1);
			xSetPointer(bladeDanceTargets, target);
			
			if (xGetDatabaseCount(bladeDanceTargets) > 1) {
				amt = amt * 2;
			}
			gainFavor(p, 0.0 - amt);
		} else {
			xClearDatabase(bladeDanceTargets);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				trChatSend(0, "There are no enemies near your cursor!");
			}
		}
	}
	
	if (xGetDatabaseCount(bladeDanceTargets) > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"bladeDanceNext")) {
			trQuestVarSet("p"+p+"bladeDanceNext", trQuestVarGet("p"+p+"bladeDanceNext") + 300);
			for(x=xGetDatabaseCount(bladeDanceTargets); >0) {
				xDatabaseNext(bladeDanceTargets);
				xUnitSelect(bladeDanceTargets, xUnitName);
				if (trUnitAlive() == false) {
					xFreeDatabaseBlock(bladeDanceTargets);
				} else if (xSetPointer(dEnemies, xGetInt(bladeDanceTargets, xBladeDanceIndex))) {
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					hit = xGetInt(bladeDanceTargets, xBladeDanceStatus);
					amt = xGetFloat(dPlayerData, xPlayerAttack) * xGetFloat(dPlayerData, xPlayerSpellDamage);
					if (hit >= xsPow(2, STATUS_SILENCE)) {
						hit = hit - xsPow(2, STATUS_SILENCE);
						silenceUnit(dEnemies, 9.0, p);
					}
					if (hit >= xsPow(2, STATUS_POISON)) {
						hit = hit - xsPow(2, STATUS_POISON);
						poisonUnit(dEnemies, 12.0, 12.0, p);
					}
					if (hit >= xsPow(2, STATUS_STUN)) {
						hit = hit - xsPow(2, STATUS_STUN);
						stunUnit(dEnemies, 2.0, p);
					}
					if (xGetInt(dEnemies, xStunStatus) > 0) {
						amt = amt * 2;
					}
					amt = amt * xsPow(2, xGetInt(dEnemies, xPoisonStatus) + xGetInt(dEnemies, xSilenceStatus));
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0", "Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					xUnitSelect(bladeDanceTargets, xUnitName);
					damageEnemy(p, amt, false, 1.0);
					OnHit(p, xGetInt(bladeDanceTargets, xBladeDanceIndex), false);
					xFreeDatabaseBlock(bladeDanceTargets);
					break;
				}
			}
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			if (xGetDatabaseCount(bladeDanceTargets) == 0) {
				trUnitChangeProtoUnit("Transport Ship Greek");
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db, xUnitID);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Swordsman Hero");
					xUnitSelectByID(db, xUnitID);
					trMutateSelected(kbGetProtoUnitID("Swordsman Hero"));
					if (PvP) {
						xSetInt(db, xCharIndex,activatePlayerUnit(xGetInt(db, xUnitName),p,kbGetProtoUnitID("Swordsman Hero")));
						xSetBool(dPlayerUnits, xIsHero, true);
						xSetFloat(dPlayerUnits, xPhysicalResist, xGetFloat(dPlayerData, xPlayerPhysicalResist, p));
						xSetFloat(dPlayerUnits, xMagicResist, xGetFloat(dPlayerData, xPlayerMagicResist, p));
						if (xGetInt(db, xUnitName) == xGetInt(dPlayerData, xPlayerUnit)) {
							xSetInt(dPlayerData, xPlayerIndex, xGetInt(db, xCharIndex));
						}
					} else if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(db, xCharIndex)) == false) {
						debugLog("Spellstealer " + p + ": Unable to restore database block");
					}
				}
				xSetBool(dPlayerData, xPlayerLaunched, false);
				equipRelicsAgain(p);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Kronny Birth SFX");
			} else {
				trUnitChangeProtoUnit("Kronny Birth SFX");
			}
		}
	}
	
	for(y=xsMin(2, xGetDatabaseCount(spellblades)); >0) {
		hit = 0;
		xDatabaseNext(spellblades);
		pos = xGetVector(spellblades, xSpellbladePrev);
		dir = xGetVector(spellblades, xSpellbladeDir);
		dist = unitDistanceToVector(xGetInt(spellblades, xUnitName), pos, false);
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies,xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else if (rayCollision(dEnemies, pos, dir, dist + 3.0, 9.0)) {
				trQuestVarSet("spellsound", 2);
				amt = 0.5 * xGetFloat(dPlayerData, xPlayerAttack) * xGetFloat(dPlayerData, xPlayerSpellDamage);
				hit = xGetInt(spellblades, xSpellbladeStatus);
				if (hit >= xsPow(2, STATUS_SILENCE)) {
					hit = hit - xsPow(2, STATUS_SILENCE);
					silenceUnit(dEnemies, 9.0, p);
				}
				if (hit >= xsPow(2, STATUS_POISON)) {
					hit = hit - xsPow(2, STATUS_POISON);
					poisonUnit(dEnemies, 12.0, 12.0, p);
				}
				if (hit >= xsPow(2, STATUS_STUN)) {
					hit = hit - xsPow(2, STATUS_STUN);
					stunUnit(dEnemies, 2.0, p);
				}
				if (xGetInt(dEnemies, xStunStatus) > 0) {
					amt = amt * 2;
				}
				amt = amt * xsPow(2, xGetInt(dEnemies, xPoisonStatus) + xGetInt(dEnemies, xSilenceStatus));
				damageEnemy(p, amt, true);
				hit = 1;
			}
		}
		if (hit == 0) {
			pos = kbGetBlockPosition(""+xGetInt(spellblades, xUnitName), true);
			if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hit = 1;
				trQuestVarSet("spellsound", 1);
			} else {
				xSetVector(spellblades, xSpellbladePrev, pos);
			}
		}
		
		if (hit == 1) {
			gainFavor(p, 1);
			xUnitSelect(spellblades, xUnitName);
			trUnitChangeProtoUnit("Lightning Sparks Ground");
			xUnitSelect(spellblades, xUnitName);
			trDamageUnitPercent(-100);
			xFreeDatabaseBlock(spellblades);
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
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trSoundPlayFN("manticorespecialattack.wav","1",-1,"","");
		for(y=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeSpellstealer(p);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				hit = spellbladesCount + 2 * xGetInt(dPlayerData, xPlayerProjectiles);
				amt = angleBetweenVectors(pos, xGetVector(dPlayerData, xPlayerWellPos));
				amt = fModulo(6.283185, amt - 0.196349 * (hit - 1) * 0.5);
				for(x=hit; >0) {
					dir = xsVectorSet(xsSin(amt),0,xsCos(amt)); // AoM devs not the sharpest tools in the shed
					end = pos + dir;
					end = vectorSetAsTargetVector(pos,end,300.0);
					amt = fModulo(6.283185, amt + 0.196349);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trMutateSelected(kbGetProtoUnitID("Stymph Bird Feather"));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitMoveToPoint(xsVectorGetX(end),0,xsVectorGetZ(end),-1,false);
					xAddDatabaseBlock(spellblades, true);
					xSetInt(spellblades, xUnitName, 1*trQuestVarGet("next"));
					xSetVector(spellblades, xSpellbladePrev, pos);
					xSetVector(spellblades, xSpellbladeDir, dir);
					xSetInt(spellblades, xSpellbladeStatus, 1*trQuestVarGet("p"+p+"spellstealStatus"));
				}
			}
		}
		trQuestVarSet("p"+p+"spellstealStatus", 0);
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		trQuestVarSet("p"+p+"negationCloak", 1);
		trQuestVarSet("p"+p+"negationCloakTimeout",
			trTimeMS() + 1000 * negationCloakDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
		trSoundPlayFN("spybirth.wav","1",-1,"","");
	}
	
	if (trQuestVarGet("p"+p+"negationCloak") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"negationCloakTimeout")) {
			trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
			trQuestVarSet("p"+p+"negationCloak", 0);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeSpellstealer(p);
				} else {
					xUnitSelect(db, xCloakSFX);
					trUnitChangeProtoUnit("Cinematic Block");
				}
			}
		} else if (trTime() > trQuestVarGet("p"+p+"negationCloakNext")) {
			trQuestVarSet("p"+p+"negationCloakNext", trTime());
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeSpellstealer(p);
				} else {
					if (kbGetBlockID(""+xGetInt(db, xCloakSFX)) == -1) {
						spyEffect(xGetInt(db, xUnitName),kbGetProtoUnitID("Kronny Birth SFX"), xsVectorSet(db, xCloakSFX, xGetPointer(db)));
					} else {
						xUnitSelect(db, xCloakSFX);
						trUnitChangeProtoUnit("Kronny Birth SFX");
					}
				}
			}
		}
	}
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseSpellstealer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * SPELLSTEALER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
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
	
	xCloakSFX = xInitAddInt(db, "cloakSFX");
	
	xSetInt(dPlayerData,xPlayerWellCooldown, spellbladesCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, 1);
	xSetFloat(dPlayerData,xPlayerLureCost, bladeDanceCost);
	xSetInt(dPlayerData,xPlayerRainCooldown,negationCloakCooldown);
	xSetFloat(dPlayerData,xPlayerRainCost, 0);
	
	if (trQuestVarGet("p"+p+"bladeDanceTargets") == 0) {
		db = xInitDatabase("p"+p+"bladeDanceTargets");
		trQuestVarSet("p"+p+"bladeDanceTargets", db);
		xInitAddInt(db, "name");
		xBladeDanceIndex = xInitAddInt(db, "index");
		xBladeDanceStatus = xInitAddInt(db, "status");
	}
	
	if (trQuestVarGet("p"+p+"spellblades") == 0) {
		db = xInitDatabase("p"+p+"spellblades");
		trQuestVarSet("p"+p+"spellblades", db);
		xInitAddInt(db, "name");
		xSpellbladePrev = xInitAddVector(db, "prev");
		xSpellbladeDir = xInitAddVector(db, "dir");
		xSpellbladeStatus = xInitAddInt(db, "status");
	}
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
}
