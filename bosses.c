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
	if (trQuestVarGet("bossUltimate") > 0) {
		trQuestVarSet("bossUsedUltimate", 1);
	}
	if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
		trQuestVarSet("bossSpell", 0);
		if (trQuestVarGet("bossUltimate") > 0) {
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

int bossInts = 0;
int bossFloats = 0;

rule initialize_boss_db
active
highFrequency
{
	xsDisableSelf();
	bossInts = zNewArray(mInt, 5, "bossInts");
	bossFloats = zNewArray(mFloat, 5, "bossFloats");
}

int yeebShieldSFX = 0;

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
				bossUnit = trQuestVarGet("yeebaagooon");
				trArmyDispatch("1,0","Dwarf",1,55,0,55,225,true);
				trUnitSelectClear();
				trUnitSelectByQV("yeebaagooon", true);
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Pharaoh of Osiris XP");
				
				spyEffect(1*trQuestVarGet("yeebaagooon"),
					kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,yeebShieldSFX));
				
				activateEnemy(bossUnit);
				xSetBool(dEnemies, xLaunched, true);
				bossPointer = xGetNewestPointer(dEnemies);
				
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
				boss = 1;
				trVectorQuestVarSet("yeebPos", trVectorQuestVarGet("startPosition"));
			}
			case 5:
			{
				trMessageSetText("Yeebaagooon's spells will drain your favor if they hit you!", -1);
				xsDisableSelf();
				trQuestVarSet("yeebRelics", xInitDatabase("yeebRelics"));
				xInitAddInt(1*trQuestVarGet("yeebRelics"),"name");
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"yeebHit") == 1) {
						found = false;
						int relics = getRelicsDB(p);
						for(x=xGetDatabaseCount(relics); >0) {
							xDatabaseNext(relics);
							if (xGetInt(relics, xRelicType) == RELIC_YEEBAAGOOON) {
								found = true;
								xAddDatabaseBlock(1*trQuestVarGet("yeebRelics"), true);
								xSetInt(1*trQuestVarGet("yeebRelics"), xUnitName, xGetInt(relics, xUnitName));
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
minInterval 2
{
	trUnitSelectClear();
	trUnitSelectByQV("bossEntranceStatue");
	if (trUnitIsSelected()) {
		startNPCDialog(NPC_BOSS_ENTRANCE);
		reselectMyself();
	}
	vector pos = kbGetBlockPosition(""+1*trQuestVarGet("bossKey"), true);
	if (unitDistanceToVector(1*trQuestVarGet("bossEntranceStatue"), pos) < 25) {
		trUnitSelectClear();
		trUnitSelectByQV("bossKey");
		trUnitChangeProtoUnit("Osiris Box Glow");
		trUnitSelectClear();
		trUnitSelectByQV("bossEntranceSymbol");
		trUnitChangeProtoUnit("Rocket");
		trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
		trMessageSetText("The boss room entrance has opened!", -1);
		pos = kbGetBlockPosition(""+1*trQuestVarGet("bossEntranceStatue"));
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-8,0,xsVectorGetZ(pos)-8,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Vortex Landing");
		trVectorQuestVarSet("bossRoomEntrance", pos - vector(8,0,8));
		xsDisableSelf();
	}
}

rule enter_boss_room
inactive
minInterval 2
{
	for(p=1; < ENEMY_PLAYER) {
		trUnitSelectClear();
		vector pos = trVectorQuestVarGet("bossRoomEntrance");
		if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit), pos) < trQuestVarGet("bossEntranceRadius")) {
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
	if (boss > 0 && trTime() > trQuestVarGet("musicTime")) {
		if (boss == 11) {
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
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			id = xGetInt(dPlayerUnits, xUnitID);
			trUnitSelectClear();
			trUnitSelectByID(id);
			if (trUnitAlive() == false) {
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
		/*
		painting walls to block off the entrance
		*/
		vector pos = xsVectorSet(trQuestVarGet("bossRoomSize"),0,trQuestVarGet("bossRoomSize"));
		pos = (trVectorQuestVarGet("bossRoomCenter") * 0.5) - pos;
		trPaintTerrain(xsVectorGetX(pos), xsVectorGetZ(pos) - 3,
			xsVectorGetX(pos) + 35, xsVectorGetZ(pos),
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(xsVectorGetX(pos), xsVectorGetZ(pos) - 3,
			xsVectorGetX(pos) + 35, xsVectorGetZ(pos),wallHeight,false);
		trPaintTerrain(xsVectorGetX(pos)-4, xsVectorGetZ(pos),
			xsVectorGetX(pos), xsVectorGetZ(pos) + 35,
			TERRAIN_WALL, TERRAIN_SUB_WALL);
		trChangeTerrainHeight(xsVectorGetX(pos) - 3, xsVectorGetZ(pos),
			xsVectorGetX(pos), xsVectorGetZ(pos) + 35,wallHeight,false);
		
		trVectorQuestVarSet("bossRoomEntrance", pos);
		trPaintTerrain(0,0,5,5,0,70,true);
		trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
		
		pos = trVectorQuestVarGet("bossRoomCenter");
		bossUnit = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("0,0","Dwarf",1,
			xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit(trStringQuestVarGet("bossProto"));
		
		bossID = kbGetBlockID(""+1*bossUnit);
		
		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		
		xsEnableRule("boss"+1*trQuestVarGet("stage")+"_init");
		
		boss = trQuestVarGet("stage");
		
		pos = pos - vector(12,0,12);
		int relics = 0;
		
		xClearDatabase(dPlayerCharacters);
		for(p=1; < ENEMY_PLAYER) {
			if (xGetBool(dPlayerData, xPlayerResigned, p) == false) {
				relics = getRelicsDB(p);
				for(x=xGetDatabaseCount(relics); >0) {
					xDatabaseNext(relics);
					xUnitSelect(relics, xUnitName);
					trUnitChangeProtoUnit("Relic");
				}
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
				trUnitDestroy();
				spawnPlayer(p, pos);
				equipRelicsAgain(p);
				trPlayerKillAllGodPowers(p);
				xSetInt(dPlayerData, xPlayerDead, 0);
				xSetBool(dPlayerData, xPlayerSilenced, true); // the un-silence will grant the right god powers
			}
		}
		
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive()) {
				trUnitDestroy();
			}
		}
		xClearDatabase(dEnemies);
		xClearDatabase(dEnemiesIncoming);
		
		activateEnemy(bossUnit, 0, 0);
		bossPointer = xGetNewestPointer(dEnemies);
		
		pos = trVectorQuestVarGet("bossRoomCenter");
		zSetProtoUnitStat("Revealer", 1, 2, 32);
		trArmyDispatch("1,0","Revealer",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		uiLookAtUnitByName(""+bossUnit);
		
		xClearDatabase(dFreeRelics);
		
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
		int pointer = xGetPointer(dEnemies);
		if (xSetPointer(dEnemies, bossPointer)) {
			if (xGetInt(dEnemies, xStunStatus) > 0) {
				xSetInt(dEnemies, xStunTimeout, xGetInt(dEnemies, xStunTimeout) - 500 * trQuestVarGet("stage"));
			}
			xSetPointer(dEnemies, pointer);
		}
	}
}

int dLionMeteors = 0;
int xLionMeteorYeehaw = 0;

int dLionShockwaves = 0;

int dLionShockwaveTargets = 0;

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
				
				dLionMeteors = xInitDatabase("lionMeteors");
				xInitAddInt(dLionMeteors, "name");
				xLionMeteorYeehaw = xInitAddInt(dLionMeteors, "yeehaw", 1);
				
				dLionShockwaves = initGenericProj("lionShockwaves",kbGetProtoUnitID("Heka Shockwave SFX"),2,15.0,4.0);
				
				dLionShockwaveTargets = xInitDatabase("lionShockwaveTargets");
				xInitAddInt(dLionShockwaveTargets, "name");
				xInitAddInt(dLionShockwaveTargets, "index");
			}
			case 1:
			{
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trModifyProtounit("Tornado", ENEMY_PLAYER, 55, 1);
				trModifyProtounit("Tornado", ENEMY_PLAYER, 8, -99);
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
				spyEffect(1*bossUnit, kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,0));
				xsEnableRule("boss1_battle");
				trQuestVarSet("bossSpell", 11);
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 2, 60);
				trQuestVarSet("bossGem", STARSTONE);
				trQuestVarSetFromRand("bossGemCount", 2, 3, true);
				xsEnableRule("boss_music");
				trMessageSetText("Use the rocks to take cover from the lion's roar!", -1);
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
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float angle = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trUnitAlive() == true) {
		if (xGetDatabaseCount(dLionMeteors) > 0) {
			xDatabaseNext(dLionMeteors);
			if (xGetInt(dLionMeteors, xLionMeteorYeehaw) == 1) {
				xSetInt(dLionMeteors, xLionMeteorYeehaw, 0);
				xUnitSelect(dLionMeteors, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Heka Shockwave SFX"));
				trUnitOverrideAnimation(2,0,true,false,-1);
			} else if (xGetInt(dLionMeteors, xLionMeteorYeehaw) == 2) {
				xUnitSelect(dLionMeteors, xUnitName);
				trUnitChangeProtoUnit("Rock Granite Big");
				xFreeDatabaseBlock(dLionMeteors);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(dLionMeteors, xUnitName), true);
				if (xsVectorGetY(pos) <= worldHeight + 0.5) {
					xUnitSelect(dLionMeteors, xUnitName);
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					xUnitSelect(dLionMeteors, xUnitName);
					trSetSelectedScale(0,0,0);
					trDamageUnitPercent(-100);
					xSetInt(dLionMeteors, xLionMeteorYeehaw, 2);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 9) {
							damagePlayerUnit(200);
						}
					}
					
					pos = vectorToGrid(pos);
					trPaintTerrain(xsVectorGetX(pos),xsVectorGetZ(pos),
						xsVectorGetX(pos),xsVectorGetZ(pos),
						TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				}
			}
		}
		for(x=xsMin(4, xGetDatabaseCount(dLionShockwaves)); >0) {
			action = processGenericProj(dLionShockwaves);
			pos = kbGetBlockPosition(""+xGetInt(dLionShockwaves, xUnitName), true);
			hit = false;
			for(y=xGetDatabaseCount(dLionShockwaveTargets); >0) {
				xDatabaseNext(dLionShockwaveTargets);
				xUnitSelect(dLionShockwaveTargets, xUnitName);
				if (trUnitAlive() == false) {
					xFreeDatabaseBlock(dLionShockwaveTargets);
				} else if (unitDistanceToVector(xGetInt(dLionShockwaveTargets, xUnitName), pos) < 9) {
					hit = true;
					if (xSetPointer(dPlayerUnits, xGetInt(dLionShockwaveTargets, xDatabaseIndex))) {
						damagePlayerUnit(200);
					}
					if (trQuestVarGet("bossSpell") < 30 || trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
						xFreeDatabaseBlock(dLionShockwaveTargets);
					}
				}
			}
			if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL) || hit) {
				xUnitSelect(dLionShockwaves, xUnitName);
				trUnitChangeProtoUnit("Dust Small");
				xFreeDatabaseBlock(dLionShockwaves);
			}
		}
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Roaring Gale", 3.0, -1, -1, -1);
				id = kbGetBlockID(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
				if (id == -1) {
					trQuestVarSet("noFake", 1);
					spyEffect(1*bossUnit,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,0));
				} else {
					trQuestVarSet("noFake", 0);
					trSetSelectedScale(0,0,0);
					trMutateSelected(kbGetProtoUnitID("Nemean Lion"));
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 24, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 25, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 26, 1);
					trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, -6.0);
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
					trUnitChangeProtoUnit("Tornado");
					trUnitConvert(ENEMY_PLAYER);
				}
				if (xSetPointer(dEnemies, bossPointer)) {
					removeEnemy();
				}
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossSpellTimeout", trTimeMS() + 12000);
				trQuestVarSet("bossSpellNext", trTimeMS() + 2000);
				xClearDatabase(dLionShockwaveTargets);
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						xAddDatabaseBlock(dLionShockwaveTargets, true);
						xSetInt(dLionShockwaveTargets, xUnitName, xGetInt(dPlayerUnits, xUnitName));
						xSetInt(dLionShockwaveTargets, xDatabaseIndex, xGetPointer(dPlayerUnits));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > trQuestVarGet("bossSpellNext")) {
					trQuestVarSet("bossSpellNext", trQuestVarGet("bossSpellNext") + 200);
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					bossAngle = fModulo(6.283185, bossAngle + 2.41);
					dir = xsVectorSet(xsCos(bossAngle),0,xsSin(bossAngle));
					addGenericProj(dLionShockwaves,bossPos,dir);
					if (trTimeMS() > trQuestVarGet("bossSpellTimeout")) {
						bossCooldown(7, 20);
						trQuestVarSet("bossUltimate", 3);
						if (trQuestVarGet("noFake") == 0) {
							trUnitSelectClear();
							trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0), true);
							trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 24, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 25, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 26, -1);
							trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, 6.0);
						}
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitChangeProtoUnit("Nemean Lion");
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trSetSelectedScale(bossScale,bossScale,bossScale);
						activateEnemy(bossUnit, 0, 0);
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
				dir = xsVectorSet(xsSin(angle),0,xsCos(angle));
				pos = (dir * 24.0) + trVectorQuestVarGet("bossRoomCenter");
				action = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Sphinx",1,
					xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
				activateEnemy(action, 0, 0);
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
						action = trGetNextUnitScenarioNameNumber();
						pos = trVectorQuestVarGet("bossRoomCenter");
						trQuestVarSet("modX", trQuestVarGet("modX") + xsVectorGetX(pos));
						trQuestVarSet("modZ", trQuestVarGet("modZ") + xsVectorGetZ(pos));
						trQuestVarSetFromRand("heading", 1, 360, true);
						trArmyDispatch("1,0","Kronny Flying",1,trQuestVarGet("modX"),0,trQuestVarGet("modZ"),trQuestVarGet("heading"),true);
						trArmySelect("1,0");
						trUnitConvert(ENEMY_PLAYER);
						zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, 0.01);
						trDamageUnitPercent(100);
						trSetSelectedScale(0,5.0,0);
						xAddDatabaseBlock(dLionMeteors, true);
						xSetInt(dLionMeteors, xUnitName, action);
						trQuestVarSet("rainNext", 500 + trQuestVarGet("rainNext"));
						trQuestVarSet("rainCount", trQuestVarGet("rainCount") - 1);
						if (trQuestVarGet("rainCount") <= 0) {
							trQuestVarSet("bossSpell", 13);
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (xGetDatabaseCount(dLionMeteors) == 0) {
					bossCooldown(7, 20);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				bossAnim = true;
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
				bossPos = kbGetBlockPosition(""+bossUnit, true);
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					trQuestVarSet("bossNext", trQuestVarGet("bossNext") + 1500);
					trQuestVarSet("bossSpell", 3);
					dir = vector(1,0,0);
					for(x=24; >0) {
						addGenericProj(dLionShockwaves,bossPos,dir);
						dir = rotationMatrix(dir, 0.965926, 0.258819);
					}
					xClearDatabase(dLionShockwaveTargets);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							xAddDatabaseBlock(dLionShockwaveTargets, true);
							xSetInt(dLionShockwaveTargets, xUnitName, xGetInt(dPlayerUnits, xUnitName));
							xSetInt(dLionShockwaveTargets, xDatabaseIndex, xGetPointer(dPlayerUnits));
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				if (trTimeMS() > trQuestVarGet("bossNext")) {
					bossAnim = false;
					trUnitChangeProtoUnit("Nemean Lion");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Nemean Lion"));
					trSetSelectedScale(bossScale,bossScale,bossScale);
					bossCooldown(7, 20);
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
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
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+bossUnit);
		xsEnableRule("boss_ded");
		xsDisableRule("gameplay_always");
	}
}

int dTreeStabs = 0;
int xTreeStabDir = 0;
int xTreeStabStep = 0;
int xTreeStabNext = 0;
int xTreeStabSFX = 0;

int dBossRainingTrees = 0;

int dBossHeals = 0;

int dBossRoots = 0;
int xBossRootEnd = 0;
int xBossRootPos = 0;
int xBossRootTimeout = 0;
int xBossRootNext = 0;

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
				vector dir = vector(15,0,0);
				vector pos = vector(0,0,0);
				for(x=7; >0) {
					pos = trVectorQuestVarGet("bossRoomCenter") + dir;
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitConvert(0);
					trUnitChangeProtoUnit("Imperial Examination");
					dir = rotationMatrix(dir, 0.62349, 0.781831);
				}
				dBossRainingTrees = initGenericProj("rainingTrees",kbGetProtoUnitID("Einheriar"),18,10.0,0,0,ENEMY_PLAYER);
				
				dTreeStabs = xInitDatabase("treeStabs");
				xInitAddInt(dTreeStabs, "name");
				xTreeStabDir = xInitAddVector(dTreeStabs, "dir");
				xTreeStabStep = xInitAddInt(dTreeStabs, "step");
				xTreeStabNext = xInitAddInt(dTreeStabs, "next");
				xTreeStabSFX = xInitAddInt(dTreeStabs, "sfx");
				
				dBossHeals = initGenericProj("bossHeals", kbGetProtoUnitID("Curse SFX"),2,1.5,4.5,0.0,0,true);
				
				dBossRoots = xInitDatabase("bossRoots");
				xInitAddInt(dBossRoots, "name");
				xBossRootEnd = xInitAddInt(dBossRoots, "end");
				xBossRootTimeout = xInitAddInt(dBossRoots, "timeout");
				xBossRootNext = xInitAddInt(dBossRoots, "next");
				xBossRootPos = xInitAddVector(dBossRoots, "pos");
			}
			case 1:
			{
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Spy Eye");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
				trSetSelectedScale(bossScale, bossScale, bossScale);
				xSetPointer(dEnemies, bossPointer);
				xFreeDatabaseBlock(dEnemies);
				trQuestVarSet("bossHealth", 100);
				trCounterAddTime("bosshealth",-1,-9999,"<color={Playercolor(2)}>Wraithwood: 100</color>", -1);
				
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 0, 6391);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 27, 656);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 28, 566);
				trModifyProtounit("Shade XP", ENEMY_PLAYER, 29, 656);
				
				xsEnableRule("boss2_battle");
				trQuestVarSet("bossSpell", 41);
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

void treeStab(vector pos = vector(0,0,0)) {
	vector dir = getUnitVector(trVectorQuestVarGet("bossRoomCenter"), pos);
	vector start = pos - (dir * 4.0);
	trUnitSelectClear();
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
	trArmySelect("1,0");
	trUnitChangeProtoUnit("Cinematic Block");
	trUnitSelectClear();
	trUnitSelect(""+(1+next), true);
	trMutateSelected(kbGetProtoUnitID("Tartarian Gate Flame"));
	trUnitSetAnimationPath("0,1,1,0,0,0,0");
	trSetUnitOrientation(dir,vector(0,1,0),true);
	trSetSelectedScale(0.8,1.0,0.5);
	trQuestVarSet("treeStabSound",1);
	xAddDatabaseBlock(dTreeStabs, true);
	xSetInt(dTreeStabs, xUnitName, next);
	xSetVector(dTreeStabs, xTreeStabDir, dir);
	xSetInt(dTreeStabs, xTreeStabNext, trTimeMS() + 2000);
	xSetInt(dTreeStabs, xTreeStabSFX, next + 1);
}


rule boss2_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float angle = 0;
	float dist = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trQuestVarGet("bossHealth") > 0) {
		if (xGetDatabaseCount(dBossRainingTrees) > 0) {
			action = processGenericProj(dBossRainingTrees);
			if (action == PROJ_GROUND) {
				xUnitSelectByID(dBossRainingTrees, xUnitID);
				trUnitChangeProtoUnit("Walking Woods Marsh");
				xUnitSelectByID(dBossRainingTrees, xUnitID);
				trDamageUnitPercent(-100);
				activateEnemy(xGetInt(dBossRainingTrees, xUnitName),-1,0);
				xFreeDatabaseBlock(dBossRainingTrees);
			}
		}
		
		if (xGetDatabaseCount(dBossHeals) > 0) {
			processGenericProj(dBossHeals);
			pos = kbGetBlockPosition(""+xGetInt(dBossHeals, xUnitName), true);
			if (distanceBetweenVectors(pos, trVectorQuestVarGet("bossRoomCenter")) < 12) {
				xUnitSelectByID(dBossHeals, xUnitID);
				trUnitChangeProtoUnit("Regeneration SFX");
				xFreeDatabaseBlock(dBossHeals);
				trSoundPlayFN("recreation.wav","1",-1,"","");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trQuestVarSet("bossHealth", xsMin(100, trQuestVarGet("bossHealth") + 5));
				trCounterAbort("bosshealth");
				trCounterAddTime("bosshealth",-1,-9999,
					"<color={Playercolor(2)}>Wraithwood: "+1*trQuestVarGet("bossHealth")+"</color>", -1);
				trUnitHighlight(0.2,false);
			} else {
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 4) {
						healUnit(xGetInt(dPlayerUnits, xPlayerOwner), 100);
						xUnitSelectByID(dBossHeals, xUnitID);
						trUnitChangeProtoUnit("Hero Death");
						trSoundPlayFN("heal.wav","1",-1,"","");
						xFreeDatabaseBlock(dBossHeals);
						break;
					}
				}
			}
		}
		
		if (xGetDatabaseCount(dBossRoots) > 0) {
			xDatabaseNext(dBossRoots);
			if (trTimeMS() > xGetInt(dBossRoots, xBossRootTimeout)) {
				for(x=xGetInt(dBossRoots, xUnitName); < xGetInt(dBossRoots, xBossRootEnd)) {
					trUnitSelectClear();
					trUnitSelect(""+x);
					trDamageUnitPercent(100);
				}
				xFreeDatabaseBlock(dBossRoots);
			} else if (trTimeMS() > xGetInt(dBossRoots, xBossRootNext)) {
				pos = xGetVector(dBossRoots, xBossRootPos);
				action = 0;
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						dist = unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos);
						if (dist > 16 && dist < 36) {
							hit = (trQuestVarGet("stunSound") == 1 || xGetInt(dPlayerUnits, xStunStatus) == 0);
							stunUnit(dPlayerUnits, 1.5);
							if (hit == false) {
								trQuestVarSet("stunSound", 0);
							}
							if (trTimeMS() > xGetInt(dBossRoots, xBossRootNext)) {
								damagePlayerUnit(10);
								action = 1;
							}
						}
					}
				}
				if (action == 1) {
					xSetInt(dBossRoots, xBossRootNext, trTimeMS() + 500);
					trQuestVarSetFromRand("sound", 1, 4, true);
					trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
			}
		}
		
		for(y=xsMin(9, xGetDatabaseCount(dTreeStabs)); >0) {
			xDatabaseNext(dTreeStabs);
			dist = trTimeMS() - xGetInt(dTreeStabs, xTreeStabNext);
			if (dist > 0) {
				switch(xGetInt(dTreeStabs,xTreeStabStep))
				{
					case 0:
					{
						dir = xGetVector(dTreeStabs, xTreeStabDir);
						xUnitSelect(dTreeStabs, xUnitName);
						trMutateSelected(kbGetProtoUnitID("Pine Dead"));
						trSetSelectedScale(0.9,dist/750,0.9);
						trSetSelectedUpVector(2.5*xsVectorGetX(dir),0,2.5*xsVectorGetZ(dir));
						
						xUnitSelect(dTreeStabs, xTreeStabSFX);
						trUnitChangeProtoUnit("Tartarian Gate flame");
						xUnitSelect(dTreeStabs, xTreeStabSFX);
						trSetSelectedScale(0.8,1.0,0.5);
						trSetUnitOrientation(dir,vector(0,1,0),true);
						trUnitSetAnimationPath("0,1,1,0,0,0,0");
						xSetInt(dTreeStabs, xTreeStabStep, 1);
					}
					case 1:
					{
						xUnitSelect(dTreeStabs, xUnitName);
						if (dist > 300) {
							trSetSelectedScale(1,0.4,1);
							xSetInt(dTreeStabs, xTreeStabStep, 2);
							xSetInt(dTreeStabs, xTreeStabNext, trTimeMS() + 2000);
							pos = kbGetBlockPosition(""+xGetInt(dTreeStabs, xUnitName), true);
							dir = xGetVector(dTreeStabs, xTreeStabDir);
							for(x=xGetDatabaseCount(dPlayerUnits); >0) {
								xDatabaseNext(dPlayerUnits);
								xUnitSelectByID(dPlayerUnits, xUnitID);
								if (trUnitAlive() == false) {
									removePlayerUnit();
								} else if (rayCollision(dPlayerUnits,pos,dir,6,3)) {
									if (xGetInt(dPlayerUnits, xUnitName) == xGetInt(dPlayerData, xPlayerUnit, trCurrentPlayer())) {
										trCameraShake(0.3, 0.8);
									}
									damagePlayerUnit(300);
									trQuestVarSet("treeHitSound", 1);
								}
							}
						} else {
							trSetSelectedScale(1,dist/750,1);
						}
					}
					case 2:
					{
						xUnitSelect(dTreeStabs, xUnitName);
						trSetSelectedScale(1, (1000.0 - dist) / 2500, 1);
						if (dist > 1000) {
							xUnitSelect(dTreeStabs, xUnitName);
							trUnitDestroy();
							xFreeDatabaseBlock(dTreeStabs);
						}
					}
				}
			}
		}
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
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
					angle = trQuestVarGet("bossAngle");
					dir = xsVectorSet(xsCos(angle),0,xsSin(angle));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					addGenericProj(dBossRainingTrees, trVectorQuestVarGet("bossRoomCenter"), dir, ENEMY_PLAYER, trQuestVarGet("speed"));
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
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
					pos = trVectorQuestVarGet("bossRoomCenter");
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-5,0,xsVectorGetZ(pos)-5,225,true);
					trUnitSelectClear();
					trUnitSelectByQV("bossWraith", true);
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Shade XP");
					trUnitSelectClear();
					trUnitSelectByQV("bossWraith", true);
					trSetSelectedScale(2,2,2);
					activateEnemy(1*trQuestVarGet("bossWraith"),0,0);
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
					if (xGetInt(dPlayerData, xPlayerDead, 1*trQuestVarGet("bossCount")) == 0) {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, 1*trQuestVarGet("bossCount")));
						treeStab(pos);
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
					dir = xsVectorSet(xsCos(trQuestVarGet("bossAngle")),0,xsSin(trQuestVarGet("bossAngle")));
					treeStab(trVectorQuestVarGet("bossRoomCenter") + (dir * trQuestVarGet("bossRadius")));
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
					pos = xsVectorSet(xsCos(trQuestVarGet("bossAngle")),0,xsSin(trQuestVarGet("bossAngle")));
					dir = vector(0,0,0) - pos;
					pos = vectorSnapToGrid((pos * 25.0) + trVectorQuestVarGet("bossRoomCenter"));
					addGenericProj(dBossHeals,pos,dir);
					
					trQuestVarSet("bossCount", trQuestVarGet("bossCount") - 1);
					if (trQuestVarGet("bossCount") == 0) {
						bossCooldown(10, 15);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				if (xGetDatabaseCount(dBossRoots) < xGetDatabaseCount(dPlayerCharacters)) {
					xDatabaseNext(dPlayerCharacters);
					pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",4,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					dir = vector(1,0,0);
					for(x=action; < trGetNextUnitScenarioNameNumber()) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trSetUnitOrientation(dir,vector(0,1,0),true);
						trUnitConvert(0);
						trUnitChangeProtoUnit("Uproot 4x4");
						dir = rotationMatrix(dir, 0.923879, 0.382683);
					}
					xAddDatabaseBlock(dBossRoots, true);
					xSetInt(dBossRoots, xUnitName, action);
					xSetInt(dBossRoots, xBossRootEnd, trGetNextUnitScenarioNameNumber());
					xSetInt(dBossRoots, xBossRootTimeout, trTimeMS() + 9000);
					xSetVector(dBossRoots, xBossRootPos, pos);
					trSoundPlayFN("hesperidesselect.wav","1",-1,"","");
					bossCooldown(3, 12);
				} else {
					trQuestVarSetFromRand("bossSpell", 1, 2, true);
					trQuestVarSet("bossSpell", 10 * trQuestVarGet("bossSpell") + 1);
				}
			}
		} else {
			if (trCountUnitsInArea(""+1*bossUnit,ENEMY_PLAYER,"Walking Woods Marsh", 40) == 0) {
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
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies, true);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+bossUnit);
		xsEnableRule("boss_ded");
		xsDisableRule("gameplay_always");
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
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
				spyEffect(1*bossUnit, kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,0));
				spyEffect(1*bossUnit, kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,1));
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
		vector pos = vector(0,0,0);
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				if (trQuestVarGet("yeebBossFight") == 1) {
					xAddDatabaseBlock(1*trQuestVarGet("yeebRelics"), true);
					xSetInt(1*trQuestVarGet("yeebRelics"), xUnitName, 1*trQuestVarGet("yeebRelic"));
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
					trUnitSelect(""+bossUnit, true);
					trUnitConvert(ENEMY_PLAYER);
					trSetSelectedScale(bossScale,bossScale,bossScale);
					spyEffect(1*bossUnit, kbGetProtoUnitID("Helepolis"), xsVectorSet(ARRAYS,bossInts,0));
					spyEffect(1*bossUnit, kbGetProtoUnitID("Barracks Atlantean"), xsVectorSet(ARRAYS,bossInts,1));
					xSetBool(dEnemies, xLaunched, true, bossPointer);
					
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
					pos = trVectorQuestVarGet("bossRoomCenter");
					trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Cinematic Block");
				}
			}
			case 1:
			{
				bossCooldown(10, 15);
				trSetLighting("anatolia",0.1);
				xAddDatabaseBlock(dBallistas, true);
				xSetInt(dBallistas, xUnitName, bossUnit);
				xSetInt(dBallistas, xSpecialIndex, bossPointer);
				xSetInt(dBallistas, xPlayerOwner, ENEMY_PLAYER);
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
				trSetSelectedScale(1.2,0.25,2.0);
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
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
				bossScale = 1;
				trQuestVarSet("yeebaagooon", bossUnit);
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitChangeProtoUnit("Pharaoh of Osiris XP");
				xSetPointer(dEnemies, bossPointer);
				xSetBool(dEnemies, xLaunched, true);
				xSetFloat(dEnemies, xPhysicalResist, 0.47);
				xSetFloat(dEnemies, xMagicResist, 0.47);
				
				spyEffect(1*trQuestVarGet("yeebaagooon"),
					kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,yeebShieldSFX));
				
				pos = xsVectorSet(trQuestVarGet("bossRoomSize"),0,trQuestVarGet("bossRoomSize"));
				trVectorQuestVarSet("bossRoomUpper", trVectorQuestVarGet("bossRoomCenter") + pos);
				trVectorQuestVarSet("bossRoomLower", trVectorQuestVarGet("bossRoomCenter") - pos);
				
				trQuestVarSet("yeebNextInvulnerabilityPhase", 30);
				
				trVectorQuestVarSet("yeebPos", trVectorQuestVarGet("bossRoomCenter"));
				
				trOverlayText("Yeebaagooon",3.0,-1,-1,-1);
				
				trQuestVarSet("lightningBolts", trGetNextUnitScenarioNameNumber());
			}
			case 3:
			{
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
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
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
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
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
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

int bossJumpPath = 0;

rule boss7_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				xsDisableRule("the_deep_damage");
				trOverlayText("Mother of the Depths", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				xsDisableRule("deep_village_always");
				for(p=1; < ENEMY_PLAYER) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"medic");
					trUnitDestroy();
				}
			}
			case 1:
			{
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
				xsEnableRule("boss7_battle");
				trQuestVarSet("bossGem", SOULSTONE);
				trQuestVarSetFromRand("bossGemCount", 4, 5, true);
				xsEnableRule("boss_music");
				
				vector pos = trVectorQuestVarGet("bossRoomCenter");
				bossJumpPath = zNewArray(mInt, 50, "jumpPath");
				for(i=0; < 50) {
					aiPlanSetUserVariableInt(ARRAYS,bossJumpPath,i,trGetUnitScenarioNameNumber());
					trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,
						xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					trMutateSelected(kbGetProtoUnitID("Rocket"));
				}
				
				trQuestVarSet("bossEscape", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,1,0,1,0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Transport Ship Greek");
				trArmySelect(""+ENEMY_PLAYER+",0");
				trSetSelectedScale(0,0,0);
				bossCooldown(10, 15);
				
				trModifyProtounit("Scylla", ENEMY_PLAYER, 27, 50);
				xSetBool(dEnemies, xLaunched, true, bossPointer);
				
				spyEffect(1*bossUnit,kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,0));
				
				trStringQuestVarSet("advice",
					"She will heal if she eats someone. You can poison her to prevent the healing.");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

rule boss8_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("Lord of the Heavens", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				xsDisableRule("the_cloud_damage");
				xsEnableRule("the_clouds_build_01");
			}
			case 1:
			{
				trModifyProtounit("Meteor", 0, 8, -12);
				trPaintTerrain(0,0,5,5,0,70,true);
				trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
				TERRAIN_WALL = 4;
				TERRAIN_SUB_WALL = 15;
				trVectorQuestVarSet("bossDir", vector(0,0,-1));
				int db = trQuestVarGet("cloudTornados");
				for(i=0; < 10) {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,db,i),true);
					trUnitDestroy();
				}
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trUnitSetStance("Passive");
				trSetSelectedScale(bossScale, bossScale, bossScale);
				xsEnableRule("boss8_battle");
				trQuestVarSet("bossGem", STARSTONE);
				trQuestVarSetFromRand("bossGemCount", 4, 5, true);
				xsEnableRule("boss_music");
				
				vector pos = trVectorQuestVarGet("bossRoomCenter");
				trQuestVarSet("bossStatue", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos)+1,0,xsVectorGetZ(pos)+1,225,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Statue of Lightning");
				trArmySelect("0,0");
				trUnitOverrideAnimation(2,0,true,false,-1);
				
				xSetPointer(dEnemies, bossPointer);
				xFreeDatabaseBlock(dEnemies);
				
				trVectorQuestVarSet("bossSmitePos", trVectorQuestVarGet("bossRoomCenter") + xsVectorSet(1, 6 + worldHeight, 1));
				
				
				trQuestVarSet("bossBreath1", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("0,0");
				trSetSelectedScale(0,0,0);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				trQuestVarSet("bossBreath2", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("0,0");
				trSetSelectedScale(0,0,0);
				trMutateSelected(kbGetProtoUnitID("Meteorite"));
				
				trQuestVarSet("bossSmiteLaser", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("0,0");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetSelectedScale(0,0,0);
				pos = trVectorQuestVarGet("bossSmitePos");
				trUnitTeleport(xsVectorGetX(pos),xsVectorGetY(pos),xsVectorGetZ(pos));
				
				trMessageSetText("Bring Spark relics to the statue in the middle to damage the boss.", -1);
				
				trQuestVarSet("secondPhase", 1);
				
				trStringQuestVarSet("advice",
					"You need to bring Spark Relics to the statue in the middle in order to deal damage to him.");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
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
					int stage = 1 + trQuestVarGet("stage");
					if (trQuestVarGet("stage") == xGetInt(dPlayerData, xPlayerProgress, p)+1) {
						xSetInt(dPlayerData, xPlayerProgress, xGetInt(dPlayerData, xPlayerProgress, p) + 1, p);
						trShowImageDialog(stageIcon(stage), "Unlocked stage " + stage + ": " + stageName(stage) + "!");
						trSoundPlayFN("favordump.wav","1",-1,"","");
					} else if (trQuestVarGet("stage") > xGetInt(dPlayerData, xPlayerProgress, p)) {
						trShowImageDialog(stageIcon(stage), "You must beat previous stages to unlock this one.");
						trSoundPlayFN("cantdothat.wav","1",-1,"","");
					} else {
						trShowImageDialog(stageIcon(stage), "You have already unlocked the next stage.");
					}
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
