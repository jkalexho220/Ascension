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
int unlockedrelics1 = 0;
int unlockedrelics2 = 0;
int unlockedrelics3 = 0;
int unlockedrelics4 = 0;


void showLoadProgress() {
	trSoundPlayFN("default","1",-1,"Loading Data:"+100 * loadProgress / 15,"icons\god power reverse time icons 64");
}

rule data_load_00
highFrequency
inactive
{
	unlockedrelics1 = trGetScenarioUserData(12);
	unlockedrelics2 = trGetScenarioUserData(13);
	unlockedrelics3 = trGetScenarioUserData(14);
	unlockedrelics4 = trGetScenarioUserData(15);


	for(x=1; < 9) {
		trQuestVarSet("ownedRelics"+x, iModulo(10, unlockedrelics1));
		unlockedrelics1 = unlockedrelics1 / 10;
		trQuestVarSet("ownedRelics"+(x+8), iModulo(10, unlockedrelics2));
		unlockedrelics2 = unlockedrelics2 / 10;
		trQuestVarSet("ownedRelics"+(x+16), iModulo(10, unlockedrelics3));
		unlockedrelics3 = unlockedrelics3 / 10;
		trQuestVarSet("ownedRelics"+(x+24), iModulo(10, unlockedrelics4));
		unlockedrelics4 = unlockedrelics4 / 10;
	}
	if (Multiplayer) {
		trSoundPlayFN("default","1",-1,"Loading:","icons\god power reverse time icons 64");

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
		trLetterBox(false);
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
		savedata = trGetScenarioUserData(0);
		currentdata = iModulo(10, savedata);
		savedata = savedata / 10;
		xsEnableRule("data_load_01_load_data");
		xsEnableRule("data_load_02_detect_data");
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
						trPlayerGrantResources(p, "Gold", x);
					} else if (loadProgress == 4) {
						trPlayerGrantResources(p, "Gold", x * 32);
					} else if (loadProgress < 15) {
						trQuestVarSet("p"+p+"relic"+(loadProgress - 3), x);
					}
					trUnitSelectClear();
					trUnitSelectByID(x);
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
					savedata = trGetScenarioUserData(1);
				}
				case 5:
				{
					savedata = trGetScenarioUserData(2);
				}
				case 10:
				{
					savedata = trGetScenarioUserData(3);
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
			trQuestVarSet("p"+p+"noob", 1);
		}
	}
	trUnblockAllSounds();
	trSoundPlayFN("favordump.wav","1",-1,"Done!","icons\god power reverse time icons 64");
	xsDisableSelf();
	xsEnableRule("Z_cin_00");
}