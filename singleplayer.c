void spSwitchToClass(int class = -1) {
	chooseClass(1, class - 3000);
}

void spExplainClass(int class = -1) {
	class = class - 4000;
	explainClass(class);
	if (trQuestVarGet("class"+class+"level") == 0) {
		trDelayedRuleActivation("singleplayer_explain_class");
	}
}

rule singleplayer_init
inactive
highFrequency
{
	if (trTime() > cActivationTime + 2) {
		xsDisableSelf();
		trLetterBox(false);
	    trUIFadeToColor(0,0,0,1000,0,false);
		trMusicPlayCurrent();
	    trPlayNextMusicTrack();
	    chooseClass(1, 1*trQuestVarGet("p1class"));

	    trVectorQuestVarSet("startPosition", vector(135,0,135));

	    trPaintTerrain(0,0,145,145,2,13,false);
	    
	    TERRAIN_WALL = 2;
	    TERRAIN_SUB_WALL = 13;

	    trPlayerSetDiplomacy(1, 2, "Enemy");
	    trPlayerSetDiplomacy(2, 1, "Enemy");
	    trPlayerSetDiplomacy(0, 1, "Neutral");
	    trArmyDispatch("1,0","Victory Marker",1,1,0,1,0,true);
	    trArmyDispatch("2,0","Victory Marker",1,1,0,1,0,true);

	    int x = 0;
	    int z = 15;
	    /* center island */
	    for(x=0; <15) {
	    	for(b=0; < 15) {
	    		if (x*x + z*z <= 225) {
	    			trPaintTerrain(72-x,72-z,72+x,72+z,0,50,false);
	    			break;
	    		} else {
	    			z = z - 1;
	    		}
	    	}
	    }

	    trPaintTerrain(57,71,73,73,0,53,false);
	    trPaintTerrain(71,57,73,73,0,53,false);

	    trQuestVarSet("levelupObelisk", trGetNextUnitScenarioNameNumber());
	    trArmyDispatch("1,0","Dwarf",1,145,0,118,0,true);
	    trArmySelect("1,0");
	    trUnitConvert(0);
	    trMutateSelected(kbGetProtoUnitID("Outpost"));

	    /* relic warehouse */
	    trPaintTerrain(45,66, 57,78, 0,70, false);
	    x = 114;
	    for(a=1; <= 10) {
	    	z = 154;
	    	if (trQuestVarGet("ownedRelics"+a) > 0) {
	    		trArmyDispatch("1,0","Dwarf",1,x,0,z+2,180,true);
		    	trArmySelect("1,0");
		    	trUnitConvert(0);
		    	trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(a)));
		    	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		    	trArmyDispatch("1,0", "Dwarf",trQuestVarGet("ownedRelics"+a),x,0,z,0,true);
		    	trArmySelect("1,0");
		    	trUnitChangeProtoUnit("Relic");
		    	for(b=0; <trQuestVarGet("ownedRelics"+a)) {
		    		yAddToDatabase("freeRelics", "next");
		    		yAddUpdateVar("freeRelics", "type", a);
		    		trQuestVarSet("next", 1 + trQuestVarGet("next"));
		    	}
	    	}
	    	
	    	z = 134;
	    	if (trQuestVarGet("ownedRelics"+(a+10)) > 0) {
	    		trArmyDispatch("1,0","Dwarf",1,x,0,z-2,0,true);
		    	trArmySelect("1,0");
		    	trUnitConvert(0);
		    	trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(a+10)));
		    	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		    	trArmyDispatch("1,0", "Dwarf",trQuestVarGet("ownedRelics"+(a+10)),x,0,z,0,true);
		    	trArmySelect("1,0");
		    	trUnitChangeProtoUnit("Relic");
		    	for(b=0; <trQuestVarGet("ownedRelics"+(a+10))) {
		    		yAddToDatabase("freeRelics", "next");
		    		yAddUpdateVar("freeRelics", "type", a+10);
		    		trQuestVarSet("next", 1 + trQuestVarGet("next"));
		    	}
	    	}
	    	
	    	x = x - 2;
	    }

	    /* if player is new */
	    if (trQuestVarGet("class1level") == 0) {
	    	xsEnableRule("singleplayer_cin");
	    	trQuestVarSet("class1level", 1);
	    	trQuestVarSet("class2level", 1);
	    	startNPCDialog(NPC_EXPLAIN_SINGLEPLAYER);
	    } else {
	    	xsEnableRule("gameplay_start_2");
	    }

	    /* class selection */
	    trPaintTerrain(69,49, 75,56, 4,15, false);
	    x = 138;
	    z = 98;
	    for(a=1; <= CLASS_COUNT) {
	    	trQuestVarSet("class"+a+"unit", trGetNextUnitScenarioNameNumber());
	    	trArmyDispatch("1,0","Dwarf",1,x,0,z,180,true);
	    	trArmySelect("1,0");
	    	trUnitConvert(0);
	    	trMutateSelected(1*trQuestVarGet("class"+a+"proto"));
	    	if (trQuestVarGet("class"+a+"level") == 0) {
	    		trPaintTerrain(x/2,z/2,x/2,z/2,2,12,false);
	    	}
	    	x = x + 4;
	    	if (x > 150) {
	    		x = 138;
	    		z = z + 4;
	    	}
	    	trEventSetHandler(3000+a, "spSwitchToClass");
	    	trEventSetHandler(4000+a, "spExplainClass");
	    }

	    trSetCounterDisplay("To save and exit, enter the Sky Passage.");
	    
	    trSetFogAndBlackmap(false, false);
	    trSetLighting("default", 0);

	    trQuestVarSet("skyPassage", trGetNextUnitScenarioNameNumber());
	    trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
	    trArmySelect("1,0");
	    trMutateSelected(kbGetProtoUnitID("Sky Passage"));

	    /* paint deployment square at the bottom of the map */
		trPaintTerrain(0,0,5,5,0,70,true);
        trPaintTerrain(0,0,5,5,2,13,false);



        xsEnableRule("singleplayer_always");
	}
}

rule singleplayer_always
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("p1unit");
	if (trUnitGetIsContained("Sky Passage")) {
		xsDisableSelf();
		xsDisableRule("gameplay_always");
		trUIFadeToColor(255,255,255,1000,0,true);
		saveAllData();
		xsEnableRule("singleplayer_end");
	}

	for(x=CLASS_COUNT; >0) {
		trUnitSelectClear();
		trUnitSelectByQV("class"+x+"unit");
		if (trUnitIsSelected()) {
			uiClearSelection();
			if (trQuestVarGet("p1class") == x || trQuestVarGet("class"+x+"level") == 0) {
				explainClass(x);
			} else if (trQuestVarGet("class"+x+"level") > 0) {
				trShowChoiceDialog(className(x) + " (Level " + 1*trQuestVarGet("class"+x+"level")+")",
				"Switch to this class", 3000 + x, "View class details", 4000 + x);
			}
		}
	}
}

rule singleplayer_end
inactive
highFrequency
{
	if (trTime() > 1 + cActivationTime) {
		trModeEnter("Pregame");
	}
}


rule singleplayer_explain_class
inactive
highFrequency
{
	if (trQuestVarGet("explain") == 0) {
		xsDisableSelf();
		int class = trQuestVarGet("pleaseExplain");
		switch(class)
		{
			case THUNDERRIDER:
			{
				uiMessageBox("To unlock this class, defeat 5 bosses.");
			}
			case FIREKNIGHT:
			{
				uiMessageBox("To unlock this class, host this map in multiplayer once.");
			}
			case FROSTKNIGHT:
			{
				uiMessageBox("To unlock this class, kill 100 Giants.");
			}
		}
	}
}
