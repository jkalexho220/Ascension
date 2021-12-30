rule singleplayer_init
inactive
highFrequency
{
	xsDisableSelf();
	trMusicPlayCurrent();
    trPlayNextMusicTrack();
    chooseClass(1, 1*trQuestVarGet("p1class"));

    trVectorQuestVarSet("startPosition", vector(95,0,95));

    trPaintTerrain(0,0,145,145,2,13,false);
    
    TERRAIN_WALL = 0;
    TERRAIN_SUB_WALL = 50;

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
    			trPaintTerrain(72-x,72-z,72+x,72+z,0,0,false);
    			break;
    		} else {
    			z = z - 1;
    		}
    	}
    }
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

    spawnPlayer(1, "startPosition");

    /* if player is new */
    if (trQuestVarGet("class1level") == 0) {
    	xsEnableRule("singleplayer_cin_00");
    	trQuestVarSet("class1level", 1);
    	trQuestVarSet("class2level", 1);
    } else {
    	trLetterBox(false);
    	trUIFadeToColor(0,0,0,1000,0,false);
    }

    /* class selection */
    trPaintTerrain(68,48, 75,55, 4,15, false);
    x = 136;
    z = 96;
    for(a=1; < CLASS_COUNT) {
    	trQuestVarSet("class"+a+"unit", trGetUnitScenarioNameNumber());
    	trArmyDispatch("1,0","Dwarf",1,x,0,z,180,true);
    	trArmySelect("1,0");
    	trUnitConvert(0);
    	trMutateSelected(1*trQuestVarGet("class"+a+"proto"));
    	if (trQuestVarGet("class"+a+"level") == 0) {
    		trPaintTerrain(x/2,z/2,x/2,z/2,2,12,false);
    	}
    }

    trSetCounterDisplay("To save and exit, cast Ragnarok.");
    
    trSetFogAndBlackmap(false, false);
    trSetLighting("default", 0);
}