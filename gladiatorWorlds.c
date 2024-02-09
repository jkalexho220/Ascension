int dPitOfDoom = 0;
int dLaplace = 0;
int dFermi = 0;
int dKepler = 0;
int dPillars = 0;
int dBigBang = 0;

int dMouthOfChaos = 0;

int dRevealerShots = 0;
int xRevealerShotPos = 0;

int dRevealerBoom = 0;
int xRevealerBoomTimeout = 0;

rule gladiator_worlds_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		dMouthOfChaos = xInitDatabase("mouthOfChaos");
		xInitAddInt(dMouthOfChaos, "name");

		dYeebFeathers = xInitDatabase("yeebFeathers");
		xInitAddInt(dYeebFeathers, "name");
		xInitAddInt(dYeebFeathers, "player");
		xYeebFeatherPos = xInitAddVector(dYeebFeathers, "pos");

		dRevealerShots = xInitDatabase("revealerShots");
		xInitAddInt(dRevealerShots, "name");
		xInitAddInt(dRevealerShots, "player");
		xRevealerShotPos = xInitAddVector(dRevealerShots, "pos");

		dRevealerBoom = xInitDatabase("revealerBooms");
		xInitAddInt(dRevealerBoom, "name");
		xInitAddInt(dRevealerBoom, "player");
		xRevealerBoomTimeout = xInitAddInt(dRevealerBoom, "timeout");

		dEdgeFrontier = xInitDatabase("edgeFrontier");
		xEdgeFrontierHeight = xInitAddInt(dEdgeFrontier, "height");
		xEdgeFrontierLoc = xInitAddVector(dEdgeFrontier, "location");

		trModifyProtounit("Revealer",1,2,999);
		trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Revealer");

		trVectorQuestVarSet("startPosition", vector(145,0,145));

		for(i=1; < 5) {
			trQuestVarSet("map"+i, i);
		}
		trQuestVarSetFromRand("rand", 1, 4, true);
		xsEnableRule("gladiator_worlds_build_"+1*trQuestVarGet("rand"));
		trQuestVarSet("map"+1*trQuestVarGet("rand"), 1);

		trQuestVarSet("gladiatorRound", 1);

		/* oracle */
		for(i=10; >0) {
			trTechSetStatus(ENEMY_PLAYER, 297, 4);
		}

		bullshitProj = trGetNextUnitScenarioNameNumber();

		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);

		xsEnableRule("gladiator_worlds_cin_1");

		trQuestVarSet("eyecandyStart", trGetNextUnitScenarioNameNumber());

		xsDisableSelf();
	}
}

rule gladiator_worlds_cin_1
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trMusicPlay("xpack\xcinematics\10_a\music.mp3", "1", 0);
				trSoundPlayFN("default","1",-1,"nottud:Welcome to Gladiator Worlds! The land of madness incarnate!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				
				if (trCurrentPlayer() == 1) {
					trQuestVarSet("nottudTicketsCount", trQuestVarGet("nottudTicketsCount") - 1);
				}
				trQuestVarSet("p1gladiatorWorlds", 0);
				
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"nottud:I see you have brought your heroes from Ascension. But even such powers are meaningless before my bullshit.","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 7);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"nottud:How long can you survive against my hordes? I'll give you precious gems for every 100 kills.","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"nottud:Good luck! You're going to need it!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 5:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gameplay_start");
				xsEnableRule("gladiator_worlds_portals");

				for(i=20; >0) {
					/* 100 health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				if (trCurrentPlayer() == 1) {
					uiMessageBox("Your copy of Gladiator Worlds was consumed. It's a one-time use item.");
				}

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
				
				trStringQuestVarSet("advice", "What do you mean you can't beat this? This is easy mode!");
			}
		}
	}
}

rule gladiator_worlds_cin_2
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trLetterBox(true);
				trSoundPlayFN("default","1",-1,"nottud:Wow! You've managed to survive the first wave! Well done!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"nottud:Let's up the ante now, shall we?","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 3);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"MODERATE DIFFICULTY:Enemies have 1.5X power","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"nottud:Good luck! You're going to need it!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 5:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gladiator_worlds_portals");
				xsEnableRule("gameplay_always");

				spawnPlayerCircle(trVectorQuestVarGet("startPosition"));

				for(i=20; >0) {
					/* 100 health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=2; >0) {
					/* 0.5 attack monstrous rage x4 */
					trTechSetStatus(ENEMY_PLAYER, 76, 4);
				}

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
				
				trStringQuestVarSet("advice", "Have you tried complaining to nottud about the difficulty? Because it doesn't work.");
			}
		}
	}
}

rule gladiator_worlds_cin_3
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trLetterBox(true);
				trSoundPlayFN("default","1",-1,"nottud:Color me impressed! You've managed to survive two rounds!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"nottud:But the show has only just begun!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 3);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"HARD DIFFICULTY:Enemies have 2X power","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"nottud:Good luck! You're going to need it!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 5:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gladiator_worlds_portals");
				xsEnableRule("gameplay_always");

				spawnPlayerCircle(trVectorQuestVarGet("startPosition"));

				for(i=20; >0) {
					/* 100 health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=2; >0) {
					/* 0.5 attack monstrous rage x4 */
					trTechSetStatus(ENEMY_PLAYER, 76, 4);
				}

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
				
				trStringQuestVarSet("advice", "Fun fact. I've never beaten Gladiator Worlds before. On any difficulty.");
			}
		}
	}
}

rule gladiator_worlds_cin_4
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trLetterBox(true);
				trSoundPlayFN("default","1",-1,"nottud:A true display of skill! I commend you for making it this far!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"nottud:But can you survive this next round?","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 3);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"TITAN DIFFICULTY:Enemies have 3X power","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"nottud:Good luck! You're going to need it!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 5:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gladiator_worlds_portals");
				xsEnableRule("gameplay_always");

				spawnPlayerCircle(trVectorQuestVarGet("startPosition"));

				for(i=40; >0) {
					/* 100 health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=4; >0) {
					/* 0.5 attack monstrous rage x4 */
					trTechSetStatus(ENEMY_PLAYER, 76, 4);
				}

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
				
				trStringQuestVarSet("advice", "You were so close! I believe in you!");
			}
		}
	}
}


rule gladiator_worlds_cin_5
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trLetterBox(true);
				trSoundPlayFN("default","1",-1,"nottud:At last, you have achieved the unfathomable. You are worthy.","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 4);
				xsEnableRule("gladiator_worlds_build_5");
				boss = 12;
				if (customContent) {
					xsEnableRule("boss_music");
				}
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"nottud:Now, let's finish this!","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 3:
			{
				trOverlayText("nottud", 6.0, -1, -1, -1);
				trSoundPlayFN("default","1",-1,"BOSS BATTLE:Enemies have ??? power","icons\special g minotaur icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 4:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);

				xsEnableRule("gameplay_always");

				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, 45000 * xsSqrt(ENEMY_PLAYER));

				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 45000 * xsSqrt(ENEMY_PLAYER));

				xsEnableRule("boss_stun_recovery");

				bossUnit = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Minotaur",1,155,0,155,225,true);
				activateEnemy(bossUnit);
				bossPointer = xGetNewestPointer(dEnemies);
				xSetBool(dEnemies, xLaunched, true, bossPointer);

				bossID = kbGetBlockID(""+bossUnit, true);

				xsEnableRule("boss12_battle");

				reselectMyself();

				trQuestVarSet("bossSpell", dPitOfDoom);
				trQuestVarSet("bossPhase", 2);
				trQuestVarSet("bossCooldownTime", trTimeMS() + 15000);

				spawnPlayerCircle(trVectorQuestVarGet("startPosition") - vector(10, 0, 10));

				if (customContent == false) {
					xsEnableRule("boss_music");
				}
				
				trStringQuestVarSet("advice", "Well, you tried. But mess with the bull and you get the bullshit.");
			}
		}
	}
}

rule gladiator_worlds_portals
inactive
highFrequency
{
	if (trTime() > cActivationTime + 3) {
		vector pos = vector(0,0,0);
		for(i=3; >0) {
			pos = trVectorQuestVarGet("startPosition") - bossDir;
			xAddDatabaseBlock(dMouthOfChaos, true);
			xSetInt(dMouthOfChaos, xUnitName, trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			if (xGetInt(dMouthOfChaos, xUnitName) == trGetNextUnitScenarioNameNumber()) {
				debugLog("Failed to create mouth of Chaos");
				xFreeDatabaseBlock(dMouthOfChaos);
			} else {
				xUnitSelect(dMouthOfChaos, xUnitName);
				trSetUnitOrientation(bossDir, vector(0,1,0), true);
				trUnitChangeProtoUnit("Tartarian Gate");
			}
			bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
		}

		trQuestVarSet("currentKills", trGetStatValue(ENEMY_PLAYER, 6));
		trQuestVarSet("totalKills", 0);
		trCounterAddTime("killcount",-1,-9999,"Kills: " + 1*trQuestVarGet("totalKills"),-1);

		xsEnableRule("gladiator_worlds_spawn");
		xsEnableRule("gladiator_worlds_always");
		xsDisableSelf();
	}
}

void processRevealerBoom() {
	int db = 0;
	int p = 0;
	vector pos = vector(0,0,0);
	if (xGetDatabaseCount(dRevealerBoom) > 0) {
		xDatabaseNext(dRevealerBoom);
		if (trTimeMS() > xGetInt(dRevealerBoom, xRevealerBoomTimeout)) {
			p = xGetInt(dRevealerBoom, xPlayerOwner);
			db = opponentDatabaseName(p);
			pos = kbGetBlockPosition(""+xGetInt(dRevealerBoom, xUnitName), true);
			xUnitSelect(dRevealerBoom, xUnitName);
			trUnitDestroy();
			for(i=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeOpponentUnit(p);
				} else if (unitDistanceToVector(xGetInt(db, xUnitName), pos) < 25.0) {
					damageOpponentUnit(p, 200.0 + 100.0 * trQuestVarGet("gladiatorRound"));
				}
			}
			xFreeDatabaseBlock(dRevealerBoom);
		}
	}
}

rule gladiator_worlds_always
inactive
highFrequency
{
	bool explode = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector end = vector(0,0,0);
	int target = 0;
	int next = 0;
	int id = 0;
	int p = 0;
	int db = 0;
	float dist = 0;
	float current = 0;
	bool hit = false;

	int kills = trGetStatValue(ENEMY_PLAYER, 6);
	if (kills > trQuestVarGet("currentKills")) {
		trQuestVarSet("totalKills", trQuestVarGet("totalKills") + kills - trQuestVarGet("currentKills"));
		trQuestVarSet("currentKills", kills);
		trCounterAbort("killcount");
		trCounterAddTime("killcount",-1,-9999,"Kills: " + 1*trQuestVarGet("totalKills"),-1);
	}

	if (trQuestVarGet("totalKills") >= 100) {
		if (xGetDatabaseCount(dEnemies) == 0) {
			xsDisableSelf();
			xsDisableRule("gameplay_always");
			trSoundPlayFN("cinematics\22_out\music (wussy mix).mp3","1",-1,"","");
			trUIFadeToColor(0,0,0,3000,0,true);
			xsEnableRule("gladiator_worlds_build_next");
			xsDisableRule("gladiator_worlds_spawn");


			trSoundPlayFN("favordump.wav","1",-1,"","");
			trQuestVarSetFromRand("gem", 0, 2, true);
			trShowImageDialog(gemstoneIcon(1*trQuestVarGet("gem")), gemstoneName(1*trQuestVarGet("gem")) + " x" + 1*trQuestVarGet("gladiatorRound"));
			trQuestVarSet("gemstone"+1*trQuestVarGet("gem"), 1 + trQuestVarGet("gemstone"+1*trQuestVarGet("gem")));
		}
	}

	if (xGetDatabaseCount(dEnemies) > 0) {
		id = xGetInt(dEnemies, xUnitID);
		if (kbUnitGetAnimationActionType(id) == 9) {
			xUnitSelectByID(dEnemies, xUnitID);
			xDatabaseNext(dPlayerCharacters);
			pos = kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName));
			trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,true);
		}
	}

	for (i=xsMin(9, xGetDatabaseCount(dYeebFeathers)); > 0) {
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
			spawnLightning(pos, xGetInt(dYeebFeathers, xPlayerOwner));
			xFreeDatabaseBlock(dYeebFeathers);
		}
	}

	if (xGetDatabaseCount(dRevealerShots) > 0) {
		xDatabaseNext(dRevealerShots);
		explode = false;
		id = kbGetBlockID(""+xGetInt(dRevealerShots, xUnitName), true);
		if (id == -1) {
			pos = xGetVector(dRevealerShots, xRevealerShotPos);
			explode = true;
		} else {
			pos = kbGetBlockPosition(""+xGetInt(dRevealerShots, xUnitName), true);
			xSetVector(dRevealerShots, xRevealerShotPos, pos);
			if (xsVectorGetY(pos) < worldHeight + 0.5) {
				explode = true;
			}
		}
		if (explode) {
			xUnitSelect(dRevealerShots, xUnitName);
			trUnitChangeProtoUnit("Tremor");
			next = trGetNextUnitScenarioNameNumber();
			xAddDatabaseBlock(dRevealerBoom, true);
			xSetInt(dRevealerBoom, xUnitName, next);
			xSetInt(dRevealerBoom, xPlayerOwner, xGetInt(dRevealerShots, xPlayerOwner));
			xSetInt(dRevealerBoom, xRevealerBoomTimeout, trTimeMS() + 1200);
			trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			trArmySelect("0,0");
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelect(""+next, true);
			trMutateSelected(kbGetProtoUnitID("Hades Door"));
			trSetSelectedScale(0,0,0);
			trUnitOverrideAnimation(25,0,false,false,-1);
			trUnitSetAnimationPath("3,0,0,0,0,0,0");
			trSetUnitOrientation(vector(0,1,0),vector(1,0,0),true);
			xFreeDatabaseBlock(dRevealerShots);
		}
	}

	processRevealerBoom();

	if (xGetDatabaseCount(dFireLancePellets) > 0) {
		for(i=xsMin(8, xGetDatabaseCount(dFireLancePellets)); >0) {
			xDatabaseNext(dFireLancePellets);
			if (xGetInt(dFireLancePellets, xFireLancePelletTimeout) == 0) {
				xSetInt(dFireLancePellets, xFireLancePelletTimeout, trTimeMS() + 1500);
				xUnitSelect(dFireLancePellets, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Thor Hammer"));
				trUnitHighlight(3.0, false);
				trUnitOverrideAnimation(2,0,true,false,-1);
				trSetSelectedScale(0.3,0.3,-0.2);
				trUnitSetAnimationPath("3,0,0,0,0,0,0");
			} else {
				dist = trTimeMS() - xGetInt(dFireLancePellets, xFireLancePelletLast);
				if (dist > 100) {
					hit = false;
					pos = xGetVector(dFireLancePellets, xFireLancePelletPrev);
					dir = xGetVector(dFireLancePellets, xFireLancePelletDir);
					p = xGetInt(dFireLancePellets, xPlayerOwner);
					dist = 0.03 * dist;
					db = opponentDatabaseName(p);
					for(j=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db, xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (rayCollision(db, pos, dir, dist + 1.0, 1.0)) {
							hit = true;
							damagePlayerUnit(40.0 + 20.0 * trQuestVarGet("gladiatorRound"));
						}
					}
					if (hit || trTimeMS() > xGetInt(dFireLancePellets, xFireLancePelletTimeout)) {
						xUnitSelect(dFireLancePellets, xUnitName);
						trUnitChangeProtoUnit("Dust Small");
						xUnitSelect(dFireLancePellets, xUnitName);
						trDamageUnitPercent(-100);
						xFreeDatabaseBlock(dFireLancePellets);
						if (hit) {
							trQuestVarSetFromRand("sound", 1, 4, true);
							trSoundPlayFN("arrowonflesh"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
						}
					} else {
						pos = pos + (dir * dist);
						xSetVector(dFireLancePellets, xFireLancePelletPrev, pos);
						xSetInt(dFireLancePellets, xFireLancePelletLast, trTimeMS());
					}
				}
			}
		}
	}

	if (xGetDatabaseCount(dFireLance) > 0) {
		hit = false;
		for(i=xGetDatabaseCount(dFireLance); >0) {
			xDatabaseNext(dFireLance);
			id = xGetInt(dFireLance, xUnitID);
			p = xGetInt(dFireLance, xPlayerOwner);
			trUnitSelectClear();
			trUnitSelectByID(id);
			if (trUnitAlive() == false) {
				xFreeDatabaseBlock(dFireLance);
			} else if (trTimeMS() > xGetInt(dFireLance, xSpecialNext)) {
				if (kbUnitGetAnimationActionType(id) == 12) {
					hit = true;
					xSetInt(dFireLance, xSpecialNext, trTimeMS() + 500);
					xsSetContextPlayer(p);
					target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
					xsSetContextPlayer(0);
					pos = kbGetBlockPosition(""+xGetInt(dFireLance, xUnitName), true);
					end = kbGetBlockPosition(""+target, true);
					dir = getUnitVector(pos, end);
					zSetProtoUnitStat("Kronny Flying", p, 1, 30.0);
					xAddDatabaseBlock(dFireLancePellets, true);
					xSetInt(dFireLancePellets, xUnitName, trGetNextUnitScenarioNameNumber());
					xSetInt(dFireLancePellets, xPlayerOwner, p);
					xSetVector(dFireLancePellets, xFireLancePelletDir, dir);
					xSetVector(dFireLancePellets, xFireLancePelletPrev, pos);
					xSetInt(dFireLancePellets, xFireLancePelletLast, trTimeMS());
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect(""+p+",0");
					trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trSetSelectedScale(0,-4.5,0);
					trDamageUnitPercent(100);
				} else {
					xSetInt(dFireLance, xSpecialNext, trTimeMS());
				}
			}
		}
		if (hit) {
			trSoundPlayFN("titanfall.wav","1",-1,"","");
		}
	}

	while (bullshitProj < trGetNextUnitScenarioNameNumber()) {
		id = kbGetBlockID(""+bullshitProj, true);
		p = kbUnitGetOwner(id);
		switch(kbGetUnitBaseTypeID(id))
		{
			case kbGetProtoUnitID("Stymph Bird Feather"):
			{
				if ((p == ENEMY_PLAYER) || (xGetInt(dPlayerData, xPlayerClass, p) != SPELLSTEALER)) {
					pos = kbGetBlockPosition(""+bullshitProj, true);
					xAddDatabaseBlock(dYeebFeathers, true);
					xSetInt(dYeebFeathers, xUnitName, bullshitProj);
					xSetInt(dYeebFeathers, xPlayerOwner, p);
					xSetVector(dYeebFeathers, xYeebFeatherPos, pos);
					trUnitSelectClear();
					trUnitSelectByID(id);
					trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
				}
			}
			case kbGetProtoUnitID("Catapult Shot"):
			{
				/* revealers */
				trUnitSelectClear();
				trUnitSelectByID(id);
				trMutateSelected(kbGetProtoUnitID("Revealer"));
				trUnitSetAnimationPath("1,0,0,0,0,0,0");
				xAddDatabaseBlock(dRevealerShots, true);
				xSetInt(dRevealerShots, xUnitName, bullshitProj);
				xSetInt(dRevealerShots, xPlayerOwner, p);
			}
			case kbGetProtoUnitID("Fire Lance Projectile"):
			{
				trUnitSelectClear();
				trUnitSelectByID(id);
				trUnitChangeProtoUnit("Dust Small");
			}
		}
		bullshitProj = bullshitProj + 1;
	}
	
}

rule gladiator_worlds_spawn
inactive
minInterval 3
{
	if (xGetDatabaseCount(dEnemies) < 30) {
		if (trQuestVarGet("totalKills") < 100) {
			vector pos = vector(0,0,0);
			int next = 0;
			xDatabaseNext(dMouthOfChaos);
			pos = kbGetBlockPosition(""+xGetInt(dMouthOfChaos, xUnitName), true);
			trQuestVarSetFromRand("rand", 5, 10);
			trQuestVarSetFromRand("proto", 1, 5, true);
			for(i=trQuestVarGet("rand"); >0) {
				trQuestVarSetFromRand("heading", 1, 360, true);
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("proto")),1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
				if (next < trGetNextUnitScenarioNameNumber()) {
					trQuestVarSetFromRand("relicdrop", 0, 1, false);
					if (trQuestVarGet("relicdrop") < 0.02) {
						trQuestVarSetFromRand("randrelic", 1, 20);
					} else {
						trQuestVarSet("randrelic", 0);
					}
					activateEnemy(next, -1, trQuestVarGet("randrelic"));
				}
			}
		}
	}
}

rule gladiator_worlds_build_next
inactive
highFrequency
{
	if (trTime() >= cActivationTime + 4) {
		trChangeTerrainHeight(0,0, 145,145, 10, true);
		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		trCounterAbort("killcount");
		xClearDatabase(dMouthOfChaos);
		for(i=trQuestVarGet("eyecandyStart"); < trGetNextUnitScenarioNameNumber()) {
			trUnitSelectClear();
			trUnitSelect(""+i, true);
			trUnitDestroy();
		}
		for(p=1; < ENEMY_PLAYER) {
			trPlayerKillAllGodPowers(p);
			xSetInt(dPlayerData, xPlayerDead, 0, p);
			xResetDatabase(getCharactersDB(p));
			if (trQuestVarGet("p"+p+"rideLightning") == 1) {
				trQuestVarSet("p"+p+"rideLightning", 0);
			}
			if (trQuestVarGet("p"+p+"nightfall") > 0) {
				trQuestVarSet("p"+p+"nightfall", 0);
			}
			xSetBool(dPlayerData, xPlayerSilenced, true, p);
		}

		trQuestVarSet("deadPlayerCount", 0);

		xResetDatabase(dPlayerUnits);
		xResetDatabase(dPlayerCharacters);
		xResetDatabase(dFreeRelics);

		trQuestVarSet("gladiatorRound", 1 + trQuestVarGet("gladiatorRound"));

		if (trQuestVarGet("gladiatorRound") < 5) {
			trQuestVarSetFromRand("rand", trQuestVarGet("gladiatorRound"), 4, true);
			xsEnableRule("gladiator_worlds_build_"+1*trQuestVarGet("map"+1*trQuestVarGet("rand")));
			trQuestVarSet("map"+1*trQuestVarGet("rand"), trQuestVarGet("map"+1*trQuestVarGet("gladiatorRound")));
		}

		xsEnableRule("gladiator_worlds_cin_"+1*trQuestVarGet("gladiatorRound"));
		trChangeTerrainHeight(0,0, 145,145, 0, false);

		trQuestVarSet("eyecandyStart", trGetNextUnitScenarioNameNumber());
		xsDisableSelf();
	}
}

void paintIsland(vector pos = vector(0,0,0)) {
	int x = 0;
	int z = 0;
	for(x=0; < 145) {
		for(z=0; < 145) {
			aiPlanSetUserVariableBool(dMapTiles, x, z, false);
		}
	}
	xClearDatabase(dEdgeFrontier);
	int pointer = 0;
	float height = 0;
	xAddDatabaseBlock(dEdgeFrontier, true);
	xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
	xSetInt(dEdgeFrontier, xEdgeFrontierHeight, 29);
	aiPlanSetUserVariableBool(dMapTiles, 1*xsVectorGetX(pos), 1*xsVectorGetZ(pos), true);
	while(xGetDatabaseCount(dEdgeFrontier) > 0) {
		trQuestVarSetFromRand("rand", 1, 8, true);
		for(i=trQuestVarGet("rand"); >0) {
			xDatabaseNext(dEdgeFrontier, false);
		}
		pointer = xGetPointer(dEdgeFrontier);
		pos = xGetVector(dEdgeFrontier, xEdgeFrontierLoc);
		x = xsVectorGetX(pos);
		z = xsVectorGetZ(pos);
		height = -1.0 + (xGetInt(dEdgeFrontier, xEdgeFrontierHeight) / 10);
		if (trGetTerrainHeight(x, z) < height || trGetTerrainHeight(x + 1, z + 1) < height) {
			trChangeTerrainHeight(x, z, 1 + x, 1 + z, height, false);
			for(a= -1; <= 1) {
				for (b= -1; <= 1) {
					if (xsAbs(a) + xsAbs(b) > 0) {
						pos = xsVectorSet(a + x, 0, b + z);
						if (x > 1 && x < 144 && z > 1 && z < 144) {
							if (aiPlanGetUserVariableBool(dMapTiles, x + a, z + b) == false) {
								trQuestVarSetFromRand("rand", 1, 3, true);
								height = xGetInt(dEdgeFrontier, xEdgeFrontierHeight, pointer) - trQuestVarGet("rand");
								xAddDatabaseBlock(dEdgeFrontier, true);
								xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
								xSetInt(dEdgeFrontier, xEdgeFrontierHeight, height);
								aiPlanSetUserVariableBool(dMapTiles, x + a, z + b, true);
							}
						}
					}
				}
			}
		}
		xFreeDatabaseBlock(dEdgeFrontier, pointer);
	}
}


rule gladiator_worlds_build_1
inactive
highFrequency
{
	trSetLighting("default", 0);
	xsDisableSelf();
	vector pos = vector(0,0,0);
	wallHeight = 9;

	TERRAIN_WALL = 2;
	TERRAIN_SUB_WALL = 1;
	
	TERRAIN_PRIMARY = 0;
	TERRAIN_SUB_PRIMARY = 5;
	
	TERRAIN_SECONDARY = 0;
	TERRAIN_SUB_SECONDARY = 3;

	trQuestVarSet("treeDensity", 0.5);
	trStringQuestVarSet("treeProto1", "Pine");
	trStringQuestVarSet("treeProto2", "Pine");
	trStringQuestVarSet("treeProto3", "Ruins");
	trQuestVarSet("spriteDensity", 1.0);
	trStringQuestVarSet("spriteProto1", "Flowers");
	trStringQuestVarSet("spriteProto2", "Grass");
	trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
	trQuestVarSet("rockDensity", 0.8);
	trStringQuestVarSet("rockProto1", "Rock Granite Big");
	trStringQuestVarSet("rockProto2", "Columns Broken");
	trStringQuestVarSet("rockProto3", "Rock Granite Small");

	trQuestVarSet("columnDensity", 0.5);

	trPaintTerrain(0, 0, 145, 145, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
	paintSecondary(0, 0, 145, 145);

	// cliff outer circle
	for(x=0; <= 145) {
		for(z=0; <= 145) {
			if (xsPow(x - 73, 2) + xsPow(z - 73, 2) > 900) { // 30 radius
				trPaintTerrain(x, z, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				trChangeTerrainHeight(x, z, x, z, wallHeight, false);
			}
		}
	}

	paintColumns(43, 43, 103, 103);
	paintEyecandy(43, 43, 103, 103, "tree");
	paintEyecandy(43, 43, 103, 103, "rock");
	paintEyecandy(43, 43, 103, 103, "sprite");
	
	trStringQuestVarSet("enemyProto1", "Petsuchos");
	trStringQuestVarSet("enemyProto2", "Mountain Giant");
	trStringQuestVarSet("enemyProto3", "Frost Giant");
	trStringQuestVarSet("enemyProto4", "Centaur");
	trStringQuestVarSet("enemyProto5", "Stymphalian Bird");

	trQuestVarSetFromRand("portalangle", 0, 3.14, false);
	bossDir = xsVectorSet(xsCos(trQuestVarGet("portalangle")), 0, xsSin(trQuestVarGet("portalangle"))) * 35.0;

	for(i=3; >0) {
		pos = vectorToGrid(trVectorQuestVarGet("startPosition") + bossDir);
		trPaintTerrain(xsVectorGetX(pos) - 3, xsVectorGetZ(pos) - 3, xsVectorGetX(pos) + 3, xsVectorGetZ(pos) + 3, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
		trChangeTerrainHeight(xsVectorGetX(pos) - 3, xsVectorGetZ(pos) - 3, xsVectorGetX(pos) + 3, xsVectorGetZ(pos) + 3, worldHeight, false);
		bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
	}
	bossDir = vector(0,0,0) - bossDir;

	trPaintTerrain(0,0,0,0,TERRAIN_WALL,TERRAIN_SUB_WALL,true);

	trShowImageDialog("ui\ui map king of the hill 256x256", "Entering bullshit canyon...");
	gadgetUnreal("ShowImageBox-CloseButton");
}

rule gladiator_worlds_build_2
inactive
highFrequency
{
	trSetLighting("default", 0);
	float height = 0;

	TERRAIN_WALL = 4;
	TERRAIN_SUB_WALL = 1;

	TERRAIN_PRIMARY = 4;
	TERRAIN_SUB_PRIMARY = 3;

	TERRAIN_SECONDARY = 4;
	TERRAIN_SUB_SECONDARY = 2;

	worldHeight = 0;

	trCoverTerrainWithWater(0, 3.0, "Aegean Sea");

	dMapTiles = aiPlanCreate("mapTiles", 8);
	for(i=0; < 145) {
		if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
			debugLog("Cannot create new user variable at " + i);
		}
	}

	paintIsland(vector(72,0,72));

	trQuestVarSetFromRand("portalangle", 0, 3.14, false);
	bossDir = xsVectorSet(xsCos(trQuestVarGet("portalangle")), 0, xsSin(trQuestVarGet("portalangle"))) * 35.0;

	for(i=3; >0) {
		paintIsland(vectorToGrid(trVectorQuestVarGet("startPosition") + bossDir));
		bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
	}
	bossDir = vector(0,0,0) - bossDir;

	subModeEnter("Simulation", "Editor");
	terrainFilter();
	terrainFilter();
	subModeLeave("Simulation", "Editor");
	modeEnter("Pregame");
	modeEnter("Simulation");

	aiPlanDestroy(dMapTiles);

	for(x=0; < 145) {
		for(z=0; < 145) {
			height = 0.5 * (trGetTerrainHeight(x, z) + trGetTerrainHeight(x+1, z+1));
			if (height > -0.2) {
				trPaintTerrain(x, z, x, z, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			} else if (height < -1.0) {
				trPaintTerrain(x, z, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			} else {
				trPaintTerrain(x, z, x, z, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
			}
		}
	}

	trQuestVarSet("treeDensity", 0.3);
	trStringQuestVarSet("treeProto1", "Palm");
	trStringQuestVarSet("treeProto2", "Palm");
	trStringQuestVarSet("treeProto3", "Savannah Tree");
	trQuestVarSet("spriteDensity", 1.0);
	trStringQuestVarSet("spriteProto1", "Grass");
	trStringQuestVarSet("spriteProto2", "Grass");
	trStringQuestVarSet("spriteProto3", "Rock Sandstone Sprite");
	trQuestVarSet("rockDensity", 0.6);
	trStringQuestVarSet("rockProto1", "Rock River Sandy");
	trStringQuestVarSet("rockProto2", "Bush");
	trStringQuestVarSet("rockProto3", "Water Reeds");

	trPaintTerrain(0, 0, 1, 1, TERRAIN_WALL, TERRAIN_SUB_WALL, true);

	paintEyecandy(43, 43, 103, 103, "tree");
	paintEyecandy(43, 43, 103, 103, "rock");
	paintEyecandy(43, 43, 103, 103, "sprite");


	trStringQuestVarSet("enemyProto1", "War Salamander");
	trStringQuestVarSet("enemyProto2", "Hydra");
	trStringQuestVarSet("enemyProto3", "Medusa");
	trStringQuestVarSet("enemyProto4", "Nereid");
	trStringQuestVarSet("enemyProto5", "Catapult");

	trShowImageDialog("ui\ui map old atlantis 256x256", "Entering bullshit island...");
	gadgetUnreal("ShowImageBox-CloseButton");

	xsDisableSelf();
}

void paintSwampPool(vector pos = vector(0,0,0), int db = 0) {
	int x = 0;
	int z = 0;
	for(x=0; < 145) {
		for(z=0; < 145) {
			aiPlanSetUserVariableBool(dMapTiles, x, z, false);
		}
	}
	xClearDatabase(dEdgeFrontier);
	int pointer = 0;
	float height = 0;
	xAddDatabaseBlock(dEdgeFrontier, true);
	xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
	xSetInt(dEdgeFrontier, xEdgeFrontierHeight, 41);
	aiPlanSetUserVariableBool(dMapTiles, 1*xsVectorGetX(pos), 1*xsVectorGetZ(pos), true);
	while(xGetDatabaseCount(dEdgeFrontier) > 0) {
		trQuestVarSetFromRand("rand", 1, 2, true);
		for(i=trQuestVarGet("rand"); >0) {
			xDatabaseNext(dEdgeFrontier, false);
		}
		pointer = xGetPointer(dEdgeFrontier);
		pos = xGetVector(dEdgeFrontier, xEdgeFrontierLoc);
		x = xsVectorGetX(pos);
		z = xsVectorGetZ(pos);
		height = 1.0 - (xGetInt(dEdgeFrontier, xEdgeFrontierHeight) / 10);
		if (aiPlanGetUserVariableFloat(db, x, z) > height) {
			aiPlanSetUserVariableFloat(db, x, z, height);
			if (height < 1.0) {
				for(a= -1; <= 1) {
					for (b= -1; <= 1) {
						if (xsAbs(a) + xsAbs(b) > 0) {
							if (x > 1 && x < 144 && z > 1 && z < 144) {
								if (aiPlanGetUserVariableBool(dMapTiles, x + a, z + b) == false) {
									pos = xsVectorSet(a + x, 0, b + z);
									trQuestVarSetFromRand("rand", 1, 4, true);
									height = xGetInt(dEdgeFrontier, xEdgeFrontierHeight, pointer) - trQuestVarGet("rand");
									xAddDatabaseBlock(dEdgeFrontier, true);
									xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
									xSetInt(dEdgeFrontier, xEdgeFrontierHeight, height);
									aiPlanSetUserVariableBool(dMapTiles, x + a, z + b, true);
								}
							}
						}
					}
				}
			}
		}
		xFreeDatabaseBlock(dEdgeFrontier, pointer);
	}
}

rule gladiator_worlds_build_3
inactive
highFrequency
{
	float height = 0;

	trSetLighting("eclipse", 0);

	TERRAIN_WALL = 4;
	TERRAIN_SUB_WALL = 38;

	TERRAIN_PRIMARY = 0;
	TERRAIN_SUB_PRIMARY = 58;

	TERRAIN_SECONDARY = 4;
	TERRAIN_SUB_SECONDARY = 40;

	worldHeight = 0;

	trCoverTerrainWithWater(0, 3.0, "Marsh Pool");
	/* circular island */
	int terrainHeights = aiPlanCreate("terrainHeights", 8);
	for(i=0; < 145) {
		aiPlanAddUserVariableFloat(terrainHeights, i, "row"+i, 145);
		for(j=0; < 145) {
			if (xsPow(i - 73, 2) + xsPow(j - 73, 2) > 900.0) {
				if (aiPlanSetUserVariableFloat(terrainHeights, i, j, -3.0) == false) {
					debugLog("Could not set terrain height for " + i + ", " + j);
				}
			} else {
				aiPlanSetUserVariableFloat(terrainHeights, i, j, 1.0);
			}
		}
	}

	dMapTiles = aiPlanCreate("mapTiles", 8);
	for(i=0; < 145) {
		if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
			debugLog("Cannot create new user variable at " + i);
		}
	}

	trQuestVarSetFromRand("portalangle", 0, 3.14, false);
	bossDir = xsVectorSet(xsCos(trQuestVarGet("portalangle")), 0, xsSin(trQuestVarGet("portalangle"))) * 40.0;

	for(i=3; >0) {
		paintSwampPool(vectorToGrid(trVectorQuestVarGet("startPosition") + bossDir), terrainHeights);
		bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
	}

	for(i=0; < 145) {
		for(j=0; < 145) {
			trChangeTerrainHeight(i, j, i + 1, j + 1, aiPlanGetUserVariableFloat(terrainHeights, i, j), false);
		}
	}

	subModeEnter("Simulation", "Editor");
	terrainFilter();
	terrainFilter();
	subModeLeave("Simulation", "Editor");
	modeEnter("Pregame");
	modeEnter("Simulation");

	aiPlanDestroy(dMapTiles);
	aiPlanDestroy(terrainHeights);

	for(x=0; < 145) {
		for(z=0; < 145) {
			height = 0.5 * (trGetTerrainHeight(x, z) + trGetTerrainHeight(x+1, z+1));
			if (height > -0.5) {
				trPaintTerrain(x, z, x, z, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			} else if (height < -1.5) {
				trPaintTerrain(x, z, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			} else {
				trPaintTerrain(x, z, x, z, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
			}
		}
	}


	
	trQuestVarSet("treeDensity", 0.3);
	trStringQuestVarSet("treeProto1", "Bamboo Tree");
	trStringQuestVarSet("treeProto2", "Bamboo Tree");
	trStringQuestVarSet("treeProto3", "Marsh Tree");
	trQuestVarSet("spriteDensity", 1.0);
	trStringQuestVarSet("spriteProto1", "Grass");
	trStringQuestVarSet("spriteProto2", "Water Reeds");
	trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
	trQuestVarSet("rockDensity", 0.5);
	trStringQuestVarSet("rockProto1", "Rock Granite Small");
	trStringQuestVarSet("rockProto2", "Bush");
	trStringQuestVarSet("rockProto3", "Water Reeds");

	trPaintTerrain(0, 0, 1, 1, TERRAIN_WALL, TERRAIN_SUB_WALL, true);

	paintEyecandy(43, 43, 103, 103, "tree");
	paintEyecandy(43, 43, 103, 103, "rock");
	paintEyecandy(43, 43, 103, 103, "sprite");

	trStringQuestVarSet("enemyProto1", "Wadjet");
	trStringQuestVarSet("enemyProto2", "Apep");
	trStringQuestVarSet("enemyProto3", "Argus");
	trStringQuestVarSet("enemyProto4", "Jiangshi");
	trStringQuestVarSet("enemyProto5", "Fire Lance");

	trShowImageDialog("ui\ui map marsh 256x256", "Entering bullshit swamp...");
	gadgetUnreal("ShowImageBox-CloseButton");	

	xsDisableSelf();
}

void paintCliff(vector pos = vector(0,0,0)) {
	int x = 0;
	int z = 0;
	for(x=0; < 145) {
		for(z=0; < 145) {
			aiPlanSetUserVariableBool(dMapTiles, x, z, false);
		}
	}
	xClearDatabase(dEdgeFrontier);
	int pointer = 0;
	float height = 0;
	xAddDatabaseBlock(dEdgeFrontier, true);
	xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
	xSetInt(dEdgeFrontier, xEdgeFrontierHeight, 30);
	aiPlanSetUserVariableBool(dMapTiles, 1*xsVectorGetX(pos), 1*xsVectorGetZ(pos), true);
	while(xGetDatabaseCount(dEdgeFrontier) > 0) {
		trQuestVarSetFromRand("rand", 1, 8, true);
		for(i=trQuestVarGet("rand"); >0) {
			xDatabaseNext(dEdgeFrontier, false);
		}
		pointer = xGetPointer(dEdgeFrontier);
		pos = xGetVector(dEdgeFrontier, xEdgeFrontierLoc);
		x = xsVectorGetX(pos);
		z = xsVectorGetZ(pos);
		height = worldHeight + 3.0 * (xGetInt(dEdgeFrontier, xEdgeFrontierHeight) / 10);
		if (trGetTerrainHeight(x, z) < height || trGetTerrainHeight(x + 1, z + 1) < height) {
			trChangeTerrainHeight(x, z, 1 + x, 1 + z, height, false);
			for(a= -1; <= 1) {
				for (b= -1; <= 1) {
					if (xsAbs(a) + xsAbs(b) > 0) {
						pos = xsVectorSet(a + x, 0, b + z);
						if (x > 1 && x < 144 && z > 1 && z < 144) {
							if (aiPlanGetUserVariableBool(dMapTiles, x + a, z + b) == false) {
								trQuestVarSetFromRand("rand", 1, 5, true);
								height = xGetInt(dEdgeFrontier, xEdgeFrontierHeight, pointer) - trQuestVarGet("rand");
								xAddDatabaseBlock(dEdgeFrontier, true);
								xSetVector(dEdgeFrontier, xEdgeFrontierLoc, pos);
								xSetInt(dEdgeFrontier, xEdgeFrontierHeight, height);
								aiPlanSetUserVariableBool(dMapTiles, x + a, z + b, true);
							}
						}
					}
				}
			}
		}
		xFreeDatabaseBlock(dEdgeFrontier, pointer);
	}
}

rule gladiator_worlds_build_4
inactive
highFrequency
{
	trSetLighting("default", 0);
	float height = 0;

	TERRAIN_WALL = 2;
	TERRAIN_SUB_WALL = 2;

	TERRAIN_PRIMARY = 0;
	TERRAIN_SUB_PRIMARY = 34;

	TERRAIN_SECONDARY = 0;
	TERRAIN_SUB_SECONDARY = 38;

	worldHeight = 0;

	/* circular island */
	for(i=0; < 145) {
		for(j=0; < 145) {
			if (xsPow(i - 73, 2) + xsPow(j - 73, 2) > 900.0) {
				trChangeTerrainHeight(i, j, i+1, j+1, 9.0, false);
			} else {
				trChangeTerrainHeight(i, j, i+1, j+1, 0, false);
			}
		}
	}

	dMapTiles = aiPlanCreate("mapTiles", 8);
	for(i=0; < 145) {
		if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
			debugLog("Cannot create new user variable at " + i);
		}
	}

	trQuestVarSetFromRand("portalangle", 0, 3.14, false);
	bossDir = xsVectorSet(xsCos(trQuestVarGet("portalangle")), 0, xsSin(trQuestVarGet("portalangle"))) * 40.0;

	for(i=3; >0) {
		paintCliff(vectorToGrid(trVectorQuestVarGet("startPosition") + bossDir));
		bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
	}


	subModeEnter("Simulation", "Editor");
	terrainFilter();
	subModeLeave("Simulation", "Editor");
	modeEnter("Pregame");
	modeEnter("Simulation");

	aiPlanDestroy(dMapTiles);

	for(x=0; < 145) {
		for(z=0; < 145) {
			height = 0.5 * (trGetTerrainHeight(x, z) + trGetTerrainHeight(x+1, z+1));
			if (height < 0.5) {
				trPaintTerrain(x, z, x, z, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			} else if (height < 1.0) {
				trPaintTerrain(x, z, x, z, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
			} else {
				trPaintTerrain(x, z, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			}
		}
	}
	
	trQuestVarSet("treeDensity", 0.3);
	trStringQuestVarSet("treeProto1", "Statue Pharaoh");
	trStringQuestVarSet("treeProto2", "Statue Pharaoh");
	trStringQuestVarSet("treeProto3", "Palm");
	trQuestVarSet("spriteDensity", 1.0);
	trStringQuestVarSet("spriteProto1", "Grass");
	trStringQuestVarSet("spriteProto2", "Water Reeds");
	trStringQuestVarSet("spriteProto3", "Rock Sandstone Sprite");
	trQuestVarSet("rockDensity", 0.5);
	trStringQuestVarSet("rockProto1", "Rock River Sandy");
	trStringQuestVarSet("rockProto2", "Bush");
	trStringQuestVarSet("rockProto3", "Rock Sandstone Big");

	trPaintTerrain(0, 0, 1, 1, TERRAIN_WALL, TERRAIN_SUB_WALL, true);

	paintEyecandy(43, 43, 103, 103, "tree");
	paintEyecandy(43, 43, 103, 103, "rock");
	paintEyecandy(43, 43, 103, 103, "sprite");

	trStringQuestVarSet("enemyProto1", "Tartarian Gate Spawn");
	trStringQuestVarSet("enemyProto2", "Avenger");
	trStringQuestVarSet("enemyProto3", "Sphinx");
	trStringQuestVarSet("enemyProto4", "Troll");
	trStringQuestVarSet("enemyProto5", "Mummy");

	trShowImageDialog("ui\ui map valley of kings 256x256", "Entering bullshit desert...");
	gadgetUnreal("ShowImageBox-CloseButton");

	xsDisableSelf();
}

// 60 x 60 square with (72,72) at the center

void paintMapTile(int x = 0, int z = 0, int db = 0) {
	vector data = aiPlanGetUserVariableVector(db, x - 42, z - 42);
	trPaintTerrain(x, z, x, z, 1*xsVectorGetX(data), 1*xsVectorGetY(data), false);
	trChangeTerrainHeight(x, z, x, z, xsVectorGetZ(data), false);
}

void paintMapOutside() {
	trPaintTerrain(0, 0, 144, 42, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
	trChangeTerrainHeight(0, 0, 145, 43, wallHeight, false);

	trPaintTerrain(0, 103, 144, 144, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
	trChangeTerrainHeight(0, 103, 145, 145, wallHeight, false);

	trPaintTerrain(0, 43, 43, 103, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
	trChangeTerrainHeight(0, 43, 44, 103, wallHeight, false);

	trPaintTerrain(103, 43, 144, 103, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
	trChangeTerrainHeight(103, 43, 145, 104, wallHeight, false);
}

void paintMapInstant(int db = 0) {
	for(x=42; < 103) {
		for(z=42; < 103) {
			paintMapTile(x, z, db);
		}
	}
	paintMapOutside();
}

void paintPillar(vector pos = vector(0,0,0), int db = 0, int primary = 0, int secondary = 0) {
	xClearDatabase(dEdgeFrontier);
	int x = 0;
	int z = 0;
	for(x=0; < 145) {
		for(z=0; < 145) {
			aiPlanSetUserVariableBool(dMapTiles, x, z, false);
		}
	}

	vector data = vector(0,0,0);
	vector dir = vector(1,0,0);
	vector temp = vector(1,0,0);
	x = xsVectorGetX(pos);
	z = xsVectorGetZ(pos);

	aiPlanSetUserVariableBool(dMapTiles, x, z, true);
	aiPlanSetUserVariableVector(db, x - 42, z - 42, xsVectorSet(primary, secondary, 15.0));
	for(i=14; >= -3) {
		temp = rotationMatrix(dir, 0, 1.0);
		if (aiPlanGetUserVariableBool(dMapTiles, x + xsVectorGetX(temp), z + xsVectorGetZ(temp)) == false) {
			dir = temp;
		}
		pos = pos + dir;
		x = xsVectorGetX(pos);
		z = xsVectorGetZ(pos);
		aiPlanSetUserVariableBool(dMapTiles, x, z, true);
		aiPlanSetUserVariableVector(db, x - 42, z - 42, xsVectorSet(primary, secondary, i));
		data = aiPlanGetUserVariableVector(db, x - 43, z - 43);
		data = xsVectorSet(primary, secondary, xsVectorGetZ(data));
		aiPlanSetUserVariableVector(db, x - 43, z - 43, data);
	}
	xClearDatabase(dEdgeFrontier);
}

void transitionRing(int dist = 0, int db = 0) {
	if (dist < 31) {
		for(i = 0 - dist; <= dist) {
			paintMapTile(72 - dist, 72 + i, db);
			paintMapTile(72 + dist, 72 + i, db);
			paintMapTile(72 + i, 72 - dist, db);
			paintMapTile(72 + i, 72 + dist, db);
		}
	}
}

float physicsSpeed = 1;
int lastTime = 0;

int physicsBallPointer = 0;
int dPhysicsBalls = 0;
int xPhysicsBallPos = 0;
int xPhysicsBallDir = 0;
int xPhysicsBallPrev = 0;
int xPhysicsBallOut = 0;

int dLaplaceLaser = 0;
int xLaplaceLaserDir = 0;
int xLaplaceLaserPos = 0;

vector keplerPos = vector(0,0,0);
vector keplerDir = vector(0,0,0);

void processPhysicsBall(int timediff = 0, float speed = 1.0, bool hitbox = false) {
	xDatabaseNext(dPhysicsBalls);
	vector pos = xGetVector(dPhysicsBalls, xPhysicsBallPos) + (xGetVector(dPhysicsBalls, xPhysicsBallDir) * 0.001 * speed * timediff);
	xSetVector(dPhysicsBalls, xPhysicsBallPos, pos);
	vector dir = (pos - vector(145,0,145)) * 3.33;

	xUnitSelectByID(dPhysicsBalls, xUnitID);
	trSetSelectedUpVector(xsVectorGetX(dir), 0.2, xsVectorGetZ(dir));

	if (hitbox) {
		float dist = distanceBetweenVectors(pos, xGetVector(dPhysicsBalls, xPhysicsBallPrev), false) + 1.0;
		dir = xsVectorNormalize(pos - xGetVector(dPhysicsBalls, xPhysicsBallPrev));
		pos = xGetVector(dPhysicsBalls, xPhysicsBallPrev);
		for(i=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			xUnitSelectByID(dPlayerUnits, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerUnit();
			} else if (rayCollision(dPlayerUnits, pos, dir, dist, 1.0)) {
				trQuestVarSet("sound", 1);
				damagePlayerUnit(200.0);
			}
		}
		xSetVector(dPhysicsBalls, xPhysicsBallPrev, xGetVector(dPhysicsBalls, xPhysicsBallPos));
	}
}

rule gladiator_worlds_build_5
inactive
highFrequency
{
	dDragonFireballs = initGenericProj("dragonFireballs",kbGetProtoUnitID("Fire Giant"),19,8,4.5,0,ENEMY_PLAYER,true);
	xDragonFireballRemove = xInitAddBool(dDragonFireballs, "rotate", true);

	dValkyrieProj = initGenericProj("valkyrieProj",kbGetProtoUnitID("Valkyrie"),15,5,4.5,0,0);

	dShadeBolts = initGenericProj("shadeBolts",kbGetProtoUnitID("Lampades"),18,8,4.5,0,1,true);
	xInitAddBool(dShadeBolts, "rotate", true);

	dBossHeals = initGenericProj("bossHeals", kbGetProtoUnitID("Curse SFX"),2,6.0,4.5,0.0,0,true);

	dLionShockwaves = initGenericProj("lionShockwaves",kbGetProtoUnitID("Heka Shockwave SFX"),2,15.0,4.0);

	trSetLighting("hades", 0);
	TERRAIN_WALL = 2;
	TERRAIN_SUB_WALL = 12;

	TERRAIN_SECONDARY = 5;
	TERRAIN_SUB_SECONDARY = 7;

	TERRAIN_PRIMARY = 5;
	TERRAIN_SUB_PRIMARY = 5;

	dPhysicsBalls = xInitDatabase("physicsBalls", 32);
	xPhysicsBallPos = xInitAddVector(dPhysicsBalls, "pos");
	xPhysicsBallDir = xInitAddVector(dPhysicsBalls, "dir");
	xInitAddInt(dPhysicsBalls, "id");
	xPhysicsBallPrev = xInitAddVector(dPhysicsBalls, "prev");
	xPhysicsBallOut = xInitAddBool(dPhysicsBalls, "out", false);

	dLaplaceLaser = xInitDatabase("laplaceLasers", 32);
	xLaplaceLaserPos = xInitAddVector(dLaplaceLaser, "pos");
	xLaplaceLaserDir = xInitAddVector(dLaplaceLaser, "dir");
	xInitAddInt(dLaplaceLaser, "id");

	float dist = 0;
	int terrain = 0;
	int next = 0;
	vector dir = vector(0,0,0);
	vector data = vector(0,0,0);
	worldHeight = 6;
	wallHeight = -3;

	trChangeTerrainHeight(140, 140, 150, 150, -3, false);
	trQuestVarSet("keplerStart", trGetNextUnitScenarioNameNumber());
	for(i=10; >0) {
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Spy Eye");
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
	}
	trQuestVarSet("keplerEnd", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
	trArmySelect("1,0");
	trUnitChangeProtoUnit("Spy Eye");
	trUnitSelectClear();
	trUnitSelectByQV("keplerEnd");
	trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
	trSetSelectedScale(0,1,0);
	trSetUnitOrientation(vector(1,0,0),vector(0,-1,0),true);
	bossCount = trQuestVarGet("keplerStart");
	bossNext = trTimeMS();
	xsEnableRule("kepler_offset");

	dMapTiles = aiPlanCreate("mapTiles", 8);
	for(i=0; < 145) {
		if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
			debugLog("Cannot create new user variable at " + i);
		}
	}

	// pit of doom
	dPitOfDoom = aiPlanCreate("pitOfDoom", 8);

	trPaintTerrain(0, 0, 144, 144, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
	trChangeTerrainHeight(0, 0, 144, 144, -3, false);

	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dPitOfDoom, x, "row"+x, 61);
		for(z=0; <= 60) {
			dist = xsPow(x - 30, 2) + xsPow(z - 30, 2);
			if (dist >= 144) {
				data = xsVectorSet(TERRAIN_WALL, TERRAIN_SUB_WALL, -3);
				trChangeTerrainHeight(x + 42, z + 42, x+43, z+43, -3, false);
			} else {
				data = xsVectorSet(TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, 6);
				trChangeTerrainHeight(x + 42, z + 42, x+43, z+43, 6, false);
			}
			aiPlanSetUserVariableVector(dPitOfDoom, x, z, data);
			trPaintTerrain(x + 42, z + 42, x + 42, z + 42, 1*xsVectorGetX(data), 1*xsVectorGetY(data), false);
			trChangeTerrainHeight(x + 42, z + 42, x + 43, z + 43, xsVectorGetZ(data), false);
		}
	}

	trPaintTerrain(0,0,0,0,TERRAIN_WALL, TERRAIN_SUB_WALL, true);

	for(x=55; <= 91) {
		for(z=55; <= 91) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
			if (dist > 144 && dist <= 289) {
				aiPlanSetUserVariableVector(dPitOfDoom, x - 42, z - 42, xsVectorSet(TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, -3));
				trPaintTerrain(x, z, x, z, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
			}
		}
	}

	for(i=32; >0) {
		// physics balls
		xAddDatabaseBlock(dPhysicsBalls, true);
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("0,0","Dwarf",1,145,0,145,0,true);
		if (next == trGetNextUnitScenarioNameNumber()) {
			debugLog("Physics ball failed to create.");
		} else {
			xSetInt(dPhysicsBalls, xUnitID, kbGetBlockID(""+next, true));
			trArmySelect("0,0");
			trUnitChangeProtoUnit("Spy Eye");
			xUnitSelectByID(dPhysicsBalls, xUnitID);
			trMutateSelected(kbGetProtoUnitID("Outpost"));
			trSetSelectedScale(0,0,0);
			trSetSelectedUpVector(0, -10, 0);
		}
		// laplace lasers
		xAddDatabaseBlock(dLaplaceLaser, true);
		next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
		if (next == trGetNextUnitScenarioNameNumber()) {
			debugLog("Laplace Laser failed to create.");
		} else {
			xSetInt(dLaplaceLaser, xUnitID, kbGetBlockID(""+next, true));
			trArmySelect("1,0");
			trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
			trSetSelectedScale(0,0,0);
		}
	}

	dPillars = aiPlanCreate("pillarsOfCreation", 8);
	// pillars of creation
	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dPillars, x, "row"+x, 61);
		for(z=0; <= 60) {
			dist = xsPow(x - 30, 2) + xsPow(z - 30, 2);
			if (dist > 144) {
				data = xsVectorSet(3, 9, -3); // coral A
			} else {
				data = xsVectorSet(5, 0, -3); // ice A
			}
			aiPlanSetUserVariableVector(dPillars, x, z, data);
		}
	}

	dir = vector(16.970568, 0, 16.970568); // distance of 32 in coordinates
	trVectorQuestVarSet("pillar1", vectorSnapToGrid(vector(145,0,145) + (dir * 0.8)));
	paintPillar(vectorToGrid(vector(145,0,145) + dir), dPillars, 2, 10); // hades 7

	dir = rotationMatrix(dir, -0.5, 0.866025);
	trVectorQuestVarSet("pillar2", vectorSnapToGrid(vector(145,0,145) + (dir * 0.8)));
	paintPillar(vectorToGrid(vector(145,0,145) + dir), dPillars, 0, 8); // gaia creep

	dir = rotationMatrix(dir, -0.5, 0.866025);
	trVectorQuestVarSet("pillar3", vectorSnapToGrid(vector(145,0,145) + (dir * 0.8)));
	paintPillar(vectorToGrid(vector(145,0,145) + dir), dPillars, 3, 10); // coral A

	dKepler = aiPlanCreate("kepler", 8);
	// gravitational pull field
	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dKepler, x, "row"+x, 61);
		for(z=0; <= 60) {
			dist = xsPow(x - 30, 2) + xsPow(z - 30, 2);
			if (dist > 144) {
				dist = xsSqrt(dist) - 12.0;
				if (dist > 5.0) {
					dist = 18;
				} else {
					dist = -0.72 * xsPow(dist - 5.0, 2) + 18.0;
				}
				data = xsVectorSet(0, 73, dist - 3); // black rock
			} else {
				data = xsVectorSet(5, 4, -3); // black
			}
			aiPlanSetUserVariableVector(dKepler, x, z, data);
		}
	}

	dFermi = aiPlanCreate("Fermi", 8);

	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dFermi, x, "row"+x, 61);
		for(z=0; <= 60) {
			aiPlanSetUserVariableVector(dFermi, x, z, xsVectorSet(0, 50, 15));
		}
	}

	for(x=0; <= 10) {
		for(z=0; <= 10) {
			dist = x + z - 5;
			if (dist < 0) {
				data = xsVectorSet(0, 53, -3);
			} else {
				data = xsVectorSet(0, 50, xsMin(15.0, 3.0 * dist));
			}
			for(a = -1; <= 1) {
				for(b = -1; <= 1) {
					aiPlanSetUserVariableVector(dFermi, 30 + 3*x + a, 30 + 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 30 + 3*x + a, 30 - 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 30 - 3*x + a, 30 + 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 30 - 3*x + a, 30 - 3*z + b, data);
				}
			}
		}
	}

	dLaplace = aiPlanCreate("Laplace", 8);
	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dLaplace, x, "row"+x, 61);
		for(z=0; <= 60) {
			dist = xsPow(x - 30, 2) + xsPow(z - 30, 2);
			if (dist > 144) {
				if (dist <= 30.0) {
					dist = -8.0 * xsSin(0.349 * (xsSqrt(dist) - 12.0));
				} else {
					dist = 6.0;
				}
				data = xsVectorSet(0, 37, dist + 6.0); // sand d
			} else {
				data = xsVectorSet(0, 53, 6); // olympus tile
			}
			aiPlanSetUserVariableVector(dLaplace, x, z, data);
		}
	}

	dBigBang = aiPlanCreate("bigBang", 8);
	for(x=0; <= 60) {
		aiPlanAddUserVariableVector(dBigBang, x, "row"+x, 61);
		for(z=0; <= 60) {
			dist = xsPow(x - 30, 2) + xsPow(z - 30, 2);
			if (dist > 144) {
				data = xsVectorSet(2, 13, 3.0); // black rock
			} else {
				data = xsVectorSet(5, 4, 3.0); // black
			}
			aiPlanSetUserVariableVector(dBigBang, x, z, data);
		}
	}

	paintMapInstant(dPitOfDoom);

	aiPlanDestroy(dMapTiles);

	trQuestVarSet("nottud0", dPitOfDoom);
	trQuestVarSet("nottud1", dPillars);
	trQuestVarSet("nottud2", dKepler);
	trQuestVarSet("nottud3", dLaplace);
	trQuestVarSet("nottud4", dFermi);
	trQuestVarSet("nottud5", dBigBang);

	xsDisableSelf();
}


void fermiFilterActivate() {
	xsEnableRule("fermi_filter_on");
	trQuestVarSet("fermiFilterNext", trTimeMS() + 3000);
	trSoundPlayFN("attackwarning.wav","1",-1,"","");
	trSoundPlayFN("cinematics\32_out\doorseal.mp3","1",-1,"","");
}

rule fermi_filter_on
inactive
highFrequency
{
	if (trTimeMS() > trQuestVarGet("fermiFilterNext")) {
		trSoundPlayFN("firegiantdie.wav","1",-1,"","");
		trSoundPlayFN("cinematics\32_out\doorbigshut.mp3","1",-1,"","");
		trCameraShake(1.0, 0.5);
		vector pos = vector(0,0,0);
		// 16 x 16 with 73 at the center
		for(x=57; < 90) {
			for(z=57; < 90) {
				if (trGetTerrainSubType(x, z) != 50) {
					if (trGetTerrainSubType(x, z) != 73) {
						trPaintTerrain(x, z, x, z, 2, 10, false);
					}
				}
			}
		}
		
		xsDisableSelf();
		xsEnableRule("fermi_filter_off");
		trQuestVarSet("fermiFilterNext", trTimeMS() + 1000);
	}
}

rule fermi_filter_off
inactive
highFrequency
{
	if (trTimeMS() > trQuestVarGet("fermiFilterNext")) {
		for(x=57; < 90) {
			for(z=57; < 90) {
				if (trGetTerrainSubType(x, z) != 50) { // not olympus A
					trPaintTerrain(x, z, x, z, 0, 53, false); // olympus tile
				}
			}
		}
		xsDisableSelf();
	} else {
		vector pos = vector(0,0,0);
		for(i=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			xUnitSelectByID(dPlayerUnits, xUnitID);
			if (trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				pos = vectorToGrid(kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName), true));
				if (terrainIsType(pos, 0, 73) == false) { // blue tiles
					trUnitDelete(false);
				}
			}
		}
	}
}

rule kepler_offset
inactive
highFrequency
{
	if (trTimeMS() >= bossNext) {
		bossNext = bossNext + 30;
		trUnitSelectClear();
		trUnitSelect(""+bossCount, true);
		trMutateSelected(kbGetProtoUnitID("Dock"));
		trSetSelectedScale(0,1,0);
		trSetSelectedUpVector(0,-2,0);
		trUnitSetAnimationPath("1,0,0,0,0,0,0");
		bossCount = bossCount + 1;
		if (bossCount == trQuestVarGet("keplerEnd")) {
			xsDisableSelf();
		}
	}
}

rule boss12_battle
inactive
highFrequency
{
	int p = 0;
	int x = 0;
	int z = 0;
	int action = 0;
	int id = 0;
	int target = 0;
	float amt = 0;
	float angle = 0;
	float dist = 0;

	int timediff = 0;
	bool hit = false;
	
	vector hitbox = vector(0,0,0);
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);

	trUnitSelectClear();
	trUnitSelectByID(bossID);
	if (trUnitAlive()) {
		// lion shockwaves
		if (xGetDatabaseCount(dLionShockwaves) > 0) {
			action = processGenericProj(dLionShockwaves);
			if (trQuestVarGet("bossPhase") == 2) {
				if (action == PROJ_FALLING) {
					pos = kbGetBlockPosition(""+xGetInt(dLionShockwaves, xUnitName), true);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits, xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if ((xGetBool(dPlayerUnits, xLaunched) == false) && (xGetInt(dPlayerUnits, xStunStatus) == 0)) {
							if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 9.0) {
								launchUnit(dPlayerUnits, pos + (xGetVector(dLionShockwaves, xProjDir) * 100.0));
							}
						}
					}
					pos = vectorToGrid(pos);
					if (trGetTerrainHeight(xsVectorGetX(pos),xsVectorGetZ(pos)) < worldHeight - 0.5 || terrainIsType(pos, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
						xFreeDatabaseBlock(dLionShockwaves);
					}
				}
			}
		}

		// nottud shockwave
		if (kbUnitGetAnimationActionType(bossID) == 6) {
			xsSetContextPlayer(ENEMY_PLAYER);
			target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(bossID));
			xsSetContextPlayer(0);
			bossPos = kbGetBlockPosition(""+bossUnit, true);
			bossAnim = true;
			trUnitSelectClear();
			trUnitSelectByID(bossID);
			trUnitOverrideAnimation(26, 0, false, false, -1);
			trVectorQuestVarSet("goreDir", getUnitVector(bossPos, kbGetBlockPosition(""+target, true)));
			trQuestVarSet("goreNext", trTimeMS() + 1000);
			trQuestVarSet("goreStep", 1);
		} else if (trTimeMS() > trQuestVarGet("goreNext")) {
			switch(1*trQuestVarGet("goreStep"))
			{
				case 1:
				{
					addGenericProj(dLionShockwaves, bossPos, trVectorQuestVarGet("goreDir"));
					trQuestVarSet("goreStep", 2);
					bossNext = bossNext + 1000;
				}
				case 2:
				{
					trQuestVarSet("goreStep", 0);
					trUnitSelectClear();
					trUnitSelectByID(bossID);
					trUnitOverrideAnimation(-1,0,false,true,-1);
					bossAnim = false;
				}
			}
		}


		switch(1*trQuestVarGet("bossPhase"))
		{
			case 0:
			{
				trUnitSelectClear();
				trUnitSelectByID(bossID);
				trQuestVarSetFromRand("rand", 1, 2 + xsMin(3, trUnitPercentDamaged() * 0.05), true);
				trQuestVarSet("bossSpell", trQuestVarGet("nottud"+1*trQuestVarGet("rand")));
				trQuestVarSet("bossPhase", 1);
				bossNext = trTimeMS();
				bossCount = 0;
				bossTimeout = 30;
				trQuestVarSet("nottud"+1*trQuestVarGet("rand"), trQuestVarGet("nottud0"));
				trQuestVarSet("nottud0", trQuestVarGet("bossSpell"));
				
				if (trQuestVarGet("bossSpell") != dBigBang) {
					paintMapTile(72, 72, 1*trQuestVarGet("bossSpell"));
				}

				xClearDatabase(dLionShockwaves);

				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				switch(1*trQuestVarGet("bossSpell"))
				{
					case dPitOfDoom:
					{
						TERRAIN_WALL = 2;
						TERRAIN_SUB_WALL = 12;
						trOverlayTextColour(255,255,255);
						trOverlayText("The Pit of Doom", 3.0, -1, -1, -1);
						trSetLighting("hades", 2.0);
						worldHeight = 6;
						wallHeight = -3;
					}
					case dLaplace:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 37;
						trOverlayTextColour(255,0,0);
						trOverlayText("Laplace's Demon", 3.0, -1, -1, -1);
						trSetLighting("dawn", 2.0);
						worldHeight = 6;
						wallHeight = 6;
					}
					case dKepler:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 73;
						trOverlayTextColour(255,0,0);
						trOverlayText("Kepler's Law", 3.0, -1, -1, -1);
						trSetLighting("Fimbulwinter", 2.0);
						worldHeight = -3;
						wallHeight = 15;
					}
					case dFermi:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 50;
						trOverlayTextColour(255,0,0);
						trOverlayText("The Fermi Paradox", 3.0, -1, -1, -1);
						trSetLighting("eclipse", 2.0);
						worldHeight = -3;
						wallHeight = 15;
					}
					case dPillars:
					{
						TERRAIN_WALL = 3;
						TERRAIN_SUB_WALL = 9;
						trOverlayTextColour(255,0,0);
						trOverlayText("Pillars of Creation", 3.0, -1, -1, -1);
						trSetLighting("default", 2.0);
						worldHeight = -3;
						wallHeight = -3;
					}
					case dBigBang:
					{
						TERRAIN_WALL = 2;
						TERRAIN_SUB_WALL = 13;
						trOverlayTextColour(255,255,255);
						trOverlayText("The Big Bang", 3.0, -1, -1, -1);
						trSetLighting("default", 2.0);
						worldHeight = 3;
						wallHeight = 3;
						trQuestVarSet("bossPhase", 3);
						bossCount = 31;
						bossTimeout = 5;
						paintMapOutside();
					}
				}
			}
			case 1:
			{
				if(trTimeMS() > bossNext) {
					bossNext = bossNext + 50;
					bossCount = bossCount + 1;
					transitionRing(bossCount, 1*trQuestVarGet("bossSpell"));
				}
				if (bossCount > bossTimeout) {
					paintMapOutside();
					trQuestVarSet("bossPhase", 2);
					trQuestVarSet("bossCooldownTime", trTimeMS() + 20000);
					trQuestVarSet("bossStep", 0);
				}
			}
			case 2:
			{
				switch(1*trQuestVarGet("bossSpell"))
				{
					case dPitOfDoom:
					{
						if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
							trQuestVarSet("bossPhase", 0);
						}
						pos = vectorToGrid(kbGetBlockPosition(""+xGetInt(dPlayerUnits, xUnitName)));
						if (trGetTerrainHeight(xsVectorGetX(pos),xsVectorGetZ(pos)) < worldHeight - 1.0) {
							xUnitSelectByID(dPlayerUnits, xUnitID);
							trDamageUnitPercent(100);
						}
					}
					case dLaplace:
					{
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								start = kbGetBlockPosition(""+bossUnit, true);
								dir = vector(20, 0, 0);
								for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
									xDatabaseNext(dPhysicsBalls);
									pos = vector(145,0,145) + dir;
									xSetVector(dPhysicsBalls, xPhysicsBallPos, pos);
									xSetVector(dPhysicsBalls, xPhysicsBallPrev, pos);
									if (xGetDatabaseCount(dPlayerCharacters) > 0) {
										xDatabaseNext(dPlayerCharacters);
										xSetVector(dPhysicsBalls, xPhysicsBallDir, rotationMatrix(xsVectorNormalize(kbGetBlockPosition(""+xGetInt(dPlayerCharacters, xUnitName)) - pos), 0.980785, 0.19509));
									} else {
										xSetVector(dPhysicsBalls, xPhysicsBallDir, rotationMatrix(xsVectorNormalize(start - pos), 0.980785, 0.19509));
									}
									
									xSetBool(dPhysicsBalls, xPhysicsBallOut, false);
									dir = rotationMatrix(dir, 0.980785, 0.19509);
								}
								trQuestVarSet("bossStep", 1);
								bossNext = trTimeMS() + 9000;
								bossTimeout = bossNext + 2000;
								physicsSpeed = 1;
								lastTime = trTimeMS();
								trSoundPlayFN("petsuchosattack.wav","1",-1,"","");
							}
							case 1:
							{
								trQuestVarSet("sound", 0);
								bossCount = 5;
								timediff = trTimeMS() - lastTime;
								lastTime = trTimeMS();
								if (physicsSpeed < 20.0) {
									physicsSpeed = xsMin(20.0, physicsSpeed + 0.003 * timediff);
								}
								xSetPointer(dPhysicsBalls, physicsBallPointer);
								for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
									processPhysicsBall(timediff, physicsSpeed, bossCount > 0);
									if (distanceBetweenVectors(xGetVector(dPhysicsBalls, xPhysicsBallPos), vector(145,0,145)) > 625.0) {
										if (xGetBool(dPhysicsBalls, xPhysicsBallOut) == false) {
											xSetBool(dPhysicsBalls, xPhysicsBallOut, true);
											trQuestVarSetFromRand("rand", 1, 3, true);
											trSoundPlayFN("mine"+1*trQuestVarGet("rand")+".wav","1",-1,"","");
											dir = xsVectorNormalize(xGetVector(dPhysicsBalls, xPhysicsBallPos) - vector(145,0,145));
											xSetVector(dPhysicsBalls, xPhysicsBallDir, xsVectorNormalize(xGetVector(dPhysicsBalls, xPhysicsBallDir) - (dir * dotProduct(dir, xGetVector(dPhysicsBalls, xPhysicsBallDir)) * 2.2)));
										}
									} else {
										xSetBool(dPhysicsBalls, xPhysicsBallOut, false);
									}

									bossCount = bossCount - 1;
									if (bossCount == -1) {
										physicsBallPointer = xGetPointer(dPhysicsBalls);
									}
								}
								if (trQuestVarGet("sound") > 0) {
									trQuestVarSetFromRand("sound", 1, 3, true);
									trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
								}

								if (trTimeMS() > bossNext) {
									physicsSpeed = 0.01 * (bossTimeout - trTimeMS());
									if (physicsSpeed < 0) {
										physicsSpeed = 0;
										trQuestVarSet("bossStep", 2);
										bossTimeout = trTimeMS() + 3000;
										bossCount = 3;

										for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
											xDatabaseNext(dPhysicsBalls);
											pos = xGetVector(dPhysicsBalls, xPhysicsBallPos);
											dir = xsVectorNormalize(xGetVector(dPhysicsBalls, xPhysicsBallDir));
											xDatabaseNext(dLaplaceLaser);
											xSetVector(dLaplaceLaser, xLaplaceLaserDir, dir);
											xSetVector(dLaplaceLaser, xLaplaceLaserPos, pos);
											xUnitSelectByID(dLaplaceLaser, xUnitID);
											trSetUnitOrientation(vector(0,0,0) - dir, vector(0,1,0), true);
											trUnitTeleport(xsVectorGetX(pos),worldHeight + 0.5, xsVectorGetZ(pos));
										}
									}
								}
							}
							case 2:
							{
								if (trTimeMS() > bossTimeout) {
									bossCount = bossCount - 1;
									trSoundPlayFN("attackwarning.wav","1",-1,"","");
									if (bossCount <= 0) {
										trQuestVarSet("bossStep", 3);
										bossCount = xGetDatabaseCount(dPhysicsBalls);
										bossNext = trTimeMS();
									} else {
										bossTimeout = bossTimeout + 1000;
									}
								}
							}
							case 3:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("sound", 1, 5, true);
									trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
									bossCount = bossCount - 1;
									bossNext = bossNext + 100;
									xDatabaseNext(dLaplaceLaser);
									dir = xGetVector(dLaplaceLaser, xLaplaceLaserDir);
									pos = xGetVector(dLaplaceLaser, xLaplaceLaserPos);
									for(i=xGetDatabaseCount(dPlayerUnits); >0) {
										xDatabaseNext(dPlayerUnits);
										xUnitSelectByID(dPlayerUnits, xUnitID);
										if (trUnitAlive() == false) {
											removePlayerUnit();
										} else if (rayCollision(dPlayerUnits, pos, dir, 999.0, 1.0)) {
											damagePlayerUnit(900.0);
										}
									}
									xUnitSelectByID(dLaplaceLaser, xUnitID);
									trUnitHighlight(10.0, false);
									trSetSelectedScale(5.0, 0, 40.0);
									xDatabaseNext(dPhysicsBalls);
									xUnitSelectByID(dPhysicsBalls, xUnitID);
									trSetSelectedUpVector(0, -10, 0);
									if (bossCount <= 0) {
										trQuestVarSet("bossStep", 4);
										bossTimeout = 1000 + trTimeMS();
									}
								}
							}
							case 4:
							{
								dist = bossTimeout - trTimeMS();
								if (dist > 0) {
									dist = 0.005 * dist;
									for(i=xGetDatabaseCount(dLaplaceLaser); >0) {
										xDatabaseNext(dLaplaceLaser);
										xUnitSelectByID(dLaplaceLaser, xUnitID);
										trSetSelectedScale(dist, 0, 40);
									}
								} else {
									for(i=xGetDatabaseCount(dLaplaceLaser); >0) {
										xDatabaseNext(dLaplaceLaser);
										xUnitSelectByID(dLaplaceLaser, xUnitID);
										trSetSelectedScale(0, 0, 0);
									}
									trQuestVarSet("bossPhase", 0);
								}
							}
						}
					}
					case dKepler:
					{
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								bossCount = trQuestVarGet("keplerStart");
								bossNext = trTimeMS();
								xsEnableRule("kepler_offset");
								for(i=trQuestVarGet("keplerStart"); < trQuestVarGet("keplerEnd")) {
									trUnitSelectClear();
									trUnitSelect(""+i, true);
									trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
								}
								trQuestVarSet("bossStep", 1);
							}
							case 1:
							{
								if (bossCount == trQuestVarGet("keplerEnd")) {
									lastTime = trTimeMS();
									keplerPos = kbGetBlockPosition(""+bossUnit, true);
									keplerDir = getUnitVector(keplerPos, vector(145,0,145), 15.0);
									
									trQuestVarSet("bossStep", 2);
									trSoundPlayFN("vortexstart.wav","1",-1,"","");
									trQuestVarSet("bossCooldownTime", trTimeMS() + 15000);
								}
							}
							case 2:
							{
								if (trTime() > trQuestVarGet("soundTime")) {
									trSoundPlayFN("vortexexist2.wav","1",-1,"","");
									trQuestVarSet("soundTime", trTime() + 6);
								}
								pos = kbGetBlockPosition(""+bossUnit, true);
								timediff = trTimeMS() - lastTime;
								dist = 0.001 * timediff;
								lastTime = trTimeMS();

								prev = keplerPos;
								keplerPos = keplerPos + (keplerDir * dist);
								keplerDir = keplerDir + getUnitVector(keplerPos, pos, dist * 10.0);

								dir = (keplerPos - vector(145.0, 0.0, 145.0)) * 0.125;

								for(i=trQuestVarGet("keplerStart"); < trQuestVarGet("keplerEnd")) {
									trUnitSelectClear();
									trUnitSelect(""+i, true);
									trSetSelectedUpVector(xsVectorGetX(dir),-0.9,xsVectorGetZ(dir));
								}

								trUnitSelectClear();
								trUnitSelectByQV("keplerEnd");
								trSetSelectedScale(0,distanceBetweenVectors(keplerPos, vector(145,0,145), false) * 0.125, 0);
								trSetUnitOrientation(vector(0,1,0),getUnitVector(vector(145,0,145), keplerPos),true);

								amt = distanceBetweenVectors(keplerPos, prev, false) + 4.0;
								dir = getUnitVector(prev, keplerPos);

								for(i=xGetDatabaseCount(dPlayerUnits); >0) {
									xDatabaseNext(dPlayerUnits);
									xUnitSelectByID(dPlayerUnits, xUnitID);
									if (trUnitAlive() == false) {
										removePlayerUnit();
									} else if (rayCollision(dPlayerUnits, prev, dir, amt, 16.0)) {
										damagePlayerUnit(dist * 3000);
									}
								}
								
								if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
									trQuestVarSet("bossPhase", 0);
									for(i=trQuestVarGet("keplerStart"); < trQuestVarGet("keplerEnd")) {
										trUnitSelectClear();
										trUnitSelect(""+i, true);
										trSetSelectedUpVector(0, -10, 0);
									}
									trUnitSelectClear();
									trUnitSelectByQV("keplerEnd");
									trSetUnitOrientation(vector(1,0,0),vector(0,-1,0),true);
								}
							}
						}
					}
					case dFermi:
					{
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								trQuestVarSet("bossStep", 1);
								bossNext = trTimeMS() + 3000;
								trMessageSetText("Stand on the blue tiles!", -1);
							}
							case 1:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("rand", 0, 1, true);
									for(x= -3; < 3) {
										hit = trQuestVarGet("rand") > 0;
										for(z= -3; < 3) {
											if (xsAbs(x) + xsAbs(z) <= 3) {
												hit = (hit == false); // invert hit
												if (hit) {
													trPaintTerrain(73 + 5*x - 2, 73 + 5*z - 2, 73 + 5*x + 2, 73 + 5*z + 2, 0, 73, false);
												}
											}
										}
									}
									trQuestVarSet("bossStep", 2);
									bossNext = trTimeMS() + 5000;
									fermiFilterActivate();
								}
							}
							case 2:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("rand", 0, 1, true);
									for(x= -3; < 3) {
										hit = trQuestVarGet("rand") > 0;
										for(z= -3; < 3) {
											if (xsAbs(x) + xsAbs(z) <= 3) {
												hit = (hit == false); // invert hit
												if (hit) {
													trPaintTerrain(73 + 5*x - 1, 73 + 5*z - 1, 73 + 5*x + 1, 73 + 5*z + 1, 0, 73, false);
												}
											}
										}
									}
									trQuestVarSet("bossStep", 3);
									bossNext = trTimeMS() + 5000;
									fermiFilterActivate();
								}
							}
							case 3:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("rand", 0, 1, true);
									for(x= -5; < 5) {
										hit = trQuestVarGet("rand") > 0;
										for(z= -5; < 5) {
											if (xsAbs(x) + xsAbs(z) <= 4) {
												hit = (hit == false); // invert hit
												if (hit) {
													trPaintTerrain(73 + 3*x, 73 + 3*z, 73 + 3*x, 73 + 3*z, 0, 73, false);
												}
											}
										}
									}
									trQuestVarSet("bossStep", 4);
									bossNext = trTimeMS() + 5000;
									fermiFilterActivate();
								}
							}
							case 4:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSet("bossPhase", 0);
								}
							}
						}
					}
					case dPillars:
					{
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								trMessageSetText("The Pillars are releasing healing orbs that will heal nottud! Touch them to destroy them!", -1);
								trQuestVarSet("bossStep", 1);
								bossNext = trTimeMS();
								bossTimeout = trTimeMS() + 15000;
							}
							case 1:
							{
								bossPos = kbGetBlockPosition(""+bossUnit, true);
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("sound", 1, 3, true);
									trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
									bossNext = bossNext + 2000;
									for(i=1; <=3) {
										pos = trVectorQuestVarGet("pillar"+i);
										dir = getUnitVector(pos, bossPos);
										addGenericProj(dBossHeals, pos, dir);
									}
								}

								if (trTimeMS() > bossTimeout) {
									trQuestVarSet("bossPhase", 0);
									xClearDatabase(dBossHeals);
								}

								
								for (j=xsMin(5, xGetDatabaseCount(dBossHeals)); > 0) {
									processGenericProj(dBossHeals);
									pos = kbGetBlockPosition(""+xGetInt(dBossHeals, xUnitName), true);
									if (distanceBetweenVectors(pos, bossPos) < 9.0) {
										xUnitSelectByID(dBossHeals, xUnitID);
										trUnitChangeProtoUnit("Regeneration SFX");
										xUnitSelectByID(dBossHeals, xUnitID);
										trDamageUnitPercent(-100);
										xFreeDatabaseBlock(dBossHeals);
										trSoundPlayFN("recreation.wav","1",-1,"","");
										trUnitSelectClear();
										trUnitSelect(""+bossUnit, true);
										trDamageUnitPercent(-3);
										trUnitHighlight(0.2,false);
									} else {
										xSetVector(dBossHeals, xProjDir, getUnitVector(pos, bossPos));
										for(x=xGetDatabaseCount(dPlayerUnits); >0) {
											xDatabaseNext(dPlayerUnits);
											xUnitSelectByID(dPlayerUnits, xUnitID);
											if (trUnitAlive() == false) {
												removePlayerUnit();
											} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 4) {
												healUnit(xGetInt(dPlayerUnits, xPlayerOwner), 100);
												xUnitSelectByID(dBossHeals, xUnitID);
												trUnitChangeProtoUnit("Hero Death");
												xUnitSelectByID(dBossHeals, xUnitID);
												trDamageUnitPercent(-100);
												trSoundPlayFN("heal.wav","1",-1,"","");
												xFreeDatabaseBlock(dBossHeals);
												break;
											}
										}
									}
								}
							}
						}
					}
					case dBigBang:
					{
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								if (trTimeMS() > bossTimeout) {
									bossDir = vector(1,0,0);
									trVectorQuestVarSet("shootLaserDir", vector(0,0,1));
									bossNext = trTimeMS();
									bossTimeout = trTimeMS();
									trQuestVarSet("bossCooldownTime", trTimeMS() + 20000);
									trQuestVarSet("blackhole", trGetNextUnitScenarioNameNumber());
									trArmyDispatch("0,0","Dwarf",1,145,0,145,180,true);
									trArmySelect("0,0");
									trUnitChangeProtoUnit("Implode Sphere Effect");
									trQuestVarSet("bossStep", 1);
									trUIFadeToColor(255,255,255,2000,0,false);
									trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
									trCameraShake(3.0, 1.0);
								}
							}
							case 1:
							{
								if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
									trQuestVarSet("bossPhase", 0);
									trUnitSelectClear();
									trUnitSelectByQV("blackhole");
									trUnitDestroy();
								}
								if (trTime() > trQuestVarGet("soundTime")) {
									trQuestVarSet("soundTime", trTime() + 8);
									trSoundPlayFN("lightning loop.wav","1",-1,"","");
								}
								for(i=xsMin(5, xGetDatabaseCount(dDragonFireballs)); >0) {
									action = processGenericProj(dDragonFireballs);
									if (action == PROJ_FALLING) {
										dir = xGetVector(dDragonFireballs, xProjDir);
										pos = kbGetBlockPosition(""+xGetInt(dDragonFireballs, xUnitName), true);
										prev = xGetVector(dDragonFireballs, xProjPrev);
										dist = distanceBetweenVectors(pos, prev, false) + 2.0;
										hit = false;
										for(j=xGetDatabaseCount(dPlayerUnits); >0) {
											xDatabaseNext(dPlayerUnits);
											xUnitSelectByID(dPlayerUnits, xUnitID);
											if (trUnitAlive() == false) {
												removePlayerUnit();
											} else if (rayCollision(dPlayerUnits, prev, dir, dist, 2.0)) {
												hit = true;
												damagePlayerUnit(300.0);
											}
										}
										if (hit) {
											trQuestVarSetFromRand("sound", 1, 2, true);
											trSoundPlayFN("fireball fall "+1*trQuestVarGet("sound")+".wav","1",-1,"","");
											xUnitSelectByID(dDragonFireballs, xUnitID);
											trUnitChangeProtoUnit("Meteorite");
											xFreeDatabaseBlock(dDragonFireballs);
										} else if (distanceBetweenVectors(pos, vector(145,0,145)) > 900.0) {
											xFreeDatabaseBlock(dDragonFireballs);
										} else {
											xSetVector(dDragonFireballs, xProjPrev, pos);
											if (xGetBool(dDragonFireballs, xDragonFireballRemove)) {
												xSetBool(dDragonFireballs, xDragonFireballRemove, false);
												xSetVector(dDragonFireballs, xProjDir, rotationMatrix(dir, 0, -1.0));
											}
										}
									}
								}
								for(i=xsMin(5, xGetDatabaseCount(dShadeBolts)); >0) {
									action = processGenericProj(dShadeBolts);
									if (action == PROJ_FALLING) {
										dir = xGetVector(dShadeBolts, xProjDir);
										pos = kbGetBlockPosition(""+xGetInt(dShadeBolts, xUnitName), true);
										prev = xGetVector(dShadeBolts, xProjPrev);
										dist = distanceBetweenVectors(pos, prev, false) + 2.0;
										hit = false;
										for(j=xGetDatabaseCount(dPlayerUnits); >0) {
											xDatabaseNext(dPlayerUnits);
											xUnitSelectByID(dPlayerUnits, xUnitID);
											if (trUnitAlive() == false) {
												removePlayerUnit();
											} else if (rayCollision(dPlayerUnits, prev, dir, dist, 2.0)) {
												hit = true;
												damagePlayerUnit(300.0);
											}
										}
										if (hit) {
											trQuestVarSetFromRand("sound", 1, 3, true);
											trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
											xUnitSelectByID(dShadeBolts, xUnitID);
											trUnitChangeProtoUnit("Lightning Sparks");
											xUnitSelectByID(dShadeBolts, xUnitID);
											trDamageUnitPercent(-100);
											xFreeDatabaseBlock(dShadeBolts);
										} else if (distanceBetweenVectors(pos, vector(145,0,145)) > 900.0) {
											xFreeDatabaseBlock(dShadeBolts);
										} else {
											xSetVector(dShadeBolts, xProjPrev, pos);
											if (xGetBool(dShadeBolts, xDragonFireballRemove)) {
												xSetBool(dShadeBolts, xDragonFireballRemove, false);
												xSetVector(dShadeBolts, xProjDir, rotationMatrix(dir, 0, 1.0));
											}
										}
									}
								}
								for(i=xsMin(5, xGetDatabaseCount(dValkyrieProj)); >0) {
									action = processGenericProj(dValkyrieProj);
									if (action == PROJ_BOUNCE) {
										trUnitSetAnimationPath("1,0,0,0,0,0,0");
									} else if (action == PROJ_FALLING) {
										if (unitDistanceToVector(xGetInt(dValkyrieProj, xUnitName), vector(145,0,145)) > 900.0) {
											xFreeDatabaseBlock(dValkyrieProj);
										}
									}
								}
								if (trTimeMS() > bossNext) {
									bossNext = bossNext + 200;
									bossDir = rotationMatrix(bossDir, -0.740544, -0.672008);
									dir = bossDir;
									addGenericProj(dDragonFireballs, vector(145,0,145), dir);

									dir = rotationMatrix(dir, -0.5, 0.866025);
									addGenericProj(dShadeBolts, vector(145, 0, 145), dir);

									dir = rotationMatrix(dir, -0.5, 0.866025);
									addGenericProj(dValkyrieProj, vector(145, 0, 145), dir);
								}
								if (trTimeMS() > bossTimeout) {
									bossTimeout = bossTimeout + 300;
									dir = trVectorQuestVarGet("shootLaserDir");
									dir = rotationMatrix(dir, -0.740544, -0.672008);
									trVectorQuestVarSet("shootLaserDir", dir);
									shootLaser(vector(145,0,145), dir, 60.0, ENEMY_PLAYER);
								}
							}
						}
					}
				}
			}
			case 3:
			{
				if(trTimeMS() > bossNext) {
					bossNext = bossNext + 50;
					bossCount = bossCount - 1;
					timediff = bossCount;
					transitionRing(bossCount, 1*trQuestVarGet("nottud"+bossTimeout));
					if (bossCount == 0) {
						bossCount = 31;
						bossTimeout = bossTimeout - 1;
						trQuestVarSetFromRand("sound", 1, 3, true);
						trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
						if (bossTimeout < 0) {
							trQuestVarSet("bossPhase", 2);
							trQuestVarSet("bossStep", 0);
							trSoundPlayFN("vortexstart.wav","1",-1,"","");
							trSoundPlayFN("meteorapproach.wav","1",-1,"","");
							trCameraShake(3.0, 0.1);
							bossTimeout = trTimeMS() + 3000;
						}
					}
				}
			}
		}
	} else {
		trUnitOverrideAnimation(-1,0,false,true,-1);
		xsDisableSelf();
		trMusicStop();
		xsDisableRule("boss_music");
		xsDisableRule("gameplay_always");

		xsEnableRule("nottud_ded");

		xRestoreCache(dPhysicsBalls);

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


rule nottud_ded
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
					"nottud:No! Impossible!","icons\special g minotaur icon 64");
			}
			case 2:
			{
				trShowImageDialog(relicIcon(RELIC_NOTTUD), "Relic: " + relicName(RELIC_NOTTUD));
				trQuestVarSet("gameOverNext", trTime() + 6);
				trQuestVarSet("ownedRelics"+RELIC_NOTTUD, 1 + trQuestVarGet("ownedRelics"+RELIC_NOTTUD));
				trSoundPlayFN("default","1",-1,"Zenophobia:Yeah what the fuck? How did you beat that?","icons\infantry g hoplite icon 64");
				trSoundPlayFN("favordump.wav","1",-1,"","");
			}
			case 3:
			{
				gadgetUnreal("ShowImageBox");
				trQuestVarSet("gameOverNext", trTime() + 3);
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
