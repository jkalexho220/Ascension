
float spellstealerPassiveRadius = 6;

float bladeDanceCost = 16;
float bladeDanceRadius = 16;

float negationCloakDuration = 3;
int negationCloakCooldown = 12;

int spellbladesCooldown = 9;
int spellbladesCount = 5;

int xBladeDanceIndex = 0;
int xBladeDanceStatus = 0;

int xSpellbladePrev = 0;
int xSpellbladeDir = 0;
int xSpellbladeStatus = 0;
int xSpellbladePierce = 0;

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
					amt = xsPow(2, xGetInt(dEnemies, xPoisonStatus) + xGetInt(dEnemies, xSilenceStatus) + xsMin(1, xGetInt(dEnemies, xStunStatus))) - 1; // -1 because character attack
					xSetInt(dPlayerData, xPlayerClass, MOONBLADE); // disable the built-in damage multiplier of damageEnemy(). hacky but whatever
					xUnitSelectByID(dEnemies, xUnitID);
					damageEnemy(p, amt * xGetFloat(dPlayerData, xPlayerAttack), false);
					xSetInt(dPlayerData, xPlayerClass, SPELLSTEALER);
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
					if ((hit == 0) || (trPlayerResourceCount(p, "favor") < amt)) {
						xFreeDatabaseBlock(bladeDanceTargets);
					} else {
						xSetInt(bladeDanceTargets, xBladeDanceStatus, trQuestVarGet("p"+p+"spellStealStatus"));
					}
				}
			}
			trQuestVarSet("p"+p+"spellstealStatus", 0);
			trQuestVarSet("p"+p+"bladeDanceNext", trTimeMS() - 1);
			xSetPointer(bladeDanceTargets, target);
			
			if (xGetDatabaseCount(bladeDanceTargets) == 1) {
				amt = amt * 0.5;
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
						silenceUnit(dEnemies, 6.0, p);
					}
					if (hit >= xsPow(2, STATUS_POISON)) {
						hit = hit - xsPow(2, STATUS_POISON);
						poisonUnit(dEnemies, 12.0, 12.0, p);
					}
					if (hit >= xsPow(2, STATUS_STUN)) {
						hit = hit - xsPow(2, STATUS_STUN);
						stunUnit(dEnemies, 3.0, p);
					}
					pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0", "Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					xUnitSelect(bladeDanceTargets, xUnitName);
					damageEnemy(p, amt, false, 1.0);
					OnHit(p, xGetInt(bladeDanceTargets, xBladeDanceIndex), false);
					xFreeDatabaseBlock(bladeDanceTargets);
					for(y=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xSetInt(db,xCharSpecialAttack,xGetInt(db,xCharSpecialAttack) - 1);
					}
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
		xDatabaseNext(spellblades);
		pos = xGetVector(spellblades, xSpellbladePrev);
		dist = unitDistanceToVector(xGetInt(spellblades, xUnitName), pos);
		if (dist > 9.0) {
			dist = xsSqrt(dist) + 2.0;
			dir = xGetVector(spellblades, xSpellbladeDir);
			target = 0;
			amt = 0.5 * xGetFloat(dPlayerData, xPlayerAttack) * xGetFloat(dPlayerData, xPlayerSpellDamage);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies,xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (rayCollision(dEnemies, pos, dir, dist, 4.0, true)) {
					trQuestVarSet("spellsound", 2);
					hit = xGetInt(spellblades, xSpellbladeStatus);
					if (hit >= xsPow(2, STATUS_SILENCE)) {
						hit = hit - xsPow(2, STATUS_SILENCE);
						silenceUnit(dEnemies, 6.0, p);
					}
					if (hit >= xsPow(2, STATUS_POISON)) {
						hit = hit - xsPow(2, STATUS_POISON);
						poisonUnit(dEnemies, 12.0, 12.0, p);
					}
					if (hit >= xsPow(2, STATUS_STUN)) {
						hit = hit - xsPow(2, STATUS_STUN);
						stunUnit(dEnemies, 3.0, p);
					}
					hit = xsPow(2, STATUS_SILENCE) * xGetInt(dEnemies, xSilenceStatus) + xsPow(2, STATUS_POISON) * xGetInt(dEnemies, xPoisonStatus) + xsPow(2, STATUS_STUN) * xsMin(1, xGetInt(dEnemies, xStunStatus));
					xSetInt(spellblades, xSpellbladeStatus, hit);
					damageEnemy(p, amt, true);
					target = target + 1;
				}
			}
			
			gainFavor(p, target);
			xSetInt(spellblades, xSpellbladePierce, xGetInt(spellblades, xSpellbladePierce) - target);
			if (xGetInt(spellblades, xSpellbladePierce) <= 0) {
				xUnitSelect(spellblades, xUnitName);
				trUnitChangeProtoUnit("Lightning Sparks Ground");
				xFreeDatabaseBlock(spellblades);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(spellblades, xUnitName), true);
				if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					xUnitSelect(spellblades, xUnitName);
					trUnitChangeProtoUnit("Lightning Sparks Ground");
					xFreeDatabaseBlock(spellblades);
					trQuestVarSet("spellsound", 1);
				} else {
					xSetVector(spellblades, xSpellbladePrev, pos);
				}
			}
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
				/*
				hit = spellbladesCount + 2 * xGetInt(dPlayerData, xPlayerProjectiles);
				amt = angleBetweenVectors(pos, xGetVector(dPlayerData, xPlayerWellPos));
				amt = fModulo(6.283185, amt - 0.196349 * (spellbladesCount - 1) * 0.5);
				dir = xsVectorSet(xsSin(amt),0,xsCos(amt)); // AoM devs not the sharpest tools in the shed
				*/
				dir = getUnitVector(pos, xGetVector(dPlayerData, xPlayerWellPos));
				dir = rotationMatrix(dir, 0.923879, -0.382682);
				for(x=spellbladesCount; >0) {
					end = pos + dir;
					end = vectorSetAsTargetVector(pos,end,300.0);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trMutateSelected(kbGetProtoUnitID("Stymph Bird Feather"));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitMoveToPoint(xsVectorGetX(end),0,xsVectorGetZ(end),-1,false);
					xAddDatabaseBlock(spellblades, true);
					xSetInt(spellblades, xUnitName, 1*trQuestVarGet("next"));
					xSetVector(spellblades, xSpellbladePrev, pos - (dir * 2.0));
					xSetVector(spellblades, xSpellbladeDir, dir);
					xSetInt(spellblades, xSpellbladeStatus, 1*trQuestVarGet("p"+p+"spellstealStatus"));
					xSetInt(spellblades, xSpellbladePierce, xGetInt(dPlayerData, xPlayerProjectiles));
					dir = rotationMatrix(dir, 0.980785, 0.195089);
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
		xSpellbladePierce = xInitAddInt(db, "pierce");
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
