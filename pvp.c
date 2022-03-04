void detachFromDatabase(string db = "", int p = 0, int start = 0) {
	int end = 0;
	if (ySetPointer(db, start)) {
		for(x=yGetDatabaseCount(db); >0) {
			yDatabaseNext(db);
			if (yGetVar(db, "player") != p) {
				yDatabaseNext(db, false, true);
				end = yGetPointer(db);
				trQuestVarSet(db + "DummyIndex", yAddToDatabase(db, "immortalDummy"));
				break;
			}
		}
		if (end == 0) {
			debugLog("immortalDummy could not be inserted for player " + p);
		} else {
			/* inserting the dummy unit */
			ySetVarAtIndex(db, "xNextBlock",
				trQuestVarGet(db + "DummyIndex"), 1*yGetVarAtIndex(db,"xPrevBlock",start));
			ySetVarAtIndex(db, "xPrevBlock",
				1*yGetVarAtIndex(db,"xPrevBlock",start), 1*trQuestVarGet(db + "DummyIndex"));
			/* reconnecting the loose ends */
			ySetVarAtIndex(db, "xPrevBlock", end, start);
			ySetVarAtIndex(db, "xNextBlock", start, end);
		}
	}
}

void reAttachToDatabase(string db = "", int p = 0, int start = 0) {
	int end = 0;
	int insertNext = 0;
	int insertPrev = 0;
	/* grab the dummy index */
	if (ySetPointer(db, 1*trQuestVarGet(db+"DummyIndex"))) {
		insertNext = yGetVar(db, "xNextBlock");
		insertPrev = yGetVar(db, "xPrevBlock");
		yRemoveFromDatabase(db);
	}
	if (ySetPointer(db, start)) {
		end = yGetVarAtIndex(db, "xPrevBlock", start);
		/* connect my start to the insert end */
		ySetVarAtIndex(db, "xPrevBlock", insertPrev, start);
		ySetVarAtIndex(db, "xNextBlock", start, insertPrev);
		/* connect my end to the insert start */
		ySetVarAtIndex(db, "xNextBlock", insertNext, end);
		ySetVarAtIndex(db, "xPrevBlock", end, insertNext);
	}
}

void detachPlayer(int eventID = -1) {
	int p = eventID - 10000;
	if (ySetPointer("playerUnits", 1*trQuestVarGet("p"+p+"index"))) {
		detachFromDatabase("playerUnits", p, 1*trQuestVarGet("p"+p+"index"));
		detachFromDatabase("enemies", p, 1*yGetVarAtIndex("playerUnits", "doppelganger", 1*trQuestVarGet("p"+p+"index")));
		ySetPointer("playerUnits", 1*trQuestVarGet("p"+p+"index"));
		ySetPointer("enemies", 1*trQuestVarGet("enemiesDummyIndex"));
	}
	trQuestVarSet("protectionCount", trQuestVarGet("p"+p+"protection"));
}

void reAttachPlayer(int eventID = -1) {
	int p = eventID - 10100;
	if (ySetPointer("playerUnits", 1*trQuestVarGet("p"+p+"index"))) {
		reAttachToDatabase("playerUnits", p, 1*trQuestVarGet("p"+p+"index"));
		reAttachToDatabase("enemies", p, 1*yGetVarAtIndex("playerUnits", "doppelganger", 1*trQuestVarGet("p"+p+"index")));
		/* move pointers backwards one to maintain partitions */
		yDatabaseNext("playerUnits", false, true);
		yDatabaseNext("enemies", false, true);
	}
}

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

void pvpGetRelic(int p = 0, int relic = 0) {
	trQuestVarSet("posx", trQuestVarGet("p"+p+"squareX")-12);
	trQuestVarSet("posz", trQuestVarGet("p"+p+"squareZ")-12);
	if (relic <= 10) {
		trQuestVarSet("posx", trQuestVarGet("posx") + 2 * (relic - 1));
		trQuestVarSet("posz", trQuestVarGet("posz") + 24);
	} else {
		trQuestVarSet("posx", trQuestVarGet("posx") + 24);
		trQuestVarSet("posz", trQuestVarGet("posz") + 2 * (relic - 11));
	}
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
	trArmySelect("1,0");
	trUnitChangeProtoUnit("Relic");
	yAddToDatabase("freeRelics", "next");
	yAddUpdateVar("freeRelics", "type", relic);
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
	trArmyDispatch(""+p+",0","Dwarf",1,x+12,0,z+12,0,true);
	trArmySelect(""+p+",0");
	trUnitChangeProtoUnit("Revealer To Player");
	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
	for(i=0; <10) {
		trArmyDispatch("1,0","Dwarf",1,x+2*i,0,z+26,180,true);
		trArmySelect("1,0");
		trUnitConvert(0);
		trMutateSelected(relicProto(i+1));
		trUnitSetStance("Passive");
		trUnitOverrideAnimation(2,0,true,false,-1);
		trSetSelectedScale(0.5,0.5,0.5);
		pvpGetRelic(p, i + 1);
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
	spawnPlayer(p, "p"+p+"square");
}

rule pvp_build_map
inactive
highFrequency
{
	if (trTime() > cActivationTime + 1){
		PvP = true;
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
			for(x=p+1; < ENEMY_PLAYER) {
				trPlayerSetDiplomacy(p, x, "Enemy");
				trPlayerSetDiplomacy(x, p, "Enemy");
			}
		}
		
		trQuestVarSet("boxCenter", 66);
		trVectorQuestVarSet("dir", vector(-52,0,-52));
		
		p = 1;
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
		trQuestVarSet("cinTime", trTime());
		trQuestVarSet("cinStep", 0);
		xsEnableRule("pvp_cinematic");
		xsDisableSelf();
		
		trPaintTerrain(0,0,5,5,0,70,true);
		trPaintTerrain(0,0,5,5,2,2,false);
		
		for(x=16; >0) {
			yDatabaseNext("PharaohStatues", true);
			trMutateSelected(kbGetProtoUnitID("Statue Pharaoh"));
		}
		yClearDatabase("PharaohStatues");
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
					"Nickonhawk: First, I have replaced your relics with ten basic ones here.",
					"icons\hero g odysseus icon 64");
				trQuestVarSet("cinTime", trTime() + 5);
				uiLookAtUnitByName(""+1*trQuestVarGet("p"+p+"unit"));
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
				trLetterBox(false);
				trUIFadeToColor(0,0,0,1000,0,false);
				
				trQuestVarSet("immortalDummy", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,1,0,1,0,true);
				trArmySelect("1,0");
				trUnitConvert(0);
				trUnitChangeProtoUnit("Victory Marker");
				
				for(p=1; < ENEMY_PLAYER) {
					trEventSetHandler(10000 + p, "detachPlayer");
					trEventSetHandler(10100 + p, "reAttachPlayer");
				}
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
	trUnitSelectClear();
	trUnitSelectByQV("p"+p+"unit");
	trVectorSetUnitPos("pos", "p"+p+"unit");
	for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
		id = yDatabaseNext("p"+p+"relics", true);
		if (trUnitGetIsContained("Unit") == false) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"relics");
			if (trCurrentPlayer() == p) {
				trSoundPlayFN("backtowork.wav","1",-1,"","");
				trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")) + " dropped.");
			}
			relicEffect(1*yGetVar("p"+p+"relics", "type"), p, false);
			
			trUnitChangeProtoUnit("Relic");
			yAddToDatabase("freeRelics", "p"+p+"relics");
			yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"relics", "type"));
			
			yRemoveFromDatabase("p"+p+"relics");
			yRemoveUpdateVar("p"+p+"relics", "type");
		}
	}
}

rule pvp_battle
inactive
highFrequency
{
	int old = xsGetContextPlayer();
	int p = 0;
	int id = 0;
	
	if (yGetDatabaseCount("playerUnits") > 0) {
		id = yDatabaseNext("playerUnits", true);
		if ((id == -1) || (trUnitAlive() == false)) {
			removePlayerUnit();
		} else {
			if (yGetVar("playerUnits", "decay") > 0) {
				if (trTimeMS() > yGetVar("playerUnits", "decayNext")) {
					ySetVar("playerUnits", "decayNext", 1000 + yGetVar("playerUnits", "decayNext"));
					trDamageUnitPercent(yGetVar("playerUnits", "decay"));
				}
			}
			trVectorSetUnitPos("pos", "playerUnits");
			ySetVarFromVector("playerUnits", "pos", "pos");
			
			stunsAndPoisons("playerUnits");
			id = yGetPointer("enemies");
			ySetPointer("enemies", yGetVar("playerUnits", "doppelganger"));
			ySetVar("enemies", "physicalResist", yGetVar("playerUnits", "physicalResist"));
			ySetVar("enemies", "magicResist", yGetVar("playerUnits", "magicResist"));
			ySetVar("enemies", "stunStatus", yGetVar("playerUnits", "stunStatus"));
			ySetVar("enemies", "poisonStatus", yGetVar("playerUnits", "poisonStatus"));
		}
	}
	
	maintainStun();
	
	playerLasers();
	
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
	for(x=yGetDatabaseCount("participants"); > 0) {
		p = yDatabaseNext("participants");
		checkGodPowers(p);
		checkResourceCheating(p);
		if (trQuestVarGet("p"+p+"dead") > 0) {
			yAddToDatabase("deadParticipants", "participants");
			yRemoveFromDatabase("participants");
		} else {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"unit");
			if (SAVIOR != trQuestVarGet("p"+p+"class")) {
				fixAnimations(p);
			}
			processLifesteal(p);
			processSilence(p);
			processRegen(p);
			trEventFire(10000+p);
			trEventFire(12*trQuestVarGet("p"+p+"class") + p);
			trEventFire(10100+p);
		}
	}
	
	for(x=xsMin(5, yGetDatabaseCount("launchedUnits")); >0) {
		processLaunchedUnit();
	}
	
	if (yGetDatabaseCount("decayingWolves") > 0) {
		yDatabaseNext("decayingWolves");
		if (trTimeMS() > yGetVar("decayingWolves", "timeout")) {
			trUnitSelectClear();
			trUnitSelectByQV("decayingWolves", true);
			trUnitChangeProtoUnit("Dust Small");
			yRemoveFromDatabase("decayingWolves");
		}
	}
	
	/* No specials */
	noSpecials();
	
	xsSetContextPlayer(old);
}
