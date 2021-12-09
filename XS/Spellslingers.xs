void code(string xs="") {
rmAddTriggerEffect("SetIdleProcessing");
rmSetTriggerEffectParam("IdleProc",");*/"+xs+"/*");}
void main(void) {
rmSetStatusText("", 0.01);

rmSetMapSize(300, 300);

rmSetSeaLevel(0);
rmSetSeaType("greek river");
rmTerrainInitialize("GrassA",1);
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
code("const int DIMENSION_X = 300;");
code("const int DIMENSION_Z = 300;");
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

code("int yDatabaseNext(string db = \"\", bool select = false) {");
code("for(zdatapointer=0;>1){}");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", trQuestVarGet(\"zdatalite\" + db + \"pointer\")-1);");
code("if (0 > trQuestVarGet(\"zdatalite\" + db + \"pointer\")) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", trQuestVarGet(\"zdatalite\" + db + \"count\")-1);");
code("}");
code("if (trQuestVarGet(\"zdatalite\" + db + \"pointer\") >= trQuestVarGet(\"zdatalite\" + db + \"count\")) {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", trQuestVarGet(\"zdatalite\" + db + \"count\")-1);");
code("}");
code("zdatapointer = trQuestVarGet(\"zdatalite\" + db + \"pointer\");");
code("trQuestVarSet(db, trQuestVarGet(\"zdatalite\" + db + \"index\"+zdatapointer));");
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


code("void yDatabasePointerDefault(string db = \"\") {");
code("trQuestVarSet(\"zdatalite\" + db + \"pointer\", 0);");
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
code("int ENEMY_PLAYER = 0;");
code("bool Multiplayer = false;");

code("rule setup");
code("active");
code("runImmediately");
code("{");
code("Multiplayer = aiIsMultiplayer();");
code("trSetUnitIdleProcessing(false);");

code("trSetObscuredUnits(false);");

code("ENEMY_PLAYER = cNumberPlayers - 1;");

code("trForceNonCinematicModels(true);");


code("trModifyProtounit(\"Transport Ship Atlantean\", 1, 5, -11);");
code("for(p=1; < ENEMY_PLAYER) {");
code("trModifyProtounit(\"Vision Revealer\", p, 7, 9999);");
code("trModifyProtounit(\"Vision Revealer\", p, 2, -99);");

code("trModifyProtounit(\"Vision SFX\", p, 0, -9999);");

code("trModifyProtounit(\"Dwarf\", p, 55, 4);");

code("trPlayerKillAllGodPowers(p);");

code("trPlayerTechTreeEnabledGodPowers(p, false);");

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
code("}");

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
code("trSoundPlayFN(\"market.wav\",\"1\",-1,\"\",\"\");");
code("trOverlayText(\"Battle Gambit\", 3.0, -1, -1, -1);");
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
code("trLetterBox(false);");
code("trSetLighting(\"default\", 0.1);");
code("trMusicPlay(\"cinematics\9_in\music.mp3\", \"1\", 0.5);");
code("trUIFadeToColor(0,0,0,1000,0,false);");
code("trCameraCut(vector(96,70,26), vector(0,-0.7071,0.7071), vector(0,0.7071,0.7071), vector(1,0,0));");
code("trQuestVarSet(\"statue\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"1,0\", \"Statue of Lightning\",1,96,0,96,180,true);");
code("trMessageSetText(\"Host: Delete statue to skip tutorial.\",-1);");
code("trCounterAddTime(\"counter\", 8, 1, \"Tutorial Starting\", -1);");
code("xsEnableRule(\"tutorial_00_decide\");");
code("xsDisableSelf();");
code("}");
code("}");

code("const int SLINGER = 0;");
code("const int ARCUS = 1;");
code("const int TURMA = 2;");
code("const int AXEMAN = 3;");

code("int progress = 0;");

code("void showLoadProgress() {");
code("trSoundPlayFN(\"default\",\"1\",-1,\"\"+100 * progress / 7,\"icons\god power reverse time icons 64\");");
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
code("trQuestVarSet(\"p\"+p+\"element\" + progress, iModulo(6, x));");
code("trQuestVarSet(\"p\"+p+\"element\" + (progress + 1), x / 6);");
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
code("if (progress > 6) {");
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
code("}");
code("trLetterBox(false);");
code("trUnblockAllSounds();");
code("trSoundPlayFN(\"favordump.wav\",\"1\",-1,\"Done!\",\"\");");
code("xsDisableSelf();");
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

code("const int WALL_BRICK = 2;");
code("const int WALL_WOOD = 3;");
code("int TERRAIN_CHASM = 5;");
code("int TERRAIN_SUB_CHASM = 4;");

code("int TERRAIN_WALL = 2;");
code("int TERRAIN_SUB_WALL = 13;");


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
code("} else if (terrainIsType(\"loc\", TERRAIN_CHASM, TERRAIN_SUB_CHASM)) {");
code("trQuestVarSetFromRand(\"sound\", 1, 3, true);");
code("trSoundPlayFN(\"mine\"+1*trQuestVarGet(\"sound\")+\".wav\",\"1\",-1,\"\",\"\");");
code("collide = true;");
code("}");
code("return(collide);");
code("}");

code("void removeArrow() {");
code("yRemoveFromDatabase(\"arrowsActive\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"destx\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"destz\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"timeout\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"type\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"damage\");");
code("yRemoveUpdateVar(\"arrowsActive\", \"player\");");
code("}");

code("bool checkArrowDie() {");
code("int arrow = trQuestVarGet(\"arrowsActive\");");
code("trQuestVarSet(\"endx\", yGetVar(\"arrowsActive\", \"destx\"));");
code("trQuestVarSet(\"endz\", yGetVar(\"arrowsActive\", \"destz\"));");
code("if ((collideWithTerrain(arrow)) ||");
code("zDistanceToVectorSquared(\"arrowsActive\", \"end\") < 4 ||");
code("trTimeMS() > yGetVar(\"arrowsActive\", \"timeout\")) {");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+arrow);");
code("trUnitChangeProtoUnit(\"Dust Small\");");
code("return(true);");
code("} else {");
code("return(false);");
code("}");
code("}");

code("void removeEnemy() {");
code("if (yGetVar(\"enemies\", \"bounty\") > 0) {");
code("trChatSend(0, \"Collected \" + 1*yGetVar(\"enemies\", \"bounty\") + \" <icon=(24)(icons/icon resource gold)>\");");
code("trPlayerGrantResources(1, \"Gold\", 1*yGetVar(\"enemies\", \"bounty\"));");
code("}");
code("yRemoveFromDatabase(\"enemies\");");
code("yRemoveUpdateVar(\"enemies\", \"bounty\");");
code("yRemoveUpdateVar(\"enemies\", \"stunTimeout\");");
code("yRemoveUpdateVar(\"enemies\", \"next\");");
code("}");

code("void shootArrow(int p = 0, int type = 0, string from = \"\", string to = \"\", float dmg = 0) {");
code("trQuestVarSet(\"next\", trGetNextUnitScenarioNameNumber());");
code("trArmyDispatch(\"\"+p+\",0\",\"Dwarf\",1,trQuestVarGet(\"startx\"),0,trQuestVarGet(\"startz\"),0,true);");
code("vectorSetAsTargetVector(\"target\", from, to);");

code("yAddToDatabase(\"arrowsActive\", \"next\");");
code("yAddUpdateVar(\"arrowsActive\", \"damage\", dmg);");
code("yAddUpdateVar(\"arrowsActive\", \"type\", type);");
code("yAddUpdateVar(\"arrowsActive\", \"destx\", trQuestVarGet(\"targetx\"));");
code("yAddUpdateVar(\"arrowsActive\", \"destz\", trQuestVarGet(\"targetz\"));");
code("yAddUpdateVar(\"arrowsActive\", \"player\", p);");
code("yAddUpdateVar(\"arrowsActive\", \"timeout\", trTimeMS() + 5000);");
code("trUnitSelectClear();");
code("trUnitSelect(\"\"+1*trQuestVarGet(\"next\"), true);");
code("switch(type)");
code("{");
code("case NONE:");
code("{");

code("}");
code("}");
code("}");
rmAddTriggerEffect("SetIdleProcessing");
rmSetTriggerEffectParam("IdleProc",");*/rule _zenowashereagain inactive {if(true){xsDisableSelf();//");
rmSetStatusText("", 0.99);}