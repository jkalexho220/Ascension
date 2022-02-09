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


void gainFavor(int p = 0, float amt = 0) {
	trQuestVarSet("p"+p+"favor", xsMin(100, xsMax(0, trQuestVarGet("p"+p+"favor") + amt)));
	trPlayerGrantResources(p,"favor", trQuestVarGet("p"+p+"favor") - trPlayerResourceCount(p, "favor"));
}

void spyEffect(int unit = 0, int proto = 0, string qv = "") {
	trUnitSelectClear();
	trUnitSelect(""+unit, true);
	if (trUnitAlive()) {
		int x = modularCounterNext("spyFind");
		trQuestVarSet("spyEye"+x, proto);
		trQuestVarSet("spyEye"+x+"unit", unit);
		trStringQuestVarSet("spyName"+x, qv);
		trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
	}
}

void silencePlayer(int p = 0, float duration = 0, bool sfx = true) {
	if (trQuestVarGet("p"+p+"negationCloak") == 1) {
		if (getBit(STATUS_SILENCE, 1*trQuestVarGet("p"+p+"spellstealStatus")) == false) {
			trQuestVarSet("p"+p+"spellstealStatus", trQuestVarGet("p"+p+"spellstealStatus") + xsPow(2, STATUS_SILENCE));
			trSoundPlayFN("shadeofhadesgrunt2.wav","1",-1,"","");
			gainFavor(p, 5);
			if (trCurrentPlayer() == p) {
				trChatSend(0, "<color=1,1,1>Silence absorbed! Your next spell will inflict Silence!</color>");
			}
		}
	} else {
		float timeout = duration * 1000 * trQuestVarGet("p"+p+"silenceResistance") + trTimeMS();
		if (trQuestVarGet("p"+p+"silenceTimeout") < timeout) {
			trQuestVarSet("p"+p+"silenceTimeout", timeout);
		}
		if (trQuestVarGet("p"+p+"silenced") == 0) {
			trQuestVarSet("p"+p+"silenced", 1);
			trSoundPlayFN("frostgiantmove1.wav","1",-1,"","");
			trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has been silenced!");
			trPlayerKillAllGodPowers(p);
			if (sfx) {
				if (trQuestVarGet("p"+p+"silenceSFX") > 0) {
					int id = kbGetBlockID(""+1*trQuestVarGet("p"+p+"silenceSFX"));
					if (id == -1) {
						spyEffect(1*trQuestVarGet("p"+p+"unit"), kbGetProtoUnitID("UI Range Indicator Egypt SFX"), "p"+p+"silenceSFX");
					} else {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"silenceSFX");
						trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
					}
				} else {
					spyEffect(1*trQuestVarGet("p"+p+"unit"), kbGetProtoUnitID("UI Range Indicator Egypt SFX"), "p"+p+"silenceSFX");
				}
			}
			if (trCurrentPlayer() == p) {
				trCounterAbort("lure");
				trCounterAbort("well");
				trCounterAbort("rain");
				trCounterAddTime("silence", -1, -9999, "SILENCED");
			}
		}
	}
}

void silenceEnemy(int p = 0, float duration = 9.0) {
	duration = 1000 * duration * trQuestVarGet("p"+p+"spellDuration");
	if (trTimeMS() + duration > yGetVar("enemies", "silenceTimeout")) {
		ySetVar("enemies", "silenceTimeout", trTimeMS() + duration);
	}
	if (trQuestVarGet("enemies") == trQuestVarGet("bossUnit")) {
		if (trTimeMS() + duration > trQuestVarGet("bossCooldownTime")) {
			trQuestVarSet("bossCooldownTime", trTimeMS() + duration);
		}
	}
	if (yGetVar("enemies", "silenceStatus") == 0) {
		ySetVar("enemies", "silenceStatus", 1);
		if (kbGetBlockID(""+1*yGetVar("enemies", "silenceSFX")) == -1) {
			spyEffect(1*trQuestVarGet("enemies"), 
				kbGetProtoUnitID("UI Range Indicator Egypt SFX"), yGetVarName("enemies", "silenceSFX"));
		} else {
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("enemies", "silenceSFX"), true);
			trUnitChangeProtoUnit("UI Range Indicator Egypt SFX");
			trUnitSelectClear();
			trUnitSelectByQV("enemies");
		}
	}
}

void healUnit(int p = 0, float amt = 0, int index = -1) {
	int old = yGetPointer("playerUnits");
	if (index < 0) {
		index = old;
	}
	if (ySetPointer("playerUnits", index)) {
		amt = amt * trQuestVarGet("p"+p+"healBoost");
		if (yGetVar("playerUnits", "poisonStatus") == 0) {
			trDamageUnit(0.0 - amt);
		}
		ySetPointer("playerUnits", old);
	}
}

void nightriderHarvest(string pos = "") {
	int index = yGetPointer("playerUnits");
	for(p=1; < ENEMY_PLAYER) {
		if ((trQuestVarGet("p"+p+"class") == NIGHTRIDER) &&
			(trQuestVarGet("p"+p+"dead") == 0)) {
			if (zDistanceToVectorSquared("p"+p+"unit", pos) < 144) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"unit", true);
				ySetPointer("playerUnits", 1*trQuestVarGet("p"+p+"index"));
				healUnit(p, 0.05 * trQuestVarGet("p"+p+"health") * trQuestVarGet("p"+p+"spellDamage"));
				gainFavor(p, 1);
			}
		}
	}
	ySetPointer("playerUnits", index);
}

void removeEnemy() {
	yVarToVector("enemies", "pos");
	nightriderHarvest("pos");
	if (yGetVar("enemies", "bounty") > 0) {
		trQuestVarSetFromRand("rand", 1, yGetVar("enemies", "bounty"), true);
		for(p=1; <ENEMY_PLAYER) {
			if (Multiplayer) {
				trQuestVarSet("p"+p+"gold", trQuestVarGet("p"+p+"gold") + yGetVar("enemies", "bounty"));
				trPlayerGrantResources(p, "Gold", yGetVar("enemies", "bounty"));
			}
			gainFavor(p, trQuestVarGet("rand"));
		}
	}
	if (yGetVar("enemies", "relic") > 0) {
		spawnRelicClosest(trQuestVarGet("posX"), trQuestVarGet("posZ"), 1*yGetVar("enemies", "relic"));
	}
	yRemoveFromDatabase("enemies");
}

void removePlayerUnit() {
	yVarToVector("playerUnits", "pos");
	nightriderHarvest("pos");
	yRemoveFromDatabase("playerUnits");
	yRemoveUpdateVar("playerUnits", "player");
	yRemoveUpdateVar("playerUnits", "currentHealth");
	yRemoveUpdateVar("playerUnits", "stunStatus");
	yRemoveUpdateVar("playerUnits", "stunTimeout");
	yRemoveUpdateVar("playerUnits", "stunSFX");
	yRemoveUpdateVar("playerUnits", "poisonStatus");
	yRemoveUpdateVar("playerUnits", "poisonTimeout");
	yRemoveUpdateVar("playerUnits", "poisonLast");
	yRemoveUpdateVar("playerUnits", "poisonDamage");
	yRemoveUpdateVar("playerUnits", "poisonSFX");
	yRemoveUpdateVar("playerUnits", "decay");
	yRemoveUpdateVar("playerUnits", "decayNext");
	yRemoveUpdateVar("playerUnits", "hero");
	yRemoveUpdateVar("playerUnits", "unity");
	yRemoveUpdateVar("playerUnits", "launched");
}

void removePlayerCharacter() {
	yRemoveFromDatabase("playerCharacters");
}

void removePlayerSpecific(int p = 0) {
	if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
		if (kbGetBlockID(""+1*trQuestVarGet("p"+p+"silenceSFX")) >= 0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"silenceSFX", true);
			trUnitChangeProtoUnit("Rocket");
		}
		trVectorSetUnitPos("dead"+p+"pos", "p"+p+"unit");
		for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
			yDatabaseNext("p"+p+"relics", true);
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		trQuestVarSet("p"+p+"dead", 10);
		trQuestVarSet("deadPlayerCount", 1 + trQuestVarGet("deadPlayerCount"));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"unit");
		trUnitOverrideAnimation(6,0,false,false,-1);
		trSoundPlayFN("aherohasfallen.wav","1",-1,"","");
		trMessageSetText(trStringQuestVarGet("p"+p+"name") + " has fallen! Clear nearby enemies to revive them!");
		silencePlayer(p, 0);
		trQuestVarSet("p"+p+"silenceSFX", 0);
		trQuestVarSet("p"+p+"reviveBeam", trGetNextUnitScenarioNameNumber());
		trVectorSetUnitPos("pos", "p"+p+"unit");
		trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
		trArmySelect(""+p+",0");
		trUnitChangeProtoUnit("Healing SFX");
		/* NOOOO MY QUEEEEN */
		if (trQuestVarGet("p"+p+"simp") > 0) {
			int simp = trQuestVarGet("p"+p+"simp");
			trUnitSelectClear();
			trUnitSelectByQV("p"+simp+"tether");
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			trQuestVarSet("p"+simp+"queen", 0);
			trQuestVarSet("p"+p+"simp", 0);
		}
	}
	yRemoveFromDatabase("p"+p+"characters");
	yRemoveUpdateVar("p"+p+"Characters", "specialAttack");
	yRemoveUpdateVar("p"+p+"Characters", "attacking");
	yRemoveUpdateVar("p"+p+"Characters", "attackNext");
	yRemoveUpdateVar("p"+p+"characters", "index");
}

void equipRelicsAgain(int p = 0) {
	for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
		yDatabaseNext("p"+p+"relics", true);
		trUnitChangeProtoUnit("Relic");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"relics");
		trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
		trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
		if (yGetVar("p"+p+"relics", "type") <= NORMAL_RELICS) {
			trSetSelectedScale(0,0,-1);
			trUnitSetAnimationPath("1,0,1,1,0,0,0");
		}
	}
}

vector wallNormalVector(string loc = "") {
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
	bool targetPlayers = (p == 0);
	duration = duration * 1000;
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration") * xsPow(0.5, 1*trQuestVarGet("p"+p+"poisonSpeed"));
		damage = damage * trQuestVarGet("p"+p+"spellDamage") * xsPow(2, 1*trQuestVarGet("p"+p+"poisonSpeed"));
	} else {
		p = yGetVar(db, "player");
		duration = duration * trQuestVarGet("p"+p+"poisonResistance");
	}
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



/*
Enemies have elemental resistance and weakness
*/
float damageEnemy(int p = 0, float dmg = 0, bool spell = true) {
	if (spell) {
		dmg = dmg - dmg * yGetVar("enemies", "magicResist");
	} else {
		dmg = dmg - dmg * yGetVar("enemies", "physicalResist");
	}
	if (yGetVar("enemies", "poisonStatus") == 1) {
		dmg = dmg * trQuestVarGet("p"+p+"poisonKiller");
	}
	trDamageUnit(dmg);
	if (spell) {
		trQuestVarSet("p"+p+"lifestealTotal", 
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"Lifesteal") * dmg * 0.5);
	} else {
		trQuestVarSet("p"+p+"lifestealTotal", 
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"Lifesteal") * dmg);
	}
	return(dmg);
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

void stunUnit(string db = "", float duration = 0, int p = 0, bool sound = true) {
	int index = 0;
	bool targetPlayers = (p == 0);
	duration = duration * 1000;
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
		if (trQuestVarGet("p"+p+"class") == FROSTKNIGHT) {
			trQuestVarSet("p"+p+"lifestealTotal", trQuestVarGet("p"+p+"lifestealTotal") + 0.08 * trQuestVarGet("p"+p+"health"));
		}
		trUnitSelectClear();
		trUnitSelectByQV(db);
		if (trQuestVarGet("p"+p+"stunDamage") > 0) {
			damageEnemy(p, trQuestVarGet("p"+p+"health") * trQuestVarGet("p"+p+"stunDamage"), true);
		}
	} else {
		p = yGetVar(db, "player");
		duration = duration * trQuestVarGet("p"+p+"stunResistance");
	}
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
		if (trQuestVarGet("protectionCount") == 0) {
			dmg = dmg - dmg * yGetVar("playerUnits", "magicResist");
			if ((yGetVar("playerUnits", "hero") == 1) && trQuestVarGet("p"+p+"negationCloak") == 1) {
				healUnit(p, dmg);
			} else {
				trDamageUnit(dmg);
			}
		}
		ySetPointer("playerUnits", old);
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

int CheckOnHit(int p = 0, int id = 0) {
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
            	gainFavor(p, trQuestVarGet("p"+p+"favorFromAttacks"));
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
	return(decay / trQuestVarGet("p"+p+"spellDuration"));
}

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

	trArmyDispatch("1,0", "Dwarf",1,trQuestVarGet(start+"x"),0,trQuestVarGet(start+"z"),0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next", true);
	trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
	zSetProtoUnitStat("Kronny Flying", p, 1, speed);
	trUnitConvert(p);
	trSetUnitOrientation(trVectorQuestVarGet(dir), vector(0,1,0), true);
	trSetSelectedScale(0, 0.0 - height, 0);
	trDamageUnitPercent(100);
	return(index);
}

int activatePlayerUnit(string db = "", int p = 0, int proto = 0, float decay = 0) {
	int index = yAddToDatabase("playerUnits", db);
	yAddUpdateVar("playerUnits", "player", p);
	yAddUpdateVar("playerUnits", "hero", 0);
	yAddUpdateVar("playerUnits", "decay", decay);
	yAddUpdateVar("playerUnits", "decayNext", trTimeMS() + 1000);
	yAddUpdateVar("playerUnits", "physicalResist", trQuestVarGet("proto"+proto+"armor"));
	yAddUpdateVar("playerUnits", "magicResist", trQuestVarGet("proto"+proto+"armor"));
    return(index);
}

int spawnPlayerUnit(int p = 0, int proto = 0, string vdb = "", float decay = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	int index = activatePlayerUnit("next", p, proto, decay);
	string pName = kbGetProtoUnitName(proto);
    trArmyDispatch(""+p+",0",pName,1,trQuestVarGet(vdb+"x"),0,trQuestVarGet(vdb+"z"),0,true);
    return(index);
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

void shootLaser(string start = "", string dir = "", float dist = -1) {
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
		trQuestVarSet("spysearch", 1 + trQuestVarGet("spysearch"));
	}
	if (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
		trQuestVarSet("spyreset", 1 + trQuestVarGet("spyreset"));
		if (trQuestVarGet("spyreset") >= 3) {
			while (trQuestVarGet("spyfind") != trQuestVarGet("spyfound")) {
				x = modularCounterNext("spyFound");
				trQuestVarSet(trStringQuestVarGet("spyName"+x), -1);
			}
			debugLog("resetting spyfound");
		}
	} else {
		trQuestVarSet("spyreset", 0);
	}
}
