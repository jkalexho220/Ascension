const int EVENT_SYMPHONY_VOTE = 10010;
const int EVENT_SYMPHONY_CHOOSE = 10011;
const int EVENT_SYMPHONY_NEXT = 10012;

bool cameraWasLocked = false;

int dSpawnerRooms = 0;
int xSpawnerRoomPos = 0;

int dActiveSpawners = 0;
int xActiveSpawnerPos = 0;

int frontierUnitsArray = 0;
int relicShopOptions = 0;

bool notVoted = false;



void symphonyLevelUpIndividual(int count = 0, int p = 0) {
	int class = 0;
	int proto = 0;
	xSetPointer(dPlayerData, p);
	class = xGetInt(dPlayerData, xPlayerClass);
	proto = xGetInt(dClass, xClassProto, class);
	trModifyProtounit(kbGetProtoUnitName(proto), p, 5, count);
	xSetInt(dPlayerData, xPlayerLevel, count + xGetInt(dPlayerData, xPlayerLevel));
	trSetCivilizationNameOverride(p, "Level " + (xGetInt(dPlayerData, xPlayerLevel) + 1));
}

void symphonyLevelUp(int count = 0) {
	for(p=1; < ENEMY_PLAYER) {
		symphonyLevelUpIndividual(count, p);
	}
}

void symphonyChooseUpgrade(int eventID = -1) {
	cameraWasLocked = cameraLockOnSelf;
	cameraLockOnSelf = false;
	int choice = eventID - EVENT_SYMPHONY_CHOOSE;
	vector pos = xsVectorSet(30 + choice * 80, 0, 73);
	lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
	RunFunction("uiSetSpecialPower(220) uiSpecialPowerAtPointer");
}

vector symphonyRoomIndexToCoordinates(int room = 0) {
	int z = room / 5;
	int x = room - z * 5;
	return(xsVectorSet(x,0,z));
}

int symphonyCoordinatesToRoomIndex(int x = 0, int z = 0) {
	return(x + 5 * z);
}

// returns the center coordinates of the vector
vector symphonyRoomToVector(int room = 0) {
	return(symphonyRoomIndexToCoordinates(room) * 27 + xsVectorSet(5 + 14, 0, 5 + 14));
}


void symphonyAddFrontier(int x = 0, int z = 0, int cameFrom = 0) {
	if (x > 4 || z > 4 || x < 0 || z < 0) {
		return;
	} else {
		int room = symphonyCoordinatesToRoomIndex(x,z);
		vector center = symphonyRoomToVector(room);
		vector pos = gridToVector(center);
		if (terrainIsType(center, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			if (trQuestVarGet("bossRoomFound") == 0) {
				trQuestVarSetFromRand("rand", 1, trQuestVarGet("availableRooms"), true);
				if (trQuestVarGet("rand") == 1) {
					trQuestVarSet("bossRoomFound", 1);
					trQuestVarSet("room"+room, ROOM_SYMPHONY_BOSS);
				}
			}
			xAddDatabaseBlock(dFrontier, true);
			xSetInt(dFrontier, xUnitName, zGetInt(frontierUnitsArray, room));
			xSetInt(dFrontier, xFrontierCameFrom, cameFrom);
			xSetInt(dFrontier, xFrontierDest, room);
			xSetInt(dFrontier, xFrontierType, ROOM_CHEST);

			trPaintTerrain(xsVectorGetX(center)-1,xsVectorGetZ(center)-1,xsVectorGetX(center)+1,xsVectorGetZ(center)+1,TERRAIN_WALL,TERRAIN_SUB_WALL + 1,false);

			trQuestVarSet("availableRooms", trQuestVarGet("availableRooms") - 1);
		}
	}
}

void symphonyBuildEdge(int from = 0, int to = 0) {
	vector top = symphonyRoomToVector(1 * xsMax(from, to));
	vector bottom = symphonyRoomToVector(1 * xsMin(from, to));
	trPaintTerrain(xsVectorGetX(bottom) - 2, xsVectorGetZ(bottom) - 2,
		xsVectorGetX(top) + 2, xsVectorGetZ(top) + 2, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
	trChangeTerrainHeight(xsVectorGetX(bottom) - 2, xsVectorGetZ(bottom) - 2,
		xsVectorGetX(top) + 3, xsVectorGetZ(top) + 3, worldHeight, false);
	paintSecondary(xsVectorGetX(bottom) - 2, xsVectorGetZ(bottom) - 2, xsVectorGetX(top) + 2, xsVectorGetZ(top) + 2);
}

void symphonyAssignSpawnerRoom(int room = 0) {
	xAddDatabaseBlock(dSpawnerRooms, true);
	xSetInt(dSpawnerRooms, xUnitName, zGetInt(frontierUnitsArray, room));
	xSetVector(dSpawnerRooms, xSpawnerRoomPos, gridToVector(symphonyRoomToVector(room)));
}

void symphonyBuildRoom(int x = 0, int z = 0, int type = 0) {
	int room = symphonyCoordinatesToRoomIndex(x,z);
	int x0 = 0;
	int z0 = 0;
	int x1 = 0;
	int z1 = 0;

	vector center = symphonyRoomToVector(room);
	vector pos = vector(0,0,0);

	for (i=2; >0) {
		/*
		trQuestVarSetFromRand("x0", x * 27 + 5, x * 27 + 17, true);
		trQuestVarSetFromRand("z0", z * 27 + 5, z * 27 + 17, true);
		trQuestVarSetFromRand("x1", x * 27 + 15, x * 27 + 27, true);
		trQuestVarSetFromRand("z1", z * 27 + 15, z * 27 + 27, true);
		*/

		trQuestVarSetFromRand("rand", -13, -3, true);
		trQuestVarSetFromRand("rand2", -13, -3, true);
		x0 = xsVectorGetX(center) + trQuestVarGet("rand");
		z0 = xsVectorGetZ(center) + trQuestVarGet("rand2");

		trQuestVarSetFromRand("rand", 3, 13, true);
		trQuestVarSetFromRand("rand2", 3, 13, true);
		x1 = xsVectorGetX(center) + trQuestVarGet("rand");
		z1 = xsVectorGetZ(center) + trQuestVarGet("rand2");

		trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
		trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
		trVectorQuestVarSet("room"+room+"top"+i, xsVectorSet(x1,0,z1));
		trVectorQuestVarSet("room"+room+"bottom"+i, xsVectorSet(x0,0,z0));
		
	}

	trPaintTerrain(0,0,5,5,0,73,true);
	trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

	trUnitSelectClear();
	trUnitSelect("" + zGetInt(frontierUnitsArray, room));
	switch(type)
	{
		case ROOM_CHEST:
		{
			xAddDatabaseBlock(dChests, true);
			xSetInt(dChests,xUnitName,zGetInt(frontierUnitsArray, room));
			xSetInt(dChests,xChestType, CHEST_SYMPHONY);
			xSetInt(dChests,xChestRoom,room);

			
			trUnitChangeProtoUnit("Great Box");
		}
		case ROOM_SYMPHONY_UPGRADE:
		{
			trUnitChangeProtoUnit("Outpost");
		}
		case ROOM_BASIC:
		{
			xAddDatabaseBlock(dSpawnerRooms, true);
			xSetInt(dSpawnerRooms, xUnitName, zGetInt(frontierUnitsArray, room));
			xSetVector(dSpawnerRooms, xSpawnerRoomPos, gridToVector(symphonyRoomToVector(room)));
			trUnitSelectClear();
			trUnitSelect(""+zGetInt(frontierUnitsArray,room));
			trUnitChangeProtoUnit("Cinematic Block");
		}
	}

	for(i = -1; <= 1) {
		for(j = -1; <= 1) {
			if (xsAbs(i + j) == 1) {
				symphonyAddFrontier(x + i, z + j, room);
			}
		}
	}

	for(i=2; >0) {
		x0 = trVectorQuestVarGetX("room"+room+"bottom"+i);
		z0 = trVectorQuestVarGetZ("room"+room+"bottom"+i);

		x1 = trVectorQuestVarGetX("room"+room+"top"+i);
		z1 = trVectorQuestVarGetZ("room"+room+"top"+i);

		paintSecondary(x0, z0, x1, z1);
		paintEyecandy(x0, z0, x1, z1, "tree");
		paintEyecandy(x0, z0, x1, z1, "rock");
		paintEyecandy(x0, z0, x1, z1, "sprite");
	}
}


// unlocks at the frontier index of your choice
void symphonyUnlockRoom(int index = -1) {
	if (index > 0) {
		xSetPointer(dFrontier, index);
	}
	int room = xGetInt(dFrontier, xFrontierDest);
	int cameFrom = xGetInt(dFrontier, xFrontierCameFrom);
	int type = xGetInt(dFrontier, xFrontierType);
	vector pos = symphonyRoomIndexToCoordinates(room);

	xFreeDatabaseBlock(dFrontier);

	symphonyBuildEdge(cameFrom, room);
	symphonyBuildRoom(xsVectorGetX(pos),xsVectorGetZ(pos), type);
}

void symphonyCreateSpawners(int count = 0) {
	for(i=count; >0) {
		trQuestVarSetFromRand("rand", 0, xGetDatabaseCount(dSpawnerRooms), true);
		for(j=trQuestVarGet("rand"); > 0) {
			xDatabaseNext(dSpawnerRooms);
		}
		xAddDatabaseBlock(dActiveSpawners, true);
		xSetInt(dActiveSpawners, xUnitName, xGetInt(dSpawnerRooms, xUnitName));
		xSetVector(dActiveSpawners, xActiveSpawnerPos, xGetVector(dSpawnerRooms, xSpawnerRoomPos));
		xUnitSelect(dActiveSpawners, xUnitName);
		trUnitChangeProtoUnit("Sky Passage");
		xFreeDatabaseBlock(dSpawnerRooms);
	}
}

void symphonyResetSpawners() {
	for(i=xGetDatabaseCount(dActiveSpawners); >0) {
		xDatabaseNext(dActiveSpawners);
		xUnitSelect(dActiveSpawners, xUnitName);
		trUnitChangeProtoUnit("Cinematic Block");
		xAddDatabaseBlock(dSpawnerRooms, true);
		xSetInt(dSpawnerRooms, xUnitName, xGetInt(dActiveSpawners, xUnitName));
		xSetVector(dSpawnerRooms, xSpawnerRoomPos, xGetVector(dActiveSpawners, xActiveSpawnerPos));
	}
	xClearDatabase(dActiveSpawners);
}

void symphonyChooseNewProtos() {
	for(i=5; >0) {
		trQuestVarSetFromRand("rand", 1, 36, true);
		trQuestVarSetFromRand("rand2", trQuestVarGet("symphonyRound") * 4, 36 + trQuestVarGet("symphonyRound") * 4, true);
		if (trQuestVarGet("rand2") < trQuestVarGet("rand")) {
			trQuestVarSet("rand", trQuestVarGet("rand2"));
		}
		trStringQuestVarSet("enemyProto"+i, kbGetProtoUnitName(monsterPetProto(trQuestVarGet("rand"))));
	}
}

void symphonyCastVote(int eventID = -1) {
	notVoted = false;
	vector pos = trVectorQuestVarGet("lookingAt");
	lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
	RunFunction("uiSetSpecialPower(220) uiSpecialPowerAtPointer");
}

rule symphony_stride_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		trQuestVarSet("symphonyMode", 1);
		xsEnableRule("symphony_stride_cin_intro");

		trOverlayTextColour(200,200,0);

		trEventSetHandler(EVENT_SYMPHONY_VOTE, "symphonyCastVote");
		trEventSetHandler(EVENT_SYMPHONY_CHOOSE, "symphonyChooseUpgrade");
		trEventSetHandler(EVENT_SYMPHONY_CHOOSE + 1, "symphonyChooseUpgrade");

		trQuestVarSet("treeDensity", 0.05);
		trStringQuestVarSet("treeProto1", "Ruins");
		trStringQuestVarSet("treeProto2", "Healing SFX");
		trStringQuestVarSet("treeProto3", "Mist");
		trQuestVarSet("spriteDensity", 0.2);
		trStringQuestVarSet("spriteProto1", "Rock Granite Sprite");
		trStringQuestVarSet("spriteProto2", "Rock Limestone Sprite");
		trStringQuestVarSet("spriteProto3", "Healing SFX");
		trQuestVarSet("rockDensity", 0.1);
		trStringQuestVarSet("rockProto1", "Columns");
		trStringQuestVarSet("rockProto2", "Columns Broken");
		trStringQuestVarSet("rockProto3", "Columns Fallen");

		frontierUnitsArray = zNewArray(mInt, 25, "frontierUnits");
		relicShopOptions = zNewArray(mInt, NORMAL_RELICS, "relicShopOptions");

		trModifyProtounit("Healing Spring Object", 0, 55, 4); // flying spring
		trModifyProtounit("Revealer", 1, 2, 205);
		trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Revealer");

		vector center = vector(0,0,0);
		vector pos = vector(0,0,0);
		int val = 0;

		for(i=0; < NORMAL_RELICS) {
			zSetInt(relicShopOptions, i, i + 1);
		}
		for(i=0; < NORMAL_RELICS) {
			val = zGetInt(relicShopOptions, i);
			trQuestVarSetFromRand("rand", i, NORMAL_RELICS - 1, true);
			zSetInt(relicShopOptions, i, zGetInt(relicShopOptions, 1*trQuestVarGet("rand")));
			zSetInt(relicShopOptions, 1*trQuestVarGet("rand"), val);
		}

		trSetCivAndCulture(0, 0, 0);
		trSetCivAndCulture(ENEMY_PLAYER, 0, 0);

		worldHeight = 0;
		wallHeight = 5;

		TERRAIN_WALL = 2;
		TERRAIN_SUB_WALL = 1;

		TERRAIN_PRIMARY = 0;
		TERRAIN_SUB_PRIMARY = 53;

		TERRAIN_SECONDARY = 0;
		TERRAIN_SUB_SECONDARY = 50;

		trPaintTerrain(0,0,145,145,TERRAIN_WALL, TERRAIN_SUB_WALL, false);
		trChangeTerrainHeight(0,0,145,145,wallHeight);

		for(i=10; > 0) {
			/* Oracle */
			trTechSetStatus(ENEMY_PLAYER, 297, 4);
		}

		trQuestVarSet("availableRooms", 24);

		dFrontier = xInitDatabase("frontier");
		xInitAddInt(dFrontier, "unit");
		xFrontierDest = xInitAddInt(dFrontier, "destination");
		xFrontierCameFrom = xInitAddInt(dFrontier, "cameFrom");
		xFrontierType = xInitAddInt(dFrontier, "type");
		xFrontierVotes = xInitAddInt(dFrontier, "votes");

		dSpawnerRooms = xInitDatabase("spawner rooms");
		xInitAddInt(dSpawnerRooms, "unit");
		xSpawnerRoomPos = xInitAddVector(dSpawnerRooms, "pos");

		dActiveSpawners = xInitDatabase("active spawners");
		xInitAddInt(dActiveSpawners, "unit");
		xActiveSpawnerPos = xInitAddVector(dActiveSpawners, "pos");

		dNottudShop = xInitDatabase("nottudShop");
		xInitAddInt(dNottudShop,"name");
		xInitAddInt(dNottudShop,"type");
		xNottudShopPrice = xInitAddInt(dNottudShop, "price");
		xNottudShopPos = xInitAddVector(dNottudShop,"pos");

		/*
		for(i=0; < 5) {
			trQuestVarSet("room"+i, ROOM_CHEST);
		}
		for(i=5; < 10) {
			trQuestVarSet("room"+i, ROOM_LEVELUP);
		}
		for(i=10; < 13) {
			trQuestVarSet("room"+i, ROOM_GOLD);
		}
		for(i=13; < 16) {
			trQuestVarSet("room"+i, ROOM_FAVOR);
		}
		for(i=16; < 19) {
			trQuestVarSet("room"+i, ROOM_HEALING);
		}
		for(i=19; < 22) {
			trQuestVarSet("room"+i, ROOM_STAT_STORE);
		}
		trQuestVarSet("room22", ROOM_JESTER_BOSS);
		trQuestVarSet("room23", ROOM_PLAGUE_BOSS);
		trQuestVarSet("room24", ROOM_MOUNTAIN_BOSS);
		*/


		// shuffle
		for(i=0; < 25) {
			trQuestVarSetFromRand("rand", i, 24, true);
			val = trQuestVarGet("room"+i);
			trQuestVarSet("room"+i, trQuestVarGet("room"+1*trQuestVarGet("rand")));
			trQuestVarSet("room"+1*trQuestVarGet("rand"), val);

			trQuestVarSet("room"+i, ROOM_CHEST);
			pos = gridToVector(symphonyRoomToVector(i));
			zSetInt(frontierUnitsArray, i, trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),135,true);
			trArmySelect("0,0");
			trUnitChangeProtoUnit("Cinematic Block");
		}

		symphonyBuildRoom(2, 2, ROOM_CHEST);
		trUnitSelectClear();
		trUnitSelect(""+zGetInt(frontierUnitsArray,12));
		trUnitChangeProtoUnit("Great Box");

		trQuestVarSet("newestRoom", symphonyCoordinatesToRoomIndex(2,2));
		trQuestVarSet("newestRoomType", ROOM_CHEST);
		trQuestVarSet("symphonyRound", 0);
		
		for(i=0; < 2) {
			trQuestVarSetFromRand("rand",1,3,true);
			for(j=trQuestVarGet("rand"); >0) {
				xDatabaseNext(dFrontier);
			}
			xSetInt(dFrontier, xFrontierType, ROOM_BASIC);
			symphonyUnlockRoom();
		}

		for(p=1; < ENEMY_PLAYER) {
			trPlayerGrantResources(p, "gold", -9999);
			xSetInt(dPlayerData, xPlayerGold, 0, p);
			trQuestVarSet("p"+p+"upgradeOptions", zNewArray(mInt, 16));
			trQuestVarSet("p"+p+"upgrade1", -1);
			trQuestVarSet("p"+p+"upgrade2", -1);
			trQuestVarSet("p"+p+"upgrade3", -1);

			// populate
			for(i=0; < 16) {
				zSetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), i, i);
			}
			// shuffle
			for(i=0; < 16) {
				trQuestVarSetFromRand("rand", i, 15, true);
				val = zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), i);
				zSetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), i, zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1*trQuestVarGet("rand")));
				zSetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1*trQuestVarGet("rand"), val);
			}
		}

		trVectorQuestVarSet("startPosition", vector(145,0,145));

		trPaintTerrain(0,0,5,5,0,73,true);
		trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

		symphonyChooseNewProtos();

		xsEnableRule("symphony_stride_always");

		xsEnableRule("symphony_stride_cin_intro");
		// TODO: delete this
		//trQuestVarSet("cinStep", 4);
		//trQuestVarSet("cinTime", trTime() + 2);
		xsDisableSelf();
	}
}

rule symphony_stride_cin_intro
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
				trSoundPlayFN("default","1",-1,"Zenophobia:Welcome to Symphony of War, the endgame mode of Ascension!","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 2:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia:In this mode, your characters can surpass level 10! (Bonus is not saved)","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 7);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia:Face challenges of increasing difficulty to power up your characters and challenge the final boss!","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia:Good luck and have fun! To start, I'll grant you 1 level up immediately.","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				trSoundPlayFN("ageadvance.wav");
				trOverlayText("Level up!", 3.0);
			}
			case 5:
			{
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gameplay_start");
				//xsEnableRule("gladiator_worlds_portals");

				trQuestVarSet("killcount", 0);
				trQuestVarSet("killgoal", 10 + ENEMY_PLAYER);

				trMusicPlayCurrent();
				trPlayNextMusicTrack();

				symphonyLevelUp(1);

				xsEnableRule("symphony_stride_new_round");
				
				//trStringQuestVarSet("advice", "What do you mean you can't beat this? This is easy mode!");
			}
		}
	}
}

rule symphony_stride_new_round
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		xsEnableRule("symphony_stride_combat");
		trSoundPlayFN("fanfare.wav");
		trQuestVarSet("symphonyRound", 1 + trQuestVarGet("symphonyRound"));
		trOverlayText("Round " + 1*trQuestVarGet("symphonyRound"), 3.0);
		symphonyResetSpawners();
		symphonyChooseNewProtos();

		for(i=10; >0) {
			/* bacchanalia */
			trTechSetStatus(ENEMY_PLAYER, 78, 4);
		}

		/* monstrous rage */
		trTechSetStatus(ENEMY_PLAYER, 76, 4);

		trQuestVarSet("killcount", 0);
		trQuestVarSet("killgoal", 10 + trQuestVarGet("killgoal"));

		//todo :delete
		//trQuestVarSet("killgoal", 1);

		symphonyCreateSpawners(2 + (trQuestVarGet("symphonyRound") / 3));

		xsDisableSelf();
	}
}

rule symphony_stride_combat
inactive
highFrequency
{
	vector pos = vector(0,0,0);
	if ((xGetDatabaseCount(dEnemies) + trQuestVarGet("killcount") < trQuestVarGet("killgoal")) && (xGetDatabaseCount(dEnemies) < (3 * trQuestVarGet("symphonyRound")))) {
		xDatabaseNext(dActiveSpawners);
		pos = xGetVector(dActiveSpawners, xActiveSpawnerPos);
		int next = 0;
		trQuestVarSetFromRand("rand", 5, 10);
		trQuestVarSetFromRand("proto", 1, 5, true);
		//trQuestVarSet("rand", 1); // TODO: delete

		trQuestVarSet("killcount", trQuestVarGet("killcount") + trQuestVarGet("rand"));
		for(i=trQuestVarGet("rand"); >0) {
			trQuestVarSetFromRand("heading", 1, 360, true);
			next = trGetNextUnitScenarioNameNumber();
			trArmyDispatch(""+ENEMY_PLAYER+",0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("proto")),1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
			if (next < trGetNextUnitScenarioNameNumber()) {
				trQuestVarSetFromRand("relicdrop", 0, 1, false);
				if (trQuestVarGet("relicdrop") < 0.03) {
					trQuestVarSetFromRand("randrelic", 1, 30);
				} else {
					trQuestVarSet("randrelic", 0);
				}
				activateEnemy(next, -1, trQuestVarGet("randrelic"));
			}
		}
	}

	// enemies wander
	if (xGetDatabaseCount(dEnemies) > 0) {
		if ((kbUnitGetAnimationActionType(xGetInt(dEnemies, xUnitID)) == 9) && (xGetInt(dEnemies, xStunStatus) == 0)) {
			pos = xGetVector(dPlayerUnits, xUnitPos);
			xUnitSelectByID(dEnemies, xUnitID);
			trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,true);
		}
	}

	// round over
	if ((trQuestVarGet("killcount") >= trQuestVarGet("killgoal")) && (xGetDatabaseCount(dEnemies) == 0)) {
		pos = gridToVector(symphonyRoomToVector(1*trQuestVarGet("newestRoom")));
		trackInsert();
		trackAddWaypoint();
		lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
		trackAddWaypoint();
		trackPlay(1000, -1);

		if (iModulo(3, 1*trQuestVarGet("symphonyRound")) == 2) {
			trMessageSetText("Round end. Choose a Symphony.", -1);
		} else {
			trMessageSetText("Round end. Level up!", -1);
			symphonyLevelUp(1);
		}

		trSoundPlayFN("fanfare.wav");
		xsEnableRule("symphony_round_end_rewards");
		
		xsDisableSelf();
	}
}

rule symphony_round_end_rewards
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		vector pos = vector(0,0,0);
		int proto = 0;
		int index = 0;
		int relic = 0;
		int next = 0;
		int room = trQuestVarGet("newestRoom");
		trQuestVarSet("cinStep", 0);
		switch(1*trQuestVarGet("newestRoomType"))
		{
			case ROOM_SYMPHONY_UPGRADE:
			{
				trSoundPlayFN("sentinelbirth.wav");
				trQuestVarSet("symphonyUpgradeNumber", 1 + trQuestVarGet("symphonyUpgradeNumber"));
				trQuestVarSet("symphonyUpgradeStart", 3 + trQuestVarGet("symphonyUpgradeStart"));
				xsEnableRule("symphony_upgrade");
				
				notVoted = true;
				int p = 0;
				for(p=1; < ENEMY_PLAYER) {
					trTechGodPower(p, "vision", 1);
				}

				p = trCurrentPlayer();

				trShowChoiceDialog("Choose a Symphony", symphonyName(zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1 * trQuestVarGet("symphonyUpgradeStart"))), EVENT_SYMPHONY_CHOOSE,
					symphonyName(zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1 + trQuestVarGet("symphonyUpgradeStart"))), EVENT_SYMPHONY_NEXT);

				trQuestVarSet("voteCount", ENEMY_PLAYER - 1);
				trQuestVarSet("votingUnitTracker", trGetNextUnitScenarioNameNumber());
				trQuestVarSet("votingCountdown", 0);

				trCounterAddTime("symphonyCountdown", 60, 1, "Next round begins", -1);
				trQuestVarSet("cinTime", trTime() + 60);
			}
			case ROOM_CHEST:
			{
				xDatabaseNext(dChests);
				xSetInt(dChests, xChestState, CHEST_STATE_UNLOCKED);
				trQuestVarSet("cinTime", trTime() + 30);
				trCounterAddTime("symphonyCountdown", 30, 1, "Next round begins", -1);
			}
			case ROOM_LEVELUP:
			{
				trOverlayText("Level up!", 3.0);
				trSoundPlayFN("ageadvance.wav");
				pos = gridToVector(symphonyRoomToVector(1*trQuestVarGet("newestRoom")));
				trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Vision SFX");
				symphonyLevelUp(2);
			}
			case ROOM_GOLD:
			{
				trSoundPlayFN("plentybirth.wav");
				for(p=1; < ENEMY_PLAYER) {
					xSetInt(dPlayerData, xPlayerGold, 500 + xGetInt(dPlayerData, xPlayerGold, p), p);
					trPlayerGrantResources(p, "gold", 500);
				}
			}
			case ROOM_HEALING:
			{
				trSoundPlayFN("restorationbirth.wav");
				trMessageSetText("All players gain +10 health regen per second.", -1);
				for(p=1; < ENEMY_PLAYER) {
					xSetFloat(dPlayerData, xPlayerHealthRegen, xGetFloat(dPlayerData, xPlayerHealthRegen, p) + 10.0, p);
				}
			}
			case ROOM_FAVOR:
			{
				trSoundPlayFN("oracledone.wav");
				trMessageSetText("All players gain +0.5 favor regen per second.", -1);
				for(p=1; < ENEMY_PLAYER) {
					xSetFloat(dPlayerData, xPlayerFavorRegen, xGetFloat(dPlayerData, xPlayerFavorRegen, p) + 0.5, p);
				}
			}
			case ROOM_STAT_STORE:
			{
				trUnitSelectClear();
				trUnitSelect(""+zGetInt(frontierUnitsArray, room));
				trUnitChangeProtoUnit("Cinematic Block");
				trSoundPlayFN("market.wav");
				pos = symphonyRoomToVector(room);
				trPaintTerrain(xsVectorGetX(pos) - 10, xsVectorGetZ(pos) - 10,
					xsVectorGetX(pos) + 10, xsVectorGetZ(pos) + 10,
					TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
				trChangeTerrainHeight(xsVectorGetX(pos) - 10, xsVectorGetZ(pos) - 10,
					xsVectorGetX(pos) + 11, xsVectorGetZ(pos) + 11,
					worldHeight,true);
				paintSecondary(xsVectorGetX(pos) - 10, xsVectorGetZ(pos) - 10,
					xsVectorGetX(pos) + 10, xsVectorGetZ(pos) + 10);
				pos = gridToVector(pos) - vector(10,0,10);
				for(i=0; <= 1) {
					for(j=0; <= 1) {
						trQuestVarSetFromRand("rand", index, NORMAL_RELICS - 1, true);
						relic = zGetInt(relicShopOptions, 1*trQuestVarGet("rand"));
						zSetInt(relicShopOptions, 1*trQuestVarGet("rand"), zGetInt(relicShopOptions, index));
						zSetInt(relicShopOptions, index, relic);

						proto = relicProto(relic);
						next = trGetNextUnitScenarioNameNumber();
						
						trArmyDispatch("0,0",kbGetProtoUnitName(proto),1,xsVectorGetX(pos) + 20 * i, 0, xsVectorGetZ(pos) + 20 * j, 225, true);
						
						trQuestVarSetFromRand("rand", 300, 600, true);
						xAddDatabaseBlock(dNottudShop, true);
						xSetInt(dNottudShop, xUnitName, next);
						xSetInt(dNottudShop, xRelicType, relic);
						xSetVector(dNottudShop, xNottudShopPos, pos + xsVectorSet(20 * i, 0, 20 * j));
						xSetInt(dNottudShop, xNottudShopPrice, trQuestVarGet("rand"));
						
						xAddDatabaseBlock(dStunnedUnits, true);
						xSetInt(dStunnedUnits, xUnitName, next);
						xSetInt(dStunnedUnits, xStunnedProto, proto);


						
						index = index + 1;
					}
				}
			}
		}
		xsDisableSelf();
		xsEnableRule("symphony_stride_round_done");
	}
}

rule symphony_upgrade
inactive
highFrequency
{
	if (trQuestVarGet("voteCount") == 0) {
		xsDisableSelf();
	} else {
		int id = 0;
		int p = 0;
		vector pos = vector(0,0,0);
		for(n=trQuestVarGet("votingUnitTracker"); < trGetNextUnitScenarioNameNumber()) {
			id = kbGetBlockID(""+n, true);
			if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Vision Revealer")) {
				p = kbUnitGetOwner(id);
				trQuestVarSet("voteCount", trQuestVarGet("voteCount") - 1);

				if (xsVectorGetX(kbGetBlockPosition(""+n, true)) < 70) {
					trQuestVarSet("p"+p+"upgrade"+1*trQuestVarGet("symphonyUpgradeNumber"), zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1 * trQuestVarGet("symphonyUpgradeStart")));
				} else {
					trQuestVarSet("p"+p+"upgrade"+1*trQuestVarGet("symphonyUpgradeNumber"), zGetInt(1*trQuestVarGet("p"+p+"upgradeOptions"), 1 + trQuestVarGet("symphonyUpgradeStart")));
				}

				trQuestVarSetFromRand("rand", 1, 5, true);
				trSoundPlayFN("ui\thunder"+1*trQuestVarGet("rand")+".wav");

				if (trCurrentPlayer() == p) {
					if (cameraWasLocked) {
						cameraLockOnSelf = true;
					}
					reselectMyself(true);
					trMessageSetText("Chosen: " + symphonyName(1*trQuestVarGet("p"+p+"upgrade"+1*trQuestVarGet("symphonyUpgradeNumber"))), -1);
				}

				switch(1*trQuestVarGet("p"+p+"upgrade"+1*trQuestVarGet("symphonyUpgradeNumber")))
				{
					case SYMPHONY_LASER_ATTACKS:
					{
						trQuestVarSet("p"+p+"laserAttacks", 100);
					}
					case SYMPHONY_MORE_PHYSICAL:
					{
						xSetPointer(dPlayerData, p);
						xSetFloat(dPlayerData, xPlayerBaseAttackTrue, xGetFloat(dPlayerData, xPlayerBaseAttackTrue) * 1.2);
						xSetFloat(dPlayerData, xPlayerHealth, xGetFloat(dPlayerData, xPlayerHealth) * 1.2);
						relicEffect(RELIC_ARMOR, p, true);
						relicEffect(RELIC_ATTACK_DAMAGE, p, true);
						relicEffect(RELIC_ATTACK_DAMAGE, p, false);
						relicEffect(RELIC_HEALTH, p, true);
						relicEffect(RELIC_HEALTH, p, false);
					}
					case SYMPHONY_MORE_MAGICAL:
					{
						xSetPointer(dPlayerData, p);
						xSetFloat(dPlayerData, xPlayerSpellDamage, xGetFloat(dPlayerData, xPlayerSpellDamage) * 1.2);
						xSetFloat(dPlayerData, xPlayerSpellRange, xGetFloat(dPlayerData, xPlayerSpellRange) * 1.2);
						xSetFloat(dPlayerData, xPlayerSpellDuration, xGetFloat(dPlayerData, xPlayerSpellDuration) * 1.2);
						relicEffect(RELIC_SPELL_POWER, p, true);
						relicEffect(RELIC_SPELL_POWER, p, false);
						relicEffect(RELIC_SPELL_DURATION, p, true);
						relicEffect(RELIC_SPELL_DURATION, p, false);
						relicEffect(RELIC_SPELL_RANGE, p, true);
						relicEffect(RELIC_SPELL_RANGE, p, false);
					}
					case SYMPHONY_REGENERATE_FAVOR:
					{
						xSetPointer(dPlayerData, p);
						xSetFloat(dPlayerData, xPlayerFavorRegen, 1.0 + xGetFloat(dPlayerData, xPlayerFavorRegen));
					}
					case SYMPHONY_GAIN_LEVELS:
					{
						symphonyLevelUpIndividual(3, p);
					}
					case SYMPHONY_GAIN_ATTACK:
					{
						xSetPointer(dPlayerData, p);
						xSetFloat(dPlayerData, xPlayerBaseAttackTrue, xGetFloat(dPlayerData, xPlayerBaseAttackTrue) + 5);
						relicEffect(RELIC_ATTACK_DAMAGE, p, true);
						relicEffect(RELIC_ATTACK_DAMAGE, p, true);
						relicEffect(RELIC_ATTACK_DAMAGE, p, true);
					}
					case SYMPHONY_GAIN_POWER:
					{
						xSetPointer(dPlayerData, p);
						xSetFloat(dPlayerData, xPlayerSpellDamage, xGetFloat(dPlayerData, xPlayerSpellDamage) + 0.1);
						relicEffect(RELIC_SPELL_POWER, p, true);
						relicEffect(RELIC_SPELL_POWER, p, true);
						relicEffect(RELIC_SPELL_POWER, p, true);
					}
				}

				if (trQuestVarGet("voteCount") == 0) {
					trQuestVarSet("cinTime", trTime() + 20);
					trCounterAbort("symphonyCountdown");
					trCounterAddTime("symphonyCountdown", 20, 1, "Next round begins", -1);
				}
			}
		}
	}
}

rule symphony_stride_always
inactive
highFrequency
{

	trUnitSelectClear();
	trUnitSelect(""+zGetInt(frontierUnitsArray, 1*trQuestVarGet("newestRoom")));
	if (trUnitIsSelected()) {
		reselectMyself();
		switch(1*trQuestVarGet("newestRoomType"))
		{
		case ROOM_CHEST:
			{
				uiMessageBox("This chest is unlocked at the end of this round.");
			}
		case ROOM_SYMPHONY_UPGRADE:
			{
				uiMessageBox("Choose a Symphony at the end of this round.");
			}
		}
	}

	if (xGetDatabaseCount(dNottudShop) > 0) {
		xDatabaseNext(dNottudShop);
		xUnitSelect(dNottudShop,xUnitName);
		if (trUnitIsSelected()) {
			int relic = xGetInt(dNottudShop, xRelicType);
			trShowImageDialog(relicIcon(relic), relicName(relic) + " (" + xGetInt(dNottudShop, xNottudShopPrice) + " gold)");
			
			reselectMyself();
		}
	}
}

rule symphony_stride_round_done
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trCounterAbort("symphonyCountdown");
				xsDisableRule("symphony_upgrade");
				trMessageSetText("A new room has opened!", -1);
				trQuestVarSet("cinTime", trTime() + 1);
				trQuestVarSetFromRand("rand",0,xGetDatabaseCount(dFrontier), true);
				for(i=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dFrontier);
				}
				trQuestVarSet("newRoom", xGetPointer(dFrontier));

				if (iModulo(3, 1*trQuestVarGet("symphonyRound")) == 1) {
					xSetInt(dFrontier, xFrontierType, ROOM_SYMPHONY_UPGRADE);
				}

				vector pos = gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest)));
				trackInsert();
				trackAddWaypoint();
				lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
				trackAddWaypoint();
				trackPlay(1000, -1);
			}
			case 2:
			{
				trSoundPlayFN("wall.wav");

				// add the old room to the enemy spawners
				xAddDatabaseBlock(dSpawnerRooms, true);
				xSetInt(dSpawnerRooms, xUnitName, zGetInt(frontierUnitsArray, 1*trQuestVarGet("newestRoom")));
				xSetVector(dSpawnerRooms, xSpawnerRoomPos, gridToVector(symphonyRoomToVector(1*trQuestVarGet("newestRoom"))));
				
				xSetPointer(dFrontier, 1*trQuestVarGet("newRoom"));

				trQuestVarSet("newestRoom", xGetInt(dFrontier, xFrontierDest));
				trQuestVarSet("newestRoomType", xGetInt(dFrontier, xFrontierType));

				symphonyUnlockRoom();

				trUIFadeToColor(200,200,200,1000,1,false);
				trQuestVarSet("cinTime", trTime() + 1);
			}
			case 3:
			{
				xsDisableSelf();
				trLetterBox(false);
				reselectMyself(true);

				xsEnableRule("symphony_stride_new_round");
			}
		}
	}
}
