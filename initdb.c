int dPlayerUnits = 0;
int dEnemies = 0;
int xPhysicalResist = 0;
int xMagicResist = 0;
int xPlayerOwner = 0;
int xIsHero = 0;
int xUnitPos = 0;

int dFreeRelics = 0;
int xRelicName = 0;
int xRelicType = 0;

int dPlayerData = 0;
int xPlayerClass = 0;
int xPlayerHealth = 0; //float
int xPlayerAttack = 0; //float
int xPlayerBaseAttackTrue = 0; //float
int xPlayerBaseAttack = 0; //float
int xPlayerRange = 0;
int xPlayerSpeed = 0;
int xPlayerFirstDelay = 0; //int
int xPlayerNextDelay = 0; //int
int xPlayerSpecialAttackCooldown = 0; //int
int xPlayerLos = 0;
int xPlayerProjectiles = 0; //int
int xPlayerSpellRange = 0;
int xPlayerSpellDamage = 0;
int xPlayerSpellDuration = 0;
int xPlayerSpellRangeTrue = 0;
int xPlayerSpellDamageTrue = 0;
int xPlayerSpellDurationTrue = 0;
int xPlayerHealBoost = 0;
int xPlayerFavorFromAttacks = 0;
int xPlayerPoisonSpeed = 0;
int xPlayerLifesteal = 0;
int xPlayerFavorRegen = 0;
int xPlayerPhysicalResist = 0;
int xPlayerMagicResist = 0;
int xPlayerPetDogs = 0;
int xPlayerMagicPen = 0;
int xPlayerCleave = 0;
int xPlayerDefiance = 0;

int xPlayerUltimateCost = 0;
int xPlayerCooldownReduction = 0;
int xPlayerStunResistance = 0;
int xPlayerPoisonResistance = 0;
int xPlayerSilenceResistance = 0;

int xPlayerUltimateCostCount = 0;
int xPlayerCooldownReductionCount = 0;
int xPlayerStunResistanceCount = 0;
int xPlayerPoisonResistanceCount = 0;
int xPlayerSilenceResistanceCount = 0;

int xPlayerStunDamage = 0;
int xPlayerPoisonKiller = 0;

int xPlayerWellCooldownStatus = 0;
int xPlayerLureCooldownStatus = 0;
int xPlayerRainCooldownStatus = 0;

int xPlayerRegenerateFavorLast = 0;

int xPlayerUnit = 0;
int xPlayerLevel = 0;
int xPlayerGodBoon = 0;

int dClass = 0;
int xClassProto = 0;
int xClassFirstDelay = 0;
int xClassNextDelay = 0;
int xClassSpecialAttackCooldown = 0;
int xClassGemstone = 0;
int xClassLevel = 0;

int setupClass(string proto = "", int class = 0, int firstDelay = 0, int nextDelay = 0,int gem = 0,int specialCD = 0) {
	int p = kbGetProtoUnitID(proto);
	xSetPointer(dClass,class);
	xSetInt(dClass,xClassProto,p);
	xSetInt(dClass,xClassFirstDelay,firstDelay);
	xSetInt(dClass,xClassNextDelay,nextDelay);
	xSetInt(dClass,xClassSpecialAttackCooldown,specialCD);
	xSetInt(dClass,xClassGemstone,gem);
	trQuestVarSet("proto"+p+"class", class);
}

rule initialize_databases
active
highFrequency
{
	xsDisableSelf();
	xsSetContextPlayer(0);
	dPlayerUnits = xInitDatabase("playerUnits", 30);
	xPhysicalResist = xInitAddFloat(dPlayerUnits,"physicalResist");
	xMagicResist = xInitAddFloat(dPlayerUnits,"magicResist");
	xPlayerOwner = xInitAddInt(dPlayerUnits,"player");
	xIsHero = xInitAddBool(dPlayerUnits,"isHero");
	xUnitPos = xInitAddVector(dPlayerUnits,"UnitPos");
	
	dEnemies = xInitDatabase("enemies", 30);
	xInitAddFloat(dEnemies,"physicalResist");
	xInitAddFloat(dEnemies,"magicResist");
	xInitAddInt(dEnemies,"player");
	xInitAddBool(dEnemies,"isHero");
	xInitAddVector(dEnemies,"UnitPos");
	
	dFreeRelics = xInitDatabase("freeRelics", 10);
	xRelicName = xInitAddInt(dFreeRelics,"name");
	xRelicType = xInitAddInt(dFreeRelics,"type");
	
	dPlayerData = xInitDatabase("playerData", cNumberPlayers - 2);
	xPlayerClass = xInitAddInt(dPlayerData,"class");
	xPlayerHealth = xInitAddFloat(dPlayerData,"health");
	xPlayerAttack = xInitAddFloat(dPlayerData,"attack");
	xPlayerBaseAttackTrue = xInitAddFloat(dPlayerData,"baseAttackTrue");
	xPlayerBaseAttack = xInitAddFloat(dPlayerData, "baseAttack");
	xPlayerRange = xInitAddFloat(dPlayerData, "range");
	xPlayerSpeed = xInitAddFloat(dPlayerData,"speed");
	xPlayerFirstDelay = xInitAddInt(dPlayerData,"firstDelay");
	xPlayerNextDelay = xInitAddInt(dPlayerData,"nextDelay");
	xPlayerSpecialAttackCooldown = xInitAddInt(dPlayerData,"specialAttackCD");
	xPlayerLos = xInitAddFloat(dPlayerData,"los",20);
	xPlayerProjectiles = xInitAddInt(dPlayerData,"projectiles",1);
	xPlayerSpellRange = xInitAddFloat(dPlayerData,"spellRange",1);
	xPlayerSpellDamage = xInitAddFloat(dPlayerData,"spellDamage",1);
	xPlayerSpellDuration = xInitAddFloat(dPlayerData,"spellDuration",1);
	xPlayerSpellRangeTrue = xInitAddFloat(dPlayerData,"spellRangeTrue",1);
	xPlayerSpellDamageTrue = xInitAddFloat(dPlayerData,"spellDamageTrue",1);
	xPlayerSpellDurationTrue = xInitAddFloat(dPlayerData,"spellDurationTrue",1);
	xPlayerHealBoost = xInitAddFloat(dPlayerData,"healBoost",1);
	xPlayerFavorFromAttacks = xInitAddInt(dPlayerData,"favorFromAttacks");
	xPlayerPoisonSpeed = xInitAddInt(dPlayerData,"poisonSpeed");
	xPlayerLifesteal = xInitAddFloat(dPlayerData,"lifesteal");
	xPlayerFavorRegen = xInitAddFloat(dPlayerData,"favorRegen");
	xPlayerPhysicalResist = xInitAddFloat(dPlayerData,"physicalResist");
	xPlayerMagicResist = xInitAddFloat(dPlayerData,"magicResist");
	xPlayerPetDogs = xInitAddInt(dPlayerData,"petDogs");
	xPlayerMagicPen = xInitAddFloat(dPlayerData,"magicPen");
	xPlayerCleave = xInitAddFloat(dPlayerData,"cleave");
	xPlayerDefiance = xInitAddFloat(dPlayerData,"defiance");
	
	xPlayerUltimateCost = xInitAddFloat(dPlayerData,"ultimateCost",1);
	xPlayerCooldownReduction = xInitAddFloat(dPlayerData,"cdr",1);
	xPlayerStunResistance = xInitAddFloat(dPlayerData,"stunResistance",1);
	xPlayerPoisonResistance = xInitAddFloat(dPlayerData,"poisonResistance",1);
	xPlayerSilenceResistance = xInitAddFloat(dPlayerData,"silenceResistance",1);
	
	xPlayerUltimateCostCount = xInitAddInt(dPlayerData,"ultimateCostCount");
	xPlayerCooldownReductionCount = xInitAddInt(dPlayerData,"cooldownReductionCount");
	xPlayerStunResistanceCount = xInitAddInt(dPlayerData,"stunResistanceCount");
	xPlayerPoisonResistanceCount = xInitAddInt(dPlayerData,"silenceResistanceCount");
	xPlayerSilenceResistanceCount = xInitAddInt(dPlayerData,"silenceResistanceCount");
	
	xPlayerStunDamage = xInitAddFloat(dPlayerData,"stunDamage");
	xPlayerPoisonKiller = xInitAddFloat(dPlayerData,"poisonKiller");
	
	xPlayerWellCooldownStatus = xInitAddInt(dPlayerData,"wellCooldownStatus");
	xPlayerLureCooldownStatus = xInitAddInt(dPlayerData,"lureCooldownStatus");
	xPlayerRainCooldownStatus = xInitAddInt(dPlayerData,"rainCooldownStatus");
	
	xPlayerRegenerateFavorLast = xInitAddInt(dPlayerData,"regenerateFavorLast");
	
	xPlayerUnit = xInitAddInt(dPlayerData,"playerUnit");
	xPlayerLevel = xInitAddInt(dPlayerData,"level");
	xPlayerGodBoon = xInitAddInt(dPlayerData,"blessing");
	
	dClass = xInitDatabase("classData", 16);
	xClassProto = xInitAddInt(dClass,"proto");
	xClassFirstDelay = xInitAddInt(dClass,"firstDelay");
	xClassNextDelay = xInitAddInt(dClass,"nextDelay");
	xClassSpecialAttackCooldown = xInitAddInt(dClass,"specialAttackCD");
	xClassGemstone = xInitAddInt(dClass,"gemstone");
	xClassLevel = xInitAddInt(dClass,"level");
	
	for(i=0; < 16) {
		xAddDatabaseBlock(dClass);
	}
	
	for(p=1; < cNumberPlayers - 1) {
		trQuestVarSet("p"+p+"relics",xInitDatabase("p"+p+"relics"));
		xInitAddInt(1*trQuestVarGet("p"+p+"relics"),"name");
		xInitAddInt(1*trQuestVarGet("p"+p+"relics"),"type");
		xAddDatabaseBlock(dPlayerData);
	}
}
