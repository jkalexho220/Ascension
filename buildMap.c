

rule choose_stage_00
inactive
highFrequency
{
    int n = yDatabaseNext("stageChoices");
    if (trCountUnitsInArea(""+n, 1, "Athena",3) == 1) {
        trQuestVarSet("stage", yGetPointer("stageChoices") + 1);
        trUnitSelectClear();
        trUnitSelectByQV("chooser", true);
        trUnitChangeProtoUnit("Rocket");
        for(x=yGetDatabaseCount("stageChoices"); >0) {
            yDatabaseNext("stageChoices", true);
            trUnitDestroy();
        }
        yClearDatabase("stageChoices");
        xsDisableSelf();
        trForceNonCinematicModels(true);
        trLetterBox(true);
        trUIFadeToColor(0,0,0,1000,0,true);
        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
        trOverlayText("Stage " + 1*trQuestVarGet("stage") + " chosen!", 3.0, -1, -1, -1);
    }
}

int edgeName(int first = 0, int second = 0) {
    return(16 * xsMin(first, second) + xsMax(first, second));
}

/*
input is in grid coordinates
return value is in vector coordinates
*/
vector randomNearEdge(int x1 = 0 , int z1 = 0, int x2 = 0, int z2 = 0) {
    /* calculate offset from center. More likely to be farther from center */
    int x0 = x2 - x1;
    int z0 = z2 - z1;
    trQuestVarSetFromRand("x0", 0, x0);
    trQuestVarSetFromRand("z0", 0, z0);
    trQuestVarSetFromRand("x1", trQuestVarGet("x0"), x0);
    trQuestVarSetFromRand("z1", trQuestVarGet("z0"), z0);
    /* randomly choose 1 or -1 */
    trQuestVarSetFromRand("negative", 0, 1, true);
    trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
    x0 = x1 + x2 + trQuestVarGet("negative") * trQuestVarGet("x1");
    trQuestVarSetFromRand("negative", 0, 1, true);
    trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
    z0 = z1 + z2 + trQuestVarGet("negative") * trQuestVarGet("z1");
    return(xsVectorSet(x0, 0, z0));
}

/*
input values are grid positions
*/
void paintRelicEdge(int x1 = 0 , int z1 = 0, int x2 = 0, int z2 = 0) {
    vector v0 = randomNearEdge(x1, z1, x2, z2);
    spawnRelic(xsVectorGetX(v0), xsVectorGetZ(v0));
}

void paintEnemies(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
    for(a=x0; < x1) {
        trQuestVarSetFromRand("deploy", 0, 1, false);
        if (trQuestVarGet("deploy") < trQuestVarGet("enemyDensity")) {
            trQuestVarSetFromRand("z", z0, z1, true);
            trQuestVarSet("posX", a);
            trQuestVarSet("posZ", trQuestVarGet("z"));
            if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet("enemyProto"+randomLow(1*trQuestVarGet("enemyProtoCount"))),1,
                    2*a,0,2*trQuestVarGet("z"),trQuestVarGet("heading"),true);
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trUnitConvert(ENEMY_PLAYER);
                yAddToDatabase("enemiesIncoming", "next");
            }
        }
    }
    for(b=z0; < z1) {
        trQuestVarSetFromRand("deploy", 0, 1, false);
        if (trQuestVarGet("deploy") < trQuestVarGet("enemyDensity")) {
            trQuestVarSetFromRand("x", x0, x1, true);
            trQuestVarSet("posX", trQuestVarGet("x"));
            trQuestVarSet("posZ", b);
            if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet("enemyProto"+randomLow(1*trQuestVarGet("enemyProtoCount"))),1,
                    2*trQuestVarGet("x"),0,2*b,trQuestVarGet("heading"),true);
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trUnitConvert(ENEMY_PLAYER);
                yAddToDatabase("enemiesIncoming", "next");
            }
        }
    }
}

void paintEyecandy(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0, string type = "sprite") {
    if (trQuestVarGet(type+"Density") > 0) {
        for(a=x0; < x1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
                trQuestVarSetFromRand("z", z0, z1, true);
                trQuestVarSetFromRand("type", 1, 3, true);
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
                    2*a,0,2*trQuestVarGet("z"),trQuestVarGet("heading"),true);
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trUnitConvert(0);
            }
        }
        for(b=z0; < z1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
                trQuestVarSetFromRand("x", x0, x1, true);
                trQuestVarSetFromRand("type", 1, 3, true);
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
                    2*trQuestVarGet("x"),0,2*b,trQuestVarGet("heading"),true);
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trUnitConvert(0);
            }
        }
    }
}

void paintColumns(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
    if (trQuestVarGet("columnDensity") > 0) {
        for(a=x0; < x1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet("columnDensity")) {
                trQuestVarSetFromRand("z", z0, z1, true);
                trPaintTerrain(a-1, 1*trQuestVarGet("z")-1, a+1, 1*trQuestVarGet("z")+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                trChangeTerrainHeight(a, 1*trQuestVarGet("z"), a+1, 1*trQuestVarGet("z")+1, 6, false);
            }
        }
        for(b=z0; < z1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet("columnDensity")) {
                trQuestVarSetFromRand("x", x0, x1, true);
                trPaintTerrain(1*trQuestVarGet("x")-1, b-1, 1*trQuestVarGet("x")+1, b+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                trChangeTerrainHeight(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x")+1, b+1, 6, false);
            }
        }
    }
}

int countRoomEntrances(int x = 0, int z = 0) {
    int room = x + 4 * z;
    int n = 0;
    int count = 0;
    for(a=1; >=0) {
        for(b=1; >=0) {
            trQuestVarSet("newX", (1 - 2 * b) * a + x);
            trQuestVarSet("newZ", (1 - 2 * b) * (1 - a) + z);
            if (trQuestVarGet("newX") < 0 || trQuestVarGet("newZ") < 0 ||
                trQuestVarGet("newX") > 3 || trQuestVarGet("newZ") > 3) {
                continue;
            }
            n = 0 + trQuestVarGet("newX") + 4 * trQuestVarGet("newZ");
            if (trQuestVarGet("edge"+edgeName(room, n)) == EDGE_NOT_FOUND) {
                continue;
            } else {
                count = count + 1;
            }
        }
    }
    return(count);
}

void paintSecondary(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
    for(a=x0; < x1) {
        trQuestVarSetFromRand("z", z0, z1, true);
        trPaintTerrain(a, 1*trQuestVarGet("z"), a, 1*trQuestVarGet("z"), TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
    }
    for(b=z0; < z1) {
        trQuestVarSetFromRand("x", x0, x1, true);
        trPaintTerrain(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x"), b, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
    }
}

void buildRoom(int x = 0, int z = 0, int type = 0) {
    int room = x + 4 * z;
    int x0 = 0;
    int z0 = 0;
    int x1 = 0;
    int z1 = 0;
    trQuestVarSet("room"+room, type);
    if (type < ROOM_STARTER) {
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
            trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);
            trVectorQuestVarSet("room"+room+"top"+i, xsVectorSet(x1,0,z1));
            trVectorQuestVarSet("room"+room+"bottom"+i, xsVectorSet(x0,0,z0));
            paintSecondary(x0, z0, x1, z1);
            paintEyecandy(x0, z0, x1, z1, "tree");
            paintEyecandy(x0, z0, x1, z1, "rock");
            paintEyecandy(x0, z0, x1, z1, "sprite");

            /* relic spawn */
            for(j=randomLow(10) - 7; >0) {
                paintRelicEdge(x0, z0, x1, z1);
            }
        }
        paintColumns(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35);
    }
    switch(type)
    {
        case ROOM_BASIC:
        {
            for(i = 2; >0) {
                x0 = trQuestVarGet("room"+room+"bottom"+i+"x");
                z0 = trQuestVarGet("room"+room+"bottom"+i+"z");
                x1 = trQuestVarGet("room"+room+"top"+i+"x");
                z1 = trQuestVarGet("room"+room+"top"+i+"z");
                paintEnemies(x0, z0, x1, z1);
            }
            trQuestVarSet("room", room);
            yAddToDatabase("basicRooms", "room");
        }
        case ROOM_AMBUSH:
        {
            trQuestVarSet("room", room);
            yAddToDatabase("ambushRooms", "room");
            yAddUpdateVar("ambushRooms", "posX", x * 70 + 40);
            yAddUpdateVar("ambushRooms", "posZ", z * 70 + 40);
            trQuestVarSetFromRand("type", 1, trQuestVarGet("enemyProtoCount"), true);
            yAddUpdateVar("ambushRooms", "type", trQuestVarGet("type"));
            trQuestVarSetFromRand("spawnRelic", 0, 1, false);
            if (trQuestVarGet("spawnRelic") < 0.5) {
                spawnRelic(x * 70 + 40, z * 70 + 40);
            }
            x0 = trQuestVarGet("room"+room+"bottom1x");
            z0 = trQuestVarGet("room"+room+"bottom1z");
            x1 = trQuestVarGet("room"+room+"top1x");
            z1 = trQuestVarGet("room"+room+"top1z");
            paintEnemies(x0, z0, x1, z1);
        }
        case ROOM_CHEST:
        {
            /* can't perform pad puzzle with only one player */
            if (ENEMY_PLAYER == 2) {
                trQuestVarSetFromRand("chestType", CHEST_KEY, CHEST_ENCOUNTER, true);
            } else {
                trQuestVarSetFromRand("chestType", CHEST_KEY, CHEST_PADS, true);
            }
            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
            yAddToDatabase("chests", "next");
            yAddUpdateVar("chests", "type", trQuestVarGet("chestType"));
            yAddUpdateVar("chests", "room", room);
            trArmyDispatch("1,0","Dwarf",1,x * 70 + 40,0, z * 70 + 40, 315, true);
            trArmySelect("1,0");
            trUnitConvert(0);
            trUnitChangeProtoUnit("Great Box");
            trChatSend(0, "chest spawned!");
        }
        case ROOM_TRANSPORTER_GUY:
        {
            x0 = trQuestVarGet("room"+room+"bottom1x");
            z0 = trQuestVarGet("room"+room+"bottom1z");
            x1 = trQuestVarGet("room"+room+"top1x");
            z1 = trQuestVarGet("room"+room+"top1z");
            trVectorQuestVarSet("center", xsVectorSet(x0 + x1, 0, z0 + z1));
            trQuestVarSetFromRand("x0", 0, 1, false);
            trQuestVarSetFromRand("z0", trQuestVarGet("x0"), 1, false);
            /* set negative to be 1 or -1 */
            trQuestVarSetFromRand("negative", 0, 1, true);
            trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
            x0 = x0 + x1 + (x1 - x0) * trQuestVarGet("x0") * trQuestVarGet("negative");
            trQuestVarSetFromRand("negative", 0, 1, true);
            trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
            z0 = z0 + z1 + (z1 - z0) * trQuestVarGet("z0") * trQuestVarGet("negative");
            trVectorQuestVarSet("relictransporterguypos", xsVectorSet(x0, 0, z0));
            trQuestVarSet("heading", 180.0 / 3.141592 * angleBetweenVectors("relictransporterguypos", "center"));
            trQuestVarSet("relicTransporterGuyName", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0","Villager Atlantean Hero",1,x0,0,z0,trQuestVarGet("heading"), true);
            trUnitSelectClear();
            trUnitSelectByQV("relicTransporterGuyName", true);
            trUnitConvert(0);
            trArmyDispatch("1,0", "Dwarf",1,x0,0,z0,0,true);
            trArmySelect("1,0");
            trUnitChangeProtoUnit("Gaia Forest effect");
            xsEnableRule("relic_transporter_guy_found");
            buildRoom(x, z, ROOM_BASIC);
        }
        case ROOM_STARTER:
        {
            trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, 0, false);
            paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
            trVectorQuestVarSet("startPosition", xsVectorSet(x*70 + 40,0,z*70+40));
            if (trQuestVarGet("stage") < 5) {
                trVectorQuestVarSet("choice1", xsVectorSet(x*70+48,0,z*70+54));
                trVectorQuestVarSet("choice2", xsVectorSet(x*70+54,0,z*70+48));
                string choice1 = kbGetProtoUnitName(1*trQuestVarGet("class"+(2*trQuestVarGet("stage")-1)+"proto"));
                string choice2 = kbGetProtoUnitName(1*trQuestVarGet("class"+(2*trQuestVarGet("stage"))+"proto"));
                trQuestVarSet("choice1unit", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", choice1, 1, trQuestVarGet("choice1x"),0,trQuestVarGet("choice1z"),225,true);
                trQuestVarSet("choice2unit", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", choice2, 1, trQuestVarGet("choice2x"),0,trQuestVarGet("choice2z"),225,false);
                trArmySelect("1,0");
                trUnitConvert(0);
                yAddToDatabase("stunnedUnits", "choice1unit");
                yAddUpdateVar("stunnedUnits", "proto", trQuestVarGet("class"+(2*trQuestVarGet("stage")-1)+"proto"));
                yAddToDatabase("stunnedUnits", "choice2unit");
                yAddUpdateVar("stunnedUnits", "proto", trQuestVarGet("class"+(2*trQuestVarGet("stage"))+"proto"));
                xsEnableRule("class_shop_always");
            }
        }
        case ROOM_BOSS:
        {
            trPaintTerrain(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 5, z * 35 + 5, x * 35 + 36, z * 35 + 36, 0, false);
            trVectorQuestVarSet("bossRoomEntrance", xsVectorSet(x * 70 + 10, 0, z * 70 + 10));
            xsEnableRule("enter_boss_room");
        }
        case ROOM_NICK:
        {
            trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, 0, 73, false);
            trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, 0, false);
            trQuestVarSet("nickonhawk", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+40, 0, 70*z+40,225,true);
            trUnitSelectClear();
            trUnitSelectByQV("nickonhawk", true);
            trUnitConvert(0);
            trUnitChangeProtoUnit("Odysseus");
            trQuestVarSet("nickShop1", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+30,0, 70*z+30,0,true);
            trQuestVarSet("nickShop2", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+30,0, 70*z+50,0,true);
            trQuestVarSet("nickShop3", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+50,0, 70*z+30,0,true);
            trQuestVarSet("nickShop4", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+50,0, 70*z+50,0,true);
            for(i=4; >0) {
                trUnitSelectClear();
                trUnitSelectByQV("nickShop"+i, true);
                trUnitConvert(0);
                trUnitChangeProtoUnit("Outpost");
            }
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
    if (trQuestVarGet("edge"+edge) == EDGE_NOT_FOUND) {
        int first = edge / 16;
        int second = edge - 16 * first;
        int z0 = first / 4;
        int x0 = first - 4 * z0;
        int z1 = second / 4;
        int x1 = second - 4 * z1;
        z0 = z0 * 35 + 17;
        x0 = x0 * 35 + 17;
        z1 = z1 * 35 + 23;
        x1 = x1 * 35 + 23;
        if (type == EDGE_BIG) {
            trQuestVarSetFromRand("rand", 0, 7, true);
            z0 = z0 - trQuestVarGet("rand");
            x0 = x0 - trQuestVarGet("rand");
            trQuestVarSetFromRand("rand", 0, 7, true);
            z1 = z1 + trQuestVarGet("rand");
            x1 = x1 + trQuestVarGet("rand");
        }
        trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
        trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);
        paintSecondary(x0, z0, x1, z1);
        trQuestVarSet("edge"+edge, type);
    }
}

rule choose_stage_01
inactive
highFrequency
{
    if (trQuestVarGet("stage") > 0) {
        xsEnableRule("choose_stage_02");
        xsDisableSelf();
    }
}


rule choose_stage_02
inactive
highFrequency
{
    if (trTime() > cActivationTime + 1) {
        xsDisableSelf();
        trBlockAllSounds();
        for(i=trQuestVarGet("stage"); >1) {
            trTechSetStatus(ENEMY_PLAYER, 76, 4);
            for(j=10; >0) {
                trTechSetStatus(ENEMY_PLAYER, 78, 4);
            }
        }
        /* minecraft time! */
        switch(1*trQuestVarGet("stage"))
        {
            case 1:
            {
                /* desert tomb */
                trOverlayText("Temple of the Lion", 3.0, -1, -1, -1);
                TERRAIN_WALL = 2;
                TERRAIN_SUB_WALL = 0;
                
                TERRAIN_PRIMARY = 0;
                TERRAIN_SUB_PRIMARY = 34;
                

                /*
                TERRAIN_PRIMARY = 0;
                TERRAIN_SUB_PRIMARY = 84;
                */

                TERRAIN_SECONDARY = 4;
                TERRAIN_SUB_SECONDARY = 12;

                trQuestVarSet("treeDensity", 0.03);
                trStringQuestVarSet("treeProto1", "Statue Lion Left");
                trStringQuestVarSet("treeProto2", "Statue Lion Right");
                trStringQuestVarSet("treeProto3", "Statue Nemean Lion");
                trQuestVarSet("spriteDensity", 0.3);
                trStringQuestVarSet("spriteProto1", "Rock Sandstone Sprite");
                trStringQuestVarSet("spriteProto2", "Grass");
                trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
                trQuestVarSet("rockDensity", 0.13);
                trStringQuestVarSet("rockProto1", "Rock Sandstone Big");
                trStringQuestVarSet("rockProto2", "Rock River Sandy");
                trStringQuestVarSet("rockProto3", "Rock Sandstone Small");

                trQuestVarSet("enemyDensity", 0.02 + 0.02 * ENEMY_PLAYER);
                trQuestVarSet("enemyProtoCount", 5);
                trStringQuestVarSet("enemyProto1", "Golden Lion");
                trStringQuestVarSet("enemyProto2", "Anubite");
                trStringQuestVarSet("enemyProto3", "Terracotta Soldier");
                trStringQuestVarSet("enemyProto4", "Sphinx");
                trStringQuestVarSet("enemyProto5", "Petsuchos");

                trQuestVarSet("columnDensity", 0.05);
            }
        }

        /* paint entire map cliff and raise it */
        trChangeTerrainHeight(0,0,150,150,6,false);
        trPaintTerrain(0,0,150,150,TERRAIN_WALL, TERRAIN_SUB_WALL,false);

        /* build the grid */
        int x = 0;
        int z = 0;
        int n = 0;
        int total = 0;
        int backtrack = 5;
        trQuestVarSet("tile0", TILE_VISITED);
        trQuestVarSet("tile1", TILE_FOUND);
        trQuestVarSet("tile4", TILE_FOUND);
        trQuestVarSet("next", 1);
        yAddToDatabase("frontier", "next");
        yAddUpdateVar("frontier", "edge", edgeName(0, 1));
        trQuestVarSet("next", 4);
        yAddToDatabase("frontier", "next");
        yAddUpdateVar("frontier", "edge", edgeName(0, 4));
        
        /* build guaranteed path to every room */
        for(i=0; < 64) {
            trQuestVarSetFromRand("search", 1, backtrack, true);
            yDatabasePointerDefault("frontier");
            for(j=trQuestVarGet("search"); >0) {
                yDatabaseNext("frontier");
            }
            if (trQuestVarGet("tile"+1*trQuestVarGet("frontier")) < TILE_VISITED) {
                z = 1*trQuestVarGet("frontier") / 4;
                x = 1*trQuestVarGet("frontier") - 4 * z;
                buildEdge(1*yGetVar("frontier", "edge"), EDGE_NORMAL);
                trQuestVarSet("tile"+1*trQuestVarGet("frontier"), TILE_VISITED);
                if (trQuestVarGet("frontier") < 15) {
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
                                trQuestVarSet("next", n);
                                yAddToDatabase("frontier", "next");
                                yAddUpdateVar("frontier", "edge", edgeName(1*trQuestVarGet("frontier"), n));
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
            yRemoveFromDatabase("frontier");
            yRemoveUpdateVar("frontier", "edge");
        }
        /* random bonus paths */
        for(i=0; <10) {
            trQuestVarSetFromRand("first", 1, 14);
            trQuestVarSetFromRand("direction", 0, 3);
            z = 1*trQuestVarGet("first") / 4;
            x = 1*trQuestVarGet("first") - z * 4;
            a = 1*trQuestVarGet("direction") / 2;
            b = 1*trQuestVarGet("direction") - a * 2;
            trQuestVarSet("newX", (1 - 2 * b) * a + x);
            trQuestVarSet("newZ", (1 - 2 * b) * (1 - a) + z);
            if (trQuestVarGet("newX") < 0 || trQuestVarGet("newZ") < 0 ||
                trQuestVarGet("newX") > 3 || trQuestVarGet("newZ") > 3 ||
                (trQuestVarGet("newX") + trQuestVarGet("newZ") == 6)) {
                continue;
            } else {
                n = 0 + trQuestVarGet("newX") + 4 * trQuestVarGet("newZ");
                buildEdge(edgeName(1*trQuestVarGet("first"), n), EDGE_BIG);
            }
        }

        buildRoom(0,0, ROOM_STARTER);
        buildRoom(3,3, ROOM_BOSS);

        bool nick = false;
        trQuestVarSetFromRand("nick", 0, 20, true);
        if (trQuestVarGet("nick") < trQuestVarGet("stage")) {
            nick = true;
        }

        int chests = 3;

        trQuestVarSetFromRand("relicTransporterGuy", 1, 14, true);

        for (i=1; < 15) {
            z = i / 4;
            x = i - z * 4;
            if (chests > 0) {
                trQuestVarSetFromRand("chestRand", 1, 12 - trQuestVarGet("stage"), true);
                if (trQuestVarGet("chestRand") == 1) {
                    chests = chests - 1;
                }
            } else {
                trQuestVarSet("chestRand", 0);
            }
            if (i == trQuestVarGet("relicTransporterGuy")) {
                buildRoom(x, z, ROOM_TRANSPORTER_GUY);
            } else if (nick && (countRoomEntrances(x, z) == 1)) {
                buildRoom(x, z, ROOM_NICK);
                nick = false;
                xsEnableRule("nick_00_visit");
            } else if (trQuestVarGet("chestRand") == 1) {
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
    
        /* finish making chest rooms */
        int room = 0;
        int x0 = 0;
        int z0 = 0;
        int x1 = 0;
        int z1 = 0;
        trQuestVarSet("keyType", RELIC_KEY_GREEK);
        string pName = "";
        for(i=yGetDatabaseCount("chests"); >0) {
            yDatabaseNext("chests");
            room = yGetVar("chests", "room");
            x0 = trQuestVarGet("room"+room+"bottom1x");
            z0 = trQuestVarGet("room"+room+"bottom1z");
            x1 = trQuestVarGet("room"+room+"top1x");
            z1 = trQuestVarGet("room"+room+"top1z");
            switch(1*yGetVar("chests", "type"))
            {
                case CHEST_KEY:
                {
                    ySetVar("chests", "keyType", trQuestVarGet("keyType"));
                    paintEnemies(x0, z0, x1, z1);
                    trQuestVarSetFromRand("key", 1, 14, true);
                    for(j=trQuestVarGet("key"); >0) {
                        room = yDatabaseNext("basicRooms");
                    }
                    x0 = trQuestVarGet("room"+room+"bottom1x");
                    z0 = trQuestVarGet("room"+room+"bottom1z");
                    x1 = trQuestVarGet("room"+room+"top1x");
                    z1 = trQuestVarGet("room"+room+"top1z");
                    trVectorQuestVarSet("pos", randomNearEdge(x0, z0, x1, z1));
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0","Militia",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitChangeProtoUnit("Relic");
                    yAddToDatabase("freeRelics", "next");
                    yAddUpdateVar("freeRelics", "type", trQuestVarGet("keyType"));

                    trVectorSetUnitPos("pos", "chests");
                    trQuestVarSet("posX", trQuestVarGet("posX") - 2);
                    trQuestVarSet("posZ", trQuestVarGet("posZ") - 2);
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),45,true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitConvert(0);
                    trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));

                    trQuestVarSet("keyType", 1 + trQuestVarGet("keyType"));
                }
                case CHEST_PADS:
                {
                    paintEnemies(x0, z0, x1, z1);
                    trVectorQuestVarSet("pos", randomNearEdge(x0, z0, x1, z1));
                    ySetVar("chests", "pad1", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0","Militia",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);

                    x0 = trQuestVarGet("room"+room+"bottom2x");
                    z0 = trQuestVarGet("room"+room+"bottom2z");
                    x1 = trQuestVarGet("room"+room+"top2x");
                    z1 = trQuestVarGet("room"+room+"top2z");

                    paintEnemies(x0, z0, x1, z1);
                    trVectorQuestVarSet("pos", randomNearEdge(x0, z0, x1, z1));
                    ySetVar("chests", "pad2", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0","Militia",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,false);
                    
                    trArmySelect("1,0");
                    trUnitConvert(0);
                    trUnitChangeProtoUnit("Statue of Automaton Base");
                }
                case CHEST_ENCOUNTER:
                {
                    ySetVar("chests", "enemiesBegin", trGetNextUnitScenarioNameNumber());
                    pName = trStringQuestVarGet("enemyProto"+1*trQuestVarGet("enemyProtoCount"));
                    trQuestVarSetFromRand("count", 1, 6, true);
                    for(j=trQuestVarGet("count"); >0) {
                        trQuestVarSetFromRand("x0", 2*x0, 2*x1, true);
                        trQuestVarSetFromRand("z0", 2*z0, 2*z1, true);
                        trQuestVarSetFromRand("heading", 1, 360, true);
                        trQuestVarSet("posX", trQuestVarGet("x0"));
                        trQuestVarSet("posZ", trQuestVarGet("z0"));
                        vectorToGrid("pos", "pos");
                        if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                            trArmyDispatch("1,0",pName,1,trQuestVarGet("x0"),0,trQuestVarGet("z0"),trQuestVarGet("heading"),true);
                            trArmySelect("1,0");
                            trUnitConvert(ENEMY_PLAYER);
                        }
                    }
                    x0 = trQuestVarGet("room"+room+"bottom2x");
                    z0 = trQuestVarGet("room"+room+"bottom2z");
                    x1 = trQuestVarGet("room"+room+"top2x");
                    z1 = trQuestVarGet("room"+room+"top2z");
                    trQuestVarSetFromRand("count", 1, 6, true);
                    for(j=trQuestVarGet("count"); >0) {
                        trQuestVarSetFromRand("x0", 2*x0, 2*x1, true);
                        trQuestVarSetFromRand("z0", 2*z0, 2*z1, true);
                        trQuestVarSetFromRand("heading", 1, 360, true);
                        trQuestVarSet("posX", trQuestVarGet("x0"));
                        trQuestVarSet("posZ", trQuestVarGet("z0"));
                        vectorToGrid("pos", "pos");
                        if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                            trArmyDispatch("1,0",pName,1,trQuestVarGet("x0"),0,trQuestVarGet("z0"),trQuestVarGet("heading"),true);
                            trArmySelect("1,0");
                            trUnitConvert(ENEMY_PLAYER);
                        }
                    }
                    ySetVar("chests", "enemiesEnd", trGetNextUnitScenarioNameNumber());
                }
            }
        }        

        /* 
        paint tiny square at bottom of map for spawning units 
        and then cover it up
        */
        trPaintTerrain(0,0,5,5,0,70,true);
        trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

        trUnblockAllSounds();
        if (trQuestVarGet("newPlayers") == 0) {
            xsEnableRule("gameplay_start");
            trUIFadeToColor(0,0,0,1000,0,false);
            trLetterBox(false);
        } else {
            xsEnableRule("choose_stage_03");
            trSoundPlayFN("default","1",-1,
                "Zenophobia: Looks like we have some new faces today!", "icons\infantry g hoplite icon 64");
        }
    }
}

rule choose_stage_03
inactive
highFrequency
{
    if (trTime() > cActivationTime + 5) {
        trSoundPlayFN("default","1",-1,
            "Zenophobia: I'll let you choose one of the starter characters for free.", "icons\infantry g hoplite icon 64");
        trUnitSelectClear();
        trUnitSelectByQV("choice1", true);
        trUnitSetStance("Passive");
        trUnitHighlight(15.0, true);
        trUnitSelectClear();
        trUnitSelectByQV("choice2", true);
        trUnitSetStance("Passive");
        trUnitHighlight(15.0, true);
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
    }
}