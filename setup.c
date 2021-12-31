const int TILE_NOT_FOUND = 0;
const int TILE_FOUND = 1;
const int TILE_VISITED = 2;

const int EDGE_NOT_FOUND = 0;
const int EDGE_NORMAL = 1;
const int EDGE_BIG = 2;

const int ROOM_BASIC = 0;
const int ROOM_AMBUSH = 1;
const int ROOM_CHEST = 2;
const int ROOM_TRANSPORTER_GUY = 9;
const int ROOM_STARTER = 10;
const int ROOM_BOSS = 11;
const int ROOM_NICK = 12;

const int ROOM_CIRCLE = 0;
const int ROOM_SQUARE = 1;

const int CHEST_KEY = 1;
const int CHEST_ENCOUNTER = 2;
const int CHEST_PADS = 3;

const int MOONBLADE = 1;
const int SUNBOW = 2;
const int THUNDERRIDER = 3;
const int FIREKNIGHT = 4;
const int FROSTKNIGHT = 5;
const int CLASS_COUNT = 5;

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
        zInitProtoUnitStat(proto, p, 2, 20);
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
    trQuestVarSet("p"+p+"los", 20);
    trQuestVarSet("p"+p+"projectiles", 1);
    trQuestVarSet("p"+p+"spellRange", 1);
    trQuestVarSet("p"+p+"spellDamage", 1);
    trQuestVarSet("p"+p+"spellDuration", 1);
    trQuestVarSet("p"+p+"healBoost", 1);

    trQuestVarSet("p"+p+"ultimateCost", 1);
    trQuestVarSet("p"+p+"cooldownReduction", 1);
    trQuestVarSet("p"+p+"stunResistance", 1);
    trQuestVarSet("p"+p+"poisonResistance", 1);
    trQuestVarSet("p"+p+"silenceResistance", 1);

    trQuestVarSet("p"+p+"ultimateCostCount", 0);
    trQuestVarSet("p"+p+"cooldownReductionCount", 0);
    trQuestVarSet("p"+p+"stunResistanceCount", 0);
    trQuestVarSet("p"+p+"poisonResistanceCount", 0);
    trQuestVarSet("p"+p+"silenceResistanceCount", 0);

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

    for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
        yDatabaseNext("p"+p+"relics");
        relicEffect(p, 1*yGetVar("p"+p+"relics", "type"), true);
    }

    if (Multiplayer == false) {
        trQuestVarSet("p"+p+"level", trQuestVarGet("class"+class+"level"));
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

    if (Multiplayer) {
        ENEMY_PLAYER = cNumberPlayers - 1;
    } else {
        ENEMY_PLAYER = 2;
    }
    
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
    setupClass("Hero Greek Atalanta", THUNDERRIDER, 630, 1400, 5);
    setupClass("Lancer Hero", FIREKNIGHT, 1155, 1500, 5);
    setupClass("Hero Greek Achilles", FROSTKNIGHT, 470, 1000, 5);

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
        trModifyProtounit("Transport Ship Greek", p, 55, 4);
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

        setupPlayerProto("Kronny Flying", 1000, 0, 0);

        setupPlayerProto("Militia", 100, 10, 4.8);
        setupPlayerProto("Wolf", 200, 10, 5);
        setupPlayerProto("Hero Greek Theseus", 1000, 50, 4.3);
        setupPlayerProto("Hero Greek Hippolyta", 1000, 50, 4.3, 16);
        setupPlayerProto("Hero Greek Atalanta", 800, 30, 6.0);
        setupPlayerProto("Lancer Hero", 1100, 55, 6.05);
        setupPlayerProto("Hero Greek Achilles", 1100, 45, 5.5);

        for(p=1; < ENEMY_PLAYER) {
            trModifyProtounit("Attack Revealer", p, 2, 9999999999999999999.0);
            trModifyProtounit("Attack Revealer", p, 2, -9999999999999999999.0);
            zInitProtoUnitStat("Attack Revealer", p, 2, 0);
        }

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
        if (Multiplayer == false) {
            trPlayerGrantResources(1, "Gold", trQuestVarGet("p1gold"));
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
                yAddUpdateVar("stageChoices", "stage", x + 1);
                posX = posX + 4;
            }

            xsEnableRule("choose_stage_00");
        }
        trSetLighting("default", 0.1);
        xsDisableSelf();
        xsEnableRule("choose_stage_01");
    }
}