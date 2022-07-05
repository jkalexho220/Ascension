

rule choose_stage_00
inactive
highFrequency
{
	xDatabaseNext(dStageChoices);
	int n = xGetInt(dStageChoices, xUnitName);
	xUnitSelect(dStageChoices,xUnitName);
	if (trCountUnitsInArea(""+n, 1, "Athena",3) == 1) {
		trQuestVarSet("stage", xGetInt(dStageChoices,xStageChoicesStage));
		if (trQuestVarGet("stage") == 10) {
			xsEnableRule("final_build_map");
		} else if (trQuestVarGet("stage") == 0) {
			xsEnableRule("pvp_build_map");
		} else if (trQuestVarGet("stage") == 12) {
			xsEnableRule("gladiator_worlds_build_map");
		} else {
			xsEnableRule("choose_stage_02");
			trStringQuestVarSet("advice", "Having difficulty at higher floors? Level up and bring some friends!");
			for(p=ENEMY_PLAYER; >0) {
				for(i=trQuestVarGet("stage"); >1) {
					/* bacchanalia 3 x stage */
					for(j=3; >0) {
						trTechSetStatus(p, 78, 4);
					}
				}
				for(i=trQuestVarGet("stage")/2; >0) {
					/* monstrous rage */
					trTechSetStatus(p, 76, 4);
					if ((trQuestVarGet("stage") > 6) && (p == ENEMY_PLAYER)) {
						trTechSetStatus(p, 76, 4);
					}
				}
			}
			for(i=trQuestVarGet("stage"); >1) {
				/* bacchanalia 7 x stage */
				for(j=7; >0) {
					trTechSetStatus(ENEMY_PLAYER, 78, 4);
				}
			}
		}
		trUnitSelectClear();
		trUnitSelectByQV("chooser", true);
		trUnitChangeProtoUnit("Rocket");
		for(x=xGetDatabaseCount(dStageChoices); >0) {
			xDatabaseNext(dStageChoices);
			xUnitSelect(dStageChoices,xUnitName);
			trUnitDestroy();
		}
		aiPlanDestroy(dStageChoices);
		xsDisableSelf();
		trForceNonCinematicModels(true);
		trLetterBox(true);
		trUIFadeToColor(0,0,0,1000,0,true);
		trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
		trOverlayText(stageName(1*trQuestVarGet("stage")), 3.0, -1, -1, -1);
		
		xsEnableRule("delayed_modify");
	} else if (trUnitIsSelected()) {
		uiClearSelection();
		if (xGetInt(dStageChoices, xStageChoicesStage) > 0 && xGetInt(dStageChoices, xStageChoicesStage) <= 10) {
			trShowImageDialog(stageIcon(xGetInt(dStageChoices,xStageChoicesStage)),
				"Floor " + xGetInt(dStageChoices,xStageChoicesStage) + ": " + stageName(xGetInt(dStageChoices,xStageChoicesStage)));
		} else {
			trShowImageDialog(stageIcon(xGetInt(dStageChoices,xStageChoicesStage)), stageName(xGetInt(dStageChoices,xStageChoicesStage)));
		}
	}
}


void buildRoom(int x = 0, int z = 0, int type = 0) {
	int next = 0;
	int room = x + 4 * z;
	int size = 0;
	int x0 = 0;
	int z0 = 0;
	int x1 = 0;
	int z1 = 0;
	bool trapped = false;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	trQuestVarSet("room"+room, type);
	if (type < ROOM_CHEST) {
		if ((trQuestVarGet("trapRooms") > 0) &&
			(countRoomEntrances(x, z) > 1) &&
			(trQuestVarGet("relictransporterguy") != room)) {
			trQuestVarSetFromRand("rand", 1, 3, true);
			if (trQuestVarGet("rand") == 1) {
				trQuestVarSet("trapRooms", trQuestVarGet("trapRooms") - 1);
				trapped = true;
				switch(1*trQuestVarGet("trapType"))
				{
					case TRAP_LASERS:
					{
						x0 = x * 35 + 12;
						x1 = x * 35 + 28;
						z0 = z * 35 + 12;
						z1 = z * 35 + 28;
						trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
						trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
						trVectorQuestVarSet("room"+room+"top1", xsVectorSet(x1,0,z1));
						trVectorQuestVarSet("room"+room+"bottom1", xsVectorSet(x0,0,z0));
						next = trGetNextUnitScenarioNameNumber();
						trArmyDispatch("1,0","Dwarf",1,x*70+41,0,z*70+41,0,true);
						xSetPointer(dLaserRooms, xAddDatabaseBlock(dLaserRooms));
						xSetInt(dLaserRooms,xUnitName,next);
						xSetVector(dLaserRooms,xLaserRoomPos,xsVectorSet(x * 70 + 40, 0, z * 70 + 40));
						for(a=0; <8) {
							/* z lasers */
							trArmyDispatch("1,0","Dwarf",1,2*x0+4*a+3,0,2*z0+1,180,false);
							trArmyDispatch("1,0","Dwarf",1,2*x0+4*a+3,0,2*z1-1,0,false);
							xSetVector(dLaserRooms,xLaserRoomXBottom + a,
								xsVectorSet(2 * x0 + 4 * a + 2, 0, 2 * z0));
							xSetVector(dLaserRooms,xLaserRoomXTop + a,
								xsVectorSet(2 * x0 + 4 * a + 4, 0, 2 * z1));
							/* x lasers */
							trArmyDispatch("1,0","Dwarf",1,2*x0+1,0,2*z0+4*a+3,270,false);
							trArmyDispatch("1,0","Dwarf",1,2*x1-1,0,2*z0+4*a+3,90,false);
							xSetVector(dLaserRooms,xLaserRoomZBottom + a,
								xsVectorSet(2 * x0, 0, 2 * z0 + 4 * a + 2));
							xSetVector(dLaserRooms,xLaserRoomZTop + a,
								xsVectorSet(2 * x1, 0, 2 * z0 + 4 * a + 4));
						}
						trArmySelect("1,0");
						trUnitSetStance("Passive");
						trSetSelectedScale(0,0,0);
						trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
						
						paintSecondary(x0, z0, x1, z1);
						paintEyecandy(x0, z0, x1, z1, "tree");
						paintEyecandy(x0, z0, x1, z1, "rock");
						paintEyecandy(x0, z0, x1, z1, "sprite");
						for(j=randomLow(11) - 8; >0) {
							paintRelicEdge(x0, z0, x1, z1);
						}
					}
					case TRAP_CAROUSEL:
					{
						x0 = x * 35 + 16;
						x1 = x * 35 + 24;
						z0 = z * 35 + 16;
						z1 = z * 35 + 24;
						trVectorQuestVarSet("room"+room+"top1", xsVectorSet(x1,0,z1));
						trVectorQuestVarSet("room"+room+"bottom1", xsVectorSet(x0,0,z0));
						paintCircle(x, z, 9, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
						pos = xsVectorSet(x * 70 + 41, 5, z * 70 + 41);
						next = trGetNextUnitScenarioNameNumber();
						trArmyDispatch("1,0", "Dwarf", 3, x * 70 + 41, 5, z * 70 + 41, 225, true);
						trArmySelect("1,0");
						trUnitConvert(0);
						trUnitSelectClear();
						trUnitSelect(""+next, true);
						trMutateSelected(kbGetProtoUnitID("Statue of Lightning"));
						trUnitOverrideAnimation(2,0,true,false,-1);
						trUnitSelectClear();
						trUnitSelect(""+(next+1), true);
						trMutateSelected(kbGetProtoUnitID("Outpost"));
						trUnitSelectClear();
						trUnitSelect(""+(next+2), true);
						trMutateSelected(kbGetProtoUnitID("Outpost"));

						xAddDatabaseBlock(dCarouselRooms, true);
						xSetInt(dCarouselRooms, xUnitName, next);
						xSetVector(dCarouselRooms, xCarouselRoomPos, pos);
					}
				}
			}
		}
		if (trapped == false) {
			if (trQuestVarGet("mapType") < MAP_OPEN) {
				for (i=2; >0) {
					trQuestVarSetFromRand("x0", x * 35 + 5, x * 35 + 18, true);
					trQuestVarSetFromRand("z0", z * 35 + 5, z * 35 + 18, true);
					trQuestVarSetFromRand("x1", x * 35 + 22, x * 35 + 35, true);
					trQuestVarSetFromRand("z1", z * 35 + 22, z * 35 + 35, true);
					x0 = trQuestVarGet("x0");
					x1 = trQuestVarGet("x1");
					z0 = trQuestVarGet("z0");
					z1 = trQuestVarGet("z1");
					trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
					trVectorQuestVarSet("room"+room+"top"+i, xsVectorSet(x1,0,z1));
					trVectorQuestVarSet("room"+room+"bottom"+i, xsVectorSet(x0,0,z0));
					paintSecondary(x0, z0, x1, z1);
					paintEyecandy(x0, z0, x1, z1, "tree");
					paintEyecandy(x0, z0, x1, z1, "rock");
					paintEyecandy(x0, z0, x1, z1, "sprite");
					
					/* relic spawn */
					for(j=randomLow(11) - 8; >0) {
						paintRelicEdge(x0, z0, x1, z1);
					}
				}
				paintColumns(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35);
			} else if (trQuestVarGet("mapType") == MAP_OPEN) {
				paintColumns(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35);
				trQuestVarSetFromRand("x0", x * 35 + 5, x * 35 + 15, true);
				trQuestVarSetFromRand("z0", z * 35 + 5, z * 35 + 15, true);
				trQuestVarSetFromRand("x1", x * 35 + 25, x * 35 + 35, true);
				trQuestVarSetFromRand("z1", z * 35 + 25, z * 35 + 35, true);
				x0 = trQuestVarGet("x0");
				x1 = trQuestVarGet("x1");
				z0 = trQuestVarGet("z0");
				z1 = trQuestVarGet("z1");
				trVectorQuestVarSet("room"+room+"top1", xsVectorSet(x1,0,z1));
				trVectorQuestVarSet("room"+room+"bottom1", xsVectorSet(x0,0,z0));
				paintSecondary(x0, z0, x1, z1);
				paintEyecandy(x0, z0, x1, z1, "tree");
				paintEyecandy(x0, z0, x1, z1, "rock");
				paintEyecandy(x0, z0, x1, z1, "sprite");
				
				/* relic spawn */
				for(j=randomLow(11) - 8; >0) {
					paintRelicEdge(x0, z0, x1, z1);
				}
			}
		}
		if (trQuestVarGet("room"+room+"key") > 0) {
			vector v0 = randomNearEdge(x0, z0, x1, z1);
			xSetInt(dUnlockWalls,xWallKey, trGetNextUnitScenarioNameNumber(), 1*trQuestVarGet("room"+room+"index"));
			spawnRelicSpecific(v0,1*trQuestVarGet("room"+room+"key"));
			trQuestVarSet("room"+room+"key", 0);
		}
	} else if (trQuestVarGet("room"+room+"key") > 0) {
		xSetInt(dUnlockWalls,xWallKey, trGetNextUnitScenarioNameNumber(), 1*trQuestVarGet("room"+room+"index"));
		if (type == ROOM_ELECTRIC) {
			spawnRelicSpecific(xsVectorSet(70 * x + 15,0,70 * z + 15),1*trQuestVarGet("room"+room+"key"));
		} else if (type == ROOM_PUZZLE) {
			spawnRelicSpecific(xsVectorSet(70 * x + 53,0,70 * z + 53),1*trQuestVarGet("room"+room+"key"));
		} else {
			spawnRelicSpecific(xsVectorSet(70 * x + 34,0,70 * z + 34),1*trQuestVarGet("room"+room+"key"));
		}
		trQuestVarSet("room"+room+"key", 0);
	}
	switch(type)
	{
		case ROOM_BASIC:
		{
			for(i = 2; >0) {
				pos = trVectorQuestVarGet("room"+room+"bottom"+i);
				x0 = xsVectorGetX(pos);
				z0 = xsVectorGetZ(pos);
				pos = trVectorQuestVarGet("room"+room+"top"+i);
				x1 = xsVectorGetX(pos);
				z1 = xsVectorGetZ(pos);
				paintEnemies(x0, z0, x1, z1);
			}
			xSetPointer(dBasicRooms, xAddDatabaseBlock(dBasicRooms));
			xSetInt(dBasicRooms, xRoomNumber, room);
		}
		case ROOM_AMBUSH:
		{
			trQuestVarSetFromRand("type", 1, trQuestVarGet("enemyProtoCount"), true);
			xSetPointer(dAmbushRooms, xAddDatabaseBlock(dAmbushRooms));
			xSetInt(dAmbushRooms,xRoomNumber,room);
			xSetInt(dAmbushRooms,xAmbushRoomType,1*trQuestVarGet("type"));
			xSetVector(dAmbushRooms,xAmbushRoomPos, xsVectorSet(x * 70 + 40, 0, 70 * z + 40));
			
			trQuestVarSetFromRand("spawnRelic", 0, 1, false);
			if (trQuestVarGet("spawnRelic") < 0.5) {
				spawnRelic(xsVectorSet(x * 70 + 40, 0, z * 70 + 40));
			}
			pos = trVectorQuestVarGet("room"+room+"bottom1");
			x0 = xsVectorGetX(pos);
			z0 = xsVectorGetZ(pos);
			pos = trVectorQuestVarGet("room"+room+"top1");
			x1 = xsVectorGetX(pos);
			z1 = xsVectorGetZ(pos);
			paintEnemies(x0, z0, x1, z1);
			xSetPointer(dBasicRooms, xAddDatabaseBlock(dBasicRooms));
			xSetInt(dBasicRooms, xRoomNumber, room);
		}
		case ROOM_BOSS_ENTRANCE:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 25, z * 35 + 25, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 25, z * 35 + 25, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 25, z * 35 + 25);
			trQuestVarSet("bossEntranceStatue", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,x*70+40,0,z*70+40,225,true);
			trUnitSelectClear();
			trUnitSelectByQV("bossEntranceStatue", true);
			trUnitConvert(0);
			trUnitChangeProtoUnit("Monument 5");
			trQuestVarSet("bossEntranceSymbol", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,x*70+36,0,z*70+36,225,true);
			trQuestVarSet("keyType", trQuestVarGet("keyType") - 1);
			xsEnableRule("boss_entrance_found");
		}
		case ROOM_CHEST:
		{
			trQuestVarSetFromRand("chestType", CHEST_KEY, CHEST_STATUES, true);
			if (trQuestVarGet("chestType") < CHEST_STATUES) {
				buildRoom(x, z, ROOM_BASIC);
				trQuestVarSet("room"+room, ROOM_CHEST);
				trPaintTerrain(x*35+15, z*35+15, x*35+25, z*35+25, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
				trChangeTerrainHeight(x*35+15, z*35+15, x*35+25, z*35+25, worldHeight, false);
			} else {
				size = 12;
				z0 = size;
				for(a=0; < size) {
					for(b=size; >0) {
						if (a*a + z0 * z0 <= size * size) {
							trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
							trChangeTerrainHeight(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, worldHeight, false);
							break;
						} else {
							z0 = z0 - 1;
						}
					}
				}
			}
			
			if (trQuestVarGet("chestType") == CHEST_KEY) {
				if (trQuestVarGet("keyType") < RELIC_KEY_GREEK) {
					trQuestVarSet("chestType", CHEST_ENCOUNTER);
				} else {
					trQuestVarSet("keyType", trQuestVarGet("keyType") - 1);
				}
			}
			xSetPointer(dChests, xAddDatabaseBlock(dChests));
			xSetInt(dChests,xUnitName,trGetNextUnitScenarioNameNumber());
			xSetInt(dChests,xChestType, 1*trQuestVarGet("chestType"));
			xSetInt(dChests,xChestRoom,room);
			trArmyDispatch("1,0","Dwarf",1,x * 70 + 40,0, z * 70 + 40, 135, true);
			trArmySelect("1,0");
			trUnitConvert(0);
			trUnitChangeProtoUnit("Great Box");
		}
		case ROOM_TRANSPORTER_GUY:
		{
			pos = trVectorQuestVarGet("room"+room+"bottom1");
			x0 = xsVectorGetX(pos);
			z0 = xsVectorGetZ(pos);
			pos = trVectorQuestVarGet("room"+room+"top1");
			x1 = xsVectorGetX(pos);
			z1 = xsVectorGetZ(pos);
			trPaintTerrain(x0,z0,x1,z1,TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x0,z0,x1,z1,worldHeight, false);
			pos = randomNearEdge(x0, z0, x1, z1);
			trVectorQuestVarSet("relictransporterguypos", pos);
			trQuestVarSet("heading", 180.0 / 3.141592 * angleBetweenVectors(pos, xsVectorSet(x0 + x1, 0, z0 + z1)));
			trQuestVarSet("relicTransporterGuyName", trGetNextUnitScenarioNameNumber());
			x0 = xsVectorGetX(pos);
			z0 = xsVectorGetZ(pos);
			trArmyDispatch("1,0","Villager Atlantean Hero",1,x0,0,z0,trQuestVarGet("heading"), true);
			trUnitSelectClear();
			trUnitSelectByQV("relicTransporterGuyName", true);
			trUnitConvert(0);
			trArmyDispatch("1,0", "Dwarf",1,x0,0,z0,0,true);
			trArmySelect("1,0");
			trUnitChangeProtoUnit("Gaia Forest effect");
			xsEnableRule("relic_transporter_guy_found");
		}
		case ROOM_PUZZLE:
		{
			trPaintTerrain(x * 35 + 11, z * 35 + 11, x * 35 + 29, z * 35 + 29, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 11, z * 35 + 11, x * 35 + 30, z * 35 + 30, worldHeight, false);
			puzzleStatues = xInitDatabase("puzzleStatues");
			trVectorQuestVarSet("puzzleRoomCenter", xsVectorSet(x * 70 + 41, 0, z * 70 + 41));
			xInitAddInt(puzzleStatues, "name");
			xStatueState = xInitAddInt(puzzleStatues,"state");
			xStatuePosition = xInitAddInt(puzzleStatues,"position");
			xStatueAngle = xInitAddFloat(puzzleStatues,"angle");
			xStatueTimeout = xInitAddInt(puzzleStatues,"timeout");
			xStatuePos = xInitAddVector(puzzleStatues,"pos"); // repurposed to the x/z coordinates of the statue

			puzzleDB = aiPlanCreate("puzzleDB", 8);
			bool soldier = false;
			trQuestVarSet("puzzleRoomRevealer", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,x * 70 + 41, 0, z * 70 + 41, 0, true);
			trArmySelect("0,0");
			trUnitChangeProtoUnit("Revealer to Player");

			trArmyDispatch("0,0","Dwarf",1,x * 70 + 55,0,z * 70 + 55, 225, true);
			trArmySelect("0,0");
			trMutateSelected(kbGetProtoUnitID("Monument 5"));

			trArmyDispatch("0,0","Dwarf",1, x * 70 + 27, 0, z * 70 + 27, 45, true);
			trArmySelect("0,0");
			trMutateSelected(kbGetProtoUnitID("Monument 4"));

			x = x * 70 + 33;
			z = z * 70 + 41;
			for(i=0; <3) {
				//soldier = (soldier == false);
				aiPlanAddUserVariableInt(puzzleDB, i, "row"+i, 4);
				for(j=0; <3) {
					aiPlanSetUserVariableInt(puzzleDB, i, j, xAddDatabaseBlock(puzzleStatues, true));
					xSetInt(puzzleStatues, xUnitName, trGetNextUnitScenarioNameNumber());
					xSetVector(puzzleStatues, xStatuePos, xsVectorSet(i, 0, j));
					if (soldier) {
						xSetFloat(puzzleStatues, xStatueAngle, 3.92699);
						trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x + i * 4 + j * 4, 0, z + j * 4 - i * 4, 225, true);
						trArmySelect(""+ENEMY_PLAYER+",0");
						trMutateSelected(kbGetProtoUnitID("Monument 2"));
					} else {
						xSetFloat(puzzleStatues, xStatueAngle, 0.785398);
						trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x + i * 4 + j * 4, 0, z + j * 4 - i * 4, 45, true);
						trArmySelect(""+ENEMY_PLAYER+",0");
						trMutateSelected(kbGetProtoUnitID("Monument"));
					}
					soldier = (soldier == false);
				}
			}
			for(i=0; <2) {
				for(j=0; <3) {
					trArmyDispatch("0,0","Dwarf",1,x + 2 + i * 4 + j * 4, 0, z - 2 + j * 4 - i * 4, 45, true);
					trArmySelect("0,0");
					trMutateSelected(kbGetProtoUnitID("Undermine Ground decal long"));
					trSetSelectedScale(0.62, 1, 0.3);

					trArmyDispatch("0,0","Dwarf",1,x + 2 + i * 4 + j * 4, 0, z + 2 + i * 4 - j * 4, 135, true);
					trArmySelect("0,0");
					trMutateSelected(kbGetProtoUnitID("Undermine Ground decal long"));
					trSetSelectedScale(0.62, 1, 0.3);
				}
			}
			turnPuzzleStatue(1, true, true);
			turnPuzzleStatue(6, true, true);
			turnPuzzleStatue(8, true, true);
			turnPuzzleStatue(8, true, true);

			xsEnableRule("puzzle_room_find");
		}
		case ROOM_ELECTRIC:
		{
			trPaintTerrain(x * 35 + 5, z * 35 + 5, x * 35 + 34, z * 35 + 34, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35, worldHeight, false);
			// paint a spiral of cliff
			dir = vector(1,0,0);
			pos = xsVectorSet(x * 35 + 10, 0, z * 35 + 30);
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, 0, 73, false); // city water pool
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 10, z * 35 + 30, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			for(i=10; >1) {
				trPaintTerrain(xsVectorGetX(pos),xsVectorGetZ(pos),xsVectorGetX(pos) + 2 * xsVectorGetX(dir) * i, xsVectorGetZ(pos) + 2 * xsVectorGetZ(dir) * i, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				pos = pos + (dir * i * 2);
				dir = rotationMatrix(dir, 0, -1);
				trPaintTerrain(xsVectorGetX(pos),xsVectorGetZ(pos),xsVectorGetX(pos) + 2 * xsVectorGetX(dir) * i, xsVectorGetZ(pos) + 2 * xsVectorGetZ(dir) * i, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				pos = pos + (dir * i * 2);
				dir = rotationMatrix(dir, 0, -1);
			}
			trQuestVarSet("electricRoomRevealer", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,x * 70 + 41, 0, z * 70 + 41,0,true);
			trArmySelect("0,0");
			trUnitChangeProtoUnit("Cinematic Block");
			trQuestVarSet("electricRoomShiny", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,x * 70 + 41, 0, z * 70 + 41,0,true);
			trArmySelect("0,0");
			trMutateSelected(kbGetProtoUnitID("Curse SFX"));
			trVectorQuestVarSet("electricRoomCenter", xsVectorSet(x * 70 + 41, 0, z * 70 + 41));
			trVectorQuestVarSet("electricRoomBot", xsVectorSet(x * 70 + 20, 0, z * 70 + 20));
			trVectorQuestVarSet("electricRoomTop", xsVectorSet(x * 70 + 60, 0, z * 70 + 60));
			xsEnableRule("electric_room_find");
			xElectricNext = xInitAddInt(dPlayerCharacters, "electricNext");
		}
		case ROOM_EXCALIBUR:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");

			trQuestVarSet("excaliburRevealer", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Victory Marker", 1, 70 * x + 41, 0, 70 * z + 41, 0, true);

			trQuestVarSet("excavation", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("0,0","Dwarf",1,x * 70 + 41, 0, z * 70 + 41, 0, true);
			trUnitSelectClear();
			trUnitSelectByQV("excavation");
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelectByQV("excavation");
			trMutateSelected(kbGetProtoUnitID("Excavation"));
			trUnitSetAnimationPath("1,0,0,0,0,0,0");

			xAddDatabaseBlock(dNpcTalk, true);
			xSetInt(dNpcTalk, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dNpcTalk, xNpcDialog, NPC_EXCALIBUR_BYSTANDER);
			trArmyDispatch("0,0","Villager Norse",1, x * 70 + 31, 0, z * 70 + 41, 90, true);

			xAddDatabaseBlock(dNpcTalk, true);
			xSetInt(dNpcTalk, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dNpcTalk, xNpcDialog, NPC_EXCALIBUR_BYSTANDER + 1);
			xAddDatabaseBlock(dStunnedUnits, true);
			xSetInt(dStunnedUnits, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Axeman"));
			trArmyDispatch("0,0","Axeman",1, x * 70 + 31, 0, z * 70 + 37, 90, true);

			xAddDatabaseBlock(dNpcTalk, true);
			xSetInt(dNpcTalk, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dNpcTalk, xNpcDialog, NPC_EXCALIBUR_BYSTANDER + 2);
			trArmyDispatch("0,0","Skult",1, x * 70 + 31, 0, z * 70 + 31, 45, true);

			xAddDatabaseBlock(dNpcTalk, true);
			xSetInt(dNpcTalk, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dNpcTalk, xNpcDialog, NPC_EXCALIBUR_BYSTANDER + 3);
			trArmyDispatch("0,0","Villager Egyptian",1, x * 70 + 37, 0, z * 70 + 31, 0, true);

			xAddDatabaseBlock(dNpcTalk, true);
			xSetInt(dNpcTalk, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dNpcTalk, xNpcDialog, NPC_EXCALIBUR_BYSTANDER + 4);
			xAddDatabaseBlock(dStunnedUnits, true);
			xSetInt(dStunnedUnits, xUnitName, trGetNextUnitScenarioNameNumber());
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Militia"));
			trArmyDispatch("0,0","Militia",1, x * 70 + 41, 0, z * 70 + 31, 0, true);

			trQuestVarSet("DPSCheckObject", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x * 70 + 41, 0, z * 70 + 41, 0, true);
			trUnitSelectClear();
			trUnitSelectByQV("DPSCheckObject");
			trUnitChangeProtoUnit("Wall Connector");
			trModifyProtounit("Wall Connector", ENEMY_PLAYER, 0, 9999999999999999999.0);
			trModifyProtounit("Wall Connector", ENEMY_PLAYER, 0, -9999999999999999999.0);
			trModifyProtounit("Wall Connector", ENEMY_PLAYER, 0, 10000);

			activateEnemy(1*trQuestVarGet("DPSCheckObject"));
			xSetBool(dEnemies, xLaunched, true, xGetNewestPointer(dEnemies));
			xsEnableRule("excalibur_find");
		}
		case ROOM_VILLAGE + 1:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");
			
			trQuestVarSet("villageX", 70 * x + 20);
			trQuestVarSet("villageZ", 70 * z + 20);
			
			deployTownEyecandy("Savannah Tree",15,37,0);
			deployTownEyecandy("Savannah Tree",11,33,50);
			deployTownEyecandy("Savannah Tree",5,21,90);
			deployTownEyecandy("Savannah Tree",29,31,47);
			deployTownEyecandy("Savannah Tree",33,29,127);
			
			deployTownEyecandy("House",7,29,180);
			deployTownEyecandy("House",9,15,0);
			
			deployTownEyecandy("Granary",7,33,0);
			deployTownEyecandy("Dock",9,13,0);
			deployTownEyecandy("Tower",33,15,270);
			deployTownEyecandy("Counter Building",23,29,180);
			
			deployTownEyecandy("Fence Wood",19,15,180);
			deployTownEyecandy("Fence Wood",19,11,180);
			deployTownEyecandy("Fence Wood",21,9,270);
			deployTownEyecandy("Fence Wood",25,17,270);
			deployTownEyecandy("Fence Wood",27,15,180);
			deployTownEyecandy("Fence Wood",27,11,180);
			trVectorQuestVarSet("pigpenLower", xsVectorSet(trQuestVarGet("villageX") + 19,0,trQuestVarGet("villageZ") + 9));
			trVectorQuestVarSet("pigpenUpper", xsVectorSet(trQuestVarGet("villageX") + 27,0,trQuestVarGet("villageZ") + 17));
			
			trQuestVarSetFromRand("localQuest", 1, 3, true);
			if (trQuestVarGet("monsterpediaQuestInProgress") > 0) {
				trQuestVarSetFromRand("localQuest", 1, 2, true);
			}
			
			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Villager Chinese",23,19,315);
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Maceman",19,25,225);
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+BOUNTY_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Maceman"));
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Villager Egyptian",11,27,180);
			
			if (trQuestVarGet("localQuest") > 1) {
				trQuestVarSet("pigStart", trGetNextUnitScenarioNameNumber());
				deployTownEyecandy("Pig",21,15,120);
				deployTownEyecandy("Pig",25,13,56);
				deployTownEyecandy("Pig",21,11,222);
				trQuestVarSet("pigEnd", trGetNextUnitScenarioNameNumber());
				for(x=trQuestVarGet("pigStart"); < trQuestVarGet("pigEnd")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trUnitConvert(ENEMY_PLAYER);
				}
			}
		}
		case ROOM_VILLAGE + 2:
		{
			size = 12;
			z0 = size;
			for(a=0; < size) {
				for(b=size; >0) {
					if (a*a + z0 * z0 <= size * size) {
						trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
						trChangeTerrainHeight(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, worldHeight, false);
						break;
					} else {
						z0 = z0 - 1;
					}
				}
			}
			trQuestVarSet("villageX", 70 * x + 16);
			trQuestVarSet("villageZ", 70 * z + 16);
			
			trQuestVarSet("start", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Marsh Tree",25,41,250);
			deployTownEyecandy("Marsh Tree",23,41,75);
			deployTownEyecandy("Marsh Tree",17,39,321);
			deployTownEyecandy("Marsh Tree",13,37,266);
			deployTownEyecandy("Marsh Tree",5,29,240);
			deployTownEyecandy("Marsh Tree",5,21,80);
			deployTownEyecandy("Marsh Tree",9,15,44);
			deployTownEyecandy("Marsh Tree",9,11,280);
			deployTownEyecandy("Marsh Tree",13,5,140);
			deployTownEyecandy("Marsh Tree",19,7,275);
			deployTownEyecandy("Marsh Tree",25,7,121);
			deployTownEyecandy("Marsh Tree",29,7,100);
			deployTownEyecandy("Marsh Tree",37,11,155);
			deployTownEyecandy("Marsh Tree",37,17,44);
			deployTownEyecandy("Marsh Tree",39,17,145);
			deployTownEyecandy("Marsh Tree",39,25,131);
			deployTownEyecandy("Marsh Tree",39,31,178);
			deployTownEyecandy("Marsh Tree",29,37,33);
			for(a=trQuestVarGet("start"); < trGetNextUnitScenarioNameNumber()) {
				trUnitSelectClear();
				trUnitSelect(""+a, true);
				trSetSelectedScale(2,2,2);
			}
			trQuestVarSet("runestone", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Runestone", 35,35,225);
			trUnitSelectClear();
			trUnitSelectByQV("runestone", true);
			trSetSelectedScale(2.5,2.5,2.5);
			trVectorQuestVarSet("runestonePos", kbGetBlockPosition(""+1*trQuestVarGet("runestone")));
			if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") >= 1 || trQuestVarGet("runestoneComplete") == 1) {
				xsEnableRule("runestone_read");
			}
			for(p=1; < ENEMY_PLAYER) {
				if (trQuestVarGet("p"+p+"runestoneQuest") >= 1) {
					deployTownEyecandy("Healing SFX", 33, 33);
					break;
				}
			}


			deployTownEyecandy("Shrine",15,31,90);
			deployTownEyecandy("Shrine",17,11,0);
			deployTownEyecandy("Shrine",35,15,270);
			
			trQuestVarSetFromRand("localQuest", 1, 3, true);
			if (trQuestVarGet("monsterpediaQuestInProgress") > 0) {
				trQuestVarSetFromRand("localQuest", 1, 2, true);
			}
			
			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Shade",25,35,180);
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Shade",37,27,270);
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Shade",21,19,30);
		}
		case ROOM_VILLAGE + 3:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");
			
			trQuestVarSet("villageX", 70 * x + 20);
			trQuestVarSet("villageZ", 70 * z + 20);
			
			deployTownEyecandy("Dwarf Foundry",29,29,180);
			deployTownEyecandy("Dwarven Forge", 11, 29,180);
			deployTownEyecandy("Armory",27,9,270);
			deployTownEyecandy("Dwarven Forge",11,11,0);
			
			trQuestVarSetFromRand("localQuest", 1, 3, true);
			if (trQuestVarGet("monsterpediaQuestInProgress") > 0) {
				trQuestVarSetFromRand("localQuest", 1, 2, true);
			}

			if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") == 3) {
				xsEnableRule("runestone_dwarf_talk");
			}

			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Throwing Axeman",21,9,270);
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Ulfsark",15,23,135);
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Dwarf",25,25,225);
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+FETCH_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Throwing Axeman"));
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+BOUNTY_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Ulfsark"));
		}
		case ROOM_VILLAGE + 4:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");
			
			trQuestVarSet("villageX", 70 * x + 24);
			trQuestVarSet("villageZ", 70 * z + 24);
			
			
			trQuestVarSet("akardTower", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Tower Mirror",12,12,0);
			trUnitSelectClear();
			trUnitSelectByQV("akardTower");
			trUnitOverrideAnimation(2,0,true,false,-1);
			trSetSelectedScale(2,2,2);
			trQuestVarSet("akardGuild1", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Guild",16,12,180);
			trQuestVarSet("akardGuild2", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Guild",12,16,270);
			trQuestVarSet("akardTree", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Tamarisk Tree", 16, 16, 0);
			trQuestVarSet("akardTowerEnd", trGetNextUnitScenarioNameNumber());
			
			trQuestVarSetFromRand("localQuest", 1, 2, true);

			for(p=1; < ENEMY_PLAYER) {
				if (trQuestVarGet("p"+p+"runestoneQuest") == 3) {
					trQuestVarSet("localQuest", FETCH_GUY);
					xsEnableRule("akard_runestone_dialog");
				}
			}
			
			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Oracle Scout",8,8,225);
		}
		case ROOM_VILLAGE + 5:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");
			
			trQuestVarSet("villageX", 70 * x + 20);
			trQuestVarSet("villageZ", 70 * z + 20);
			
			deployTownEyecandy("Tent", 7, 33, 45);
			deployTownEyecandy("Tent", 31, 33, 135);
			deployTownEyecandy("Tent", 35, 21, 180);
			deployTownEyecandy("Tent", 19, 9, 180);
			deployTownEyecandy("Tent", 11, 17, 90);
			deployTownEyecandy("Columns Fallen", 5, 25, 220);
			deployTownEyecandy("Columns Fallen", 19, 33, 150);
			deployTownEyecandy("Columns", 33, 13, 0);
			deployTownEyecandy("Columns", 27, 7, 0);
			deployTownEyecandy("Campfire", 9, 7, 0);
			trQuestVarSet("runestone", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Runestone", 33,7,315);
			trUnitSelectClear();
			trUnitSelectByQV("runestone", true);
			trSetSelectedScale(2.5,2.5,2.5);
			trVectorQuestVarSet("runestonePos", kbGetBlockPosition(""+1*trQuestVarGet("runestone")));


			if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") >= 2 || trQuestVarGet("runestoneComplete") == 1) {
				xsEnableRule("runestone_read");
			}
			for(p=1; < ENEMY_PLAYER) {
				if (trQuestVarGet("p"+p+"runestoneQuest") >= 2) {
					deployTownEyecandy("Healing SFX", 31, 9);
					break;
				}
			}
			
			trQuestVarSetFromRand("localQuest", 1, 3, true);
			
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Toxotes",35,17,270);
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Halberdier",13,9,270);
			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Hypaspist",9,13,180);
			trQuestVarSet("kastor", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Kastor", 11, 29, 135);
			xSetPointer(dNpcTalk, xAddDatabaseBlock(dNpcTalk));
			xSetInt(dNpcTalk, xUnitName, 1*trQuestVarGet("kastor"));
			xSetInt(dNpcTalk, xNpcDialog, NPC_KASTOR);
			
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+SHOP_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Toxotes"));
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+BOUNTY_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Halberdier"));
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy"+FETCH_GUY));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Hypaspist"));
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("Kastor"));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Kastor"));
		}
		case ROOM_VILLAGE + 6:
		{
			trPaintTerrain(x * 35 + 13, z * 35 + 13, x * 35 + 27, z * 35 + 27, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 13, z * 35 + 13, x * 35 + 28, z * 35 + 28, worldHeight, false);
			
			trQuestVarSet("villageX", 70 * x + 26);
			trQuestVarSet("villageZ", 70 * z + 26);
			
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Statue of Lightning", 25, 25, 225);
			deployTownEyecandy("Statue of Lightning", 5, 25, 135);
			deployTownEyecandy("Statue of Lightning", 25, 5, 315);
			deployTownEyecandy("Statue of Lightning", 5, 5, 45);
			for(i=trQuestVarGet("next"); < trGetNextUnitScenarioNameNumber()) {
				trUnitSelectClear();
				trUnitSelect(""+i, true);
				trUnitOverrideAnimation(2,0,true,false,-1);
			}
			
			trQuestVarSetFromRand("localQuest", 1, 2, true);
			
			trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
			trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
			trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
			deployTownEyecandy("Pharaoh of Osiris", 19, 19, 225);
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("guy1"));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Pharaoh of Osiris"));
			
			if (trQuestVarGet("localQuest") == FETCH_GUY) {
				deployTownEyecandy("Cinematic Scorch", 15, 9);
				deployTownEyecandy("Smoke", 15, 9);
			} else {
				trVectorQuestVarSet("yeebRoomBottom", xsVectorSet(70 * x + 26, 0, 70 * z + 26));
				trVectorQuestVarSet("yeebRoomTop", xsVectorSet(70 * x + 56, 0, 70 * z + 56));
				trQuestVarSet("yeebRelic", trGetNextUnitScenarioNameNumber());
				pos = randomNearEdge(x * 35 + 15, z * 35 + 15, x * 35 + 25, z * 35 + 25);
				spawnRelicSpecific(pos, RELIC_YEEBAAGOOON);
				for(i=10; >0) {
					xSetPointer(dWorthlessJunk, xAddDatabaseBlock(dWorthlessJunk));
					xSetInt(dWorthlessJunk, xUnitName, trGetNextUnitScenarioNameNumber());
					pos = randomNearEdge(x * 35 + 15, z * 35 + 15, x * 35 + 25, z * 35 + 25);
					spawnRelicSpecific(pos, RELIC_WORTHLESS_JUNK);
				}
				xsEnableRule("yeeb_hit_list");
			}
		}
		case ROOM_VILLAGE + 7:
		{
			trPaintTerrain(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35, worldHeight, false);
			paintSecondary(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35);
			trQuestVarSet("villageX", 70 * x + 20);
			trQuestVarSet("villageZ", 70 * z + 20);
			trQuestVarSet("stageWonder", trGetNextUnitScenarioNameNumber());
			trUnitSelectClear();
			trUnitSelect(""+deployTownEyecandy("Cinematic Block", 21, 21, 180), true);
			trUnitConvert(ENEMY_PLAYER);
			trMutateSelected(kbGetProtoUnitID("Wonder SPC"));
			trUnitSetAnimationPath("3,1,0,0,0,0,0");
			
			deployTownEyecandy("Atlantis Wall Connector", 3, 3);
			deployTownEyecandy("Atlantis Wall Connector", 3, 15);
			deployTownEyecandy("Atlantis Wall Connector", 15, 3);
			deployTownEyecandy("Atlantis Wall Long", 9, 3);
			deployTownEyecandy("Atlantis Wall Long", 3, 9, 90);
			
			deployTownEyecandy("Atlantis Wall Connector", 39, 39);
			deployTownEyecandy("Atlantis Wall Connector", 39, 27);
			deployTownEyecandy("Atlantis Wall Connector", 27, 39);
			deployTownEyecandy("Atlantis Wall Long", 33, 39);
			deployTownEyecandy("Atlantis Wall Long", 39, 33, 90);
			
			deployTownEyecandy("Atlantis Wall Connector", 39, 3);
			deployTownEyecandy("Atlantis Wall Connector", 27, 3);
			deployTownEyecandy("Atlantis Wall Connector", 39, 15);
			deployTownEyecandy("Atlantis Wall Long", 33, 3);
			deployTownEyecandy("Atlantis Wall Long", 39, 9, 90);
			
			deployTownEyecandy("Atlantis Wall Connector", 3, 39);
			deployTownEyecandy("Atlantis Wall Connector", 3, 27);
			deployTownEyecandy("Atlantis Wall Connector", 15, 39);
			deployTownEyecandy("Atlantis Wall Long", 3, 33, 90);
			deployTownEyecandy("Atlantis Wall Long", 9, 39);
			
			trQuestVarSet("deepDeployCenterX", 70 * x + 40);
			trQuestVarSet("deepDeployCenterZ", 70 * z + 40);
			
			trPaintTerrain(x * 35 + 11, z * 35 + 11, x * 35 + 17, z * 35 + 11, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trPaintTerrain(x * 35 + 11, z * 35 + 11, x * 35 + 11, z * 35 + 17, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			
			trPaintTerrain(x * 35 + 29, z * 35 + 29, x * 35 + 23, z * 35 + 29, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trPaintTerrain(x * 35 + 29, z * 35 + 29, x * 35 + 29, z * 35 + 23, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			
			trPaintTerrain(x * 35 + 29, z * 35 + 11, x * 35 + 23, z * 35 + 11, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trPaintTerrain(x * 35 + 29, z * 35 + 11, x * 35 + 29, z * 35 + 17, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			
			trPaintTerrain(x * 35 + 11, z * 35 + 29, x * 35 + 11, z * 35 + 23, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trPaintTerrain(x * 35 + 11, z * 35 + 29, x * 35 + 17, z * 35 + 29, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			
			paintEnemies(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35);
			
			xsEnableRule("deep_village_always");
			trQuestVarSet("deepDeployNext", trTime() + 30);
		}
		case ROOM_VILLAGE + 8:
		{
			paintCircle(x, z, 12, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			trQuestVarSet("villageX", 70 * x + 16);
			trQuestVarSet("villageZ", 70 * z + 16);
			trQuestVarSet("stageWonder", trGetNextUnitScenarioNameNumber());
			trUnitSelectClear();
			trUnitSelect(""+deployTownEyecandy("Cinematic Block", 34, 34, 225), true);
			trUnitConvert(ENEMY_PLAYER);
			trMutateSelected(kbGetProtoUnitID("Wonder SPC"));
			dir = vector(0,0,-1);
			for(i=0; < 7) {
				xSetPointer(dEnemiesIncoming, xAddDatabaseBlock(dEnemiesIncoming));
				xSetInt(dEnemiesIncoming, xUnitName, trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Statue of Lightning",1,
					70.0 * x + 41.0 - 16.0 * xsVectorGetX(dir),0,70.0 * z + 41.0 - 16.0 * xsVectorGetZ(dir),0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				dir = rotationMatrix(dir, 0.707107, 0.707107);
			}
		}
		case ROOM_VILLAGE + 9:
		{
			paintCircle(x, z, 12, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			trQuestVarSet("villageX", 70 * x + 16);
			trQuestVarSet("villageZ", 70 * z + 16);
			trQuestVarSet("stageWonder", trGetNextUnitScenarioNameNumber());
			trUnitSelectClear();
			trUnitSelect(""+deployTownEyecandy("Cinematic Block", 25, 25, 225), true);
			trUnitConvert(ENEMY_PLAYER);
			trMutateSelected(kbGetProtoUnitID("Wonder SPC"));
			dir = vector(0,0,-1);
			for(i=0; < 8) {
				xSetPointer(dEnemiesIncoming, xAddDatabaseBlock(dEnemiesIncoming));
				xSetInt(dEnemiesIncoming, xUnitName, trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Fire Giant",1,
					70.0 * x + 41.0 - 16.0 * xsVectorGetX(dir),0,70.0 * z + 41.0 - 16.0 * xsVectorGetZ(dir),0,true);
				trArmySelect(""+ENEMY_PLAYER+",0");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				dir = rotationMatrix(dir, 0.707107, 0.707107);
			}
		}
		case ROOM_STARTER:
		{
			trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, worldHeight, false);
			paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
			trVectorQuestVarSet("startPosition", xsVectorSet(x*70 + 40,0,z*70+40));
			if (trQuestVarGet("newPlayers") > 0) {
				pos = xsVectorSet(x*70+48,0,z*70+54);
				trQuestVarSet("choice1unit", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0", "Hero Greek Theseus", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
				pos = xsVectorSet(x*70+54,0,z*70+48);
				trQuestVarSet("choice2unit", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0", "Hero Greek Hippolyta", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),225,false);
				trArmySelect("1,0");
				trUnitConvert(0);
				xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
				xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("choice1unit"));
				xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Hero Greek Theseus"));
				xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
				xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("choice2unit"));
				xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Hero Greek Hippolyta"));
				xsEnableRule("class_shop_always");
			}
		}
		case ROOM_BOSS:
		{
			size = trQuestVarGet("bossRoomSize");
			if (trQuestVarGet("mapType") == MAP_OPEN) {
				trPaintTerrain(x*35+17-size,z*35+17-size,x*35+23+size,z*35+23+size, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				trChangeTerrainHeight(x*35+17-size,z*35+17-size,x*35+24+size,z*35+24+size, wallHeight, false);
				trQuestVarSet("bossEntranceRadius", xsPow(8, 2));
			} else {
				trQuestVarSet("bossEntranceRadius", xsPow(2*size, 2));
			}
			switch(1*trQuestVarGet("bossRoomShape"))
			{
				case ROOM_CIRCLE:
				{
					z0 = size;
					for(a=0; < size+3) {
						for(b=size+3; >0) {
							if (a*a + z0 * z0 <= size * size) {
								if (trQuestVarGet("stage") == 3) {
									trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, 5, 0, false);
								} else {
									trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
								}
								trChangeTerrainHeight(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, worldHeight, false);
								break;
							} else {
								z0 = z0 - 1;
							}
						}
					}
				}
				case ROOM_SQUARE:
				{
					trPaintTerrain(x*35+20-size,z*35+20-size,x*35+20+size,z*35+20+size, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
					trChangeTerrainHeight(x*35+20-size,z*35+20-size,x*35+21+size,z*35+21+size, worldHeight, false);
				}
				
			}
			paintSecondary(x*35+20-size,z*35+20-size,x*35+20+size,z*35+20+size);
			trVectorQuestVarSet("bossRoomCenter", xsVectorSet(x * 70 + 40, 0, z * 70 + 40));
			trVectorQuestVarSet("bossRoomLoc", vectorToGrid(trVectorQuestVarGet("bossRoomCenter")));
			trVectorQuestVarSet("bossRoomEntrance", trVectorQuestVarGet("bossRoomCenter"));
			
			xsEnableRule("enter_boss_room");
		}
		case ROOM_NOTTUD:
		{
			dNottudShop = xInitDatabase("nottudShop");
			xInitAddInt(dNottudShop,"name");
			xInitAddInt(dNottudShop,"type");
			xNottudShopPos = xInitAddVector(dNottudShop,"pos");
			
			paintCircle(x, z, 10, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			trQuestVarSet("nottud", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0", "Victory Marker", 1, 70*x+40, 0, 70*z+40,225,true);
			trUnitSelectClear();
			trUnitSelectByQV("nottud", true);
			trUnitConvert(0);
			trUnitChangeProtoUnit("Minotaur");
			xSetPointer(dStunnedUnits, xAddDatabaseBlock(dStunnedUnits));
			xSetInt(dStunnedUnits, xUnitName, 1*trQuestVarGet("nottud"));
			xSetInt(dStunnedUnits, xStunnedProto, kbGetProtoUnitID("Minotaur"));
			xsEnableRule("nottud_always");
			for(i=0; <5) {
				trQuestVarSet("choice"+i, RELIC_CURSED_RANGE + i);
			}
			trQuestVarSet("choice5", RELIC_NOTTUD_TICKET);
			trQuestVarSet("obeliskx0", 70*x+46);
			trQuestVarSet("obeliskz0", 70*z+46);
			trQuestVarSet("obeliskx1", 70*x+46);
			trQuestVarSet("obeliskz1", 70*z+34);
			trQuestVarSet("obeliskx2", 70*x+34);
			trQuestVarSet("obeliskz2", 70*z+46);
			for(i=0; <3) {
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("obeliskx"+i),0,trQuestVarGet("obeliskz"+i),225,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Spy Eye");
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trMutateSelected(kbGetProtoUnitID("Osiris Box Glow"));
				trSetSelectedScale(0,1,0);
				trSetSelectedUpVector(0,-1,0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("obeliskx"+i),0,trQuestVarGet("obeliskz"+i),225,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trMutateSelected(kbGetProtoUnitID("Outpost"));
				trQuestVarSetFromRand("rand", 0, 5 - i, true);
				xSetPointer(dNottudShop, xAddDatabaseBlock(dNottudShop));
				xSetInt(dNottudShop, xUnitName, 1*trQuestVarGet("next"));
				xSetInt(dNottudShop, xRelicType, 1*trQuestVarGet("choice"+1*trQuestVarGet("rand")));
				xSetVector(dNottudShop, xNottudShopPos, xsVectorSet(trQuestVarGet("obeliskx"+i),0,trQuestVarGet("obeliskz"+i)));
				trQuestVarSet("choice"+1*trQuestVarGet("rand"), trQuestVarGet("choice"+(5-i)));
			}
		}
		case ROOM_TIGER:
		{
			paintCircle(x, z, 10, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			bossUnit = trGetNextUnitScenarioNameNumber();
			trArmyDispatch("1,0", "Victory Marker", 1, 70*x+40, 0, 70*z+40,225,true);
			trUnitSelectClear();
			trUnitSelect(""+bossUnit, true);
			trUnitConvert(ENEMY_PLAYER);
			trUnitChangeProtoUnit("White Tiger");
			xAddDatabaseBlock(dEnemiesIncoming,true);
			xSetInt(dEnemiesIncoming, xUnitName, bossUnit);
			xsEnableRule("white_tiger_find");

			trModifyProtounit("White Tiger", ENEMY_PLAYER, 0, 9999999999999999999.0);
			trModifyProtounit("White Tiger", ENEMY_PLAYER, 0, -9999999999999999999.0);
			trModifyProtounit("White Tiger", ENEMY_PLAYER, 0, 12000 * ENEMY_PLAYER);

			trModifyProtounit("Tartarian Gate flame", 0, 8, -999);

			trModifyProtounit("Victory Marker", ENEMY_PLAYER, 0, 12000 * ENEMY_PLAYER);
		}
		case ROOM_TEMPLE + 2:
		{
			trQuestVarSet("templeSize", 20);
			trVectorQuestVarSet("templePos", xsVectorSet(70*x+39,0,70*z+39));
			paintCircle(x, z, 10, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			placeTemple(x, z, 16);
			dir = vector(1,0,0);
			trQuestVarSetFromRand("templeShadeTrue", 0, 15, true);
			trQuestVarSet("templeShadesStart", trGetNextUnitScenarioNameNumber());
			for(i=0; < 16) {
				if (trQuestVarGet("templeShadeTrue") == i) {
					trQuestVarSet("templeShadeTrue", trGetNextUnitScenarioNameNumber());
				}
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Hoplite",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trUnitTeleport(70.0*x+40.0-xsVectorGetX(dir)*12.0,0,70.0*z+40.0-xsVectorGetZ(dir)*12.0);
				dir = rotationMatrix(dir, 0.923879, 0.382683);
			}
			trQuestVarSet("templeShadesEnd", trGetNextUnitScenarioNameNumber());
			xsEnableRule("shade_temple_always");
			xsEnableRule("shade_temple_init");
		}
		case ROOM_TEMPLE + 3:
		{
			trVectorQuestVarSet("templePos", xsVectorSet(70*x+39,0,70*z+39));
			trQuestVarSet("frostDriftSFX", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("templePosX"),0,trQuestVarGet("templePosZ"),0,true);
			trArmySelect("1,0");
			trUnitConvert(0);
			trUnitChangeProtoUnit("Cinematic Block");
			paintCircle(x,z,8,5,0,worldHeight);
			placeTemple(x, z, 16);
			xsEnableRule("snow_temple_always");
		}
		case ROOM_TEMPLE + 4:
		{
			trQuestVarSet("templeChallengeActive", 1);
			trPaintTerrain(x*35+12, z*35+12, x*35+28, z*35+28, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x*35+12, z*35+12, x*35+28, z*35+28, worldHeight, false);
			placeTemple(x, z, 10);
			if (trQuestVarGet("p"+trCurrentPlayer()+"relicsSacrificed") < 10) {
				/* this doesn't desync don't worry */
				xsEnableRule("greedy_temple_always");
			}
		}
		case ROOM_TEMPLE + 5:
		{
			dPoisonRelics = xInitDatabase("poisonRelics", 9);
			xInitAddInt(dPoisonRelics, "name");
			xPoisonRelicIndex = xInitAddInt(dPoisonRelics, "index");
			trPaintTerrain(x*35+12, z*35+12, x*35+28, z*35+28, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x*35+12, z*35+12, x*35+28, z*35+28, worldHeight, false);
			placeTemple(x, z, 10);
			x0 = x * 70 + 31;
			z0 = z * 70 + 47;
			trQuestVarSetFromRand("rand",0,8,true);
			for(i=0; < 9) {
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Dwarf",1,x0,0,z0,225,true);
				trArmySelect("0,0");
				trUnitChangeProtoUnit("Relic");
				if (i == trQuestVarGet("rand")) {
					trQuestVarSet("correctGoblet", trQuestVarGet("next"));
				}
				xSetPointer(dPoisonRelics, xAddDatabaseBlock(dPoisonRelics));
				xSetInt(dPoisonRelics, xUnitName, 1*trQuestVarGet("next"));
				xSetInt(dPoisonRelics, xPoisonRelicIndex, xAddDatabaseBlock(dFreeRelics));
				xSetPointer(dFreeRelics, xGetNewestPointer(dFreeRelics));
				xSetInt(dFreeRelics, xUnitName, xGetInt(dPoisonRelics, xUnitName));
				xSetInt(dFreeRelics, xRelicType, RELIC_BINARY_POISON);
				x0 = x0 + 2;
				z0 = z0 - 2;
			}
			trQuestVarSet("poisonGuesses", 3);
			trQuestVarSet("templeChallengeActive", 1);
			xsEnableRule("poison_temple_always");
			
			trQuestVarSet("doomedPlayerCount", 0);
			trQuestVarSet("doomedPlayers", zNewArray(mInt,3,"doomedPlayers"));
		}
		case ROOM_TEMPLE + 6:
		{
			trQuestVarSet("templeSize", 16);
			trVectorQuestVarSet("templePos", xsVectorSet(70*x+40,0,70*z+40));
			trVectorQuestVarSet("templeRoomUpper", xsVectorSet(70*x+56,0,70*z+56));
			trVectorQuestVarSet("templeRoomLower", xsVectorSet(70*x+24,0,70*z+24));
			trPaintTerrain(x*35+12, z*35+12, x*35+28, z*35+28, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x*35+12, z*35+12, x*35+28, z*35+28, worldHeight, false);
			placeTemple(x, z, 20);
			xsEnableRule("yeebaagooon_temple_always");
		}
		case ROOM_TEMPLE + 7:
		{
			trPaintTerrain(x*35+10, z*35+10, x*35+30, z*35+30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
			trChangeTerrainHeight(x*35+10, z*35+10, x*35+30, z*35+30, worldHeight, false);
			paintSecondary(x*35+10, z*35+10, x*35+30, z*35+30);
			paintEyecandy(x*35+10, z*35+10, x*35+30, z*35+30);
			placeTemple(x, z, 10);
			trVectorQuestVarSet("templePos", xsVectorSet(70*x+40,0,70*z+40));
			dir = vector(1,0,0);
			trQuestVarSet("columnsStart", trGetNextUnitScenarioNameNumber());
			for(i=0; < 8) {
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("0,0","Hoplite",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trUnitTeleport(70.0*x+40.0-xsVectorGetX(dir)*16.0,0,70.0*z+40.0-xsVectorGetZ(dir)*16.0);
				dir = rotationMatrix(dir, 0.707107, 0.707107);
			}
			trQuestVarSet("columnsEnd", trGetNextUnitScenarioNameNumber());
			xsEnableRule("monster_temple_init");
			xsEnableRule("monster_temple_always");
		}
		case ROOM_TEMPLE + 8:
		{
			size = 12;
			paintCircle(x, z, size, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			placeTemple(x, z, 24);
			xSetPointer(dChests, xAddDatabaseBlock(dChests));
			xSetInt(dChests, xUnitName, 1*trQuestVarGet("temple"));
			xSetInt(dChests, xChestType, CHEST_STATUES);
			xSetInt(dChests, xChestRoom, room);
			xSetInt(dChests, xChestKey, 1);
		}
		case ROOM_TEMPLE + 9:
		{
			size = 9;
			paintCircle(x, z, size, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, worldHeight);
			placeTemple(x, z, 18);
		}
		case ROOM_TEMPLE + 11:
		{
			dZenoRelics = xInitDatabase("zenoRelics");
			xInitAddInt(dZenoRelics, "name");
			
			dMirrorTowerLasers = xInitDatabase("mirrorTowerLasers");
			xInitAddInt(dMirrorTowerLasers, "name");
			xMirrorTowerLaserLength = xInitAddFloat(dMirrorTowerLasers, "length");
			xMirrorTowerLaserTimeout = xInitAddInt(dMirrorTowerLasers, "timeout");
			
			size = 12;
			paintCircle(x, z, size, 0, 53, worldHeight);
			placeTemple(x, z, 20);
			trVectorQuestVarSet("templePos", xsVectorSet(70*x+41,worldHeight + 15.0,70*z+41));
			xsEnableRule("zeno_temple_always");
		}
	}
}

int getOtherVertex(int edge = 0, int v = 0) {
	if (edge >= 16 * v) {
		return(edge - 16 * v);
	} else {
		return(edge / 16);
	}
}

void buildEdge(int edge = 0, int type = 0) {
	int first = edge / 16;
	int second = edge - 16 * first;
	int z0 = first / 4;
	int x0 = first - 4 * z0;
	int z1 = second / 4;
	int x1 = second - 4 * z1;
	int x = 0;
	int z = 0;
	float height = 0;
	vector pos = vector(0,0,0);
	if (trQuestVarGet("edge"+edge) == EDGE_NOT_FOUND) {
		if (type == EDGE_MOUNTAIN) {
			xClearDatabase(dEdgeFrontier);
			x0 = x0 * 35;
			z0 = z0 * 35;
			x1 = x1 * 35;
			z1 = z1 * 35;
			x1 = (x0 + x1) / 2 + 20;
			z1 = (z0 + z1) / 2 + 20;
			xSetPointer(dEdgeFrontier, xAddDatabaseBlock(dEdgeFrontier));
			xSetFloat(dEdgeFrontier, xEdgeFrontierHeight, wallHeight - worldHeight);
			xSetVector(dEdgeFrontier, xEdgeFrontierLoc, xsVectorSet(x1,0,z1));
			aiPlanSetUserVariableBool(dMapTiles,x1,z1, true);
			
			trQuestVarSetFromRand("size", 40, 80);
			for(a=trQuestVarGet("size"); >0) {
				trQuestVarSetFromRand("rand", 0, 7, true);
				xDatabaseNext(dEdgeFrontier);
				for(b=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dEdgeFrontier);
				}
				pos = xGetVector(dEdgeFrontier, xEdgeFrontierLoc);
				x0 = xsVectorGetX(pos);
				z0 = xsVectorGetZ(pos);
				trPaintTerrain(x0-2,z0-2,x0+1,z0+1,TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				trChangeTerrainHeight(x0-1,z0-1,x0+1,z0+1,worldHeight+xGetFloat(dEdgeFrontier,xEdgeFrontierHeight),false);
				if (xsPow(x0-x1, 2) + xsPow(z0-z1,2) < 324) {
					for(b=0; < 4) {
						trQuestVarSetFromRand("next", -1, 0, true);
						height = trQuestVarGet("next") + xGetFloat(dEdgeFrontier,xEdgeFrontierHeight);
						if (height > 0) {
							x = x0 + 2 * trQuestVarGet("rotX"+b);
							z = z0 + 2 * trQuestVarGet("rotZ"+b);
							if (aiPlanGetUserVariableBool(dMapTiles, x, z) == false) {
								xSetPointer(dEdgeFrontier, xAddDatabaseBlock(dEdgeFrontier));
								xSetFloat(dEdgeFrontier, xEdgeFrontierHeight, height);
								xSetVector(dEdgeFrontier, xEdgeFrontierLoc, xsVectorSet(x,0,z));
								aiPlanSetUserVariableBool(dMapTiles, x, z, true);
							}
						}
					}
				}
			}
		} else if (type == EDGE_PORTAL) {
			if (x0 + 4 * z0 != trQuestVarGet("village")) {
				buildRoom(x0, z0, ROOM_BASIC);
			}
			if (x1 + 4 * z1 != trQuestVarGet("village")) {
				buildRoom(x1, z1, ROOM_BASIC);
			}
			trQuestVarSet("next1", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,x0*70+41,0,z0*70+41,0,true);
			trQuestVarSet("next2", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,x1*70+41,0,z1*70+41,0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next1", true);
			trUnitConvert(0);
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelectByQV("next1", true);
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			trSetSelectedScale(0,0.2,0);
			trUnitSelectClear();
			trUnitSelectByQV("next2", true);
			trUnitConvert(0);
			trUnitChangeProtoUnit("Spy Eye");
			trUnitSelectClear();
			trUnitSelectByQV("next2", true);
			trMutateSelected(kbGetProtoUnitID("Sky Passage"));
			trSetSelectedScale(0,0.2,0);
			xAddDatabaseBlock(dSkyPassages, true);
			xSetInt(dSkyPassages, xUnitName, 1*trQuestVarGet("next1"));
			xSetInt(dSkyPassages, xSkyPassageExit, 1*trQuestVarGet("next2"));
			xAddDatabaseBlock(dSkyPassages, true);
			xSetInt(dSkyPassages, xUnitName, 1*trQuestVarGet("next2"));
			xSetInt(dSkyPassages, xSkyPassageExit, 1*trQuestVarGet("next1"));
		} else {
			z0 = z0 * 35 + 17;
			x0 = x0 * 35 + 17;
			z1 = z1 * 35 + 23;
			x1 = x1 * 35 + 23;
			/* Zeno's Paradox */
			trQuestVarSet("rand", 0);
			if (trQuestVarGet("stage") == 11) {
				trQuestVarSetFromRand("rand", 1, 5, true);
			}
			if (trQuestVarGet("rand") == 5) {
				trPaintTerrain(x0, z0, x1, z1, 0, 70, false);
				paintEyecandy(x0, z0, x1, z1, "sparkles");
				trPaintTerrain(x0, z0, x1, z1, 5, 4, false);
			} else {
				trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
				trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
				paintSecondary(x0, z0, x1, z1);
			}
			
			if ((type == EDGE_WALL) && (trQuestVarGet("wallEdges") * xGetDatabaseCount(dVisited) > 0)) {
				trQuestVarSetFromRand("rand", 1, 16, true);
				for (x=trQuestVarGet("rand"); >0) {
					xDatabaseNext(dVisited);
				}
				trQuestVarSet("room"+xGetInt(dVisited,xRoomNumber)+"key", trQuestVarGet("keyType"));
				trQuestVarSet("room"+xGetInt(dVisited,xRoomNumber)+"index", xAddDatabaseBlock(dUnlockWalls, true));
				xSetInt(dUnlockWalls, xWallSFX, trGetNextUnitScenarioNameNumber());
				xSetVector(dUnlockWalls, xWallPos, xsVectorSet(x0 + x1, 0, z0 + z1));
				xFreeDatabaseBlock(dVisited);
				
				if (x1 - x0 > z1 - z0) {
					xSetVector(dUnlockWalls, xWallMod, xsVectorSet(0, 0, z1 - z0));
					x0 = (x0 + x1);
					for(z=z0; <= z1) {
						trArmyDispatch("1,0","Dwarf",1,x0,0,2*z,0,true);
						trArmySelect("1,0");
						trMutateSelected(kbGetProtoUnitID("UI Range Indicator Greek SFX"));
					}
					x0 = x0 / 2;
					trPaintTerrain(x0-1, z0-1, x0+1, z1+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
					trChangeTerrainHeight(x0, z0, x0+1, z1+1, wallHeight, false);
					xSetInt(dUnlockWalls, xWallCircles, trGetNextUnitScenarioNameNumber());
					x0 = x0 * 2;
					for(z=z0; <= z1) {
						trArmyDispatch("1,0","Dwarf",1,x0,0,2*z,270,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
					}
				} else {
					xSetVector(dUnlockWalls, xWallMod, xsVectorSet(x1-x0,0,0));
					z0 = (z0 + z1);
					for(x=x0; <= x1) {
						trArmyDispatch("1,0","Dwarf",1,2*x,0,z0,90,true);
						trArmySelect("1,0");
						trMutateSelected(kbGetProtoUnitID("UI Range Indicator Greek SFX"));
					}
					z0 = z0 / 2;
					trPaintTerrain(x0-1, z0-1, x1+1, z0+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
					trChangeTerrainHeight(x0, z0, x1+1, z0+1, wallHeight, false);
					xSetInt(dUnlockWalls, xWallCircles, trGetNextUnitScenarioNameNumber());
					z0 = z0 * 2;
					for(x=x0; <= x1) {
						trArmyDispatch("1,0","Dwarf",1,2*x,0,z0,180,true);
						trArmySelect("1,0");
						trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
					}
				}
				xSetInt(dUnlockWalls, xWallEnd, trGetNextUnitScenarioNameNumber());
				trQuestVarSet("keyType", trQuestVarGet("keyType") - 1);
				trQuestVarSet("wallEdges", trQuestVarGet("wallEdges") - 1);
			}
			
		}
		trQuestVarSet("edge"+edge, type);
	}
}

void connectStatues(int index1 = 0, int index2 = 0, int room = 0) {
	int db = trQuestVarGet("statuesIn"+room);
	xSetPointer(db, index1);
	int index = xGetInt(db, xStatueConnections);
	xSetInt(db, xStatueConnections, 1 + index);
	aiPlanSetNumberUserVariableValues(ARRAYS,xGetInt(db,xStatueArray),xGetInt(db,xStatueConnections),false);
	aiPlanSetUserVariableInt(ARRAYS,xGetInt(db,xStatueArray),index,index2);
	vector first = xGetVector(db,xStatuePos);
	
	xSetPointer(db, index2);
	index = xGetInt(db, xStatueConnections);
	xSetInt(db, xStatueConnections, 1 + index);
	aiPlanSetNumberUserVariableValues(ARRAYS,xGetInt(db,xStatueArray),xGetInt(db,xStatueConnections),false);
	aiPlanSetUserVariableInt(ARRAYS,xGetInt(db,xStatueArray),index,index1);
	vector second = xGetVector(db,xStatuePos);
	
	vector dir = getUnitVector(first, second);
	dir = rotationMatrix(dir, 0, 1);
	
	float dist = distanceBetweenVectors(first, second, false);
	float x = 0.5 * (xsVectorGetX(first) + xsVectorGetX(second));
	float z = 0.5 * (xsVectorGetZ(first) + xsVectorGetZ(second));
	
	trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	trArmySelect("1,0");
	trUnitConvert(0);
	trSetSelectedScale(dist / 9.0, 1, 0.3);
	trSetUnitOrientation(dir,vector(0,1,0),true);
	trUnitTeleport(x,0,z);
	trMutateSelected(kbGetProtoUnitID("undermine ground decal long"));
}

rule choose_stage_02
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		xsDisableSelf();
		trBlockAllSounds();
		
		int chests = 3;
		int x = 0;
		int z = 0;
		int n = 0;
		int total = 0;
		int backtrack = 5;
		int index = 0;
		
		int room = 0;
		int x0 = 0;
		int z0 = 0;
		int x1 = 0;
		int z1 = 0;
		string pName = "";
		
		int next = 0;
		
		vector pos = vector(0,0,0);
		vector dir = vector(0,0,0);
		
		trQuestVarSetFromRand("village", 1, 14, true);
		
		/* minecraft time! */
		switch(1*trQuestVarGet("stage"))
		{
			case 1:
			{
				startDeliveryQuest(1);
				trQuestVarSet("templeRoom", -1);
				trSetCivAndCulture(0, 5, 1);
				trQuestVarSet("bossRoomSize", 16);
				trQuestVarSet("extraEdges", 9);
				/* this had better be good to hook them in */
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 0;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 34;
				
				TERRAIN_SECONDARY = 4;
				TERRAIN_SUB_SECONDARY = 12;
				
				trQuestVarSet("mapType", MAP_OPEN);
				trQuestVarSet("treeDensity", 0.06);
				trStringQuestVarSet("treeProto1", "Statue Lion Left");
				trStringQuestVarSet("treeProto2", "Statue Lion Right");
				trStringQuestVarSet("treeProto3", "Savannah Tree");
				trQuestVarSet("spriteDensity", 0.6);
				trStringQuestVarSet("spriteProto1", "Rock Sandstone Sprite");
				trStringQuestVarSet("spriteProto2", "Grass");
				trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
				trQuestVarSet("rockDensity", 0.25);
				trStringQuestVarSet("rockProto1", "Rock Sandstone Big");
				trStringQuestVarSet("rockProto2", "Rock River Sandy");
				trStringQuestVarSet("rockProto3", "Rock Sandstone Small");
				
				trQuestVarSet("enemyDensity", 0.03 + 0.03 * ENEMY_PLAYER);

				trQuestVarSet("enemyProtoCount", 5);
				trStringQuestVarSet("enemyProto1", "Golden Lion");
				trStringQuestVarSet("enemyProto2", "Anubite");
				trStringQuestVarSet("enemyProto3", "Terracotta Soldier");
				trStringQuestVarSet("enemyProto4", "Sphinx");
				trStringQuestVarSet("enemyProto5", "Petsuchos");
				
				trQuestVarSet("columnDensity", 0.05);
				
				trStringQuestVarSet("bossProto", "Nemean Lion");
				bossScale = 2;
				
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, 4000 * ENEMY_PLAYER);
				trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, 2.4);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 4000 * ENEMY_PLAYER);

				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"swordpieceQuest"+SWORD_HANDLE) == 1) {
						trQuestVarSetFromRand("excaliburRoom", 1, 14, true);
						trQuestVarSet("excaliburRoom", trQuestVarGet("village") + trQuestVarGet("excaliburRoom"));
						if (trQuestVarGet("excaliburRoom") > 14) {
							trQuestVarSet("excaliburRoom", trQuestVarGet("excaliburRoom") - 13);
						}
						break;
					}
				}
			}
			case 2:
			{
				trQuestVarSet("stageTemple", BOON_DECAY_HALVED);
				trSetCivAndCulture(0, 8, 2);
				trQuestVarSet("bossRoomSize", 12);
				trSetLighting("Fimbulwinter", 0.1);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 4;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 58;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 22;
				
				trQuestVarSet("mapType", MAP_PORTALS);
				trQuestVarSet("treeDensity", 0.2);
				trQuestVarSet("treeScale", 2);
				trStringQuestVarSet("treeProto1", "Marsh Tree");
				trStringQuestVarSet("treeProto2", "Marsh Tree");
				trStringQuestVarSet("treeProto3", "Marsh Tree");
				trQuestVarSet("spriteDensity", 0.3);
				trStringQuestVarSet("spriteProto1", "Rock Limestone Sprite");
				trStringQuestVarSet("spriteProto2", "Water Reeds");
				trStringQuestVarSet("spriteProto3", "Flowers");
				trQuestVarSet("rockDensity", 0.15);
				trStringQuestVarSet("rockProto1", "Rock Granite Big");
				trStringQuestVarSet("rockProto2", "Imperial Examination");
				trStringQuestVarSet("rockProto3", "Rock Granite Small");
				
				trQuestVarSet("enemyDensity", 0.02 + 0.02 * ENEMY_PLAYER);
				trQuestVarSet("enemyProtoCount", 5);
				trStringQuestVarSet("enemyProto1", "Walking Woods Marsh");
				trStringQuestVarSet("enemyProto2", "Centaur");
				trStringQuestVarSet("enemyProto3", "Dryad");
				trStringQuestVarSet("enemyProto4", "Medusa");
				trStringQuestVarSet("enemyProto5", "Mountain Giant");
				
				trQuestVarSet("columnDensity", 0);
				
				trStringQuestVarSet("bossProto", "Tamarisk Tree");
				bossScale = 1.5;
			}
			case 3:
			{
				startDeliveryQuest(2);
				trQuestVarSet("stageTemple", BOON_REGENERATE_HEALTH);
				trSetCivAndCulture(0, 7, 2);
				trQuestVarSet("bossRoomSize", 14);
				trQuestVarSet("extraEdges", 0);
				trQuestVarSet("wallEdges", 4);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 3;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 41;
				
				TERRAIN_SECONDARY = 5;
				TERRAIN_SUB_SECONDARY = 0;
				
				trQuestVarSet("mapType", MAP_STANDARD);
				trQuestVarSet("treeDensity", 0.1);
				trStringQuestVarSet("treeProto1", "Pine Snow");
				trStringQuestVarSet("treeProto2", "Pine Snow");
				trStringQuestVarSet("treeProto3", "Snow Drift Archery");
				trQuestVarSet("spriteDensity", 0.25);
				trStringQuestVarSet("spriteProto1", "Rock Granite Sprite");
				trStringQuestVarSet("spriteProto2", "Ice Block");
				trStringQuestVarSet("spriteProto3", "Rock Granite Small");
				trQuestVarSet("rockDensity", 0.15);
				trStringQuestVarSet("rockProto1", "Rock Granite Big");
				trStringQuestVarSet("rockProto2", "Rock River Icy");
				trStringQuestVarSet("rockProto3", "Rock River Icy");
				
				trQuestVarSet("enemyDensity", 0.02 + 0.02 * ENEMY_PLAYER);
				trQuestVarSet("enemyProtoCount", 5);
				trStringQuestVarSet("enemyProto1", "Fenris Wolf");
				trStringQuestVarSet("enemyProto2", "Valkyrie");
				trStringQuestVarSet("enemyProto3", "Ballista");
				trStringQuestVarSet("enemyProto4", "Mountain Giant");
				trStringQuestVarSet("enemyProto5", "Frost Giant");
				
				
				
				trQuestVarSet("columnDensity", 0.05);
				
				trStringQuestVarSet("bossProto", "King Folstag");
				bossScale = 1.25;
				
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, 8000 * ENEMY_PLAYER);
				trModifyProtounit("King Folstag", ENEMY_PLAYER, 1, 1.075);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 8000 * ENEMY_PLAYER);

				trModifyProtounit("King Folstag", 0, 0, 9999999999999999999.0);
				trModifyProtounit("King Folstag", 0, 0, -9999999999999999999.0);
				trModifyProtounit("King Folstag", 0, 0, 8000 * ENEMY_PLAYER);
			}
			case 4:
			{
				startDeliveryQuest(4);
				trQuestVarSet("stageTemple", BOON_MORE_GOLD);
				trSetCivAndCulture(0, statueCiv(1*trQuestVarGet("stageTemple")), statueCulture(1*trQuestVarGet("stageTemple")));
				trQuestVarSet("eyecandyStart", trGetNextUnitScenarioNameNumber());
				wallHeight = worldHeight + 6;
				trQuestVarSet("bossRoomSize", 14);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 1;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 25;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 3;
				
				trQuestVarSet("mapType", MAP_OPEN);
				trQuestVarSet("treeDensity", 0.15);
				trStringQuestVarSet("treeProto1", "Pine");
				trStringQuestVarSet("treeProto2", "Pine");
				trStringQuestVarSet("treeProto3", "Ruins");
				trQuestVarSet("spriteDensity", 0.6);
				trStringQuestVarSet("spriteProto1", "Flowers");
				trStringQuestVarSet("spriteProto2", "Grass");
				trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
				trQuestVarSet("rockDensity", 0.2);
				trStringQuestVarSet("rockProto1", "Rock Granite Big");
				trStringQuestVarSet("rockProto2", "Columns Broken");
				trStringQuestVarSet("rockProto3", "Rock Granite Small");
				
				trQuestVarSet("enemyDensity", 0.03 + 0.03 * ENEMY_PLAYER);
				
				trQuestVarSet("enemyProtoCount", 5);
				trStringQuestVarSet("enemyProto1", "Cyclops");
				trStringQuestVarSet("enemyProto2", "Satyr");
				trStringQuestVarSet("enemyProto3", "Behemoth");
				trStringQuestVarSet("enemyProto4", "Medusa");
				trStringQuestVarSet("enemyProto5", "Avenger");
				
				trStringQuestVarSet("bossProto", "Chimera");
				bossScale = 1.5;
				
				trModifyProtounit("Chimera", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Chimera", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Chimera", ENEMY_PLAYER, 0, 12000 * ENEMY_PLAYER);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 12000 * ENEMY_PLAYER);
			}
			case 5:
			{
				startDeliveryQuest(3);
				trQuestVarSet("stageTemple", BOON_SPELL_POISON);
				trSetCivAndCulture(0, 3, 1);
				trQuestVarSet("bossRoomShape", ROOM_SQUARE);
				trQuestVarSet("bossRoomSize", 11);
				trSetLighting("dawn", 0.1);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 11;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 32;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 33;
				
				trQuestVarSet("mapType", MAP_PORTALS);
				trQuestVarSet("treeDensity", 0.05);
				trStringQuestVarSet("treeProto1", "Statue Pharaoh");
				trStringQuestVarSet("treeProto2", "Statue Pharaoh");
				trStringQuestVarSet("treeProto3", "Statue Pharaoh");
				trQuestVarSet("spriteDensity", 0.3);
				trStringQuestVarSet("spriteProto1", "Rock Dirt Sprite");
				trStringQuestVarSet("spriteProto2", "Cinematic Scorch");
				trStringQuestVarSet("spriteProto3", "Skeleton");
				trQuestVarSet("rockDensity", 0.1);
				trStringQuestVarSet("rockProto1", "Rock Dirt Big");
				trStringQuestVarSet("rockProto2", "Columns Broken");
				trStringQuestVarSet("rockProto3", "Pestilence SFX1");
				
				trQuestVarSet("enemyDensity", 0.03 + 0.03 * ENEMY_PLAYER);
				
				trQuestVarSet("enemyProtoCount", 6);
				trStringQuestVarSet("enemyProto1", "Dryad");
				trStringQuestVarSet("enemyProto2", "Sphinx");
				trStringQuestVarSet("enemyProto3", "Wadjet");
				trStringQuestVarSet("enemyProto4", "Scorpion Man");
				trStringQuestVarSet("enemyProto5", "Scarab");
				trStringQuestVarSet("enemyProto6", "Mummy");
				
				trQuestVarSet("columnDensity", 0.1);
				
				trStringQuestVarSet("bossProto", "Shade of Hades");
				bossScale = 1.5;
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, 16000 * ENEMY_PLAYER);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 16000 * ENEMY_PLAYER);

				trModifyProtounit("Shade of Hades", 0, 0, 9999999999999999999.0);
				trModifyProtounit("Shade of Hades", 0, 0, -9999999999999999999.0);
				trModifyProtounit("Shade of Hades", 0, 0, 16000 * ENEMY_PLAYER);
				
				trQuestVarSet("pestilenceStart", trGetNextUnitScenarioNameNumber());
			}
			case 6:
			{
				startDeliveryQuest(5);
				trQuestVarSet("stageTemple", BOON_STATUS_COOLDOWNS);
				xsEnableRule("laser_rooms_always");
				initializeLaserTrapDatabase();
				/* engineers */
				trTechSetStatus(ENEMY_PLAYER, 59, 4);
				ambientColor(100,50,0);
				sunColor(150,150,0);
				trSetCloudData(1.0, 50.0, 50.0, 0.0, 1.0, 0.0);
				trSetCloudMap("shockwall");
				trSetCivAndCulture(0, 4, 1);
				trQuestVarSet("bossRoomShape", ROOM_SQUARE);
				trQuestVarSet("bossRoomSize", 10);
				trQuestVarSet("wallEdges", 4);
				trQuestVarSet("trapRooms", 3);
				trQuestVarSet("trapType", TRAP_LASERS);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 2;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 70;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 64;
				
				trQuestVarSet("mapType", MAP_STANDARD);
				trQuestVarSet("spriteDensity", 0.15);
				trStringQuestVarSet("spriteProto1", "Cinematic Scorch");
				trStringQuestVarSet("spriteProto2", "Cinematic Scorch");
				trStringQuestVarSet("spriteProto3", "Cinematic Scorch");
				
				trQuestVarSet("rockDensity", 0.1);
				trStringQuestVarSet("rockProto1", "Smoke");
				trStringQuestVarSet("rockProto2", "Broken Siege Weapons");
				trStringQuestVarSet("rockProto3", "Thor Hammer");
				
				trQuestVarSet("enemyDensity", 0.03 + 0.03 * ENEMY_PLAYER);
				trQuestVarSet("enemyProtoCount", 5);
				
				trStringQuestVarSet("enemyProto1", "Automaton SPC");
				trStringQuestVarSet("enemyProto2", "Ballista");
				trStringQuestVarSet("enemyProto3", "Fire Siphon");
				trStringQuestVarSet("enemyProto4", "Battle Boar");
				trStringQuestVarSet("enemyProto5", "Colossus");
				
				trStringQuestVarSet("bossProto", "Helepolis");
				bossScale = 0.25;
				
				trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, 20000 * ENEMY_PLAYER);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 20000 * ENEMY_PLAYER);

				trModifyProtounit("Helepolis", 0, 0, 9999999999999999999.0);
				trModifyProtounit("Helepolis", 0, 0, -9999999999999999999.0);
				trModifyProtounit("Helepolis", 0, 0, 20000 * ENEMY_PLAYER);
				
				trModifyProtounit("Helepolis", ENEMY_PLAYER, 13, -2);

				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"swordpieceQuest"+SWORD_HILT) == 1) {
						trQuestVarSetFromRand("electricRoom", 1, 14, true);
						trQuestVarSet("electricRoom", trQuestVarGet("village") + trQuestVarGet("electricRoom"));
						if (trQuestVarGet("electricRoom") > 14) {
							trQuestVarSet("electricRoom", trQuestVarGet("electricRoom") - 13);
						}
						break;
					}
				}
			}
			case 7:
			{
				xDeepDamageLast = xInitAddInt(dPlayerUnits, "deepDamageLast");
				trQuestVarSet("stageTemple", BOON_HEAL_FAVOR);
				trSetLighting("fimbulwinter", 0.01);
				wallHeight = worldHeight + 3;
				trSetCivAndCulture(0, 11, 3);
				trQuestVarSet("bossRoomSize", 15);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 11;
				
				TERRAIN_PRIMARY = 3;
				TERRAIN_SUB_PRIMARY = 8;
				
				TERRAIN_SECONDARY = 3;
				TERRAIN_SUB_SECONDARY = 9;
				
				trQuestVarSet("mapType", MAP_OPEN);
				trQuestVarSet("treeDensity", 0.2);
				trStringQuestVarSet("treeProto1", "Columns");
				trStringQuestVarSet("treeProto2", "Columns Broken");
				trStringQuestVarSet("treeProto3", "Columns Fallen");
				trQuestVarSet("spriteDensity", 0.5);
				trStringQuestVarSet("spriteProto1", "Seaweed");
				trStringQuestVarSet("spriteProto2", "Water Reeds");
				trStringQuestVarSet("spriteProto3", "Rock Granite Small");
				trQuestVarSet("rockDensity", 0.2);
				trStringQuestVarSet("rockProto1", "Rock Granite Big");
				trStringQuestVarSet("rockProto2", "Rock Limestone Sprite");
				trStringQuestVarSet("rockProto3", "Shipwreck");
				
				trModifyProtounit("Seaweed",1,55,1);
				trModifyProtounit("Shipwreck",1,55,1);
				
				trQuestVarSet("enemyDensity", 0.04 + 0.04 * ENEMY_PLAYER);
				
				trQuestVarSet("enemyProtoCount", 6);
				trStringQuestVarSet("enemyProto1", "Servant");
				trStringQuestVarSet("enemyProto2", "Nereid");
				trStringQuestVarSet("enemyProto3", "Wadjet");
				trStringQuestVarSet("enemyProto4", "Kraken");
				trStringQuestVarSet("enemyProto5", "Medusa");
				trStringQuestVarSet("enemyProto6", "Hydra");
				
				trStringQuestVarSet("bossProto", "Scylla");
				bossScale = 1.3;
				
				trModifyProtounit("Scylla", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Scylla", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Scylla", ENEMY_PLAYER, 0, 24000 * ENEMY_PLAYER);
				
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 0, 24000 * ENEMY_PLAYER);
				
				trQuestVarSet("fishHawks", zNewArray(mInt, 30, "fishHawks"));
				trQuestVarSet("fishes", zNewArray(mInt, 30, "fishes"));
				for(i=0; < 30) {
					next = trGetNextUnitScenarioNameNumber();
					aiPlanSetUserVariableInt(ARRAYS, 1*trQuestVarGet("fishHawks"), i, next);
					trQuestVarSetFromRand("rand",1,360,true);
					trArmyDispatch("1,0","Dwarf",1,150,0,150,trQuestVarGet("rand"),true);
					
					spyEffect(next, kbGetProtoUnitID("Cinematic Block"), xsVectorSet(ARRAYS, 1*trQuestVarGet("fishes"), i));
				}
				xsEnableRule("the_deep_build_01");
			}
			case 8:
			{
				dCloudDeployStars = initGenericProj("cloudDeployStars",kbGetProtoUnitID("Lampades"),18,0.01,-2.5,0.0,ENEMY_PLAYER);
				xDeepDamageLast = xInitAddInt(dPlayerUnits, "tornadoDamageLast");
				xsEnableRule("the_clouds_build_01");
				xsEnableRule("the_clouds_build_02");
				worldHeight = 5;
				wallHeight = -3;
				trQuestVarSet("stageTemple", BOON_DOUBLE_FAVOR);
				trSetCivAndCulture(0, 0, 0);
				trSetCivAndCulture(ENEMY_PLAYER, 0, 0);
				trQuestVarSet("bossRoomShape", ROOM_CIRCLE);
				trQuestVarSet("bossRoomSize", 10);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 1;
				
				TERRAIN_PRIMARY = 0;
				TERRAIN_SUB_PRIMARY = 53;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 50;
				
				trQuestVarSet("mapType", MAP_PORTALS);
				trQuestVarSet("treeDensity", 0.05);
				trStringQuestVarSet("treeProto1", "Ruins");
				trStringQuestVarSet("treeProto2", "Rock Granite Big");
				trStringQuestVarSet("treeProto3", "Mist");
				trQuestVarSet("spriteDensity", 0.2);
				trStringQuestVarSet("spriteProto1", "Rock Granite Sprite");
				trStringQuestVarSet("spriteProto2", "Rock Granite Small");
				trStringQuestVarSet("spriteProto3", "Rock Limestone Small");
				trQuestVarSet("rockDensity", 0.12);
				trStringQuestVarSet("rockProto1", "Columns");
				trStringQuestVarSet("rockProto2", "Columns Broken");
				trStringQuestVarSet("rockProto3", "Columns Fallen");
				
				trQuestVarSet("enemyDensity", 0.04 + 0.04 * ENEMY_PLAYER);
				
				trQuestVarSet("enemyProtoCount", 6);
				trStringQuestVarSet("enemyProto1", "Griffon");
				trStringQuestVarSet("enemyProto2", "Satyr");
				trStringQuestVarSet("enemyProto3", "Einheriar");
				trStringQuestVarSet("enemyProto4", "Avenger");
				trStringQuestVarSet("enemyProto5", "Statue of Lightning");
				trStringQuestVarSet("enemyProto6", "Lampades");
				
				trModifyProtounit("Statue of Lightning", ENEMY_PLAYER, 1, 3.0);
				
				trModifyProtounit("Nidhogg", 0, 0, 9999999999999999999.0);
				trModifyProtounit("Nidhogg", 0, 0, -9999999999999999999.0);
				trModifyProtounit("Nidhogg", 0, 0, 100000);
				trModifyProtounit("Nidhogg", 0, 24, 1);
				trModifyProtounit("Nidhogg", 0, 25, 1);
				trModifyProtounit("Nidhogg", 0, 26, 1);
				
				trStringQuestVarSet("bossProto", "Nidhogg");
				bossScale = 1.0;
				
				trQuestVarSet("cloudTornados", zNewArray(mInt, 10, "cloudTornados"));
				trQuestVarSet("cloudTornadoSFX", zNewArray(mInt, 10, "cloudTornadoSFX"));
				trQuestVarSet("cloudTornadoBlock", zNewArray(mInt, 10, "cloudTornadoBlock"));
				for(i=0; <10) {
					next = trGetNextUnitScenarioNameNumber();
					trQuestVarSetFromRand("heading", 1, 360, true);
					aiPlanSetUserVariableInt(ARRAYS,1*trQuestVarGet("cloudTornados"),i,next);
					trArmyDispatch(""+ENEMY_PLAYER+",0","Militia",1,145,0,145,trQuestVarGet("heading"),true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					spyEffect(next,kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS, 1*trQuestVarGet("cloudTornadoSFX"), i));
					spyEffect(next,kbGetProtoUnitID("Invisible Target"),xsVectorSet(ARRAYS, 1*trQuestVarGet("cloudTornadoBlock"), i));
				}
			}
			case 9:
			{
				dPitGates = xInitDatabase("pitGates");
				xInitAddInt(dPitGates, "name");

				dPitSpawn = xInitDatabase("pitSpawn");
				xInitAddInt(dPitSpawn, "name");
				xPitSpawnNext = xInitAddInt(dPitSpawn, "next");

				xsEnableRule("carousel_rooms_always");
				worldHeight = 5;
				wallHeight = 10;
				trQuestVarSet("stageTemple", BOON_ATTACK_PROLONGS_STUN); // kronos
				trSetCivAndCulture(0, 9, 3);
				trSetCivAndCulture(ENEMY_PLAYER, 9, 3);
				trQuestVarSet("bossRoomShape", ROOM_CIRCLE);
				trQuestVarSet("bossRoomSize", 15);
				trSetLighting("hades", 0.1);

				initializeCarouselTrapDatabase();

				xsEnableRule("the_pit_build_01");
				xsEnableRule("the_pit_build_02");

				trQuestVarSet("wallEdges", 5);
				trQuestVarSet("trapRooms", 3);
				trQuestVarSet("trapType", TRAP_CAROUSEL);
				TERRAIN_WALL = 2; // hades cliff
				TERRAIN_SUB_WALL = 11;
				
				TERRAIN_PRIMARY = 0; // hades buildable 1
				TERRAIN_SUB_PRIMARY = 84;
				
				TERRAIN_SECONDARY = 0; // hades buildable 2
				TERRAIN_SUB_SECONDARY = 85;
				
				trQuestVarSet("mapType", MAP_STANDARD);
				trQuestVarSet("treeDensity", 0.1);
				trStringQuestVarSet("treeProto1", "Ruins");
				trStringQuestVarSet("treeProto2", "Rock Granite Big");
				trStringQuestVarSet("treeProto3", "Smoke");
				trQuestVarSet("spriteDensity", 0.3);
				trStringQuestVarSet("spriteProto1", "Skeleton");
				trStringQuestVarSet("spriteProto2", "Rock Granite Small");
				trStringQuestVarSet("spriteProto3", "Rock Limestone Small");
				trQuestVarSet("rockDensity", 0.3);
				trStringQuestVarSet("rockProto1", "Stalagmite");
				trStringQuestVarSet("rockProto2", "Rock Granite Big");
				trStringQuestVarSet("rockProto3", "Stalagmite");

				trQuestVarSet("columnDensity", 0.1);
				
				trQuestVarSet("enemyDensity", 0.04 + 0.04 * ENEMY_PLAYER);
				
				trQuestVarSet("enemyProtoCount", 6);
				trStringQuestVarSet("enemyProto1", "Tartarian Gate Spawn");
				trStringQuestVarSet("enemyProto2", "Manticore");
				trStringQuestVarSet("enemyProto3", "Troll");
				trStringQuestVarSet("enemyProto4", "Frost Giant");
				trStringQuestVarSet("enemyProto5", "Mummy");
				trStringQuestVarSet("enemyProto6", "Fire Giant");

				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 0, 9999999999999999999.0);
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 0, -9999999999999999999.0);
				trModifyProtounit("Titan Atlantean", ENEMY_PLAYER, 0, 32000 * ENEMY_PLAYER);

				trModifyProtounit("Titan Atlantean", 0, 0, 9999999999999999999.0);
				trModifyProtounit("Titan Atlantean", 0, 0, -9999999999999999999.0);
				trModifyProtounit("Titan Atlantean", 0, 0, 32000 * ENEMY_PLAYER);
				
				trStringQuestVarSet("bossProto", "Heka Gigantes");
				bossScale = 1.0;

				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"swordpieceQuest"+SWORD_BLADE) == 1) {
						trQuestVarSetFromRand("puzzleRoom", 1, 14, true);
						trQuestVarSet("puzzleRoom", trQuestVarGet("village") + trQuestVarGet("puzzleRoom"));
						if (trQuestVarGet("puzzleRoom") > 14) {
							trQuestVarSet("puzzleRoom", trQuestVarGet("puzzleRoom") - 13);
						}
						break;
					}
				}
			}
			case 11:
			{
				trQuestVarSet("stageTemple", BOON_SPELL_ATTACK);
				trQuestVarSet("village", -1);
				trStringQuestVarSet("advice", "And then there were none...");
				xsEnableRule("laser_rooms_always");
				initializeLaserTrapDatabase();
				trSetCivAndCulture(0, statueCiv(1*trQuestVarGet("stageTemple")), statueCulture(1*trQuestVarGet("stageTemple")));
				worldHeight = 0;
				wallHeight = 0;
				trQuestVarSet("bossRoomShape", ROOM_CIRCLE);
				trQuestVarSet("bossRoomSize", 12);
				trSetLighting("eclipse", 0.1);
				
				trQuestVarSet("trapRooms", 3);
				trQuestVarSet("trapType", TRAP_LASERS);
				TERRAIN_WALL = 2;
				TERRAIN_SUB_WALL = 13;
				
				TERRAIN_PRIMARY = 9;
				TERRAIN_SUB_PRIMARY = 0;
				
				TERRAIN_SECONDARY = 0;
				TERRAIN_SUB_SECONDARY = 73;
				
				trQuestVarSet("mapType", MAP_STANDARD);
				trQuestVarSet("treeDensity", 0.1);
				trStringQuestVarSet("treeProto1", "Oak Tree Burning");
				trStringQuestVarSet("treeProto2", "Marsh Tree");
				trStringQuestVarSet("treeProto3", "Pine Snow");
				trQuestVarSet("spriteDensity", 0.3);
				trStringQuestVarSet("spriteProto1", "Cinematic Dead Bodies");
				trStringQuestVarSet("spriteProto2", "Cinematic Dead Bodies Xpack");
				trStringQuestVarSet("spriteProto3", "Columns");
				trQuestVarSet("rockDensity", 0.1);
				trStringQuestVarSet("rockProto1", "Ruins");
				trStringQuestVarSet("rockProto2", "Columns Broken");
				trStringQuestVarSet("rockProto3", "Cinematic Dead Bodies");
				trQuestVarSet("sparklesDensity", 0.5);
				trStringQuestVarSet("sparklesProto1", "Imperial Examination");
				trStringQuestVarSet("sparklesProto2", "Imperial Examination");
				trStringQuestVarSet("sparklesProto3", "Imperial Examination");
				
				trQuestVarSet("enemyDensity", 0.02 + 0.02 * ENEMY_PLAYER);
				
				
				trQuestVarSet("enemyProtoCount", ENEMY_PLAYER - 1);
				for(p=1; < ENEMY_PLAYER) {
					trStringQuestVarSet("enemyProto" + p,
						kbGetProtoUnitName(xGetInt(dClass, xClassProto, xGetInt(dPlayerData, xPlayerClass, p))));
				}
				
				trQuestVarSet("timeshiftHawks", zNewArray(mInt, 20, "timeshiftHawks"));
				trQuestVarSet("timeshiftHawkSFX", zNewArray(mInt, 20, "timeshiftHawkSFX"));
				for(i=0; < 20) {
					next = trGetNextUnitScenarioNameNumber();
					aiPlanSetUserVariableInt(ARRAYS,1*trQuestVarGet("timeshiftHawks"),i,next);
					trQuestVarSetFromRand("rand",1,360,true);
					trArmyDispatch("1,0","Dwarf",1,150,0,150,trQuestVarGet("rand"),true);
					spyEffect(next,kbGetProtoUnitID("Timeshift In"),xsVectorSet(ARRAYS, 1*trQuestVarGet("timeshiftHawkSFX"), i));
				}
				xsEnableRule("zenos_paradox_build_01");
				
				trQuestVarSet("templeRoom", 5);
				
				trQuestVarSet("akard", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,50,0,50,225,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trUnitChangeProtoUnit("Oracle Scout");

				trModifyProtounit("Revealer", ENEMY_PLAYER, 0, 9999999);
				trStringQuestVarSet("bossProto", "Revealer");
			}
		}
		
		/* paint entire map cliff and raise it */
		trChangeTerrainHeight(0,0,145,145,wallHeight,false);
		trPaintTerrain(0,0,145,145,TERRAIN_WALL, TERRAIN_SUB_WALL,false);

		if (trQuestVarGet("stage") == 9) {
			// on stage 9, walls are +10 but the lava is -3
			trChangeTerrainHeight(0,0,145,145,-3,false);
		}
		
		if (trQuestVarGet("mapType") == MAP_OPEN) {
			trChangeTerrainHeight(5,5,140,140,worldHeight,false);
			trPaintTerrain(5,5,139,139,TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY,false);
			paintSecondary(5,5,139,139);
		}
		
		dSkyPassages = xInitDatabase("skyPassages");
		xInitAddInt(dSkyPassages,"name");
		xSkyPassageExit = xInitAddInt(dSkyPassages, "exit");
		
		dMagicalJourney = xInitDatabase("magicalJourney");
		xInitAddInt(dMagicalJourney, "name");
		
		/* build the grid */
		trQuestVarSet("keyType", RELIC_KEY_EGYPT);
		
		bool edgeIsPortal = false;
		trQuestVarSet("tile0", TILE_VISITED);
		trQuestVarSet("tile1", TILE_FOUND);
		trQuestVarSet("tile4", TILE_FOUND);
		
		if (trQuestVarGet("mapType") == MAP_PORTALS) {
			trQuestVarSet("portalsActive", 1);
			total = 1;
			trQuestVarSet("tile"+1*trQuestVarGet("village"), TILE_VISITED);
			trQuestVarSetFromRand("villageEntrance", 1, 13, true);
			trQuestVarSet("villageEntrance", trQuestVarGet("village") + trQuestVarGet("villageEntrance"));
			if (trQuestVarGet("villageEntrance") > 14) {
				trQuestVarSet("villageEntrance", trQuestVarGet("villageEntrance") - 14);
			}
			buildEdge(edgeName(1*trQuestVarGet("villageEntrance"), 1*trQuestVarGet("village")), EDGE_PORTAL);
		} else if (trQuestVarGet("mapType") == MAP_OPEN) {
			trQuestVarSetFromRand("bossRoomEntranceMod", 1, 13, true);
			trQuestVarSet("bossEntranceRoom", trQuestVarGet("village") + trQuestVarGet("bossRoomEntranceMod"));
			if (trQuestVarGet("bossEntranceRoom") > 14) {
				trQuestVarSet("bossEntranceRoom", trQuestVarGet("bossEntranceRoom") - 14);
			}
			trQuestVarSetFromRand("bossKeyRoomMod", 1, 13, true);
			trQuestVarSet("bossKeyRoom", trQuestVarGet("bossEntranceRoom") + trQuestVarGet("bossKeyRoomMod"));
			if (trQuestVarGet("bossKeyRoom") > 14) {
				trQuestVarSet("bossKeyRoom", trQuestVarGet("bossKeyRoom") - 14);
			}
			dMapTiles = aiPlanCreate("mapTiles", 8);
			dEdgeFrontier = xInitDatabase("edgeFrontier");
			xEdgeFrontierHeight = xInitAddFloat(dEdgeFrontier, "height");
			xEdgeFrontierLoc = xInitAddVector(dEdgeFrontier, "location");
			for(i=0; < 144) {
				if (aiPlanAddUserVariableBool(dMapTiles,i,"row"+i,144) == false) {
					trSoundPlayFN("cantdothat.wav","1",-1,"","");
					debugLog("Cannot create new user variable at " + i);
				}
			}
		}
		
		if (trQuestVarGet("templeRoom") == 0) {
			trQuestVarSetFromRand("templeRoom", 1, 14, true);
			trQuestVarSet("templeRoom", trQuestVarGet("templeRoom") + trQuestVarGet("village"));
			if (trQuestVarGet("templeRoom") > 14) {
				trQuestVarSet("templeRoom", trQuestVarGet("templeRoom") - 14);
			}
		}
		
		dVisited = xInitDatabase("visited");
		xInitAddInt(dVisited,"roomNumber");
		dFrontier = xInitDatabase("frontier");
		xInitAddInt(dFrontier, "tile");
		xInitAddInt(dFrontier,"roomNumber");
		xFrontierEdge = xInitAddInt(dFrontier, "edge");
		xFrontierType = xInitAddInt(dFrontier, "type");
		
		if (trQuestVarGet("tile1") == TILE_FOUND) {
			xAddDatabaseBlock(dFrontier, true);
			xSetInt(dFrontier, xRoomNumber, 1);
			xSetInt(dFrontier, xFrontierEdge, edgeName(0, 1));
			xSetInt(dFrontier, xFrontierType, EDGE_NORMAL);
		}
		if (trQuestVarGet("mapType") == MAP_OPEN) {
			xSetInt(dFrontier, xFrontierType, EDGE_MOUNTAIN);
		}
		
		if (trQuestVarGet("tile4") == TILE_FOUND) {
			xAddDatabaseBlock(dFrontier, true);
			xSetInt(dFrontier, xRoomNumber, 4);
			xSetInt(dFrontier, xFrontierEdge, edgeName(0, 4));
			xSetInt(dFrontier, xFrontierType, EDGE_NORMAL);
		}
		if (trQuestVarGet("mapType") == MAP_OPEN) {
			xSetInt(dFrontier, xFrontierType, EDGE_MOUNTAIN);
		}
		
		trQuestVarSetFromRand("relicTransporterDepth", 1, 5, true);
		
		/* build guaranteed path to every room */
		for(i=0; < 64) {
			trQuestVarSetFromRand("search", 0, backtrack, true);
			for(j=trQuestVarGet("search"); >0) {
				xDatabaseNext(dFrontier, true);
			}
			index = xGetPointer(dFrontier);
			if (trQuestVarGet("tile"+xGetInt(dFrontier, xRoomNumber)) < TILE_VISITED) {
				z = xGetInt(dFrontier, xRoomNumber) / 4;
				x = xGetInt(dFrontier, xRoomNumber) - 4 * z;
				buildEdge(xGetInt(dFrontier, xFrontierEdge), xGetInt(dFrontier, xFrontierType));
				
				edgeIsPortal = (xGetInt(dFrontier, xFrontierType) == EDGE_PORTAL);
				trQuestVarSet("tile"+xGetInt(dFrontier, xRoomNumber), TILE_VISITED);
				if (xGetInt(dFrontier, xRoomNumber) < 15 && xGetInt(dFrontier, xRoomNumber) != trQuestVarGet("village")) {
					xAddDatabaseBlock(dVisited, true);
					xSetInt(dVisited, xRoomNumber, xGetInt(dFrontier, xRoomNumber));
				}
				/* only add more edges if the room is not the boss room or the village room */
				if (xGetInt(dFrontier, xRoomNumber) < 15 && xGetInt(dFrontier, xRoomNumber) != trQuestVarGet("village")) {
					for(a=1; >=0) {
						for(b=1; >=0) {
							trQuestVarSet("newX", (1 - 2 * b) * a + x);
							trQuestVarSet("newZ", (1 - 2 * b) * (1 - a) + z);
							if (trQuestVarGet("newX") < 0 || trQuestVarGet("newZ") < 0 ||
								trQuestVarGet("newX") > 3 || trQuestVarGet("newZ") > 3) {
								continue;
							}
							n = 0 + trQuestVarGet("newX") + 4 * trQuestVarGet("newZ");
							if (trQuestVarGet("tile"+n) < TILE_VISITED) {
								xAddDatabaseBlock(dFrontier, true);
								xSetInt(dFrontier, xRoomNumber, n);
								xSetInt(dFrontier, xFrontierEdge, edgeName(xGetInt(dFrontier, xRoomNumber, index), n));
								xSetInt(dFrontier, xFrontierType, EDGE_NORMAL);
								if (trQuestVarGet("wallEdges") > 0 && xGetDatabaseCount(dVisited) > 0) {
									trQuestVarSetFromRand("rand", 1, xsMin(3, trQuestVarGet("wallEdges")), true);
									if (trQuestVarGet("rand") == 1) {
										xSetInt(dFrontier, xFrontierType, EDGE_WALL);
									}
								}
								if (trQuestVarGet("mapType") == MAP_OPEN) {
									xSetInt(dFrontier, xFrontierType, EDGE_MOUNTAIN);
								}
							}
						}
					}
					if (edgeIsPortal == false) {
						trQuestVarSet("relicTransporterDepth", trQuestVarGet("relicTransporterDepth") - 1);
						if ((trQuestVarGet("relicTransporterDepth") <= 0) &&
							(trQuestVarGet("relictransporterguy") == 0) &&
							(xGetInt(dFrontier, xRoomNumber, index) != trQuestVarGet("villageEntrance")) &&
							(xGetInt(dFrontier, xRoomNumber, index) != trQuestVarGet("bossEntranceRoom"))) {
							trQuestVarSet("relicTransporterGuy", xGetInt(dFrontier, xRoomNumber, index));
						} else if (trQuestVarGet("mapType") == MAP_PORTALS) {
							trQuestVarSetFromRand("rand", 1, 14, true);
							n = trQuestVarGet("rand");
							if (trQuestVarGet("tile"+n) < TILE_VISITED &&
								n != trQuestVarGet("villageEntrance") &&
								n != trQuestVarGet("relicTransporterGuy") &&
								trQuestVarGet("villageEntrance") != xGetInt(dFrontier, xRoomNumber, index)) {
								xAddDatabaseBlock(dFrontier, true);
								xSetInt(dFrontier, xRoomNumber, n);
								xSetInt(dFrontier, xFrontierEdge, edgeName(xGetInt(dFrontier, xRoomNumber, index), n));
								xSetInt(dFrontier, xFrontierType, EDGE_PORTAL);
							}
						}
					}
				}
				
				total = total + 1;
				if (total == 15) {
					break;
				}
			} else {
				backtrack = backtrack + 1;
			}
			xFreeDatabaseBlock(dFrontier, index);
		}
		
		buildRoom(0,0, ROOM_STARTER);
		buildRoom(3,3, ROOM_BOSS);
		
		/* make portals more visible */
		for(x=xGetDatabaseCount(dSkyPassages); >0) {
			xDatabaseNext(dSkyPassages);
			pos = kbGetBlockPosition(""+xGetInt(dSkyPassages, xUnitName));
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-4,0,xsVectorGetZ(pos),0,true);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)+4,0,xsVectorGetZ(pos),0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos)-4,0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos)+4,0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)+3,0,xsVectorGetZ(pos)+3,0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)+3,0,xsVectorGetZ(pos)-3,0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-3,0,xsVectorGetZ(pos)+3,0,false);
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos)-3,0,xsVectorGetZ(pos)-3,0,false);
			trArmySelect("1,0");
			trUnitConvert(0);
			trUnitChangeProtoUnit("Garrison Flag Sky Passage");
		}
		
		bool nottudSpawn = false;
		trQuestVarSetFromRand("nottudSpawn", 0, 20, true);
		if (trQuestVarGet("nottudSpawn") < trQuestVarGet("stage")) {
			nottudSpawn = true;
		}

		bool tigerSpawn = false;
		if (trQuestVarGet("stage") > 1 && trQuestVarGet("stage") < 7) {
			trQuestVarSetFromRand("tigerSpawn", 1, 10, true);
			if (trQuestVarGet("tigerSpawn") < trQuestVarGet("stage")) {
				tigerSpawn = true;
			}
		}
		
		if (trQuestVarGet("stage") > 10) {
			trQuestVarSet("relicTransporterGuy", -1);
			trQuestVarSet("bossEntranceRoom", -1);
			chests = 0;
			nottudSpawn = false;
		}
		
		for (i=1; < 15) {
			z = i / 4;
			x = i - z * 4;
			if (trQuestVarGet("room"+i) == 0) {
				if (chests > 0) {
					trQuestVarSetFromRand("chestRand", 1, 12 - trQuestVarGet("stage"), true);
				} else {
					trQuestVarSet("chestRand", 0);
				}
				if (i == 1*trQuestVarGet("bossEntranceRoom")) {
					buildRoom(x, z, ROOM_BOSS_ENTRANCE);
				} else if (i == 1*trQuestVarGet("puzzleRoom")) {
					buildRoom(x, z, ROOM_PUZZLE);
				} else if (i == 1*trQuestVarGet("electricRoom")) {
					buildRoom(x, z, ROOM_ELECTRIC);
				} else if (i == 1*trQuestVarGet("excaliburRoom")) {
					buildRoom(x, z, ROOM_EXCALIBUR);
				} else if (i == 1*trQuestVarGet("relicTransporterGuy")) {
					buildRoom(x, z, ROOM_TRANSPORTER_GUY);
				} else if (i == 1*trQuestVarGet("village")) {
					xsEnableRule("town_always");
					buildRoom(x, z, ROOM_VILLAGE + trQuestVarGet("stage"));
					/* quest givers */
					for(x=3; >0) {
						if (trQuestVarGet("localQuest") == x) {
							pos = kbGetBlockPosition(""+1*trQuestVarGet("guy"+x));
							trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Healing SFX");
							trQuestVarSet("questGuy", trQuestVarGet("guy"+x));
						} else {
							xAddDatabaseBlock(dNpcTalk, true);
							xSetInt(dNpcTalk, xUnitName, 1*trQuestVarGet("guy"+x));
							if ((trQuestVarGet("stage") < 4) && (x == SHOP_GUY) && (trQuestVarGet("p"+trCurrentPlayer()+"monsterpediaQuest") == 1)) {
								xSetInt(dNpcTalk, xNpcDialog, MONSTERPEDIA_NPC + trQuestVarGet("stage"));
							} else {
								xSetInt(dNpcTalk, xNpcDialog, 10 * x + trQuestVarGet("stage"));
							}
						}
					}
				} else if (trQuestVarGet("templeRoom") == i) {
					xsEnableRule("find_temple");
					buildRoom(x, z, ROOM_TEMPLE + trQuestVarGet("stage"));
				} else if (nottudSpawn && (countRoomEntrances(x, z) == 1)) {
					buildRoom(x, z, ROOM_NOTTUD);
					nottudSpawn = false;
				} else if (tigerSpawn && (countRoomEntrances(x, z) == 1)) {
					buildRoom(x, z, ROOM_TIGER);
					tigerSpawn = false;
				} else if (trQuestVarGet("chestRand") == 1) {
					chests = chests - 1;
					buildRoom(x, z, ROOM_CHEST);
				} else {
					trQuestVarSetFromRand("roomType", ROOM_BASIC, ROOM_AMBUSH, true);
					trQuestVarSetFromRand("roomType2", ROOM_BASIC, ROOM_AMBUSH, true);
					if (trQuestVarGet("roomType2") < trQuestVarGet("roomType")) {
						trQuestVarSet("roomType", trQuestVarGet("roomType2"));
					}
					buildRoom(x, z, 1*trQuestVarGet("roomType"));
				}
			}
		}
		
		int dStatuesReady = xInitDatabase("statuesReady");
		int xStatuesReadyIndex = xInitAddInt(dStatuesReady, "index");
		
		/* finish making chest rooms */
		trQuestVarSet("keyType", RELIC_KEY_GREEK);
		for(i=xGetDatabaseCount(dChests); >0) {
			xDatabaseNext(dChests);
			room = xGetInt(dChests, xChestRoom);
			pos = trVectorQuestVarGet("room"+room+"bottom1");
			x0 = xsVectorGetX(pos);
			z0 = xsVectorGetZ(pos);
			pos = trVectorQuestVarGet("room"+room+"top1");
			x1 = xsVectorGetX(pos);
			z1 = xsVectorGetZ(pos);
			switch(xGetInt(dChests,xChestType))
			{
				case CHEST_KEY:
				{
					paintEnemies(x0, z0, x1, z1);
					trQuestVarSetFromRand("key", 1, 14, true);
					for(j=trQuestVarGet("key"); >0) {
						room = xDatabaseNext(dBasicRooms);
					}
					pos = trVectorQuestVarGet("room"+room+"bottom1");
					x0 = xsVectorGetX(pos);
					z0 = xsVectorGetZ(pos);
					pos = trVectorQuestVarGet("room"+room+"top1");
					x1 = xsVectorGetX(pos);
					z1 = xsVectorGetZ(pos);
					pos = randomNearEdge(x0, z0, x1, z1);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Militia",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitChangeProtoUnit("Relic");
					xAddDatabaseBlock(dFreeRelics,true);
					xSetInt(dFreeRelics,xRelicName, 1*trQuestVarGet("next"));
					xSetInt(dFreeRelics, xRelicType, 1*trQuestVarGet("keyType"));
					
					xSetInt(dChests, xChestKey, 1*trQuestVarGet("next"));
					
					pos = kbGetBlockPosition(""+xGetInt(dChests,xUnitName));
					pos = xsVectorSet(xsVectorGetX(pos) - 2, 0, xsVectorGetZ(pos) - 2);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitConvert(0);
					trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
					xSetInt(dChests, xChestSFX, 1*trQuestVarGet("next"));
					
					trQuestVarSet("keyType", 1 + trQuestVarGet("keyType"));
				}
				case CHEST_STATUES:
				{
					initializeStatuePuzzle(room);
					int db = trQuestVarGet("statuesIn"+room);
					float statueX = 0;
					float statueZ = 0;
					pos = kbGetBlockPosition(""+xGetInt(dChests,xUnitName));
					trQuestVarSetFromRand("rand", 4, 7, true);
					if (xGetInt(dChests, xChestKey) == 1) {
						trQuestVarSet("rand", 9);
					}
					float angle = 0.785398;
					float angleMod = 6.283185 / trQuestVarGet("rand");
					for(x=0; < trQuestVarGet("rand")) {
						dir = xsVectorSet(xsSin(angle),0,xsCos(angle)); // AoM developers weren't the brightest light bulbs
						if (xGetInt(dChests, xChestKey) == 0) { // not a temple
							statueX = xsVectorGetX(pos) - 10.0 * xsVectorGetX(dir);
							statueZ = xsVectorGetZ(pos) - 10.0 * xsVectorGetZ(dir);
						} else {
							statueX = xsVectorGetX(pos) - 15.0 * xsVectorGetX(dir);
							statueZ = xsVectorGetZ(pos) - 15.0 * xsVectorGetZ(dir);
						}
						
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trArmyDispatch("1,0","Dwarf",1,1,0,1,180,true);
						trUnitSelectClear();
						trUnitSelectByQV("next", true);
						trUnitConvert(ENEMY_PLAYER);
						trUnitTeleport(statueX,0,statueZ);
						trMutateSelected(kbGetProtoUnitID("Monument 2"));
						trSetUnitOrientation(dir,vector(0,1,0),true);
						
						xAddDatabaseBlock(dStatuesReady, true);
						xSetInt(dStatuesReady, xStatuesReadyIndex, xAddDatabaseBlock(db, true));
						xSetInt(db, xUnitName, 1*trQuestVarGet("next"));
						xSetFloat(db, xStatueAngle, angle);
						xSetVector(db, xStatuePos, xsVectorSet(statueX, 0, statueZ));
						xSetInt(db, xStatueArray, zNewArray(mInt, 1*trQuestVarGet("rand") - 1, "statue"+xGetPointer(db)));
						
						trQuestVarSetFromRand("jump", 1, 2, true);
						for (y=trQuestVarGet("jump"); >0) {
							xDatabaseNext(dStatuesReady);
						}
						
						angle = fModulo(6.283185, angle + angleMod);
					}
					
					for(x=trQuestVarGet("rand"); >0) {
						xDatabaseNext(dStatuesReady);
						int pointer = xGetPointer(dStatuesReady);
						trQuestVarSetFromRand("jump", 1, (trQuestVarGet("rand") - 1) / 2, true);
						for(y=trQuestVarGet("jump"); >0) {
							xDatabaseNext(dStatuesReady);
						}
						connectStatues(xGetInt(dStatuesReady, xStatuesReadyIndex, pointer), xGetInt(dStatuesReady, xStatuesReadyIndex), room);
						xSetPointer(dStatuesReady, pointer);
					}
					trQuestVarSet("correctStatuesIn"+room, trQuestVarGet("rand"));
					for(x=trQuestVarGet("rand"); >1) {
						xDatabaseNext(db);
						trQuestVarSetFromRand("amt",1,3,true);
						for(y=trQuestVarGet("amt"); >0) {
							turnStatue(room, 0, true, true);
						}
					}
					xClearDatabase(dStatuesReady);
				}
				case CHEST_ENCOUNTER:
				{
					vector loc = vector(0,0,0);
					xSetInt(dChests, xChestBegin, trGetNextUnitScenarioNameNumber());
					pName = trStringQuestVarGet("enemyProto"+1*trQuestVarGet("enemyProtoCount"));
					trQuestVarSetFromRand("count", 1, 6, true);
					for(j=trQuestVarGet("count"); >0) {
						trQuestVarSetFromRand("x0", 2*x0, 2*x1, true);
						trQuestVarSetFromRand("z0", 2*z0, 2*z1, true);
						trQuestVarSetFromRand("heading", 1, 360, true);
						pos = xsVectorSet(trQuestVarGet("x0"),0,trQuestVarGet("z0"));
						
						loc = vectorToGrid(pos);
						if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
							xAddDatabaseBlock(dEnemiesIncoming, true);
							xSetInt(dEnemiesIncoming, xUnitName, trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0",pName,1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
							trArmySelect("1,0");
							trUnitConvert(ENEMY_PLAYER);
						}
					}
					pos = trVectorQuestVarGet("room"+room+"bottom2");
					x0 = xsVectorGetX(pos);
					z0 = xsVectorGetZ(pos);
					pos = trVectorQuestVarGet("room"+room+"top2");
					x1 = xsVectorGetX(pos);
					z1 = xsVectorGetZ(pos);
					trQuestVarSetFromRand("count", 1, 6, true);
					for(j=trQuestVarGet("count"); >0) {
						trQuestVarSetFromRand("x0", 2*x0, 2*x1, true);
						trQuestVarSetFromRand("z0", 2*z0, 2*z1, true);
						trQuestVarSetFromRand("heading", 1, 360, true);
						pos = xsVectorSet(trQuestVarGet("x0"),0,trQuestVarGet("z0"));
						
						loc = vectorToGrid(pos);
						if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
							xAddDatabaseBlock(dEnemiesIncoming, true);
							xSetInt(dEnemiesIncoming, xUnitName, trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0",pName,1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
							trArmySelect("1,0");
							trUnitConvert(ENEMY_PLAYER);
						}
					}
					xSetInt(dChests,xChestEnd, trGetNextUnitScenarioNameNumber());
				}
			}
		}
		
		aiPlanDestroy(dStatuesReady);
		
		if (trQuestVarGet("mapType") == MAP_OPEN) {
			trUnitSelectClear();
			trUnitSelectByQV("bossEntranceSymbol", true);
			trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
			
			i = trQuestVarGet("bossKeyRoom");
			z = i / 4;
			x = i - z * 4;
			trQuestVarSet("bossKey", trGetNextUnitScenarioNameNumber());
			spawnRelicSpecific(xsVectorSet(x * 70 + 40, 0, z * 70 + 40), 1*trQuestVarGet("keyType"));
			trQuestVarSet("keyType", 1 + trQuestVarGet("keyType"));

			paintEyecandy(10, 10, 140, 140, "tree");
			paintEyecandy(10, 10, 140, 140, "sprite");
			paintEyecandy(10, 10, 140, 140, "rock");
		}
		
		if (trQuestVarGet("pestilenceStart") > 0) {
			for(x=trQuestVarGet("pestilenceStart"); < trGetNextUnitScenarioNameNumber()) {
				if (kbGetUnitBaseTypeID(kbGetBlockID(""+x)) == kbGetProtoUnitID("Pestilence SFX1")) {
					pos = kbGetBlockPosition(""+x);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Victory Marker");
				}
			}
		}
		
		if ((trQuestVarGet("nickQuestSpawn") == 1) &&
			(trQuestVarGet("stage") < 6)) {
			trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms));
			for(x=trQuestVarGet("rand"); >0) {
				xDatabaseNext(dBasicRooms);
			}
			pos = randomNearEdgeOfRoom(xGetInt(dBasicRooms, xRoomNumber));
			trQuestVarSet("nickonhawkRelicObject", trGetNextUnitScenarioNameNumber());
			spawnRelicSpecific(pos, RELIC_NICKONHAWK);
			if (trQuestVarGet("nickonhawkRelicObject") == trGetNextUnitScenarioNameNumber()) {
				// somehow he didn't spawn
				trQuestVarSet("nickonhawkRelicObject", -1);
				xFreeDatabaseBlock(dFreeRelics);
			}
		}

		if (trQuestVarGet("monsterpediaQuestLocation") == trQuestVarGet("stage")) {
			trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms));
			for(x=trQuestVarGet("rand"); >0) {
				xDatabaseNext(dBasicRooms);
			}
			z = xGetInt(dBasicRooms, xRoomNumber) / 4;
			x = iModulo(4, xGetInt(dBasicRooms, xRoomNumber));
			trQuestVarSet("devil_do1", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+ENEMY_PLAYER+",0","Shaba Ka",1,70 * x + 37,0,70 * z + 37,0,true);
			xAddDatabaseBlock(dEnemiesIncoming, true);
			xSetInt(dEnemiesIncoming, xUnitName, trQuestVarGet("devil_do1"));
			xsEnableRule("devil_do1_find");
		}

		if (trQuestVarGet("hippocampusQuestLocation") == trQuestVarGet("stage")) {
			trModifyProtounit("Hawk",ENEMY_PLAYER,1,10.2); // hawk goes zoom
			trQuestVarSet("hippocampusHawk", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+ENEMY_PLAYER+",0","Militia",1,270,0,270,0,true);
			trQuestVarSet("hippocampusUnit", mNewInt());
			spyEffect(1*trQuestVarGet("hippocampusHawk"),kbGetProtoUnitID("Hippocampus"),xsVectorSet(MALLOC, mInt * 3 + xData - 1, 1*trQuestVarGet("hippocampusUnit")));
			xsEnableRule("hippocampus_next");
		}
		
		trUnblockAllSounds();
		if (trQuestVarGet("newPlayers") > 0) {
			xsEnableRule("choose_stage_03");
			trSoundPlayFN("default","1",-1,
				"Zenophobia: Looks like we have some new faces today!", "icons\infantry g hoplite icon 64");
		} else if (trQuestVarGet("yeebBossFight") > 0) {
			xsEnableRule("yeeb_boss_message");
			trQuestVarSet("yeebStep", 0);
			trPaintTerrain(30, 5, 32, 30, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trPaintTerrain(5, 30, 30, 32, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			trChangeTerrainHeight(31, 5, 32, 32, wallHeight, false);
			trChangeTerrainHeight(5, 31, 32, 32, wallHeight, false);
		} else {
			xsEnableRule("gameplay_start");
			trUIFadeToColor(0,0,0,1000,0,false);
			trLetterBox(false);
		}
		
		/*
		If an enemy is not properly deployed, this can accidentally be assigned to a player
		*/
		for(x=xGetDatabaseCount(dEnemiesIncoming); >0) {
			xDatabaseNext(dEnemiesIncoming);
			if (kbGetBlockID(""+xGetInt(dEnemiesIncoming, xUnitName)) == -1) {
				xFreeDatabaseBlock(dEnemiesIncoming);
			}
		}
		
		/*
		paint tiny square at bottom of map for spawning units
		and then cover it up
		*/
		trPaintTerrain(0,0,5,5,0,70,true);
		trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
		
		if (trQuestVarGet("mapType") == MAP_OPEN) {
			aiPlanDestroy(dMapTiles);
			aiPlanDestroy(dEdgeFrontier);
		}
		aiPlanDestroy(dFrontier);

		if (trQuestVarGet("stage") < 10) {
			dKeeperTargets = xInitDatabase("keeperTargets");
			xKeeperTargetPlayer = xInitAddInt(dKeeperTargets, "player");
			for(p=1; < ENEMY_PLAYER) {
				if (trQuestVarGet("p"+p+"runestoneQuest") == 3) {
					trQuestVarSet("keeperQuestActive", 1);
					xAddDatabaseBlock(dKeeperTargets, true);
					xSetInt(dKeeperTargets, xKeeperTargetPlayer, p);
					xsEnableRule("the_keeper_hunt_start");
					xsEnableRule("the_keeper_hunt_sfx");
				}
			}
		}

		spysearch = trGetNextUnitScenarioNameNumber();
	}
}

rule choose_stage_03
inactive
highFrequency
{
	if (trTime() > cActivationTime + 5) {
		trSoundPlayFN("default","1",-1,
			"Zenophobia: I'll let you have the starter characters for free.", "icons\infantry g hoplite icon 64");
		xsEnableRule("choose_stage_04");
		xsDisableSelf();
	}
}

rule choose_stage_04
inactive
highFrequency
{
	if (trTime() > cActivationTime + 6) {
		trUIFadeToColor(0,0,0,1000,0,false);
		trLetterBox(false);
		xsEnableRule("gameplay_start");
		xsDisableSelf();
		trUnitSelectClear();
		trUnitSelectByQV("choice1unit", true);
		trUnitSetStance("Passive");
		trUnitHighlight(30.0, true);
		trUnitSelectClear();
		trUnitSelectByQV("choice2unit", true);
		trUnitSetStance("Passive");
		trUnitHighlight(30.0, true);
	}
}

rule zenos_paradox_ambience
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("ambienceNext")) {
		trQuestVarSet("ambienceNext", trTime() + 50);
		trSoundPlayFN("cinematics\13_out\penguins.mp3","1",-1,"","");
	}
}

rule zenos_paradox_build_01
inactive
highFrequency
{
	int db = trQuestVarGet("timeshiftHawks");
	if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
		xsEnableRule("zenos_paradox_ambience");
		for(x=0; < 20) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,db,x),true);
			trMutateSelected(kbGetProtoUnitID("Hawk"));
			trSetSelectedScale(0,0,0);
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,1*trQuestVarGet("timeshiftHawkSFX"),x), true);
			trUnitSetAnimationPath("0,1,1,0,0,0,0");
		}
		xsDisableSelf();
		
		db = xInitDatabase("zenoRelicsIncoming");
		xInitAddInt(db, "name");
		xInitAddInt(db, "type");
		trQuestVarSet("zenoRelicsIncoming", db);
		int jump = xGetDatabaseCount(dEnemiesIncoming) / 20;
		for(x=20; >0) {
			for(i=jump; >0) {
				xDatabaseNext(dEnemiesIncoming);
			}
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trUnitChangeProtoUnit("Relic");
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trImmediateUnitGarrison(""+xGetInt(dEnemiesIncoming,xUnitName));
			xAddDatabaseBlock(db, true);
			xSetInt(db, xUnitName, 1*trQuestVarGet("next"));
			xSetInt(db, xRelicType, RELIC_MATH_PROBLEM + x);
		}

		trQuestVarSet("runestone", trGetNextUnitScenarioNameNumber());
		vector pos = trVectorQuestVarGet("bossRoomCenter");
		trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
		trUnitSelectClear();
		trUnitSelectByQV("runestone");
		trUnitChangeProtoUnit("Runestone");
		trUnitSelectClear();
		trUnitSelectByQV("runestone");
		trSetSelectedScale(2.5,2.5,2.5);
		if (trQuestVarGet("runestoneComplete") == 1) {
			xsEnableRule("runestone_read");
		}

		if (trQuestVarGet("keeperQuestActive") == 0) {
			xsDisableRule("enter_boss_room");
		}
	}
}


rule the_deep_build_01
inactive
highFrequency
{
	if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
		int db = trQuestVarGet("fishHawks");
		int fishes = trQuestVarGet("fishes");
		for(x=0; < 30) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,db,x),true);
			trMutateSelected(kbGetProtoUnitID("Hawk"));
			trSetSelectedScale(0,0,0);
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,fishes,x),true);
			trQuestVarSetFromRand("rand", 1, 3, true);
			switch(1*trQuestVarGet("rand"))
			{
				case 1:
				{
					trUnitChangeProtoUnit("Fish - Salmon");
				}
				case 2:
				{
					trUnitChangeProtoUnit("Fish - Mahi");
				}
				case 3:
				{
					trUnitChangeProtoUnit("Fish - Perch");
				}
			}
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,fishes,x),true);
			trSetSelectedUpVector(0,-10,0);
			trQuestVarSetFromRand("scale", 1, 2, false);
			trSetSelectedScale(trQuestVarGet("scale"),0.1 * trQuestVarGet("scale"),trQuestVarGet("scale"));
		}
		xsEnableRule("the_deep_build_02");
		xsDisableSelf();
	}
}

rule the_deep_build_02
inactive
highFrequency
{
	int class = 0;
	string proto = "";
	vector pos = trVectorQuestVarGet("startPosition");
	if (trQuestVarGet("play") == 1) {
		/* no LOS for you */
		for(p=1; < ENEMY_PLAYER) {
			class = xGetInt(dPlayerData, xPlayerClass, p);
			proto = kbGetProtoUnitName(xGetInt(dClass, xClassProto, class));
			trModifyProtounit(proto, p, 2, -999);
			trModifyProtounit("Dog", p, 2, -999);
			trModifyProtounit("Wolf", p, 2, -999);
			trModifyProtounit("Minion", p, 2, -999);
			trModifyProtounit("Audrey", p, 2, -999);
			trModifyProtounit("Walking Berry Bush", p, 2, -999);
			
			trModifyProtounit("Hippocampus", p, 55, 1);
			zSetProtoUnitStat("Hippocampus", p, 2, 25.0);
			
			if (trQuestVarGet("p"+p+"hippocampus") == 1) {
				trQuestVarSet("p"+p+"medic", trGetNextUnitScenarioNameNumber());
				spawnPlayerUnit(p, kbGetProtoUnitID("Hippocampus"), pos);
				if (trCurrentPlayer() == p) {
					startNPCDialog(NPC_EXPLAIN_DEEP);
				}
			} else if (trCurrentPlayer() == p) {
				trStringQuestVarSet("advice", "You need a Hippocampus in order to survive on this floor. Go and find one!");
			}
		}
		for (i=1; < 40) {
			proto = kbGetProtoUnitName(monsterPetProto(i));
			for(p=1; < ENEMY_PLAYER) {
				trModifyProtounit(proto, p, 2, -999);
			}
		}
		xsEnableRule("the_deep_damage");
		xsDisableSelf();
	}
}


rule hippocampus_next
inactive
highFrequency
{
	if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
		int target = aiPlanGetUserVariableInt(MALLOC,mInt * 3 + xData - 1,1*trQuestVarGet("hippocampusUnit"));
		trQuestVarSet("hippocampusUnit", target);
		trUnitSelectClear();
		trUnitSelectByQV("hippocampusHawk");
		trMutateSelected(kbGetProtoUnitID("Hawk"));
		trSetSelectedScale(0,0,0);
		trUnitSelectClear();
		trUnitSelectByQV("hippocampusUnit");
		trUnitChangeProtoUnit("Hippocampus");
		trUnitSelectClear();
		trUnitSelectByQV("hippocampusUnit");
		trMutateSelected(kbGetProtoUnitID("Hippocampus"));
		trUnitOverrideAnimation(15,0,true,false,-1);
		activateEnemy(target);
		trQuestVarSet("hippocampusIndex", xGetNewestPointer(dEnemies));
		xSetBool(dEnemies, xLaunched, true);
		debugLog("hippocampus index is " + 1*trQuestVarGet("hippocampusIndex"));
		xsEnableRule("hippocampus_always");
		xsDisableSelf();
	}
}

rule the_clouds_build_01
inactive
highFrequency
{
	bool paint = true;
	for(x=1; < 145) {
		for(z=1; < 145) {
			paint = true;
			for(i=2;>= -1) {
				for(j=2;>= -1) {
					if (trGetTerrainHeight(x+i,z+j) > -2.0) {
						paint = false;
						break;
					}
				}
				if (paint == false) {
					break;
				}
			}
			if (paint) {
				trPaintTerrain(x,z,x,z,4,15,false);
			} else if (trGetTerrainHeight(x, z) < worldHeight - 0.1 && trGetTerrainHeight(x - 1, z - 1) > worldHeight - 0.1) {
				trPaintTerrain(x - 1, z - 1, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			}
		}
	}
	trPaintTerrain(0,0,0,146,4,15,false);
	trPaintTerrain(0,0,146,0,4,15,false);
	trPaintTerrain(0,146,146,146,4,15,false);
	trPaintTerrain(146,0,146,146,4,15,false);
	xsDisableSelf();
}

rule the_clouds_build_02
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
		
		trModifyProtounit("Hawk", ENEMY_PLAYER, 1, -2);
		trModifyProtounit("Tornado", 0, 2, -999);
		
		int db = trQuestVarGet("cloudTornados");
		int cloudSFX = trQuestVarGet("cloudTornadoSFX");
		int cloudBlock = trQuestVarGet("cloudTornadoBlock");
		
		for(i=0; < 10) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,db,i),true);
			trMutateSelected(kbGetProtoUnitID("Hawk"));
			trSetSelectedScale(0,0,0);
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,cloudSFX,i), true);
			trUnitChangeProtoUnit("Tornado");
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,cloudSFX,i), true);
			trUnitOverrideAnimation(1,0,true,true,-1);
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,cloudBlock,i), true);
			trUnitChangeProtoUnit("Invisible Target");
		}
		
		
		for(p=1; < ENEMY_PLAYER) {
			xSetFloat(dPlayerData,xPlayerFavorRegen, xGetFloat(dPlayerData,xPlayerFavorRegen) - 0.5, p);
		}
		startNPCDialog(NPC_EXPLAIN_CLOUDS);
		xsEnableRule("the_cloud_damage");
		trQuestVarSet("cloudDeployNext", trTime() + 90);
		trVectorQuestVarSet("cloudDeployDir", vector(0,0,1));
		xsDisableSelf();
	}
}

rule the_pit_build_01
inactive
highFrequency
{
	xsDisableSelf();
	bool paint = true;
	for(x=1; < 145) {
		for(z=1; < 145) {
			paint = true;
			for(i=2;>= -1) {
				for(j=2;>= -1) {
					if (trGetTerrainHeight(x+i,z+j) > -2.0) {
						paint = false;
						break;
					}
				}
				if (paint == false) {
					break;
				}
			}
			if (paint) {
				trPaintTerrain(x,z,x,z,2,12,false);
			} else if (trGetTerrainHeight(x, z) < worldHeight - 0.1 && trGetTerrainHeight(x - 1, z - 1) > worldHeight - 0.1) {
				trPaintTerrain(x - 1, z - 1, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
			}
		}
	}
	trPaintTerrain(0,0,0,146,2,12,false);
	trPaintTerrain(0,0,146,0,2,12,false);
	trPaintTerrain(0,146,146,146,2,12,false);
	trPaintTerrain(146,0,146,146,2,12,false);
	xsDisableSelf();
}

rule the_pit_build_02
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
		pitLatest = trGetNextUnitScenarioNameNumber();
		xsDisableSelf();
		startNPCDialog(NPC_EXPLAIN_PIT);
		xsEnableRule("the_pit_damage");
		xsEnableRule("the_pit_deploy");
		trQuestVarSet("pitDamageNext", trTime());
		trQuestVarSetFromRand("pitDeployNext", 30, 90, true);
		trQuestVarSet("pitDeployNext", trTime() + trQuestVarGet("pitDeployNext"));
	}
}

rule rebuild_map
inactive
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("play", 0);
	trQuestVarSet("yeebBossFight", 0);
	for(p=1; < ENEMY_PLAYER) {
		xSetInt(dPlayerData, xPlayerUnit, 0, p);
		xClearDatabase(getCharactersDB(p));
		xClearDatabase(getRelicsDB(p));
		xClearDatabase(getWarehouseDB(p));
	}
	xClearDatabase(dFreeRelics);
	xClearDatabase(dEnemies);
	xClearDatabase(dEnemiesIncoming);
	xClearDatabase(dAmbushRooms);
	xClearDatabase(dBasicRooms);
	xClearDatabase(dChests);
	xClearDatabase(dPlayerCharacters);
	xClearDatabase(dPlayerUnits);
	for(i = trQuestVarGet("eyecandyStart"); < trGetNextUnitScenarioNameNumber()) {
		trUnitSelectClear();
		trUnitSelect(""+i, true);
		trUnitDestroy();
	}
	int x = 0;
	int z = 0;
	for(i = 0; <16) {
		trQuestVarSet("tile"+i, 0);
		trQuestVarSet("room"+i, 0);
		z = i / 4;
		x = i - z * 4;
		for(j=0; <4) {
			trQuestVarSet("edge"+edgeName(i, x + trQuestVarGet("rotX"+j) + 4 * (z + trQuestVarGet("rotZ"+j))), EDGE_NOT_FOUND);
		}
	}
	xsEnableRule("choose_stage_02");
}

rule final_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		worldHeight = 13;
		TERRAIN_WALL = 0;
		TERRAIN_SUB_WALL = 50;

		trUIFadeToColor(0,0,0,1000,0,false);
		trLetterBox(false);
		xsDisableSelf();
		int x = 0;
		int z = 0;
		vector dir = vector(-1,0,-1);
		vector offset = vector(0,0,0);
		int tempX = 0;
		int tempZ = 0;
		float height = 0;

		trSetCivAndCulture(0, 1, 0);

		trSetLighting("dusk", 0);

		trPaintTerrain(0, 0, 145, 145, 0, 37, false);
		trChangeTerrainHeight(0, 0, 145, 145, -2.0, false);
		trChangeTerrainHeight(57, 57, 87, 87, 13.0, false);

		// southeast slope, made of black rock
		for(a=37; < 107) {
			for(b=37; < 57) {
				if (b <= a && b <= 145 - a && 80 < a + b) {
					x = a;
					z = b;
					dir = vector(0,0,0);
					offset = vector(0,0,-1);
					height = 15.0 * (b - 37) / 20.0 - 2.0;
					for(i=4; >0) {
						trPaintTerrain(x + xsVectorGetX(dir), z + xsVectorGetZ(dir), x + xsVectorGetX(dir), z + xsVectorGetZ(dir), 2, 13, false);
						trChangeTerrainHeight(x, z, x, z, height);
						tempX = x - 72;
						tempZ = z - 72;
						x = tempZ + 72;
						z = 72 - tempX;
						dir = dir + offset;
						offset = rotationMatrix(offset, 0, -1.0);
					}
				}
			}
		}

		// south pathway
		/*
		12.4266 is the length of one side of the octagon
		72 - 15 = 57
		72 - 6.213 = 65.787
		y = -x + b
		57 = -65.787 + b
		65.787 = 57 + b
		b = 122.787
		y = x + 8.787
		y = x - 8.787
		37 = x - 8.787
		x = 45.787
		87 = b - 78
		b = 
		*/
		for(a=37; < 72) {
			for(b=37; < 72) {
				if ((b > a - 8.787) && (b < 122.787 - a) && (b < a + 8.787)) {
					x = a;
					z = b;
					dir = vector(0,0,0);
					offset = vector(0,0,-1);
					height = 15.0 * (1.0 * a + b - 82.787) / 40 - 2.0;
					for(i=4; >0) {
						if (height > -2.0) {
							trPaintTerrain(x + xsVectorGetX(dir), z + xsVectorGetZ(dir), x + xsVectorGetX(dir), z + xsVectorGetZ(dir), 5, 4, false);
							trChangeTerrainHeight(x, z, x, z, height);
						} else {
							trPaintTerrain(x + xsVectorGetX(dir), z + xsVectorGetZ(dir), x + xsVectorGetX(dir), z + xsVectorGetZ(dir), 0, 37, false);
							trChangeTerrainHeight(x, z, x, z, -2.0);
						}
						tempX = x - 72;
						tempZ = z - 72;
						x = tempZ + 72;
						z = 72 - tempX;
						dir = dir + offset;
						offset = rotationMatrix(offset, 0, -1.0);
					}
				}
			}
		}

		/*
		Tall Columns
		57
		65.787

		59, 67 = 118, 134
		145 - 118 = 27
		145 - 134 = 11

		*/
		x = 135;
		offset = vector(27, 0, 11);
		dir = vector(11,0,27);
		for(i=4; >0) {
			trArmyDispatch("0,0","Dwarf",1,144 + xsVectorGetX(offset),0,144 + xsVectorGetZ(offset), x, true);
			trArmyDispatch("0,0","Dwarf",1,144 + xsVectorGetX(dir),0,144 + xsVectorGetZ(dir), x, false);
			trArmySelect("0,0");
			trMutateSelected(kbGetProtoUnitID("Columns"));
			trSetSelectedScale(3,3,3);
			offset = rotationMatrix(offset, 0, 1);
			dir = rotationMatrix(dir, 0, 1);
			x = iModulo(360, x - 90);
		}

		/*
		for(x=78; < 87) {
			z = 165 - x;
			trPaintTerrain(x, z, x, z, 5, 4, false);
		}
		*/

		// update passability
		trPaintTerrain(0,0,0,0, 0, 50, true);

		// re-paint the black outside with something else
		for(x=0; < 145) {
			for(z=0; < 145) {
				if (trGetTerrainHeight(x, z) > worldHeight - 0.1 && trGetTerrainHeight(x + 1, z + 1) > worldHeight - 0.1) {
					trPaintTerrain(x, z, x, z, 0, 70, false);
				} else if (trGetTerrainType(x, z) == 2) {
					trPaintTerrain(x, z, x, z, 0, 50, false);
				} else if (trGetTerrainType(x, z) == 5) {
					trPaintTerrain(x, z, x, z, 0, 53, false);
				}
			}
		}

		//trPaintTerrain(0, 0, 145, 145, 0, 37, false);


		/* eyecandy - candy for the eyes */
		for(x=0; < 145) {
			trQuestVarSetFromRand("rand", 0, 145, true);
			if (trGetTerrainSubType(x, trQuestVarGet("rand")) == 37) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trArmyDispatch("0,0","Ruins",1,x * 2 + 1, 0, trQuestVarGet("rand") * 2 + 1, trQuestVarGet("heading"), true);
			}
			trQuestVarSetFromRand("rand", 0, 145, true);
			if (trGetTerrainSubType(x, trQuestVarGet("rand")) == 37) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trArmyDispatch("0,0","Columns Broken",1,x * 2 + 1, 0, trQuestVarGet("rand") * 2 + 1, trQuestVarGet("heading"), true);
			}
		}

		for(z=0; < 145) {
			trQuestVarSetFromRand("rand", 0, 145, true);
			if (trGetTerrainSubType(trQuestVarGet("rand"), z) == 37) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trArmyDispatch("0,0","Columns Fallen",1, trQuestVarGet("rand") * 2 + 1, 0, z * 2 + 1, trQuestVarGet("heading"), true);
			}
			trQuestVarSetFromRand("rand", 0, 145, true);
			if (trGetTerrainSubType(trQuestVarGet("rand"), z) == 37) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trArmyDispatch("0,0","Columns",1, trQuestVarGet("rand") * 2 + 1, 0, z * 2 + 1, trQuestVarGet("heading"), true);
			}
		}

		trQuestVarSet("bossRevealer", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("0,0", "Dwarf", 1, 145, 0, 145, 0, true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit("Cinematic Block");
		zSetProtoUnitStat("Revealer", 0, 2, 50);

		bossUnit = trGetNextUnitScenarioNameNumber();
		trArmyDispatch("0,0", "Dwarf", 1, 145, 0, 145, 225, true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit("Guardian Sleeping");

		xsEnableRule("guardian_find");
		xsEnableRule("gameplay_start");
		trVectorQuestVarSet("startPosition", vector(15,0,15));

		trQuestVarSet("reviveCount", ENEMY_PLAYER - 1);
		startNPCDialog(NPC_EXPLAIN_SPACE);
	}
}