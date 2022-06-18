const int CHEST_KEY = 1;
const int CHEST_ENCOUNTER = 2;
const int CHEST_STATUES = 3;

const int MOONBLADE = 1;
const int SUNBOW = 2;
const int FIREKNIGHT = 3;
const int THRONESHIELD = 4;

const int STORMPIERCER = 5;
const int BLASTMAGE = 6;
const int THUNDERRIDER = 7;
const int ALCHEMIST = 8;

const int GARDENER = 9;
const int STARSEER = 10;
const int COMMANDO = 11;
const int SPELLSTEALER = 12;

const int FROSTHAMMER = 13;
const int SPARKWITCH = 14;
const int SAVIOR = 15;
const int NIGHTRIDER = 16;

const int CLASS_COUNT = 16;

const int STARSTONE = 0;
const int SOULSTONE = 1;
const int MANASTONE = 2;

const int SWORD_BLADE = 1;
const int SWORD_HILT = 2;
const int SWORD_HANDLE = 3;

float worldHeight = 0;
float wallHeight = 5;
int ENEMY_PLAYER = 0;
bool Multiplayer = false;
bool PvP = false;

bool customContent = false;

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

string gemstoneIcon(int gem = 0) {
	string img = "icons\improvement hands of the pharaoh icons 64";
	switch(gem)
	{
		case STARSTONE:
		{
			img = "icons\improvement focus icons 64";
		}
		case SOULSTONE:
		{
			img = "icons\god power healing spring icon 64";
		}
		case MANASTONE:
		{
			img = "icons\improvement hands of the pharaoh icons 64";
		}
	}
	return(img);
}

string gemstoneName(int gem = 0) {
	string name = "Dust";
	switch(gem)
	{
		case STARSTONE:
		{
			name = "Starstone";
		}
		case SOULSTONE:
		{
			name = "Soulstone";
		}
		case MANASTONE:
		{
			name = "Manastone";
		}
	}
	return(name);
}

string stageName(int stage = 0) {
	string name = "(This stage has not been made yet)";
	switch(stage)
	{
		case 1:
		{
			name = "The Lion's Den";
		}
		case 2:
		{
			name = "Murkwood";
		}
		case 3:
		{
			name = "Frozen Canyon";
		}
		case 4:
		{
			name = "Emerald Fields";
		}
		case 5:
		{
			name = "Miasma Crypt";
		}
		case 6:
		{
			name = "The Factory";
		}
		case 7:
		{
			name = "The Deep";
		}
		case 8:
		{
			name = "The Clouds";
		}
		case 9:
		{
			name = "The Pit";
		}
		case 10:
		{
			name = "The Apex";
		}
		case 0:
		{
			name = "Glory Stadium";
		}
		case 12:
		{
			name = "Gladiator Worlds v" + 1*trQuestVarGet("gladiatorWorldsVersion") + " by nottud";
		}
	}
	return(name);
}

string stageIcon(int stage = 0) {
	string img = "icons\infantry g hoplite icon 64";
	switch(stage)
	{
		case 1:
		{
			img = "ui\ui map watering hole 256x256";
		}
		case 2:
		{
			img = "ui\ui map deep jungle 256x256";
		}
		case 3:
		{
			img = "ui\ui map jotunheim 256x256";
		}
		case 4:
		{
			img = "ui\ui map open fields 256x256";
		}
		case 5:
		{
			img = "ui\ui map river styx 256x256";
		}
		case 6:
		{
			img = "icons\building siege workshop icon 64";
		}
		case 7:
		{
			img = "icons\improvement poseidons secret icons 64";
		}
		case 8:
		{
			img = "icons\improvement safe passage icons 64";
		}
		case 9:
		{
			img = "ui\ui map erebus 256x256";
		}
		case 10:
		{
			img = "ui\ui map missing 256x256";
		}
		case 0:
		{
			img = "icons\god power valor icons 64";
		}
		case 12:
		{
			img = "icons\infantry x oracle hero icons 64";
		}
	}
	return(img);
}

void reselectMyself() {
	uiClearSelection();
	int p = trCurrentPlayer();
	int class = xGetInt(dPlayerData, xPlayerClass, p);
	trackInsert();
	trackAddWaypoint();
	trackAddWaypoint();
	trBlockAllSounds(false);
	uiFindType(kbGetProtoUnitName(xGetInt(dClass, xClassProto, class)));
	trackPlay(1,999);
}


void setupClass(string proto = "", int class = 0, int firstDelay = 0, int nextDelay = 0,int gem = 0,int specialCD = 0) {
	int p = kbGetProtoUnitID(proto);
	xSetPointer(dClass,class);
	xSetInt(dClass,xClassProto,p);
	xSetInt(dClass,xClassFirstDelay,firstDelay);
	xSetInt(dClass,xClassNextDelay,nextDelay);
	xSetInt(dClass,xClassSpecialAttackCooldown,specialCD);
	xSetInt(dClass,xClassGemstone,gem);
	trQuestVarSet("proto"+p+"class", class);
}

void setupPlayerProto(string proto="",float health=0,float attack=0,float speed=4,float armor=0,float range=0) {
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
		trModifyProtounit(proto, p, 24, armor);
		trModifyProtounit(proto, p, 25, armor);
		trModifyProtounit(proto, p, 26, armor);
		trQuestVarSet("proto"+pNum+"armor", armor);
		
		/* damage bonus vs myth */
		trModifyProtounit(proto, p, 33, 9999999999999999999.0);
		trModifyProtounit(proto, p, 33, -9999999999999999999.0);
		trModifyProtounit(proto, p, 33, 1.0);
		trModifyProtounit(proto, p, 44, 9999999999999999999.0);
		trModifyProtounit(proto, p, 44, -9999999999999999999.0);
		trModifyProtounit(proto, p, 44, 1.0);
	}
	trModifyProtounit(proto, 0, 2, -20);
}


void chooseClass(int p = 0, int class = 0) {
	trEventFire(1000 + 12 * class + p);
	int proto = xGetInt(dClass,xClassProto,class);
	xSetPointer(dPlayerData,p);
	xResetValues(dPlayerData, p, xPlayerWellPos);
	xSetInt(dPlayerData, xPlayerClass, class);
	xSetFloat(dPlayerData,xPlayerHealth,trQuestVarGet("proto"+proto+"health"));
	xSetFloat(dPlayerData,xPlayerBaseAttack,trQuestVarGet("proto"+proto+"attack"));
	xSetFloat(dPlayerData,xPlayerBaseAttackTrue,trQuestVarGet("proto"+proto+"attack"));
	xSetFloat(dPlayerData,xPlayerAttack,trQuestVarGet("proto"+proto+"attack"));
	xSetFloat(dPlayerData,xPlayerRange,trQuestVarGet("proto"+proto+"range"));
	xSetFloat(dPlayerData,xPlayerSpeed,trQuestVarGet("proto"+proto+"speed"));
	
	xSetInt(dPlayerData,xPlayerFirstDelay,xGetInt(dClass, xClassFirstDelay, class));
	xSetInt(dPlayerData,xPlayerNextDelay,xGetInt(dClass, xClassNextDelay, class));
	xSetInt(dPlayerData,xPlayerSpecialAttackCooldown,xGetInt(dClass, xClassSpecialAttackCooldown, class));
	
	xSetFloat(dPlayerData,xPlayerPhysicalResist,trQuestVarGet("proto"+proto+"armor"));
	xSetFloat(dPlayerData,xPlayerMagicResist,trQuestVarGet("proto"+proto+"armor"));
	
	zSetProtoUnitStat("Victory Marker", p, 0, xGetFloat(dPlayerData,xPlayerHealth));
	
	trUnitSelectClear();
	trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit),true);
	if (trUnitAlive() && xGetInt(dPlayerData,xPlayerUnit) > 0) {
		trMutateSelected(proto);
	}
	trPlayerKillAllGodPowers(p);
	if (trCurrentPlayer() == p) {
		trCounterAbort("lure");
		trCounterAbort("well");
		trCounterAbort("rain");
	}
	if (class > 0) {
		xSetInt(dPlayerData,xPlayerWellCooldownStatus,1);
		xSetInt(dPlayerData,xPlayerLureCooldownStatus,1);
		xSetInt(dPlayerData,xPlayerRainCooldownStatus,1);
	}
	
	if (Multiplayer == false) {
		xSetInt(dPlayerData,xPlayerLevel,xGetInt(dClass,xClassLevel,class) - 1);
		trSetCivilizationNameOverride(p, "Level " + (1+xGetInt(dPlayerData,xPlayerLevel)));
	}
	
	int relics = getRelicsDB(p);
	for(x=xGetDatabaseCount(relics); >0) {
		xDatabaseNext(relics);
		if ((x > xGetInt(dPlayerData,xPlayerLevel,p)+1) || xGetInt(relics, xRelicType) > NORMAL_RELICS) {
			int index = xAddDatabaseBlock(dFreeRelics);
			xSetInt(dFreeRelics,xRelicName,xGetInt(relics,xRelicName),index);
			xSetInt(dFreeRelics,xRelicType,xGetInt(relics,xRelicType),index);
			xFreeDatabaseBlock(relics);
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dFreeRelics,xRelicName,index),true);
			trUnitChangeProtoUnit("Relic");
			if (xGetInt(relics, xRelicType) == RELIC_NICKONHAWK) {
				trQuestVarSet("p"+p+"nickEquipped", 0);
			}
		} else {
			relicEffect(xGetInt(relics,xRelicType), p, true);
		}
	}
	
	zSetProtoUnitStat("Cinematic Block", p, 0, xGetFloat(dPlayerData,xPlayerHealth));
}

void customContentEnabled(int eventID = -1) {
	customContent = true;
}

rule setup
active
runImmediately
{
	Multiplayer = aiIsMultiplayer();
	trSetUnitIdleProcessing(false);

	gadgetUnreal("ScoreDisplay");

	// If this plays, you got the goods
	trEventSetHandler(998, "customContentEnabled");
	trSoundPlayFN("Zenophobia\Test sound.mp3", "1", 998, "", "");
	
	gadgetUnreal("GodPowers");
	gadgetUnreal("tributedlg-sendButton");
	gadgetUnreal("tributedlg-clearButton");
	
	trSetObscuredUnits(false);
	configUndef("ErodeBuildingFoundations");
	
	trVectorQuestVarSet("zeroVector", vector(0,0,0));
	
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
	
	/*
	player 0 omniscience
	*/
	trTechSetStatus(0, 304, 4);
	trTechSetStatus(ENEMY_PLAYER, 304, 4);
	aiSet("NoAI", 0);
	aiSet("NoAI", ENEMY_PLAYER);
	xsSetContextPlayer(ENEMY_PLAYER);
	aiSetAttackResponseDistance(0.0);
	xsSetContextPlayer(0);
	
	trModifyProtounit("Wonder SPC", ENEMY_PLAYER, 24, 1);
	trModifyProtounit("Wonder SPC", ENEMY_PLAYER, 25, 1);
	trModifyProtounit("Wonder SPC", ENEMY_PLAYER, 26, 1);
	trModifyProtounit("Wonder SPC", ENEMY_PLAYER, 0, -9899);
	
	trModifyProtounit("Dwarf", 0, 55, 4);
	trModifyProtounit("Dwarf", 0, 2, -99);
	trModifyProtounit("Dwarf", ENEMY_PLAYER, 55, 4);
	trModifyProtounit("Scylla", 0, 55, 1);
	trModifyProtounit("Scylla", 1, 55, 1);
	trModifyProtounit("Scylla", ENEMY_PLAYER, 55, 1);
	for(p=1; < ENEMY_PLAYER) {
		trPlayerSetDiplomacy(p, 0, "neutral");
		trPlayerSetDiplomacy(p, ENEMY_PLAYER, "Enemy");
		trPlayerSetDiplomacy(ENEMY_PLAYER, p, "Enemy");
		for(x=p+1; < ENEMY_PLAYER) {
			trPlayerSetDiplomacy(p, x, "ally");
			trPlayerSetDiplomacy(x, p, "ally");
		}
		trSetCivAndCulture(p, 1, 0);
		/* LOS and flying */
		trModifyProtounit("Animal Attractor", p, 2, -99);
		trModifyProtounit("Animal Attractor", p, 55, 4);
		trModifyProtounit("Tunnel", p, 2, -99);
		trModifyProtounit("Tunnel", p, 55, 4);
		/* carry capacity */
		trModifyProtounit("Ajax", p, 5, 99);
		trModifyProtounit("Transport Ship Greek", p, 5, 99);
		/* flying */
		trModifyProtounit("Dwarf", p, 55, 4);
		trModifyProtounit("Dwarf", p, 2, -99);
		trModifyProtounit("Transport Ship Greek", p, 55, 4);
		trModifyProtounit("Athena", p, 55, 4);
		
		trModifyProtounit("Servant", p, 55, 1);
		trModifyProtounit("Phoenix", p, 55, 1);
		trModifyProtounit("Phoenix From Egg", p, 55, 1);
		trModifyProtounit("Sea Turtle", p, 55, 1);
		trModifyProtounit("Pegasus", p, 55, 1);
		trModifyProtounit("Hippocampus", p, 55, 1);
		trModifyProtounit("Junk", p, 55, 1);

		trModifyProtounit("Petosuchus Projectile", p, 55, 2);
		
		trModifyProtounit("Statue of Lightning", p, 1, 3.5);
		
		trPlayerKillAllGodPowers(p);
		trPlayerTechTreeEnabledGodPowers(p, false);
		
		/* hand of the pharaoh */
		trTechSetStatus(p, 471, 4);
		
		/* ox cart stuff */
		trTechSetStatus(p, 9, 0);
		trTechSetStatus(p, 101, 0);
		trTechSetStatus(p, 34, 0);
		trTechSetStatus(p, 35, 0);
		
		trForbidProtounit(p, "Trident Soldier Hero");
		trForbidProtounit(p, "Archer Atlantean Hero");
		trForbidProtounit(p, "Royal Guard Hero");
		trForbidProtounit(p, "Lancer Hero");
		trForbidProtounit(p, "Oracle Hero");
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
		trForbidProtounit(p, "Manor");
		trForbidProtounit(p, "Guild");
	}
	
	trQuestVarSet("rotX0", -1);
	trQuestVarSet("rotX1", 1);
	trQuestVarSet("rotX2", 0);
	trQuestVarSet("rotX3", 0);
	trQuestVarSet("rotZ0", 0);
	trQuestVarSet("rotZ1", 0);
	trQuestVarSet("rotZ2", -1);
	trQuestVarSet("rotZ3", 1);

	trEventSetHandler(10002, "readTheRunestone");
	
	xsEnableRule("data_load_00");
	xsDisableSelf();
}

rule setup_classes
active
highFrequency
{
	%
	for(p=1; < cNumberNonGaiaPlayers) {
		code("trStringQuestVarSet(\"p"+p+"name\", \""+rmGetPlayerName(p)+"\");");
	}
	code("map(\"esc\", \"ShowImageBox\", \"gadgetUnreal("+quote+"ShowImageBox"+quote+")\");");
	%
	/* Proto , Enumeration , First delay , Next delay , special attack cooldown */
	setupClass("Hero Greek Theseus", MOONBLADE, 460, 1000, STARSTONE, 7);
	setupClass("Hero Greek Hippolyta", SUNBOW, 1350, 1750, STARSTONE);
	setupClass("Hero Greek Atalanta", THUNDERRIDER, 630, 1400, MANASTONE, 4);
	setupClass("Lancer Hero", FIREKNIGHT, 1155, 1500, MANASTONE, 5);
	setupClass("Hero Greek Achilles", NIGHTRIDER, 470, 1000, SOULSTONE, 7);
	setupClass("Priest", BLASTMAGE, 500, 800, STARSTONE);
	setupClass("Oracle Hero", STARSEER, 540, 1500, STARSTONE, 8);
	setupClass("Archer Atlantean Hero", STORMPIERCER, 400, 1000, MANASTONE);
	setupClass("Pharaoh", ALCHEMIST, 550, 1200, SOULSTONE);
	setupClass("Swordsman Hero", SPELLSTEALER, 400, 800, MANASTONE, 6);
	setupClass("Javelin Cavalry Hero", COMMANDO, 1000, 2000, STARSTONE);
	setupClass("Trident Soldier Hero", THRONESHIELD, 625, 1250, SOULSTONE, 10);
	setupClass("Hero Greek Bellerophon", SAVIOR, 625, 1250, SOULSTONE, 3);
	setupClass("Hero Greek Chiron", GARDENER, 900, 1500, SOULSTONE);
	setupClass("Circe", SPARKWITCH, 1400, 2800, MANASTONE);
	setupClass("Hero Norse", FROSTHAMMER, 500, 1200, MANASTONE, 5);
	//setupClass("Regent", GAMBLER, 500, 1100, SOULSTONE);
	xsDisableSelf();
}

rule delayed_modify
inactive
highFrequency
{
	zInitProtoUnitStat("Revealer", 1, 2, 12);
	setupPlayerProto("Kronny Flying", 1000, 0, 0);
	
	/* i gotta look good */
	trTechSetStatus(0, 7, 4);
	trTechSetStatus(0, 476, 4);
	trTechSetStatus(ENEMY_PLAYER, 7, 4);
	trTechSetStatus(ENEMY_PLAYER, 476, 4);
	
	setupPlayerProto("Hero Boar", 1000, 0, 4.2, 0);
	setupPlayerProto("Militia", 100, 10, 4.8);
	setupPlayerProto("Prisoner", 100, 10, 4.8);
	setupPlayerProto("Wolf", 200, 10, 5, 0.2);
	setupPlayerProto("Dog", 300, 15, 5, 0.2);
	setupPlayerProto("Minion", 240, 10, 5, 0.2);
	setupPlayerProto("Hero Greek Theseus", 1000, 50, 4.3, 0.3);
	setupPlayerProto("Hero Greek Hippolyta", 1000, 50, 4.3, 0, 16);
	setupPlayerProto("Phoenix From Egg", 1000, 0, 6.3, 0, 0);
	setupPlayerProto("Hero Greek Atalanta", 800, 30, 6.0, 0);
	setupPlayerProto("Lancer Hero", 1100, 55, 6.05, 0.4);
	setupPlayerProto("Hero Greek Achilles", 1200, 50, 5.5, 0.3);
	setupPlayerProto("Archer Atlantean Hero", 900, 30, 4.05, 0, 20);
	setupPlayerProto("Pharaoh", 1000, 50, 4.0, 0, 12);
	setupPlayerProto("Swordsman Hero", 1000, 50, 4.8, 0.3);
	setupPlayerProto("Javelin Cavalry Hero", 1200, 45, 5.3, 0, 12);
	setupPlayerProto("Trident Soldier Hero", 1200, 30, 3.9, 0);
	setupPlayerProto("Hero Greek Bellerophon", 1200, 60, 6.0, 0.4);
	setupPlayerProto("Hero Greek Chiron", 1000, 50, 5.5, 0, 16);
	setupPlayerProto("Priest", 1000, 10, 3.6, 0, 16);
	setupPlayerProto("Oracle Hero", 1000, 0, 4.0, 0.3);
	setupPlayerProto("Circe", 1000, 100, 3.7, 0, 15);
	setupPlayerProto("Audrey", 1000, 50, 0);
	setupPlayerProto("Walking Berry Bush", 500, 25, 3.5, 0.3);
	setupPlayerProto("Hero Norse", 1200, 50, 4.3, 0.2);
	setupPlayerProto("Hippocampus", 1200, 0, 6.0, 0.5);
	
	setupPlayerProto("Villager Atlantean Hero", 500, 0, 4.0);
	setupPlayerProto("Caravan Greek", 500, 0, 4.0);
	
	trModifyProtounit("Sky Passage", 0, 5, 999);
	
	trModifyProtounit("Walking Woods Marsh", ENEMY_PLAYER, 6, 1);
	
	trModifyProtounit("Monument 2", ENEMY_PLAYER, 24, -1);
	trModifyProtounit("Monument 2", ENEMY_PLAYER, 25, -1);
	trModifyProtounit("Monument 2", ENEMY_PLAYER, 26, -1);
	trModifyProtounit("Monument 2", 0, 24, -1);
	trModifyProtounit("Monument 2", 0, 25, -1);
	trModifyProtounit("Monument 2", 0, 26, -1);
	trModifyProtounit("Monument 2", ENEMY_PLAYER, 0, 1000);
	trModifyProtounit("Monument 2", 0, 0, 1000);
	
	trModifyProtounit("Uproot 2x2", 0, 8, -99);
	
	zInitProtoUnitStat("Revealer to Player", 0, 2, 12);
	for(p=ENEMY_PLAYER; >0) {
		trModifyProtounit("Onager", p, 13, -1);
		zInitProtoUnitStat("Revealer to Player", p, 2, 12);
		zSetProtoUnitStat("Revealer to Player", p, 2, 32);
		trModifyProtounit("Revealer to Player", p, 0, 999999);
		trModifyProtounit("Revealer", p, 0, 999999);
		trModifyProtounit("Circe", p, 9, -99);
		trModifyProtounit("Ox Cart", p, 0, 120);
		trModifyProtounit("Spy Eye", p, 2, -99);
		trModifyProtounit("Villager Atlantean Hero", p, 5, 2);
		trModifyProtounit("Pharaoh", p, 15, -999);
		trModifyProtounit("Pharaoh", p, 14, -999);
		trModifyProtounit("Priest", p, 15, -999);
		trModifyProtounit("Priest", p, 14, -999);
		trModifyProtounit("Wadjet Spit", p, 1, -15);
		trModifyProtounit("Ajax", p, 5, 999);
		trModifyProtounit("Ball of Fire Impact", p, 8, 9999999999999999999.0);
		trModifyProtounit("Ball of Fire Impact", p, 8, -9999999999999999999.0);
		trModifyProtounit("Ball of Fire Impact", p, 8, 4.0);
		zInitProtoUnitStat("Ball of Fire Impact", p, 8, 4.0);
		trModifyProtounit("Kronny Flying", p, 1, 9999999999999999999.0);
		trModifyProtounit("Kronny Flying", p, 1, -9999999999999999999.0);
		zInitProtoUnitStat("Kronny Flying", p, 1, 0);
		zInitProtoUnitStat("Meteorite",p,1,100);
		trModifyProtounit("Minion", p, 8, -999);
		trModifyProtounit("Arkantos God Out", p, 8, 1);
		zInitProtoUnitStat("Victory Marker", p, 0, 300);
		zInitProtoUnitStat("Priest Projectile", p, 8, 2);

		trModifyProtounit("Phoenix From Egg", p, 29, 9999999999999999999.0);
		trModifyProtounit("Phoenix From Egg", p, 29, -9999999999999999999.0);
		trModifyProtounit("Phoenix From Egg", p, 29, 0);

		trModifyProtounit("Troll", p, 11, 2); // troll range 16
		
		zInitProtoUnitStat("Ballista Shot", p, 1, 30);
		
		trModifyProtounit("Servant", p, 55, 1);
		trModifyProtounit("Kraken", p, 55, 1);
		trModifyProtounit("Nereid", p, 55, 1);
		trModifyProtounit("Scylla", p, 55, 1);

		trModifyProtounit("Catapult Shot", p, 8, -99);
		trModifyProtounit("Petrobolos Shot", p, 8, -99);
	}

	trModifyProtounit("Shaba Ka", ENEMY_PLAYER, 0, 9999999999999999999.0);
	trModifyProtounit("Shaba Ka", ENEMY_PLAYER, 0, -9999999999999999999.0);
	trModifyProtounit("Shaba Ka", ENEMY_PLAYER, 0, 3000 * ENEMY_PLAYER);

	trModifyProtounit("Hippocampus", 0, 0, 9999999999999999999.0);
	trModifyProtounit("Hippocampus", 0, 0, -9999999999999999999.0);
	trModifyProtounit("Hippocampus", 0, 0, 1200);
	
	trModifyProtounit("Minion", 0, 8, -999);
	
	trModifyProtounit("Kronny Flying", 0, 1, 9999999999999999999.0);
	trModifyProtounit("Kronny Flying", 0, 1, -9999999999999999999.0);
	zInitProtoUnitStat("Kronny Flying", 0, 1, 0);
	
	trModifyProtounit("Bolt Strike", 0, 27, -10000);
	trModifyProtounit("Bolt Strike", 0, 28, -10000);
	trModifyProtounit("Bolt Strike", 0, 29, -10000);
	zInitProtoUnitStat("Bolt Strike", 0, 27, 0);
	
	xsEnableRule("setup_enemies");
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
		if (Multiplayer == false) {
			trPlayerGrantResources(1, "Gold", xGetInt(dPlayerData, xPlayerGold, 1));
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
		trOverlayText("Ascension MMORPG", 3.0, -1, -1, -1);
		trUIFadeToColor(0,0,0,1000,3000,true);
		xsEnableRule("Z_cin_02");
		xsDisableSelf();
	}
}

int dStageChoices = 0;
int xStageChoicesStage = 0;

void paintTowerSegment(int stage = 0) {
	int tPrimary = 0;
	int tSubPrimary = 0;
	switch(stage)
	{
		case 1:
		{
			tPrimary = 0;
			tSubPrimary = 34;
		}
		case 2:
		{
			tPrimary = 0;
			tSubPrimary = 58;
		}
		case 3:
		{
			tPrimary = 0;
			tSubPrimary = 41;
		}
		case 4:
		{
			tPrimary = 0;
			tSubPrimary = 25;
		}
		case 5:
		{
			tPrimary = 5;
			tSubPrimary = 5;
		}
		case 6:
		{
			tPrimary = 0;
			tSubPrimary = 70;
		}
		case 7:
		{
			tPrimary = 3;
			tSubPrimary = 9;
		}
		case 8:
		{
			tPrimary = 0;
			tSubPrimary = 50;
		}
		case 9:
		{
			tPrimary = 2;
			tSubPrimary = 12;
		}
		case 10:
		{
			tPrimary = 5;
			tSubPrimary = 4;
		}
	}
	trPaintTerrain(68,43 + 3 * stage,76,46 + 3 * stage, tPrimary, tSubPrimary, false);
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	int x = 139;
	int z = 89 + 6 * stage;
	if (iModulo(2, stage) == 0) {
		x = 151;
	}
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,x,0,z,180,true);
	trArmySelect("0,0");
	trMutateSelected(kbGetProtoUnitID("Outpost"));
	xSetPointer(dStageChoices,xAddDatabaseBlock(dStageChoices));
	xSetInt(dStageChoices,xUnitName,next);
	xSetInt(dStageChoices,xStageChoicesStage,stage);

	if (trQuestVarGet("monsterpediaQuestLocation") == stage) {
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Shaba Ka"));
		trUnitOverrideAnimation(2,0,true,false,-1);
	} else if (trQuestVarGet("hippocampusQuestLocation") == stage) {
		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Hippocampus"));
	}
}

rule Z_cin_02
inactive
highFrequency
{
	int next = 0;
	if (trTime() > cActivationTime + 5) {
		trModifyProtounit("Curse SFX", 1, 8, -8);
		if (xGetInt(dPlayerData,xPlayerProgress,1) <= 0) {
			trQuestVarSet("stage", 1);
			xsEnableRule("choose_stage_02");
			xsEnableRule("delayed_modify");
		} else {
			dStageChoices = xInitDatabase("stageChoices",xGetInt(dPlayerData,xPlayerProgress,1));
			xInitAddInt(dStageChoices,"name");
			xStageChoicesStage = xInitAddInt(dStageChoices,"stage");
			
			trLetterBox(false);
			uiClearSelection();
			trMusicPlay("cinematics\9_in\music.mp3", "1", 0.5);
			trUIFadeToColor(0,0,0,1000,0,false);
			trCameraCut(vector(145,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
			trQuestVarSet("chooser", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0", "Athena",1,145,0,89,0,true);
			trMessageSetText("Host: Choose a floor to challenge.",-1);
			
			trPaintTerrain(0,45,195,195,4,15,false); // shoreline atlantic B
			trPaintTerrain(0,0,195,45,0,34,false); // sand A
			
			if ((trQuestVarGet("p1nickQuestProgress") == 6) && (trQuestVarGet("newPlayers") == 0) && ENEMY_PLAYER > 2) {
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch("0,0","Dwarf",1,129,0,93,180,true);
				trArmySelect("0,0");
				trMutateSelected(kbGetProtoUnitID("Hero Greek Odysseus"));
				xSetPointer(dStageChoices,xAddDatabaseBlock(dStageChoices));
				xSetInt(dStageChoices,xUnitName,next);
				xSetInt(dStageChoices,xStageChoicesStage,0);
				
				trPaintTerrain(65,47,65,47,0,80);
				trPaintTerrain(65,46,65,46,0,74);
				trPaintTerrain(65,45,65,45,0,81);
				trPaintTerrain(64,45,64,45,0,75);
				trPaintTerrain(63,45,63,45,0,82);
				trPaintTerrain(63,46,63,46,0,74);
				trPaintTerrain(63,47,63,47,0,83);
				trPaintTerrain(64,47,64,47,0,75);
				trPaintTerrain(64,46,64,46,0,34);
			}

			if (trQuestVarGet("p1gladiatorWorlds") == 1) {
				trQuestVarSetFromRand("gladiatorWorldsVersion", 100, 1100, true);
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch("0,0","Dwarf",1,161,0,93,180,true);
				trArmySelect("0,0");
				trMutateSelected(kbGetProtoUnitID("Minotaur"));
				xSetPointer(dStageChoices,xAddDatabaseBlock(dStageChoices));
				xSetInt(dStageChoices,xUnitName,next);
				xSetInt(dStageChoices,xStageChoicesStage,12);
				trPaintTerrain(81,47,81,47,0,80);
				trPaintTerrain(81,46,81,46,0,74);
				trPaintTerrain(81,45,81,45,0,81);
				trPaintTerrain(80,45,80,45,0,75);
				trPaintTerrain(79,45,79,45,0,82);
				trPaintTerrain(79,46,79,46,0,74);
				trPaintTerrain(79,47,79,47,0,83);
				trPaintTerrain(80,47,80,47,0,75);
				trPaintTerrain(80,46,80,46,0,34);

			}

			if (trQuestVarGet("p1monsterpediaQuest") == 2) {
				trQuestVarSetFromRand("monsterpediaQuestLocation", 1, xGetInt(dPlayerData, xPlayerProgress, 1), true);
			}
			if (xGetInt(dPlayerData, xPlayerProgress, 1) >= 6 && trQuestVarGet("p1hippocampus") == 0) {
				trQuestVarSetFromRand("hippocampusQuestLocation", 1, 5, true);
			}
			trPaintTerrain(68,46,76,76,5,4,false); // black
			for(i=0; <= xGetInt(dPlayerData,xPlayerProgress,1)) {
				paintTowerSegment(i+1);
			}
			trPaintTerrain(67,46,67,76,0,74,false); // left wall
			trPaintTerrain(77,46,77,76,0,74,false); // right wall
			trPaintTerrain(67,77,77,77,0,75,false); // ceiling
			trPaintTerrain(67,77,67,77,0,83,false); // corner
			trPaintTerrain(77,77,77,77,0,80,false); // corner
			trPaintTerrain(67,45,67,45,0,81,false);
			trPaintTerrain(77,45,77,45,0,82,false);
			
			xsEnableRule("choose_stage_00");
		}
		trSetLighting("default", 0.1);
		xsDisableSelf();
	}
}

int monsterPetProto(int i = 0) {
	int proto = kbGetProtoUnitID("Golden Lion");
	switch(i)
	{
		case 1:
		{
			proto = kbGetProtoUnitID("Anubite");
		}
		case 2:
		{
			proto = kbGetProtoUnitID("Sphinx");
		}
		case 3:
		{
			proto = kbGetProtoUnitID("Petsuchos");
		}
		case 4:
		{
			proto = kbGetProtoUnitID("Centaur");
		}
		case 5:
		{
			proto = kbGetProtoUnitID("Dryad");
		}
		case 6:
		{
			proto = kbGetProtoUnitID("Medusa");
		}
		case 7:
		{
			proto = kbGetProtoUnitID("Mountain Giant");
		}
		case 8:
		{
			proto = kbGetProtoUnitID("Fenris Wolf");
		}
		case 9:
		{
			proto = kbGetProtoUnitID("Valkyrie");
		}
		case 10:
		{
			proto = kbGetProtoUnitID("Ballista");
		}
		case 11:
		{
			proto = kbGetProtoUnitID("Frost Giant");
		}
		case 12:
		{
			proto = kbGetProtoUnitID("Cyclops");
		}
		case 13:
		{
			proto = kbGetProtoUnitID("Satyr");
		}
		case 14:
		{
			proto = kbGetProtoUnitID("Behemoth");
		}
		case 15:
		{
			proto = kbGetProtoUnitID("Avenger");
		}
		case 16:
		{
			proto = kbGetProtoUnitID("Wadjet");
		}
		case 17:
		{
			proto = kbGetProtoUnitID("Scorpion Man");
		}
		case 18:
		{
			proto = kbGetProtoUnitID("Scarab");
		}
		case 19:
		{
			proto = kbGetProtoUnitID("Mummy");
		}
		case 20:
		{
			proto = kbGetProtoUnitID("Automaton SPC");
		}
		case 21:
		{
			proto = kbGetProtoUnitID("Fire Siphon");
		}
		case 22:
		{
			proto = kbGetProtoUnitID("Battle Boar");
		}
		case 23:
		{
			proto = kbGetProtoUnitID("Colossus");
		}
		case 24:
		{
			proto = kbGetProtoUnitID("Servant");
		}
		case 25:
		{
			proto = kbGetProtoUnitID("Nereid");
		}
		case 26:
		{
			proto = kbGetProtoUnitID("Kraken");
		}
		case 27:
		{
			proto = kbGetProtoUnitID("Hydra");
		}
		case 28:
		{
			proto = kbGetProtoUnitID("Griffon");
		}
		case 29:
		{
			proto = kbGetProtoUnitID("Einheriar");
		}
		case 30:
		{
			proto = kbGetProtoUnitID("Statue of Lightning");
		}
		case 31:
		{
			proto = kbGetProtoUnitID("Lampades");
		}
		case 32:
		{
			proto = kbGetProtoUnitID("Tartarian Gate spawn");
		}
		case 33:
		{
			proto = kbGetProtoUnitID("Manticore");
		}
		case 34:
		{
			proto = kbGetProtoUnitID("Troll");
		}
		case 35:
		{
			proto = kbGetProtoUnitID("Fire Giant");
		}
	}
	return(proto);
}
