int dMouthOfChaos = 0;
int bullshitProj = 0;

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
					/* 100% health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=4; >0) {
					/* 100% attack monstrous rage x4 */
					trTechSetStatus(ENEMY_PLAYER, 76, 4);
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
					/* 100% health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=4; >0) {
					/* 100% attack monstrous rage x4 */
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
					/* 100% health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=4; >0) {
					/* 100% attack monstrous rage x4 */
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
					/* 100% health bacchanalia x 20 */
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}

				for(i=8; >0) {
					/* 100% attack monstrous rage x4 */
					trTechSetStatus(ENEMY_PLAYER, 76, 4);
				}
				
				trStringQuestVarSet("advice", "You were so close! I believe in you!");
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
					trQuestVarSetFromRand("randrelic", 1, 20);
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
		trQuestVarSetFromRand("rand", trQuestVarGet("gladiatorRound"), 4, true);
		xsEnableRule("gladiator_worlds_build_"+1*trQuestVarGet("map"+1*trQuestVarGet("rand")));
		trQuestVarSet("map"+1*trQuestVarGet("rand"), trQuestVarGet("map"+1*trQuestVarGet("gladiatorRound")));

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
	dEdgeFrontier = xInitDatabase("edgeFrontier");
	xEdgeFrontierHeight = xInitAddInt(dEdgeFrontier, "height");
	xEdgeFrontierLoc = xInitAddVector(dEdgeFrontier, "location");
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
	dEdgeFrontier = xInitDatabase("edgeFrontier");
	xEdgeFrontierHeight = xInitAddInt(dEdgeFrontier, "height");
	xEdgeFrontierLoc = xInitAddVector(dEdgeFrontier, "location");
	for(i=0; < 145) {
		if (aiPlanAddUserVariableFloat(dMapTiles,i,"row"+i,144) == false) {
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
	dEdgeFrontier = xInitDatabase("edgeFrontier");
	xEdgeFrontierHeight = xInitAddInt(dEdgeFrontier, "height");
	xEdgeFrontierLoc = xInitAddVector(dEdgeFrontier, "location");
	for(i=0; < 145) {
		if (aiPlanAddUserVariableFloat(dMapTiles,i,"row"+i,144) == false) {
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