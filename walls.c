const int WALL_WAITING = 0;
const int WALL_FALLING = 1;

void processWalls() {
	vector pos = vector(0,0,0);
	vector wallpos = vector(0,0,0);
	vector wallmod = vector(0,0,0);
	if (xGetDatabaseCount(dUnlockWalls) > 0) {
		xDatabaseNext(dUnlockWalls);
		wallpos = xGetVector(dUnlockWalls,xWallPos);
		wallmod = xGetVector(dUnlockWalls,xWallMod);
		switch(1*xGetInt(dUnlockWalls, xWallState))
		{
			case WALL_WAITING:
			{
				pos = kbGetBlockPosition(""+xGetInt(dUnlockWalls,xWallKey));
				if ((xsVectorGetX(wallpos) - xsVectorGetX(wallmod) - 7 < xsVectorGetX(pos)) &&
					(xsVectorGetX(wallpos) + xsVectorGetX(wallmod) + 7 > xsVectorGetX(pos)) &&
					(xsVectorGetZ(wallpos) - xsVectorGetZ(wallmod) - 7 < xsVectorGetZ(pos)) &&
					(xsVectorGetZ(wallpos) + xsVectorGetZ(wallmod) + 7 > xsVectorGetZ(pos))) {
					xSetInt(dUnlockWalls,xWallState, WALL_FALLING);
					xSetInt(dUnlockWalls,xWallTimeout, trTimeMS() + 2500);
					for(x=xGetInt(dUnlockWalls, xWallCircles); < xGetInt(dUnlockWalls, xWallEnd)) {
						trUnitSelectClear();
						trUnitSelect(""+x);
						trUnitDestroy();
					}
					for(x=xGetInt(dUnlockWalls, xWallSFX); < xGetInt(dUnlockWalls, xWallCircles)) {
						trUnitSelectClear();
						trUnitSelect(""+x);
						trUnitChangeProtoUnit("Heka Shockwave SFX");
					}
					xUnitSelect(dUnlockWalls,xWallKey);
					trUnitChangeProtoUnit("Osiris Box Glow");
					trSoundPlayFN("xpack\xcinematics\8_in\pyramidscrape.mp3","1",-1,"","");
				}
			}
			case WALL_FALLING:
			{
				float progress = (xGetInt(dUnlockWalls, xWallTimeout) - trTimeMS()) / 2500.0;
				int x0 = (xsVectorGetX(wallpos) - xsVectorGetX(wallmod)) / 2;
				int x1 = (xsVectorGetX(wallpos) + xsVectorGetX(wallmod)) / 2;
				int z0 = (xsVectorGetZ(wallpos) - xsVectorGetZ(wallmod)) / 2;
				int z1 = (xsVectorGetZ(wallpos) + xsVectorGetZ(wallmod)) / 2;
				if (progress > 0) {
					progress = progress * (wallHeight - worldHeight) + worldHeight;
					trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, progress, false);
				} else {
					trChangeTerrainHeight(x0, z0, x1 + 1, z1 + 1, worldHeight, false);
					trPaintTerrain(x0-1,z0-1,x1+1,z1+1,TERRAIN_PRIMARY,TERRAIN_SUB_PRIMARY,false);
					trPaintTerrain(0,0,5,5,0,70,true);
					trPaintTerrain(0,0,5,5,TERRAIN_WALL,TERRAIN_SUB_WALL,false);
					trSoundPlayFN("cinematics\12_in\bang1.mp3","1",-1,"","");
					for(x=xGetInt(dUnlockWalls, xWallSFX); < xGetInt(dUnlockWalls, xWallCircles)) {
						trUnitSelectClear();
						trUnitSelect(""+x);
						trUnitDestroy();
					}
					xFreeDatabaseBlock(dUnlockWalls);
				}
			}
		}
	}
}
