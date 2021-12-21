const int TILE_NOT_FOUND = 0;
const int TILE_FOUND = 1;
const int TILE_VISITED = 2;

const int EDGE_NOT_FOUND = 0;
const int EDGE_NORMAL = 1;
const int EDGE_CHASM = 2;

const int ROOM_BASIC = 0;
const int ROOM_STARTER = 1;
const int ROOM_BOSS = 2;
const int ROOM_NICK = 3;

const int CHEST_KEY = 1;
const int CHEST_PADS = 2;
const int CHEST_ENCOUNTER = 3;

const int MOONBLADE = 1;
const int SUNBOW = 2;
const int THUNDERCALLER = 3;

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

void setupPlayerProto(string proto = "", float health = 0, float attack = 0, float range = 12, float speed = 4) {
    for(p=1; <ENEMY_PLAYER) {
        /* attack */
        trModifyProtounit(proto, p, 27, 9999999999999999999.0);
        trModifyProtounit(proto, p, 27, -9999999999999999999.0);
        trModifyProtounit(proto, p, 27, attack);
        zInitProtoUnitStat(proto, p, 27, attack);
        trModifyProtounit(proto, p, 30, 9999999999999999999.0);
        trModifyProtounit(proto, p, 30, -9999999999999999999.0);
        trModifyProtounit(proto, p, 31, 9999999999999999999.0);
        trModifyProtounit(proto, p, 31, -9999999999999999999.0);
        trModifyProtounit(proto, p, 31, attack);
        zInitProtoUnitStat(proto, p, 31, attack);
        trQuestVarSet("proto"+proto+"attack", attack);
        /* projectiles */
        zInitProtoUnitStat(proto, p, 13, 1);
        /* health */
        trModifyProtounit(proto, p, 0, 9999999999999999999.0);
        trModifyProtounit(proto, p, 0, -9999999999999999999.0);
        trModifyProtounit(proto, p, 0, health);
        zInitProtoUnitStat(proto, p, 0, health);
        trQuestVarSet("proto"+proto+"health", health);
        /* LOS */
        trModifyProtounit(proto, p, 2, 9999999999999999999.0);
        trModifyProtounit(proto, p, 2, -9999999999999999999.0);
        trModifyProtounit(proto, p, 2, 20);
        /* range */
        trModifyProtounit(proto, p, 11, 9999999999999999999.0);
        trModifyProtounit(proto, p, 11, -9999999999999999999.0);
        trModifyProtounit(proto, p, 11, range);
        zInitProtoUnitStat(proto, p, 11, range);
        trQuestVarSet("proto"+proto+"range", range);
        /* speed */
        trModifyProtounit(proto, p, 1, 9999999999999999999.0);
        trModifyProtounit(proto, p, 1, -9999999999999999999.0);
        trModifyProtounit(proto, p, 1, speed);
        zInitProtoUnitStat(proto, p, 1, speed);
        trQuestVarSet("proto"+proto+"speed", speed);
        /* armor */
        trModifyProtounit(proto, p, 24, -1);
        trModifyProtounit(proto, p, 25, -1);
        trModifyProtounit(proto, p, 26, -1);
    }
}

void setupClass(string proto = "", int class = 0, int firstDelay = 0, int nextDelay = 0, int specialCD = 0) {
    int p = kbGetProtoUnitID(proto);
    trQuestVarSet("class"+class+"proto", p);
    trQuestVarSet("proto"+p+"class", class);
    trQuestVarSet("class"+class+"firstDelay", firstDelay);
    trQuestVarSet("class"+class+"nextDelay", nextDelay);
    trQuestVarSet("class"+class+"specialCooldown", specialCD);
}

void chooseClass(int p = 0, int class = 0) {
    trQuestVarSet("p"+p+"class", class);
    trEventFire(1000 + 12 * class + 9);
    int proto = trQuestVarGet("class"+class+"proto");
    trQuestVarSet("p"+p+"health", trQuestVarGet("proto"+proto+"health"));
    trQuestVarSet("p"+p+"attack", trQuestVarGet("proto"+proto+"attack"));
    trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("proto"+proto+"attack"));
    trQuestVarSet("p"+p+"range", trQuestVarGet("proto"+proto+"range"));
    trQuestVarSet("p"+p+"speed", trQuestVarGet("proto"+proto+"speed"));
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
    
    
    ambientColor(0,0,0);
    sunColor(0,0,0);
    trLetterBox(true);
    trUIFadeToColor(0,0,0,0,0,true);

    modularCounterInit("spyFind", 32);
    modularCounterInit("spyFound", 32);

    /*
    player 0 omniscience
    */
    trTechSetStatus(0, 304, 4);
    aiSet("NoAI", 0);
    aiSet("NoAI", ENEMY_PLAYER);

    setupClass("Militia", 0, 500, 1000);
    /* Proto , Enumeration , First delay , Next delay , special attack cooldown */
    setupClass("Hero Greek Theseus", MOONBLADE, 460, 1000, 7);
    setupClass("Hero Greek Hippolyta", SUNBOW, 1350, 1750);
    setupClass("Royal Guard Hero", THUNDERCALLER, 600, 1000, 6);

    for(p=1; < ENEMY_PLAYER) {
        /* pop count */
        trModifyProtounit("Vision Revealer", p, 7, 9999);
        /* LOS */
        trModifyProtounit("Vision Revealer", p, 2, -99);
        /* carry capacity */
        trModifyProtounit("Ajax", p, 5, 99);
        /* health */
        trModifyProtounit("Vision SFX", p, 0, -9999);
        /* flying */
        trModifyProtounit("Dwarf", p, 55, 4);
        trModifyProtounit("Athena", p, 55, 4);

        trPlayerKillAllGodPowers(p);
        trPlayerTechTreeEnabledGodPowers(p, false);

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
        trQuestVarSet("p"+p+"projectiles", 1);
        trQuestVarSet("p"+p+"spellRange", 1);
        trQuestVarSet("p"+p+"spellDamage", 1);
        trQuestVarSet("p"+p+"spellDuration", 1);
    }

    xsEnableRule("setup_enemies");
    xsEnableRule("delayed_modify");
    xsEnableRule("data_load_00");
    xsDisableSelf();
}

rule delayed_modify
inactive
highFrequency
{
    if (trTime() > cActivationTime) {
        /* Transport Ship max contained */
        trModifyProtounit("Transport Ship Atlantean", 1, 5, 2147483648.0);
        trModifyProtounit("Transport Ship Atlantean", 1, 5, 2147483648.0);
        trModifyProtounit("Transport Ship Atlantean", 1, 5, 1);

        setupPlayerProto("Militia", 100, 10, 0, 4.8);
        setupPlayerProto("Hero Greek Theseus", 1000, 50, 0, 4.3);
        setupPlayerProto("Hero Greek Hippolyta", 1000, 50, 16, 4.3);
        setupPlayerProto("Royal Guard Hero", 1200, 30, 0, 4.6);
        xsDisableSelf();
    }
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
        trOverlayText("Hero Build Fighters 3", 3.0, -1, -1, -1);
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
        int minProgress = 10;
        for (p=1; < ENEMY_PLAYER) {
            minProgress = xsMin(minProgress, trQuestVarGet("p"+p+"progress"));
        }
        if (minProgress == 0) {
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

            int posX = 96 - 2 * minProgress;

            for(x=0; <= minProgress) {
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

void paintEnemies(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
    for(a=x0; < x1) {
        trQuestVarSetFromRand("deploy", 0, 1, false);
        if (trQuestVarGet("deploy") < trQuestVarGet("enemyDensity")) {
            trQuestVarSetFromRand("z", z0, z1, true);
            trQuestVarSet("posX", a);
            trQuestVarSet("posZ", trQuestVarGet("z"));
            if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
                trQuestVarSetFromRand("type", 1, trQuestVarGet("enemyProtoCount"), true);
                trQuestVarSetFromRand("type2", 1, trQuestVarGet("enemyProtoCount"), true);
                if (trQuestVarGet("type2") < trQuestVarGet("type")) {
                    trQuestVarSet("type", trQuestVarGet("type2"));
                }
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("type")),1,
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
                trQuestVarSetFromRand("type", 1, trQuestVarGet("enemyProtoCount"), true);
                trQuestVarSetFromRand("type2", 1, trQuestVarGet("enemyProtoCount"), true);
                if (trQuestVarGet("type2") < trQuestVarGet("type")) {
                    trQuestVarSet("type", trQuestVarGet("type2"));
                }
                trQuestVarSetFromRand("heading", 0, 360, true);
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0",trStringQuestVarGet("enemyProto"+1*trQuestVarGet("type")),1,
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
    switch(type)
    {
        case ROOM_BASIC:
        {
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
                trVectorQuestVarSet("room"+room+"center"+i, xsVectorSet(x0 + x1,0,z0 + z1));
                trVectorQuestVarSet("room"+room+"bottom"+i, xsVectorSet(x0,0,z0));
                paintSecondary(x0, z0, x1, z1);
                paintEyecandy(x0, z0, x1, z1, "tree");
                paintEyecandy(x0, z0, x1, z1, "rock");
                paintEyecandy(x0, z0, x1, z1, "sprite");
                paintEnemies(x0, z0, x1, z1);
            }
            paintColumns(x0, z0, x1, z1);
            trQuestVarSet("room", room);
            yAddToDatabase("basicRooms", "room");
        }
        case ROOM_STARTER:
        {
            trPaintTerrain(x * 35 + 10, z * 35 + 10, x * 35 + 30, z * 35 + 30, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 10, z * 35 + 10, x * 35 + 31, z * 35 + 31, 0, false);
            trVectorQuestVarSet("startPosition", xsVectorSet(x*70 + 40,0,z*70+40));
        }
        case ROOM_BOSS:
        {
            trPaintTerrain(x * 35 + 5, z * 35 + 5, x * 35 + 35, z * 35 + 35, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
            trChangeTerrainHeight(x * 35 + 5, z * 35 + 5, x * 35 + 36, z * 35 + 36, 0, false);
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
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+26,0, 70*z+26,0,true);
            trQuestVarSet("nickShop2", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+26,0, 70*z+54,0,true);
            trQuestVarSet("nickShop3", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+54,0, 70*z+26,0,true);
            trQuestVarSet("nickShop4", trGetNextUnitScenarioNameNumber());
            trArmyDispatch("1,0", "Victory Marker", 1, 70*x+54,0, 70*z+54,0,true);
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
        xsEnableRule("setup_enemies");
        xsDisableSelf();
        /*
        trLetterBox(true);
        trUIFadeToColor(0,0,0,0,0,true);
        */
        trBlockAllSounds();
        trSoundPlayFN("default","1",-1,"Building stage...","");
        /* minecraft time! */
        switch(1*trQuestVarGet("stage"))
        {
            case 1:
            {
                /* desert tomb */
                trOverlayText("Desert Tomb", 3.0, -1, -1, -1);
                TERRAIN_WALL = 2;
                TERRAIN_SUB_WALL = 2;

                TERRAIN_PRIMARY = 0;
                TERRAIN_SUB_PRIMARY = 34;

                TERRAIN_SECONDARY = 0;
                TERRAIN_SUB_SECONDARY = 17;

                trQuestVarSet("treeDensity", 0);
                trQuestVarSet("spriteDensity", 0.3);
                trStringQuestVarSet("spriteProto1", "Rock Sandstone Sprite");
                trStringQuestVarSet("spriteProto2", "Skeleton");
                trStringQuestVarSet("spriteProto3", "Cinematic Scorch");
                trQuestVarSet("rockDensity", 0.1);
                trStringQuestVarSet("rockProto1", "Rock Sandstone Big");
                trStringQuestVarSet("rockProto2", "Rock Sandstone Big");
                trStringQuestVarSet("rockProto3", "Statue Pharaoh");

                trQuestVarSet("enemyDensity", 0.03 * ENEMY_PLAYER);
                trQuestVarSet("enemyProtoCount", 5);
                trStringQuestVarSet("enemyProto1", "Minion");
                trStringQuestVarSet("enemyProto2", "Anubite");
                trStringQuestVarSet("enemyProto3", "Sphinx");
                trStringQuestVarSet("enemyProto4", "Scarab");
                trStringQuestVarSet("enemyProto5", "Mummy");

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
                buildEdge(edgeName(1*trQuestVarGet("first"), n), EDGE_NORMAL);
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
        for(i=1; <=3) {
            trQuestVarSet("chestType"+i, i);
        }
        for(i=3; >0) {
            trQuestVarSetFromRand("swap", 1, i, true);
            trQuestVarSet("temp", trQuestVarGet("chestType"+i));
            trQuestVarSet("chestType"+i, trQuestVarGet("chestType"+1*trQuestVarGet("swap")));
            trQuestVarSet("chestType"+1*trQuestVarGet("swap"), trQuestVarGet("temp"));
        }

        for (i=1; < 15) {
            z = i / 4;
            x = i - z * 4;
            if (chests > 0) {
                trQuestVarSetFromRand("chest", 1, 7, true);
                if (trQuestVarGet("chest") == 1) {
                    chests = chests - 1;
                }
            } else {
                trQuestVarSet("chest", 0);
            }
            if (nick && (countRoomEntrances(x, z) == 1)) {
                buildRoom(x, z, ROOM_NICK);
                nick = false;
                xsEnableRule("nick_00_visit");
            } else {
                buildRoom(x, z, ROOM_BASIC);
            }
        }

        /* 
        paint tiny square at bottom of map for spawning units 
        and then cover it up
        */
        trPaintTerrain(0,0,5,5,0,70,true);
        trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);

        trUnblockAllSounds();
        xsEnableRule("gameplay_start");
        trLetterBox(false);
        trUIFadeToColor(0,0,0,1000,0,false);
    }
}