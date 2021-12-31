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
		
		trUIFadeToColor(0,0,0,1000,0,true);
		trLetterBox(true);
	}
}

rule boss_cin_00
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
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

		trQuestVarSet("bossUnit", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0",trStringQuestVarGet("bossProto"),1,
			trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit", true);
		trUnitConvert(0);

		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		
		xsEnableRule("boss"+1*trQuestVarGet("stage")+"_init");

		int id = 0;
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id >= 0 && trUnitAlive()) {
				yAddToDatabase("enemiesIncoming", "enemies");
			}
		}
		yClearDatabase("enemies");

		trQuestVarSet("bossPointer", yAddToDatabase("enemies", "bossUnit"));

		trModifyProtounit("Revealer", 1, 2, 20);
		trArmyDispatch("1,0","Revealer",1,trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		uiLookAtUnitByName(""+1*trQuestVarGet("bossUnit"));
	}
}

rule boss1_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("The King of Beasts", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
			}
			case 1:
			{
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(trQuestVarGet("bossScale"), trQuestVarGet("bossScale"), trQuestVarGet("bossScale"));
				xsEnableRule("boss1_battle");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}