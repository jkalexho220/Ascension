const int DIMENSION_X = 290;
const int DIMENSION_Z = 290;
const float PI = 3.141592;

void trVectorQuestVarSet(string VQVname = "", vector QVv = vector(-1,-1,-1)) {
	if (VQVname == "") return;
	if (trQuestVarGet(VQVname) == 0) {
		trQuestVarSet(VQVname, mNewVector(QVv));
	} else {
		mSetVector(1*trQuestVarGet(VQVname),QVv);
	}
}

vector trVectorQuestVarGet(string name = "") {
if (name == "") { return(vector(-1,-1,-1)); }
	
	vector ret = mGetVector(1*trQuestVarGet(name));
	return(ret);
}

float trVectorQuestVarGetX(string VQVname = "") {
	if (VQVname == "") return(-1);
	float val = xsVectorGetX(mGetVector(1*trQuestVarGet(VQVname)));
	return(val);
}

float trVectorQuestVarGetY(string VQVname = "") {
	if (VQVname == "") return(-1);
	float val = xsVectorGetY(mGetVector(1*trQuestVarGet(VQVname)));
	return(val);
}

float trVectorQuestVarGetZ(string VQVname = "") {
	if (VQVname == "") return(-1);
	float val = xsVectorGetZ(mGetVector(1*trQuestVarGet(VQVname)));
	return(val);
}

void trVectorQuestVarEcho(string VQVname = "") {
	if (VQVname == "") return;
	trChatSend(0, ""+VQVname+": "+trVectorQuestVarGet(VQVname));
}


void trStringQuestVarSet(string name = "", string value = "") {
	if (trQuestVarGet("string"+name) > 0) {
		mSetString(1*trQuestVarGet("string"+name), value);
	} else {
		trQuestVarSet("string"+name, mNewString(value));
	}
}

string trStringQuestVarGet(string name="") {
	string val = mGetString(1*trQuestVarGet("string"+name));
	return(val);
}


bool playerIsPlaying(int p = 0) {
	return(kbIsPlayerHuman(p) == true && kbIsPlayerResigned(p) == false);
}


void trUnitTeleportToVector(string v = "") {
	trUnitTeleport(trVectorQuestVarGetX(v), trVectorQuestVarGetY(v), trVectorQuestVarGetZ(v));
}

void trUnitSelectByQV(string s = "", bool reverse = true) {
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
	trVectorQuestVarSet(db, trVectorQuestVarGet(db) * s);
}

vector vectorSnapToGrid(vector v = vector(0,0,0)) {
	int x = xsVectorGetX(v) / 2;
	int z = xsVectorGetZ(v) / 2;
	return(xsVectorSet(x * 2 + 1,xsVectorGetY(v),z * 2 + 1));
}

void zVectorSnapToGrid(string qv = "") {
	trVectorQuestVarSet(qv, vectorSnapToGrid(trVectorQuestVarGet(qv)));
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

bool getBit(int bit = 0, int val = 0) {
	val = val / xsPow(2, bit);
	return((iModulo(2, val) == 1));
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

vector vectorToGrid(vector v = vector(0,0,0)) {
	return(xsVectorSet(0 + xsVectorGetX(v) / 2,xsVectorGetY(v),0 + xsVectorGetZ(v) / 2));
}

void zVectorToGrid(string from = "", string to = ""){
	trVectorQuestVarSet(to, vectorToGrid(trVectorQuestVarGet(from)));
}

vector gridToVector(vector g = vector(0,0,0)) {
	return(xsVectorSet(xsVectorGetX(g) * 2 + 1,xsVectorGetY(g),xsVectorGetZ(g) * 2 + 1));
}

void zGridToVector(string from = "", string to = "") {
	trVectorQuestVarSet(to, gridToVector(trVectorQuestVarGet(from)));
}

void zSquareVar(string qv = "") {
	trQuestVarSet(qv, xsPow(trQuestVarGet(qv), 2));
}

float manhattanDistance(vector start = vector(0,0,0), vector end = vector(0,0,0)) {
	float xdiff = xsAbs(xsVectorGetX(start) - xsVectorGetX(end));
	float zdiff = xsAbs(xsVectorGetZ(start) - xsVectorGetZ(end));
	return(xdiff + zdiff);
}

float distanceBetweenVectors(vector start = vector(0,0,0), vector end = vector(0,0,0), bool squared = true) {
	float xDiff = xsVectorGetX(end) - xsVectorGetX(start);
	float zDiff = xsVectorGetZ(end) - xsVectorGetZ(start);
	float dist = xDiff * xDiff + zDiff * zDiff;
	if (squared == false) {
		dist = xsSqrt(dist);
	}
	return(dist);
}

float zDistanceBetweenVectorsSquared(string start = "", string end = "") {
	return(distanceBetweenVectors(trVectorQuestVarGet(start),trVectorQuestVarGet(end)));
}

bool vectorInRectangle(vector pos = vector(0,0,0), vector bot = vector(0,0,0), vector top = vector(0,0,0)) {
	if (xsVectorGetX(pos) < xsVectorGetX(bot)) {
		return(false);
	}
	if (xsVectorGetX(pos) > xsVectorGetX(top)) {
		return(false);
	}
	if (xsVectorGetZ(pos) < xsVectorGetZ(bot)) {
		return(false);
	}
	if (xsVectorGetZ(pos) > xsVectorGetZ(top)) {
		return(false);
	}
	return(true);
}

bool zVectorInRectangle(string pos = "", string bot = "", string top = "") {
	vector tempPos = mGetVector(1*trQuestVarGet(pos));
	vector tempBot = mGetVector(1*trQuestVarGet(bot));
	vector tempTop = mGetVector(1*trQuestVarGet(top));
	return(vectorInRectangle(tempPos,tempBot,tempTop));
}

vector rotationMatrix(vector v = vector(0,0,0), float cosT = 0, float sinT = 0) {
	float x = xsVectorGetX(v);
	float z = xsVectorGetZ(v);
	vector ret = xsVectorSet(x * cosT - z * sinT, 0, x * sinT + z * cosT);
	return(ret);
}

float zDistanceBetweenVectors(string start = "", string end = "") {
	return(distanceBetweenVectors(trVectorQuestVarGet(start),trVectorQuestVarGet(end),false));
}

float distanceBetweenVectors3d(vector start = vector(0,0,0), vector end = vector(0,0,0), bool squared = true) {
	float xdiff = xsVectorGetX(start) - xsVectorGetX(end);
	float ydiff = xsVectorGetY(start) - xsVectorGetY(end);
	float zdiff = xsVectorGetZ(start) - xsVectorGetZ(end);
	float dist = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
	if (squared == false) {
		dist = xsSqrt(dist);
	}
	return(dist);
}

float zDistanceBetweenVectors3d(string start = "", string end = "") {
	return(distanceBetweenVectors3d(trVectorQuestVarGet(start),trVectorQuestVarGet(end),false));
}

float unitDistanceToVector(int name = 0, vector v = vector(0,0,0), bool squared = true) {
	vector temp = kbGetBlockPosition(""+name,true);
	return(distanceBetweenVectors(temp,v,squared));
}

float zDistanceToVectorSquared(string qv = "", string v = "") {
	return(unitDistanceToVector(1*trQuestVarGet(qv),trVectorQuestVarGet(v)));
}

/* For use in a ySearch */
float zDistanceToVector(string qv = "", string v = "") {
	return(unitDistanceToVector(1*trQuestVarGet(qv),trVectorQuestVarGet(v),false));
}

vector vectorSetFromAngle(float angle = 0) {
	return(xsVectorSet(xsSin(angle), 0, xsCos(angle)));
}

void trVectorSetFromAngle(string qv = "", float angle = 0) {
	trVectorQuestVarSet(qv,xsVectorSet(xsSin(angle), 0, xsCos(angle)));
}

float angleBetweenVectors(vector from = vector(0,0,0), vector to = vector(0,0,0)) {
	float a = xsVectorGetX(to) - xsVectorGetX(from);
	if (xsVectorGetZ(to) - xsVectorGetZ(from) == 0) {
		a = 0;
	} else {
		a = a / (xsVectorGetZ(to) - xsVectorGetZ(from));
	}
	a = xsAtan(a);
	if (xsVectorGetZ(from) > xsVectorGetZ(to)) {
		if (xsVectorGetX(from) > xsVectorGetX(to)) {
			a = a - PI;
		} else {
			a = a + PI;
		}
	}
	return(a);
}

float zAngleBetweenVectors(string from = "", string to = "") {
	return(angleBetweenVectors(trVectorQuestVarGet(from),trVectorQuestVarGet(to)));
}

float angleOfVector(vector dir = vector(0,0,0)) {
	float a = xsVectorGetX(dir) / xsVectorGetZ(dir);
	a = xsAtan(a);
	if (0.0 > xsVectorGetZ(dir)) {
		if (0.0 > xsVectorGetX(dir)) {
			a = a - PI;
		} else {
			a = a + PI;
		}
	}
	return(a);
}

float zAngleOfVector(string v = "") {
	return(angleOfVector(trVectorQuestVarGet(v)));
}

vector getUnitVector(vector start = vector(0,0,0), vector end = vector(0,0,0), float mod = 1.0) {
	float xdiff = xsVectorGetX(end) - xsVectorGetX(start);
	float zdiff = xsVectorGetZ(end) - xsVectorGetZ(start);
	float dist = xsSqrt(xdiff * xdiff + zdiff * zdiff);
	vector ret = vector(1,0,0);
	if (dist > 0) {
		ret = xsVectorSet(xdiff / dist * mod, 0, zdiff / dist * mod);
	}
	return(ret);
}

vector zGetUnitVector(string start = "", string end = "", float mod = 1.0) {
	return(getUnitVector(trVectorQuestVarGet(start),trVectorQuestVarGet(end),mod));
}

vector getUnitVector3d(vector start = vector(0,0,0), vector end = vector(0,0,0), float mod = 1.0) {
	float xdiff = xsVectorGetX(end) - xsVectorGetX(start);
	float ydiff = xsVectorGetY(end) - xsVectorGetY(start);
	float zdiff = xsVectorGetZ(end) - xsVectorGetZ(start);
	float dist = xsSqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
	vector ret = vector(0,1,0);
	if (dist > 0) {
		ret = xsVectorSet(xdiff / dist * mod, ydiff / dist * mod, zdiff / dist * mod);
	}
	return(ret);
}

vector zGetUnitVector3d(string start = "", string end = "", float mod = 1.0) {
	return(getUnitVector3d(trVectorQuestVarGet(start),trVectorQuestVarGet(end),mod));
}

vector crossProduct(vector a = vector(0,0,0), vector b = vector(0,0,0)) {
	float x = xsVectorGetY(a) * xsVectorGetZ(b) - xsVectorGetZ(a) * xsVectorGetY(b);
	float y = xsVectorGetZ(a) * xsVectorGetX(b) - xsVectorGetX(a) * xsVectorGetZ(b);
	float z = xsVectorGetX(a) * xsVectorGetY(b) - xsVectorGetY(a) * xsVectorGetX(b);
	vector ret = xsVectorSet(x, y, z);
	return(ret);
}

float dotProduct(vector a = vector(0,0,0), vector b = vector(0,0,0)) {
	return(xsVectorGetX(a) * xsVectorGetX(b) + xsVectorGetZ(a) * xsVectorGetZ(b));
}

bool terrainIsType(vector v = vector(0,0,0), int type = 0, int subtype = 0) {
	bool isType = trGetTerrainType(xsVectorGetX(v),xsVectorGetZ(v)) == type;
	isType = isType && (trGetTerrainSubType(xsVectorGetX(v),xsVectorGetZ(v)) == subtype);
	return(isType);
}

bool zTerrainIsType(string qv = "", int type = 0, int subtype = 0) {
	return(terrainIsType(trVectorQuestVarGet(qv),type,subtype));
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

void RunFunction(string function=""){
	subModeEnter("Simulation", "Editor");
	uiMessageBox("moo",function);
	uiCycleCurrentActivate();
	subModeLeave("Simulation", "Editor");
	modeEnter("pregame");
	modeEnter("Simulation");
}