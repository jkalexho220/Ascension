int dPlayerUnits = 0;
int dEnemies = 0;
int dEnemiesIncoming = 0;

int xUnitName = 0;
int xPlayerOwner = 0;
int xUnitID = 0;
int xPhysicalResist = 0;
int xMagicResist = 0;
int xIsHero = 0;
int xUnitPos = 0;
int xDoppelganger = 0;

int xCurrentHealth = 0;
int xUnitProto = 0;
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

int xDatabaseIndex = 0;

/* playerUnits specific */
int xDecay = 0;
int xDecayNext = 0;
int xDeepDamageLast = 0;

/* enemies specific */
int xBounty = 0;
int xDropRelic = 0;
int xDeathSentence = 0;
int xEchoBomb = 0;
int xMissingTimeout = 0;

/* playerCharacters */
int dPlayerCharacters = 0;
int xCharIndex = 0;
int xCharSmiteDamage = 0;
int xCharAttacking = 0;
int xCharAttackNext = 0;
int xCharAttackTarget = 0;
int xCharAttackTargetIndex = 0;
int xCharSpecialAttack = 0;

int xElectricNext = 0;

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
int xPlayerFavorSpecial = 0;

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
int xPlayerPoisonKillerActive = 0;

int xPlayerWellCooldownStatus = 0;
int xPlayerLureCooldownStatus = 0;
int xPlayerRainCooldownStatus = 0;
int xPlayerWellCooldown = 0;
int xPlayerLureCooldown = 0;
int xPlayerRainCooldown = 0;
int xPlayerWellCost = 0;
int xPlayerLureCost = 0;
int xPlayerRainCost = 0;
int xPlayerWellActivated = 0;
int xPlayerLureActivated = 0;
int xPlayerRainActivated = 0;
int xPlayerWellReadyTime = 0;
int xPlayerLureReadyTime = 0;
int xPlayerRainReadyTime = 0;
int xPlayerWellPos = 0;
int xPlayerLurePos = 0;

int xPlayerRegenerateFavorLast = 0;
int xPlayerRegenerateHealthLast = 0;

int xPlayerUnit = 0;
int xPlayerLevel = 0;
int xPlayerProgress = 0;
int xPlayerGodBoon = 0;
int xPlayerIndex = 0;
int xPlayerSimp = 0;
int xPlayerQueen = 0;
int xPlayerLaunched = 0;
int xPlayerAnimation = 0;
int xPlayerAction = 0;
int xPlayerSilenced = 0;

int xPlayerTether = 0;

int xPlayerMonsterIndex = 0;
int xPlayerMonsterProto = 0;
int xPlayerMonsterName = 0;
int xPlayerRelicTransporterLevel = 0;
int xPlayerGold = 0;
int xPlayerFavor = 0;
int xPlayerDead = 0;
int xPlayerResigned = 0;
int xPlayerReviveNext = 0;

int xPlayerPetDogReady = 0;
int xPlayerPetDogNext = 0;
int xPlayerPetMonsterReady = 0;
int xPlayerPetMonsterNext = 0;

int xPlayerHealFavorCharges = 0;
int xPlayerHealFavorNext = 0;

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

/* generic projectiles */
int xProjYeehaw = 0;
int xProjScale = 0;
int xProjProto = 0;
int xProjAnim = 0;
int xProjDir = 0;
int xProjHeight = 0;
int xProjSpeed = 0;
int xProjPrev = 0;
int xProjDist = 0;

int dBoons = 0;
int xBoonType = 0;

int dPlayerWolves = 0;
int xPlayerWolfDead = 0;
int xPlayerWolfTimeout = 0;

int dPlayerLasers = 0;
int xPlayerLaserTimeout = 0;
int xPlayerLaserRange = 0;

int dSlotRelics = 0;
int xSlotRelicPad = 0;

int dSlotUnits = 0;

int dMonsterpedia = 0;
int xMonsterIndex = 0;

int dRelicDescriptors = 0;

int dPlayerCharacterArray = 0;
int dPlayerRelicsArray = 0;
int dPlayerWarehouseArray = 0;

int dApplicants = 0;

int dZenoRelics = 0;

int dKeeperPaint = 0;
int xKeeperPaintTimeout = 0;
int xKeeperPaintPrimary = 0;
int xKeeperPaintSecondary = 0;
int xKeeperPaintPos = 0;

int rememberRelics = 0;

int getCharactersDB(int p = 0) {
	return(aiPlanGetUserVariableInt(ARRAYS,dPlayerCharacterArray,p));
}

int getRelicsDB(int p = 0) {
	return(aiPlanGetUserVariableInt(ARRAYS,dPlayerRelicsArray,p));
}

int getWarehouseDB(int p = 0) {
	return(aiPlanGetUserVariableInt(ARRAYS,dPlayerWarehouseArray,p));
}

rule initialize_databases
active
highFrequency
{
	xsSetContextPlayer(0);
	xsDisableSelf();
	int db = 0;

	rememberRelics = zNewArray(mBool, 31, "remember_relics");

	dPlayerUnits = xInitDatabase("playerUnits", 30);
	dEnemies = xInitDatabase("enemies", 30);
	for(db=dEnemies; >= dPlayerUnits) {
		xUnitName = xInitAddInt(db,"name");
		xPlayerOwner = xInitAddInt(db,"player");
		xUnitID = xInitAddInt(db,"ID");
		xPhysicalResist = xInitAddFloat(db,"physicalResist");
		xMagicResist = xInitAddFloat(db,"magicResist");
		xIsHero = xInitAddBool(db,"isHero");
		xUnitPos = xInitAddVector(db,"UnitPos");
		xDoppelganger = xInitAddInt(db,"doppelganger");
		xCurrentHealth = xInitAddFloat(db,"currentHP");
		xUnitProto = xInitAddInt(db, "unitProto");
		xStunStatus = xInitAddInt(db,"stunStatus");
		xPoisonStatus = xInitAddInt(db,"poisonStatus");
		xSilenceStatus = xInitAddInt(db,"silenceStatus");
		xStunTimeout = xInitAddInt(db,"stunTimeout");
		xPoisonTimeout = xInitAddInt(db,"poisonTimeout");
		xSilenceTimeout = xInitAddInt(db,"silenceTimeout");
		xPoisonLast = xInitAddInt(db,"poisonLast");
		xPoisonDamage = xInitAddFloat(db,"poisonDamage");
		xStunSFX = xInitAddInt(db,"stunSFX");
		xPoisonSFX = xInitAddInt(db,"poisonSFX");
		xSilenceSFX = xInitAddInt(db,"silenceSFX");
		xLaunched = xInitAddBool(db,"launched");
	}
	xDecay = xInitAddFloat(dPlayerUnits,"decay");
	xDecayNext = xInitAddInt(dPlayerUnits,"decayNext");
	
	xBounty = xInitAddInt(dEnemies,"bounty");
	xDropRelic = xInitAddInt(dEnemies,"relic");
	xDeathSentence = xInitAddBool(dEnemies,"deathSentence", false);
	xEchoBomb = xInitAddBool(dEnemies,"echoBomb", false);
	xMissingTimeout = xInitAddInt(dEnemies,"missingTimeout");
	
	xDatabaseIndex = xPlayerOwner;
	
	dEnemiesIncoming = xInitDatabase("enemiesIncoming",50);
	xInitAddInt(dEnemiesIncoming,"name");
	
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
	xPlayerFavorSpecial = xInitAddFloat(dPlayerData,"defiance");
	
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
	xPlayerPoisonKillerActive = xInitAddInt(dPlayerData,"poisonKillerActive");
	
	xPlayerWellCooldownStatus = xInitAddInt(dPlayerData,"wellCooldownStatus");
	xPlayerLureCooldownStatus = xInitAddInt(dPlayerData,"lureCooldownStatus");
	xPlayerRainCooldownStatus = xInitAddInt(dPlayerData,"rainCooldownStatus");
	xPlayerWellCooldown = xInitAddInt(dPlayerData,"wellCooldown");
	xPlayerLureCooldown = xInitAddInt(dPlayerData,"lureCooldown");
	xPlayerRainCooldown = xInitAddInt(dPlayerData,"rainCooldown");
	xPlayerWellCost = xInitAddFloat(dPlayerData,"wellCost");
	xPlayerLureCost = xInitAddFloat(dPlayerData,"lureCost");
	xPlayerRainCost = xInitAddFloat(dPlayerData,"rainCost");
	xPlayerWellActivated = xInitAddBool(dPlayerData, "wellActivated");
	xPlayerLureActivated = xInitAddBool(dPlayerData, "lureActivated");
	xPlayerRainActivated = xInitAddBool(dPlayerData, "rainActivated");
	xPlayerWellReadyTime = xInitAddInt(dPlayerData, "wellReadyTime");
	xPlayerLureReadyTime = xInitAddInt(dPlayerData, "lureReadyTime");
	xPlayerRainReadyTime = xInitAddInt(dPlayerData, "rainReadyTime");
	xPlayerWellPos = xInitAddVector(dPlayerData, "wellPos");
	xPlayerLurePos = xInitAddVector(dPlayerData, "lurePos");
	
	xPlayerRegenerateFavorLast = xInitAddInt(dPlayerData,"regenerateFavorLast");
	xPlayerRegenerateHealthLast = xInitAddInt(dPlayerData,"regenerateHealthLast");
	
	xPlayerUnit = xInitAddInt(dPlayerData,"playerUnit");
	xPlayerLevel = xInitAddInt(dPlayerData,"level");
	xPlayerProgress = xInitAddInt(dPlayerData,"progress");
	xPlayerGodBoon = xInitAddInt(dPlayerData,"blessing");
	xPlayerGold = xInitAddInt(dPlayerData,"gold");
	xPlayerFavor = xInitAddFloat(dPlayerData,"favor");
	xPlayerDead = xInitAddInt(dPlayerData,"dead");
	xPlayerResigned = xInitAddBool(dPlayerData,"resigned");
	xPlayerReviveNext = xInitAddInt(dPlayerData,"reviveNext");
	xPlayerIndex = xInitAddInt(dPlayerData,"index");
	xPlayerSimp = xInitAddInt(dPlayerData,"simp");
	xPlayerQueen = xInitAddInt(dPlayerData,"queen");
	xPlayerLaunched = xInitAddBool(dPlayerData,"launched");
	xPlayerAnimation = xInitAddInt(dPlayerData,"animation");
	xPlayerAction = xInitAddInt(dPlayerData,"action");
	xPlayerSilenced = xInitAddBool(dPlayerData,"silenced");
	
	xPlayerTether = xInitAddInt(dPlayerData,"tether",-1);
	
	xPlayerMonsterIndex = xInitAddInt(dPlayerData,"monsterIndex");
	xPlayerMonsterProto = xInitAddInt(dPlayerData,"monsterProto");
	xPlayerMonsterName = xInitAddInt(dPlayerData,"monsterName");
	xPlayerRelicTransporterLevel = xInitAddInt(dPlayerData,"relicTransporterLevel");
	
	xPlayerPetDogReady = xInitAddBool(dPlayerData, "petDogReady");
	xPlayerPetDogNext = xInitAddInt(dPlayerData, "petDogNext");
	xPlayerPetMonsterReady = xInitAddBool(dPlayerData, "petMonsterReady", true);
	xPlayerPetMonsterNext = xInitAddInt(dPlayerData, "petMonsterNext");

	xPlayerHealFavorCharges = xInitAddInt(dPlayerData, "healFavorCharges");
	xPlayerHealFavorNext = xInitAddInt(dPlayerData, "healFavorNext");
	
	dStunnedUnits = xInitDatabase("stunnedUnits", 10);
	xInitAddInt(dStunnedUnits,"name");
	xInitAddInt(dStunnedUnits,"player");
	xInitAddInt(dStunnedUnits,"id");
	xStunnedProto = xInitAddInt(dStunnedUnits,"proto");
	
	dLaunchedUnits = xInitDatabase("launchedUnits",10);
	xInitAddInt(dLaunchedUnits,"name");
	xInitAddInt(dLaunchedUnits,"player");
	xInitAddInt(dLaunchedUnits,"id");
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
	xInitAddInt(dPlayerCharacters,"id");
	xCharIndex = xInitAddInt(dPlayerCharacters,"index");
	
	dPlayerCharacterArray = zNewArray(mInt, cNumberPlayers, "playerCharacterArrays");
	dPlayerRelicsArray = zNewArray(mInt, cNumberPlayers, "playerRelicArrays");
	dPlayerWarehouseArray = zNewArray(mInt, cNumberPlayers, "playerWarehouseArrays");
	
	for(p=1; < cNumberPlayers - 1) {
		db = xInitDatabase("p"+p+"relics");
		aiPlanSetUserVariableInt(ARRAYS, dPlayerRelicsArray, p, db);
		xInitAddInt(db,"name");
		xInitAddInt(db,"type");
		
		db = xInitDatabase("p"+p+"warehouse");
		aiPlanSetUserVariableInt(ARRAYS, dPlayerWarehouseArray, p, db);
		xInitAddInt(db,"name");
		xInitAddInt(db,"type");
		
		db = xInitDatabase("p"+p+"characters");
		aiPlanSetUserVariableInt(ARRAYS, dPlayerCharacterArray, p, db);
		/* the three below are shared with playerCharacter */
		xInitAddInt(db,"name");
		xCharSpecialAttack = xInitAddInt(db,"specialAttack");
		xInitAddInt(db,"id");
		xInitAddInt(db,"index");
		xCharSmiteDamage = xInitAddFloat(db, "smiteDamage");
		xCharAttacking = xInitAddBool(db,"attacking");
		xCharAttackNext = xInitAddInt(db,"attackNext");
		xCharAttackTarget = xInitAddInt(db,"attackTarget");
		xCharAttackTargetIndex = xInitAddInt(db,"attackTargetIndex");
		
		xAddDatabaseBlock(dPlayerData);
	}
	
	dPlayerWolves = xInitDatabase("playerWolves");
	xInitAddInt(dPlayerWolves, "name");
	xPlayerWolfDead = xInitAddBool(dPlayerWolves, "dead");
	xPlayerWolfTimeout = xInitAddInt(dPlayerWolves, "timeout");
	
	dPlayerLasers = xInitDatabase("playerLasers");
	xInitAddInt(dPlayerLasers, "name");
	xPlayerLaserRange = xInitAddFloat(dPlayerLasers, "range");
	xPlayerLaserTimeout = xInitAddInt(dPlayerLasers, "timeout");
	
	dApplicants = xInitDatabase("applicants");
	xInitAddInt(dApplicants, "name");
	xInitAddInt(dApplicants, "player");
}

void resetCharacterCustomVars(int p = 0) {
	int db = getCharactersDB(p);
	/*
	The latest var is xCharAttackTargetIndex
	We remove all extra user variables created
	*/
	int start = xCharAttackTargetIndex - xVarNames + 1;
	for (i = start; < aiPlanGetNumberUserVariableValues(db,xVarNames)) {
		aiPlanRemoveUserVariable(db,i + xVarNames);
	}
	aiPlanSetNumberUserVariableValues(db, xVarNames, start, false);
	aiPlanSetNumberUserVariableValues(db, xMetadata, start + mVariableTypes, false);
}
