int dPlayerUnits = 0;
int dEnemies = 0;
int xUnitName = 0;
int xPlayerOwner = 0;
int xPhysicalResist = 0;
int xMagicResist = 0;
int xIsHero = 0;
int xUnitPos = 0;
int xDoppelganger = 0;

int xCurrentHealth = 0;
int xStunStatus = 0;
int xStunTimeout = 0;
int xStunSFX = 0;
int xPoisonStatus = 0;
int xPoisonTimeout = 0;
int xPoisonLast = 0;
int xPoisonDamage = 0;
int xPoisonSFX = 0;
int xSilenceStatus = 0;
int xSilenceTimeout = 0;
int xSilenceSFX = 0;
int xLaunched = 0;

/* playerUnits specific */
int xDecay = 0;
int xDecayNext = 0;
int xUnity = 0;

/* enemies specific */
int xBounty = 0;
int xDropRelic = 0;
int xDeathSentence = 0;

/* playerCharacters */
int dPlayerCharacters = 0;
int xIndex = 0;
int xCharAttacking = 0;
int xCharAttackNext = 0;
int xCharSpecialAttack = 0;

/* relics */
int dFreeRelics = 0;
int xRelicName = 0;
int xRelicType = 0;

/* player data */
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
int xPlayerLifestealTotal = 0;
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
int xPlayerWellCooldown = 0;
int xPlayerLureCooldown = 0;
int xPlayerRainCooldown = 0;

int xPlayerRegenerateFavorLast = 0;

int xPlayerUnit = 0;
int xPlayerLevel = 0;
int xPlayerProgress = 0;
int xPlayerGodBoon = 0;
int xPlayerIndex = 0;
int xPlayerSimp = 0;
int xPlayerQueen = 0;
int xPlayerLaunched = 0;

int xPlayerMonsterIndex = 0;
int xPlayerRelicTransporterLevel = 0;
int xPlayerGold = 0;
int xPlayerFavor = 0;
int xPlayerDead = 0;

int dClass = 0;
int xClassProto = 0;
int xClassFirstDelay = 0;
int xClassNextDelay = 0;
int xClassSpecialAttackCooldown = 0;
int xClassGemstone = 0;
int xClassLevel = 0;

int dFrostGiantsIncoming = 0;
int xFrostTargetSize = 0;
int xFrostCurrentSize = 0;
int xFrostTargetTime = 0;

int dStunnedUnits = 0;
int xStunnedProto = 0;

int dLaunchedUnits = 0;
int xLaunchedCar = 0;
int xLaunchedDB = 0;
int xLaunchedIndex = 0;
int xLaunchedStun = 0;
int xLaunchedDest = 0;
int xLaunchedTimeout = 0;

rule initialize_databases
active
highFrequency
{
	xsDisableSelf();
	xsSetContextPlayer(0);
	dPlayerUnits = xInitDatabase("playerUnits", 30);
	dEnemies = xInitDatabase("enemies", 30);
	for(db=dEnemies; >= dPlayerUnits) {
		xUnitName = xInitAddInt(db);
		xPhysicalResist = xInitAddFloat(db,"physicalResist");
		xMagicResist = xInitAddFloat(db,"magicResist");
		xPlayerOwner = xInitAddInt(db,"player");
		xIsHero = xInitAddBool(db,"isHero");
		xUnitPos = xInitAddVector(db,"UnitPos");
		xDoppelganger = xInitAddInt(db,"doppelganger");
		xCurrentHealth = xInitAddFloat(db,"currentHP");
		xStunStatus = xInitAddInt(db,"stunStatus");
		xStunTimeout = xInitAddInt(db,"stunTimeout");
		xStunSFX = xInitAddInt(db,"stunSFX");
		xPoisonStatus = xInitAddInt(db,"poisonStatus");
		xPoisonTimeout = xInitAddInt(db,"poisonTimeout");
		xPoisonLast = xInitAddInt(db,"poisonLast");
		xPoisonDamage = xInitAddFloat(db,"poisonDamage");
		xPoisonSFX = xInitAddInt(db,"poisonSFX");
		xSilenceStatus = xInitAddInt(db,"silenceStatus");
		xSilenceTimeout = xInitAddInt(db,"silenceTimeout");
		xSilenceSFX = xInitAddInt(db,"silenceSFX");
		xLaunched = xInitAddBool(db,"launched");
	}
	xDecay = xInitAddInt(dPlayerUnits,"decay");
	xDecayNext = xInitAddInt(dPlayerUnits,"decayNext");
	xUnity = xInitAddInt(dPlayerUnits,"unity");
	
	xBounty = xInitAddInt(dEnemies,"bounty");
	xDropRelic = xInitAddInt(dEnemies,"relic");
	xDeathSentence = xInitAddInt(dEnemies,"deathSentence");
	
	
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
	xPlayerLifestealTotal = xInitAddFloat(dPlayerData,"lifestealTotal");
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
	xPlayerWellCooldown = xInitAddInt(dPlayerData,"wellCooldown");
	xPlayerLureCooldown = xInitAddInt(dPlayerData,"lureCooldown");
	xPlayerRainCooldown = xInitAddInt(dPlayerData,"rainCooldown");
	
	xPlayerRegenerateFavorLast = xInitAddInt(dPlayerData,"regenerateFavorLast");
	
	xPlayerUnit = xInitAddInt(dPlayerData,"playerUnit");
	xPlayerLevel = xInitAddInt(dPlayerData,"level");
	xPlayerProgress = xInitAddInt(dPlayerData,"progress");
	xPlayerGodBoon = xInitAddInt(dPlayerData,"blessing");
	xPlayerGold = xInitAddInt(dPlayerData,"gold");
	xPlayerFavor = xInitAddFloat(dPlayerData,"favor");
	xPlayerDead = xInitAddInt(dPlayerData,"dead");
	xPlayerIndex = xInitAddInt(dPlayerData,"index");
	xPlayerSimp = xInitAddInt(dPlayerData,"simp");
	xPlayerQueen = xInitAddInt(dPlayerData,"queen");
	xPlayerLaunched = xInitAddBool(dPlayerData,"launched");
	
	xPlayerMonsterIndex = xInitAddInt(dPlayerData,"monsterIndex");
	xPlayerRelicTransporterLevel = xInitAddInt(dPlayerData,"relicTransporterLevel");
	
	dStunnedUnits = xInitDatabase("stunnedUnits", 10);
	xInitAddInt(dStunnedUnits,"name");
	xInitAddInt(dStunnedUnits,"player");
	xStunnedProto = xInitAddInt(dStunnedUnits,"proto");
	
	dLaunchedUnits = xInitDatabase("launchedUnits",10);
	xInitAddInt(dLaunchedUnits,"name");
	xInitAddInt(dLaunchedUnits,"player");
	xInitAddInt(dLaunchedUnits,"proto");
	xLaunchedCar = xInitAddInt(dLaunchedUnits,"car");
	xLaunchedDB = xInitAddInt(dLaunchedUnits,"database");
	xLaunchedIndex = xInitAddInt(dLaunchedUnits,"index");
	xLaunchedStun = xInitAddBool(dLaunchedUnits,"stun");
	xLaunchedDest = xInitAddVector(dLaunchedUnits,"dest");
	xLaunchedTimeout = xInitAddInt(dLaunchedUnits,"timeout");
	
	
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
	
	dPlayerCharacters = xInitDatabase("playerCharacters", cNumberPlayers - 2);
	xInitAddInt(dPlayerCharacters,"name");
	xInitAddInt(dPlayerCharacters,"player");
	xIndex = xInitAddInt(dPlayerCharacters,"index");
	
	for(p=1; < cNumberPlayers - 1) {
		trQuestVarSet("p"+p+"relics",xInitDatabase("p"+p+"relics"));
		xInitAddInt(1*trQuestVarGet("p"+p+"relics"),"name");
		xInitAddInt(1*trQuestVarGet("p"+p+"relics"),"type");
		
		trQuestVarSet("p"+p+"warehouse",xInitDatabase("p"+p+"warehouse"));
		xInitAddInt(1*trQuestVarGet("p"+p+"warehouse"),"name");
		xInitAddInt(1*trQuestVarGet("p"+p+"warehouse"),"type");
		
		trQuestVarSet("p"+p+"characters",xInitDatabase("p"+p+"characters"));
		/* the three below are shared with playerCharacter */
		xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"name");
		xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"player");
		xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"index");
		xCharSpecialAttack = xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"specialAttack");
		xCharAttacking = xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"attacking");
		xCharAttackNext = xInitAddInt(1*trQuestVarGet("p"+p+"characters"),"attackNext");
		
		xAddDatabaseBlock(dPlayerData);
	}
}
