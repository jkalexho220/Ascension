const int EVENT_SYMPHONY_VOTE = 10010;

int dSpawnerRooms = 0;
int xSpawnerRoomPos = 0;

int dActiveSpawners = 0;
int xActiveSpawnerPos = 0;

int frontierUnitsArray = 0;
int petShopOptions = 0;

bool notVoted = false;

void symphonyLevelUp(int count = 0) {
	int class = 0;
	int proto = 0;
	for(p=1; < ENEMY_PLAYER) {
		xSetPointer(dPlayerData, p);
		class = xGetInt(dPlayerData, xPlayerClass);
		proto = xGetInt(dClass, xClassProto, class);
		trModifyProtounit(kbGetProtoUnitName(proto), p, 5, count);
		xSetInt(dPlayerData, xPlayerLevel, count + xGetInt(dPlayerData, xPlayerLevel));
		trSetCivilizationNameOverride(p, "Level " + (xGetInt(dPlayerData, xPlayerLevel) + 1));
	}
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

string symphonyRoomDescription(int type = 0) {
	string msg = "How in the fuck did you get here???";
	switch(type)
	{
		case ROOM_CHEST:
		{
			msg = "Relics";
		}
		case ROOM_GOLD:
		{
			msg = "Gold";
		}
		case ROOM_LEVELUP:
		{
			msg = "Level Up";
		}
		case ROOM_HEALING:
		{
			msg = "Healing Room";
		}
		case ROOM_PET_STORE:
		{
			msg = "Pet Shop";
		}
		case ROOM_REVIVES:
		{
			msg = "Refresh Revives";
		}
	}
	return(msg);
}

string symphonyRoomProtoUnit(int type = 0) {
	string proto = "Outpost";
	switch(type)
	{
		case ROOM_CHEST:
		{
			proto = "Great Box";
		}
		case ROOM_GOLD:
		{
			proto = "Gold Mine";
		}
		case ROOM_LEVELUP:
		{
			proto = "Outpost";
		}
		case ROOM_HEALING:
		{
			proto = "Healing Spring Object";
		}
		case ROOM_PET_STORE:
		{
			proto = "Villager Atlantean";
		}
		case ROOM_REVIVES:
		{
			proto = "Athena";
		}
	}
	return(proto);
}

void symphonyAddFrontier(int x = 0, int z = 0, int cameFrom = 0) {
	if (x > 4 || z > 4 || x < 0 || z < 0) {
		return;
	} else {
		int room = symphonyCoordinatesToRoomIndex(x,z);
		vector center = symphonyRoomToVector(room);
		vector pos = gridToVector(center);
		if (terrainIsType(center, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			xAddDatabaseBlock(dFrontier, true);
			xSetInt(dFrontier, xUnitName, zGetInt(frontierUnitsArray, room));
			xSetInt(dFrontier, xFrontierCameFrom, cameFrom);
			xSetInt(dFrontier, xFrontierDest, room);
			xSetInt(dFrontier, xFrontierType, trQuestVarGet("room"+room));

			trPaintTerrain(xsVectorGetX(center)-3,xsVectorGetZ(center)-3,xsVectorGetX(center)+3,xsVectorGetZ(center)+3,0,73,false);

			xUnitSelect(dFrontier, xUnitName);
			if (trQuestVarGet("room"+room) > ROOM_HEALING) {
				trSetUnitOrientation(vector(-0.707107,0,-0.707107),vector(0,1,0),true);
			}
			trUnitChangeProtoUnit(symphonyRoomProtoUnit(xGetInt(dFrontier, xFrontierType)));
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

	switch(type)
	{
		case ROOM_CHEST:
		{
			xAddDatabaseBlock(dChests, true);
			xSetInt(dChests,xUnitName,zGetInt(frontierUnitsArray, room));
			xSetInt(dChests,xChestType, CHEST_SYMPHONY);
			xSetInt(dChests,xChestRoom,room);
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
		petShopOptions = zNewArray(mInt, 36, "petOptions");

		trModifyProtounit("Healing Spring Object", 0, 55, 4); // flying spring
		trModifyProtounit("Revealer", 1, 2, 205);
		trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Revealer");

		vector center = vector(0,0,0);
		vector pos = vector(0,0,0);
		int val = 0;

		for(i=0; < 36) {
			zSetInt(petShopOptions, i, i);
		}
		for(i=0; < 36) {
			val = zGetInt(petShopOptions, i);
			trQuestVarSetFromRand("rand", i, 35, true);
			zSetInt(petShopOptions, i, zGetInt(petShopOptions, 1*trQuestVarGet("rand")));
			zSetInt(petShopOptions, 1*trQuestVarGet("rand"), val);
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

		for(i=0; < 6) {
			trQuestVarSet("room"+i, ROOM_CHEST);
		}
		for(i=6; < 12) {
			trQuestVarSet("room"+i, ROOM_GOLD);
		}
		for(i=12; < 18) {
			trQuestVarSet("room"+i, ROOM_LEVELUP);
		}
		for(i=18; < 22) {
			trQuestVarSet("room"+i, ROOM_HEALING);
		}
		for(i=22; < 25) {
			trQuestVarSet("room"+i, ROOM_PET_STORE);
		}

		// shuffle
		for(i=0; < 25) {
			trQuestVarSetFromRand("rand", i, 24, true);
			val = trQuestVarGet("room"+i);
			trQuestVarSet("room"+i, trQuestVarGet("room"+1*trQuestVarGet("rand")));
			trQuestVarSet("room"+1*trQuestVarGet("rand"), val);

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
				trSoundPlayFN("default","1",-1,"Zenophobia:Good luck and have fun! To start, I'll grant you 2 level ups immediately.","icons\infantry g hoplite icon 64");
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

				symphonyLevelUp(2);

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
		if (iModulo(2, 1 * trQuestVarGet("symphonyRound")) == 0) {
			trTechSetStatus(ENEMY_PLAYER, 76, 4);
		}

		trQuestVarSet("killcount", 0);
		trQuestVarSet("killgoal", 10 + trQuestVarGet("killgoal"));

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
		// trQuestVarSet("rand", 1); // TODO: delete

		trQuestVarSet("killcount", trQuestVarGet("killcount") + trQuestVarGet("rand"));
		for(i=trQuestVarGet("rand"); >0) {
			trQuestVarSetFromRand("heading", 1, 360, true);
			next = trGetNextUnitScenarioNameNumber();
			trArmyDispatch(""+ENEMY_PLAYER+",0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("proto")),1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
			if (next < trGetNextUnitScenarioNameNumber()) {
				trQuestVarSetFromRand("relicdrop", 0, 1, false);
				if (trQuestVarGet("relicdrop") < 0.02) {
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
		

		trSoundPlayFN("fanfare.wav");
		trOverlayText("Round Over!", 3.0);
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
		int next = 0;
		int room = trQuestVarGet("newestRoom");
		switch(1*trQuestVarGet("newestRoomType"))
		{
			case ROOM_CHEST:
			{
				xDatabaseNext(dChests);
				xSetInt(dChests, xChestState, CHEST_STATE_UNLOCKED);
			}
			case ROOM_LEVELUP:
			{
				trSoundPlayFN("ageadvance.wav");
				pos = gridToVector(symphonyRoomToVector(1*trQuestVarGet("newestRoom")));
				trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Vision SFX");
				symphonyLevelUp(1);
			}
			case ROOM_GOLD:
			{
				trSoundPlayFN("plentybirth.wav");
				for(p=1; < ENEMY_PLAYER) {
					trPlayerGrantResources(p, "gold", 500);
				}
			}
			case ROOM_HEALING:
			{
				trUnitSelectClear();
				trUnitSelect(""+zGetInt(frontierUnitsArray, room));
				pos = gridToVector(symphonyRoomToVector(room));
				trUnitChangeProtoUnit("Cinematic Block");
				trTechGodPower(0, "healing spring", 1);
				trTechInvokeGodPower(0, "healing spring", pos, pos);
			}
			case ROOM_PET_STORE:
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
						index = zGetInt(petShopOptions, 1*trQuestVarGet("petShopNext"));
						proto = monsterPetProto(index);
						next = trGetNextUnitScenarioNameNumber();
						
						trArmyDispatch("0,0",kbGetProtoUnitName(proto),1,xsVectorGetX(pos) + 20 * i, 0, xsVectorGetZ(pos) + 20 * j, 225, true);
						
						trQuestVarSetFromRand("rand", 100 + index, 200 + index * 2, true);
						xAddDatabaseBlock(dNottudShop, true);
						xSetInt(dNottudShop, xUnitName, next);
						xSetInt(dNottudShop, xRelicType, proto);
						xSetVector(dNottudShop, xNottudShopPos, pos + xsVectorSet(20 * i, 0, 20 * j));
						xSetInt(dNottudShop, xNottudShopPrice, trQuestVarGet("rand"));
						
						xAddDatabaseBlock(dStunnedUnits, true);
						xSetInt(dStunnedUnits, xUnitName, next);
						xSetInt(dStunnedUnits, xStunnedProto, proto);
						trQuestVarSet("petShopNext", 1 + trQuestVarGet("petShopNext"));
					}
				}
			}
		}
		xsEnableRule("symphony_start_voting");
		xsDisableSelf();
	}
}

rule symphony_start_voting
inactive
highFrequency
{
	if (trTime() > cActivationTime + 2) {
		if (trQuestVarGet("symphonyRound") == 1) {
			xsEnableRule("symphony_stride_cin_vote");
			trQuestVarSet("cinStep",0);
			trQuestVarSet("cinTime", trTime() + 1);
			trLetterBox(true);
		} else {
			trMessageSetText("You may vote for your next destination now.", -1);
			reselectMyself(true);
		}

		trUnitSelectClear();
		trUnitSelect(""+zGetInt(frontierUnitsArray, 1*trQuestVarGet("newestRoom")));
		trUnitChangeProtoUnit("Cinematic Block");
		
		notVoted = true;
		for(p=1; < ENEMY_PLAYER) {
			trTechGodPower(p, "vision", 1);
		}
		trQuestVarSet("voteCount", ENEMY_PLAYER - 1);
		trQuestVarSet("votingUnitTracker", trGetNextUnitScenarioNameNumber());
		trQuestVarSet("votingCountdown", 0);
		xsEnableRule("symphony_stride_voting");
		xsDisableSelf();
	}
}

rule symphony_stride_always
inactive
highFrequency
{
	xDatabaseNext(dFrontier);
	xUnitSelect(dFrontier, xUnitName);
	if (trUnitIsSelected()) {
		reselectMyself();
		string description = symphonyRoomDescription(xGetInt(dFrontier, xFrontierType));
		if (notVoted) {
			trVectorQuestVarSet("lookingAt", gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest))));
			trShowChoiceDialog(description, "Vote for this room", EVENT_SYMPHONY_VOTE, "Cancel", -1);
		} else {
			uiMessageBox(description);
		}
	}

	trUnitSelectClear();
	trUnitSelect(""+zGetInt(frontierUnitsArray, 1*trQuestVarGet("newestRoom")));
	if (trUnitIsSelected()) {
		reselectMyself();
		uiMessageBox("This reward is unlocked at the end of this round.");
	}

	if (xGetDatabaseCount(dNottudShop) > 0) {
		xDatabaseNext(dNottudShop);
		xUnitSelect(dNottudShop,xUnitName);
		if (trUnitIsSelected()) {
			int proto = xGetInt(dNottudShop, xRelicType);
			uiMessageBox(kbGetProtoUnitName(proto) + " (" + xGetInt(dNottudShop, xNottudShopPrice) + "g)");
			
			reselectMyself();
		}
	}
}

rule symphony_stride_voting
inactive
highFrequency
{
	int id = 0;
	int p = 0;
	int next = 0;
	int closest = 0;
	int destUnit = 0;
	float closestDistance = 0;
	float currentDistance = 0;
	vector center = vector(0,0,0);
	vector pos = vector(0,0,0);
	for(n = trQuestVarGet("votingUnitTracker"); < trGetNextUnitScenarioNameNumber()) {
		id = kbGetBlockID(""+n, true);
		if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Vision Revealer")) {
			center = kbGetBlockPosition(""+n, true);
			p = kbUnitGetOwner(id);
			trUnitSelectClear();
			trUnitSelect("" + n, true);
			//trUnitChangeProtoUnit("Folstag Flag Bearer"); //TODO: make it deploy there instead
			trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has voted!");
			trQuestVarSet("voteCount", trQuestVarGet("voteCount") - 1);

			closestDistance = 100000;
			for(i=xGetDatabaseCount(dFrontier); >0) {
				xDatabaseNext(dFrontier);
				pos = gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest)));
				currentDistance = distanceBetweenVectors(pos, center);
				if (currentDistance < closestDistance) {
					closest = xGetPointer(dFrontier);
					closestDistance = currentDistance;
				}
			}

			xSetPointer(dFrontier, closest);
			pos = gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest)));
			xSetInt(dFrontier, xFrontierVotes, 1 + xGetInt(dFrontier, xFrontierVotes));

			destUnit = zGetInt(frontierUnitsArray, xGetInt(dFrontier, xFrontierDest));
			trUnitSelectClear();
			trUnitSelect(""+destUnit);
			trUnitChangeProtoUnit("Transport Ship Greek");

			trQuestVarSet("p"+p+"senator", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Folstag Flag Bearer", 1, 1, 0, 1, 0, true);
			trArmySelect("0,0");
			trImmediateUnitGarrison("" + destUnit);

			trUnitSelectClear();
			trUnitSelect(""+destUnit);
			trUnitChangeProtoUnit(symphonyRoomProtoUnit(xGetInt(dFrontier, xFrontierType)));

			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"senator");
			trUnitConvert(p);

			trMinimapFlare(p, 10.0, pos, true);

			if ((xGetInt(dFrontier, xFrontierVotes) >= (ENEMY_PLAYER / 2)) || (trQuestVarGet("voteCount") == 0)) {
				xsEnableRule("symphony_stride_voting_done");
				trQuestVarSet("cinStep", 0);
				trQuestVarSet("cinTime", trTime() + 1);
				trCounterAbort("votingCounter");
				xsDisableSelf();
				break;
			} else if (trQuestVarGet("votingCountdown") == 0) {
				trQuestVarSet("votingCountdown", 1);
				trQuestVarSet("votingTimeout", trTime() + 60);
				trCounterAddTime("votingCounter", 60, 0, "Voting Ends", -1);
			}
		}
	}

	if ((trQuestVarGet("votingCountdown") == 1) && (trTime() > trQuestVarGet("votingTimeout"))) {
		xsEnableRule("symphony_stride_voting_done");
		trQuestVarSet("cinStep", 0);
		trQuestVarSet("cinTime", trTime() + 1);
		xsDisableSelf();
	}

	trQuestVarSet("votingUnitTracker", trGetNextUnitScenarioNameNumber());
}

rule symphony_stride_voting_done
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
				//trMusicPlay("xpack\xcinematics\10_a\music.mp3", "1", 0);
				trSoundPlayFN("default","1",-1,"The votes have been decided.");
				trQuestVarSet("cinTime", trTime() + 3);
				trQuestVarSet("newRoom", xGetPointer(dFrontier));
				for(i=xGetDatabaseCount(dFrontier); >0) {
					xDatabaseNext(dFrontier);
					if (xGetInt(dFrontier, xFrontierVotes) > xGetInt(dFrontier, xFrontierVotes, 1*trQuestVarGet("newRoom"))) {
						trQuestVarSet("newRoom", xGetPointer(dFrontier));
					}
				}

				xSetPointer(dFrontier, 1*trQuestVarGet("newRoom"));
				vector pos = gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest)));
				lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));

				trUIFadeToColor(0,0,0,1000,1900,true);

				for(i=xGetDatabaseCount(dFrontier); >0) {
					xDatabaseNext(dFrontier);
					xSetInt(dFrontier, xFrontierVotes, 0);
				}
			}
			case 2:
			{
				for(p=1; < ENEMY_PLAYER) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"senator");
					trUnitDestroy();
				}
				trSoundPlayFN("wall.wav");

				if (trQuestVarGet("newestRoomType") < ROOM_HEALING) {
					xAddDatabaseBlock(dSpawnerRooms, true);
					xSetInt(dSpawnerRooms, xUnitName, zGetInt(frontierUnitsArray, 1*trQuestVarGet("newestRoom")));
					xSetVector(dSpawnerRooms, xSpawnerRoomPos, gridToVector(symphonyRoomToVector(1*trQuestVarGet("newestRoom"))));
				}

				xSetPointer(dFrontier, 1*trQuestVarGet("newRoom"));

				trQuestVarSet("newestRoom", xGetInt(dFrontier, xFrontierDest));
				trQuestVarSet("newestRoomType", xGetInt(dFrontier, xFrontierType));

				symphonyUnlockRoom();



				trUIFadeToColor(0,0,0,1000,100,false);
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


rule symphony_stride_cin_vote
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
				trMusicPlay("xpack\xcinematics\10_a\music.mp3", "1", 0);
				trSoundPlayFN("default","1",-1,"Zenophobia:Round end! Now it's time to rest and vote on your next destination!","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 2:
			{
				xDatabaseNext(dFrontier);
				xUnitSelect(dFrontier, xUnitName);
				trUnitHighlight(5.0, true);
				pos = gridToVector(symphonyRoomToVector(xGetInt(dFrontier, xFrontierDest)));
				trackInsert();
				trackAddWaypoint();
				lookAt(xsVectorGetX(pos),xsVectorGetZ(pos));
				trackAddWaypoint();
				trackPlay(1000,-1);
				trSoundPlayFN("default","1",-1,"Zenophobia:You may have noticed these blue squares earlier. Select one to see what it is.","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 3:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia:If you want to unlock this room next, you can vote for it.","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 4);
			}
			case 4:
			{
				xsDisableSelf();
				trLetterBox(false);
				trMessageSetText("You may vote for your next destination now.", -1);

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
			}
		}
	}
}