void deployStadiumEyecandy(int index = 0) {
	trQuestVarSet("posx", trQuestVarGet("centerx") - trQuestVarGet("dist"+index) * trQuestVarGet("dirx"));
	trQuestVarSet("posz", trQuestVarGet("centerz") - trQuestVarGet("dist"+index) * trQuestVarGet("dirz"));
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next");
	trUnitConvert(0);
	trUnitTeleport(trQuestVarGet("posx"),0,trQuestVarGet("posz"));
	if (index < 5) {
		trMutateSelected(1*trQuestVarGet("proto"+index));
		trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
	} else {
		trMutateSelected(kbGetProtoUnitID("Bridge"));
		trVectorQuestVarSet("dir2", rotationMatrix("dir", 0, 1));
		trSetUnitOrientation(trVectorQuestVarGet("dir2"),vector(0,1,0),true);
	}
}

rule pvp_build_map
inactive
highFrequency
{
	trVectorQuestVarSet("center", xsVectorSet(145, 0, 145));
	float dist = 0;
	float scale = 0;
	
	trQuestVarSet("dist0", 25.5); // statue pharaoh
	trQuestVarSet("dist1", 25); // wall long scale 1, 1.3, 1
	trQuestVarSet("dist2", 23); // undermine ground decal
	trQuestVarSet("dist3", 49.0);
	trQuestVarSet("dist4", 40.0);
	trQuestVarSet("proto0", kbGetProtoUnitID("Statue of Lightning"));
	trQuestVarSet("proto1", kbGetProtoUnitID("Wall Long"));
	trQuestVarSet("proto2", kbGetProtoUnitID("undermine ground decal long"));
	trQuestVarSet("proto3", kbGetProtoUnitID("Migdol Stronghold"));
	trQuestVarSet("proto4", kbGetProtoUnitID("Statue of Lightning"));
	
	trQuestVarSet("scale3", 2.45);
	trQuestVarSet("scale4", 3.1);
	/* bridges */
	trQuestVarSet("dist5", 26.0);
	trQuestVarSet("dist6", 28.0);
	trQuestVarSet("dist7", 30.5);
	trQuestVarSet("dist8", 33.5);
	trQuestVarSet("dist9", 37.0);
	trQuestVarSet("scale5", 0.34116638);
	trQuestVarSet("scale6", 0.40473232);
	trQuestVarSet("scale7", 0.48499176);
	trQuestVarSet("scale8", 0.5811668);
	trQuestVarSet("scale9", 0.64910287);
	
	trVectorQuestVarSet("dir", vector(1,0,0));
	for(x=0;<16) {
		for(y=1; < 10) {
			deployStadiumEyecandy(y);
			if (y == 1) {
				trUnitSetAnimationPath("0,2,0,0,0,0,0");
				trSetSelectedScale(1, 1.3, 1);
			} else if (y > 2) {
				trSetSelectedScale(trQuestVarGet("scale"+y),trQuestVarGet("scale"+y),trQuestVarGet("scale"+y));
			}
			/* statues alternate */
			if (y == 4) {
				trUnitSetAnimationPath(""+iModulo(2, x)+",0,0,0,0,0,0");
				trUnitOverrideAnimation(2,0,true,false,-1);
			}
		}
		trVectorQuestVarSet("dir", rotationMatrix("dir", 0.980785, 0.19509));
		deployStadiumEyecandy(0);
		yAddToDatabase("PharaohStatues", "next");
		trVectorQuestVarSet("dir", rotationMatrix("dir", 0.980785, 0.19509));
	}
	
	trPaintTerrain(0,0,145,145,2,2,false); // cliff egyptian
	for(x=12; >= -12) {
		for(z=12; >= -12) {
			if (xsPow(x,2) + xsPow(z, 2) <= 144) {
				trPaintTerrain(72+x,72+z,72+x,72+z,0,34,false);
			}
		}
	}
	
	xsDisableSelf();
	xsEnableRule("pvp_build_map_done");
}

rule pvp_build_map_done
inactive
highFrequency
{
	yDatabaseNext("PharaohStatues");
	if (zDistanceToVectorSquared("PharaohStatues", "center") < 900.0) {
		trLetterBox(false);
		trUIFadeToColor(0,0,0,1000,0,false);
		xsDisableSelf();
		
		for(x=16; >0) {
			yDatabaseNext("PharaohStatues", true);
			trMutateSelected(kbGetProtoUnitID("Statue Pharaoh"));
		}
		yClearDatabase("PharaohStatues");
	}
}
