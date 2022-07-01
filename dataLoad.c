/*
progress
0 = stage progress
1 = current class level
2 = god boon
3 = current class
4-5 = gold
6-17 = equipped artifacts

data at 13, 14, 15 is unlocked relic data
*/
int loadProgress = 0;
int savedata = 0;
int currentdata = 0;
const int VERSION_NUMBER = 6;
const int TOTAL_LOAD = 25;

void saveAllData() {
	xsSetContextPlayer(0);
	trSetCurrentScenarioUserData(VERSION_NUMBER, 2);
	int p = trCurrentPlayer();
	int relic = 0;
	/* relic transporter guy */
	int db = getWarehouseDB(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		relic = xGetInt(db,xRelicType);
		if (relic <= NORMAL_RELICS) {
			trQuestVarSet("ownedRelics"+relic, 1 + trQuestVarGet("ownedRelics"+relic));
		}
	}
	
	if ((trQuestVarGet("p"+p+"nickQuestProgress") < 5) && (trQuestVarGet("p"+p+"nickEquipped") == 0) && Multiplayer) {
		trQuestVarSet("p"+p+"nickQuestProgress", 0);
	}

	if (Multiplayer) {
		trQuestVarSet("nottudTicketsCount", trQuestVarGet("nottudTicketsCount") + trQuestVarGet("ownedRelics"+RELIC_NOTTUD_TICKET));
	}
	
	xSetPointer(dPlayerData,p);
	if (xGetInt(dPlayerData, xPlayerClass) == 17) {
		xSetInt(dPlayerData, xPlayerClass, 0);
	}
	/* slot 0 */
	savedata = 0;
	savedata = savedata * 40 + xGetInt(dPlayerData,xPlayerMonsterIndex);
	savedata = savedata * 10 + xGetInt(dPlayerData,xPlayerRelicTransporterLevel);
	savedata = savedata * 31 + xGetInt(dPlayerData,xPlayerClass);
	savedata = savedata * 13 + xGetInt(dPlayerData,xPlayerGodBoon);
	savedata = savedata * 10 + xGetInt(dPlayerData,xPlayerLevel);
	savedata = savedata * 10 + xGetInt(dPlayerData,xPlayerProgress);
	trSetCurrentScenarioUserData(0, savedata);
	/* gold */
	savedata = xGetInt(dPlayerData,xPlayerGold) - trQuestVarGet("p"+p+"startingGold");
	savedata = savedata + trGetScenarioUserData(1);
	trSetCurrentScenarioUserData(1, savedata);
	/* current relics */
	db = getRelicsDB(p);
	for(x=12; > xGetDatabaseCount(db)) {
		trQuestVarSet("p"+p+"relic"+x, 0);
	}
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		if (xGetInt(db, xRelicType) <= NORMAL_RELICS) {
			trQuestVarSet("p"+p+"relic"+x, xGetInt(db, xRelicType));
		} else {
			if (xGetInt(db, xRelicType) == RELIC_NICKONHAWK_TICKET) {
				trQuestVarSet("p"+p+"nickQuestProgress", 6);
			}
			trQuestVarSet("p"+p+"relic"+x, 0);
		}
	}
	/* equipped relics */
	savedata = 0;
	currentdata = 0;
	for(x=6; > 0) {
		savedata = savedata * 31 + (1*trQuestVarGet("p"+p+"relic"+x));
		currentdata = currentdata * 31 + (1*trQuestVarGet("p"+p+"relic"+(x+6)));
	}
	trSetCurrentScenarioUserData(2, savedata);
	trSetCurrentScenarioUserData(3, currentdata);
	
	/* owned relics */
	for(y=0; < 4) {
		savedata = 0;
		for(x=8; >0) {
			currentdata = 1*xsMin(12, 1*trQuestVarGet("ownedRelics"+(x+8*y)));
			savedata = savedata * 13 + currentdata;
		}
		trSetCurrentScenarioUserData(12 + y, savedata);
	}
	
	/* gemstones */
	savedata = xsMin(9, trQuestVarGet("nottudTicketsCount"));
	currentdata = xsMin(9, trQuestVarGet("dreamGogglesCount"));
	savedata = savedata * 10 + currentdata;
	for(x=2; >=0) {
		currentdata = 1*xsMin(99, 1*trQuestVarGet("gemstone"+x));
		savedata = savedata * 100 + currentdata;
	}
	trSetCurrentScenarioUserData(9, savedata);
	
	if (Multiplayer == false) {
		/* class levels */
		for(y=0; <2) {
			savedata = 0;
			for(x=8; >0) {
				currentdata = 1*xsMin(10, xGetInt(dClass, xClassLevel, x + 8 * y));
				savedata = savedata * 11 + currentdata;
			}
			trSetCurrentScenarioUserData(10 + y, savedata);
		}
	}
	
	/* class unlock progress */
	savedata = 0;
	currentdata = trQuestVarGet("doggoQuestProgress");
	savedata = savedata * 6 + currentdata;
	currentdata = trQuestVarGet("runestoneComplete");
	savedata = savedata * 2 + currentdata;
	currentdata = trQuestVarGet("monsterpediaQuestComplete");
	savedata = savedata * 2 + currentdata;
	currentdata = xsMin(10, trQuestVarGet("chestCount"));
	savedata = savedata * 11 + currentdata;
	currentdata = xsMin(9, trQuestVarGet("zenoQuiz"));
	savedata = savedata * 10 + currentdata;
	currentdata = xsMin(10, trQuestVarGet("questCount"));
	savedata = savedata * 11 + currentdata;
	currentdata = xsMin(100, trQuestVarGet("giantKills"));
	savedata = savedata * 101 + currentdata;
	currentdata = xsMin(5, trQuestVarGet("bossKills"));
	savedata = savedata * 6 + currentdata;
	currentdata = trQuestVarGet("playerHasHosted");
	savedata = savedata * 2 + currentdata;
	trSetCurrentScenarioUserData(8, savedata);
	
	/* boon unlocks */
	savedata = 0;
	for(x=12; >=0) {
		currentdata = trQuestVarGet("boonUnlocked"+x);
		savedata = savedata * 2 + currentdata;
	}
	trSetCurrentScenarioUserData(7, savedata);
	
	/* Quest data */
	savedata = 0;
	currentdata = xsMin(2, trQuestVarGet("p"+p+"monsterpediaQuest"));
	savedata = savedata * 3 + currentdata;
	currentdata = xsMin(9, trQuestVarGet("p"+p+"relicsSacrificed"));
	savedata = savedata * 10 + currentdata;

	currentdata = 0; // 3 placeholder bits
	for(x=3; >0) {
		currentdata = trQuestVarGet("p"+p+"swordpieceQuest"+x);
		savedata = savedata * 2 + currentdata;
	}
	currentdata = xsMin(3, trQuestVarGet("p"+p+"runestoneQuest"));
	savedata = savedata * 4 + currentdata;

	for(x=3; >0) {
		currentdata = trQuestVarGet("p"+p+"swordpiece"+x);
		savedata = savedata * 2 + currentdata;
	}
	currentdata = xsMin(trQuestVarGet("p"+p+"gladiatorWorlds"), 1);
	savedata = savedata * 2 + currentdata;
	currentdata = trQuestVarGet("p"+p+"hippocampus");
	savedata = savedata * 2 + currentdata;
	
	currentdata = trQuestVarGet("yeebHit");
	savedata = savedata * 2 + currentdata;
	currentdata = trQuestVarGet("p"+p+"nickEquipped");
	savedata = savedata * 2 + currentdata;
	currentdata = trQuestVarGet("p"+p+"nickQuestProgress");
	savedata = savedata * 7 + currentdata;
	trSetCurrentScenarioUserData(4, savedata);
}

void showLoadProgress() {
	trSoundPlayFN("default","1",-1,"Loading Data:"+100 * loadProgress / TOTAL_LOAD,"icons\god power reverse time icons 64");
}

rule data_load_00
highFrequency
inactive
{
	int proto = 0;
	/* only the local client needs this info */
	/*
	VERSION CHANGE
	v1. Added a new block under relics sacrificed/monsterpedia quest. Shift old block up by 1
	*/
	if (trGetScenarioUserData(VERSION_NUMBER) == 0) {
		savedata = trGetScenarioUserData(4);
		currentdata = savedata / 896;
		savedata = iModulo(896, savedata) + 32 * currentdata * 896;
		trSetCurrentScenarioUserData(4, savedata);
		trSetCurrentScenarioUserData(VERSION_NUMBER, 1);
	}
	if (trGetScenarioUserData(VERSION_NUMBER) == 1) {
		savedata = trGetScenarioUserData(7);
		currentdata = savedata / xsPow(2, BOON_HEALTH_ATTACK);
		currentdata = iModulo(2, currentdata);
		trSetCurrentScenarioUserData(8, trGetScenarioUserData(8) + currentdata * 2933040);
		trSetCurrentScenarioUserData(VERSION_NUMBER, 2);
	}
	


	/* owned relics */
	for(y=0; < 4) {
		savedata = trGetScenarioUserData(12 + y);
		if (savedata < 0) {
			savedata = 0;
		}
		for(x=1; < 9) {
			trQuestVarSet("ownedRelics"+(x+8*y), iModulo(13, savedata));
			savedata = savedata / 13;
		}
	}
	
	
	/* gemstones */
	savedata = trGetScenarioUserData(9);
	if (savedata < 0) {
		savedata = 0;
	}
	for(x=0; <3) {
		trQuestVarSet("gemstone"+x, iModulo(100, savedata));
		savedata = savedata / 100;
	}
	trQuestVarSet("dreamGogglesCount", iModulo(10, savedata));
	savedata = savedata / 10;
	trQuestVarSet("nottudTicketsCount", iModulo(10, savedata));
	savedata = savedata / 10;
	/* class unlock progress */
	savedata = trGetScenarioUserData(8);
	if (savedata < 0) {
		savedata = 0;
	}
	trQuestVarSet("playerHasHosted", iModulo(2, savedata));
	savedata = savedata / 2;
	trQuestVarSet("bossKills", iModulo(6, savedata));
	savedata = savedata / 6;
	trQuestVarSet("giantKills", iModulo(101, savedata));
	savedata = savedata / 101;
	trQuestVarSet("questCount", iModulo(11, savedata));
	savedata = savedata / 11;
	trQuestVarSet("zenoQuiz", iModulo(10, savedata));
	savedata = savedata / 10;
	trQuestVarSet("chestCount", iModulo(11, savedata));
	savedata = savedata / 11;
	trQuestVarSet("monsterpediaQuestComplete", iModulo(2, savedata));
	savedata = savedata / 2;
	trQuestVarSet("runestoneComplete", iModulo(2, savedata));
	savedata = savedata / 2;
	trQuestVarSet("doggoQuestProgress", iModulo(6, savedata));
	savedata = savedata / 6;
	
	if ((trCurrentPlayer() == 1) && Multiplayer) {
		trQuestVarSet("playerHasHosted", 1);
	}
	
	/* boons */
	savedata = trGetScenarioUserData(7);
	if (savedata < 0) {
		savedata = 0;
	}
	for(x=0; <= 12) {
		trQuestVarSet("boonUnlocked"+x, iModulo(2, savedata));
		savedata = savedata / 2;
	}
	
	if (Multiplayer) {
		
		int posX = 10;
		
		for(p=1; < ENEMY_PLAYER) {
			trModifyProtounit("Swordsman Hero", p, 6, -100);
			trModifyProtounit("Swordsman Hero", p, 16, 9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 17, 9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 18, 9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 19, 9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 16, -9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 17, -9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 18, -9999999999999999999.0);
			trModifyProtounit("Swordsman Hero", p, 19, -9999999999999999999.0);
			trArmyDispatch(""+p+",0","Swordsman", 32, posX,0,10,0,true);
			posX = posX + 10;
		}
		
		trBlockAllSounds(true);
		trDelayedRuleActivation("data_load_01_ready");
	} else {
		if (xSetPointer(dPlayerData,1) == false) {
			debugLog("Cannot set pointer to 1 for dPlayerData!");
		}
		trForbidProtounit(1, "Swordsman Hero");
		
		/* progress, level, class */
		savedata = trGetScenarioUserData(0);
		if (savedata < 0) {
			savedata = 0;
		}
		xSetInt(dPlayerData,xPlayerProgress,iModulo(10, savedata));
		savedata = savedata / 10;
		xSetInt(dPlayerData,xPlayerLevel,iModulo(10, savedata));
		savedata = savedata / 10;
		xSetInt(dPlayerData,xPlayerGodBoon,iModulo(13, savedata));
		savedata = savedata / 13;
		xSetInt(dPlayerData,xPlayerClass,iModulo(31, savedata));
		savedata = savedata / 31;
		xSetInt(dPlayerData,xPlayerRelicTransporterLevel,iModulo(10, savedata));
		savedata = savedata / 10;
		xSetInt(dPlayerData,xPlayerMonsterIndex,iModulo(40, savedata));
		savedata = savedata / 40;
		trQuestVarSet("p1monsterProto", monsterPetProto(xGetInt(dPlayerData,xPlayerMonsterIndex)));
		
		/* gold */
		savedata = trGetScenarioUserData(1);
		if (savedata < 0) {
			savedata = 0;
		}
		xSetInt(dPlayerData,xPlayerGold,savedata);
		trQuestVarSet("p1startinggold", savedata);
		
		/* equipped relics */
		for(y=0; <2) {
			savedata = trGetScenarioUserData(2 + y);
			if (savedata < 0) {
				savedata = 0;
			}
			for(x=1; <7) {
				trQuestVarSet("p1relic"+(x+6*y), iModulo(31, savedata));
				savedata = savedata / 31;
			}
		}
		
		/* class levels */
		for(y=0; <2) {
			savedata = trGetScenarioUserData(10 + y);
			if (savedata < 0) {
				savedata = 0;
			}
			for(x=1; <9) {
				xSetInt(dClass, xClassLevel, iModulo(11, savedata), x + 8 * y);
				savedata = savedata / 11;
			}
		}
		
		/* quest data */
		savedata = trGetScenarioUserData(4);
		if (savedata < 0) {
			savedata = 0;
		}
		trQuestVarSet("p1nickQuestProgress", iModulo(7, savedata));
		savedata = savedata / 7;
		trQuestVarSet("p1nickEquipped", iModulo(2, savedata));
		savedata = savedata / 2;
		trQuestVarSet("p1yeebHit", iModulo(2, savedata));
		trQuestVarSet("yeebHit", trQuestVarGet("p1yeebHit"));
		savedata = savedata / 2;

		trQuestVarSet("p1hippocampus", iModulo(2, savedata));
		savedata = savedata / 2;
		trQuestVarSet("p1gladiatorWorlds", iModulo(2, savedata));
		savedata = savedata / 2;
		for(x=1; <= 3) {
			trQuestVarSet("p1swordpiece"+x, iModulo(2, savedata));
			savedata = savedata / 2;
		}

		trQuestVarSet("p1runestoneQuest", iModulo(4, savedata));
		savedata = savedata / 4;
		for(x=1; <= 3) {
			trQuestVarSet("p1swordpieceQuest"+x, iModulo(2, savedata));
			savedata = savedata / 2;
		}

		trQuestVarSet("p1relicsSacrificed", iModulo(10, savedata));
		savedata = savedata / 10;
		trQuestVarSet("p1monsterpediaQuest", iModulo(3, savedata));
		savedata = savedata / 3;
		
		xsEnableRule("singleplayer_init");
		trDelayedRuleActivation("delayed_modify");
	}
	/*
	Deploy an enemy Victory Marker so they don't lose the game
	*/
	trQuestVarSet("temp", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Victory Marker",1,1,0,1,0,true);
	trArmySelect("1,0");
	trUnitConvert(ENEMY_PLAYER);
	xsDisableSelf();
}

rule data_load_01_ready
highFrequency
inactive
{
	int swordsmen = 0;
	int total = 32 * (cNumberPlayers - 2);
	for(p=1; < ENEMY_PLAYER) {
		swordsmen = swordsmen + trPlayerUnitCountSpecific(p, "Swordsman");
	}
	if (swordsmen == total) {
		savedata = trGetScenarioUserData(0);
		if (savedata < 0) {
			savedata = 0;
		}
		currentdata = iModulo(10, savedata);
		savedata = savedata / 10;
		
		xsEnableRule("data_load_01_load_data");
		xsEnableRule("data_load_02_detect_data");
		xsEnableRule("data_load_emergency_exit");
		xsDisableSelf();
	}
}

/*
ASYNC
*/
rule data_load_01_load_data
highFrequency
inactive
{
	trLetterBox(false);
	trBlockAllSounds(true);
	if (currentdata >= 32) {
		trSoundPlayFN("cantdothat.wav","1",-1,"Invalid code at " + loadProgress + "!","");
	}
	trUnitSelectClear();
	trUnitSelectByID(currentdata + 32 * (trCurrentPlayer() - 1));
	for(i=32; >0) {
		if (trUnitIsSelected() == false) {
			uiFindType("Swordsman");
		} else {
			break;
		}
	}
	uiTransformSelectedUnit("Swordsman Hero");
	trForceNonCinematicModels(true);
	
	trLetterBox(true);
	
	showLoadProgress();
	xsDisableSelf();
}

/*
SYNC
*/
rule data_load_02_detect_data
highFrequency
inactive
{
	int swordsmen = 0;
	for(p=1; < ENEMY_PLAYER) {
		swordsmen = swordsmen + trPlayerUnitCountSpecific(p, "Swordsman Hero");
	}
	if (swordsmen == cNumberPlayers - 2) {
		for(p=1; < ENEMY_PLAYER) {
			if (xSetPointer(dPlayerData,p) == false) {
				debugLog("Cannot set pointer for " + aiPlanGetName(dPlayerData) + " to: " + p);
				debugLog("database size is " + aiPlanGetNumberUserVariableValues(dPlayerData,xDirtyBit));
				debugLog("Progress: " + loadProgress + " context: " + xsGetContextPlayer());
			}
			swordsmen = 32 * (p - 1);
			for(x=0; < 32) {
				if (kbGetUnitBaseTypeID(x + swordsmen) == kbGetProtoUnitID("Swordsman Hero")) {
					/* read the data */
					if (loadProgress == 0) {
						xSetInt(dPlayerData,xPlayerProgress,x);
					} else if (loadProgress == 1) {
						xSetInt(dPlayerData,xPlayerLevel,x);
					} else if (loadProgress == 2) {
						xSetInt(dPlayerData,xPlayerGodBoon,x);
					} else if (loadProgress == 3) {
						xSetInt(dPlayerData,xPlayerClass,x);
					} else if (loadProgress == 4) {
						xSetInt(dPlayerData,xPlayerRelicTransporterLevel,x);
					} else if (loadProgress == 5) {
						xSetInt(dPlayerData,xPlayerMonsterIndex,x * 4);
					} else if (loadProgress == 6) {
						xSetInt(dPlayerData,xPlayerMonsterIndex,xGetInt(dPlayerData,xPlayerMonsterIndex) + x);
						xSetInt(dPlayerData,xPlayerMonsterProto,monsterPetProto(xGetInt(dPlayerData,xPlayerMonsterIndex)));
					} else if (loadProgress == 7) {
						xSetInt(dPlayerData,xPlayerGold,x);
					} else if (loadProgress == 8) {
						xSetInt(dPlayerData,xPlayerGold,xGetInt(dPlayerData,xPlayerGold) + 32 * x);
					} else if (loadProgress < 21) {
						trQuestVarSet("p"+p+"relic"+(loadProgress - 8), x);
					} else if (loadProgress == 21) {
						currentdata = x;
						trQuestVarSet("p"+p+"nickQuestProgress", iModulo(7, currentdata));
						currentdata = currentdata / 7;
						trQuestVarSet("p"+p+"nickEquipped", iModulo(2, currentdata));
						currentdata = currentdata / 2;
						trQuestVarSet("p"+p+"yeebHit", iModulo(2, currentdata));
					} else if (loadProgress == 22) {
						currentdata = x;
						trQuestVarSet("p"+p+"hippocampus", iModulo(2, currentdata));
						currentdata = currentdata / 2;
						trQuestVarSet("p"+p+"gladiatorWorlds", iModulo(2, currentdata));
						currentdata = currentdata / 2;
						for(i=1; <= 3) {
							trQuestVarSet("p"+p+"swordpiece"+i, iModulo(2, currentdata));
							currentdata = currentdata / 2;
						}
					} else if (loadProgress == 23) {
						currentdata = x;
						trQuestVarSet("p"+p+"runestoneQuest", iModulo(4, currentdata));
						currentdata = currentdata / 4;
						for(i=1; <= 3) {
							trQuestVarSet("p"+p+"swordpieceQuest"+i, iModulo(2, currentdata));
							currentdata = currentdata / 2;
						}
					} else if (loadProgress == 24) {
						currentdata = x;
						trQuestVarSet("p"+p+"relicsSacrificed", iModulo(10, currentdata));
						currentdata = currentdata / 10;
						trQuestVarSet("p"+p+"monsterpediaQuest", iModulo(3, currentdata));
						currentdata = currentdata / 3;
					}
					trUnitSelectClear();
					trUnitSelectByID(x + swordsmen);
					trMutateSelected(kbGetProtoUnitID("Swordsman"));
					break;
				}
			}
		}
		loadProgress = loadProgress + 1;
		showLoadProgress();
		if (loadProgress == TOTAL_LOAD) {
			xsDisableSelf();
			xsEnableRule("data_load_03_done");
		} else {
			/* prepare the next data */
			xsEnableRule("data_load_01_load_data");
			switch(loadProgress)
			{
				case 7: // gold
				{
					savedata = trGetScenarioUserData(1);
					if (savedata < 0) {
						savedata = 0;
					} else if (savedata > 1000) {
						savedata = 1000;
					}
				}
				case 9: // relics part 1
				{
					savedata = trGetScenarioUserData(2);
					if (savedata < 0) {
						savedata = 0;
					}
				}
				case 15: // relics part 2
				{
					savedata = trGetScenarioUserData(3);
					if (savedata < 0) {
						savedata = 0;
					}
				}
				case 21: // quest data
				{
					savedata = trGetScenarioUserData(4);
					if (savedata < 0) {
						savedata = 0;
					}
				}
			}
			if (loadProgress < 2) { // progress and level
				currentdata = iModulo(10, savedata);
				savedata = savedata / 10;
			} else if (loadProgress == 21) { // nick quest, yeeb quest, etc.
				currentdata = iModulo(28, savedata);
				savedata = savedata / 28;
			} else if (loadProgress >= 9 && loadProgress <= 20) { // relics
				currentdata = iModulo(31, savedata);
				savedata = savedata / 31;
			} else if (loadProgress == 6) { // monster pet proto
				currentdata = iModulo(4, savedata);
				savedata = savedata / 40;
			} else if (loadProgress == 5) { // monster pet stage
				currentdata = iModulo(40, savedata) / 4;
			} else if (loadProgress == 4) { // relic transporter level
				currentdata = iModulo(10, savedata);
				savedata = savedata / 10;
			} else if (loadProgress == 3) { // class
				currentdata = iModulo(31, savedata);
				savedata = savedata / 31;
			} else if (loadProgress == 2) { // godBoon
				currentdata = iModulo(13, savedata);
				savedata = savedata / 13;
			} else {
				currentdata = iModulo(32, savedata);
				savedata = savedata / 32;
			}
		}
	}
}


rule data_load_03_done
highFrequency
inactive
{
	/*
	Destroy swordsmen
	*/
	for(x=0; < trQuestVarGet("temp")) {
		trUnitSelectClear();
		trUnitSelectByID(x);
		trUnitDestroy();
	}
	/*
	Deploy victory markers to avoid defeats
	modify class hold capacity
	*/
	int class = 0;
	int proto = 0;
	trQuestVarSet("monsterpediaQuestInProgress", 0);
	for(p=1; < ENEMY_PLAYER) {
		trQuestVarSet("monsterpediaQuestInProgress", 
			trQuestVarGet("monsterpediaQuestInProgress") + trQuestVarGet("p"+p+"monsterpediaQuest"));
		trForbidProtounit(p, "Swordsman Hero");
		trArmyDispatch(""+p+",0","Victory Marker",1,1,0,1,0,true);
		class = xGetInt(dPlayerData,xPlayerClass,p);
		proto = xGetInt(dClass, xClassProto, class);
		trModifyProtounit(kbGetProtoUnitName(proto), p, 5, xGetInt(dPlayerData,xPlayerLevel,p));
		if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_TWO_RELICS) {
			trModifyProtounit(kbGetProtoUnitName(proto), p, 5, 2);
		} else if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_DOUBLE_FAVOR) {
			trSetCivAndCulture(p, 0, 0);
		}
		if (xGetInt(dPlayerData,xPlayerClass,p) == 0) {
			xSetInt(dPlayerData,xPlayerClass,1,p);
			trQuestVarSet("newPlayers", 1);
			trQuestVarSet("p"+p+"noob", 1);
		}
		if (trQuestVarGet("p"+p+"nickQuestProgress") == 0) {
			trQuestVarSet("nickQuestSpawn", 1);
		}
		if (trQuestVarGet("p"+p+"yeebHit") == 1) {
			trQuestVarSet("yeebBossFight", p);
		}
		trQuestVarSet("p"+p+"starsword", 0);
		for(i=1; <= 3) {
			trQuestVarSet("p"+p+"starsword", trQuestVarGet("p"+p+"starsword") + trQuestVarGet("p"+p+"swordpiece"+i));
		}
		trPlayerGrantResources(p, "Gold", xGetInt(dPlayerData,xPlayerGold, p));
		trQuestVarSet("p"+p+"startingGold", xGetInt(dPlayerData,xPlayerGold, p));
	}
	if (trQuestVarGet("p"+trCurrentPlayer()+"yeebHit") == 1) {
		trQuestVarSet("yeebHit", 1);
	}
	trUnblockAllSounds();
	trSoundPlayFN("favordump.wav","1",-1,"Done!","icons\god power reverse time icons 64");
	xsDisableSelf();
	xsEnableRule("Z_cin_00");
}

rule data_load_emergency_exit
highFrequency
inactive
{
	if (trTime() > cActivationTime + 5) {
		xsDisableSelf();
		if (loadProgress == 0) {
			int x = 0;
			for(p=1; < ENEMY_PLAYER) {
				x = x + trPlayerUnitCountSpecific(p, "Swordsman Hero");
			}
			if (x <= 1) {
				trSoundPlayFN("default","1",-1,
					"Zenophobia: Hmm, looks like AoM has sent everyone into singleplayer. Returning you to main menu now.",
					"icons\infantry g hoplite icon 64");
				if (trCurrentPlayer() == 1) {
					xsEnableRule("data_load_emergency_exit_01");
				} else {
					xsEnableRule("data_load_emergency_exit_02");
				}
			}
		}
	}
}

rule data_load_emergency_exit_01
highFrequency
inactive
{
	if (trTime() > cActivationTime + 5) {
		xsDisableSelf();
		xsEnableRule("data_load_emergency_exit_02");
		trSoundPlayFN("default","1",-1,
			"Zenophobia:Host, make sure all spots are filled with humans and the last player is a CPU.",
			"icons\infantry g hoplite icon 64");
	}
}

rule data_load_emergency_exit_02
highFrequency
inactive
{
	if (trTime() > cActivationTime + 7) {
		xsDisableSelf();
		trLetterBox(false);
		subModeEnter("Simulation","Editor");
		uiMessageBox("","leaveGame()");
		uiCycleCurrentActivate();
		uiCycleCurrentActivate();
		subModeLeave("Simulation","Editor");
		modeEnter("pregame");
		modeEnter("Simulation");
	}
}
