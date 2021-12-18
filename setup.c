const int TILE_NOT_FOUND = 0;
const int TILE_FOUND = 1;
const int TILE_VISITED = 2;

const int EDGE_NOT_FOUND = 0;
const int EDGE_NORMAL = 1;
const int EDGE_CHASM = 2;

const int ROOM_BASIC = 0;
const int ROOM_STARTER = 1;
const int ROOM_BOSS = 2;

int ENEMY_PLAYER = 0;
bool Multiplayer = false;

/*
Chasm terrain is black
*/
int TERRAIN_CHASM = 5;
int TERRAIN_SUB_CHASM = 4;

/* 
Wall terrain is blackrock
*/
int TERRAIN_WALL = 2;
int TERRAIN_SUB_WALL = 13;

/*
Floor terrain
*/
int TERRAIN_PRIMARY = 0;
int TERRAIN_SUB_PRIMARY = 1;

/*
Secondary terrain
*/
int TERRAIN_SECONDARY = 0;
int TERRAIN_SUB_SECONDARY = 1;

void setupPlayerProto(string proto = "", int p = 0, float health = 1000, float range = 12, float speed = 4) {
    /* attack = 0 */
    trModifyProtounit(proto, p, 27, 9999999999999999999.0);
    trModifyProtounit(proto, p, 27, -9999999999999999999.0);
    trModifyProtounit(proto, p, 30, 9999999999999999999.0);
    trModifyProtounit(proto, p, 30, -9999999999999999999.0);
    trModifyProtounit(proto, p, 31, 9999999999999999999.0);
    trModifyProtounit(proto, p, 31, -9999999999999999999.0);
    /* health */
    trModifyProtounit(proto, p, 0, 9999999999999999999.0);
    trModifyProtounit(proto, p, 0, -9999999999999999999.0);
    trModifyProtounit(proto, p, 0, health);
    /* LOS */
    trModifyProtounit(proto, p, 2, 9999999999999999999.0);
    trModifyProtounit(proto, p, 2, -9999999999999999999.0);
    trModifyProtounit(proto, p, 2, 16);
    /* range */
    trModifyProtounit(proto, p, 11, 9999999999999999999.0);
    trModifyProtounit(proto, p, 11, -9999999999999999999.0);
    trModifyProtounit(proto, p, 11, range); 
    /* speed */
    trModifyProtounit(proto, p, 1, 9999999999999999999.0);
    trModifyProtounit(proto, p, 1, -9999999999999999999.0);
    trModifyProtounit(proto, p, 1, speed); 
    /* armor */
    trModifyProtounit(proto, p, 24, -1);
    trModifyProtounit(proto, p, 25, -1);
    trModifyProtounit(proto, p, 26, -1);
}

rule setup
active
runImmediately
{
    Multiplayer = aiIsMultiplayer();
    trSetUnitIdleProcessing(false); 
    
    trSetObscuredUnits(false);

    ENEMY_PLAYER = cNumberPlayers - 1;

    trForceNonCinematicModels(true);
    
    /*
    ambientColor(0,0,0);
    sunColor(0,0,0);
    trLetterBox(true);
    trUIFadeToColor(0,0,0,0,0,true);
    */

    modularCounterInit("spyFind", 32);
    modularCounterInit("spyFound", 32);

    /*
    player 0 omniscience
    */
    trTechSetStatus(0, 304, 4);
    
    /* Transport Ship max contained */
    trModifyProtounit("Transport Ship Atlantean", 1, 5, -11);
    for(p=1; < ENEMY_PLAYER) {
        /* pop count */
        trModifyProtounit("Vision Revealer", p, 7, 9999);
        /* LOS */
        trModifyProtounit("Vision Revealer", p, 2, -99);
        /* carry capacity */
        trModifyProtounit("Hero Greek Achilles", p, 5, 99);
        /* health */
        trModifyProtounit("Vision SFX", p, 0, -9999);
        /* flying */
        trModifyProtounit("Dwarf", p, 55, 4);
        trModifyProtounit("Athena", p, 55, 4);

        trPlayerKillAllGodPowers(p);
        trPlayerTechTreeEnabledGodPowers(p, false);

        trModifyProtounit("Sling Stone", p, 1, -29);
        trModifyProtounit("Arrow Flaming", p, 1, -29);
        trModifyProtounit("Javelin Flaming", p, 1, -29);
        trModifyProtounit("Axe", p, 1, -29);

        setupPlayerProto("Slinger", p, 1000, 12, 4);
        setupPlayerProto("Archer Atlantean", p, 1000, 22, 4);
        setupPlayerProto("Javelin Cavalry", p, 1000, 4, 5);
        setupPlayerProto("Throwing Axeman", p, 1000, 12, 4);
        setupPlayerProto("Minion", p, 300, 0, 4);

        trForbidProtounit(p, "Trident Soldier Hero");
        trForbidProtounit(p, "Archer Atlantean Hero");
        trForbidProtounit(p, "Royal Guard Hero");
        trForbidProtounit(p, "Lancer Hero");
        trForbidProtounit(p, "Javelin Cavalry Hero");
        trForbidProtounit(p, "Wall Connector");
        trForbidProtounit(p, "Dock");
        trForbidProtounit(p, "Settlement Level 1");
        trForbidProtounit(p, "House");
        trForbidProtounit(p, "Granary");
        trForbidProtounit(p, "Mining Camp");
        trForbidProtounit(p, "Lumber Camp");
        trForbidProtounit(p, "Monument");
        trForbidProtounit(p, "Temple");
        trForbidProtounit(p, "Wonder");
        trForbidProtounit(p, "Market");
        trForbidProtounit(p, "Dwarf Foundry");
        trForbidProtounit(p, "Armory");
        trForbidProtounit(p, "Hill Fort");
        trForbidProtounit(p, "Tower");
        trForbidProtounit(p, "Longhouse");

        trQuestVarSet("p"+p+"attackRange", 12);
        trQuestVarSet("p"+p+"attackSpeed", 1);
        trQuestVarSet("p"+p+"spellRange", 1);
        trQuestVarSet("p"+p+"spellDamage", 1);
        trQuestVarSet("p"+p+"spellDuration", 1);
    }

    xsEnableRule("setup_enemies");
    xsEnableRule("data_load_00");
    xsDisableSelf();
}

rule no_extra_resources
active
highFrequency
{
    if (trTime() > cActivationTime) {
        for (p=1; <cNumberPlayers) {
            trPlayerGrantResources(p, "Food", -1000.0);
            trPlayerGrantResources(p, "Wood", -1000.0);
            trPlayerGrantResources(p, "Gold", -1000.0);
            trPlayerGrantResources(p, "Favor", -1000.0);
        }
        xsDisableSelf();
    }
}

rule Z_cin_00
inactive
highFrequency
{
    if (trTime() > cActivationTime + 1) {
        trSetFogAndBlackmap(false, false);
        /* logo */
        trCameraCut(vector(96,90,98), vector(0,-1,0), vector(0,0,1), vector(1,0,0));
        int posX = 96 - 10;
        int posZ = 96 + 10;
        trModifyProtounit("Curse SFX", 1, 8, 8);
        trArmyDispatch("1,0", "Dwarf",1,posX,0,posZ,0,true);
        for(x=10; >0) {
            posX = posX + 2;
            trArmyDispatch("1,0", "Dwarf",1,posX,0,posZ,0,false);
        }
        for(x=5; >0) {
            posX = posX - 2;
            posZ = posZ - 2;
            trArmyDispatch("1,0", "Dwarf",1,posX,0,posZ,0,false);
        }
        for (x=4; >0) {
            posX = posX + 2;
            posZ = posZ - 2;
            trArmyDispatch("1,0", "Dwarf",1,posX,0,posZ,0,false);
        }
        trArmySelect("1,0");
        trUnitChangeProtoUnit("Curse SFX");

        trUIFadeToColor(0,0,0,500,0,false);
        trSoundPlayFN("cinematics\15_in\gong.wav", "4", -1, "","");
        trSoundPlayFN("godpower.wav", "6", -1, "","");
        trOverlayText("Zenophobia presents...", 4.0, -1, -1, -1);

        xsEnableRule("Z_cin_01");
        xsDisableSelf();
    }
}

rule Z_cin_01
inactive
highFrequency
{
    if (trTime() > cActivationTime + 5) {
        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
        trOverlayText("Spellcrafters", 3.0, -1, -1, -1);
        trUIFadeToColor(0,0,0,1000,3000,true);
        xsEnableRule("Z_cin_02");
        xsDisableSelf();
    }
}

rule Z_cin_02
inactive
highFrequency
{
    if (trTime() > cActivationTime + 5) {
        if (trQuestVarGet("p1progress") == 0) {
            trQuestVarSet("stage", 1);
        } else {
            trLetterBox(false);
            trSetLighting("default", 0.1);
            trMusicPlay("cinematics\9_in\music.mp3", "1", 0.5);
            trUIFadeToColor(0,0,0,1000,0,false);
            trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
            trQuestVarSet("chooser", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Athena",1,96,0,90,0,true);
            trMessageSetText("Host: Choose a stage to challenge.",-1);

            int posX = 96 - 2 * trQuestVarGet("p1progress");

            for(x=0; <= trQuestVarGet("p1progress")) {
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0","Flag Numbered",1,posX,0,100,0,true);
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trUnitSetAnimationPath(""+x+",0,0,0,0,0,0");
                yAddToDatabase("stageChoices", "next");
                posX = posX + 4;
            }

            xsEnableRule("choose_stage_00");
            xsDisableSelf();
        }
        xsEnableRule("choose_stage_01");
    }
}

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
    }
}

void buildRoom(int x = 0, int z = 0, int type = 0) {
    int room = x + 4 * z;
    switch(type)
    {
        case ROOM_BASIC:
        {
            for (i=2; >0) {
                trQuestVarSetFromRand("x0", x * 35 + 5, x * 35 + 18, true);
                trQuestVarSetFromRand("z0", z * 35 + 5, z * 35 + 18, true);
                trQuestVarSetFromRand("x1", x * 35 + 22, x * 35 + 35, true);
                trQuestVarSetFromRand("z1", z * 35 + 22, z * 35 + 35, true);
                int x0 = trQuestVarGet("x0");
                int x1 = trQuestVarGet("x1");
                int z0 = trQuestVarGet("z0");
                int z1 = trQuestVarGet("z1");
                trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
                trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);
                trVectorQuestVarSet("room"+room+"center"+i, xsVectorSet(x0 + x1,0,z0 + z1));
                trVectorQuestVarSet("room"+room+"bottom"+i, xsVectorSet(x0,0,z0));
            }
        }
    }
}

int edgeName(int first = 0, int second = 0) {
    return(16 * xsMin(first, second) + xsMax(first, second));
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
        trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
        trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);
        trQuestVarSet("edge"+edge, type);
    }
}


rule choose_stage_01
inactive
highFrequency
{
    if (trQuestVarGet("stage") > 0) {
        xsDisableSelf();
        /*
        trLetterBox(true);
        trUIFadeToColor(0,0,0,0,0,true);
        */
        trSoundPlayFN("default","1",-1,"Building stage:0","");
        /* minecraft time! */
        switch(1*trQuestVarGet("stage"))
        {
            case 1:
            {
                /* desert tomb */
                TERRAIN_WALL = 2;
                TERRAIN_SUB_WALL = 2;

                TERRAIN_PRIMARY = 0;
                TERRAIN_SUB_PRIMARY = 34;

                TERRAIN_SECONDARY = 0;
                TERRAIN_SUB_SECONDARY = 64;
            }
        }

        /* paint entire map cliff and raise it */
        trChangeTerrainHeight(0,0,150,150,10,false);
        trPaintTerrain(0,0,150,150,TERRAIN_WALL, TERRAIN_SUB_WALL,false);

        /* build the grid */
        int x = 0;
        int z = 0;
        int n = 0;
        int total = 0;
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
        while(total < 15) {
            trQuestVarSetFromRand("search", 1, 5, true);
            yDatabasePointerDefault("frontier");
            for(j=trQuestVarGet("search"); >0) {
                yDatabaseNext("frontier");
            }
            if (trQuestVarGet("tile"+1*trQuestVarGet("frontier")) < TILE_VISITED) {
                z = 1*trQuestVarGet("frontier") / 4;
                x = 1*trQuestVarGet("frontier") - 4 * z;
                buildEdge(1*yGetVar("frontier", "edge"), EDGE_NORMAL);
                trQuestVarSet("tile"+1*trQuestVarGet("frontier"), TILE_VISITED);
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
                total = total + 1;
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
                trQuestVarGet("newX") > 3 || trQuestVarGet("newZ") > 3) {
                continue;
            } else {
                n = 0 + trQuestVarGet("newX") + 4 * trQuestVarGet("newZ");
                buildEdge(edgeName(1*trQuestVarGet("first"), n), EDGE_NORMAL);
            }
        }

        for (i=1; < 15) {
            z = i / 4;
            x = i - z * 4;
            buildRoom(x, z, ROOM_BASIC);
        }

        /* paint tiny square at bottom of map for spawning units */
        trPaintTerrain(0,0,3,3,0,70,true);
        trPaintTerrain(0,0,3,3,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
    }
}