rule enter_boss_room
inactive
highFrequency
{
	for(p=1; < ENEMY_PLAYER) {
		trUnitSelectClear();
		trVectorSetUnitPos("pos", "p"+p+"unit");
		if (trQuestVarGet("posX") > trQuestVarGet("bossRoomEntranceX") &&
			trQuestVarGet("posZ") > trQuestVarGet("bossRoomEntranceZ")) {
			if (trQuestVarGet("p"+p+"enteredBossRoom") == 0) {
				trQuestVarSet("p"+p+"enteredBossRoom", 1);
				trQuestVarSet("playersInBossRoom", 1 + trQuestVarGet("playersInBossRoom"));
				trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has entered the boss room!");
				trChatSend(0, "All players must be present to start the boss!");
			}
		} else if (trQuestVarGet("p"+p+"enteredBossRoom") == 1) {
			trQuestVarSet("p"+p+"enteredBossRoom", 0);
			trQuestVarSet("playersInBossRoom", trQuestVarGet("playersInBossRoom") - 1);
		}
	}

	if (trQuestVarGet("playersInBossRoom") == trQuestVarGet("activePlayerCount")) {
		xsDisableSelf();
		xsEnableRule("boss_cin_00");
		/*
		trUIFadeToColor(0,0,0,1000,0,true);
		trLetterBox(true);
		*/
	}
}

rule boss_cin_00
inactive
highFrequency
{
	if (trTime() > cActivationTime + 2) {
		xsDisableSelf();
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 - 3,
			trQuestVarGet("bossRoomEntranceX")/2 + 35, trQuestVarGet("bossRoomEntranceZ")/2,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 - 3,
			trQuestVarGet("bossRoomEntranceX")/2 + 35, trQuestVarGet("bossRoomEntranceZ")/2,5,false);
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX")/2-4, trQuestVarGet("bossRoomEntranceZ")/2,
			trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 + 35,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX")/2 - 3, trQuestVarGet("bossRoomEntranceZ")/2,
			trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 + 35,5,true);
	}
}