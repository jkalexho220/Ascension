rmSetStatusText("", 0.01);

rmSetMapSize(192, 192);

rmSetSeaLevel(0);
rmSetSeaType("greek river");
rmTerrainInitialize("BlackRock",1);
rmSetGaiaCiv(cCivZeus);
rmSetLightingSet("Default");
rmSetStatusText("", 0.1);

int id = rmCreateArea("middle");
rmSetAreaSize(id, 0.11, 0.11);
rmSetAreaTerrainType(id, "EgyptianRoadA");
rmSetAreaCoherence(id, 1);
rmSetAreaLocation(id, 0.5, 0.5);
rmSetAreaBaseHeight(id, 0);
rmSetAreaHeightBlend(id, 1);
rmBuildArea(id);

id = rmCreateArea("middler");
rmSetAreaSize(id, 0.07, 0.07);
rmSetAreaTerrainType(id, "sandA");
rmSetAreaCoherence(id, 1);
rmSetAreaLocation(id, 0.5, 0.5);
rmSetAreaBaseHeight(id, 0);
rmSetAreaHeightBlend(id, 1);
rmBuildArea(id);


rmSetStatusText("", 0.2);
float angleStep = 2.0 * 3.141592 / cNumberPlayers;

float angle = 0;
float progress = 0.2;
for (x=0; < cNumberPlayers) {
    id = rmCreateArea("island"+x);
    rmSetAreaSize(id, 0.023, 0.023);
    if (x == 0) {
        rmSetAreaTerrainType(id, "CityTileA");
    } else {
        rmSetAreaTerrainType(id, "OlympusTile");
    }
    rmSetAreaCoherence(id, 1);
    rmSetAreaLocation(id, xsSin(angle) * 0.43 + 0.5, xsCos(angle) * 0.43 + 0.5);
    rmSetAreaBaseHeight(id, 0);
    rmSetAreaHeightBlend(id, 1);
    rmBuildArea(id);

    angle = angle + angleStep;
    progress = progress + 0.05;
    rmSetStatusText("", progress);
}