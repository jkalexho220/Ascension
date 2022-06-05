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
int dYeebObelisks = 0;

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
				dYeebObelisks = xInitDatabase("yeebObelisks");
				xInitAddInt(dYeebObelisks, "name");
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

				activateEnemy(bossUnit);
				bossPointer = xGetNewestPointer(dEnemies);
				
				spyEffect(1*trQuestVarGet("yeebaagooon"),
					kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,yeebShieldSFX));
				
				xSetBool(dEnemies, xLaunched, true, bossPointer);
				
				trVectorQuestVarSet("bossRoomUpper", xsVectorSet(60,0,60));
				if (trQuestVarGet("mapType") == MAP_OPEN) {
					trVectorQuestVarSet("bossRoomLower", xsVectorSet(10, 0, 10));
				} else {
					trVectorQuestVarSet("bossRoomLower", xsVectorSet(20, 0, 20));
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
				if (trQuestVarGet("p"+trCurrentPlayer()+"yeebHit") == 1) {
					trQuestVarSet("yeebRelicRetrieved", 1);
				}
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"yeebHit") == 1) {
						found = false;
						int relics = getRelicsDB(p);
						for(x=xGetDatabaseCount(relics); >0) {
							xDatabaseNext(relics);
							if (xGetInt(relics, xRelicType) == RELIC_YEEBAAGOOON) {
								found = true;
								trUnitDestroy();
								xFreeDatabaseBlock(relics);
								break;
							}
						}
						if (found == false) {
							if (trCurrentPlayer() == p) {
								trQuestVarSet("ownedRelics"+RELIC_YEEBAAGOOON, trQuestVarGet("ownedRelics"+RELIC_YEEBAAGOOON) - 1);
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
	if (unitDistanceToVector(1*trQuestVarGet("bossEntranceStatue"), pos) < 36) {
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
	int count = 0;
	for(p=1; < ENEMY_PLAYER) {
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p));
		vector pos = trVectorQuestVarGet("bossRoomEntrance");
		if ((unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), pos) < trQuestVarGet("bossEntranceRadius")) && trUnitAlive()) {
			if (trQuestVarGet("p"+p+"enteredBossRoom") == 0) {
				trQuestVarSet("p"+p+"enteredBossRoom", 1);
				trQuestVarSet("playersInBossRoom", 1 + trQuestVarGet("playersInBossRoom"));
				trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has entered the boss room!");
				trChatSend(0, "All living players must be present to start the boss!");
				count = trQuestVarGet("activePlayerCount") - trQuestVarGet("deadPlayerCount");
				trChatSend(0, "Players: (" + 1*trQuestVarGet("playersInBossRoom") + "/" + count + ")");
			}
		} else if (trQuestVarGet("p"+p+"enteredBossRoom") == 1) {
			trQuestVarSet("p"+p+"enteredBossRoom", 0);
			trQuestVarSet("playersInBossRoom", trQuestVarGet("playersInBossRoom") - 1);
			trChatSend(0, "<color=1,1,1>{Playername("+p+")} has left the boss room!");
		}
	}
	
	if (trQuestVarGet("playersInBossRoom") == trQuestVarGet("activePlayerCount") - trQuestVarGet("deadPlayerCount") &&
		trQuestVarGet("playersInBossRoom") > 0) {
		xsDisableSelf();
		xsEnableRule("boss_cin_00");
		
		trUIFadeToColor(0,0,0,1000,0,true);
		trLetterBox(true);
		
		if (trQuestVarGet("stage") == 7) {
			xsDisableRule("the_deep_damage");
		}
	}
}


rule boss_music
minInterval 3
inactive
{
	if (boss > 0 && trTime() >= trQuestVarGet("musicTime")) {
		if (customContent && (trQuestVarGet("stage") > 1)) {
			switch(1*trQuestVarGet("stage"))
			{
				case 2:
				{
					trMusicPlay("Zenophobia\Enlightenment is Found Within.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 100);
				}
				case 3:
				{
					trMusicPlay("Zenophobia\To Overcome This Crisis.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 119);
				}
				case 4:
				{
					trMusicPlay("Zenophobia\Onslaught.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 145);
				}
				case 5:
				{
					trMusicPlay("Zenophobia\Mad Flowers In Bloom.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 175);
				}
				case 6:
				{
					trMusicPlay("Zenophobia\Death's Embrace.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 120);
				}
				case 7:
				{
					trMusicPlay("Zenophobia\A Fearful Existence.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 201);
				}
				case 8:
				{
					trMusicPlay("Zenophobia\Celestial Battle.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 360);
				}
				case 9:
				{
					trMusicPlay("Zenophobia\Aim for the Top of the Crucible.mp3", "1", 0);
					trQuestVarSet("musicTime", trTime() + 121);
				}
				case 10:
				{
					trMusicPlay("Zenophobia\Apex of the World.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 540);
				}
				case 11:
				{
					trMusicPlay("Zenophobia\True Origin.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 89);
				}
				case 12:
				{
					trMusicPlay("Zenophobia\God Shattering Star.mp3", "1", 1.0);
					trQuestVarSet("musicTime", trTime() + 474);
				}
			}
		} else {
			trQuestVarSet("musicTime", trTime() + 50);
			trQuestVarSet("music", 1 + trQuestVarGet("music"));
			if (trQuestVarGet("music") > 6) {
				trQuestVarSet("music", 1);
			}
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
		trQuestVarSetFromRand("music", 1, 6, true);
		xRestoreCache(dPlayerUnits);
		int id = 0;
		trQuestVarSet("deadPlayerCount", 0);
		int level = trQuestVarGet("stage") / 3;
		trQuestVarSet("bossRelicCount", randomLow(2 + level));
		trQuestVarSet("normalRelicCount", (2 + level) - trQuestVarGet("bossRelicCount"));
		trQuestVarSet("gameOverStep", 0);
		/*
		painting walls to block off the entrance
		*/
		if (trQuestVarGet("stage") == 9) {
			wallHeight = -3;
		}
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
		
		bossID = kbGetBlockID(""+bossUnit);
		
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
					trUnitDestroy();
					xSetInt(relics, xUnitName, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				}
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
				trUnitDestroy();
				trPlayerKillAllGodPowers(p);
				xSetInt(dPlayerData, xPlayerDead, 0, p);
				xSetBool(dPlayerData, xPlayerSilenced, true, p); // the un-silence will grant the right god powers
				spawnPlayer(p, pos);
				equipRelicsAgain(p);		
			}
		}

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
				spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,0));
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
					spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,0));
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
				bossNext = trTimeMS() + 1500;
				trQuestVarSet("bossSpell", 2);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 1500;
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
				if (trTimeMS() > bossNext) {
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
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		boss = 0;
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
					bossNext = trTimeMS();
					trQuestVarSetFromRand("bossCount", ENEMY_PLAYER, 12, true);
					trQuestVarSetFromRand("bossCount", ENEMY_PLAYER, bossCount, true);
					bossCount = trQuestVarGet("bossCount");
				}
				trCounterAbort("bosshealth");
				trCounterAddTime("bosshealth",-1,-9999,
					"<color={Playercolor(2)}>Wraithwood: "+1*trQuestVarGet("bossHealth")+"</color>", -1);
			} else if (trQuestVarGet("bossSpell") == 42) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 500;
					bossCount = bossCount - 1;
					trQuestVarSetFromRand("speed", 5, 15, false);
					bossAngle = fModulo(6.283185, bossAngle + 2.43);
					dir = xsVectorSet(xsCos(bossAngle),0,xsSin(bossAngle));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					addGenericProj(dBossRainingTrees, trVectorQuestVarGet("bossRoomCenter"), dir, ENEMY_PLAYER, trQuestVarGet("speed"));
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
					if (bossCount <= 0) {
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
				bossNext = trTimeMS() + 1500;
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
				bossNext = trTimeMS();
				bossCount = ENEMY_PLAYER - 1;
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 1000;
					if (xGetInt(dPlayerData, xPlayerDead, 1*bossCount) == 0) {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, 1*bossCount));
						treeStab(pos);
					}
					bossCount = bossCount - 1;
					if (bossCount == 0) {
						trQuestVarSet("bossSpell", 23);
						bossCount = 32;
						trQuestVarSet("bossRadius", 6);
						trQuestVarSetFromRand("bossAngle", 0, 3.14, false);
						bossNext = trTimeMS() + 2000;
					}
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 200;
					bossAngle = fModulo(6.283185, bossAngle + 2.45);
					trQuestVarSet("bossRadius", trQuestVarGet("bossRadius") + 0.5);
					trVectorSetFromAngle("dir", bossAngle);
					dir = xsVectorSet(xsCos(bossAngle),0,xsSin(bossAngle));
					treeStab(trVectorQuestVarGet("bossRoomCenter") + (dir * trQuestVarGet("bossRadius")));
					bossCount = bossCount - 1;
					if (bossCount == 0) {
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
				bossCount = 3 + ENEMY_PLAYER / 2;
				trQuestVarSet("bossSpell", 12);
				bossNext = trTimeMS();
				trQuestVarSetFromRand("bossAngle", 0, 3.141592, false);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					trQuestVarSetFromRand("sound",1,3, true);
					trSoundPlayFN("gaiasparkle"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					bossNext = bossNext + 1500;
					bossAngle = bossAngle + 2.4;
					pos = xsVectorSet(xsCos(bossAngle),0,xsSin(bossAngle));
					dir = vector(0,0,0) - pos;
					pos = vectorSnapToGrid((pos * 25.0) + trVectorQuestVarGet("bossRoomCenter"));
					addGenericProj(dBossHeals,pos,dir);
					
					bossCount = bossCount - 1;
					if (bossCount == 0) {
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
			if (trCountUnitsInArea(""+bossUnit,ENEMY_PLAYER,"Walking Woods Marsh", 40) == 0) {
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
		boss = 0;
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

int dBossBreath = 0;

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
				
				dFrostGiantsIncoming = xInitDatabase("frostGiantsIncoming");
				xInitAddInt(dFrostGiantsIncoming, "name");
				xFrostTargetSize = xInitAddFloat(dFrostGiantsIncoming, "targetSize");
				xFrostCurrentSize = xInitAddFloat(dFrostGiantsIncoming, "currentSize");
				xFrostTargetTime = xInitAddInt(dFrostGiantsIncoming, "targetTime");
				
				dBossBreath = initGenericProj("bossBreath",kbGetProtoUnitID("Curse SFX"),2,10.0,4.5,ENEMY_PLAYER);
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
				spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,0)); // ice block
				spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,1)); // aurora sfx
				spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,2)); // ice age sfx
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


rule boss3_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int action = 0;
	int target = 0;
	int id = 0;
	float amt = 0;
	float angle = 0;
	float current = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trUnitAlive() == true) {
		for(x=xsMin(5, xGetDatabaseCount(dBossBreath)); >0) {
			processGenericProj(dBossBreath);
			pos = kbGetBlockPosition(""+xGetInt(dBossBreath, xUnitName), true);
			if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				trUnitChangeProtoUnit("Curse SFX");
				xFreeDatabaseBlock(dBossBreath);
			} else {
				action = 0;
				for(y=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 3) {
						stunUnit(dPlayerUnits, 3.0);
						damagePlayerUnit(75);
						action = 1;
					}
				}
				if (action == 1) {
					xUnitSelectByID(dBossBreath, xUnitID);
					trUnitChangeProtoUnit("Lightning Sparks ground");
					xFreeDatabaseBlock(dBossBreath);
				}
			}
		}
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		if (trUnitPercentDamaged() > xGetDatabaseCount(dFrostGiantsIncoming)) {
			if (trTimeMS() > trQuestVarGet("bossSummonTime")) {
				trQuestVarSet("bossSummonTime", trTimeMS() + 1000 * xGetDatabaseCount(dFrostGiantsIncoming));
				trQuestVarSetFromRand("xMod", 0 - 2*trQuestVarGet("bossRoomSize"), 2*trQuestVarGet("bossRoomSize"), true);
				trQuestVarSet("leftover", xsSqrt(xsPow(2*trQuestVarGet("bossRoomSize"),2) - xsPow(trQuestVarGet("xMod"), 2)));
				trQuestVarSetFromRand("zMod", 0 - trQuestVarGet("leftover"), trQuestVarGet("leftover"), true);
				pos = trVectorQuestVarGet("bossRoomCenter") + xsVectorSet(trQuestVarGet("xMod"),0,trQuestVarGet("zMod"));
				action = trGetNextUnitScenarioNameNumber();
				trQuestVarSetFromRand("heading", 1, 360, true);
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
				trUnitSelectClear();
				trUnitSelect(""+action,true);
				trUnitConvert(0);
				trMutateSelected(kbGetProtoUnitID("Ice Block"));
				xAddDatabaseBlock(dFrostGiantsIncoming, true);
				xSetInt(dFrostGiantsIncoming, xUnitName, action);
				xSetFloat(dFrostGiantsIncoming, xFrostTargetSize, 2);
				xSetInt(dFrostGiantsIncoming, xFrostTargetTime, trTimeMS() + 1000);
			}
		}
		for (x = xsMin(5,xGetDatabaseCount(dFrostGiantsIncoming)); > 0) {
			xDatabaseNext(dFrostGiantsIncoming);
			xUnitSelect(dFrostGiantsIncoming, xUnitName);
			amt = xGetInt(dFrostGiantsIncoming, xFrostTargetTime) - trTimeMS();
			if (amt > 0) {
				amt = xGetFloat(dFrostGiantsIncoming, xFrostTargetSize) - amt * 0.001;
				trSetSelectedScale(amt * 0.5, amt, amt * 0.5);
			} else if (xGetFloat(dFrostGiantsIncoming, xFrostTargetSize) >= 5) {
				trUnitChangeProtoUnit("Frost Giant");
				xUnitSelect(dFrostGiantsIncoming, xUnitName);
				trUnitConvert(ENEMY_PLAYER);
				activateEnemy(xGetInt(dFrostGiantsIncoming, xUnitName),0,0);
				trSoundPlayFN("mythcreate.wav","1",-1,"","");
				xFreeDatabaseBlock(dFrostGiantsIncoming);
			} else {
				amt = xGetFloat(dFrostGiantsIncoming, xFrostTargetSize);
				trSetSelectedScale(amt * 0.5, amt, amt * 0.5);
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
				trOverlayText("Ice Age",3.0,-1,-1,-1);
				trSetLighting("night", 1.0);
				trQuestVarSet("bossSpell", 32);
				bossNext = trTimeMS() + 1500;
				bossTimeout = trTimeMS() + 15000;
				if (kbGetBlockID(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,2)) >= 0) {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,2), true);
					trUnitChangeProtoUnit("Ice Sheet");
				} else {
					spyEffect(bossUnit,kbGetProtoUnitID("Ice Sheet"),xsVectorSet(ARRAYS,bossInts,2));
				}
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > bossNext) {
					trSoundPlayFN("pegasusflap.wav","1",-1,"","");
					bossNext = bossNext + 1500;
					pos = kbGetBlockPosition(""+bossUnit, true);
					for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
						xDatabaseNext(dPlayerCharacters);
						xUnitSelectByID(dPlayerCharacters, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerCharacter();
						} else {
							addGenericProj(dMedusaBalls,pos,
								getUnitVector(pos, kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName))),ENEMY_PLAYER);
							xSetInt(dMedusaBalls, xMedusaBallTarget, xGetInt(dPlayerCharacters, xUnitName));
							xSetInt(dMedusaBalls, xMedusaBallBounces, 10);
						}
					}
					if (trTimeMS() > bossTimeout) {
						bossCooldown(12, 18);
						trQuestVarSet("bossUltimate", 3);
						trSetLighting("default",1.0);
						trUnitSelectClear();
						trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,2), true);
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
				bossScale = 2;
				trSetSelectedScale(2, 2, 2);
				trQuestVarSet("bossSpell", 22);
				bossTimeout = trTimeMS() + 12000;
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, 100);
			} else if (trQuestVarGet("bossSpell") == 22) {
				id = kbGetBlockID(""+bossUnit);
				if (trQuestVarGet("bossAttacking") == 0) {
					if (kbUnitGetAnimationActionType(id) == 6) {
						trQuestVarSet("bossAttacking", 1);
						bossNext = trTimeMS() + 1000;
					}
				} else if (kbUnitGetAnimationActionType(id) == 6) {
					if (trTimeMS() > bossNext) {
						xsSetContextPlayer(ENEMY_PLAYER);
						target = kbUnitGetTargetUnitID(id);
						xsSetContextPlayer(0);
						bossPos = kbGetBlockPosition(""+bossUnit, true);
						bossNext = bossNext + 2000;
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							if (target == xGetInt(dPlayerUnits, xUnitID)) {
								if (trUnitAlive()) {
									pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName),true);
									growFrostGiantsIncoming(pos);
									pos = vectorSetAsTargetVector(bossPos, pos,100.0);
									launchUnit(dPlayerUnits,pos);
								}
								break;
							}
						}
					}
				} else {
					trQuestVarSet("bossAttacking", 0);
				}
				if (trTimeMS() > bossTimeout) {
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 27, -100);
					trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
					bossScale = 1.25;
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
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
				for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelectByID(dPlayerCharacters, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
						break;
					}
				}
				bossPos = vectorSnapToGrid(kbGetBlockPosition(""+bossUnit, true));
				
				bossAngle = angleBetweenVectors(bossPos, pos);
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("King Folstag"));
				bossDir = getUnitVector(bossPos, pos);
				trSetUnitOrientation(bossDir, vector(0,1,0), true);
				trUnitOverrideAnimation(3,1,false,false,-1);
				trQuestVarSet("bossSpell", 12);
				bossNext = trTimeMS() + 2000;
				trQuestVarSet("arm1", trGetNextUnitScenarioNameNumber());
				trQuestVarSet("arm2", trGetNextUnitScenarioNameNumber()+1);
				trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
				trArmySelect("1,0");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(vector(0,0,0) - bossDir,vector(0,1,0),true);
				trSetSelectedScale(2,0,30);
				trUnitHighlight(2.0, false);
				bossAnim = true;
			} else if (trQuestVarGet("bossSpell") == 12) {
				amt = bossNext - trTimeMS();
				if (amt < 0) {
					trQuestVarSet("bossSpell", 13);
					bossNext = trTimeMS()+1;
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
					current = fModulo(6.283185, bossAngle - angle - 3.141592);
					dir = xsVectorSet(xsSin(current),0,xsCos(current));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("arm2", true);
					trSetSelectedScale(amt, 0, 30);
					current = fModulo(6.283185, bossAngle + angle - 3.141592);
					dir = xsVectorSet(xsSin(current),0,xsCos(current));
					trSetUnitOrientation(dir,vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > bossNext) {
					trUnitOverrideAnimation(40,0,false,false,-1);
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					trQuestVarSet("bossSpell", 14);
					bossTimeout = trTimeMS() + 2100;
					bossNext = trTimeMS() + 500;
				}
			} else if (trQuestVarGet("bossSpell") == 14) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 100;
					trQuestVarSetFromRand("rand", -0.3, 0.3, false);
					current = fModulo(6.283185, bossAngle + trQuestVarGet("rand"));
					dir = xsVectorSet(xsSin(current),0,xsCos(current));
					addGenericProj(dBossBreath,bossPos,dir);
					if (trTimeMS() > bossTimeout) {
						trModifyProtounit("King Folstag", ENEMY_PLAYER, 55, 1);
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitOverrideAnimation(-1,1,false,true,-1);
						bossAnim = false;
						bossCooldown(10, 14);
					}
				}
			}
			if (trQuestVarGet("bossSpell") > 11) {
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trSetUnitOrientation(bossDir, vector(0,1,0), true);
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
					xSetFloat(dEnemies, xPhysicalResist, 1, bossPointer);
					xSetFloat(dEnemies, xMagicResist, 0, bossPointer);
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
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
					trMutateSelected(kbGetProtoUnitID("Ice Block"));
					trUnitSetAnimationPath("0,0,0,0,0,0,0");
					trSetSelectedScale(5,7,5);
				} else {
					trSoundPlayFN("sphinxteleportin.wav","1",-1,"","");
					trSoundPlayFN("godpower.wav","1",-1,"","");
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 24, -1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 25, -1);
					trModifyProtounit("King Folstag", ENEMY_PLAYER, 26, -1);
					xSetFloat(dEnemies, xPhysicalResist, 0, bossPointer);
					xSetFloat(dEnemies, xMagicResist, 1, bossPointer);
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
					if (kbGetBlockID(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0)) >= 0) {
						trUnitSelectClear();
						trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
						trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					} else {
						spyEffect(bossUnit, kbGetProtoUnitID("Vortex Finish Linked"), xsVectorSet(ARRAYS,bossInts,0));
					}
				}
				bossCooldown(5, 10);
			} else {
				trQuestVarSet("bossSpell", 11);
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		boss = 0;
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

int dChimeraClouds = 0;
int xChimeraCloudType = 0;

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
				
				dChimeraClouds = initGenericProj("chimeraClouds",kbGetProtoUnitID("Lampades Blood"),2,10.0,4.5,1.0,ENEMY_PLAYER,true);
				xChimeraCloudType = xInitAddInt(dChimeraClouds, "type");
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
				spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,0));
				spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,1));
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


rule boss4_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trUnitAlive() == true) {
		for (i=xsMin(3,xGetDatabaseCount(dChimeraClouds)); > 0) {
			action = processGenericProj(dChimeraClouds);
			if ((action == PROJ_FALLING) && xGetInt(dChimeraClouds, xChimeraCloudType) < 99) {
				pos = kbGetBlockPosition(""+xGetInt(dChimeraClouds, xUnitName), true);
				prev = xGetVector(dChimeraClouds, xProjPrev);
				amt = distanceBetweenVectors(pos, prev, false);
				if (amt > 4.0) {
					dir = xGetVector(dChimeraClouds, xProjDir);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits,prev,dir,amt + 2.0,4.0)) {
							poisonUnit(dPlayerUnits, 10, 30);
						}
					}
					xSetVector(dChimeraClouds, xProjPrev, pos);
				}
				if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					xFreeDatabaseBlock(dChimeraClouds);
				}
			} else if ((action == PROJ_GROUND) && xGetInt(dChimeraClouds, xChimeraCloudType) == 99) {
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
				xUnitSelectByID(dChimeraClouds, xUnitID);
				trDamageUnitPercent(-100);
				activateEnemy(xGetInt(dChimeraClouds, xUnitName),-1,0);
				xFreeDatabaseBlock(dChimeraClouds);
			}
		}
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trOverlayText("Land of Ashes", 3.0, -1, -1, -1);
				trSetLighting("night", 1.0);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				bossNext = trTimeMS() + 2000;
				trQuestVarSet("bossSpell", 32);
			} else if (trQuestVarGet("bossSpell") == 32) {
				/* fire carousel */
				trQuestVarSet("bossSpell", 33);
				if (kbGetBlockID(""+1*trQuestVarGet("bossCrossFireSFX")) == -1) {
					spyEffect(bossUnit,kbGetProtoUnitID("Meteorite"),xsVectorSet(ARRAYS,bossInts,0));
					spyEffect(bossUnit,kbGetProtoUnitID("Meteorite"),xsVectorSet(ARRAYS,bossInts,1));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				if ((trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) &&
					trTimeMS() > bossNext) {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
					trUnitOverrideAnimation(6,0,true,false,-1);
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					trSoundPlayFN("nidhoggflame1.wav","1",-1,"","");
					bossAngle = 0;
					trQuestVarSet("bossAngleMod", 0);
					bossTimeout = trTimeMS() + 12000;
					bossNext = trTimeMS();
					trQuestVarSet("bossSpell", 34);
				}
			} else if (trQuestVarGet("bossSpell") >= 34) {
				dist = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				bossAngle = fModulo(6.283185, bossAngle + 0.0004 * dist);
				dir = xsVectorSet(xsCos(bossAngle),0,xsSin(bossAngle));
				trQuestVarSet("bossSpell", 1 + trQuestVarGet("bossSpell"));
				if (trQuestVarGet("bossSpell") >= 36) {
					trQuestVarSet("bossSpell", 34);
					dir = vector(0,0,0) - dir;
				}
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0),true);
				trSetSelectedUpVector(4.0 * xsVectorGetX(dir),-1,4.0 * xsVectorGetZ(dir));
				dir = rotationMatrix(dir, 0, 1.0);
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1),true);
				trSetSelectedUpVector(4.0 * xsVectorGetX(dir),-1,4.0 * xsVectorGetZ(dir));
				if (trTimeMS() > bossNext + 100) {
					trQuestVarSet("bossAngleMod", fModulo(6.283185, trQuestVarGet("bossAngleMod") + 1.570796));
					angle = fModulo(6.283185, bossAngle + trQuestVarGet("bossAngleMod"));
					pos = xsVectorSet(xsCos(angle),0,xsSin(angle));
					bossNext = bossNext + 100;
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits,bossPos,pos,16,16)) {
							damagePlayerUnit(150);
						}
					}
				}
				if (trTimeMS() > bossTimeout) {
					trQuestVarSet("bossUltimate", 3);
					trSetLighting("default", 1.0);
					trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0),true);
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1),true);
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					bossCooldown(12, 16);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				xSetBool(dEnemies, xLaunched, true, bossPointer);
				trModifyProtounit("Chimera", ENEMY_PLAYER, 55, 2);
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				trSoundPlayFN("flamingweapons.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
				for(x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dPlayerCharacters);
					bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				}
				bossNext = trTimeMS() + 1500;
				trQuestVarSet("bossSpell", 22);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossDir = getUnitVector(bossPos, bossTargetPos);
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				bossAngle = angleBetweenVectors(bossPos, bossTargetPos);
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > bossNext) {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
					trUnitOverrideAnimation(19,0,false,false,-1);
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 23);
					bossNext =trTimeMS() + 600;
					bossTimeout = trTimeMS() + 2000;
					trQuestVarSet("bossBreath", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
					trArmySelect("1,0");
					trUnitConvert(0);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					bossAnim = true;
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				if (trTimeMS() > bossNext) {
					trCameraShake(1.5, 0.4);
					trSoundPlayFN("sonofosirisbolt.wav","1",-1,"","");
					bossAngle = fModulo(6.283185, bossAngle + 0.5);
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath");
					trUnitOverrideAnimation(6,0,true,false,-1);
					dir = xsVectorSet(xsSin(bossAngle),0,xsCos(bossAngle));
					trSetSelectedUpVector(10.0 * xsVectorGetX(dir),0,10.0 * xsVectorGetZ(dir));
					trQuestVarSet("bossSpell", 24);
					trVectorQuestVarSet("bossLastDir", dir);
				} else {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 24) {
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				amt = 1.0 * (trTimeMS() - bossNext) / 1400;
				amt = fModulo(6.283185, bossAngle - amt * 1.116);
				trUnitSelectClear();
				trUnitSelectByQV("bossBreath");
				dir = xsVectorSet(xsSin(amt),0,xsCos(amt));
				trSetSelectedUpVector(10.0 * xsVectorGetX(dir),0,10.0 * xsVectorGetZ(dir));
				amt = dotProduct(trVectorQuestVarGet("bossLastDir"), dir);
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
						prev = getUnitVector(bossPos, pos);
						if (dotProduct(prev, dir) > amt &&
							dotProduct(prev, trVectorQuestVarGet("bossLastDir")) > amt) {
							damagePlayerUnit(800);
						}
					}
				}
				trVectorQuestVarSet("bossLastDir", dir);
				
				if (trTimeMS() > bossTimeout) {
					bossAnim = false;
					xSetBool(dEnemies, xLaunched, false, bossPointer);
					trModifyProtounit("Chimera", ENEMY_PLAYER, 55, 1);
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath");
					trUnitChangeProtoUnit("Rocket");
					trSetLighting("default", 1.0);
					bossCooldown(10, 16);
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Chimera"));
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trSoundPlayFN("argusfreezeattack.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
				for(x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dPlayerCharacters);
					bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				}
				bossAnim = true;
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossDir = getUnitVector(bossPos, bossTargetPos);
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				trUnitOverrideAnimation(19,0,false,false,-1);
				bossAngle = angleBetweenVectors(bossPos, bossTargetPos);
				bossAngle = fModulo(6.283185, bossAngle + 0.558);
				trQuestVarSet("bossSpell", 12);
				bossNext =trTimeMS();
				bossCount = 8;
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 200;
					dir = xsVectorSet(xsSin(bossAngle),0,xsCos(bossAngle));
					addGenericProj(dChimeraClouds,bossPos,dir);
					xSetVector(dChimeraClouds, xProjPrev, bossPos);
					xSetInt(dChimeraClouds, xChimeraCloudType, STATUS_POISON);
					bossAngle = bossAngle - 0.139;
					bossCount = bossCount - 1;
					if (bossCount <= 0) {
						bossAnim = false;
						bossCooldown(10, 16);
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trMutateSelected(kbGetProtoUnitID("Chimera"));
						trUnitOverrideAnimation(-1,0,false,true,-1);
					}
				} else {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				if (trQuestVarGet("bossSummons") == 1) {
					trQuestVarSet("bossSummons", 0);
					trQuestVarSet("bossSpell", 11);
				} else {
					trSoundPlayFN("mummyconvert.wav","1",-1,"","");
					trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
					for(x=trQuestVarGet("rand"); >0) {
						xDatabaseNext(dPlayerCharacters);
						bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
					}
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					bossDir = getUnitVector(bossPos, bossTargetPos);
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
					trUnitOverrideAnimation(19,0,false,false,-1);
					bossAngle = fModulo(6.283185, angleBetweenVectors(bossPos, bossTargetPos) + 0.558);
					trQuestVarSet("bossSpell", 2);
					bossNext = trTimeMS();
					bossCount = 8;
					bossAnim = true;
				}
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 100;
					dir = xsVectorSet(xsSin(bossAngle),0,xsCos(bossAngle));
					trQuestVarSetFromRand("speed", 10.0, 20.0, false);
					addGenericProj(dChimeraClouds,bossPos,dir,ENEMY_PLAYER,trQuestVarGet("speed"));
					xSetVector(dChimeraClouds, xProjPrev, bossPos);
					xSetInt(dChimeraClouds, xChimeraCloudType, 99);
					xSetInt(dChimeraClouds, xProjProto, kbGetProtoUnitID("Kronny Birth SFX"));
					bossAngle = bossAngle - 0.139;
					bossCount = bossCount - 1;
					if (bossCount <= 0) {
						bossAnim = false;
						bossCooldown(15, 20);
						trQuestVarSet("bossSummons", 1);
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trMutateSelected(kbGetProtoUnitID("Chimera"));
						trUnitOverrideAnimation(-1,0,false,true,-1);
					}
				} else {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		boss = 0;
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

int dShadeBolts = 0;
int dShadeCloud = 0;
int dSplatterUnits = 0;

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
				
				dShadeBolts = initGenericProj("shadeBolts",kbGetProtoUnitID("Medusa"),40,9.0,5.0,0,ENEMY_PLAYER,true);
				
				dShadeCloud = initGenericProj("shadeCloud",kbGetProtoUnitID("Kronny Birth SFX"),2,20.0,4.5,0,0,true);
				
				dSplatterUnits = xInitDatabase("splatterUnits");
				xInitAddInt(dSplatterUnits, "name");
				xInitAddInt(dSplatterUnits, "index");
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


rule boss5_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
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
					pos = trVectorQuestVarGet("bossRoomLoc");
					x = xsVectorGetX(pos);
					z = xsVectorGetZ(pos);
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
						pos = xsVectorSet(2.0 * trQuestVarGet("bossRoomSize"),0,2.0 * trQuestVarGet("bossRoomSize"));
						vector bottom = trVectorQuestVarGet("bossRoomCenter") - pos;
						vector top = trVectorQuestVarGet("bossRoomCenter") + pos;
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else {
								pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
								if (vectorInRectangle(pos, bottom, top) == false) {
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
					pos = trVectorQuestVarGet("bossRoomLoc");
					x = xsVectorGetX(pos);
					z = xsVectorGetZ(pos);
					trPaintTerrain(x - action, z - action, x + action, z + action, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trQuestVarSet("bossChokeNext", trQuestVarGet("bossChokeNext") + 500);
					trQuestVarSet("bossChokeStep", 1);
					trQuestVarSet("bossChokeCount", trQuestVarGet("bossChokeCount") - 1);
				}
			}
		}
		for (i=xsMin(5, xGetDatabaseCount(dShadeBolts)); > 0) {
			action = processGenericProj(dShadeBolts);
			if (action == PROJ_FALLING) {
				pos = kbGetBlockPosition(""+xGetInt(dShadeBolts, xUnitName), true);
				hit = false;
				prev = xGetVector(dShadeBolts, xProjPrev);
				dir = xGetVector(dShadeBolts, xProjDir);
				amt = distanceBetweenVectors(pos, prev, false) + 1.0;
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits,xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (rayCollision(dPlayerUnits,prev,dir,amt,2.0)) {
						damagePlayerUnit(100);
						poisonUnit(dPlayerUnits, 10, 5 * trQuestVarGet("stage"));
						hit = true;
					}
				}
				if (hit || terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					xUnitSelectByID(dShadeBolts, xUnitID);
					trDamageUnitPercent(-100);
					trUnitChangeProtoUnit("Lightning sparks");
					xUnitSelectByID(dShadeBolts, xUnitID);
					trDamageUnitPercent(-100);
					if (hit) {
						trQuestVarSet("boltsound", 1);
					} else {
						trQuestVarSet("boltsound", 2);
					}
					xFreeDatabaseBlock(dShadeBolts);
				} else {
					xSetVector(dShadeBolts, xProjPrev, pos);
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
		trUnitSelect(""+bossUnit, true);
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown(21);
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Voiceless Scream", 3.0, -1, -1, -1);
				xSetBool(dEnemies, xLaunched, true, bossPointer);
				trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
				for(x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dPlayerCharacters);
				}
				bossAnim = true;
				trQuestVarSet("bossMove", 1.0);
				bossTarget = xGetInt(dPlayerCharacters, xUnitName);
				pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossAngle = angleBetweenVectors(bossPos, pos);
				bossDir = getUnitVector(bossPos, pos);
				dir = vector(0,0,0) - bossDir;
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				for(x=1; <3) {
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn"+x, true);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+bossUnit);
					trUnitChangeProtoUnit("Dwarf");
				}
				for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Dwarf"));
					trImmediateUnitGarrison(""+bossUnit);
					trUnitChangeProtoUnit("Dwarf");
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trMutateSelected(kbGetProtoUnitID("Rocket"));
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
				for(x=1; <3) {
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn"+x, true);
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trSetSelectedScale(3,0,60);
				}
				trQuestVarSet("bossSpell", 22);
				bossNext = trTimeMS() + 2000;
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 55, 2);
			} else if (trQuestVarGet("bossSpell") == 22) {
				amt = bossNext - trTimeMS();
				if (amt < 0) {
					bossAnim = true;
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trUnitOverrideAnimation(15,0,true,false,-1);
					for(x=1; <3) {
						trUnitSelectClear();
						trUnitSelectByQV("bossWarn"+x);
						trUnitChangeProtoUnit("Cinematic Block");
					}
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					bossTimeout = trTimeMS() + 9000;
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
					dist = fModulo(6.283185, bossAngle - angle - 3.141592);
					trSetUnitOrientation(xsVectorSet(xsSin(dist),0,xsCos(dist)),vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn2", true);
					dist = fModulo(6.283185, bossAngle + angle - 3.141592);
					trSetUnitOrientation(xsVectorSet(xsSin(dist),0,xsCos(dist)),vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				trUnitSelectClear();
				trUnitSelect(""+bossTarget, true);
				if (trUnitAlive()) {
					pos = kbGetBlockPosition(""+bossTarget, true);
				} else {
					xDatabaseNext(dPlayerCharacters);
					bossTarget = xGetInt(dPlayerCharacters, xUnitName);
				}
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				angle = fModulo(6.283185, bossAngle + amt * trQuestVarGet("bossMove") * 0.0003);
				bossAngle = angle;
				dir = xsVectorSet(xsSin(angle),0,xsCos(angle));
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trSetUnitOrientation(dir,vector(0,1,0),true);
				bossTargetPos = getUnitVector(bossPos, pos);
				trVectorQuestVarSet("bossDestDir", bossTargetPos);
				if (dotProduct(bossTargetPos, dir) < dotProduct(bossDir, bossTargetPos)) {
					trQuestVarSet("bossMove", 0.0 - trQuestVarGet("bossMove"));
				}
				bossDir = dir;
				vector first = xsVectorSet(xsSin(angle - 0.3),0,xsCos(angle - 0.3));
				vector second = xsVectorSet(xsSin(angle + 0.3),0,xsCos(angle + 0.3));
				angle = fModulo(6.283185, angle - 0.3);
				dir = xsVectorSet(xsSin(angle),0,xsCos(angle));
				for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
					trUnitSelectClear();
					trUnitSelect(""+x);
					trSetSelectedUpVector(5.0 * xsVectorGetX(dir),-1,5.0 * xsVectorGetZ(dir));
					dir = rotationMatrix(dir, 0.995004, -0.099833);
				}
				if (trTimeMS() > bossNext) {
					trCameraShake(0.5, 0.1);
					bossNext = 500 + bossNext;
					trQuestVarSetFromRand("sound", 1, 3, true);
					trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					amt = dotProduct(first, second);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits,xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
							dir = getUnitVector(bossPos, pos);
							if ((dotProduct(dir, first) > amt &&
									dotProduct(dir, second) > amt) ||
								distanceBetweenVectors(bossPos, pos) < 9.0) {
								damagePlayerUnit(200);
								silenceUnit(dPlayerUnits, 1.0);
								p = xGetInt(dPlayerUnits, xPlayerOwner);
								if (xGetInt(dPlayerUnits,xUnitName) == xGetInt(dPlayerData, xPlayerUnit, p)) {
									if (trCurrentPlayer() == p) {
										trCameraShake(0.5, 0.3);
									}
								}
							}
						}
					}
					if (trTimeMS() > bossTimeout) {
						bossAnim = false;
						trSetLighting("dawn", 1.0);
						trQuestVarSet("bossUltimate", 3);
						trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitOverrideAnimation(-1,0,false,true,-1);
						for(x=trQuestVarGet("bossScreamStart"); < trQuestVarGet("bossScreamEnd")) {
							trUnitSelectClear();
							trUnitSelect(""+x);
							trUnitChangeProtoUnit("Cinematic Block");
						}
						xSetBool(dEnemies, xLaunched, false, bossPointer);
						bossAnim = false;
						trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 55, 4);
						bossCooldown(12, 20);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSet("bossSpell", 12);
				bossCount = 3;
				bossAnim = true;
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
					for(x=trQuestVarGet("rand"); >0) {
						xDatabaseNext(dPlayerCharacters);
					}
					trModifyProtounit("Shade Of Hades", ENEMY_PLAYER, 55, 2);
					bossTargetPos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters,xUnitName),true));
					dir = getUnitVector(bossPos, bossTargetPos);
					bossDir = vector(0,0,0) - dir;
					bossPos = bossTargetPos + (dir * 2.0);
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Transport Ship Greek");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trImmediateUnitGarrison(""+action);
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trSetSelectedScale(bossScale,bossScale,bossScale);
					trUnitOverrideAnimation(1,0,false,false,-1);
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelect(""+action, true);
					trUnitChangeProtoUnit("Kronny Birth SFX");
					trVectorQuestVarSet("slashDir", xsVectorSet(0.0 - xsVectorGetZ(dir),0,xsVectorGetX(dir)));
					trQuestVarSet("bossSlash", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0", "Dwarf",1,xsVectorGetX(bossTargetPos),0,xsVectorGetZ(bossTargetPos),0,true);
					trArmySelect("1,0");
					trSetUnitOrientation(trVectorQuestVarGet("slashDir"),vector(0,1,0),true);
					trUnitChangeProtoUnit("Cinematic Block");
					trQuestVarSet("bossSpell", 13);
					bossNext = trTimeMS() + 1000;
				} else {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				if (trTimeMS() > bossNext) {
					trUnitSelectClear();
					trUnitSelectByQV("bossSlash");
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					trUnitSelectClear();
					trUnitSelectByQV("bossSlash");
					trSetSelectedScale(1,1,2);
					dir = trVectorQuestVarGet("slashDir");
					bossTargetPos = bossTargetPos - (dir * 4.0);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits, bossTargetPos, dir, 8.0, 2)) {
							damagePlayerUnit(500);
						}
					}
					bossNext = bossNext + 1000;
					bossCount = bossCount - 1;
					if (bossCount <= 0) {
						trQuestVarSet("bossSpell", 14);
					} else {
						trQuestVarSet("bossSpell", 12);
					}
				} else {
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
				}
			} else if (trQuestVarGet("bossSpell") == 14) {
				if (trTimeMS() > bossNext) {
					trModifyProtounit("Shade Of Hades", ENEMY_PLAYER, 55, 4);
					trUnitOverrideAnimation(-1,0,false,true,-1);
					bossCooldown(10, 16);
					bossAnim = false;
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trSoundPlayFN("shadeofhadesselect2.wav","1",-1,"","");
				xSetBool(dEnemies, xLaunched, true, bossPointer);
				trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
				for(x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dPlayerCharacters);
				}
				pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossDir = getUnitVector(bossPos, pos);
				dir = vector(0,0,0) - bossDir;
				bossAnim = true;
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
				trUnitSelectClear();
				trUnitSelectByQV("bossWarn1", true);
				trMutateSelected(kbGetProtoUnitID("Dwarf"));
				trImmediateUnitGarrison(""+1*bossUnit);
				trUnitChangeProtoUnit("Dwarf");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
				trUnitSelectClear();
				trUnitSelectByQV("bossWarn1", true);
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trSetSelectedScale(3,0,60);
				trQuestVarSet("bossSpell", 2);
				bossNext = trTimeMS() + 1000;
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					trSoundPlayFN("shadeofhadesacknowledge1.wav","1",-1,"","");
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossEscape"));
					trUnitSelectClear();
					trUnitSelectByQV("bossWarn1", true);
					trUnitChangeProtoUnit("Cinematic Block");
					addGenericProj(dShadeCloud,bossPos,bossDir);
					xSetVector(dShadeCloud, xProjPrev, bossPos);
					trQuestVarSet("bossSpell", 3);
					bossNext = trTimeMS() + 7000;
					xClearDatabase(dSplatterUnits);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xAddDatabaseBlock(dSplatterUnits, true);
						xSetInt(dSplatterUnits, xUnitName, xGetInt(dPlayerUnits, xUnitName));
						xSetInt(dSplatterUnits, xDatabaseIndex, xGetPointer(dPlayerUnits));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				action = processGenericProj(dShadeCloud);
				if (action == PROJ_FALLING) {
					bossPos = kbGetBlockPosition(""+xGetInt(dShadeCloud, xUnitName));
					prev = xGetVector(dShadeCloud, xProjPrev);
					dir = xGetVector(dShadeCloud, xProjDir);
					pos = bossPos + (dir * 2.0);
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						dir = getBounceDir(pos, vectorToGrid(pos), dir);
						xSetVector(dShadeCloud, xProjDir, dir);
						xSetInt(dShadeCloud, xProjYeehaw, 99);
						dir = rotationMatrix(wallNormalVector(vectorToGrid(pos)),0, -1.0);
						for(x=7; >0) {
							addGenericProj(dShadeBolts,prev,dir);
							xSetVector(dShadeBolts, xProjPrev, prev);
							dir = rotationMatrix(dir, 0.866025, 0.5);
						}
						
						xClearDatabase(dSplatterUnits);
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xAddDatabaseBlock(dSplatterUnits, true);
							xSetInt(dSplatterUnits, xUnitName, xGetInt(dPlayerUnits, xUnitName));
							xSetInt(dSplatterUnits, xDatabaseIndex, xGetPointer(dPlayerUnits));
						}
						
						trSoundPlayFN("buildingdeath.wav","1",-1,"","");
						trCameraShake(0.5, 0.3);
						
						if (terrainIsType(vectorToGrid(bossPos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							bossNext = trTimeMS();
							debugLog("Whoops! We entered the ZONE");
						}
					} else {
						dist = distanceBetweenVectors(pos, prev, false);
						for(x=xGetDatabaseCount(dSplatterUnits); >0) {
							xDatabaseNext(dSplatterUnits);
							xUnitSelect(dSplatterUnits, xUnitName);
							if (trUnitAlive() == false) {
								xFreeDatabaseBlock(dSplatterUnits);
							} else if (rayCollision(dSplatterUnits, prev, dir, dist + 1.0, 4.0)) {
								damagePlayerUnit(500, xGetInt(dSplatterUnits, xDatabaseIndex));
								trQuestVarSetFromRand("splatterSound", 1, 2, true);
								xFreeDatabaseBlock(dSplatterUnits);
							}
						}
						if (trQuestVarGet("splatterSound") > 0) {
							trSoundPlayFN("titanpunch"+1*trQuestVarGet("splatterSound")+".wav","1",-1,"","");
							trQuestVarSet("splatterSound", 0);
						}
						xSetVector(dShadeCloud, xProjPrev, bossPos);
					}
				}
				if (action == PROJ_REMOVE || trTimeMS() > bossNext) {
					if (action != PROJ_REMOVE) {
						bossPos = xGetVector(dShadeCloud, xProjPrev);
						xUnitSelectByID(dShadeCloud, xUnitID);
						trUnitChangeProtoUnit("Kronny Birth SFX");
						xFreeDatabaseBlock(dShadeCloud);
					}
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitChangeProtoUnit("Transport Ship Greek");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trImmediateUnitGarrison(""+action);
					trUnitChangeProtoUnit("Shade of Hades");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Shade of Hades"));
					trSetSelectedScale(bossScale, bossScale, bossScale);
					trUnitSelectClear();
					trUnitSelect(""+action, true);
					trUnitChangeProtoUnit("Kronny Birth SFX");
					bossCooldown(10, 15);
					bossAnim = false;
					xSetBool(dEnemies, xLaunched, false, bossPointer);
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(2, trUnitPercentDamaged() * 0.05), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 21 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 1, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
		xsDisableSelf();
		trMusicStop();
		boss = 0;
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

int dBossLasers = 0;
int xBossLaserNext = 0;
int xBossLaserStep = 0;
int xBossLaserPos = 0;

int dDeployRobots = 0;

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
					dYeebObelisks = xInitDatabase("yeebObelisks");
					xInitAddInt(dYeebObelisks, "name");

					trQuestVarSet("cinTime", trTime() + 6);
					int p = trQuestVarGet("yeebTarget");
					if (trCurrentPlayer() == p) {
						trQuestVarSet("yeebRelicRetrieved", 1);
					}
					int relics = getRelicsDB(p);
					for(x=xGetDatabaseCount(relics); >0) {
						xDatabaseNext(relics);
						if (xGetInt(relics, xUnitName) == trQuestVarGet("yeebRelic")) {
							xFreeDatabaseBlock(relics);
						}
					}
					trUnitSelectClear();
					trUnitSelectByQV("yeebRelic");
					trUnitDestroy();
					
					trSoundPlayFN("","1",-1,
						"Yeebaagooon:" + trStringQuestVarGet("p"+p+"name") + ". Did you really think I wouldn't notice you stealing my relic?",
						"icons\special e son of osiris icon 64");
			
					trQuestVarSet("cinStep", 1);
				} else {
					dBossLasers = xInitDatabase("bossLasers");
					xInitAddInt(dBossLasers, "name");
					xBossLaserNext = xInitAddInt(dBossLasers, "next");
					xBossLaserStep = xInitAddInt(dBossLasers, "step",1);
					xBossLaserPos = xInitAddVector(dBossLasers, "pos");
					
					dDeployRobots = initGenericProj("deployRobots",kbGetProtoUnitID("Statue of Automaton Base"),2,10.0,3.0,1,ENEMY_PLAYER);
					
					dSplatterUnits = xInitDatabase("splatterUnits");
					xInitAddInt(dSplatterUnits, "name");
					xInitAddInt(dSplatterUnits, "index");
					
					trOverlayText("The Exterminator", 3.0, -1, -1, -1);
					trQuestVarSet("cinTime", trTime() + 2);
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trUnitConvert(ENEMY_PLAYER);
					trSetSelectedScale(bossScale,bossScale,bossScale);
					spyEffect(bossUnit, kbGetProtoUnitID("Helepolis"), xsVectorSet(ARRAYS,bossInts,0));
					spyEffect(bossUnit, kbGetProtoUnitID("Barracks Atlantean"), xsVectorSet(ARRAYS,bossInts,1));
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
				addSpecialToDatabase(dBallistas,bossUnit,dEnemies,ENEMY_PLAYER);
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
				
				pos = xsVectorSet(2*trQuestVarGet("bossRoomSize"),0,2*trQuestVarGet("bossRoomSize"));
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


rule boss6_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float angle = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trUnitAlive() == true) {
		if (xGetDatabaseCount(dBossLasers) > 0) {
			xDatabaseNext(dBossLasers);
			if (trTimeMS() > xGetInt(dBossLasers, xBossLaserNext)) {
				switch(xGetInt(dBossLasers, xBossLaserStep))
				{
					case 1:
					{
						xSetInt(dBossLasers, xBossLaserStep, 2);
						for(x=0; < 4) {
							trUnitSelectClear();
							trUnitSelect(""+(x+xGetInt(dBossLasers, xUnitName)), true);
							trSetSelectedScale(6,6,60);
							trUnitHighlight(3, false);
						}
						pos = xGetVector(dBossLasers, xBossLaserPos);
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else {
								prev = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
								if (xsAbs(xsVectorGetX(prev) - xsVectorGetX(pos)) < 1.0 ||
									xsAbs(xsVectorGetZ(prev) - xsVectorGetZ(pos)) < 1.0) {
									damagePlayerUnit(300);
								}
							}
						}
						trSoundPlayFN("sky passage.wav","1",-1,"","");
						trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					}
					case 2:
					{
						amt = 500 - trTimeMS() + xGetInt(dBossLasers, xBossLaserNext);
						if (amt > 0) {
							amt = amt * 0.012;
							for(x=0; < 4) {
								trUnitSelectClear();
								trUnitSelect(""+(x+xGetInt(dBossLasers, xUnitName)), true);
								trSetSelectedScale(amt,amt,60);
							}
						} else {
							for(x=0; < 4) {
								trUnitSelectClear();
								trUnitSelect(""+(x+xGetInt(dBossLasers, xUnitName)), true);
								trUnitDestroy();
							}
							xFreeDatabaseBlock(dBossLasers);
						}
					}
				}
			}
		}
		if (xGetDatabaseCount(dDeployRobots) > 0) {
			if (processGenericProj(dDeployRobots) == PROJ_GROUND) {
				trDamageUnitPercent(-100);
				trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("bossSummonProto")));
				xUnitSelectByID(dDeployRobots, xUnitID);
				trDamageUnitPercent(-100);
				if (terrainIsType(vectorToGrid(kbGetBlockPosition(""+xGetInt(dDeployRobots, xUnitName), true)),
						TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trUnitChangeProtoUnit("Dust Large");
				} else {
					activateEnemy(xGetInt(dDeployRobots, xUnitName),-1,0);
				}
				xFreeDatabaseBlock(dDeployRobots);
			}
		}
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			trMutateSelected(kbGetProtoUnitID("Helepolis"));
			if (trQuestVarGet("bossSpell") == 31) {
				trStringQuestVarSet("advice", "Having trouble dodging the Extinction Cannon? Try equipping speed relics!");
				trSetLighting("night", 1.0);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trOverlayText("Extinction Cannon",3.0,-1,-1,-1);
				trQuestVarSet("bossSpell", 32);
				bossCount = 10;
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossDir = vectorSetFromAngle(bossAngle);
				
				trUnitSelectClear();
				trUnitSelectByQV("bossTurret", true);
				trUnitSetStance("Aggressive");
				trUnitTeleport(xsVectorGetX(bossPos),worldHeight+2.0,xsVectorGetZ(bossPos));
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				trUnitSelectClear();
				trUnitSelectByQV("bossBarrel", true);
				trUnitSetStance("Aggressive");
				trUnitTeleport(xsVectorGetX(bossPos),worldHeight+3.5,xsVectorGetZ(bossPos));
				trSetUnitOrientation(vector(0,1,0),bossDir,true);
			} else if (trQuestVarGet("bossSpell") == 32) {
				pos = kbGetBlockPosition(""+1*trQuestVarGet("bossTurret"));
				if (distanceBetweenVectors(pos, bossPos) < 1.0) {
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
				if (trTimeMS() > bossNext) {
					if (bossCount > 0) {
						trSetSelectedScale(1.5,1.0,1.5);
						bossDir = vectorSetFromAngle(bossAngle);
						trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
						for(x=trQuestVarGet("rand"); >0) {
							xDatabaseNext(dPlayerCharacters);
							xUnitSelectByID(dPlayerCharacters, xUnitID);
							if (trUnitAlive()) {
								trUnitHighlight(2.5, true);
								break;
							}
						}
						bossTarget = xGetInt(dPlayerCharacters, xUnitName);
						pos = kbGetBlockPosition(""+bossTarget, true);
						bossPrev = bossDir;
						bossTargetPos = getUnitVector(bossPos, pos);
						dir = rotationMatrix(bossDir,0,1.0);
						if (dotProduct(dir, bossTargetPos) > dotProduct(rotationMatrix(bossDir,0,-1.0), bossTargetPos)) {
							trQuestVarSet("bossTurnDir", -1.0);
						} else {
							trQuestVarSet("bossTurnDir", 1.0);
						}
						bossNext = trTimeMS() + 3000;
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
					}
				} else {
					amt = bossNext - trTimeMS();
					amt = 1.0 - 0.0005 * amt;
					trSetSelectedScale(1.5,amt,1.5);
				}
			} else if (trQuestVarGet("bossSpell") == 34) {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				angle = fModulo(6.283185, bossAngle + trQuestVarGet("bossTurnDir") * amt * 0.0016);
				bossAngle = angle;
				bossDir = vectorSetFromAngle(angle);
				pos = kbGetBlockPosition(""+bossTarget, true);
				/* unit isn't dead */
				if (xsVectorGetX(pos) > 0) {
					bossTargetPos = getUnitVector(bossPos, pos);
				}
				if (dotProduct(bossDir, bossTargetPos) < dotProduct(bossPrev, bossTargetPos) ||
					trTimeMS() > bossNext) {
					/* we passed them */
					trQuestVarSet("bossSpell", 35);
					bossNext = trTimeMS() + 1000;
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
					bossPrev = bossDir;
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossTurret", true);
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				trUnitSelectClear();
				trUnitSelectByQV("bossBarrel", true);
				trSetUnitOrientation(vector(0,1,0),bossDir,true);
			} else if (trQuestVarGet("bossSpell") == 35) {
				if (trTimeMS() > bossNext) {
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
					trSetUnitOrientation(vector(0,1,0), bossDir, true);
					trSetSelectedUpVector(xsVectorGetX(bossDir),0,xsVectorGetZ(bossDir));
					trUnitOverrideAnimation(18,0,false,false,-1);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBarrel"));
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedScale(0,0,0);
					
					
					trUnitSelectClear();
					trUnitSelectByQV("volcano2", true);
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetUnitOrientation(vector(0,1,0), bossDir, true);
					trSetSelectedUpVector(xsVectorGetX(bossDir),0,xsVectorGetZ(bossDir));
					trUnitOverrideAnimation(18,0,false,false,-1);
					trMutateSelected(kbGetProtoUnitID("Relic"));
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBarrel"));
					trMutateSelected(kbGetProtoUnitID("Volcano"));
					trSetSelectedScale(0,0,0);
					
					
					trUnitSelectClear();
					trUnitSelectByQV("bossBarrel");
					trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					bossCount = bossCount - 1;
					pos = bossPos;
					for(x=50; > 0) {
						pos = pos + (bossDir * 2.0);
						if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
							trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Meteor Impact Ground");
							trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
							trArmySelect("1,0");
							trMutateSelected(kbGetProtoUnitID("Meteor"));
							break;
						}
					}
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits,xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits,bossPos,bossDir,100.0,10.0) ||
							unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName),pos) < 25.0) {
							damagePlayerUnit(2000);
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
					bossNext = trTimeMS() + 1500;
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
				trQuestVarSetFromRand("rand", 5, 5 + ENEMY_PLAYER);
				bossCount = trQuestVarGet("rand");
				trQuestVarSet("bossSpell", 22);
				bossNext = trTimeMS();
				trSoundPlayFN("catapultattack.wav","1",-1,"","");
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 200;
					trQuestVarSetFromRand("rand", 0, 6.283185, false);
					dir = vectorSetFromAngle(trQuestVarGet("rand"));
					trQuestVarSetFromRand("speed", 5, 12, false);
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					addGenericProj(dDeployRobots,bossPos,dir,ENEMY_PLAYER,trQuestVarGet("speed"));
					bossCount = bossCount - 1;
					if (bossCount <= 0) {
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
				trQuestVarSet("bossStunIndex", xAddDatabaseBlock(dStunnedUnits, true));
				xSetInt(dStunnedUnits, xUnitName, bossUnit);
				xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Helepolis"));
				xDatabaseNext(dPlayerCharacters);
				bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				bossDir = getUnitVector(bossPos, bossTargetPos);
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				pos = bossTargetPos;
				for(x=50; >0) {
					pos = pos + (bossDir * 2.0);
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						break;
					} else {
						bossTargetPos = pos;
					}
				}
				bossNext = trTimeMS() + 1000;
				trQuestVarSet("bossSpell", 12);
				trQuestVarSet("bossCar", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trImmediateUnitGarrison(""+bossUnit);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					xFreeDatabaseBlock(dStunnedUnits, 1*trQuestVarGet("bossStunIndex"));
					trUnitSelectClear();
					trUnitSelectByQV("bossCar", true);
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Hero Greek Achilles");
					trUnitSelectClear();
					trUnitSelectByQV("bossCar", true);
					trSetUnitOrientation(bossDir,vector(0,1,0),true);
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
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
					trUnitMoveToPoint(xsVectorGetX(bossTargetPos),0,xsVectorGetZ(bossTargetPos),-1,false);
					trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 13);
					bossTimeout = trTimeMS() + 3000;
					xClearDatabase(dSplatterUnits);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xAddDatabaseBlock(dSplatterUnits, true);
						xSetInt(dSplatterUnits, xUnitName, xGetInt(dPlayerUnits, xUnitName));
						xSetInt(dSplatterUnits, xDatabaseIndex, xGetPointer(dPlayerUnits));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				if (distanceBetweenVectors(bossPos, bossTargetPos) < 4 ||
					trTimeMS() > bossTimeout) {
					trUnitChangeProtoUnit("Helepolis");
					trUnitSelectClear();
					trUnitSelectByQV("bossSFX");
					trDamageUnitPercent(100);
					trUnitSelectClear();
					trUnitSelectByQV("bossCar");
					trUnitChangeProtoUnit("Meteor Impact Ground");
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), bossPos) < 16) {
							damagePlayerUnit(500);
						}
					}
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
					trCameraShake(0.5,0.5);
					
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trSetSelectedScale(bossScale,bossScale,bossScale);
					bossCooldown(10, 16);
				} else {
					trQuestVarSet("sound", 0);
					for(x=xGetDatabaseCount(dSplatterUnits); >0) {
						xDatabaseNext(dSplatterUnits);
						xUnitSelect(dSplatterUnits, xUnitName);
						if (trUnitAlive() == false) {
							xFreeDatabaseBlock(dSplatterUnits);
						} else if (unitDistanceToVector(xGetInt(dSplatterUnits, xUnitName), bossPos) < 25) {
							if (xSetPointer(dPlayerUnits, xGetInt(dSplatterUnits, xDatabaseIndex))) {
								trQuestVarSet("sound", 1);
								damagePlayerUnit(500);
								id = xGetInt(dPlayerUnits, xUnitID);
								xsSetContextPlayer(xGetInt(dPlayerUnits, xPlayerOwner));
								action = kbUnitGetCurrentHitpoints(id);
								xsSetContextPlayer(0);
								if (action > 0) {
									pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
									pos = pos - bossPos + bossTargetPos;
									launchUnit(dPlayerUnits, pos);
								} else {
									trSetSelectedUpVector(5.0*xsVectorGetX(bossDir),-0.5,5.0*xsVectorGetZ(bossDir));
								}
							}
							xFreeDatabaseBlock(dSplatterUnits);
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
				bossNext = trTimeMS();
				bossCount = 5;
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 1000;
					xDatabaseNext(dPlayerCharacters);
					pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos)+2,180,true);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos)-2,0,false);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)+2,0,xsVectorGetZ(pos),270,false);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-2,0,xsVectorGetZ(pos),90,false);
					trArmySelect("1,0");
					trUnitSetStance("Passive");
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetSelectedScale(3,0,60);
					xAddDatabaseBlock(dBossLasers, true);
					xSetInt(dBossLasers, xUnitName, action);
					xSetInt(dBossLasers, xBossLaserNext, trTimeMS() + 2000);
					xSetVector(dBossLasers, xBossLaserPos, pos);
					trSoundPlayFN("skypassagein.wav","1",-1,"","");
					bossCount = bossCount - 1;
					if (bossCount <= 0) {
						bossCooldown(9, 15);
					}
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		boss = 0;
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

rule yeebaagooon_battle
inactive
highFrequency
{
	
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);

	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	if (trUnitAlive() == true) {
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
				if (kbGetBlockID(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,yeebShieldSFX)) == -1) {
					trQuestVarSet("bossSpell", 52);
					spyEffect(1*trQuestVarGet("yeebaagooon"),
						kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,yeebShieldSFX));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,yeebShieldSFX), true);
					trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
					trSetSelectedScale(2,2,2);
					trQuestVarSet("bossSpell", 53);
				}
				xSetFloat(dEnemies, xPhysicalResist, 1.0, bossPointer);
				xSetFloat(dEnemies, xMagicResist, 1.0, bossPointer);
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 24, 1);
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 25, 1);
				trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 26, 1);
				
				trQuestVarSetFromRand("bossDirection", 0, 1, true);
				bossCount = 9;
				bossNext = trTimeMS();
				pos = trVectorQuestVarGet("bossRoomLower");
				trQuestVarSet("lower", xsVectorGetX(pos) + 2);
				amt = trVectorQuestVarGetX("bossRoomUpper") - xsVectorGetX(pos);
				for(x=0; <= 1) {
					for(z=0; <= 1) {
						action = trGetNextUnitScenarioNameNumber();
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("lower") + amt * x, 0, trQuestVarGet("lower") + amt * z, 0, true);
						trUnitSelectClear();
						trUnitSelect(""+action, true);
						trUnitConvert(ENEMY_PLAYER);
						trUnitChangeProtoUnit("Outpost");
						xAddDatabaseBlock(dYeebObelisks, true);
						xSetInt(dYeebObelisks, xUnitName, action);
					}
				}
			} else if (trQuestVarGet("bossSpell") == 52) {
				if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,yeebShieldSFX),true);
					trMutateSelected(kbGetProtoUnitID("Phoenix Egg"));
					trSetSelectedScale(2,2,2);
					trQuestVarSet("bossSpell", 53);
				}
			} else if (trQuestVarGet("bossSpell") >= 53) {
				xDatabaseNext(dYeebObelisks);
				xUnitSelect(dYeebObelisks, xUnitName);
				if (trUnitAlive() == false) {
					xFreeDatabaseBlock(dYeebObelisks);
					if (xGetDatabaseCount(dYeebObelisks) == 0) {
						bossCooldown(1, 2);
						xSetFloat(dEnemies, xPhysicalResist, 0.47, bossPointer);
						xSetFloat(dEnemies, xMagicResist, 0.47, bossPointer);
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 24, -0.53);
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 25, -0.53);
						trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 26, -0.53);
						trUnitSelectClear();
						trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,yeebShieldSFX),true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					}
				}
				if (trTimeMS() > bossNext) {
					if (trQuestVarGet("bossSpell") == 53) {
						bossNext = bossNext + 1000;
						for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
							xDatabaseNext(dPlayerCharacters);
							xUnitSelectByID(dPlayerCharacters, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerCharacter();
							} else {
								pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
								spawnLightning(pos, ENEMY_PLAYER);
							}
						}
						bossCount = bossCount - 1;
						if (bossCount == 0) {
							trQuestVarSet("bossSpell", 54);
							trQuestVarSet("bossDirection", 1 - trQuestVarGet("bossDirection"));
							pos = trVectorQuestVarGet("bossRoomUpper");
							trQuestVarSet("lightningLineCount", (xsVectorGetX(pos) - trVectorQuestVarGetX("bossRoomLower")) / 4);
							trVectorQuestVarSet("lightningLineStart", pos - vector(1.0,0,1.0));
							if (trQuestVarGet("bossDirection") == 1) {
								trVectorQuestVarSet("lineDir", vector(-4,0,0));
								trVectorQuestVarSet("lineMove", vector(0,0,-2));
							} else if (trQuestVarGet("bossDirection") == 0) {
								trVectorQuestVarSet("lineDir", vector(0,0,-4));
								trVectorQuestVarSet("lineMove", vector(-2,0,0));
							}
						}
					} else {
						while(trTimeMS() > bossNext) {
							bossNext = trTimeMS() + 100;
							pos = trVectorQuestVarGet("lightningLineStart");
							for(x=trQuestVarGet("lightningLineCount"); >0) {
								spawnLightning(pos, ENEMY_PLAYER);
								pos = pos + trVectorQuestVarGet("lineDir");
							}
							trVectorQuestVarSet("lightningLineStart", trVectorQuestVarGet("lightningLineStart") + trVectorQuestVarGet("lineMove"));
							if (terrainIsType(vectorToGrid(trVectorQuestVarGet("lightningLineStart")), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
								trQuestVarSet("bossSpell", 53);
								bossCount = 10;
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
				bossNext = trTimeMS() + 3000;
				xDatabaseNext(dPlayerCharacters);
				trVectorQuestVarSet("hammerPos", kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
				dir = vector(0,0,-16);
				trQuestVarSet("heading", 360);
				trQuestVarSet("hammerIndicatorsStart", trGetNextUnitScenarioNameNumber());
				for(x=8; >0) {
					pos = trVectorQuestVarGet("hammerPos") + dir;
					if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
						trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
					}
					trQuestVarSet("heading", trQuestVarGet("heading") - 45);
					dir = rotationMatrix(dir, 0.707107, 0.707107);
				}
				trQuestVarSet("hammerIndicatorsEnd", trGetNextUnitScenarioNameNumber());
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > bossNext) {
					trQuestVarSet("bossSpell", 33);
					zSetProtoUnitStat("Kronny Flying", 0, 1, 0.001);
					pos = trVectorQuestVarGet("hammerPos");
					trQuestVarSet("hammerObject", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Kronny Flying",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
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
					bossNext = trTimeMS() + 1000;
					trQuestVarSet("bossSpell", 35);
					trMutateSelected(kbGetProtoUnitID("Thor Hammer"));
					trSetSelectedScale(2,-2,2);
					trUnitOverrideAnimation(2,0,true,false,-1);
					trUnitSetAnimationPath("0,0,0,0,0,0,0");
				}
			} else if (trQuestVarGet("bossSpell") == 35) {
				if (trTimeMS() > bossNext) {
					trQuestVarSet("bossSpell", 36);
					bossCount = 0;
					trQuestVarSet("bossRadius", 0);
					trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
					trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
					trCameraShake(1.0, 0.5);
					bossDir = vector(1,0,0);
					for(x=trQuestVarGet("hammerIndicatorsStart"); < trQuestVarGet("hammerIndicatorsEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trUnitDestroy();
					}
				}
			} else if (trQuestVarGet("bossSpell") == 36) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 200;
					bossCount = 1 + bossCount;
					angle = 3.141592 / bossCount;
					trQuestVarSet("cos", xsCos(angle));
					trQuestVarSet("sin", xsSin(angle));
					for(x=bossCount*2; >0) {
						bossDir = rotationMatrix(bossDir, trQuestVarGet("cos"), trQuestVarGet("sin"));
						pos = trVectorQuestVarGet("hammerPos") + (bossDir * trQuestVarGet("bossRadius"));
						trArmyDispatch("1,0","Meteor Impact Ground",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					}
					trQuestVarSet("bossRadius", trQuestVarGet("bossRadius") + 4);
					dist = xsPow(trQuestVarGet("bossRadius"), 2);
					pos = trVectorQuestVarGet("hammerPos");
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
							trDamageUnitPercent(100);
						}
					}
					
					if (boss > 999) {
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
								trDamageUnitPercent(100);
							}
						}
					}
					
					if (bossCount == 4) {
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
				pos = kbGetBlockPosition(""+1*trQuestVarGet("yeebaagooon"),true);
				trQuestVarSetFromRand("rand", 0, 3.14, false);
				dir = vectorSetFromAngle(trQuestVarGet("rand"));
				for(x=8; >0) {
					addGenericProj(dYeebLightningBalls,pos,dir,ENEMY_PLAYER);
					xSetVector(dYeebLightningBalls, xProjPrev, pos);
					xSetInt(dYeebLightningBalls, xProjDist, 3);
					dir = rotationMatrix(dir, 0.707107, 0.707107);
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
				trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dPlayerCharacters), true);
				for(x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dPlayerCharacters);
				}
				trVectorQuestVarSet("cageCenter", vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true)));
				trQuestVarSet("cageCount", 12);
				trQuestVarSet("cageRadius", 6);
				trQuestVarSet("bossSpell", 12);
				bossNext = trTimeMS();
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 100;
					dir = xsVectorSet(2 * trQuestVarGet("cageRadius"), 0, 2 * (trQuestVarGet("cageCount") - trQuestVarGet("cageRadius")));
					for(x=4; >0) {
						pos = dir + trVectorQuestVarGet("cageCenter");
						spawnLightning(pos, ENEMY_PLAYER);
						dir = rotationMatrix(dir, 0, -1.0);
					}
					trQuestVarSet("cageCount", trQuestVarGet("cageCount") - 1);
					if (trQuestVarGet("cageCount") == 0) {
						trQuestVarSet("cageRadius", trQuestVarGet("cageRadius") - 1);
						trQuestVarSet("cageCount", trQuestVarGet("cageRadius") * 2);
						if (trQuestVarGet("cageRadius") <= 0) {
							bossCooldown(6, 12);
							if (boss > 999) {
								pos = trVectorQuestVarGet("cageCenter");
								trQuestVarSet("boat", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								trUnitSelectClear();
								trUnitSelectByQV("boat", true);
								trUnitConvert(0);
								trUnitChangeProtoUnit("Transport Ship Greek");
								trUnitSelectClear();
								trUnitSelectByQV("yeebBird", true);
								trMutateSelected(kbGetProtoUnitID("Dwarf"));
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
				bossTimeout = trTimeMS() + 500;
				bossNext = 500;
				trQuestVarSet("zappyStart", trQuestVarGet("yeebaagooon"));
				z = 0;
				for(x=1; < ENEMY_PLAYER) {
					if (xGetInt(dPlayerData, xPlayerDead, x) == 0) {
						z = z + 1;
						trQuestVarSet("choose"+z, x);
					}
				}
				trQuestVarSetFromRand("zappyTarget", 1, z, true);
				p = trQuestVarGet("choose"+1*trQuestVarGet("zappyTarget"));
				trQuestVarSet("zappyTarget", p);
				trQuestVarSet("zappyEnd", xGetInt(dPlayerData, xPlayerUnit, p));
				trQuestVarSet("zappyIndex", xGetPointer(dPlayerUnits));
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (bossTimeout - trTimeMS() < bossNext) {
					bossNext = bossNext - 100;
					dist = bossNext / 500;
					pos = kbGetBlockPosition(""+1*trQuestVarGet("zappyEnd"), true);
					pos = pos + (kbGetBlockPosition(""+1*trQuestVarGet("zappyStart"),true) - pos) * dist;
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Lightning sparks Ground");
					if (bossNext <= 0) {
						if (boss > 999) {
							spawnRelicSpecific(pos, RELIC_MAGIC_DETECTOR);
						}
						trUnitSelectClear();
						trUnitSelectByQV("zappyEnd", true);
						trUnitHighlight(0.2, false);
						damagePlayerUnit(150, 1*trQuestVarGet("zappyIndex"));
						p = trQuestVarGet("zappyTarget");
						action = xGetInt(dPlayerData, xPlayerClass, p);
						trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dClass, xClassProto, action)));
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
		} else if (boss > 999) {
			
			trQuestVarSetFromRand("bossSpell", 0, 3, true);
			trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(5, (trUnitPercentDamaged() * 0.05)), true);
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
		
		uiLookAtUnitByName(""+bossUnit);
	}
	
	if (trQuestVarGet("gameOverStep") > 0) {
		trQuestVarSet("gameOverStep", 7);
		trQuestVarSet("yeebhit", 0);
		xsDisableSelf();
	}
}

int dYeebFeathers = 0;
int xYeebFeatherPos = 0;

rule yeebaagooon_ded
inactive
highFrequency
{
	int x = 0;
	vector pos = vector(0,0,0);
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
				dYeebFeathers = xInitDatabase("yeebFeathers");
				xInitAddInt(dYeebFeathers, "name");
				xYeebFeatherPos = xInitAddVector(dYeebFeathers, "pos");
			}
			case 2:
			{
				if (boss == 1) {
					trPaintTerrain(30, 10, 32, 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trPaintTerrain(10, 30, 30, 32, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trChangeTerrainHeight(31, 10, 32, 32, worldHeight, false);
					trChangeTerrainHeight(10, 31, 32, 32, worldHeight, false);
				} else {
					pos = trVectorQuestVarGet("bossRoomEntrance");
					trPaintTerrain(xsVectorGetX(pos), xsVectorGetZ(pos) - 3,
						xsVectorGetX(pos) + 35, xsVectorGetZ(pos),
						TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY);
					trChangeTerrainHeight(xsVectorGetX(pos), xsVectorGetZ(pos) - 3,
						xsVectorGetX(pos) + 35, xsVectorGetZ(pos),worldHeight,false);
					trPaintTerrain(xsVectorGetX(pos)-4, xsVectorGetZ(pos),
						xsVectorGetX(pos), xsVectorGetZ(pos) + 35,
						TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY);
					trChangeTerrainHeight(xsVectorGetX(pos) - 3, xsVectorGetZ(pos),
						xsVectorGetX(pos), xsVectorGetZ(pos) + 35,worldHeight,false);
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
				pos = trVectorQuestVarGet("yeebPos");
				trQuestVarSet("yeebOverNext", trTime() + 6);
				trQuestVarSet("yeebBird", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Stymphalian Bird",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("yeebBird", true);
				trTechGodPower(ENEMY_PLAYER, "spy", 2);
				trTechInvokeGodPower(ENEMY_PLAYER,"spy",vector(1,1,1),vector(1,1,1));
				x = modularCounterNext("spyFind");
				aiPlanSetUserVariableInt(ARRAYS,spyProto,x,kbGetProtoUnitID("Pharaoh of Osiris"));
				aiPlanSetUserVariableInt(ARRAYS,spyUnit,x,1*trQuestVarGet("yeebBird"));
				aiPlanSetUserVariableBool(ARRAYS,spyActive,x,true);
				aiPlanSetUserVariableVector(ARRAYS,spyDest,x,xsVectorSet(ARRAYS,bossInts,1));
				aiPlanSetUserVariableVector(ARRAYS,spyScale,x,vector(1,1,1));
				// thunderclouds
				trTechInvokeGodPower(ENEMY_PLAYER,"spy",vector(1,1,1),vector(1,1,1));
				x = modularCounterNext("spyFind");
				aiPlanSetUserVariableInt(ARRAYS,spyProto,x,kbGetProtoUnitID("Cinematic Block"));
				aiPlanSetUserVariableInt(ARRAYS,spyUnit,x,1*trQuestVarGet("yeebBird"));
				aiPlanSetUserVariableBool(ARRAYS,spyActive,x,true);
				aiPlanSetUserVariableVector(ARRAYS,spyDest,x,xsVectorSet(ARRAYS,bossInts,2));
				aiPlanSetUserVariableVector(ARRAYS,spyScale,x,vector(1,1,1));
			}
			case 4:
			{
				trQuestVarSet("yeebBirdID", kbGetBlockID(""+1*trQuestVarGet("yeebBird")));
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,2), true);
				trUnitChangeProtoUnit("Lightning Cloud");
				trQuestVarSet("yeebaagooon", aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
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
				bossUnit = trGetNextUnitScenarioNameNumber();
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Cinematic Block");
				xsEnableRule("yeebaagooon_battle");
				bossCooldown(10, 12);
				trQuestVarSet("musicTime", 0);
				xsEnableRule("boss_music");
				trVectorQuestVarSet("yeebDir", vector(1,0,0));
				trQuestVarSet("yeebLightningNext", trTimeMS());
				if (boss == 1) {
					trQuestVarSet("yeebBossFight", 0);
					boss = 1000;
					trVectorQuestVarSet("yeebDestination", trVectorQuestVarGet("bossRoomCenter"));
				} else {
					boss = 1001;
					trVectorQuestVarSet("yeebDestination", trVectorQuestVarGet("startPosition"));
					trArmyDispatch("0,0","Gaia Forest effect",1,
						trVectorQuestVarGetX("startPosition"),0,trVectorQuestVarGetZ("startPosition"),0,true);
				}
				trQuestVarSet("yeebLatestFeather", trGetNextUnitScenarioNameNumber() - 1);
				
			}
			case 5:
			{
				if (boss == 1000) {
					trMessageSetText("Reach the boss room to escape. You will get to keep the stolen relic.", 60);
				} else {
					trMessageSetText("Return to the starting room to escape. You will get to keep the stolen relic.", 60);
				}
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
	vector pos = vector(0,0,0);
	if (kbUnitGetAnimationActionType(1*trQuestVarGet("yeebBirdID")) == 9) {
		if (trTime() > trQuestVarGet("yeebBirdMoveTime")) {
			trQuestVarSet("yeebBirdMoveTime", trTime());
			xDatabaseNext(dPlayerUnits);
			pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
			trUnitSelectClear();
			trUnitSelectByQV("yeebBird", true);
			trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,true);
		}
	}
	
	if (xGetDatabaseCount(dYeebFeathers) > 0) {
		hit = false;
		xDatabaseNext(dYeebFeathers);
		id = kbGetBlockID(""+xGetInt(dYeebFeathers, xUnitName));
		xUnitSelect(dYeebFeathers, xUnitName);
		if (id == -1) {
			pos = xGetVector(dYeebFeathers, xYeebFeatherPos);
			hit = true;
		} else {
			pos = kbGetBlockPosition(""+xGetInt(dYeebFeathers, xUnitName), true);
			if (xsVectorGetY(pos) < worldHeight + 0.5) {
				hit = true;
			} else {
				xSetVector(dYeebFeathers, xYeebFeatherPos, pos);
			}
		}
		
		if (hit) {
			spawnLightning(pos, ENEMY_PLAYER);
			xFreeDatabaseBlock(dYeebFeathers);
		}
	}
	
	while (yFindLatest("yeebLatestFeather", "Stymph Bird Feather", 0) > 0) {
		pos = kbGetBlockPosition(""+1*trQuestVarGet("yeebLatestFeather"), true);
		xAddDatabaseBlock(dYeebFeathers, true);
		xSetInt(dYeebFeathers, xUnitName, 1*trQuestVarGet("yeebLatestFeather"));
		xSetVector(dYeebFeathers, xYeebFeatherPos, pos);
		trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
	}
	
	if (trTimeMS() > trQuestVarGet("yeebLightningNext")) {
		pos = kbGetBlockPosition(""+1*trQuestVarGet("yeebaagooon"), true);
		trQuestVarSet("yeebLightningNext",
			trQuestVarGet("yeebLightningNext") + 2.0 * distanceBetweenVectors(pos, trVectorQuestVarGet("yeebDestination"), false));
		trQuestVarSetFromRand("rand", 4, 20, false);
		trVectorQuestVarSet("yeebDir", rotationMatrix(trVectorQuestVarGet("yeebDir"), -0.757323, 0.653041));
		pos = vectorSnapToGrid(pos + (trVectorQuestVarGet("yeebDir") * trQuestVarGet("rand")));
		spawnLightning(pos, ENEMY_PLAYER);
	}
	
	
	if (trQuestVarGet("gameOverStep") > 0) {
		xsDisableSelf();
	} else if ((boss == 1001) && (trQuestVarGet("deadPlayerCount") < trQuestVarGet("activePlayerCount"))) {
		
		int escape = 0;
		pos = trVectorQuestVarGet("yeebDestination");
		for(p=1; < ENEMY_PLAYER) {
			if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), pos) < 400) {
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
	} else if (boss < 1000) {
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

int dBossWhirlpoolBalls = 0;
int xBossWhirlpoolBallLast = 0;
int bossJumpPath = 0;

rule boss7_init
inactive
highFrequency
{
	vector pos = vector(0,0,0);
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
				dBossWhirlpoolBalls = initGenericProj("bossWhirlpoolBalls",
					kbGetProtoUnitID("Pharaoh of Osiris XP"),50,8.0,5.0,0,ENEMY_PLAYER,true);
				xBossWhirlpoolBallLast = xInitAddInt(dBossWhirlpoolBalls, "last");
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
				
				pos = trVectorQuestVarGet("bossRoomCenter");
				bossJumpPath = zNewArray(mInt, 50, "jumpPath");
				for(i=0; < 50) {
					aiPlanSetUserVariableInt(ARRAYS,bossJumpPath,i,trGetNextUnitScenarioNameNumber());
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
				
				spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,0));
				spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS,bossInts,1));
				
				trStringQuestVarSet("advice",
					"She will heal if she eats someone. You can poison her to prevent the healing.");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}


rule boss7_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
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
	
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (trUnitAlive() == true) {
		if (trQuestVarGet("secondPhase") == 1) {
			if (trTime() > trQuestVarGet("bossSpawnNext")) {
				trQuestVarSet("bossSpawnNext", trTime() + 30);
				trQuestVarSetFromRand("rand", 1, 6, true);
				trQuestVarSetFromRand("count", 3, 5, true);
				pos = kbGetBlockPosition(""+bossUnit);
				for(i=trQuestVarGet("count"); >0) {
					trQuestVarSetFromRand("heading", 1, 360, false);
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch(""+ENEMY_PLAYER+",0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("rand")),1,
						xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
					activateEnemy(action,-1,0);
				}
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
				trOverlayText("Drowning Whirlpool",3.0,-1,-1,-1);
				trQuestVarSet("bossSpell", 32);
				bossNext = trTimeMS();
				bossTimeout = trTimeMS() + 2000;
				bossAngle = 0;
				trQuestVarSet("bossAngularVelocity", 0.1);
				if (kbGetBlockID(""+1*trQuestVarGet("bossWhirlpoolSFX")) == -1) {
					spyEffect(1*bossUnit,kbGetProtoUnitID("Kronny Birth"), xsVectorSet(ARRAYS,bossInts,0));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0), true);
					trMutateSelected(kbGetProtoUnitID("Kronny Birth"));
				}
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Scylla"));
			} else if ((trQuestVarGet("bossSpell") >= 32) && (trQuestVarGet("bossSpell") <= 33)) {
				trMutateSelected(kbGetProtoUnitID("Scylla"));
				amt = trTimeMS() - bossNext;
				bossNext = trTimeMS();
				if (trQuestVarGet("bossSpell") == 32) {
					trQuestVarSet("bossAngularVelocity", trQuestVarGet("bossAngularVelocity") + 0.006 * amt);
				} else {
					bossScale = xsMax(0, bossScale - amt * 0.0013);
					trSetSelectedScale(bossScale,1.3,bossScale);
				}
				bossAngle = fModulo(6.283185, bossAngle - 0.001 * amt * trQuestVarGet("bossAngularVelocity"));
				dir = vectorSetFromAngle(bossAngle);
				trSetUnitOrientation(dir,vector(0,1,0),true);
				if (trTimeMS() > bossTimeout) {
					if (trQuestVarGet("bossSpell") == 32) {
						trQuestVarSet("bossSpell", 33);
						bossTimeout = bossTimeout + 1000;
					} else {
						trSetSelectedScale(0,0,0);
						trQuestVarSet("bossSpell", 34);
						bossTimeout = trTimeMS() + 15000;
						bossNext = trTimeMS();
						bossDir = vector(1,0,0);
						trModifyProtounit("Scylla", ENEMY_PLAYER, 1, -2);
					}
				}
			} else if (trQuestVarGet("bossSpell") == 34) {
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				for(i=xsMin(6, xGetDatabaseCount(dBossWhirlpoolBalls)); >0) {
					action = processGenericProj(dBossWhirlpoolBalls);
					if (action == PROJ_FALLING) {
						hit = false;
						pos = kbGetBlockPosition(""+xGetInt(dBossWhirlpoolBalls, xUnitName), true);
						prev = xGetVector(dBossWhirlpoolBalls, xProjPrev);
						dir = xGetVector(dBossWhirlpoolBalls, xProjDir);
						dist = distanceBetweenVectors(pos, prev, false) + 2.0;
						for(j=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else if (rayCollision(dPlayerUnits,prev,dir,dist,4.0)) {
								damagePlayerUnit(300);
								hit = true;
							}
						}
						if (hit) {
							trQuestVarSetFromRand("sound", 1, 3, true);
							trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
							xUnitSelectByID(dBossWhirlpoolBalls, xUnitID);
							trUnitChangeProtoUnit("Rocket");
							xFreeDatabaseBlock(dBossWhirlpoolBalls);
						} else if (unitDistanceToVector(xGetInt(dBossWhirlpoolBalls, xUnitName), bossPos) < 36.0) {
							xUnitSelectByID(dBossWhirlpoolBalls, xUnitID);
							trUnitChangeProtoUnit("Rocket");
							xFreeDatabaseBlock(dBossWhirlpoolBalls);
						} else {
							amt = 0.004 * (trTimeMS() - xGetInt(dBossWhirlpoolBalls, xBossWhirlpoolBallLast));
							xSetInt(dBossWhirlpoolBalls, xBossWhirlpoolBallLast, trTimeMS());
							xSetVector(dBossWhirlpoolBalls, xProjPrev, pos);
							dir = xsVectorNormalize(dir + (getUnitVector(pos, bossPos) * amt));
							xSetVector(dBossWhirlpoolBalls,xProjDir,dir);
						}
					}
				}
				if (trTimeMS() > bossNext) {
					bossDir = rotationMatrix(bossDir, -0.757323, 0.653041);
					dir = rotationMatrix(bossDir, -0.707107, 0.707107);
					pos = trVectorQuestVarGet("bossRoomCenter") + (bossDir * (2.0 * trQuestVarGet("bossRoomSize")));
					addGenericProj(dBossWhirlpoolBalls, pos,dir);
					xSetVector(dBossWhirlpoolBalls, xProjPrev, pos);
					xSetInt(dBossWhirlpoolBalls, xBossWhirlpoolBallLast, trTimeMS());
					bossNext = bossNext + 300;
				}
				if (trTimeMS() > bossTimeout) {
					bossCooldown(10, 15);
					trQuestVarSet("bossUltimate", 3);
					xClearDatabase(dBossWhirlpoolBalls);
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,0));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					bossScale = 1.3;
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trSetSelectedScale(bossScale,bossScale,bossScale);
					trSetLighting("Fimbulwinter", 1.0);
					trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
					trModifyProtounit("Scylla", ENEMY_PLAYER, 1, 2);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: I'm going to eat you!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: This one looks tasty!");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Mother of the Depths: Delicious!");
				}
				trSoundPlayFN("scyllamove2.wav","1",-1,"","");
				for(i=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					p = xGetInt(dPlayerCharacters, xPlayerOwner);
					if (xGetInt(dPlayerData, xPlayerUnit, p) == xGetInt(dPlayerCharacters, xUnitName)) {
						bossTarget = p;
						break;
					}
				}
				bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				dir = getUnitVector(bossTargetPos, bossPos, 4.0);
				bossPos = vectorSnapToGrid(bossTargetPos + dir);
				dir = getUnitVector(bossPos, bossTargetPos);
				trQuestVarSet("boat", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trUnitChangeProtoUnit("Transport Ship Greek");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Dwarf"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("boat"));
				trUnitChangeProtoUnit("Scylla");
				trQuestVarSet("bossSpell", 22);
				bossNext = trTimeMS() + 1500;
				trUnitSelectClear();
				trUnitSelectByQV("boat");
				trUnitChangeProtoUnit("Meteor Impact Water");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trSetSelectedScale(bossScale,bossScale,bossScale);
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > bossNext) {
					trUnitSetAnimationPath("4,0,0,0,0,0,0");
					trUnitOverrideAnimation(1,0,false,false,-1);
					bossNext = trTimeMS() + 700;
					trQuestVarSet("bossSpell", 23);
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				if (trTimeMS() > bossNext) {
					p = bossTarget;
					if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), bossTargetPos) < 16 && aiPlanGetUserVariableBool(dPlayerUnits, xDirtyBit, xGetInt(dPlayerData, xPlayerIndex, p))) {
						xSetInt(dPlayerUnits, xSilenceStatus, 1, xGetInt(dPlayerData, xPlayerIndex, p));
						xSetBool(dPlayerData, xPlayerSilenced, true, p);
						xSetBool(dPlayerData, xPlayerLaunched, true, p);
						trPlayerKillAllGodPowers(p);
						trUnitSelectClear();
						trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
						xDetachDatabaseBlock(dPlayerUnits,xGetInt(dPlayerData, xPlayerIndex, p));
						trSoundPlayFN("changeunit.wav","1",-1,"","");
						trSoundPlayFN("titanpunch1.wav","1",-1,"","");
						trQuestVarSet("bossSpell", 24);
						bossTimeout = trTimeMS() + 10000;
						bossNext = trTimeMS() + 1000;
						trQuestVarSet("bossDamage", 0);
						trMessageSetText(trStringQuestVarGet("p"+p+"name") + " has been eaten! Poison the boss to make her spit them out!", -1);
						bossScale = 2.3;
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trSetSelectedScale(bossScale,bossScale,bossScale);
						trUnitOverrideAnimation(-1,0,false,true,-1);
					} else {
						bossCooldown(3, 6);
					}
				}
			} else if (trQuestVarGet("bossSpell") == 24) {
				if (trTimeMS() > bossNext) {
					if (xGetInt(dEnemies,xPoisonStatus,bossPointer) > 0) {
						bossTimeout = bossTimeout - 1000;
					} else {
						trSoundPlayFN("colossuseat.wav","1",-1,"","");
						trDamageUnitPercent(-1);
						pos = kbGetBlockPosition(""+bossUnit, true);
						trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("Regeneration SFX");
						if (trQuestVarGet("secondPhase") == 1) {
							trQuestVarSetFromRand("rand", 1, 6, true);
							action = trGetNextUnitScenarioNameNumber();
							trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),180,true);
							trArmySelect(""+ENEMY_PLAYER+",0");
							trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("rand")));
							activateEnemy(action);
						}
					}
					bossNext = bossNext + 1000;
					trQuestVarSet("bossDamage", trQuestVarGet("bossDamage") + 150);
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					if (trTimeMS() > bossTimeout) {
						bossScale = 1.3;
						trSetSelectedScale(bossScale,bossScale,bossScale);
						p = bossTarget;
						if (xRestoreDatabaseBlock(dPlayerUnits, xGetInt(dPlayerData, xPlayerIndex, p)) == false) {
							debugLog("Could not restore playerUnits block");
						} else if (xSetPointer(dPlayerUnits, xGetInt(dPlayerData, xPlayerIndex, p)) == false) {
							debugLog("Could not set pointer for eat target");
						} else {
							xSetInt(dPlayerUnits, xSilenceStatus, 0, xGetInt(dPlayerData, xPlayerIndex, p));
							xSetBool(dPlayerData, xPlayerLaunched, false, p);
							pos = kbGetBlockPosition(""+bossUnit, true);
							x = trGetNextUnitScenarioNameNumber();
							trArmyDispatch(""+bossTarget+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
							trUnitSelectClear();
							trUnitSelect(""+x, true);
							trUnitChangeProtoUnit("Transport Ship Greek");
							xUnitSelect(dPlayerUnits, xUnitName);
							trUnitChangeProtoUnit("Militia");
							xUnitSelect(dPlayerUnits, xUnitName);
							trImmediateUnitGarrison(""+x);
							trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dClass, xClassProto, xGetInt(dPlayerData, xPlayerClass, p))));
							trUnitSelectClear();
							trUnitSelect(""+x, true);
							trUnitChangeProtoUnit("Meteor Impact Water");
							xUnitSelect(dPlayerUnits, xUnitName);
							damagePlayerUnit(trQuestVarGet("bossDamage"));
						}
						equipRelicsAgain(p);
						bossCooldown(3, 6);
					} else {
						bossScale = 1.3 + 0.0001 * (bossTimeout - trTimeMS());
						trSetSelectedScale(bossScale,bossScale,bossScale);
					}
				}
			}
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
				bossCount = 3;
				if (trQuestVarGet("secondPhase") == 1) {
					bossCount = 5;
				}
				trQuestVarSet("bossSpell", 12);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Dwarf"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("bossEscape"));
				trUnitChangeProtoUnit("Scylla");
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Meteor Impact Water");
				trSoundPlayFN("shipmove1.wav","1",-1,"","");
				trModifyProtounit("Wadjet Spit",ENEMY_PLAYER,55,2);//make them water so they don't run off
			} else if (trQuestVarGet("bossSpell") == 12) {
				for (x=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelectByID(dPlayerCharacters, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						break;
					}
				}
				bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
				dir = getUnitVector(bossPos, bossTargetPos);
				dist = 0.5 * distanceBetweenVectors(bossPos, bossTargetPos, false);
				amt = dist * 0.04;
				m = 15.0 / xsPow(dist, 2);
				for(i=0; < 50) {
					angle = 0.0 - 2.0 * m * (amt * i - dist); // the derivative of the line getting us the slope at the point
					cVal = xsSqrt(xsPow(angle, 2) + 1);
					sVal = angle / cVal;
					cVal = 1.0 / cVal;
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossJumpPath,i), true);
					trSetUnitOrientation(xsVectorSet(xsVectorGetX(dir)*cVal,sVal,xsVectorGetZ(dir)*cVal),
						xsVectorSet(0.0-xsVectorGetX(dir)*sVal,cVal,0.0-xsVectorGetZ(dir)*sVal),true);
					trUnitTeleport(xsVectorGetX(bossPos) + xsVectorGetX(dir) * amt * i,
						worldHeight + 15.0 - m * xsPow(amt * i - dist, 2),
						xsVectorGetZ(bossPos) + xsVectorGetZ(dir) * amt * i);
				}
				trQuestVarSet("bossSpell", 13);
				bossNext = trTimeMS() + 1000;
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > bossNext) {
					trSetSelectedScale(bossScale,bossScale,bossScale);
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Meteor Impact Water");
					trSoundPlayFN("geyserhit1.wav","1",-1,"","");
					trQuestVarSet("bossSpell", 14);
					if (trQuestVarGet("secondPhase") == 0) {
						action = 2000;
					} else {
						action = 1000;
					}
					trQuestVarSet("bossPrev", -1);
					trQuestVarSet("bossStep", action / 50);
					bossNext = trTimeMS();
				}
			} else if (trQuestVarGet("bossSpell") == 14) {
				action = trTimeMS() - bossNext;
				action = action / trQuestVarGet("bossStep");
				if (action > trQuestVarGet("bossPrev")) {
					if (action < 50) {
						trQuestVarSet("bossPrev", action);
						trUnitSelectClear();
						trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossJumpPath,action),true);
						trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitChangeProtoUnit("Dwarf");
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trMutateSelected(kbGetProtoUnitID("Relic"));
						trImmediateUnitGarrison(""+aiPlanGetUserVariableInt(ARRAYS,bossJumpPath,action));
						trMutateSelected(kbGetProtoUnitID("Scylla"));
						trUnitSelectClear();
						trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossJumpPath,action),true);
						trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
					} else {
						trSoundPlayFN("meteorsplash.wav","1",-1,"","");
						trCameraShake(0.7, 0.4);
						trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(bossTargetPos),0,xsVectorGetZ(bossTargetPos),0,true);
						dir = vector(6,0,0);
						for(i=8; >0) {
							x = xsVectorGetX(bossTargetPos) + xsVectorGetX(dir);
							z = xsVectorGetZ(bossTargetPos) + xsVectorGetZ(dir);
							trArmyDispatch("0,0","Dwarf",1,x,0,z,0,false);
							dir = rotationMatrix(dir, 0.707107, 0.707107);
						}
						trArmySelect("0,0");
						trUnitChangeProtoUnit("Meteor Impact Water");
						for(i=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), bossTargetPos) < 64.0) {
								damagePlayerUnit(500);
							}
						}
						bossCount = bossCount - 1;
						if (bossCount <= 0) {
							trModifyProtounit("Wadjet Spit", ENEMY_PLAYER, 55, 4);
							trUnitSelectClear();
							trUnitSelect(""+bossUnit, true);
							trUnitChangeProtoUnit("Scylla");
							trUnitSelectClear();
							trUnitSelect(""+bossUnit, true);
							trSetSelectedScale(bossScale,bossScale,bossScale);
							bossCooldown(10, 15);
						} else {
							trQuestVarSet("bossSpell", 12);
							bossPos = bossTargetPos;
							trUnitSelectClear();
							trUnitSelect(""+bossUnit, true);
							trUnitChangeProtoUnit("Scylla");
							trUnitSelectClear();
							trUnitSelect(""+bossUnit, true);
							trSetSelectedScale(0,0,0);
						}
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				xsSetContextPlayer(ENEMY_PLAYER);
				id = kbUnitGetTargetUnitID(bossID);
				xsSetContextPlayer(0);
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
					bossPos = vectorSnapToGrid(kbGetBlockPosition(""+bossUnit, true));
					pos = kbGetBlockPosition(""+trGetUnitScenarioNameNumber(id));
					bossDir = getUnitVector(pos, bossPos);
					trVectorQuestVarSet("bossSplashDir", bossDir);
					trVectorQuestVarSet("bossStartDir", vector(0,0,0) - bossDir);
					bossAngle = angleBetweenVectors(pos, bossPos);
					trQuestVarSet("bossWarnStart", trGetNextUnitScenarioNameNumber());
					dir = vector(0,0,-12);
					trQuestVarSet("heading", 360);
					for(i=8; >0) {
						x = xsVectorGetX(bossPos) + xsVectorGetX(dir);
						z = xsVectorGetZ(bossPos) + xsVectorGetZ(dir);
						trArmyDispatch("0,0","Dwarf",1,x,0,z,trQuestVarGet("heading"),true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("UI Range Indicator Greek SFX");
						dir = rotationMatrix(dir, 0.707107, 0.707107);
						trQuestVarSet("heading", trQuestVarGet("heading") - 45);
					}
					trQuestVarSet("bossWarnEnd", trGetNextUnitScenarioNameNumber());
					
					trQuestVarSet("bossSpell", 2);
					bossNext = trTimeMS() + 1000;
					action = 1500;
					if (trQuestVarGet("secondPhase") == 1) {
						action = 500;
					}
					bossTimeout = bossNext + action;
					trQuestVarSet("bossAngularVelocity", 6.283185 / action);
				}
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					for(i=trQuestVarGet("bossWarnStart"); < trQuestVarGet("bossWarnEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+i, true);
						trUnitDestroy();
					}
					trQuestVarSet("bossLast", bossNext);
					trQuestVarSet("bossSpell", 3);
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Tremor");
					trSoundPlayFN("shockwave.wav","1",-1,"","");
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				bossAngle = bossAngle + amt * trQuestVarGet("bossAngularVelocity");
				prev = vectorSetFromAngle(bossAngle);
				angle = dotProduct(prev, bossDir);
				for(i=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (xGetBool(dPlayerUnits, xLaunched) == false) {
						pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
						if (distanceBetweenVectors(pos, bossPos) < 144.0) {
							dir = getUnitVector(bossPos, pos);
							if (dotProduct(dir, prev) > angle) {
								if (dotProduct(dir, bossDir) > angle) {
									damagePlayerUnit(500);
									if (trUnitAlive()) {
										launchUnit(dPlayerUnits, bossPos + (dir * 50.0));
									}
								}
							}
						}
					}
				}
				dir = trVectorQuestVarGet("bossSplashDir");
				if (dotProduct(dir, bossDir) > dotProduct(dir, prev)) {
					pos = bossPos + (dir * 12.0);
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Meteor Impact Water");
					trVectorQuestVarSet("bossSplashDir", rotationMatrix(dir, 0.866025, -0.5));
				}
				bossDir = prev;
				dir = vector(0,0,0) - bossDir;
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				if (trTimeMS() > bossTimeout) {
					trSetUnitOrientation(trVectorQuestVarGet("bossStartDir"),vector(0,1,0),true);
					if (trQuestVarGet("secondPhase") == 1) {
						trQuestVarSet("bossSpell", 4);
						bossTimeout = trTimeMS() + 500;
						bossAngle = 0;
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
					trSetUnitOrientation(xsVectorSet(xsVectorGetX(dir)*0.866025,-0.5,xsVectorGetZ(dir)*0.866025),
						xsVectorSet(xsVectorGetX(dir)*0.5,0.866025,xsVectorGetZ(dir)*0.5), true);
				}
			} else {
				amt = trTimeMS() - trQuestVarGet("bossLast");
				trQuestVarSet("bossLast", trTimeMS());
				if (trQuestVarGet("bossSpell") == 4) {
					trQuestVarSet("bossScale", bossScale + 0.001 * amt);
				} else {
					trQuestVarSet("bossScale", bossScale - 0.001 * amt);
				}
				bossAngle = bossAngle + amt * trQuestVarGet("bossAngularVelocity");
				sVal = xsSin(bossAngle);
				cVal = xsCos(bossAngle);
				
				dir = trVectorQuestVarGet("bossStartDir");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trSetSelectedScale(1.3,1.3,bossScale);
				trSetUnitOrientation(xsVectorSet(xsVectorGetX(dir)*cVal,0.0-sVal,xsVectorGetZ(dir)*cVal),
					xsVectorSet(xsVectorGetX(dir)*sVal,cVal,xsVectorGetZ(dir)*sVal), true);
				if (trTimeMS() > bossTimeout) {
					trQuestVarSet("bossSpell", 1 + trQuestVarGet("bossSpell"));
					if (trQuestVarGet("bossSpell") == 5) {
						trSoundPlayFN("shockwave.wav","1",-1,"","");
						trSoundPlayFN("meteorsplash.wav","1",-1,"","");
						trQuestVarSet("bossDistance", 0);
						bossNext = trTimeMS() - 1;
						bossTimeout = bossTimeout + 500;
					} else if (trQuestVarGet("bossSpell") == 6) {
						trQuestVarSet("bossScale", 1.3);
						trSetSelectedScale(bossScale,bossScale,bossScale);
						trSetUnitOrientation(trVectorQuestVarGet("bossStartDir"),vector(0,1,0),true);
						trUnitSetStance("Aggressive");
						bossCooldown(10, 15);
					}
				}
				if (trQuestVarGet("bossSpell") == 5) {
					if (trTimeMS() > bossNext) {
						bossNext = bossNext + 100;
						trQuestVarSet("bossDistance", 6 + trQuestVarGet("bossDistance"));
						pos = bossPos + (dir * trQuestVarGet("bossDistance"));
						trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("Meteor Impact Water");
						for(x=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 16.0) {
								damagePlayerUnit(500);
								if (trUnitAlive()) {
									stunUnit(dPlayerUnits, 2.0);
								}
							}
						}
					}
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
			debugLog("bossSpell: " + 1*trQuestVarGet("bossSpell"));
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
		xsDisableSelf();
		trMusicStop();
		xsDisableRule("boss_music");
		if (trQuestVarGet("secondPhase") == 1) {
			xsEnableRule("boss_ded");
			xsDisableRule("gameplay_always");
		} else {
			trQuestVarSet("secondPhase", 1);
			trQuestVarSet("cinStep", 0);
			trQuestVarSet("cinTime", trTime() + 3);
			xsEnableRule("boss7_start_again");
			trForceNonCinematicModels(true);
			trUIFadeToColor(0,0,0,1000,0,true);
			trLetterBox(true);
		}
		boss = 0;
		trSetLighting("Fimbulwinter", 1.0);
		trSoundPlayFN("win.wav","1",-1,"","");
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+bossUnit);
	}
}


rule boss7_start_again
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trSoundPlayFN("","1",-1,"Mother of the Depths:Don't you dare think this is over!");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 2:
			{
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"Mother of the Depths:Children of the depths, heed my call!");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 3:
			{
				xsEnableRule("boss7_battle");
				bossUnit = trGetNextUnitScenarioNameNumber();
				vector pos = trVectorQuestVarGet("bossRoomCenter");
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,
					xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Scylla");
				boss = trQuestVarGet("stage");
				activateEnemy(bossUnit);
				bossID = kbGetBlockID(""+bossUnit, true);
				bossPointer = xGetNewestPointer(dEnemies);
				xSetBool(dEnemies, xLaunched, true);
				bossCooldown(10, 15);
				xsEnableRule("boss_music");
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				trQuestVarSet("musicTime", 0);
				
				int next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Hydra",1,
					xsVectorGetX(pos) + 6.0,0,xsVectorGetZ(pos) - 6.0,225,true);
				activateEnemy(next);
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Hydra",1,
					xsVectorGetX(pos) - 6.0,0,xsVectorGetZ(pos) + 6.0,225,true);
				activateEnemy(next);
			}
		}
	}
}

int dDragonMeteors = 0;
int xDragonMeteorSFX = 0;
int xDragonMeteorTimeout = 0;
int xDragonMeteorPos = 0;

int dFallingFireballs = 0;

int dDragonFireballs = 0;
int xDragonFireballRemove = 0;

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
				
				dDragonMeteors = xInitDatabase("dragonMeteors");
				xInitAddInt(dDragonMeteors, "name");
				xDragonMeteorSFX = xInitAddInt(dDragonMeteors, "sfx");
				xDragonMeteorTimeout = xInitAddInt(dDragonMeteors, "timeout");
				xDragonMeteorPos = xInitAddVector(dDragonMeteors, "pos");
				
				dFallingFireballs = initGenericProj("fallingFireballs",kbGetProtoUnitID("Fire Giant"),19,10.0,0,0,ENEMY_PLAYER);
				
				dDragonFireballs = initGenericProj("dragonFireballs",kbGetProtoUnitID("Fire Giant"),19,6,4.5,0,ENEMY_PLAYER,true);
				xDragonFireballRemove = xInitAddBool(dDragonFireballs, "remove");
			}
			case 1:
			{
				trModifyProtounit("Meteor", 0, 8, -12);
				trPaintTerrain(0,0,5,5,0,70,true);
				trPaintTerrain(0,0,5,5,4,15,false);
				bossDir = vector(0,0,-1);
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
				trUnitSetStance("Passive");
				trSetSelectedScale(bossScale, bossScale, bossScale);
				xsEnableRule("boss8_battle");
				trQuestVarSet("bossGem", STARSTONE);
				trQuestVarSetFromRand("bossGemCount", 4, 5, true);
				xsEnableRule("boss_music");
				
				vector pos = trVectorQuestVarGet("bossRoomCenter");
				
				xSetPointer(dEnemies, bossPointer);
				xFreeDatabaseBlock(dEnemies);

				bossDir = vector(1,0,0);

				trQuestVarSet("bossGround", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("0,0");
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				
				for(i=1; <= 4) {
					trQuestVarSet("bossBreath"+i, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("0,0");
					trSetSelectedScale(0,0,0);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
				}

				trPlayerSetDiplomacy(ENEMY_PLAYER, 0, "neutral");
				
				trStringQuestVarSet("advice",
					"You need to lure the dragon over the spotlights to damage him.");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}


void dragonMeteor(vector pos = vector(0,0,0)) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	trArmySelect("0,0");
	trUnitChangeProtoUnit("Relic");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trMutateSelected(kbGetProtoUnitID("Hero Birth"));
	xAddDatabaseBlock(dDragonMeteors, true);
	xSetInt(dDragonMeteors, xUnitName, next);
	xSetInt(dDragonMeteors, xDragonMeteorSFX, trGetNextUnitScenarioNameNumber());
	xSetInt(dDragonMeteors, xDragonMeteorTimeout, trTimeMS() + 2000);
	xSetVector(dDragonMeteors, xDragonMeteorPos, pos);
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	trArmySelect("0,0");
	trUnitChangeProtoUnit("Gaia Forest effect");
}

rule boss8_battle
inactive
highFrequency
{
	int p = 0;
	int x = 0;
	int action = 0;
	int id = 0;
	float current = 0;
	float dist = 0;
	float amt = 0;
	bool hit = false;
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	if (trUnitAlive()) {
		switch(1*trQuestVarGet("dragonSpotlightStep"))
		{
			case 0:
			{
				trQuestVarSet("dragonSpotlightStep", 1);
				trQuestVarSet("dragonSpotlightNext", trTime() + 10);
			}
			case 1:
			{
				if (trTime() > trQuestVarGet("dragonSpotlightNext")) {
					pos = vectorSnapToGrid(trVectorQuestVarGet("bossRoomCenter") + (bossDir * 8.0));
					trQuestVarSet("dragonSpotlight", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					if (trQuestVarGet("dragonSpotlight") < trGetNextUnitScenarioNameNumber()) {
						trQuestVarSet("dragonSpotlightStep", 2);
						trUnitSelectClear();
						trUnitSelectByQV("dragonSpotlight");
						trUnitChangeProtoUnit("Spy Eye");
						trUnitSelectClear();
						trUnitSelectByQV("dragonSpotlight");
						trMutateSelected(kbGetProtoUnitID("Outpost"));
						trSetSelectedScale(0,0,0);
						trUnitSetAnimationPath("1,0,0,0,0,0,0");
						trMessageSetText("A spotlight has appeared! Lure the boss over it to damage him!", -1);
						trVectorQuestVarSet("dragonSpotlightPos", pos);
						trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trArmySelect("0,0");
						trUnitChangeProtoUnit("Vision SFX");
					}
				}
			}
			case 2:
			{
				pos = kbGetBlockPosition(""+bossUnit);
				if (distanceBetweenVectors(trVectorQuestVarGet("dragonSpotlightPos"), pos) < 9.0) {
					trDamageUnit(10000);
					trUnitSelectClear();
					trUnitSelectByQV("dragonSpotlight");
					trUnitChangeProtoUnit("Dwarf");
					trUnitSelectClear();
					trUnitSelectByQV("dragonSpotlight");
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetUnitOrientation(vector(0,-1,0), vector(1,0,0), true);
					trUnitHighlight(2.0, false);
					xAddDatabaseBlock(dPlayerLasers, true);
					xSetInt(dPlayerLasers, xUnitName, trQuestVarGet("dragonSpotlight"));
					xSetInt(dPlayerLasers, xPlayerLaserTimeout, trTimeMS() + 1000);
					xSetFloat(dPlayerLasers, xPlayerLaserRange, 28);
					trSoundPlayFN("petsuchosattack.wav", "1", -1, "", "");
					trQuestVarSet("dragonSpotlightStep", 0);
				}
			}
		}

		xsSetContextPlayer(0);
		id = kbUnitGetTargetUnitID(bossID);

		if (kbUnitGetOwner(id) == ENEMY_PLAYER) {
			xDatabaseNext(dPlayerUnits);
			trUnitSelectClear();
			trUnitSelect(""+bossUnit);
			trUnitDoWorkOnUnit(""+xGetInt(dPlayerUnits, xUnitName));
		}

		if (xGetDatabaseCount(dFallingFireballs) > 0) {
			for(i=xsMin(3, xGetDatabaseCount(dFallingFireballs)); >0) {
				if (PROJ_GROUND == processGenericProj(dFallingFireballs)) {
					trUnitChangeProtoUnit("Dwarf");
					pos = kbGetBlockPosition(""+xGetInt(dFallingFireballs, xUnitName), true);
					xUnitSelectByID(dFallingFireballs, xUnitID);
					trDamageUnitPercent(-100);
					trUnitChangeProtoUnit("Meteorite");
					xUnitSelectByID(dFallingFireballs, xUnitID);
					trDamageUnitPercent(100);
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("fireball fall " + 1*trQuestVarGet("sound") + ".wav","1",-1,"","");
					for (j=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName),pos) < 9) {
							damagePlayerUnit(200);
						}
					}
					xFreeDatabaseBlock(dFallingFireballs);
				}
			}
			
		}
		
		if (xGetDatabaseCount(dDragonMeteors) > 0) {
			hit = false;
			for(i=xsMin(3, xGetDatabaseCount(dDragonMeteors)); >0) {
				xDatabaseNext(dDragonMeteors);
				if (trTimeMS() > xGetInt(dDragonMeteors, xDragonMeteorTimeout)) {
					hit = true;
					xUnitSelect(dDragonMeteors, xDragonMeteorSFX);
					trDamageUnitPercent(100);
					trUnitChangeProtoUnit("Meteorite");
					xUnitSelect(dDragonMeteors, xUnitName);
					trUnitChangeProtoUnit("Meteor");
					pos = xGetVector(dDragonMeteors, xDragonMeteorPos);
					for(j=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 16.0) {
							damagePlayerUnit(2000.0);
						}
					}
					xFreeDatabaseBlock(dDragonMeteors);
				}
			}
			if (hit) {
				trSoundPlayFN("meteorsmallhit.wav","1",-1,"","");
				trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
			}
		}
		
		if (xGetDatabaseCount(dDragonFireballs) > 0) {
			for(i=1 + xGetDatabaseCount(dDragonFireballs) / 3; >0) {
				action = processGenericProj(dDragonFireballs);
				if (action == PROJ_FALLING) {
					pos = kbGetBlockPosition(""+xGetInt(dDragonFireballs, xUnitName), true);
					prev = xGetVector(dDragonFireballs, xProjPrev);
					dist = distanceBetweenVectors(pos, prev);
					hit = false;
					if (dist > 4.0) {
						dist = xsSqrt(dist);
						dir = xGetVector(dDragonFireballs, xProjDir);
						for(j=xGetDatabaseCount(dPlayerUnits); >0) {
							xDatabaseNext(dPlayerUnits);
							xUnitSelectByID(dPlayerUnits, xUnitID);
							if (trUnitAlive() == false) {
								removePlayerUnit();
							} else if (rayCollision(dPlayerUnits,prev,dir, dist + 1.0, 2.0)) {
								damagePlayerUnit(300);
								hit = true;
							}
						}
						if (hit) {
							trQuestVarSetFromRand("sound", 1, 2, true);
							trSoundPlayFN("fireball fall " + 1*trQuestVarGet("sound") + ".wav","1",-1,"","");
							xUnitSelect(dDragonFireballs, xUnitName);
							trUnitChangeProtoUnit("Meteorite");
							xFreeDatabaseBlock(dDragonFireballs);
						}
						xSetVector(dDragonFireballs, xProjPrev, pos);
					}
				} else if (action == PROJ_GROUND) {
					pos = vectorToGrid(kbGetBlockPosition(""+xGetInt(dDragonFireballs, xUnitName), true));
					if (trGetTerrainHeight(xsVectorGetX(pos), xsVectorGetZ(pos)) < worldHeight - 1.0) {
						xSetBool(dDragonFireballs,xDragonFireballRemove,true);
					}
				} else if (action == PROJ_BOUNCE) {
					if (xGetBool(dDragonFireballs,xDragonFireballRemove)) {
						xFreeDatabaseBlock(dDragonFireballs);
					}
				}
			}
		}
		
		for (i=xGetDatabaseCount(dCloudDeployStars); >0) {
			if (PROJ_GROUND == processGenericProj(dCloudDeployStars)) {
				trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+1*trQuestVarGet("cloudDeployProto")));
				xUnitSelectByID(dCloudDeployStars, xUnitID);
				trDamageUnitPercent(-100);
				activateEnemy(xGetInt(dCloudDeployStars, xUnitName), -1, 0);
				xFreeDatabaseBlock(dCloudDeployStars);
				trQuestVarSetFromRand("sound", 1, 2, true);
				trSoundPlayFN("vortexland"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
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
				if (trUnitPercentDamaged() < 10) {
					bossCooldown(10, 15);
				} else {
					trQuestVarSetFromRand("rand", 1 + trUnitPercentDamaged() / 20, 1 + trUnitPercentDamaged() / 10, true);
					bossCount = trQuestVarGet("rand");
					trQuestVarSetFromRand("cloudDeployProto", 1, 6, true);
					if ((bossCount + xGetDatabaseCount(dEnemies) > ENEMY_PLAYER) && (trQuestVarGet("secondPhase") == 0)) {
						bossCount = ENEMY_PLAYER - xGetDatabaseCount(dEnemies);
					}
					trQuestVarSet("bossSpell", 42);
					bossNext = trTimeMS();
					if (bossCount > 0) {
						trQuestVarSet("bCos", xsCos(6.283185 / bossCount));
						trQuestVarSet("bSin", xsSin(6.283185 / bossCount));
					}
				}
			} else if (trQuestVarGet("bossSpell") == 42) {
				if (bossCount <= 0) {
					bossCooldown(5, 10);
				} else if (trTimeMS() > bossNext) {
					bossDir = rotationMatrix(bossDir,trQuestVarGet("bCos"),trQuestVarGet("bSin"));
					trQuestVarSetFromRand("sound", 1, 3, true);
					trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					bossNext = bossNext + 200;
					pos = trVectorQuestVarGet("bossRoomCenter") - (bossDir * 16.0);
					addGenericProj(dCloudDeployStars,pos,bossDir);
					bossCount = bossCount - 1;
				}
			}
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trQuestVarSet("bossUsedUltimate", 1);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Heavenfall",3.0,-1,-1,-1);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				trArmyDispatch("0,0","Dwarf",2,xsVectorGetX(bossPos),0,xsVectorGetZ(bossPos),0,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Vision SFX");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitChangeProtoUnit("Cinematic Block");
				trQuestVarSet("bossSpell", 32);
				trQuestVarSet("bossBarrageCount", 1);
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trQuestVarGet("bossBarrageCount") >= 4) {
					trQuestVarSet("bossSpell", 36);
					trQuestVarSet("bossDist", 5.0);
					bossNext = trTimeMS() + 3000;
				} else {
					bossCount = 3;
					trQuestVarSet("bossSpell", 33);
					bossNext = trTimeMS() + 1500;
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				if (trTimeMS() > bossNext) {
					for(p=1; < ENEMY_PLAYER) {
						if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
							dragonMeteor(vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p))));
						}
					}
					bossCount = bossCount - 1;
					if (bossCount == 0) {
						trQuestVarSet("bossSpell", 34);
						bossNext = trTimeMS() + 2000;
						bossCount = trQuestVarGet("bossBarrageCount");
						trQuestVarSet("bossBarrageCount", 1 + trQuestVarGet("bossBarrageCount"));
					} else {
						bossNext = bossNext + 1000;
					}
				}
			} else if (trQuestVarGet("bossSpell") == 34) {
				if (trTimeMS() > bossNext) {
					trQuestVarSet("bossSpell", 35);
					bossDir = rotationMatrix(bossDir, -0.757323, 0.653041);
					bossPos = trVectorQuestVarGet("bossRoomCenter") + (bossDir * 20.0);
					for(i=xGetDatabaseCount(dPlayerCharacters); >0) {
						xDatabaseNext(dPlayerCharacters);
						xUnitSelectByID(dPlayerCharacters, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerCharacter();
						} else {
							bossTargetPos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true);
							break;
						}
					}
					bossDir = getUnitVector(bossPos, bossTargetPos);
				}
			} else if (trQuestVarGet("bossSpell") == 35) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 200;
					trQuestVarSetFromRand("modx", -3, 3, false);
					trQuestVarSetFromRand("modz", -3, 3, false);
					dragonMeteor(vectorSnapToGrid(xsVectorSet(xsVectorGetX(bossPos)+trQuestVarGet("modx"),0,
								xsVectorGetZ(bossPos)+trQuestVarGet("modx"))));
					bossPos = bossPos + (bossDir * 4.0);
					if (distanceBetweenVectors(bossPos,trVectorQuestVarGet("bossRoomCenter")) > 441.0) {
						bossCount = bossCount - 1;
						if (bossCount > 0) {
							bossNext = trTimeMS() + 2000;
							trQuestVarSet("bossSpell", 34);
						} else {
							trQuestVarSet("bossSpell", 32);
						}
					}
				}
			} else if (trQuestVarGet("bossSpell") == 36) {
				if (trTimeMS() > bossNext) {
					bossNext = trTimeMS() + 100;
					pos = vectorSnapToGrid(trVectorQuestVarGet("bossRoomCenter") + (bossDir * trQuestVarGet("bossDist")));
					dragonMeteor(pos);
					trQuestVarSet("bossDist", trQuestVarGet("bossDist") + 1.0);
					if (trQuestVarGet("bossDist") >= 20) {
						trQuestVarSet("bossSpell", 41);
						trQuestVarSet("bossUltimate", 3);
						trSetLighting("default", 1.0);
						trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitChangeProtoUnit("Nidhogg");
					} else {
						bossDir = rotationMatrix(bossDir, 0.71934, 0.694658);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				if (trQuestVarGet("secondPhase") == 1) {
					trQuestVarSetFromRand("rand", 1, 5, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Burn to ash!");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Drill Beam");
					} else if (trQuestVarGet("rand") == 3) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Become dust!");
					}
				}
				trUnitSetStance("Passive");
				trMutateSelected(kbGetProtoUnitID("Roc"));
				trSoundPlayFN("nidhoggselect1.wav","1",-1,"","");

				if (trQuestVarGet("secondPhase") == 0) {
					for(i=1; <= 3) {
						trUnitSelectClear();
						trUnitSelectByQV("bossBreath"+i);
						trMutateSelected(kbGetProtoUnitID("Militia"));
						trImmediateUnitGarrison(""+bossUnit);
						trUnitChangeProtoUnit("Cinematic Block");
					}
				} else {
					xDatabaseNext(dPlayerUnits);
					pos = xGetVector(dPlayerUnits, xUnitPos);
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("0,0");
					trUnitChangeProtoUnit("Transport Ship Greek");
					for(i=1; <= 3) {
						trUnitSelectClear();
						trUnitSelectByQV("bossBreath"+i);
						trMutateSelected(kbGetProtoUnitID("Militia"));
						trImmediateUnitGarrison(""+action);
						trUnitChangeProtoUnit("Cinematic Block");
					}
					trUnitSelectClear();
					trUnitSelect(""+action, true);
					trUnitDestroy();

					bossPos = kbGetBlockPosition(""+bossUnit);
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trSetUnitOrientation(getUnitVector(bossPos, pos), vector(0,1,0), true);
				}

				trQuestVarSet("bossSpell", 22);
				bossNext = trTimeMS() + 1000;


				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trMutateSelected(kbGetProtoUnitID("Nidhogg"));
				trUnitOverrideAnimation(1,0,false,false,-1);
			} else if (trQuestVarGet("bossSpell") == 22) {
				if (trTimeMS() > bossNext) {
					trUnitOverrideAnimation(1,-1,false,false,-1);
					trSoundPlayFN("nidhoggflame2.wav","1",-1,"","");
					trSoundPlayFN("sonofosirisbolt.wav","1",-1,"","");
					bossTimeout = trTimeMS() + 8000 + 4000 * trQuestVarGet("secondPhase");
					trQuestVarSet("bossSpell", 23);
					trQuestVarSet("bossAlternate", 1);
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath3");
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trUnitOverrideAnimation(6,0,true,false,-1);
					trQuestVarSet("bossDamageNext", trTimeMS());
					bossTargetPos = kbGetBlockPosition(""+1*trQuestVarGet("bossBreath1"));
				}
			} else if (trQuestVarGet("bossSpell") == 23) {
				trQuestVarSet("bossAlternate", 1 - trQuestVarGet("bossAlternate"));
				trUnitSelectClear();
				trUnitSelectByQV("bossBreath1");
				trMutateSelected(kbGetProtoUnitID("Militia"));
				trUnitSelectClear();
				trUnitSelectByQV("bossBreath2");
				trUnitChangeProtoUnit("Transport Ship Greek");
				if (trQuestVarGet("bossAlternate") == 1) {
					trUnitSelectClear();
					trUnitSelect(""+bossUnit);
					trMutateSelected(kbGetProtoUnitID("Roc"));
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath1");
					trImmediateUnitGarrison(""+bossUnit);
					trUnitChangeProtoUnit("Ball of Fire");
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath1");
					trUnitChangeProtoUnit("Pegasus");
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath1");
					trUnitChangeProtoUnit("Ball of Fire");
					trUnitSelectClear();
					trUnitSelect(""+bossUnit, true);
					trMutateSelected(kbGetProtoUnitID("Nidhogg"));
					trUnitOverrideAnimation(1,-1,false,false,-1);
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath1");
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBreath2"));
					trUnitChangeProtoUnit("Ball of Fire");
				}
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 100;
					bossDir = rotationMatrix(bossDir, -0.740544, -0.672008);
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("0,0","Dwarf",1,1,0,1,0,true);
					trArmySelect("0,0");
					trImmediateUnitGarrison(""+1*trQuestVarGet("bossBreath2"));
					trUnitChangeProtoUnit("Dwarf");
					trUnitSelectClear();
					trUnitSelect(""+action, true);
					trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
					zSetProtoUnitStat("Kronny Flying", 0, 1, xGetFloat(dDragonFireballs, xProjSpeed, 0));
					trSetUnitOrientation(bossDir, vector(0,1,0), true);
					trSetSelectedScale(0, -4.5, 0);
					trDamageUnitPercent(100);
					xAddDatabaseBlock(dDragonFireballs, true);
					xSetInt(dDragonFireballs, xUnitName, action);
					xSetVector(dDragonFireballs, xProjDir, bossDir);
					xSetInt(dDragonFireballs, xUnitID, kbGetBlockID(""+action, true));
				}
				if (trTimeMS() > trQuestVarGet("bossDamageNext")) {
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), bossTargetPos) < 25.0) {
							damagePlayerUnit(300.0);
						}
					}
					trQuestVarSet("bossDamageNext", trQuestVarGet("bossDamageNext") + 500);
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("nidhoggflame"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					trCameraShake(0.5, 0.3);
				}
				trUnitSelectClear();
				trUnitSelectByQV("bossBreath2");
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				if (trTimeMS() > bossTimeout) {
					for(i=1; <=3) {
						trUnitSelectClear();
						trUnitSelectByQV("bossBreath" + i);
						trUnitChangeProtoUnit("Cinematic Block");
					}
					trUnitSelectClear();
					trUnitSelect(""+bossUnit);
					trUnitOverrideAnimation(-1,0,false,true,-1);
					trUnitSetStance("Aggressive");
					trQuestVarSet("bossSpell", 41);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				if (trQuestVarGet("secondPhase") == 1) {
					trQuestVarSetFromRand("rand", 1, 5, true);
					if (trQuestVarGet("rand") == 1) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Fuck you in particular!");
					} else if (trQuestVarGet("rand") == 2) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Blast Breath");
					} else if (trQuestVarGet("rand") == 3) {
						trChatSendSpoofed(ENEMY_PLAYER, "Lord of the Heavens: Roast human for dinner!");
					}
				}
				trSoundPlayFN("nidhogggrunt1.wav","1",-1,"","");
				trMutateSelected(kbGetProtoUnitID("Nidhogg"));
				trUnitSetStance("Passive");
				trUnitOverrideAnimation(2,0,false,false,-1);
				trUnitOverrideAnimation(2,-1,false,false,-1);
				bossPos = kbGetBlockPosition(""+bossUnit, true);
				for(i=2; > 0) {
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath"+i);
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetSelectedScale(0,0,0);
					trUnitTeleport(xsVectorGetX(bossPos),xsVectorGetY(bossPos),xsVectorGetZ(bossPos));
				}
				for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelectByID(dPlayerCharacters, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerCharacter();
					} else {
						trUnitHighlight(2.5, true);
						bossTarget = xGetInt(dPlayerCharacters, xUnitName);
						break;
					}
				}
				bossTargetPos = xsVectorSetY(kbGetBlockPosition(""+bossUnit, true), worldHeight);
				trQuestVarSet("bossSpell", 12);
				bossNext = trTimeMS() + 1500;
			} else if (trQuestVarGet("bossSpell") == 12) {
				bossDir = getUnitVector(bossPos, kbGetBlockPosition(""+bossTarget, true));
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trSetUnitOrientation(bossDir,vector(0,1,0),true);
				if (trTimeMS() > bossNext) {
					trQuestVarSet("bossLast", trTimeMS());
					bossNext = trTimeMS() + 500;
					bossTimeout = trTimeMS() + 8000;
					trSoundPlayFN("sonofosirisbolt.wav","1",-1,"","");
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath1");
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trUnitOverrideAnimation(6,0,true,false,-1);
					trSetSelectedUpVector(0,-1,0);
					trUnitSelectClear();
					trUnitSelectByQV("bossBreath2");
					trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
					trSetSelectedScale(10.0, 10.0, 30.0);
					trUnitHighlight(30.0, false);
					trSetUnitOrientation(vector(0,1,0),vector(1,0,0),true);
					trQuestVarSet("bossSpell", 13);
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				trUnitSelectClear();
				trUnitSelect(""+bossTarget, true);
				if (trUnitAlive() == false) {
					for(i=xGetDatabaseCount(dPlayerCharacters); >0) {
						xDatabaseNext(dPlayerCharacters);
						xUnitSelectByID(dPlayerCharacters,xUnitID);
						if (trUnitAlive() == false) {
							removePlayerCharacter();
						} else {
							bossTarget = xGetInt(dPlayerCharacters, xUnitName);
							break;
						}
					}
				}
				pos = kbGetBlockPosition(""+bossTarget, true);
				if (distanceBetweenVectors(pos, bossTargetPos) > 0.1) {
					
					amt = trTimeMS() - trQuestVarGet("bossLast");
					trQuestVarSet("bossLast", trTimeMS());
					amt = amt * 0.004;
					if (trQuestVarGet("secondPhase") == 1) {
						amt = amt * 1.5;
					}
					dir = getUnitVector(bossTargetPos, pos, amt);
					bossTargetPos = dir + bossTargetPos;
					if (distanceBetweenVectors(bossPos,bossTargetPos) > 1.0) {
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trSetUnitOrientation(getUnitVector(bossPos, bossTargetPos),vector(0,1,0),true);
						dir = getUnitVector3d(bossPos, bossTargetPos);
						trUnitSelectClear();
						trUnitSelectByQV("bossBreath1");
						trSetSelectedUpVector(5.0 * xsVectorGetX(dir),5.0 * xsVectorGetY(dir) - 1.0,5.0 * xsVectorGetZ(dir));
						trUnitSelectClear();
						trUnitSelectByQV("bossBreath2");
						trSetUnitOrientation(vector(0,0,0) - dir, xsVectorNormalize(xsVectorSet(xsVectorGetZ(dir),0,0.0 - xsVectorGetX(dir))), true);
					}
				} else {
					trQuestVarSet("bossLast", trTimeMS());
				}
				
				if (trTimeMS() > bossNext) {
					for(i=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), bossTargetPos) < 9.0) {
							damagePlayerUnit(250);
						}
					}
					
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("nidhoggflame"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					
					bossNext = bossNext + 500;
					if (trTimeMS() > bossTimeout) {
						trSoundPlayFN("godpowerfailed.wav","1",-1,"","");
						trUnitSelectClear();
						trUnitSelect(""+bossUnit, true);
						trUnitOverrideAnimation(-1,0,false,true,-1);
						trUnitSetStance("Aggressive");
						
						for(i=2; >0) {
							trUnitSelectClear();
							trUnitSelectByQV("bossBreath"+i);
							trSetSelectedUpVector(0,1,0);
							trMutateSelected(kbGetProtoUnitID("Rocket"));
							trUnitOverrideAnimation(2,0,true,false,-1);
						}
						trQuestVarSet("bossSpell", 41);
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trUnitSetStance("Aggressive");
				xDatabaseNext(dPlayerUnits);
				pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName));
				trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,true);
				trQuestVarSet("bossSpell", 2);
				bossTimeout = trTimeMS() + 10000;
				bossNext = trTimeMS();
				if (trQuestVarGet("secondPhase") == 1) {
					trQuestVarSet("bossRange", 15.0);
				}
			} else if (trQuestVarGet("bossSpell") == 2) {
				if (trTimeMS() > bossNext) {
					if (trQuestVarGet("secondPhase") == 1) {
						bossDir = rotationMatrix(bossDir, 0.740544, 0.672008);
						bossNext = trTimeMS() + 200;
						trQuestVarSet("rand", trQuestVarGet("bossRange"));
						trQuestVarSet("bossRange", trQuestVarGet("bossRange") - 0.3);
					} else {
						bossDir = rotationMatrix(bossDir, -0.757323, 0.653041);
						bossNext = trTimeMS() + 500;
						trQuestVarSetFromRand("rand", 4.0, 10.0, false);
					}
					bossPos = kbGetBlockPosition(""+bossUnit, true);
					addGenericProj(dFallingFireballs,bossPos, bossDir,ENEMY_PLAYER,trQuestVarGet("rand"));
					if (trTimeMS() > bossTimeout) {
						trQuestVarSet("bossSpell", 41);
					}
				}
			}
		} else if ((trQuestVarGet("bossUsedUltimate") == 0) && trUnitPercentDamaged() >= 70) {
			trQuestVarSet("bossSpell", 31);
		} else {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		xsDisableRule("boss_music");
		boss = 0;
		if (trQuestVarGet("secondPhase") == 1) {
			xsEnableRule("boss_ded");
			xsDisableRule("gameplay_always");
		} else {
			trQuestVarSet("secondPhase", 1);
			trQuestVarSet("cinStep", 0);
			trQuestVarSet("cinTime", trTime() + 3);
			xsEnableRule("boss8_start_again");
			trForceNonCinematicModels(true);
			trUIFadeToColor(0,0,0,1000,0,true);
			trLetterBox(true);
		}
		for(i=3; >0) {
			trUnitSelectClear();
			trUnitSelectByQV("bossBreath"+i);
			trUnitDestroy();
		}
		trSetLighting("default", 1.0);
		trSoundPlayFN("win.wav","1",-1,"","");
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies, true);
			xUnitSelectByID(dEnemies, xUnitID);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+bossUnit);
	}
}



rule boss8_start_again
inactive
highFrequency
{
	vector pos = vector(0,0,0);
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trSoundPlayFN("","1",-1,"Lord of the Heavens: Most impressive. Seems you are worthy of my attention.");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 2:
			{
				bossUnit = trGetNextUnitScenarioNameNumber();
				pos = trVectorQuestVarGet("bossRoomCenter");
				trArmyDispatch("0,0","Nidhogg",1,
					xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"Lord of the Heavens:Now, allow me to demonstrate the extent of my power!");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 3:
			{
				trModifyProtounit("Nidhogg", 0, 0, 100000);
				xsEnableRule("boss8_battle");
				boss = trQuestVarGet("stage");
				bossID = kbGetBlockID(""+bossUnit, true);
				bossCooldown(2, 2);
				xsEnableRule("boss_music");
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				trQuestVarSet("musicTime", 0);
				trModifyProtounit("Nidhogg", 0, 1, 1.5);
				trModifyProtounit("Nidhogg", 0, 27, 100);

				pos = trVectorQuestVarGet("bossRoomCenter");
				for(i=1; <= 4) {
					trQuestVarSet("bossBreath"+i, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("0,0");
					trSetSelectedScale(0,0,0);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
				}
			}
		}
	}
}

int dLavaTileDB = 0;
int xLavaTilePos = 0;
int lavaTiles = 0;
int lavaTileStartX = 0;
int lavaTileStartZ = 0;

int dHekaProj = 0;

int lavaDamageNext = 0;
int lavaDamagePointer = 0;

rule boss9_init
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trOverlayText("Hellkeeper", 3.0, -1, -1, -1);
				trQuestVarSet("cinTime", trTime() + 2);
				xsEnableRule("the_pit_build_01");
				dHekaProj = initGenericProj("hekaProj",kbGetProtoUnitID("Meteorite death"),-1,15.0,4.5,0,0,true);

				dLionMeteors = xInitDatabase("lionMeteors");
				xInitAddInt(dLionMeteors, "name");
				xLionMeteorYeehaw = xInitAddInt(dLionMeteors, "yeehaw", 1);

				dLavaTileDB = xInitDatabase("lavaTileDB");
				xLavaTilePos = xInitAddVector(dLavaTileDB, "pos");

				vector center = trVectorQuestVarGet("bossRoomCenter");
				lavaTileStartX = xsVectorGetX(center) / 2 - trQuestVarGet("bossRoomSize") - 1;
				lavaTileStartZ = xsVectorGetZ(center) / 2 - trQuestVarGet("bossRoomSize") - 1;
				lavaTiles = aiPlanCreate("lavaTiles", 8);
				if (lavaTiles < 0) {
					debugLog("Unable to allocate lavaTiles plan");
				}
				for (x=trQuestVarGet("bossRoomSize") * 2 + 2; >=0) {
					aiPlanAddUserVariableInt(lavaTiles,x,"row"+x,2*trQuestVarGet("bossRoomSize"));
				}
			}
			case 1:
			{
				trPaintTerrain(0,0,5,5,5,7,false);
				bossDir = vector(0,0,-1);

				xSetBool(dEnemies, xLaunched, true, bossPointer);

				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitConvert(ENEMY_PLAYER);
				trSetSelectedScale(bossScale, bossScale, bossScale);
				xsEnableRule("boss9_battle");
				trQuestVarSet("bossGem", MANASTONE);
				trQuestVarSetFromRand("bossGemCount", 4, 5, true);
				xsEnableRule("boss_music");

				spyEffect(bossUnit,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS,bossInts,1));

				bossCooldown(8, 12);

				lavaDamageNext = trTimeMS();

				for(i=xStunSFX; <= xSilenceSFX) {
					spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"), xsVectorSet(dEnemies, i, bossPointer));
				}
				xsEnableRule("boss9_ready");

				trStringQuestVarSet("advice","You can do this! I believe in you!");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

void paintLava(vector pos = vector(0,0,0)) {
	vector grid = vectorToGrid(pos);
	if (trGetTerrainHeight(xsVectorGetX(grid),xsVectorGetZ(grid)) > worldHeight - 1.0) {
		trPaintTerrain(xsVectorGetX(grid),xsVectorGetZ(grid),xsVectorGetX(grid),xsVectorGetZ(grid),5,7,false);
		aiPlanSetUserVariableInt(lavaTiles,xsVectorGetX(grid) - lavaTileStartX, xsVectorGetZ(grid) - lavaTileStartZ, trTimeMS() + 20000);
		xAddDatabaseBlock(dLavaTileDB, true);
		xSetVector(dLavaTileDB, xLavaTilePos, grid);
	}
}

rule boss9_battle
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float angle = 0;
	float dist = 0;
	bool hit = false;
	
	vector hitbox = vector(0,0,0);
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (trUnitAlive() == true) {
		if (trQuestVarGet("secondPhase") == 1) {
			if ((trTime() > trQuestVarGet("pitDeployNext")) || (trQuestVarGet("noGates") == 1)) {
				// tartarian gates
				dir = trVectorQuestVarGet("gateSpawnDir");
				pos = trVectorQuestVarGet("bossRoomCenter") - (dir * 15.0);
				trPlayerKillAllGodPowers(ENEMY_PLAYER);
				trTechGodPower(ENEMY_PLAYER, "tartarian gate", 1);
				trUnitSelectClear();
				trTechInvokeGodPower(ENEMY_PLAYER, "tartarian gate", pos, vector(0,0,0));

				trQuestVarSet("noGates", 1);
				trVectorQuestVarSet("gateSpawnDir", rotationMatrix(dir, -0.757322, 0.653041));
				trQuestVarSet("pitDeployNext", trTime() + 30);
			} else if (trUnitPercentDamaged() > trQuestVarGet("pitDeployDamage")) {
				trQuestVarSet("pitDeployNext", trQuestVarGet("pitDeployNext") - trUnitPercentDamaged() + trQuestVarGet("pitDeployDamage"));
				trQuestVarSet("pitDeployDamage", trUnitPercentDamaged());
			}
		}

		// lava tiles vanish
		for (x=xsMin(5, xGetDatabaseCount(dLavaTileDB)); > 0) {
			xDatabaseNext(dLavaTileDB);
			pos = xGetVector(dLavaTileDB, xLavaTilePos);
			if (trTimeMS() > aiPlanGetUserVariableInt(lavaTiles,xsVectorGetX(pos) - lavaTileStartX, xsVectorGetZ(pos) - lavaTileStartZ)) {
				trPaintTerrain(xsVectorGetX(pos),xsVectorGetZ(pos),xsVectorGetX(pos),xsVectorGetZ(pos),TERRAIN_PRIMARY,TERRAIN_SUB_PRIMARY,false);
				xFreeDatabaseBlock(dLavaTileDB);
			}
		}

		for(i=xsMin(3, xGetDatabaseCount(dLionMeteors)); >0) {
			xDatabaseNext(dLionMeteors);
			if (xGetInt(dLionMeteors, xLionMeteorYeehaw) == 1) {
				xSetInt(dLionMeteors, xLionMeteorYeehaw, 0);
				xUnitSelect(dLionMeteors, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Fire Giant"));
				trUnitOverrideAnimation(19,0,true,false,-1);
				trSetSelectedScale(0,0,0);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(dLionMeteors, xUnitName), true);
				if (xsVectorGetY(pos) <= worldHeight + 0.5) {
					if (trQuestVarGet("secondPhase") == 1) {
						paintLava(pos);
					}
					trQuestVarSetFromRand("heading", 0, 360, true);
					action = trGetNextUnitScenarioNameNumber();
					trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					trQuestVarSetFromRand("rand", 1, 5, true);
					if (trQuestVarGet("rand") + trQuestVarGet("secondPhase") >= 5) {
						trUnitChangeProtoUnit("Fire Giant");
						activateEnemy(action);
					} else {
						trUnitHighlight(3.0, true);
						trUnitChangeProtoUnit("Phoenix Egg");
						xAddDatabaseBlock(dTartarianEggs, true);
						xSetInt(dTartarianEggs, xUnitName, action);
						xSetInt(dTartarianEggs, xPlayerOwner, ENEMY_PLAYER);
						xSetInt(dTartarianEggs, xTartarianEggTimeout, trTimeMS() + 3000);
					}
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("fireball fall " + 1*trQuestVarGet("sound") + ".wav", "1", -1, "", "");
					xUnitSelect(dLionMeteors, xUnitName);
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					xUnitSelect(dLionMeteors, xUnitName);
					trDamageUnitPercent(-100);
					trSetSelectedScale(0,0,0);
					xFreeDatabaseBlock(dLionMeteors);
				}
			}
		}

		if (trTimeMS() > lavaDamageNext) {
			if (xGetDatabaseCount(dPlayerUnits) > 0) {
				lavaDamageNext = lavaDamageNext + 500 / xGetDatabaseCount(dPlayerUnits);
				action = xGetPointer(dPlayerUnits);
				xSetPointer(dPlayerUnits, lavaDamagePointer);
				lavaDamagePointer = xDatabaseNext(dPlayerUnits);
				pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
				if (terrainIsType(vectorToGrid(pos), 5, 7)) {
					xUnitSelectByID(dPlayerUnits, xUnitID);
					damagePlayerUnit(100.0);
				}

				xSetPointer(dPlayerUnits, action);
			}
		}

		for (y=xsMin(2, xGetDatabaseCount(dHekaProj)); > 0) {
			if (processGenericProj(dHekaProj) == PROJ_FALLING) {
				start = kbGetBlockPosition(""+xGetInt(dHekaProj, xUnitName), true);
				prev = xGetVector(dHekaProj, xProjPrev);
				dir = xGetVector(dHekaProj, xProjDir);
				dist = distanceBetweenVectors(prev, start, false);
				if (dist > 2.0) {
					for (x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (xGetBool(dPlayerUnits, xLaunched) == false) {
							pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true);
							amt = distanceBetweenVectors(pos, prev, false);
							if (amt < dist + 4.0) {
								hitbox = prev + (dir * amt);
								if (distanceBetweenVectors(pos, hitbox) < 16.0) {
									launchUnit(dPlayerUnits, pos + (dir * 60.0));
								}
							}
						}
					}
					xSetVector(dHekaProj, xProjPrev, start);

					if (trQuestVarGet("secondPhase") == 1) {
						pos = prev;
						for(x=(1 + dist) / 2; >0) {
							paintLava(pos);
							pos = pos + (dir * 2.0);
						}
					}
				}
				
				if (distanceBetweenVectors(start, trVectorQuestVarGet("bossRoomCenter"), false) > (2.0 * trQuestVarGet("bossRoomSize") - 4.0)) {
					xUnitSelectByID(dHekaProj, xUnitID);
					if (trUnitVisToPlayer()) {
						trCameraShake(0.5, 0.5);
						trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
					}
					trUnitChangeProtoUnit("Meteor Impact Ground");
					xUnitSelectByID(dHekaProj, xUnitID);
					trDamageUnitPercent(-100);
					trSoundPlayFN("meteorbighit.wav","1",-1,"","");
					trQuestVarSetFromRand("rand", 0, 3.14, false);
					
					dir = vectorSetFromAngle(trQuestVarGet("rand"));
					for(i=16; >0) {
						addGenericProj(dFireGiantBalls,start,dir,ENEMY_PLAYER);
						xSetVector(dFireGiantBalls, xProjPrev, start);
						dir = rotationMatrix(dir, 0.92388, 0.382683);
					}

					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), start) < 36.0) {
							damageOpponentUnit(p, 500.0);
						}
					}

					xFreeDatabaseBlock(dHekaProj);
				}
			}
		}
		
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			if (trQuestVarGet("bossSpell") == 31) {
				trQuestVarSet("bossUsedUltimate", 1);
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				trSetLighting("night", 1.0);
				trOverlayText("Doomsday",3.0,-1,-1,-1);
				trUnitOverrideAnimation(25,0,false,false,-1);
				bossAnim = true;
				bossTimeout = trTimeMS() + 4000;
				trQuestVarSet("bossSpell", 32);
			} else if (trQuestVarGet("bossSpell") == 32) {
				if (trTimeMS() > bossTimeout) {
					trUnitOverrideAnimation(-1,0,false,true,-1);
					bossAnim = false;
					bossNext = trTimeMS();
					bossTimeout = trTimeMS() + 8000 + 4000 * trQuestVarGet("secondPhase");
					trQuestVarSet("bossSpell", 33);
				}
			} else if (trQuestVarGet("bossSpell") == 33) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 800;
					trQuestVarSetFromRand("modx", 4.0 - 2 * trQuestVarGet("bossRoomSize"), 2 * trQuestVarGet("bossRoomSize") - 4.0, true);
					dist = xsSqrt(xsPow(2.0 * trQuestVarGet("bossRoomSize") - 8.0, 2) - xsPow(trQuestVarGet("modx"), 2));
					trQuestVarSetFromRand("modz", 0.0 - dist, dist, true);
					pos = vectorSnapToGrid(xsVectorSet(trQuestVarGet("modx"),0,trQuestVarGet("modz")) + trVectorQuestVarGet("bossRoomCenter"));
					zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, 0.00001);
					xAddDatabaseBlock(dLionMeteors, true);
					xSetInt(dLionMeteors, xUnitName, trGetNextUnitScenarioNameNumber());
					zSetProtoUnitStat("Kronny Flying", 0, 1, 0.00001);
					trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
					trSetSelectedScale(0,10.0,0);
					trDamageUnitPercent(100);
				}
				if (trTimeMS() > bossTimeout) {
					bossCooldown(8, 12);
					trQuestVarSet("bossUltimate", 3);
				}
			}
		} else if (trQuestVarGet("bossSpell") > 20) {
			if (trQuestVarGet("bossSpell") == 21) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Hellpath!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Who wants a piece of this?");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: These hands are rated E for everyone!");
				}
				trSoundPlayFN("wild.wav","1",-1,"","");
				trSoundPlayFN("cinematics\32_out\doorseal.mp3","1",-1,"","");
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 1, 3.0);
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 27, 200.0);
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
				trMutateSelected(kbGetProtoUnitID("Tartarian Gate"));
				trUnitOverrideAnimation(6,0,false,false,-1);
				trSetSelectedScale(0,0,0);

				bossPos = kbGetBlockPosition(""+bossUnit);
				trQuestVarSet("bossSpell", 22);
				bossTimeout = trTimeMS() + 12000;
			} else if (trQuestVarGet("bossSpell") == 22) {
				pos = kbGetBlockPosition(""+bossUnit);
				dist = distanceBetweenVectors(bossPos, pos);
				if (dist > 0) {
					dist = xsSqrt(dist);
					dir = getUnitVector(bossPos, pos, 2.0);
					for (i = 1 + dist / 2; > 0) {
						paintLava(bossPos);
						bossPos = bossPos + dir;
					}
					bossPos = pos;
				}
				if (trTimeMS() > bossTimeout) {
					bossCooldown(8, 12);
					trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 1, -3.0);
					trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 27, -200.0);
					trUnitSelectClear();
					trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,bossInts,1));
					trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				}
			}
		} else if (trQuestVarGet("bossSpell") > 10) {
			if (trQuestVarGet("bossSpell") == 11) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Eruption!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Watch your feet!");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Are you ready to rumble?!");
				}
				bossNext = trTimeMS() + 1800;
				trQuestVarSet("bossSpell", 12);
				bossAnim = true;
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 55, 2);
				trMutateSelected(kbGetProtoUnitID("Titan Atlantean"));
				trUnitOverrideAnimation(26, 0, false, false, -1);
			} else if (trQuestVarGet("bossSpell") == 12) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 1100;
					trQuestVarSet("bossWarnStart", trGetNextUnitScenarioNameNumber());
					for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
						xDatabaseNext(dPlayerCharacters);
						xUnitSelectByID(dPlayerCharacters, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerCharacter();
						} else {
							pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName), true));
							trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
							trArmySelect("1,0");
							trMutateSelected(kbGetProtoUnitID("Ball of Fire"));
						}
					}
					trQuestVarSet("bossSpell", 13);
					trQuestVarSet("bossWarnEnd", trGetNextUnitScenarioNameNumber());
					trCameraShake(0.7, 0.3);
					trSoundPlayFN("implode reverse.wav","1",-1,"","");
				}
			} else if (trQuestVarGet("bossSpell") == 13) {
				if (trTimeMS() > bossNext) {
					trCameraShake(0.5, 0.8);
					trSoundPlayFN("cinematics\32_out\explosion.mp3","1",-1,"","");
					bossCooldown(8, 12);
					bossAnim = false;
					trUnitOverrideAnimation(-1, 0, false, true, -1);
					trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 55, 1);
					for(x=trQuestVarGet("bossWarnStart"); < trQuestVarGet("bossWarnEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						if (trUnitAlive()) {
							pos = kbGetBlockPosition(""+x, true);
							trUnitChangeProtoUnit("Volcano");
							for(y=xGetDatabaseCount(dPlayerUnits); >0) {
								xDatabaseNext(dPlayerUnits);
								xUnitSelectByID(dPlayerUnits, xUnitID);
								if (trUnitAlive() == false) {
									removePlayerUnit();
								} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 9.0) {
									damagePlayerUnit(1000.0);
								}
							}
							if (trQuestVarGet("secondPhase") == 1) {
								paintLava(pos);
								dir = vector(2,0,0);
								for(y=4; >0) {
									paintLava(pos + dir);
									dir = rotationMatrix(dir, 0.0, 1.0);
								}
							}
						}
					}
				}
			}
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				trQuestVarSetFromRand("rand", 1, 5, true);
				if (trQuestVarGet("rand") == 1) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Crossfire!");
				} else if (trQuestVarGet("rand") == 2) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Welcome to hell!");
				} else if (trQuestVarGet("rand") == 3) {
					trChatSendSpoofed(ENEMY_PLAYER, "Hellkeeper: Smash!");
				}
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 55, 2);
				trMutateSelected(kbGetProtoUnitID("Titan Atlantean"));
				bossPos = vectorSnapToGrid(kbGetBlockPosition(""+bossUnit, true));
				trQuestVarSet("bossWarnStart", trGetNextUnitScenarioNameNumber());
				trQuestVarSetFromRand("rand", 0, 1, true);
				if (trQuestVarGet("rand") == 0) {
					dir = vector(-2,0,0);
				} else {
					dir = vector(-2,0,-2);
				}
				bossDir = xsVectorNormalize(dir);
				for(x=4; >0) {
					pos = bossPos + dir;
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("0,0");
					trSetUnitOrientation(bossDir, vector(0,1,0), true);
					trUnitChangeProtoUnit("UI Range Indicator Atlantean SFX");
					bossDir = rotationMatrix(bossDir, 0.0, 1.0);
					dir = rotationMatrix(dir, 0.0, 1.0);
				}
				trQuestVarSet("bossWarnEnd", trGetNextUnitScenarioNameNumber());

				trQuestVarSet("bossSpell", 2);
				bossNext = trTimeMS() + 1200;
			} else if (trQuestVarGet("bossSpell") == 2) {
				trMutateSelected(kbGetProtoUnitID("Titan Atlantean"));
				if (trTimeMS() > bossNext) {
					bossAnim = true;
					trQuestVarSet("bossSpell", 3);
					bossNext = trTimeMS() + 1800;
					trUnitOverrideAnimation(26, 0, false, false, -1);
				}
			} else if (trQuestVarGet("bossSpell") == 3) {
				if (trTimeMS() > bossNext) {
					bossNext = bossNext + 1100;
					for (x=trQuestVarGet("bossWarnStart"); < trQuestVarGet("bossWarnEnd")) {
						trUnitSelectClear();
						trUnitSelect(""+x, true);
						trUnitDestroy();
					}
					for(x=4; >0) {
						addGenericProj(dHekaProj,bossPos,bossDir);
						xSetVector(dHekaProj, xProjPrev, bossPos);
						bossDir = rotationMatrix(bossDir, 0.0, 1.0);
					}
					trQuestVarSet("bossSpell", 4);
				}
			} else if (trQuestVarGet("bossSpell") == 4) {
				if (trTimeMS() > bossNext) {
					trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 55, 1);
					bossAnim = false;
					trUnitOverrideAnimation(-1,0,false,true,-1);
					bossCooldown(6, 12);
				}
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
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
		xsDisableRule("boss_music");
		if (trQuestVarGet("secondPhase") == 1) {
			xsEnableRule("boss_ded");
			xsDisableRule("gameplay_always");
		} else {
			trQuestVarSet("secondPhase", 1);
			trQuestVarSet("cinStep", 0);
			trQuestVarSet("cinTime", trTime() + 3);
			xsEnableRule("boss9_start_again");
			trForceNonCinematicModels(true);
			trUIFadeToColor(0,0,0,1000,0,true);
			trLetterBox(true);
		}
		boss = 0;
		trSetLighting("hades", 1.0);
		trSoundPlayFN("win.wav","1",-1,"","");
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			trDamageUnitPercent(100);
		}
		uiLookAtUnitByName(""+bossUnit);
	}
}


rule boss9_start_again
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trSoundPlayFN("","1",-1,"Hellkeeper: Foolish mortals. I will never let you reach the final floor!");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 2:
			{
				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"Hellkeeper: The Pit shall be your grave!");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 3:
			{
				xsEnableRule("boss9_battle");
				bossUnit = trGetNextUnitScenarioNameNumber();
				vector pos = trVectorQuestVarGet("bossRoomCenter");
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,
					xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit("Heka Gigantes");
				boss = trQuestVarGet("stage");
				activateEnemy(bossUnit);
				bossID = kbGetBlockID(""+bossUnit, true);
				bossPointer = xGetNewestPointer(dEnemies);
				xSetBool(dEnemies, xLaunched, true);
				for(i=xStunSFX; <= xSilenceSFX) {
					spyEffect(bossUnit, kbGetProtoUnitID("Cinematic Block"), xsVectorSet(dEnemies, i, bossPointer));
				}
				xsEnableRule("boss9_ready");
				bossCooldown(8, 12);
				xsEnableRule("boss_music");
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				trQuestVarSet("musicTime", 0);
				trVectorQuestVarSet("gateSpawnDir", vector(1,0,0));
			}
		}
	}
}

rule boss9_ready
inactive
highFrequency
{
	if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
		xsDisableSelf();
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		trMutateSelected(kbGetProtoUnitID("Titan Atlantean"));
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
				if (trQuestVarGet("normalRelicCount") > 0) {
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


void keeperWrongAnswer(int eventID = -1) {
	trCameraShake(5.0, 2.5);
	trSoundPlayFN("pestilencebirth.wav","1",-1,"","");
	uiMessageBox("WRONG");
}

void keeperQuestion() {
	trQuestVarSetFromRand("rand1", 10, 20, true);
	trQuestVarSetFromRand("rand2", 1, trQuestVarGet("rand1") - 1, true);
	trQuestVarSetFromRand("rand", 0, 1, true);
	if (trIsGadgetVisible("ShowChoiceBox") == false) {
		trSoundPlayFN("spybirth.wav","1",-1,"","");
		trQuestVarSet("answer1", trQuestVarGet("rand1") + trQuestVarGet("rand2"));
		trQuestVarSet("answer0", trQuestVarGet("rand1") - trQuestVarGet("rand2"));
		trShowChoiceDialog(""+1*trQuestVarGet("rand1") + " + " + 1*trQuestVarGet("rand2") + " = ?", 
			"" + 1*trQuestVarGet("answer"+1*trQuestVarGet("rand")), 10001 - trQuestVarGet("rand"),
			"" + 1*trQuestVarGet("answer"+(1 - trQuestVarGet("rand"))), 10000 + trQuestVarGet("rand"));
	}
}


int dFingers = 0;
int xFingerDir = 0;
int xFingerStep = 0;
int xFingerTimeout = 0;

rule the_keeper_hunt_start
inactive
highFrequency
{
	xsDisableSelf();

	if (Multiplayer) {
		vector pos = trVectorQuestVarGet("bossRoomCenter");
		trVectorQuestVarSet("keeperPos", vectorToGrid(pos));
		trQuestVarSet("keeperHawk", trGetNextUnitScenarioNameNumber());
		trArmyDispatch(""+ENEMY_PLAYER+",0","Militia",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		spyEffect(1*trQuestVarGet("keeperHawk"), kbGetProtoUnitID("Tartarian Gate"), xsVectorSet(ARRAYS, bossInts, 1));
	}

	dKeeperPaint = xInitDatabase("keeperPaint");
	xKeeperPaintTimeout = xInitAddInt(dKeeperPaint, "timeout");
	xKeeperPaintPrimary = xInitAddInt(dKeeperPaint, "primary");
	xKeeperPaintSecondary = xInitAddInt(dKeeperPaint, "secondary");
	xKeeperPaintPos = xInitAddVector(dKeeperPaint, "pos");

	dFingers = xInitDatabase("fingers");
	xInitAddInt(dFingers, "name");
	xFingerDir = xInitAddVector(dFingers, "dir");
}

rule the_keeper_hunt_sfx
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
		trQuestVarSet("keeperTartarianGate", aiPlanGetUserVariableInt(ARRAYS, bossInts, 1));

		xsDisableSelf();
		trModifyProtounit("Parrot", ENEMY_PLAYER, 1, 1.2);
		trUnitSelectClear();
		trUnitSelectByQV("keeperHawk");
		trMutateSelected(kbGetProtoUnitID("Parrot"));
		trSetSelectedScale(0,0,0);
		trUnitSelectClear();
		trUnitSelectByQV("keeperTartarianGate");
		trUnitChangeProtoUnit("Tartarian Gate");
		trUnitSelectClear();
		trUnitSelectByQV("keeperTartarianGate");
		trUnitOverrideAnimation(6,0,true,false,-1);
		trSetSelectedScale(0,0,0);

		xsEnableRule("the_keeper_hunt");
		xsEnableRule("the_keeper_hunt_warn");
		if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") == 3) {
			trMessageSetText("Something ominous is searching for you.", -1);
		}
	}
}

rule the_keeper_hunt
inactive
highFrequency
{
	vector pos = vectorToGrid(kbGetBlockPosition(""+1*trQuestVarGet("keeperHawk")));
	vector prev = trVectorQuestVarGet("keeperPos");
	int primary = 0;
	int secondary = 0;
	int x = 0;
	int z = 0;
	int p = 0;
	if (xsAbs(xsVectorGetX(pos) - xsVectorGetX(prev)) + xsAbs(xsVectorGetZ(pos) - xsVectorGetZ(prev)) > 0) {
		for(x=xsVectorGetX(pos) - 2; <= xsVectorGetX(pos) + 2) {
			for(z=xsVectorGetZ(pos) - 2; <= xsVectorGetZ(pos) + 2) {
				primary = trGetTerrainType(x, z);
				secondary = trGetTerrainSubType(x, z);
				if (primary != 5 || secondary != 4) {
					trQuestVarSetFromRand("rand", 2000, 3000, true);
					xAddDatabaseBlock(dKeeperPaint, true);
					xSetInt(dKeeperPaint, xKeeperPaintTimeout, trTimeMS() + trQuestVarGet("rand"));
					xSetInt(dKeeperPaint, xKeeperPaintPrimary, primary);
					xSetInt(dKeeperPaint, xKeeperPaintSecondary, secondary);
					xSetVector(dKeeperPaint, xKeeperPaintPos, xsVectorSet(x, 0, z));
					trPaintTerrain(x, z, x, z, 5, 4, false);
				}
			}
		}
		trVectorQuestVarSet("keeperPos", pos);
		trUnitSelectClear();
		trUnitSelectByQV("keeperTartarianGate");
		if (trUnitVisToPlayer()) {
			if (trTime() > trQuestVarGet("keeperSoundTime")) {
				trSoundPlayFN("cinematics\32_out\kronosbehinddorrshort.mp3","1",-1,"","");
				trQuestVarSet("keeperSoundTime", trTime() + 3);
			}
		}
		for(x=xGetDatabaseCount(dKeeperTargets); >0) {
			xDatabaseNext(dKeeperTargets);
			p = xGetInt(dKeeperTargets, xKeeperTargetPlayer);
			if ((xGetBool(dPlayerData, xPlayerLaunched, p) == false) && (xGetInt(dPlayerData, xPlayerDead, p) == 0)) {
				pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit, p)));
				if (terrainIsType(vectorToGrid(pos), 5, 4)) {
					gadgetUnreal("ShowChoiceBox");
					trUnitSelectClear();
					trUnitSelectByQV("keeperTartarianGate");
					trUnitDestroy();
					trUnitSelectClear();
					trUnitSelectByQV("keeperHawk");
					trUnitDestroy();
					// uh oh... 
					xsDisableSelf();
					trPlayerKillAllGodPowers(p);
					xSetBool(dPlayerData, xPlayerSilenced, false, p); // can't escape now buddy
					xAddDatabaseBlock(dStunnedUnits, true);
					xSetInt(dStunnedUnits, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
					xSetInt(dStunnedUnits, xStunnedProto, xGetInt(dClass, xClassProto, xGetInt(dPlayerData, xPlayerClass, p)));
					uiLookAtUnitByName(""+xGetInt(dPlayerData, xPlayerUnit, p));
					trSoundPlayFN("Zenophobia\True Origin shortened.mp3","1",-1,"","");
					trSoundPlayFN("cinematics\32_out\kronosbehinddorrlong.mp3","1",-1,"","");
					trSoundPlayFN("xpack\xcinematics\8_in\guardianawaken.mp3","1",-1,"","");
					trCameraShake(10.0, 0.1);
					trVectorQuestVarSet("keeperPos", pos);
					trQuestVarSet("keeperTartarianGate", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("keeperTartarianGate");
					trUnitChangeProtoUnit("Tartarian Gate");
					trUnitSelectClear();
					trUnitSelectByQV("keeperTartarianGate");
					trUnitOverrideAnimation(6,0,true,false,-1);
					trSetSelectedScale(0,0,0);
					trQuestVarSet("keeperGrabStep", 0);
					trQuestVarSet("keeperGrabTime", trTimeMS());
					xsEnableRule("keeper_grab");
					trQuestVarSet("keeperTarget", p);
					trMessageSetText(trStringQuestVarGet("p"+p+"name") + " has been devoured and lost a level!", -1);
					break;
				}
			}
		}
	}

	if (xGetDatabaseCount(dKeeperPaint) > 0) {
		xDatabaseNext(dKeeperPaint);
		if (trTimeMS() > xGetInt(dKeeperPaint, xKeeperPaintTimeout)) {
			pos = xGetVector(dKeeperPaint, xKeeperPaintPos);
			x = xsVectorGetX(pos);
			z = xsVectorGetZ(pos);
			trPaintTerrain(x, z, x, z, xGetInt(dKeeperPaint, xKeeperPaintPrimary), xGetInt(dKeeperPaint, xKeeperPaintSecondary), false);
			xFreeDatabaseBlock(dKeeperPaint);
		}
	}
}

void spawnFinger(vector center = vector(0,0,0), vector offset = vector(0,0,0), vector dest = vector(0,0,0), float scale = 0) {
	vector pos = center + offset;
	vector dir = xsVectorNormalize(rotationMatrix(dest - offset, 0, -1.0));
	xAddDatabaseBlock(dFingers, true);
	xSetInt(dFingers, xUnitName, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	xUnitSelect(dFingers, xUnitName);
	trMutateSelected(kbGetProtoUnitID("Taproot Small"));
	trSetSelectedScale(0.0 - scale, scale, 0.0 - scale);
	trUnitSetAnimationPath("0,0,0,0,0,0,0");
	trSetUnitOrientation(dir, vector(0,-1,0), true);
	xSetVector(dFingers, xFingerDir, dir);
}

rule keeper_grab
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	int primary = 0;
	int secondary = 0;
	int p = trQuestVarGet("keeperTarget");
	int dist = 0;
	vector pos = vector(0,0,0);
	vector center = vector(0,0,0);
	vector test = vector(0,0,0);
	switch(1*trQuestVarGet("keeperGrabStep"))
	{
		case 0:
		{
			if (trTimeMS() > trQuestVarGet("keeperGrabTime")) {
				trQuestVarSet("keeperGrabTime", trQuestVarGet("keeperGrabTime") + 400);
				trQuestVarSet("keeperGrabRadius", 1 + trQuestVarGet("keeperGrabRadius"));
				pos = vectorToGrid(trVectorQuestVarGet("keeperPos"));
				dist = xsPow(trQuestVarGet("keeperGrabRadius"), 2);
				for(a=0-trQuestVarGet("keeperGrabRadius"); <= trQuestVarGet("keeperGrabRadius")) {
					for(b=0-trQuestVarGet("keeperGrabRadius"); <= trQuestVarGet("keeperGrabRadius")) {
						test = pos + xsVectorSet(a, 0, b);
						if (distanceBetweenVectors(pos, test) <= dist) {
							x = xsVectorGetX(test);
							z = xsVectorGetZ(test);
							primary = trGetTerrainType(x, z);
							secondary = trGetTerrainSubType(x, z);
							if ((primary != 5) || (secondary != 4)) {
								xAddDatabaseBlock(dKeeperPaint, true);
								xSetInt(dKeeperPaint, xKeeperPaintPrimary, primary);
								xSetInt(dKeeperPaint, xKeeperPaintSecondary, secondary);
								xSetVector(dKeeperPaint, xKeeperPaintPos, xsVectorSet(x, 0, z));
								trPaintTerrain(x, z, x, z, 5, 4, false);
								trChangeTerrainHeight(x, z, x, z, worldHeight, false);
							}
						}
					}
				}
				if (trQuestVarGet("keeperGrabRadius") >= 8) {
					trQuestVarSet("keeperGrabStep", 1);
				}
			}
		}
		case 1:
		{
			center = trVectorQuestVarGet("keeperPos");
			test = vector(-10, 0, -10);
			trQuestVarSet("keeperGrabStep", 2);
			// thumb
			spawnFinger(center, vector(-6, 0, 2), vector(-1,0,-1), 3.5);
			// other fingers
			spawnFinger(center, vector(0, 0, 4), test, 4.5);
			spawnFinger(center, vector(2, 0, 2), test, 5.5);
			spawnFinger(center, vector(4, 0, 0), test, 4.5);
			spawnFinger(center, vector(4, 0, -2), test, 3.5);

			trSoundPlayFN("cinematics\31_in\swipenew.mp3","1",-1,"","");
			trQuestVarSet("keeperGrabStep", 2);
			trQuestVarSet("keeperAngle", 3.141592);
			trQuestVarSet("keeperGrabTime", trTimeMS());

			trCameraShake(3.0, 0.3);
		}
		case 2:
		{
			float diff = trTimeMS() - trQuestVarGet("keeperGrabTime");
			trQuestVarSet("keeperAngle", 3.141592 - 0.001 * diff);
			float mCos = xsCos(trQuestVarGet("keeperAngle"));
			float mSin = xsSin(trQuestVarGet("keeperAngle"));
			for(i=5; >0) {
				xDatabaseNext(dFingers);
				xUnitSelect(dFingers, xUnitName);
				pos = xsVectorSetY(rotationMatrix(xGetVector(dFingers, xFingerDir), 0.0, -1.0) * mSin, mCos);
				trSetUnitOrientation(xGetVector(dFingers, xFingerDir), pos, true);
			}
			if (trQuestVarGet("keeperAngle") < 0) {
				trQuestVarSet("keeperGrabStep", 3);
				trSoundPlayFN("xpack\xcinematics\7_in\krioschange.mp3","1",-1,"","");
				trUnitSelectClear();
				trUnitSelectByQV("keeperTartarianGate");
				trUnitChangeProtoUnit("Tartarian Gate birth");
				trUnitSelectClear();
				trUnitSelectByQV("keeperTartarianGate");
				trSetSelectedScale(0,0,0);
				trCameraShake(3.0, 0.5);
				if (Multiplayer) {
					for(x=xGetDatabaseCount(getRelicsDB(p)); >0) {
						xDatabaseNext(getRelicsDB(p));
						xUnitSelect(getRelicsDB(p), xUnitName);
						trUnitDestroy();
					}
					xSetInt(dPlayerData, xPlayerDead, 10, p);
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p));
					trUnitDelete(false);
					trQuestVarSet("keeperGrabTime", trTimeMS() + 1000);
					if (xGetInt(dPlayerData, xPlayerLevel, p) > 0) {
						if (trCurrentPlayer() == p) {
							/* class levels */
							for(a=0; <2) {
								savedata = trGetScenarioUserData(10 + a);
								if (savedata < 0) {
									savedata = 0;
								}
								for(b=1; <9) {
									xSetInt(dClass, xClassLevel, iModulo(11, savedata), b + 8 * a);
									savedata = savedata / 11;
								}
							}
							xSetInt(dClass, xClassLevel, xGetInt(dPlayerData, xPlayerLevel, p), xGetInt(dPlayerData, xPlayerClass, p));
							for(a=0; <2) {
								savedata = 0;
								for(b=8; >0) {
									currentdata = 1*xsMin(10, xGetInt(dClass, xClassLevel, b + 8 * a));
									savedata = savedata * 11 + currentdata;
								}
								trSetCurrentScenarioUserData(10 + a, savedata);
							}
						}
						xSetInt(dPlayerData, xPlayerLevel, xGetInt(dPlayerData, xPlayerLevel, p) - 1, p);
						trModifyProtounit(kbGetProtoUnitName(xGetInt(dClass, xClassProto, xGetInt(dPlayerData, xPlayerClass, p))), p, 5, -1);
					}
				} else {
					trUnitSelectClear();
					trUnitSelectByQV("venlesh");
					trUnitChangeProtoUnit("Kronny Birth SFX");
					trSoundPlayPaused("dialog\ko\genr122m.mp3","1",-1,"","");
				}
			}
		}
		case 3:
		{
			if (trTimeMS() > trQuestVarGet("keeperGrabTime")) {
				trQuestVarSet("keeperGrabTime", trTimeMS() + 400);
				trQuestVarSet("keeperGrabRadius", trQuestVarGet("keeperGrabRadius") - 1);
				dist = xsPow(trQuestVarGet("keeperGrabRadius"), 2);
				center = vectorToGrid(trVectorQuestVarGet("keeperPos"));
				for(i=xGetDatabaseCount(dKeeperPaint); >0) {
					xDatabaseNext(dKeeperPaint);
					pos = xGetVector(dKeeperPaint, xKeeperPaintPos);
					if (distanceBetweenVectors(center, pos) >= dist) {
						x = xsVectorGetX(pos);
						z = xsVectorGetZ(pos);
						trPaintTerrain(x, z, x, z, xGetInt(dKeeperPaint, xKeeperPaintPrimary), xGetInt(dKeeperPaint, xKeeperPaintSecondary), false);
						xFreeDatabaseBlock(dKeeperPaint);
					}
				}
				if (xGetDatabaseCount(dKeeperPaint) == 0) {
					if (trCurrentPlayer() == trQuestVarGet("keeperTarget")) {
						trSoundPlayFN("tartariangateselect.wav","1",-1,"","");
						uiMessageBox("You have lost a level.");
					}
					xsDisableSelf();
					trSoundPlayFN("spybirth.wav","1",-1,"","");
					trCameraShake(3.0, 1.0);
					for(i=xGetDatabaseCount(dFingers); >0) {
						xDatabaseNext(dFingers);
						xUnitSelect(dFingers, xUnitName);
						trUnitChangeProtoUnit("Hero Death");
					}
					trUnitSelectClear();
					trUnitSelectByQV("keeperTartarianGate");
					trUnitDestroy();
				}
			}
		}
	}
}


rule boss11_init
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	float dist = 0;
	vector pos = vector(0,0,0);
	if (trTime() > trQuestVarGet("cinTime")) {
		switch(1*trQuestVarGet("cinStep"))
		{
			case 0:
			{
				trUIFadeToColor(0,0,0,1000,0,false);
				trQuestVarSet("cinTime", trTime() + 2);
				uiLookAtUnitByName(""+1*trQuestVarGet("runestone"));
				xFreeDatabaseBlock(dEnemies, bossPointer);
				unitTransform("Imperial Examination", "Rocket");
			}
			case 1:
			{
				trCameraShake(8.0, 0.2);
				trUnitSelectClear();
				trUnitSelect(""+bossUnit);
				trUnitChangeProtoUnit("Tartarian Gate birth");
				trUnitSelectClear();
				trUnitSelect(""+bossUnit);
				trSetSelectedScale(0,0,0);
				trSoundPlayFN("cinematics\32_out\kronosbehinddorrlong.mp3","1",-1,"","");
				trQuestVarSet("keeperGrabTime", trTimeMS());
				trQuestVarSet("keeperGrabRadius", 0);
				trVectorQuestVarSet("keeperPos", vectorSnapToGrid(trVectorQuestVarGet("bossRoomCenter")));
			}
			case 2:
			{
				if (trTimeMS() > trQuestVarGet("keeperGrabTime")) {
					trQuestVarSet("keeperGrabTime", trQuestVarGet("keeperGrabTime") + 200);
					trQuestVarSet("keeperGrabRadius", 1 + trQuestVarGet("keeperGrabRadius"));
					pos = vectorToGrid(trVectorQuestVarGet("keeperPos"));
					dist = xsPow(trQuestVarGet("keeperGrabRadius"), 2);
					for(a=0-trQuestVarGet("keeperGrabRadius"); <= trQuestVarGet("keeperGrabRadius")) {
						for(b=0-trQuestVarGet("keeperGrabRadius"); <= trQuestVarGet("keeperGrabRadius")) {
							vector test = pos + xsVectorSet(a, 0, b);
							if (distanceBetweenVectors(pos, test) <= dist) {
								x = xsVectorGetX(test);
								z = xsVectorGetZ(test);
								trPaintTerrain(x, z, x, z, 5, 4, false);
							}
						}
					}
				}
				if (trQuestVarGet("keeperGrabRadius") < trQuestVarGet("bossRoomSize")) {
					trQuestVarSet("cinStep", 1);
				}
			}
			case 3:
			{
				trUnitSelectClear();
				trUnitSelectByQV("runestone");
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
				trUnitChangeProtoUnit("Revealer");
				trUIFadeToColor(0,0,0,2000,0,true);
				trQuestVarSet("cinTime", trTime() + 3);
			}
			case 4:
			{
				bossDir = xsVectorSet(0,0,trQuestVarGet("bossRoomSize") * 2);
				for(x=32; >0) {
					pos = vectorSnapToGrid(trVectorQuestVarGet("keeperPos") + bossDir);
					z = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelect(""+z, true);
					trUnitChangeProtoUnit("Spy Eye");
					trUnitSelectClear();
					trUnitSelect(""+z, true);
					trMutateSelected(kbGetProtoUnitID("Tartarian Gate"));
					trSetSelectedScale(0,0,0);
					trUnitOverrideAnimation(6,0,true,false,-1);
					bossDir = rotationMatrix(bossDir, 0.980785, 0.19509);
				}
				trQuestVarSet("cinTime", trTime() + 3);
				trOverlayText("The Shadow That Crawls", 3.0, -1, -1, -1);
			}
			case 5:
			{
				trEventSetHandler(10001, "keeperWrongAnswer");
				xFingerStep = xInitAddInt(dFingers, "step");
				xFingerTimeout = xInitAddInt(dFingers, "timeout");

				bossDir = vector(1,0,0);

				pos = trVectorQuestVarGet("keeperPos");
				zSetProtoUnitStat("Revealer", 1, 2, 32);
				trArmyDispatch("1,0","Revealer",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
				trArmySelect("1,0");
				trUnitConvert(0);

				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsDisableSelf();

				trQuestVarSet("keeperClawAttack", trTime() + 20);
				trQuestVarSet("keeperMathQuestionTime", trTime() + 30);
				trQuestVarSet("runestoneSpawnTime", trTime() + 15);
				
				xsEnableRule("boss11_battle");
				
				xsEnableRule("boss_music");

				xsEnableRule("boss11_battle");

				bossNext = trTimeMS() + 3000;

				trQuestVarSet("bossHealth", 100);
				trCounterAddTime("bossHealth", -1, -9999, "<color={Playercolor(2)}>Crawling Shadow: " + 1*trQuestVarGet("bossHealth") + "</color>", -1);

				trStringQuestVarSet("advice","The Shadow's hunt continues.");
			}
		}
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
	}
}

void spawnAttackFinger(vector pos = vector(0,0,0), vector dir = vector(0,0,0)) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	trArmySelect("0,0");
	trUnitChangeProtoUnit("Tartarian Gate Flame");
	trUnitSelectClear();
	trUnitSelect(""+next, true);
	trSetSelectedScale(0,0,0);
	trSetUnitOrientation(dir, vector(0,1,0), true);

	xAddDatabaseBlock(dFingers, true);
	xSetInt(dFingers, xUnitName, trGetNextUnitScenarioNameNumber());
	trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
	xUnitSelect(dFingers, xUnitName);
	trMutateSelected(kbGetProtoUnitID("Taproot Small"));
	trSetSelectedScale(-0.5, 2.0, -2.0);
	trUnitSetAnimationPath("0,0,0,0,0,0,0");
	trSetUnitOrientation(rotationMatrix(dir, 0, -1.0), vector(0,0,0) - dir, true);
	xSetVector(dFingers, xFingerDir, dir);
	xSetInt(dFingers, xFingerTimeout, trTimeMS() + 1500);
}

rule boss11_battle
inactive
highFrequency
{
	float diff = 0;
	float mSin = 0;
	float mCos = 0;
	int count = 0;
	int next = 0;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	trQuestVarSet("sound", 0);
	for(i=xGetDatabaseCount(dFingers); >0) {
		xDatabaseNext(dFingers);
		xUnitSelect(dFingers, xUnitName);
		switch(xGetInt(dFingers, xFingerStep))
		{
			case 0:
			{
				diff = xGetInt(dFingers, xFingerTimeout) - trTimeMS();
				if (diff > 0) {
					diff = 3.5 - 0.002 * diff;
					trSetSelectedScale(0.0 - diff, 0.5 * diff, -2.0);
				} else {
					xSetInt(dFingers, xFingerStep, 1);
					xSetInt(dFingers, xFingerTimeout, trTimeMS());
					trQuestVarSetFromRand("sound", 1, 3, true);
					trSoundPlayFN("tartarianspawngrunt"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				}
			}
			case 1:
			{
				diff = 1.570796 + 0.01 * (trTimeMS() - xGetInt(dFingers, xFingerTimeout));
				mSin = xsSin(diff);
				mCos = xsCos(diff);
				dir = xsVectorSetY(xGetVector(dFingers, xFingerDir) * mSin, mCos);
				trSetUnitOrientation(rotationMatrix(xGetVector(dFingers, xFingerDir), 0.0, -1.0), vector(0,0,0) - dir, true);
				if (diff > 3.5) {
					trQuestVarSet("sound", 0);
					xSetInt(dFingers,xFingerStep, 2);
					xSetInt(dFingers, xFingerTimeout, trTimeMS() + 1000);
					dir = xGetVector(dFingers, xFingerDir);
					pos = kbGetBlockPosition(""+xGetInt(dFingers, xUnitName), true);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (rayCollision(dPlayerUnits, pos, dir, 6.0, 2.0)) {
							damagePlayerUnit(500.0);
							trQuestVarSet("sound", 1);
						}
					}

					if (trQuestVarGet("sound") == 1) {
						trQuestVarSetFromRand("sound", 1, 2, true);
						trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					}
				}
			}
			case 2:
			{
				diff = trTimeMS() - xGetInt(dFingers, xFingerTimeout);
				if (diff > 0) {
					diff = 3.5 - 0.002 * diff;
					if (diff > 0) {
						trSetSelectedScale(0.0 - diff, diff, 2.0);
					} else {
						trUnitDestroy();
						xFreeDatabaseBlock(dFingers);
					}
				}
			}
		}
	}

	if (trTimeMS() > bossNext) {
		trQuestVarSetFromRand("rand", 100, 3000, true);
		trQuestVarSetFromRand("rand2", 100, 3000, true);
		if (trQuestVarGet("rand2") < trQuestVarGet("rand")) {
			trQuestVarSet("rand", trQuestVarGet("rand2"));
		}
		bossNext = bossNext + trQuestVarGet("rand") * (50 + trQuestVarGet("bossHealth")) / 100; // as boss health goes down, it gets faster
		trQuestVarSetFromRand("rand", 1, 3, true);
		xDatabaseNext(dPlayerCharacters);
		xUnitSelectByID(dPlayerCharacters, xUnitID);
		if (trUnitAlive()) {
			pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName)) + bossDir * 4.0);
			dir = getUnitVector(pos, kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName)));
			spawnAttackFinger(pos, dir);
		}

		bossDir = rotationMatrix(bossDir, -0.740544, -0.672008);
	}

	if (trTime() > trQuestVarGet("keeperMathQuestionTime")) {
		trQuestVarSetFromRand("rand", 10, 30, true);
		trQuestVarSet("keeperMathQuestionTime", trTime() + trQuestVarGet("rand") * (50 + trQuestVarGet("bossHealth")) / 100);
		keeperQuestion();
	}

	if (trTime() > trQuestVarGet("keeperClawAttack")) {
		trSoundPlayFN("changeunit.wav","1",-1,"","");
		trQuestVarSetFromRand("rand", 10, 30, true);
		trQuestVarSet("keeperClawAttack", trTime() + trQuestVarGet("rand") * (50 + trQuestVarGet("bossHealth")) / 100);
		pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName)));
		dir = vector(1,0,0);
		for(i=8; >0) {
			spawnAttackFinger(pos - (dir * 4.0), dir);
			dir = rotationMatrix(dir, 0.707107, 0.707107);
		}
	}

	if (trQuestVarGet("runestoneActive") == 1) {
		pos = trVectorQuestVarGet("runestonePos");
		for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
			xDatabaseNext(dPlayerCharacters);
			xUnitSelectByID(dPlayerCharacters, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerCharacter();
			} else if (unitDistanceToVector(xGetInt(dPlayerCharacters, xUnitName), pos) < 36.0) {
				trUnitSelectClear();
				trUnitSelectByQV("runestone");
				trUnitChangeProtoUnit("Osiris SFX");
				trSoundPlayFN("recreation.wav","1",-1,"","");
				trQuestVarSet("bossHealth", trQuestVarGet("bossHealth") - 10);
				if (trQuestVarGet("bossHealth") > 0) {
					trCounterAbort("bossHealth");
					trCounterAddTime("bossHealth", -1, -9999, "<color={Playercolor(2)}>Crawling Shadow: " + 1*trQuestVarGet("bossHealth") + "</color>", -1);
					trQuestVarSet("runestoneActive", 0);
					trQuestVarSetFromRand("rand", 15, 30, true);
					trQuestVarSet("runestoneSpawnTime", trTime() + trQuestVarGet("rand"));
					dir = bossDir;
					count = (100 - trQuestVarGet("bossHealth")) * 0.1;
					diff = 6.283185 / count;
					mCos = xsCos(diff);
					mSin = xsSin(diff);
					for(y=count; >0) {
						pos = vectorSnapToGrid(trVectorQuestVarGet("bossRoomCenter") - dir * 20.0);
						next = trGetNextUnitScenarioNameNumber();
						trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						if (next < trGetNextUnitScenarioNameNumber()) {
							trArmySelect(""+ENEMY_PLAYER+",0");
							trUnitChangeProtoUnit("Argus");
							activateEnemy(next);
							trUnitSelectClear();
							trUnitSelect(""+next, true);
							trSetUnitOrientation(dir, vector(0,1,0), true);
						}
						dir = rotationMatrix(dir, mCos, mSin);
					}
					trCameraShake(3.0, 0.1 * count);
					trSoundPlayFN("cinematics\32_out\kronosbehinddorrshort.mp3","1",-1,"","");
				} else {
					xsDisableSelf();
					for(y=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						trUnitDelete(false);
					}
					xsDisableRule("boss_music");
					xsDisableRule("gameplay_always");
					xsEnableRule("boss11_dead");
					trQuestVarSet("gameOverNext", 0);
					trQuestVarSet("gameOverStep", 0);
				}
				break;
			}
		}
	} else if (trTime() > trQuestVarGet("runestoneSpawnTime")) {
		trQuestVarSet("runestoneActive", 1);
		pos = vectorSnapToGrid(trVectorQuestVarGet("bossRoomCenter") - bossDir * 20.0);
		trVectorQuestVarSet("runestonePos", pos);
		trQuestVarSet("runestone", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit("Runestone");
		trUnitSelectClear();
		trUnitSelectByQV("runestone");
		trSetUnitOrientation(bossDir, vector(0,1,0), true);
		trSetSelectedScale(2.5, 2.5, 2.5);
		trUnitHighlight(5.0, true);
		trMessageSetText("The Runestone has surfaced! You must touch it!", -1);
		trSoundPlayFN("cinematics\13_out\prayshort.mp3","1",-1,"","");
	}
}

rule boss11_dead
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("gameOverNext")) {
		trQuestVarSet("gameOverStep", 1 + trQuestVarGet("gameOverStep"));
		switch(1*trQuestVarGet("gameOverStep"))
		{
			case 1:
			{
				trQuestVarSet("playersWon", 1);
				trLetterBox(true);
				trUIFadeToColor(0,0,0, 2000,0,true);
				trQuestVarSet("gameOverNext", trTime() + 4);
				trSoundPlayFN("default","1",-1,
					"Zenophobia: The Shadow That Crawls has been vanquished, for now...","icons\infantry g hoplite icon 64");
			}
			case 2:
			{
				trQuestVarSet("gameOverNext", trTime() + 7);
				trQuestVarSet("boonUnlocked"+BOON_HEALTH_ATTACK, 1);
				trShowImageDialog(boonIcon(BOON_HEALTH_ATTACK), boonName(BOON_HEALTH_ATTACK));
			}
			case 4:
			{
				gadgetUnreal("ShowImageBox");
				trQuestVarSet("gameOverNext", trTime() + 7);
				if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") == 3) {
					trSoundPlayFN("default","1",-1,
						"Zenophobia: You have completed the quest. Return to singleplayer to read the final Runestone.", "icons\infantry g hoplite icon 64");
				} else {
					trSoundPlayFN("default.wav","1",-1,
						"Zenophobia: Rewards have been sent to your warehouse. You can access them by playing this map in singleplayer.",
						"icons\infantry g hoplite icon 64");
				}
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

rule white_tiger_boss
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	float amt = 0;
	float angle = 0;
	float dist = 0;
	bool hit = false;
	
	vector hitbox = vector(0,0,0);
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (trUnitAlive() == true) {
		
		trUnitSelectClear();
		trUnitSelect(""+bossUnit, true);
		
		if (trQuestVarGet("bossSpell") == BOSS_SPELL_COOLDOWN) {
			processBossCooldown();
		} else if (trQuestVarGet("bossSpell") > 30) {
			
		} else if (trQuestVarGet("bossSpell") > 20) {
			
		} else if (trQuestVarGet("bossSpell") > 10) {
			
		} else if (trQuestVarGet("bossSpell") > 0) {
			if (trQuestVarGet("bossSpell") == 1) {
				bossNext = trTimeMS() + 1000;
				trUnitSelectClear();
				trUnitSelect(""+bossUnit, true);
			}
		} else if (xGetInt(dEnemies, xStunStatus, bossPointer) == 0) {
			trQuestVarSetFromRand("bossSpell", 0, xsMin(3, trUnitPercentDamaged() * 0.05), true);
			trQuestVarSet("bossSpell", trQuestVarGet("bossSpell") * 10 + 1);
			if (trQuestVarGet("bossSpell") == 31 && trQuestVarGet("bossUltimate") > 0) {
				trQuestVarSetFromRand("bossSpell", 0, 2, true);
				trQuestVarSet("bossSpell", 1 + 10 * trQuestVarGet("bossSpell"));
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
		xsDisableSelf();
		xsDisableRule("boss_music");
		boss = 0;
	}
}
