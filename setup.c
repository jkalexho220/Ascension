int ENEMY_PLAYER = 0;
bool Multiplayer = false;

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
    

    /* Transport Ship max contained */
    trModifyProtounit("Transport Ship Atlantean", 1, 5, -11);
    for(p=1; < ENEMY_PLAYER) {
        /* pop count */
        trModifyProtounit("Vision Revealer", p, 7, 9999);
        trModifyProtounit("Vision Revealer", p, 2, -99);

        trModifyProtounit("Vision SFX", p, 0, -9999);

        trModifyProtounit("Dwarf", p, 55, 4);

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
        trSoundPlayFN("market.wav","1",-1,"","");
        trOverlayText("Battle Gambit", 3.0, -1, -1, -1);
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
        trMessageSetText("Host: Delete statue to skip tutorial.",-1);
        trCounterAddTime("counter", 8, 1, "Tutorial Starting", -1);
        xsEnableRule("tutorial_00_decide");
        xsDisableSelf();
    }
}