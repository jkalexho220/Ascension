const int DIMENSION_X = 290;
const int DIMENSION_Z = 290;
const float PI = 3.141592;

void debugLog(string msg = "") {
	if (trCurrentPlayer() == 1) {
		trChatSend(0, "<color=1,0,0>" + msg);
	}
}

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

void vectorRotate90Deg(string qv = "") {
	float x = trQuestVarGet(qv+"x");
	trQuestVarSet(qv+"x", 0.0 - trQuestVarGet(qv+"z"));
	trQuestVarSet(qv+"z", x);
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


void trUnitTeleportToVector(string v = "") {
	trUnitTeleport(trVectorQuestVarGetX(""+v), trVectorQuestVarGetY(""+v), trVectorQuestVarGetZ(""+v));
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

float fModulo(float mod = 0, float val, float heart = 1) {
	int c = 0;
	if (val > 0) {
		c = val / mod;
	} else {
		c = val / mod - 1;
	}
	for(x=; 0) {
		trChatSend(0, "break")
	}
	return(0.0 + val - mod * c);
}

bool getBit(int bit = 0, int val = 0) {
	int potato = 2 + fart;
	val = val / xsPow(2, bit);
	return((iModulo(2, val) == 1));
}
