/*
0 = attack + special attack element * 6
1 = Q + W * 6
2 = E + R * 6
3 = artifact1
4 = artifact2
5 = artifact3
6 = artifact4
7 = artifact5
8 = class + progress
*/

const int SLINGER = 0;
const int ARCUS = 1;
const int TURMA = 2;
const int AXEMAN = 3;

int progress = 0;

void showLoadProgress() {
	trSoundPlayFN("default","1",-1,"Loading:"+100 * progress / 9,"icons\god power reverse time icons 64");
}

rule data_load_00
highFrequency
inactive
{
	if (Multiplayer) {
		trSoundPlayFN("default","1",-1,"Loading:","icons\god power reverse time icons 64");

		int posX = 10;
		
		for(p=1; < ENEMY_PLAYER) {
			trModifyProtounit("Swordsman Hero", p, 6, -100);
			trModifyProtounit("Swordsman Hero", p, 16, -1000);
			trModifyProtounit("Swordsman Hero", p, 17, -1000);
			trModifyProtounit("Swordsman Hero", p, 18, -1000);
			trModifyProtounit("Swordsman Hero", p, 19, -1000);
			trArmyDispatch(""+p+",0","Swordsman", 36, posX,0,10,0,true);
		}
		
		trBlockAllSounds(true);
		xsEnableRule("data_load_01_ready");
	} else {
		for(p=1; < ENEMY_PLAYER) {
			trForbidProtounit(p, "Swordsman Hero");
		}
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
	int total = 36 * (cNumberPlayers - 2);
	for(p=1; < ENEMY_PLAYER) {
		swordsmen = swordsmen + trPlayerUnitCountSpecific(p, "Swordsman");
	}
	if (swordsmen == total) {
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
	int c = trGetScenarioUserData(progress);
	if (c >= 36) {
		trSoundPlayFN("cantdothat.wav","1",-1,"Invalid code at " + progress + "!","");
	}
	c = c + 36 * (trCurrentPlayer() - 1);
	trChatSend(0, "Class data: " + c);
	trUnitSelectClear();
	trUnitSelectByID(c);
	for(i=36; >0) {
		if (trUnitIsSelected() == false) {
			uiFindType("Swordsman");
		} else {
			trChatSend(0, "Found!");
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
			for(x=0; < 36 * p) {
				if (kbGetUnitBaseTypeID(x) == kbGetProtoUnitID("Swordsman Hero")) {
					if (progress <= 2) {
						/*
						Loading elements
						*/
						trQuestVarSet("p"+p+"element" + (2 * progress), iModulo(6, x));
						trQuestVarSet("p"+p+"element" + (2 * progress + 1), x / 6);
					} else if (progress == 8) {
						trQuestVarSet("p"+p+"class", iModulo(4, x));
						trQuestVarSet("p"+p+"progress", x / 4);
					} else {
						/*
						Loading artifacts
						*/
						trQuestVarSet("p"+p+"artifact" + (progress - 2), x);
					}
					trUnitSelectClear();
					trUnitSelectByID(x);
					trMutateSelected(kbGetProtoUnitID("Swordsman"));
					break;
				}
			}
		}
		progress = progress + 1;
		showLoadProgress();
		if (progress == 9) {
			xsDisableSelf();
			xsEnableRule("data_load_03_done");
		}
		xsEnableRule("data_load_01_load_data");
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
	for(p=1; < ENEMY_PLAYER) {
		trForbidProtounit(p, "Swordsman Hero");
		trQuestVarSet("p"+p+"victoryMarker", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+p+",0","Victory Marker",1,1,0,1,0,true);

		switch(1*trQuestVarGet("p"+p+"element0"))
		{
			case NONE:
			{
				trQuestVarSet("p"+p+"attack", 60);
			}
			case FIRE:
			{
				trQuestVarSet("p"+p+"attack", 100);
			}
			case ICE:
			{
				trQuestVarSet("p"+p+"attack", 50);
			}
			case THUNDER:
			{
				trQuestVarSet("p"+p+"attack", 20);
			}
			case LIGHT:
			{
				trQuestVarSet("p"+p+"attack", 50);
			}
			case DARK:
			{
				trQuestVarSet("p"+p+"attack", 0);
			}
		}
	}
	trUnblockAllSounds();
	trSoundPlayFN("favordump.wav","1",-1,"Done!","");
	xsDisableSelf();
	xsEnableRule("Z_cin_00");
}