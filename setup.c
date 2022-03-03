

const int CHEST_KEY = 1;
const int CHEST_ENCOUNTER = 2;
const int CHEST_STATUES = 3;

const int MOONBLADE = 1;
const int SUNBOW = 2;
const int FIREKNIGHT = 3;
const int THRONESHIELD = 4;

const int STORMCUTTER = 5;
const int BLASTMAGE = 6;
const int THUNDERRIDER = 7;
const int ALCHEMIST = 8;

const int GARDENER = 9;
const int STARSEER = 10;
const int COMMANDO = 11;
const int SPELLSTEALER = 12;

const int NIGHTRIDER = 13;
const int SPARKWITCH = 14;
const int SAVIOR = 15;

const int CLASS_COUNT = 16;
const int FROSTKNIGHT = 17;

const int STARSTONE = 0;
const int SOULSTONE = 1;
const int MANASTONE = 2;

float worldHeight = 0;
float wallHeight = 5;
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
			name = "The Unknown";
		}
		case 0:
		{
			name = "Dream Realm";
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
			img = "ui\ui map land unknown 256x256";
		}
	}
	return(img);
}

void reselectMyself() {
	uiClearSelection();
	int p = trCurrentPlayer();
	int class = trQuestVarGet("p"+p+"class");
	trackInsert();
	trackAddWaypoint();
	trackAddWaypoint();
	trBlockAllSounds(false);
	uiFindType(kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto")));
	trackPlay(1,999);
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

void setupClass(string proto = "", int class = 0, int firstDelay = 0, int nextDelay = 0,int gem = 0,int specialCD = 0) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("class"+class+"proto", p);
	trQuestVarSet("proto"+p+"class", class);
	trQuestVarSet("class"+class+"firstDelay", firstDelay);
	trQuestVarSet("class"+class+"nextDelay", nextDelay);
	trQuestVarSet("class"+class+"specialAttackCooldown", specialCD);
	trQuestVarSet("class"+class+"gemstone", gem);
}

void chooseClass(int p = 0, int class = 0) {
	trQuestVarSet("p"+p+"class", class);
	trEventFire(1000 + 12 * class + p);
	int proto = trQuestVarGet("class"+class+"proto");
	trQuestVarSet("p"+p+"health", trQuestVarGet("proto"+proto+"health"));
	trQuestVarSet("p"+p+"attack", trQuestVarGet("proto"+proto+"attack"));
	trQuestVarSet("p"+p+"baseAttackTrue", trQuestVarGet("proto"+proto+"attack"));
	trQuestVarSet("p"+p+"baseAttack", trQuestVarGet("proto"+proto+"attack"));
	trQuestVarSet("p"+p+"attack", trQuestVarGet("proto"+proto+"attack"));
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
	trQuestVarSet("p"+p+"spellRangeTrue", 1);
	trQuestVarSet("p"+p+"spellDamageTrue", 1);
	trQuestVarSet("p"+p+"spellDurationTrue", 1);
	trQuestVarSet("p"+p+"healBoost", 1);
	trQuestVarSet("p"+p+"favorFromAttacks", 0);
	trQuestVarSet("p"+p+"poisonSpeed", 0);
	trQuestVarSet("p"+p+"lifesteal", 0);
	trQuestVarSet("p"+p+"favorRegen", 0);
	trQuestVarSet("p"+p+"physicalResist", trQuestVarGet("proto"+proto+"armor"));
	trQuestVarSet("p"+p+"magicResist", trQuestVarGet("proto"+proto+"armor"));
	
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
	
	trQuestVarSet("p"+p+"stunDamage", 0);
	trQuestVarSet("p"+p+"poisonKiller", 0);
	
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"unit");
	if (trUnitAlive() && trQuestVarGet("p"+p+"unit") > 0) {
		trMutateSelected(proto);
	}
	trPlayerKillAllGodPowers(p);
	if (trCurrentPlayer() == p) {
		trCounterAbort("lure");
		trCounterAbort("well");
		trCounterAbort("rain");
	}
	if (class > 0) {
		trQuestVarSet("p"+p+"wellCooldownStatus", 1);
		trQuestVarSet("p"+p+"lureCooldownStatus", 1);
		trQuestVarSet("p"+p+"rainCooldownStatus", 1);
	}
	
	if (Multiplayer == false) {
		trQuestVarSet("p"+p+"level", trQuestVarGet("class"+class+"level") - 1);
		trSetCivilizationNameOverride(p, "Level " + (1+trQuestVarGet("p"+p+"level")));
	}
	
	for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
		yDatabaseNext("p"+p+"relics");
		if (x > trQuestVarGet("p"+p+"level")+1) {
			yAddToDatabase("freeRelics", "p"+p+"relics");
			yAddUpdateVar("freeRelics", "type", 1*yGetVar("p"+p+"relics", "type"));
			yRemoveFromDatabase("p"+p+"relics");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"relics");
			trUnitChangeProtoUnit("Relic");
		} else {
			relicEffect(1*yGetVar("p"+p+"relics", "type"), p, true);
		}
	}
}

rule setup
active
runImmediately
{
	Multiplayer = aiIsMultiplayer();
	trSetUnitIdleProcessing(false);
	
	%
	for(p=1; < cNumberNonGaiaPlayers) {
		code("trStringQuestVarSet(\"p"+p+"name\", \""+rmGetPlayerName(p)+"\");");
	}
	code("map(\"esc\", \"ShowImageBox\", \"gadgetUnreal("+quote+"ShowImageBox"+quote+")\");");
	%
	
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
	
	modularCounterInit("spyFind", 32);
	modularCounterInit("spyFound", 32);
	
	/*
	player 0 omniscience
	*/
	trTechSetStatus(0, 304, 4);
	trTechSetStatus(ENEMY_PLAYER, 304, 4);
	aiSet("NoAI", 0);
	aiSet("NoAI", ENEMY_PLAYER);
	xsSetContextPlayer(ENEMY_PLAYER);
	aiSetAttackResponseDistance(0.0);
	
	setupClass("Militia", 0, 500, 1000);
	setupClass("Militia", 13, 500, 1000);
	setupClass("Militia", 14, 500, 1000);
	setupClass("Militia", 15, 500, 1000);
	setupClass("Militia", 16, 500, 1000);
	/* Proto , Enumeration , First delay , Next delay , special attack cooldown */
	setupClass("Hero Greek Theseus", MOONBLADE, 460, 1000, STARSTONE, 7);
	setupClass("Hero Greek Hippolyta", SUNBOW, 1350, 1750, STARSTONE);
	setupClass("Hero Greek Atalanta", THUNDERRIDER, 630, 1400, MANASTONE, 5);
	setupClass("Lancer Hero", FIREKNIGHT, 1155, 1500, MANASTONE, 5);
	setupClass("Hero Greek Achilles", NIGHTRIDER, 470, 1000, SOULSTONE, 8);
	setupClass("Priest", BLASTMAGE, 500, 800, MANASTONE);
	setupClass("Oracle Hero", STARSEER, 540, 1500, STARSTONE, 8);
	setupClass("Archer Atlantean Hero", STORMCUTTER, 400, 1000, MANASTONE);
	setupClass("Pharaoh", ALCHEMIST, 550, 1200, SOULSTONE);
	setupClass("Swordsman Hero", SPELLSTEALER, 400, 800, MANASTONE, 6);
	setupClass("Javelin Cavalry Hero", COMMANDO, 1000, 2000, STARSTONE);
	setupClass("Trident Soldier Hero", THRONESHIELD, 625, 1250, SOULSTONE, 10);
	setupClass("Hero Greek Bellerophon", SAVIOR, 625, 1250, STARSTONE, 3);
	setupClass("Hero Greek Chiron", GARDENER, 900, 1500, SOULSTONE);
	setupClass("Circe", SPARKWITCH, 1400, 2800, MANASTONE);
	
	trQuestVarSet("p"+ENEMY_PLAYER+"stunResistance", 1);
	trQuestVarSet("p"+ENEMY_PLAYER+"poisonResistance", 1);
	trQuestVarSet("p0stunResistance", 1);
	trQuestVarSet("p0poisonResistance", 1);
	for(p=1; < ENEMY_PLAYER) {
		trPlayerSetDiplomacy(p, 0, "neutral");
		trPlayerSetDiplomacy(p, ENEMY_PLAYER, "Enemy");
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
		trModifyProtounit("Sea Turtle", p, 55, 1);
		trModifyProtounit("Pegasus", p, 55, 1);
		trModifyProtounit("Flying Medic", p, 55, 1);
		trModifyProtounit("Junk", p, 55, 1);
		
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
	
	xsEnableRule("delayed_modify");
	xsEnableRule("data_load_00");
	xsDisableSelf();
}

rule delayed_modify
inactive
highFrequency
{
	if (trTime() > cActivationTime) {
		zInitProtoUnitStat("Revealer", 1, 2, 12);
		zInitProtoUnitStat("Revealer to Player", 1, 2, 12);
		zSetProtoUnitStat("Revealer to Player", 1, 2, 32);
		setupPlayerProto("Kronny Flying", 1000, 0, 0);
		
		/* i gotta look good */
		trTechSetStatus(0, 7, 4);
		trTechSetStatus(0, 476, 4);
		trTechSetStatus(ENEMY_PLAYER, 7, 4);
		trTechSetStatus(ENEMY_PLAYER, 476, 4);
		
		setupPlayerProto("Militia", 100, 10, 4.8);
		setupPlayerProto("Wolf", 200, 10, 5, 0.2);
		setupPlayerProto("Minion", 240, 10, 5, 0.2);
		setupPlayerProto("Hero Greek Theseus", 1000, 50, 4.3, 0.3);
		setupPlayerProto("Hero Greek Hippolyta", 1000, 50, 4.3, 0, 16);
		setupPlayerProto("Hero Greek Atalanta", 800, 30, 6.0, 0);
		setupPlayerProto("Lancer Hero", 1100, 55, 6.05, 0.5);
		setupPlayerProto("Hero Greek Achilles", 1200, 50, 5.5, 0.4);
		setupPlayerProto("Archer Atlantean Hero", 900, 30, 4.05, 0, 20);
		setupPlayerProto("Pharaoh", 1000, 50, 4.0, 0, 12);
		setupPlayerProto("Swordsman Hero", 1000, 50, 4.8, 0.3);
		setupPlayerProto("Javelin Cavalry Hero", 1200, 45, 5.3, 0, 12);
		setupPlayerProto("Trident Soldier Hero", 1200, 30, 3.9, 0);
		setupPlayerProto("Hero Greek Bellerophon", 1200, 60, 6.0, 0.3);
		setupPlayerProto("Hero Greek Chiron", 1000, 50, 5.5, 0, 16);
		setupPlayerProto("Priest", 1000, 10, 3.6, 0, 16);
		setupPlayerProto("Oracle Hero", 1000, 0, 4.0, 0.3);
		setupPlayerProto("Circe", 1000, 0, 3.7, 0, 15);
		setupPlayerProto("Audrey", 1000, 50, 0);
		setupPlayerProto("Walking Berry Bush", 500, 25, 3.5, 0.3);
		
		
		setupPlayerProto("Villager Atlantean Hero", 500, 0, 4.0);
		
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
		
		for(p=ENEMY_PLAYER; >0) {
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
			zInitProtoUnitStat("Cinematic Block", p, 0, 300);
			zInitProtoUnitStat("Priest Projectile", p, 8, 2);
			
			zInitProtoUnitStat("Ballista Shot", p, 1, 30);
			
			trModifyProtounit("Servant", p, 55, 1);
			trModifyProtounit("Nereid", p, 55, 1);
			trModifyProtounit("Scylla", p, 55, 1);
		}
		
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
		trOverlayText("Ascension MMORPG", 3.0, -1, -1, -1);
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
		trModifyProtounit("Curse SFX", 1, 8, -8);
		if (trQuestVarGet("p1progress") == 0) {
			trQuestVarSet("stage", 1);
		} else {
			trLetterBox(false);
			trMusicPlay("cinematics\9_in\music.mp3", "1", 0.5);
			trUIFadeToColor(0,0,0,1000,0,false);
			trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));
			trQuestVarSet("chooser", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0", "Athena",1,96,0,90,0,true);
			trMessageSetText("Host: Choose a floor to challenge.",-1);
			
			int posX = 97 - 2 * trQuestVarGet("p1progress");
			if (trQuestVarGet("p1nickQuestProgress") == 6) {
				posX = posX - 2;
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,posX,0,101,180,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trMutateSelected(kbGetProtoUnitID("Hero Greek Odysseus"));
				yAddToDatabase("stageChoices", "next");
				yAddUpdateVar("stageChoices", "stage", 0);
				yAddUpdateVar("stageChoices", "obelisk", trQuestVarGet("next"));
				posX = posX + 4;
			}
			
			for(x=0; <= trQuestVarGet("p1progress")) {
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Flag Numbered",1,posX,0,101,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitSetAnimationPath(""+x+",0,0,0,0,0,0");
				yAddToDatabase("stageChoices", "next");
				yAddUpdateVar("stageChoices", "stage", x + 1);
				yAddUpdateVar("stageChoices", "obelisk", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,posX,0,103,0,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trMutateSelected(kbGetProtoUnitID("Outpost"));
				posX = posX + 4;
			}
			
			xsEnableRule("choose_stage_00");
		}
		trSetLighting("default", 0.1);
		xsDisableSelf();
		xsEnableRule("choose_stage_01");
	}
}
