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
		}
	}

	if (trQuestVarGet("playersInBossRoom") == trQuestVarGet("activePlayerCount")) {
		xsDisableSelf();
		xsEnableRule("boss" + 1*trQuestVarGet("stage") + "_cin_00");
	}
}