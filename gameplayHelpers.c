const int NONE = 0;
const int FIRE = 1;
const int ICE = 2;
const int THUNDER = 3;
const int LIGHT = 4;
const int DARK = 5;

const int SPECIAL_NONE = 6;
const int SPECIAL_FIRE = 7;
const int SPECIAL_ICE = 8;
const int SPECIAL_THUNDER = 9;
const int SPECIAL_LIGHT = 10;
const int SPECIAL_DARK = 11;

const int WALL_BRICK = 2;
const int WALL_WOOD = 3;
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


bool terrainIsType(string qv = "", int type = 0, int subtype = 0) {
	bool isType = trGetTerrainType(trQuestVarGet(qv+"x"),trQuestVarGet(qv+"z")) == type;
	isType = trGetTerrainSubType(trQuestVarGet(qv+"x"),trQuestVarGet(qv+"z")) == subtype;
	return(isType);
}

string getElementColor(int element = 0) {
	switch(element)
	{
		case NONE:		{ return(""); }
		case FIRE:		{ return("<color=0.9,0.2,0>"); }
		case ICE:		{ return("<color=0.4,0.4,1.0>"); }
		case LIGHT:		{ return("<color=1,1,0.5>"); }
		case DARK:		{ return("<color=0.3,0,0.3>"); }
		case THUNDER:	{ return("<color=0.1,0.9,0.7>"); }
	}
	return("NOT AN ELEMENT");
}

string getElementName(int element = 0) {
	switch(element)
	{
		case NONE:		{ return(""); }
		case FIRE:		{ return("Fire"); }
		case ICE:		{ return("Ice"); }
		case LIGHT:		{ return("Light"); }
		case DARK:		{ return("Wind"); }
		case THUNDER:	{ return("Thunder"); }
	}
	return("NOT AN ELEMENT");
}


void removeArrow() {
	for(x=2;<0) {
		if (yGetVar("arrowsActive", "sfx"+x) > 0) {
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("arrowsActive", "sfx"+x), true);
			trMutateSelected(kbGetProtoUnitID("Rocket"));
			trUnitDestroy();
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
		for(p=1; < ENEMY_PLAYER) {
			trPlayerGrantResources(p, "Gold", 1*yGetVar("enemies", "bounty"));
		}
	}
	yRemoveFromDatabase("enemies");
	yRemoveUpdateVar("enemies", "bounty");
	yRemoveUpdateVar("enemies", "stunTimeout");
	yRemoveUpdateVar("enemies", "stunSFX");
	yRemoveUpdateVar("enemies", "poisonTimeout");
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
}

void removePlayerUnit() {
	yRemoveFromDatabase("playerUnits");
	yRemoveUpdateVar("playerUnits", "stunTimeout");
	yRemoveUpdateVar("playerUnits", "stunSFX");
	yRemoveUpdateVar("playerUnits", "poisonTimeout");
	yRemoveUpdateVar("playerUnits", "poisonDamage");
	yRemoveUpdateVar("playerUnits", "poisonSFX");
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

void stunUnit(string db = "", float duration = 0) {
	if (trTimeMS() + duration > yGetVar(db, "stunTimeout")) {
		if (trTimeMS() > yGetVar(db, "stunTimeout")) {
			yAddToDatabase("stunnedUnits", db);
			if (yGetVar(db, "stunSFX") == 0) {
				ySetVar(db, "stunSFX", 0 - spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID("Shockwave stun effect")));
			} else {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar(db, "stunSFX"), true);
				trMutateSelected(kbGetProtoUnitID("Shockwave stun effect"));
			}
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

/*
Enemies have elemental resistance and weakness
*/
void elementalDamage(int p = 0, int element = 0, float dmg = 0, bool spell = false) {
	dmg = dmg * (1 + trQuestVarGet("p"+p+"element"+element+"bonus")) * (1.0 - yGetVar("enemies", "resist"+element));
	trDamageUnit(dmg);
}


void shootArrow(int p = 0, int type = 0, string from = "", string to = "", float dmg = 0, int special = -1) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet(from+"x"),0,trQuestVarGet(from+"z"),0,true);
    vectorSetAsTargetVector("target", from, to);

    int sfx = 0;

    if (type == LIGHT) {
    	float range = trQuestVarGet("p"+p+"rangeBase") * trQuestVarGet("p"+p+"spellRange");
    } else {
    	yAddToDatabase("arrowsActive", "next");
	    yAddUpdateVar("arrowsActive", "damage", dmg);
	    yAddUpdateVar("arrowsActive", "element", type);
	    yAddUpdateVar("arrowsActive", "destx", trQuestVarGet("targetx"));
	    yAddUpdateVar("arrowsActive", "destz", trQuestVarGet("targetz"));
	    yAddUpdateVar("arrowsActive", "player", p);
	    yAddUpdateVar("arrowsActive", "timeout", trTimeMS() + 5000);
	    yAddUpdateVar("arrowsActive", "special", special);
	    trUnitSelectClear();
	    trUnitSelectByQV("next", true);
	    trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));

	    switch(type)
	    {
	    	case NONE:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx1", addEffect(1*trQuestVarGet("next"), "Petosuchus Projectile"));
	    	}
	    	case FIRE:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx1", addEffect(1*trQuestVarGet("next"), "Ball of Fire"));
	    	}
	    	case ICE:
	    	{
	    		sfx = addEffect(1*trQuestVarGet("next"), "Lampades");
	    		trUnitSelectClear();
	    		trUnitSelect(""+sfx, true);
	    		trSetSelectedScale(0,0,0);
	    		yAddUpdateVar("arrowsActive", "sfx1", sfx);
	    	}
	    	case THUNDER:
	    	{
	    		sfx = addEffect(1*trQuestVarGet("next"), "Arkantos God");
	    		trUnitSelectClear();
	    		trUnitSelect(""+sfx, true);
	    		trSetSelectedScale(0,0,0);
	    		yAddUpdateVar("arrowsActive", "sfx1", sfx);
	    	}
	    	case DARK:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx1", addEffect(1*trQuestVarGet("next"), "Poison SFX"));
	    	}
	    }

	    switch(special)
	    {
	    	case NONE:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx2", addEffect(1*trQuestVarGet("next"), "Curse SFX"));
	    		yAddUpdateVar("arrowsActive", "damage", 3*dmg);
	    	}
	    	case FIRE:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx2", addEffect(1*trQuestVarGet("next"), "Hades Fire"));
	    		yAddUpdateVar("arrowsActive", "damage", 2*dmg);
	    	}
	    	case ICE:
	    	{
	    		yAddUpdateVar("arrowsActive", "sfx2", addEffect(1*trQuestVarGet("next"), "Lampades Bolt"));
	    	}
	    }

	    trUnitSelectClear();
	    trUnitSelectByQV("next", true);
	    trMutateSelected(kbGetProtoUnitID("Wadjet Spit"));
	    trUnitMoveToPoint(trQuestVarGet(to+"x"),0,trQuestVarGet(to+"z"), -1, false);
    }
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
