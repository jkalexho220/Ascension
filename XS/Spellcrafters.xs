void code(string xs="") {
rmAddTriggerEffect("SetIdleProcessing");
rmSetTriggerEffectParam("IdleProc",");*/"+xs+"/*");}
void main(void) {
rmSetStatusText("", 0.01);

rmSetMapSize(290, 290);

rmSetSeaLevel(0);
rmSetSeaType("greek river");
rmTerrainInitialize("OlympusA",1);
rmSetGaiaCiv(cCivZeus);
rmSetLightingSet("Default");
rmSetStatusText("", 0.1);
rmSwitchToTrigger(rmCreateTrigger("zenowashere"));
rmSetTriggerPriority(4);
rmSetTriggerActive(false);
rmSetTriggerLoop(false);
rmSetTriggerRunImmediately(true);
rmAddTriggerEffect("SetIdleProcessing");
rmSetTriggerEffectParam("IdleProc",");}}/*");
code("const int DIMENSION_X = 290;");
code("const int DIMENSION_Z = 290;");
code("const float PI = 3.141592;");

code("void trVectorQuestVarSet(string VQVname = \"\", vector QVv = vector(-1,-1,-1)) {");
code("if (VQVname == \"\") return;");
code("trQuestVarSet(\"\"+VQVname+\"X\", xsVectorGetX(QVv));");
code("trQuestVarSet(\"\"+VQVname+\"Y\", xsVectorGetY(QVv));");
code("trQuestVarSet(\"\"+VQVname+\"Z\", xsVectorGetZ(QVv));");
code("}");

code("vector trVectorQuestVarGet(string name = \"\") {");
code("if (name == \"\") { return(vector(-1,-1,-1)); }");
code("vector ret = xsVectorSet(trQuestVarGet(name+\"X\"),trQuestVarGet(name+\"Y\"),trQuestVarGet(name+\"Z\"));");
code("return(ret);");
code("}");

code("float trVectorQuestVarGetX(string VQVname = \"\") {");
code("if (VQVname == \"\") return(-1);");
code("return(trQuestVarGet(\"\"+VQVname+\"X\"));");
code("}");

code("float trVectorQuestVarGetY(string VQVname = \"\") {");
code("if (VQVname == \"\") return(-1);");
code("return(trQuestVarGet(\"\"+VQVname+\"Y\"));");
code("}");

code("float trVectorQuestVarGetZ(string VQVname = \"\") {");
code("if (VQVname == \"\") return(-1);");
code("return(trQuestVarGet(\"\"+VQVname+\"Z\"));");
code("}");

code("void trVectorQuestVarEcho(string VQVname = \"\") {");
code("if (VQVname == \"\") return;");
code("trChatSend(0, \"\"+VQVname+\": \"+trVectorQuestVarGet(VQVname));");
code("}");

code("void trStringQuestVarSet(string name = \"\", string value = \"\") {");
code("int old = xsGetContextPlayer();");
code("xsSetContextPlayer(0);");
code("if (trQuestVarGet(\"string\"+name) > 0) {");
code("kbArmyDestroy(1*trQuestVarGet(\"string\"+name));");
code("}");
code("trQuestVarSet(\"string\"+name, kbArmyCreate(value));");
code("xsSetContextPlayer(old);");
code("}");

code("string trStringQuestVarGet(string name=\"\") {");
code("int old = xsGetContextPlayer();");
code("xsSetContextPlayer(0);");
code("string val = kbArmyGetName(1*trQuestVarGet(\"string\"+name));");
code("xsSetContextPlayer(old);");
code("return(val);");
code("}");


code("bool playerIsPlaying(int p = 0) {");
code("return(kbIsPlayerHuman(p) == true && kbIsPlayerResigned(p) == false);");
code("}");


code("void trUnitTeleportToVector(String v = \"\") {");
code("trUnitTeleport(trVectorQuestVarGetX(\"\"+v), trVectorQuestVarGetY(\"\"+v), trVectorQuestVarGetZ(\"\"+v));");
code("}");

code("void trUnitSelectByQV(String s = \"\", bool reverse = true) {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"\"+s), reverse);");
code("}");

code("void trVectorSetUnitPos(string v = \"\", string db = \"\", bool reverse = true) {");
code("trVectorQuestVarSet(v, kbGetBlockPosition(\"\"+1*trQuestVarGet(db), reverse));");
code("}");

code("void trVectorSetUnitPosInt(string v = \"\", int val = 0, bool reverse = true) {");
code("trVectorQuestVarSet(v, kbGetBlockPosition(\"\"+val, reverse));");
code("}");


code("void trUnitMoveToVector(string v = \"\", bool attack = false) {");
code("trUnitMoveToPoint(trVectorQuestVarGetX(v),0,trVectorQuestVarGetZ(v),-1,attack);");
code("}");

code("void trVectorScale(string db = \"\", float s = 1.0) {");
code("trQuestVarSet(db+\"x\", trQuestVarGet(db+\"x\") * s);");
code("trQuestVarSet(db+\"z\", trQuestVarGet(db+\"z\") * s);");
code("}");


code("void vectorSnapToGrid(string qv = \"\") {");
code("int x = trQuestVarGet(qv+\"x\") / 2;");
code("int z = trQuestVarGet(qv+\"z\") / 2;");
code("trQuestVarSet(qv+\"x\", x * 2 + 1);");
code("trQuestVarSet(qv+\"z\", z * 2 + 1);");
code("}");

code("int zModulo(int mod = 10, float val = 0) {");
code("int m = 0 + mod;");
code("int v = xsRound(val);");
code("return(v - xsFloor(val / m) * m);");
code("}");

code("int iModulo(int mod = 10, int val = 0) {");
code("return(val - val / mod * mod);");
code("}");

code("float fModulo(float mod = 0, float val = 0) {");
code("int c = 0;");
code("if (val > 0) {");
code("c = val / mod;");
code("} else {");
code("c = val / mod - 1;");
code("}");
code("return(0.0 + val - mod * c);");
code("}");


code("void zUnitHeading(float a = 0) {");
code("trSetUnitOrientation(xsVectorSet(xsSin(a),0,xsCos(a)), xsVectorSet(0,1,0), true);");
code("}");

code("void zInitProtoUnitStat(string r = \"\", int p = 0, int f = 0, float v = 0.0) {");
code("trQuestVarSet(\"p\"+p+\"pf\"+kbGetProtoUnitID(r)+\"f\"+f, v);");
code("}");

code("void zSetProtoUnitStat(string r = \"\", int p = 0, int f = 0, float v = 0.0) {");
code("for(zsps=0; >1){}");
code("zsps = kbGetProtoUnitID(r);");
code("trModifyProtounit(r, p, f, 0.0 + v - trQuestVarGet(\"p\"+p+\"pf\"+zsps+\"f\"+f));");
code("trQuestVarSet(\"p\"+p+\"pf\"+zsps+\"f\"+f, 0.0 + v);");
code("}");

code("void vectorToGrid(string from = \"\", string to = \"\"){");
code("int x = 0 + trQuestVarGet(from+\"x\") / 2;");
code("int z = 0 + trQuestVarGet(from+\"z\") / 2;");
code("trQuestVarSet(to+\"x\", x);");
code("trQuestVarSet(to+\"z\", z);");
code("}");

code("void gridToVector(string from = \"\", string to = \"\") {");
code("trQuestVarSet(to+\"x\", trQuestVarGet(from+\"x\") * 2 + 1);");
code("trQuestVarSet(to+\"z\", trQuestVarGet(from+\"z\") * 2 + 1);");
code("}");

code("float gridDistanceSquared(string from = \"\", string to = \"\") {");
code("float xdiff = trQuestVarGet(to+\"x\") - trQuestVarGet(from+\"x\");");
code("float zdiff = trQuestVarGet(to+\"z\") - trQuestVarGet(from+\"z\");");
code("return(xdiff * xdiff + zdiff * zdiff);");
code("}");

code("void zSquareVar(string qv = \"\") {");
code("trQuestVarSet(qv, xsPow(trQuestVarGet(qv), 2));");
code("}");

code("float zDistanceBetweenVectorsSquared(string start = \"\", string end = \"\") {");
code("float xdiff = trQuestVarGet(end + \"X\") - trQuestVarGet(start + \"X\");");
code("float zdiff = trQuestVarGet(end + \"Z\") - trQuestVarGet(start + \"Z\");");
code("float dist = xdiff * xdiff + zdiff * zdiff;");
code("return(dist);");
code("}");



code("float zDistanceBetweenVectors(string start = \"\", string end = \"\") {");
code("float xdiff = trQuestVarGet(end + \"X\") - trQuestVarGet(start + \"X\");");
code("float zdiff = trQuestVarGet(end + \"Z\") - trQuestVarGet(start + \"Z\");");
code("float dist = xsSqrt(xdiff * xdiff + zdiff * zdiff);");
code("return(dist);");
code("}");

code("float zDistanceBetweenVectors3d(string start = \"\", string end = \"\") {");
code("float xdiff = trQuestVarGet(end + \"X\") - trQuestVarGet(start + \"X\");");
code("float ydiff = trQuestVarGet(end + \"Y\") - trQuestVarGet(start + \"Y\");");
code("float zdiff = trQuestVarGet(end + \"Z\") - trQuestVarGet(start + \"Z\");");
code("float dist = xsSqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);");
code("return(dist);");
code("}");

code("float zDistanceToVectorSquared(string qv = \"\", string v = \"\") {");
code("trVectorQuestVarSet(\"abcd\", kbGetBlockPosition(\"\"+1*trQuestVarGet(qv), true));");
code("return(zDistanceBetweenVectorsSquared(\"abcd\", v));");
code("}");

code("float zDistanceToVector(string qv = \"\", string v = \"\") {");
code("trVectorQuestVarSet(\"abcd\", kbGetBlockPosition(\"\"+1*trQuestVarGet(qv), true));");
code("return(zDistanceBetweenVectors(\"abcd\", v));");
code("}");

code("void trVectorSetFromAngle(string qv = \"\", float angle = 0) {");
code("trVectorQuestVarSet(qv,xsVectorSet(xsSin(angle), 0, xsCos(angle)));");
code("}");

code("float angleBetweenVectors(string from = \"\", string to = \"\") {");
code("float a = trQuestVarGet(to+\"X\")-trQuestVarGet(from+\"X\");");
code("a = a / (trQuestVarGet(to+\"Z\")-trQuestVarGet(from+\"Z\"));");
code("a = xsAtan(a);");
code("if (trVectorQuestVarGetZ(from) > trVectorQuestVarGetZ(to)) {");
code("if (trVectorQuestVarGetX(from) > trVectorQuestVarGetX(to)) {");
code("a = a - PI;");
code("} else {");
code("a = a + PI;");
code("}");
code("}");
code("return(a);");
code("}");

code("vector zGetUnitVector(string start = \"\", string end = \"\", float mod = 1.0) {");
code("float xdiff = trQuestVarGet(end + \"X\") - trQuestVarGet(start + \"X\");");
code("float zdiff = trQuestVarGet(end + \"Z\") - trQuestVarGet(start + \"Z\");");
code("float dist = xsSqrt(xdiff * xdiff + zdiff * zdiff);");
code("vector ret = xsVectorSet(xdiff / dist * mod, 0, zdiff / dist * mod);");
code("return(ret);");
code("}");

code("vector zGetUnitVector3d(string start = \"\", string end = \"\", float mod = 1.0) {");
code("float xdiff = trQuestVarGet(end + \"X\") - trQuestVarGet(start + \"X\");");
code("float ydiff = trQuestVarGet(end + \"Y\") - trQuestVarGet(start + \"Y\");");
code("float zdiff = trQuestVarGet(end + \"Z\") - trQuestVarGet(start + \"Z\");");
code("float dist = xsSqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);");
code("vector ret = xsVectorSet(xdiff / dist * mod, ydiff / dist * mod, zdiff / dist * mod);");
code("return(ret);");
code("}");

code("vector crossProduct(string a = \"\", string b = \"\") {");
code("float x = trQuestVarGet(a + \"y\") * trQuestVarGet(b + \"z\") - trQuestVarGet(a + \"z\") * trQuestVarGet(b + \"y\");");
code("float y = trQuestVarGet(a + \"z\") * trQuestVarGet(b + \"x\") - trQuestVarGet(a + \"x\") * trQuestVarGet(b + \"z\");");
code("float z = trQuestVarGet(a + \"x\") * trQuestVarGet(b + \"y\") - trQuestVarGet(a + \"y\") * trQuestVarGet(b + \"x\");");
code("vector ret = xsVectorSet(x, y, z);");
code("return(ret);");
code("}");

code("vector intersectionWithCircle(string start = \"\", string end = \"\", string center = \"\", float radius = 0) {");
code("trVectorQuestVarSet(\"iDir\", zGetUnitVector(start, end));");
code("float x = 0;");
code("float z = 0;");
code("float dist = 0;");
code("float len = radius * 2;");
code("float lenmod = radius;");
code("for(i=8; >0) {");
code("x = trQuestVarGet(start+\"x\") + trQuestVarGet(\"iDirx\") * len;");
code("z = trQuestVarGet(start+\"z\") + trQuestVarGet(\"iDirz\") * len;");
code("dist = xsPow(x - trQuestVarGet(center+\"x\"), 2) + xsPow(z - trQuestVarGet(center+\"z\"), 2);");
code("if (dist > radius * radius) {");
code("len = len - lenmod;");
code("} else {");
code("len = len + lenmod;");
code("}");
code("lenmod = lenmod * 0.5;");
code("}");
code("return(xsVectorSet(x,0,z));");
code("}");

code("void modularCounterInit(string name = \"\", int size = 0) {");
code("trQuestVarSet(\"counter\" + name + \"size\", size);");
code("trQuestVarSet(\"counter\" + name + \"pointer\", 1);");
code("trQuestVarSet(name, 1);");
code("}");

code("int modularCounterNext(string name = \"\") {");
code("trQuestVarSet(\"counter\" + name + \"pointer\", 1 + trQuestVarGet(\"counter\" + name + \"pointer\"));");
code("if (trQuestVarGet(\"counter\" + name + \"pointer\") > trQuestVarGet(\"counter\" + name + \"size\")) {");
code("trQuestVarSet(\"counter\" + name + \"pointer\", 1);");
code("}");
code("trQuestVarSet(name, trQuestVarGet(\"counter\"+name+\"pointer\"));");
code("return(0 + trQuestVarGet(\"counter\" + name + \"pointer\"));");
code("}");

code("int peekModularCounterNext(string name = \"\") {");
code("trQuestVarSet(\"counter\" + name + \"fake\", 1 + trQuestVarGet(\"counter\" + name + \"pointer\"));");
code("if (trQuestVarGet(\"counter\" + name + \"fake\") >= trQuestVarGet(\"counter\" + name + \"size\")) {");
code("trQuestVarSet(\"counter\" + name + \"fake\", 1);");
code("}");
code("return(0 + trQuestVarGet(\"counter\" + name + \"fake\"));");
code("}");

code("void zBankInit(string name = \"\", int start = 0, int length = 0) {");
code("trQuestVarSet(\"z\"+name+\"start\", start);");
code("trQuestVarSet(\"z\"+name+\"end\", start + length);");
code("trQuestVarSet(\"z\"+name+\"pointer\", start);");
code("trQuestVarSet(name, start);");
code("}");

code("int zBankNext(string name = \"\", bool select = false) {");
code("trQuestVarSet(\"z\"+name+\"pointer\", trQuestVarGet(\"z\"+name+\"pointer\") + 1);");
code("if (trQuestVarGet(\"z\"+name+\"pointer\") >= trQuestVarGet(\"z\"+name+\"end\")) {");
code("trQuestVarSet(\"z\"+name+\"pointer\", trQuestVarGet(\"z\"+name+\"start\"));");
code("}");
code("trQuestVarSet(name, trQuestVarGet(\"z\"+name+\"pointer\"));");
code("if (select) {");
code("trUnitSelectClear();");
code("trUnitSelectByID(1*trQuestVarGet(name));");
code("}");
code("return(1*trQuestVarGet(name));");
code("}");

code("void zSetVar(string name = \"\", string var = \"\", float val = 0) {");
code("int index = trQuestVarGet(name);");
code("trQuestVarSet(\"z\"+name+\"i\"+index+\"v\"+var, val);");
code("}");

code("void zSetVarByIndex(string name = \"\", string var = \"\", int index = 0, float val = 0) {");
code("trQuestVarSet(\"z\"+name+\"i\"+index+\"v\"+var, val);");
code("}");

code("float zGetVar(string name = \"\", string var = \"\") {");
code("int index = trQuestVarGet(name);");
code("return(trQuestVarGet(\"z\"+name+\"i\"+index+\"v\"+var));");
code("}");

code("float zGetVarByIndex(string name = \"\", string var = \"\", int index = 0) {");
code("return(trQuestVarGet(\"z\"+name+\"i\"+index+\"v\"+var));");
code("}");

code("int zGetBankCount(string name = \"\") {");
code("return(trQuestVarGet(\"z\"+name+\"end\") - trQuestVarGet(\"z\"+name+\"start\"));");
code("}");

code("void yAddToDatabase(string to = \"\", string from = \"\") {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + to + \"count\");");
code("trQuestVarSet(\"zdatalite\" + to + \"index\"+zdatacount, trQuestVarGet(from));");
code("trQuestVarSet(\"zdatalite\" + to + \"count\", zdatacount+1);");
code("}");

code("int yGetDatabaseCount(string db = \"\") {");
code("return(trQuestVarGet(\"zdatalite\" + db + \"count\"));");
code("}");

code("int yDatabaseNext(string db = \"\", bool select = false, int pointer = 0) {");
code("for(zdatapointer=0;>1){}");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\"+pointer, trQuestVarGet(\"zdatalite\" + db + \"pointer\"+pointer)-1);");
code("if (0 > trQuestVarGet(\"zdatalite\" + db + \"pointer\"+pointer)) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\"+pointer, trQuestVarGet(\"zdatalite\" + db + \"count\")-1);");
code("}");
code("if (trQuestVarGet(\"zdatalite\" + db + \"pointer\"+pointer) >= trQuestVarGet(\"zdatalite\" + db + \"count\")) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\"+pointer, trQuestVarGet(\"zdatalite\" + db + \"count\")-1);");
code("}");
code("zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\"+pointer);");
code("trQuestVarSet(db, trQuestVarGet(\"zdatalite\" + db + \"index\"+zdatapointer));");
code("trQuestVarSet(\"zdatalite\"+db+\"pointer\", zdatapointer);");
code("if (select) {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(db), true);");
code("return(kbGetBlockID(\"\"+1*trQuestVarGet(db), true));");
code("}");
code("return(trQuestVarGet(db));");
code("}");

code("void yDatabaseSelectAll(string db = \"\") {");
code("trUnitSelectClear();");
code("for(zdatapointer=0; <yGetDatabaseCount(db)) {");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"zdatalite\"+db+\"index\"+zdatapointer), true);");
code("}");
code("}");

code("bool yDatabaseContains(string db = \"\", int index = 0) {");
code("for (x=yGetDatabaseCount(db) - 1; >=0) {");
code("if (xsRound(trQuestVarGet(\"zdatalite\" + db + \"index\" + x)) == index) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", x);");
code("trQuestVarSet(db, index);");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+index, true);");
code("return(true);");
code("}");
code("}");
code("return(false);");
code("}");


code("void yDatabasePointerDefault(string db = \"\", int pointer = 0) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\"+pointer, 0);");
code("}");

code("void yRemoveFromDatabase(string db = \"\") {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + db + \"count\") - 1;");
code("int zdataremove = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trQuestVarSet(\"zdatalite\" + db + \"index\"+zdataremove,");
code("trQuestVarGet(\"zdatalite\" + db + \"index\"+zdatacount));");
code("trQuestVarSet(\"zdatalite\" + db + \"count\", zdatacount);");
code("}");

code("void yRemoveUpdateString(string db = \"\", string attr = \"\") {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + db + \"count\");");
code("int zdataremove = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trStringQuestVarSet(\"zdatalite\" + db + \"\" + zdataremove + \"\" + attr,");
code("trStringQuestVarGet(\"zdatalite\" + db + \"\" + zdatacount + \"\" + attr));");
code("trStringQuestVarSet(\"zdatalite\" + db + \"\" + zdatacount + \"\" + attr, \" \");");
code("}");

code("void yRemoveUpdateVar(string db = \"\", string attr = \"\") {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + db + \"count\");");
code("int zdataremove = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trQuestVarSet(\"zdatalite\" + db + \"\" + zdataremove + \"\" + attr,");
code("trQuestVarGet(\"zdatalite\" + db + \"\" + zdatacount + \"\" + attr));");
code("trQuestVarSet(\"zdatalite\" + db + \"\" + zdatacount + \"\" + attr, 0);");
code("}");

code("void yTransferUpdateVar(string to = \"\", string from = \"\", string attr = \"\") {");
code("int zdatato = trQuestVarGet(\"zdatalite\" + to + \"count\") - 1;");
code("int zdatafrom = trQuestVarGet(\"zdatalite\" + from + \"pointer\");");
code("trQuestVarSet(\"zdatalite\" + to + \"\"  + zdatato + \"\" + attr,");
code("trQuestVarGet(\"zdatalite\" + from + \"\"  + zdatafrom + \"\" + attr));");
code("}");

code("void yTransferUpdateString(string to = \"\", string from = \"\", string attr = \"\") {");
code("int zdatato = trQuestVarGet(\"zdatalite\" + to + \"count\") - 1;");
code("int zdatafrom = trQuestVarGet(\"zdatalite\" + from + \"pointer\");");
code("trStringQuestVarSet(\"zdatalite\" + to + \"\"  + zdatato + \"\" + attr,");
code("trStringQuestVarGet(\"zdatalite\" + from + \"\"  + zdatafrom + \"\" + attr));");
code("}");

code("void yAddUpdateString(string db = \"\", string attr = \"\", string value = \"\") {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + db + \"count\") - 1;");
code("trStringQuestVarSet(\"zdatalite\" + db + zdatacount + attr, value);");
code("}");

code("void yAddUpdateVar(string db = \"\", string attr = \"\", float value = 0.0) {");
code("int zdatacount = trQuestVarGet(\"zdatalite\" + db + \"count\") - 1;");
code("trQuestVarSet(\"zdatalite\" + db + zdatacount + attr, value);");
code("}");

code("string yGetString(string db = \"\", string attr = \"\") {");
code("int zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("return(trStringQuestVarGet(\"zdatalite\" + db + zdatapointer + attr));");
code("}");

code("float yGetVar(string db = \"\", string attr = \"\") {");
code("int zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("return(trQuestVarGet(\"zdatalite\" + db + \"\" + zdatapointer + \"\" + attr));");
code("}");

code("string yGetStringByIndex(string db = \"\", string attr = \"\", int index = 0) {");
code("return(trStringQuestVarGet(\"zdatalite\" + db + \"\" + index + \"\" + attr));");
code("}");

code("float yGetVarByIndex(string db = \"\", string attr = \"\", int index = 0) {");
code("return(trQuestVarGet(\"zdatalite\" + db + \"\" + index + \"\" + attr));");
code("}");

code("void ySetString(string db = \"\", string attr = \"\", string value = \"\") {");
code("int zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trStringQuestVarSet(\"zdatalite\" + db + \"\" + zdatapointer + \"\" + attr, value);");
code("}");

code("void ySetVar(string db = \"\", string attr = \"\", float value = 0.0) {");
code("int zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trQuestVarSet(\"zdatalite\" + db + \"\" + zdatapointer + \"\" + attr, value);");
code("}");

code("void ySetStringByIndex(string db = \"\", string attr = \"\", int index = 0, string value = \"\") {");
code("trStringQuestVarSet(\"zdatalite\" + db + \"\" + index + \"\" + attr, value);");
code("}");

code("void ySetVarByIndex(string db = \"\", string attr = \"\", int index = 0, float value = 0.0) {");
code("trQuestVarSet(\"zdatalite\" + db + \"\" + index + \"\" + attr, value);");
code("}");

code("int yGetUnitAtIndex(string db = \"\", int index = 0) {");
code("return(trQuestVarGet(\"zdatalite\"+db+\"index\"+index));");
code("}");

code("int ySetUnitAtIndex(string db = \"\", int index = 0, int value = 0) {");
code("trQuestVarSet(\"zdatalite\"+db+\"index\"+index, value);");
code("}");

code("int yGetPointer(string db = \"\") {");
code("return(trQuestVarGet(\"zdatalite\"+db+\"pointer\"));");
code("}");

code("void ySetPointer(string db = \"\", int val = 0) {");
code("trQuestVarSet(\"zdatalite\"+db+\"pointer\", val);");
code("trQuestVarSet(db, trQuestVarGet(\"zdatalite\" + db + \"index\"+val));");
code("}");

code("void yClearDatabase(string db = \"\") {");
code("trQuestVarSet(\"zdatalite\" + db + \"count\", 0);");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", 0);");
code("trQuestVarSet(db, 0);");
code("}");


code("int yFindLatestReverse(string qv = \"\", string proto = \"\", int p = 0) {");
code("int id = kbGetProtoUnitID(proto);");
code("trUnitSelectClear();");
code("for(x=trGetNextUnitScenarioNameNumber(); >trQuestVarGet(qv)) {");
code("int i = kbGetBlockID(\"\"+x, true);");
code("if (kbGetUnitBaseTypeID(i) == id) {");
code("trUnitSelectClear();");
code("trUnitSelectByID(i);");
code("if (trUnitIsOwnedBy(p)) {");
code("trQuestVarSet(qv, x);");
code("return(i);");
code("}");
code("}");
code("}");
code("return(-1);");
code("}");

code("int yFindLatest(string qv = \"\", string proto = \"\", int p = 0) {");
code("int id = kbGetProtoUnitID(proto);");
code("trUnitSelectClear();");
code("trQuestVarSet(\"next\",trGetNextUnitScenarioNameNumber() - 1);");
code("while(trQuestVarGet(\"next\") > trQuestVarGet(qv)) {");
code("trQuestVarSet(qv, 1 + trQuestVarGet(qv));");
code("int i = kbGetBlockID(\"\"+1*trQuestVarGet(qv), true);");
code("if (kbGetUnitBaseTypeID(i) == id) {");
code("trUnitSelectClear();");
code("trUnitSelectByID(i);");
code("if (trUnitIsOwnedBy(p)) {");
code("return(i);");
code("}");
code("}");
code("}");
code("return(-1);");
code("}");
code("const int TILE_NOT_FOUND = 0;");
code("const int TILE_FOUND = 1;");
code("const int TILE_VISITED = 2;");

code("const int EDGE_NOT_FOUND = 0;");
code("const int EDGE_NORMAL = 1;");
code("const int EDGE_CHASM = 2;");

code("const int ROOM_BASIC = 0;");
code("const int ROOM_STARTER = 1;");
code("const int ROOM_BOSS = 2;");

code("int ENEMY_PLAYER = 0;");
code("bool Multiplayer = false;");

code("int TERRAIN_CHASM = 5;");
code("int TERRAIN_SUB_CHASM = 4;");

code("int TERRAIN_WALL = 2;");
code("int TERRAIN_SUB_WALL = 13;");

code("int TERRAIN_PRIMARY = 0;");
code("int TERRAIN_SUB_PRIMARY = 1;");

code("int TERRAIN_SECONDARY = 0;");
code("int TERRAIN_SUB_SECONDARY = 1;");

code("void setupPlayerProto(string proto = \"\", int p = 0, float health = 1000, float range = 12, float speed = 4) {");
code("trModifyProtounit(proto, p, 27, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 27, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 30, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 30, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 31, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 31, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 0, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 0, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 0, health);");
code("trModifyProtounit(proto, p, 2, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 2, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 2, 16);");
code("trModifyProtounit(proto, p, 11, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 11, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 11, range);");
code("trModifyProtounit(proto, p, 1, 9999999999999999999.0);");
code("trModifyProtounit(proto, p, 1, -9999999999999999999.0);");
code("trModifyProtounit(proto, p, 1, speed);");
code("trModifyProtounit(proto, p, 24, -1);");
code("trModifyProtounit(proto, p, 25, -1);");
code("trModifyProtounit(proto, p, 26, -1);");
code("}");

code("rule setup");
code("active");
code("runImmediately");
code("{");
code("Multiplayer = aiIsMultiplayer();");
code("trSetUnitIdleProcessing(false);");

code("trSetObscuredUnits(false);");

code("ENEMY_PLAYER = cNumberPlayers - 1;");

code("trForceNonCinematicModels(true);");


code("modularCounterInit(\"spyFind\", 32);");
code("modularCounterInit(\"spyFound\", 32);");

code("trTechSetStatus(0, 304, 4);");

code("trModifyProtounit(\"Transport Ship Atlantean\", 1, 5, -11);");
code("for(p=1; < ENEMY_PLAYER) {");
code("trModifyProtounit(\"Vision Revealer\", p, 7, 9999);");
code("trModifyProtounit(\"Vision Revealer\", p, 2, -99);");
code("trModifyProtounit(\"Hero Greek Achilles\", p, 5, 99);");
code("trModifyProtounit(\"Vision SFX\", p, 0, -9999);");
code("trModifyProtounit(\"Dwarf\", p, 55, 4);");
code("trModifyProtounit(\"Athena\", p, 55, 4);");

code("trPlayerKillAllGodPowers(p);");
code("trPlayerTechTreeEnabledGodPowers(p, false);");

code("trModifyProtounit(\"Sling Stone\", p, 1, -29);");
code("trModifyProtounit(\"Arrow Flaming\", p, 1, -29);");
code("trModifyProtounit(\"Javelin Flaming\", p, 1, -29);");
code("trModifyProtounit(\"Axe\", p, 1, -29);");

code("setupPlayerProto(\"Slinger\", p, 1000, 12, 4);");
code("setupPlayerProto(\"Archer Atlantean\", p, 1000, 22, 4);");
code("setupPlayerProto(\"Javelin Cavalry\", p, 1000, 4, 5);");
code("setupPlayerProto(\"Throwing Axeman\", p, 1000, 12, 4);");
code("setupPlayerProto(\"Minion\", p, 300, 0, 4);");

code("trForbidProtounit(p, \"Trident Soldier Hero\");");
code("trForbidProtounit(p, \"Archer Atlantean Hero\");");
code("trForbidProtounit(p, \"Royal Guard Hero\");");
code("trForbidProtounit(p, \"Lancer Hero\");");
code("trForbidProtounit(p, \"Javelin Cavalry Hero\");");
code("trForbidProtounit(p, \"Wall Connector\");");
code("trForbidProtounit(p, \"Dock\");");
code("trForbidProtounit(p, \"Settlement Level 1\");");
code("trForbidProtounit(p, \"House\");");
code("trForbidProtounit(p, \"Granary\");");
code("trForbidProtounit(p, \"Mining Camp\");");
code("trForbidProtounit(p, \"Lumber Camp\");");
code("trForbidProtounit(p, \"Monument\");");
code("trForbidProtounit(p, \"Temple\");");
code("trForbidProtounit(p, \"Wonder\");");
code("trForbidProtounit(p, \"Market\");");
code("trForbidProtounit(p, \"Dwarf Foundry\");");
code("trForbidProtounit(p, \"Armory\");");
code("trForbidProtounit(p, \"Hill Fort\");");
code("trForbidProtounit(p, \"Tower\");");
code("trForbidProtounit(p, \"Longhouse\");");

code("trQuestVarSet(\"p\"+p+\"attackRange\", 12);");
code("trQuestVarSet(\"p\"+p+\"attackSpeed\", 1);");
code("trQuestVarSet(\"p\"+p+\"spellRange\", 1);");
code("trQuestVarSet(\"p\"+p+\"spellDamage\", 1);");
code("trQuestVarSet(\"p\"+p+\"spellDuration\", 1);");
code("}");

code("xsEnableRule(\"setup_enemies\");");
code("xsEnableRule(\"data_load_00\");");
code("xsDisableSelf();");
code("}");

code("rule no_extra_resources");
code("active");
code("highFrequency");
code("{");
code("if (trTime() > cActivationTime) {");
code("for (p=1; <cNumberPlayers) {");
code("trPlayerGrantResources(p, \"Food\", -1000.0);");
code("trPlayerGrantResources(p, \"Wood\", -1000.0);");
code("trPlayerGrantResources(p, \"Gold\", -1000.0);");
code("trPlayerGrantResources(p, \"Favor\", -1000.0);");
code("}");
code("xsDisableSelf();");
code("}");
code("}");

code("rule Z_cin_00");
code("inactive");
code("highFrequency");
code("{");
code("if (trTime() > cActivationTime + 1) {");
code("trSetFogAndBlackmap(false, false);");
code("trCameraCut(vector(96,90,98), vector(0,-1,0), vector(0,0,1), vector(1,0,0));");
code("int posX = 96 - 10;");
code("int posZ = 96 + 10;");
code("trModifyProtounit(\"Curse SFX\", 1, 8, 8);");
code("trArmyDispatch(\"1,0\", \"Dwarf\",1,posX,0,posZ,0,true);");
code("for(x=10; >0) {");
code("posX = posX + 2;");
code("trArmyDispatch(\"1,0\", \"Dwarf\",1,posX,0,posZ,0,false);");
code("}");
code("for(x=5; >0) {");
code("posX = posX - 2;");
code("posZ = posZ - 2;");
code("trArmyDispatch(\"1,0\", \"Dwarf\",1,posX,0,posZ,0,false);");
code("}");
code("for (x=4; >0) {");
code("posX = posX + 2;");
code("posZ = posZ - 2;");
code("trArmyDispatch(\"1,0\", \"Dwarf\",1,posX,0,posZ,0,false);");
code("}");
code("trArmySelect(\"1,0\");");
code("trUnitChangeProtoUnit(\"Curse SFX\");");

code("trUIFadeToColor(0,0,0,500,0,false);");
code("trSoundPlayFN(\"cinematics\15_in\gong.wav\", \"4\", -1, \"\",\"\");");
code("trSoundPlayFN(\"godpower.wav\", \"6\", -1, \"\",\"\");");
code("trOverlayText(\"Zenophobia presents...\", 4.0, -1, -1, -1);");

code("xsEnableRule(\"Z_cin_01\");");
code("xsDisableSelf();");
code("}");
code("}");

code("rule Z_cin_01");
code("inactive");
code("highFrequency");
code("{");
code("if (trTime() > cActivationTime + 5) {");
code("trSoundPlayFN(\"ui\thunder2.wav\",\"1\",-1,\"\",\"\");");
code("trOverlayText(\"Spellcrafters\", 3.0, -1, -1, -1);");
code("trUIFadeToColor(0,0,0,1000,3000,true);");
code("xsEnableRule(\"Z_cin_02\");");
code("xsDisableSelf();");
code("}");
code("}");

code("rule Z_cin_02");
code("inactive");
code("highFrequency");
code("{");
code("if (trTime() > cActivationTime + 5) {");
code("if (trQuestVarGet(\"p1progress\") == 0) {");
code("trQuestVarSet(\"stage\", 1);");
code("} else {");
code("trLetterBox(false);");
code("trSetLighting(\"default\", 0.1);");
code("trMusicPlay(\"cinematics\9_in\music.mp3\", \"1\", 0.5);");
code("trUIFadeToColor(0,0,0,1000,0,false);");
code("trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));");
code("trQuestVarSet(\"chooser\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"1,0\", \"Athena\",1,96,0,90,0,true);");
code("trMessageSetText(\"Host: Choose a stage to challenge.\",-1);");

code("int posX = 96 - 2 * trQuestVarGet(\"p1progress\");");

code("for(x=0; <= trQuestVarGet(\"p1progress\")) {");
code("trQuestVarSet(\"next\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"1,0\",\"Flag Numbered\",1,posX,0,100,0,true);");
code("trUnitSelectClear();");
code("trUnitSelectByQV(\"next\", true);");
code("trUnitSetAnimationPath(\"\"+x+\",0,0,0,0,0,0\");");
code("yAddToDatabase(\"stageChoices\", \"next\");");
code("posX = posX + 4;");
code("}");

code("xsEnableRule(\"choose_stage_00\");");
code("xsDisableSelf();");
code("}");
code("xsEnableRule(\"choose_stage_01\");");
code("}");
code("}");

code("rule choose_stage_00");
code("inactive");
code("highFrequency");
code("{");
code("int n = yDatabaseNext(\"stageChoices\");");
code("if (trCountUnitsInArea(\"\"+n, 1, \"Athena\",3) == 1) {");
code("trQuestVarSet(\"stage\", yGetPointer(\"stageChoices\") + 1);");
code("trUnitSelectClear();");
code("trUnitSelectByQV(\"chooser\", true);");
code("trUnitChangeProtoUnit(\"Rocket\");");
code("for(x=yGetDatabaseCount(\"stageChoices\"); >0) {");
code("yDatabaseNext(\"stageChoices\", true);");
code("trUnitDestroy();");
code("}");
code("yClearDatabase(\"stageChoices\");");
code("xsDisableSelf();");
code("}");
code("}");

code("void buildRoom(int x = 0, int z = 0, int type = 0) {");
code("switch(type)");
code("{");
code("case ROOM_BASIC:");
code("{");
code("for (i=2; >0) {");
code("trQuestVarSetFromRand(\"x0\", x * 35 + 5, x * 35 + 18, true);");
code("trQuestVarSetFromRand(\"z0\", z * 35 + 5, z * 35 + 18, true);");
code("trQuestVarSetFromRand(\"x1\", x * 35 + 22, x * 35 + 35, true);");
code("trQuestVarSetFromRand(\"z1\", z * 35 + 22, z * 35 + 35, true);");
code("int x0 = trQuestVarGet(\"x0\");");
code("int x1 = trQuestVarGet(\"x1\");");
code("int z0 = trQuestVarGet(\"z0\");");
code("int z1 = trQuestVarGet(\"z1\");");
code("trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);");
code("trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);");
code("}");
code("}");
code("}");
code("}");

code("int edgeName(int first = 0, int second = 0) {");
code("return(16 * xsMin(first, second) + xsMax(first, second));");
code("}");

code("int getOtherVertex(int edge = 0, int v = 0) {");
code("if (edge >= 16 * v) {");
code("return(edge - 16 * v);");
code("} else {");
code("return(edge / 16);");
code("}");
code("}");

code("void buildEdge(int edge = 0, int type = 0) {");
code("if (trQuestVarGet(\"edge\"+edge) == EDGE_NOT_FOUND) {");
code("int first = edge / 16;");
code("int second = edge - 16 * first;");
code("int z0 = first / 4;");
code("int x0 = first - 4 * z0;");
code("int z1 = second / 4;");
code("int x1 = second - 4 * z1;");
code("z0 = z0 * 35 + 17;");
code("x0 = x0 * 35 + 17;");
code("z1 = z1 * 35 + 23;");
code("x1 = x1 * 35 + 23;");
code("trPaintTerrain(x0, z0, x1, z1, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);");
code("trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, 0, false);");
code("trQuestVarSet(\"edge\"+edge, type);");
code("}");
code("}");


code("rule choose_stage_01");
code("inactive");
code("highFrequency");
code("{");
code("if (trQuestVarGet(\"stage\") > 0) {");
code("xsDisableSelf();");
code("trSoundPlayFN(\"default\",\"1\",-1,\"Building stage:0\",\"\");");
code("switch(1*trQuestVarGet(\"stage\"))");
code("{");
code("case 1:");
code("{");
code("TERRAIN_WALL = 2;");
code("TERRAIN_SUB_WALL = 2;");

code("TERRAIN_PRIMARY = 0;");
code("TERRAIN_SUB_PRIMARY = 34;");

code("TERRAIN_SECONDARY = 0;");
code("TERRAIN_SUB_SECONDARY = 64;");
code("}");
code("}");

code("trChangeTerrainHeight(0,0,150,150,10,false);");
code("trPaintTerrain(0,0,150,150,TERRAIN_WALL, TERRAIN_SUB_WALL,false);");

code("int x = 0;");
code("int z = 0;");
code("int n = 0;");
code("int total = 0;");
code("trQuestVarSet(\"tile0\", TILE_VISITED);");
code("trQuestVarSet(\"tile1\", TILE_FOUND);");
code("trQuestVarSet(\"tile4\", TILE_FOUND);");
code("trQuestVarSet(\"next\", 1);");
code("yAddToDatabase(\"frontier\", \"next\");");
code("yAddUpdateVar(\"frontier\", \"edge\", edgeName(0, 1));");
code("trQuestVarSet(\"next\", 4);");
code("yAddToDatabase(\"frontier\", \"next\");");
code("yAddUpdateVar(\"frontier\", \"edge\", edgeName(0, 4));");

code("while(total < 15) {");
code("trQuestVarSetFromRand(\"search\", 1, 5, true);");
code("yDatabasePointerDefault(\"frontier\");");
code("for(j=trQuestVarGet(\"search\"); >0) {");
code("yDatabaseNext(\"frontier\");");
code("}");
code("if (trQuestVarGet(\"tile\"+1*trQuestVarGet(\"frontier\")) < TILE_VISITED) {");
code("z = 1*trQuestVarGet(\"frontier\") / 4;");
code("x = 1*trQuestVarGet(\"frontier\") - 4 * z;");
code("buildEdge(1*yGetVar(\"frontier\", \"edge\"), EDGE_NORMAL);");
code("trQuestVarSet(\"tile\"+1*trQuestVarGet(\"frontier\"), TILE_VISITED);");
code("for(a=1; >=0) {");
code("for(b=1; >=0) {");
code("trQuestVarSet(\"newX\", (1 - 2 * b) * a + x);");
code("trQuestVarSet(\"newZ\", (1 - 2 * b) * (1 - a) + z);");
code("if (trQuestVarGet(\"newX\") < 0 || trQuestVarGet(\"newZ\") < 0 ||");
code("trQuestVarGet(\"newX\") > 3 || trQuestVarGet(\"newZ\") > 3) {");
code("continue;");
code("}");
code("n = 0 + trQuestVarGet(\"newX\") + 4 * trQuestVarGet(\"newZ\");");
code("if (trQuestVarGet(\"tile\"+n) < TILE_VISITED) {");
code("trQuestVarSet(\"next\", n);");
code("yAddToDatabase(\"frontier\", \"next\");");
code("yAddUpdateVar(\"frontier\", \"edge\", edgeName(1*trQuestVarGet(\"frontier\"), n));");
code("}");
code("}");
code("}");
code("total = total + 1;");
code("}");
code("yRemoveFromDatabase(\"frontier\");");
code("yRemoveUpdateVar(\"frontier\", \"edge\");");
code("}");
code("for(i=0; <10) {");
code("trQuestVarSetFromRand(\"first\", 1, 14);");
code("trQuestVarSetFromRand(\"direction\", 0, 3);");
code("z = 1*trQuestVarGet(\"first\") / 4;");
code("x = 1*trQuestVarGet(\"first\") - z * 4;");
code("a = 1*trQuestVarGet(\"direction\") / 2;");
code("b = 1*trQuestVarGet(\"direction\") - a * 2;");
code("trQuestVarSet(\"newX\", (1 - 2 * b) * a + x);");
code("trQuestVarSet(\"newZ\", (1 - 2 * b) * (1 - a) + z);");
code("if (trQuestVarGet(\"newX\") < 0 || trQuestVarGet(\"newZ\") < 0 ||");
code("trQuestVarGet(\"newX\") > 3 || trQuestVarGet(\"newZ\") > 3) {");
code("continue;");
code("} else {");
code("n = 0 + trQuestVarGet(\"newX\") + 4 * trQuestVarGet(\"newZ\");");
code("buildEdge(edgeName(1*trQuestVarGet(\"first\"), n), EDGE_NORMAL);");
code("}");
code("}");

code("for (i=1; < 15) {");
code("z = i / 4;");
code("x = i - z * 4;");
code("buildRoom(x, z, ROOM_BASIC);");
code("}");

code("trPaintTerrain(0,0,3,3,0,70,true);");
code("trPaintTerrain(0,0,3,3,TERRAIN_WALL,TERRAIN_SUB_WALL,false);");
code("}");
code("}");
code("const int NONE = 0;");
code("const int FIRE = 1;");
code("const int ICE = 2;");
code("const int THUNDER = 3;");
code("const int LIGHT = 4;");
code("const int DARK = 5;");

code("const int SPECIAL_NONE = 6;");
code("const int SPECIAL_FIRE = 7;");
code("const int SPECIAL_ICE = 8;");
code("const int SPECIAL_THUNDER = 9;");
code("const int SPECIAL_LIGHT = 10;");
code("const int SPECIAL_DARK = 11;");



code("bool terrainIsType(string qv = \"\", int type = 0, int subtype = 0) {");
code("bool isType = trGetTerrainType(trQuestVarGet(qv+\"x\"),trQuestVarGet(qv+\"z\")) == type;");
code("isType = trGetTerrainSubType(trQuestVarGet(qv+\"x\"),trQuestVarGet(qv+\"z\")) == subtype;");
code("return(isType);");
code("}");

code("string getElementColor(int element = 0) {");
code("switch(element)");
code("{");
code("case NONE:		{ return(\"\"); }");
code("case FIRE:		{ return(\"<color=0.9,0.2,0>\"); }");
code("case ICE:		{ return(\"<color=0.4,0.4,1.0>\"); }");
code("case LIGHT:		{ return(\"<color=1,1,0.5>\"); }");
code("case DARK:		{ return(\"<color=0.3,0,0.3>\"); }");
code("case THUNDER:	{ return(\"<color=0.1,0.9,0.7>\"); }");
code("}");
code("return(\"NOT AN ELEMENT\");");
code("}");

code("string getElementName(int element = 0) {");
code("switch(element)");
code("{");
code("case NONE:		{ return(\"\"); }");
code("case FIRE:		{ return(\"Fire\"); }");
code("case ICE:		{ return(\"Ice\"); }");
code("case LIGHT:		{ return(\"Light\"); }");
code("case DARK:		{ return(\"Wind\"); }");
code("case THUNDER:	{ return(\"Thunder\"); }");
code("}");
code("return(\"NOT AN ELEMENT\");");
code("}");


code("void removeArrow() {");
code("for(x=2;<0) {");
code("if (yGetVar(\"arrowsActive\", \"sfx\"+x) > 0) {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*yGetVar(\"arrowsActive\", \"sfx\"+x), true);");
code("trMutateSelected(kbGetProtoUnitID(\"Rocket\"));");
code("trUnitDestroy();");
code("}");
code("}");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"arrowsActive\"), true);");
code("yRemoveFromDatabase(\"arrowsActive\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"destx\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"destz\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"timeout\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"element\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"damage\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"player\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"sfx1\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"sfx2\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"special\");");
code("}");

code("void removeEnemy() {");
code("if (yGetVar(\"enemies\", \"bounty\") > 0) {");
code("trChatSend(0, \"Collected \" + 1*yGetVar(\"enemies\", \"bounty\") + \" <icon=(24)(icons/icon resource gold)>\");");
code("for(p=1; < ENEMY_PLAYER) {");
code("trPlayerGrantResources(p, \"Gold\", 1*yGetVar(\"enemies\", \"bounty\"));");
code("}");
code("}");
code("yRemoveFromDatabase(\"enemies\");");
code("yRemoveUpdateVar(\"enemies\", \"bounty\");");
code("yRemoveUpdateVar(\"enemies\", \"stunTimeout\");");
code("yRemoveUpdateVar(\"enemies\", \"stunSFX\");");
code("yRemoveUpdateVar(\"enemies\", \"poisonTimeout\");");
code("yRemoveUpdateVar(\"enemies\", \"poisonLast\");");
code("yRemoveUpdateVar(\"enemies\", \"poisonDamage\");");
code("yRemoveUpdateVar(\"enemies\", \"poisonSFX\");");
code("yRemoveUpdateVar(\"enemies\", \"frostCount\");");
code("yRemoveUpdateVar(\"enemies\", \"shockCount\");");
code("for(x=1; <=5) {");
code("yRemoveUpdateVar(\"enemies\", \"resist\"+x);");
code("}");
code("}");

code("void removePlayerCharacter() {");
code("yRemoveFromDatabase(\"playerCharacters\");");
code("yRemoveUpdateVar(\"playerCharacters\", \"player\");");
code("yRemoveUpdateVar(\"playerCharacters\", \"specialAttack\");");
code("yRemoveUpdateVar(\"playerCharacters\", \"attacking\");");
code("yRemoveUpdateVar(\"playerCharacters\", \"attackNext\");");
code("}");

code("void removePlayerUnit() {");
code("yRemoveFromDatabase(\"playerUnits\");");
code("yRemoveUpdateVar(\"playerUnits\", \"stunTimeout\");");
code("yRemoveUpdateVar(\"playerUnits\", \"stunSFX\");");
code("yRemoveUpdateVar(\"playerUnits\", \"poisonTimeout\");");
code("yRemoveUpdateVar(\"playerUnits\", \"poisonLast\");");
code("yRemoveUpdateVar(\"playerUnits\", \"poisonDamage\");");
code("yRemoveUpdateVar(\"playerUnits\", \"poisonSFX\");");
code("yRemoveUpdateVar(\"playerUnits\", \"decay\");");
code("}");


code("void vectorSetAsTargetVector(string target = \"\", string from = \"\", string to = \"\", float dist = 40.0) {");
code("trVectorQuestVarSet(\"dir\", zGetUnitVector(from, to, dist));");
code("trQuestVarSet(target+\"x\", trQuestVarGet(\"dirx\") + trQuestVarGet(from+\"x\"));");
code("trQuestVarSet(target+\"z\", trQuestVarGet(\"dirz\") + trQuestVarGet(from+\"z\"));");

code("float scale = 0;");
code("if (trQuestVarGet(target+\"x\") < 0) {");
code("scale = trQuestVarGet(target+\"x\") / (trQuestVarGet(target+\"x\") - trQuestVarGet(from+\"x\"));");
code("trQuestVarSet(target+\"z\", trQuestVarGet(target+\"z\") + scale * (trQuestVarGet(from+\"z\") - trQuestVarGet(target+\"z\")));");
code("trQuestVarSet(target+\"x\", 0);");
code("} else if (trQuestVarGet(target+\"x\") > DIMENSION_X) {");
code("scale = (trQuestVarGet(target+\"x\") - DIMENSION_X) / (trQuestVarGet(target+\"x\") - trQuestVarGet(from+\"x\"));");
code("trQuestVarSet(target+\"z\", trQuestVarGet(target+\"z\") + scale * (trQuestVarGet(from+\"z\") - trQuestVarGet(target+\"z\")));");
code("trQuestVarSet(target+\"x\", DIMENSION_X);");
code("}");

code("if (trQuestVarGet(target+\"z\") < 0) {");
code("scale = trQuestVarGet(target+\"z\") / (trQuestVarGet(target+\"z\") - trQuestVarGet(from+\"z\"));");
code("trQuestVarSet(target+\"x\", trQuestVarGet(target+\"x\") + scale * (trQuestVarGet(from+\"x\") - trQuestVarGet(target+\"x\")));");
code("trQuestVarSet(target+\"z\", 0);");
code("} else if (trQuestVarGet(target+\"z\") > DIMENSION_Z) {");
code("scale = (trQuestVarGet(target+\"z\") - DIMENSION_Z) / (trQuestVarGet(target+\"z\") - trQuestVarGet(from+\"z\"));");
code("trQuestVarSet(target+\"x\", trQuestVarGet(target+\"x\") + scale * (trQuestVarGet(from+\"x\") - trQuestVarGet(target+\"x\")));");
code("trQuestVarSet(target+\"z\", DIMENSION_Z);");
code("}");
code("}");


code("bool collideWithTerrain(int arrow = 0) {");
code("bool collide = false;");
code("trVectorQuestVarSet(\"loc\", kbGetBlockPosition(\"\"+arrow));");
code("vectorToGrid(\"loc\", \"loc\");");
code("if (trCountUnitsInArea(\"\"+arrow,ENEMY_PLAYER,\"Phoenix Egg\", 1.5) > 0) {");
code("trQuestVarSetFromRand(\"sound\", 1, 3, true);");
code("trSoundPlayFN(\"mine\"+1*trQuestVarGet(\"sound\")+\".wav\",\"1\",-1,\"\",\"\");");
code("collide = true;");
code("} else if (terrainIsType(\"loc\", TERRAIN_WALL, TERRAIN_SUB_WALL)) {");
code("trQuestVarSetFromRand(\"sound\", 1, 3, true);");
code("trSoundPlayFN(\"mine\"+1*trQuestVarGet(\"sound\")+\".wav\",\"1\",-1,\"\",\"\");");
code("collide = true;");
code("}");
code("return(collide);");
code("}");


code("bool checkArrowDie() {");
code("int arrow = trQuestVarGet(\"arrowsActive\");");
code("trQuestVarSet(\"endx\", yGetVar(\"arrowsActive\", \"destx\"));");
code("trQuestVarSet(\"endz\", yGetVar(\"arrowsActive\", \"destz\"));");
code("if ((collideWithTerrain(arrow)) ||");
code("zDistanceToVectorSquared(\"arrowsActive\", \"end\") < 9 ||");
code("trTimeMS() > yGetVar(\"arrowsActive\", \"timeout\")) {");
code("return(true);");
code("} else {");
code("return(false);");
code("}");
code("}");

code("int spyEffect(int unit = 0, int proto = 0) {");
code("int x = modularCounterNext(\"spyFind\");");
code("trQuestVarSet(\"spyEye\"+x, 0 - proto);");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+unit, true);");
code("trTechInvokeGodPower(0, \"spy\", vector(0,0,0), vector(0,0,0));");
code("return(x);");
code("}");

code("void poisonUnit(string db = \"\", float duration = 0, float damage = 0, int p = 0) {");
code("if (p > 0) {");
code("duration = duration * trQuestVarGet(\"p\"+p+\"spellDuration\");");
code("damage = damage * trQuestVarGet(\"p\"+p+\"spellDamage\");");
code("}");
code("if (trTimeMS() + duration > yGetVar(db, \"poisonTimeout\")) {");
code("if (trTimeMS() > yGetVar(db, \"poisonTimeout\")) {");
code("if (yGetVar(db, \"poisonSFX\") == 0) {");
code("ySetVar(db, \"poisonSFX\", 0 - spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID(\"Shockwave poison effect\")));");
code("} else {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*yGetVar(db, \"poisonSFX\"), true);");
code("trMutateSelected(kbGetProtoUnitID(\"Poison SFX\"));");
code("}");
code("}");
code("ySetVar(db, \"poisonTimeout\", trTimeMS() + duration);");
code("}");
code("if (damage > yGetVar(db, \"poisonDamage\")) {");
code("ySetVar(db, \"poisonDamage\", damage);");
code("}");
code("}");

code("void stunUnit(string db = \"\", float duration = 0, int p = 0) {");
code("if (p > 0) {");
code("duration = duration * trQuestVarGet(\"p\"+p+\"spellDuration\");");
code("}");
code("if (trTimeMS() + duration > yGetVar(db, \"stunTimeout\")) {");
code("if (trTimeMS() > yGetVar(db, \"stunTimeout\")) {");
code("yAddToDatabase(\"stunnedUnits\", db);");
code("yAddUpdateVar(\"stunnedUnits\", \"proto\", kbGetUnitBaseTypeID(kbGetBlockID(\"\"+1*trQuestVarGet(db), true)));");
code("if (yGetVar(db, \"stunSFX\") == 0) {");
code("ySetVar(db, \"stunSFX\", 0 - spyEffect(1*trQuestVarGet(db), kbGetProtoUnitID(\"Shockwave stun effect\")));");
code("} else {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*yGetVar(db, \"stunSFX\"), true);");
code("trMutateSelected(kbGetProtoUnitID(\"Shockwave stun effect\"));");
code("}");
code("}");
code("ySetVar(db, \"stunTimeout\", trTimeMS() + duration);");
code("}");
code("}");

code("int addEffect(int car = 0, string proto = \"\", string anim = \"0,0,0,0,0,0,0\") {");
code("int sfx = trGetNextUnitScenarioNameNumber();");
code("trArmyDispatch(\"1,0\",\"Dwarf\",1,1,0,1,0,true);");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+sfx, true);");
code("trMutateSelected(kbGetProtoUnitID(proto));");
code("trUnitSetAnimationPath(anim);");
code("trMutateSelected(kbGetProtoUnitID(\"Relic\"));");
code("trImmediateUnitGarrison(\"\"+car);");
code("trMutateSelected(kbGetProtoUnitID(proto));");
code("return(sfx);");
code("}");

code("void healUnit(int p = 0, float amt = 0) {");
code("trDamageUnit(0.0 - amt);");
code("}");

code("void elementalDamage(int p = 0, int element = 0, float dmg = 0, bool spell = false) {");
code("dmg = dmg * (1 + trQuestVarGet(\"p\"+p+\"element\"+element+\"bonus\")) * (1.0 - yGetVar(\"enemies\", \"resist\"+element));");
code("trDamageUnit(dmg);");
code("}");


code("void arrowHitEnemy(int p = 0, int id = 0, int element = 0, float damage = 0, int special = 0) {");
code("if (special == FIRE) {");
code("damage = 2.0 * damage;");
code("} else if (special == NONE) {");
code("damage = 3.0 * damage;");
code("}");
code("elementalDamage(p, element, damage);");
code("if ((special == ICE) && kbUnitGetCurrentHitpoints(id) > 0) {");
code("stunUnit(\"enemies\", 1.5, p);");
code("}");
code("if ((special == DARK) && kbUnitGetCurrentHitpoints(id) > 0) {");
code("poisonUnit(\"enemies\", 12.0, 12.0, p);");
code("}");
code("}");

code("void arrowHit(int p = 0) {");
code("switch(1*yGetVar(\"arrowsActive\", \"element\"))");
code("{");
code("case ICE:");
code("{");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"p\"+p+\"unit\"), true);");
code("healUnit(p, 10);");
code("}");
code("case THUNDER:");
code("{");
code("for(y=yGetDatabaseCount(\"playerCharacters\"); >0) {");
code("yDatabaseNext(\"playerCharacters\");");
code("if (yGetVar(\"playerCharacters\", \"player\") == p) {");
code("ySetVar(\"playerCharacters\", \"specialAttack\", yGetVar(\"playerCharacters\", \"specialAttack\") - 1);");
code("break;");
code("}");
code("}");
code("}");
code("}");
code("}");


code("void shootArrow(int p = 0, int type = 0, string from = \"\", string to = \"\", float dmg = 0, int special = -1) {");
code("trQuestVarSet(\"next\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"\"+p+\",0\",\"Dwarf\",1,trQuestVarGet(from+\"x\"),0,trQuestVarGet(from+\"z\"),0,true);");
code("vectorSetAsTargetVector(\"target\", from, to);");

code("int sfx = 0;");

code("if (type == LIGHT) {");
code("float range = trQuestVarGet(\"p\"+p+\"attackRange\");");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"next\"), true);");
code("trMutateSelected(kbGetProtoUnitID(\"Petosuchus Projectile\"));");
code("trUnitHighlight(1.0, false);");
code("trSetUnitOrientation(zGetUnitVector(to, from), vector(0,1,0), true);");
code("trSetSelectedScale(3, 3, range * 1.25);");

code("yAddToDatabase(\"playerLasers\", \"next\");");
code("yAddUpdateVar(\"playerLasers\", \"range\", range * 1.25);");
code("yAddUpdateVar(\"playerLasers\", \"timeout\", trTimeMS() + 500);");
code("arrowHit(p);");
code("float dist = 0;");
code("float hitDist = 4;");
code("if (special == FIRE) {");
code("hitDist = 16;");
code("yAddUpdateVar(\"playerLasers\", \"timeout\", trTimeMS() + 1000);");
code("}");
code("for(x=yGetDatabaseCount(\"enemies\"); >0) {");
code("yDatabaseNext(\"enemies\");");
code("trVectorSetUnitPos(\"pos\", \"enemies\");");
code("dist = zDistanceBetweenVectors(from, \"pos\");");
code("if (dist < range) {");
code("trVectorQuestVarSet(\"hitbox\", zGetUnitVector(from, to, dist));");
code("trQuestVarSet(\"hitboxx\", trQuestVarGet(\"hitboxx\") + trQuestVarGet(from+\"x\"));");
code("trQuestVarSet(\"hitboxz\", trQuestVarGet(\"hitboxz\") + trQuestVarGet(from+\"z\"));");
code("if (zDistanceToVectorSquared(\"enemies\", \"hitbox\") < hitDist) {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"enemies\"), true);");
code("arrowHitEnemy(p, kbGetBlockID(\"\"+1*trQuestVarGet(\"enemies\"), true), LIGHT, dmg, special);");
code("}");
code("}");
code("}");
code("} else {");
code("yAddToDatabase(\"arrowsActive\", \"next\");");
code("yAddUpdateVar(\"arrowsActive\", \"damage\", dmg);");
code("yAddUpdateVar(\"arrowsActive\", \"element\", type);");
code("yAddUpdateVar(\"arrowsActive\", \"destx\", trQuestVarGet(\"targetx\"));");
code("yAddUpdateVar(\"arrowsActive\", \"destz\", trQuestVarGet(\"targetz\"));");
code("yAddUpdateVar(\"arrowsActive\", \"player\", p);");
code("yAddUpdateVar(\"arrowsActive\", \"timeout\", trTimeMS() + 5000);");
code("yAddUpdateVar(\"arrowsActive\", \"special\", special);");
code("trUnitSelectClear();");
code("trUnitSelectByQV(\"next\", true);");
code("trMutateSelected(kbGetProtoUnitID(\"Hero Greek Achilles\"));");

code("switch(type)");
code("{");
code("case NONE:");
code("{");
code("sfx = addEffect(1*trQuestVarGet(\"next\"), \"Petosuchus Projectile\");");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+sfx, true);");
code("trSetSelectedScale(2,2,2);");
code("yAddUpdateVar(\"arrowsActive\", \"sfx1\", sfx);");
code("}");
code("case FIRE:");
code("{");
code("yAddUpdateVar(\"arrowsActive\", \"sfx1\", addEffect(1*trQuestVarGet(\"next\"), \"Ball of Fire\"));");
code("}");
code("case ICE:");
code("{");
code("sfx = addEffect(1*trQuestVarGet(\"next\"), \"Lampades\");");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+sfx, true);");
code("trSetSelectedScale(0,0,0);");
code("yAddUpdateVar(\"arrowsActive\", \"sfx1\", sfx);");
code("}");
code("case THUNDER:");
code("{");
code("sfx = addEffect(1*trQuestVarGet(\"next\"), \"Arkantos God\");");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+sfx, true);");
code("trSetSelectedScale(0,0,0);");
code("yAddUpdateVar(\"arrowsActive\", \"sfx1\", sfx);");
code("}");
code("case DARK:");
code("{");
code("yAddUpdateVar(\"arrowsActive\", \"sfx1\", addEffect(1*trQuestVarGet(\"next\"), \"Poison SFX\"));");
code("}");
code("}");

code("switch(special)");
code("{");
code("case NONE:");
code("{");
code("yAddUpdateVar(\"arrowsActive\", \"sfx2\", addEffect(1*trQuestVarGet(\"next\"), \"Curse SFX\"));");
code("}");
code("case FIRE:");
code("{");
code("yAddUpdateVar(\"arrowsActive\", \"sfx2\", addEffect(1*trQuestVarGet(\"next\"), \"Hades Fire\"));");
code("}");
code("case ICE:");
code("{");
code("yAddUpdateVar(\"arrowsActive\", \"sfx2\", addEffect(1*trQuestVarGet(\"next\"), \"Lampades Bolt\"));");
code("}");
code("}");

code("trUnitSelectClear();");
code("trUnitSelectByQV(\"next\", true);");
code("trMutateSelected(kbGetProtoUnitID(\"Wadjet Spit\"));");
code("trUnitMoveToPoint(trQuestVarGet(to+\"x\"),0,trQuestVarGet(to+\"z\"), -1, false);");
code("}");
code("}");

code("rule spy_find");
code("active");
code("highFrequency");
code("{");
code("if ((trQuestVarGet(\"spyfound\") == trQuestVarGet(\"spyfind\")) == false) {");
code("while(trQuestVarGet(\"spysearch\") < trGetNextUnitScenarioNameNumber()) {");
code("int id = kbGetBlockID(\"\"+1*trQuestVarGet(\"spysearch\"), true);");
code("if (id >= 0) {");
code("if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID(\"Spy Eye\")) {");
code("int x = modularCounterNext(\"spyfound\");");
code("trUnitSelectClear();");
code("trUnitSelectByID(id);");
code("trMutateSelected(0 - trQuestVarGet(\"spyEye\"+x));");
code("trQuestVarSet(\"spyEye\"+x, trQuestVarGet(\"spysearch\"));");
code("}");
code("}");
code("trQuestVarSet(\"spysearch\", 1 + trQuestVarGet(\"spysearch\"));");
code("}");
code("trQuestVarSet(\"spyreset\", 1 + trQuestVarGet(\"spyreset\"));");
code("if (trQuestVarGet(\"spyreset\") >= 10) {");
code("trQuestVarSet(\"spyfound\", trQuestVarGet(\"spyfind\"));");
code("}");
code("} else {");
code("trQuestVarSet(\"spyreset\", 0);");
code("trQuestVarSet(\"spysearch\", trGetNextUnitScenarioNameNumber());");
code("}");
code("}");

code("const int SLINGER = 0;");
code("const int ARCUS = 1;");
code("const int TURMA = 2;");
code("const int AXEMAN = 3;");

code("int progress = 0;");

code("void showLoadProgress() {");
code("trSoundPlayFN(\"default\",\"1\",-1,\"Loading Data:\"+100 * progress / 9,\"icons\god power reverse time icons 64\");");
code("}");

code("rule data_load_00");
code("highFrequency");
code("inactive");
code("{");
code("if (Multiplayer) {");
code("trSoundPlayFN(\"default\",\"1\",-1,\"Loading:\",\"icons\god power reverse time icons 64\");");

code("int posX = 10;");

code("for(p=1; < ENEMY_PLAYER) {");
code("trModifyProtounit(\"Swordsman Hero\", p, 6, -100);");
code("trModifyProtounit(\"Swordsman Hero\", p, 16, -1000);");
code("trModifyProtounit(\"Swordsman Hero\", p, 17, -1000);");
code("trModifyProtounit(\"Swordsman Hero\", p, 18, -1000);");
code("trModifyProtounit(\"Swordsman Hero\", p, 19, -1000);");
code("trArmyDispatch(\"\"+p+\",0\",\"Swordsman\", 36, posX,0,10,0,true);");
code("}");

code("trBlockAllSounds(true);");
code("xsEnableRule(\"data_load_01_ready\");");
code("} else {");
code("for(p=1; < ENEMY_PLAYER) {");
code("trForbidProtounit(p, \"Swordsman Hero\");");
code("}");
code("trLetterBox(false);");
code("}");
code("trQuestVarSet(\"enemyVictoryMarker\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"1,0\",\"Victory Marker\",1,1,0,1,0,true);");
code("trArmySelect(\"1,0\");");
code("trUnitConvert(ENEMY_PLAYER);");
code("xsDisableSelf();");
code("}");

code("rule data_load_01_ready");
code("highFrequency");
code("inactive");
code("{");
code("int swordsmen = 0;");
code("int total = 36 * (cNumberPlayers - 2);");
code("for(p=1; < ENEMY_PLAYER) {");
code("swordsmen = swordsmen + trPlayerUnitCountSpecific(p, \"Swordsman\");");
code("}");
code("if (swordsmen == total) {");
code("xsEnableRule(\"data_load_01_load_data\");");
code("xsEnableRule(\"data_load_02_detect_data\");");
code("xsDisableSelf();");
code("}");
code("}");

code("rule data_load_01_load_data");
code("highFrequency");
code("inactive");
code("{");
code("trLetterBox(false);");
code("trBlockAllSounds(true);");
code("int c = trGetScenarioUserData(progress);");
code("if (c >= 36) {");
code("trSoundPlayFN(\"cantdothat.wav\",\"1\",-1,\"Invalid code at \" + progress + \"!\",\"\");");
code("}");
code("c = c + 36 * (trCurrentPlayer() - 1);");
code("trChatSend(0, \"Class data: \" + c);");
code("trUnitSelectClear();");
code("trUnitSelectByID(c);");
code("for(i=36; >0) {");
code("if (trUnitIsSelected() == false) {");
code("uiFindType(\"Swordsman\");");
code("} else {");
code("trChatSend(0, \"Found!\");");
code("break;");
code("}");
code("}");
code("uiTransformSelectedUnit(\"Swordsman Hero\");");
code("trForceNonCinematicModels(true);");
code("trLetterBox(true);");
code("showLoadProgress();");
code("xsDisableSelf();");
code("}");

code("rule data_load_02_detect_data");
code("highFrequency");
code("inactive");
code("{");
code("int swordsmen = 0;");
code("for(p=1; < ENEMY_PLAYER) {");
code("swordsmen = swordsmen + trPlayerUnitCountSpecific(p, \"Swordsman Hero\");");
code("}");
code("if (swordsmen == cNumberPlayers - 2) {");
code("for(p=1; < ENEMY_PLAYER) {");
code("for(x=0; < 36 * p) {");
code("if (kbGetUnitBaseTypeID(x) == kbGetProtoUnitID(\"Swordsman Hero\")) {");
code("if (progress <= 2) {");
code("trQuestVarSet(\"p\"+p+\"element\" + (2 * progress), iModulo(6, x));");
code("trQuestVarSet(\"p\"+p+\"element\" + (2 * progress + 1), x / 6);");
code("} else if (progress == 8) {");
code("trQuestVarSet(\"p\"+p+\"class\", iModulo(4, x));");
code("trQuestVarSet(\"p\"+p+\"progress\", x / 4);");
code("} else {");
code("trQuestVarSet(\"p\"+p+\"artifact\" + (progress - 2), x);");
code("}");
code("trUnitSelectClear();");
code("trUnitSelectByID(x);");
code("trMutateSelected(kbGetProtoUnitID(\"Swordsman\"));");
code("break;");
code("}");
code("}");
code("}");
code("progress = progress + 1;");
code("showLoadProgress();");
code("if (progress == 9) {");
code("xsDisableSelf();");
code("xsEnableRule(\"data_load_03_done\");");
code("} else {");
code("xsEnableRule(\"data_load_01_load_data\");");
code("}");
code("}");
code("}");


code("rule data_load_03_done");
code("highFrequency");
code("inactive");
code("{");
code("for(x=0; < trQuestVarGet(\"enemyVictoryMarker\")) {");
code("trUnitSelectClear();");
code("trUnitSelectByID(x);");
code("trUnitDestroy();");
code("}");
code("for(p=1; < ENEMY_PLAYER) {");
code("trForbidProtounit(p, \"Swordsman Hero\");");
code("trQuestVarSet(\"p\"+p+\"victoryMarker\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"\"+p+\",0\",\"Victory Marker\",1,1,0,1,0,true);");

code("switch(1*trQuestVarGet(\"p\"+p+\"element0\"))");
code("{");
code("case NONE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 60);");
code("trModifyProtounit(\"Wadjet Spit\", p, 1, -10);");
code("}");
code("case FIRE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 100);");
code("trModifyProtounit(\"Wadjet Spit\", p, 1, -10);");
code("}");
code("case ICE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 50);");
code("trModifyProtounit(\"Wadjet Spit\", p, 1, -15);");
code("}");
code("case THUNDER:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 20);");
code("trModifyProtounit(\"Wadjet Spit\", p, 1, -20);");
code("}");
code("case LIGHT:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 50);");
code("}");
code("case DARK:");
code("{");
code("trQuestVarSet(\"p\"+p+\"attack\", 0);");
code("trModifyProtounit(\"Wadjet Spit\", p, 1, -10);");
code("}");
code("}");

code("switch(1*trQuestVarGet(\"p\"+p+\"element1\"))");
code("{");
code("case NONE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 4);");
code("}");
code("case FIRE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 5);");
code("}");
code("case ICE:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 8);");
code("}");
code("case THUNDER:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 6);");
code("}");
code("case LIGHT:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 8);");
code("}");
code("case DARK:");
code("{");
code("trQuestVarSet(\"p\"+p+\"specialAttackCooldown\", 10);");
code("}");
code("}");
code("}");
code("trUnblockAllSounds();");
code("trSoundPlayFN(\"favordump.wav\",\"1\",-1,\"Done!\",\"\");");
code("xsDisableSelf();");
code("xsEnableRule(\"Z_cin_00\");");
code("}");
code("void setupProtounitBounty(string proto = \"\", int bounty = 2) {");
code("int p = kbGetProtoUnitID(proto);");
code("trQuestVarSet(\"proto\"+p+\"bounty\", bounty);");
code("trModifyProtounit(proto, ENEMY_PLAYER, 24, -1);");
code("trModifyProtounit(proto, ENEMY_PLAYER, 25, -1);");
code("trModifyProtounit(proto, ENEMY_PLAYER, 26, -1);");
code("trModifyProtounit(proto, ENEMY_PLAYER, 6, 6);");
code("}");

code("void setupProtounitResist(string proto = \"\", int element = 0, float resist = 0) {");
code("int p = kbGetProtoUnitID(proto);");
code("trQuestVarSet(\"proto\"+p+\"resist\"+element, resist);");
code("}");

code("void activateEnemy(int id = 0) {");
code("int proto = kbGetUnitBaseTypeID(id);");
code("int bounty = trQuestVarGet(\"proto\"+proto+\"bounty\");");

code("yAddToDatabase(\"enemies\", \"enemiesIncoming\");");
code("trQuestVarSetFromRand(\"bounty\", bounty / 2, bounty, true);");
code("yAddUpdateVar(\"enemies\", \"bounty\", trQuestVarGet(\"bounty\"));");
code("for(i = NONE; <= DARK) {");
code("yAddUpdateVar(\"enemies\", \"resist\"+i, trQuestVarGet(\"proto\"+proto+\"resist\"+i));");
code("}");
code("}");

code("rule setup_enemies");
code("inactive");
code("highFrequency");
code("{");
code("setupProtounitBounty(\"Anubite\", 5);");
code("setupProtounitResist(\"Anubite\", ICE, -0.5);");

code("setupProtounitBounty(\"Shade of Hades\", 5);");
code("setupProtounitResist(\"Shade of Hades\", LIGHT, -1);");
code("setupProtounitResist(\"Shade of Hades\", DARK, 1);");
code("xsDisableSelf();");
code("}");

code("rule enemies_always");
code("inactive");
code("highFrequency");
code("{");
code("int old = xsGetContextPlayer();");
code("int proto = 0;");
code("int id = 0;");
code("id = yDatabaseNext(\"enemiesIncoming\", true);");
code("for(p=1; < ENEMY_PLAYER) {");
code("if (trUnitHasLOS(p)) {");
code("activateEnemy(id);");
code("break;");
code("}");
code("}");

code("xsSetContextPlayer(ENEMY_PLAYER);");
code("while(trQuestVarGet(\"nextProj\") < trGetNextUnitScenarioNameNumber()) {");
code("id = kbGetBlockID(\"\"+1*trQuestVarGet(\"nextProj\"), true);");
code("proto = kbGetUnitBaseTypeID(id);");
code("if ((proto == kbGetProtoUnitID(\"Sling Stone\")) ||");
code("(proto == kbGetProtoUnitID(\"Arrow Flaming\")) ||");
code("(proto == kbGetProtoUnitID(\"Javelin Flaming\")) ||");
code("(proto == kbGetProtoUnitID(\"Axe\"))) {");
code("trUnitSelectClear();");
code("trUnitSelectByQV(\"nextProj\");");
code("if (trUnitIsOwnedBy(ENEMY_PLAYER)) {");
code("} else {");
code("trUnitDestroy();");
code("}");
code("}");
code("trQuestVarSet(\"nextProj\", 1 + trQuestVarGet(\"nextProj\"));");
code("}");

code("xsSetContextPlayer(old);");
code("}");
code("void spawnPlayerClone(int p = 0, string vdb = \"\") {");
code("trQuestVarSet(\"next\", trGetNextUnitScenarioNameNumber());");
code("string proto = \"Slinger\";");
code("switch(1*trQuestVarGet(\"p\"+p+\"class\"))");
code("{");
code("case 1:");
code("{");
code("proto = \"Archer Atlantean\";");
code("}");
code("case 2:");
code("{");
code("proto = \"Javelin Cavalry\";");
code("}");
code("case 3:");
code("{");
code("proto = \"Throwing Axeman\";");
code("}");
code("}");
code("trArmyDispatch(\"\"+p+\",0\",proto,1,trQuestVarGet(vdb+\"x\"),0,trQuestVarGet(vdb+\"z\"),0,true);");
code("yAddToDatabase(\"playerCharacters\", \"next\");");
code("yAddUpdateVar(\"playerCharacters\", \"player\", p);");
code("yAddUpdateVar(\"playerCharacters\", \"attacking\", 0);");
code("yAddUpdateVar(\"playerCharacters\", \"specialAttack\", trQuestVarGet(\"p\"+p+\"specialAttackCooldown\"));");
code("yAddToDatabase(\"playerUnits\", \"next\");");
code("}");

code("void spawnPlayer(int p = 0, string vdb = \"\") {");
code("trQuestVarSet(\"p\"+p+\"unit\", trGetNextUnitScenarioNameNumber());");
code("spawnPlayerClone(p, vdb);");
code("}");

code("void spawnMinion(int p = 0, string pos = \"\") {");
code("trQuestVarSet(\"next\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"\"+p+\",0\",\"Minion\",1,trQuestVarGet(pos+\"x\"),0,trQuestVarGet(pos+\"z\"),0,true);");
code("yAddToDatabase(\"playerUnits\", \"next\");");
code("yAddUpdateVar(\"playerUnits\", \"decay\", 8 / trQuestVarGet(\"p\"+p+\"spellDuration\"));");
code("}");

code("rule gameplay_start");
code("inactive");
code("highFrequency");
code("{");
code("xsDisableSelf();");
code("trSetFogAndBlackmap(true, true);");
code("xsEnableRule(\"gameplay_always\");");
code("xsEnableRule(\"enemies_always\");");
code("trQuestVarSet(\"nextProj\", trGetNextUnitScenarioNameNumber());");

code("}");

code("rule gameplay_always");
code("inactive");
code("highFrequency");
code("{");
code("int old = xsGetContextPlayer();");
code("int id = 0;");
code("int target = 0;");
code("int p = 0;");
code("float dist = 0;");
code("float dmg = 0;");
code("float angle = 0;");
code("for (i = 0; < xsMin(yGetDatabaseCount(\"playerCharacters\"), 1 + ENEMY_PLAYER / 3)) {");
code("id = yDatabaseNext(\"playerCharacters\", true);");
code("p = yGetVar(\"playerCharacters\", \"player\");");
code("if (yGetVar(\"playerCharacters\", \"attacking\") == 0) {");
code("if (kbUnitGetAnimationActionType(id) == 12) {");
code("ySetVar(\"playerCharacters\", \"attacking\", 1);");
code("ySetVar(\"playerCharacters\", \"attackNext\", trTimeMS() + 500);");
code("}");
code("} else {");
code("if (kbUnitGetAnimationActionType(id) == 12) {");
code("if (trTimeMS() > yGetVar(\"playerCharacters\", \"attackNext\")) {");
code("ySetVar(\"playerCharacters\", \"attackNext\",");
code("yGetVar(\"playerCharacters\", \"attackNext\") + 1000.0 / trQuestVarGet(\"p\"+p+\"attackSpeed\"));");
code("xsSetContextPlayer(p);");
code("target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));");
code("trVectorSetUnitPos(\"start\", \"playerCharacters\");");
code("trVectorQuestVarSet(\"end\", kbGetBlockPosition(\"\"+target, true));");

code("ySetVar(\"playerCharacters\", \"specialAttack\", yGetVar(\"playerCharacters\", \"specialAttack\") - 1);");
code("if (yGetVar(\"playerCharacters\", \"specialAttack\") <= 0) {");
code("ySetVar(\"playerCharacters\", \"specialAttack\", trQuestVarGet(\"p\"+p+\"specialAttackCooldown\"));");
code("shootArrow(p, 1*trQuestVarGet(\"p\"+p+\"element0\"), \"start\", \"end\",");
code("trQuestVarGet(\"p\"+p+\"attack\"), 1*trQuestVarGet(\"p\"+p+\"element1\"));");
code("switch(1*trQuestVarGet(\"p\"+p+\"element1\"))");
code("{");
code("case THUNDER:");
code("{");
code("angle = fModulo(6.283185, angleBetweenVectors(\"start\", \"end\") - 0.39);");
code("for(x=5; >0) {");
code("angle = fModulo(6.283185, angle + 0.13);");
code("if (x == 3) {");
code("continue;");
code("}");
code("trVectorSetFromAngle(\"end\", angle);");
code("trQuestVarSet(\"endx\", trQuestVarGet(\"endx\") + trQuestVarGet(\"startx\"));");
code("trQuestVarSet(\"endz\", trQuestVarGet(\"endz\") + trQuestVarGet(\"startz\"));");
code("shootArrow(p, 1*trQuestVarGet(\"p\"+p+\"element0\"), \"start\", \"end\",");
code("trQuestVarGet(\"p\"+p+\"attack\"), 1*trQuestVarGet(\"p\"+p+\"element1\"));");
code("}");
code("}");
code("case LIGHT:");
code("{");
code("for(x=yGetDatabaseCount(\"playerUnits\"); >0) {");
code("id = yDatabaseNext(\"playerUnits\", true);");
code("if (id == -1 || trUnitAlive() == false) {");
code("removePlayerUnit();");
code("} else {");
code("healUnit(p, trQuestVarGet(\"p\"+p+\"attack\"));");
code("}");
code("}");
code("}");
code("case DARK:");
code("{");
code("spawnMinion(p, \"start\");");
code("}");
code("}");
code("} else {");
code("shootArrow(p, 1*trQuestVarGet(\"p\"+p+\"element0\"), \"start\", \"end\", trQuestVarGet(\"p\"+p+\"attack\"));");
code("}");
code("}");
code("} else {");
code("ySetVar(\"playerCharacters\", \"attacking\", 0);");
code("}");
code("}");
code("}");



code("for(i=0; < xsMin(cNumberPlayers, yGetDatabaseCount(\"arrowsActive\"))) {");
code("id = yDatabaseNext(\"arrowsActive\", true);");
code("if (id == -1) {");
code("removeArrow();");
code("} else if (checkArrowDie()) {");
code("removeArrow();");
code("trUnitChangeProtoUnit(\"Dust Small\");");
code("} else if (trCountUnitsInArea(\"\"+1*trQuestVarGet(\"arrowsActive\"),ENEMY_PLAYER,\"Unit\",2.5) > 0) {");
code("p = yGetVar(\"arrowsActive\", \"player\");");
code("trVectorSetUnitPos(\"pos\", \"arrowsActive\");");
code("if (yGetVar(\"arrowsActive\", \"special\") == FIRE) {");
code("dist = 36;");
code("} else {");
code("dist = 9;");
code("}");
code("for(x=yGetDatabaseCount(\"enemies\"); >0) {");
code("id = yDatabaseNext(\"enemies\", true);");
code("if (id == -1 || trUnitAlive() == false) {");
code("removeEnemy();");
code("} else {");
code("if (zDistanceToVectorSquared(\"enemies\", \"pos\") <= dist) {");
code("arrowHitEnemy(p, id, 1*yGetVar(\"arrowsActive\", \"element\"),");
code("yGetVar(\"arrowsActive\", \"damage\"), 1*yGetVar(\"arrowsActive\", \"special\"));");
code("}");
code("}");
code("}");
code("arrowHit(p);");
code("removeArrow();");
code("if (yGetVar(\"arrowsActive\", \"special\") == FIRE) {");
code("trDamageUnitPercent(100);");
code("trUnitChangeProtoUnit(\"Meteorite\");");
code("} else {");
code("trUnitChangeProtoUnit(\"Lightning Sparks\");");
code("}");
code("}");
code("}");

code("for(x=yGetDatabaseCount(\"stunnedUnits\"); >0) {");
code("yDatabaseNext(\"stunnedUnits\", true);");
code("trMutateSelected(1*yGetVar(\"stunnedUnits\", \"proto\"));");
code("trUnitOverrideAnimation(2, 0, false, true, -1, 0);");
code("}");

code("for(x=xsMin(3, yGetDatabaseCount(\"playerLasers\")); >0) {");
code("yDatabaseNext(\"playerLasers\", true);");
code("if (trTimeMS() > yGetVar(\"playerLasers\", \"timeout\")) {");
code("trUnitDestroy();");
code("yRemoveFromDatabase(\"playerLasers\");");
code("yRemoveUpdateVar(\"playerLasers\", \"timeout\");");
code("yRemoveUpdateVar(\"playerLasers\", \"range\");");
code("} else {");
code("float width = 3.0 * (yGetVar(\"playerLasers\", \"timeout\") - trTimeMS()) / 500;");
code("trSetSelectedScale(width, width, yGetVar(\"playerLasers\", \"range\"));");
code("}");
code("}");

code("xsSetContextPlayer(old);");
code("}");
rmAddTriggerEffect("SetIdleProcessing");
rmSetTriggerEffectParam("IdleProc",");*/rule _zenowashereagain inactive {if(true){xsDisableSelf();//");
rmSetStatusText("", 0.99);}