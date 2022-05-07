void removeCamTracks(int eventID = -1) {
	trackRemove();
	trUnblockAllSounds();
}

void fixAnimations(int p = 0) {
	/*
	Prevent hip-thrusting heroes from invisible relic holding
	*/
	int action = 0;
	int id = kbGetBlockID(""+xGetInt(dPlayerData,xPlayerUnit,p));
	if (id > 0) {
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
		action = kbUnitGetAnimationActionType(id);
		if (xGetInt(dPlayerData, xPlayerAnimation, p) == action) {
			if (action == -1) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			}
		} else {
			xSetInt(dPlayerData, xPlayerAnimation, action, p);
			if ((action == 11) && (xGetInt(dPlayerData, xPlayerAction, p) != 12) &&
				(xGetInt(dPlayerData, xPlayerAction, p) != 30)) {
				trUnitSetAnimationPath("0,0,0,0,0,0,0");
			} else if (action == -1) {
				xsSetContextPlayer(p);
				action = kbUnitGetActionType(id);
				xsSetContextPlayer(0);
				xSetInt(dPlayerData, xPlayerAction, action, p);
			}
		}
	}
}

int noSpecialsNext = 0;
void noSpecials() {
	if (trTime() > noSpecialsNext) {
		noSpecialsNext = trTime();
		for(p=ENEMY_PLAYER; >0) {
			trModifyProtounit("Frost Giant", p, 9, -99990);
			trModifyProtounit("Frost Giant", p, 9, 99999);
			trModifyProtounit("Medusa", p, 9, -99990);
			trModifyProtounit("Medusa", p, 9, 99999);
			trModifyProtounit("Mummy", p, 9, -99990);
			trModifyProtounit("Mummy", p, 9, 99999);
			trModifyProtounit("Scorpion Man", p, 9, -99990);
			trModifyProtounit("Scorpion Man", p, 9, 99999);
			trModifyProtounit("Battle Boar", p, 9, -99990);
			trModifyProtounit("Battle Boar", p, 9, 99999);
			trModifyProtounit("Nereid", p, 9, -99990);
			trModifyProtounit("Nereid", p, 9, 99999);
			trModifyProtounit("Lampades", p, 9, -99990);
			trModifyProtounit("Lampades", p, 9, 99999);
			trModifyProtounit("Einheriar", p, 9, -99990);
			trModifyProtounit("Einheriar", p, 9, 99999);
			trModifyProtounit("Fire Giant", p, 9, -99990);
			trModifyProtounit("Fire Giant", p, 9, 99999);
		}
	}
}

void processSilence(int p = 0) {
	if (xGetBool(dPlayerData, xPlayerSilenced, p) && (xGetInt(dPlayerData, xPlayerDead, p) == 0)) {
		if (xGetInt(dPlayerUnits, xSilenceStatus, xGetInt(dPlayerData, xPlayerIndex, p)) == 0) {
			xSetBool(dPlayerData, xPlayerSilenced, false, p);
			if (xGetInt(dPlayerData, xPlayerWellCooldownStatus, p) == ABILITY_READY) {
				trTechGodPower(p, "Underworld Passage", 1);
				if (trCurrentPlayer() == p) {
					trCounterAbort("well");
					trCounterAddTime("well", -1, -99999, wellName);
				}
			}
			if (xGetInt(dPlayerData, xPlayerLureCooldownStatus, p) == ABILITY_READY) {
				trTechGodPower(p, "Animal magnetism", 1);
				if (trCurrentPlayer() == p) {
					trCounterAbort("lure");
					trCounterAddTime("lure", -1, -99999, lureName);
				}
			}
			if (xGetInt(dPlayerData, xPlayerRainCooldownStatus, p) == ABILITY_READY) {
				trTechGodPower(p, "rain", 1);
				if (trCurrentPlayer() == p) {
					trCounterAbort("rain");
					trCounterAddTime("rain", -1, -99999, rainName);
				}
			}
			if (trCurrentPlayer() == p) {
				trCounterAbort("silence");
			}
		}
	}
}

void processLifesteal(int p = 0) {
	int simp = 0;
	if (xGetFloat(dPlayerData, xPlayerLifestealTotal, p) > 0) {
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
		healUnit(p, xGetFloat(dPlayerData,xPlayerLifestealTotal,p), xGetInt(dPlayerData,xPlayerIndex,p));
		/* simp benefits */
		simp = xGetInt(dPlayerData,xPlayerSimp,p);
		if (simp > 0) {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,simp),true);
			healUnit(p, xGetFloat(dPlayerData,xPlayerLifestealTotal,p), xGetInt(dPlayerData,xPlayerIndex,simp));
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
		}
		xSetFloat(dPlayerData,xPlayerLifestealTotal,0,p);
	}
}

void processRegen(int p = 0) {
	float amt = 0;
	float diff = 0;
	if (xGetFloat(dPlayerData, xPlayerFavorRegen, p) != 0) {
		if (trTimeMS() > xGetInt(dPlayerData, xPlayerRegenerateFavorLast, p) + 1000) {
			amt = trTimeMS() - xGetInt(dPlayerData, xPlayerRegenerateFavorLast, p);
			amt = amt * 0.001 * xGetFloat(dPlayerData, xPlayerFavorRegen, p);
			gainFavor(p, amt);
			xSetInt(dPlayerData, xPlayerRegenerateFavorLast,trTimeMS(),p);
		}
	}
	if (trTimeMS() > xGetInt(dPlayerData, xPlayerRegenerateHealthLast, p) + 1000) {
		amt = 0;
		diff = trTimeMS() - xGetInt(dPlayerData, xPlayerRegenerateHealthLast, p);
		if (xGetInt(dPlayerData, xPlayerGodBoon, p) == BOON_REGENERATE_HEALTH) {
			amt = diff * 0.00002 * xGetFloat(dPlayerData, xPlayerHealth, p);
		}
		if (xGetFloat(dPlayerData, xPlayerDefiance, p) > 0) {
			amt = amt + 0.001 * diff * xGetFloat(dPlayerData, xPlayerDefiance, p) * xGetDatabaseCount(dEnemies);
		}
		xSetFloat(dPlayerData, xPlayerLifestealTotal, xGetFloat(dPlayerData, xPlayerLifestealTotal, p) + amt, p);
		xSetInt(dPlayerData, xPlayerRegenerateHealthLast, trTimeMS(), p);
	}
}

void checkResourceCheating(int p = 0) {
	if (trPlayerResourceCount(p, "gold") > xGetInt(dPlayerData, xPlayerGold, p)) {
		trPlayerGrantResources(p, "gold", xGetInt(dPlayerData, xPlayerGold, p) - trPlayerResourceCount(p, "gold"));
		if (trCurrentPlayer() == p) {
			trChatSendSpoofed(0, "Zenophobia: Did you really think I wouldn't catch that?");
		}
	} else if (trPlayerResourceCount(p, "gold") < xGetInt(dPlayerData, xPlayerGold, p)) {
		xSetInt(dPlayerData, xPlayerGold, trPlayerResourceCount(p, "gold"), p);
	}
	if (trPlayerResourceCount(p, "favor") > 1 + xGetFloat(dPlayerData, xPlayerFavor, p)) {
		gainFavor(p, 0);
		if (trCurrentPlayer() == p) {
			trChatSendSpoofed(0, "Zenophobia: Nice try, buddy.");
		}
	}
}

void checkGodPowers(int p = 0) {
	float cost = 0;
	/* well ability */
	switch(xGetInt(dPlayerData, xPlayerWellCooldownStatus, p))
	{
		case ABILITY_READY:
		{
			if (trPlayerUnitCountSpecific(p, "Tunnel") == 2) {
				yFindLatest("p"+p+"wellObject", "Tunnel", p);
				xSetVector(dPlayerData, xPlayerWellPos, kbGetBlockPosition(""+1*trQuestVarGet("p"+p+"wellObject"), true), p);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trDamageUnitPercent(100);
				yFindLatest("p"+p+"wellObject", "Tunnel", p);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				trDamageUnitPercent(100);
				xSetBool(dPlayerData, xPlayerWellActivated, true, p);
				xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COOLDOWN, p);
				xSetInt(dPlayerData, xPlayerWellReadyTime,
					trTimeMS() + 1000 * xGetInt(dPlayerData,xPlayerWellCooldown,p) * xGetFloat(dPlayerData,xPlayerCooldownReduction,p), p);
				if (trCurrentPlayer() == p) {
					trCounterAbort("well");
					trCounterAddTime("well",
						xGetInt(dPlayerData, xPlayerWellCooldown, p) * xGetFloat(dPlayerData, xPlayerCooldownReduction, p), 0, wellName);
				}
			} else {
				trQuestVarSet("p"+p+"wellObject", trGetNextUnitScenarioNameNumber() - 1);
			}
		}
		case ABILITY_COOLDOWN:
		{
			if (trTimeMS() > xGetInt(dPlayerData, xPlayerWellReadyTime, p)) {
				xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COST, p);
			}
		}
		case ABILITY_COST:
		{
			cost = xGetFloat(dPlayerData, xPlayerWellCost, p) * xGetFloat(dPlayerData, xPlayerUltimateCost, p);
			if (xGetFloat(dPlayerData, xPlayerFavor, p) >= cost) {
				xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_READY, p);
				if (xGetBool(dPlayerData, xPlayerSilenced, p) == false) {
					trTechGodPower(p, "Underworld Passage", 1);
					if (trCurrentPlayer() == p) {
						trCounterAddTime("well", -1, -99999, wellName);
						if (wellIsUltimate) {
							trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
						}
					}
				}
			}
		}
	}
	
	/* rain ability */
	switch(xGetInt(dPlayerData, xPlayerRainCooldownStatus, p))
	{
		case ABILITY_READY:
		{
			if (trCheckGPActive("rain", p)) {
				xSetBool(dPlayerData, xPlayerRainActivated, true, p);
				xSetInt(dPlayerData, xPlayerRainCooldownStatus, ABILITY_COOLDOWN, p);
				xSetInt(dPlayerData, xPlayerRainReadyTime,
					trTimeMS() + 1000 * xGetInt(dPlayerData,xPlayerRainCooldown,p) * xGetFloat(dPlayerData,xPlayerCooldownReduction,p), p);
				if (trCurrentPlayer() == p) {
					trCounterAbort("Rain");
					trCounterAddTime("Rain",
						xGetInt(dPlayerData, xPlayerRainCooldown, p) * xGetFloat(dPlayerData, xPlayerCooldownReduction, p), 0, rainName);
				}
			}
		}
		case ABILITY_COOLDOWN:
		{
			if (trTimeMS() > xGetInt(dPlayerData, xPlayerRainReadyTime, p)) {
				xSetInt(dPlayerData, xPlayerRainCooldownStatus, ABILITY_COST, p);
			}
		}
		case ABILITY_COST:
		{
			cost = xGetFloat(dPlayerData, xPlayerRainCost, p) * xGetFloat(dPlayerData, xPlayerUltimateCost, p);
			if (xGetFloat(dPlayerData, xPlayerFavor, p) >= cost) {
				xSetInt(dPlayerData, xPlayerRainCooldownStatus, ABILITY_READY, p);
				if (xGetBool(dPlayerData, xPlayerSilenced, p) == false) {
					trTechGodPower(p, "Rain", 1);
					if (trCurrentPlayer() == p) {
						trCounterAddTime("Rain", -1, -99999, rainName);
						if (rainIsUltimate) {
							trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
						}
					}
				}
			}
		}
	}
	
	/* lure ability */
	switch(xGetInt(dPlayerData, xPlayerLureCooldownStatus, p))
	{
		case ABILITY_READY:
		{
			if (trPlayerUnitCountSpecific(p, "Animal Attractor") == 1) {
				yFindLatestReverse("p"+p+"LureObject", "Animal Attractor", p);
				xSetVector(dPlayerData, xPlayerLurePos, kbGetBlockPosition(""+1*trQuestVarGet("p"+p+"LureObject"), true), p);
				trMutateSelected(kbGetProtoUnitID("Rocket"));
				xSetBool(dPlayerData, xPlayerLureActivated, true, p);
				xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COOLDOWN, p);
				xSetInt(dPlayerData, xPlayerLureReadyTime,
					trTimeMS() + 1000 * xGetInt(dPlayerData,xPlayerLureCooldown,p) * xGetFloat(dPlayerData,xPlayerCooldownReduction,p), p);
				if (trCurrentPlayer() == p) {
					trCounterAbort("Lure");
					trCounterAddTime("Lure",
						xGetInt(dPlayerData, xPlayerLureCooldown, p) * xGetFloat(dPlayerData, xPlayerCooldownReduction, p), 0, lureName);
				}
			} else {
				trQuestVarSet("p"+p+"LureObject", trGetNextUnitScenarioNameNumber() - 1);
			}
		}
		case ABILITY_COOLDOWN:
		{
			if (trTimeMS() > xGetInt(dPlayerData, xPlayerLureReadyTime, p)) {
				xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST, p);
			}
		}
		case ABILITY_COST:
		{
			cost = xGetFloat(dPlayerData, xPlayerLureCost, p) * xGetFloat(dPlayerData, xPlayerUltimateCost, p);
			if (xGetFloat(dPlayerData, xPlayerFavor, p) >= cost) {
				xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_READY, p);
				if (xGetBool(dPlayerData, xPlayerSilenced, p) == false) {
					trTechGodPower(p, "animal magnetism", 1);
					if (trCurrentPlayer() == p) {
						trCounterAddTime("Lure", -1, -99999, lureName);
						if (lureIsUltimate) {
							trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
						}
					}
				}
			}
		}
	}
}

void maintainStun() {
	int id = 0;
	for(x=xGetDatabaseCount(dStunnedUnits); >0) {
		xDatabaseNext(dStunnedUnits);
		xUnitSelect(dStunnedUnits, xUnitName);
		if (trUnitAlive() == false) {
			trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dStunnedUnits, xStunnedProto)));
			xFreeDatabaseBlock(dStunnedUnits);
		} else {
			if ((xGetInt(dStunnedUnits, xUnitName) != bossUnit) ||
				(bossAnim == false)) {
				trMutateSelected(xGetInt(dStunnedUnits, xStunnedProto));
				trUnitOverrideAnimation(2, 0, false, false, -1, 0);
			}
		}
	}
}

void playerLasers() {
	for(x=xsMin(3, xGetDatabaseCount(dPlayerLasers)); >0) {
		xDatabaseNext(dPlayerLasers);
		xUnitSelect(dPlayerLasers, xUnitName);
		if (trTimeMS() > xGetInt(dPlayerLasers, xPlayerLaserTimeout)) {
			trUnitDestroy();
			xFreeDatabaseBlock(dPlayerLasers);
		} else {
			float width = 4.0 * (xGetInt(dPlayerLasers, xPlayerLaserTimeout) - trTimeMS()) / 500;
			trSetSelectedScale(width, width, xGetFloat(dPlayerLasers, xPlayerLaserRange));
		}
	}
}

void relicTransporterGuy(int p = 0) {
	int id = 0;
	int db = getWarehouseDB(p);
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelect(db, xUnitName);
		if ((trUnitGetIsContained("Villager Atlantean Hero") || trUnitGetIsContained("Cinematic Block")) == false) {
			id = kbGetBlockID(""+xGetInt(db, xUnitName));
			if (kbGetUnitBaseTypeID(id) == relicProto(xGetInt(db, xRelicType))) {
				trUnitChangeProtoUnit("Relic");
				xAddDatabaseBlock(dFreeRelics, true);
				xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
				xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
			} else {
				trSoundPlayFN("storehouse.wav","1",-1,"","");
			}
			xFreeDatabaseBlock(db);
		}
	}
}

void processFreeRelics(int count = 1) {
	float amt = 0;
	int db = 0;
	vector pos = vector(0,0,0);
	for (x=xsMin(count, xGetDatabaseCount(dFreeRelics)); > 0) {
		amt = 0;
		xDatabaseNext(dFreeRelics);
		xUnitSelect(dFreeRelics, xUnitName);
		if (trUnitGetIsContained("Unit")) {
			if (trUnitGetIsContained("Villager Atlantean Hero")) {
				if (xGetInt(dFreeRelics, xRelicType) == RELIC_NICKONHAWK) {
					if (trUnitIsOwnedBy(trCurrentPlayer())) {
						startNPCDialog(NPC_NICK_NO);
					}
					trUnitChangeProtoUnit("Relic");
				} else {
					for(p=1; < ENEMY_PLAYER) {
						if (trUnitIsOwnedBy(p)) {
							trSetSelectedScale(0,0,-1);
							trMutateSelected(relicProto(xGetInt(dFreeRelics, xRelicType)));
							if (xGetInt(dFreeRelics, xRelicType) < KEY_RELICS) {
								trUnitSetAnimationPath("1,0,1,1,0,0,0");
							}
							db = getWarehouseDB(p);
							xAddDatabaseBlock(db, true);
							xSetInt(db, xUnitName, xGetInt(dFreeRelics, xUnitName));
							xSetInt(db, xRelicType, xGetInt(dFreeRelics, xRelicType));
							xFreeDatabaseBlock(dFreeRelics);
							break;
						}
					}
				}
			} else {
				pos = kbGetBlockPosition(""+xGetInt(dFreeRelics, xUnitName), true);
				for(p=1; < ENEMY_PLAYER) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
					if (trUnitAlive()) {
						if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), pos) < 1.5) {
							amt = 1;
							break;
						}
					}
				}
				if (amt == 1) {
					xUnitSelect(dFreeRelics, xUnitName);
					trSetSelectedScale(0,0,-1);
					trMutateSelected(relicProto(xGetInt(dFreeRelics, xRelicType)));
					if (xGetInt(dFreeRelics, xRelicType) < KEY_RELICS) {
						trUnitSetAnimationPath("1,0,1,1,0,0,0");
					}
					if (trCurrentPlayer() == p) {
						trChatSend(0, relicName(xGetInt(dFreeRelics, xRelicType)) + " equipped!");
						trSoundPlayFN("researchcomplete.wav","1",-1,"","");
					}
					db = getRelicsDB(p);
					xAddDatabaseBlock(db, true);
					xSetInt(db, xUnitName, xGetInt(dFreeRelics, xUnitName));
					xSetInt(db, xRelicType, xGetInt(dFreeRelics, xRelicType));
					relicEffect(xGetInt(dFreeRelics, xRelicType), p, true);
					xFreeDatabaseBlock(dFreeRelics);
				}
			}
		} else if (trUnitIsSelected()) {
			trShowImageDialog(relicIcon(xGetInt(dFreeRelics, xRelicType)), relicName(xGetInt(dFreeRelics, xRelicType)));
			reselectMyself();
		}
	}
}

void processWolves() {
	if (xGetDatabaseCount(dPlayerWolves) > 0) {
		xDatabaseNext(dPlayerWolves);
		if (xGetBool(dPlayerWolves, xPlayerWolfDead)) {
			if (trTimeMS() > xGetInt(dPlayerWolves, xPlayerWolfTimeout)) {
				xUnitSelect(dPlayerWolves, xUnitName);
				trUnitChangeProtoUnit("Dust Small");
				xFreeDatabaseBlock(dPlayerWolves);
			}
		} else {
			xUnitSelect(dPlayerWolves, xUnitName);
			if (trUnitAlive() == false) {
				xSetInt(dPlayerWolves, xPlayerWolfTimeout, 3000 + trTimeMS());
				xSetBool(dPlayerWolves, xPlayerWolfDead, true);
			}
		}
	}
}

void petDogs(int p = 0) {
	vector pos = vector(0,0,0);
	if (xGetBool(dPlayerData, xPlayerPetDogReady, p)) {
		if (trTime() > xGetInt(dPlayerData, xPlayerPetDogNext, p)) {
			pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
			xAddDatabaseBlock(dPlayerWolves, true);
			xSetInt(dPlayerWolves, xUnitName, trGetNextUnitScenarioNameNumber());
			spawnPlayerUnit(p, kbGetProtoUnitID("Dog"), pos);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("bellaselect1.wav","1",-1,"","");
			}
			xSetBool(dPlayerData, xPlayerPetDogReady, false, p);
		}
	} else if (2 * xGetInt(dPlayerData, xPlayerPetDogs, p) > trPlayerUnitCountSpecific(p, "Dog")) {
		xSetInt(dPlayerData, xPlayerPetDogNext, trTime() + 30, p);
		xSetBool(dPlayerData, xPlayerPetDogReady, true, p);
		if (trCurrentPlayer() == p) {
			trCounterAddTime("petDogs",30,1,"Pet Dog respawn",-1);
		}
	}
	if ((xGetInt(dPlayerData, xPlayerGodBoon, p) == BOON_MONSTER_COMPANION) && Multiplayer) {
		if (xGetBool(dPlayerData, xPlayerPetMonsterReady, p)) {
			if (trTime() > xGetInt(dPlayerData, xPlayerPetMonsterNext, p)) {
				pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
				xSetInt(dPlayerData, xPlayerMonsterName, trGetNextUnitScenarioNameNumber(), p);
				spawnPlayerUnit(p, xGetInt(dPlayerData,xPlayerMonsterProto,p), pos);
				if (trCurrentPlayer() == p) {
					trSoundPlayFN("mythcreate.wav","1",-1,"","");
				}
				xSetBool(dPlayerData, xPlayerPetMonsterReady, false, p);
			}
		} else {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData, xPlayerMonsterName, p), true);
			if (trUnitAlive() == false) {
				xSetInt(dPlayerData, xPlayerPetMonsterNext, trTime() + 30, p);
				xSetBool(dPlayerData, xPlayerPetMonsterReady, true, p);
				if (trCurrentPlayer() == p) {
					trCounterAddTime("petMonsters",30,1,"Pet Monster respawn",-1);
				}
			}
		}
	}
}

rule enable_chat
inactive
highFrequency
{
	xsDisableSelf();
	trChatSetStatus(true);
}

rule gameplay_start
inactive
highFrequency
{
	xsDisableSelf();
	trDelayedRuleActivation("gameplay_start_2");
	for(p=1; < ENEMY_PLAYER) {
		chooseClass(p, xGetInt(dPlayerData, xPlayerClass, p));
	}
	trMusicPlayCurrent();
	trPlayNextMusicTrack();
	trSetFogAndBlackmap(true, true);
	trPlayerResetBlackMapForAllPlayers();
	trQuestVarSet("play", 1);
}

rule gameplay_start_2
inactive
highFrequency
{
	xsDisableSelf();
	trEventSetHandler(999, "removeCamTracks");
	
	trCameraCut(vector(0,70.710701,0), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
	xsEnableRule("gameplay_always");
	if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") < 5) {
		trQuestVarSet("nickQuestProgressLocal", trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress"));
		trQuestVarSet("nickEquippedLocal", trQuestVarGet("p"+trCurrentPlayer()+"nickEquipped"));
		xsEnableRule("nick_dialog");
	}
	vector pos = trVectorQuestVarGet("startPosition");
	int db = 0;
	for(p=1; < ENEMY_PLAYER) {
		spawnPlayer(p, pos);
		trQuestVarSet("p"+p+"lureObject", trGetNextUnitScenarioNameNumber()-1);
		trQuestVarSet("p"+p+"wellObject", trGetNextUnitScenarioNameNumber()-1);
		if (trQuestVarGet("p"+p+"nickEquipped") == 1) {
			trQuestVarSet("p"+p+"nickEquipped", 0); // Set it to 0 because relicEffect will set it back to 1
			trQuestVarSet("p"+p+"relic12", RELIC_NICKONHAWK);
		}
		db = getRelicsDB(p);
		for(x=12; >0) {
			if (trQuestVarGet("p"+p+"relic"+x) > 0) {
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				xAddDatabaseBlock(db, true);
				xSetInt(db, xUnitName, 1*trQuestVarGet("next"));
				xSetInt(db, xRelicType, 1*trQuestVarGet("p"+p+"relic"+x));
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitChangeProtoUnit("Relic");
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerUnit, p));
				trMutateSelected(relicProto(1*trQuestVarGet("p"+p+"relic"+x)));
				trSetSelectedScale(0,0,-1);
				trUnitSetAnimationPath("1,0,1,1,0,0,0");
				relicEffect(1*trQuestVarGet("p"+p+"relic"+x), p, true);
			}
		}
		trSetCivilizationNameOverride(p, "Level " + (1+xGetInt(dPlayerData, xPlayerLevel, p)));
	}
	trQuestVarSet("nextProj", trGetNextUnitScenarioNameNumber());
	
	if (Multiplayer) {
		trSetUnitIdleProcessing(true);
	}
	/*
	TESTING STUFF BELOW THIS LINE
	*/
	
	if (Multiplayer) {
		pos = trVectorQuestVarGet("bossRoomCenter");
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, 1), true);
		trUnitTeleport(xsVectorGetX(pos),0,xsVectorGetZ(pos));
	}
	/*
	*/
}

rule gameplay_always
inactive
highFrequency
{
	int old = xsGetContextPlayer();
	int id = 0;
	int p = 0;
	int count = 0;
	int relic = 0;
	int db = 0;
	float amt = 0;
	bool relicReturned = true;
	
	vector pos = vector(0,0,0);
	
	if (xGetDatabaseCount(dPlayerUnits) > 0) {
		xDatabaseNext(dPlayerUnits);
		xUnitSelectByID(dPlayerUnits, xUnitID);
		if (trUnitAlive() == false) {
			removePlayerUnit();
		} else {
			if (xGetFloat(dPlayerUnits, xDecay) > 0) {
				count = trTimeMS() - xGetInt(dPlayerUnits, xDecayNext);
				if (count > 1000) {
					xSetInt(dPlayerUnits, xDecayNext, trTimeMS());
					trDamageUnitPercent(xGetFloat(dPlayerUnits, xDecay) * 0.001 * count);
				}
			}
			if (trQuestVarGet("stage") == 5) {
				if (trCountUnitsInArea(""+xGetInt(dPlayerUnits, xUnitName),ENEMY_PLAYER,"Victory Marker",4) > 0) {
					poisonUnit(dPlayerUnits, 10, 10);
				}
			}
			xSetVector(dPlayerUnits, xUnitPos, kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true));
			
			stunsAndPoisons(dPlayerUnits);
		}
	}
	
	enemiesAlways();
	specialUnitsAlways();
	
	maintainStun();
	
	playerLasers();
	
	processWolves();
	
	
	/* protection */
	if (trQuestVarGet("protectionCount") > 0) {
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			id = xGetInt(dPlayerUnits, xUnitID);
			xUnitSelectByID(dPlayerUnits, xUnitID);
			trUnitHighlight(0.2, false);
			xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner));
			amt = kbUnitGetCurrentHitpoints(id);
			xsSetContextPlayer(0);
			if (amt > xGetFloat(dPlayerUnits, xCurrentHealth)) {
				xSetFloat(dPlayerUnits, xCurrentHealth, amt);
			} else {
				trDamageUnit(amt - xGetFloat(dPlayerUnits, xCurrentHealth));
			}
		}
		if (trQuestVarGet("protectionWas") == 0) {
			trQuestVarSet("protectionWas", 1);
		}
	} else if (trQuestVarGet("protectionWas") == 1) {
		trQuestVarSet("protectionWas", 0);
	}
	
	/* relics dropped */
	trQuestVarSet("relicPlayer", 1 + trQuestVarGet("relicPlayer"));
	if (trQuestVarGet("relicPlayer") >= ENEMY_PLAYER) {
		trQuestVarSet("relicPlayer", 1);
	}
	trQuestVarSet("shopping", 0);
	p = trQuestVarGet("relicPlayer");
	xSetPointer(dPlayerData, p);
	if (xGetBool(dPlayerData, xPlayerResigned) == false) {
		xUnitSelect(dPlayerData, xPlayerUnit);
		if (trUnitAlive() && (xGetBool(dPlayerData, xPlayerLaunched) == false) && xGetInt(dPlayerData, xPlayerDead) <= 0) {
			pos = kbGetBlockPosition(""+xGetInt(dPlayerData,xPlayerUnit), true);
			db = getRelicsDB(p);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelect(db, xUnitName);
				if (trUnitGetIsContained("Unit") == false) {
					if (xGetInt(db, xRelicType) < KEY_RELICS) {
						relicReturned = false;
						if (distanceBetweenVectors(pos, trVectorQuestVarGet("relicTransporterGuyPos")) < 36) {
							relicReturned = true;
							if (trPlayerUnitCountSpecific(p, "Villager Atlantean Hero") == 0) {
								if (trPlayerResourceCount(p, "gold") >= 100) {
									trPlayerGrantResources(p, "gold", -100);
									trQuestVarSet("p"+p+"transporterPurchased", 1);
									spawnPlayerUnit(p, kbGetProtoUnitID("Villager Atlantean Hero"), pos);
									if (trCurrentPlayer() == p) {
										trChatSend(0, "A Relic Transporter has been hired!");
										trSoundPlayFN("favordump.wav","1",-1,"","");
										trSoundPlayFN("villagercreate.wav","1",-1,"","");
									}
								} else if (trCurrentPlayer() == p) {
									trChatSend(0, "You don't have enough gold! You need 100!");
									trSoundPlayFN("cantdothat.wav","1",-1,"","");
								}
							}
						} else if (unitDistanceToVector(1*trQuestVarGet("temple"), pos) < 36) {
							if (trQuestVarGet("templeChallengeActive") == 0) {
								trQuestVarSet("templeChallengeActive", 1);
								relicReturned = true;
							} else if (trQuestVarGet("stageTemple") == BOON_MORE_GOLD) {
								relicReturned = false;
								if ((trQuestVarGet("p"+p+"relicsSacrificed") < 10) && (xGetInt(db, xRelicType) <= NORMAL_RELICS)) {
									trUnitChangeProtoUnit("Conversion SFX");
									trQuestVarSet("p"+p+"relicsSacrificed", 1 + trQuestVarGet("p"+p+"relicsSacrificed"));
									if (trCurrentPlayer() == p) {
										trSoundPlayFN("favordump.wav","1",-1,"","");
										trChatSend(0, "<color=1,1,1>Relic sacrificed! ("+1*trQuestVarGet("p"+p+"relicsSacrificed")+"/10)</color>");
									}
								}
							}
						} else if (xGetInt(db, xRelicType) == RELIC_SPARK) {
							if (distanceBetweenVectors(pos, trVectorQuestVarGet("bossRoomCenter")) < 25.0) {
								relicReturned = false;
								xUnitSelect(db, xUnitName);
								trUnitChangeProtoUnit("Lightning Sparks Ground");
								trQuestVarSet("bossSmite", 1 + trQuestVarGet("bossSmite"));
							} else {
								debugLog("what's going on here!");
							}
						} else if (1*trQuestVarGet("nottud") > 0) {
							for(i=3; >0) {
								xDatabaseNext(dNottudShop);
								if (distanceBetweenVectors(pos, xGetVector(dNottudShop, xNottudShopPos)) < 25) {
									break;
								}
							}
							if (i > 0) {
								relicReturned = true;
								if (trQuestVarGet("shopping") == 0) {
									trQuestVarSet("shopping", 1);
									if (trPlayerResourceCount(p, "gold") >= 300) {
										relic = xGetInt(dNottudShop, xRelicType);
										trPlayerGrantResources(p, "gold", -300);
										if (trCurrentPlayer() == p) {
											trSoundPlayFN("favordump.wav","1",-1,"","");
											trChatSend(0, "Purchased " + relicName(relic) + "!");
											trChatSend(0, "The purchased relic has been added to your warehouse.");
											trQuestVarSet("ownedRelics"+relic, 1 + trQuestVarGet("ownedRelics"+relic));
										}
									} else if (trCurrentPlayer() == p) {
										trChatSend(0, "You don't have enough gold! You need 300!");
										trSoundPlayFN("cantdothat.wav","1",-1,"","");
									}
								}
							}
						} else if (trQuestVarGet("shopGuyActive") == 1) {
							if (distanceBetweenVectors(pos, trVectorQuestVarGet("shopGuyPos")) < 9) {
								relicReturned = true;
								if (trQuestVarGet("shopping") == 0) {
									trQuestVarSet("shopping", 1);
									if (trPlayerResourceCount(p, "gold") >= 200) {
										relic = trQuestVarGet("shopRelic");
										trPlayerGrantResources(p, "gold", -200);
										if (trCurrentPlayer() == p) {
											trSoundPlayFN("favordump.wav","1",-1,"","");
											trChatSend(0, "Purchased " + relicName(relic) + "!");
											trChatSend(0, "The purchased relic has been added to your warehouse.");
											trQuestVarSet("ownedRelics"+relic, 1 + trQuestVarGet("ownedRelics"+relic));
										}
									} else if (trCurrentPlayer() == p) {
										trChatSend(0, "You don't have enough gold! You need 200!");
										trSoundPlayFN("cantdothat.wav","1",-1,"","");
									}
								}
							}
						}
						if (relicReturned == false) {
							id = kbGetBlockID(""+xGetInt(db, xUnitName));
							if (trCurrentPlayer() == p) {
								trSoundPlayFN("backtowork.wav","1",-1,"","");
								trChatSend(0, relicName(xGetInt(db, xRelicType)) + " dropped.");
							}
							relicEffect(xGetInt(db, xRelicType), p, false);
							/* Nickonhawk's Quantum Slot Machine */
							if ((Multiplayer == false) &&
								(trQuestVarGet("p1nickQuestProgress") >= 5) &&
								(distanceBetweenVectors(pos, trVectorQuestVarGet("nickPos")) < 9) &&
								(xGetDatabaseCount(dSlotRelics) < 3)) {
								trSoundPlayFN("storehouse.wav","1",-1,"","");
								xUnitSelect(db, xUnitName);
								trUnitChangeProtoUnit("Conversion SFX");
								xAddDatabaseBlock(dSlotRelics, true);
								xSetInt(dSlotRelics, xUnitName, trGetNextUnitScenarioNameNumber());
								xSetInt(dSlotRelics, xRelicType, xGetInt(db, xRelicType));
								trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
								trUnitSelectClear();
								trUnitSelectByQV("pad"+xGetPointer(dSlotRelics));
								trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
								trUnitSelectClear();
								trUnitSelectByQV("next", true);
								trImmediateUnitGarrison(""+1*trQuestVarGet("pad"+xGetPointer(dSlotRelics)));
								trUnitChangeProtoUnit("Dwarf");
								trUnitSelectClear();
								trUnitSelectByQV("next", true);
								trUnitSetHeading(225);
								trUnitConvert(0);
								trUnitChangeProtoUnit("Relic");
								trUnitSelectClear();
								trUnitSelectByQV("pad"+xGetPointer(dSlotRelics));
								trMutateSelected(kbGetProtoUnitID("Statue of Automaton Base"));
								if (xGetDatabaseCount(dSlotRelics) == 3) {
									trUnitSelectClear();
									trUnitSelectByQV("nickonhawk", true);
									trUnitHighlight(3.0, true);
								}
							} else if (kbGetUnitBaseTypeID(id) == relicProto(xGetInt(db, xRelicType))) {
								trUnitChangeProtoUnit("Relic");
								xAddDatabaseBlock(dFreeRelics, true);
								xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
								xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
							}
							xFreeDatabaseBlock(db);
						} else {
							xUnitSelect(db, xUnitName);
							trUnitChangeProtoUnit("Relic");
							xUnitSelect(db, xUnitName);
							trImmediateUnitGarrison(""+xGetInt(dPlayerData, xPlayerUnit, p));
							trMutateSelected(relicProto(xGetInt(db, xRelicType)));
							trSetSelectedScale(0,0,-1);
							trUnitSetAnimationPath("1,0,1,1,0,0,0");
						}
					} else {
						/* KEY_RELICS */
						trSoundPlayFN("storehouse.wav","1",-1,"","");
						xFreeDatabaseBlock(db);
					}
				}
			}
		}
		
		relicTransporterGuy(p);
	}
	
	/* free relics */
	if (Multiplayer) {
		count = 5;
	} else {
		count = 30;
	}
	processFreeRelics(count);
	
	/* sounds */
	if (trQuestVarGet("stunSound") == 1) {
		trQuestVarSet("stunSound", 0);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("woodcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	
	if (trQuestVarGet("poisonSound") == 1) {
		trQuestVarSet("poisonSound", 0);
		trSoundPlayFN("lampadesblood.wav","1",-1,"","");
		trSoundPlayFN("carnivorabirth.wav","1",-1,"","");
	}
	
	/* misc */
	for(p=1; < ENEMY_PLAYER) {
		xSetPointer(dPlayerData, p);
		if (xGetBool(dPlayerData, xPlayerResigned) == false) {
			checkGodPowers(p);
			/* no gold cheating */
			checkResourceCheating(p);
			if (xGetInt(dPlayerData, xPlayerDead) <= 0) {
				xUnitSelect(dPlayerData, xPlayerUnit);
				if (Multiplayer) {
					if (trUnitIsSelected() && trCurrentPlayer() != p) {
						if (trQuestVarGet("ISelected"+p) == 0) {
							trQuestVarSet("iSelected"+p, 1);
							trChatSend(0, "<color=1,1,1><u>"+trStringQuestVarGet("p"+p+"name")+"</u></color>");
							if (xGetInt(dPlayerData, xPlayerGodBoon) > 0) {
								trChatSend(0, boonName(xGetInt(dPlayerData, xPlayerGodBoon)));
							}
							db = getRelicsDB(p);
							count = xGetPointer(db);
							for(x=xGetDatabaseCount(db); >0) {
								xDatabaseNext(db);
								trChatSend(0, relicName(xGetInt(db, xRelicType)));
							}
							xSetPointer(db, count);
							/* don't want no desync here */
						}
					} else if (trQuestVarGet("iSelected"+p) == 1) {
						trQuestVarSet("iSelected"+p, 0);
					}
					if (SAVIOR != xGetInt(dPlayerData, xPlayerClass)) {
						fixAnimations(p);
					}
				}
				processLifesteal(p);
				processSilence(p);
				processRegen(p);
				petDogs(p);
			} else if (trTimeMS() > xGetInt(dPlayerData, xPlayerReviveNext)) {
				count = 0;
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), trVectorQuestVarGet("dead"+p+"pos")) < 225) {
						count = count - 1;
					}
				}
				xSetInt(dPlayerData, xPlayerReviveNext, trTimeMS() + 1000);
				trQuestVarSet("playersReviving", 0);
				if (Multiplayer == false) {
					trQuestVarSet("playersReviving", 1);
					count = 1;
				} else {
					for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
						xDatabaseNext(dPlayerCharacters);
						xUnitSelectByID(dPlayerCharacters, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerCharacter();
						} else if (unitDistanceToVector(xGetInt(dPlayerCharacters, xUnitName), trVectorQuestVarGet("dead"+p+"pos")) < 100) {
							if (xGetInt(dPlayerData, xPlayerClass, xGetInt(dPlayerCharacters, xPlayerOwner)) == SUNBOW) {
								if (count < 0) {
									count = 0;
								}
							}
							count = count + 1;
							trQuestVarSet("playersReviving", 1);
						}
					}
				}
				if (trQuestVarGet("playersReviving") == 1) {
					if (count > 0) {
						xSetInt(dPlayerData, xPlayerDead, xGetInt(dPlayerData, xPlayerDead) - 1);
						trChatSend(0,
							"<color={Playercolor("+p+")}>{Playername("+p+")}</color> is being revived: " + xGetInt(dPlayerData, xPlayerDead));
					}
					if (xGetInt(dPlayerData, xPlayerDead) <= 0) {
						revivePlayer(p);
						xSetInt(dPlayerData, xPlayerRegenerateHealthLast, trTimeMS(), p);
						xSetInt(dPlayerData, xPlayerRegenerateFavorLast, trTimeMS(), p);
					}
				}
			}
			/* resign */
			if (kbIsPlayerResigned(p)) {
				xSetBool(dPlayerData, xPlayerResigned, true);
				trQuestVarSet("activePlayerCount", trQuestVarGet("activePlayerCount") - 1);
				db = getRelicsDB(p);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					if (xGetInt(db, xRelicType) > NORMAL_RELICS) {
						xAddDatabaseBlock(dFreeRelics, true);
						xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
						xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
						xUnitSelect(db, xUnitName);
						trUnitConvert(0);
						trUnitChangeProtoUnit("Relic");
					}
				}
				db = getWarehouseDB(p);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					if (xGetInt(db, xRelicType) > NORMAL_RELICS) {
						xAddDatabaseBlock(dFreeRelics, true);
						xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
						xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
						xUnitSelect(db, xUnitName);
						trUnitConvert(0);
						trUnitChangeProtoUnit("Relic");
					}
				}
				if (xGetInt(dPlayerData, xPlayerDead) > 0) {
					trQuestVarSet("deadPlayerCount", trQuestVarGet("deadPlayerCount") - 1);
				}
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					if (xGetInt(dPlayerUnits, xPlayerOwner) == p) {
						xUnitSelectByID(dPlayerUnits, xUnitID);
						trUnitChangeProtoUnit("Hero Death");
					}
				}
				if ((trCurrentPlayer() == p) && Multiplayer) {
					saveAllData();
				}
			} else {
				trEventFire(12*xGetInt(dPlayerData, xPlayerClass, p) + p);
			}
		}
	}
	
	/* sky passages */
	if (xGetDatabaseCount(dSkyPassages) > 0) {
		if (trQuestVarGet("portalsActive") == 1) {
			xDatabaseNext(dSkyPassages);
			pos = kbGetBlockPosition(""+xGetInt(dSkyPassages, xUnitName));
			trQuestVarSet("sound", 0);
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 6) {
					xAddDatabaseBlock(dMagicalJourney, true);
					xSetInt(dMagicalJourney, xUnitName, xGetInt(dPlayerUnits, xUnitID));
					if (xGetInt(dPlayerData, xPlayerUnit, trCurrentPlayer()) == xGetInt(dPlayerUnits, xUnitName)) {
						uiLookAtUnitByName(""+xGetInt(dSkyPassages, xSkyPassageExit));
						trQuestVarSet("sound", 1);
					}
				}
			}
		}
		
		if (xGetDatabaseCount(dMagicalJourney) > 0) {
			xUnitSelect(dSkyPassages, xSkyPassageExit);
			trUnitChangeProtoUnit("Dwarf");
			xUnitSelect(dSkyPassages, xSkyPassageExit);
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			for(x=xGetDatabaseCount(dMagicalJourney); >0) {
				xDatabaseNext(dMagicalJourney);
				xUnitSelectByID(dMagicalJourney, xUnitName);
				trImmediateUnitGarrison(""+xGetInt(dSkyPassages, xSkyPassageExit));
			}
			xClearDatabase(dMagicalJourney);
			xUnitSelect(dSkyPassages, xSkyPassageExit);
			trUnitEjectContained();
			trUnitChangeProtoUnit("Spy Eye");
			xUnitSelect(dSkyPassages, xSkyPassageExit);
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			trSetSelectedScale(0, 0.2, 0);
		}
		if (trQuestVarGet("sound") == 1) {
			trSoundPlayFN("skypassageout.wav","1",-1,"","");
		}
	}
	
	for(x=xsMin(3, xGetDatabaseCount(dLaunchedUnits)); >0) {
		processLaunchedUnit();
	}
	
	processChests();
	processWalls();
	
	/* GAME OVER */
	if (trQuestVarGet("deadPlayerCount") == trQuestVarGet("activePlayerCount") && Multiplayer &&
		trQuestVarGet("activePlayerCount") > 0) {
		trQuestVarSet("play", 0);
		xsDisableSelf();
		xsDisableRule("boss"+1*trQuestVarGet("stage")+"_battle");
		trSoundPlayFN("lose.wav","1",-1,"","");
		trUIFadeToColor(0,0,0,1500,0,true);
		trLetterBox(true);
		trQuestVarSet("gameOverNext", trTime() + 2);
		xsEnableRule("game_over");
		if (trQuestVarGet("newPlayers") == 0) {
			trQuestVarSet("gameOverStep", 6);
		}
	}
	
	/* No specials */
	noSpecials();
}

rule game_over
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("gameOverNext")) {
		trQuestVarSet("gameOverStep", 1 + trQuestVarGet("gameOverStep"));
		switch(1*trQuestVarGet("gameOverStep"))
		{
			case 1:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia: Your journey is not over! This map remembers your progress!",
					"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverNext", trTime() + 4);
			}
			case 2:
			{
				trSoundPlayFN("default", "1",-1,
					"Zenophobia: Your level, gold, and relics have been saved! The next time you play, you will keep them!",
					"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverNext", trTime() + 6);
			}
			case 3:
			{
				trSoundPlayFN("default", "1",-1,
					"Zenophobia: Be sure to also play this map in singleplayer to switch classes, level up, and more!",
					"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverNext", trTime() + 6);
			}
			case 4:
			{
				trSoundPlayFN("default", "1",-1,
					"Zenophobia: That's Singleplayer -> Random Map -> Ascension MMORPG",
					"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverNext", trTime() + 6);
			}
			case 5:
			{
				trQuestVarSet("gameOverNext", trTime() + 1);
				trUIFadeToColor(0,0,0,1000,0,false);
				trLetterBox(false);
				saveAllData();
			}
			case 6:
			{
				xsDisableSelf();
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("playersWon") == 1) {
						trSetPlayerWon(p);
					} else {
						trSetPlayerDefeated(p);
					}
				}
				trEndGame();
			}
			case 7:
			{
				trSoundPlayFN("default", "1",-1,"Zenophobia:" + trStringQuestVarGet("advice"),"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverStep", 4);
				trQuestVarSet("gameOverNext", trTime() + 5);
			}
			case 8:
			{
				trSoundPlayFN("","1",-1,"Yeebaagooon: That'll teach you to steal from me.",
					"icons\special e son of osiris icon 64");
				trQuestVarSet("gameOverNext", trTime() + 5);
			}
			case 9:
			{
				if (trQuestVarGet("p"+trCurrentPlayer()+"yeebHit") == 1) {
					trSoundPlayFN("","1",-1,":Yeebaagooon has taken back his stolen relic!","");
				}
				trQuestVarSet("gameOverStep", 4);
				trQuestVarSet("gameOverNext", trTime() + 5);
			}
		}
	}
}

rule deep_village_always
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("deepDeployNext")) {
		trUnitSelectClear();
		trUnitSelectByQV("stageWonder");
		if ((trUnitAlive() == false) || (trQuestVarGet("play") == 0)) {
			xsDisableSelf();
			trMessageSetText("The Palace of the Deep has been destroyed!",-1);
			trUnitOverrideAnimation(6,0,false,true,-1);
		} else if (xGetDatabaseCount(dPlayerUnits) > 0) {
			int x = 0;
			int z = 0;
			trQuestVarSet("deepDeployNext", trTime() + 60);
			vector dir = vector(-13,0,-13);
			int heading = 45;
			for(i=4; >0) {
				xDatabaseNext(dPlayerUnits);
				vector dest = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
				x = trQuestVarGet("deepDeployCenterX") + xsVectorGetX(dir);
				z = trQuestVarGet("deepDeployCenterZ") + xsVectorGetZ(dir);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x,0,z,heading,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Nereid");
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitMoveToPoint(xsVectorGetX(dest),0,xsVectorGetZ(dest),-1,true);
				activateEnemy(1*trQuestVarGet("next"));
				
				dir = rotationMatrix(dir, 0, -1);
				heading = heading + 90;
			}
		}
	}
}

rule the_deep_damage
inactive
highFrequency
{
	int index = xGetPointer(dPlayerUnits);
	xSetPointer(dPlayerUnits, 1*trQuestVarGet("deepPlayerPointer"));
	xDatabaseNext(dPlayerUnits);
	trQuestVarSet("deepPlayerPointer", xGetPointer(dPlayerUnits));
	bool found = false;
	float amt = trTimeMS() - xGetInt(dPlayerUnits, xDeepDamageLast);
	if (amt > 500) {
		xSetInt(dPlayerUnits, xDeepDamageLast, trTimeMS() + 500);
		for(p=1; < ENEMY_PLAYER) {
			if (trCountUnitsInArea(""+xGetInt(dPlayerUnits, xUnitName),p,"Flying Medic", 25) > 0) {
				found = true;
				break;
			}
		}
		if (found == false) {
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			xUnitSelectByID(dPlayerUnits, xUnitID);
			trDamageUnit(0.2 * amt);
		}
	}
	
	xSetPointer(dPlayerUnits, index);
}

rule the_cloud_damage
inactive
highFrequency
{
	vector pos = vector(0,0,0);
	int hit = 0;
	float amt = trTimeMS() - xGetInt(dPlayerUnits, xDeepDamageLast);
	if (amt > 500) {
		xSetInt(dPlayerUnits, xDeepDamageLast, trTimeMS() + 500);
		hit = trCountUnitsInArea(""+xGetInt(dPlayerUnits,xUnitName),0,"Invisible Target",6);
		
		if (hit > 0) {
			trQuestVarSetFromRand("sound", 1, 4, true);
			trSoundPlayFN("swordonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			xUnitSelectByID(dPlayerUnits, xUnitID);
			damagePlayerUnit(amt * hit * 0.5);
		}
	}
	trUnitSelectClear();
	trUnitSelectByQV("stageWonder");
	if (trUnitAlive() == false) {
		int db = trQuestVarGet("cloudTornados");
		for(i=0; < 10) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,db,i));
			trUnitDestroy();
		}
		trMessageSetText("The Sky Palace has been destroyed! Tornados dispelled!", -1);
		xsDisableSelf();
	}
	for (i=xGetDatabaseCount(dCloudDeployStars); >0) {
		if (PROJ_GROUND == processGenericProj(dCloudDeployStars)) {
			trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("cloudDeployProto")));
			xUnitSelectByID(dCloudDeployStars, xUnitID);
			trDamageUnitPercent(-100);
			activateEnemy(xGetInt(dCloudDeployStars, xUnitName));
			xFreeDatabaseBlock(dCloudDeployStars);
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("vortexland"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	switch(1*trQuestVarGet("cloudDeployStep"))
	{
		case 0:
		{
			if (trTime() > trQuestVarGet("cloudDeployNext")) {
				for(i=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelectByID(dPlayerCharacters, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						trVectorQuestVarSet("cloudDeployCenter", kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
						break;
					}
				}
				pos = trVectorQuestVarGet("cloudDeployCenter");
				trQuestVarSet("cloudDeployStep", 1);
				trQuestVarSet("cloudDeployLanding", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Vortex Landing");
				trQuestVarSet("cloudDeployNext", trTimeMS() + 1500);
				trSoundPlayFN("vortexstart.wav","1",-1,"","");
			}
		}
		case 1:
		{
			if (trTimeMS() > trQuestVarGet("cloudDeployNext")) {
				trQuestVarSetFromRand("cloudDeployCount", 3, 6, true);
				trQuestVarSetFromRand("cloudDeployProto", 1, 6, true);
				trQuestVarSet("cloudDeployStep", 2);
			}
		}
		case 2:
		{
			if (trTimeMS() > trQuestVarGet("cloudDeployNext")) {
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				trVectorQuestVarSet("cloudDeployDir", rotationMatrix(trVectorQuestVarGet("cloudDeployDir"), -0.757323, 0.653041));
				trQuestVarSetFromRand("dist", 2.0, 10.0, false);
				vector dir = trVectorQuestVarGet("cloudDeployDir");
				pos = trVectorQuestVarGet("cloudDeployCenter");
				pos = xsVectorSet(xsVectorGetX(pos) - trQuestVarGet("dist") * xsVectorGetX(dir),0,
					xsVectorGetZ(pos) - trQuestVarGet("dist") * xsVectorGetZ(dir));
				addGenericProj(dCloudDeployStars,pos,dir);
				trQuestVarSet("cloudDeployCount",trQuestVarGet("cloudDeployCount") - 1);
				if (trQuestVarGet("cloudDeployCount") == 0) {
					trQuestVarSet("cloudDeployStep", 3);
				} else {
					trQuestVarSet("cloudDeployNext", trQuestVarGet("cloudDeployNext") + 500);
				}
			}
		}
		case 3:
		{
			if (xGetDatabaseCount(dCloudDeployStars) == 0) {
				trQuestVarSet("cloudDeployStep", 0);
				trUnitSelectClear();
				trUnitSelectByQV("cloudDeployLanding");
				trUnitDestroy();
				trQuestVarSetFromRand("rand", 60, 90, true);
				trQuestVarSet("cloudDeployNext", trTime() + trQuestVarGet("rand"));
			}
		}
	}
}

rule the_pit_damage
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
		int id = 0;
		for(i=trGetNextUnitScenarioNameNumber() - 1; > pitLatest) {
			id = kbGetBlockID(""+i, true);
			switch(kbGetUnitBaseTypeID(id))
			{
				case kbGetProtoUnitID("Tartarian Gate"):
				{
					xAddDatabaseBlock(dPitGates, true);
					xSetInt(dPitGates, xUnitName, i);
					trQuestVarSetFromRand("angle", 0, 6.283185, false);
					trUnitSelectClear();
					trUnitSelectByID(id);
					trSetUnitOrientation(xsVectorSet(xsCos(trQuestVarGet("angle")),0,xsSin(trQuestVarGet("angle"))), vector(0,1,0), true);
				}
				case kbGetProtoUnitID("Tartarian Gate Spawn"):
				{
					trUnitSelectClear();
					trUnitSelectByID(id);
					if (trUnitIsOwnedBy(0)) {
						trSetSelectedScale(0.5,0.5,0.5);
						xAddDatabaseBlock(dPitSpawn, true);
						xSetInt(dPitSpawn, xUnitName, i);
						xSetInt(dPitSpawn, xPitSpawnNext, trTimeMS() + 500);
					}
				}
				case kbGetProtoUnitID("Tartarian Gate birth"):
				{
					trQuestVarSet("noGates", 0);
				}
			}
		}
		pitLatest = trGetNextUnitScenarioNameNumber() - 1;

		if (xGetDatabaseCount(dPitSpawn) >0) {
			xDatabaseNext(dPitSpawn);
			if (trTimeMS() > xGetInt(dPitSpawn, xPitSpawnNext)) {
				if (xsVectorGetY(kbGetBlockPosition(""+xGetInt(dPitSpawn, xUnitName), true)) <= worldHeight) {
					xUnitSelect(dPitSpawn, xUnitName);
					trUnitConvert(ENEMY_PLAYER);
					trQuestVarSetFromRand("rand", 1, trQuestVarGet("enemyProtoCount"), true);
					trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("rand")));
					activateEnemy(xGetInt(dPitSpawn, xUnitName));
					xFreeDatabaseBlock(dPitSpawn);
				}
			}
		}

		if (trTime() > trQuestVarGet("pitDamageNext")) {
			trQuestVarSet("pitDamageNext", trTime());
			
			vector pos = vector(0,0,0);
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else {
					trDamageUnit(10);
				}
			}
			
			trUnitSelectClear();
			trUnitSelectByQV("stageWonder");
			if (trUnitAlive() && (boss == 0) && ((trTime() > trQuestVarGet("pitDeployNext")) || (trQuestVarGet("noGates") == 1))) {
				trQuestVarSetFromRand("rand", 30, 90, true);
				trQuestVarSet("pitDeployNext", trTime() + trQuestVarGet("rand"));

				for (i=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelect(dPlayerCharacters, xUnitName);
					if (trUnitAlive()) {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
						break;
					} else {
						xFreeDatabaseBlock(dPlayerCharacters);
					}
				}

				trQuestVarSetFromRand("modx", -10, 10, true);
				trQuestVarSetFromRand("modz", -10, 10, true);
				pos = xsVectorSet(1 * (xsVectorGetX(pos) - 10) / 70, 0, 1 * (xsVectorGetZ(pos) - 10) / 70);
				pos = pos * 70 + vector(39,0,39) + xsVectorSet(trQuestVarGet("modx"), 0, trQuestVarGet("modz")); // center of whatever room we're standing in

				trPlayerKillAllGodPowers(ENEMY_PLAYER);
				trTechGodPower(ENEMY_PLAYER, "tartarian gate", 1);
				trUnitSelectClear();
				trTechInvokeGodPower(ENEMY_PLAYER, "tartarian gate", pos, vector(0,0,0));

				trQuestVarSet("noGates", 1);
			}

			for(i=xGetDatabaseCount(dPitGates); >0) {
				xDatabaseNext(dPitGates);
				xUnitSelect(dPitGates, xUnitName);
				trDamageUnitPercent(5);
			}
		}
	}
}

rule devil_do1_find
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("devil_do1");
	for(p=1; < ENEMY_PLAYER) {
		if (trUnitHasLOS(p)) {
			trUnitHighlight(5.0, true);
			uiLookAtUnitByName(""+1*trQuestVarGet("devil_do1"));
			trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: Pursuers from the Guild? Just try and catch me!");
			trCounterAddTime("devilEscape",150,0,"devil_do1 escapes",-1);
			trQuestVarSet("devilEscapeTime", trTime() + 150);
			trQuestVarSet("devilNextSummon", trTime() - 10 * trQuestVarGet("stage"));
			xsEnableRule("devil_do1_battle");
			xsDisableSelf();
			break;
		}
	}
}

rule devil_do1_battle
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("devilNextSummon")) {
		trSoundPlayFN("mythcreate.wav","1",-1,"","");
		trQuestVarSet("devilNextSummon", trQuestVarGet("devilNextSummon") + 10);
		trQuestVarSetFromRand("rand", 1, 19, true);
		int proto = monsterPetProto(1*trQuestVarGet("rand"));
		vector pos = kbGetBlockPosition(""+1*trQuestVarGet("devil_do1"));
		trQuestVarSetFromRand("heading", 1, 360, true);
		int next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+ENEMY_PLAYER+",0",kbGetProtoUnitName(proto),1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
		activateEnemy(next);
		trQuestVarSetFromRand("rand", 1, 10);
		switch(1*trQuestVarGet("rand"))
		{
			case 1:
			{
				trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: Mwuahaha! This is fun!");
			}
			case 2:
			{
				trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: Go forth, my pretties!");
			}
			case 3:
			{
				trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: Behold the power of the Monsterpedia!");
			}
		}
	}

	trUnitSelectClear();
	trUnitSelectByQV("devil_do1");
	if (trUnitAlive() == false) {
		trCounterAbort("devilEscape");
		trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: Noooo! How can this be?!");
		xsDisableSelf();
		trUnitChangeProtoUnit("Lampades Blood");
		if (trQuestVarGet("p"+trCurrentPlayer()+"monsterpediaQuest") == 2) {
			startNPCDialog(NPC_DEVIL_DIE);
			trQuestVarSet("monsterpediaQuestComplete", 1);
		}
	} else if (trTime() > trQuestVarGet("devilEscapeTime")) {
		xsDisableSelf();
		trSoundPlayFN("cantdothat.wav","1",-1,"","");
		trSoundPlayFN("vortexstart.wav","1",-1,"","");
		trChatSendSpoofed(ENEMY_PLAYER, "devil_do1: So long, suckers!");
		trUnitChangeProtoUnit("Dust Large");
	}
}