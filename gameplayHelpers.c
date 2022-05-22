const int ABILITY_READY = 0;
const int ABILITY_COOLDOWN = 1;
const int ABILITY_COST = 2;

const int ON_HIT_NONE = 0;
const int ON_HIT_JUMP = 1;
const int ON_HIT_ATTACKING = 2;
const int ON_HIT_NORMAL = 3;
const int ON_HIT_SPECIAL = 4;

const int PROJ_NONE = 0;
const int PROJ_GROUND = 1;
const int PROJ_FALLING = 2;
const int PROJ_BOUNCE = 3;
const int PROJ_REMOVE = 4;

const int STATUS_NONE = 0;
const int STATUS_STUN = 1;
const int STATUS_POISON = 2;
const int STATUS_SILENCE = 3;

string wellName = "";
string lureName = "";
string rainName = "";

bool wellIsUltimate = false;
bool rainIsUltimate = false;
bool lureIsUltimate = false;

int spysearch = 0;
int spyreset = 0;
int spyProto = 0;
int spyUnit = 1;
int spyDest = 2;
int spyScale = 3;
int spyActive = 4;

int boss = 0;
int bossUnit = 0;
int bossID = 0;
int bossPointer = 0;
int bossCooldownTime = 0;
int bossNext = 0;
int bossTimeout = 0;
int bossCount = 0;
int bossTarget = 0;
float bossScale = 0;
float bossAngle = 0;
bool bossAnim = false;

vector bossPos = vector(0,0,0);
vector bossTargetPos = vector(0,0,0);
vector bossDir = vector(0,0,0);
vector bossPrev = vector(0,0,0);

int nextproj = 0;
bool pvpDetached = false;

rule initialize_spy_database
active
highFrequency
{
	xsDisableSelf();
	xsSetContextPlayer(0);
	modularCounterInit("spyFind", 63);
	modularCounterInit("spyFound", 63);
	spyProto = zNewArray(mInt,64,"spyProto");
	spyUnit = zNewArray(mInt,64,"spyUnit");
	spyDest = zNewArray(mVector,64,"spyDest");
	spyScale = zNewArray(mVector,64,"spyScale");
	spyActive = zNewArray(mBool,64,"spyActive");
}

void advanceCooldowns(int p = 0, float seconds = 0) {
	int diff = 0;
	int prev = xGetPointer(dPlayerData);
	xSetPointer(dPlayerData,p);
	if (xGetInt(dPlayerData,xPlayerWellCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerWellReadyTime, xGetInt(dPlayerData,xPlayerWellReadyTime) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerWellReadyTime) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("well");
			trCounterAddTime("well",diff,1,wellName, -1);
		}
	}
	if (xGetInt(dPlayerData,xPlayerRainCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerRainReadyTime, xGetInt(dPlayerData,xPlayerRainReadyTime) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerRainReadyTime) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("rain");
			trCounterAddTime("rain",diff,1,rainName, -1);
		}
	}
	if (xGetInt(dPlayerData,xPlayerLureCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerLureReadyTime, xGetInt(dPlayerData,xPlayerLureReadyTime) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerLureReadyTime) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("lure");
			trCounterAddTime("lure",diff,1,lureName, -1);
		}
	}
	xSetPointer(dPlayerData, prev);
}

void gainFavor(int p = 0, float amt = 0) {
	xSetFloat(dPlayerData,xPlayerFavor,xsMax(0, xGetFloat(dPlayerData,xPlayerFavor,p) + amt),p);
	if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_DOUBLE_FAVOR) {
		xSetFloat(dPlayerData,xPlayerFavor,xsMin(200, xGetFloat(dPlayerData,xPlayerFavor,p)),p);
	} else {
		xSetFloat(dPlayerData,xPlayerFavor,xsMin(100, xGetFloat(dPlayerData,xPlayerFavor,p)),p);
	}
	trPlayerGrantResources(p,"favor", xGetFloat(dPlayerData,xPlayerFavor,p) - trPlayerResourceCount(p, "favor"));
}

void spyEffect(int unit = 0, int proto = 0, vector dest = vector(0,0,0), vector scale = vector(1,1,1)) {
	if (peekModularCounterNext("spyFind") != trQuestVarGet("spyFound")) {
		trUnitSelectClear();
		trUnitSelect(""+unit, true);
		if (trUnitAlive()) {
			int x = modularCounterNext("spyFind");
			aiPlanSetUserVariableInt(ARRAYS,spyProto,x,proto);
			aiPlanSetUserVariableInt(ARRAYS,spyUnit,x,unit);
			aiPlanSetUserVariableBool(ARRAYS,spyActive,x,true);
			aiPlanSetUserVariableVector(ARRAYS,spyDest,x,dest);
			aiPlanSetUserVariableVector(ARRAYS,spyScale,x,scale);
			trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
		}
	} else {
		debugLog("Spy buffer overflow");
	}
}

float distanceTraveled(int last = 0, float speed = 0) {
	float dist = trTimeMS() - last;
	dist = dist * 0.001 * speed;
	return(dist);
}

vector vectorSetAsCurrentPosition(vector prev = vector(0,0,0),vector dir = vector(0,0,0), float dist = 0) {
	vector v = xsVectorSet(xsVectorGetX(prev) + dist * xsVectorGetX(dir),0,xsVectorGetZ(prev) + dist * xsVectorGetZ(dir));
	return(v);
}


void silencePlayer(int p = 0) {
	if ((trQuestVarGet("p"+p+"negationCloak") == 1) && (xGetInt(dPlayerData,xPlayerDead,p) == 0)) {
		if (getBit(STATUS_SILENCE, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellstealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_SILENCE));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Silence absorbed! Your next spell will inflict Silence!</color>");
			}
		}
	} else {
		if (xGetBool(dPlayerData, xPlayerSilenced, p) == false) {
			xSetBool(dPlayerData, xPlayerSilenced, true, p);
			if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
				trSoundPlayFN("frostgiantmove1.wav","1",-1,"","");
				trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has been silenced!");
			}
			trPlayerKillAllGodPowers(p);
			if (trCurrentPlayer() == p) {
				trCounterAbort("lure");
				trCounterAbort("well");
				trCounterAbort("rain");
				trCounterAddTime("silence", -1, -9999, "SILENCED");
			}
		}
	}
}

void silenceUnit(int db = 0, float duration = 9.0, int p = 0) {
	if (db == dEnemies) {
		duration = duration * xGetFloat(dPlayerData,xPlayerSpellDuration,p);
		if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_STATUS_COOLDOWNS) {
			if ((xGetInt(dPlayerData,xPlayerClass,p) != MOONBLADE && xGetInt(dPlayerData,xPlayerClass,p) != FROSTHAMMER) || (xGetInt(db, xSilenceStatus) == 0)) {
				advanceCooldowns(p, 1);
			}
		}
		if (PvP) {
			int old = xGetPointer(dPlayerUnits);
			int index = xGetInt(dEnemies, xDoppelganger);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,true);
			xSetPointer(dPlayerUnits, index);
			silenceUnit(dPlayerUnits, duration);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,false);
			xSetPointer(dPlayerUnits, old);
			return;
		}
	} else {
		p = xGetInt(db, xPlayerOwner);
		duration = duration * xGetFloat(dPlayerData,xPlayerSilenceResistance,p);
		if (xGetInt(dPlayerData,xPlayerUnit,p) == xGetInt(db,xUnitName)) {
			silencePlayer(p);
		}
	}
	duration = duration * 1000;
	if (trTimeMS() + duration > xGetInt(db, xSilenceTimeout)) {
		xSetInt(db, xSilenceTimeout, trTimeMS() + duration);
	}
	if (xGetInt(db,xUnitName) == bossUnit) {
		if (trTimeMS() + duration > bossCooldownTime) {
			bossCooldownTime = trTimeMS() + duration;
		}
	}
	if (xGetInt(db, xSilenceStatus) == 0) {
		xSetInt(db, xSilenceStatus, 1);
		if (kbGetBlockID(""+1*xGetInt(db, xSilenceSFX)) == -1) {
			spyEffect(xGetInt(db,xUnitName),kbGetProtoUnitID("UI Range Indicator Egypt SFX"),
				xsVectorSet(db,xSilenceSFX,xGetPointer(db)));
		} else {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(db, xSilenceSFX), true);
			trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
			xUnitSelectByID(db,xUnitID);
		}
	}
}

void healUnit(int p = 0, float amt = 0, int index = -1) {
	int old = xGetPointer(dPlayerUnits);
	if (index < 0) {
		index = old;
	}
	if (xSetPointer(dPlayerUnits, index)) {
		amt = amt * xGetFloat(dPlayerData,xPlayerHealBoost,p);
		if (xGetInt(dPlayerUnits, xPoisonStatus) == 0) {
			trDamageUnit(0.0 - amt);
		}
		xSetPointer(dPlayerUnits, old);
	}
	if (xGetInt(dPlayerData, xPlayerGodBoon, p) == BOON_HEAL_FAVOR) {
		if (xGetInt(dPlayerData, xPlayerHealFavorCharges, p) > 0) {
			xSetInt(dPlayerData, xPlayerHealFavorCharges, xGetInt(dPlayerData, xPlayerHealFavorCharges, p) - 1, p);
			gainFavor(p, 1.0);
		}
	}
}

void nightriderHarvest(vector pos = vector(0,0,0)) {
	for(p=1; < ENEMY_PLAYER) {
		if ((xGetInt(dPlayerData,xPlayerClass,p) == NIGHTRIDER) &&
			(xGetInt(dPlayerData,xPlayerDead,p) == 0)) {
			if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), pos) < 144) {
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
				healUnit(p, 0.05 * xGetFloat(dPlayerData,xPlayerHealth,p) * xGetFloat(dPlayerData,xPlayerSpellDamage,p),
					xGetInt(dPlayerData,xPlayerIndex,p));
				gainFavor(p, 1);
			}
		}
	}
}

void removeEnemy() {
	if (PvP == false) {
		vector pos = xGetVector(dEnemies,xUnitPos);
		nightriderHarvest(pos);
		int bounty = xGetInt(dEnemies, xBounty);
		if (bounty > 0) {
			trQuestVarSetFromRand("rand", 1, bounty, true);
			for(p=1; <ENEMY_PLAYER) {
				if (Multiplayer) {
					xSetInt(dPlayerData,xPlayerGold,xGetInt(dPlayerData,xPlayerGold,p) + bounty, p);
					trPlayerGrantResources(p, "Gold", bounty);
					if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_MORE_GOLD) {
						xSetInt(dPlayerData,xPlayerGold,xGetInt(dPlayerData,xPlayerGold,p) + bounty, p);
						trPlayerGrantResources(p, "Gold", bounty);
					}
				}
				gainFavor(p, trQuestVarGet("rand"));
			}
		}
		if (xGetInt(dEnemies, xDropRelic) > 0) {
			if (xGetInt(dEnemies, xDropRelic) <= 10) {
				spawnRelicClosest(pos, xGetInt(dEnemies, xDropRelic));
			} else {
				spawnRelicSpecific(pos, xGetInt(dEnemies, xDropRelic));
			}
		}
		xFreeDatabaseBlock(dEnemies);
	}
}

void removePlayerUnit() {
	if (pvpDetached == false) {
		vector pos = xGetVector(dPlayerUnits,xUnitPos);
		nightriderHarvest(pos);
		if (PvP) {
			if (xSetPointer(dEnemies, xGetInt(dPlayerUnits, xDoppelganger))) {
				xFreeDatabaseBlock(dEnemies);
			} else {
				debugLog("Unable to restore doppelganger from playerUnits.");
			}
		}
		xFreeDatabaseBlock(dPlayerUnits);
	}
}

void removePlayerCharacter() {
	debugLog("Player character removed for player " + xGetInt(dPlayerCharacters, xPlayerOwner));
	xFreeDatabaseBlock(dPlayerCharacters);
}

void removePlayerSpecific(int p = 0) {
	int db = getCharactersDB(p);
	int relics = getRelicsDB(p);
	if (xGetInt(db,xUnitName) == xGetInt(dPlayerData,xPlayerUnit,p)) {
		vector pos = kbGetBlockPosition(""+xGetInt(dPlayerData,xPlayerUnit));
		trVectorQuestVarSet("dead"+p+"pos",pos);
		for(x=xGetDatabaseCount(relics); >0) {
			xDatabaseNext(relics);
			xUnitSelect(relics, xUnitName);
			if (xGetInt(relics, xRelicType) <= NORMAL_RELICS) {
				trUnitDestroy();
			} else {
				trUnitChangeProtoUnit("Relic");
				xAddDatabaseBlock(dFreeRelics, true);
				xSetInt(dFreeRelics, xUnitName, xGetInt(relics, xUnitName));
				xSetInt(dFreeRelics, xRelicType, xGetInt(relics, xRelicType));
				relicEffect(xGetInt(relics, xRelicType), p, false);
				xFreeDatabaseBlock(relics);
			}
		}
		xSetInt(dPlayerData,xPlayerDead,10 + trQuestVarGet("stage"));
		trQuestVarSet("deadPlayerCount", 1 + trQuestVarGet("deadPlayerCount"));
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
		if (PvP == false) {
			trUnitOverrideAnimation(6,0,false,false,-1);
			trSoundPlayFN("aherohasfallen.wav","1",-1,"","");
			trMessageSetText(trStringQuestVarGet("p"+p+"name") + " has fallen! Clear nearby enemies to revive them!");
			trQuestVarSet("p"+p+"reviveBeam", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
			trArmySelect(""+p+",0");
			trUnitChangeProtoUnit("Healing SFX");
		} else {
			trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dClass,xClassProto,xGetInt(dPlayerData,xPlayerClass,p))));
		}
		silencePlayer(p);
		/* NOOOO MY QUEEEEN */
		xSetInt(dPlayerData, xPlayerTether, 0);
		if (xGetInt(dPlayerData,xPlayerSimp,p) > 0) {
			int simp = xGetInt(dPlayerData,xPlayerSimp,p);
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData, xPlayerTether, simp), true);
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			xSetInt(dPlayerData,xPlayerQueen,0,simp);
			xSetInt(dPlayerData,xPlayerSimp,0,p);
		}
	}
	xFreeDatabaseBlock(db);
}

void equipRelicsAgain(int p = 0) {
	int db = getRelicsDB(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(db,xRelicName),true);
		trUnitChangeProtoUnit("Relic");
		trUnitSelectClear();
		trUnitSelect(""+xGetInt(db,xRelicName),true);
		trImmediateUnitGarrison(""+xGetInt(dPlayerData,xPlayerUnit,p));
		trMutateSelected(relicProto(xGetInt(db,xRelicType)));
		if (xGetInt(db,xRelicType) < KEY_RELICS) {
			trSetSelectedScale(0,0,-1);
			trUnitSetAnimationPath("1,0,1,1,0,0,0");
		}
	}
}

vector wallNormalVector(vector v = vector(0,0,0)) {
	vector retVal = vector(1,0,0);
	vector pos = vector(0,0,0);
	for(x=0; <4) {
		pos = xsVectorSet(xsVectorGetX(v) + trQuestVarGet("rotX"+x),0,xsVectorGetZ(v) + trQuestVarGet("rotZ"+x));
		if (terrainIsType(pos, TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
			retVal = xsVectorSet(trQuestVarGet("rotX"+x),0,trQuestVarGet("rotZ"+x));
		}
	}
	
	return(retVal);
}

/*
dist is the actual value
width is the squared value
*/
bool rayCollision(int db = 0, vector start = vector(0,0,0), vector dir = vector(1,0,0),
	float dist = 0, float width = 0) {
	vector pos = kbGetBlockPosition(""+xGetInt(db,xUnitName),true);
	float current = distanceBetweenVectors(pos, start, false);
	if (current < dist) {
		vector hitbox = xsVectorSet(xsVectorGetX(start) + current * xsVectorGetX(dir),0,
			xsVectorGetZ(start) + current * xsVectorGetZ(dir));
		if (distanceBetweenVectors(pos, hitbox, true) < width) {
			return(true);
		}
	}
	return(false);
}

/*
called after confirming that the projectile is on WALL terrain.
*/
vector getBounceDir(vector pos = vector(0,0,0), vector loc = vector(0,0,0), vector dir = vector(0,0,0)) {
	bool bounced = false;
	int xMod = 1;
	int zMod = 1;
	if (xsVectorGetX(dir) < 0) {
		xMod = -1;
	}
	if (xsVectorGetZ(dir) < 0) {
		zMod = -1;
	}
	vector ret = dir;
	/* time to bounce! */
	vector horizontal = xsVectorSetX(loc,xsVectorGetX(loc) - xMod);
	vector vertical = xsVectorSetZ(loc,xsVectorGetZ(loc) - zMod);
	if (terrainIsType(horizontal, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
		ret = xsVectorSetZ(ret, 0.0 - xsVectorGetZ(dir));
		bounced = true;
	}
	if (terrainIsType(vertical, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
		ret = xsVectorSetX(ret, 0.0 - xsVectorGetX(dir));
		bounced = true;
	}
	if (bounced == false) {
		/*
		we collided with a column. time for cool math
		a = position of the projectile in a unit square
		b = normalized vector from the contested corner to the projectile position
		*/
		vector a = (pos - (loc * 2)) / 2;
		vector b = xsVectorSet((1 - xMod) / 2,0,(1 - zMod) / 2);
		b = dir + xsVectorNormalize(b - a);
		/*
		sign is different, which means we invert the other axis
		*/
		if (xsVectorGetX(b) * xsVectorGetX(dir) < 0) {
			ret = xsVectorSetZ(ret, 0.0 - xsVectorGetZ(ret));
		} else {
			ret = xsVectorSetX(ret, 0.0 - xsVectorGetX(ret));
		}
	}
	return(ret);
}

/*
Draws a line from 'from' to 'to,' stopping at the edge of the map.
*/
vector vectorSetAsTargetVector(vector from = vector(0,0,0), vector to = vector(0,0,0), float dist = 40.0) {
	vector dir = getUnitVector(from, to, dist);
	vector target = xsVectorSet(xsVectorGetX(dir) + xsVectorGetX(from), 0, xsVectorGetZ(dir) + xsVectorGetZ(from));
	
	/*
	No out-of-bounds allowed
	*/
	float scale = 0;
	if (xsVectorGetX(target) < 0) {
		scale = xsVectorGetX(target) / (xsVectorGetX(target) - xsVectorGetX(from));
		target = xsVectorSet(0,0, xsVectorGetZ(target) + scale * (xsVectorGetZ(from) - xsVectorGetZ(target)));
	} else if (xsVectorGetX(target) > DIMENSION_X) {
		scale = (xsVectorGetX(target) - DIMENSION_X) / (xsVectorGetX(target) - xsVectorGetX(from));
		target = xsVectorSet(DIMENSION_X,0,xsVectorGetZ(target) + scale * (xsVectorGetZ(from) - xsVectorGetZ(target)));
	}
	
	if (xsVectorGetZ(target) < 0) {
		scale = xsVectorGetZ(target) / (xsVectorGetZ(target) - xsVectorGetZ(from));
		target = xsVectorSet(xsVectorGetX(target) + scale * (xsVectorGetX(from) - xsVectorGetX(target)),0,0);
	} else if (xsVectorGetZ(target) > DIMENSION_Z) {
		scale = (xsVectorGetZ(target) - DIMENSION_Z) / (xsVectorGetZ(target) - xsVectorGetZ(from));
		target = xsVectorSet(xsVectorGetX(target) + scale * (xsVectorGetX(from) - xsVectorGetX(target)),0,DIMENSION_Z);
	}
	return(target);
}

void poisonUnit(int db = 0, float duration = 0, float damage = 0, int p = 0) {
	bool targetPlayers = (db == dPlayerUnits);
	if (db == dEnemies) {
		if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_STATUS_COOLDOWNS) {
			advanceCooldowns(p, 1);
		}
		duration = duration * xGetFloat(dPlayerData,xPlayerSpellDuration,p) * xsPow(0.5, xGetInt(dPlayerData,xPlayerPoisonSpeed,p));
		damage = damage * xGetFloat(dPlayerData,xPlayerSpellDamage,p) * xsPow(2, xGetInt(dPlayerData,xPlayerPoisonSpeed,p));
		if (PvP) {
			int old = xGetPointer(dPlayerUnits);
			int index = xGetInt(dEnemies, xDoppelganger);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,true);
			xSetPointer(dPlayerUnits, index);
			poisonUnit(dPlayerUnits, duration, damage, 0);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,false);
			xSetPointer(dPlayerUnits, old);
			return;
		}
	} else {
		p = xGetInt(db, xPlayerOwner);
		duration = duration * xGetFloat(dPlayerData,xPlayerPoisonResistance, p);
	}
	duration = duration * 1000;
	if (targetPlayers && xGetBool(db, xIsHero) && (trQuestVarGet("p"+p+"negationCloak") == 1)) {
		if (getBit(STATUS_POISON, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellstealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_POISON));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Poison absorbed! Your next spell will inflict Poison!</color>");
			}
		}
	} else if (trTimeMS() + duration > xGetInt(db, xPoisonTimeout)) {
		if (xGetInt(db, xPoisonStatus) == 0) {
			if (xGetInt(db, xPoisonSFX) == 0) {
				spyEffect(xGetInt(db, xUnitName), kbGetProtoUnitID("Poison SFX"), xsVectorSet(db,xPoisonSFX,xGetPointer(db)));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(db,xPoisonSFX), true);
				trMutateSelected(kbGetProtoUnitID("Poison SFX"));
			}
			xSetInt(db,xPoisonStatus,1);
			xSetInt(db,xPoisonLast,trTimeMS());
			trQuestVarSet("poisonSound", 1);
		}
		xSetInt(db, xPoisonTimeout, trTimeMS() + duration);
		if (damage > xGetFloat(db, xPoisonDamage)) {
			xSetFloat(db, xPoisonDamage, damage);
		}
	}
	
}


void growFrostGiantsIncoming(vector pos = vector(0,0,0)) {
	for(x=xGetDatabaseCount(dFrostGiantsIncoming); >0) {
		xDatabaseNext(dFrostGiantsIncoming);
		if (unitDistanceToVector(xGetInt(dFrostGiantsIncoming,xUnitName), pos) < 100) {
			xUnitSelect(dFrostGiantsIncoming,xUnitName);
			trUnitHighlight(0.5, false);
			if (xGetFloat(dFrostGiantsIncoming, xFrostTargetSize) < 5) {
				trQuestVarSet("frostGiantIncomingSound", 1);
				xSetFloat(dFrostGiantsIncoming, xFrostTargetSize, 2 + xGetFloat(dFrostGiantsIncoming, xFrostTargetSize));
				xSetInt(dFrostGiantsIncoming, xFrostTargetTime,
					xsMax(trTimeMS(), xGetInt(dFrostGiantsIncoming, xFrostTargetTime)) + 1000);
			}
		}
	}
}

/*
protection blocks all damage.
*/
void damagePlayerUnit(float dmg = 0, int index = -1) {
	int old = xGetPointer(dPlayerUnits);
	if (index < 0) {
		index = old;
	}
	if (xSetPointer(dPlayerUnits, index)) {
		int p = xGetInt(dPlayerUnits, xPlayerOwner);
		if (PvP) {
			trQuestVarSet("protectionCount", trQuestVarGet("p"+p+"protection"));
		}
		if (trQuestVarGet("protectionCount") == 0) {
			if (PvP == false) {
				dmg = dmg - dmg * xGetFloat(dPlayerUnits, xMagicResist);
			}
			if (xGetBool(dPlayerUnits, xIsHero) && (trQuestVarGet("p"+p+"negationCloak") == 1)) {
				healUnit(p, dmg);
			} else {
				trDamageUnit(dmg);
			}
		}
		xSetPointer(dPlayerUnits, old);
	}
}


/*
Enemies have elemental resistance and weakness
*/
float damageEnemy(int p = 0, float dmg = 0, bool spell = true, float pierce = 0) {
	if (spell) {
		dmg = dmg - dmg * xGetFloat(dEnemies, xMagicResist) * (1.0 - pierce - xGetFloat(dPlayerData,xPlayerMagicPen,p));
	} else {
		dmg = dmg - dmg * xGetFloat(dEnemies, xPhysicalResist) * (1.0 - pierce);
	}
	if (xGetInt(dEnemies, xPoisonStatus) == 1) {
		dmg = dmg * (1.0 + xGetFloat(dPlayerData,xPlayerPoisonKiller,p));
	}
	float lifesteal = xGetFloat(dPlayerData,xPlayerLifesteal,p) * dmg;
	if (spell) {
		if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_SPELL_POISON) {
			poisonUnit(dEnemies, 12.0, 12.0, p);
		}
		lifesteal = 0.5 * lifesteal;
	}
	xSetFloat(dPlayerData,xPlayerLifestealTotal,xGetFloat(dPlayerData,xPlayerLifestealTotal,p) + lifesteal,p);
	if (PvP) {
		int index = xGetInt(dEnemies, xDoppelganger);
		aiPlanSetUserVariableBool(dPlayerUnits, xDirtyBit, index, true);
		damagePlayerUnit(dmg, index);
		aiPlanSetUserVariableBool(dPlayerUnits, xDirtyBit, index, false);
	} else {
		trDamageUnit(dmg);
	}
	return(dmg);
}


void stunUnit(int db = 0, float duration = 0, int p = 0, bool sound = true) {
	int index = 0;
	bool targetPlayers = (db == dPlayerUnits);
	if (db == dEnemies) {
		if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_STATUS_COOLDOWNS) {
			advanceCooldowns(p, 1);
		}
		duration = duration * xGetFloat(dPlayerData,xPlayerSpellDuration,p);
		xUnitSelectByID(db,xUnitID);
		if (xGetFloat(dPlayerData,xPlayerStunDamage,p) > 0) {
			damageEnemy(p, xGetFloat(dPlayerData,xPlayerHealth,p) * xGetFloat(dPlayerData,xPlayerStunDamage,p), true);
		}
		if (PvP) {
			int old = xGetPointer(dPlayerUnits);
			index = xGetInt(dEnemies, xDoppelganger);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,true);
			xSetPointer(dPlayerUnits, index);
			stunUnit(dPlayerUnits, duration, 0, sound);
			aiPlanSetUserVariableBool(dPlayerUnits,xDirtyBit,index,false);
			xSetPointer(dPlayerUnits, old);
			return;
		}
	} else {
		p = xGetInt(db, xPlayerOwner);
		duration = duration * xGetFloat(dPlayerData,xPlayerStunResistance,p);
	}
	duration = duration * 1000;
	if (targetPlayers && (xGetBool(db, xIsHero)) && (trQuestVarGet("p"+p+"negationCloak") == 1)) {
		if (getBit(STATUS_STUN, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellStealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_STUN));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Stun absorbed! Your next spell will inflict Stun!</color>");
			}
		}
	} else {
		if (trTimeMS() + duration > xGetInt(db, xStunTimeout)) {
			if (xGetInt(db, xStunStatus) == 0) {
				trQuestVarSet("stunSound", 1);
				if (boss == 3) {
					growFrostGiantsIncoming(kbGetBlockPosition(""+xGetInt(db,xUnitName)));
				}
				index = xAddDatabaseBlock(dStunnedUnits);
				xSetInt(dStunnedUnits,xUnitName,xGetInt(db,xUnitName),index);
				xSetInt(dStunnedUnits,xPlayerOwner,xGetInt(db,xPlayerOwner),index);
				xSetInt(dStunnedUnits,xUnitID,xGetInt(db,xUnitID),index);
				xSetInt(dStunnedUnits,xStunnedProto,kbGetUnitBaseTypeID(kbGetBlockID(""+xGetInt(db,xUnitName), true)),index);
				if (xGetInt(db, xStunSFX) == 0) {
					spyEffect(xGetInt(db,xUnitName), kbGetProtoUnitID("Shockwave stun effect"), xsVectorSet(db,xStunSFX,xGetPointer(db)));
				} else {
					xUnitSelect(db,xStunSFX);
					trMutateSelected(kbGetProtoUnitID("Shockwave stun effect"));
					xUnitSelectByID(db,xUnitID);
				}
				xSetInt(db, xStunStatus, index);
			} else if (sound) {
				trQuestVarSet("stunSound", 1);
			}
			xSetInt(db,xStunTimeout,trTimeMS() + duration);
		}
	}
	
}

void processLaunchedUnit() {
	xDatabaseNext(dLaunchedUnits);
	vector dest = xGetVector(dLaunchedUnits,xLaunchedDest);
	xUnitSelectByID(dLaunchedUnits,xUnitID);
	if (trUnitAlive() == false ||
		unitDistanceToVector(xGetInt(dLaunchedUnits,xUnitName), dest) < 4 ||
		trTimeMS() > xGetInt(dLaunchedUnits, xLaunchedTimeout)) {
		if (trUnitAlive()) {
			int p = xGetInt(dLaunchedUnits,xPlayerOwner);
			int db = xGetInt(dLaunchedUnits,xLaunchedDB);
			xSetBool(db, xLaunched, false, xGetInt(dLaunchedUnits,xLaunchedIndex));
			trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dLaunchedUnits,xStunnedProto)));
			xUnitSelect(dLaunchedUnits,xUnitName);
			trMutateSelected(xGetInt(dLaunchedUnits,xStunnedProto));
			if ((p < ENEMY_PLAYER) && (xGetInt(dLaunchedUnits,xUnitName) == xGetInt(dPlayerData,xPlayerUnit,p))) {
				equipRelicsAgain(p);
				xSetBool(dPlayerData,xPlayerLaunched,false,p);
			}
			if (xGetInt(dLaunchedUnits,xUnitName) == bossUnit) {
				xUnitSelect(dLaunchedUnits,xUnitName);
				trSetSelectedScale(bossScale,bossScale,bossScale);
			}
			if (xGetBool(dLaunchedUnits, xLaunchedStun)) {
				int index = xGetPointer(db);
				if (xSetPointer(db, xGetInt(dLaunchedUnits,xLaunchedIndex))) {
					stunUnit(db, 3.0);
					xSetPointer(db, index);
				}
			}
			if (PvP && (db == dEnemies)) {
				xSetBool(dPlayerUnits, xLaunched, false, xGetInt(db, xDoppelganger));
			}
		} else {
			trUnitChangeProtoUnit(kbGetProtoUnitName(xGetInt(dLaunchedUnits,xStunnedProto)));
		}
		xUnitSelect(dLaunchedUnits,xLaunchedCar);
		trUnitChangeProtoUnit("Dust Small");
		xFreeDatabaseBlock(dLaunchedUnits);
	}
}

void launchUnit(int db = 0, vector dest = vector(0,0,0)) {
	bool hitWall = false;
	if (xGetBool(db, xLaunched) == false) {
		xSetBool(db, xLaunched, true);
		if (PvP && (db == dEnemies)) {
			xSetBool(dPlayerUnits, xLaunched, true, xGetInt(dEnemies, xDoppelganger));
		}
		int type = kbGetUnitBaseTypeID(kbGetBlockID(""+xGetInt(db,xUnitName)));
		int p = xGetInt(db,xPlayerOwner);
		xUnitSelectByID(db,xUnitID);
		trUnitChangeProtoUnit("Transport Ship Greek");
		
		vector start = kbGetBlockPosition(""+xGetInt(db,xUnitName));
		vector dir = getUnitVector(start,dest);
		
		int next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
		trUnitSelectClear();
		trUnitSelect(""+next,true);
		trImmediateUnitGarrison(""+xGetInt(db,xUnitName));
		trUnitChangeProtoUnit("Dwarf");
		
		trUnitSelectClear();
		trUnitSelect(""+next,true);
		trSetUnitOrientation(dir, vector(0,1,0), true);
		trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
		
		xUnitSelectByID(db,xUnitID);
		trMutateSelected(type);
		trUnitOverrideAnimation(24,0,true,true,-1);
		trMutateSelected(kbGetProtoUnitID("Relic"));
		trImmediateUnitGarrison(""+next);
		trMutateSelected(type);
		if (xGetInt(db,xUnitName) == bossUnit) {
			trSetSelectedScale(bossScale,bossScale,bossScale);
		}
		
		float dist = distanceBetweenVectors(start, dest, false);
		for(x=0; < dist / 2) {
			vector nextpos = xsVectorSet(xsVectorGetX(start) + 2.0 * xsVectorGetX(dir),0,
				xsVectorGetZ(start) + 2.0 * xsVectorGetZ(dir));
			if (terrainIsType(vectorToGrid(nextpos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hitWall = true;
				break;
			} else {
				start = nextpos;
			}
		}
		
		trUnitSelectClear();
		trUnitSelect(""+next,true);
		trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
		trUnitMoveToPoint(xsVectorGetX(start),0,xsVectorGetZ(start),-1,false);
		
		xSetPointer(dLaunchedUnits, xAddDatabaseBlock(dLaunchedUnits));
		xSetInt(dLaunchedUnits,xUnitName,xGetInt(db,xUnitName));
		xSetInt(dLaunchedUnits,xPlayerOwner,xGetInt(db,xPlayerOwner));
		xSetInt(dLaunchedUnits,xUnitID,xGetInt(db,xUnitID));
		xSetInt(dLaunchedUnits, xLaunchedCar, next);
		xSetInt(dLaunchedUnits,xStunnedProto,type);
		xSetInt(dLaunchedUnits,xLaunchedIndex,xGetPointer(db));
		xSetVector(dLaunchedUnits,xLaunchedDest,start);
		xSetInt(dLaunchedUnits,xLaunchedTimeout, trTimeMS() + 1100 * dist / 15);
		xSetBool(dLaunchedUnits,xLaunchedStun, hitWall);
		xSetInt(dLaunchedUnits, xLaunchedDB, db);
		
		
		if ((p < ENEMY_PLAYER) && (xGetInt(db,xUnitName) == xGetInt(dPlayerData,xPlayerUnit,p))) {
			xSetBool(dPlayerData,xPlayerLaunched,true,p);
			int relics = getRelicsDB(p);
			for(x=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				xUnitSelect(relics,xRelicName);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
}

void stunsAndPoisons(int db = 0) {
	if (xGetInt(db, xPoisonStatus) == 1) {
		float amt = trTimeMS() - xGetInt(db, xPoisonLast);
		if (trTimeMS() > xGetInt(db, xPoisonTimeout)) {
			xSetInt(db, xPoisonStatus, 0);
			xUnitSelect(db, xPoisonSFX);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
		} else if (amt > 500) {
			trDamageUnit(amt * xGetFloat(db, xPoisonDamage) * 0.001);
			xSetInt(db, xPoisonLast, trTimeMS());
		}
	}
	if (xGetInt(db, xStunStatus) >= 1) {
		if (trTimeMS() > xGetInt(db, xStunTimeout)) {
			xUnitSelectByID(db,xUnitID);
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xUnitSelect(db, xStunSFX);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
			xFreeDatabaseBlock(dStunnedUnits, xGetInt(db, xStunStatus));
			xSetInt(db, xStunStatus, 0);
		}
	}
	if (xGetInt(db, xSilenceStatus) == 1) {
		if (trTimeMS() > xGetInt(db, xSilenceTimeout)) {
			xUnitSelect(db, xSilenceSFX);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
			xSetInt(db, xSilenceStatus, 0);
		}
	}
}

void OnHit(int p = 0, int index = 0, bool magic = false) {
	gainFavor(p, xGetInt(dPlayerData,xPlayerFavorFromAttacks,p));
	if (xGetInt(dPlayerData, xPlayerGodBoon, p) == BOON_ATTACK_PROLONGS_STUN) {
		int pid = 0;
		if (PvP) {
			pid = xGetInt(dEnemies, xDoppelganger, index);
		}
		for(x=xStunStatus; <= xSilenceStatus) {
			if (xGetInt(dEnemies, x, index) > 0) {
				if (PvP) {
					xSetInt(dPlayerUnits, x + 3, xGetInt(dPlayerUnits, x + 3, pid) + 1000, pid);
				} else {
					xSetInt(dEnemies, x + 3, xGetInt(dEnemies, x + 3, index) + 1000, index);
				}
			}
		}
	}
	if (xGetFloat(dPlayerData,xPlayerCleave) > 0) {
		int prev = xGetPointer(dEnemies);
		if (index != prev) {
			xSetPointer(dEnemies, index);
		}
		vector pos = kbGetBlockPosition(""+xGetInt(dEnemies, xUnitName), true);
		for(x=xGetDatabaseCount(dEnemies); >1) {
			xDatabaseNext(dEnemies);
			if (unitDistanceToVector(xGetInt(dEnemies,xUnitName), pos) < 16.0) {
				xUnitSelectByID(dEnemies,xUnitID);
				damageEnemy(p, xGetFloat(dPlayerData,xPlayerCleave,p) * xGetFloat(dPlayerData,xPlayerAttack,p), magic);
			}
		}
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("1,0");
		trDamageUnitPercent(100);
		trUnitChangeProtoUnit("Meteorite");
		xSetPointer(dEnemies,prev);
	}
}

int CheckOnHit(int p = 0, bool onhit = true) {
	int status = ON_HIT_NONE;
	int class = xGetInt(dPlayerData,xPlayerClass,p);
	int db = getCharactersDB(p);
	int target = 0;
	int simp = xGetInt(dPlayerData,xPlayerSimp,p);
	int id = xGetInt(db,xUnitID);
	int action = kbUnitGetAnimationActionType(id);
	float amt = 0;
	if (action == 32) {
		status = ON_HIT_JUMP;
	} else if (xGetBool(db, xCharAttacking) == false) {
		if ((action == 12) || (action == 6)) {
			xsSetContextPlayer(p);
			target = kbUnitGetTargetUnitID(id);
			xsSetContextPlayer(0);
			xSetInt(db, xCharAttackTarget, target);
			xSetBool(db, xCharAttacking, true);
			xSetInt(db, xCharAttackNext, trTimeMS() + xGetInt(dPlayerData,xPlayerFirstDelay,p));
			xSetInt(db, xCharAttackTargetIndex, 0);
		}
	} else {
		if ((action == 12) || (action == 6)) {
			if (trTimeMS() > xGetInt(db, xCharAttackNext)) {
				
				status = ON_HIT_NORMAL;
				xSetInt(db, xCharAttackNext, xGetInt(db, xCharAttackNext) + xGetInt(dPlayerData,xPlayerNextDelay,p));
				/* only melee characters have special attacks */
				if (action == 6) {
					xSetInt(db,xCharSpecialAttack,xGetInt(db,xCharSpecialAttack) - 1);
					if (xGetInt(db, xCharSpecialAttack) <= 0) {
						xSetInt(db, xCharSpecialAttack, xGetInt(dPlayerData,xPlayerSpecialAttackCooldown,p));
						status = ON_HIT_SPECIAL;
					}
				}
				/* simp benefits */
				if (simp > 0) {
					simp = trQuestVarGet("p"+simp+"characters");
					for(x=xGetDatabaseCount(simp); >0) {
						xDatabaseNext(simp);
						xSetInt(simp, xCharSpecialAttack, xGetInt(simp, xCharSpecialAttack) - 1);
					}
				}
				/* get the target */
				if (xGetInt(db, xCharAttackTargetIndex) == 0) {
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						if (xGetInt(dEnemies,xUnitID) == xGetInt(db, xCharAttackTarget)) {
							xSetInt(db, xCharAttackTargetIndex, xGetPointer(dEnemies));
							xSetInt(dPlayerData,xPlayerPoisonKillerActive, xGetInt(dEnemies, xPoisonStatus),p);
							break;
						}
					}
					if (xGetInt(db, xCharAttackTargetIndex) == 0) {
						debugLog("Player " + p +" target not found: " + xGetInt(db, xCharAttackTarget));
					}
				} else {
					xSetInt(dPlayerData,xPlayerPoisonKillerActive, xGetInt(dEnemies, xPoisonStatus, xGetInt(db,xCharAttackTargetIndex)),p);
				}
				/* lifesteal */
				amt = xGetFloat(dPlayerData,xPlayerLifesteal,p) * xGetFloat(dPlayerData,xPlayerAttack,p);
				if (xGetInt(dPlayerData,xPlayerPoisonKillerActive) == 1) {
					amt = amt * (1.0 + xGetFloat(dPlayerData,xPlayerPoisonKiller,p));
				}
				xSetFloat(dPlayerData,xPlayerLifestealTotal, xGetFloat(dPlayerData,xPlayerLifestealTotal,p) + amt,p);
				if (onhit) {
					OnHit(p, xGetInt(db,xCharAttackTargetIndex));
				}
			} else {
				xsSetContextPlayer(p);
				target = kbUnitGetTargetUnitID(id);
				xsSetContextPlayer(0);
				if (xGetInt(db, xCharAttackTarget) != target) {
					xSetInt(db, xCharAttackNext, trTimeMS() + xGetInt(dPlayerData,xPlayerFirstDelay,p));
					xSetInt(db,xCharAttackTarget,target);
					xSetInt(db,xCharAttackTargetIndex,0);
				}
				status = ON_HIT_ATTACKING;
			}
		} else {
			xSetBool(db,xCharAttacking,false);
			xSetInt(dPlayerData,xPlayerPoisonKillerActive,0,p);
		}
	}
	return(status);
}

/* call this at the end of each class's function */
void poisonKillerBonus(int p = 0) {
	int class = xGetInt(dPlayerData,xPlayerClass,p);
	float amt = xGetFloat(dPlayerData,xPlayerPoisonKiller,p) * xGetFloat(dPlayerData,xPlayerAttack,p);
	amt = amt * xGetInt(dPlayerData,xPlayerPoisonKillerActive,p);
	zSetProtoUnitStat(kbGetProtoUnitName(xGetInt(dClass,xClassProto,class)),p,27,
		amt+xGetFloat(dPlayerData,xPlayerAttack,p));
	zSetProtoUnitStat(kbGetProtoUnitName(xGetInt(dClass,xClassProto,class)),p,31,
		amt+xGetFloat(dPlayerData,xPlayerAttack,p));
}

float calculateDecay(int p = 0, float decay = 0) {
	if (xGetInt(dPlayerData,xPlayerGodBoon,p) == BOON_DECAY_HALVED) {
		decay = decay * 0.5;
	}
	return(decay / xGetFloat(dPlayerData,xPlayerSpellDuration,p));
}

/*
this performs a yDatabaseNext on the database and keeps projectiles moving. It will return
an enumeration giving you the state of the projectile.
PROJ_REMOVE = the projectile was removed because it was somehow destroyed
PROJ_BOUNCE = the projectile has just turned into kronny and is falling back down to the ground
PROJ_GROUND = the projectile has hit the ground and will turn into kronny
PROJ_FALLING = the projectile is falling towards the ground. This is the recommended state for
adding any additional computation
*/
int processGenericProj(int db = 0) {
	int id = 0;
	int action = PROJ_NONE;
	float scale = 0;
	xDatabaseNext(db);
	xUnitSelectByID(db,xUnitID);
	id = kbGetBlockID(""+xGetInt(db,xUnitName));
	int yeehaw = xGetInt(db, xProjYeehaw);
	if (id == -1) {
		xFreeDatabaseBlock(db);
		action = PROJ_REMOVE;
	} else if (yeehaw == 1) {
		trMutateSelected(xGetInt(db,xProjProto));
		trUnitOverrideAnimation(xGetInt(db,xProjAnim),0,true,true,-1);
		scale = xGetFloat(db,xProjScale);
		trSetSelectedScale(scale,scale,scale);
		xSetInt(db,xProjYeehaw,0);
		action = PROJ_BOUNCE;
	} else if (yeehaw == 2) {
		/* first time search */
		xSetInt(db, xProjYeehaw, 1);
	} else {
		vector pos = kbGetBlockPosition(""+xGetInt(db,xUnitName));
		if (xsVectorGetY(pos) < worldHeight + 0.5 || xGetInt(db,xProjYeehaw) == 99) {
			action = PROJ_GROUND;
			vector dir = xGetVector(db, xProjDir);
			zSetProtoUnitStat("Kronny Flying", xGetInt(db,xPlayerOwner), 1, xGetFloat(db,xProjSpeed));
			trUnitChangeProtoUnit("Kronny Flying");
			xUnitSelectByID(db,xUnitID);
			trDamageUnitPercent(-100);
			trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
			trSetUnitOrientation(dir, vector(0,1,0), true);
			trSetSelectedScale(0,0.0-xGetFloat(db, xProjHeight),0);
			trDamageUnitPercent(100);
			xSetInt(db,xProjYeehaw,1);
		} else {
			action = PROJ_FALLING;
		}
	}
	
	return(action);
}

/*
db = name of the ydatabase to put the projectile in
start = the name of the start trVector
dir = the name of the direction vector. Must be a normal vector (length 1) starting from the origin
height = the negative height of the object (if you want something falling from a higher location, input a negative number here)
*/
int addGenericProj(int db = 0, vector start = vector(0,0,0), vector dir = vector(0,0,0),
	int p = -1, float speed = -1, float height = -1) {
	int next = trGetNextUnitScenarioNameNumber();
	int index = xAddDatabaseBlock(db, true);
	xSetInt(db, xUnitName, next);
	xSetVector(db,xProjDir, dir);
	
	if (speed == -1) {
		speed = xGetFloat(db, xProjSpeed, 0);
	} else {
		xSetFloat(db, xProjSpeed, speed);
	}
	if (height == -1) {
		height = xGetFloat(db, xProjHeight, 0);
	} else {
		xSetFloat(db, xProjHeight, height);
	}
	if (p == -1) {
		p = xGetInt(db, xPlayerOwner, 0);
	} else {
		xSetInt(db, xPlayerOwner, p);
	}
	
	trArmyDispatch(""+p+",0", "Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
	trUnitSelectClear();
	trUnitSelect(""+next,true);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	zSetProtoUnitStat("Kronny Flying", p, 1, speed);
	trSetUnitOrientation(dir, vector(0,1,0), true);
	trSetSelectedScale(0, 0.0 - height, 0);
	trDamageUnitPercent(100);
	xSetInt(db, xUnitID, kbGetBlockID(""+next));
	return(index);
}

int initGenericProj(string name = "", int proto = 0, int anim = 0, float speed = 10.0,
	float height = 4.5, float scale = 0, int p = 0, bool hitbox = false, int count = 0) {
	int db = xInitDatabase(name,count);
	xInitAddInt(db, "name");
	xInitAddInt(db, "player", p);
	xInitAddInt(db, "id");
	xProjProto = xInitAddInt(db, "proto", proto);
	xProjYeehaw = xInitAddInt(db, "yeehaw", 2);
	xProjAnim = xInitAddInt(db, "anim", anim);
	xProjHeight = xInitAddFloat(db, "height", height);
	xProjSpeed = xInitAddFloat(db, "speed", speed);
	xProjScale = xInitAddFloat(db, "scale", scale);
	xProjDir = xInitAddVector(db, "dir");
	if (hitbox) {
		xProjPrev = xInitAddVector(db,"prev");
	}
	return(db);
}

int dSphinxes = 0;
int dDryads = 0;
int dMedusas = 0;
int dMountainGiants = 0;
int dFrostGiants = 0;
int dValkyries = 0;
int dBallistas = 0;
int dFireSiphons = 0;
int dBattleBoars = 0;
int dAutomatons = 0;
int dScarabs = 0;
int dSatyrs = 0;
int dAvengers = 0;
int dScorpionMen = 0;
int dMummies = 0;
int dNereids = 0;
int dHydras = 0;
int dKrakens = 0;
int dLampades = 0;
int dEinherjars = 0;
int dLightningStatues = 0;
int dManticores = 0;
int dFireGiants = 0;

int dAutomatonBombs = 0;

int xSpecialIndex = 0;
int xSpecialStep = 0;
int xSpecialNext = 0;
int xSpecialTarget = 0;

int dDelayLasers = 0;
int xLaserDir = 0;
int xLaserPhase = 0;
int xLaserNext = 0;
int xLaserDist = 0;

int dBallistaShots = 0;
int xBallistaShot1 = 0;
int xBallistaShot2 = 0;

int dYeebLightning = 0;
int xTimeout = 0;

int dYeebLightningEnd = 0;

int dBarrages = 0;
int xBarragePos = 0;
int xBarrageDir = 0;
int xBarrageCount = 0;

int dMedusaBalls = 0;
int xMedusaBallTarget = 0;
int xMedusaBallBounces = 0;

int dMummyBalls = 0;
int xProjType = 0;

int dAvengerProj = 0;
int xAvengerProjDist = 0;
int xAvengerProjUnit = 0;
int xAvengerProjIndex = 0;

int xMummyStart = 0;
int xMummyDir = 0;

int dManticoreProj = 0;
int dFireGiantProj = 0;
int xFireGiantProjOwner = 0;
int dFireGiantBalls = 0;

int dYeebLightningBalls = 0;

int dTartarianSpawns = 0;

int dTartarianEggs = 0;
int xTartarianEggTimeout = 0;

int dAmbushRooms = 0;
int xAmbushRoomType = 0;
int xAmbushRoomPos = 0;

int initSpecialDatabase(string name = "", bool step = true) {
	int db = xInitDatabase(name);
	xInitAddInt(db, "name");
	xInitAddInt(db, "player");
	xInitAddInt(db, "id");
	xSpecialIndex = xInitAddInt(db, "index");
	xSpecialStep = xInitAddInt(db, "step");
	if (step) {
		xSpecialNext = xInitAddInt(db,"next");
	}
	return(db);
}

void addSpecialToDatabase(int db = 0,int name = 0, int from = 0, int p = 0) {
	xSetPointer(db, xAddDatabaseBlock(db));
	xSetInt(db, xUnitName,name);
	xSetInt(db, xPlayerOwner,p);
	xSetInt(db, xUnitID, xGetInt(from,xUnitID));
	xSetInt(db, xSpecialIndex, xGetNewestPointer(from));
}

rule initialize_special_database
active
highFrequency
{
	xsDisableSelf();
	dSphinxes = initSpecialDatabase("Sphinxes");
	
	dDryads = initSpecialDatabase("Dryads", false);
	xInitAddVector(dDryads,"pos");
	
	dMedusas = initSpecialDatabase("Medusas");
	xSpecialTarget = xInitAddInt(dMedusas, "target");
	
	dMountainGiants = initSpecialDatabase("MountainGiants");
	xInitAddVector(dMountainGiants, "target");
	
	dFrostGiants = initSpecialDatabase("FrostGiants");
	xInitAddInt(dFrostGiants,"target");
	
	dValkyries = initSpecialDatabase("Valkyries",false);
	xInitAddInt(dValkyries,"sfx");
	
	dBallistas = initSpecialDatabase("Ballistas",false);
	
	dFireSiphons = initSpecialDatabase("FireSiphons",false);
	
	dBattleBoars = initSpecialDatabase("BattleBoars");
	xInitAddInt(dBattleBoars,"target");
	
	dAutomatons = initSpecialDatabase("Automatons",false);
	
	dScarabs = initSpecialDatabase("Scarabs", false);
	xInitAddVector(dScarabs,"pos");
	
	dSatyrs = initSpecialDatabase("Satyrs");
	
	dAvengers = initSpecialDatabase("Avengers");
	xInitAddInt(dAvengers,"projIndex");
	
	dScorpionMen = initSpecialDatabase("ScorpionMen");
	xInitAddInt(dScorpionMen,"target");
	
	dMummies = initSpecialDatabase("Mummies");
	xMummyStart = xInitAddVector(dMummies,"start");
	xMummyDir = xInitAddVector(dMummies,"dir");
	
	dNereids = initSpecialDatabase("Nereids");
	xInitAddVector(dNereids,"target");
	
	dHydras = initSpecialDatabase("Hydras",false);
	
	dKrakens = initSpecialDatabase("Krakens");
	xInitAddVector(dKrakens, "target");
	
	dLampades = initSpecialDatabase("SkyWitches");
	xInitAddVector(dLampades,"target");
	
	dEinherjars = initSpecialDatabase("Einherjars");
	
	dLightningStatues = initSpecialDatabase("LightningStatues");
	
	dManticores = initSpecialDatabase("Manticores");

	dTartarianSpawns = initSpecialDatabase("TartarianSpawns", false);
	xInitAddVector(dTartarianSpawns,"pos");

	dTartarianEggs = xInitDatabase("TartarianEggs");
	xInitAddInt(dTartarianEggs, "name");
	xInitAddInt(dTartarianEggs, "player");
	xTartarianEggTimeout = xInitAddInt(dTartarianEggs, "timeout");
	
	dDelayLasers = xInitDatabase("delayLasers");
	xInitAddInt(dDelayLasers,"name");
	xInitAddInt(dDelayLasers,"player");
	xLaserDir = xInitAddVector(dDelayLasers, "dir");
	xLaserPhase = xInitAddInt(dDelayLasers, "phase");
	xLaserNext = xInitAddInt(dDelayLasers, "next");
	xLaserDist = xInitAddFloat(dDelayLasers, "dist");
	
	dBallistaShots = xInitDatabase("ballistaShots");
	xInitAddInt(dBallistaShots,"name");
	xInitAddInt(dBallistaShots,"player");
	xTimeout = xInitAddInt(dBallistaShots,"timeout");
	xBallistaShot1 = xInitAddInt(dBallistaShots,"shot1");
	xBallistaShot2 = xInitAddInt(dBallistaShots,"shot2");
	
	dYeebLightning = xInitDatabase("yeebLightning", 150);
	xInitAddInt(dYeebLightning,"name");
	xInitAddInt(dYeebLightning,"player");
	xInitAddInt(dYeebLightning,"timeout");
	
	dYeebLightningEnd = xInitDatabase("yeebLightningEnd", 150);
	xInitAddInt(dYeebLightningEnd,"name");
	xInitAddInt(dYeebLightningEnd,"player");
	
	dYeebLightningBalls = initGenericProj("yeebLightningBalls",kbGetProtoUnitID("Arkantos God"),26,10.0,5.0,0.0,0,true);
	xInitAddInt(dYeebLightningBalls,"bounces");
	
	dAutomatonBombs = xInitDatabase("automatonBombs");
	xInitAddInt(dAutomatonBombs,"name");
	xInitAddInt(dAutomatonBombs,"player");
	xInitAddInt(dAutomatonBombs,"timeout");
	
	dMedusaBalls = initGenericProj("medusaBalls",kbGetProtoUnitID("Curse SFX"),2,4.0,4.5,0.0);
	xMedusaBallTarget = xInitAddInt(dMedusaBalls,"target");
	xMedusaBallBounces = xInitAddInt(dMedusaBalls,"bounces");
	
	dMummyBalls = initGenericProj("mummyBalls",kbGetProtoUnitID("Kronny Birth SFX"),2,8.0,4.5,0.0,ENEMY_PLAYER,true);
	xProjDist = xInitAddFloat(dMummyBalls,"dist");
	xProjType = xInitAddInt(dMummyBalls,"type");
	
	dBarrages = xInitDatabase("barrages");
	xInitAddInt(dBarrages,"name");
	xInitAddInt(dBarrages,"player");
	xInitAddInt(dBarrages,"timeout");
	xBarragePos = xInitAddVector(dBarrages,"pos");
	xBarrageDir = xInitAddVector(dBarrages,"dir");
	xBarrageCount = xInitAddInt(dBarrages,"count");
	
	dAvengerProj = initGenericProj("avengerProj",kbGetProtoUnitID("Avenger"),39,10.0,4.5,1.0,ENEMY_PLAYER,true);
	xProjDist = xInitAddFloat(dAvengerProj,"dist");
	xAvengerProjDist = xInitAddFloat(dAvengerProj,"maxDist");
	xAvengerProjUnit = xInitAddInt(dAvengerProj,"rider");
	xAvengerProjIndex = xInitAddInt(dAvengerProj,"index");

	dFireGiants = initSpecialDatabase("fireGiants");
	xInitAddVector(dFireGiants, "dir");

	dFireGiantProj = initGenericProj("fireGiantProj",kbGetProtoUnitID("Meteorite death"),-1,20.0,4.0,0,0,true);
	xFireGiantProjOwner = xInitAddInt(dFireGiantProj, "owner"); // for some reason, only p0 meteorite death works
	dFireGiantBalls = initGenericProj("fireGiantBalls",kbGetProtoUnitID("Fire Giant"),19,10.0,4.5,0,ENEMY_PLAYER,true);
	xProjDist = xInitAddFloat(dFireGiantBalls,"dist");
	
	dAmbushRooms = xInitDatabase("ambushRooms");
	xAmbushRoomType = xInitAddInt(dAmbushRooms, "type");
	xAmbushRoomPos = xInitAddVector(dAmbushRooms,"pos");
	
}


void activateSpecialUnit(int name = 1, int db = 0, int proto = 0, int p = 0) {
	int index = 0;
	switch(proto)
	{
		case kbGetProtoUnitID("Sphinx"):
		{
			addSpecialToDatabase(dSphinxes,name,db,p);
		}
		case kbGetProtoUnitID("Dryad"):
		{
			addSpecialToDatabase(dDryads,name,db,p);
		}
		case kbGetProtoUnitID("Wadjet"):
		{
			addSpecialToDatabase(dDryads,name,db,p);
		}
		case kbGetProtoUnitID("Medusa"):
		{
			addSpecialToDatabase(dMedusas,name,db,p);
		}
		case kbGetProtoUnitID("Mountain Giant"):
		{
			addSpecialToDatabase(dMountainGiants,name,db,p);
		}
		case kbGetProtoUnitID("Frost Giant"):
		{
			addSpecialToDatabase(dFrostGiants,name,db,p);
		}
		case kbGetProtoUnitID("Valkyrie"):
		{
			xSetFloat(db,xMagicResist,1,xGetNewestPointer(db));
			addSpecialToDatabase(dValkyries,name,db,p);
			spyEffect(name,kbGetProtoUnitID("Vortex Finish Linked"),
				xsVectorSet(dValkyries,xSpecialNext,xGetNewestPointer(dValkyries)));
		}
		case kbGetProtoUnitID("Ballista"):
		{
			addSpecialToDatabase(dBallistas,name,db,p);
		}
		case kbGetProtoUnitID("Colossus"):
		{
			xSetFloat(db,xMagicResist,1,xGetNewestPointer(db));
			addSpecialToDatabase(dValkyries,name,db,p);
			spyEffect(name,kbGetProtoUnitID("Vortex Finish Linked"),
				xsVectorSet(dValkyries,xSpecialNext,xGetNewestPointer(dValkyries)));
		}
		case kbGetProtoUnitID("Fire Siphon"):
		{
			addSpecialToDatabase(dFireSiphons,name,db,p);
		}
		case kbGetProtoUnitID("Battle Boar"):
		{
			addSpecialToDatabase(dBattleBoars,name,db,p);
		}
		case kbGetProtoUnitID("Automaton SPC"):
		{
			addSpecialToDatabase(dAutomatons,name,db,p);
		}
		case kbGetProtoUnitID("Behemoth"):
		{
			xSetFloat(db,xMagicResist,-1,xGetNewestPointer(db));
		}
		case kbGetProtoUnitID("Scarab"):
		{
			xSetFloat(db,xMagicResist,-1,xGetNewestPointer(db));
			addSpecialToDatabase(dScarabs,name,db,p);
		}
		case kbGetProtoUnitID("Satyr"):
		{
			addSpecialToDatabase(dSatyrs,name,db,p);
		}
		case kbGetProtoUnitID("Avenger"):
		{
			addSpecialToDatabase(dAvengers,name,db,p);
		}
		case kbGetProtoUnitID("Scorpion Man"):
		{
			addSpecialToDatabase(dScorpionMen,name,db,p);
		}
		case kbGetProtoUnitID("Mummy"):
		{
			addSpecialToDatabase(dMummies,name,db,p);
		}
		case kbGetProtoUnitID("Nereid"):
		{
			addSpecialToDatabase(dNereids,name,db,p);
		}
		case kbGetProtoUnitID("Hydra"):
		{
			addSpecialToDatabase(dHydras,name,db,p);
			xSetInt(dHydras,xSpecialStep,trTime());
		}
		case kbGetProtoUnitID("Kraken"):
		{
			addSpecialToDatabase(dKrakens,name,db,p);
		}
		case kbGetProtoUnitID("Lampades"):
		{
			addSpecialToDatabase(dLampades,name,db,p);
		}
		case kbGetProtoUnitID("Einheriar"):
		{
			addSpecialToDatabase(dEinherjars,name,db,p);
		}
		case kbGetProtoUnitID("Statue of Lightning"):
		{
			addSpecialToDatabase(dLightningStatues,name,db,p);
		}
		case kbGetProtoUnitID("Fire Giant"):
		{
			addSpecialToDatabase(dFireGiants,name,db,p);
		}
		case kbGetProtoUnitID("Manticore"):
		{
			addSpecialToDatabase(dManticores,name,db,p);
		}
		case kbGetProtoUnitID("Tartarian Gate Spawn"):
		{
			addSpecialToDatabase(dTartarianSpawns,name,db,p);
		}
		case kbGetProtoUnitID("Troll"):
		{
			addSpecialToDatabase(dHydras,name,db,p);
			xSetInt(dHydras,xSpecialStep,trTime());
		}
	}
}

int activatePlayerUnit(int name = 0, int p = 0, int proto = 0, float decay = 0) {
	int id = kbGetBlockID(""+name,true);
	int index = xAddDatabaseBlock(dPlayerUnits);
	xSetPointer(dPlayerUnits,index);
	xSetInt(dPlayerUnits,xUnitName,name);
	xSetInt(dPlayerUnits,xPlayerOwner,p);
	xSetInt(dPlayerUnits,xUnitID,id);
	xSetFloat(dPlayerUnits,xDecay,decay);
	xSetInt(dPlayerUnits,xDecayNext,trTimeMS()+1000);
	xSetFloat(dPlayerUnits,xPhysicalResist,trQuestVarGet("proto"+proto+"armor"));
	xSetFloat(dPlayerUnits,xMagicResist,trQuestVarGet("proto"+proto+"armor"));
	if (PvP) {
		xSetInt(dPlayerUnits,xDoppelganger, xAddDatabaseBlock(dEnemies, true));
		xSetInt(dEnemies,xDoppelganger,index);
		xSetInt(dEnemies,xPlayerOwner,p);
		xSetInt(dEnemies,xUnitName,name);
		xSetInt(dEnemies,xUnitID,id);
		xSetFloat(dEnemies,xPhysicalResist,trQuestVarGet("proto"+proto+"armor"));
		xSetFloat(dEnemies,xMagicResist,trQuestVarGet("proto"+proto+"armor"));
		if (pvpDetached) {
			xDetachDatabaseBlock(dEnemies);
		}
	}
	activateSpecialUnit(name, dPlayerUnits, proto, p);
	return(index);
}

int spawnPlayerUnit(int p = 0, int proto = 0, vector vdb = vector(0,0,0), float decay = 0) {
	int next = trGetNextUnitScenarioNameNumber();
	string pName = kbGetProtoUnitName(proto);
	trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(vdb),0,xsVectorGetZ(vdb),0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit(pName);
	return(activatePlayerUnit(next, p, proto, decay));
}

int spawnPlayerClone(int p = 0, vector vdb = vector(0,0,0)) {
	int prev = xGetPointer(dPlayerData);
	xSetPointer(dPlayerData,p);
	int class = xGetInt(dPlayerData,xPlayerClass);
	int next = trGetNextUnitScenarioNameNumber();
	int db = getCharactersDB(p);
	int index = spawnPlayerUnit(p, xGetInt(dClass,xClassProto,class), vdb);
	int id = kbGetBlockID(""+next,true);
	xSetPointer(db, xAddDatabaseBlock(db));
	xSetInt(db, xUnitName, next);
	xSetInt(db,xUnitID,id);
	xSetInt(db, xCharIndex, index);
	xSetBool(dPlayerUnits,xIsHero,true);
	xSetFloat(dPlayerUnits,xPhysicalResist,xGetFloat(dPlayerData,xPlayerPhysicalResist));
	xSetFloat(dPlayerUnits,xMagicResist,xGetFloat(dPlayerData,xPlayerMagicResist));
	xSetPointer(dPlayerCharacters, xAddDatabaseBlock(dPlayerCharacters));
	xSetInt(dPlayerCharacters,xUnitName,next);
	xSetInt(dPlayerCharacters,xPlayerOwner,p);
	xSetInt(dPlayerCharacters,xUnitID,id);
	xSetInt(dPlayerCharacters, xCharIndex, index);
	xSetPointer(dPlayerData, prev);
	return(index);
}

void spawnPlayer(int p = 0, vector vdb = vector(0,0,0)) {
	xSetInt(dPlayerData,xPlayerUnit,trGetNextUnitScenarioNameNumber(),p);
	spawnPlayerClone(p, vdb);
	xSetInt(dPlayerData,xPlayerIndex,xGetNewestPointer(dPlayerUnits),p);
	xSetInt(dPlayerData,xPlayerTether,-1,p);
	if (trCurrentPlayer() == p) {
		int class = xGetInt(dPlayerData,xPlayerClass,p);
		string proto = kbGetProtoUnitName(xGetInt(dClass,xClassProto,class));
		uiFindType(proto);
	}
}

void revivePlayer(int p = 0) {
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"reviveBeam");
	trUnitChangeProtoUnit("Rocket");
	int prev = xGetPointer(dPlayerData);
	xSetPointer(dPlayerData,p);
	xUnitSelect(dPlayerData,xPlayerUnit);
	trUnitDestroy();
	spawnPlayer(p, trVectorQuestVarGet("dead"+p+"pos"));
	trSoundPlayFN("herorevived.wav","1",-1,"","");
	xUnitSelect(dPlayerData,xPlayerUnit);
	trDamageUnitPercent(50);
	trQuestVarSet("deadPlayerCount", trQuestVarGet("deadPlayerCount") - 1);
	if (trQuestVarGet("deadPlayerCount") < 0) {
		trQuestVarSet("deadPlayerCount", 0);
		debugLog("deadPlayerCount was negative!");
	}
	trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has been revived!");
	int db = getRelicsDB(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xSetInt(db, xUnitName, trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	}
	equipRelicsAgain(p);
	if (trCurrentPlayer() == p) {
		uiLookAtUnitByName(""+xGetInt(dPlayerData,xPlayerUnit));
	}
	xSetPointer(dPlayerData, prev);
}

void shootLaser(vector start = vector(0,0,0), vector dir = vector(0,0,0), float dist = -1, int p = 0) {
	int next = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("1,0", "Dwarf",1,xsVectorGetX(start),0,xsVectorGetZ(start),0,true);
	trUnitSelectClear();
	trUnitSelect(""+next,true);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	if (dist < 0) {
		trSetSelectedScale(3.0,0.0,80.0);
		dist = 60.0;
	} else {
		trSetSelectedScale(3.0,0.0,dist * 1.3);
	}
	trSetUnitOrientation(xsVectorSet(0.0-xsVectorGetX(dir),0,0.0-xsVectorGetZ(dir)), xsVectorSet(0,1,0), true);
	
	xSetPointer(dDelayLasers, xAddDatabaseBlock(dDelayLasers));
	xSetInt(dDelayLasers,xUnitName,next);
	xSetInt(dDelayLasers,xPlayerOwner,p);
	xSetInt(dDelayLasers,xLaserNext,trTimeMS() + 2000);
	xSetInt(dDelayLasers,xLaserPhase,0);
	xSetFloat(dDelayLasers,xLaserDist,dist);
	xSetVector(dDelayLasers, xLaserDir, dir);
}

rule spy_find
active
highFrequency
{
	int x = 0;
	int id = 0;
	vector scale = vector(0,0,0);
	vector dest = vector(0,0,0);
	while(spysearch < trGetNextUnitScenarioNameNumber()) {
		id = kbGetBlockID(""+spysearch, true);
		if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Spy Eye")) {
			x = modularCounterNext("spyfound");
			if (aiPlanGetUserVariableBool(ARRAYS,spyActive,x)) {
				aiPlanSetUserVariableBool(ARRAYS,spyActive,x,false);
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,spyUnit,x),true);
				dest = aiPlanGetUserVariableVector(ARRAYS,spyDest,x);
				if (trUnitAlive() == false) {
					if (aiPlanSetUserVariableInt(1*xsVectorGetX(dest),1*xsVectorGetY(dest),1*xsVectorGetZ(dest),-1) == false) {
						debugLog("spy error dead: " + 1*xsVectorGetX(dest) + "," + 1*xsVectorGetY(dest) + "," + 1*xsVectorGetZ(dest));
					}
				} else {
					trUnitSelectClear();
					trUnitSelectByID(id);
					trMutateSelected(aiPlanGetUserVariableInt(ARRAYS,spyProto,x));
					scale = aiPlanGetUserVariableVector(ARRAYS,spyScale,x);
					trSetSelectedScale(xsVectorGetX(scale),xsVectorGetY(scale),xsVectorGetZ(scale));
					if (aiPlanSetUserVariableInt(1*xsVectorGetX(dest),1*xsVectorGetY(dest),1*xsVectorGetZ(dest),spysearch) == false) {
						debugLog("spy error N/A: " + 1*xsVectorGetX(dest) + "," + 1*xsVectorGetY(dest) + "," + 1*xsVectorGetZ(dest));
					}
				}
			}
		}
		spysearch = 1 + spysearch;
	}
	if (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
		spyreset = 1 + spyreset;
		if (spyreset >= 5) {
			while (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
				x = modularCounterNext("spyFound");
				if (aiPlanGetUserVariableBool(ARRAYS,spyActive,x)) {
					aiPlanSetUserVariableBool(ARRAYS,spyActive,x,false);
					aiPlanSetUserVariableInt(xsVectorGetX(dest),xsVectorGetY(dest),xsVectorGetZ(dest),-1);
				}
			}
			debugLog("resetting spyfound");
		}
	} else {
		spyreset = 0;
	}
}

int databaseName(int p = 0) {
	if (p == ENEMY_PLAYER) {
		return(dEnemies);
	} else {
		return(dPlayerUnits);
	}
}

int opponentDatabaseName(int p = 0) {
	if (p == ENEMY_PLAYER) {
		return(dPlayerUnits);
	} else {
		return(dEnemies);
	}
}

void removeAllyUnit(int p = 0) {
	if (p == ENEMY_PLAYER) {
		removeEnemy();
	} else {
		removePlayerUnit();
	}
}

void removeOpponentUnit(int p = 0) {
	if (p == ENEMY_PLAYER) {
		removePlayerUnit();
	} else {
		removeEnemy();
	}
}

void damageOpponentUnit(int p = 0, float amt = 0) {
	if (p == ENEMY_PLAYER) {
		damagePlayerUnit(amt);
	} else {
		damageEnemy(p, amt);
	}
}

/*
enemies database: we loop through our playerUnits database and pull the doppelgangers out of the enemies database into a temporary cache
at the end of the function, we return everything back

only the playerUnits database has doppelganger pointers
*/

void pvpDetachPlayer(int p = 0) {
	if (PvP) {
		for(x=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			if (xGetInt(dPlayerUnits, xPlayerOwner) != p) {
				xDetachDatabaseBlock(dPlayerUnits);
			} else if (xSetPointer(dEnemies, xGetInt(dPlayerUnits, xDoppelganger))) {
				xDetachDatabaseBlock(dEnemies);
			}
		}
	}
}

void pvpReattachPlayer() {
	if (PvP) {
		xRestoreCache(dPlayerUnits);
		xRestoreCache(dEnemies);
	}
}