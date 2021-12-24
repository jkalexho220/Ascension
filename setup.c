const int TILE_NOT_FOUND = 0;
const int TILE_FOUND = 1;
const int TILE_VISITED = 2;

const int EDGE_NOT_FOUND = 0;
const int EDGE_NORMAL = 1;
const int EDGE_CHASM = 2;

const int ROOM_BASIC = 0;
const int ROOM_AMBUSH = 1;
const int ROOM_CHEST_KEY = 2;
const int ROOM_TRANSPORTER_GUY = 9;
const int ROOM_STARTER = 10;
const int ROOM_BOSS = 11;
const int ROOM_NICK = 12;

const int CHEST_KEY = 1;
const int CHEST_PADS = 2;
const int CHEST_ENCOUNTER = 3;

const int MOONBLADE = 1;
const int SUNBOW = 2;
const int CLASS_COUNT = 2;

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

void setupPlayerProto(string proto = "", float health = 0, float attack = 0, float speed = 4, float range = 0) {
    int pNum = kbGetProtoUnitID(proto);
    for(p=0; <ENEMY_PLAYER) {
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
        trQuestVarSet("proto"+pNum+"attack", attack);
        /* projectiles */
        zInitProtoUnitStat(proto, p, 13, 1);
        /* health */
        trModifyProtounit(proto, p, 0, 9999999999999999999.0);
        trModifyProtounit(proto, p, 0, -9999999999999999999.0);
        trModifyProtounit(proto, p, 0, health);
        zInitProtoUnitStat(proto, p, 0, health);
        trQuestVarSet("proto"+pNum+"health", health);
        /* LOS */
        trModifyProtounit(proto, p, 2, 9999999999999999999.0);
        trModifyProtounit(proto, p, 2, -9999999999999999999.0);
        trModifyProtounit(proto, p, 2, 20);
        /* range */
        trModifyProtounit(proto, p, 11, 9999999999999999999.0);
        trModifyProtounit(proto, p, 11, -9999999999999999999.0);
        trModifyProtounit(proto, p, 11, range);
        zInitProtoUnitStat(proto, p, 11, range);
        trQuestVarSet("proto"+pNum+"range", range);
        /* speed */
        trModifyProtounit(proto, p, 1, 9999999999999999999.0);
        trModifyProtounit(proto, p, 1, -9999999999999999999.0);
        trModifyProtounit(proto, p, 1, speed);
        zInitProtoUnitStat(proto, p, 1, speed);
        trQuestVarSet("proto"+pNum+"speed", speed);
        /* armor */
        trModifyProtounit(proto, p, 24, -1);
        trModifyProtounit(proto, p, 25, -1);
        trModifyProtounit(proto, p, 26, -1);
    }
    trModifyProtounit(proto, 0, 2, -20);
}

void setupClass(string proto = "", int class = 0, int firstDelay = 0, int nextDelay = 0, int specialCD = 0) {
    int p = kbGetProtoUnitID(proto);
    trQuestVarSet("class"+class+"proto", p);
    trQuestVarSet("proto"+p+"class", class);
    trQuestVarSet("class"+class+"firstDelay", firstDelay);
    trQuestVarSet("class"+class+"nextDelay", nextDelay);
    trQuestVarSet("class"+class+"specialAttackCooldown", specialCD);
}

void chooseClass(int p = 0, int class = 0) {
    trQuestVarSet("p"+p+"class", class);
    trEventFire(1000 + 12 * class + p);
    int proto = trQuestVarGet("class"+class+"proto");
    trQuestVarSet("p"+p+"health", trQuestVarGet("proto"+proto+"health"));
    trQuestVarSet("p"+p+"attack", trQuestVarGet("proto"+proto+"attack"));
    trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("proto"+proto+"attack"));
    trQuestVarSet("p"+p+"range", trQuestVarGet("proto"+proto+"range"));
    trQuestVarSet("p"+p+"speed", trQuestVarGet("proto"+proto+"speed"));
    trQuestVarSet("p"+p+"firstDelay", trQuestVarGet("class"+class+"firstDelay"));
    trQuestVarSet("p"+p+"nextDelay", trQuestVarGet("class"+class+"nextDelay"));
    trQuestVarSet("p"+p+"specialAttackCooldown", trQuestVarGet("class"+class+"specialAttackCooldown"));
    trQuestVarSet("p"+p+"projectiles", 1);
    trQuestVarSet("p"+p+"spellRange", 1);
    trQuestVarSet("p"+p+"spellDamage", 1);
    trQuestVarSet("p"+p+"spellDuration", 1);
    trQuestVarSet("p"+p+"cooldownReduction", 1);
    trUnitSelectClear();
    trUnitSelectByQV("p"+p+"unit");
    if (trUnitAlive()) {
        trMutateSelected(proto);
    }
    trPlayerKillAllGodPowers(p);
    trCounterAbort("lure");
    trCounterAbort("well");
    trCounterAbort("rain");
    if (class > 0) {
        trQuestVarSet("p"+p+"wellCooldownStatus", 2);
        trQuestVarSet("p"+p+"lureCooldownStatus", 2);
        trQuestVarSet("p"+p+"rainCooldownStatus", 2);
    }
}

rule setup
active
runImmediately
{
    Multiplayer = aiIsMultiplayer();
    trSetUnitIdleProcessing(false); 
    
    trSetObscuredUnits(false);
    configUndef("ErodeBuildingFoundations");

    ENEMY_PLAYER = cNumberPlayers - 1;
    trQuestVarSet("activePlayerCount", ENEMY_PLAYER - 1);

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

    for(p=1; < ENEMY_PLAYER) {
        trPlayerSetDiplomacy(p, 0, "neutral");
        trSetCivAndCulture(p, 1, 0);
        /* LOS and flying */
        trModifyProtounit("Animal Attractor", p, 2, -99);
        trModifyProtounit("Animal Attractor", p, 55, 4);
        trModifyProtounit("Tunnel", p, 2, -99);
        trModifyProtounit("Tunnel", p, 55, 4);
        /* carry capacity */
        trModifyProtounit("Ajax", p, 5, 99);
        /* health */
        trModifyProtounit("Vision SFX", p, 0, -9999);
        /* flying */
        trModifyProtounit("Dwarf", p, 55, 4);
        trModifyProtounit("Athena", p, 55, 4);

        trModifyProtounit("Sea Turtle", p, 55, 1);
        trModifyProtounit("Pegasus", p, 55, 1);

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
    }

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

        setupPlayerProto("Militia", 100, 10, 4.8);
        setupPlayerProto("Wolf", 200, 10, 5);
        setupPlayerProto("Hero Greek Theseus", 1000, 50, 4.3);
        setupPlayerProto("Hero Greek Hippolyta", 1000, 50, 4.3, 16);
        setupPlayerProto("Royal Guard Hero", 1200, 30, 4.6);

        xsEnableRule("setup_enemies");
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
            trQuestVarSet("newPlayers", 1);
            trQuestVarSet("stage", 1);
        } else {
            trLetterBox(false);
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
        }
        trSetLighting("default", 0.1);
        xsDisableSelf();
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

/*
input values are grid positions
*/
void paintRelicEdge(int x1 = 0 , int z1 = 0, int x2 = 0, int z2 = 0) {
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
    spawnRelic(x0, z0);
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
            xsEnableRule("relic_transporter_guy_always");
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
                TERRAIN_SUB_WALL = 2;
                
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

        trQuestVarSetFromRand("relicTransporterGuy", 1, 14, true);

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
            if (i == trQuestVarGet("relicTransporterGuy")) {
                buildRoom(x, z, ROOM_TRANSPORTER_GUY);
            } else if (nick && (countRoomEntrances(x, z) == 1)) {
                buildRoom(x, z, ROOM_NICK);
                nick = false;
                xsEnableRule("nick_00_visit");
            } else {
                trQuestVarSetFromRand("roomType", ROOM_BASIC, ROOM_AMBUSH, true);
                trQuestVarSetFromRand("roomType2", ROOM_BASIC, ROOM_AMBUSH, true);
                if (trQuestVarGet("roomType2") < trQuestVarGet("roomType")) {
                    trQuestVarSet("roomType", trQuestVarGet("roomType2"));
                }
                buildRoom(x, z, 1*trQuestVarGet("roomType"));
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