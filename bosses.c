const int BOSS_SPELL_COOLDOWN = -1;

void bossCooldown(int minVal = 0, int maxVal = 0) {
	trQuestVarSet("bossSpell", BOSS_SPELL_COOLDOWN);
	trQuestVarSetFromRand("bossCooldownTime", minVal, maxVal, true);
	trQuestVarSet("bossCooldownTime", trTimeMS() + 1000 * trQuestVarGet("bossCooldownTime"));
	trQuestVarSet("bossOriginalCooldownTime", trQuestVarGet("bossCooldownTime"));
}

void processBossCooldown(int ultimate = 31) {
	float diff = trTimeMS() - trQuestVarGet("bossCooldownLastCheck");
	trQuestVarSet("bossCooldownLastCheck", trTimeMS());
	if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
		trQuestVarSet("bossSpell", 0);
		if (trQuestVarGet("bossUltimate") > 0) {
			trQuestVarSet("bossUsedUltimate", 1);
			trQuestVarSet("bossUltimate", trQuestVarGet("bossUltimate") - 1);
		}
	} else if (trUnitPercentDamaged() > 70 && trQuestVarGet("bossUsedUltimate") == 0) {// boss hasn't used ultimate yet
		trQuestVarSet("bossSpell", ultimate);
	} else if (trUnitPercentDamaged() > trQuestVarGet("bossDamaged")) {
		trQuestVarSet("bossDamaged", trQuestVarGet("bossDamaged") + 3);
		trQuestVarSet("bossCooldownTime", trQuestVarGet("bossCooldownTime") - 1000);
	} else if (trTimeMS() > trQuestVarGet("bossOriginalCooldownTime")) {
		diff = 0.0001 * diff * (trQuestVarGet("bossCooldownTime") - trQuestVarGet("bossOriginalCooldownTime"));
		trQuestVarSet("bossCooldownTime", trQuestVarGet("bossCooldownTime") - diff);
	}
}

rule yeeb_boss_message
inactive
highFrequency
{
	bool found = false;
	if (trTime() > trQuestVarGet("yeebNext")) {
		trQuestVarSet("yeebStep", 1 + trQuestVarGet("yeebStep"));
		switch(1*trQuestVarGet("yeebStep"))
		{
			case 1:
			{
				trSetLighting("anatolia", 1.0);
				if (trQuestVarGet("p"+trCurrentPlayer()+"yeebHit") == 1) {
					trSoundPlayFN("","1",-1,"Yeebaagooon: Oh, " + trStringQuestVarGet("p"+trCurrentPlayer()+"name")+" you utter fool.",
						"icons\special e son of osiris icon 64");
				} else {
					trSoundPlayFN("","1",-1,
						"Yeebaagooon: Oh, " + trStringQuestVarGet("p"+1*trQuestVarGet("yeebBossFight")+"name")+" you utter fool.",
						"icons\special e son of osiris icon 64");
				}
				trQuestVarSet("yeebNext", trTime() + 6);
			}
			case 2:
			{
				trSoundPlayFN("","1",-1,"Yeebaagooon: Did you really think you could escape me by resigning before the bossfight?",
					"icons\special e son of osiris icon 64");
				trQuestVarSet("yeebNext", trTime() + 6);
			}
			case 3:
			{
				trSoundPlayFN("","1",-1,"Yeebaagooon: Foolish. I'll be taking back what's mine, no matter what.",
					"icons\special e son of osiris icon 64");
				trQuestVarSet("yeebNext", trTime() + 6);
			}
			case 4:
			{
				trArmyDispatch("1,0","Revealer",1,41,0,41,0,true);
				zSetProtoUnitStat("Revealer", 1, 2, 30);
				trArmySelect("1,0");
				trUnitConvert(0);
				
				trQuestVarSet("yeebaagooon", trGetNextUnitScenarioNameNumber());
				trQuestVarSet("bossUnit", trQuestVarGet("yeebaagooon"));
				trArmyDispatch("1,0","Dwarf",1,55,0,55,225,true);
				trUnitSelectClear();
				trUnitSelectByQV("yeebaagooon", true);
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Pharaoh of Osiris XP");
				
				spyEffect(1*trQuestVarGet("yeebaagooon"), kbGetProtoUnitID("Cinematic Block"), "yeebShieldSFX");
				
				activateEnemy("bossUnit");
				yAddUpdateVar("enemies", "launched", 1);
				trQuestVarSet("bossPointer", yGetNewestPointer("enemies"));
				
				trVectorQuestVarSet("bossRoomUpper", xsVectorSet(60,0,60));
				if (trQuestVarGet("mapType") == MAP_OPEN) {
					trVectorQuestVarSet("bossRoomLower", xsVectorSet(10, 0, 20));
				} else {
					trVectorQuestVarSet("bossRoomLower", xsVectorSet(20, 0, 10));
				}
				
				trQuestVarSet("yeebNextInvulnerabilityPhase", 30);
				
				xsEnableRule("yeebaagooon_battle");
				xsEnableRule("boss_stun_recovery");
				
				trOverlayText("Yeebaagooon",3.0,-1,-1,-1);
				
				trQuestVarSet("lightningBolts", trGetNextUnitScenarioNameNumber());
				
				bossCooldown(10, 12);
				
				xsEnableRule("gameplay_start");
				trUIFadeToColor(0,0,0,1000,0,false);
				trLetterBox(false);
				trQuestVarSet("yeebNext", trTime() + 3);
				xsEnableRule("boss_music");
				trQuestVarSet("boss", 1);
				trQuestVarSet("yeebPosx", trQuestVarGet("startPositionx"));
				trQuestVarSet("yeebPosz", trQuestVarGet("startPositionz"));
			}
			case 5:
			{
				trMessageSetText("Yeebaagooon's spells will drain your favor if they hit you!", -1);
				xsDisableSelf();
				
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"yeebHit") == 1) {
						found = false;
						for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
							yDatabaseNext("p"+p+"relics");
							if (yGetVar("p"+p+"relics", "type") == RELIC_YEEBAAGOOON) {
								found = true;
								yAddToDatabase("yeebRelics", "p"+p+"relics");
								break;
							}
						}
						if (found == false) {
							if (trCurrentPlayer() == p) {
								trQuestVarSet("ownedRelics"+RELIC_YEEBAAGOOON, trQuestVarGet("ownedRelics"+RELIC_YEEBAAGOOON) - 1);
								trQuestVarSet("yeebRelicRetrieved", 1);
							}
						}
					}
				}
			}
		}
	}
}

rule boss_entrance_always
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossEntranceStatue");
	if (trUnitIsSelected()) {
		startNPCDialog(NPC_BOSS_ENTRANCE);
		reselectMyself();
	}
	trVectorSetUnitPos("pos", "bossKey");
	if (zDistanceToVectorSquared("bossEntranceStatue", "pos") < 25) {
		trUnitSelectClear();
		trUnitSelect(""+1*trQuestVarGet("bossKey"));
		trUnitChangeProtoUnit("Osiris Box Glow");
		trUnitSelectClear();
		trUnitSelect(""+1*trQuestVarGet("bossEntranceSymbol"));
		trUnitChangeProtoUnit("Rocket");
		trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
		trMessageSetText("The boss room entrance has opened!", -1);
		trVectorSetUnitPos("pos", "bossEntranceStatue");
		trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")-8,0,trQuestVarGet("posZ")-8,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Vortex Landing");
		trQuestVarSet("bossRoomEntranceX", trQuestVarGet("posX") - 8);
		trQuestVarSet("bossRoomEntranceZ", trQuestVarGet("posZ") - 8);
		xsDisableSelf();
	}
}

rule enter_boss_room
inactive
highFrequency
{
	for(p=1; < ENEMY_PLAYER) {
		trUnitSelectClear();
		if (zDistanceToVectorSquared("p"+p+"unit", "bossRoomEntrance") < trQuestVarGet("bossEntranceRadius")) {
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
			switch(1*trQuestVarGet("music"))
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
		trQuestVarSet("bossRoomEntranceX", trQuestVarGet("bossRoomCenterX") / 2 - trQuestVarGet("bossRoomSize"));
		trQuestVarSet("bossRoomEntranceZ", trQuestVarGet("bossRoomCenterZ") / 2 - trQuestVarGet("bossRoomSize"));
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") - 3,
			trQuestVarGet("bossRoomEntranceX") + 35, trQuestVarGet("bossRoomEntranceZ"),
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") - 3,
			trQuestVarGet("bossRoomEntranceX") + 35, trQuestVarGet("bossRoomEntranceZ"),wallHeight,false);
		trPaintTerrain(trQuestVarGet("bossRoomEntranceX")-4, trQuestVarGet("bossRoomEntranceZ"),
			trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") + 35,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX") - 3, trQuestVarGet("bossRoomEntranceZ"),
			trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") + 35,wallHeight,false);
		
		trPaintTerrain(0,0,5,5,0,70,true);
		trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
		
		trQuestVarSet("bossUnit", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("0,0","Dwarf",1,
			trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit(trStringQuestVarGet("bossProto"));
		
		trQuestVarSet("bossID", kbGetBlockID(""+1*trQuestVarGet("bossUnit")));
		
		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		
		xsEnableRule("boss"+1*trQuestVarGet("stage")+"_init");
		
		trQuestVarSet("boss", trQuestVarGet("stage"));
		
		trQuestVarSet("bossRoomPlayersX", trQuestVarGet("bossRoomCenterX") - 12);
		trQuestVarSet("bossRoomPlayersZ", trQuestVarGet("bossRoomCenterZ") - 12);
		
		yClearDatabase("playerCharacters");
		for(p=1; < ENEMY_PLAYER) {
			if (trQuestVarGet("p"+p+"resigned") == 0) {
				for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
					yDatabaseNext("p"+p+"relics", true);
					trUnitChangeProtoUnit("Relic");
				}
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"unit");
				trUnitDestroy();
				spawnPlayer(p, "bossRoomPlayers");
				equipRelicsAgain(p);
				trPlayerKillAllGodPowers(p);
				trQuestVarSet("p"+p+"dead", 0);
				trQuestVarSet("p"+p+"silenced", 1);
				trQuestVarSet("p"+p+"silenceTimeout", 0);
			}
		}
		
		for(x=yGetDatabaseCount("enemies"); >0) {
			id = yDatabaseNext("enemies", true);
			if (id >= 0 && trUnitAlive()) {
				trUnitDestroy();
			}
		}
		yClearDatabase("enemies");
		yClearDatabase("enemiesIncoming");
		
		activateEnemy("bossUnit", 0, 0);
		trQuestVarSet("bossPointer", yGetNewestPointer("enemies"));
		
		zSetProtoUnitStat("Revealer", 1, 2, 32);
		trArmyDispatch("1,0","Revealer",1,trQuestVarGet("bossRoomCenterX"),0,trQuestVarGet("bossRoomCenterZ"),225,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		uiLookAtUnitByName(""+1*trQuestVarGet("bossUnit"));
		
		yClearDatabase("freeRelics");
		
		if (trQuestVarGet("stage") > 3) {
			xsEnableRule("boss_stun_recovery");
		}
	}
}

rule boss_stun_recovery
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("bossStunRecoveryLast")) {
		trQuestVarSet("bossStunRecoveryLast", trTime());
		int pointer = yGetPointer("enemies");
		if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
			if (yGetVar("enemies", "stunStatus") == 1) {
				trQuestVarSet("bossStunResistance", 1 + trQuestVarGet("bossStunResistance"));
			} else if (trQuestVarGet("bossStunResistance") > 0) {
				trQuestVarSet("bossStunResistance", trQuestVarGet("bossStunResistance") - 1);
			}
			trQuestVarSet("p0stunResistance", xsPow(0.9, trQuestVarGet("bossStunResistance")));
			ySetPointer("enemies", pointer);
		}
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
				trQuestVarSetFromRand("bossGemCount", 2, 3, true);
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
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 27, 656);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 28, 566);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 29, 656);
				
				xsEnableRule("boss2_battle");
				trQuestVarSet("bossSpell", 41);
				trQuestVarSet("boss", 1);
				trQuestVarSet("bossGem", SOULSTONE);
				trQuestVarSetFromRand("bossGemCount", 2, 3, true);
				xsEnableRule("boss_music");
				trMessageSetText("Defeat the spawned enemies to damage the Wraithwood!", -1);
				trStringQuestVarSet("advice",
					"The Wraithwood only summons new enemies once the old ones are gone. Make sure to kill the spawns!");
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
				//trRenderSnow(0.2);
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
				trQuestVarSetFromRand("bossGemCount", 2, 3, true);
				xsEnableRule("boss_music");
				
				bossCooldown(10, 15);
				
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, 20);
				
				trStringQuestVarSet("advice",
					"If anything is stunned near an icicle, it will grow. Big icicles will turn into Frost Giants!");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss6_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				if (trQuestVarGet("yeebBossFight") == 1) {
					yAddToDatabase("yeebRelics", "yeebRelic");
					trQuestVarSet("cinTime", trTime() + 6);
					trUnitSelectClear();
					trUnitSelectByQV("yeebRelic", true);
					for(p=1; < ENEMY_PLAYER) {
						if (trUnitIsOwnedBy(p)) {
							trSoundPlayFN("","1",-1,
								"Yeebaagooon:" + trStringQuestVarGet("p"+p+"name") + ". Did you really think I wouldn't notice you stealing my relic?",
								"icons\special e son of osiris icon 64");
							break;
						}
					}
					trQuestVarSet("cinStep", 1);
				} else {
					trOverlayText("The Exterminator", 3.0, -1, -1, -1);
					trQuestVarSet("cinTime", trTime() + 2);
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit", true);
					trUnitConvert(ENEMY_PLAYER);
					trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
					spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Helepolis"), "biggerHelepolis");
					spyEffect(1*trQuestVarGet("bossUnit"), kbGetProtoUnitID("Barracks Atlantean"), "biggerBallista");
					ySetVarAtIndex("enemies", "launched", 1, 1*trQuestVarGet("bossPointer"));
					
					trQuestVarSet("bossTurretObject", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
					trQuestVarSet("bossTurret", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
					
					trQuestVarSet("bossBarrelObject", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
					trQuestVarSet("bossBarrel", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
					
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel", true);
					trUnitConvert(ENEMY_PLAYER);
					trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
					trUnitSelectClear();
					trUnitSelectByQV("bossTurret", true);
					trUnitConvert(ENEMY_PLAYER);
					trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
					
					trUnitSelectClear();
					trUnitSelectByQV("bossTurretObject", true);
					trUnitConvert(ENEMY_PLAYER);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossTurret"));
					trMutateSelected(kbGetProtoUnitID("Fire Siphon"));
					
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrelObject", true);
					trUnitConvert(ENEMY_PLAYER);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBarrel"));
					trMutateSelected(kbGetProtoUnitID("Torch"));
					trUnitSetAnimationPath("1,0,0,0,0,0,0");
					trSetSelectedScale(1.5, 1.0, 1.5);
					
					
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel", true);
					trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					trUnitSelectClear();
					trUnitSelectByQV("bossTurret", true);
					trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					
					trQuestVarSet("volcano1", trGetNextUnitScenarioNameNumber());
					trQuestVarSet("volcano2", 1 + trQuestVarGet("volcano1"));
					trArmyDispatch("1,0","Dwarf",2,trQuestVarGet("bossRoomCenterx"),0,trQuestVarGet("bossRoomCenterZ"),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Cinematic Block");
				}
			}
			case 1:
			{
				bossCooldown(10, 15);
				trSetLighting("anatolia",0.1);
				yAddToDatabase("ballistas", "bossUnit");
				yAddUpdateVar("ballistas", "index", trQuestVarGet("bossPointer"));
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelectByQV("biggerHelepolis", true);
				trSetSelectedScale(1.2,0.25,2.0);
				trUnitSelectClear();
				trUnitSelectByQV("biggerBallista", true);
				trSetSelectedScale(0.7,0.5,0.5);
				xsEnableRule("boss6_battle");
				trQuestVarSet("bossGem", MANASTONE);
				trQuestVarSetFromRand("bossGemCount", 3, 4, true);
				xsEnableRule("boss_music");
				
				trModifyProtounit("Helepolis", ENEMY_PLAYER, 27, 20);
			}
			case 2:
			{
				trQuestVarSet("cinTime", trTime() + 6);
				trSoundPlayFN("","1",-1,"Yeebaagooon: I'll be taking back what's mine now... along with your LIFE!",
					"icons\special e son of osiris icon 64");
				trQuestVarSet("bossScale", 1);
				trQuestVarSet("yeebaagooon", trQuestVarGet("bossUnit"));
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitChangeProtoUnit("Pharaoh of Osiris XP");
				ySetVarAtIndex("enemies", "launched", 1, 1*trQuestVarGet("bossPointer"));
				ySetVarAtIndex("enemies", "physicalResist", 0.47, 1*trQuestVarGet("bossPointer"));
				ySetVarAtIndex("enemies", "magicResist", 0.47, 1*trQuestVarGet("bossPointer"));
				
				spyEffect(1*trQuestVarGet("yeebaagooon"), kbGetProtoUnitID("Cinematic Block"), "yeebShieldSFX");
				
				trQuestVarSet("bossRoomUpperX", trQuestVarGet("bossRoomCenterX") + trQuestVarGet("bossRoomSize"));
				trQuestVarSet("bossRoomUpperz", trQuestVarGet("bossRoomCenterZ") + trQuestVarGet("bossRoomSize"));
				trQuestVarSet("bossRoomLowerX", trQuestVarGet("bossRoomCenterX") + trQuestVarGet("bossRoomSize"));
				trQuestVarSet("bossRoomLowerZ", trQuestVarGet("bossRoomCenterZ") + trQuestVarGet("bossRoomSize"));
				
				trQuestVarSet("yeebNextInvulnerabilityPhase", 30);
				
				trQuestVarSet("yeebPosx", trQuestVarGet("bossRoomCenterX"));
				trQuestVarSet("yeebPosz", trQuestVarGet("bossRoomCenterZ"));
				
				trOverlayText("Yeebaagooon",3.0,-1,-1,-1);
				
				trQuestVarSet("lightningBolts", trGetNextUnitScenarioNameNumber());
			}
			case 3:
			{
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitConvert(ENEMY_PLAYER);
				xsEnableRule("yeebaagooon_battle");
				xsEnableRule("boss_stun_recovery");
				bossCooldown(10, 12);
				
				trUIFadeToColor(0,0,0,1000,0,false);
				trLetterBox(false);
				xsEnableRule("boss_music");
				trQuestVarSet("cinTime", trTime() + 3);
			}
			case 4:
			{
				trMessageSetText("Yeebaagooon's spells will drain your favor if they hit you.", -1);
				xsDisableSelf();
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}


rule boss4_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("Escaped Amalgam", 3.0, -1, -1, -1);
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
				xsEnableRule("boss4_battle");
				trQuestVarSet("bossGem", STARSTONE);
				trQuestVarSetFromRand("bossGemCount", 3, 4, true);
				xsEnableRule("boss_music");
				
				bossCooldown(10, 15);
				
				trModifyProtounit("Chimera", ENEMY_PLAYER, 27, 20);
				
				zSetProtoUnitStat("Meteorite", ENEMY_PLAYER, 1, 6);
				
				trStringQuestVarSet("advice", "Try bringing poison and silence resistance!");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss5_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("The Lonely Ghost", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
			}
			case 1:
			{
				trQuestVarSet("bossChoke", 10);
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(trQuestVarGet("bossScale"), trQuestVarGet("bossScale"), trQuestVarGet("bossScale"));
				xsEnableRule("boss5_battle");
				trQuestVarSet("bossGem", SOULSTONE);
				trQuestVarSetFromRand("bossGemCount", 3, 4, true);
				xsEnableRule("boss_music");
				
				trQuestVarSet("bossWarn1", trGetNextUnitScenarioNameNumber());
				trQuestVarSet("bossWarn2", 1 + trQuestVarGet("bossWarn1"));
				trArmyDispatch("1,0","Dwarf",2,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Cinematic Block");
				
				trQuestVarSet("bossScreamStart", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",7,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Cinematic Block");
				trQuestVarSet("bossScreamEnd", trGetNextUnitScenarioNameNumber());
				
				trQuestVarSet("bossEscape", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				
				bossCooldown(10, 15);
				
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 27, 20);
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 55, 4);
				
				trStringQuestVarSet("advice", "Try bringing poison resistance!");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}


rule boss7_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("Mother of the Depths", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				xsDisableRule("deep_village_always");
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
				xsEnableRule("boss7_battle");
				trQuestVarSet("bossGem", SOULSTONE);
				trQuestVarSetFromRand("bossGemCount", 4, 5, true);
				xsEnableRule("boss_music");
				
				for(i=0; < 50) {
					trQuestVarSet("jumpPath"+i, trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,1,0,1,0,true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					trUnitChangeProtoUnit("Cinematic Block");
				}
				
				trQuestVarSet("bossEscape", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,1,0,1,0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Transport Ship Greek");
				trArmySelect(""+ENEMY_PLAYER+",0");
				trSetSelectedScale(0,0,0);
				bossCooldown(10, 15);
				
				trModifyProtounit("Scylla", ENEMY_PLAYER, 27, 50);
				ySetVarAtIndex("enemies", "launched", 1, 1*trQuestVarGet("bossIndex"));
				
				trQuestVarSet("secondPhase", 1);
				
				trStringQuestVarSet("advice",
					"Watch out when she tries to eat you. She will spawn allies if she eats someone!");
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
			processBossCooldown();
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
				trArmyDispatch(""+ENEMY_PLAYER+",0","Sphinx",1,
					trQuestVarGet("dirx"),0,trQuestVarGet("dirz"),trQuestVarGet("heading"),true);
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
		trUnitOverrideAnimation(-1,0,false,true,-1);
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
		trUnitOverrideAnimation(-1,0,false,true,-1);
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
			processBossCooldown();
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
							yAddUpdateVar("MedusaBalls","player",ENEMY_PLAYER);
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
		trUnitOverrideAnimation(-1,0,false,true,-1);
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


rule boss6_battle
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
		if (yGetDatabaseCount("bossLasers") > 0) {
			yDatabaseNext("bossLasers");
			if (trTimeMS() > yGetVar("bossLasers", "next")) {
				switch(1*yGetVar("bossLasers", "step"))
				{
					case 1:
					{
						ySetVar("bossLasers", "step", 2);
						for(x=0; < 4) {
							trUnitSelectClear();
							trUnitSelect(""+(x+trQuestVarGet("bossLasers")), true);
							trSetSelectedScale(6,6,60);
							trUnitHighlight(3, false);
						}
						yVarToVector("bossLasers", "pos");
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else {
								trVectorSetUnitPos("end", "playerUnits");
								if (xsAbs(trQuestVarGet("endx") - trQuestVarGet("posx")) < 1.0 ||
									xsAbs(trQuestVarGet("endz") - trQuestVarGet("posz")) < 1.0) {
									damagePlayerUnit(300);
								}
							}
						}
						trSoundPlayFN("sky passage.wav","1",-1,"","");
						trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					}
					case 2:
					{
						amt = 500 - trTimeMS() + yGetVar("bossLasers", "next");
						if (amt > 0) {
							amt = amt * 0.012;
							for(x=0; < 4) {
								trUnitSelectClear();
								trUnitSelect(""+(x+trQuestVarGet("bossLasers")), true);
								trSetSelectedScale(amt,amt,60);
							}
						} else {
							for(x=0; < 4) {
								trUnitSelectClear();
								trUnitSelect(""+(x+trQuestVarGet("bossLasers")), true);
								trUnitDestroy();
							}
							yRemoveFromDatabase("bossLasers");
						}
					}
				}
			}
		}
		if (yGetDatabaseCount("deployRobots") > 0) {
			if (processGenericProj("deployRobots") == PROJ_GROUND) {
				trDamageUnitPercent(-100);
				trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("bossSummonProto")));
				trUnitSelectClear();
				trUnitSelectByQV("deployRobots", true);
				trDamageUnitPercent(-100);
				trVectorSetUnitPos("pos", "deployRobots");
				vectorToGrid("pos", "loc");
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trUnitChangeProtoUnit("Dust Large");
				} else {
					activateEnemy("deployRobots",-1,0);
				}
				yRemoveFromDatabase("deployRobots");
			}
		}
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trStringQuestVarSet("advice", "Having trouble dodging the Extinction Cannon? Try equipping speed relics!");
				trSetLighting("night", 1.0);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trOverlayText("Extinction Cannon",3.0,-1,-1,-1);
				trQuestVarSet("bossStunIndex", yAddToDatabase("stunnedUnits", "bossUnit"));
				yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Helepolis"));
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossCount", 10);
				trVectorSetUnitPos("bossPos", "bossUnit");
				trVectorSetFromAngle("bossdir", trQuestVarGet("bossangle"));
				
				trUnitSelectClear();
				trUnitSelectByQV("bossTurret", true);
				trUnitSetStance("Aggressive");
				trUnitTeleport(trQuestVarGet("bossPosx"),worldHeight+2.0,trQuestVarGet("bossPosz"));
				trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				trUnitSelectClear();
				trUnitSelectByQV("bossBarrel", true);
				trUnitSetStance("Aggressive");
				trUnitTeleport(trQuestVarGet("bossPosx"),worldHeight+3.5,trQuestVarGet("bossPosz"));
				trSetUnitOrientation(vector(0,1,0),trVectorQuestVarGet("bossdir"),true);
			} else if (trQuestVarGet("bossSpell") == 32) {
				trVectorSetUnitPos("pos", "bossTurret");
				if (zDistanceBetweenVectorsSquared("pos", "bossPos") < 1.0) {
					trQuestVarSet("bossSpell", 33);
					trUnitSelectClear();
					trUnitSelectByQV("bossTurret", true);
					trUnitSetStance("Passive");
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel", true);
					trUnitSetStance("Passive");
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				trUnitSelectClear();
				trUnitSelectByQV("bossBarrelObject", true);
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					if (trQuestVarGet("bossCount") > 0) {
						trSetSelectedScale(1.5,1.0,1.5);
						trVectorSetFromAngle("bossdir", trQuestVarGet("bossangle"));
						trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
						for(x=trQuestVarGet("rand"); >0) {
							yDatabaseNext("playerCharacters", true);
							if (trUnitAlive()) {
								trUnitHighlight(2.5, true);
								break;
							}
						}
						trVectorSetUnitPos("pos", "playerCharacters");
						trQuestVarSet("bossTarget", trQuestVarGet("playerCharacters"));
						trQuestVarSet("bossPrevx", trQuestVarGet("bossDirx"));
						trQuestVarSet("bossPrevz", trQuestVarGet("bossDirz"));
						trVectorQuestVarSet("bossDestDir", zGetUnitVector("bossPos", "pos"));
						trQuestVarSet("testDir1x", 0.0 - trQuestVarGet("bossDirz"));
						trQuestVarSet("testDir1z", trQuestVarGet("bossDirx"));
						trQuestVarSet("testDir2x", trQuestVarGet("bossDirz"));
						trQuestVarSet("testDir2z", 0.0 - trQuestVarGet("bossDirx"));
						if (dotProduct("testDir1", "bossDestDir") > dotProduct("testDir2", "bossDestDir")) {
							trQuestVarSet("bossTurnDir", -1.0);
						} else {
							trQuestVarSet("bossTurnDir", 1.0);
						}
						trQuestVarSet("bossNext", trTimeMS() + 3000);
						trQuestVarSet("bossLast", trTimeMS());
						trQuestVarSet("bossSpell", 34);
						trSoundPlayFN("automatonspcbirth.wav","1",-1,"","");
					} else {
						bossCooldown(10, 20);
						trQuestVarSet("bossUltimate", 3);
						trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
						trUnitSelectClear();
						trUnitSelectByQV("bossBarrel", true);
						trUnitSetStance("Aggressive");
						trUnitTeleport(1,0,1);
						trUnitSelectClear();
						trUnitSelectByQV("bossTurret", true);
						trUnitSetStance("Aggressive");
						trUnitTeleport(1,0,1);
						trSetLighting("anatolia",2.0);
						ySetPointer("stunnedUnits", 1*trQuestVarGet("bossStunIndex"));
						yRemoveFromDatabase("stunnedUnits");
					}
				} else {
					amt = trQuestVarGet("bossNext") - trTimeMS();
					amt = 1.0 - 0.0005 * amt;
					trSetSelectedScale(1.5,amt,1.5);
				}
			} else if (trQuestVarGet("bossSpell") == 34) {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				angle = fModulo(6.283185, trQuestVarGet("bossAngle") + trQuestVarGet("bossTurnDir") * amt * 0.0016);
				trQuestVarSet("bossAngle", angle);
				trVectorSetFromAngle("bossDir", angle);
				trVectorSetUnitPos("target", "bossTarget");
				/* unit isn't dead */
				if (trQuestVarGet("targetx") > 0) {
					trVectorQuestVarSet("bossDestDir", zGetUnitVector("bossPos", "target"));
				}
				if (dotProduct("bossDir", "bossDestDir") < dotProduct("bossPrev", "bossDestDir") ||
					trTimeMS() > trQuestVarGet("bossNext")) {
					/* we passed them */
					angle = angleOfVector("bossDir");
					trQuestVarSet("bossSpell", 35);
					trQuestVarSet("bossNext", trTimeMS() + 1000);
					trSoundPlayFN("storehouse.wav","1",-1,"","");
					trQuestVarSetFromRand("rand", 1, 10, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: PARRY THIS YOU FILTHY CASUAL");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: EXTERMINATE. EXTERMINATE.");
					} else if (trQuestVarGet("rand") == 3) {
						trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: DEATH TO ALL HUMANS");
					}
				} else {
					trQuestVarSet("bossPrevx", trQuestVarGet("bossDirx"));
					trQuestVarSet("bossPrevz", trQuestVarGet("bossDirz"));
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossTurret", true);
				trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				trUnitSelectClear();
				trUnitSelectByQV("bossBarrel", true);
				trSetUnitOrientation(vector(0,1,0),trVectorQuestVarGet("bossdir"),true);
			} else if (trQuestVarGet("bossSpell") == 35) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel");
					trMutateSelected(kbGetProtoUnitID("Lancer Hero"));
					trUnitSelectClear();
					trUnitSelectByQV("volcano1", true);
					trUnitChangeProtoUnit("Cinematic Block");
					trUnitSelectClear();
					trUnitSelectByQV("volcano2", true);
					trUnitChangeProtoUnit("Cinematic Block");
					trUnitSelectClear();
					trUnitSelectByQV("volcano1", true);
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedUpVector(trQuestVarGet("bossDirx"),0,trQuestVarGet("bossDirz"));
					trUnitOverrideAnimation(18,0,false,false,-1);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBarrel"));
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedScale(0,0,0);
					
					
					trUnitSelectClear();
					trUnitSelectByQV("volcano2", true);
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedUpVector(trQuestVarGet("bossDirx"),0,trQuestVarGet("bossDirz"));
					trUnitOverrideAnimation(18,0,false,false,-1);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBarrel"));
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedScale(0,0,0);
					
					
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel");
					trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					trQuestVarSet("destx", trQuestVarGet("bossPosx"));
					trQuestVarSet("destz", trQuestVarGet("bossPosz"));
					for(x=50; > 0) {
						trQuestVarSet("destx", trQuestVarGet("destx") + 2.0 * trQuestVarGet("bossDirx"));
						trQuestVarSet("destz", trQuestVarGet("destZ") + 2.0 * trQuestVarGet("bossDirz"));
						vectorToGrid("dest", "loc");
						if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("destx"),0,trQuestVarGet("destz"),0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Meteor Impact Ground");
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("destx"),0,trQuestVarGet("destz"),0,true);
							trArmySelect("1,0");
							trMutateSelected(kbGetProtoUnitID("Meteor"));
							break;
						}
					}
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							amt = zDistanceToVector("playerUnits", "bossPos");
							trQuestVarSet("hitboxX", trQuestVarGet("bossPosx") + amt * trQuestVarGet("bossDirx"));
							trQuestVarSet("hitboxZ", trQuestVarGet("bossPosz") + amt * trQuestVarGet("bossDirz"));
							if (zDistanceToVectorSquared("playerUnits", "hitbox") < 10.0 ||
								zDistanceToVectorSquared("playerUnits", "dest") < 36.0) {
								damagePlayerUnit(1000);
							}
						}
					}
					trSoundPlayFN("ui\thunder1.wav","1",-1,"","");
					trSoundPlayFN("cinematics\32_out\doorbigshut.mp3","1",-1,"","");
					trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
					trCameraShake(0.7, 0.8);
					trQuestVarSet("bossSpell", 33);
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrelObject", true);
					trSetSelectedScale(1.5,0.25,1.5);
					trQuestVarSet("bossNext", trTimeMS() + 1500);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trQuestVarSetFromRand("rand", 1, 3, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: DEPLOYING DRONES");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: EXTERMINATE THE HUMANS");
				}
				trQuestVarSetFromRand("bossSummonProto", 1, 3, true);
				trQuestVarSetFromRand("bossCount", 5, 5 + ENEMY_PLAYER);
				trQuestVarSet("bossSpell", 22);
				trQuestVarSet("bossNext", trTimeMS());
				trSoundPlayFN("catapultattack.wav","1",-1,"","");
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 200);
					trQuestVarSetFromRand("angle", 0, 6.283185, false);
					trVectorSetFromAngle("dir", trQuestVarGet("angle"));
					trQuestVarSetFromRand("speed", 5, 12, false);
					trVectorSetUnitPos("pos", "bossUnit");
					addGenericProj("deployRobots","pos","dir",kbGetProtoUnitID("Statue of Automaton Base"),
						2,trQuestVarGet("speed"),3,1);
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") <= 0) {
						bossCooldown(10, 20);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSetFromRand("rand", 1, 3, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: RAMMING SPEED");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: THRUSTERS ENGAGED");
				}
				trSoundPlayFN("siegeacknowledge.wav","1",-1,"","");
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				trQuestVarSet("bossStunIndex", yAddToDatabase("stunnedUnits", "bossUnit"));
				yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Helepolis"));
				yDatabaseNext("playerCharacters");
				trVectorSetUnitPos("bossTargetPos", "playerCharacters");
				trVectorSetUnitPos("bossPos", "bossUnit");
				trVectorQuestVarSet("bossdir", zGetUnitVector("bossPos", "bossTargetPos"));
				trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				trQuestVarSet("destx", trQuestVarGet("bossPosx"));
				trQuestVarSet("destz", trQuestVarGet("bossPosz"));
				for(x=50; >0) {
					trQuestVarSet("destx", trQuestVarGet("destx") + 2.0 * trQuestVarGet("bossdirx"));
					trQuestVarSet("destz", trQuestVarGet("destz") + 2.0 * trQuestVarGet("bossdirz"));
					vectorToGrid("dest", "loc");
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					} else {
						trQuestVarSet("bossTargetPosx", trQuestVarGet("destx"));
						trQuestVarSet("bossTargetPosz", trQuestVarGet("destz"));
					}
				}
				trQuestVarSet("bossNext", trTimeMS() + 1000);
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossCar", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trImmediateUnitGarrison(""+1*trQuestVarGet("bossUnit"));
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					ySetPointer("stunnedUnits", 1*trQuestVarGet("bossStunIndex"));
					yRemoveFromDatabase("stunnedUnits");
					trUnitSelectClear();
					trUnitSelectByQV("bossCar", true);
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Hero Greek Achilles");
					trUnitSelectClear();
					trUnitSelectByQV("bossCar", true);
					trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit", true);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossCar"));
					trMutateSelected(kbGetProtoUnitID("Helepolis"));
					trQuestVarSet("bossSFX", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
					trUnitSelectClear();
					trUnitSelectByQV("bossSFX");
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trUnitOverrideAnimation(6,0,true,false,-1);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossCar"));
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					
					trUnitSelectClear();
					trUnitSelectByQV("bossCar", true);
					trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					trUnitMoveToPoint(trQuestVarGet("bossTargetPosx"),0,trQuestVarGet("bossTargetPosz"),-1,false);
					trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 13);
					trQuestVarSet("bossTimeout", trTimeMS() + 3000);
					yClearDatabase("splatterUnits");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) > -1) {
							yAddToDatabase("splatterUnits", "playerUnits");
							yAddUpdateVar("splatterUnits", "index", yGetPointer("playerUnits"));
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				trVectorSetUnitPos("bosspos", "bossUnit");
				if (zDistanceBetweenVectorsSquared("bossPos", "bossTargetPos") < 4 ||
					trTimeMS() > trQuestVarGet("bossTimeout")) {
					trUnitChangeProtoUnit("Helepolis");
					trUnitSelectClear();
					trUnitSelectByQV("bossSFX");
					trDamageUnitPercent(100);
					trUnitSelectClear();
					trUnitSelectByQV("bossCar");
					trUnitChangeProtoUnit("Meteor Impact Ground");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (zDistanceToVectorSquared("playerUnits", "bossPos") < 16) {
							damagePlayerUnit(500);
						}
					}
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
					trCameraShake(0.5,0.5);
					
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
					bossCooldown(10, 16);
				} else {
					trQuestVarSet("sound", 0);
					for(x=yGetDatabaseCount("splatterUnits"); >0) {
						id = yDatabaseNext("splatterUnits", true);
						if (id == -1 || trUnitAlive() == false) {
							yRemoveFromDatabase("splatterUnits");
						} else if (zDistanceToVectorSquared("splatterUnits", "bossPos") < 25) {
							if (ySetPointer("playerUnits", 1*yGetVar("splatterUnits", "index"))) {
								trQuestVarSet("sound", 1);
								damagePlayerUnit(500);
								xsSetContextPlayer(1*yGetVar("playerUnits", "player"));
								if (kbUnitGetCurrentHitpoints(id) > 0) {
									trVectorSetUnitPos("pos", "playerUnits");
									trQuestVarSet("endx", trQuestVarGet("posx") - trQuestVarGet("bossPosx") + trQuestVarGet("bossTargetPosx"));
									trQuestVarSet("endz", trQuestVarGet("posz") - trQuestVarGet("bossPosz") + trQuestVarGet("bossTargetPosz"));
									launchUnit("playerUnits", "end");
								} else {
									trSetSelectedUpVector(5.0*trQuestVarGet("bossDirx"),-0.5,5.0*trQuestVarGet("bossDirz"));
								}
							}
							yRemoveFromDatabase("splatterUnits");
						}
					}
					if (trQuestVarGet("sound") == 1) {
						trQuestVarSetFromRand("sound", 1, 2, true);
						trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
						trQuestVarSet("sound", 0);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trQuestVarSetFromRand("rand", 1, 3, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: TARGET LOCKED");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "The Exterminator: ACTIVATING LASERS");
				}
				trQuestVarSet("bossSpell", 2);
				trQuestVarSet("bossNext", trTimeMS());
				trQuestVarSet("bossCount", 5);
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 1000);
					yDatabaseNext("playerCharacters");
					trVectorSetUnitPos("pos", "playerCharacters");
					vectorSnapToGrid("pos");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz")+2,180,true);
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz")-2,0,false);
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx")+2,0,trQuestVarGet("posz"),270,false);
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx")-2,0,trQuestVarGet("posz"),90,false);
					trArmySelect("1,0");
					trUnitSetStance("Passive");
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetSelectedScale(3,0,60);
					yAddToDatabase("bossLasers", "next");
					yAddUpdateVar("bossLasers", "step", 1);
					yAddUpdateVar("bossLasers", "posx", trQuestVarGet("posx"));
					yAddUpdateVar("bossLasers", "posz", trQuestVarGet("posz"));
					yAddUpdateVar("bossLasers", "next", trTimeMS() + 2000);
					trSoundPlayFN("skypassagein.wav","1",-1,"","");
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") <= 0) {
						bossCooldown(9, 15);
					}
				}
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
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

rule boss4_battle
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
	float dist = 0;
	float angle = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		for (i=xsMin(3,yGetDatabaseCount("bossClouds")); > 0) {
			action = processGenericProj("bossClouds");
			if ((action == PROJ_FALLING) && yGetVar("bossClouds", "type") < 99) {
				yVarToVector("bossClouds", "prev");
				yVarToVector("bossClouds", "dir");
				amt = zDistanceBetweenVectors("pos", "prev");
				if (amt > 4.0) {
					yVarToVector("bossClouds", "dir");
					ySetVar("bossClouds", "currentDist", yGetVar("bossClouds", "currentDist") + amt);
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							dist = zDistanceToVector("playerUnits", "prev");
							if (dist < amt + 1.0) {
								trQuestVarSet("hitboxX", trQuestVarGet("prevX") + dist * trQuestVarGet("dirX"));
								trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + dist * trQuestVarGet("dirZ"));
								if (zDistanceToVectorSquared("playerUnits", "hitbox") < 4.0) {
									poisonUnit("playerUnits", 10, 30);
								}
							}
						}
					}
					ySetVarFromVector("bossClouds", "prev", "pos");
				}
				trQuestVarSet("nextx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
				trQuestVarSet("nextz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
				vectorToGrid("next", "loc");
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					yRemoveFromDatabase("bossClouds");
				}
			} else if ((action == PROJ_GROUND) && yGetVar("bossClouds", "type") == 99) {
				trDamageUnitPercent(-100);
				trQuestVarSetFromRand("rand", 1, 5, true);
				switch(1*trQuestVarGet("rand"))
				{
					case 1:
					{
						trUnitChangeProtoUnit("Sphinx");
					}
					case 2:
					{
						trUnitChangeProtoUnit("Cyclops");
					}
					case 3:
					{
						trUnitChangeProtoUnit("Avenger");
					}
					case 4:
					{
						trUnitChangeProtoUnit("Dryad");
					}
					case 5:
					{
						trUnitChangeProtoUnit("Walking Woods Marsh");
					}
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossClouds", true);
				trDamageUnitPercent(-100);
				activateEnemy("bossClouds",-1,0);
				yRemoveFromDatabase("bossClouds");
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trOverlayText("Land of Ashes", 3.0, -1, -1, -1);
				trSetLighting("night", 1.0);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trQuestVarSet("bossNext", trTimeMS() + 2000);
				trQuestVarSet("bossSpell", 32);
			} else if (trQuestVarGet("bossSpell") == 32) {
				/* fire carousel */
				trQuestVarSet("bossSpell", 33);
				if (kbGetBlockID(""+1*trQuestVarGet("bossCrossFireSFX")) == -1) {
					spyEffect(1*trQuestVarGet("bossUnit"),kbGetProtoUnitID("Meteorite"),"bossCrossFireSFX");
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("bossCrossFireSFX");
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
					trUnitSelectClear();
					trUnitSelectByQV("bossCrossFireSFX", true);
					trUnitOverrideAnimation(6,0,true,false,-1);
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					trSoundPlayFN("nidhoggflame1.wav","1",-1,"","");
					trQuestVarSet("bossAngle", 0);
					trQuestVarSet("bossTimeout", trTimeMS() + 12000);
					for(x=4; >0) {
						trQuestVarSet("bossNext"+x, trTimeMS() + 100 * x);
					}
					trQuestVarSet("bossSpell", 34);
				}
			} else if (trQuestVarGet("bossSpell") >= 34) {
				trQuestVarSet("bossSpell", 1 + trQuestVarGet("bossSpell"));
				if (trQuestVarGet("bossSpell") >= 38) {
					trQuestVarSet("bossSpell", 34);
				}
				dist = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 0.0004 * dist));
				trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
				action = 1*trQuestVarGet("bossSpell") - 22;
				switch(action)
				{
					case 1:
					{
						amt = trQuestVarGet("dirX");
						trQuestVarSet("dirX", 0.0 - trQuestVarGet("dirZ"));
						trQuestVarSet("dirZ", amt);
					}
					case 2:
					{
						trQuestVarSet("dirX", 0.0 - trQuestVarGet("dirX"));
						trQuestVarSet("dirZ", 0.0 - trQuestVarGet("dirZ"));
					}
					case 3:
					{
						amt = trQuestVarGet("dirZ");
						trQuestVarSet("dirZ", 0.0 - trQuestVarGet("dirX"));
						trQuestVarSet("dirX", amt);
					}
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossCrossFireSFX", true);
				trSetSelectedUpVector(4.0 * trQuestVarGet("dirX"),-1,4.0 * trQuestVarGet("dirZ"));
				if (trTimeMS() > trQuestVarGet("bossNext"+action) + 400) {
					amt = trTimeMS() - trQuestVarGet("bossNext"+action);
					trQuestVarSet("bossNext"+action, 400 + trQuestVarGet("bossNext"+action));
					trVectorSetUnitPos("bossPos", "bossUnit");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							dist = zDistanceToVector("playerUnits", "bossPos");
							if (dist < 16) {
								dist = xsMin(dist, 12);
								trQuestVarSet("hitboxX", trQuestVarGet("bossPosX") + dist * trQuestVarGet("dirX"));
								trQuestVarSet("hitboxZ", trQuestVarGet("bossPosZ") + dist * trQuestVarGet("dirZ"));
								if (zDistanceToVectorSquared("playerUnits", "hitbox") < 16) {
									damagePlayerUnit(0.4 * amt);
								}
							}
						}
					}
				}
				if (trTimeMS() > trQuestVarGet("bossTimeout")) {
					trQuestVarSet("bossUltimate", 3);
					trSetLighting("default", 1.0);
					trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
					trUnitSelectClear();
					trUnitSelectByQV("bossCrossFireSFX");
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					bossCooldown(12, 16);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				trSoundPlayFN("flamingweapons.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
				for(x=trQuestVarGet("rand"); >0) {
					yDatabaseNext("playerCharacters");
					trVectorSetUnitPos("bossTargetPos", "playerCharacters");
				}
				if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
					ySetVar("enemies", "launched", 1);
				}
				trQuestVarSet("bossNext", trTimeMS() + 1500);
				trQuestVarSet("bossSpell", 22);
				trVectorSetUnitPos("bossPos", "bossUnit");
				trVectorQuestVarSet("bossDir", zGetUnitVector("bossPos", "bossTargetPos"));
				trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				trQuestVarSet("bossAngle", angleBetweenVectors("bossPos", "bossTargetPos"));
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
					trUnitOverrideAnimation(19,0,false,false,-1);
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 23);
					trQuestVarSet("bossNext",trTimeMS() + 600);
					trQuestVarSet("bossTimeout", trTimeMS() + 2000);
					trQuestVarSet("bossBreath", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("bossPosx"),0,trQuestVarGet("bossPosz"),0,true);
					trArmySelect("1,0");
					trUnitConvert(0);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trQuestVarSet("bossAnim", 1);
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trCameraShake(1.5, 0.4);
					trSoundPlayFN("sonofosirisbolt.wav","1",-1,"","");
					trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 0.5));
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath");
					trUnitOverrideAnimation(6,0,true,false,-1);
					trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
					trSetSelectedUpVector(10.0 * trQuestVarGet("dirx"),0,10.0 * trQuestVarGet("dirz"));
					trQuestVarSet("bossSpell", 24);
					trQuestVarSet("bossLastDirx",trQuestVarGet("dirx"));
					trQuestVarSet("bossLastDirz",trQuestVarGet("dirz"));
				} else {
					trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 24) {
				trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				amt = 1.0 * (trTimeMS() - trQuestVarGet("bossNext")) / 1400;
				amt = fModulo(6.283185, trQuestVarGet("bossAngle") - amt * 1.116);
				trUnitSelectClear();
				trUnitSelectByQV("bossBreath");
				trVectorSetFromAngle("dir", amt);
				trSetSelectedUpVector(10.0 * trQuestVarGet("dirx"),0,10.0 * trQuestVarGet("dirz"));
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						trVectorSetUnitPos("pos", "playerUnits");
						trVectorQuestVarSet("hit", zGetUnitVector("bossPos", "pos"));
						amt = dotProduct("bossLastDir", "dir");
						if (dotProduct("hit", "dir") > amt &&
							dotProduct("hit", "bossLastDir") > amt) {
							damagePlayerUnit(800);
						}
					}
				}
				trQuestVarSet("bossLastDirx",trQuestVarGet("dirx"));
				trQuestVarSet("bossLastDirz",trQuestVarGet("dirz"));
				
				if (trTimeMS() > trQuestVarGet("bossTimeout")) {
					trQuestVarSet("bossAnim", 0);
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath");
					trUnitChangeProtoUnit("Rocket");
					trSetLighting("default", 1.0);
					bossCooldown(10, 16);
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trMutateSelected(kbGetProtoUnitID("Chimera"));
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trSoundPlayFN("argusfreezeattack.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
				for(x=trQuestVarGet("rand"); >0) {
					yDatabaseNext("playerCharacters");
					trVectorSetUnitPos("bossTargetPos", "playerCharacters");
				}
				if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
					ySetVar("enemies", "launched", 1);
				}
				trQuestVarSet("bossAnim", 1);
				trVectorSetUnitPos("bossPos", "bossUnit");
				trVectorQuestVarSet("bossDir", zGetUnitVector("bossPos", "bossTargetPos"));
				trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				trUnitOverrideAnimation(19,0,false,false,-1);
				trQuestVarSet("bossAngle", angleBetweenVectors("bossPos", "bossTargetPos"));
				trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 0.558));
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossNext",trTimeMS());
				trQuestVarSet("bossCount", 8);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 200);
					trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
					addGenericProj("bossClouds","bossPos","dir",kbGetProtoUnitID("Lampades Blood"),2,10.0,4.5,1.0);
					yAddUpdateVar("bossClouds", "prevx", trQuestVarGet("bossPosx"));
					yAddUpdateVar("bossClouds", "prevz", trQuestVarGet("bossPosz"));
					yAddUpdateVar("bossClouds", "type", STATUS_POISON);
					trQuestVarSet("bossAngle", trQuestVarGet("bossAngle") - 0.139);
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") <= 0) {
						trQuestVarSet("bossAnim", 0);
						bossCooldown(10, 16);
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trMutateSelected(kbGetProtoUnitID("Chimera"));
						trUnitOverrideAnimation(-1,0,false,true,-1);
						if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
							ySetVar("enemies", "launched", 0);
						}
					}
				} else {
					trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				if (trQuestVarGet("bossSummons") == 1) {
					trQuestVarSet("bossSummons", 0);
					trQuestVarSet("bossSpell", 11);
				} else {
					trSoundPlayFN("mummyconvert.wav","1",-1,"","");
					trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
					for(x=trQuestVarGet("rand"); >0) {
						yDatabaseNext("playerCharacters");
						trVectorSetUnitPos("bossTargetPos", "playerCharacters");
					}
					if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
						ySetVar("enemies", "launched", 1);
					}
					trVectorSetUnitPos("bossPos", "bossUnit");
					trVectorQuestVarSet("bossDir", zGetUnitVector("bossPos", "bossTargetPos"));
					trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
					trUnitOverrideAnimation(19,0,false,false,-1);
					trQuestVarSet("bossAngle", angleBetweenVectors("bossPos", "bossTargetPos"));
					trQuestVarSet("bossAngle", fModulo(6.283185, trQuestVarGet("bossAngle") + 0.558));
					trQuestVarSet("bossSpell", 2);
					trQuestVarSet("bossNext",trTimeMS());
					trQuestVarSet("bossCount", 8);
					trQuestVarSet("bossAnim", 1);
				}
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 100);
					trVectorSetFromAngle("dir", trQuestVarGet("bossAngle"));
					trQuestVarSetFromRand("speed", 10.0, 20.0, false);
					addGenericProj("bossClouds","bossPos","dir",kbGetProtoUnitID("Kronny Birth SFX"),2,trQuestVarGet("speed"),4.0,1.0);
					yAddUpdateVar("bossClouds", "prevx", trQuestVarGet("bossPosx"));
					yAddUpdateVar("bossClouds", "prevz", trQuestVarGet("bossPosz"));
					yAddUpdateVar("bossClouds", "type", 99);
					trQuestVarSet("bossAngle", trQuestVarGet("bossAngle") - 0.139);
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") <= 0) {
						trQuestVarSet("bossAnim", 0);
						bossCooldown(15, 20);
						trQuestVarSet("bossSummons", 1);
						if (ySetPointer("enemies", 1*trQuestVarGet("bossPointer"))) {
							ySetVar("enemies", "launched", 0);
						}
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trMutateSelected(kbGetProtoUnitID("Chimera"));
						trUnitOverrideAnimation(-1,0,false,true,-1);
					}
				} else {
					trSetUnitOrientation(trVectorQuestVarGet("bossDir"),vector(0,1,0),true);
				}
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
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


rule boss5_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossUnit");
	int old = xsGetContextPlayer();
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		switch(1*trQuestVarGet("bossChokeStep"))
		{
			case 0:
			{
				if (trUnitPercentDamaged() > trQuestVarGet("bossChoke")) {
					trQuestVarSet("bossChoke", trQuestVarGet("bossChoke") + 20);
					trQuestVarSet("bossChokeNext", trTimeMS());
					trQuestVarSet("bossChokeStep", 1);
					trQuestVarSet("bossChokeCount", 3);
					trSoundPlayFN("attackwarning.wav","1",-1,"","");
					trMessageSetText("The arena is getting smaller!", -1);
				}
			}
			case 1:
			{
				if (trTimeMS() > trQuestVarGet("bossChokeNext")) {
					action = trQuestVarGet("bossRoomSize");
					x = trQuestVarGet("bossRoomLocX");
					z = trQuestVarGet("bossRoomLocZ");
					if (trQuestVarGet("bossChokeCount") <= 0) {
						trQuestVarSet("bossChokeStep", 0);
						trPaintTerrain(x - action, z - action, x + action, z + action, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
						trChangeTerrainHeight(x - action-1, z - action-1, x + action+1, z + action+1, wallHeight, false);
						
						trPaintTerrain(x - action+1, z - action+1, x + action-2, z + action-2, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
						trChangeTerrainHeight(x - action+1, z - action+1, x + action-1, z + action-1, worldHeight, false);
						
						trPaintTerrain(0,0,5,5,0,70,true);
						trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
						trSoundPlayFN("wall.wav","1",-1,"","");
						trQuestVarSet("bossRoomSize", action - 1);
						
						if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
							trQuestVarSet("bossSpell", 1);
						}
						trQuestVarSet("bottomX", trQuestVarGet("bossRoomCenterX") - 2 * trQuestVarGet("bossRoomSize"));
						trQuestVarSet("bottomZ", trQuestVarGet("bossRoomCenterZ") - 2 * trQuestVarGet("bossRoomSize"));
						trQuestVarSet("topX", trQuestVarGet("bossRoomCenterX") + 2 * trQuestVarGet("bossRoomSize"));
						trQuestVarSet("topZ", trQuestVarGet("bossRoomCenterZ") + 2 * trQuestVarGet("bossRoomSize"));
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else {
								trVectorSetUnitPos("pos", "playerUnits");
								if (vectorInRectangle("pos", "bottom", "top") == false) {
									trUnitDelete(false);
									removePlayerUnit();
								}
							}
						}
					} else {
						trPaintTerrain(x - action, z - action, x + action, z + action, 2, 10, false);
						trPaintTerrain(x - action+1, z - action+1, x + action-1, z + action-1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
						trQuestVarSet("bossChokeNext", trQuestVarGet("bossChokeNext") + 500);
						trQuestVarSet("bossChokeStep", 2);
					}
				}
			}
			case 2:
			{
				if (trTimeMS() > trQuestVarGet("bossChokeNext")) {
					action = trQuestVarGet("bossRoomSize");
					x = trQuestVarGet("bossRoomLocX");
					z = trQuestVarGet("bossRoomLocZ");
					trPaintTerrain(x - action, z - action, x + action, z + action, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trQuestVarSet("bossChokeNext", trQuestVarGet("bossChokeNext") + 500);
					trQuestVarSet("bossChokeStep", 1);
					trQuestVarSet("bossChokeCount", trQuestVarGet("bossChokeCount") - 1);
				}
			}
		}
		for (i=xsMin(5, yGetDatabaseCount("bossBolts")); > 0) {
			action = processGenericProj("bossBolts");
			if (action == PROJ_FALLING) {
				vectorToGrid("pos", "loc");
				hit = false;
				yVarToVector("bossBolts", "prev");
				yVarToVector("bossBolts", "dir");
				amt = zDistanceBetweenVectors("pos", "prev") + 1.0;
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (rayCollision("playerUnits","prev","dir",amt,2.0)) {
						damagePlayerUnit(100);
						poisonUnit("playerUnits", 10, 5 * trQuestVarGet("stage"));
						hit = true;
					}
				}
				if (hit || terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trUnitSelectClear();
					trUnitSelectByQV("bossBolts", true);
					trDamageUnitPercent(-100);
					trUnitChangeProtoUnit("Lightning sparks");
					trUnitSelectClear();
					trUnitSelectByQV("bossBolts", true);
					trDamageUnitPercent(-100);
					if (hit) {
						trQuestVarSet("boltsound", 1);
					} else {
						trQuestVarSet("boltsound", 2);
					}
					yRemoveFromDatabase("bossBolts");
				} else {
					ySetVarFromVector("bossBolts", "prev", "pos");
				}
			}
		}
		if (trQuestVarGet("boltsound") == 1) {
			trQuestVarSetFromRand("sound", 1, 4, true);
			trSoundPlayFN("swordonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		} else if (trQuestVarGet("boltsound") == 2) {
			trQuestVarSetFromRand("sound", 1, 3, true);
			trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
		trQuestVarSet("boltsound", 0);
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown(21);
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Voiceless Scream", 3.0, -1, -1, -1);
				trQuestVarSet("bossStunIndex", yAddToDatabase("stunnedUnits", "bossUnit"));
				yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Shade of Hades"));
				ySetVarAtIndex("enemies", "launched", 1, 1*trQuestVarGet("bossPointer"));
				trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
				for(x=trQuestVarGet("rand"); >0) {
					yDatabaseNext("playerCharacters");
				}
				trQuestVarSet("bossAnim", 1);
				trQuestVarSet("bossMove", 1.0);
				trQuestVarSet("bossTarget", trQuestVarGet("playerCharacters"));
				trVectorSetUnitPos("pos", "playerCharacters");
				trVectorSetUnitPos("bossPos", "bossUnit");
				trQuestVarSet("bossAngle", angleBetweenVectors("bossPos", "pos"));
				trVectorQuestVarSet("bossdir", zGetUnitVector("bossPos", "pos"));
				trQuestVarSet("dirx", 0.0 - trQuestVarGet("bossdirx"));
				trQuestVarSet("dirz", 0.0 - trQuestVarGet("bossdirz"));
				trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				for(x=1; <3) {
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn"+x, true);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossUnit"));
					trUnitChangeProtoUnit("Dwarf");
				}
				for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossUnit"));
					trUnitChangeProtoUnit("Dwarf");
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Rocket"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
				for(x=1; <3) {
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn"+x, true);
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trSetSelectedScale(3,0,60);
				}
				trQuestVarSet("bossSpell", 22);
				trQuestVarSet("bossNext", trTimeMS() + 2000);
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 55, 2);
			} else if (trQuestVarGet("bossSpell") == 22) {
				amt = trQuestVarGet("bossNext") - trTimeMS();
				if (amt < 0) {
					trQuestVarSet("bossAnim", 1);
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trUnitOverrideAnimation(15,0,true,false,-1);
					for(x=1; <3) {
						trUnitSelectClear();
						trUnitSelectByQV("bossWarn"+x);
						trUnitChangeProtoUnit("Cinematic Block");
					}
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					trQuestVarSet("bossTimeout", trTimeMS() + 9000);
					trQuestVarSet("bossLast", trTimeMS());
					trQuestVarSet("bossSpell", 23);
					for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trMutateSelected(kbGetProtoUnitID("Vortex Start Linked"));
						trUnitSetAnimationPath("0,0,1,0,0,0,0");
					}
				} else {
					/* 0.3 is max. amt is 0 - 2000. 0.3 at 0 and 0 at 2000 */
					angle = 0.00015 * (2000 - amt);
					amt = amt * 0.001;
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn1", true);
					trVectorSetFromAngle("dir", fModulo(6.283185, trQuestVarGet("bossAngle") - angle - 3.141592));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn2", true);
					trVectorSetFromAngle("dir", fModulo(6.283185, trQuestVarGet("bossAngle") + angle - 3.141592));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				trUnitSelectClear();
				trUnitSelectByQV("bossTarget");
				if (trUnitAlive()) {
					trVectorSetUnitPos("pos", "bossTarget");
				} else {
					trQuestVarSet("bossTarget", yDatabaseNext("playerCharacters"));
				}
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				angle = fModulo(6.283185, trQuestVarGet("bossAngle") + amt * trQuestVarGet("bossMove") * 0.0003);
				trQuestVarSet("bossAngle", angle);
				trVectorSetFromAngle("dir", angle);
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trVectorQuestVarSet("bossDestDir", zGetUnitVector("bossPos", "pos"));
				if (dotProduct("bossDestDir", "dir") < dotProduct("bossDir", "bossDestDir")) {
					trQuestVarSet("bossMove", 0.0 - trQuestVarGet("bossMove"));
				}
				trQuestVarSet("bossDirX", trQuestVarGet("dirX"));
				trQuestVarSet("bossDirZ", trQuestVarGet("dirZ"));
				trVectorSetFromAngle("bossDir1", angle - 0.3);
				trVectorSetFromAngle("bossDir2", angle + 0.3);
				angle = fModulo(6.283185, angle - 0.3);
				for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
					trVectorSetFromAngle("dir", angle);
					trUnitSelectClear();
					trUnitSelect(""+x);
					trSetSelectedUpVector(5.0 * trQuestVarGet("dirx"),-1,5.0 * trQuestVarGet("dirz"));
					angle = angle + 0.1;
				}
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trCameraShake(0.5, 0.1);
					trQuestVarSet("bossNext", 500 + trQuestVarGet("bossNext"));
					trQuestVarSetFromRand("sound", 1, 3, true);
					trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					amt = dotProduct("bossDir1", "bossDir2");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							trVectorSetUnitPos("pos", "playerUnits");
							trVectorQuestVarSet("dir", zGetUnitVector("bossPos", "pos"));
							if ((dotProduct("dir", "bossDir1") > amt &&
									dotProduct("dir", "bossDir2") > amt) ||
								zDistanceBetweenVectorsSquared("bossPos", "pos") < 9.0) {
								damagePlayerUnit(200);
								p = 1*yGetVar("playerUnits","player");
								if (trQuestVarGet("playerUnits") == trQuestVarGet("p"+p+"unit")) {
									silencePlayer(p, 2.0);
									if (trCurrentPlayer() == p) {
										trCameraShake(0.5, 0.3);
									}
								}
							}
						}
					}
					if (trTimeMS() > trQuestVarGet("bossTimeout")) {
						trQuestVarSet("bossAnim", 0);
						trSetLighting("dawn", 1.0);
						trQuestVarSet("bossUltimate", 3);
						trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
						trUnitSelectClear();
						trUnitSelectByQV("bossUnit");
						trUnitOverrideAnimation(-1,0,false,true,-1);
						for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
							trUnitSelectClear();
							trUnitSelect(""+x);
							trUnitChangeProtoUnit("Cinematic Block");
						}
						ySetVarAtIndex("enemies", "launched", 0, 1*trQuestVarGet("bossPointer"));
						ySetPointer("stunnedUnits", 1*trQuestVarGet("bossStunIndex"));
						yRemoveFromDatabase("stunnedUnits");
						trQuestVarSet("bossAnim", 0);
						trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 55, 4);
						bossCooldown(12, 20);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossCount", 3);
				trQuestVarSet("bossAnim", 1);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					trVectorSetUnitPos("bossPos", "bossUnit");
					trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
					for(x=trQuestVarGet("rand"); >0) {
						yDatabaseNext("playerCharacters");
					}
					trVectorSetUnitPos("bossTargetpos", "playerCharacters");
					vectorSnapToGrid("bossTargetpos");
					trVectorQuestVarSet("dir", zGetUnitVector("bossPos", "bossTargetpos"));
					trQuestVarSet("bossdirx", 0.0 - trQuestVarGet("dirx"));
					trQuestVarSet("bossdirz", 0.0 - trQuestVarGet("dirz"));
					trQuestVarSet("bossPosx", trQuestVarGet("bossTargetPosX") + 2.0 * trQuestVarGet("dirx"));
					trQuestVarSet("bossPosz", trQuestVarGet("bossTargetPosZ") + 2.0 * trQuestVarGet("dirz"));
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("bossPosx"),0,trQuestVarGet("bossPosz"),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Transport Ship Greek");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit", true);
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit", true);
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
					trUnitOverrideAnimation(1,0,false,false,-1);
					trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitChangeProtoUnit("Kronny Birth SFX");
					trQuestVarSet("slashdirX", 0.0 - trQuestVarGet("dirZ"));
					trQuestVarSet("slashdirZ", trQuestVarGet("dirX"));
					trQuestVarSet("bossSlash", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0", "Dwarf",1,trQuestVarGet("bossTargetposx"),0,trQuestVarGet("bossTargetPosz"),0,true);
					trArmySelect("1,0");
					trSetUnitOrientation(trVectorQuestVarGet("slashDir"),vector(0,1,0),true);
					trUnitChangeProtoUnit("Cinematic Block");
					trQuestVarSet("bossSpell", 13);
					trQuestVarSet("bossNext", trTimeMS() + 1000);
				} else {
					trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trUnitSelectClear();
					trUnitSelectByQV("bossSlash");
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					trUnitSelectClear();
					trUnitSelectByQV("bossSlash");
					trSetSelectedScale(1,1,2);
					trQuestVarSet("bossTargetposX", trQuestVarGet("bossTargetposX") - 4.0 * trQuestVarGet("slashDirX"));
					trQuestVarSet("bossTargetposZ", trQuestVarGet("bossTargetposZ") - 4.0 * trQuestVarGet("slashDirZ"));
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision("playerUnits", "bossTargetPos", "slashDir", 8.0, 2)) {
							damagePlayerUnit(500);
						}
					}
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1000);
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") <= 0) {
						trQuestVarSet("bossSpell", 14);
					} else {
						trQuestVarSet("bossSpell", 12);
					}
				} else {
					trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 14) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trUnitOverrideAnimation(-1,0,false,true,-1);
					bossCooldown(10, 16);
					trQuestVarSet("bossAnim", 0);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trSoundPlayFN("shadeofhadesselect2.wav","1",-1,"","");
				ySetVarAtIndex("enemies", "launched", 1, 1*trQuestVarGet("bossPointer"));
				trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
				for(x=trQuestVarGet("rand"); >0) {
					yDatabaseNext("playerCharacters");
				}
				trVectorSetUnitPos("pos", "playerCharacters");
				trVectorSetUnitPos("bossPos", "bossUnit");
				trVectorQuestVarSet("bossdir", zGetUnitVector("bossPos", "pos"));
				trQuestVarSet("dirx", 0.0 - trQuestVarGet("bossdirx"));
				trQuestVarSet("dirz", 0.0 - trQuestVarGet("bossdirz"));
				trSetUnitOrientation(trVectorQuestVarGet("bossdir"),vector(0,1,0),true);
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				trQuestVarSet("bossStunIndex", yAddToDatabase("stunnedUnits", "bossUnit"));
				yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Shade of Hades"));
				trUnitSelectClear();
				trUnitSelectByQV("bossWarn1", true);
				trMutateSelected(kbGetProtoUnitID("Dwarf"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("bossUnit"));
				trUnitChangeProtoUnit("Dwarf");
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit", true);
				trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
				trUnitSelectClear();
				trUnitSelectByQV("bossWarn1", true);
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trSetSelectedScale(3,0,60);
				trQuestVarSet("bossSpell", 2);
				trQuestVarSet("bossNext", trTimeMS() + 1000);
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trSoundPlayFN("shadeofhadesacknowledge1.wav","1",-1,"","");
					ySetPointer("stunnedUnits", 1*trQuestVarGet("bossStunIndex"));
					yRemoveFromDatabase("stunnedUnits");
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossEscape"));
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn1", true);
					trUnitChangeProtoUnit("Cinematic Block");
					addGenericProj("bossCloud","bossPos","bossdir",kbGetProtoUnitID("Kronny Birth SFX"),2,20,4.5);
					yAddUpdateVar("bossCloud", "prevX", trQuestVarGet("bossPosx"));
					yAddUpdateVar("bossCloud", "prevZ", trQuestVarGet("bossPosz"));
					trQuestVarSet("bossSpell", 3);
					trQuestVarSet("bossNext", trTimeMS() + 7000);
					yClearDatabase("splatterUnits");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						yDatabaseNext("playerUnits");
						yAddToDatabase("splatterUnits", "playerUnits");
						yAddUpdateVar("splatterUnits", "index", yGetPointer("playerUnits"));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				action = processGenericProj("bossCloud");
				if (action == PROJ_FALLING) {
					yVarToVector("bossCloud", "prev");
					yVarToVector("bossCloud", "dir");
					trQuestVarSet("nextx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
					trQuestVarSet("nextz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
					vectorToGrid("next", "loc");
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						trVectorQuestVarSet("dir", getBounceDir("loc", "dir"));
						ySetVarFromVector("bossCloud", "dir", "dir");
						ySetVar("bossCloud", "yeehaw", 99);
						trVectorQuestVarSet("dir", wallNormalVector("loc"));
						amt = trQuestVarGet("dirZ");
						trQuestVarSet("dirZ", 0.0 - trQuestVarGet("dirX"));
						trQuestVarSet("dirX", amt);
						for(x=7; >0) {
							addGenericProj("bossBolts","prev","dir",kbGetProtoUnitID("Medusa"),40,9,5);
							yAddUpdateVar("bossBolts", "prevX", trQuestVarGet("prevX"));
							yAddUpdateVar("bossBolts", "prevZ", trQuestVarGet("prevZ"));
							trVectorQuestVarSet("dir", rotationMatrix("dir", 0.866025, 0.5));
						}
						
						yClearDatabase("splatterUnits");
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							yDatabaseNext("playerUnits");
							yAddToDatabase("splatterUnits", "playerUnits");
							yAddUpdateVar("splatterUnits", "index", yGetPointer("playerUnits"));
						}
						
						trSoundPlayFN("buildingdeath.wav","1",-1,"","");
						trCameraShake(0.5, 0.3);
						
						vectorToGrid("pos", "loc");
						if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							trQuestVarSet("bossNext", trTimeMS());
							debugLog("Whoops! We entered the ZONE");
						}
					} else {
						dist = zDistanceBetweenVectors("pos", "prev");
						for(x=yGetDatabaseCount("splatterUnits"); >0) {
							if (yDatabaseNext("splatterUnits", true) == -1 || trUnitAlive() == false) {
								yRemoveFromDatabase("splatterUnits");
							} else if (rayCollision("splatterUnits", "prev", "dir", dist + 1.0, 4.0)) {
								damagePlayerUnit(500, 1*yGetVar("splatterUnits", "index"));
								trQuestVarSetFromRand("splatterSound", 1, 2, true);
								yRemoveFromDatabase("splatterUnits");
							}
						}
						if (trQuestVarGet("splatterSound") > 0) {
							trSoundPlayFN("titanpunch"+1*trQuestVarGet("splatterSound")+".wav","1",-1,"","");
							trQuestVarSet("splatterSound", 0);
						}
						ySetVarFromVector("bossCloud", "prev", "pos");
					}
				}
				if (action == PROJ_REMOVE || trTimeMS() > trQuestVarGet("bossNext")) {
					if (action != PROJ_REMOVE) {
						yVarToVector("bossCloud", "prev");
						trQuestVarSet("bossPosX", trQuestVarGet("prevX"));
						trQuestVarSet("bossPosZ", trQuestVarGet("prevZ"));
						trUnitSelectClear();
						trUnitSelectByQV("bossCloud", true);
						trUnitChangeProtoUnit("Kronny Birth SFX");
						yRemoveFromDatabase("bossCloud");
					}
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("bossposX"),0,trQuestVarGet("bossposZ"),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitChangeProtoUnit("Transport Ship Greek");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelectByQV("bossUnit");
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trSetSelectedScale(trQuestVarGet("bossScale"), trQuestVarGet("bossScale"), trQuestVarGet("bossScale"));
					trUnitSelectClear();
					trUnitSelectByQV("next");
					trUnitChangeProtoUnit("Kronny Birth SFX");
					bossCooldown(10, 15);
					ySetVarAtIndex("enemies", "launched", 0, 1*trQuestVarGet("bossPointer"));
				}
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(2, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 21 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
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

bool spawnLightning(string pos = "") {
	vectorToGrid(pos, "loc");
	if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),0,true);
		trUnitSelectClear();
		trUnitSelectByQV("next", true);
		trUnitConvert(ENEMY_PLAYER);
		trUnitSetStance("Passive");
		trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
		yAddToDatabase("yeebLightning", "next");
		yAddUpdateVar("yeebLightning", "timeout", trTimeMS() + 2000);
		return(true);
	}
	return(false);
}

rule yeebaagooon_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossUnit", true);
	int old = xsGetContextPlayer();
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		
		if (yGetDatabaseCount("yeebLightningEnd") > 0) {
			trQuestVarSetFromRand("sound", 1, 5, true);
			hit = false;
			for(y=yGetDatabaseCount("yeebLightningEnd"); >0) {
				yDatabaseNext("yeebLightningEnd", true);
				if (trUnitVisToPlayer()) {
					hit = true;
				}
				trVectorSetUnitPos("pos", "yeebLightningEnd");
				trUnitChangeProtoUnit("Lightning sparks");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 1.0) {
						damagePlayerUnit(470);
						if (yGetVar("playerUnits", "hero") == 1) {
							gainFavor(1*yGetVar("playerUnits", "player"), -5.0);
						}
					}
				}
				if (trQuestVarGet("boss") > 999) {
					for(x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "pos") < 1.0) {
							damageEnemy(0, 470, true);
							damageEnemy(0, 470, false);
						}
					}
				}
			}
			yClearDatabase("yeebLightningEnd");
			if (hit) {
				trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
		
		for(x=xsMin(10, yGetDatabaseCount("yeebLightning")); >0) {
			yDatabaseNext("yeebLightning");
			if (trTimeMS() > yGetVar("yeebLightning", "timeout")) {
				hit = true;
				trChatSetStatus(false);
				trDelayedRuleActivation("enable_chat");
				yAddToDatabase("yeebLightningEnd", "yeebLightning");
				trUnitSelectClear();
				trUnitSelectByQV("yeebLightning", true);
				trUnitChangeProtoUnit("Militia");
				trUnitSelectClear();
				trUnitSelectByQV("yeebLightning", true);
				trSetSelectedScale(0,0,0);
				trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
				yRemoveFromDatabase("yeebLightning");
			}
		}
		
		for(y=xsMin(4, yGetDatabaseCount("yeebLightningBalls")); >0) {
			action = processGenericProj("yeebLightningBalls");
			if (action == PROJ_FALLING) {
				yVarToVector("yeebLightningBalls", "dir");
				yVarToVector("yeebLightningBalls", "prev");
				trQuestVarSet("destx", trQuestVarGet("dirx") * 3.0 + trQuestVarGet("posx"));
				trQuestVarSet("destz", trQuestVarGet("dirz") * 3.0 + trQuestVarGet("posz"));
				vectorToGrid("dest", "loc");
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trVectorQuestVarSet("dir", getBounceDir("loc", "dir"));
					ySetVar("yeebLightningBalls", "yeehaw", 99);
					ySetVarFromVector("yeebLightningBalls", "dir", "dir");
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("implodehit"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				} else {
					dist = zDistanceBetweenVectorsSquared("pos", "prev");
					if (dist > 4.0) {
						dist = xsSqrt(dist) + 3.0;
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else if (rayCollision("playerUnits", "prev", "dir", dist, 9.0)) {
								damagePlayerUnit(120);
								stunUnit("playerUnits", 3.0, 0, false);
								if (yGetVar("playerUnits", "hero") == 1) {
									gainFavor(1*yGetVar("playerUnits","player"), -1.0);
								}
							}
						}
						ySetVarFromVector("yeebLightningBalls", "prev", "pos");
					}
				}
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
			if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
				if (trUnitPercentDamaged() > trQuestVarGet("yeebNextInvulnerabilityPhase")) {
					trQuestVarSet("bossSpell", 51);
					trQuestVarSet("yeebNextInvulnerabilityPhase", trQuestVarGet("yeebNextInvulnerabilityPhase") + 20);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 50) {
			if (trQuestVarGet("bossSpell") == 51) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Invincibility Shield");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: *Yawns*");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: You can't touch me.");
				}
				trSoundPlayFN("bronzebirth.wav","1",-1,"","");
				trMessageSetText("Destroy the obelisks to break Yeebaagooon's shield.", -1);
				if (kbGetBlockID(""+1*trQuestVarGet("yeebShieldSFX")) == -1) {
					trQuestVarSet("bossSpell", 52);
					spyEffect(1*trQuestVarGet("yeebaagooon"),kbGetProtoUnitID("Cinematic Block"),"yeebShieldSFX");
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("yeebShieldSFX", true);
					trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
					trSetSelectedScale(2,2,2);
					trQuestVarSet("bossSpell", 53);
				}
				
				ySetVarAtIndex("enemies", "physicalResist", 1, 1*trQuestVarGet("bossPointer"));
				ySetVarAtIndex("enemies", "magicResist", 1, 1*trQuestVarGet("bossPointer"));
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 24, 1);
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 25, 1);
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 26, 1);
				
				trQuestVarSetFromRand("bossDirection", 0, 1, true);
				trQuestVarSet("bossCount", 9);
				trQuestVarSet("bossNext", trTimeMS());
				trQuestVarSet("lower", trQuestVarGet("bossRoomLowerX") + 2);
				amt = trQuestVarGet("bossRoomUpperX") - trQuestVarGet("bossRoomLowerX") - 6;
				for(x=0; <= 1) {
					for(z=0; <= 1) {
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("lower") + amt * x, 0, trQuestVarGet("lower") + amt * z, 0, true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trUnitConvert(ENEMY_PLAYER);
						trUnitChangeProtoUnit("Outpost");
						yAddToDatabase("yeebObelisks", "next");
					}
				}
			} else if (trQuestVarGet("bossSpell") == 52) {
				if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
					trUnitSelectClear();
					trUnitSelectByQV("yeebShieldSFX", true);
					trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
					trSetSelectedScale(2,2,2);
					trQuestVarSet("bossSpell", 53);
				}
			} else if (trQuestVarGet("bossSpell") >= 53) {
				yDatabaseNext("yeebObelisks", true);
				if (trUnitAlive() == false) {
					yRemoveFromDatabase("yeebObelisks");
					if (yGetDatabaseCount("yeebObelisks") == 0) {
						bossCooldown(1, 2);
						ySetVarAtIndex("enemies", "physicalResist", 0.47, 1*trQuestVarGet("bossPointer"));
						ySetVarAtIndex("enemies", "magicResist", 0.47, 1*trQuestVarGet("bossPointer"));
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 24, -0.53);
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 25, -0.53);
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 26, -0.53);
					}
				}
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					if (trQuestVarGet("bossSpell") == 53) {
						trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1000);
						for(x=yGetDatabaseCount("playerCharacters"); >0) {
							if (yDatabaseNext("playerCharacters", true) == -1 || trUnitAlive() == false) {
								removePlayerCharacter();
							} else {
								trVectorSetUnitPos("pos", "playerCharacters");
								vectorSnapToGrid("pos");
								spawnLightning("pos");
							}
						}
						trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
						if (trQuestVarGet("bossCount") == 0) {
							trQuestVarSet("bossSpell", 54);
							trQuestVarSet("bossDirection", 1 - trQuestVarGet("bossDirection"));
							trQuestVarSet("lightningLineCount", (trQuestVarGet("bossRoomUpperX") - trQuestVarGet("bossRoomLowerX")) / 4);
							trQuestVarSet("lightningLineStartx", trQuestVarGet("bossRoomUpperx") - 1);
							trQuestVarSet("lightningLineStartz", trQuestVarGet("bossRoomUpperz") - 1);
							if (trQuestVarGet("bossDirection") == 1) {
								trQuestVarSet("linedirx", -4);
								trQuestVarSet("linedirz", 0);
								trQuestVarSet("lineMovex", 0);
								trQuestVarSet("lineMovez", -2);
							} else if (trQuestVarGet("bossDirection") == 0) {
								trQuestVarSet("linedirx", 0);
								trQuestVarSet("linedirz", -4);
								trQuestVarSet("lineMovex", -2);
								trQuestVarSet("lineMovez", 0);
							}
						}
					} else {
						while(trTimeMS() > trQuestVarGet("bossNext")) {
							trQuestVarSet("bossNext", trTimeMS() + 100);
							trQuestVarSet("posx", trQuestVarGet("lightningLineStartx"));
							trQuestVarSet("posz", trQuestVarGet("lightningLineStartz"));
							for(x=trQuestVarGet("lightningLineCount"); >0) {
								spawnLightning("pos");
								trQuestVarSet("posx", trQuestVarGet("posx") + trQuestVarGet("linedirx"));
								trQuestVarSet("posz", trQuestVarGet("posz") + trQuestVarGet("linedirz"));
							}
							trQuestVarSet("lightningLineStartx", trQuestVarGet("lightningLineStartx") + trQuestVarGet("lineMovex"));
							trQuestVarSet("lightningLineStartz", trQuestVarGet("lightningLineStartz") + trQuestVarGet("lineMovez"));
							vectorToGrid("lightningLineStart", "loc");
							if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
								trQuestVarSet("bossSpell", 53);
								trQuestVarSet("bossCount", 10);
								break;
							}
						}
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trOverlayText("The Banhammer", 3.0, -1, -1, -1);
				trSetLighting("night", 1.0);
				trSoundPlayFN("meteorapproach.wav","1",-1,"","");
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossNext", trTimeMS() + 3000);
				yDatabaseNext("playerCharacters");
				trVectorSetUnitPos("hammerPos", "playerCharacters");
				trVectorQuestVarSet("dir", vector(0,0,-16));
				trQuestVarSet("heading", 360);
				trQuestVarSet("hammerIndicatorsStart", trGetNextUnitScenarioNameNumber());
				for(x=8; >0) {
					trQuestVarSet("posx", trQuestVarGet("hammerPosx") + trQuestVarGet("dirx"));
					trQuestVarSet("posz", trQuestVarGet("hammerPosz") + trQuestVarGet("dirz"));
					vectorToGrid("pos", "loc");
					if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),trQuestVarGet("heading"),true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
					}
					trQuestVarSet("heading", trQuestVarGet("heading") - 45);
					trVectorQuestVarSet("dir", rotationMatrix("dir", 0.707107, 0.707107));
				}
				trQuestVarSet("hammerIndicatorsEnd", trGetNextUnitScenarioNameNumber());
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossSpell", 33);
					zSetProtoUnitStat("Kronny Flying", 0, 1, 0.001);
					trQuestVarSet("hammerObject", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Kronny Flying",1,trQuestVarGet("hammerPosx"),0,trQuestVarGet("hammerPosz"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("hammerObject", true);
					trUnitConvert(0);
					trUnitSelectClear();
					trUnitSelectByQV("hammerObject", true);
					trSetSelectedScale(0,15,0);
					trDamageUnitPercent(100);
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				trQuestVarSet("bossSpell", 34);
			} else if (trQuestVarGet("bossSpell") == 34) {
				trUnitSelectClear();
				trUnitSelectByQV("hammerObject", true);
				if (trUnitDead()) {
					trQuestVarSet("bossNext", trTimeMS() + 1000);
					trQuestVarSet("bossSpell", 35);
					trMutateSelected(kbGetProtoUnitID("Thor Hammer"));
					trSetSelectedScale(2,-2,2);
					trUnitOverrideAnimation(2,0,true,false,-1);
					trUnitSetAnimationPath("0,0,0,0,0,0,0");
				}
			} else if (trQuestVarGet("bossSpell") == 35) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossSpell", 36);
					trQuestVarSet("bossCount", 0);
					trQuestVarSet("bossRadius", 0);
					trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
					trCameraShake(1.0, 0.5);
					trVectorQuestVarSet("bossdir", vector(1,0,0));
					for(x=trQuestVarGet("hammerIndicatorsStart"); < trQuestVarGet("hammerIndicatorsEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trUnitDestroy();
					}
				}
			} else if (trQuestVarGet("bossSpell") == 36) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 200);
					trQuestVarSet("bossCount", 1 + trQuestVarGet("bossCount"));
					angle = 3.141592 / trQuestVarGet("bossCount");
					trQuestVarSet("cos", xsCos(angle));
					trQuestVarSet("sin", xsSin(angle));
					for(x=trQuestVarGet("bossCount")*2; >0) {
						trVectorQuestVarSet("bossdir", rotationMatrix("bossdir", trQuestVarGet("cos"), trQuestVarGet("sin")));
						trQuestVarSet("posx", trQuestVarGet("bossdirx") * trQuestVarGet("bossRadius") + trQuestVarGet("hammerPosx"));
						trQuestVarSet("posz", trQuestVarGet("bossdirz") * trQuestVarGet("bossRadius") + trQuestVarGet("hammerPosz"));
						trArmyDispatch("1,0","Meteor Impact Ground",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					}
					trQuestVarSet("bossRadius", trQuestVarGet("bossRadius") + 4);
					dist = xsPow(trQuestVarGet("bossRadius"), 2);
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (zDistanceToVectorSquared("playerUnits", "hammerPos") < dist) {
							damagePlayerUnit(999);
						}
					}
					
					if (trQuestVarGet("boss") > 999) {
						for(x=yGetDatabaseCount("enemies"); >0) {
							if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else if (zDistanceToVectorSquared("enemies", "hammerPos") < dist) {
								damageEnemy(0, 999);
								damageEnemy(0, 999, false);
							}
						}
					}
					
					if (trQuestVarGet("bossCount") == 4) {
						trSetLighting("anatolia", 1.0);
						bossCooldown(12, 16);
						trQuestVarSet("bossUltimate", 3);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Shockwave");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Don't be shocked about this one.");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Lightning has many interesting properities.");
				}
				trVectorSetUnitPos("start", "yeebaagooon");
				trQuestVarSetFromRand("angle", 0, 3.14, false);
				trVectorSetFromAngle("dir", trQuestVarGet("angle"));
				for(x=8; >0) {
					addGenericProj("yeebLightningBalls","start","dir",kbGetProtoUnitID("Arkantos God"),26,10,5);
					yAddUpdateVar("yeebLightningBalls", "prevx", trQuestVarGet("startx"));
					yAddUpdateVar("yeebLightningBalls", "prevz", trQuestVarGet("startz"));
					yAddUpdateVar("yeebLightningBalls", "bounces", 3);
					trVectorQuestVarSet("dir", rotationMatrix("dir", 0.707107, 0.707107));
				}
				bossCooldown(6, 12);
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Lightning Cage");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Time to die~");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Can you escape this?");
				}
				trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("playerCharacters"), true);
				for(x=trQuestVarGet("rand"); >0) {
					yDatabaseNext("playerCharacters");
				}
				trVectorSetUnitPos("cageCenter", "playerCharacters");
				vectorSnapToGrid("cageCenter");
				trQuestVarSet("cageCount", 12);
				trQuestVarSet("cageRadius", 6);
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossNext", trTimeMS());
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trTimeMS() + 100);
					trQuestVarSet("dirx", 2 * trQuestVarGet("cageRadius"));
					trQuestVarSet("dirz", 2 * trQuestVarGet("cageCount") - trQuestVarGet("dirx"));
					for(x=4; >0) {
						trQuestVarSet("posx", trQuestVarGet("dirx") + trQuestVarGet("cageCenterx"));
						trQuestVarSet("posz", trQuestVarGet("dirz") + trQuestVarGet("cageCenterz"));
						spawnLightning("pos");
						vectorRotate90Deg("dir");
					}
					trQuestVarSet("cageCount", trQuestVarGet("cageCount") - 1);
					if (trQuestVarGet("cageCount") == 0) {
						trQuestVarSet("cageRadius", trQuestVarGet("cageRadius") - 1);
						trQuestVarSet("cageCount", trQuestVarGet("cageRadius") * 2);
						if (trQuestVarGet("cageRadius") <= 0) {
							bossCooldown(6, 12);
							if (trQuestVarGet("boss") > 999) {
								/* phase 2 */
								trQuestVarSet("boat", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("cageCenterx"),0,trQuestVarGet("cageCenterz"),0,true);
								trUnitSelectClear();
								trUnitSelectByQV("boat", true);
								trUnitConvert(0);
								trUnitChangeProtoUnit("Transport Ship Greek");
								trUnitSelectClear();
								trUnitSelectByQV("yeebBird", true);
								trImmediateUnitGarrison(""+1*trQuestVarGet("boat"));
								trUnitChangeProtoUnit("Dwarf");
								trUnitSelectClear();
								trUnitSelectByQV("yeebBird", true);
								trMutateSelected(kbGetProtoUnitID("Stymphalian Bird"));
								trSetSelectedScale(0,0,0);
								trUnitSelectClear();
								trUnitSelectByQV("boat", true);
								trUnitChangeProtoUnit("Hero Birth");
							}
						}
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			/* zappy zap */
			if (trQuestVarGet("bossSpell") == 1) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Zappy Zap");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Relics begone!");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Want to see a magic trick?");
				}
				trQuestVarSet("bossSpell", 2);
				trQuestVarSet("bossTimeout", trTimeMS() + 500);
				trQuestVarSet("bossNext", 500);
				trQuestVarSet("zappyStart", trQuestVarGet("yeebaagooon"));
				z = 0;
				for(x=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+x+"dead") == 0) {
						z = z + 1;
						trQuestVarSet("choose"+z, x);
					}
				}
				trQuestVarSetFromRand("zappyTarget", 1, z, true);
				p = trQuestVarGet("choose"+1*trQuestVarGet("zappyTarget"));
				trQuestVarSet("zappyTarget", p);
				trQuestVarSet("zappyEnd", trQuestVarGet("p"+p+"unit"));
				trQuestVarSet("zappyIndex", yGetPointer("playerUnits"));
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trQuestVarGet("bossTimeout") - trTimeMS() < trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") - 100);
					trVectorSetUnitPos("start", "zappyStart");
					trVectorSetUnitPos("end", "zappyEnd");
					dist = trQuestVarGet("bossNext") / 500;
					trQuestVarSet("posx", dist * (trQuestVarGet("startx") - trQuestVarGet("endx")) + trQuestVarGet("endx"));
					trQuestVarSet("posz", dist * (trQuestVarGet("startz") - trQuestVarGet("endz")) + trQuestVarGet("endz"));
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning sparks Ground");
					if (trQuestVarGet("bossNext") <= 0) {
						if (trQuestVarGet("boss") > 999) {
							trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Relic");
							yAddToDatabase("freeRelics", "next");
							yAddUpdateVar("freeRelics", "type", RELIC_MAGIC_DETECTOR);
						}
						trUnitSelectClear();
						trUnitSelectByQV("zappyEnd", true);
						trUnitHighlight(0.2, false);
						damagePlayerUnit(150, 1*trQuestVarGet("zappyIndex"));
						p = trQuestVarGet("zappyTarget");
						action = trQuestVarGet("p"+p+"class");
						trUnitChangeProtoUnit(kbGetProtoUnitName(1*trQuestVarGet("class"+action+"proto")));
						trQuestVarSetFromRand("sound", 1, 5, true);
						trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
						gainFavor(p, -5.0);
						if (trCurrentPlayer() == p) {
							trChatSend(0, "<color=1,1,1>Yeebaagooon has ejected all of your relics!");
						}
						
						bossCooldown(6, 12);
					}
				}
			}
		} else if (trQuestVarGet("boss") > 999) {
			/* phase 2 */
			trQuestVarSetFromRand("bossSpell", 0, 3, true);
			trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(5, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 21 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		xsDisableSelf();
		xsDisableRule("boss_music");
		xsEnableRule("yeebaagooon_ded");
		
		trMusicStop();
		trSetLighting("anatolia", 1.0);
		
		uiLookAtUnitByName(""+1*trQuestVarGet("bossUnit"));
	}
	
	if (trQuestVarGet("gameOverStep") > 0) {
		trQuestVarSet("gameOverStep", 7);
		trQuestVarSet("yeebhit", 0);
		for(x=yGetDatabaseCount("yeebRelics"); >0) {
			yDatabaseNext("yeebRelics", true);
			trUnitDestroy();
		}
		for(p=1; < ENEMY_PLAYER) {
			for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
				if (yDatabaseNext("p"+p+"relics", true) == -1) {
					yRemoveFromDatabase("p"+p+"relics");
				}
			}
		}
		xsDisableSelf();
	}
	xsSetContextPlayer(old);
}

rule yeebaagooon_ded
inactive
highFrequency
{
	int x = 0;
	if (trTime() > trQuestVarGet("yeebOverNext")) {
		trQuestVarSet("yeebOverStep", 1 + trQuestVarGet("yeebOverStep"));
		switch(1*trQuestVarGet("yeebOverStep"))
		{
			case 1:
			{
				trSetUnitIdleProcessing(false);
				trUIFadeToColor(0,0,0,1000,0,true);
				trLetterBox(true);
				trQuestVarSet("yeebOverNext", trTime() + 2);
			}
			case 2:
			{
				if (trQuestVarGet("boss") == 1) {
					trPaintTerrain(30, 10, 32, 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trPaintTerrain(10, 30, 30, 32, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trChangeTerrainHeight(31, 10, 32, 32, worldHeight, false);
					trChangeTerrainHeight(10, 31, 32, 32, worldHeight, false);
				} else {
					trPaintTerrain(trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") - 3,
						trQuestVarGet("bossRoomEntranceX") + 35, trQuestVarGet("bossRoomEntranceZ"),
						TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY);
					trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") - 3,
						trQuestVarGet("bossRoomEntranceX") + 35, trQuestVarGet("bossRoomEntranceZ"),worldHeight,false);
					trPaintTerrain(trQuestVarGet("bossRoomEntranceX")-4, trQuestVarGet("bossRoomEntranceZ"),
						trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") + 35,
						TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY);
					trChangeTerrainHeight(trQuestVarGet("bossRoomEntranceX") - 3, trQuestVarGet("bossRoomEntranceZ"),
						trQuestVarGet("bossRoomEntranceX"), trQuestVarGet("bossRoomEntranceZ") + 35,worldHeight,false);
				}
				trPaintTerrain(0,0,5,5,0,70,true);
				trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
				trPlayerSetDiplomacy(ENEMY_PLAYER, 0, "Enemy");
				trSoundPlayFN("","1",-1,"Yeebaagooon: Blargh, I am dead.","icons\special e son of osiris icon 64");
				trQuestVarSet("yeebOverNext", trTime() + 4);
				trUnitSelectClear();
				trUnitSelectByQV("yeebaagooon", true);
				trUnitDestroy();
			}
			case 3:
			{
				trSoundPlayFN("","1",-1,"Yeebaagooon: Just kidding. Did you think you were the only ones who could revive?",
					"icons\special e son of osiris icon 64");
				trQuestVarSet("yeebOverNext", trTime() + 6);
				trQuestVarSet("yeebBird", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Stymphalian Bird",1,trQuestVarGet("yeebPosx"),0,trQuestVarGet("yeebPosz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("yeebBird", true);
				trUnitConvert(0);
				trTechGodPower(ENEMY_PLAYER, "spy", 1);
				trTechInvokeGodPower(ENEMY_PLAYER,"spy",vector(1,1,1),vector(1,1,1));
				x = modularCounterNext("spyFind");
				trQuestVarSet("spyEye"+x, kbGetProtoUnitID("Pharaoh of Osiris"));
				trQuestVarSet("spyEye"+x+"unit", trQuestVarGet("yeebBird"));
				trStringQuestVarSet("spyName"+x, "yeebaagooon");
			}
			case 4:
			{
				trQuestVarSet("yeebBirdID", kbGetBlockID(""+1*trQuestVarGet("yeebBird")));
				trUnitSelectClear();
				trUnitSelectByQV("yeebaagooon", true);
				trUnitOverrideAnimation(33,0,true,false,-1);
				trUnitSelectClear();
				trUnitSelectByQV("yeebBird", true);
				trSetSelectedScale(0,0,0);
				trSetUnitIdleProcessing(true);
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				trSoundPlayFN("herorevived.wav","1",-1,"","");
				uiLookAtUnitByName(""+1*trQuestVarGet("yeebBird"));
				trQuestVarSet("yeebOverNext", trTime() + 3);
				trChatSendSpoofed(ENEMY_PLAYER, "Yeebaagooon: Now, die!");
			}
			case 5:
			{
				trQuestVarSet("bossUnit", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Cinematic Block");
				xsEnableRule("yeebaagooon_battle");
				bossCooldown(10, 12);
				trQuestVarSet("musicTime", 0);
				xsEnableRule("boss_music");
				trVectorQuestVarSet("yeebDir", vector(1,0,0));
				trQuestVarSet("yeebLightningNext", trTimeMS());
				if (trQuestVarGet("boss") == 1) {
					trQuestVarSet("yeebBossFight", 0);
					trQuestVarSet("boss", 1000);
					trMessageSetText("Reach the boss room to escape. You will get to keep the stolen relic.", 60);
					trVectorQuestVarSet("yeebDestination", trVectorQuestVarGet("bossRoomCenter"));
				} else {
					trQuestVarSet("boss", 1001);
					trMessageSetText("Return to the starting room to escape. You will get to keep the stolen relic.", 60);
					trVectorQuestVarSet("yeebDestination", trVectorQuestVarGet("startPosition"));
				}
				trQuestVarSet("yeebLatestFeather", trGetNextUnitScenarioNameNumber() - 1);
				trMinimapFlare(trCurrentPlayer(), 60, trVectorQuestVarGet("yeebDestination"), true);
				trSoundPlayFN("xnew_objective.wav","1",-1,"","");
				xsEnableRule("yeebaagooon_battle_2");
				xsDisableSelf();
			}
		}
	}
}

rule yeebaagooon_battle_2
inactive
highFrequency
{
	bool hit = false;
	int id = 0;
	if (kbUnitGetAnimationActionType(1*trQuestVarGet("yeebBirdID")) == 9) {
		if (trTime() > trQuestVarGet("yeebBirdMoveTime")) {
			trQuestVarSet("yeebBirdMoveTime", trTime());
			yDatabaseNext("playerUnits");
			trVectorSetUnitPos("pos", "playerUnits");
			trUnitSelectClear();
			trUnitSelectByQV("yeebBird", true);
			trUnitMoveToPoint(trQuestVarGet("posx"),0,trQuestVarGet("posz"),-1,true);
		}
	}
	
	if (yGetDatabaseCount("yeebFeathers") > 0) {
		hit = false;
		id = yDatabaseNext("yeebFeathers", true);
		if (id == -1) {
			yVarToVector("yeebFeathers", "pos");
			hit = true;
		} else {
			trVectorSetUnitPos("pos", "yeebFeathers");
			if (trQuestVarGet("posY") < worldHeight + 0.5) {
				hit = true;
			} else {
				ySetVarFromVector("yeebFeathers", "pos", "pos");
			}
		}
		
		if (hit) {
			spawnLightning("pos");
			yRemoveFromDatabase("yeebFeathers");
		}
	}
	
	while (yFindLatest("yeebLatestFeather", "Stymph Bird Feather", 0) > 0) {
		trVectorSetUnitPos("pos", "yeebLatestFeather");
		yAddToDatabase("yeebFeathers", "yeebLatestFeather");
		yAddUpdateVar("yeebFeathers", "posx", trQuestVarGet("posx"));
		yAddUpdateVar("yeebFeathers", "posz", trQuestVarGet("posz"));
		trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
	}
	
	if (trTimeMS() > trQuestVarGet("yeebLightningNext")) {
		trVectorSetUnitPos("pos", "yeebaagooon");
		trQuestVarSet("yeebLightningNext",
			trQuestVarGet("yeebLightningNext") + 2.0 * zDistanceBetweenVectors("pos", "yeebDestination"));
		trQuestVarSetFromRand("dist", 4, 20, false);
		trVectorQuestVarSet("yeebDir", rotationMatrix("yeebDir", -0.757323, 0.653041));
		trQuestVarSet("posx", trQuestVarGet("posx") + trQuestVarGet("yeebDirx") * trQuestVarGet("dist"));
		trQuestVarSet("posz", trQuestVarGet("posz") + trQuestVarGet("yeebDirz") * trQuestVarGet("dist"));
		vectorSnapToGrid("pos");
		spawnLightning("pos");
	}
	
	/* all dead */
	if (trQuestVarGet("gameOverStep") > 0) {
		xsDisableSelf();
	} else if ((trQuestVarGet("boss") == 1000) && (trQuestVarGet("deadPlayerCount") < trQuestVarGet("activePlayerCount"))) {
		/* successful escape */
		int escape = 0;
		for(p=1; < ENEMY_PLAYER) {
			if (zDistanceToVectorSquared("p"+p+"unit", "yeebDestination") < 300) {
				escape = escape + 1;
			}
		}
		if (escape == trQuestVarGet("activePlayerCount") - trQuestVarGet("deadPlayerCount")) {
			trSoundPlayFN("win.wav","1",-1,"","");
			trQuestVarSet("gameOverStep", 4);
			trQuestVarSet("playersWon", 1);
			xsEnableRule("game_over");
			xsDisableRule("gameplay_always");
			xsDisableRule("yeebaagooon_battle");
			xsDisableSelf();
			trQuestVarSet("yeebhit", 0);
			trQuestVarSet("ownedRelics"+RELIC_YEEBAAGOOON,
				trQuestVarGet("ownedRelics"+RELIC_YEEBAAGOOON) + trQuestVarGet("yeebRelicRetrieved"));
		}
	} else if (trQuestVarGet("boss") < 1000) {
		xsDisableRule("yeebaagooon_battle");
		xsDisableSelf();
		trQuestVarSet("yeebhit", 0);
		trQuestVarSet("ownedRelics"+RELIC_YEEBAAGOOON,
			trQuestVarGet("ownedRelics"+RELIC_YEEBAAGOOON) + trQuestVarGet("yeebRelicRetrieved"));
		trUnitSelectClear();
		trUnitSelectByQV("yeebBird");
		trUnitChangeProtoUnit("Cinematic Block");
		trUnitSelectClear();
		trUnitSelectByQV("yeebaagooon");
		trUnitChangeProtoUnit("Cinematic Block");
	}
}


rule boss7_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossUnit");
	int old = xsGetContextPlayer();
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float angle = 0;
	float sVal = 0;
	float cVal = 0;
	float m = 0;
	float dist = 0;
	bool hit = false;
	if (trUnitAlive() == true) {
		
		
		trUnitSelectClear();
		trUnitSelectByQV("bossUnit");
		
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			
		} else if (trQuestVarGet("bossSpell") > 20) {
			
		} else if (trQuestVarGet("bossSpell") > 10) {
			/* do you know what a derivative is? */
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: I will drag you to the depths!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: A dance of death!");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: Your death will be swift!");
				}
				trQuestVarSet("bossCount", 3);
				if (trQuestVarGet("secondPhase") == 1) {
					trQuestVarSet("bossCount", 5);
				}
				trQuestVarSet("bossSpell", 12);
				trVectorSetUnitPos("bossPos", "bossUnit");
				trMutateSelected(kbGetProtoUnitID("Dwarf"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("bossEscape"));
				trUnitChangeProtoUnit("Scylla");
				trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("bossPosx"),0,trQuestVarGet("bossPosz"),0,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Meteor Impact Water");
				trSoundPlayFN("shipmove1.wav","1",-1,"","");
				trModifyProtounit("Wadjet Spit",ENEMY_PLAYER,55,2);//make them water so they don't run off
			} else if (trQuestVarGet("bossSpell") == 12) {
				yDatabaseNext("playerCharacters");
				trVectorSetUnitPos("bossTargetPos", "playerCharacters");
				trVectorQuestVarSet("dir", zGetUnitVector("bossPos", "bossTargetPos"));
				dist = 0.5 * zDistanceBetweenVectors("bossPos", "bossTargetPos");
				amt = dist * 0.04;
				m = 15.0 / xsPow(dist, 2);
				for(i=0; < 50) {
					angle = 0.0 - 2.0 * m * (amt * i - dist); // the derivative of the line getting us the slope at the point
					cVal = xsSqrt(xsPow(angle, 2) + 1);
					sVal = angle / cVal;
					cVal = 1.0 / cVal;
					trUnitSelectClear();
					trUnitSelectByQV("jumpPath"+i);
					trSetUnitOrientation(xsVectorSet(trQuestVarGet("dirX")*cVal,sVal,trQuestVarGet("dirZ")*cVal),
						xsVectorSet(0.0-trQuestVarGet("dirX")*sVal,cVal,0.0-trQuestVarGet("dirZ")*sVal),true);
					trUnitTeleport(trQuestVarGet("bossPosX") + trQuestVarGet("dirX") * amt * i,
						worldHeight + 15.0 - m * xsPow(amt * i - dist, 2),
						trQuestVarGet("bossPosZ") + trQuestVarGet("dirZ") * amt * i);
				}
				trQuestVarSet("bossSpell", 13);
				trQuestVarSet("bossNext", trTimeMS() + 1000);
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trSoundPlayFN("geyserhit1.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 14);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				xsSetContextPlayer(ENEMY_PLAYER);
				id = kbUnitGetTargetUnitID(1*trQuestVarGet("bossID"));
				if (id >= 0) {
					trQuestVarSetFromRand("rand", 1, 5, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: Begone!");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: Impact!");
					} else if (trQuestVarGet("rand") == 3) {
						trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: Perish!");
					}
					trSoundPlayFN("shipmove2.wav","1",-1,"","");
					trMutateSelected(kbGetProtoUnitID("Scylla"));
					trUnitSetStance("Passive");
					trVectorSetUnitPos("bossPos", "bossUnit");
					vectorSnapToGrid("bossPos");
					trVectorQuestVarSet("target", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(id)));
					trVectorQuestVarSet("bossDir", zGetUnitVector("target", "bossPos"));
					trVectorQuestVarSet("bossSplashDir", trVectorQuestVarGet("bossDir"));
					trQuestVarSet("bossStartDirx", 0.0 - trQuestVarGet("bossDirx"));
					trQuestVarSet("bossStartDirz", 0.0 - trQuestVarGet("bossDirz"));
					trQuestVarSet("bossAngle", angleBetweenVectors("target", "bossPos"));
					trQuestVarSet("bossWarnStart", trGetNextUnitScenarioNameNumber());
					trVectorQuestVarSet("dir", vector(0,0,-12));
					trQuestVarSet("heading", 360);
					for(i=8; >0) {
						x = trQuestVarGet("bossPosx") + trQuestVarGet("dirx");
						z = trQuestVarGet("bossPosz") + trQuestVarGet("dirz");
						trArmyDispatch("0,0","Dwarf",1,x,0,z,trQuestVarGet("heading"),true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("UI Range Indicator Greek SFX");
						trVectorQuestVarSet("dir", rotationMatrix("dir", 0.707107, 0.707107));
						trQuestVarSet("heading", trQuestVarGet("heading") - 45);
					}
					trQuestVarSet("bossWarnEnd", trGetNextUnitScenarioNameNumber());
					
					trQuestVarSet("bossSpell", 2);
					trQuestVarSet("bossNext", trTimeMS() + 1000);
					action = 1500;
					if (trQuestVarGet("secondPhase") == 1) {
						action = 500;
					}
					trQuestVarSet("bossTimeout", trQuestVarGet("bossNext") + action);
					trQuestVarSet("bossAngularVelocity", 6.283185 / action);
				}
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					for(i=trQuestVarGet("bossWarnStart"); < trQuestVarGet("bossWarnEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+i, true);
						trUnitDestroy();
					}
					trQuestVarSet("bossLast", trQuestVarGet("bossNext"));
					trQuestVarSet("bossSpell", 3);
					trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("bossPosX"),0,trQuestVarGet("bossPosZ"),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Tremor");
					trSoundPlayFN("shockwave.wav","1",-1,"","");
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				trQuestVarSet("bossAngle", trQuestVarGet("bossAngle") + amt * trQuestVarGet("bossAngularVelocity"));
				trVectorSetFromAngle("newDir", trQuestVarGet("bossangle"));
				angle = dotProduct("newDir", "bossDir");
				for(i=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (yGetVar("playerUnits", "launched") == 0) {
						trVectorSetUnitPos("pos", "playerUnits");
						if (zDistanceBetweenVectorsSquared("pos", "bossPos") < 144.0) {
							trVectorQuestVarSet("dir", zGetUnitVector("bossPos", "pos"));
							if (dotProduct("dir", "newDir") > angle) {
								if (dotProduct("dir", "bossDir") > angle) {
									damagePlayerUnit(500);
									if (trUnitAlive()) {
										trQuestVarSet("destx", 50.0 * trQuestVarGet("dirx") + trQuestVarGet("bossPosx"));
										trQuestVarSet("destz", 50.0 * trQuestVarGet("dirz") + trQuestVarGet("bossPosz"));
										launchUnit("playerUnits", "dest");
									}
								}
							}
						}
					}
				}
				if (dotProduct("bossSplashDir", "bossDir") > dotProduct("bossSplashDir", "newDir")) {
					trQuestVarSet("posx", trQuestVarGet("bossPosx") + 12.0 * trQuestVarGet("bossSplashDirx"));
					trQuestVarSet("posz", trQuestVarGet("bossPosz") + 12.0 * trQuestVarGet("bossSplashDirz"));
					trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Meteor Impact Water");
					trVectorQuestVarSet("bossSplashDir", rotationMatrix("bossSplashDir", 0.866025, -0.5));
				}
				trQuestVarSet("bossDirx", trQuestVarGet("newDirx"));
				trQuestVarSet("bossDirz", trQuestVarGet("newDirz"));
				trQuestVarSet("dirx", 0.0 - trQuestVarGet("bossDirx"));
				trQuestVarSet("dirz", 0.0 - trQuestVarGet("bossDirz"));
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				if (trTimeMS() > trQuestVarGet("bossTimeout")) {
					trSetUnitOrientation(trVectorQuestVarGet("bossStartDir"),vector(0,1,0),true);
					if (trQuestVarGet("secondPhase") == 1) {
						trQuestVarSet("bossSpell", 4);
						trQuestVarSet("bossTimeout", trTimeMS() + 500);
						trQuestVarSet("bossAngle", 0);
						trQuestVarSet("bossAngularVelocity", 3.141592 / 500);
					} else {
						trUnitSetStance("Aggressive");
						bossCooldown(10, 15);
					}
				} else {
					/*
					sin(30) = 0.5
					cos(30) = 0.866025
					the model is angled downwards so that the tail can be above the water surface
					*/
					trSetUnitOrientation(xsVectorSet(trQuestVarGet("DirX")*0.866025,-0.5,trQuestVarGet("DirZ")*0.866025),
						xsVectorSet(trQuestVarGet("DirX")*0.5,0.866025,trQuestVarGet("DirZ")*0.5), true);
				}
			} else {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				if (trQuestVarGet("bossSpell") == 4) {
					trQuestVarSet("bossScale", trQuestVarGet("bossScale") + 0.001 * amt);
				} else {
					trQuestVarSet("bossScale", trQuestVarGet("bossScale") - 0.001 * amt);
				}
				trQuestVarSet("bossAngle", trQuestVarGet("bossAngle") + amt * trQuestVarGet("bossAngularVelocity"));
				sVal = xsSin(trQuestVarGet("bossAngle"));
				cVal = xsCos(trQuestVarGet("bossAngle"));
				
				trUnitSelectClear();
				trUnitSelectByQV("bossUnit");
				trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
				trSetUnitOrientation(xsVectorSet(trQuestVarGet("bossStartDirX")*cVal,0.0-sVal,trQuestVarGet("bossStartDirZ")*cVal),
					xsVectorSet(trQuestVarGet("bossStartDirX")*sVal,cVal,trQuestVarGet("bossStartDirZ")*sVal), true);
				if (trTimeMS() > trQuestVarGet("bossTimeout")) {
					trQuestVarSet("bossSpell", 1 + trQuestVarGet("bossSpell"));
					if (trQuestVarGet("bossSpell") == 5) {
						trSoundPlayFN("shockwave.wav","1",-1,"","");
						trSoundPlayFN("meteorsplash.wav","1",-1,"","");
						trQuestVarSet("bossDistance", 0);
						trQuestVarSet("bossNext", trTimeMS() - 1);
						trQuestVarSet("bossTimeout", trQuestVarGet("bossTimeout") + 500);
					} else if (trQuestVarGet("bossSpell") == 6) {
						trQuestVarSet("bossScale", 1.3);
						trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
						trSetUnitOrientation(trVectorQuestVarGet("bossStartDir"),vector(0,1,0),true);
						trUnitSetStance("Aggressive");
						bossCooldown(10, 15);
					}
				}
				if (trQuestVarGet("bossSpell") == 5) {
					if (trTimeMS() > trQuestVarGet("bossNext")) {
						trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 100);
						trQuestVarSet("bossDistance", 6 + trQuestVarGet("bossDistance"));
						trQuestVarSet("posx", trQuestVarGet("bossPosx") + trQuestVarGet("bossDistance") * trQuestVarGet("bossStartDirx"));
						trQuestVarSet("posz", trQuestVarGet("bossPosz") + trQuestVarGet("bossDistance") * trQuestVarGet("bossStartDirz"));
						trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("Meteor Impact Water");
						for(x=yGetDatabaseCount("playerUnits"); >0) {
							if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
								removePlayerUnit();
							} else if (zDistanceToVectorSquared("playerUnits", "pos") < 16.0) {
								damagePlayerUnit(500);
								if (trUnitAlive()) {
									stunUnit("playerUnits", 2.0);
								}
							}
						}
					}
				}
			}
		} else if (yGetVarAtIndex("enemies", "stunStatus", 1*trQuestVarGet("bossPointer")) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, 1 * (trUnitPercentDamaged() * 0.05)), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
			trQuestVarSet("bossSpell", 11);
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
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

/**/
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
				if (trQuestVarGet("newPlayers") == 0) {
					trQuestVarSet("gameOverStep", 4);
				}
				xsDisableSelf();
				xsEnableRule("game_over");
				trQuestVarSet("bossKills", 1 + trQuestVarGet("bossKills"));
			}
		}
		gadgetUnreal("ShowImageBox-CloseButton");
	}
}
