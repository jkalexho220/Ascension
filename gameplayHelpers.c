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
	} else if (terrainIsType("loc", TERRAIN_CHASM, TERRAIN_SUB_CHASM)) {
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		collide = true;
	}
	return(collide);
}

void removeArrow() {
	yRemoveFromDatabase("arrowsActive");
	yRemoveUpdateVar("arrowsActive", "destx");
	yRemoveUpdateVar("arrowsActive", "destz");
	yRemoveUpdateVar("arrowsActive", "timeout");
	yRemoveUpdateVar("arrowsActive", "type");
	yRemoveUpdateVar("arrowsActive", "damage");
	yRemoveUpdateVar("arrowsActive", "player");
}

bool checkArrowDie() {
	int arrow = trQuestVarGet("arrowsActive");
	trQuestVarSet("endx", yGetVar("arrowsActive", "destx"));
	trQuestVarSet("endz", yGetVar("arrowsActive", "destz"));
	if ((collideWithTerrain(arrow)) ||
		zDistanceToVectorSquared("arrowsActive", "end") < 4 ||
		trTimeMS() > yGetVar("arrowsActive", "timeout")) {
		trUnitSelectClear();
		trUnitSelect(""+arrow);
		trUnitChangeProtoUnit("Dust Small");
		return(true);
	} else {
		return(false);
	}
}

void removeEnemy() {
	if (yGetVar("enemies", "bounty") > 0) {
		trChatSend(0, "Collected " + 1*yGetVar("enemies", "bounty") + " <icon=(24)(icons/icon resource gold)>");
		trPlayerGrantResources(1, "Gold", 1*yGetVar("enemies", "bounty"));
	}
	yRemoveFromDatabase("enemies");
	yRemoveUpdateVar("enemies", "bounty");
	yRemoveUpdateVar("enemies", "stunTimeout");
	yRemoveUpdateVar("enemies", "next");
}

void shootArrow(int p = 0, int type = 0, string from = "", string to = "", float dmg = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
    vectorSetAsTargetVector("target", from, to);

    yAddToDatabase("arrowsActive", "next");
    yAddUpdateVar("arrowsActive", "damage", dmg);
    yAddUpdateVar("arrowsActive", "type", type);
    yAddUpdateVar("arrowsActive", "destx", trQuestVarGet("targetx"));
    yAddUpdateVar("arrowsActive", "destz", trQuestVarGet("targetz"));
    yAddUpdateVar("arrowsActive", "player", p);
    yAddUpdateVar("arrowsActive", "timeout", trTimeMS() + 5000);
    trUnitSelectClear();
    trUnitSelect(""+1*trQuestVarGet("next"), true);
    switch(type)
    {
    	case NONE:
    	{

    	}
    }
}
