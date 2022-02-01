const int MAP_STANDARD = 0;
const int MAP_PORTALS = 1;
const int MAP_OPEN = 2;

const int TRAP_LASERS = 1;
const int TRAP_CAROUSEL = 2;

void deployTownEyecandy(string proto = "", int x = 0, int z = 0, int heading = 0) {
    int n = trGetNextUnitScenarioNameNumber();
    trArmyDispatch("1,0","Dwarf",1,x+trQuestVarGet("villageX"),0,z+trQuestVarGet("villageZ"),heading,true);
    trUnitSelectClear();
    trUnitSelect(""+n, true);
    trUnitConvert(0);
    trUnitChangeProtoUnit(proto);
}

rule choose_stage_00
inactive
highFrequency
{
    int n = yDatabaseNext("stageChoices");
    if (trCountUnitsInArea(""+n, 1, "Athena",3) == 1) {
        trQuestVarSet("stage", yGetVar("stageChoices", "stage"));
        trUnitSelectClear();
        trUnitSelectByQV("chooser", true);
        trUnitChangeProtoUnit("Rocket");
        for(x=yGetDatabaseCount("stageChoices"); >0) {
            yDatabaseNext("stageChoices", true);
            trUnitDestroy();
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("stageChoices", "obelisk"), true);
            trUnitDestroy();
        }
        yClearDatabase("stageChoices");
        xsDisableSelf();
        trForceNonCinematicModels(true);
        trLetterBox(true);
        trUIFadeToColor(0,0,0,1000,0,true);
        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
        trOverlayText(stageName(1*trQuestVarGet("stage")), 3.0, -1, -1, -1);
    } else {
        trUnitSelectClear();
        trUnitSelect(""+1*yGetVar("stageChoices", "obelisk"), true);
        if (trUnitIsSelected()) {
            uiClearSelection();
            trShowImageDialog(stageIcon(1*yGetVar("stageChoices", "stage")), 
                "Stage " + 1*yGetVar("stageChoices", "stage") + ": " + stageName(1*yGetVar("stageChoices", "stage")));
        }
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
    trVectorQuestVarSet("pos",randomNearEdge(x1, z1, x2, z2));
    int count = (x2-x1) / 2;
    for(i = count; >0) {
        vectorToGrid("pos", "loc");
        if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
            trVectorQuestVarSet("pos",randomNearEdge(x1, z1, x2, z2));
        } else {
            break;
        }
    }
    spawnRelic(trQuestVarGet("posx"), trQuestVarGet("posz"));
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
    float scale = trQuestVarGet(type+"scale");
    if (trQuestVarGet(type+"Density") > 0) {
        for(a=x0; < x1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
                trQuestVarSetFromRand("z", z0, z1, true);
                trQuestVarSet("locX", a);
                trQuestVarSet("locZ", trQuestVarGet("z"));
                if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                    trQuestVarSetFromRand("type", 1, 3, true);
                    trQuestVarSetFromRand("heading", 0, 360, true);
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
                        2*a,0,2*trQuestVarGet("z"),trQuestVarGet("heading"),true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitConvert(0);
                    if (scale > 0) {
                        trSetSelectedScale(scale,scale,scale);
                    }
                }
            }
        }
        for(b=z0; < z1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
                trQuestVarSetFromRand("x", x0, x1, true);
                trQuestVarSet("locX", trQuestVarGet("x"));
                trQuestVarSet("locZ", b);
                if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
                    trQuestVarSetFromRand("type", 1, 3, true);
                    trQuestVarSetFromRand("heading", 0, 360, true);
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
                        2*trQuestVarGet("x"),0,2*b,trQuestVarGet("heading"),true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitConvert(0);
                    if (scale > 0) {
                        trSetSelectedScale(scale,scale,scale);
                    }
                }
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
                trChangeTerrainHeight(a, 1*trQuestVarGet("z"), a+1, 1*trQuestVarGet("z")+1, wallHeight, false);
            }
        }
        for(b=z0; < z1) {
            trQuestVarSetFromRand("deploy", 0, 1, false);
            if (trQuestVarGet("deploy") < trQuestVarGet("columnDensity")) {
                trQuestVarSetFromRand("x", x0, x1, true);
                trPaintTerrain(1*trQuestVarGet("x")-1, b-1, 1*trQuestVarGet("x")+1, b+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                trChangeTerrainHeight(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x")+1, b+1, wallHeight, false);
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
        trQuestVarSet("locx", a);
        trQuestVarSet("locz", trQuestVarGet("z"));
        if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
            trPaintTerrain(a, 1*trQuestVarGet("z"), a, 1*trQuestVarGet("z"), TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
        }
    }
    for(b=z0; < z1) {
        trQuestVarSetFromRand("x", x0, x1, true);
        trQuestVarSet("locx", trQuestVarGet("x"));
        trQuestVarSet("locz", b);
        if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
            trPaintTerrain(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x"), b, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
        }
    }
}

void buildRoom(int x = 0, int z = 0, int type = 0) {
    int room = x + 4 * z;
    int size = 0;
    int x0 = 0;
    int z0 = 0;
    int x1 = 0;
    int z1 = 0;
    bool trapped = false;
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
                        trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                        trArmyDispatch("1,0","Dwarf",1,x*70+41,0,z*70+41,0,true);
                        yAddToDatabase("laserRooms","next");
                        yAddUpdateVar("laserRooms", "posX", x * 70 + 40);
                        yAddUpdateVar("laserRooms", "posZ", z * 70 + 40);
                        yAddUpdateVar("laserRooms", "room", room);
                        yAddUpdateVar("laserRooms", "active", 0);
                        for(a=0; <8) {
                            /* z lasers */
                            trArmyDispatch("1,0","Dwarf",1,2*x0+4*a+3,0,2*z0+1,180,false);
                            trArmyDispatch("1,0","Dwarf",1,2*x0+4*a+3,0,2*z1-1,0,false);
                            yAddUpdateVar("laserRooms", "xLaser"+a+"0x", 2*x0+4*a + 2);
                            yAddUpdateVar("laserRooms", "xLaser"+a+"1x", 2*x0+4*a + 4);
                            yAddUpdateVar("laserRooms", "xLaser"+a+"0z", 2*z0);
                            yAddUpdateVar("laserRooms", "xLaser"+a+"1z", 2*z1);
                            /* x lasers */
                            trArmyDispatch("1,0","Dwarf",1,2*x0+1,0,2*z0+4*a+3,270,false);
                            trArmyDispatch("1,0","Dwarf",1,2*x1-1,0,2*z0+4*a+3,90,false);
                            yAddUpdateVar("laserRooms", "zLaser"+a+"0x", 2*x0);
                            yAddUpdateVar("laserRooms", "zLaser"+a+"1x", 2*x1);
                            yAddUpdateVar("laserRooms", "zLaser"+a+"0z", 2*z0+4*a + 2);
                            yAddUpdateVar("laserRooms", "zLaser"+a+"1z", 2*z0+4*a + 4);
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
    }
    if (trQuestVarGet("room"+room+"key") > 0) {
        vector v0 = randomNearEdge(x0, z0, x1, z1);
        ySetVarAtIndex("unlockWalls", "relic", trGetNextUnitScenarioNameNumber(), 1*trQuestVarGet("room"+room+"index"));
        spawnRelicSpecific(xsVectorGetX(v0),xsVectorGetZ(v0),1*trQuestVarGet("room"+room+"key"));
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
            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
            yAddToDatabase("chests", "next");
            yAddUpdateVar("chests", "type", trQuestVarGet("chestType"));
            yAddUpdateVar("chests", "room", room);
            trArmyDispatch("1,0","Dwarf",1,x * 70 + 40,0, z * 70 + 40, 135, true);
            trArmySelect("1,0");
            trUnitConvert(0);
            trUnitChangeProtoUnit("Great Box");
        }
        case ROOM_TRANSPORTER_GUY:
        {
            x0 = trQuestVarGet("room"+room+"bottom1x");
            z0 = trQuestVarGet("room"+room+"bottom1z");
            x1 = trQuestVarGet("room"+room+"top1x");
            z1 = trQuestVarGet("room"+room+"top1z");
            trVectorQuestVarSet("center", xsVectorSet(x0 + x1, 0, z0 + z1));
            trVectorQuestVarSet("relictransporterguypos", randomNearEdge(x0, z0, x1, z1));
            trQuestVarSet("heading", 180.0 / 3.141592 * angleBetweenVectors("relictransporterguypos", "center"));
            trQuestVarSet("relicTransporterGuyName", trGetNextUnitScenarioNameNumber());
            x0 = trQuestVarGet("relicTransporterGuyPosx");
            z0 = trQuestVarGet("relicTransporterGuyPosz");
            trArmyDispatch("1,0","Villager Atlantean Hero",1,x0,0,z0,trQuestVarGet("heading"), true);
            trUnitSelectClear();
            trUnitSelectByQV("relicTransporterGuyName", true);
            trUnitConvert(0);
            trArmyDispatch("1,0", "Dwarf",1,x0,0,z0,0,true);
            trArmySelect("1,0");
            trUnitChangeProtoUnit("Gaia Forest effect");
            xsEnableRule("relic_transporter_guy_found");
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

            trQuestVarSetFromRand("localQuest", 1, 3, true);

            trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Villager Chinese",23,19,315);
            trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Maceman",19,25,225);
            yAddToDatabase("stunnedUnits", "guy"+BOUNTY_GUY);
            yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Maceman"));
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
            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Runestone", 35,35,225);
            trUnitSelectClear();
            trUnitSelectByQV("next", true);
            trSetSelectedScale(2.5,2.5,2.5);
            deployTownEyecandy("Shrine",15,31,90);
            deployTownEyecandy("Shrine",17,11,0);
            deployTownEyecandy("Shrine",35,15,270);

            trQuestVarSetFromRand("localQuest", 1, 3, true);

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

            trQuestVarSet("guy"+FETCH_GUY, trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Throwing Axeman",21,9,270);
            trQuestVarSet("guy"+BOUNTY_GUY, trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Ulfsark",15,23,135);
            trQuestVarSet("guy"+SHOP_GUY, trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Dwarf",25,25,225);
            yAddToDatabase("stunnedUnits", "guy"+FETCH_GUY);
            yAddUpdateVar("stunnedUnits","proto", kbGetProtoUnitID("Throwing Axeman"));
            yAddToDatabase("stunnedUnits", "guy"+BOUNTY_GUY);
            yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Ulfsark"));
        }
        case ROOM_VILLAGE + 4:
        {
            trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, worldHeight, false);
            paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
            paintEyecandy(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, "sprite");

            trQuestVarSet("villageX", 70 * x + 21);
            trQuestVarSet("villageZ", 70 * z + 21);


            trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
            deployTownEyecandy("Tower Mirror",12,12,0);
            trUnitSelectClear();
            trUnitSelectByQV("next");
            trUnitOverrideAnimation(2,0,true,false,-1);
            trSetSelectedScale(2,2,2);
            deployTownEyecandy("Guild",16,12,180);
            deployTownEyecandy("Guild",12,16,270);
            deployTownEyecandy("Tamarisk Tree", 16, 16, 0);

            trQuestVarSetFromRand("localQuest", 1, 3, true);

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
        }
        case ROOM_STARTER:
        {
            trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, worldHeight, false);
            paintSecondary(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30);
            trVectorQuestVarSet("startPosition", xsVectorSet(x*70 + 40,0,z*70+40));
            if (trQuestVarGet("newPlayers") > 0) {
                trVectorQuestVarSet("choice1", xsVectorSet(x*70+48,0,z*70+54));
                trVectorQuestVarSet("choice2", xsVectorSet(x*70+54,0,z*70+48));
                trQuestVarSet("choice1unit", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", "Hero Greek Theseus", 1, trQuestVarGet("choice1x"),0,trQuestVarGet("choice1z"),225,true);
                trQuestVarSet("choice2unit", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", "Hero Greek Hippolyta", 1, trQuestVarGet("choice2x"),0,trQuestVarGet("choice2z"),225,false);
                trArmySelect("1,0");
                trUnitConvert(0);
                yAddToDatabase("stunnedUnits", "choice1unit");
                yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Hero Greek Theseus"));
                yAddToDatabase("stunnedUnits", "choice2unit");
                yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Hero Greek Hippolyta"));
                xsEnableRule("class_shop_always");
            }
        }
        case ROOM_BOSS:
        {
            size = trQuestVarGet("bossRoomSize");
            if (trQuestVarGet("mapType") == MAP_OPEN) {
                trPaintTerrain(x*35+17-size,z*35+17-size,x*35+23+size,z*35+23+size, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                trChangeTerrainHeight(x*35+17-size,z*35+17-size,x*35+24+size,z*35+24+size, wallHeight, false);
                trQuestVarSet("bossEntranceRadius", xsPow(5, 2));
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
            trQuestVarSet("bossRoomLocX", 0.5 * trQuestVarGet("bossRoomCenterX"));
            trQuestVarSet("bossRoomLocZ", 0.5 * trQuestVarGet("bossRoomCenterZ"));
            trQuestVarSet("bossRoomEntranceX", trQuestVarGet("bossRoomCenterX"));
            trQuestVarSet("bossRoomEntranceZ", trQuestVarGet("bossRoomCenterZ"));
            xsEnableRule("enter_boss_room");
        }
        case ROOM_NOTTUD:
        {
            debugLog("nottud room is " + room);
            z0 = 10;
            for(a=0; < 10) {
                for(b=8; >0) {
                    if (a*a + z0 * z0 <= 100) {
                        trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
                        trChangeTerrainHeight(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, worldHeight, false);
                        break;
                    } else {
                        z0 = z0 - 1;
                    }
                }
            }
            trQuestVarSet("nottud", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+40, 0, 70*z+40,225,true);
            trUnitSelectClear();
            trUnitSelectByQV("nottud", true);
            trUnitConvert(0);
            trUnitChangeProtoUnit("Minotaur");
            yAddToDatabase("stunnedUnits", "nottud");
            yAddUpdateVar("stunnedUnits", "proto", kbGetProtoUnitID("Minotaur"));
            xsEnableRule("nottud_always");
            for(i=0; <5) {
                trQuestVarSet("choice"+i, 21 + i);
            }
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
                trQuestVarSetFromRand("rand", 0, 4 - i, true);
                yAddToDatabase("nottudShop", "next");
                yAddUpdateVar("nottudShop", "relic", trQuestVarGet("choice"+1*trQuestVarGet("rand")));
                yAddUpdateVar("nottudShop", "shopPosx", trQuestVarGet("obeliskx"+i));
                yAddUpdateVar("nottudShop", "shopPosz", trQuestVarGet("obeliskz"+i));
                trQuestVarSet("choice"+1*trQuestVarGet("rand"), trQuestVarGet("choice"+(4-i)));
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
    int first = edge / 16;
    int second = edge - 16 * first;
    int z0 = first / 4;
    int x0 = first - 4 * z0;
    int z1 = second / 4;
    int x1 = second - 4 * z1;
    int x = 0;
    int z = 0;
    if (trQuestVarGet("edge"+edge) == EDGE_NOT_FOUND) {
        if (type == EDGE_MOUNTAIN) {
            yClearDatabase("edgeFrontier");
            x0 = x0 * 35;
            z0 = z0 * 35;
            x1 = x1 * 35;
            z1 = z1 * 35;
            x1 = (x0 + x1) / 2 + 20;
            z1 = (z0 + z1) / 2 + 20;
            trQuestVarSet("next", wallHeight - worldHeight);
            yAddToDatabase("edgeFrontier", "next");
            yAddUpdateVar("edgeFrontier", "locx", x1);
            yAddUpdateVar("edgeFrontier", "locz", z1);
            trQuestVarSet("x"+x1+"z"+z1, TILE_FOUND);
            trQuestVarSetFromRand("size", 40, 80);
            for(a=trQuestVarGet("size"); >0) {
                trQuestVarSetFromRand("rand", 0, 7, true);
                yDatabaseNext("edgeFrontier");
                for(b=trQuestVarGet("rand"); >0) {
                    yDatabaseNext("edgeFrontier");
                }
                x0 = yGetVar("edgeFrontier", "locx");
                z0 = yGetVar("edgeFrontier", "locz");
                trPaintTerrain(x0-2,z0-2,x0+1,z0+1,TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                trChangeTerrainHeight(x0-1,z0-1,x0+1,z0+1,worldHeight+trQuestVarGet("edgeFrontier"),false);
                if (xsPow(x0-x1, 2) + xsPow(z0-z1,2) < 324) {
                    for(b=0; < 4) {
                        trQuestVarSetFromRand("next", trQuestVarGet("edgeFrontier") - 1, trQuestVarGet("edgeFrontier"), true);
                        if (trQuestVarGet("next") > 0) {
                            x = x0 + 2 * trQuestVarGet("rotX"+b);
                            z = z0 + 2 * trQuestVarGet("rotZ"+b);
                            if (trQuestVarGet("x"+x+"z"+z) == TILE_NOT_FOUND) {
                                yAddToDatabase("edgeFrontier", "next");
                                yAddUpdateVar("edgeFrontier", "locx", x);
                                yAddUpdateVar("edgeFrontier", "locz", z);
                                trQuestVarSet("x"+x+"z"+z, TILE_FOUND);
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
            trArmyDispatch("1,0","Dwarf",1,x0*70+40,0,z0*70+40,0,true);
            trQuestVarSet("next2", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0","Dwarf",1,x1*70+40,0,z1*70+40,0,true);
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
            yAddToDatabase("skyPassages", "next1");
            yAddUpdateVar("skyPassages", "exit", trQuestVarGet("next2"));
            yAddToDatabase("skyPassages", "next2");
            yAddUpdateVar("skyPassages", "exit", trQuestVarGet("next1"));
        } else {
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
            trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
            paintSecondary(x0, z0, x1, z1);
            
            if ((type == EDGE_WALL) && (trQuestVarGet("wallEdges") * yGetDatabaseCount("visited") > 0)) {
                trQuestVarSetFromRand("rand", 1, 16, true);
                for (x=trQuestVarGet("rand"); >0) {
                    yDatabaseNext("visited");
                }
                trQuestVarSet("room"+1*trQuestVarGet("visited")+"key", trQuestVarGet("keyType"));
                trQuestVarSet("room"+1*trQuestVarGet("visited")+"index", yAddToDatabase("unlockWalls", "visited"));
                yAddUpdateVar("unlockWalls", "sfxStart", trGetNextUnitScenarioNameNumber());
                yAddUpdateVar("unlockWalls", "posX", x0 + x1);
                yAddUpdateVar("unlockWalls", "posZ", z0 + z1);
                yRemoveFromDatabase("visited");

                if (x1 - x0 > z1 - z0) {
                    yAddUpdateVar("unlockWalls", "xMod", 0);
                    yAddUpdateVar("unlockWalls", "zMod", z1 - z0);
                    x0 = (x0 + x1);
                    for(z=z0; <= z1) {
                        trArmyDispatch("1,0","Dwarf",1,x0+2,0,2*z,0,true);
                        trArmySelect("1,0");
                        trMutateSelected(kbGetProtoUnitID("UI Range Indicator Greek SFX"));
                    }
                    x0 = x0 / 2;
                    trPaintTerrain(x0-1, z0-1, x0+1, z1+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                    trChangeTerrainHeight(x0, z0, x0+1, z1+1, wallHeight, false);
                    yAddUpdateVar("unlockWalls", "greenCirclesStart", trGetNextUnitScenarioNameNumber());
                    x0 = x0 * 2;
                    for(z=z0; <= z1) {
                        trArmyDispatch("1,0","Dwarf",1,x0,0,2*z,270,true);
                        trArmySelect("1,0");
                        trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
                    }
                } else {
                    yAddUpdateVar("unlockWalls", "xMod", x1 - x0);
                    yAddUpdateVar("unlockWalls", "zMod", 0);
                    z0 = (z0 + z1);
                    for(x=x0; <= x1) {
                        trArmyDispatch("1,0","Dwarf",1,2*x,0,z0,90,true);
                        trArmySelect("1,0");
                        trMutateSelected(kbGetProtoUnitID("UI Range Indicator Greek SFX"));
                    }
                    z0 = z0 / 2;
                    trPaintTerrain(x0-1, z0-1, x1+1, z0+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
                    trChangeTerrainHeight(x0, z0, x1+1, z0+1, wallHeight, false);
                    yAddUpdateVar("unlockWalls", "greenCirclesStart", trGetNextUnitScenarioNameNumber());
                    z0 = z0 * 2;
                    for(x=x0; <= x1) {
                        trArmyDispatch("1,0","Dwarf",1,2*x,0,z0,180,true);
                        trArmySelect("1,0");
                        trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
                    }
                }
                yAddUpdateVar("unlockWalls", "end", trGetNextUnitScenarioNameNumber());
                trQuestVarSet("keyType", trQuestVarGet("keyType") - 1);
                trQuestVarSet("wallEdges", trQuestVarGet("wallEdges") - 1);
            }

        }
        trQuestVarSet("edge"+edge, type);
    }
}

void connectStatues(int index1 = 0, int index2 = 0, int room = 0) {
    ySetPointer("statuesIn"+room, index1);
    ySetVar("statuesIn"+room, "connections", 1 + yGetVar("statuesIn"+room, "connections"));
    ySetVar("statuesIn"+room, "connection"+1*yGetVar("statuesIn"+room, "connections"), index2);
    trQuestVarSet("pos1X", yGetVar("statuesIn"+room, "posX"));
    trQuestVarSet("pos1Z", yGetVar("statuesIn"+room, "posZ"));

    ySetPointer("statuesIn"+room, index2);
    ySetVar("statuesIn"+room, "connections", 1 + yGetVar("statuesIn"+room, "connections"));
    ySetVar("statuesIn"+room, "connection"+1*yGetVar("statuesIn"+room, "connections"), index1);
    trQuestVarSet("pos2X", yGetVar("statuesIn"+room, "posX"));
    trQuestVarSet("pos2Z", yGetVar("statuesIn"+room, "posZ"));

    trVectorQuestVarSet("dir", zGetUnitVector("pos1", "pos2"));
    trQuestVarSet("temp", trQuestVarGet("dirX"));
    trQuestVarSet("dirX", 0.0 - trQuestVarGet("dirZ"));
    trQuestVarSet("dirZ", trQuestVarGet("temp"));

    float dist = zDistanceBetweenVectors("pos1", "pos2");
    float x = 0.5 * (trQuestVarGet("pos1x") + trQuestVarGet("pos2x"));
    float z = 0.5 * (trQuestVarGet("pos1z") + trQuestVarGet("pos2z"));

    trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
    trArmySelect("1,0");
    trUnitConvert(0);
    trSetSelectedScale(dist / 9.0, 1, 0.3);
    trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
    trUnitTeleport(x,0,z);
    trMutateSelected(kbGetProtoUnitID("undermine ground decal long"));
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
            /* bacchanalia 10 x stage */
            for(j=10; >0) {
                trTechSetStatus(ENEMY_PLAYER, 78, 4);
            }
        }
        for(i=trQuestVarGet("stage")/2; >0) {
            /* monstrous rage */
            trTechSetStatus(ENEMY_PLAYER, 76, 4);
        }
        trQuestVarSet("rotX0", -1);
        trQuestVarSet("rotX1", 1);
        trQuestVarSet("rotX2", 0);
        trQuestVarSet("rotX3", 0);
        trQuestVarSet("rotZ0", 0);
        trQuestVarSet("rotZ1", 0);
        trQuestVarSet("rotZ2", -1);
        trQuestVarSet("rotZ3", 1);
        /* minecraft time! */
        switch(1*trQuestVarGet("stage"))
        {
            case 1:
            {
                trSetCivAndCulture(0, 3, 1);
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

                trQuestVarSet("enemyDensity", 0.02 + 0.02 * ENEMY_PLAYER);
                trQuestVarSet("enemyProtoCount", 5);
                trStringQuestVarSet("enemyProto1", "Golden Lion");
                trStringQuestVarSet("enemyProto2", "Anubite");
                trStringQuestVarSet("enemyProto3", "Terracotta Soldier");
                trStringQuestVarSet("enemyProto4", "Sphinx");
                trStringQuestVarSet("enemyProto5", "Petsuchos");

                trQuestVarSet("columnDensity", 0.05);

                trStringQuestVarSet("bossProto", "Nemean Lion");
                trQuestVarSet("bossScale", 2);

                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, 9999999999999999999.0);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, -9999999999999999999.0);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 0, 4000 * ENEMY_PLAYER);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 1, 4.8);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 24, -1);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 25, -1);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 26, -1);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 34, 9999999999999999999.0);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 34, -9999999999999999999.0);
                trModifyProtounit("Nemean Lion", ENEMY_PLAYER, 34, 1.0);
            }
            case 2:
            {
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

                trQuestVarSet("enemyDensity", 0.03 + 0.03 * ENEMY_PLAYER);
                trQuestVarSet("enemyProtoCount", 5);
                trStringQuestVarSet("enemyProto1", "Walking Woods Marsh");
                trStringQuestVarSet("enemyProto2", "Centaur");
                trStringQuestVarSet("enemyProto3", "Dryad");
                trStringQuestVarSet("enemyProto4", "Medusa");
                trStringQuestVarSet("enemyProto5", "Mountain Giant");

                trQuestVarSet("columnDensity", 0);

                trStringQuestVarSet("bossProto", "Tamarisk Tree");
                trQuestVarSet("bossScale", 1.5);
            }
            case 3:
            {
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
                trQuestVarSet("bossScale", 1.25);

                trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, 9999999999999999999.0);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, -9999999999999999999.0);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 0, 8000 * ENEMY_PLAYER);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 1, 1.075);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 24, -1);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 25, -1);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 26, -1);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 34, 9999999999999999999.0);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 34, -9999999999999999999.0);
                trModifyProtounit("King Folstag", ENEMY_PLAYER, 34, 1.0);
            }
            case 4:
            {
                wallHeight = worldHeight + 6;
                trSetCivAndCulture(0, 0, 0);
                trQuestVarSet("bossRoomSize", 14);
                TERRAIN_WALL = 2;
                TERRAIN_SUB_WALL = 1;
                
                TERRAIN_PRIMARY = 0;
                TERRAIN_SUB_PRIMARY = 25;

                TERRAIN_SECONDARY = 0;
                TERRAIN_SUB_SECONDARY = 3;

                trQuestVarSet("mapType", MAP_OPEN);
                trQuestVarSet("treeDensity", 0.3);
                trStringQuestVarSet("treeProto1", "Pine");
                trStringQuestVarSet("treeProto2", "Pine");
                trStringQuestVarSet("treeProto3", "Ruins");
                trQuestVarSet("spriteDensity", 0.6);
                trStringQuestVarSet("spriteProto1", "Flowers");
                trStringQuestVarSet("spriteProto2", "Grass");
                trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
                trQuestVarSet("rockDensity", 0.4);
                trStringQuestVarSet("rockProto1", "Rock Granite Big");
                trStringQuestVarSet("rockProto2", "Columns Broken");
                trStringQuestVarSet("rockProto3", "Rock Granite Small");

                trQuestVarSet("enemyDensity", 0.045 + 0.045 * ENEMY_PLAYER);
                
                trQuestVarSet("enemyProtoCount", 5);
                trStringQuestVarSet("enemyProto1", "Cyclops");
                trStringQuestVarSet("enemyProto2", "Satyr");
                trStringQuestVarSet("enemyProto3", "Behemoth");
                trStringQuestVarSet("enemyProto4", "Medusa");
                trStringQuestVarSet("enemyProto5", "Avenger");
                
                trStringQuestVarSet("bossProto", "Chimera");
                trQuestVarSet("bossScale", 1.5);

                trModifyProtounit("Chimera", ENEMY_PLAYER, 0, 9999999999999999999.0);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 0, -9999999999999999999.0);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 0, 12000 * ENEMY_PLAYER);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 24, -1);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 25, -1);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 26, -1);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 34, 9999999999999999999.0);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 34, -9999999999999999999.0);
                trModifyProtounit("Chimera", ENEMY_PLAYER, 34, 1.0);
            }
            case 5:
            {
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
                trQuestVarSet("treeDensity", 0.1);
                trStringQuestVarSet("treeProto1", "Statue Pharaoh");
                trStringQuestVarSet("treeProto2", "Statue Pharaoh");
                trStringQuestVarSet("treeProto3", "Statue Pharaoh");
                trQuestVarSet("spriteDensity", 0.3);
                trStringQuestVarSet("spriteProto1", "Rock Dirt Sprite");
                trStringQuestVarSet("spriteProto2", "Cinematic Scorch");
                trStringQuestVarSet("spriteProto3", "Skeleton");
                trQuestVarSet("rockDensity", 0.2);
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
                trQuestVarSet("bossScale", 1.5);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, 9999999999999999999.0);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, -9999999999999999999.0);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 0, 16000 * ENEMY_PLAYER);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 24, -1);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 25, -1);
                trModifyProtounit("Shade of Hades", ENEMY_PLAYER, 26, -1);
                trQuestVarSet("pestilenceStart", trGetNextUnitScenarioNameNumber());
            }
            case 6:
            {
                xsEnableRule("laser_rooms_always");
                /* engineers */
                trTechSetStatus(ENEMY_PLAYER, 59, 4);
                ambientColor(100,50,0);
                sunColor(150,150,0);
                trSetCloudData(1.0, 50.0, 50.0, 0.0, 1.0, 0.0);
                trSetCloudMap("shockwall");
                trSetCivAndCulture(0, 3, 1);
                trQuestVarSet("bossRoomShape", ROOM_SQUARE);
                trQuestVarSet("bossRoomSize", 10);
                trQuestVarSet("wallEdges", 4);
                trQuestVarSet("trapRooms", 3);
                trQuestVarSet("trapType", TRAP_LASERS);
                TERRAIN_WALL = 2;
                TERRAIN_SUB_WALL = 5;
                
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
                trQuestVarSet("bossScale", 0.25);

                trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, 9999999999999999999.0);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, -9999999999999999999.0);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 0, 20000 * ENEMY_PLAYER);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 24, -1);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 25, -1);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 26, -1);
                trModifyProtounit("Helepolis", ENEMY_PLAYER, 13, -2);
            }
        }

        /* paint entire map cliff and raise it */
        trChangeTerrainHeight(0,0,145,145,wallHeight,false);
        trPaintTerrain(0,0,145,145,TERRAIN_WALL, TERRAIN_SUB_WALL,false);

        if (trQuestVarGet("mapType") == MAP_OPEN) {
            trChangeTerrainHeight(5,5,140,140,worldHeight,false);
            trPaintTerrain(5,5,139,139,TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY,false);
            paintSecondary(5,5,139,139);
        }

        /* build the grid */
        trQuestVarSet("keyType", RELIC_KEY_EGYPT);
        int chests = 3;
        int x = 0;
        int z = 0;
        int n = 0;
        int total = 0;
        int backtrack = 5;
        bool edgeIsPortal = false;
        trQuestVarSet("tile0", TILE_VISITED);
        trQuestVarSet("tile1", TILE_FOUND);
        trQuestVarSet("tile4", TILE_FOUND);

        trQuestVarSetFromRand("village", 1, 14, true);
        if (trQuestVarGet("mapType") == MAP_PORTALS) {
            total = 1;
            trQuestVarSet("tile"+1*trQuestVarGet("village"), TILE_VISITED);
            trQuestVarSetFromRand("villageEntrance", 1, 13, true);
            trQuestVarSet("villageEntrance", trQuestVarGet("village") + trQuestVarGet("villageEntrance"));
            if (trQuestVarGet("villageEntrance") > 14) {
                trQuestVarSet("villageEntrance", trQuestVarGet("villageEntrance") - 13);
            }
            debugLog("Village entrance is " + 1*trQuestVarGet("villageEntrance"));
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
        }

        if (trQuestVarGet("tile1") == TILE_FOUND) {
            trQuestVarSet("next", 1);
            yAddToDatabase("frontier", "next");
            yAddUpdateVar("frontier", "edge", edgeName(0, 1));
            yAddUpdateVar("frontier", "type", EDGE_NORMAL);
        }
        if (trQuestVarGet("mapType") == MAP_OPEN) {
            yAddUpdateVar("frontier", "type", EDGE_MOUNTAIN);
        }

        if (trQuestVarGet("tile4") == TILE_FOUND) {
            trQuestVarSet("next", 4);
            yAddToDatabase("frontier", "next");
            yAddUpdateVar("frontier", "edge", edgeName(0, 4));
            yAddUpdateVar("frontier", "type", EDGE_NORMAL);
        }
        if (trQuestVarGet("mapType") == MAP_OPEN) {
            yAddUpdateVar("frontier", "type", EDGE_MOUNTAIN);
        }
        
        /* build guaranteed path to every room */
        for(i=0; < 64) {
            trQuestVarSetFromRand("search", 1, backtrack, true);
            for(j=trQuestVarGet("search"); >0) {
                yDatabaseNext("frontier", false, false);
            }
            if (trQuestVarGet("tile"+1*trQuestVarGet("frontier")) < TILE_VISITED) {
                z = 1*trQuestVarGet("frontier") / 4;
                x = 1*trQuestVarGet("frontier") - 4 * z;
                buildEdge(1*yGetVar("frontier", "edge"), 1*yGetVar("frontier", "type"));
                
                edgeIsPortal = (yGetVar("frontier", "type") == EDGE_PORTAL);
                trQuestVarSet("tile"+1*trQuestVarGet("frontier"), TILE_VISITED);
                if (trQuestVarGet("frontier") < 15 && trQuestVarGet("frontier") != trQuestVarGet("village")) {
                    yAddToDatabase("visited", "frontier");
                }
                /* only add more edges if the room is not the boss room or the village room */
                if (trQuestVarGet("frontier") < 15 && trQuestVarGet("frontier") != trQuestVarGet("village")) {
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
                                yAddUpdateVar("frontier", "type", EDGE_NORMAL);
                                if (trQuestVarGet("wallEdges") > 0 && yGetDatabaseCount("visited") > 0) {
                                    trQuestVarSetFromRand("rand", 1, xsMin(3, trQuestVarGet("wallEdges")), true);
                                    if (trQuestVarGet("rand") == 1) {
                                        yAddUpdateVar("frontier", "type", EDGE_WALL);
                                    }
                                }
                                if (trQuestVarGet("mapType") == MAP_OPEN) {
                                    yAddUpdateVar("frontier", "type", EDGE_MOUNTAIN);
                                }
                            }
                        }
                    }
                    if (edgeIsPortal == false) {
                        if ((trQuestVarGet("relictransporterguy") == 0) && (trQuestVarGet("frontier") != trQuestVarGet("villageEntrance")) &&
                            (trQuestVarGet("frontier") != trQuestVarGet("bossEntranceRoom"))) {
                            trQuestVarSet("relicTransporterGuy", trQuestVarGet("frontier"));
                        } else if (trQuestVarGet("mapType") == MAP_PORTALS) {
                            trQuestVarSetFromRand("rand", 1, 14, true);
                            n = trQuestVarGet("rand");
                            if (trQuestVarGet("tile"+n) < TILE_VISITED && 
                                n != trQuestVarGet("villageEntrance") &&
                                n != trQuestVarGet("relicTransporterGuy") &&
                                trQuestVarGet("villageEntrance") != trQuestVarGet("frontier")) {
                                yAddToDatabase("frontier", "rand");
                                yAddUpdateVar("frontier", "edge", edgeName(1*trQuestVarGet("frontier"), n));
                                yAddUpdateVar("frontier", "type", EDGE_PORTAL);
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
            yRemoveUpdateVar("frontier", "type");
        }

        buildRoom(0,0, ROOM_STARTER);
        buildRoom(3,3, ROOM_BOSS);

        /* make portals more visible */
        for(x=yGetDatabaseCount("skypassages"); >0) {
            yDatabaseNext("skyPassages");
            trVectorSetUnitPos("pos", "skyPassages");
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")-4,0,trQuestVarGet("posZ"),0,true);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")+4,0,trQuestVarGet("posZ"),0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ")-4,0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ")+4,0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")+3,0,trQuestVarGet("posZ")+3,0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")+3,0,trQuestVarGet("posZ")-3,0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")-3,0,trQuestVarGet("posZ")+3,0,false);
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX")-3,0,trQuestVarGet("posZ")-3,0,false);
            trArmySelect("1,0");
            trUnitConvert(0);
            trUnitChangeProtoUnit("Garrison Flag Sky Passage");
        }

        bool nottudSpawn = false;
        trQuestVarSetFromRand("nottudSpawn", 0, 20, true);
        if (trQuestVarGet("nottudSpawn") < trQuestVarGet("stage")) {
            nottudSpawn = true;
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
                } else if (i == 1*trQuestVarGet("relicTransporterGuy")) {
                    buildRoom(x, z, ROOM_TRANSPORTER_GUY);
                } else if (i == 1*trQuestVarGet("village")) {
                    xsEnableRule("town_always");
                    buildRoom(x, z, ROOM_VILLAGE + trQuestVarGet("stage"));
                    /* quest givers */
                    for(x=3; >0) {
                        if (trQuestVarGet("localQuest") == x) {
                            trVectorSetUnitPos("pos", "guy"+x);
                            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
                            trArmySelect("1,0");
                            trUnitChangeProtoUnit("Healing SFX");
                            trQuestVarSet("questGuy", trQuestVarGet("guy"+x));
                        } else {
                            yAddToDatabase("npcTalk", "guy"+x);
                            yAddUpdateVar("npcTalk", "dialog", 10 * x + trQuestVarGet("stage"));
                        }
                    }
                } else if (nottudSpawn && (countRoomEntrances(x, z) == 1)) {
                    buildRoom(x, z, ROOM_NOTTUD);
                    nottudSpawn = false;
                    xsEnableRule("nick_00_visit");
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
    
        /* finish making chest rooms */
        int room = 0;
        int x0 = 0;
        int z0 = 0;
        int x1 = 0;
        int z1 = 0;
        string pName = "";
        trQuestVarSet("keyType", RELIC_KEY_GREEK);
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
                    ySetVar("chests", "key", trQuestVarGet("next"));

                    trVectorSetUnitPos("pos", "chests");
                    trQuestVarSet("posX", trQuestVarGet("posX") - 2);
                    trQuestVarSet("posZ", trQuestVarGet("posZ") - 2);
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),225,true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitConvert(0);
                    trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));
                    ySetVar("chests", "indicator", trQuestVarGet("next"));

                    trQuestVarSet("keyType", 1 + trQuestVarGet("keyType"));
                }
                case CHEST_STATUES:
                {
                    trVectorSetUnitPos("pos", "chests");
                    trQuestVarSetFromRand("rand", 4, 7, true);
                    trQuestVarSet("angle", 0.785398);
                    trQuestVarSet("angleMod", 6.283185 / trQuestVarGet("rand"));
                    for(x=0; < trQuestVarGet("rand")) {
                        trVectorSetFromAngle("dir", trQuestVarGet("angle"));
                        trQuestVarSet("statueX", trQuestVarGet("posX") - 10.0 * trQuestVarGet("dirX"));
                        trQuestVarSet("statueZ", trQuestVarGet("posZ") - 10.0 * trQuestVarGet("dirZ"));
                        

                        trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                        trArmyDispatch("1,0","Dwarf",1,1,0,1,180,true);
                        trUnitSelectClear();
                        trUnitSelectByQV("next", true);
                        trUnitConvert(ENEMY_PLAYER);
                        trUnitTeleport(trQuestVarGet("statueX"),0,trQuestVarGet("statueZ"));
                        trMutateSelected(kbGetProtoUnitID("Monument 2"));
                        trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);

                        trQuestVarSet("index", yAddToDatabase("statuesIn"+room, "next"));
                        yAddUpdateVar("statuesIn"+room, "angle", trQuestVarGet("angle"));
                        yAddUpdateVar("statuesIn"+room, "posX", trQuestVarGet("statueX"));
                        yAddUpdateVar("statuesIn"+room, "posZ", trQuestVarGet("statueZ"));
                        yAddToDatabase("statuesReady", "index");

                        trQuestVarSetFromRand("jump", 1, 2, true);
                        for (y=trQuestVarGet("jump"); >0) {
                            yDatabaseNext("statuesReady");
                        }

                        trQuestVarSet("angle", fModulo(6.283185, trQuestVarGet("angle") + trQuestVarGet("angleMod")));
                    }

                    for(x=trQuestVarGet("rand"); >0) {
                        yDatabaseNext("statuesReady");
                        trQuestVarSet("start", yGetPointer("statuesReady"));
                        trQuestVarSetFromRand("jump", 1, (trQuestVarGet("rand") - 1) / 2, true);
                        for(y=trQuestVarGet("jump"); >0) {
                            yDatabaseNext("statuesReady");
                        }
                        connectStatues(yGetUnitAtIndex("statuesReady", 1*trQuestVarGet("start")), 1*trQuestVarGet("statuesReady"), room);
                        ySetPointer("statuesReady", 1*trQuestVarGet("start"));
                    }
                    trQuestVarSet("correctStatuesIn"+room, trQuestVarGet("rand"));
                    for(x=trQuestVarGet("rand"); >1) {
                        yDatabaseNext("statuesIn"+room);
                        trQuestVarSetFromRand("amt",1,3,true);
                        for(y=trQuestVarGet("amt"); >0) {
                            turnStatue(room, 0, true, true);
                        }
                    }
                    yClearDatabase("statuesReady");
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
                            yAddToDatabase("enemiesIncoming", "next");
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
                            yAddToDatabase("enemiesIncoming", "next");
                        }
                    }
                    ySetVar("chests", "enemiesEnd", trGetNextUnitScenarioNameNumber());
                }
            }
        }

        if (trQuestVarGet("mapType") == MAP_OPEN) {
            trUnitSelectClear();
            trUnitSelectByQV("bossEntranceSymbol", true);
            trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(1*trQuestVarGet("keyType"))));

            i = trQuestVarGet("bossKeyRoom");
            z = i / 4;
            x = i - z * 4;
            trQuestVarSet("bossKey", trGetNextUnitScenarioNameNumber());
            spawnRelicSpecific(x * 70 + 40, z * 70 + 40, 1*trQuestVarGet("keyType"));
            trQuestVarSet("keyType", 1 + trQuestVarGet("keyType"));
        }

        if (trQuestVarGet("pestilenceStart") > 0) {
            for(x=trQuestVarGet("pestilenceStart"); < trGetNextUnitScenarioNameNumber()) {
                if (kbGetUnitBaseTypeID(kbGetBlockID(""+x)) == kbGetProtoUnitID("Pestilence SFX1")) {
                    trQuestVarSet("next", x);
                    trVectorSetUnitPos("pos", "next");
                    trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
                    trArmySelect("1,0");
                    trUnitConvert(ENEMY_PLAYER);
                    trUnitChangeProtoUnit("Victory Marker");
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
            "Zenophobia: I'll let you have the starter characters for free.", "icons\infantry g hoplite icon 64");
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