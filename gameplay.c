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
		}
	}
}

void processSilence(int p = 0) {
	if (xGetBool(dPlayerData, xPlayerSilenced, p) && (xGetInt(dPlayerData, xPlayerDead, p) == 0)) {
		if (xGetInt(dPlayerUnits, xSilenceStatus, xGetInt(dPlayerData, xPlayerIndex)) == 0) {
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
			amt = diff * 0.00003 * xGetFloat(dPlayerData, xPlayerHealth, p);
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
	if (trPlayerResourceCount(p, "favor") > 1 + xGetFloat(dPlayerData, xPlayerFavor)) {
		gainFavor(p, 0);
		if (trCurrentPlayer() == p) {
			trChatSendSpoofed(0, "Zenophobia: Did you really think I wouldn't catch that?");
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
			if (trPlayerUnitCountSpecific(p, "Animal Attractor") == 2) {
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
			if ((xGetInt(dStunnedUnits, xUnitName) == bossUnit) ||
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
	int db = trQuestVarGet("p"+p+"warehouse");
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if ((trUnitGetIsContained("Villager Atlantean Hero") || trUnitGetIsContained("Cinematic Block")) == false) {
			if (xGetInt(db, xRelicType) < KEY_RELICS ||
				trPlayerUnitCountSpecific(p, "Villager Atlantean Hero") == 0) {
				if (kbGetUnitBaseTypeID(id) == relicProto(xGetInt(db, xRelicType))) {
					trUnitChangeProtoUnit("Relic");
					xAddDatabaseBlock(dFreeRelics, true);
					xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
					xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
				}
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
							db = trQuestVarGet("p"+p+"warehouse");
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
					db = trQuestVarGet("p"+p+"relics");
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
	if (trQuestVarGet("p"+p+"godBoon") == BOON_MONSTER_COMPANION) {
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
		chooseClass(p, xGetInt(dPlayerData xPlayerClass, p));
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
		db = trQuestVarGet("p"+p+"relics");
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
	*
	
	if (Multiplayer) {
		trUnitSelectClear();
		trUnitSelectByQV("p1unit");
		trUnitTeleport(trQuestVarGet("bossRoomCenterX")-10,0,trQuestVarGet("bossRoomCenterZ")-10);
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
	float amt = 0;
	bool relicReturned = true;
	
	if (xGetDatabaseCount(dPlayerUnits) > 0) {
		id = yDatabaseNext("playerUnits", true);
		if ((id == -1) || (trUnitAlive() == false)) {
			removePlayerUnit();
		} else {
			if (yGetVar("playerUnits", "decay") > 0) {
				if (trTimeMS() > yGetVar("playerUnits", "decayNext")) {
					ySetVar("playerUnits", "decayNext", 1000 + yGetVar("playerUnits", "decayNext"));
					trDamageUnitPercent(yGetVar("playerUnits", "decay"));
				}
			}
			if (trCountUnitsInArea(""+1*trQuestVarGet("playerUnits"),ENEMY_PLAYER,"Victory Marker",4) > 0) {
				poisonUnit("playerUnits", 10, 10);
			}
			trVectorSetUnitPos("pos", "playerUnits");
			ySetVarFromVector("playerUnits", "pos", "pos");
			
			stunsAndPoisons("playerUnits");
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
			id = yDatabaseNext("playerUnits", true);
			trUnitHighlight(0.2, false);
			xsSetContextPlayer(1*yGetVar("playerUnits", "player"));
			amt = kbUnitGetCurrentHitpoints(id);
			if (amt > yGetVar("playerUnits", "currentHealth")) {
				ySetVar("playerUnits", "currentHealth", amt);
			} else {
				trDamageUnit(amt - yGetVar("playerUnits", "currentHealth"));
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
	if (trQuestVarGet("p"+p+"resigned") == 0) {
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"unit");
		if (trUnitAlive() && trQuestVarGet("p"+p+"launched") == 0 && trQuestVarGet("p"+p+"dead") <= 0) {
			trVectorSetUnitPos("pos", "p"+p+"unit");
			for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
				id = yDatabaseNext("p"+p+"relics", true);
				if (trUnitGetIsContained("Unit") == false) {
					if (yGetVar("p"+p+"relics", "type") < KEY_RELICS) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"relics");
						relicReturned = false;
						if (zDistanceBetweenVectorsSquared("pos", "relicTransporterGuyPos") < 36) {
							relicReturned = true;
							if (trPlayerUnitCountSpecific(p, "Villager Atlantean Hero") == 0) {
								if (trPlayerResourceCount(p, "gold") >= 100) {
									trPlayerGrantResources(p, "gold", -100);
									trQuestVarSet("p"+p+"transporterPurchased", 1);
									trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
									yAddToDatabase("playerUnits", "next");
									yAddUpdateVar("playerUnits", "player", p);
									trArmyDispatch(""+p+",0","Villager Atlantean Hero",1,
										trQuestVarGet("posx"),0,trQuestVarGet("posz"),180,true);
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
						} else if (zDistanceToVectorSquared("temple", "pos") < 36) {
							if (trQuestVarGet("templeChallengeActive") == 0) {
								trQuestVarSet("templeChallengeActive", 1);
								relicReturned = true;
							} else if (trQuestVarGet("stageTemple") == BOON_MORE_GOLD) {
								relicReturned = false;
								if ((trQuestVarGet("p"+p+"relicsSacrificed") < 10) && (yGetVar("p"+p+"relics","type") <= NORMAL_RELICS)) {
									trUnitChangeProtoUnit("Conversion SFX");
									trQuestVarSet("p"+p+"relicsSacrificed", 1 + trQuestVarGet("p"+p+"relicsSacrificed"));
									if (trCurrentPlayer() == p) {
										trSoundPlayFN("favordump.wav","1",-1,"","");
										trChatSend(0, "<color=1,1,1>Relic sacrificed! ("+1*trQuestVarGet("p"+p+"relicsSacrificed")+"/10)</color>");
									}
								}
							}
						} else if (1*trQuestVarGet("nottud") > 0) {
							for(i=3; >0) {
								yDatabaseNext("nottudShop");
								yVarToVector("nottudShop", "shopPos");
								if (zDistanceBetweenVectorsSquared("pos", "shopPos") < 25) {
									break;
								}
							}
							if (i > 0) {
								relicReturned = true;
								if (trQuestVarGet("shopping") == 0) {
									trQuestVarSet("shopping", 1);
									if (trPlayerResourceCount(p, "gold") >= 300) {
										relic = yGetVar("nottudShop", "relic");
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
							if (zDistanceBetweenVectorsSquared("pos", "shopGuyPos") < 9) {
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
						} else if (yGetVar("p"+p+"relics","type") == RELIC_SPARK) {
							if (zDistanceBetweenVectorsSquared("pos", "bossRoomCenter") < 16.0) {
								relicReturned = false;
								trUnitSelectClear();
								trUnitSelectByQV("p"+p+"relics");
								trUnitChangeProtoUnit("Lightning Sparks Ground");
								trQuestVarSet("bossSmite", 1 + trQuestVarGet("bossSmite"));
							}
						}
						if (relicReturned == false) {
							if (trCurrentPlayer() == p) {
								trSoundPlayFN("backtowork.wav","1",-1,"","");
								trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")) + " dropped.");
							}
							relicEffect(1*yGetVar("p"+p+"relics", "type"), p, false);
							/* Nickonhawk's Quantum Slot Machine */
							if ((Multiplayer == false) &&
								(trQuestVarGet("p1nickQuestProgress") >= 5) &&
								(zDistanceBetweenVectorsSquared("pos", "nickPos") < 9) &&
								(yGetDatabaseCount("slotRelics") < 3)) {
								trSoundPlayFN("storehouse.wav","1",-1,"","");
								trUnitSelectClear();
								trUnitSelectByQV("p"+p+"relics", true);
								trUnitChangeProtoUnit("Conversion SFX");
								trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
								yAddToDatabase("slotRelics", "next");
								yAddUpdateVar("slotRelics", "type", yGetVar("p"+p+"relics", "type"));
								ySetPointer("slotRelics", yGetNewestPointer("slotRelics"));
								if (yGetVar("slotRelics", "pad") == 0) {
									trQuestVarSet("nextPad", 1 + trQuestVarGet("nextPad"));
									ySetVar("slotRelics", "pad", trQuestVarGet("pad"+1*trQuestVarGet("nextPad")));
								}
								trUnitSelectClear();
								trUnitSelect(""+1*yGetVar("slotRelics", "pad"));
								trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
								trUnitSelectClear();
								trUnitSelectByQV("next", true);
								trImmediateUnitGarrison(""+1*yGetVar("slotRelics", "pad"));
								trUnitChangeProtoUnit("Dwarf");
								trUnitSelectClear();
								trUnitSelectByQV("next", true);
								trUnitSetHeading(225);
								trUnitConvert(0);
								trUnitChangeProtoUnit("Relic");
								trUnitSelectClear();
								trUnitSelect(""+1*yGetVar("slotRelics", "pad"));
								trMutateSelected(kbGetProtoUnitID("Statue of Automaton Base"));
								if (yGetDatabaseCount("slotRelics") == 3) {
									trUnitSelectClear();
									trUnitSelectByQV("nickonhawk", true);
									trUnitHighlight(3.0, true);
								}
							} else if (kbGetUnitBaseTypeID(id) == relicProto(1*yGetVar("p"+p+"relics", "type"))) {
								trUnitChangeProtoUnit("Relic");
								yAddToDatabase("freeRelics", "p"+p+"relics");
								yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"relics", "type"));
							}
							yRemoveFromDatabase("p"+p+"relics");
							yRemoveUpdateVar("p"+p+"relics", "type");
						} else {
							trUnitSelectClear();
							trUnitSelectByQV("p"+p+"relics", true);
							trUnitChangeProtoUnit("Relic");
							trUnitSelectClear();
							trUnitSelectByQV("p"+p+"relics", true);
							trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
							trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
							trSetSelectedScale(0,0,-1);
							trUnitSetAnimationPath("1,0,1,1,0,0,0");
						}
					} else {
						/* KEY_RELICS */
						trSoundPlayFN("storehouse.wav","1",-1,"","");
						yRemoveFromDatabase("p"+p+"relics");
						yRemoveUpdateVar("p"+p+"relics", "type");
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
		if (trQuestVarGet("p"+p+"resigned") == 0) {
			checkGodPowers(p);
			/* no gold cheating */
			checkResourceCheating(p);
			if (trQuestVarGet("p"+p+"dead") == 0) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"unit");
				if (Multiplayer) {
					if (trUnitIsSelected() && trCurrentPlayer() != p) {
						if (trQuestVarGet("ISelected"+p) == 0) {
							trQuestVarSet("iSelected"+p, 1);
							trChatSend(0, "<color=1,1,1><u>"+trStringQuestVarGet("p"+p+"name")+"</u></color>");
							if (trQuestVarGet("p"+p+"godBoon") > 0) {
								trChatSend(0, boonName(1*trQuestVarGet("p"+p+"godBoon")));
							}
							count = yGetPointer("p"+p+"relics");
							for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
								yDatabaseNext("p"+p+"relics");
								trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")));
							}
							ySetPointer("p"+p+"relics", count);
							/* don't want no desync here */
						}
					} else if (trQuestVarGet("iSelected"+p) == 1) {
						trQuestVarSet("iSelected"+p, 0);
					}
					if (SAVIOR != trQuestVarGet("p"+p+"class")) {
						fixAnimations(p);
					}
				}
				processLifesteal(p);
				processSilence(p);
				processRegen(p);
				petDogs(p);
			} else if (trTimeMS() > trQuestVarGet("p"+p+"reviveNext")) {
				count = 0;
				for(x=xGetDatabaseCount(dEnemies); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "dead"+p+"pos") < 225) {
						count = count - 1;
					}
				}
				trQuestVarSet("p"+p+"reviveNext", trTimeMS() + 1000);
				trQuestVarSet("playersReviving", 0);
				for(x=yGetDatabaseCount("playerCharacters"); >0) {
					id = yDatabaseNext("playerCharacters", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerCharacter();
					} else if (zDistanceToVectorSquared("playerCharacters", "dead"+p+"pos") < 100) {
						if (trQuestVarGet("p"+1*yGetVar("playerCharacters", "player")+"class") == SUNBOW) {
							if (count < 0) {
								count = 0;
							}
						}
						count = count + 1;
						trQuestVarSet("playersReviving", 1);
					}
				}
				if (Multiplayer == false) {
					trQuestVarSet("playersReviving", 1);
					count = 1;
				}
				if (trQuestVarGet("playersReviving") == 1) {
					if (count <= 0) {
						trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> can't be revived. Too many enemies nearby!");
					} else {
						trQuestVarSet("p"+p+"dead", xsMax(0, trQuestVarGet("p"+p+"dead") - count));
						trChatSend(0,
							"<color={Playercolor("+p+")}>{Playername("+p+")}</color> is being revived: " + 1*trQuestVarGet("p"+p+"dead"));
					}
					if (trQuestVarGet("p"+p+"dead") <= 0) {
						revivePlayer(p);
					}
				}
			}
			/* resign */
			if (kbIsPlayerResigned(p)) {
				trQuestVarSet("p"+p+"resigned", 1);
				trQuestVarSet("activePlayerCount", trQuestVarGet("activePlayerCount") - 1);
				for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
					yDatabaseNext("p"+p+"relics", true);
					if (yGetVar("p"+p+"relics", "type") > KEY_RELICS) {
						yAddToDatabase("freeRelics", "p"+p+"relics");
						yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"relics", "type"));
						trUnitConvert(0);
						trUnitChangeProtoUnit("Relic");
					}
				}
				for(x=yGetDatabaseCount("p"+p+"warehouse"); >0) {
					yDatabaseNext("p"+p+"warehouse", true);
					if (yGetVar("p"+p+"warehouse", "type") > KEY_RELICS) {
						yAddToDatabase("freeRelics", "p"+p+"warehouse");
						yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"warehouse", "type"));
						trUnitConvert(0);
						trUnitChangeProtoUnit("Relic");
					}
				}
				if (trQuestVarGet("p"+p+"dead") > 0) {
					trQuestVarSet("deadPlayerCount", trQuestVarGet("deadPlayerCount") - 1);
				}
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					yDatabaseNext("playerUnits", true);
					if (yGetVar("playerUnits", "player") == p) {
						trUnitChangeProtoUnit("Hero Death");
					}
				}
				if ((trCurrentPlayer() == p) && Multiplayer) {
					saveAllData();
				}
			} else {
				trEventFire(12*trQuestVarGet("p"+p+"class") + p);
			}
		}
	}
	
	/* sky passages */
	if (yGetDatabaseCount("skyPassages") > 0) {
		if (trQuestVarGet("portalsActive") == 1) {
			yDatabaseNext("skyPassages");
			trVectorSetUnitPos("pos", "skyPassages");
			trQuestVarSet("sound", 0);
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				if (zDistanceToVectorSquared("playerUnits", "pos") < 6) {
					yAddToDatabase("magicalJourney", "playerUnits");
					if (yGetVar("playerUnits", "hero") == 1) {
						if (trCurrentPlayer() == yGetVar("playerUnits", "player")) {
							uiLookAtUnitByName(""+1*yGetVar("skyPassages", "exit"));
							trQuestVarSet("sound", 1);
						}
					}
				}
			}
		}
		
		if (yGetDatabaseCount("magicalJourney") > 0) {
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
			trUnitChangeProtoUnit("Dwarf");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			for(x=yGetDatabaseCount("magicalJourney"); >0) {
				yDatabaseNext("magicalJourney", true);
				trImmediateUnitGarrison(""+1*yGetVar("skyPassages", "exit"));
			}
			yClearDatabase("magicalJourney");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
			trUnitEjectContained();
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			trSetSelectedScale(0, 0.2, 0);
		}
		if (trQuestVarGet("sound") == 1) {
			trSoundPlayFN("skypassageout.wav","1",-1,"","");
		}
	}
	
	for(x=xsMin(5, yGetDatabaseCount("launchedUnits")); >0) {
		processLaunchedUnit();
	}
	
	processChests();
	processWalls();
	
	xsSetContextPlayer(old);
	
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
			trUnitOverrideAnimation(-1,0,true,true,-1);
		} else if (xGetDatabaseCount(dPlayerUnits) > 0) {
			int x = 0;
			int z = 0;
			trQuestVarSet("deepDeployNext", trTime() + 60);
			trVectorQuestVarSet("dir",vector(-13,0,-13));
			int heading = 45;
			for(i=4; >0) {
				xDatabaseNext(dPlayerUnits);
				trVectorSetUnitPos("dest", "playerUnits");
				x = trQuestVarGet("deepDeployCenterX") + trQuestVarGet("dirX");
				z = trQuestVarGet("deepDeployCenterZ") + trQuestVarGet("dirZ");
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x,0,z,heading,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Nereid");
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitMoveToPoint(trQuestVarGet("destx"),0,trQuestVarGet("destz"),-1,true);
				activateEnemy("next");
				
				trVectorQuestVarSet("dir", rotationMatrix("dir", 0, -1));
				heading = heading + 90;
			}
		}
	}
}

rule the_deep_damage
inactive
highFrequency
{
	bool found = false;
	float amt = trTimeMS() - yGetVar("playerUnits", "deepLast");
	if (amt > 500) {
		ySetVar("playerUnits","deepLast", trTimeMS() + 500);
		trUnitSelectClear();
		trUnitSelectByQV("playerUnits");
		for(p=1; < ENEMY_PLAYER) {
			if (trCountUnitsInArea(""+1*trQuestVarGet("playerUnits"),p,"Flying Medic", 25) > 0) {
				found = true;
				break;
			}
		}
		if (found == false) {
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			trDamageUnit(0.2 * amt);
		}
	}
}

rule the_cloud_damage
inactive
highFrequency
{
	int hit = 0;
	float amt = trTimeMS() - yGetVar("playerUnits", "deepLast");
	if (amt > 500) {
		ySetVar("playerUnits","deepLast",trTimeMS() + 500);
		hit = trCountUnitsInArea(""+1*trQuestVarGet("playerUnits"),0,"Invisible Target",6);
		
		if (hit > 0) {
			trQuestVarSetFromRand("sound", 1, 4, true);
			trSoundPlayFN("swordonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			trUnitSelectClear();
			trUnitSelectByQV("playerUnits");
			damagePlayerUnit(amt * hit * 0.5);
		}
	}
	trUnitSelectClear();
	trUnitSelectByQV("stageWonder");
	if (trUnitAlive() == false) {
		for(i=yGetDatabaseCount("cloudTornados"); >0) {
			yDatabaseNext("cloudTornados",true);
			trUnitDestroy();
		}
		trMessageSetText("The Sky Palace has been destroyed! Tornados dispelled!", -1);
		yClearDatabase("cloudTornados");
		xsDisableSelf();
	}
	for (i=yGetDatabaseCount("cloudDeployStars"); >0) {
		if (PROJ_GROUND == processGenericProj("cloudDeployStars")) {
			trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("cloudDeployProto")));
			trUnitSelectClear();
			trUnitSelectByQV("cloudDeployStars");
			trDamageUnitPercent(-100);
			activateEnemy("cloudDeployStars");
			yRemoveFromDatabase("cloudDeployStars");
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("vortexland"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	switch(1*trQuestVarGet("cloudDeployStep"))
	{
		case 0:
		{
			if (trTime() > trQuestVarGet("cloudDeployNext")) {
				for(i=yGetDatabaseCount("playerCharacters"); >0) {
					if (yDatabaseNext("playerCharacters",true) == -1 || trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						trVectorSetUnitPos("cloudDeploycenter", "playerCharacters");
						break;
					}
				}
				trQuestVarSet("cloudDeployStep", 1);
				trQuestVarSet("cloudDeployLanding", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("cloudDeploycenterx"),0,trQuestVarGet("cloudDeploycenterz"),0,true);
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
				trVectorQuestVarSet("cloudDeployDir", rotationMatrix("cloudDeployDir", -0.757323, 0.653041));
				trQuestVarSetFromRand("dist", 2.0, 10.0, false);
				trQuestVarSet("posx", trQuestVarGet("cloudDeployCenterx") - trQuestVarGet("dist") * trQuestVarGet("cloudDeployDirx"));
				trQuestVarSet("posz", trQuestVarGet("cloudDeployCenterz") - trQuestVarGet("dist") * trQuestVarGet("cloudDeployDirz"));
				addGenericProj("cloudDeployStars","pos","cloudDeployDir",kbGetProtoUnitID("Lampades"),18,0.01,2.5,0,ENEMY_PLAYER);
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
			if (yGetDatabaseCount("cloudDeployStars") == 0) {
				trQuestVarSet("cloudDeployStep", 0);
				trUnitSelectClear();
				trUnitSelectByQV("cloudDeployLanding");
				trUnitDestroy();
				trQuestVarSetFromRand("rand", 30, 90, true);
				trQuestVarSet("cloudDeployNext", trTime() + trQuestVarGet("rand"));
			}
		}
	}
}
