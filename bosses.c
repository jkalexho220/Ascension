const int BOSS_SPELL_COOLDOWN = 100;

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


rule boss_music
minInterval 3
inactive
{
	if (trQuestVarGet("boss") > 0 && trTime() > trQuestVarGet("musicTime")) {
		if (trQuestVarGet("boss") == 11) {
			trMusicPlay("music\interface\if you can use a doorknob.mp3", "1",0.2);
			trQuestVarSet("musicTime", trTime() + 60);
		} else {
			trQuestVarSet("musicTime", trTime() + 50);
			trQuestVarSetFromRand("music", 1, 6, true);
			switch(trQuestVarGet("music"))
			{
				case 1:
				{
					trMusicPlay("music\fight\i wish i could throw shapes.mp3","1",3.0);
				}
				case 2:
				{
					trMusicPlay("music\fight\li'l drips.mp3","1",3.0);	
				}
				case 3:
				{
					trMusicPlay("music\fight\meatier shower.mp3","1",3.0);	
				}
				case 4:
				{
					trMusicPlay("music\fight\oi, that pops!!!.mp3","1",3.0);	
				}
				case 5:
				{
					trMusicPlay("music\fight\rot loaf.mp3","1",3.0);	
				}
				case 6:
				{
					trMusicPlay("music\fight\the fire brigade.mp3","1",3.0);	
				}
			}
		}
	}
}

rule boss_cin_00
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		xsDisableSelf();
		trQuestVarSet("bossRelicCount", randomLow(2));
		trQuestVarSet("normalRelicCount", 3 - trQuestVarGet("bossRelicCount"));
		trQuestVarSet("gameOverStep", 0);
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 - 3,
			trQuestVarGet("bossRoomEntranceX")/2 + 35, trQuestVarGet("bossRoomEntranceZ")/2,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 - 3,
			trQuestVarGet("bossRoomEntranceX")/2 + 35, trQuestVarGet("bossRoomEntranceZ")/2,5,false);
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX")/2-4, trQuestVarGet("bossRoomEntranceZ")/2,
			trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 + 35,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX")/2 - 3, trQuestVarGet("bossRoomEntranceZ")/2,
			trQuestVarGet("bossRoomEntranceX")/2, trQuestVarGet("bossRoomEntranceZ")/2 + 35,5,false);

		trPaintTerrain(0,0,5,5,0,70,true);
        trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

		trQuestVarSet("bossUnit", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0",trStringQuestVarGet("bossProto"),1,
			trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit", true);
		trUnitConvert(0);

		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		
		xsEnableRule("boss"+1*trQuestVarGet("stage")+"_init");

		trQuestVarSet("boss", trQuestVarGet("stage"));

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
				trModifyProtounit("Tornado", ENEMY_PLAYER, 55, 1);
				trModifyProtounit("Tornado", ENEMY_PLAYER, 8, -99);
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(trQuestVarGet("bossScale"), trQuestVarGet("bossScale"), trQuestVarGet("bossScale"));
				spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Cinematic Block"), "fakeBoss");
				xsEnableRule("boss1_battle");
				trQuestVarSet("bossSpell", 11);
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 2, 60);
				trQuestVarSet("boss", 1);
				trQuestVarSet("bossGem", STARSTONE);
				trQuestVarSetFromRand("bossGemCount", 1, 2, true);
				xsEnableRule("boss_music");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss1_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossUnit");
	int old = xsGetContextPlayer();
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float angle = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		if (yGetDatabaseCount("bossMeteors") > 0) {
			yDatabaseNext("bossMeteors");
			if (yGetVar("bossMeteors", "yeehaw") == 1) {
				ySetVar("bossMeteors", "yeehaw", 0);
				trUnitSelectClear();
				trUnitSelectByQV("bossMeteors");
				trMutateSelected(kbGetProtoUnitID("Heka Shockwave SFX"));
				trUnitOverrideAnimation(2,0,true,false,-1);
			} else if (yGetVar("bossMeteors", "yeehaw") == 2) {
				trUnitSelectClear();
				trUnitSelectByQV("bossMeteors");
				trUnitChangeProtoUnit("Rock Granite Big");
				yRemoveFromDatabase("bossMeteors");
			} else {
				trVectorSetUnitPos("pos", "bossMeteors");
				if (trQuestVarGet("posY") <= worldHeight + 0.5) {
					trUnitSelectClear();
					trUnitSelectByQV("bossMeteors");
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					trUnitSelectClear();
					trUnitSelectByQV("bossMeteors");
					trSetSelectedScale(0,0,0);
					trDamageUnitPercent(-100);
					vectorToGrid("pos", "loc");
					trPaintTerrain(1*trQuestVarGet("locX"),1*trQuestVarGet("locZ"),
						1*trQuestVarGet("locX"),1*trQuestVarGet("locZ"),
						TERRAIN_WALL, TERRAIN_SUB_WALL, false);
					ySetVar("bossMeteors", "yeehaw", 2);
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						id = yDatabaseNext("playeRUnits", true);
						if (id == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (zDistanceToVectorSquared("playerUnits", "pos") < 9) {
							damagePlayerUnit(200);
						}
					}
				}
			}
		}
		for(x=xsMin(4, yGetDatabaseCount("bossShockwaves")); >0) {
			action = processGenericProj("bossShockwaves");
			trVectorSetUnitPos("pos", "bossShockwaves");
			hit = false;
			for(y=yGetDatabaseCount("bossShockwaveTargets"); >0) {
				id = yDatabaseNext("bossShockwaveTargets", true);
				if (id == -1 || trUnitAlive() == false) {
					yRemoveFromDatabase("bossShockwaveTargets");
				} else if (zDistanceToVectorSquared("bossShockwaveTargets", "pos") < 9) {
					hit = true;
					ySetPointer("playerUnits", 1*yGetVar("bossShockwaveTargets", "index"));
					damagePlayerUnit(200);
					if (trQuestVarGet("bossSpell") < 30 || trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
						yRemoveFromDatabase("bossShockwaveTargets");
					}
				}
			}
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) || hit) {
				trUnitSelectClear();
				trUnitSelectByQV("bossShockwaves", true);
				trUnitChangeProtoUnit("Dust Small");
				yRemoveFromDatabase("bossShockwaves");
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
				trQuestVarSet("bossSpell", 0);
				trQuestVarSet("bossUltimate", trQuestVarGet("bossUltimate") - 1);
			} else if (trUnitPercentDamaged() > trQuestVarGet("bossDamaged")) {
				trQuestVarSet("bossDamaged", trQuestVarGet("bossDamaged") + 3);
				trQuestVarSet("bossCooldownTime", trQuestVarGet("bossCooldownTime") - 1000);
			}
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Roaring Gale", 3.0, -1, -1, -1);
				id = kbGetBlockID(""+1*trQuestVarGet("fakeBoss"));
				if (id == -1) {
					trQuestVarSet("noFake", 1);
					spyEffect(1*trQuestVarGet("bossUnit"),kbGetProtoUnitID("Cinematic Block"),"fakeBoss");
				} else {
					trQuestVarSet("noFake", 0);
					trSetSelectedScale(0,0,0);
					trMutateSelected(kbGetProtoUnitID("Nemean Lion"));
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 24, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 25, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 26, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, -6.0);
					trUnitSelectClear();
					trUnitSelectByQV("fakeBoss");
					trUnitChangeProtoUnit("Tornado");
					trUnitConvert(ENEMY_PLAYER);
				}
				ySetPointer("enemies", 1*trQuestVarGet("bossPointer"));
				removeEnemy();
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossSpellTimeout", trTimeMS() + 12000);
				trQuestVarSet("bossSpellNext", trTimeMS() + 2000);
				yClearDatabase("bossShockwaveTargets");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					id = yDatabaseNext("playerUnits", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						yAddToDatabase("bossShockwaveTargets", "playerUnits");
						yAddUpdateVar("bossShockwaveTargets", "index", yGetPointer("playerUnits"));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > trQuestVarGet("bossSpellNext")) {
					trQuestVarSet("bossSpellNext", trQuestVarGet("bossSpellNext") + 200);
					trVectorSetUnitPos("bossPos", "bossUnit");
					trQuestVarSet("bossangle", fModulo(6.283185, trQuestVarGet("bossangle") + 2.41));
					id = kbGetProtoUnitID("Heka Shockwave SFX");
					trVectorSetFromAngle("dir", trQuestVarGet("bossangle"));
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					addGenericProj("bossShockwaves","bossPos","dir",id,2,15,4);
					if (trTimeMS() > trQuestVarGet("bossSpellTimeout")) {
						trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
						trQuestVarSetFromRand("bossCooldownTime", 7, 20, true);
						trQuestVarSet("bossCooldownTime", 1000 * trQuestVarGet("bossCooldownTime") + trTimeMS());
						trQuestVarSet("bossUltimate", 3);
						if (trQuestVarGet("noFake") == 0) {
							trUnitSelectClear();
							trUnitSelectByQV("fakeBoss");
							trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 24, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 25, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 26, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, 6.0);
						}
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trUnitChangeProtoUnit("Nemean Lion");
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
						yAddToDatabase("enemies", "bossUnit");
						yAddUpdateVar("enemies", "bounty", 0);
						yAddUpdateVar("enemies", "relic", 0);
						trSetLighting("default", 1);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			trQuestVarSetFromRand("rand", 1, 5, true);
			switch(1*trQuestVarGet("rand"))
			{
				case 1:
				{
					trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Royal Guards!");
				}
				case 2:
				{
					trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Attack!");
				}
			}
			trSoundPlayFN("wild.wav","1",-1,"","");
			trQuestVarSet("anglemod", 6.283185 / (1.0 + 2*ENEMY_PLAYER));
			trQuestVarSet("heading", fModulo(360, 57.295779 * angle - 180.0));
			trQuestVarSet("headingmod", 360.0 / (1.0 + 2*ENEMY_PLAYER));
			for(x=1+2*ENEMY_PLAYER; >0) {
				angle = fModulo(6.283185, angle + trQuestVarGet("anglemod"));
				trQuestVarSet("heading", fModulo(360.0, trQuestVarGet("heading") + trQuestVarGet("headingMod")));
				trVectorSetFromAngle("dir", angle);
				trQuestVarSet("dirX", 24.0 * trQuestVarGet("dirX") + trQuestVarGet("bossRoomCenterX"));
				trQuestVarSet("dirZ", 24.0 * trQuestVarGet("dirZ") + trQuestVarGet("bossRoomCenterZ"));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("dirx"),0,trQuestVarGet("dirz"),trQuestVarGet("heading"),true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Sphinx");
				yAddToDatabase("enemies", "next");
				yAddUpdateVar("enemies", "bounty", 0);
				yAddUpdateVar("enemies", "relic", 0);
				yAddToDatabase("Sphinxes", "next");
			}
			trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
			trQuestVarSetFromRand("bossCooldownTime", 7, 20, true);
			trQuestVarSet("bossCooldownTime", trTimeMS() + 1000 * trQuestVarGet("bossCooldownTime"));
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSet("bossMeteorCount", 1 + trQuestVarGet("bossMeteorCount"));
				trQuestVarSetFromRand("rand", 1, 5, true);
				switch(1*trQuestVarGet("rand"))
				{
					case 1:
					{
						trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Be crushed!");
					}
					case 2:
					{
						trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Quake!");
					}
				}
				trSoundPlayFN("earthquakeexist.wav","1",-1,"","");
				trCameraShake(6.0, 0.25);
				trQuestVarSet("rainCount", 9);
				trQuestVarSet("rainNext", trTimeMS());
				trQuestVarSet("bossSpell", 12);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("rainNext")) {
					trQuestVarSetFromRand("modX", -30, 30, true);
					trQuestVarSetFromRand("modZ", -30, 30, true);
					if (xsPow(trQuestVarGet("modX"), 2) + xsPow(trQuestVarGet("modZ"), 2) < 900) {
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trQuestVarSet("modX", trQuestVarGet("modX") + trQuestVarGet("bossRoomCenterX"));
						trQuestVarSet("modZ", trQuestVarGet("modZ") + trQuestVarGet("bossRoomCenterZ"));
						trQuestVarSetFromRand("heading", 1, 360, true);
						trArmyDispatch("1,0","Kronny Flying",1,trQuestVarGet("modX"),0,trQuestVarGet("modZ"),trQuestVarGet("heading"),true);
						trArmySelect("1,0");
						trUnitConvert(ENEMY_PLAYER);
						zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, 0.01);
						trDamageUnitPercent(100);
						trSetSelectedScale(0,5.0,0);
						yAddToDatabase("bossMeteors", "next");
						yAddUpdateVar("bossMeteors", "yeehaw", 1);
						trQuestVarSet("rainNext", 500 + trQuestVarGet("rainNext"));
						trQuestVarSet("rainCount", trQuestVarGet("rainCount") - 1);
						if (trQuestVarGet("rainCount") <= 0) {
							trQuestVarSet("bossSpell", 13);
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (yGetDatabaseCount("bossMeteors") == 0) {
					trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
					trQuestVarSetFromRand("bossCooldownTime", 7, 20, true);
					trQuestVarSet("bossCooldownTime", trTimeMS() + 1000 * trQuestVarGet("bossCooldownTime"));
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trQuestVarSetFromRand("rand", 1, 4, true);
				switch(1*trQuestVarGet("rand"))
				{
					case 1:
					{
						trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Kneel!");
					}
					case 2:
					{
						trChatSendSpoofed(ENEMY_PLAYER, "King of Beasts: Bow before me!");
					}
				}
				
				trMutateSelected(kbGetProtoUnitID("Nemean Lion"));
				trUnitOverrideAnimation(39,0,false,false,-1);
				trUnitSetStance("Passive");
				trQuestVarSet("bossNext", trTimeMS() + 1500);
				trQuestVarSet("bossSpell", 2);
				trVectorSetUnitPos("bossPos", "bossUnit");
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1500);
					trQuestVarSet("bossSpell", 3);
					id = kbGetProtoUnitID("Heka Shockwave SFX");
					for(x=24; >0) {
						angle = fModulo(6.283185, angle + 0.2618);
						trVectorSetFromAngle("dir", angle);
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						addGenericProj("bossShockwaves","bossPos","dir",id,2,15,4);
					}
					yClearDatabase("bossShockwaveTargets");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						id = yDatabaseNext("playerUnits", true);
						if (id == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							yAddToDatabase("bossShockwaveTargets", "playerUnits");
							yAddUpdateVar("bossShockwaveTargets", "index", yGetPointer("playerUnits"));
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trUnitOverrideAnimation(-1,0,false,true,-1);
					trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
					trQuestVarSetFromRand("bossCooldownTime", 7, 20, true);
					trQuestVarSet("bossCooldownTime", trTimeMS() + 1000 * trQuestVarGet("bossCooldownTime"));
				}
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 11 && trQuestVarGet("bossMeteorCount") == 3) {
				trQuestVarSet("bossSpell", 1);
			}
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		xsDisableSelf();
		trMusicStop();
		trQuestVarSet("boss", 0);
		trSetLighting("default", 1.0);
		trSoundPlayFN("win.wav","1",-1,"","");
		for(x=yGetDatabaseCount("enemies"); >0) {
			yDatabaseNext("enemies", true);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+1*trQuestVarGet("bossUnit"));
		xsEnableRule("boss_ded");
		xsDisableRule("gameplay_always");
	}
	xsSetContextPlayer(old);
}

rule boss_ded
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("gameOverNext")) {
		int relic = 0;
		trQuestVarSet("gameOverStep", 1 + trQuestVarGet("gameOverStep"));
		switch(1*trQuestVarGet("gameOverStep"))
		{
			case 1:
			{
				trQuestVarSet("playersWon", 1);
				trLetterBox(true);
				trUIFadeToColor(0,0,0, 2000,0,true);
				trQuestVarSet("gameOverNext", trTime() + 2);
				trSoundPlayFN("default","1",-1,
					"Zenophobia: Boss defeated! Here are the rewards!","icons\infantry g hoplite icon 64");
			}
			case 2:
			{
				trQuestVarSet("gameOverNext", trTime() + 3);
				int gem = trQuestVarGet("bossGem");
				trShowImageDialog(gemstoneIcon(gem), gemstoneName(gem) + " x" + 1*trQuestVarGet("bossGemCount"));
				trQuestVarSet("gemstone"+gem, trQuestVarGet("bossGemCount") + trQuestVarGet("gemstone"+gem));
				trSoundPlayFN("favordump.wav","1",-1,"","");
			}
			case 3:
			{
				trQuestVarSet("gameOverNext", trTime() + 3);
				relic = randomStageClosest();
				trShowImageDialog(relicIcon(relic), "Relic: " + relicName(relic));
				trQuestVarSet("normalRelicCount", trQuestVarGet("normalRelicCount") - 1);
				if (trQuestVarGet("normalRelicCount") > 0) {
					trQuestVarSet("gameOverStep", 2);
				}
				trQuestVarSet("ownedRelics"+relic, 1 + trQuestVarGet("ownedRelics"+relic));
				trSoundPlayFN("favordump.wav","1",-1,"","");
			}
			case 4:
			{
				trQuestVarSet("gameOverNext", trTime() + 3);
				relic = randomStageClosest() + 10;
				trShowImageDialog(relicIcon(relic), "Relic: " + relicName(relic));
				trQuestVarSet("bossRelicCount", trQuestVarGet("bossRelicCount") - 1);
				if (trQuestVarGet("bossRelicCount") > 0) {
					trQuestVarSet("gameOverStep", 3);
				}
				trQuestVarSet("ownedRelics"+relic, 1 + trQuestVarGet("ownedRelics"+relic));
				trSoundPlayFN("favordump.wav","1",-1,"","");
			}
			case 5:
			{
				if (trQuestVarGet("stage") < 10) {
					int p = trCurrentPlayer();
					if (trQuestVarGet("stage") == trQuestVarGet("p"+p+"progress")+1) {
						trQuestVarSet("p"+p+"progress", trQuestVarGet("p"+p+"progress")+1);
					}
					int stage = 1 + trQuestVarGet("stage");
					trShowImageDialog(stageIcon(stage), "Unlocked stage " + stage + ": " + stageName(stage) + "!");
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trQuestVarSet("gameOverNext", trTime() + 5);
				}
			}
			case 6:
			{
				gadgetUnreal("ShowImageBox");
				trQuestVarSet("gameOverNext", trTime() + 8);
				trSoundPlayFN("default.wav","1",-1,
					"Zenophobia: Rewards have been sent to your warehouse. You can access them by playing this map in singleplayer.",
					"icons\infantry g hoplite icon 64");
				trQuestVarSet("gameOverStep", 1);
				xsDisableSelf();
				xsEnableRule("game_over");
			}
		}
		gadgetUnreal("ShowImageBox-CloseButton");
	}
}