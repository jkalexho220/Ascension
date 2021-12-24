/*
0-1 = gold
2 = gameplay loadProgress
3 = current class
4-13 = equipped artifacts
14-15 = unlocked classes

data at 13, 14, 15 is unlocked relic data
*/
int loadProgress = 0;
int savedata = 0;
int currentdata = 0;
int unlockedrelics1 = 0;
int unlockedrelics2 = 0;
int unlockedrelics3 = 0;

int miscdata6 = 0;

void showLoadProgress() {
	trSoundPlayFN("default","1",-1,"Loading Data:"+100 * loadProgress / 16,"icons\god power reverse time icons 64");
}

rule data_load_00
highFrequency
inactive
{
	unlockedrelics1 = trGetScenarioUserData(13);
	unlockedrelics2 = trGetScenarioUserData(14);
	unlockedrelics3 = trGetScenarioUserData(15);

	miscdata6 = trGetScenarioUserData(6);

	for(x=1; < 12) {
		trQuestVarSet("ownedRelics"+x, iModulo(5, unlockedrelics1));
		unlockedrelics1 = unlockedrelics1 / 5;
		trQuestVarSet("ownedRelics"+(x+12), iModulo(5, unlockedrelics2));
		unlockedrelics2 = unlockedrelics2 / 5;
		trQuestVarSet("ownedRelics"+(x+24), iModulo(5, unlockedrelics3));
		unlockedrelics3 = unlockedrelics3 / 5;
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
						trQuestVarSet("p"+p+"class", x);
					} else if (loadProgress == 2) {
						trPlayerGrantResources(p, "Gold", x);
					} else if (loadProgress == 3) {
						trPlayerGrantResources(p, "Gold", x * 32);
					} else if (loadProgress < 15) {
						trQuestVarSet("p"+p+"relic"+(loadProgress - 3), x);
					} else if (loadProgress == 15) {
						trQuestVarSet("p"+p+"unlocked", x);
					} else {
						trQuestVarSet("p"+p+"unlocked", trQuestVarGet("p"+p+"unlocked") + 32 * x);
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
		if (loadProgress == 16) {
			xsDisableSelf();
			xsEnableRule("data_load_03_done");
		} else {
			/* prepare the next data */
			xsEnableRule("data_load_01_load_data");
			switch(loadProgress)
			{
				case 1:
				{
					currentdata = savedata / 10;
				}
				case 2:
				{
					savedata = trGetScenarioUserData(1);
				}
				case 4:
				{
					savedata = trGetScenarioUserData(2);
				}
				case 9:
				{
					savedata = trGetScenarioUserData(3);
				}
				case 14:
				{
					savedata = trGetScenarioUserData(4);
				}
			}
			if (loadProgress >=2) {
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
	for(x=0; < trQuestVarGet("enemyVictoryMarker")) {
		trUnitSelectClear();
		trUnitSelectByID(x);
		trUnitDestroy();
	}
	/*
	Deploy victory markers to avoid defeats
	*/
	int n = 0;
	int current = 0;
	int proto = 0;
	for(p=1; < ENEMY_PLAYER) {
		trQuestVarSet("p"+p+"noob", 1);
		n = trQuestVarGet("p"+p+"unlocked");
		trForbidProtounit(p, "Swordsman Hero");
		trQuestVarSet("p"+p+"victoryMarker", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Victory Marker",1,1,0,1,0,true);
		current = xsPow(2, CLASS_COUNT);
		for(c=CLASS_COUNT; >0) {
			proto = trQuestVarGet("class"+c+"proto");
			trModifyProtounit(kbGetProtoUnitName(proto), p, 5, trQuestVarGet("p"+p+"progress"));
			if (n >= current) {
				trQuestVarSet("p"+p+"unlocked"+c, 1);
				trQuestVarSet("p"+p+"noob", 0);
				n = n - current;
			}
			current = current / 2;
		}
	}
	trUnblockAllSounds();
	trSoundPlayFN("favordump.wav","1",-1,"Done!","icons\god power reverse time icons 64");
	xsDisableSelf();
	xsEnableRule("Z_cin_00");
}