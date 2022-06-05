rule gladiator_worlds_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1) {
		float dist = 0;

		TERRAIN_WALL = 2;
		TERRAIN_SUB_WALL = 1;
		
		TERRAIN_PRIMARY = 0;
		TERRAIN_SUB_PRIMARY = 25;
		
		TERRAIN_SECONDARY = 0;
		TERRAIN_SUB_SECONDARY = 3;

		trQuestVarSet("treeDensity", 0.3);
		trStringQuestVarSet("treeProto1", "Pine");
		trStringQuestVarSet("treeProto2", "Pine");
		trStringQuestVarSet("treeProto3", "Ruins");
		trQuestVarSet("spriteDensity", 0.6);
		trStringQuestVarSet("spriteProto1", "Flowers");
		trStringQuestVarSet("spriteProto2", "Grass");
		trStringQuestVarSet("spriteProto3", "Rock Limestone Sprite");
		trQuestVarSet("rockDensity", 0.4);
		trStringQuestVarSet("rockProto1", "Rock Granite Big");
		trStringQuestVarSet("rockProto2", "Columns Broken");
		trStringQuestVarSet("rockProto3", "Rock Granite Small");

		trVectorQuestVarSet("startPosition", vector(145,0,145));

		trPaintTerrain(0, 0, 145, 145, TERRAIN_PRIMARY, TERRAIN_SUB_PRIMARY, false);
		paintSecondary(0, 0, 145, 145);

		// cliff outer circle
		for(x=0; <= 145) {
			for(z=0; <= 145) {
				if (xsPow(x - 73, 2) + xsPow(z - 73, 2) > 1764) {
					trPaintTerrain(x, z, x, z, TERRAIN_WALL, TERRAIN_SUB_WALL, false);
					trChangeTerrainHeight(x, z, x, z, wallHeight, false);
				}
			}
		}

		paintEyecandy(0, 0, 145, 145, "tree");
		paintEyecandy(0, 0, 145, 145, "rock");
		paintEyecandy(0, 0, 145, 145, "sprite");

		xsDisableSelf();
		xsEnableRule("gladiator_worlds_cin");
	}
}