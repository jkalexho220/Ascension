const int WALL_WAITING = 0;
const int WALL_FALLING = 1;

void processWalls() {
	if (yGetDatabaseCount("unlockWalls") > 0) {
		yDatabaseNext("unlockWalls");
	    switch(1*yGetVar("unlockWalls", "state"))
	    {
	    	case WALL_WAITING:
	    	{
                trVectorQuestVarSet("pos", kbGetBlockPosition(""+1*yGetVar("unlockWalls", "relic")));
                if ((yGetVar("unlockWalls", "posX") - yGetVar("unlockWalls", "xMod") - 6 < trQuestVarGet("posX")) &&
                	(yGetVar("unlockWalls", "posX") + yGetVar("unlockWalls", "xMod") + 6 > trQuestVarGet("posX")) &&
                	(yGetVar("unlockWalls", "posZ") - yGetVar("unlockWalls", "zMod") - 6 < trQuestVarGet("posZ")) &&
                	(yGetVar("unlockWalls", "posZ") + yGetVar("unlockWalls", "zMod") + 6 > trQuestVarGet("posZ"))) {
                	ySetVar("unlockWalls", "state", WALL_FALLING);
                	ySetVar("unlockWalls", "timeout", trTimeMS() + 2500);
	                for(x=yGetVar("unlockWalls", "greenCirclesStart"); < yGetVar("unlockWalls", "end")) {
	                	trUnitSelectClear();
	                	trUnitSelect(""+x);
	                	trUnitDestroy();
	                }
	                for(x=yGetVar("unlockWalls", "sfxStart"); < yGetVar("unlockWalls", "greenCirclesStart")) {
	                	trUnitSelectClear();
	                	trUnitSelect(""+x);
	                	trUnitChangeProtoUnit("Heka Shockwave SFX");
	                }
	                trUnitSelectClear();
	                trUnitSelect(""+1*yGetVar("unlockWalls", "relic"));
	                trUnitChangeProtoUnit("Osiris Box Glow");
	                trSoundPlayFN("xpack\xcinematics\8_in\pyramidscrape.mp3","1",-1,"","");
                }
	    	}
	    	case WALL_FALLING:
	    	{
	    		float progress = (yGetVar("unlockWalls", "timeout") - trTimeMS()) / 2500.0;
	    		int x0 = (yGetVar("unlockWalls", "posX") - yGetVar("unlockWalls", "xMod")) / 2;
    			int x1 = (yGetVar("unlockWalls", "posX") + yGetVar("unlockWalls", "xMod")) / 2;
    			int z0 = (yGetVar("unlockWalls", "posZ") - yGetVar("unlockWalls", "zMod")) / 2;
    			int z1 = (yGetVar("unlockWalls", "posZ") + yGetVar("unlockWalls", "zMod")) / 2;
	    		if (progress > 0) {
	    			progress = progress * (wallHeight - worldHeight) + worldHeight;
	    			trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, progress, false);
	    		} else {
	    			trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
	    			trPaintTerrain(x0-1,z0-1,x1+1,z1+1,TERRAIN_PRIMARY,TERRAIN_SUB_PRIMARY,true);
	    			trSoundPlayFN("cinematics\12_in\bang1.mp3","1",-1,"","");
	    			for(x=yGetVar("unlockWalls", "sfxStart"); < yGetVar("unlockWalls", "greenCirclesStart")) {
	                	trUnitSelectClear();
	                	trUnitSelect(""+x);
	                	trUnitDestroy();
	                }
	                yRemoveFromDatabase("unlockWalls");
	    		}
	    	}
	    }
	}
}