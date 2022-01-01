const int ABILITY_READY = 0;
const int ABILITY_COOLDOWN = 1;
const int ABILITY_COST = 2;

const int ABILITY_OFF = 0;
const int ABILITY_ON = 1;

const int ON_HIT_NONE = 0;
const int ON_HIT_NORMAL = 1;
const int ON_HIT_SPECIAL = 2;

string wellName = "";
string lureName = "";
string rainName = "";

bool wellIsUltimate = false;
bool rainIsUltimate = false;
bool lureIsUltimate = false;

int spyEffect(int unit = 0, int proto = 0, string qv = "") {
	int x = modularCounterNext("spyFind");
	trQuestVarSet("spyEye"+x, 0 - proto);
	trStringQuestVarSet("spyName"+x, qv);
	trUnitSelectClear();
	trUnitSelect(""+unit, true);
	trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
	return(x);
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

void removeArrow() {
	for(x=2;>0) {
		if (yGetVar("arrowsActive", "sfx"+x) > 0) {
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("arrowsActive", "sfx"+x), true);
			trUnitChangeProtoUnit("Rocket");
		}
	}
	trUnitSelectClear();
	trUnitSelect(""+1*trQuestVarGet("arrowsActive"), true);
	yRemoveFromDatabase("arrowsActive");
	yRemoveUpdateVar("arrowsActive", "destx");
	yRemoveUpdateVar("arrowsActive", "destz");
	yRemoveUpdateVar("arrowsActive", "timeout");
	yRemoveUpdateVar("arrowsActive", "element");
	yRemoveUpdateVar("arrowsActive", "damage");
	yRemoveUpdateVar("arrowsActive", "player");
	yRemoveUpdateVar("arrowsActive", "sfx1");
	yRemoveUpdateVar("arrowsActive", "sfx2");
	yRemoveUpdateVar("arrowsActive", "special");
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
}

void removePlayerCharacter(int p = 0) {
	if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
		for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
			yDatabaseNext("p"+p+"relics", true);
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		trQuestVarSet("p"+p+"dead", 1);
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


bool collideWithTerrain(int arrow = 0) {
	bool collide = false;
	trVectorQuestVarSet("loc", kbGetBlockPosition(""+arrow));
	vectorToGrid("loc", "loc");
	if (trCountUnitsInArea(""+arrow,ENEMY_PLAYER,"Phoenix Egg", 1.5) > 0) {
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		collide = true;
	} else if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		collide = true;
	}
	return(collide);
}


bool checkArrowDie() {
	int arrow = trQuestVarGet("arrowsActive");
	trQuestVarSet("endx", yGetVar("arrowsActive", "destx"));
	trQuestVarSet("endz", yGetVar("arrowsActive", "destz"));
	if ((collideWithTerrain(arrow)) ||
		zDistanceToVectorSquared("arrowsActive", "end") < 9 ||
		trTimeMS() > yGetVar("arrowsActive", "timeout")) {
		return(true);
	} else {
		return(false);
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
				spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Shockwave poison effect"), yGetVarName(db, "poisonSFX"));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar(db, "poisonSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Poison SFX"));
			}
			ySetVar(db, "poisonStatus", 1);
			ySetVar(db, "poisonLast", trTimeMS());
		}
		ySetVar(db, "poisonTimeout", trTimeMS() + duration);
	}
	if (damage > yGetVar(db, "poisonDamage")) {
		ySetVar(db, "poisonDamage", damage);
	}
}

void stunUnit(string db = "", float duration = 0, int p = 0) {
	trQuestVarSet("stunSound", 1);
	int index = 0;
	duration = duration * 1000;
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
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

int addEffect(int car = 0, string proto = "", string anim = "0,0,0,0,0,0,0") {
	int sfx = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	trUnitSelectClear();
	trUnitSelect(""+sfx, true);
	trMutateSelected(kbGetProtoUnitID(proto));
	trUnitSetAnimationPath(anim);
	trMutateSelected(kbGetProtoUnitID("Relic"));
	trImmediateUnitGarrison(""+car);
	trMutateSelected(kbGetProtoUnitID(proto));
	return(sfx);
}

void healUnit(int p = 0, float amt = 0) {
	amt = amt * trQuestVarGet("p"+p+"healBoost");
	trDamageUnit(0.0 - amt);
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
float damagePlayerUnit(float dmg = 0) {
	if (trQuestVarGet("protectionCount") == 0) {
		trDamageUnit(dmg);
	}
}

void stunsAndPoisons(string db = "") {
	if (yGetVar(db, "poisonStatus") == 1) {
    	if (trTimeMS() > yGetVar(db, "poisonTimeout")) {
    		ySetVar(db, "poisonStatus", 0);
    		trUnitSelectClear();
    		trUnitSelect(""+1*yGetVar(db, "poisonSFX"), true);
    		trMutateSelected(kbGetProtoUnitID("Rocket"));
    	} else {
    		trDamageUnit((trTimeMS() - yGetVar(db, "poisonLast")) * yGetVar(db, "poisonDamage") * 0.001);
    		ySetVar(db, "poisonLast", trTimeMS());
    	}
	}
	if (yGetVar(db, "stunStatus") >= 1) {
    	if (trTimeMS() > yGetVar(db, "stunTimeout")) {
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

void processGenericProj() {
	int id = 0;
	float scale = 0;
	if (yGetDatabaseCount("genericProj") > 0) {
		for(x=1+yGetDatabaseCount("genericProj")/4; >0) {
			id = yDatabaseNext("genericProj", true);
			if (id == -1) {
				yRemoveFromDatabase("genericProj");
			} else if (yGetVar("genericProj", "yeehaw") == 1) {
				trMutateSelected(1*yGetVar("genericProj", "proto"));
				trUnitOverrideAnimation(1*yGetVar("genericProj", "anim"),0,true,true,-1);
				scale = yGetVar("genericProj", "scale");
				trSetSelectedScale(scale,scale,scale);
				ySetVar("genericProj", "yeehaw", 0);
			} else if (yGetVar("genericProj", "yeehaw") == 2) {
				/* first time search */
				ySetVar("genericProj", "yeehaw", 1);
			} else {
				trVectorSetUnitPos("pos", "genericProj");
				if (trQuestVarGet("posY") < worldHeight + 0.5) {
					yVarToVector("genericProj", "dir");
					zSetProtoUnitStat("Kronny Flying", ENEMY_PLAYER, 1, yGetVar("genericProj", "speed"));
					trUnitChangeProtoUnit("Kronny Flying");
					trUnitSelectClear();
					trUnitSelectByQV("genericProj");
					trDamageUnitPercent(-100);
					trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"), vector(0,1,0), true);
					trSetSelectedScale(0,0.0-yGetVar("genericProj", "height"),0);
					trDamageUnitPercent(100);
					ySetVar("genericProj", "yeehaw", 1);
				}
			}
		}
	}
}

int addGenericProj(string start="",string dir="",int proto=0,int anim=0,float speed=10.0,float height=4,float scale=0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	int index = yAddToDatabase("genericProj", "next");
	yAddUpdateVar("genericProj", "proto", proto);
	yAddUpdateVar("genericProj", "anim", anim);
	yAddUpdateVar("genericProj", "speed", speed);
	yAddUpdateVar("genericProj", "height", height);
	yAddUpdateVar("genericProj", "scale", scale);
	yAddUpdateVar("genericProj", "dirX", trQuestVarGet(dir+"x"));
	yAddUpdateVar("genericProj", "dirZ", trQuestVarGet(dir+"z"));
	yAddUpdateVar("genericProj", "yeehaw", 2);

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
					trUnitSelectByID(id);
					trMutateSelected(0 - trQuestVarGet("spyEye"+x));
					trQuestVarSet("spyEye"+x, trQuestVarGet("spysearch"));
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
