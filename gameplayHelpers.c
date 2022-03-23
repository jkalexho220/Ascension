const int ABILITY_READY = 0;
const int ABILITY_COOLDOWN = 1;
const int ABILITY_COST = 2;

const int ABILITY_OFF = 0;
const int ABILITY_ON = 1;

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

int spyDB = 0;
const int spyProto = 0;
const int spyUnit = 1;
const int spyDest = 2;
const int spyActive = 3;

int bossUnit = 0;
int bossCooldownTime = 0;

rule initialize_spy_database
active
highFrequency
{
	xsDisableSelf();
	xsSetContextPlayer(0);
	modularCounterInit("spyFind", 64);
	modularCounterInit("spyFound", 64);
	spyDB = aiPlanCreate("spyDB");
	aiPlanAddUserVariableInt(spyDB,spyProto,"proto",64);
	aiPlanAddUserVariableInt(spyDB,spyUnit,"unit",64);
	aiPlanAddUserVariableVector(spyDB,spyDest,"destination",64);
	aiPlanAddUserVariableBool(spyDB,spyActive,"active",64);
}

void advanceCooldowns(int p = 0, float seconds = 0) {
	int diff = 0;
	xSetPointer(dPlayerData,p);
	if (xGetInt(dPlayerData,xPlayerWellCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerWellCooldown, xGetInt(dPlayerData,xPlayerWellCooldown) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerWellCooldown) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("well");
			trCounterAddTime("well",diff,0,wellName, -1);
		}
	}
	if (xGetInt(dPlayerData,xPlayerRainCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerRainCooldown, xGetInt(dPlayerData,xPlayerRainCooldown) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerRainCooldown) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("rain");
			trCounterAddTime("rain",diff,0,rainName, -1);
		}
	}
	if (xGetInt(dPlayerData,xPlayerLureCooldownStatus) == ABILITY_COOLDOWN) {
		xSetInt(dPlayerData,xPlayerLureCooldown, xGetInt(dPlayerData,xPlayerLureCooldown) - 1000 * seconds);
		diff = (xGetInt(dPlayerData,xPlayerLureCooldown) - trTimeMS()) / 1000;
		if (diff > 0 && trCurrentPlayer() == p) {
			trCounterAbort("lure");
			trCounterAddTime("lure",diff,0,lureName, -1);
		}
	}
}

void gainFavor(int p = 0, float amt = 0) {
	xSetFloat(dPlayerData,xPlayerFavor,xsMin(100, xsMax(0, xGetFloat(dPlayerData,xPlayerFavor,p) + amt)),p);
	trPlayerGrantResources(p,"favor", xGetFloat(dPlayerData,xPlayerFavor,p) - trPlayerResourceCount(p, "favor"));
}

void spyEffect(int unit = 0, int proto = 0, vector dest = vector(0,0,0)) {
	if (peekModularCounterNext("spyFind") != trQuestVarGet("spyFound")) {
		trUnitSelectClear();
		trUnitSelect(""+unit, true);
		if (trUnitAlive()) {
			int x = modularCounterNext("spyFind");
			aiPlanSetUserVariableInt(spyDB,spyProto,x,proto);
			aiPlanSetUserVariableInt(spyDB,spyUnit,x,unit);
			aiPlanSetUserVariableBool(spyDB,spyActive,x,true);
			aiPlanSetUserVariableVector(spyDB,spyDest,x,dest);
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

vector vectorSetAsCurrentPosition(vector prev = vector(0,0,0),
	vector dir = vector(0,0,0), int last = 0, float speed = 0) {
	float dist = distanceTraveled(last, speed);
	vector v = xsVectorSet(xsVectorGetX(prev) + dist * xsVectorGetX(dir),0,xsVectorGetZ(prev) + dist * xsVectorGetZ(dir));
	return(v);
}


void silencePlayer(int p = 0, float duration = 0) {
	if ((trQuestVarGet("p"+p+"negationCloak") == 1) && (trQuestVarGet("p"+p+"dead") == 0)) {
		if (getBit(STATUS_SILENCE, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellstealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_SILENCE));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Silence absorbed! Your next spell will inflict Silence!</color>");
			}
		}
	} else {
		float timeout = duration * 1000 + trTimeMS();
		if (trQuestVarGet("p"+p+"silenceTimeout") < timeout) {
			trQuestVarSet("p"+p+"silenceTimeout", timeout);
		}
		if (trQuestVarGet("p"+p+"silenced") == 0) {
			trQuestVarSet("p"+p+"silenced", 1);
			trSoundPlayFN("frostgiantmove1.wav","1",-1,"","");
			trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has been silenced!");
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
	xSetPointer(dPlayerData,p);
	if (p > 0 && p < ENEMY_PLAYER) {
		duration = duration * xGetFloat(dPlayerData,xPlayerSpellDuration);
		if (xGetInt(dPlayerData,xPlayerGodBoon) == BOON_STATUS_COOLDOWNS) {
			advanceCooldowns(p, 1);
		}
		if (PvP) {
			int old = xGetPointer(dPlayerUnits);
			if (xSetPointer(dPlayerUnits, 1*xGetInt(dEnemies, xDoppelganger))) {
				silenceUnit(dPlayerUnits, duration);
			}
			xSetPointer(dPlayerUnits, old);
			return;
		}
	} else {
		p = xGetInt(db, xPlayerOwner);
		xSetPointer(dPlayerData,p);
		duration = duration * xGetFloat(dPlayerData,xPlayerSilenceResistance);
		if (xGetInt(dPlayerData,xPlayerUnit) == xGetInt(db,xUnitName)) {
			silencePlayer(p, duration);
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
				xsVectorSet(db,xSilenceStatus,xGetPointer(db)));
		} else {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(db, xSilenceSFX), true);
			trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(db,xUnitName),true);
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
}

void nightriderHarvest(vector pos = vector(0,0,0)) {
	int index = xGetPointer(dPlayerUnits);
	for(p=1; < ENEMY_PLAYER) {
		xSetPointer(dPlayerData,p);
		if ((xGetInt(dPlayerData,xPlayerClass) == NIGHTRIDER) &&
			(xGetInt(dPlayerData,xPlayerDead) == 0)) {
			if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit), pos) < 144) {
				trUnitSelectClear();
				trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit),true);
				xSetPointer(dPlayerUnits, xGetInt(dPlayerData,xPlayerIndex));
				healUnit(p, 0.05 * xGetFloat(dPlayerData,xPlayerHealth) * xGetFloat(dPlayerData,xPlayerSpellDamage));
				gainFavor(p, 1);
			}
		}
	}
	xSetPointer(dPlayerUnits,index);
}

void removeEnemy() {
	if (PvP == false) {
		int bounty = xGetInt(dEnemies, xBounty);
		vector pos = xGetVector(dEnemies,xUnitPos);
		nightriderHarvest(pos);
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
	vector pos = xGetVector(dPlayerUnits,xUnitPos);
	nightriderHarvest(pos);
	/*
	if (PvP) {
		if (ySetPointer("enemies", 1*yGetVar("playerUnits", "doppelganger"))) {
			yRemoveFromDatabase("enemies");
			yRemoveUpdateVar("enemies", "doppelganger");
			yRemoveUpdateVar("enemies", "silenceSFX");
			if (trQuestVarGet("detached") == 1) {
				trQuestVarSet("xdataenemiescount", 1 + trQuestVarGet("xdataenemiescount"));
				trQuestVarSet("enemiesDetachedSize", trQuestVarGet("enemiesDetachedSize") - 1);
			}
		}
		ySetPointer("enemies", 1*trQuestVarGet("enemiesDummyIndex"));
		yVarToVector("playerUnits", "pos");
		nightriderHarvest("pos");
		if (trQuestVarGet("playerUnitsLeaveIndex") == yGetPointer("playerUnits")) {
			debugLog("player units removed the wrong thing! Removed playerunitsLeaveIndex!");
		} else if (trQuestVarGet("playerUnitsDummyIndex") == yGetPointer("playerUnits")) {
			debugLog("player units removed the wrong thing! Removed playerUnitsDummyIndex");
		}
	}
	*/
	xFreeDatabaseBlock(dPlayerUnits);
}

void removePlayerCharacter() {
	xFreeDatabaseBlock(dPlayerCharacters);
}

void removePlayerSpecific(int p = 0) {
	int db = trQuestVarGet("p"+p+"characters");
	if (xGetInt(db,xUnitName) == xGetInt(dPlayerData,xPlayerUnit,p)) {
		vector pos = kbGetBlockPosition(""+xGetInt(dPlayerData,xPlayerUnit));
		trVectorQuestVarSet("dead"+p+"pos",pos);
		for(x=xGetDatabaseCount(1*trQuestVarGet("p"+p+"relics")); >0) {
			xDatabaseNext(1*trQuestVarGet("p"+p+"relics"));
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(1*trQuestVarGet("p"+p+"relics",xRelicName)),true);
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		xSetInt(dPlayerData,xPlayerDead,10);
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
		silencePlayer(p, 0);
		/* NOOOO MY QUEEEEN */
		if (xGetInt(dPlayerData,xPlayerSimp,p) > 0) {
			int simp = xGetInt(dPlayerData,xPlayerSimp,p);
			trUnitSelectClear();
			trUnitSelectByQV("p"+simp+"tether");
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			xSetInt(dPlayerData,xPlayerQueen,0,simp);
			xSetInt(dPlayerData,xPlayerSimp,0,p);
		}
	}
	xFreeDatabaseBlock(db);
}

void equipRelicsAgain(int p = 0) {
	int db = trQuestVarGet("p"+p+"relics");
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
	
	for(x=0; <4) {
		trQuestVarSet("rotX", trQuestVarGet(loc+"X") + trQuestVarGet("rotX"+x));
		trQuestVarSet("rotZ", trQuestVarGet(loc+"Z") + trQuestVarGet("rotZ"+x));
		if (terrainIsType("rot", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
			retVal = xsVectorSet(trQuestVarGet("rotX"+x),0,trQuestVarGet("rotZ"+x));
		}
	}
	
	return(retVal);
}

/*
dist is the actual value
width is the squared value
*/
bool rayCollision(string db = "", string start = "", string dir = "", float dist = 0, float width = 0) {
	trVectorSetUnitPos("collidePos", db);
	float current = zDistanceBetweenVectors("collidePos", start);
	if (current < dist) {
		trQuestVarSet("hitboxX", trQuestVarGet(start+"x") + current * trQuestVarGet(dir+"x"));
		trQuestVarSet("hitboxZ", trQuestVarGet(start+"z") + current * trQuestVarGet(dir+"z"));
		if (zDistanceBetweenVectorsSquared("collidePos", "hitbox") < width) {
			return(true);
		}
	}
	return(false);
}

/*
called after confirming that the projectile is on WALL terrain.
*/
vector getBounceDir(string loc = "", string dir = "") {
	bool bounced = false;
	int xMod = 1;
	int zMod = 1;
	if (trQuestVarGet(dir+"x") < 0) {
		xMod = -1;
	}
	if (trQuestVarGet(dir+"z") < 0) {
		zMod = -1;
	}
	vector ret = trVectorQuestVarGet(dir);
	/* time to bounce! */
	trQuestVarSet("horizontalX", trQuestVarGet(loc+"X") - xMod);
	trQuestVarSet("horizontalZ", trQuestVarGet(loc+"Z"));
	trQuestVarSet("verticalX", trQuestVarGet(loc+"X"));
	trQuestVarSet("verticalZ", trQuestVarGet(loc+"Z") - zMod);
	if (terrainIsType("horizontal", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
		ret = xsVectorSetZ(ret, 0.0 - trQuestVarGet(dir+"z"));
		bounced = true;
	}
	if (terrainIsType("vertical", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
		ret = xsVectorSetX(ret, 0.0 - trQuestVarGet(dir+"x"));
		bounced = true;
	}
	if (bounced == false) {
		/*
		we collided with a column. time for cool math
		a = position of the projectile in a unit square
		b = normalized vector from the contested corner to the projectile position
		*/
		vector a = (trVectorQuestVarGet(loc) - (trVectorQuestVarGet(loc) * 2)) / 2;
		vector b = xsVectorSet((1 - xMod) / 2,0,(1 - zMod) / 2);
		b = trVectorQuestVarGet(dir) + xsVectorNormalize(b - a);
		/*
		sign is different, which means we invert the other axis
		*/
		if (xsVectorGetX(b) * trQuestVarGet(dir+"x") < 0) {
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
void vectorSetAsTargetVector(string target = "", string from = "", string to = "", float dist = 40.0) {
	trVectorQuestVarSet("dirrr", zGetUnitVector(from, to, dist));
	trQuestVarSet(target+"x", trQuestVarGet("dirrrx") + trQuestVarGet(from+"x"));
	trQuestVarSet(target+"z", trQuestVarGet("dirrrz") + trQuestVarGet(from+"z"));
	
	/*
	No out-of-bounds allowed
	*/
	float scale = 0;
	if (trQuestVarGet(target+"x") < 0) {
		scale = trQuestVarGet(target+"x") / (trQuestVarGet(target+"x") - trQuestVarGet(from+"x"));
		trQuestVarSet(target+"z", trQuestVarGet(target+"z") + scale * (trQuestVarGet(from+"z") - trQuestVarGet(target+"z")));
		trQuestVarSet(target+"x", 0);
	} else if (trQuestVarGet(target+"x") > DIMENSION_X) {
		scale = (trQuestVarGet(target+"x") - DIMENSION_X) / (trQuestVarGet(target+"x") - trQuestVarGet(from+"x"));
		trQuestVarSet(target+"z", trQuestVarGet(target+"z") + scale * (trQuestVarGet(from+"z") - trQuestVarGet(target+"z")));
		trQuestVarSet(target+"x", DIMENSION_X);
	}
	
	if (trQuestVarGet(target+"z") < 0) {
		scale = trQuestVarGet(target+"z") / (trQuestVarGet(target+"z") - trQuestVarGet(from+"z"));
		trQuestVarSet(target+"x", trQuestVarGet(target+"x") + scale * (trQuestVarGet(from+"x") - trQuestVarGet(target+"x")));
		trQuestVarSet(target+"z", 0);
	} else if (trQuestVarGet(target+"z") > DIMENSION_Z) {
		scale = (trQuestVarGet(target+"z") - DIMENSION_Z) / (trQuestVarGet(target+"z") - trQuestVarGet(from+"z"));
		trQuestVarSet(target+"x", trQuestVarGet(target+"x") + scale * (trQuestVarGet(from+"x") - trQuestVarGet(target+"x")));
		trQuestVarSet(target+"z", DIMENSION_Z);
	}
}

void poisonUnit(string db = "", float duration = 0, float damage = 0, int p = 0) {
	bool targetPlayers = (p == 0) || (p == ENEMY_PLAYER);
	if (p > 0 && p < ENEMY_PLAYER) {
		if (trQuestVarGet("p"+p+"godBoon") == BOON_STATUS_COOLDOWNS) {
			advanceCooldowns(p, 1);
		}
		duration = duration * trQuestVarGet("p"+p+"spellDuration") * xsPow(0.5, 1*trQuestVarGet("p"+p+"poisonSpeed"));
		damage = damage * trQuestVarGet("p"+p+"spellDamage") * xsPow(2, 1*trQuestVarGet("p"+p+"poisonSpeed"));
		if (PvP) {
			int old = yGetPointer("playerUnits");
			if (ySetPointer("playerUnits", 1*yGetVar("enemies", "doppelganger"))) {
				poisonUnit("playerUnits", duration, damage, 0);
			}
			ySetPointer("playerUnits", old);
			return;
		}
	} else {
		p = yGetVar(db, "player");
		duration = duration * trQuestVarGet("p"+p+"poisonResistance");
	}
	duration = duration * 1000;
	if (targetPlayers && (yGetVar(db, "hero") == 1) && (trQuestVarGet("p"+p+"negationCloak") == 1)) {
		if (getBit(STATUS_POISON, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellstealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_POISON));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Poison absorbed! Your next spell will inflict Poison!</color>");
			}
		}
	} else if (trTimeMS() + duration > yGetVar(db, "poisonTimeout")) {
		if (yGetVar(db, "poisonStatus") == 0) {
			if (yGetVar(db, "poisonSFX") == 0) {
				spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Poison SFX"), yGetVarName(db, "poisonSFX"));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar(db, "poisonSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Poison SFX"));
			}
			ySetVar(db, "poisonStatus", 1);
			ySetVar(db, "poisonLast", trTimeMS());
			trQuestVarSet("poisonSound", 1);
		}
		ySetVar(db, "poisonTimeout", trTimeMS() + duration);
		if (damage > yGetVar(db, "poisonDamage")) {
			ySetVar(db, "poisonDamage", damage);
		}
	}
	
}


void growFrostGiantsIncoming(string pos = "") {
	for(x=yGetDatabaseCount("frostGiantsIncoming"); >0) {
		yDatabaseNext("frostGiantsIncoming");
		if (zDistanceToVectorSquared("frostGiantsIncoming", pos) < 100) {
			trUnitSelectClear();
			trUnitSelectByQV("frostGiantsIncoming", true);
			trUnitHighlight(0.5, false);
			if (yGetVar("frostGiantsIncoming", "targetSize") < 5) {
				trQuestVarSet("frostGiantIncomingSound", 1);
				ySetVar("frostGiantsIncoming", "targetSize", 2 + yGetVar("frostGiantsIncoming", "targetSize"));
				ySetVar("frostGiantsIncoming", "targetTime", xsMax(trTimeMS(), yGetVar("frostGiantsIncoming", "targetTime")) + 1000);
			}
		}
	}
}

/*
protection blocks all damage.
*/
void damagePlayerUnit(float dmg = 0, int index = -1) {
	int old = yGetPointer("playerUnits");
	if (index < 0) {
		index = old;
	}
	if (ySetPointer("playerUnits", index)) {
		int p = yGetVar("playerUnits", "player");
		if (PvP) {
			trQuestVarSet("protectionCount", trQuestVarGet("p"+p+"protection"));
		}
		if (trQuestVarGet("protectionCount") == 0) {
			if (PvP == false) {
				dmg = dmg - dmg * yGetVar("playerUnits", "magicResist");
			}
			if ((yGetVar("playerUnits", "hero") == 1) && trQuestVarGet("p"+p+"negationCloak") == 1) {
				healUnit(p, dmg);
			} else {
				trDamageUnit(dmg);
			}
		}
		ySetPointer("playerUnits", old);
	}
}


/*
Enemies have elemental resistance and weakness
*/
float damageEnemy(int p = 0, float dmg = 0, bool spell = true, float pierce = 0) {
	if (spell) {
		dmg = dmg - dmg * yGetVar("enemies", "magicResist") * (1.0 - pierce - trQuestVarGet("p"+p+"magicPen"));
	} else {
		dmg = dmg - dmg * yGetVar("enemies", "physicalResist") * (1.0 - pierce);
	}
	if (yGetVar("enemies", "poisonStatus") == 1) {
		dmg = dmg * (1.0 + trQuestVarGet("p"+p+"poisonKiller"));
	}
	if (spell) {
		if (trQuestVarGet("p"+p+"godBoon") == BOON_SPELL_POISON) {
			poisonUnit("enemies", 12.0, 12.0, p);
		}
		trQuestVarSet("p"+p+"lifestealTotal",
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"Lifesteal") * dmg * 0.5);
	} else {
		trQuestVarSet("p"+p+"lifestealTotal",
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"Lifesteal") * dmg);
	}
	if (PvP) {
		damagePlayerUnit(dmg, 1*yGetVar("enemies", "doppelganger"));
	} else {
		trDamageUnit(dmg);
	}
	return(dmg);
}


void stunUnit(string db = "", float duration = 0, int p = 0, bool sound = true) {
	int index = 0;
	bool targetPlayers = (p == 0) || (p == ENEMY_PLAYER);
	if (p > 0 && p < ENEMY_PLAYER) {
		if (trQuestVarGet("p"+p+"godBoon") == BOON_STATUS_COOLDOWNS) {
			advanceCooldowns(p, 1);
		}
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
		trUnitSelectClear();
		trUnitSelectByQV(db, true);
		if (trQuestVarGet("p"+p+"stunDamage") > 0) {
			damageEnemy(p, trQuestVarGet("p"+p+"health") * trQuestVarGet("p"+p+"stunDamage"), true);
		}
		if (PvP) {
			int old = yGetPointer("playerUnits");
			if (ySetPointer("playerUnits", 1*yGetVar("enemies", "doppelganger"))) {
				stunUnit("playerUnits", duration, 0, sound);
			}
			ySetPointer("playerUnits", old);
			return;
		}
	} else {
		p = yGetVar(db, "player");
		duration = duration * trQuestVarGet("p"+p+"stunResistance");
	}
	duration = duration * 1000;
	if (targetPlayers && (yGetVar(db, "hero") == 1) && (trQuestVarGet("p"+p+"negationCloak") == 1)) {
		if (getBit(STATUS_STUN, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellStealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_STUN));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Stun absorbed! Your next spell will inflict Stun!</color>");
			}
		}
	} else {
		if (trTimeMS() + duration > yGetVar(db, "stunTimeout")) {
			if (yGetVar(db, "stunStatus") == 0) {
				trQuestVarSet("stunSound", 1);
				if (trQuestVarGet("boss") == 3) {
					trVectorSetUnitPos("stunpos", db);
					growFrostGiantsIncoming("stunpos");
				}
				index = yAddToDatabase("stunnedUnits", db);
				yAddUpdateVar("stunnedUnits", "proto", kbGetUnitBaseTypeID(kbGetBlockID(""+1*trQuestVarGet(db), true)));
				if (yGetVar(db, "stunSFX") == 0) {
					spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Shockwave stun effect"), yGetVarName(db, "stunSFX"));
				} else {
					trUnitSelectClear();
					trUnitSelect(""+1*yGetVar(db, "stunSFX"), true);
					trMutateSelected(kbGetProtoUnitID("Shockwave stun effect"));
					trUnitSelectClear();
					trUnitSelectByQV(db, true);
				}
				ySetVar(db, "stunStatus", index);
			} else if (sound) {
				trQuestVarSet("stunSound", 1);
			}
			ySetVar(db, "stunTimeout", trTimeMS() + duration);
		}
	}
	
}

void processLaunchedUnit() {
	yDatabaseNext("launchedUnits");
	yVarToVector("launchedUnits", "dest");
	trUnitSelectClear();
	trUnitSelect(""+1*yGetVar("launchedUnits", "unit"));
	if (trUnitAlive() == false ||
		zDistanceToVectorSquared("launchedUnits", "dest") < 4 ||
		trTimeMS() > yGetVar("launchedUnits", "timeout")) {
		if (trUnitAlive()) {
			string db = "playerUnits";
			int p = yGetVar("launchedUnits", "player");
			if (p == ENEMY_PLAYER) {
				db = "enemies";
			}
			ySetVarAtIndex(db, "launched", 0, 1*yGetVar("launchedUnits", "index"));
			trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("launchedUnits", "proto")));
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("launchedUnits", "unit"));
			trMutateSelected(1*yGetVar("launchedUnits", "proto"));
			if ((p < ENEMY_PLAYER) && (yGetVar("launchedUnits", "unit") == trQuestVarGet("p"+p+"unit"))) {
				equipRelicsAgain(p);
				trQuestVarSet("p"+p+"launched", 0);
			}
			if (yGetVar("launchedUnits", "unit") == trQuestVarGet("bossUnit")) {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("launchedUnits", "unit"));
				trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
			}
			if (yGetVar("launchedUnits", "stun") == 1) {
				int index = yGetPointer(db);
				if (ySetPointer(db, 1*yGetVar("launchedUnits", "index"))) {
					stunUnit(db, 3.0);
					ySetPointer(db, index);
				}
			}
		} else {
			trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("launchedUnits", "proto")));
		}
		trUnitSelectClear();
		trUnitSelectByQV("launchedUnits");
		trUnitChangeProtoUnit("Dust Small");
		yRemoveFromDatabase("launchedUnits");
	}
}

void launchUnit(string db = "", string dest = "") {
	int hitWall = 0;
	int index = 0;
	if (PvP && (db == "enemies")) {
		index = yGetPointer("playerUnits");
		db = "playerUnits";
		ySetPointer("playerUnits", 1*yGetVar("enemies", "doppelganger"));
	}
	if (yGetVar(db, "launched") == 0) {
		ySetVar(db, "launched", 1);
		int type = kbGetUnitBaseTypeID(kbGetBlockID(""+1*trQuestVarGet(db)));
		int p = ENEMY_PLAYER;
		trUnitSelectClear();
		trUnitSelectByQV(db);
		if (trUnitIsOwnedBy(ENEMY_PLAYER) == false) {
			p = yGetVar(db, "player");
		}
		trUnitChangeProtoUnit("Transport Ship Greek");
		
		trVectorSetUnitPos("start", db);
		trVectorQuestVarSet("dir", zGetUnitVector("start", dest));
		
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
		trUnitSelectClear();
		trUnitSelectByQV("next");
		trImmediateUnitGarrison(""+1*trQuestVarGet(db));
		trUnitConvert(p);
		trUnitChangeProtoUnit("Dwarf");
		
		trUnitSelectClear();
		trUnitSelectByQV("next");
		trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
		trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
		
		trUnitSelectClear();
		trUnitSelectByQV(db);
		trMutateSelected(type);
		trUnitOverrideAnimation(24,0,true,true,-1);
		trMutateSelected(kbGetProtoUnitID("Relic"));
		trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
		trMutateSelected(type);
		if (trQuestVarGet(db) == trQuestVarGet("bossUnit")) {
			trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
		}
		
		float dist = zDistanceBetweenVectors("start", dest);
		for(x=0; < dist / 2) {
			trQuestVarSet("nextx", trQuestVarGet("startx") + 2.0 * trQuestVarGet("dirx"));
			trQuestVarSet("nextz", trQuestVarGet("startz") + 2.0 * trQuestVarGet("dirz"));
			vectorToGrid("next", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				hitWall = 1;
				break;
			} else {
				trQuestVarSet("startx", trQuestVarGet("nextx"));
				trQuestVarSet("startz", trQuestVarGet("nextz"));
			}
		}
		
		trUnitSelectClear();
		trUnitSelectByQV("next");
		trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
		trUnitMoveToPoint(trQuestVarGet("startx"),0,trQuestVarGet("startz"),-1,false);
		
		yAddToDatabase("launchedUnits", "next");
		yAddUpdateVar("launchedUnits", "unit", trQuestVarGet(db));
		yAddUpdateVar("launchedUnits", "index", yGetPointer(db));
		yAddUpdateVar("launchedUnits", "player", p);
		yAddUpdateVar("launchedUnits", "proto", type);
		yAddUpdateVar("launchedUnits", "destX", trQuestVarGet("startx"));
		yAddUpdateVar("launchedUnits", "destz", trQuestVarGet("startz"));
		yAddUpdateVar("launchedUnits", "timeout", trTimeMS() + 1100 * dist / 15);
		yAddUpdateVar("launchedUnits", "stun", hitWall);
		
		if ((p < ENEMY_PLAYER) && (trQuestVarGet(db) == trQuestVarGet("p"+p+"unit"))) {
			trQuestVarSet("p"+p+"launched", 1);
			for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
				yDatabaseNext("p"+p+"relics", true);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
	if (PvP) {
		ySetPointer("playerUnits", index);
	}
}

void stunsAndPoisons(string db = "") {
	if (yGetVar(db, "poisonStatus") == 1) {
		float amt = trTimeMS() - yGetVar(db, "poisonLast");
		if (trTimeMS() > yGetVar(db, "poisonTimeout")) {
			ySetVar(db, "poisonStatus", 0);
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar(db, "poisonSFX"), true);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
		} else if (amt > 500) {
			trDamageUnit(amt * yGetVar(db, "poisonDamage") * 0.001);
			ySetVar(db, "poisonLast", yGetVar("poisonLast") + trTimeMS());
		}
	}
	if (yGetVar(db, "stunStatus") >= 1) {
		if (trTimeMS() > yGetVar(db, "stunTimeout")) {
			trUnitSelectClear();
			trUnitSelectByQV(db);
			trUnitOverrideAnimation(-1,0,false,true,-1);
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar(db, "stunSFX"), true);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
			if (ySetPointer("stunnedUnits", 1*yGetVar(db, "stunStatus"))) {
				yRemoveFromDatabase("stunnedUnits");
			}
			ySetVar(db, "stunStatus", 0);
		}
	}
	if (yGetVar(db, "silenceStatus") == 1) {
		if (trTimeMS() > yGetVar(db, "silenceTimeout")) {
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar(db, "silenceSFX"));
			trMutateSelected(kbGetProtoUnitID("Rocket"));
			ySetVar(db, "silenceStatus", 0);
		}
	}
}

void OnHit(int p = 0, int index = 0, bool magic = false) {
	gainFavor(p, trQuestVarGet("p"+p+"favorFromAttacks"));
	if (trQuestVarGet("p"+p+"cleave") > 0) {
		int prev = yGetPointer("enemies");
		if (index != prev) {
			ySetPointer("enemies", index);
		}
		trVectorSetUnitPos("onHitPos", "enemies");
		for(x=yGetDatabaseCount("enemies"); >1) {
			yDatabaseNext("enemies");
			if (zDistanceToVectorSquared("enemies", "onHitPos") < 16.0) {
				trUnitSelectClear();
				trUnitSelectByQV("enemies");
				damageEnemy(p, trQuestVarGet("p"+p+"cleave") * trQuestVarGet("p"+p+"attack"), magic);
			}
		}
		trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("onHitPosx"),0,trQuestVarGet("onHitPosz"),0,true);
		trArmySelect("1,0");
		trDamageUnitPercent(100);
		trUnitChangeProtoUnit("Meteorite");
		ySetPointer("enemies", prev);
	}
}

int CheckOnHit(int p = 0, int id = 0, bool onhit = true) {
	int action = kbUnitGetAnimationActionType(id);
	int status = ON_HIT_NONE;
	int class = trQuestVarGet("p"+p+"class");
	int simp = 0;
	float amt = 0;
	if (action == 32) {
		status = ON_HIT_JUMP;
	} else if (yGetVar("p"+p+"characters", "attacking") == 0) {
		if ((action == 12) || (action == 6)) {
			ySetVar("p"+p+"characters", "attackTarget", kbUnitGetTargetUnitID(id));
			ySetVar("p"+p+"characters", "attacking", 1);
			ySetVar("p"+p+"characters", "attackNext", trTimeMS() + trQuestVarGet("p"+p+"firstDelay"));
			ySetVar("p"+p+"characters", "attackTargetIndex", 0);
		}
	} else {
		if ((action == 12) || (action == 6)) {
			if (trTimeMS() > yGetVar("p"+p+"characters", "attackNext")) {
				
				status = ON_HIT_NORMAL;
				ySetVar("p"+p+"characters", "attackNext",
					yGetVar("p"+p+"characters", "attackNext") + trQuestVarGet("p"+p+"nextDelay"));
				/* only melee characters have special attacks */
				if (action == 6) {
					ySetVar("p"+p+"characters", "specialAttack", yGetVar("p"+p+"characters", "specialAttack") - 1);
					if (yGetVar("p"+p+"characters", "specialAttack") <= 0) {
						ySetVar("p"+p+"characters", "specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
						status = ON_HIT_SPECIAL;
					}
				}
				/* simp benefits */
				if (trQuestVarGet("p"+p+"simp") > 0) {
					simp = trQuestVarGet("p"+p+"simp");
					for(x=yGetDatabaseCount("p"+simp+"characters"); >0) {
						yDatabaseNext("p"+simp+"characters");
						ySetVar("p"+simp+"characters", "specialAttack", yGetVar("p"+simp+"characters", "specialAttack") - 1);
					}
				}
				/* get the target */
				if (yGetVar("p"+p+"characters", "attackTargetIndex") == 0) {
					for(x=yGetDatabaseCount("enemies"); >0) {
						yDatabaseNext("enemies");
						if (kbGetBlockID(""+1*trQuestVarGet("enemies")) == yGetVar("p"+p+"characters", "attackTarget")) {
							ySetVar("p"+p+"characters", "attackTargetIndex", yGetPointer("enemies"));
							trQuestVarSet("p"+p+"poisonKillerActive", yGetVar("enemies", "poisonStatus"));
							break;
						}
					}
				} else {
					trQuestVarSet("p"+p+"poisonKillerActive",
						yGetVarAtIndex("enemies", "poisonStatus", 1*yGetVar("p"+p+"characters", "attackTargetIndex")));
				}
				/* lifesteal */
				amt = trQuestVarGet("p"+p+"Lifesteal") * trQuestVarGet("p"+p+"attack");
				if (trQuestVarGet("p"+p+"poisonKillerActive") == 1) {
					amt = amt * (1.0 + trQuestVarGet("p"+p+"poisonKiller"));
				}
				trQuestVarSet("p"+p+"lifestealTotal", trQuestVarGet("p"+p+"lifestealTotal") + amt);
				if (onhit) {
					OnHit(p, 1*yGetVar("p"+p+"characters", "attackTargetIndex"));
				}
			} else {
				int target = kbUnitGetTargetUnitID(id);
				if (yGetVar("p"+p+"characters", "attackTarget") != target) {
					ySetVar("p"+p+"characters", "attackNext", trTimeMS() + trQuestVarGet("p"+p+"firstDelay"));
					ySetVar("p"+p+"characters", "attackTarget", target);
					ySetVar("p"+p+"characters", "attackTargetIndex", 0);
				}
				status = ON_HIT_ATTACKING;
			}
		} else {
			ySetVar("p"+p+"characters", "attacking", 0);
			trQuestVarSet("p"+p+"poisonKillerActive", 0);
		}
	}
	return(status);
}

/* call this at the end of each class's function */
void poisonKillerBonus(int p = 0) {
	int class = trQuestVarGet("p"+p+"class");
	float amt = trQuestVarGet("p"+p+"poisonKiller")*trQuestVarGet("p"+p+"attack")*trQuestVarGet("p"+p+"poisonKillerActive");
	zSetProtoUnitStat(kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto")),p,27,amt+trQuestVarGet("p"+p+"attack"));
	zSetProtoUnitStat(kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto")),p,31,amt+trQuestVarGet("p"+p+"attack"));
}

float calculateDecay(int p = 0, float decay = 0) {
	if (trQuestVarGet("p"+p+"godBoon") == BOON_DECAY_HALVED) {
		decay = decay * 0.5;
	}
	return(decay / trQuestVarGet("p"+p+"spellDuration"));
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
int processGenericProj(string db = "") {
	int id = 0;
	int action = PROJ_NONE;
	float scale = 0;
	id = yDatabaseNext(db, true);
	if (id == -1) {
		yRemoveFromDatabase(db);
		action = PROJ_REMOVE;
	} else if (yGetVar(db, "yeehaw") == 1) {
		trMutateSelected(1*yGetVar(db, "proto"));
		trUnitOverrideAnimation(1*yGetVar(db, "anim"),0,true,true,-1);
		scale = yGetVar(db, "scale");
		trSetSelectedScale(scale,scale,scale);
		ySetVar(db, "yeehaw", 0);
		action = PROJ_BOUNCE;
	} else if (yGetVar(db, "yeehaw") == 2) {
		/* first time search */
		ySetVar(db, "yeehaw", 1);
	} else {
		trVectorSetUnitPos("pos", db);
		if (trQuestVarGet("posY") < worldHeight + 0.5 || yGetVar(db, "yeehaw") == 99) {
			action = PROJ_GROUND;
			yVarToVector(db, "dir");
			zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, yGetVar(db, "speed"));
			trUnitChangeProtoUnit("Kronny Flying");
			trUnitSelectClear();
			trUnitSelectByQV(db);
			trDamageUnitPercent(-100);
			trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
			trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
			trSetSelectedScale(0,0.0-yGetVar(db, "height"),0);
			trDamageUnitPercent(100);
			ySetVar(db, "yeehaw", 1);
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
int addGenericProj(string db = "",string start="",string dir="",
	int proto=0,int anim=0,float speed=10.0,float height=4,float scale=0, int p = 0) {
	if (p == 0) {
		p = ENEMY_PLAYER;
	}
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	int index = yAddToDatabase(db, "next");
	yAddUpdateVar(db, "proto", proto);
	yAddUpdateVar(db, "anim", anim);
	yAddUpdateVar(db, "speed", speed);
	yAddUpdateVar(db, "height", height);
	yAddUpdateVar(db, "scale", scale);
	yAddUpdateVar(db, "dirX", trQuestVarGet(dir+"x"));
	yAddUpdateVar(db, "dirZ", trQuestVarGet(dir+"z"));
	yAddUpdateVar(db, "yeehaw", 2);
	
	trArmyDispatch(""+p+",0", "Dwarf",1,trQuestVarGet(start+"x"),0,trQuestVarGet(start+"z"),0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	zSetProtoUnitStat("Kronny Flying", p, 1, speed);
	trSetUnitOrientation(trVectorQuestVarGet(dir), vector(0,1,0), true);
	trSetSelectedScale(0, 0.0 - height, 0);
	trDamageUnitPercent(100);
	return(index);
}


void activateSpecialUnit(string db = "", string pName = "", int proto = 0, int p = 0) {
	switch(proto)
	{
		case kbGetProtoUnitID("Sphinx"):
		{
			yAddToDatabase("Sphinxes", db);
			yAddUpdateVar("Sphinxes", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Sphinxes", "step", 0);
			yAddUpdateVar("Sphinxes", "player", p);
		}
		case kbGetProtoUnitID("Dryad"):
		{
			yAddToDatabase("Dryads", db);
			yAddUpdateVar("dryads", "index", yGetNewestPointer(pName));
			yAddUpdateVar("dryads", "player", p);
		}
		case kbGetProtoUnitID("Wadjet"):
		{
			yAddToDatabase("Dryads", db);
			yAddUpdateVar("dryads", "index", yGetNewestPointer(pName));
			yAddUpdateVar("dryads", "player", p);
		}
		case kbGetProtoUnitID("Medusa"):
		{
			yAddToDatabase("Medusas", db);
			yAddUpdateVar("Medusas", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Medusas", "step", 0);
			yAddUpdateVar("Medusas", "player", p);
		}
		case kbGetProtoUnitID("Mountain Giant"):
		{
			yAddToDatabase("MountainGiants", db);
			yAddUpdateVar("MountainGiants", "index", yGetNewestPointer(pName));
			yAddUpdateVar("MountainGiants", "step", 0);
			yAddUpdateVar("MountainGiants", "player", p);
		}
		case kbGetProtoUnitID("Frost Giant"):
		{
			yAddToDatabase("FrostGiants", db);
			yAddUpdateVar("FrostGiants", "index", yGetNewestPointer(pName));
			yAddUpdateVar("FrostGiants", "step", 0);
			yAddUpdateVar("FrostGiants", "player", p);
		}
		case kbGetProtoUnitID("Valkyrie"):
		{
			yAddUpdateVar(pName, "magicResist", 1);
			yAddToDatabase("Valkyries", db);
			yAddUpdateVar("Valkyries", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Valkyries", "silenced", 0);
			spyEffect(1*trQuestVarGet(db),
				kbGetProtoUnitID("Vortex Finish Linked"), yGetNewVarName("Valkyries", "sfx"));
			yAddUpdateVar("Valkyries", "player", p);
		}
		case kbGetProtoUnitID("Ballista"):
		{
			yAddToDatabase("ballistas", db);
			yAddUpdateVar("ballistas", "index", yGetNewestPointer(pName));
			yAddUpdateVar("ballistas", "player", p);
		}
		case kbGetProtoUnitID("Colossus"):
		{
			yAddUpdateVar(pName, "magicResist", 1);
			yAddToDatabase("Valkyries", db);
			yAddUpdateVar("Valkyries", "index", yGetNewestPointer(pName));
			spyEffect(1*trQuestVarGet(db),
				kbGetProtoUnitID("Vortex Finish Linked"), yGetNewVarName("Valkyries", "sfx"));
			yAddUpdateVar("Valkyries", "player", p);
		}
		case kbGetProtoUnitID("Fire Siphon"):
		{
			yAddToDatabase("siphons", db);
			yAddUpdateVar("siphons", "index", yGetNewestPointer(pName));
			yAddUpdateVar("siphons", "player", p);
		}
		case kbGetProtoUnitID("Battle Boar"):
		{
			yAddToDatabase("battleBoars", db);
			yAddUpdateVar("battleBoars", "index", yGetNewestPointer(pName));
			yAddUpdateVar("battleBoars", "step", 0);
			yAddUpdateVar("battleBoars", "player", p);
		}
		case kbGetProtoUnitID("Automaton SPC"):
		{
			yAddToDatabase("automatons", db);
			yAddUpdateVar("automatons", "index", yGetNewestPointer(pName));
			yAddUpdateVar("automatons", "player", p);
		}
		case kbGetProtoUnitID("Behemoth"):
		{
			yAddUpdateVar(pName, "physicalResist", 1);
			yAddUpdateVar(pName, "magicResist", -1);
		}
		case kbGetProtoUnitID("Scarab"):
		{
			yAddUpdateVar(pName, "physicalResist", 1);
			yAddUpdateVar(pName, "magicResist", -1);
			yAddToDatabase("scarabs", db);
			yAddUpdateVar("scarabs", "index", yGetNewestPointer(pName));
			yAddUpdateVar("scarabs", "player", p);
		}
		case kbGetProtoUnitID("Satyr"):
		{
			yAddToDatabase("Satyrs", db);
			yAddUpdateVar("Satyrs", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Satyrs", "step", 0);
			yAddUpdateVar("Satyrs", "player", p);
		}
		case kbGetProtoUnitID("Avenger"):
		{
			yAddToDatabase("Avengers", db);
			yAddUpdateVar("Avengers", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Avengers", "step", 0);
			yAddUpdateVar("Avengers", "player", p);
		}
		case kbGetProtoUnitID("Scorpion Man"):
		{
			yAddToDatabase("ScorpionMen", db);
			yAddUpdateVar("ScorpionMen", "index", yGetNewestPointer(pName));
			yAddUpdateVar("ScorpionMen", "step", 0);
			yAddUpdateVar("ScorpionMen", "player", p);
		}
		case kbGetProtoUnitID("Mummy"):
		{
			yAddToDatabase("Mummies", db);
			yAddUpdateVar("Mummies", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Mummies", "step", 0);
			yAddUpdateVar("Mummies", "player", p);
		}
		case kbGetProtoUnitID("Nereid"):
		{
			yAddToDatabase("Nereids", db);
			yAddUpdateVar("Nereids", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Nereids", "step", 0);
			yAddUpdateVar("Nereids", "player", p);
		}
		case kbGetProtoUnitID("Hydra"):
		{
			yAddToDatabase("Hydras", db);
			yAddUpdateVar("Hydras", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Hydras", "last", trTime());
			yAddUpdateVar("Hydras", "player", p);
		}
		case kbGetProtoUnitID("Kraken"):
		{
			yAddToDatabase("Krakens", db);
			yAddUpdateVar("Krakens", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Krakens", "step", 0);
			yAddUpdateVar("Krakens", "player", p);
		}
		case kbGetProtoUnitID("Lampades"):
		{
			yAddToDatabase("SkyWitches", db);
			yAddUpdateVar("SkyWitches", "index", yGetNewestPointer(pName));
			yAddUpdateVar("SkyWitches", "step", 0);
			yAddUpdateVar("SkyWitches", "player", p);
		}
		case kbGetProtoUnitID("Einheriar"):
		{
			yAddToDatabase("Einherjars", db);
			yAddUpdateVar("Einherjars", "index", yGetNewestPointer(pName));
			yAddUpdateVar("Einherjars", "step", 0);
			yAddUpdateVar("Einherjars", "player", p);
		}
		case kbGetProtoUnitID("Statue of Lightning"):
		{
			yAddToDatabase("lightningStatues", db);
			yAddUpdateVar("lightningStatues", "index", yGetNewestPointer(pName));
			yAddUpdateVar("lightningStatues", "step", 0);
			yAddUpdateVar("lightningStatues", "player", p);
		}
		case kbGetProtoUnitID("Manticore"):
		{
			yAddToDatabase("Manticores", db);
		}
	}
}

int activatePlayerUnit(string db = "", int p = 0, int proto = 0, float decay = 0) {
	int index = yAddToDatabase("playerUnits", db);
	yAddUpdateVar("playerUnits", "player", p);
	yAddUpdateVar("playerUnits", "hero", 0);
	yAddUpdateVar("playerUnits", "decay", decay);
	yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 1000);
	yAddUpdateVar("playerUnits", "physicalResist", trQuestVarGet("proto"+proto+"armor"));
	yAddUpdateVar("playerUnits", "magicResist", trQuestVarGet("proto"+proto+"armor"));
	if (PvP) {
		if (trQuestVarGet("detached") == 1) {
			ySetPointer("enemies", 1*trQuestVarGet("enemiesLeaveIndex"));
		}
		yAddUpdateVar("playerUnits", "doppelganger", yAddToDatabase("enemies", db));
		yAddUpdateVar("enemies", "doppelganger", yGetNewestPointer("playerUnits"));
		yAddUpdateVar("enemies", "player", p);
		ySetPointer("enemies", 1*trQuestVarGet("enemiesDummyIndex"));
	}
	activateSpecialUnit(db, "playerUnits", proto, p);
	return(index);
}

int spawnPlayerUnit(int p = 0, int proto = 0, string vdb = "", float decay = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	string pName = kbGetProtoUnitName(proto);
	trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet(vdb+"x"),0,trQuestVarGet(vdb+"z"),0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit(pName);
	return(activatePlayerUnit("next", p, proto, decay));
}

void spawnPlayerClone(int p = 0, string vdb = "") {
	int class = trQuestVarGet("p"+p+"class");
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	yAddToDatabase("p"+p+"characters", "next");
	yAddUpdateVar("p"+p+"characters", "index", spawnPlayerUnit(p, 1*trQuestVarGet("class"+class+"proto"), vdb));
	yAddUpdateVar("playerUnits", "hero", 1);
	yAddToDatabase("playerCharacters", "next");
	yAddUpdateVar("playerCharacters", "player", p);
	yAddUpdateVar("playerUnits", "physicalResist", trQuestVarGet("p"+p+"physicalResist"));
	yAddUpdateVar("playerUnits", "magicResist", trQuestVarGet("p"+p+"magicResist"));
}

void spawnPlayer(int p = 0, string vdb = "") {
	trQuestVarSet("p"+p+"unit", trGetNextUnitScenarioNameNumber());
	spawnPlayerClone(p, vdb);
	trQuestVarSet("p"+p+"index", yGetNewestPointer("playerUnits"));
	if (trCurrentPlayer() == p) {
		int class = trQuestVarGet("p"+p+"class");
		string proto = kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto"));
		uiFindType(proto);
	}
}

void revivePlayer(int p = 0) {
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"reviveBeam");
	trUnitChangeProtoUnit("Rocket");
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"unit");
	trUnitDestroy();
	spawnPlayer(p, "dead"+p+"pos");
	trSoundPlayFN("herorevived.wav","1",-1,"","");
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"unit");
	trDamageUnitPercent(50);
	trQuestVarSet("deadPlayerCount", trQuestVarGet("deadPlayerCount") - 1);
	equipRelicsAgain(p);
	if (trCurrentPlayer() == p) {
		uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"unit"));
	}
}

void shootLaser(string start = "", string dir = "", float dist = -1, int p = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0", "Dwarf",1,trQuestVarGet(start+"x"),0,trQuestVarGet(start+"z"),0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
	if (dist < 0) {
		trSetSelectedScale(3.0,0.0,80.0);
		dist = 60.0;
	} else {
		trSetSelectedScale(3.0,0.0,dist * 1.3);
	}
	trSetUnitOrientation(xsVectorSet(0.0-trQuestVarGet(dir+"x"),0,0.0-trQuestVarGet(dir+"z")), xsVectorSet(0,1,0), true);
	yAddToDatabase("delayLasers", "next");
	yAddUpdateVar("delayLasers", "dirx", trQuestVarGet(dir+"x"));
	yAddUpdateVar("delayLasers", "dirz", trQuestVarGet(dir+"z"));
	yAddUpdateVar("delayLasers", "next", trTimeMS() + 2000);
	yAddUpdateVar("delayLasers", "phase", 0);
	yAddUpdateVar("delayLasers", "dist", dist);
	yAddUpdateVar("delayLasers", "player", p);
}

rule spy_find
active
highFrequency
{
	int x = 0;
	int id = 0;
	while(trQuestVarGet("spysearch") < trGetNextUnitScenarioNameNumber()) {
		id = kbGetBlockID(""+1*trQuestVarGet("spysearch"), true);
		if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Spy Eye")) {
			x = modularCounterNext("spyfound");
			if (trQuestVarGet("spyEye"+x+"active") == 1) {
				trQuestVarSet("spyEye"+x+"active", 0);
				trUnitSelectClear();
				trUnitSelectByQV("spyEye"+x+"unit");
				if (trUnitAlive() == false) {
					trQuestVarSet(trStringQuestVarGet("spyName"+x), -1);
				} else {
					trUnitSelectClear();
					trUnitSelectByID(id);
					trMutateSelected(1*trQuestVarGet("spyEye"+x));
					trQuestVarSet(trStringQuestVarGet("spyName"+x), trQuestVarGet("spysearch"));
				}
			}
		}
		trQuestVarSet("spysearch", 1 + trQuestVarGet("spysearch"));
	}
	if (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
		trQuestVarSet("spyreset", 1 + trQuestVarGet("spyreset"));
		if (trQuestVarGet("spyreset") >= 3) {
			while (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
				x = modularCounterNext("spyFound");
				if (trQuestVarGet("spyEye"+x+"active") == 1) {
					trQuestVarSet("spyEye"+x+"active", 0);
					trQuestVarSet(trStringQuestVarGet("spyName"+x), -1);
				}
			}
			debugLog("resetting spyfound");
		}
	} else {
		trQuestVarSet("spyreset", 0);
	}
}

string databaseName(int p = 0) {
	if (p == ENEMY_PLAYER) {
		return("enemies");
	} else {
		return("playerUnits");
	}
}

string opponentDatabaseName(int p = 0) {
	if (p == ENEMY_PLAYER) {
		return("playerUnits");
	} else {
		return("enemies");
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
