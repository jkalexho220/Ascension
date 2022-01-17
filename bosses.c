const int BOSS_SPELL_COOLDOWN = -1;

void bossCooldown(int minVal = 0, int maxVal = 0) {
	trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
	trQuestVarSetFromRand("bossCooldownTime", minVal, maxVal, true);
	trQuestVarSet("bossCooldownTime", trTimeMS() + 1000 * trQuestVarGet("bossCooldownTime"));
}

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

	if (trQuestVarGet("playersInBossRoom") == trQuestVarGet("activePlayerCount") - trQuestVarGet("deadPlayerCount") &&
		trQuestVarGet("playersInBossRoom") > 0) {
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
		int id = 0;
		for(x=yGetDatabaseCount("playerUnits"); >0) {
			id = yDatabaseNext("playerUnits", true);
			if (id == -1 || trUnitAlive() == false) {
				removePlayerUnit();
			} else if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Villager Atlantean Hero")) {
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				removePlayerUnit();
			}
		}
		trQuestVarSet("deadPlayerCount", 0);
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

		trQuestVarSet("bossRoomPlayersX", trQuestVarGet("bossRoomCenterX") - 12);
		trQuestVarSet("bossRoomPlayersZ", trQuestVarGet("bossRoomCenterZ") - 12);

		yClearDatabase("playerCharacters");
		for(p=1; < ENEMY_PLAYER) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"unit");
			trUnitDestroy();
			spawnPlayer(p, "bossRoomPlayers");
			equipRelicsAgain(p);
			trQuestVarSet("p"+p+"silenceTimeout", 0);
		}

		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id >= 0 && trUnitAlive()) {
				trUnitDestroy();
			}
		}
		yClearDatabase("enemies");
		yClearDatabase("enemiesIncoming");

		trQuestVarSet("bossPointer", yAddToDatabase("enemies", "bossUnit"));

		zSetProtoUnitStat("Revealer", 1, 2, 32);
		trArmyDispatch("1,0","Revealer",1,trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		trArmySelect("1,0");
		trUnitConvert(0);
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
				trMessageSetText("Use the rocks to take cover from the lion's roar!", -1);
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss2_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("The Wraithwood", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				float angle = 0;
				for(x=7; >0) {
					trVectorSetFromAngle("dir", angle);
					angle = angle + 0.897597;
					trQuestVarSet("dirX", trQuestVarGet("dirX") * 15.0 + trQuestVarGet("bossRoomCenterX"));
					trQuestVarSet("dirZ", trQuestVarGet("dirZ") * 15.0 + trQuestVarGet("bossRoomCenterZ"));
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("dirX"),0,trQuestVarGet("dirZ"),0,true);
					trArmySelect("1,0");
					trUnitConvert(0);
					trUnitChangeProtoUnit("Imperial Examination");
				}
			}
			case 1:
			{
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Spy Eye");
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
				trSetSelectedScale(trQuestVarGet("bossScale"), trQuestVarGet("bossScale"), trQuestVarGet("bossScale"));
				ySetPointer("enemies", 1*trQuestVarGet("bossPointer"));
				yRemoveFromDatabase("enemies");
				trQuestVarSet("bossHealth", 100);
				trCounterAddTime("bosshealth",-1,-9999,"<color={Playercolor(2)}>Wraithwood: 100</color>", -1);

				trModifyProtounit("Shade XP", ENEMY_PLAYER, 0, 6391);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 1, -2.8);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 27, 656);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 28, 566);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 29, 656);
				
				xsEnableRule("boss2_battle");
				trQuestVarSet("bossSpell", 41);
				trQuestVarSet("boss", 1);
				trQuestVarSet("bossGem", SOULSTONE);
				trQuestVarSetFromRand("bossGemCount", 1, 2, true);
				xsEnableRule("boss_music");
				trMessageSetText("Defeat the spawned enemies to damage the Wraithwood!", -1);
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss3_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("King of Ice", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				trRenderSnow(0.2);
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
				spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Cinematic Block"), "auroraSFX");
				spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Cinematic Block"), "iceAgeSFX");
				xsEnableRule("boss3_battle");
				trQuestVarSet("bossGem", MANASTONE);
				trQuestVarSetFromRand("bossGemCount", 1, 2, true);
				xsEnableRule("boss_music");

				trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, 20);
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

void treeStab(string db = "") {
	trVectorQuestVarSet("dir", zGetUnitVector("bossRoomCenter", db));
	trQuestVarSet("startx", trQuestVarGet(db+"x") - trQuestVarGet("dirx") * 4.0);
	trQuestVarSet("startz", trQuestVarGet(db+"z") - trQuestVarGet("dirz") * 4.0);
	trUnitSelectClear();
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",2,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
	trArmySelect("1,0");
	trUnitChangeProtoUnit("Cinematic Block");
	trUnitSelectClear();
	trUnitSelect(""+(1+trQuestVarGet("next")), true);
	trMutateSelected(kbGetProtoUnitID("Tartarian Gate Flame"));
	trUnitSetAnimationPath("0,1,1,0,0,0,0");
	trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
	trSetSelectedScale(0.8,1.0,0.5);
	trQuestVarSet("treeStabSound",1);
	yAddToDatabase("treeStabs", "next");
	yAddUpdateVar("treeStabs", "dirx", trQuestVarGet("dirx"));
	yAddUpdateVar("treeStabs", "dirz", trQuestVarGet("dirz"));
	yAddUpdateVar("treeStabs", "step", 0);
	yAddUpdateVar("treeStabs", "next", trTimeMS() + 2000);
	yAddUpdateVar("treeStabs", "sfx", trQuestVarGet("next") + 1);
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
					if (ySetPointer("playerUnits", 1*yGetVar("bossShockwaveTargets", "index"))) {
						damagePlayerUnit(200);
					}
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
				if (yGetVarAtIndex("enemies", "silenceStatus", 1*trQuestVarGet("bossPointer")) == 0) {
					trQuestVarSet("bossSpell", 0);
					trQuestVarSet("bossUltimate", trQuestVarGet("bossUltimate") - 1);
				}
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
				if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
					removeEnemy();
				}
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
						bossCooldown(7, 20);
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
				activateEnemy("next", 0, 0);
				yAddToDatabase("Sphinxes", "next");
			}
			bossCooldown(7, 20);
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
					bossCooldown(7, 20);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trQuestVarSet("bossAnim", 1);
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
					trQuestVarSet("bossAnim", 0);
					trUnitChangeProtoUnit("Nemean Lion");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trMutateSelected(kbGetProtoUnitID("Nemean Lion"));
					trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
					bossCooldown(7, 20);
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


rule boss2_battle
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
	float dist = 0;
	bool hit = false;
	if (trQuestVarGet("bossHealth") > 0) {
		if (yGetDatabaseCount("bossRainingTrees") > 0) {
			action = processGenericProj("bossRainingTrees");
			if (action == PROJ_GROUND) {
				trUnitSelectClear();
				trUnitSelectByQV("bossRainingTrees");
				trUnitChangeProtoUnit("Walking Woods Marsh");
				trUnitSelectClear();
				trUnitSelectByQV("bossRainingTrees");
				trDamageUnitPercent(-100);
				activateEnemy("bossRainingTrees",-1,0);
				yRemoveFromDatabase("bossRainingTrees");
			}
		}

		if (yGetDatabaseCount("bossHeals") > 0) {
			processGenericProj("bossHeals");
			trVectorSetUnitPos("pos", "bossHeals");
			if (zDistanceBetweenVectorsSquared("pos", "bossRoomCenter") < 12) {
				trUnitSelectClear();
				trUnitSelectByQV("bossHeals");
				trUnitChangeProtoUnit("Regeneration SFX");
				yRemoveFromDatabase("bossHeals");
				trSoundPlayFN("recreation.wav","1",-1,"","");
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				trQuestVarSet("bossHealth", xsMin(100, trQuestVarGet("bossHealth") + 5));
				trCounterAbort("bosshealth");
				trCounterAddTime("bosshealth",-1,-9999,
					"<color={Playercolor(2)}>Wraithwood: "+1*trQuestVarGet("bossHealth")+"</color>", -1);
				trUnitHighlight(0.2,false);
			} else {
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					id = yDatabaseNext("playerUnits", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 4) {
						healUnit(1*yGetVar("playerUnits","player"), 100);
						trUnitSelectClear();
						trUnitSelectByQV("bossHeals");
						trUnitChangeProtoUnit("Hero Death");
						trSoundPlayFN("heal.wav","1",-1,"","");
						yRemoveFromDatabase("bossHeals");
						break;
					}
				}
			}
		}

		if (yGetDatabaseCount("bossRoots") > 0) {
			yDatabaseNext("bossRoots");
			if (trTimeMS() > yGetVar("bossRoots", "timeout")) {
				for(x=trQuestVarGet("bossRoots"); < yGetVar("bossRoots", "last")) {
					trUnitSelectClear();
					trUnitSelect(""+x);
					trDamageUnitPercent(100);
				}
				yRemoveFromDatabase("bossRoots");
			} else {
				yVarToVector("bossRoots", "pos");
				action = 0;
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					id = yDatabaseNext("playerUnits", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						dist = zDistanceToVectorSquared("playerUnits", "pos");
						if (dist > 16 && dist < 36) {
							hit = (trQuestVarGet("stunSound") == 1 || yGetVar("playerUnits", "stunStatus") == 0);
							stunUnit("playerUnits", 1.5);
							if (hit == false) {
								trQuestVarSet("stunSound", 0);
							}
							if (trTimeMS() > yGetVar("bossRoots", "next")) {
								damagePlayerUnit(10);
								action = 1;
							}
						}
					}
				}
				if (action == 1) {
					ySetVar("bossRoots", "next", trTimeMS() + 500);
					trQuestVarSetFromRand("sound", 1, 4, true);
					trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
			}
		}

		for(y=xsMin(9, yGetDatabaseCount("treeStabs")); >0) {
			yDatabaseNext("treeStabs");
			dist = trTimeMS() - yGetVar("treeStabs", "next");
			if (dist > 0) {
				switch(1*yGetVar("treeStabs", "step"))
				{
					case 0:
					{
						yVarToVector("treeStabs", "dir");
						trUnitSelectClear();
						trUnitSelectByQV("treeStabs",true);
						trMutateSelected(kbGetProtoUnitID("Pine Dead"));
						trSetSelectedScale(0.9,dist/750,0.9);
						trSetSelectedUpVector(2.5*trQuestVarGet("dirx"),0,2.5*trQuestVarGet("dirz"));

						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("treeStabs","sfx"),true);
						trUnitChangeProtoUnit("Tartarian Gate flame");
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("treeStabs","sfx"),true);
						trSetSelectedScale(0.8,1.0,0.5);
						trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
						trUnitSetAnimationPath("0,1,1,0,0,0,0");
						ySetVar("treeStabs", "step", 1);
					}
					case 1:
					{
						trUnitSelectClear();
						trUnitSelectByQV("treeStabs",true);
						if (dist > 300) {
							trSetSelectedScale(1,0.4,1);
							ySetVar("treeStabs", "step", 2);
							ySetVar("treeStabs", "next", trTimeMS() + 2000);
							trVectorSetUnitPos("pos", "treeStabs");
							yVarToVector("treeStabs", "dir");
							for(x=yGetDatabaseCount("playerUnits"); >0) {
								id = yDatabaseNext("playerUnits", true);
								if (id == -1 || trUnitAlive() == false) {
									removePlayerUnit();
								} else {
									dist = zDistanceToVector("playerUnits", "pos");
									if (dist < 6) {
										trQuestVarSet("hitboxX", trQuestVarGet("posX") + dist * trQuestVarGet("dirX"));
										trQuestVarSet("hitboxZ", trQuestVarGet("posZ") + dist * trQuestVarGet("dirZ"));
										if (zDistanceToVectorSquared("playerUnits", "hitbox") < 3) {
											if (trQuestVarGet("playerUnits") == trQuestVarGet("p"+trCurrentPlayer()+"unit")) {
												trCameraShake(0.3, 0.8);
											}
											damagePlayerUnit(300);
											trQuestVarSet("treeHitSound", 1);
										}
									}
								}
							}
						} else {
							trSetSelectedScale(1,dist/750,1);
						}
					}
					case 2:
					{
						trUnitSelectClear();
						trUnitSelectByQV("treeStabs", true);
						trSetSelectedScale(1, (1000.0 - dist) / 2500, 1);
						if (dist > 1000) {
							trUnitSelectClear();
							trUnitSelectByQV("treeStabs", true);
							trUnitDestroy();
							yRemoveFromDatabase("treeStabs");
						}
					}
				}
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
				trQuestVarSet("bossSpell", 0);
			}
		} else if (trQuestVarGet("bossSpell") > 40) {
			if (trQuestVarGet("bossSpell") == 41) {
				trQuestVarSet("bossHealth", trQuestVarGet("bossHealth") - 10);
				if (trQuestVarGet("bossHealth") > 0) {
					trSoundPlayFN("walkingwoodsbirth.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 42);
					trQuestVarSet("bossNext", trTimeMS());
					trQuestVarSetFromRand("bossCount", ENEMY_PLAYER, 12, true);
					trQuestVarSetFromRand("bossCount", ENEMY_PLAYER, trQuestVarGet("bossCount"), true);
				}
				trCounterAbort("bosshealth");
				trCounterAddTime("bosshealth",-1,-9999,
					"<color={Playercolor(2)}>Wraithwood: "+1*trQuestVarGet("bossHealth")+"</color>", -1);
			} else if (trQuestVarGet("bossSpell") == 42) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 500);
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					trQuestVarSetFromRand("speed", 5, 15, false);
					trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 2.43));
					trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					addGenericProj("bossRainingTrees", "bossRoomCenter", "dir", kbGetProtoUnitID("Einheriar"),
						18,trQuestVarGet("speed"),0);
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
					if (trQuestVarGet("bossCount") == 0) {
						bossCooldown(7, 12);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSoundPlayFN("underworldpassage.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Wraith Unleashed", 3.0, -1, -1, -1);
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossNext", trTimeMS() + 1500);
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > trQuestVarGet("bossSpell")) {
					trQuestVarSet("bossWraith", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("bossRoomCenterX")-5,0,trQuestVarGet("bossRoomCenterZ")-5,225,true);
					trUnitSelectClear();
					trUnitSelectByQV("bossWraith", true);
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Shade XP");
					trUnitSelectClear();
					trUnitSelectByQV("bossWraith", true);
					trSetSelectedScale(2,2,2);
					yAddToDatabase("enemies", "bossWraith");
					trSetLighting("Fimbulwinter", 3.0);
					trQuestVarSet("bossUltimate", 1);
					bossCooldown(6, 15);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trCameraShake(0.5,0.25);
				trSoundPlayFN("xpack\xcinematics\7_in\bigoltitan.wav","1",-1,"","");
				trQuestVarSet("bossSpell", 22);
				trQuestVarSet("bossNext", trTimeMS());
				trQuestVarSet("bossCount", ENEMY_PLAYER - 1);
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 1000);
					if (trQuestVarGet("p"+1*trQuestVarGet("bossCount")+"dead") == 0) {
						trVectorSetUnitPos("pos", "p"+1*trQuestVarGet("bossCount")+"unit");
						treeStab("pos");
					}
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") == 0) {
						trQuestVarSet("bossSpell", 23);
						trQuestVarSet("bossCount", 32);
						trQuestVarSet("bossRadius", 6);
						trQuestVarSetFromRand("bossAngle", 0, 3.14, false);
						trQuestVarSet("bossNext", trTimeMS() + 2000);
					}
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 200);
					trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 2.45));
					trQuestVarSet("bossRadius", trQuestVarGet("bossRadius") + 0.5);
					trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
					trQuestVarSet("startx", trQuestVarGet("bossRoomCenterX") + trQuestVarGet("dirX") * trQuestVarGet("bossRadius"));
					trQuestVarSet("startz", trQuestVarGet("bossRoomCenterZ") + trQuestVarGet("dirZ") * trQuestVarGet("bossRadius"));
					treeStab("start");
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") == 0) {
						bossCooldown(10, 15);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				if (trQuestVarGet("bossExplain") == 0) {
					trQuestVarSet("bossExplain", 1);
					trMessageSetText("If the white lights reach the Wraithwood, it will heal! Touch them to destroy them!",-1);
				}
				trSoundPlayFN("lapadesconvert.wav","1",-1,"","");
				trQuestVarSet("bossCount", 3 + ENEMY_PLAYER / 2);
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossNext", trTimeMS());
				trQuestVarSetFromRand("bossAngle", 0, 3.141592, false);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSetFromRand("sound",1,3, true);
					trSoundPlayFN("gaiasparkle"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1500);
					trQuestVarSet("bossAngle", trQuestVarGet("bossAngle") + 2.4);
					trVectorSetFromAngle("start", trQuestVarGet("bossAngle"));
					trQuestVarSet("dirx", 0.0 - trQuestVarGet("startx"));
					trQuestVarSet("dirz", 0.0 - trQuestVarGet("startZ"));
					trQuestVarSet("startx", trQuestVarGet("startx") * 25.0 + trQuestVarGet("bossRoomCenterX"));
					trQuestVarSet("startz", trQuestVarGet("startz") * 25.0 + trQuestVarGet("bossRoomCenterZ"));
					vectorSnapToGrid("start");
					addGenericProj("bossHeals","start","dir",kbGetProtoUnitID("Curse SFX"),2,1.5,5);

					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") == 0) {
						bossCooldown(10, 15);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				if (yGetDatabaseCount("bossRoots") < yGetDatabaseCount("playerCharacters")) {
					yDatabaseNext("playerCharacters");
					trVectorSetUnitPos("pos", "playerCharacters");
					vectorSnapToGrid("pos");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",4,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
					angle = 0;
					for(x=trQuestVarGet("next"); < trGetNextUnitScenarioNameNumber()) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trVectorSetFromAngle("dir", angle);
						angle = angle + 0.392699;
						trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
						trUnitConvert(0);
						trUnitChangeProtoUnit("Uproot 4x4");
					}
					yAddToDatabase("bossRoots", "next");
					yAddUpdateVar("bossRoots", "last", trGetNextUnitScenarioNameNumber());
					yAddUpdateVar("bossRoots", "timeout", trTimeMS() + 9000);
					yAddUpdateVar("bossRoots", "posx", trQuestVarGet("posx"));
					yAddUpdateVar("bossRoots", "posz", trQuestVarGet("posz"));
					trSoundPlayFN("hesperidesselect.wav","1",-1,"","");
					bossCooldown(3, 12);
				} else {
					trQuestVarSetFromRand("bossSpell", 1, 2, true);
					trQuestVarSet("bossSpell", 10 * trQuestVarGet("bossSpell") + 1);
				}
			}
		} else {
			if (trCountUnitsInArea(""+1*trQuestVarGet("bosSUnit"),ENEMY_PLAYER,"Walking Woods Marsh", 40) == 0) {
				trQuestVarSet("bossSpell", 41);
			} else {
				action = 100.0 - trQuestVarGet("bossHealth");
				trQuestVarSetFromRand("bossSpell", 0, xsMin(3, action / 20), true);
				trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
				if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
					trQuestVarSetFromRand("bossSpell", 0, 2, true);
					trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
				}
			}
		}

		if (trQuestVarGet("treeStabSound") == 1) {
			trQuestVarSet("treeStabSound", 0);
			trQuestVarSetFromRand("sound", 2, 4, true);
			trSoundPlayFN("minionbirth"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}

		if (trQuestVarGet("treeHitSound") == 1) {
			trQuestVarSet("treeHitSound", 0);
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
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

rule boss3_battle
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
	float amt = 0;
	float angle = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		for(x=xsMin(5, yGetDatabaseCount("bossBreath")); >0) {
			processGenericProj("bossBreath");
			trVectorSetUnitPos("pos","bossBreath");
			vectorToGrid("pos","loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				trUnitChangeProtoUnit("Curse SFX");
				yRemoveFromDatabase("bossBreath");
			} else {
				action = 0;
				for(y=yGetDatabaseCount("playerUnits"); >0) {
					id = yDatabaseNext("playerUnits", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 3) {
						stunUnit("playerUnits", 3.0);
						damagePlayerUnit(75);
						action = 1;
					}
				}
				if (action == 1) {
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath", true);
					trUnitChangeProtoUnit("Curse SFX");
					yRemoveFromDatabase("bossBreath");
				}
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trUnitPercentDamaged() > yGetDatabaseCount("frostGiantsIncoming")) {
			if (trTimeMS() > trQuestVarGet("bossSummonTime")) {
				trQuestVarSet("bossSummonTime", trTimeMS() + 1000 * yGetDatabaseCount("frostGiantsIncoming"));
				trQuestVarSetFromRand("xMod", 0 - 2*trQuestVarGet("bossRoomSize"), 2*trQuestVarGet("bossRoomSize"), true);
				trQuestVarSet("leftover", xsSqrt(xsPow(2*trQuestVarGet("bossRoomSize"),2) - xsPow(trQuestVarGet("xMod"), 2)));
				trQuestVarSetFromRand("zMod", 0 - trQuestVarGet("leftover"), trQuestVarGet("leftover"), true);
				trQuestVarSet("posX", trQuestVarGet("bossRoomCenterX") + trQuestVarGet("xMod"));
				trQuestVarSet("posZ", trQuestVarGet("bossRoomCenterZ") + trQuestVarGet("zMod"));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trQuestVarSetFromRand("heading", 1, 360, true);
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),trQuestVarGet("heading"),true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitConvert(0);
				trMutateSelected(kbGetProtoUnitID("Ice Block"));
				yAddToDatabase("frostGiantsIncoming", "next");
				yAddUpdateVar("frostGiantsIncoming", "posX", trQuestVarGet("posX"));
				yAddUpdateVar("frostGiantsIncoming", "posZ", trQuestVarGet("posZ"));
				yAddUpdateVar("frostGiantsIncoming", "targetSize", 2);
				yAddUpdateVar("frostGiantsIncoming", "targetTime", trTimeMS() + 1000);
			}
		}
		for (x = xsMin(5,yGetDatabaseCount("frostGiantsIncoming")); > 0) {
			yDatabaseNext("frostGiantsIncoming", true);
			amt = yGetVar("frostGiantsIncoming", "targetTime") - trTimeMS();
			if (amt > 0) {
				amt = yGetVar("frostGiantsIncoming", "targetSize") - amt * 0.001;
				trSetSelectedScale(amt * 0.5, amt, amt * 0.5);
			} else if (yGetVar("frostGiantsIncoming", "targetSize") >= 5) {
				trUnitChangeProtoUnit("Frost Giant");
				trUnitSelectClear();
				trUnitSelectByQV("frostGiantsIncoming");
				trUnitConvert(ENEMY_PLAYER);
				activateEnemy("frostGiantsIncoming",0,0);
				yAddToDatabase("frostGiants", "frostGiantsIncoming");
				trSoundPlayFN("mythcreate.wav","1",-1,"","");
				yRemoveFromDatabase("frostGiantsIncoming");
			} else {
				amt = yGetVar("frostGiantsIncoming", "targetSize");
				trSetSelectedScale(amt * 0.5, amt, amt * 0.5);
			}
		}

		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
				if (yGetVarAtIndex("enemies", "silenceStatus", 1*trQuestVarGet("bossPointer")) == 0) {
					trQuestVarSet("bossSpell", 0);
					trQuestVarSet("bossUltimate", trQuestVarGet("bossUltimate") - 1);
				}
			} else if (trUnitPercentDamaged() > trQuestVarGet("bossDamaged")) {
				trQuestVarSet("bossDamaged", trQuestVarGet("bossDamaged") + 3);
				trQuestVarSet("bossCooldownTime", trQuestVarGet("bossCooldownTime") - 1000);
			}
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trOverlayText("Ice Age",3.0,-1,-1,-1);
				trSetLighting("night", 1.0);
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossNext", trTimeMS() + 1500);
				trQuestVarSet("bossTimeout", trTimeMS() + 15000);
				if (kbGetBlockID(""+1*trQuestVarGet("iceAgeSFX")) >= 0) {
					trUnitSelectClear();
					trUnitSelectByQV("iceAgeSFX");
					trUnitChangeProtoUnit("Ice Sheet");
				} else {
					spyEffect(1*trQuestVarGet("bossUnit"),kbGetProtoUnitID("Ice Sheet"),"iceAgeSFX");
				}
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trSoundPlayFN("pegasusflap.wav","1",-1,"","");
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1500);
					trVectorSetUnitPos("pos", "bossUnit");
					for(x=yGetDatabaseCount("playerCharacters"); >0) {
						id = yDatabaseNext("playerCharacters");
						if (id == -1 || trUnitAlive() == false) {
							removePlayerCharacter();
						} else {
							trVectorSetUnitPos("target", "playerCharacters");
							trVectorQuestVarSet("dir", zGetUnitVector("pos", "target"));
							addGenericProj("MedusaBalls","pos","dir",kbGetProtoUnitID("Lampades"),37,4,4.7);
		                    yAddUpdateVar("MedusaBalls", "target", trQuestVarGet("playerCharacters"));
		                    yAddUpdateVar("MedusaBalls", "bounces", 10);
						}
					}
					if (trTimeMS() > trQuestVarGet("bossTimeout")) {
						bossCooldown(12, 18);
						trQuestVarSet("bossUltimate", 3);
						trSetLighting("default",1.0);
						trUnitSelectClear();
						trUnitSelectByQV("iceAgeSFX");
						trUnitChangeProtoUnit("Cinematic Block");
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trQuestVarSetFromRand("rand", 1, 3, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Now I'm angry!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Meet my fist!");
				}
				trSoundPlayFN("titangrunt2.wav","1",-1,"","");
				trQuestVarSet("bossScale", 2);
				trSetSelectedScale(2, 2, 2);
				trQuestVarSet("bossSpell", 22);
				trQuestVarSet("bossTimeout", trTimeMS() + 12000);
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, 100);
			} else if (trQuestVarGet("bossSpell") == 22) {
				id = kbGetBlockID(""+1*trQuestVarGet("bossUnit"));
				if (trQuestVarGet("bossAttacking") == 0) {
					if (kbUnitGetAnimationActionType(id) == 6) {
						trQuestVarSet("bossAttacking", 1);
						trQuestVarSet("bossNext", trTimeMS() + 1000);
					}
				} else if (kbUnitGetAnimationActionType(id) == 6) {
					if (trTimeMS() > trQuestVarGet("bossNext")) {
						xsSetContextPlayer(ENEMY_PLAYER);
						trVectorSetUnitPos("start", "bossUnit");
						trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 2000);
						trQuestVarSet("target", kbUnitGetTargetUnitID(id));
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							if (1*trQuestVarGet("target") == yDatabaseNext("playerUnits", true)) {
								if (trUnitAlive()) {
									trVectorSetUnitPos("end", "playerUnits");
									vectorSetAsTargetVector("dest","start","end",100.0);
									launchUnit("playerUnits","dest");
									growFrostGiantsIncoming("end");
								}
								break;
							}
						}
					}
				} else {
					trQuestVarSet("bossAttacking", 0);
				}
				if (trTimeMS() > trQuestVarGet("bossTimeout")) {
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, -100);
					trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
					trQuestVarSet("bossScale", 1.25);
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trSetSelectedScale(1.25,1.25,1.25);
					bossCooldown(6, 12);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			/* breath attack */
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSetFromRand("rand", 1, 3, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Feel the cold!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Chill!");
				}
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 55, 2);
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				for(x=yGetDatabaseCount("playerCharacters"); >0) {
					yDatabaseNext("playerCharacters", true);
					if (id == -1 || trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						trVectorSetUnitPos("end", "playerCharacters");
						break;
					}
				}
				trVectorSetUnitPos("start", "bossUnit");
				vectorSnapToGrid("start");
				trQuestVarSet("bossAngle", angleBetweenVectors("start", "end"));
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				trMutateSelected(kbGetProtoUnitID("King Folstag"));
				trVectorQuestVarSet("bossDir", zGetUnitVector("start", "end"));
				trSetUnitOrientation(trVectorQuestVarGet("bossDir"), vector(0,1,0), true);
				trUnitOverrideAnimation(3,1,false,false,-1);
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossNext", trTimeMS() + 2000);
				trQuestVarSet("arm1", trGetNextUnitScenarioNameNumber());
				trQuestVarSet("arm2", trGetNextUnitScenarioNameNumber()+1);
				trArmyDispatch("1,0","Dwarf",2,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
				trArmySelect("1,0");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(xsVectorSet(0.0-trQuestVarGet("bossDirx"),0,0.0-trQuestVarGet("bossDirz")),vector(0,1,0),true);
				trSetSelectedScale(2,0,30);
				trUnitHighlight(2.0, false);
				trQuestVarSet("bossAnim", 1);
			} else if ((trQuestVarGet("bossSpell") == 12)) {
				amt = trQuestVarGet("bossNext") - trTimeMS();
				if (amt < 0) {
					trQuestVarSet("bossSpell", 13);
					trQuestVarSet("bossNext", trTimeMS()+1);
					trUnitOverrideAnimation(-1,1,false,true,-1);
					trUnitSelectClear();
					trUnitSelectByQV("arm1");
					trUnitSelectByQV("arm2");
					trUnitDestroy();
				} else {
					/* 0.3 is max. amt is 0 - 2000. 0.3 at 0 and 0 at 2000 */
					angle = 0.00015 * (2000 - amt);
					amt = amt * 0.001;
					trUnitSelectClear();
					trUnitSelectByQV("arm1", true);
					trSetSelectedScale(amt, 0, 30);
					trVectorSetFromAngle("dir", fModulo(6.283185, trQuestVarGet("bossAngle") - angle - 3.141592));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("arm2", true);
					trSetSelectedScale(amt, 0, 30);
					trVectorSetFromAngle("dir", fModulo(6.283185, trQuestVarGet("bossAngle") + angle - 3.141592));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trUnitOverrideAnimation(40,0,false,false,-1);
					trVectorSetUnitPos("bossPos", "bossUnit");
					trQuestVarSet("bossSpell", 14);
					trQuestVarSet("bossTimeout", trTimeMS() + 2100);
					trQuestVarSet("bossNext", trTimeMS() + 500);
				}
			} else if (trQuestVarGet("bossSpell") == 14) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 100);
					trQuestVarSetFromRand("newAngle", -0.3, 0.3, false);
					trQuestVarSet("newAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + trQuestVarGet("newAngle")));
					trVectorSetFromAngle("dir", trQuestVarGet("newAngle"));
					addGenericProj("bossBreath","bossPos","dir",kbGetProtoUnitID("Curse SFX"),2,10.0,4.5);
					if (trTimeMS() > trQuestVarGet("bossTimeout")) {
						trModifyProtounit("King Folstag", ENEMY_PLAYER, 55, 1);
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trUnitOverrideAnimation(-1,1,false,true,-1);
						trQuestVarSet("bossAnim", 0);
						bossCooldown(5, 14);
					}
				}
			}
			if (trQuestVarGet("bossSpell") > 11) {
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				trSetUnitOrientation(trVectorQuestVarGet("bossDir"), vector(0,1,0), true);
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			trQuestVarSet("iceKingChange", trQuestVarGet("iceKingChange") - 1);
			if (trQuestVarGet("iceKingChange") <= 0) {
				trQuestVarSet("iceKingChange", 2);
				trQuestVarSet("iceKingArmor", 1 - trQuestVarGet("iceKingArmor"));
				if (trQuestVarGet("iceKingArmor") == 0) {
					trSoundPlayFN("icemono.wav","1",-1,"","");
					trSoundPlayFN("bronzebirth.wav","1",-1,"","");
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 24, 1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 25, 1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 26, 1);
					ySetVarAtIndex("enemies", "physicalResist", 1, 1*trQuestVarGet("bossPointer"));
					ySetVarAtIndex("enemies", "magicResist", 0, 1*trQuestVarGet("bossPointer"));
					trQuestVarSetFromRand("rand", 1, 3, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Your blades won't reach me!");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Ice Armor!");
					}
					if (trQuestVarGet("bossExplain") < 2) {
						trMessageSetText("The King of Ice is now resistant to attacks but vulnerable to spells.",-1);
						trQuestVarSet("bossExplain", 1 + trQuestVarGet("bossExplain"));
					}
					trUnitSelectClear();
					trUnitSelectByQV("auroraSFX");
					trMutateSelected(kbGetProtoUnitID("Ice Block"));
					trUnitSetAnimationPath("0,0,0,0,0,0,0");
					trSetSelectedScale(5,7,5);
				} else {
					trSoundPlayFN("sphinxteleportin.wav","1",-1,"","");
					trSoundPlayFN("godpower.wav","1",-1,"","");
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 24, -1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 25, -1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 26, -1);
					ySetVarAtIndex("enemies", "physicalResist", 0, 1*trQuestVarGet("bossPointer"));
					ySetVarAtIndex("enemies", "magicResist", 1, 1*trQuestVarGet("bossPointer"));
					trQuestVarSetFromRand("rand", 1, 3, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Your magic is meaningless!");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "King of Ice: Aurora Armor!");
					}
					if (trQuestVarGet("bossExplain") < 2) {
						trMessageSetText("The King of Ice is now resistant to spells but vulnerable to attacks.",-1);
						trQuestVarSet("bossExplain", 1 + trQuestVarGet("bossExplain"));
					}
					if (kbGetBlockID(""+1*trQuestVarGet("auroraSFX")) >= 0) {
						trUnitSelectClear();
						trUnitSelectByQV("auroraSFX");
						trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					} else {
						spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Cinematic Block"), "auroraSFX");
					}
				}
				bossCooldown(5, 10);
			} else {
				trQuestVarSet("bossSpell", 11);
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}

		if (trQuestVarGet("frostGiantIncomingSound") == 1) {
			trQuestVarSet("frostGiantIncomingSound", 0);
			trSoundPlayFN("icemono.wav","1",-1,"","");
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
				trQuestVarSet("gameOverNext", trTime() + 5);
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
				trQuestVarSet("bossKills", 1 + trQuestVarGet("bossKills"));
			}
		}
		gadgetUnreal("ShowImageBox-CloseButton");
	}
}