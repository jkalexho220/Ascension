const int ABILITY_READY = 0;
const int ABILITY_COOLDOWN = 1;
const int ABILITY_COST = 2;

const int ABILITY_OFF = 0;
const int ABILITY_ON = 1;

const int ON_HIT_NONE = 0;
const int ON_HIT_NORMAL = 1;
const int ON_HIT_SPECIAL = 2;

const int PROJ_NONE = 0;
const int PROJ_GROUND = 1;
const int PROJ_FALLING = 2;
const int PROJ_BOUNCE = 3;
const int PROJ_REMOVE = 4;

string wellName = "";
string lureName = "";
string rainName = "";

bool wellIsUltimate = false;
bool rainIsUltimate = false;
bool lureIsUltimate = false;

void spyEffect(int unit = 0, int proto = 0, string qv = "") {
	int x = modularCounterNext("spyFind");
	trQuestVarSet("spyEye"+x, proto);
	trQuestVarSet("spyEye"+x+"unit", unit);
	trStringQuestVarSet("spyName"+x, qv);
	trUnitSelectClear();
	trUnitSelect(""+unit, true);
	trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
}

void silencePlayer(int p = 0, float duration = 0, bool sfx = true) {
	float timeout = duration * 1000 + trTimeMS();
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
	}
}

void removeEnemy() {
	if (yGetVar("enemies", "bounty") > 0) {
		trQuestVarSetFromRand("rand", 1, yGetVar("enemies", "bounty"), true);
		for(p=1; <ENEMY_PLAYER) {
			if (Multiplayer) {
				trPlayerGrantResources(p, "Gold", yGetVar("enemies", "bounty"));
			}
			trPlayerGrantResources(p, "Favor", trQuestVarGet("rand"));
		}
	}
	if (yGetVar("enemies", "relic") > 0) {
		spawnRelicClosest(yGetVar("enemies", "posX"), yGetVar("enemies", "posZ"), 1*yGetVar("enemies", "relic"));
	}
	yRemoveFromDatabase("enemies");
	yRemoveUpdateVar("enemies", "bounty");
	yRemoveUpdateVar("enemies", "stunStatus");
	yRemoveUpdateVar("enemies", "stunTimeout");
	yRemoveUpdateVar("enemies", "stunSFX");
	yRemoveUpdateVar("enemies", "poisonStatus");
	yRemoveUpdateVar("enemies", "poisonTimeout");
	yRemoveUpdateVar("enemies", "poisonLast");
	yRemoveUpdateVar("enemies", "poisonDamage");
	yRemoveUpdateVar("enemies", "poisonSFX");
	yRemoveUpdateVar("enemies", "relic");
	yRemoveUpdateVar("enemies", "posX");
	yRemoveUpdateVar("enemies", "posZ");
	yRemoveUpdateVar("enemies", "launched");
}

void removePlayerUnit() {
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
	yRemoveUpdateVar("playerUnits", "launched");
}

void removePlayerCharacter() {
	yRemoveFromDatabase("playerCharacters");
}

void removePlayerSpecific(int p = 0) {
	if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
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
	}
	yRemoveFromDatabase("p"+p+"characters");
	yRemoveUpdateVar("p"+p+"Characters", "specialAttack");
	yRemoveUpdateVar("p"+p+"Characters", "attacking");
	yRemoveUpdateVar("p"+p+"Characters", "attackNext");
	yRemoveUpdateVar("p"+p+"characters", "index");
}

/*
called after confirming that the projectile is on WALL terrain.
*/
vector getBounceDir(string pos = "", string dir = "") {
	bool bounced = false;
	int xMod = 1;
	int zMod = 1;
	if (trQuestVarGet(dir+"x") < 0) {
		xMod = -1;
	}
	if (trQuestVarGet(dir+"z") < 0) {
		zMod = -1;
	}
	vectorToGrid(pos, "loc");
	vector ret = trVectorQuestVarGet(dir);
	/* time to bounce! */
	trQuestVarSet("horizontalX", trQuestVarGet("locX") - xMod);
	trQuestVarSet("horizontalZ", trQuestVarGet("locZ"));
	trQuestVarSet("verticalX", trQuestVarGet("locX"));
	trQuestVarSet("verticalZ", trQuestVarGet("locZ") - zMod);
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
		vector a = (trVectorQuestVarGet(pos) - (trVectorQuestVarGet("loc") * 2)) / 2;
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
	trVectorQuestVarSet("dir", zGetUnitVector(from, to, dist));
	trQuestVarSet(target+"x", trQuestVarGet("dirx") + trQuestVarGet(from+"x"));
	trQuestVarSet(target+"z", trQuestVarGet("dirz") + trQuestVarGet(from+"z"));

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
	duration = duration * 1000;
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
		damage = damage * trQuestVarGet("p"+p+"spellDamage");
	}
	if (trTimeMS() + duration > yGetVar(db, "poisonTimeout")) {
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
	}
	if (damage > yGetVar(db, "poisonDamage")) {
		ySetVar(db, "poisonDamage", damage);
	}
}

void healUnit(int p = 0, float amt = 0, int index = -1) {
	int old = yGetPointer("playerUnits");
	if (index > 0) {
		ySetPointer("playerUnits", index);
	}
	amt = amt * trQuestVarGet("p"+p+"healBoost");
	if (yGetVar("playerUnits", "poisonStatus") == 0) {
		trDamageUnit(0.0 - amt);
	}
	if (index > 0) {
		ySetPointer("playerUnits", old);
	}
}

void stunUnit(string db = "", float duration = 0, int p = 0) {
	trQuestVarSet("stunSound", 1);
	int index = 0;
	duration = duration * 1000;
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
		if (trQuestVarGet("p"+p+"class") == FROSTKNIGHT) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"unit");
			healUnit(p, 0.05 * trQuestVarGet("p"+p+"health"), 1*trQuestVarGet("p"+p+"index"));
			trUnitSelectClear();
			trUnitSelectByQV(db);
		}
	} else if (p < 0) {
		duration = duration * trQuestVarGet("p"+(0-p)+"stunResistance");
	}
	if (trTimeMS() + duration > yGetVar(db, "stunTimeout")) {
		if (yGetVar(db, "stunStatus") == 0) {
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
		}
		ySetVar(db, "stunTimeout", trTimeMS() + duration);
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
			if (yGetVar("launchedUnits", "player") == ENEMY_PLAYER) {
				db = "enemies";
			}
			ySetVarAtIndex(db, "launched", 0, 1*yGetVar("launchedUnits", "index"));
			trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("launchedUnits", "proto")));
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("launchedUnits", "unit"));
			trMutateSelected(1*yGetVar("launchedUnits", "proto"));
		} else {
			trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("launchedUnits", "proto")));
			if (yGetVar("launchedUnits", "unit") == trQuestVarGet("bossUnit")) {
				trUnitSelectClear();
				trUnitSelectByQV("launchedUnits");
				trSetSelectedScale(trQuestVarGet("bossScale"),trQuestVarGet("bossScale"),trQuestVarGet("bossScale"));
			}
		}
		trUnitSelectClear();
		trUnitSelectByQV("launchedUnits");
		trUnitChangeProtoUnit("Dust Small");
		yRemoveFromDatabase("launchedUnits");
	}
}

void launchUnit(string db = "", string dest = "") {
	if (yGetVar(db, "launched") == 0) {
		ySetVar(db, "launched", 1);
		int type = kbGetUnitBaseTypeID(kbGetBlockID(""+1*trQuestVarGet(db)));
		int owner = ENEMY_PLAYER;
		trUnitSelectClear();
		trUnitSelectByQV(db);
		if (trUnitIsOwnedBy(ENEMY_PLAYER) == false) {
			owner = yGetVar(db, "player");
		}
		trUnitChangeProtoUnit("Transport Ship Greek");

		trVectorSetUnitPos("start", db);
		trVectorQuestVarSet("dir", zGetUnitVector("start", dest));

		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
		trUnitSelectClear();
		trUnitSelectByQV("next");
		trImmediateUnitGarrison(""+1*trQuestVarGet(db));
		trUnitConvert(owner);
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
		yAddUpdateVar("launchedUnits", "player", owner);
		yAddUpdateVar("launchedUnits", "proto", type);
		yAddUpdateVar("launchedUnits", "destX", trQuestVarGet("startx"));
		yAddUpdateVar("launchedUnits", "destz", trQuestVarGet("startz"));
		yAddUpdateVar("launchedUnits", "timeout", trTimeMS() + 1100 * dist / 15);
	}
}


/*
Enemies have elemental resistance and weakness
*/
float damageEnemy(int p = 0, float dmg = 0, bool spell = true) {
	trDamageUnit(dmg);
	if (spell) {
		trQuestVarSet("p"+p+"lifestealTotal", 
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"spellLifesteal") * dmg);
	} else {
		trQuestVarSet("p"+p+"lifestealTotal", 
			trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"attackLifesteal") * dmg);
	}
	return(dmg);
}

/*
protection blocks all damage.
*/
void damagePlayerUnit(float dmg = 0, int index = -1) {
	int old = 0;
	if (index > 0) {
		old = yGetPointer("playerUnits");
		ySetPointer("playerUnits", index);
	}
	if (trQuestVarGet("protectionCount") == 0) {
		trDamageUnit(dmg);
	}
	if (index > 0) {
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
    		ySetPointer("stunnedUnits", 1*yGetVar(db, "stunStatus"));
    		yRemoveFromDatabase("stunnedUnits");
    		ySetVar(db, "stunStatus", 0);
    	}
	}
}

int CheckOnHit(int p = 0, int id = 0) {
    int action = kbUnitGetAnimationActionType(id);
    int status = ON_HIT_NONE;
    if (yGetVar("p"+p+"characters", "attacking") == 0) {
        if ((action == 12) || (action == 6)) {
            ySetVar("p"+p+"characters", "attacking", 1);
            ySetVar("p"+p+"characters", "attackNext", trTimeMS() + trQuestVarGet("p"+p+"firstDelay"));
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
                /* lifesteal */
                trQuestVarSet("p"+p+"lifestealTotal", 
					trQuestVarGet("p"+p+"lifestealTotal") + trQuestVarGet("p"+p+"attackLifesteal") * trQuestVarGet("p"+p+"attack"));
            }
        } else {
            ySetVar("p"+p+"characters", "attacking", 0);
        }
    }
    return(status);
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
		if (trQuestVarGet("posY") < worldHeight + 0.5) {
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
	int proto=0,int anim=0,float speed=10.0,float height=4,float scale=0) {
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
	zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, speed);
	trUnitConvert(ENEMY_PLAYER);
	trSetUnitOrientation(trVectorQuestVarGet(dir), vector(0,1,0), true);
	trSetSelectedScale(0, 0.0 - height, 0);
	trDamageUnitPercent(100);
	return(index);
}

rule spy_find
active
highFrequency
{
	if ((trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) == false) {
		while(trQuestVarGet("spysearch") < trGetNextUnitScenarioNameNumber()) {
			int id = kbGetBlockID(""+1*trQuestVarGet("spysearch"), true);
			if (id >= 0) {
				if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Spy Eye")) {
					int x = modularCounterNext("spyfound");
					trUnitSelectClear();
					trUnitSelectByQV("spyEye"+x+"unit");
					while(trUnitAlive() == false) {
						if (trQuestVarGet("spyfound") == trQuestVarGet("spyfind")) {
							break;
						}
						x = modularCounterNext("spyfound");
						trUnitSelectClear();
						trUnitSelectByQV("spyEye"+x+"unit");
					}
					trUnitSelectClear();
					trUnitSelectByID(id);
					trMutateSelected(1*trQuestVarGet("spyEye"+x));
					trQuestVarSet(trStringQuestVarGet("spyName"+x), trQuestVarGet("spysearch"));
				}
			}
			trQuestVarSet("spysearch", 1 + trQuestVarGet("spysearch"));
		}
		trQuestVarSet("spyreset", 1 + trQuestVarGet("spyreset"));
		if (trQuestVarGet("spyreset") >= 10) {
			trQuestVarSet("spyfound", trQuestVarGet("spyfind"));
		}
	} else {
		trQuestVarSet("spyreset", 0);
		trQuestVarSet("spysearch", trGetNextUnitScenarioNameNumber());
	}
}
