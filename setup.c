const int CIV_GREEK = 0;
const int CIV_EGYPTIAN = 1;
const int CIV_NORSE = 2;
const int CIV_ATLANTEAN = 3;

const int ITEM_REINFORCEMENTS = 0;
const int ITEM_SON_OF_OSIRIS = 1;
const int ITEM_CALL_TO_ARMS = 2;
const int ITEM_BOLT = 3;
const int ITEM_NIDHOGG = 4;
const int ITEM_TRAITOR = 5;
const int ITEM_CARNIVORA = 6;
const int ITEM_FROST = 7;
const int ITEM_ANCESTORS = 8;


const int ITEM_COUNT = 8;

rule setup
active
runImmediately
{
    trSetUnitIdleProcessing(false); 
    trUIFadeToColor(0,0,0,0,0,true);
    trSetObscuredUnits(false);

    trQuestVarSetFromRand("civ", 0, 3, true);

    switch(1*trQuestVarGet("civ"))
    {
        case CIV_GREEK:
        {
            trStringQuestVarSet("unit1", "Hypaspist");
            trStringQuestVarSet("unit2", "Toxotes");
            trStringQuestVarSet("unit3", "Hippikon");
            trStringQuestVarSet("unit4", "Crossbowman");
            trStringQuestVarSet("unit5", "Cyclops");
            trStringQuestVarSet("unit6", "Centaur");
            trStringQuestVarSet("unit7", "Manticore");
            trStringQuestVarSet("unit8", "Nemean Lion");
            trStringQuestVarSet("unit9", "Colossus");
            trStringQuestVarSet("unit10", "Chimera");
            trStringQuestVarSet("unit11", "Hero Greek Polyphemus");
            trStringQuestVarSet("unit12", "Hero Greek Bellerophon");
        }
        case CIV_EGYPTIAN:
        {
            trStringQuestVarSet("unit1", "Spearman");
            trStringQuestVarSet("unit2", "Chariot Archer");
            trStringQuestVarSet("unit3", "Camelry");
            trStringQuestVarSet("unit4", "Anubite");
            trStringQuestVarSet("unit5", "War Elephant");
            trStringQuestVarSet("unit6", "Sphinx");
            trStringQuestVarSet("unit7", "Wadjet");
            trStringQuestVarSet("unit8", "Scorpion Man");
            trStringQuestVarSet("unit9", "Petsuchos");
            trStringQuestVarSet("unit10", "Avenger");
            trStringQuestVarSet("unit11", "Mummy");
            trStringQuestVarSet("unit12", "Pharaoh of Osiris");
        }
        case CIV_NORSE:
        {
            trStringQuestVarSet("unit1", "Ulfsark");
            trStringQuestVarSet("unit2", "Throwing Axeman");
            trStringQuestVarSet("unit3", "Raiding Cavalry");
            trStringQuestVarSet("unit4", "Bogsveigir");
            trStringQuestVarSet("unit5", "Einheriar");
            trStringQuestVarSet("unit6", "Troll");
            trStringQuestVarSet("unit7", "Valkyrie");
            trStringQuestVarSet("unit8", "Fenris Wolf");
            trStringQuestVarSet("unit9", "Battle Boar");
            trStringQuestVarSet("unit10", "Jarl");
            trStringQuestVarSet("unit11", "Frost Giant");
            trStringQuestVarSet("unit12", "Fire Giant");
        }
        case CIV_ATLANTEAN:
        {
            trStringQuestVarSet("unit1", "Swordsman");
            trStringQuestVarSet("unit2", "Archer Atlantean");
            trStringQuestVarSet("unit3", "Lancer");
            trStringQuestVarSet("unit4", "Automaton SPC");
            trStringQuestVarSet("unit5", "Javelin Cavalry Hero");
            trStringQuestVarSet("unit6", "Dryad");
            trStringQuestVarSet("unit7", "Trident Soldier Hero");
            trStringQuestVarSet("unit8", "Royal Guard Hero");
            trStringQuestVarSet("unit9", "Promethean");
            trStringQuestVarSet("unit10", "Satyr");
            trStringQuestVarSet("unit11", "Argus");
            trStringQuestVarSet("unit12", "Lampades");
        }
    }

    trForceNonCinematicModels(true);
    ambientColor(0,0,0);
    sunColor(0,0,0);
    trLetterBox(true);

    float angle = 0;
    float angleStep = 2.0 * 3.141592 / cNumberPlayers;
    float posX = 0;
    float posZ = 0;

    /* Center revealer */
    trModifyProtounit("Revealer", 0, 2, 24);

    trVectorQuestVarSet("center", xsVectorSet(96,0,96));
    trQuestVarSet("center", trGetNextUnitScenarioNameNumber());
    trArmyDispatch("1,0","Pegasus",1,96,0,96,0,true);
    trUnitSelectClear();
    trUnitSelect(""+1*trQuestVarGet("center"), true);
    trUnitConvert(0);
    trUnitChangeProtoUnit("Revealer");
    
    for(p=1; <cNumberPlayers) {
        trSetCivilizationNameOverride(p, "Wins: 0");
        angle = angle + angleStep;
        /* Remove LOS for all units except Revealers */
        for(x=931;>0) {
            trModifyProtounit(kbGetProtoUnitName(x), p, 2, -99);
        }

        /* LOS for summoned units */
        trModifyProtounit("Minion", p, 2, 99);
        trModifyProtounit("Audrey", p, 2, 99);

        /* pop count */
        trModifyProtounit("Folstag Flag Bearer", p, 6, -2);
        trModifyProtounit("Pharaoh of Osiris", p, 6, 9);
        trModifyProtounit("Dryad", p, 6, 3);
        trModifyProtounit("Minion", p, 6, 1);
        trModifyProtounit("Transport Ship Atlantean", p, 6, -3);

        trModifyProtounit("Revealer to Player", p, 2, 105);

        /* Transport Ship max contained */
        trModifyProtounit("Transport Ship Atlantean", p, 5, 987);

        /* Speed tweaks */
        trModifyProtounit("Great Box Cart", p, 1, -0.75);
        trModifyProtounit("Pharaoh of Osiris", p, 1, 0.5);

        posX = xsSin(angle) * 82.56 + 96;
        posZ = xsCos(angle) * 82.56 + 96;
        trVectorQuestVarSet("p"+p+"island", xsVectorSet(posX,0,posZ));

        int next = trGetNextUnitScenarioNameNumber();
        trArmyDispatch(""+p+",0","Pegasus",1,posX,0,posZ,true);
        trUnitSelectClear();
        trUnitSelect(""+next, true);
        trUnitChangeProtoUnit("Revealer To Player");

        trQuestVarSet("p"+p+"victoryMarker", trGetNextUnitScenarioNameNumber());
        trArmyDispatch(""+p+",0","Victory Marker",1,posX,0,posZ,0,true);
        trUnitSelectClear();
        trUnitSelect(""+1*trQuestVarGet("p"+p+"victoryMarker"), true);
        trUnitChangeProtoUnit("Victory Marker");

        trQuestVarSet("p"+p+"boat", trGetNextUnitScenarioNameNumber());
        trArmyDispatch(""+p+",0","Pegasus",1,posX,0,posZ,180,true);
        trUnitSelectClear();
        trUnitSelect(""+1*trQuestVarGet("p"+p+"boat"), true);
        trUnitChangeProtoUnit("Transport Ship Atlantean");

        trQuestVarSet("start", trGetNextUnitScenarioNameNumber());
        trArmyDispatch(""+p+",0", trStringQuestVarGet("unit1"),8,posX-12,0,posZ,180,true);
        trArmyDispatch(""+p+",0", trStringQuestVarGet("unit2"),8,posX+12,0,posZ,180,true);
        trArmyDispatch(""+p+",0", trStringQuestVarGet("unit3"),8,posX,0,posZ-12,180,true);
        trArmyDispatch(""+p+",0", "Pharaoh of Osiris",1,posX,0,posZ+12,180,true);
        for(x=trQuestVarGet("start"); < trGetNextUnitScenarioNameNumber()) {
            if (kbGetBlockID(""+x) > 0) {
                trQuestVarSet("next", x);
                yAddToDatabase("p"+p+"units", "next");
            }
        }

        trPlayerKillAllGodPowers(p);

        trPlayerTechTreeEnabledGodPowers(p, false);

        trForbidProtounit(p, "Swordsman Hero");
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

        trSetCivAndCulture(p, 3*trQuestVarGet("civ"), 1*trQuestVarGet("civ"));

        /* heavy all and iron all */
        trTechSetStatus(p, 476, 4);
        trTechSetStatus(p, 479, 4);

        trQuestVarSet("leaderboard"+p, p);
        trQuestVarSet("p"+p+"score", 0);
    }

    /* Shop Revealer */
    trModifyProtounit("Revealer", 1, 2, 99 + 6);
    posX = 96;
    posZ = 82 + 96;
    trArmyDispatch("1,0","Revealer",1,posX,0,posZ,0,true);

    xsEnableRule("Z_cin_00");
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


rule tutorial_00_decide
inactive
highFrequency
{
    trUnitSelectClear();
    trUnitSelect(""+1*trQuestVarGet("statue"), true);
    if (trUnitAlive() == false) {
        /* Skip */
        trUIFadeToColor(0,0,0,1000,0,true);
        xsEnableRule("round_00_start");
        trQuestVarSet("timeNext", trTime() + 2);
        trCounterAbort("counter");
        xsDisableSelf();
        trMusicPlayCurrent();
    } else if (trTime() > cActivationTime + 7) {
        /* Watch */
        trQuestVarSet("cinNext", trTime() + 2);
        trQuestVarSet("cinStep", 1);
        trUnitChangeProtoUnit("Hero Birth");
        trForceNonCinematicModels(true);
        trLetterBox(true);
        trUIFadeToColor(0,0,0,1000,0,true);
        xsEnableRule("tutorial_01_play");
        xsDisableSelf();
    }
}

void zenoDialog(string text="", int duration = 1) {
    trSoundPlayFN("default","1",-1, "Zenophobia:" + text, "icons\infantry g hoplite icon 64");
    trQuestVarSet("cinNext", trTime() + duration);
}

rule tutorial_01_play
inactive
highFrequency
{
    if (trTime() > trQuestVarGet("cinNext")) {
        switch(1*trQuestVarGet("cinStep"))
        {
            case 1:
            {
                trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                trSetFogAndBlackmap(false, false);
                trUIFadeToColor(0,0,0,1000,0,false);
                zenoDialog("Welcome to Battle Gambit!", 3);
                trMusicPlay("music\culture\n. d. nile.mp3", "1", 0.1);
            }
            case 2:
            {
                zenoDialog("Battle Gambit is played in a series of rounds.", 4);
            }
            case 3:
            {
                zenoDialog("At the start of each round, a random reward is displayed.", 5);
                trQuestVarSet("sphinxStart", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,1", "Sphinx",3,96,0,96,180,true);
                trArmySelect("1,1");
                trUnitConvert(0);
                trUnitHighlight(5.0, true);
                trQuestVarSet("sphinxEnd", trGetNextUnitScenarioNameNumber());
            }
            case 4:
            {
                zenoDialog("If you want the reward, you can choose to fight for it by sending out units.", 6);
                trackInsert();
                trackEditWaypoint();
                trackPlay(1000,-1);
                trackAddWaypoint();
                trVectorSetUnitPos("camera", "p1boat");
                trArmyDispatch("1,0", "Spearman", 4, trQuestVarGet("cameraX"),0,trQuestVarGet("cameraZ") - 10,0,true);
                trQuestVarSet("cameraZ", trQuestVarGet("cameraZ") - 70);
                trQuestVarSet("cameraY", 70);
                trCameraCut(trVectorQuestVarGet("camera"), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                trackEditWaypoint();
                trackPlay(1000,-1);
            }
            case 5:
            {
                zenoDialog("To do so, garrison units of your choice in your Transport Ship.", 5);
                for (x=trQuestVarGet("sphinxStart"); < trQuestVarGet("sphinxEnd")) {
                    trUnitSelectClear();
                    trUnitSelect(""+x);
                    trUnitDestroy();
                }
                trArmySelect("1,0");
                trUnitDoWorkOnUnit(""+1*trQuestVarGet("p1boat"));
            }
            case 6:
            {
                zenoDialog("After 25 seconds, the Transport Ships will teleport your units to the arena.", 2);
                trackInsert();
                trackEditWaypoint();
                trackPlay(1000,-1);
                trackAddWaypoint();
                trCameraCut(vector(96,100,6), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                trackEditWaypoint();
                trackPlay(1000, -1);
                trArmySelect("1,0");
                trUnitDestroy();
                trQuestVarSet("flag", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0","Dwarf",1, 96,0,126,0,true);
                trArmySelect("1,0");
                trSetSelectedScale(3,3,3);
                trMutateSelected(kbGetProtoUnitID("Waypoint Flag"));
                trQuestVarSet("cart", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", "Great Box Cart",1,96,0,96,0,true);
                trArmySelect("1,0");
                trUnitConvert(0);
                trArmyDispatch("1,0","Spearman",4,96,0,126,0,true);
            }
            case 7:
            {
                trArmySelect("1,0");
                trUnitMoveToUnit(""+1*trQuestVarGet("cart"),false);
                trQuestVarSet("cinNext", trTime() + 4);
            }
            case 8:
            {
                zenoDialog("Whoever captures the Osiris Piece Cart and brings it back to their flag gets the reward!", 5);
                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("cart"), true);
                trUnitMoveToUnit(""+1*trQuestVarGet("flag"), false);
            }
            case 9:
            {
                trackInsert();
                trackEditWaypoint();
                trackPlay(1000,-1);
                trackAddWaypoint();
                trVectorSetUnitPos("camera", "p1boat");
                trQuestVarSet("cameraZ", trQuestVarGet("cameraZ") - 70);
                trQuestVarSet("cameraY", 70);
                trCameraCut(trVectorQuestVarGet("camera"), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                trackEditWaypoint();
                trackPlay(1000,-1);
                trQuestVarSet("cinNext", trTime() + 2);
            }
            case 10:
            {
                trQuestVarSet("cinNext", trTime() + 4);
                trArmySelect("1,0");
                trUnitDestroy();
                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("flag"), true);
                trUnitDestroy();
                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("cart"), true);
                trUnitDestroy();
                trArmyDispatch("1,0","Sphinx",3,trVectorQuestVarGetX("p1island"),0,trVectorQuestVarGetZ("p1island"),180,true);
                trSoundPlayFN("favordump.wav","1",-1,"","");
            }
            case 11:
            {
                zenoDialog("Beware! Units that you send into battle don't come back! Spend wisely!", 5);
            }
            case 12:
            {
                zenoDialog("If you run out of units, don't worry! There is a Comeback Shop", 5);
                trackInsert();
                trackEditWaypoint();
                trackPlay(1000,-1);
                trackAddWaypoint();
                trCameraCut(vector(96, 70, 116), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                trackEditWaypoint();
                trackPlay(1000,-1);
                trArmySelect("1,0");
                trUnitDestroy();
            }
            case 13:
            {
                zenoDialog("Items in this shop cost gold. You earn gold each round and also when your population decreases.", 9);
            }
            case 14:
            {
                trCameraCut(vector(96, 70, 26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
                zenoDialog("The first player to win 7 rounds will win the game! Good luck and have fun!", 4);
            }
            case 15:
            {
                trUIFadeToColor(0,0,0,2000,0,true);
                trQuestVarSet("cinNext", trTime() + 3);
            }
            case 16:
            {
                trSetFogAndBlackmap(true, true);
                trLetterBox(false);
                trMusicPlayCurrent();
                xsEnableRule("round_00_start");
                trQuestVarSet("timeNext", trTime());
                xsDisableSelf();
            }
        }
        trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
    }
}
