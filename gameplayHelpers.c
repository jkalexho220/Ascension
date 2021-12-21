
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
		trChatSend(0, "Collected " + 1*yGetVar("enemies", "bounty") + " <icon=(24)(icons/icon resource gold)>");
		for(p=1; <ENEMY_PLAYER) {
			trPlayerGrantResources(p, "Gold", yGetVar("enemies", "bounty"));
			trPlayerGrantResources(p, "Favor", yGetVar("enemies", "bounty"));
		}
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
	yRemoveUpdateVar("enemies", "frostCount");
	yRemoveUpdateVar("enemies", "shockCount");
	for(x=1; <=5) {
		yRemoveUpdateVar("enemies", "resist"+x);
	}	
}

void removePlayerCharacter() {
	yRemoveFromDatabase("playerCharacters");
	yRemoveUpdateVar("playerCharacters", "player");
	yRemoveUpdateVar("playerCharacters", "specialAttack");
	yRemoveUpdateVar("playerCharacters", "attacking");
	yRemoveUpdateVar("playerCharacters", "attackNext");
	yRemoveUpdateVar("playerCharacters", "firstDelay");
	yRemoveUpdateVar("playerCharacters", "nextDelay");
}

void removePlayerUnit() {
	yRemoveFromDatabase("playerUnits");
	yRemoveUpdateVar("playerUnits", "stunStatus");
	yRemoveUpdateVar("playerUnits", "stunTimeout");
	yRemoveUpdateVar("playerUnits", "stunSFX");
	yRemoveUpdateVar("playerUnits", "poisonStatus");
	yRemoveUpdateVar("playerUnits", "poisonTimeout");
	yRemoveUpdateVar("playerUnits", "poisonLast");
	yRemoveUpdateVar("playerUnits", "poisonDamage");
	yRemoveUpdateVar("playerUnits", "poisonSFX");
	yRemoveUpdateVar("playerUnits", "decay");
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

int spyEffect(int unit = 0, int proto = 0) {
	int x = modularCounterNext("spyFind");
	trQuestVarSet("spyEye"+x, 0 - proto);
	trUnitSelectClear();
	trUnitSelect(""+unit, true);
	trTechInvokeGodPower(0, "spy", vector(0,0,0), vector(0,0,0));
	return(x);
}

void poisonUnit(string db = "", float duration = 0, float damage = 0, int p = 0) {
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
		damage = damage * trQuestVarGet("p"+p+"spellDamage");
	}
	if (trTimeMS() + duration > yGetVar(db, "poisonTimeout")) {
		if (yGetVar(db, "poisonStatus") == 0) {
			if (yGetVar(db, "poisonSFX") == 0) {
				ySetVar(db, "poisonSFX", 0 - spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Shockwave poison effect")));
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
	if (p > 0) {
		duration = duration * trQuestVarGet("p"+p+"spellDuration");
	}
	if (trTimeMS() + duration > yGetVar(db, "stunTimeout")) {
		if (yGetVar(db, "stunStatus") == 0) {
			yAddToDatabase("stunnedUnits", db);
			yAddUpdateVar("stunnedUnits", "proto", kbGetUnitBaseTypeID(kbGetBlockID(""+1*trQuestVarGet(db), true)));
			if (yGetVar(db, "stunSFX") == 0) {
				ySetVar(db, "stunSFX", 0 - spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Shockwave stun effect")));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar(db, "stunSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Shockwave stun effect"));
			}
			ySetVar(db, "stunStatus", 1);
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
	trDamageUnit(0.0 - amt);
}

/*
Enemies have elemental resistance and weakness
*/
float damageEnemy(int p = 0, float dmg = 0, bool spell = false) {
	trDamageUnit(dmg);
	return(dmg);
}

void stunsAndPoisons(string db = "") {
	if (yGetVar(db, "poisonStatus") == 1) {
		if ((yGetVar(db, "poisonSFX") < 0) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
    		ySetVar(db, "poisonSFX", trQuestVarGet("spyEye"+(0-yGetVar(db, "poisonSFX"))));
    	}
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
	if (yGetVar(db, "stunStatus") == 1) {
		if ((yGetVar(db, "stunSFX") < 0) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
    		ySetVar(db, "stunSFX", trQuestVarGet("spyEye"+(0-yGetVar(db, "stunSFX"))));
    	}
    	if (trTimeMS() > yGetVar(db, "stunTimeout")) {
    		ySetVar(db, "stunStatus", 0);
    		trUnitSelectClear();
    		trUnitSelect(""+1*yGetVar(db, "stunSFX"), true);
    		trMutateSelected(kbGetProtoUnitID("Rocket"));
    		yRemoveAllCopies("stunnedUnits", 1*trQuestVarGet(db));
    	}
	}
}

/* values are names */
void OnHitEffects(int p = 0, int attacker = 0, int target = 0) {

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
