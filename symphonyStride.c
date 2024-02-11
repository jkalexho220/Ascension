void symphonyLevelUp(int count = 0) {
	int class = 0;
	int proto = 0;
	for(p=1; < ENEMY_PLAYER) {
		class = xGetInt(dPlayerData, xPlayerClass, p);
		proto = xGetInt(dClass, xClassProto, class);
		trModifyProtounit(kbGetProtoUnitName(proto), p, 5, count);
		trSetCivilizationNameOverride(p, "Level " + (1+xGetInt(dPlayerData, xPlayerLevel, p)) + " (Temporary)");
	}
}

vector symphonyRoomIndexToCoordinates(int room = 0) {
	int x = room / 5;
	int z = room - x * 5;
	return(xsVectorSet(x,0,z));
}

int symphonyCoordinatesToRoomIndex(int x = 0, int z = 0) {
	return(x + 5 * z);
}

// returns the center coordinates of the vector
vector symphonyRoomToVector(int room = 0) {
	return(symphonyRoomIndexToCoordinates(room) * 27 + xsVectorSet(5 + 14, 0, 5 + 14));
}

string symphonyRoomProtoUnit(int type = 0) {
	string proto = "Outpost";
	switch(type)
	{
		case ROOM_CHEST:
		{
			proto = "Great Box";
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
			xSetInt(dFrontier, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dFrontier, xFrontierCameFrom, cameFrom);
			xSetInt(dFrontier, xFrontierDest, room);
			xSetInt(dFrontier, xFrontierType, trQuestVarGet("room"+room));

			trPaintTerrain(xsVectorGetX(center)-2,xsVectorGetZ(center)-2,xsVectorGetX(center)+2,xsVectorGetZ(center)+2,0,73,false);

			trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			trArmySelect("0,0");
			trUnitChangeProtoUnit(symphonyRoomProtoUnit(xGetInt(dFrontier, xFrontierType)));
		}
	}
}

void symphonyBuildEdge(int from = 0, int to = 0) {
	vector top = symphonyRoomToVector(1 * xsMax(from, to));
	vector bottom = symphonyRoomToVector(1 * xsMin(from, to));
	trPaintTerrain(xsVectorGetX(bottom) - 2, xsVectorGetZ(bottom) - 2,
		xsVectorGetX(top) + 2, xsVectorGetZ(top) + 2, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
	paintSecondary(xsVectorGetX(bottom) - 2, xsVectorGetZ(bottom) - 2, xsVectorGetX(top) + 2, xsVectorGetZ(top) + 2);
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
		paintSecondary(x0, z0, x1, z1);
		paintEyecandy(x0, z0, x1, z1, "tree");
		paintEyecandy(x0, z0, x1, z1, "rock");
		paintEyecandy(x0, z0, x1, z1, "sprite");
	}

	switch(type)
	{
		case ROOM_CHEST:
		{
			xAddDatabaseBlock(dChests, true);
			xSetInt(dChests,xUnitName,trGetNextUnitScenarioNameNumber());
			xSetInt(dChests,xChestType, CHEST_SYMPHONY);
			xSetInt(dChests,xChestRoom,room);
			pos = gridToVector(center);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0, xsVectorGetZ(pos), 135, true);
			trArmySelect("1,0");
			trUnitConvert(0);
			trUnitChangeProtoUnit("Great Box");
		}
	}

	for(i = -1; <= 1) {
		for(j = -1; <= 1) {
			if (xsAbs(i + j) == 1) {
				symphonyAddFrontier(x + i, z + j, room);
			}
		}
	}
}

rule symphony_stride_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		trQuestVarSet("symphonyMode", 1);
		xsEnableRule("symphony_stride_cin_intro");

		vector center = vector(0,0,0);
		vector pos = vector(0,0,0);
		int val = 0;

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

		for(i=70; >0) {
			/* bacchanalia */
			trTechSetStatus(ENEMY_PLAYER, 78, 4);
		}

		for(i=5; >0) {
			/* monstrous rage */
			trTechSetStatus(ENEMY_PLAYER, 76, 4);
		}

		dFrontier = xInitDatabase("frontier");
		xInitAddInt(dFrontier, "unit");
		xFrontierDest = xInitAddInt(dFrontier, "destination");
		xFrontierCameFrom = xInitAddInt(dFrontier, "cameFrom");
		xFrontierType = xInitAddInt(dFrontier, "type");

		

		for(i=0; < 25) {
			trQuestVarSet("room"+i, ROOM_CHEST);
			pos = symphonyRoomToVector(i);
			trPaintTerrain(xsVectorGetX(pos)-2,xsVectorGetZ(pos)-2,xsVectorGetX(pos)+2,xsVectorGetZ(pos)+2,0,73,false);
		}

		symphonyBuildRoom(2, 2, ROOM_CHEST);
		/*
		center = symphonyRoomToVector(12);
		trPaintTerrain(xsVectorGetX(center) - 9, xsVectorGetZ(center) - 9, xsVectorGetX(center) + 9, xsVectorGetZ(center) + 9, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
		*/
		
		trVectorQuestVarSet("startPosition", vector(145,0,145));

		trPaintTerrain(0,0,5,5,0,73,true);
		trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

		// shuffle
		for(i=0; < 25) {
			trQuestVarSetFromRand("rand", i, 24, true);
			val = trQuestVarGet("room"+i);
			trQuestVarSet("room"+i, trQuestVarGet("room"+1*trQuestVarGet("rand")));
			trQuestVarSet("room"+1*trQuestVarGet("rand"), val);

			pos = symphonyRoomToVector(i);
			trPaintTerrain(xsVectorGetX(pos)-2,xsVectorGetZ(pos)-2,xsVectorGetX(pos)+2,xsVectorGetZ(pos)+2,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
		}

		xsEnableRule("symphony_stride_cin_intro");
		// TODO: delete this
		trQuestVarSet("cinStep", 5);
		trQuestVarSet("cinTime", trTime() + 2);
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
				trSoundPlayFN("default","1",-1,"Zenophobia:Face 10 challenges of increasing difficulty to win! Your characters will level up after each challenge!","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 4:
			{
				trSoundPlayFN("default","1",-1,"Zenophobia:Good luck and have fun! To start, I'll grant you 2 level ups immediately.","icons\infantry g hoplite icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				trSoundPlayFN("ageadvance.wav");
				trOverlayText("Level up!", 3.0);
				symphonyLevelUp(2);
			}
			case 5:
			{
				xsDisableSelf();
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				xsEnableRule("gameplay_start");
				//xsEnableRule("gladiator_worlds_portals");

				trMusicPlayCurrent();
				trPlayNextMusicTrack();
				
				//trStringQuestVarSet("advice", "What do you mean you can't beat this? This is easy mode!");
			}
		}
	}
}