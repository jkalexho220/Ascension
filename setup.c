int ENEMY_PLAYER = 0;
bool Multiplayer = false;

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
        trLetterBox(false);
        trSetLighting("default", 0.1);
        trMusicPlay("cinematics\9_in\music.mp3", "1", 0.5);
        trUIFadeToColor(0,0,0,1000,0,false);
        trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
        trQuestVarSet("statue", trGetNextUnitScenarioNameNumber());
        trArmyDispatch("1,0", "Statue of Lightning",1,96,0,96,180,true);
        trMessageSetText("Host: Choose a stage to challenge.",-1);
        xsEnableRule("gameplay_start");
        xsDisableSelf();
    }
}