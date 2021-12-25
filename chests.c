const int CHEST_STATE_CLOSED = 0;
const int CHEST_STATE_UNLOCKED = 1;
const int CHEST_STATE_OPENING = 2;
const int CHEST_STATE_REWARDING = 3;
const int CHEST_STATE_EXPLODING = 4;

void removeChest() {
	yRemoveFromDatabase("chests");
	yRemoveUpdateVar("chests", "count");
	yRemoveUpdateVar("chests", "next");
	yRemoveUpdateVar("chests", "state");
	yRemoveUpdateVar("chests", "egg");
	yRemoveUpdateVar("chests", "type");
	yRemoveUpdateVar("chests", "key");
	yRemoveUpdateVar("chests", "enemiesBegin");
	yRemoveUpdateVar("chests", "enemiesEnd");
}

void processChests() {
	for(x=yGetDatabaseCount("rainingRelics"); > 0) {
    	yDatabaseNext("rainingRelics", true);
    	if (yGetVar("rainingRelics", "morphed") == 0) {
    		ySetVar("rainingRelics", "morphed", 1);
    		trMutateSelected(kbGetProtoUnitID("Curse SFX"));
    		trUnitSetAnimationPath("0,0,0,0,0,0,0");
    	} else {
    		trVectorSetUnitPos("pos", "rainingRelics");
    		if (trQuestVarGet("posY") <= 0) {
    			trUnitChangeProtoUnit("Relic");
    			trUnitSelectClear();
    			trUnitSelectByQV("rainingRelics", true);
    			trDamageUnitPercent(-100);
    			yAddToDatabase("freeRelics", "rainingRelics");
    			yAddUpdateVar("freeRelics", "type", randomStageClosest());
    			yRemoveFromDatabase("rainingRelics");
    			yRemoveUpdateVar("rainingRelics", "morphed");
    		}
    	}
    }
	int id = 0;
	if (yGetDatabaseCount("chests") > 0) {
		id = yDatabaseNext("chests", true);
	    switch(1*yGetVar("chests", "state"))
	    {
	    	case CHEST_STATE_CLOSED:
	    	{
	    		switch(1*yGetVar("chests", "type"))
		        {
		        	case CHEST_KEY:
		            {
		            	if (trUnitIsSelected()) {
		            		uiClearSelection();
		                	uiMessageBox("Find a relic with a matching symbol and drop it here to open this chest.");
		                }
		                trVectorQuestVarSet("pos", kbGetBlockPosition(""+1*yGetVar("chests", "key")));
		                if (zDistanceToVectorSquared("chests", "pos") < 16) {
		                	ySetVar("chests", "state", CHEST_STATE_UNLOCKED);
		                	trUnitSelectClear();
		                	trUnitSelect(""+1*yGetVar("chests", "key"));
		                	trUnitChangeProtoUnit("Osiris Box Glow");
		                }
		            }
		            case CHEST_PADS:
		            {
		                if (trUnitIsSelected()) {
		            		uiClearSelection();
		                	uiMessageBox("There are two pressure pads in this room. A player must stand on each to open this chest.");
		                }
		            }
		            case CHEST_ENCOUNTER:
		            {
		                if (trUnitIsSelected()) {
		            		uiClearSelection();
		                	uiMessageBox("You must defeat all the enemies in this room to open this chest.");
		                }
		                trQuestVarSet("allDead", 1);
		                for(x=yGetVar("chests", "enemiesBegin"); < yGetVar("chests", "enemiesEnd")) {
		                	trUnitSelectClear();
		                	trUnitSelect(""+x);
		                	if (trUnitAlive()) {
		                		trQuestVarSet("allDead", 0);
		                		break;
		                	}
		                }
		                if (trQuestVarGet("allDead") == 1) {
		                	ySetVar("chests", "state", CHEST_STATE_UNLOCKED);
		                }
		            }
		        }
	    	}
	    	case CHEST_STATE_OPENING:
	    	{
	    		if (kbUnitGetAnimationActionType(id) == -1) {
	    			trQuestVarSetFromRand("rand", 1, 10, true);
	    			if (trQuestVarGet("rand") == 1) {
	    				ySetVar("chests", "state", CHEST_STATE_EXPLODING);
	    				ySetVar("chests", "egg", trGetNextUnitScenarioNameNumber());
	    				trVectorSetUnitPos("pos", "chests");
	    				trArmyDispatch("1,0", "Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
	    				trArmySelect("1,0");
	    				trUnitConvert(0);
	    				trUnitChangeProtoUnit("Phoenix Egg");
	    				ySetVar("chests", "next", trTimeMS() + 1000);
	    				ySetVar("chests", "count", 3);
	    				trSoundPlayFN("attackwarning.wav","1",-1,"","");
	    				trMessageSetText("The chest was a bomb! Run!",-1);
	    			} else {
	    				trQuestVarSetFromRand("rand", 1, trQuestVarGet("rand"), true);
	    				if (trQuestVarGet("rand") < ENEMY_PLAYER) {
	    					trQuestVarSet("rand", ENEMY_PLAYER);
	    				}
	    				ySetVar("chests", "state", CHEST_STATE_REWARDING);
	    				ySetVar("chests", "next", trTimeMS());
	    				ySetVar("chests", "count", trQuestVarGet("rand"));
	    				trSoundPlayFN("plentybirth.wav","1",-1,"","");
	    			}
	    		}
	    	}
	    	case CHEST_STATE_UNLOCKED:
	    	{
	    		trUnitSelectClear();
	    		trUnitSelectByQV("chests");
	    		trUnitSetAnimation("SE_Great_Box_Opening",false,-1);
	    		ySetVar("chests", "state", CHEST_STATE_OPENING);
	    		trSoundPlayFN("siegetowerdeath.wav","1",-1,"","");
	    	}
	    	case CHEST_STATE_REWARDING:
	    	{
	    		if (trTimeMS() > yGetVar("chests", "next")) {
	    			ySetVar("chests", "count", yGetVar("chests", "count") - 1);
	    			if (yGetVar("chests", "count") == 0) {
	    				removeChest();
	    			} else {
	    				ySetVar("chests", "next", trTimeMS() + 500);
	    				trVectorSetUnitPos("pos", "chests");
			    		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			    		trQuestVarSetFromRand("heading",1,360,true);
			    		trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),trQuestVarGet("heading"),true);
			    		trQuestVarSetFromRand("speed", 0, 4);
			    		zSetProtoUnitStat("Kronny Flying", 0, 1, trQuestVarGet("speed"));
			    		trUnitSelectClear();
			    		trUnitSelectByQV("next", true);
			    		trUnitConvert(0);
			    		trDamageUnitPercent(100);
			    		trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
			    		trSetSelectedScale(0,-3,0);
			    		yAddToDatabase("rainingRelics", "next");
	    			}
	    		}
	    	}
	    	case CHEST_STATE_EXPLODING:
	    	{
	    		trUnitSelectClear();
	    		trUnitSelect(""+1*yGetVar("chests", "egg"));
	    		trQuestVarSet("scale", 3.0 - yGetVar("chests", "count") + 0.001 * (trTimeMS() - yGetVar("chests", "next") + 1000));
	    		if (trTimeMS() > yGetVar("chests", "next")) {
	    			ySetVar("chests", "count", yGetVar("chests", "count") - 1);
	    			if (yGetVar("chests", "count") == 0) {
	    				removeChest();
	    			} else {
	    				trSoundPlayFN("attackwarning.wav","1",-1,"","");
	    			}
	    		}
	    	}
	    }
	}
}