

void deployStadiumEyecandy(int index = 0) {
	vector pos = trVectorQuestVarGet("center") - (trVectorQuestVarGet("dir") * trQuestVarGet("dist"+index));
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	trUnitSelectClear();
	trUnitSelectByQV("next");
	trUnitConvert(0);
	trUnitTeleport(xsVectorGetX(pos),0,xsVectorGetZ(pos));
	if (index < 5) {
		trMutateSelected(1*trQuestVarGet("proto"+index));
		trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
	} else {
		trMutateSelected(kbGetProtoUnitID("Bridge"));
		trVectorQuestVarSet("dir2", rotationMatrix(trVectorQuestVarGet("dir"), 0, 1));
		trSetUnitOrientation(trVectorQuestVarGet("dir2"),vector(0,1,0),true);
	}
}

void pvpGetRelic(int p = 0, int relic = 0) {
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
	
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"relic"+relic+"holder");
	trMutateSelected(kbGetProtoUnitID("Transport Ship Greek"));
	
	trArmySelect("1,0");
	trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"relic"+relic+"holder"));
	trUnitChangeProtoUnit("Relic");
	
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"relic"+relic+"holder");
	trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
	
	if (trCurrentPlayer() == p) {
		trQuestVarSet("myRelics"+relic, 1 + trQuestVarGet("myRelics"+relic));
	}
	
	xAddDatabaseBlock(dFreeRelics, true);
	xSetInt(dFreeRelics, xUnitName, 1*trQuestVarGet("next"));
	xSetInt(dFreeRelics, xRelicType, relic);
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
	
	zSetProtoUnitStat("Revealer To Player", p, 2, 20);
	trVectorQuestVarSet("p"+p+"square",xsVectorSet(x+12,0,z+12));
	trArmyDispatch(""+p+",0","Dwarf",1,x+12,0,z+12,0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit("Revealer To Player");
	for(i=0; <10) {
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,x+2*i,0,z+26,180,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trMutateSelected(relicProto(i+1));
		trUnitSetStance("Passive");
		trUnitOverrideAnimation(2,0,true,false,-1);
		trSetSelectedScale(0.5,0.5,0.5);
		if (trCurrentPlayer() == p) {
			xAddDatabaseBlock(dRelicDescriptors, true);
			xSetInt(dRelicDescriptors, xUnitName, 1*trQuestVarGet("next"));
			xSetInt(dRelicDescriptors, xRelicType, i + 1);
		}
		trQuestVarSet("p"+p+"relic"+(i+1)+"holder", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,x+2*i,0,z+24,180,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trUnitChangeProtoUnit("Cinematic Block");
		pvpGetRelic(p, i + 1);
	}
	for(i=0;<10) {
		trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,x+26,0,z+2*i,270,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trMutateSelected(relicProto(i+11));
		trUnitSetStance("Passive");
		trUnitOverrideAnimation(2,0,true,false,-1);
		trSetSelectedScale(0.5,0.5,0.5);
		if (trCurrentPlayer() == p) {
			xAddDatabaseBlock(dRelicDescriptors, true);
			xSetInt(dRelicDescriptors, xUnitName, 1*trQuestVarGet("next"));
			xSetInt(dRelicDescriptors, xRelicType, i + 11);
		}
		trQuestVarSet("p"+p+"relic"+(i+11)+"holder", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,x+24,0,z+2*i,180,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trUnitChangeProtoUnit("Cinematic Block");
	}
	for(i=21; <= 30) {
		trQuestVarSet("p"+p+"relic"+i+"holder", trGetNextUnitScenarioNameNumber());
	}
	trArmyDispatch("1,0","Dwarf",1,x+4,0,z+4,180,true);
	trArmySelect("1,0");
	trUnitConvert(0);
	trUnitChangeProtoUnit("Cinematic Block");
	trQuestVarSet("p"+p+"passage", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,x+22,0,z+22,225,true);
	trArmySelect("1,0");
	trUnitConvert(p);
	trMutateSelected(kbGetProtoUnitID("Sky Passage"));
	
	int class = xGetInt(dPlayerData, xPlayerClass, p);
	xSetInt(dPlayerData, xPlayerUnit, trGetNextUnitScenarioNameNumber(), p);
	vector pos = trVectorQuestVarGet("p"+p+"square");
	trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),45,true);
	trArmySelect(""+p+",0");
	trMutateSelected(xGetInt(dClass, xClassProto, class));
	xAddDatabaseBlock(dPlayerCharacters, true);
	xSetInt(dPlayerCharacters, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
	
	for(i=12; >0) {
		if (trQuestVarGet("p"+p+"relic"+i) >0) {
			pvpGetRelic(p, 1*trQuestVarGet("p"+p+"relic"+i));
		}
	}
}

int dParticipants = 0;
int xParticipantPlayer = 0;

rule pvp_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1){
		PvP = true;
		dRelicDescriptors = xInitDatabase("relicDescriptors");
		xInitAddInt(dRelicDescriptors, "name");
		xInitAddInt(dRelicDescriptors, "type");

		dParticipants = xInitDatabase("participants");
		xParticipantPlayer = xInitAddInt(dParticipants, "player");

		trQuestVarSet("nickonhawk", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Odysseus",1,145,0,145,225,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trPaintTerrain(0,0,145,145,2,2,false); // cliff egyptian
		for(x=12; >= -12) {
			for(z=12; >= -12) {
				if (xsPow(x,2) + xsPow(z, 2) <= 144) {
					trPaintTerrain(72+x,72+z,72+x,72+z,0,34,false);
				}
			}
		}
		
		int p = 1;
		
		for(p=1; < ENEMY_PLAYER) {
			chooseClass(p, xGetInt(dPlayerData, xPlayerClass, p));
			trSetCivilizationNameOverride(p, "Level " + (1+xGetInt(dPlayerData, xPlayerLevel, p)));
			for(x=p+1; < ENEMY_PLAYER) {
				trPlayerSetDiplomacy(p, x, "Enemy");
				trPlayerSetDiplomacy(x, p, "Enemy");
			}
		}
		
		trQuestVarSet("boxCenter", 66);
		vector dir = vector(-52,0,-52);
		
		p = 1;
		for(x=3; >0) {
			for(z=4; >0) {
				if (p < ENEMY_PLAYER) {
					buildPvPSquare(trQuestVarGet("boxCenter")+xsVectorGetX(dir),trQuestVarGet("boxCenter")+xsVectorGetZ(dir),p);
					p = p + 1;
				}
				dir = rotationMatrix(dir, 0, 1);
			}
			dir = dir + vector(35,0,0);
		}
		
		trVectorQuestVarSet("center", xsVectorSet(145, 0, 145));
		
		trQuestVarSet("dist0", 25.5); // statue pharaoh
		trQuestVarSet("dist1", 25); // wall long scale 1, 1.3, 1
		trQuestVarSet("dist2", 23); // undermine ground decal
		trQuestVarSet("dist3", 49.0);
		trQuestVarSet("dist4", 40.0);
		trQuestVarSet("proto0", kbGetProtoUnitID("Militia"));
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
			trVectorQuestVarSet("dir", rotationMatrix(trVectorQuestVarGet("dir"), 0.980785, 0.19509));
			deployStadiumEyecandy(0);
			trQuestVarSet("pharaohStatue", trQuestVarGet("next"));
			trVectorQuestVarSet("dir", rotationMatrix(trVectorQuestVarGet("dir"), 0.980785, 0.19509));
		}
		
		xsDisableSelf();
		xsEnableRule("pvp_build_map_teleport_done");
	}
}

rule pvp_build_map_teleport_done
inactive
highFrequency
{
	if (unitDistanceToVector(1*trQuestVarGet("pharaohStatue"), trVectorQuestVarGet("center")) < 900.0) {
		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		xsEnableRule("pvp_cinematic");
		xsDisableSelf();
		
		TERRAIN_WALL = 2;
		TERRAIN_SUB_WALL = 2;
		
		trPaintTerrain(0,0,5,5,0,70,true);
		trPaintTerrain(0,0,5,5,2,2,false);
		
		unitTransform("Militia", "Statue Pharaoh");
	}
}

rule pvp_cinematic
inactive
highFrequency
{
	int p = trCurrentPlayer();
	if (trTime() > trQuestVarGet("cinTime")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trCameraCut(vector(130.856827,22.875111,129.921448), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trackInsert();
				trackAddWaypoint();
				trCameraCut(vector(98.131416,70.710236,95.031502), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trackAddWaypoint();
				trackPlay(12000,-1);
				trUIFadeToColor(0,0,0,1000,0,false);
				
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_cine332iwas",false,-1);
				trMusicPlay("music\interface\a cat named mittens.mp3","0.3",0);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Welcome to Glory Stadium! (I forced Zenophobia to repurpose his stadium into a PvP arena)",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
				//trQuestVarSet("cinStep", 14);
			}
			case 2:
			{
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Now, get ready for some high stakes PvP combat! Here are the rules.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 3:
			{
				trSoundPlayFN("","1",-1,
					"Nickonhawk: First, I have given you ten basic relics here.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				uiLookAtUnitByName(""+xGetInt(dPlayerData, xPlayerUnit, p));
			}
			case 4:
			{
				trSoundPlayFN("","1",-1,
					"Nickonhawk: You will use these relics to fight for more relics in the Glory Stadium.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 5:
			{
				trShowImageDialog(relicIcon(RELIC_PROJECTILES), "(3x) " + relicName(RELIC_PROJECTILES));
				gadgetUnreal("ShowImageBox-CloseButton");
				trSoundPlayFN("","1",-1,
					"Nickonhawk: The game is played in rounds. At the start of each round, a reward will be displayed.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 8);
			}
			case 6:
			{
				gadgetUnreal("ShowImageBox");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"passage");
				trUnitHighlight(7.0, true);
				trackInsert();
				trackAddWaypoint();
				uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"passage"));
				trackAddWaypoint();
				trackPlay(2000, -1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: If you want this reward, enter your Sky Passage. After a delay, you will be transported to the arena.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 7);
			}
			case 7:
			{
				trCameraCut(vector(98.131416,70.710236,95.031502), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trSoundPlayFN("","1",-1,
					"Nickonhawk: In the arena, you will fight other players. Last player standing gets the reward!",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 7);
			}
			case 8:
			{
				trMusicPlay("music\standard\behold the great science fi.mp3","1",3.0);
				trackInsert();
				trackAddWaypoint();
				trCameraCut(vector(130.856827,22.875111,129.921448), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trackAddWaypoint();
				trackPlay(1000, -1);
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_aomsp08tense",false,-1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: However, there's a catch! ",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
			}
			case 9:
			{
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_talk5sec",false,-1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Relics in this game are one-time use! Once you've used a relic in battle, it disappears!",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 10:
			{
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_aomsp09gianthorse",false,-1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: In addition, you must have at least one relic equipped in order to enter the arena!",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 11:
			{
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_talk5sec",false,-1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Winning in this game will require careful spending of your relics!",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 12:
			{
				trCameraCut(vector(98.131416,70.710236,95.031502), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"unit"));
				trSoundPlayFN("","1",-1,
					"Nickonhawk: After ten rounds are over, the game ends, and you get to keep any relics you can carry.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 13:
			{
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Players with a Golden Ticket will also get a Relic Transporter to carry 5 additional relics.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 6);
			}
			case 14:
			{
				trCameraCut(vector(130.856827,22.875111,129.921448), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitSetAnimation("ody_aomsp242thanks",false,-1);
				trSoundPlayFN("","1",-1,
					"Nickonhawk: Ready to battle? Let the games begin!",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				trUIFadeToColor(0,0,0,1000,3000,true);
			}
			case 15:
			{
				trUnitSelectClear();
				trUnitSelectByQV("nickonhawk");
				trUnitChangeProtoUnit("Revealer");
				trModifyProtounit("Revealer",0,2,40);
				trCameraCut(vector(98.131416,70.710236,95.031502), vector(0.483750,-0.707102,0.515744),
					vector(0.483751,0.707102,0.515744), vector(0.729368,0.000000,-0.684122));
				trSetFogAndBlackmap(true, true);
				trPlayerResetBlackMapForAllPlayers();
				xsDisableSelf();
				
				trQuestVarSet("immortalDummy", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trUnitChangeProtoUnit("Victory Marker");
				
				trQuestVarSet("pvpStep", 1);
				trQuestVarSet("pvpTime", trTime() + 1);
				trQuestVarSet("pvpRound", 1);
				
				for(p=1; < ENEMY_PLAYER) {
					xDatabaseNext(dPlayerCharacters);
					xUnitSelect(dPlayerCharacters, xUnitName);
					trUnitDestroy();
				}
				xClearDatabase(dPlayerCharacters);
				
				xsEnableRule("pvp_manager");
				trMusicPlayCurrent();
				trPlayNextMusicTrack();
			}
		}
	}
}

rule pvp_manager
inactive
highFrequency
{
	int p = 0;
	int relic = 0;
	int class = 0;
	int relics = 0;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trTime() > trQuestVarGet("pvpTime")) {
		switch(1*trQuestVarGet("pvpStep"))
		{
			case 0:
			{
				for(p=1; < ENEMY_PLAYER) {
					trPlayerKillAllGodPowers(p);
				}
				trQuestVarSet("pvpRound", 1 + trQuestVarGet("pvpRound"));
				if (trQuestVarGet("pvpRound") < 10) {
					trQuestVarSet("pvpStep", 1);
				} else if (trQuestVarGet("pvpRound") == 10) {
					trQuestVarSet("pvpStep", 11);
				} else if (trQuestVarGet("pvpRound") == 11) {
					trQuestVarSet("pvpStep", 21);
				}
				trQuestVarSet("pvpTime", trTime() + 1);
			}
			case 1:
			{
				trOverlayText("Round " + 1*trQuestVarGet("pvpRound"),3.0,-1,-1,-1);
				trSoundPlayFN("fanfare.wav","1",-1,"","");
				trQuestVarSet("pvpStep", 2);
				trQuestVarSet("pvpTime", trTime() + 2);
			}
			case 2:
			{
				gadgetUnreal("ShowImageBox-CloseButton");
				trSoundPlayFN("market.wav","1",-1,"","");
				trQuestVarSet("pvpStep", 3);
				trQuestVarSet("pvpTime", trTime() + 3);
				trQuestVarSetFromRand("pvpReward",trQuestVarGet("pvpRound"),20,true);
				trQuestVarSetFromRand("pvpCount", 2, 5, true);
				trQuestVarSetFromRand("count2", 2, 5, true);
				if (trQuestVarGet("count2") < trQuestVarGet("pvpCount")) {
					trQuestVarSet("pvpCount", trQuestVarGet("count2"));
				}
				trShowImageDialog(relicIcon(1*trQuestVarGet("pvpReward")),
					"("+1*trQuestVarGet("pvpCount")+"x) " + relicName(1*trQuestVarGet("pvpReward")));
			}
			case 3:
			{
				gadgetReal("ShowImageBox-CloseButton");
				gadgetUnreal("ShowImageBox");
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"rideLightning") == 1) {
						trQuestVarSet("p"+p+"rideLightning", 0);
						xClearDatabase(1*trQuestVarGet("p"+p+"lightningBalls"));
					}
					xSetBool(dPlayerData, xPlayerLaunched, false, p);
					trPlayerKillAllGodPowers(p);
					class = xGetInt(dPlayerData, xPlayerClass, p);
					xSetInt(dPlayerData, xPlayerUnit, trGetNextUnitScenarioNameNumber(), p);
					pos = trVectorQuestVarGet("p"+p+"square");
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),45,true);
					trArmySelect(""+p+",0");
					trMutateSelected(xGetInt(dClass, xClassProto, class));
					xAddDatabaseBlock(dPlayerCharacters, true);
					xSetInt(dPlayerCharacters, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
					equipRelicsAgain(p);
				}
				uiLookAtUnitByName(""+xGetInt(dPlayerData, xPlayerUnit, trCurrentPlayer()));
				trQuestVarSet("pvpStep", 4);
				trQuestVarSet("pvpTime", trTime() + 1);
				trCounterAddTime("countdown",31,1,"Battle begins",-1);
				xsEnableRule("pvp_peace");
			}
			case 4:
			{
				trQuestVarSet("pvpStep", 5);
				trQuestVarSet("pvpTime", trTime() + 29);
				trOverlayText("("+1*trQuestVarGet("pvpCount")+"x) " + relicName(1*trQuestVarGet("pvpReward")), 9999,20,20,1000);
			}
			case 5:
			{
				trSoundPlayFN("militarycreate.wav","1",-1,"","");
				trLetterBox(true);
				trUIFadeToColor(0,0,0,1000,0,true);
				trQuestVarSet("pvpStep", 6);
				trQuestVarSet("pvpTime", trTime() + 1);
				xsDisableRule("pvp_peace");
			}
			case 6:
			{
				xClearDatabase(dParticipants);
				for(p=1; < ENEMY_PLAYER) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
					if (trUnitGetIsContained("Sky Passage")) {
						xSetInt(dPlayerData, xPlayerRegenerateFavorLast, trTimeMS(), p);
						xSetInt(dPlayerData, xPlayerRegenerateHealthLast, trTimeMS(), p);
						xAddDatabaseBlock(dParticipants, true);
						xSetInt(dParticipants, xParticipantPlayer, p);
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"passage");
						trUnitEjectContained();
						trUnitSelectClear();
						trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
						trUnitEjectContained();
						relics = getRelicsDB(p);
						for(x=xGetDatabaseCount(relics); >0) {
							xDatabaseNext(relics);
							xUnitSelect(relics, xUnitName);
							trUnitDestroy();
							relic = xGetInt(relics, xRelicType);
							if (trCurrentPlayer() == p) {
								trQuestVarSet("myRelics"+relic, trQuestVarGet("myRelics"+relic) - 1);
							}
							if (trQuestVarGet("pvpRound") == 10) {
								pvpGetRelic(p, relic);
							}
						}
						xSetInt(dPlayerData, xPlayerDead, 0, p);
					}
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"passage");
					trUnitChangeProtoUnit("Automaton");
					trDamageUnitPercent(-100);
					trMutateSelected(kbGetProtoUnitID("Sky Passage"));
					
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
					trUnitDestroy();
				}
				if (xGetDatabaseCount(dParticipants) == 0) {
					trQuestVarSet("pvpTime", trTime() + 2);
					trQuestVarSet("pvpStep", 0);
					trSoundPlayFN("","1",-1,"Nickonhawk: No participants!","icons\hero g odysseus icon 64");
				} else if (xGetDatabaseCount(dParticipants) == 1) {
					xDatabaseNext(dParticipants);
					p = xGetInt(dParticipants, xParticipantPlayer);
					trQuestVarSet("pvpTime", trTime() + 2);
					trQuestVarSet("pvpStep", 0);
					trSoundPlayFN("","1",-1,"Nickonhawk: "+trStringQuestVarGet("p"+p+"name")+" was the only participant! Default victory!",
						"icons\hero g odysseus icon 64");
					for(x=trQuestVarGet("pvpCount"); >0) {
						pvpGetRelic(p, 1*trQuestVarGet("pvpReward"));
					}
				} else {
					pvpDetached = false;
					float angle = 6.283185 / xGetDatabaseCount(dParticipants);
					dir = vector(15,0,-15);
					float cos = xsCos(angle);
					float sin = xsSin(angle);
					trChatSend(0, "<color=1,1,1><u>Challengers</u></color>");
					for (x=xGetDatabaseCount(dParticipants); >0) {
						xDatabaseNext(dParticipants);
						p = xGetInt(dParticipants, xParticipantPlayer);
						relics = getRelicsDB(p);
						trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")} - "+xGetDatabaseCount(relics)+" relics");
						pos = dir + vector(145,0,145);
						spawnPlayer(p, pos);
						dir = rotationMatrix(dir, cos, sin);
						xSetInt(dPlayerData, xPlayerWellCooldownStatus, ABILITY_COST, p);
						xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST, p);
						xSetInt(dPlayerData, xPlayerRainCooldownStatus, ABILITY_COST, p);
					}
					trQuestVarSet("pvpStep", 7);
					trQuestVarSet("pvpTime", trTime() + 1);
				}
			}
			case 7:
			{
				reselectMyself();
				uiLookAtUnitByName(""+1*trQuestVarGet("nickonhawk"));
				xsEnableRule("pvp_battle");
				trQuestVarSet("pvpStep", 8);
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
			}
			case 8:
			{
				/*
				burn players
				*/
				if (trTime() - trQuestVarGet("pvpTime") > 30) {
					trQuestVarSet("pvpTime", trTime());
					trQuestVarSet("burnNext", trTime());
					trQuestVarSet("burnDamage", 1);
					trQuestVarSet("burnTimeout", trTime() + 10);
					trQuestVarSet("pvpStep", 9);
					trMessageSetText("Players have started burning!", -1);
				}
			}
			case 10:
			{
				trLetterBox(true);
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					trUnitDestroy();
				}
				xResetDatabase(dPlayerUnits);
				xResetDatabase(dEnemies);
				trQuestVarSet("pvpStep", 0);
				trCounterAbort("well");
				trCounterAbort("lure");
				trCounterAbort("rain");
			}
			case 11:
			{
				trQuestVarSet("pvpTime", trTime() + 4);
				trQuestVarSet("pvpStep", 12);
				trSoundPlayFN("","1",-1,"Nickonhawk:It's time for the final round!","icons\hero g odysseus icon 64");
			}
			case 12:
			{
				trQuestVarSet("pvpTime", trTime() + 7);
				trQuestVarSet("pvpStep", 13);
				trSoundPlayFN("","1",-1,
					"Nickonhawk:This is a special round! You won't lose relics this round, so go all out!",
					"icons\hero g odysseus icon 64");
			}
			case 13:
			{
				trQuestVarSet("pvpTime", trTime() + 6);
				trQuestVarSet("pvpStep", 14);
				trSoundPlayFN("","1",-1,
					"Nickonhawk:And now... the reward for winning this round...",
					"icons\hero g odysseus icon 64");
			}
			case 14:
			{
				gadgetUnreal("ShowImageBox-CloseButton");
				trShowImageDialog(boonIcon(BOON_TWO_RELICS), boonName(BOON_TWO_RELICS));
				trQuestVarSet("pvpTime", trTime() + 8);
				trQuestVarSet("pvpStep", 3);
				trSoundPlayFN("market.wav","1",-1,"","");
				trQuestVarSetFromRand("pvpReward",20,25,true);
				trQuestVarSet("pvpCount", 1);
			}
			case 20:
			{
				trQuestVarSet("pvpTime", trTime() + 7);
				trQuestVarSet("pvpStep", 21);
				trSoundPlayFN("","1",-1,"Nickonhawk:Alas, the battles are over. It is time for you to collect your rewards!",
					"icons\hero g odysseus icon 64");
			}
			case 21:
			{
				trUIFadeToColor(0,0,0,1000,0,false);
				for(p=1; < ENEMY_PLAYER) {
					if (trQuestVarGet("p"+p+"rideLightning") == 1) {
						trQuestVarSet("p"+p+"rideLightning", 0);
						xClearDatabase(1*trQuestVarGet("p"+p+"lightningBalls"));
					}
					xSetBool(dPlayerData, xPlayerLaunched, false, p);
					trPlayerKillAllGodPowers(p);
					class = xGetInt(dPlayerData, xPlayerClass, p);
					xSetInt(dPlayerData, xPlayerUnit, trGetNextUnitScenarioNameNumber(), p);
					pos = trVectorQuestVarGet("p"+p+"square");
					trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),45,true);
					trArmySelect(""+p+",0");
					trMutateSelected(xGetInt(dClass, xClassProto, class));
					xAddDatabaseBlock(dPlayerCharacters, true);
					xSetInt(dPlayerCharacters, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
					equipRelicsAgain(p);
					if (trQuestVarGet("p"+p+"nickQuestProgress") == 6) {
						trQuestVarSet("p"+p+"nickQuestProgress", 5);
						trArmyDispatch(""+p+",0","Villager Atlantean Hero",1,
							xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
						trModifyProtounit("Villager Atlantean Hero", p, 5, 2);
						if (trCurrentPlayer() == p) {
							trQuestVarSet("dreamGogglesCount", trQuestVarGet("dreamGogglesCount") - 1);
						}
					}
					uiLookAtUnitByName(""+xGetInt(dPlayerData, xPlayerUnit, trCurrentPlayer()));
				}
				trQuestVarSet("pvpStep", 22);
				trQuestVarSet("pvpTime", trTime() + 6);
				trSoundPlayFN("","1",-1,
					"Nickonhawk:You can now bring home any relics you can carry. Enter the Sky Passage to finish.",
					"icons\hero g odysseus icon 64");
			}
			case 22:
			{
				trLetterBox(false);
				xsDisableSelf();
				if (trPlayerUnitCountSpecific(trCurrentPlayer(), "Villager Atlantean Hero") == 1) {
					trMessageSetText("Your Golden Ticket has been consumed to get a Relic Transporter that can hold 5 relics.", -1);
				}
				for(p=1; < ENEMY_PLAYER) {
					xAddDatabaseBlock(dPlayerUnits, true);
					xSetInt(dPlayerUnits, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
					xSetInt(dPlayerUnits, xPlayerOwner, p);
				}
				xsEnableRule("pvp_save");
				xsEnableRule("pvp_peace");
			}
		}
	}
}

rule pvp_peace
inactive
highFrequency
{
	processFreeRelics(10);
	trQuestVarSet("relicPlayer", 1 + trQuestVarGet("relicPlayer"));
	if (trQuestVarGet("relicPlayer") >= ENEMY_PLAYER) {
		trQuestVarSet("relicPlayer", 1);
	}
	int p = trQuestVarGet("relicPlayer");
	int id = 0;
	int relics = getRelicsDB(p);
	vector pos = kbGetBlockPosition(""+xGetInt(dPlayerData, xPlayerUnit), true);
	trVectorSetUnitPos("pos", "p"+p+"unit");
	for(x=xGetDatabaseCount(relics); >0) {
		xDatabaseNext(relics);
		xUnitSelect(relics, xUnitName);
		if (trUnitGetIsContained("Unit") == false) {
			xUnitSelect(relics, xUnitName);
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("backtowork.wav","1",-1,"","");
				trChatSend(0, relicName(xGetInt(relics, xRelicType)) + " dropped.");
			}
			relicEffect(xGetInt(relics, xRelicType), p, false);
			
			trUnitChangeProtoUnit("Relic");
			xAddDatabaseBlock(dFreeRelics, true);
			xSetInt(dFreeRelics, xUnitName, xGetInt(relics, xUnitName));
			xSetInt(dFreeRelics, xRelicType, xGetInt(relics, xRelicType));
			
			xFreeDatabaseBlock(relics);
		}
	}
	trUnitSelectClear();
	trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
	if (trUnitGetIsContained("Sky Passage")) {
		if (xGetDatabaseCount(relics) == 0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"passage");
			trUnitEjectContained();
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				trChatSend(0, "<color=1,1,1>You must have at least one relic equipped to enter!</color>");
			}
		}
	}
	/* immortal sky passage */
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"passage");
	if (trUnitAlive() == false) {
		trUnitChangeProtoUnit("Automaton");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"passage");
		trDamageUnitPercent(-100);
		trMutateSelected(kbGetProtoUnitID("Sky Passage"));
	}
	/* my relics */
	trUnitSelectClear();
	if (xGetDatabaseCount(dRelicDescriptors) > 0) {
		xDatabaseNext(dRelicDescriptors);
		xUnitSelect(dRelicDescriptors, xUnitName);
		if (trUnitIsSelected()) {
			if (trQuestVarGet("selectedDescriptor") != xGetInt(dRelicDescriptors, xUnitName)) {
				trStringQuestVarSet("description", relicName(xGetInt(dRelicDescriptors,xRelicType)) + ":Count: ");
				trSoundPlayFN("","1",-1,
					trStringQuestVarGet("description") + 1*trQuestVarGet("myRelics"+xGetInt(dRelicDescriptors,xRelicType)),"");
				trQuestVarSet("selectedDescriptor", xGetInt(dRelicDescriptors, xUnitName));
			}
		} else if (trQuestVarGet("selectedDescriptor") == xGetInt(dRelicDescriptors, xUnitName)) {
			trQuestVarSet("selectedDescriptor",0);
			trLetterBox(false);
		}
	}
	trUnitSelectClear();
}

rule pvp_battle
inactive
highFrequency
{
	int old = xsGetContextPlayer();
	int p = 0;
	int id = 0;
	int relics = 0;
	pvpDetached = false;
	for (i=xGetDatabaseCount(dPlayerUnits); > 0) {
		xDatabaseNext(dPlayerUnits);
		xUnitSelectByID(dPlayerUnits, xUnitID);
		if (trUnitAlive() == false) {
			removePlayerUnit();
		} else {
			if (xGetFloat(dPlayerUnits, xDecay) > 0) {
				if (trTimeMS() > xGetInt(dPlayerUnits, xDecayNext)) {
					xSetInt(dPlayerUnits, xDecayNext, 1000 + xGetInt(dPlayerUnits, xDecayNext));
					trDamageUnitPercent(xGetFloat(dPlayerUnits, xDecay));
				}
			}
			xSetVector(dPlayerUnits, xUnitPos, kbGetBlockPosition(""+xGetInt(dPlayerUnits,xUnitName), true));
			
			stunsAndPoisons(dPlayerUnits);
			if (xSetPointer(dEnemies, xGetInt(dPlayerUnits, xDoppelganger)) == false) {
				debugLog("playerUnit " + xGetInt(dPlayerUnits, xUnitName) + " is missing a doppelganger!");
				xSetInt(dPlayerUnits, xDoppelganger, xAddDatabaseBlock(dEnemies, true));
			}
			xSetInt(dEnemies, xUnitName, xGetInt(dPlayerUnits, xUnitName));
			xSetInt(dEnemies, xPlayerOwner, xGetInt(dPlayerUnits, xPlayerOwner));
			xSetInt(dEnemies, xUnitID, xGetInt(dPlayerUnits, xUnitID));
			xSetInt(dEnemies, xStunStatus, xGetInt(dPlayerUnits, xStunStatus));
			xSetInt(dEnemies, xPoisonStatus, xGetInt(dPlayerUnits, xPoisonStatus));
			xSetInt(dEnemies, xSilenceStatus, xGetInt(dPlayerUnits, xSilenceStatus));
			xSetInt(dEnemies, xDoppelganger, xGetPointer(dPlayerUnits));
			xSetFloat(dEnemies, xPhysicalResist, xGetFloat(dPlayerUnits, xPhysicalResist));
			xSetFloat(dEnemies, xMagicResist, xGetFloat(dPlayerUnits, xMagicResist));

			if (trQuestVarGet("pvpStep") == 9) {
				if (trTime() > trQuestVarGet("burnNext")) {
					trDamageUnitPercent(trQuestVarGet("burnDamage"));
				}
			}
		}
	}

	if (trQuestVarGet("pvpStep") == 9) {
		trQuestVarSet("burnNext", trTime());
		if (trTime() > trQuestVarGet("burnTimeout")) {
			trQuestVarSet("burnTimeout", trTime() + 10);
			trQuestVarSet("burnDamage", trQuestVarGet("burnDamage") + 1);
		}
	}
	
	maintainStun();
	
	playerLasers();
	
	processWolves();
	
	/* sounds */
	if (trQuestVarGet("stunSound") == 1) {
		trQuestVarSet("stunSound", 0);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("woodcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
	}
	
	if (trQuestVarGet("poisonSound") == 1) {
		trQuestVarSet("poisonSound", 0);
		trSoundPlayFN("lampadesblood.wav","1",-1,"","");
		trSoundPlayFN("carnivorabirth.wav","1",-1,"","");
	}
	
	/* misc */
	for(x=xGetDatabaseCount(dParticipants); > 0) {
		xDatabaseNext(dParticipants);
		p = xGetInt(dParticipants, xParticipantPlayer);
		relics = getRelicsDB(p);
		if (xGetDatabaseCount(dParticipants) == 1) {
			for(y=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				relicEffect(xGetInt(relics, xRelicType), p, false);
			}
			xClearDatabase(relics);
			xClearDatabase(getCharactersDB(p));
			trSoundPlayFN("favordump.wav","1",-1,"","");
			trUIFadeToColor(0,0,0,1000,1000,true);
			xsDisableRule("pvp_battle");
			for(y=trQuestVarGet("pvpCount"); >0) {
				pvpGetRelic(p, 1*trQuestVarGet("pvpReward"));
			}
			trQuestVarSet("pvpStep", 10);
			trQuestVarSet("pvpTime", trTime() + 2);
		} else if (xGetInt(dPlayerData, xPlayerDead, p) > 0) {
			for(y=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				relicEffect(xGetInt(relics, xRelicType), p, false);
			}
			xClearDatabase(relics);
			xClearDatabase(getCharactersDB(p));
			
			xFreeDatabaseBlock(dParticipants);
			trSoundPlayFN("townbell.wav","1",-1,"","");
			trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has been knocked out!");
			for(y=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitIsOwnedBy(p)) {
					trUnitDelete(false);
					removePlayerUnit();
				}
			}
		}
	}
	if (xGetDatabaseCount(dParticipants) > 1) {
		for(x=xGetDatabaseCount(dParticipants); >0) {
			xDatabaseNext(dParticipants);
			p = xGetInt(dParticipants, xParticipantPlayer);
			checkGodPowers(p);
			checkResourceCheating(p);
			
			trUnitSelectClear();
			trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
			processLifesteal(p);
			processSilence(p);
			processRegen(p);
			petDogs(p);
			trEventFire(12 * xGetInt(dPlayerData, xPlayerClass, p) + p);
		}
	}
	
	
	for(x=xsMin(5, xGetDatabaseCount(dLaunchedUnits)); >0) {
		processLaunchedUnit();
	}
	
	processWolves();
	
	/* No specials */
	noSpecials();
	
	pvpDetached = true;
}


rule pvp_save
inactive
highFrequency
{
	int p = trCurrentPlayer();
	trUnitSelectClear();
	trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, p), true);
	if (trUnitGetIsContained("Sky Passage")) {
		trUIFadeToColor(255,255,255,1000,0,true);
		saveAllData();
		xsEnableRule("pvp_exit");
		xsDisableSelf();
	}
}

rule pvp_exit
inactive
highFrequency
{
	if (trTime() > 1 + cActivationTime) {
		subModeEnter("Simulation","Editor");
		uiMessageBox("","leaveGame()");
		uiCycleCurrentActivate();
		uiCycleCurrentActivate();
		subModeLeave("Simulation","Editor");
		modeEnter("pregame");
		modeEnter("Simulation");
	}
}
