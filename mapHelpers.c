const int MAP_STANDARD = 0;
const int MAP_PORTALS = 1;
const int MAP_OPEN = 2;

const int TILE_NOT_FOUND = 0;
const int TILE_FOUND = 1;
const int TILE_VISITED = 2;

const int EDGE_NOT_FOUND = 0;
const int EDGE_NORMAL = 1;
const int EDGE_BIG = 2;
const int EDGE_PORTAL = 3;
const int EDGE_WALL = 4;
const int EDGE_MOUNTAIN = 5;

const int ROOM_BASIC = 1;
const int ROOM_AMBUSH = 2;
const int ROOM_BOSS_ENTRANCE = 3;
const int ROOM_TRANSPORTER_GUY = 4;
const int ROOM_CHEST = 5;
const int ROOM_STARTER = 6;
const int ROOM_BOSS = 7;
const int ROOM_NOTTUD = 8;

const int ROOM_VILLAGE = 10;
const int ROOM_TEMPLE = 50;

const int ROOM_CIRCLE = 0;
const int ROOM_SQUARE = 1;

const int TRAP_LASERS = 1;
const int TRAP_CAROUSEL = 2;

int deployTownEyecandy(string proto = "", int x = 0, int z = 0, int heading = 0) {
	int n = trGetNextUnitScenarioNameNumber();
	trArmyDispatch("0,0","Dwarf",1,x+trQuestVarGet("villageX"),0,z+trQuestVarGet("villageZ"),heading,true);
	trUnitSelectClear();
	trUnitSelect(""+n, true);
	trUnitChangeProtoUnit(proto);
	return(n);
}


int edgeName(int first = 0, int second = 0) {
	return(16 * xsMin(first, second) + xsMax(first, second));
}

/*
input is in grid coordinates
return value is in vector coordinates
*/
vector randomNearEdge(int x1 = 0 , int z1 = 0, int x2 = 0, int z2 = 0) {
	/* calculate offset from center. More likely to be farther from center */
	int x0 = x2 - x1;
	int z0 = z2 - z1;
	trQuestVarSetFromRand("x0", 0, x0);
	trQuestVarSetFromRand("z0", 0, z0);
	trQuestVarSetFromRand("x1", trQuestVarGet("x0"), x0);
	trQuestVarSetFromRand("z1", trQuestVarGet("z0"), z0);
	/* randomly choose 1 or -1 */
	trQuestVarSetFromRand("negative", 0, 1, true);
	trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
	x0 = x1 + x2 + trQuestVarGet("negative") * trQuestVarGet("x1");
	trQuestVarSetFromRand("negative", 0, 1, true);
	trQuestVarSet("negative", 1 - 2 * trQuestVarGet("negative"));
	z0 = z1 + z2 + trQuestVarGet("negative") * trQuestVarGet("z1");
	return(xsVectorSet(x0, 0, z0));
}

vector randomNearEdgeOfRoom(int room = 0) {
	trQuestVarSetFromRand("rand", 1, 2, true);
	int i = trQuestVarGet("rand");
	vector ans = randomNearEdge(1*trQuestVarGet("room"+room+"bottom"+i+"x"),1*trQuestVarGet("room"+room+"bottom"+i+"z"),
		1*trQuestVarGet("room"+room+"top"+i+"x"),1*trQuestVarGet("room"+room+"top"+i+"z"));
	return(ans);
}

/*
input values are grid positions
*/
void paintRelicEdge(int x1 = 0 , int z1 = 0, int x2 = 0, int z2 = 0) {
	trVectorQuestVarSet("pos",randomNearEdge(x1, z1, x2, z2));
	int count = (x2-x1) / 2;
	for(i = count; >0) {
		vectorToGrid("pos", "loc");
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
			trVectorQuestVarSet("pos",randomNearEdge(x1, z1, x2, z2));
		} else {
			break;
		}
	}
	spawnRelic(trQuestVarGet("posx"), trQuestVarGet("posz"));
}

void paintEnemies(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
	for(a=x0; < x1) {
		trQuestVarSetFromRand("deploy", 0, 1, false);
		if (trQuestVarGet("deploy") < trQuestVarGet("enemyDensity")) {
			trQuestVarSetFromRand("z", z0, z1, true);
			trQuestVarSet("posX", a);
			trQuestVarSet("posZ", trQuestVarGet("z"));
			if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,2*a,0,2*trQuestVarGet("z"),trQuestVarGet("heading"),true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+randomLow(1*trQuestVarGet("enemyProtoCount"))));
				yAddToDatabase("enemiesIncoming", "next");
			}
		}
	}
	for(b=z0; < z1) {
		trQuestVarSetFromRand("deploy", 0, 1, false);
		if (trQuestVarGet("deploy") < trQuestVarGet("enemyDensity")) {
			trQuestVarSetFromRand("x", x0, x1, true);
			trQuestVarSet("posX", trQuestVarGet("x"));
			trQuestVarSet("posZ", b);
			if (terrainIsType("pos", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
				trQuestVarSetFromRand("heading", 0, 360, true);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,2*trQuestVarGet("x"),0,2*b,trQuestVarGet("heading"),true);
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+randomLow(1*trQuestVarGet("enemyProtoCount"))));
				yAddToDatabase("enemiesIncoming", "next");
			}
		}
	}
}

void paintEyecandy(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0, string type = "sprite") {
	float scale = trQuestVarGet(type+"scale");
	if (trQuestVarGet(type+"Density") > 0) {
		for(a=x0; < x1) {
			trQuestVarSetFromRand("deploy", 0, 1, false);
			if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
				trQuestVarSetFromRand("z", z0, z1, true);
				trQuestVarSet("locX", a);
				trQuestVarSet("locZ", trQuestVarGet("z"));
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
					trQuestVarSetFromRand("type", 1, 3, true);
					trQuestVarSetFromRand("heading", 0, 360, true);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
						2*a,0,2*trQuestVarGet("z"),trQuestVarGet("heading"),true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitConvert(0);
					if (scale > 0) {
						trSetSelectedScale(scale,scale,scale);
					}
				}
			}
		}
		for(b=z0; < z1) {
			trQuestVarSetFromRand("deploy", 0, 1, false);
			if (trQuestVarGet("deploy") < trQuestVarGet(type+"Density")) {
				trQuestVarSetFromRand("x", x0, x1, true);
				trQuestVarSet("locX", trQuestVarGet("x"));
				trQuestVarSet("locZ", b);
				if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
					trQuestVarSetFromRand("type", 1, 3, true);
					trQuestVarSetFromRand("heading", 0, 360, true);
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0",trStringQuestVarGet(type+"Proto"+1*trQuestVarGet("type")),1,
						2*trQuestVarGet("x"),0,2*b,trQuestVarGet("heading"),true);
					trUnitSelectClear();
					trUnitSelectByQV("next", true);
					trUnitConvert(0);
					if (scale > 0) {
						trSetSelectedScale(scale,scale,scale);
					}
				}
			}
		}
	}
}

void paintColumns(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
	if (trQuestVarGet("columnDensity") > 0) {
		for(a=x0; < x1) {
			trQuestVarSetFromRand("deploy", 0, 1, false);
			if (trQuestVarGet("deploy") < trQuestVarGet("columnDensity")) {
				trQuestVarSetFromRand("z", z0, z1, true);
				trPaintTerrain(a-1, 1*trQuestVarGet("z")-1, a+1, 1*trQuestVarGet("z")+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				trChangeTerrainHeight(a, 1*trQuestVarGet("z"), a+1, 1*trQuestVarGet("z")+1, wallHeight, false);
			}
		}
		for(b=z0; < z1) {
			trQuestVarSetFromRand("deploy", 0, 1, false);
			if (trQuestVarGet("deploy") < trQuestVarGet("columnDensity")) {
				trQuestVarSetFromRand("x", x0, x1, true);
				trPaintTerrain(1*trQuestVarGet("x")-1, b-1, 1*trQuestVarGet("x")+1, b+1, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
				trChangeTerrainHeight(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x")+1, b+1, wallHeight, false);
			}
		}
	}
}

int countRoomEntrances(int x = 0, int z = 0) {
	int room = x + 4 * z;
	int n = 0;
	int count = 0;
	for(a=1; >=0) {
		for(b=1; >=0) {
			trQuestVarSet("newX", (1 - 2 * b) * a + x);
			trQuestVarSet("newZ", (1 - 2 * b) * (1 - a) + z);
			if (trQuestVarGet("newX") < 0 || trQuestVarGet("newZ") < 0 ||
				trQuestVarGet("newX") > 3 || trQuestVarGet("newZ") > 3) {
				continue;
			}
			n = 0 + trQuestVarGet("newX") + 4 * trQuestVarGet("newZ");
			if (trQuestVarGet("edge"+edgeName(room, n)) == EDGE_NOT_FOUND) {
				continue;
			} else {
				count = count + 1;
			}
		}
	}
	return(count);
}

void paintSecondary(int x0 = 0, int z0 = 0, int x1 = 0, int z1 = 0) {
	for(a=x0; < x1) {
		trQuestVarSetFromRand("z", z0, z1, true);
		trQuestVarSet("locx", a);
		trQuestVarSet("locz", trQuestVarGet("z"));
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
			trPaintTerrain(a, 1*trQuestVarGet("z"), a, 1*trQuestVarGet("z"), TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
		}
	}
	for(b=z0; < z1) {
		trQuestVarSetFromRand("x", x0, x1, true);
		trQuestVarSet("locx", trQuestVarGet("x"));
		trQuestVarSet("locz", b);
		if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
			trPaintTerrain(1*trQuestVarGet("x"), b, 1*trQuestVarGet("x"), b, TERRAIN_SECONDARY, TERRAIN_SUB_SECONDARY, false);
		}
	}
}

/*
x and z are the room coordinates, not world coordinates.
*/
void paintCircle(int x = 0, int z = 0, int size = 0, int terrainType = 0, int terrainSubType = 0, float height = 0) {
	int z0 = size;
	for(a=0; < size+3) {
		for(b=size+3; >0) {
			if (a*a + z0 * z0 <= size * size) {
				trPaintTerrain(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, terrainType, terrainSubType, false);
				trChangeTerrainHeight(x*35+20-a, z*35+20-z0, x*35+20+a, z*35+20+z0, height, false);
				break;
			} else {
				z0 = z0 - 1;
			}
		}
	}
}

void placeTemple(int x = 0, int z = 0, float los = 0) {
	trQuestVarSet("templeRevealer", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,70*x+41,0,70*z+41,0,true);
	trUnitSelectClear();
	trUnitSelectByQV("templeRevealer", true);
	trUnitChangeProtoUnit("Cinematic Block");
	trQuestVarSet("temple", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,70*x+41,0,70*z+41,225,true);
	trUnitSelectClear();
	trUnitSelectByQV("temple", true);
	trUnitConvert(0);
	trMutateSelected(kbGetProtoUnitID("Statue of Lightning"));
	trSetSelectedScale(2,2,2);
	trUnitOverrideAnimation(2,0,true,false,-1);
	trQuestVarSet("templeLOS", los);
}
