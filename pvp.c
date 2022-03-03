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

void buildPvPSquare(int x = 0, int z = 0, int p = 0) {
	trPaintTerrain(x,z,x+12,z+12,0,34,false); // sand
	/* vertical lines */
	trPaintTerrain(x,z,x,z+12,0,74,false);
	trPaintTerrain(x+12,z,x+12,z+12,0,74,false);
	/* horizontal lines */
	trPaintTerrain(x,z,x+12,z,0,75,false);
	trPaintTerrain(x,z+12,x+12,z+12,0,75,false);
	/* corners */
	trPaintTerrain(x,z,x,z,0,82,false);
	trPaintTerrain(x+12,z,x+12,z,0,81,false);
	trPaintTerrain(x,z+12,x,z+12,0,83,false);
	trPaintTerrain(x+12,z+12,x+12,z+12,0,80,false);
	/* deploy stuff */
	x = 1 + x * 2;
	z = 1 + z * 2;
	
	trVectorQuestVarSet("p"+p+"square",xsVectorSet(x+12,0,z+12));
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	for(i=0; <10) {
		trArmyDispatch("1,0","Dwarf",1,x+2*i,0,z+26,180,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trMutateSelected(relicProto(i+1));
		trUnitSetStance("Passive");
		trUnitOverrideAnimation(2,0,true,false,-1);
		trSetSelectedScale(0.5,0.5,0.5);
	}
	for(i=0;<10) {
		trArmyDispatch("1,0","Dwarf",1,x+26,0,z+2*i,270,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trMutateSelected(relicProto(i+11));
		trUnitSetStance("Passive");
		trUnitOverrideAnimation(2,0,true,false,-1);
		trSetSelectedScale(0.5,0.5,0.5);
	}
	trQuestVarSet("p"+p+"passage", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,x+22,0,z+22,0,true);
	trArmySelect("1,0");
	trUnitConvert(p);
	trMutateSelected(kbGetProtoUnitID("Sky Passage"));
}

rule pvp_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1){
		trPaintTerrain(0,0,145,145,2,2,false); // cliff egyptian
		for(x=12; >= -12) {
			for(z=12; >= -12) {
				if (xsPow(x,2) + xsPow(z, 2) <= 144) {
					trPaintTerrain(72+x,72+z,72+x,72+z,0,34,false);
				}
			}
		}
		
		trQuestVarSet("boxCenter", 66);
		trVectorQuestVarSet("dir", vector(-52,0,-52));
		int p = 1;
		for(x=3; >0) {
			for(z=4; >0) {
				if (p < ENEMY_PLAYER) {
					buildPvPSquare(trQuestVarGet("boxCenter")+trQuestVarGet("dirx"),trQuestVarGet("boxCenter")+trQuestVarGet("dirz"),p);
					p = p + 1;
				}
				trVectorQuestVarSet("dir", rotationMatrix("dir", 0, 1));
			}
			trQuestVarSet("dirx", trQuestVarGet("dirx") + 35);
		}
		
		trVectorQuestVarSet("center", xsVectorSet(145, 0, 145));
		
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
		
		xsDisableSelf();
		xsEnableRule("pvp_build_map_teleport_done");
	}
}

rule pvp_build_map_teleport_done
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
