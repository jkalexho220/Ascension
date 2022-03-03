const int CHEST_STATE_CLOSED = 0;
const int CHEST_STATE_UNLOCKED = 1;
const int CHEST_STATE_REWARDING = 2;
const int CHEST_STATE_COUNTDOWN = 3;

void removeChest() {
	yRemoveFromDatabase("chests");
	yRemoveUpdateVar("chests", "count");
	yRemoveUpdateVar("chests", "next");
	yRemoveUpdateVar("chests", "state");
	yRemoveUpdateVar("chests", "type");
	yRemoveUpdateVar("chests", "key");
	yRemoveUpdateVar("chests", "enemiesBegin");
	yRemoveUpdateVar("chests", "enemiesEnd");
	yRemoveUpdateVar("chests", "indicator");
}

void turnStatue(int room = 0, int index = 0, bool first = false, bool immediate = false) {
	int old = yGetPointer("statuesIn"+room);
	if (index > 0) {
		ySetPointer("statuesIn"+room, index);
	}
	ySetVar("statuesIn"+room, "angle", fModulo(6.283185, 1.570796 + yGetVar("statuesIn"+room, "angle")));
	ySetVar("statuesIn"+room, "position", 1 + yGetVar("statuesIn"+room, "position"));
	if (yGetVar("statuesIn"+room, "position") >= 4) {
		ySetVar("statuesIn"+room, "position", 0);
		trQuestVarSet("correctStatuesIn"+room, 1 + trQuestVarGet("correctStatuesIn"+room));
	} else if (yGetVar("statuesIn"+room, "position") == 1) {
		trQuestVarSet("correctStatuesIn"+room, trQuestVarGet("correctStatuesIn"+room) - 1);
	}
	trUnitSelectClear();
	trUnitSelectByQV("statuesIn"+room);
	trDamageUnitPercent(-100);
	if (immediate) {
		trVectorSetFromAngle("dir", yGetVar("statuesIn"+room, "angle"));
		trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
	} else {
		trUnitConvert(0);
		if (yGetVar("statuesIn"+room, "state") == 1) {
			ySetVar("statuesIn"+room, "timeout", 1000 + yGetVar("statuesIn"+room, "timeout"));
		} else {
			ySetVar("statuesIn"+room, "state", 1);
			ySetVar("statuesIn"+room, "timeout", trTimeMS() + 1000);
			trQuestVarSet("movingStatuesIn"+room, 1 + trQuestVarGet("movingStatuesIn"+room));
		}
	}
	
	if (first) {
		for(x=yGetVar("statuesIn"+room, "connections"); >0) {
			turnStatue(room, 1*yGetVar("statuesIn"+room, "connection"+x), false, immediate);
		}
		if (immediate == false) {
			trSoundPlayFN("trojangateopen.wav","1",-1,"","");
		}
	}
	
	if (index > 0) {
		ySetPointer("statuesIn"+room, old);
	}
}

void processChests() {
	int id = 0;
	int room = 0;
	float angle = 0;
	for(x=yGetDatabaseCount("rainingRelics"); > 0) {
		yDatabaseNext("rainingRelics", true);
		if (yGetVar("rainingRelics", "morphed") == 0) {
			ySetVar("rainingRelics", "morphed", 1);
			trMutateSelected(kbGetProtoUnitID("Curse SFX"));
			trUnitSetAnimationPath("0,0,0,0,0,0,0");
		} else {
			trVectorSetUnitPos("pos", "rainingRelics");
			if (trQuestVarGet("posY") <= worldHeight + 0.3) {
				trUnitChangeProtoUnit("Relic");
				trUnitSelectClear();
				trUnitSelectByQV("rainingRelics", true);
				trDamageUnitPercent(-100);
				yAddToDatabase("freeRelics", "rainingRelics");
				yAddUpdateVar("freeRelics", "type", randomStageClosest(20));
				yRemoveFromDatabase("rainingRelics");
				yRemoveUpdateVar("rainingRelics", "morphed");
			}
		}
	}
	for(x=xsMin(8, yGetDatabaseCount("rainingFire")); >0) {
		yDatabaseNext("rainingFire", true);
		if (yGetVar("rainingFire", "morphed") == 0) {
			ySetVar("rainingFire", "morphed", 1);
			trMutateSelected(kbGetProtoUnitID("Meteorite death"));
		} else {
			trVectorSetUnitPos("pos", "rainingFire");
			vectorToGrid("pos", "loc");
			if (trQuestVarGet("posY") <= 0 ||
			terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				trUnitChangeProtoUnit("Cinematic Scorch");
				trUnitSelectClear();
				trUnitSelectByQV("rainingFire", true);
				trDamageUnitPercent(-100);
				trUnitSetAnimationPath("2,0,0,0,0,0,0");
				yRemoveFromDatabase("rainingFire");
				yRemoveUpdateVar("rainingFire", "morphed");
			} else {
				trUnitSelectClear();
				for(i=yGetDatabaseCount("rainingFireTargets"); >0) {
					yDatabaseNext("rainingFireTargets", true);
					if (zDistanceToVectorSquared("rainingFireTargets", "pos") < 10) {
						if (yGetVar("rainingFireTargets", "enemy") == 0) {
							damagePlayerUnit(500.0);
						} else {
							trDamageUnit(500.0);
						}
						yRemoveFromDatabase("rainingFireTargets");
						yRemoveUpdateVar("rainingFireTargets", "enemy");
					}
				}
			}
		}
	}
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
							reselectMyself();
							uiMessageBox("Find a relic with a matching symbol and bring it here to open this chest.");
						}
						trVectorQuestVarSet("pos", kbGetBlockPosition(""+1*yGetVar("chests", "key")));
						if (zDistanceToVectorSquared("chests", "pos") < 16) {
							ySetVar("chests", "state", CHEST_STATE_UNLOCKED);
							trUnitSelectClear();
							trUnitSelect(""+1*yGetVar("chests", "key"));
							trUnitChangeProtoUnit("Osiris Box Glow");
							trUnitSelectClear();
							trUnitSelect(""+1*yGetVar("chests", "indicator"));
							trUnitChangeProtoUnit("Rocket");
						}
					}
					case CHEST_STATUES:
					{
						if (trUnitIsSelected()) {
							reselectMyself();
							uiMessageBox("Make all the statues face the chest to open it.");
						}
						room = yGetVar("chests", "room");
						for(x=yGetDatabaseCount("statuesIn"+room); >0) {
							yDatabaseNext("statuesIn"+room, true);
							switch(1*yGetVar("statuesIn"+room, "state"))
							{
								case 0:
								{
									/* waiting */
									if (trUnitPercentDamaged() > 0) {
										turnStatue(room,0,true,false);
									}
								}
								case 1:
								{
									/* turning */
									angle = 0.001 * (yGetVar("statuesIn"+room, "timeout") - trTimeMS());
									if (angle < 0) {
										angle = yGetVar("statuesIn"+room, "angle");
										ySetVar("statuesIn"+room, "state", 0);
										trUnitConvert(ENEMY_PLAYER);
										trDamageUnitPercent(-100);
										trQuestVarSet("movingStatuesIn"+room, trQuestVarGet("movingStatuesIn"+room) - 1);
									} else {
										angle = fModulo(6.283185, yGetVar("statuesIn"+room, "angle") - angle * 1.570796);
									}
									trVectorSetFromAngle("dir", angle);
									trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
								}
							}
						}
						if ((trQuestVarGet("correctStatuesIn"+room) == yGetDatabaseCount("statuesIn"+room)) &&
						trQuestVarGet("movingStatuesIn"+room) == 0) {
							ySetVar("chests", "state", CHEST_STATE_UNLOCKED);
							trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
							for(x=yGetDatabaseCount("statuesIn"+room); >0) {
								yDatabaseNext("statuesIn"+room, true);
								trDamageUnitPercent(-100);
								trUnitConvert(0);
							}
						}
					}
					case CHEST_ENCOUNTER:
					{
						if (trUnitIsSelected()) {
							reselectMyself();
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
			case CHEST_STATE_UNLOCKED:
			{
				trQuestVarSet("chestCount", 1 + trQuestVarGet("chestCount"));
				trUnitSelectClear();
				trUnitSelectByQV("chests");
				trUnitSetAnimation("SE_Great_Box_Opening",false,-1);
				trSoundPlayFN("siegetowerdeath.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, 10, true);
				if (trQuestVarGet("rand") == 1) {
					ySetVar("chests", "state", CHEST_STATE_COUNTDOWN);
					trUnitSelectClear();
					trUnitSelectByQV("chests");
					trUnitHighlight(4.0, true);
					trUnitChangeProtoUnit("Phoenix Egg");
					ySetVar("chests", "next", trTimeMS() + 1000);
					ySetVar("chests", "count", 3);
					trSoundPlayFN("attackwarning.wav","1",-1,"","");
					trMessageSetText("The chest was a bomb! Run!",-1);
				} else {
					room = yGetVar("chests", "room");
					trQuestVarSetFromRand("rand", 1, 1*trQuestVarGet("rand"), true);
					trQuestVarSet("rand", trQuestVarGet("rand") + trQuestVarGet("correctStatuesIn"+room));
					if (trQuestVarGet("rand") < ENEMY_PLAYER) {
						trQuestVarSet("rand", ENEMY_PLAYER);
					}
					debugLog("Relic reward count is " + 1*trQuestVarGet("rand"));
					ySetVar("chests", "state", CHEST_STATE_REWARDING);
					ySetVar("chests", "next", trTimeMS());
					ySetVar("chests", "count", trQuestVarGet("rand"));
					trSoundPlayFN("plentybirth.wav","1",-1,"","");
				}
			}
			case CHEST_STATE_REWARDING:
			{
				if (trTimeMS() > yGetVar("chests", "next")) {
					if (yGetVar("chests", "count") == 0) {
						removeChest();
					} else {
						trSoundPlayFN("tributereceived.wav","1",-1,"","");
						ySetVar("chests", "next", trTimeMS() + 500);
						trVectorSetUnitPos("pos", "chests");
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trQuestVarSetFromRand("heading",1,360,true);
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),trQuestVarGet("heading"),true);
						trQuestVarSetFromRand("speed", 2, 10);
						zSetProtoUnitStat("Kronny Flying", 0, 1, trQuestVarGet("speed"));
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trUnitConvert(0);
						trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
						trDamageUnitPercent(100);
						trSetSelectedScale(0,-3,0);
						yAddToDatabase("rainingRelics", "next");
						ySetVar("chests", "count", yGetVar("chests", "count") - 1);
					}
				}
			}
			case CHEST_STATE_COUNTDOWN:
			{
				trUnitSelectClear();
				trUnitSelectByQV("chests");
				trQuestVarSet("scale", 4.0 - yGetVar("chests", "count") + 0.001 * (trTimeMS() - yGetVar("chests", "next") + 1000));
				trSetSelectedScale(trQuestVarGet("scale"), trQuestVarGet("scale"), trQuestVarGet("scale"));
				if (trTimeMS() > yGetVar("chests", "next")) {
					ySetVar("chests", "next", trTimeMS() + 1000);
					ySetVar("chests", "count", yGetVar("chests", "count") - 1);
					if (yGetVar("chests", "count") == 0) {
						trVectorSetUnitPos("pos", "chests");
						trQuestVarSet("heading", 0);
						zSetProtoUnitStat("Kronny Flying", 0, 1, 15);
						for(x=12; >0) {
							trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),trQuestVarGet("heading"),true);
							trUnitSelectClear();
							trUnitSelectByQV("next", true);
							trUnitConvert(0);
							trDamageUnitPercent(100);
							trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
							trSetSelectedScale(0,-2.0,0);
							yAddToDatabase("rainingFire", "next");
							trQuestVarSet("heading", trQuestVarGet("heading") + 30);
						}
						yClearDatabase("rainingFireTargets");
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							yDatabaseNext("playerUnits");
							if (zDistanceToVectorSquared("playerUnits", "pos") < 720) {
								yAddToDatabase("rainingFireTargets", "playerUnits");
							}
						}
						for(x=yGetDatabaseCount("enemies"); >0) {
							yDatabaseNext("enemies");
							if (zDistanceToVectorSquared("enemies", "pos") < 720) {
								yAddToDatabase("rainingFireTargets", "enemies");
								yAddUpdateVar("rainingFireTargets", "enemy", 1);
							}
						}
						trUnitSelectClear();
						trUnitSelectByQV("chests");
						trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
						if (trUnitVisToPlayer()) {
							trUIFadeToColor(255,255,255,1000,0,false);
							trCameraShake(3.0, 0.25);
						}
						trDamageUnitPercent(100);
						removeChest();
					} else {
						trSoundPlayFN("attackwarning.wav","1",-1,"","");
					}
				}
			}
		}
	}
}
