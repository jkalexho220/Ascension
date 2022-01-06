const int DIMENSION_X = 290;
const int DIMENSION_Z = 290;
const float PI = 3.141592;

void trVectorQuestVarSet(string VQVname = "", vector QVv = vector(-1,-1,-1)) {
	if (VQVname == "") return;
	trQuestVarSet(""+VQVname+"X", xsVectorGetX(QVv));
	trQuestVarSet(""+VQVname+"Y", xsVectorGetY(QVv));
	trQuestVarSet(""+VQVname+"Z", xsVectorGetZ(QVv));
}

vector trVectorQuestVarGet(string name = "") {
	if (name == "") { return(vector(-1,-1,-1)); }
	vector ret = xsVectorSet(trQuestVarGet(name+"X"),trQuestVarGet(name+"Y"),trQuestVarGet(name+"Z"));
	return(ret);
}

float trVectorQuestVarGetX(string VQVname = "") {
	if (VQVname == "") return(-1);
	return(trQuestVarGet(""+VQVname+"X"));
}

float trVectorQuestVarGetY(string VQVname = "") {
	if (VQVname == "") return(-1);
	return(trQuestVarGet(""+VQVname+"Y"));
}

float trVectorQuestVarGetZ(string VQVname = "") {
	if (VQVname == "") return(-1);
	return(trQuestVarGet(""+VQVname+"Z"));
}

void trVectorQuestVarEcho(string VQVname = "") {
	if (VQVname == "") return;
	trChatSend(0, ""+VQVname+": "+trVectorQuestVarGet(VQVname));
}

void trStringQuestVarSet(string name = "", string value = "") {
	int old = xsGetContextPlayer();
	xsSetContextPlayer(0);
	if (trQuestVarGet("string"+name) > 0) {
		kbArmyDestroy(1*trQuestVarGet("string"+name));
	}
	trQuestVarSet("string"+name, kbArmyCreate(value));
	xsSetContextPlayer(old);
}

string trStringQuestVarGet(string name="") {
	int old = xsGetContextPlayer();
	xsSetContextPlayer(0);
	string val = kbArmyGetName(1*trQuestVarGet("string"+name));
	xsSetContextPlayer(old);
	return(val);
}


bool playerIsPlaying(int p = 0) {
	return(kbIsPlayerHuman(p) == true && kbIsPlayerResigned(p) == false);
}


void trUnitTeleportToVector(String v = "") {
	trUnitTeleport(trVectorQuestVarGetX(""+v), trVectorQuestVarGetY(""+v), trVectorQuestVarGetZ(""+v));
}

void trUnitSelectByQV(String s = "", bool reverse = true) {
	trUnitSelectClear();
	trUnitSelect(""+1*trQuestVarGet(""+s), reverse);
}

/* 
Given a quest var that stores a unit name, store
the unit's position in the vector.
*/
void trVectorSetUnitPos(string v = "", string db = "", bool reverse = true) {
	trVectorQuestVarSet(v, kbGetBlockPosition(""+1*trQuestVarGet(db), reverse));
}

void trVectorSetUnitPosInt(string v = "", int val = 0, bool reverse = true) {
	trVectorQuestVarSet(v, kbGetBlockPosition(""+val, reverse));
}


void trUnitMoveToVector(string v = "", bool attack = false) {
	trUnitMoveToPoint(trVectorQuestVarGetX(v),0,trVectorQuestVarGetZ(v),-1,attack);
}

void trVectorScale(string db = "", float s = 1.0) {
	trQuestVarSet(db+"x", trQuestVarGet(db+"x") * s);
	trQuestVarSet(db+"z", trQuestVarGet(db+"z") * s);
}


void vectorSnapToGrid(string qv = "") {
	int x = trQuestVarGet(qv+"x") / 2;
	int z = trQuestVarGet(qv+"z") / 2;
	trQuestVarSet(qv+"x", x * 2 + 1);
	trQuestVarSet(qv+"z", z * 2 + 1);
}

int zModulo(int mod = 10, float val = 0) {
	int m = 0 + mod;
	int v = xsRound(val);
	return(v - xsFloor(val / m) * m);
}

int iModulo(int mod = 10, int val = 0) {
	return(val - val / mod * mod);
}

float fModulo(float mod = 0, float val = 0) {
	int c = 0;
	if (val > 0) {
		c = val / mod;
	} else {
		c = val / mod - 1;
	}
	return(0.0 + val - mod * c);
}


void zUnitHeading(float a = 0) {
	trSetUnitOrientation(xsVectorSet(xsSin(a),0,xsCos(a)), xsVectorSet(0,1,0), true);
}

void zInitProtoUnitStat(string r = "", int p = 0, int f = 0, float v = 0.0) {
	trQuestVarSet("p"+p+"pf"+kbGetProtoUnitID(r)+"f"+f, v);
}

void zSetProtoUnitStat(string r = "", int p = 0, int f = 0, float v = 0.0) {
	for(zsps=0; >1){}
	zsps = kbGetProtoUnitID(r);
	trModifyProtounit(r, p, f, 0.0 + v - trQuestVarGet("p"+p+"pf"+zsps+"f"+f));
	trQuestVarSet("p"+p+"pf"+zsps+"f"+f, 0.0 + v);
}

void vectorToGrid(string from = "", string to = ""){
	int x = 0 + trQuestVarGet(from+"x") / 2;
	int z = 0 + trQuestVarGet(from+"z") / 2;
	trQuestVarSet(to+"x", x);
	trQuestVarSet(to+"z", z);
}

void gridToVector(string from = "", string to = "") {
	trQuestVarSet(to+"x", trQuestVarGet(from+"x") * 2 + 1);
	trQuestVarSet(to+"z", trQuestVarGet(from+"z") * 2 + 1);
}

float gridDistanceSquared(string from = "", string to = "") {
	float xdiff = trQuestVarGet(to+"x") - trQuestVarGet(from+"x");
	float zdiff = trQuestVarGet(to+"z") - trQuestVarGet(from+"z");
	return(xdiff * xdiff + zdiff * zdiff);
}

void zSquareVar(string qv = "") {
	trQuestVarSet(qv, xsPow(trQuestVarGet(qv), 2));
}

float zDistanceBetweenVectorsSquared(string start = "", string end = "") {
	float xdiff = trQuestVarGet(end + "X") - trQuestVarGet(start + "X");
	float zdiff = trQuestVarGet(end + "Z") - trQuestVarGet(start + "Z");
	float dist = xdiff * xdiff + zdiff * zdiff;
	return(dist);
}



float zDistanceBetweenVectors(string start = "", string end = "") {
	float xdiff = trQuestVarGet(end + "X") - trQuestVarGet(start + "X");
	float zdiff = trQuestVarGet(end + "Z") - trQuestVarGet(start + "Z");
	float dist = xsSqrt(xdiff * xdiff + zdiff * zdiff);
	return(dist);
}

float zDistanceBetweenVectors3d(string start = "", string end = "") {
	float xdiff = trQuestVarGet(end + "X") - trQuestVarGet(start + "X");
	float ydiff = trQuestVarGet(end + "Y") - trQuestVarGet(start + "Y");
	float zdiff = trQuestVarGet(end + "Z") - trQuestVarGet(start + "Z");
	float dist = xsSqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
	return(dist);
}

float zDistanceToVectorSquared(string qv = "", string v = "") {
	trVectorQuestVarSet("abcd", kbGetBlockPosition(""+1*trQuestVarGet(qv), true));
	return(zDistanceBetweenVectorsSquared("abcd", v));
}

/* For use in a ySearch */
float zDistanceToVector(string qv = "", string v = "") {
	trVectorQuestVarSet("abcd", kbGetBlockPosition(""+1*trQuestVarGet(qv), true));
	return(zDistanceBetweenVectors("abcd", v));
}

void trVectorSetFromAngle(string qv = "", float angle = 0) {
	trVectorQuestVarSet(qv,xsVectorSet(xsSin(angle), 0, xsCos(angle)));
}

float angleBetweenVectors(string from = "", string to = "") {
	float a = trQuestVarGet(to+"X")-trQuestVarGet(from+"X");
	a = a / (trQuestVarGet(to+"Z")-trQuestVarGet(from+"Z"));
	a = xsAtan(a);
	if (trVectorQuestVarGetZ(from) > trVectorQuestVarGetZ(to)) {
	    if (trVectorQuestVarGetX(from) > trVectorQuestVarGetX(to)) {
			a = a - PI;
	    } else {
			a = a + PI;
	    }
  	}
  	return(a);
}

vector zGetUnitVector(string start = "", string end = "", float mod = 1.0) {
	float xdiff = trQuestVarGet(end + "X") - trQuestVarGet(start + "X");
	float zdiff = trQuestVarGet(end + "Z") - trQuestVarGet(start + "Z");
	float dist = xsSqrt(xdiff * xdiff + zdiff * zdiff);
	vector ret = xsVectorSet(xdiff / dist * mod, 0, zdiff / dist * mod);
	return(ret);
}

vector zGetUnitVector3d(string start = "", string end = "", float mod = 1.0) {
	float xdiff = trQuestVarGet(end + "X") - trQuestVarGet(start + "X");
	float ydiff = trQuestVarGet(end + "Y") - trQuestVarGet(start + "Y");
	float zdiff = trQuestVarGet(end + "Z") - trQuestVarGet(start + "Z");
	float dist = xsSqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
	vector ret = xsVectorSet(xdiff / dist * mod, ydiff / dist * mod, zdiff / dist * mod);
	return(ret);
}

vector crossProduct(string a = "", string b = "") {
	float x = trQuestVarGet(a + "y") * trQuestVarGet(b + "z") - trQuestVarGet(a + "z") * trQuestVarGet(b + "y");
	float y = trQuestVarGet(a + "z") * trQuestVarGet(b + "x") - trQuestVarGet(a + "x") * trQuestVarGet(b + "z");
	float z = trQuestVarGet(a + "x") * trQuestVarGet(b + "y") - trQuestVarGet(a + "y") * trQuestVarGet(b + "x");
	vector ret = xsVectorSet(x, y, z);
	return(ret);
}

float dotProduct(string a = "", string b = "") {
	return(trQuestVarGet(a+"x") * trQuestVarGet(b+"x") + trQuestVarGet(a+"z") * trQuestVarGet(b+"z"));
}

bool terrainIsType(string qv = "", int type = 0, int subtype = 0) {
	bool isType = trGetTerrainType(trQuestVarGet(qv+"x"),trQuestVarGet(qv+"z")) == type;
	isType = trGetTerrainSubType(trQuestVarGet(qv+"x"),trQuestVarGet(qv+"z")) == subtype;
	return(isType);
}

/* 
A shitty binary search algorithm to approximate the intersection of a line with 
the circle specified by the center vector and radius. Behavior is undefined if start
vector is outside the circle.  
Did this to avoid using trig as much as possible because trig is expensive.
*/
vector intersectionWithCircle(string start = "", string end = "", string center = "", float radius = 0) {
	trVectorQuestVarSet("iDir", zGetUnitVector(start, end));
	float x = 0;
	float z = 0;
	float dist = 0;
	float len = radius * 2;
	float lenmod = radius;
	for(i=8; >0) {
		x = trQuestVarGet(start+"x") + trQuestVarGet("iDirx") * len;
		z = trQuestVarGet(start+"z") + trQuestVarGet("iDirz") * len;
		dist = xsPow(x - trQuestVarGet(center+"x"), 2) + xsPow(z - trQuestVarGet(center+"z"), 2);
		if (dist > radius * radius) {
			len = len - lenmod;
		} else {
			len = len + lenmod;
		}
		lenmod = lenmod * 0.5;
	}
	return(xsVectorSet(x,0,z));
}

/* initializes a modular counter. */
void modularCounterInit(string name = "", int size = 0) {
	trQuestVarSet("counter" + name + "size", size);
	trQuestVarSet("counter" + name + "pointer", 1);
	trQuestVarSet(name, 1);
}

/* Progresses the modular counter by 1 and returns the value */
int modularCounterNext(string name = "") {
	trQuestVarSet("counter" + name + "pointer", 1 + trQuestVarGet("counter" + name + "pointer"));
	if (trQuestVarGet("counter" + name + "pointer") > trQuestVarGet("counter" + name + "size")) {
		trQuestVarSet("counter" + name + "pointer", 1);
	}
	trQuestVarSet(name, trQuestVarGet("counter"+name+"pointer"));
	return(0 + trQuestVarGet("counter" + name + "pointer"));
}

/* Peeks at the next value of the modular counter */
int peekModularCounterNext(string name = "") {
	trQuestVarSet("counter" + name + "fake", 1 + trQuestVarGet("counter" + name + "pointer"));
	if (trQuestVarGet("counter" + name + "fake") >= trQuestVarGet("counter" + name + "size")) {
		trQuestVarSet("counter" + name + "fake", 1);
	}
	return(0 + trQuestVarGet("counter" + name + "fake"));
}


float yGetVarAtIndex(string db = "", string var = "", int index = 0) {
	return(trQuestVarGet("xdata"+db+index+var));
}

float yGetVar(string db = "", string var = "") {
	int index = trQuestVarGet("xdata"+db+"pointer");
	return(trQuestVarGet("xdata"+db+index+var));
}

string yGetVarName(string db = "", string var = "") {
	int index = trQuestVarGet("xdata"+db+"pointer");
	return("xdata"+db+index+var);
}

float ySetVarAtIndex(string db = "", string var = "", float val = 0, int index = 0) {
	trQuestVarSet("xdata"+db+index+var, val);
}

float ySetVar(string db = "", string var = "", float val = 0) {
	int index = trQuestVarGet("xdata"+db+"pointer");
	ySetVarAtIndex(db, var, val, index);
}

int yDatabaseNext(string db = "", bool select = false, bool reverse = false) {
	int index = yGetVar(db, "xNextBlock");
	if (reverse) {
		index = yGetVar(db, "xPrevBlock");
	}
	trQuestVarSet("xdata"+db+"pointer", index);
	int u = trQuestVarGet("xdata"+db+"index"+index);
	trQuestVarSet(db, u);
	if (select) {
		trUnitSelectClear();
		trUnitSelect(""+u, true);
		return(kbGetBlockID(""+u, true));
	} else {
		return(trQuestVarGet(db));
	}
}

void yRemoveFromDatabase(string db = "") {
	int index = trQuestVarGet("xdata"+db+"pointer");
	/* connect next with prev */
	ySetVarAtIndex(db, "xNextBlock", yGetVar(db, "xNextBlock"), 1*yGetVar(db, "xPrevBlock"));
	ySetVarAtIndex(db, "xPrevBlock", yGetVar(db, "xPrevBlock"), 1*yGetVar(db, "xNextBlock"));
	
	ySetVar(db, "xNextBlock", trQuestVarGet("xdata"+db+"nextFree"));
	trQuestVarSet("xdata"+db+"nextFree", index);

	trQuestVarSet("xdata"+db+"pointer", yGetVar(db, "xPrevBlock"));
	trQuestVarSet("xdata"+db+"count", trQuestVarGet("xdata"+db+"count") - 1);
}

void yRemoveUpdateVar(string db = "", string attr = "") {
	int index = trQuestVarGet("xdata"+db+"nextFree");
	ySetVarAtIndex(db, attr, 0, index);
}

int yAddToDatabase(string db = "", string val = "") {
	int next = trQuestVarGet("xdata"+db+"nextFree");
	if (next == 0) {
		next = 1 + trQuestVarGet("xdata"+db+"total");
		trQuestVarSet("xdata"+db+"total", next);
	} else {
		trQuestVarSet("xdata"+db+"nextFree", yGetVarAtIndex(db, "xNextBlock", next));
	}
	trQuestVarSet("xdata"+db+"index"+next, trQuestVarGet(val));
	if (trQuestVarGet("xdata"+db+"count") == 0) {
		ySetVarAtIndex(db, "xNextBlock", next, next);
		ySetVarAtIndex(db, "xPrevBlock", next, next);
		trQuestVarSet("xdata"+db+"pointer", next);
	} else {
		int index = trQuestVarGet("xdata"+db+"pointer");
		ySetVarAtIndex(db, "xNextBlock", yGetVarAtIndex(db, "xNextBlock", index), next);
		ySetVarAtIndex(db, "xPrevBlock", index, next);
		ySetVarAtIndex(db, "xNextBlock", next, index);
		ySetVarAtIndex(db, "xPrevBlock", next, 1*yGetVarAtIndex(db, "xNextBlock", next));
	}
	trQuestVarSet("xdata"+db+"newest", next);
	trQuestVarSet("xdata"+db+"count", trQuestVarGet("xdata"+db+"count") + 1);
	return(next);
}

int yGetNewestPointer(string db = "") {
	return(1*trQuestVarGet("xdata"+db+"newest"));
}

void yAddUpdateVar(string db = "", string var = "", float val = 0) {
	ySetVarAtIndex(db, var, val, 1*trQuestVarGet("xdata"+db+"newest"));
}

int yGetDatabaseCount(string db = "") {
	return(1*trQuestVarGet("xdata"+db+"count"));
}

int yGetUnitAtIndex(string db = "", int index = 0) {
	return(trQuestVarGet("xdata"+db+"index"+index));
}

void ySetUnitAtIndex(string db = "", int index = 0, int value = 0) {
	trQuestVarSet("xdata"+db+"index"+index, value);
}

void ySetUnit(string db = "", int value = 0) {
	int index = trQuestVarGet("xdata"+db+"pointer");
	ySetUnitAtIndex(db, index, value);
}

int yGetPointer(string db = "") {
	return(trQuestVarGet("xdata"+db+"pointer"));
}

void ySetPointer(string db = "", int index = 0) {
	trQuestVarSet("xdata"+db+"pointer", index);
	trQuestVarSet(db, trQuestVarGet("xdata"+db+"index"+index));
}

void yClearDatabase(string db = "") {
	int index = trQuestVarGet("xdata"+db+"nextFree");
	trQuestVarSet("xdata"+db+"nextFree", yGetVar(db, "xNextBlock"));
	ySetVar(db, "xNextBlock", index);
	trQuestVarSet("xdata"+db+"count", 0);
	trQuestVarSet("xdata"+db+"pointer", 0);
}

void yVarToVector(string db = "", string v = "") {
	trQuestVarSet(v+"x", yGetVar(db, v + "x"));
	trQuestVarSet(v+"z", yGetVar(db, v + "z"));
}

void ySetVarFromVector(string db = "", string attr = "", string v = "") {
	ySetVar(db, attr+"x", trQuestVarGet(v+"x"));
	ySetVar(db, attr+"z", trQuestVarGet(v+"z"));
}

/* 
Starting from NextUnitScenarioNameNumber and going backwards until the quest var 'qv',
looks for the specified protounit. If none found, returns -1. Otherwise, returns the
unit name.
*/
int yFindLatestReverse(string qv = "", string proto = "", int p = 0) {
	int id = kbGetProtoUnitID(proto);
	trUnitSelectClear();
	for(x=trGetNextUnitScenarioNameNumber(); >trQuestVarGet(qv)) {
		int i = kbGetBlockID(""+x, true);
		if (kbGetUnitBaseTypeID(i) == id) {
			trUnitSelectClear();
			trUnitSelectByID(i);
			if (trUnitIsOwnedBy(p)) {
				trQuestVarSet(qv, x);
				return(i);
			}
		}
	}
	return(-1);
}

/*
Starting from quest var 'qv' and going up until NextUnitScenarioNameNumber,
looks for the specified protounit. If none found, returns -1. Otherwise, returns the
unit name.
*/
int yFindLatest(string qv = "", string proto = "", int p = 0) {
	int id = kbGetProtoUnitID(proto);
	trUnitSelectClear();
	trQuestVarSet("next",trGetNextUnitScenarioNameNumber() - 1);
	while(trQuestVarGet("next") > trQuestVarGet(qv)) {
		trQuestVarSet(qv, 1 + trQuestVarGet(qv));
		int i = kbGetBlockID(""+1*trQuestVarGet(qv), true);
		if (kbGetUnitBaseTypeID(i) == id) {
			trUnitSelectClear();
			trUnitSelectByID(i);
			if (trUnitIsOwnedBy(p)) {
				return(i);
			}
		}
	}
	return(-1);
}
