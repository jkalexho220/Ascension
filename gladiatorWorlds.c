int dPitOfDoom = 0;
int dLaplace = 0;
int dFermi = 0;
int dKepler = 0;
int dPillars = 0;
int dBigBang = 0;

int dMouthOfChaos = 0;
int bullshitProj = 0;

int dOnagerShots = 0;
int xOnagerShotProto = 0;
int xOnagerShotPos = 0;

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

		dOnagerShots = xInitDatabase("onagerShots");
		xInitAddInt(dOnagerShots, "name");
		xInitAddInt(dOnagerShots, "player");
		xOnagerShotPos = xInitAddVector(dOnagerShots, "pos");
		xOnagerShotProto = xInitAddInt(dOnagerShots, "proto");

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
		//xsEnableRule("gladiator_worlds_build_"+1*trQuestVarGet("rand"));
		trQuestVarSet("map"+1*trQuestVarGet("rand"), 1);

		trQuestVarSet("gladiatorRound", 1);

		/* oracle */
		for(i=10; >0) {
			trTechSetStatus(ENEMY_PLAYER, 297, 4);
		}

		bullshitProj = trGetNextUnitScenarioNameNumber();

		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);

		//xsEnableRule("gladiator_worlds_cin_1");
		xsEnableRule("gladiator_worlds_cin_5");

		trQuestVarSet("eyecandyStart", trGetNextUnitScenarioNameNumber());

		xsDisableSelf();
	}
}

void spawnPlayerCircle(vector pos = vector(0,0,0)) {
	float angle = 6.283185 / (ENEMY_PLAYER - 1);
	float mCos = xsCos(angle);
	float mSin = xsSin(angle);
	vector dir = vector(4, 0, 4);
	for(p=1; < ENEMY_PLAYER) {
		spawnPlayer(p, pos + dir);
		equipRelicsAgain(p);
		dir = rotationMatrix(dir, mCos, mSin);
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
				trOverlayText("nottud", 3.0, -1, -1, -1);
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

				//xsEnableRule("gameplay_always");
				xsEnableRule("gameplay_start");

				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Minotaur", ENEMY_PLAYER, 0, 24000 * ENEMY_PLAYER);

				xsEnableRule("boss_stun_recovery");

				bossUnit = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Minotaur",1,155,0,155,225,true);
				activateEnemy(bossUnit);
				bossPointer = xGetNewestPointer(dEnemies);
				xSetBool(dEnemies, xLaunched, true, bossPointer);

				bossID = kbGetBlockID(""+bossUnit, true);

				xsEnableRule("boss12_battle");

				trQuestVarSet("bossSpell", dPitOfDoom);
				trQuestVarSet("bossPhase", 2);
				trQuestVarSet("bossCooldownTime", trTimeMS() + 3000);

				//spawnPlayerCircle(trVectorQuestVarGet("startPosition") - vector(10, 0, 10));

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
		xsEnableRule("gladiator_worlds_spawn");
		xsEnableRule("gladiator_worlds_always");
		xsDisableSelf();

		trQuestVarSet("currentKills", trGetStatValue(ENEMY_PLAYER, 6));
		trQuestVarSet("totalKills", 0);
		trQuestVarSet("killReward", 0);
		trCounterAddTime("killcount",-1,-9999,"Kills: " + 1*trQuestVarGet("totalKills"),-1);
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
	bool hit = false;

	int kills = trGetStatValue(ENEMY_PLAYER, 6);
	if (kills > trQuestVarGet("currentKills")) {
		trQuestVarSet("totalKills", trQuestVarGet("totalKills") + kills - trQuestVarGet("currentKills"));
		trQuestVarSet("currentKills", kills);
		trCounterAbort("killcount");
		trCounterAddTime("killcount",-1,-9999,"Kills: " + 1*trQuestVarGet("totalKills"),-1);
	}

	if (trQuestVarGet("totalKills") >= 100) {
		if (trQuestVarGet("killReward") == 0) {
			trQuestVarSet("killReward", 1);
			trSoundPlayFN("favordump.wav","1",-1,"","");
			trQuestVarSetFromRand("gem", 0, 2, true);
			trShowImageDialog(gemstoneIcon(1*trQuestVarGet("gem")), gemstoneName(1*trQuestVarGet("gem")) + " x" + 1*trQuestVarGet("gladiatorRound"));
			trQuestVarSet("gemstone"+1*trQuestVarGet("gem"), 1 + trQuestVarGet("gemstone"+1*trQuestVarGet("gem")));
		} else if (xGetDatabaseCount(dEnemies) == 0) {
			xsDisableSelf();
			xsDisableRule("gameplay_always");
			trSoundPlayFN("cinematics\22_out\music (wussy mix).mp3","1",-1,"","");
			trUIFadeToColor(0,0,0,3000,0,true);
			xsEnableRule("gladiator_worlds_build_next");
			xsDisableRule("gladiator_worlds_spawn");
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

	if (xGetDatabaseCount(dOnagerShots) > 0) {
		xDatabaseNext(dOnagerShots);
		explode = false;
		id = kbGetBlockID(""+xGetInt(dOnagerShots, xUnitName), true);
		if (id == -1) {
			pos = xGetVector(dOnagerShots, xOnagerShotPos);
			explode = true;
		} else {
			pos = kbGetBlockPosition(""+xGetInt(dOnagerShots, xUnitName), true);
			xSetVector(dOnagerShots, xOnagerShotPos, pos);
			if (xsVectorGetY(pos) < worldHeight + 0.5) {
				explode = true;
			}
		}
		if (explode) {
			xUnitSelect(dOnagerShots, xUnitName);
			trUnitChangeProtoUnit("Dust Large");
			trSoundPlayFN("backtowork.wav","1",-1,"","");
			p = xGetInt(dOnagerShots, xPlayerOwner);
			if (p == ENEMY_PLAYER) {
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dOnagerShots, xOnagerShotProto)));
				activateEnemy(next);
			} else {
				spawnPlayerUnit(p, xGetInt(dOnagerShots, xOnagerShotProto), pos, 5.0);
			}
			xFreeDatabaseBlock(dOnagerShots);
		}
	}

	if (xGetDatabaseCount(dFireLancePellets) > 0) {
		for(i=xsMin(8, xGetDatabaseCount(dFireLancePellets)); >0) {
			xDatabaseNext(dFireLancePellets);
			if (xGetInt(dFireLancePellets, xFireLancePelletTimeout) == 0) {
				xSetInt(dFireLancePellets, xFireLancePelletTimeout, trTimeMS() + 1500);
				xUnitSelect(dFireLancePellets, xUnitName);
				trMutateSelected(kbGetProtoUnitID("Thor Hammer"));
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
				pos = kbGetBlockPosition(""+bullshitProj, true);
				xAddDatabaseBlock(dYeebFeathers, true);
				xSetInt(dYeebFeathers, xUnitName, bullshitProj);
				xSetInt(dYeebFeathers, xPlayerOwner, p);
				xSetVector(dYeebFeathers, xYeebFeatherPos, pos);
				trUnitSelectClear();
				trUnitSelectByID(id);
				trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
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
			case kbGetProtoUnitID("Petrobolos Shot"):
			{
				/* onager launches units at you */
				trQuestVarSetFromRand("proto", 10 * trQuestVarGet("gladiatorRound") - 10, 10 * trQuestVarGet("gladiatorRound"), true);
				target = monsterPetProto(1*trQuestVarGet("proto"));
				trUnitSelectClear();
				trUnitSelectByID(id);
				trMutateSelected(target);
				trUnitOverrideAnimation(24, 0, true, false, -1);
				xAddDatabaseBlock(dOnagerShots, true);
				xSetInt(dOnagerShots, xUnitName, bullshitProj);
				xSetInt(dOnagerShots, xPlayerOwner, p);
				xSetInt(dOnagerShots, xOnagerShotProto, target);
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
					if (trQuestVarGet("relicdrop") < 0.05) {
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
	trStringQuestVarSet("enemyProto4", "Mummy");
	trStringQuestVarSet("enemyProto5", "Onager");

	trShowImageDialog("ui\ui map valley of kings 256x256", "Entering bullshit desert...");
	gadgetUnreal("ShowImageBox-CloseButton");

	xsDisableSelf();
}

void paintMapTile(int x = 0, int z = 0, int db = 0) {
	vector data = aiPlanGetUserVariableVector(db, x, z);
	trPaintTerrain(x, z, x, z, 1*xsVectorGetX(data), 1*xsVectorGetY(data), false);
	trChangeTerrainHeight(x, z, x, z, xsVectorGetZ(data), false);
}

void paintMapInstant(int db = 0) {
	for(x=0; < 145) {
		for(z=0; < 145) {
			paintMapTile(x, z, db);
		}
	}
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
	aiPlanSetUserVariableVector(db, x, z, xsVectorSet(primary, secondary, 15.0));
	for(i=14; >= -3) {
		temp = rotationMatrix(dir, 0, 1.0);
		if (aiPlanGetUserVariableBool(dMapTiles, x + xsVectorGetX(temp), z + xsVectorGetZ(temp)) == false) {
			dir = temp;
		}
		pos = pos + dir;
		x = xsVectorGetX(pos);
		z = xsVectorGetZ(pos);
		aiPlanSetUserVariableBool(dMapTiles, x, z, true);
		aiPlanSetUserVariableVector(db, x, z, xsVectorSet(primary, secondary, i));
		data = aiPlanGetUserVariableVector(db, x-1, z-1);
		data = xsVectorSet(primary, secondary, xsVectorGetZ(data));
		aiPlanSetUserVariableVector(db, x-1, z-1, data);
	}
	xClearDatabase(dEdgeFrontier);
}

void transitionRing(int dist = 0, int db = 0) {
	if (dist < 73) {
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

int dFermiBombers = 0;
int xFermiBomberDir = 0;
int xFermiBomberPos = 0;
int xFermiBomberNext = 0;

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
			} else if (rayCollision(dPlayerUnits, pos, dir, dist, 0.5)) {
				trQuestVarSet("sound", 1);
				damagePlayerUnit(100.0);
			}
		}
		xSetVector(dPhysicsBalls, xPhysicsBallPrev, xGetVector(dPhysicsBalls, xPhysicsBallPos));
	}
}

rule gladiator_worlds_build_5
inactive
highFrequency
{
	dFallingFireballs = initGenericProj("fallingFireballs",kbGetProtoUnitID("Fire Giant"),19,10.0,0,0,ENEMY_PLAYER,true);

	dBossWhirlpoolBalls = initGenericProj("bossWhirlpoolBalls",kbGetProtoUnitID("Pharaoh of Osiris XP"),50,8.0,0,0,0,true);

	dShadeBolts = initGenericProj("shadeBolts",kbGetProtoUnitID("Lampades"),18,9.0,0,0,1,true);

	dLionShockwaves = initGenericProj("lionShockwaves",kbGetProtoUnitID("Heka Shockwave SFX"),2,15.0,4.0);

	dFermiBombers = xInitDatabase("fermiBombers");
	xFermiBomberNext = xInitAddInt(dFermiBombers, "next");
	xFermiBomberPos = xInitAddVector(dFermiBombers, "pos");
	xFermiBomberDir = xInitAddVector(dFermiBombers, "dir");

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

	dMapTiles = aiPlanCreate("mapTiles", 8);
	for(i=0; < 145) {
		if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
			trSoundPlayFN("cantdothat.wav","1",-1,"","");
			debugLog("Cannot create new user variable at " + i);
		}
	}

	// pit of doom
	dPitOfDoom = aiPlanCreate("pitOfDoom", 8);

	for(x=0; <145) {
		aiPlanAddUserVariableVector(dPitOfDoom, x, "row"+x, 145);
		for(z=0; <145) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
			if (dist >= 144) {
				data = xsVectorSet(TERRAIN_WALL, TERRAIN_SUB_WALL, -3);
				trChangeTerrainHeight(x, z, x+1, z+1, -3, false);
			} else {
				data = xsVectorSet(TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, 6);
				trChangeTerrainHeight(x, z, x+1, z+1, 6, false);
			}
			aiPlanSetUserVariableVector(dPitOfDoom, x, z, data);
			trPaintTerrain(x, z, x, z, 1*xsVectorGetX(data), 1*xsVectorGetY(data), false);
			trChangeTerrainHeight(x, z, x, z, xsVectorGetZ(data), false);
		}
	}

	trPaintTerrain(0,0,0,0,TERRAIN_WALL, TERRAIN_SUB_WALL, true);

	for(x=56; < 90) {
		for(z=56; < 90) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
			if (dist > 144 && dist <= 256) {
				aiPlanSetUserVariableVector(dPitOfDoom, x, z, xsVectorSet(TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, -3));
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
	for(x=0; <145) {
		aiPlanAddUserVariableVector(dPillars, x, "row"+x, 145);
		for(z=0; < 145) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
			if (dist > 144) {
				data = xsVectorSet(3, 9, -3); // coral A
			} else {
				data = xsVectorSet(5, 0, -3); // ice A
			}
			aiPlanSetUserVariableVector(dPillars, x, z, data);
		}
	}

	dir = vector(16.970568, 0, 16.970568); // distance of 32 in coordinates
	trVectorQuestVarSet("firePillar", vectorSnapToGrid(vector(145,0,145) + dir));
	paintPillar(vectorToGrid(trVectorQuestVarGet("firePillar")), dPillars, 2, 10); // hades 7

	dir = rotationMatrix(dir, -0.5, 0.866025);
	trVectorQuestVarSet("grassPillar", vectorSnapToGrid(vector(147,0,147) + dir));
	paintPillar(vectorToGrid(trVectorQuestVarGet("grassPillar")), dPillars, 0, 8); // gaia creep

	dir = rotationMatrix(dir, -0.5, 0.866025);
	trVectorQuestVarSet("coralPillar", vectorSnapToGrid(vector(147,0,147) + dir));
	paintPillar(vectorToGrid(trVectorQuestVarGet("coralPillar")), dPillars, 3, 10); // coral A

	dKepler = aiPlanCreate("kepler", 8);
	// gravitational pull field
	for(x=0; <145) {
		aiPlanAddUserVariableVector(dKepler, x, "row"+x, 145);
		for(z=0; < 145) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
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

	for(x=0; < 145) {
		aiPlanAddUserVariableVector(dFermi, x, "row"+x, 145);
		for(z=0; < 145) {
			aiPlanSetUserVariableVector(dFermi, x, z, xsVectorSet(0, 50, 15));
		}
	}

	for(x=0; < 20) {
		for(z=0; < 20) {
			dist = x + z - 5;
			if (dist < 0) {
				data = xsVectorSet(0, 53, -3);
			} else {
				data = xsVectorSet(0, 50, xsMin(15.0, 3.0 * dist));
			}
			for(a = -1; <= 1) {
				for(b = -1; <= 1) {
					aiPlanSetUserVariableVector(dFermi, 73 + 3*x + a, 73 + 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 73 + 3*x + a, 73 - 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 73 - 3*x + a, 73 + 3*z + b, data);
					aiPlanSetUserVariableVector(dFermi, 73 - 3*x + a, 73 - 3*z + b, data);
				}
			}
		}
	}

	dLaplace = aiPlanCreate("Laplace", 8);
	for(x=0; <145) {
		aiPlanAddUserVariableVector(dLaplace, x, "row"+x, 145);
		for(z=0; < 145) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
			if (dist > 144) {
				dist = -8.0 * xsSin(0.2 * (xsSqrt(dist) - 12.0));
				data = xsVectorSet(0, 37, dist + 6.0); // sand d
			} else {
				data = xsVectorSet(0, 53, 6); // olympus tile
			}
			aiPlanSetUserVariableVector(dLaplace, x, z, data);
		}
	}

	dBigBang = aiPlanCreate("bigBang", 8);
	for(x=0; < 145) {
		aiPlanAddUserVariableVector(dBigBang, x, "row"+x, 145);
		for(z=0; < 145) {
			dist = xsPow(x - 73, 2) + xsPow(z - 73, 2);
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

void processPillarBall(int db = 0) {
	for (j=xsMin(xGetDatabaseCount(db), 3); > 0) {
		int action = processGenericProj(db);
		if (action == PROJ_GROUND) {
			vector pos = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
			if (xsVectorGetX(pos) < 0) {
				pos = xGetVector(db, xProjPrev);
			}
			for(i=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 4.0) {
					damagePlayerUnit(100.0);
				}
			}
			xUnitSelectByID(db, xUnitID);
			trUnitChangeProtoUnit("Dwarf");
			xUnitSelectByID(db, xUnitID);
			trDamageUnitPercent(-100);
			trUnitChangeProtoUnit("Lightning Sparks");
			xFreeDatabaseBlock(db);
		} else {
			xSetVector(db, xProjPrev, kbGetBlockPosition(""+xGetInt(db, xUnitName),true));
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
		switch(1*trQuestVarGet("bossPhase"))
		{
			case 0:
			{
				trUnitSelectClear();
				trUnitSelectByID(bossID);
				trUnitChangeProtoUnit("Minotaur");
				trUnitSelectClear();
				trUnitSelectByID(bossID);
				trQuestVarSetFromRand("rand", 1, 2 + xsMin(3, trUnitPercentDamaged() * 0.05), true);
				trQuestVarSet("rand", 5);
				trQuestVarSet("bossSpell", trQuestVarGet("nottud"+1*trQuestVarGet("rand")));
				trQuestVarSet("bossPhase", 1);
				bossNext = trTimeMS();
				bossCount = 0;
				bossTimeout = 72;
				trQuestVarSet("nottud"+1*trQuestVarGet("rand"), trQuestVarGet("nottud0"));
				trQuestVarSet("nottud0", trQuestVarGet("bossSpell"));
				
				if (trQuestVarGet("bossSpell") != dBigBang) {
					paintMapTile(72, 72, 1*trQuestVarGet("bossSpell"));
				}

				trSoundPlayFN("cinematics\15_in\gong.wav","1",-1,"","");
				trSoundPlayFN("godpower.wav","1",-1,"","");
				switch(1*trQuestVarGet("bossSpell"))
				{
					case dPitOfDoom:
					{
						TERRAIN_WALL = 2;
						TERRAIN_SUB_WALL = 12;
						trOverlayText("The Pit of Doom", 3.0, -1, -1, -1);
						trSetLighting("hades", 2.0);
						worldHeight = 6;
					}
					case dLaplace:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 37;
						trOverlayText("Laplace's Demon", 3.0, -1, -1, -1);
						trSetLighting("dawn", 2.0);
						worldHeight = 6;
					}
					case dKepler:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 73;
						trOverlayText("Kepler's Law", 3.0, -1, -1, -1);
						trSetLighting("Fimbulwinter", 2.0);
						worldHeight = -3;
					}
					case dFermi:
					{
						TERRAIN_WALL = 0;
						TERRAIN_SUB_WALL = 50;
						trOverlayText("The Fermi Paradox", 3.0, -1, -1, -1);
						trSetLighting("eclipse", 2.0);
						worldHeight = -3;
					}
					case dPillars:
					{
						TERRAIN_WALL = 3;
						TERRAIN_SUB_WALL = 9;
						trOverlayText("Pillars of Creation", 3.0, -1, -1, -1);
						trSetLighting("default", 2.0);
						worldHeight = -3;
					}
					case dBigBang:
					{
						TERRAIN_WALL = 2;
						TERRAIN_SUB_WALL = 13;
						trOverlayText("The Big Bang", 3.0, -1, -1, -1);
						trSetLighting("default", 2.0);
						worldHeight = 3;
						trQuestVarSet("bossPhase", 3);
						bossCount = 30;
					}
				}
			}
			case 1:
			{
				while(trTimeMS() > bossNext) {
					bossNext = bossNext + 2500 / (25 + bossCount);
					bossCount = bossCount + 1;
					transitionRing(bossCount, 1*trQuestVarGet("bossSpell"));
				}
				if (bossCount > bossTimeout) {
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
						if (xGetDatabaseCount(dLionShockwaves) > 0) {
							action = processGenericProj(dLionShockwaves);
							if (action == PROJ_FALLING) {
								pos = kbGetBlockPosition(""+xGetInt(dLionShockwaves, xUnitName), true);
								for(x=xGetDatabaseCount(dPlayerUnits); >0) {
									xDatabaseNext(dPlayerUnits);
									xUnitSelectByID(dPlayerUnits, xUnitID);
									if (trUnitAlive() == false) {
										removePlayerUnit();
									} else if (xGetBool(dPlayerUnits, xLaunched) == false) {
										if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 9.0) {
											launchUnit(dPlayerUnits, pos + (xGetVector(dLionShockwaves, xProjDir) * 100.0));
										}
									}
								}
								pos = vectorToGrid(pos);
								if (trGetTerrainHeight(xsVectorGetX(pos),xsVectorGetZ(pos)) < worldHeight - 0.5) {
									xFreeDatabaseBlock(dLionShockwaves);
								}
							}
						}
						if (kbUnitGetAnimationActionType(bossID) == 6) {
							xsSetContextPlayer(ENEMY_PLAYER);
							bossTarget = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(bossID));
							xsSetContextPlayer(0);
							bossPos = kbGetBlockPosition(""+bossUnit, true);
							trUnitSelectClear();
							trUnitSelectByID(bossID);
							trUnitOverrideAnimation(26, 0, false, false, -1);
							bossDir = getUnitVector(bossPos, kbGetBlockPosition(""+bossTarget, true));
							bossNext = trTimeMS() + 1000;
							trQuestVarSet("bossStep", 1);
						} else if (trTimeMS() > bossNext) {
							switch(1*trQuestVarGet("bossStep"))
							{
								case 1:
								{
									addGenericProj(dLionShockwaves, bossPos, bossDir);
									trQuestVarSet("bossStep", 2);
									bossNext = bossNext + 1000;
								}
								case 2:
								{
									trQuestVarSet("bossStep", 0);
									trUnitSelectClear();
									trUnitSelectByID(bossID);
									trUnitOverrideAnimation(-1,0,false,true,-1);
								}
							}
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
								bossNext = trTimeMS() + 20000;
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
								if (physicsSpeed < 10) {
									physicsSpeed = xsMin(10.0, physicsSpeed + 0.003 * timediff);
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
									physicsSpeed = 0.005 * (bossTimeout - trTimeMS());
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
								while (trTimeMS() > bossNext) {
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
											damagePlayerUnit(500.0);
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
										bossTimeout = 1000;
										break;
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
									trQuestVarSet("bossStep", 5);
									trQuestVarSet("bossCooldownTime", trTimeMS() + 2000);
								}
							}
							case 5:
							{
								if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
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
								lastTime = trTimeMS();
								dir = vector(1,0,0);
								amt = 2.5;
								for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
									xDatabaseNext(dPhysicsBalls);
									xSetVector(dPhysicsBalls, xPhysicsBallPos, vector(145,0,145));
									xSetVector(dPhysicsBalls, xPhysicsBallPrev, vector(145, 0, 145));
									xSetVector(dPhysicsBalls, xPhysicsBallDir, dir * amt);
									dir = rotationMatrix(dir, -0.19509, 0.980785);
									amt = amt + 0.4;
								}
								physicsSpeed = 1;
								trQuestVarSet("bossStep", 1);
								trSoundPlayFN("vortexstart.wav","1",-1,"","");
								trQuestVarSet("bossCooldownTime", trTimeMS() + 15000);
							}
							case 1:
							{
								trQuestVarSet("sound", 0);
								bossCount = 5;
								timediff = trTimeMS() - lastTime;
								lastTime = trTimeMS();
								pos = kbGetBlockPosition(""+bossUnit, true);
								xSetPointer(dPhysicsBalls, physicsBallPointer);
								for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
									processPhysicsBall(timediff, physicsSpeed, bossCount > 0);
									dir = getUnitVector(xGetVector(dPhysicsBalls, xPhysicsBallPos), pos, 0.01 * timediff);
									xSetVector(dPhysicsBalls, xPhysicsBallDir, xGetVector(dPhysicsBalls, xPhysicsBallDir) + dir);

									bossCount = bossCount - 1;
									if (bossCount == -1) {
										physicsBallPointer = xGetPointer(dPhysicsBalls);
									}
								}
								if (trQuestVarGet("sound") > 0) {
									trQuestVarSetFromRand("sound", 1, 3, true);
									trSoundPlayFN("fleshcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
								}
								if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
									trQuestVarSet("bossPhase", 0);
									for(i=xGetDatabaseCount(dPhysicsBalls); >0) {
										xDatabaseNext(dPhysicsBalls);
										xUnitSelectByID(dPhysicsBalls, xUnitID);
										trSetSelectedUpVector(0, -10, 0);
									}
								}
							}
						}
					}
					case dFermi:
					{
						if (xGetDatabaseCount(dFermiBombers) > 0) {
							zSetProtoUnitStat("Kronny Flying", 0, 1, 0.01);
							xDatabaseNext(dFermiBombers);
							if (trTimeMS() > xGetInt(dFermiBombers, xFermiBomberNext)) {
								pos = xGetVector(dFermiBombers, xFermiBomberPos);
								xSetInt(dFermiBombers, xFermiBomberNext, xGetInt(dFermiBombers, xFermiBomberNext) + 500);
								
								xSetVector(dFermiBombers, xFermiBomberPos, pos + xGetVector(dFermiBombers, xFermiBomberDir));
								if (terrainIsType(vectorToGrid(xGetVector(dFermiBombers, xFermiBomberPos)), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
									xFreeDatabaseBlock(dFermiBombers);
								}

								trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								trArmySelect("0,0");
								trUnitChangeProtoUnit("Traitors effect");

								action = trGetNextUnitScenarioNameNumber();
								xAddDatabaseBlock(dRevealerBoom, true);
								xSetInt(dRevealerBoom, xUnitName, action);
								xSetInt(dRevealerBoom, xPlayerOwner, ENEMY_PLAYER);
								xSetInt(dRevealerBoom, xRevealerBoomTimeout, trTimeMS() + 1200);
								trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								trArmySelect("0,0");
								trUnitChangeProtoUnit("Spy Eye");
								trUnitSelectClear();
								trUnitSelect(""+action, true);
								trMutateSelected(kbGetProtoUnitID("Hades Door"));
								trSetSelectedScale(0,0,0);
								trUnitOverrideAnimation(25,0,false,false,-1);
								trUnitSetAnimationPath("3,0,0,0,0,0,0");
								trSetUnitOrientation(vector(0,1,0),vector(1,0,0),true);
							}
						}
						switch(1*trQuestVarGet("bossStep"))
						{
							case 0:
							{
								bossDir = vector(1,0,0);
								trQuestVarSet("bossStep", 1);
								bossNext = trTimeMS();
								bossCount = 2500;
								bossTimeout = trTimeMS() + 20000;
							}
							case 1:
							{
								if (trTimeMS() > bossNext) {
									trQuestVarSetFromRand("rand", -4, 4, true);
									pos = vector(145, 0, 145) + (bossDir * trQuestVarGet("rand") * 2);
									dist = 15.0 - (trQuestVarGet("rand") * 2.0);
									bossDir = rotationMatrix(bossDir, 0, 1.0);
									pos = pos - (bossDir * dist);
									xAddDatabaseBlock(dFermiBombers, true);
									xSetInt(dFermiBombers, xFermiBomberNext, trTimeMS());
									xSetVector(dFermiBombers, xFermiBomberDir, (bossDir * 4.0));
									xSetVector(dFermiBombers, xFermiBomberPos, pos);
									bossNext = bossNext + bossCount;
									bossCount = bossCount - 100;
								}
								if (trTimeMS() > bossTimeout) {
									trQuestVarSet("bossStep", 2);
								}
							}
							case 2:
							{
								if (xGetDatabaseCount(dFermiBombers) == 0) {
									trQuestVarSet("bossCooldownTime", trTimeMS() + 5000);
									trQuestVarSet("bossStep", 3);
								}
							}
							case 3:
							{
								if (trTimeMS() > trQuestVarGet("bossCooldownTime")) {
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
								trQuestVarSet("bossStep", 1);
								bossNext = trTimeMS();
								bossDir = vector(1,0,0);
								bossAngle = 0.1;
								bossScale = 0;
								bossTimeout = trTimeMS() + 20000;
							}
							case 1:
							{
								processPillarBall(dFallingFireballs);
								processPillarBall(dShadeBolts);
								processPillarBall(dBossWhirlpoolBalls);
								if (trTimeMS() > bossTimeout) {
									trQuestVarSet("bossPhase", 0);
								} else if (trTimeMS() > bossNext) {
									bossScale = fModulo(6.283185, bossScale + 0.1);
									bossAngle = fModulo(6.283185, bossAngle + 0.02);
									bossNext = bossNext + 100;
									bossDir = rotationMatrix(bossDir, xsCos(bossAngle), xsSin(bossAngle));

									bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
									addGenericProj(dFallingFireballs, trVectorQuestVarGet("firePillar"), bossDir, 1, 12.0 + 6.0 * xsCos(bossScale));

									bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
									addGenericProj(dShadeBolts, trVectorQuestVarGet("coralPillar"), bossDir, 0, 12.0 + 6.0 * xsCos(fModulo(6.283185, bossScale + 2.094395)));

									bossDir = rotationMatrix(bossDir, -0.5, 0.866025);
									addGenericProj(dBossWhirlpoolBalls, trVectorQuestVarGet("grassPillar"), bossDir, ENEMY_PLAYER, 12.0 + 6.0 * xsCos(fModulo(6.283185, bossScale + 4.18879)));
								}
							}
						}
					}
					case dBigBang:
					{
						
					}
				}
			}
			case 3:
			{
				while(trTimeMS() > bossNext) {
					bossNext = bossNext + 50;
					bossCount = bossCount - 1;
					timediff = bossCount;
					for(i=5; >= 0) {
						timediff = timediff + 30;
						if (timediff > 0) {
							transitionRing(timediff, 1*trQuestVarGet("nottud"+i));
							if (timediff == 20) {
								trQuestVarSetFromRand("sound", 1, 3, true);
								trSoundPlayFN("suckup"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
							}
						} else {
							paintMapTile(72, 72, 1*trQuestVarGet("nottud"+i));
						}
					}
					if (timediff <= 0) {
						trQuestVarSet("bossPhase", 2);
						trQuestVarSet("bossStep", 0);
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
