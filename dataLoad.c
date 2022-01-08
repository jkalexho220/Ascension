/*
progress
0 = stage progress
1 = current class level
2 = current class
3-4 = gold
5-14 = equipped artifacts
15-16 = unlocked classes

data at 13, 14, 15 is unlocked relic data
*/
int loadProgress = 0;
int savedata = 0;
int currentdata = 0;

void saveAllData() {
	int p = trCurrentPlayer();
	/* slot 0 */
	savedata = 1*trQuestVarGet("p"+p+"progress") + 10 * trQuestVarGet("p"+p+"level") + 100 * trQuestVarGet("p"+p+"class");
	trSetCurrentScenarioUserData(0, savedata);
	/* gold */
	savedata = xsMax(0, trGetScenarioUserData(1)) + trPlayerResourceCount(p, "Gold") - trQuestVarGet("p"+p+"gold");
	trSetCurrentScenarioUserData(1, savedata);
	/* current relics */
	for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
		yDatabaseNext("p"+p+"relics");
		if (yGetVar("p"+p+"relics", "type") < RELIC_KEY_GREEK) {
			trQuestVarSet("p"+p+"relic"+x, yGetVar("p"+p+"relics", "type"));
		}
	}
	/* equipped relics */
	savedata = 0;
	currentdata = 0;
	for(x=5; > 0) {
		savedata = savedata * 32 + (1*trQuestVarGet("p"+p+"relic"+x));
		currentdata = currentdata * 32 + (1*trQuestVarGet("p"+p+"relic"+(x+5)));
	}
	trSetCurrentScenarioUserData(2, savedata);
	trSetCurrentScenarioUserData(3, currentdata);

	/* owned relics */
	for(y=0; < 4) {
		savedata = 0;
		for(x=8; >0) {
			currentdata = 1*xsMin(10, 1*trQuestVarGet("ownedRelics"+(x+8*y)));
			savedata = savedata * 11 + currentdata;
		}
		trSetCurrentScenarioUserData(12 + y, savedata);
	}

	/* gemstones */
	savedata = 0;
	for(x=3; >=0) {
		currentdata = 1*xsMin(100, 1*trQuestVarGet("gemstone"+x));
		savedata = savedata * 100 + currentdata;
	}
	trSetCurrentScenarioUserData(9, savedata);

	if (Multiplayer == false) {
		/* class levels */
		for(y=0; <2) {
			savedata = 0;
			for(x=8; >0) {
				currentdata = 1*xsMin(10, 1*trQuestVarGet("class"+(x+8*y)+"level"));
				savedata = savedata * 11 + currentdata;
			}
			trSetCurrentScenarioUserData(10 + y, savedata);
		}
	}

	/* class unlock progress */
	savedata = 0;
	currentdata = xsMin(10, trQuestVarGet("questCount"));
	savedata = savedata * 11 + currentdata;
	currentdata = xsMin(100, trQuestVarGet("giantKills"));
	savedata = savedata * 101 + currentdata;
	currentdata = xsMin(5, trQuestVarGet("bossKills"));
	savedata = savedata * 6 + currentdata;
	currentdata = trQuestVarGet("playerHasHosted");
	savedata = savedata * 2 + trQuestVarGet("playerHasHosted");
	trSetCurrentScenarioUserData(8, savedata);
}

void showLoadProgress() {
	trSoundPlayFN("default","1",-1,"Loading Data:"+100 * loadProgress / 15,"icons\god power reverse time icons 64");
}

rule data_load_00
highFrequency
inactive
{
	int proto = 0;
	/* only the local client needs this info */
	/* owned relics */
	for(y=0; < 4) {
		savedata = xsMax(0, trGetScenarioUserData(12 + y));
		for(x=1; < 9) {
			trQuestVarSet("ownedRelics"+(x+8*y), iModulo(11, savedata));
			savedata = savedata / 11;
		}
	}
	/* gemstones */
	savedata = xsMax(0, trGetScenarioUserData(9));
	for(x=0; <4) {
		trQuestVarSet("gemstone"+x, iModulo(100, savedata));
		savedata = savedata / 100;
	}
	/* class unlock progress */
	savedata = xsMax(0, trGetScenarioUserData(8));
	trQuestVarSet("playerHasHosted", iModulo(2, savedata));
	savedata = savedata / 2;
	trQuestVarSet("bossKills", iModulo(6, savedata));
	savedata = savedata / 6;
	trQuestVarSet("giantKills", iModulo(101, savedata));
	savedata = savedata / 100;
	trQuestVarSet("questCount", iModulo(11, savedata));
	savedata = savedata / 11;

	if (trCurrentPlayer() == 1) {
		trQuestVarSet("playerHasHosted", 1);
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
		xsEnableRule("data_load_01_ready");
	} else {
		trForbidProtounit(1, "Swordsman Hero");

		/* progress, level, class */
		savedata = xsMax(0, trGetScenarioUserData(0));
		trQuestVarSet("p1progress", iModulo(10, savedata));
		savedata = savedata / 10;
		trQuestVarSet("p1level", iModulo(10, savedata));
		savedata = savedata / 10;
		trQuestVarSet("p1class", savedata);

		/* gold */
		savedata = xsMax(0, trGetScenarioUserData(1));
		trQuestVarSet("p1gold", savedata);

		/* equipped relics */
		for(y=0; <2) {
			savedata = xsMax(0, trGetScenarioUserData(2 + y));
			for(x=1; <6) {
				trQuestVarSet("p1relic"+(x+5*y), iModulo(32, savedata));
				savedata = savedata / 32;
			}
		}

		/* class levels */
		for(y=0; <2) {
			savedata = xsMax(0, trGetScenarioUserData(10 + y));
			for(x=1; <9) {
				trQuestVarSet("class"+(x+8*y)+"level", iModulo(11, savedata));
				savedata = savedata / 11;
			}
		}

		xsEnableRule("singleplayer_init");
	}
	/*
	Deploy an enemy Victory Marker so they don't lose the game
	*/
	trQuestVarSet("enemyVictoryMarker", trGetNextUnitScenarioNameNumber());
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
		savedata = xsMax(0, trGetScenarioUserData(0));
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
			swordsmen = 32 * (p - 1);
			for(x=0; < 32) {
				if (kbGetUnitBaseTypeID(x + swordsmen) == kbGetProtoUnitID("Swordsman Hero")) {
					/* read the data */
					if (loadProgress == 0) {
						trQuestVarSet("p"+p+"progress", x);
					} else if (loadProgress == 1) {
						trQuestVarSet("p"+p+"level", x);
					} else if (loadProgress == 2) {
						trQuestVarSet("p"+p+"class", x);
					} else if (loadProgress == 3) {
						trQuestVarSet("p"+p+"gold", x);
					} else if (loadProgress == 4) {
						trQuestVarSet("p"+p+"gold", trQuestVarGet("p"+p+"gold") + 32 * x);
					} else if (loadProgress < 15) {
						trQuestVarSet("p"+p+"relic"+(loadProgress - 4), x);
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
		if (loadProgress == 15) {
			xsDisableSelf();
			xsEnableRule("data_load_03_done");
		} else {
			/* prepare the next data */
			xsEnableRule("data_load_01_load_data");
			switch(loadProgress)
			{
				case 3:
				{
					savedata = xsMin(1000,xsMax(0, trGetScenarioUserData(1)));
				}
				case 5:
				{
					savedata = xsMax(0, trGetScenarioUserData(2));
				}
				case 10:
				{
					savedata = xsMax(0, trGetScenarioUserData(3));
				}
			}
			if (loadProgress >=3) {
				currentdata = iModulo(32, savedata);
				savedata = savedata / 32;
			} else if (loadProgress == 2) {
				currentdata = savedata;
			} else {
				currentdata = iModulo(10, savedata);
				savedata = savedata / 10;
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
	for(x=0; < trQuestVarGet("enemyVictoryMarker")) {
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
	for(p=1; < ENEMY_PLAYER) {
		trForbidProtounit(p, "Swordsman Hero");
		trQuestVarSet("p"+p+"victoryMarker", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Victory Marker",1,1,0,1,0,true);
		class = trQuestVarGet("p"+p+"class");
		proto = trQuestVarGet("class"+class+"proto");
		trModifyProtounit(kbGetProtoUnitName(proto), p, 5, trQuestVarGet("p"+p+"level"));
		if (trQuestVarGet("p"+p+"class") == 0) {
			trQuestVarSet("newPlayers", 1);
			trQuestVarSet("p"+p+"noob", 1);
		}
		trPlayerGrantResources(p, "Gold", trQuestVarGet("p"+p+"gold"));
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
	if (trTime() > cActivationTime + 8) {
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
					"Zenophobia:Host, make sure all spots are filled and the last player is a CPU.",
					"icons\infantry g hoplite icon 64");
	}
}

rule data_load_emergency_exit_02
highFrequency
inactive
{
	if (trTime() > cActivationTime + 7) {
		xsDisableSelf();
		trModeEnter("Pregame");
	}
}