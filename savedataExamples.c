rule saveData
inactive
highFrequency
{
	int savedata = 0;
	int currentdata = 0;

	/*
	level = 3
	progress = 6
	health = 8
	experience = 2
	0 - 9
	*/

	savedata = savedata + trQuestVarGet("level"); // 3

	currentdata = trQuestVarGet("progress");
	savedata = savedata * 10 + currentdata; // 36
	
	currentdata = trQuestVarGet("health");
	savedata = savedata * 10 + currentdata; // 368
	
	currentdata = trQuestVarGet("experience");
	savedata = savedata * 10 + currentdata; // 3682


	trSetCurrentScenarioUserData(0, savedata);
	xsDisableSelf();
}

rule readData
active
highFrequency
{
	int savedata = 0; // 3682

	for(p=1; < 10) {
		trModifyProtounit("Swordsman Hero", p, 6, -100);
		trModifyProtounit("Swordsman Hero", p, 16, 9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 17, 9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 18, 9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 19, 9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 16, -9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 17, -9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 18, -9999999999999999999.0);
		trModifyProtounit("Swordsman Hero", p, 19, -9999999999999999999.0);
	}

	/*
	3682 / 10 = 368
	368 * 10 = 3680

	3682 - 3680 = 2
	*/

	savedata = trGetScenarioUserData(0);

	trQuestVarSet("experience", iModulo(10, savedata));
	savedata = savedata / 10; // 368
	trQuestVarSet("health", iModulo(10, savedata));
	savedata = savedata / 10; // 36
	trQuestVarSet("progress", iModulo(10, savedata));
	savedata = savedata / 10; // 3
	trQuestVarSet("level", iModulo(10, savedata));
	savedata = savedata / 10; // 3

	// Deploying swordsmen for player 1
	trArmyDispatch("1,0","Swordsman",10,10,0,10,0,true);
	// we know these swordsmen have names 0-9

	xsDisableSelf();
	xsEnableRule("transfer_experience");
}

// each player runs this. And it does something different for each player
rule transfer_experience
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByID(1*trQuestVarGet("experience"));
	for(i=10; >0) {
		if (trUnitIsSelected() == false) {
			uiFindType("Swordsman");
		} else {
			break;
		}
	}
	uiTransformSelectedUnit("Swordsman Hero");
	xsDisableSelf();
}

// this happens for all players
rule receive_experience
inactive
highFrequency
{
	if (trPlayerUnitCountSpecific(1, "Swordsman Hero")) {
		for(i=10; >0) {
			if (kbGetProtoUnitID(i) == kbGetProtoUnitID("Swordsman Hero")) {
				trQuestVarSet("p1experience", i);
				break;
			}
		}
		xsDisableSelf();
	}
}

/*
Age of Mythology is deterministic (when you repeat all the actions in the game, the exact same game should play out again)

Networking: Only mouse clicks get broadcasted on the network

Every person's running instance of the map is independent

Now we need to somehow send network traffic
*/