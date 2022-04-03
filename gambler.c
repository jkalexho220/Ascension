const int DICE_COPY = 1;
const int DICE_EXPLODE = 2;
const int DICE_HEAL = 3;

const int DECK_BURN = 1;
const int DECK_STUN = 2;
const int DECK_RELICS = 3;

void removeGambler(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "animating");
}

void gamblerAlways(int eventID = -1) {
	int p = eventID - 12 * GAMBLER;
	int id = 0;
	int action = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeGambler(p);
		} else if (yGetVar("p"+p+"characters", "animating") == 1) {
			if (trTimeMS() > yGetVar("p"+p+"characters", "timeout")) {
				trUnitOverrideAnimation(-1, 0, false, true, -1);
				ySetVar("p"+p+"characters", "animating", 0);
			}
		} else {
			switch(1*yGetVar("p"+p+"characters","spy"))
			{
				case 0:
				{
					if (yGetVarAtIndex("playerUnits","stunSFX",1*yGetVar("p"+p+"characters","index")) == 0) {
						ySetPointer("playerUnits",1*yGetVar("p"+p+"characters","index"));
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"characters");
						trMutateSelected(kbGetProtoUnitID("Hypaspist"));
						spyEffect(1*trQuestVarGet("p"+p+"characters"),kbGetProtoUnitID("Cinematic Block"),yGetVarName("playerUnits","stunSFX"));
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
							kbGetProtoUnitID("Cinematic Block"),yGetVarName("playerUnits","poisonSFX"));
						spyEffect(1*trQuestVarGet("p"+p+"characters"),
							kbGetProtoUnitID("Cinematic Block"),yGetVarName("playerUnits","silenceSFX"));
						ySetVar("p"+p+"characters","spy", 1);
					}
				}
				case 1:
				{
					if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"characters");
						trMutateSelected(kbGetProtoUnitID("Regent"));
						ySetVar("p"+p+"characters","spy", 2);
					}
				}
			}
		}
	}
	
	switch(1*trQuestVarGet("p"+p+"gambleSpy"))
	{
		case 0:
		{
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"unit");
			if (trUnitAlive()) {
				if ((trQuestVarGet("p"+p+"numberFlag") == 0) || (kbGetBlockID(""+1*trQuestVarGet("p"+p+"numberFlag")) == -1)) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"unit");
					trMutateSelected(kbGetProtoUnitID("Hypaspist"));
					spyEffect(1*trQuestVarGet("p"+p+"unit"),kbGetProtoUnitID("Cinematic Block"),"p"+p+"numberFlag");
					spyEffect(1*trQuestVarGet("p"+p+"unit"),kbGetProtoUnitID("Cinematic Block"),"p"+p+"gambleSFX");
					trQuestVarSet("p"+p+"gambleSpy", 1);
				}
			}
		}
		case 1:
		{
			if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"unit");
				trMutateSelected(kbGetProtoUnitID("Regent"));
				trQuestVarSet("p"+p+"gambleSpy", 2);
			}
		}
		case 2:
		{
			if ((trQuestVarGet("p"+p+"numberFlag") != 0) && (kbGetBlockID(""+1*trQuestVarGet("p"+p+"numberFlag")) != -1)) {
				trQuestVarSet("p"+p+"gambleSpy", 0);
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"dice") > 0) {
		yDatabaseNext("p"+p+"dice");
		yVarToVector("p"+p+"dice", "prev");
		trVectorSetUnitPos("pos", "p"+p+"dice");
		dist = zDistanceBetweenVectors("pos", "prev");
		if (dist > 2.0) {
			yVarToVector("p"+p+"dice", "dir");
			if (yGetVar("p"+p+"dice", "type") == DICE_HEAL) {
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					if (yDatabaseNext("playerUnits",true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (rayCollision("playerUnits","prev","dir",dist + 2.0,4.0)) {
						healUnit(p, dist * trQuestVarGet("diceDamage") * 0.25 * trQuestVarGet("p"+p+"spellDamage"));
						gainFavor(p, dist);
					}
				}
			} else {
				for(x=xGetDatabaseCount(dEnemies); >0) {
					id = yDatabaseNext("enemies", true);
					if (id == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision("enemies","prev","dir",dist + 2.0, 4.0)) {
						hit = 1;
						break;
					}
				}
			}
			
			vectorToGrid("pos", "loc");
			if (hit == 1) {
				gainFavor(p, 3.0);
				switch(1*yGetVar("p"+p+"dice", "type"))
				{
					case DICE_COPY:
					{
						if (trQuestVarGet("bossUnit") != trQuestVarGet("enemies")) {
							id = kbGetUnitBaseTypeID(id);
							trSoundPlayFN("favordump.wav","1",-1,"","");
							trSoundPlayFN("changeunit.wav","1",-1,"","");
							for(x=yGetVar("p"+p+"dice","count"); >0) {
								trQuestVarSetFromRand("heading",1,360,true);
								trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
								trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),trQuestVarGet("heading"),true);
								trUnitSelectClear();
								trUnitSelectByQV("next");
								trMutateSelected(id);
								activatePlayerUnit("next", p, id, calculateDecay(p, 4.0));
							}
							trUnitSelectClear();
							trUnitSelect(""+1*yGetVar("p"+p+"dice", "dice"), true);
							trUnitChangeProtoUnit("Osiris Box Glow");
							trUnitSelectClear();
							trUnitSelect(""+1*yGetVar("p"+p+"dice", "sfx"), true);
							trUnitChangeProtoUnit("Tremor");
							trUnitSelectClear();
							trUnitSelectByQV("p"+p+"dice");
							trUnitChangeProtoUnit("Dust Large");
						}
					}
					case DICE_EXPLODE:
					{
						trSoundPlayFN("favordump.wav","1",-1,"","");
						trSoundPlayFN("cinematics\32_out\explosion.mp3","1",-1,"","");
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"dice", "dice"), true);
						trUnitChangeProtoUnit("Meteor Impact Ground");
						trUnitSelectClear();
						trUnitSelect(""+1*yGetVar("p"+p+"dice", "sfx"), true);
						trUnitChangeProtoUnit("Qilin Heal");
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"dice");
						trDamageUnitPercent(100);
						trUnitChangeProtoUnit("Meteorite");
						
						dist = xsPow(trQuestVarGet("diceRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
						amt = trQuestVarGet("diceDamage") * trQuestVarGet("p"+p+"spellDamage");
						for(x=xGetDatabaseCount(dEnemies); >0) {
							if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
								removeEnemy();
							} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
								damageEnemy(p, amt);
							}
						}
					}
				}
				yRemoveFromDatabase("p"+p+"dice");
			} else if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"dice", "dice"), true);
				trUnitChangeProtoUnit("Dust Small");
				trUnitSelectClear();
				trUnitSelect(""+1*yGetVar("p"+p+"dice", "sfx"), true);
				trUnitChangeProtoUnit("Dust Small");
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"dice");
				trUnitChangeProtoUnit("Dust Small");
				yRemoveFromDatabase("p"+p+"dice");
			} else {
				ySetVarFromVector("p"+p+"dice","prev","pos");
				trQuestVarSetFromRand("sound", 1, 5, true);
				if (trQuestVarGet("sound") <= 3) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"dice");
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("mine"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					}
				}
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trQuestVarSetFromRand("sound", 1, 3, true);
		trSoundPlayFN("swing"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		amt = xsSqrt(trQuestVarGet("p"+p+"gamble")) * 0.5;
		trModifyProtounit("Hero Greek Achilles", p, 5, 2);
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			if (yDatabaseNext("p"+p+"characters",true) == -1 || trUnitAlive() == false) {
				removeGambler(p);
			} else {
				trVectorSetUnitPos("start", "p"+p+"characters");
				trVectorQuestVarSet("dir", zGetUnitVector("start","p"+p+"wellPos"));
				trMutateSelected(kbGetProtoUnitID("Regent"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trUnitOverrideAnimation(1,0,false,false,-1);
				ySetVar("p"+p+"characters", "animating", 1);
				ySetVar("p"+p+"characters", "timeout", trTimeMS() + 1100);
				
				vectorSetAsTargetVector("target","start","p"+p+"wellPos", 300.0);
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("startx"),0,trQuestVarGet("startz"),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				
				trQuestVarSet("dice", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trQuestVarSet("sfx", trGetNextUnitScenarioNameNumber());
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelectByQV("dice");
				trUnitChangeProtoUnit("Revealer");
				trUnitSelectClear();
				trUnitSelectByQV("dice");
				trUnitSetAnimationPath("1,0,0,0,0,0,0");
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trMutateSelected(kbGetProtoUnitID("Revealer"));
				trSetSelectedScale(amt,amt,amt);
				trUnitSelectClear();
				trUnitSelectByQV("sfx");
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+1*trQuestVarGet("next"));
				trUnitSelectClear();
				trUnitSelectByQV("sfx");
				switch(1*trQuestVarGet("p"+p+"diceType"))
				{
					case DICE_COPY:
					{
						trMutateSelected(kbGetProtoUnitID("Kronny Birth SFX"));
					}
					case DICE_EXPLODE:
					{
						trMutateSelected(kbGetProtoUnitID("Ball of Fire"));
					}
					case DICE_HEAL:
					{
						trMutateSelected(kbGetProtoUnitID("Curse SFX"));
					}
				}
				
				trUnitSelectClear();
				trUnitSelectByQV("next");
				trMutateSelected(kbGetProtoUnitID("Axe"));
				trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"),-1,false);
				trSetSelectedScale(0.5,0.01,0.5);
				
				yAddToDatabase("p"+p+"dice", "next");
				yAddUpdateVar("p"+p+"dice","prevx",trQuestVarGet("startx"));
				yAddUpdateVar("p"+p+"dice","prevz",trQuestVarGet("startz"));
				yAddUpdateVar("p"+p+"dice", "dice", trQuestVarGet("dice"));
				yAddUpdateVar("p"+p+"dice", "sfx", trQuestVarGet("sfx"));
				yAddUpdateVar("p"+p+"dice", "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar("p"+p+"dice", "dirz", trQuestVarGet("dirz"));
				yAddUpdateVar("p"+p+"dice", "type", trQuestVarGet("p"+p+"diceType"));
				yAddUpdateVar("p"+p+"dice", "count", trQuestVarGet("p"+p+"gamble"));
			}
		}
		trModifyProtounit("Hero Greek Achilles", p, 5, -2);
		trQuestVarSet("p"+p+"gamble", 1);
		if (trQuestVarGet("p"+p+"gambleStep") == 0) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"numberFlag");
			trUnitSetAnimationPath("0,0,0,0,0,0,0");
			trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
		}
		trQuestVarSetFromRand("rand",1,2,true);
		trQuestVarSet("p"+p+"diceType", trQuestVarGet("p"+p+"diceType") + trQuestVarGet("rand"));
		if (trQuestVarGet("p"+p+"diceType") >= 4) {
			trQuestVarSet("p"+p+"diceType", trQuestVarGet("p"+p+"diceType") - 3);
		}
		if (trCurrentPlayer() == p) {
			switch(1*trQuestVarGet("p"+p+"diceType"))
			{
				case DICE_COPY:
				{
					wellName = "(Q) Loaded Dice: COPY";
				}
				case DICE_EXPLODE:
				{
					wellName = "(Q) Loaded Dice: EXPLODE";
				}
				case DICE_HEAL:
				{
					wellName = "(Q) Loaded Dice: HEAL";
				}
			}
			trCounterAbort("well");
			trCounterAddTime("well",
				trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 0, wellName);
		}
	}
	
	if (trQuestVarGet("p"+p+"gambleStep") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"gambleNext")) {
			trQuestVarSet("p"+p+"gambleNext", trQuestVarGet("p"+p+"gambleNext") + 100);
			trQuestVarSetFromRand("rand", 0, 5, true);
			if (kbGetBlockID(""+1*trQuestVarGet("p"+p+"numberFlag")) != -1) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"numberFlag");
				trUnitSetAnimationPath(""+1*trQuestVarGet("rand")+",0,0,0,0,0,0");
			}
			if (trTimeMS() > trQuestVarGet("p"+p+"gambleTimeout")) {
				trQuestVarSet("p"+p+"gambleStep", 0);
				trQuestVarSet("p"+p+"gamble", trQuestVarGet("rand") + 1);
				trSoundPlayFN("favordump.wav","1",-1,"","");
				trUnitHighlight(0.3,false);
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"gambleSFX");
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		gainFavor(p, 0.0 - trQuestVarGet("gambleCost") * trQuestVarGet("p"+p+"ultimateCost"));
		trSoundPlayFN("plentybirth.wav","1",-1,"","");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"unit");
		if (trUnitAlive()) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"numberFlag");
			trMutateSelected(kbGetProtoUnitID("Flag Numbered"));
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"gambleSFX");
			trMutateSelected(kbGetProtoUnitID("Curse SFX"));
		}
		trQuestVarSet("p"+p+"gambleNext", trTimeMS());
		trQuestVarSet("p"+p+"gambleTimeout", trTimeMS() + 1500);
		trQuestVarSet("p"+p+"gambleStep", 1);
	}
	
	if (yGetDatabaseCount("p"+p+"deckBurns") > 0) {
		yDatabaseNext("p"+p+"deckBurns");
		if (trTimeMS() > yGetVar("p"+p+"deckBurns","next")) {
			ySetVar("p"+p+"deckBurns","next", trTimeMS() + 500);
			yVarToVector("p"+p+"deckBurns","pos");
			amt = 2;
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
					removePlayerUnit();
				} else if (zDistanceToVectorSquared("playerUnits", "pos") < yGetVar("p"+p+"deckBurns", "radius")) {
					amt = 1 + amt;
				}
			}
			amt = amt * 0.5 * yGetVar("p"+p+"deckBurns","damage");
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
					removeEnemy();
				} else if (zDistanceToVectorSquared("enemies", "pos") < yGetVar("p"+p+"deckBurns", "radius")) {
					damageEnemy(p, amt);
				}
			}
			if (trTimeMS() > yGetVar("p"+p+"deckBurns", "timeout")) {
				for(x=trQuestVarGet("p"+p+"deckBurns"); < yGetVar("p"+p+"deckBurns","end")) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trUnitDestroy();
				}
				yRemoveFromDatabase("p"+p+"deckBurns");
			}
		}
	}
	
	if (yGetDatabaseCount("p"+p+"tempRelics") > 0) {
		target = yDatabaseNext("p"+p+"tempRelics");
		if (trTimeMS() > yGetVar("p"+p+"tempRelics", "timeout")) {
			relicEffect(1*yGetVar("p"+p+"tempRelics","type"),target,false);
			yRemoveFromDatabase("p"+p+"tempRelics");
		}
	}
	
	for (y=xsMin(3, yGetDatabaseCount("p"+p+"cards")); > 0) {
		action = processGenericProj("p"+p+"cards");
		if (action == PROJ_BOUNCE) {
			trSetSelectedScale(0.5,0.1,0.8);
		}
		if (trTimeMS() > yGetVar("p"+p+"cards", "timeout")) {
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"cards");
			trUnitChangeProtoUnit("Fimbulwinter SFX");
			trUnitSelectClear();
			trUnitSelectByQV("p"+p+"cards");
			trDamageUnitPercent(-100);
			switch(1*yGetVar("p"+p+"cards","type"))
			{
				case DECK_BURN:
				{
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					yVarToVector("p"+p+"cards", "target");
					dist = trQuestVarGet("deckRadius") * trQuestVarGet("p"+p+"spellRange");
					trVectorQuestVarSet("dir", xsVectorSet(dist,0,0));
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					for(x=16; >0) {
						trQuestVarSet("posx",trQuestVarGet("targetx") + trQuestVarGet("dirx"));
						trQuestVarSet("posz",trQuestVarGet("targetz") + trQuestVarGet("dirz"));
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
						trArmySelect("1,0");
						trMutateSelected(kbGetProtoUnitID("Hades Fire"));
						trUnitSetAnimationPath("1,1,0,0,0,0,0");
						trVectorQuestVarSet("dir", rotationMatrix("dir", 0.923879, 0.382683));
					}
					yAddToDatabase("p"+p+"deckBurns","next");
					yAddUpdateVar("p"+p+"deckBurns","end",trGetNextUnitScenarioNameNumber());
					yAddUpdateVar("p"+p+"deckBurns","radius", xsPow(dist, 2));
					yAddUpdateVar("p"+p+"deckBurns", "posx", trQuestVarGet("targetx"));
					yAddUpdateVar("p"+p+"deckBurns", "posz", trQuestVarGet("targetz"));
					yAddUpdateVar("p"+p+"deckBurns", "damage",
						trQuestVarGet("deckDamage") * trQuestVarGet("p"+p+"spellDamage") * yGetVar("p"+p+"cards","count"));
					yAddUpdateVar("p"+p+"deckBurns", "next", trTimeMS());
					yAddUpdateVar("p"+p+"deckBurns", "timeout",
						trTimeMS() + 1000 * trQuestVarGet("deckDuration") * trQuestVarGet("p"+p+"spellDuration"));
				}
				case DECK_STUN:
				{
					trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
					yVarToVector("p"+p+"cards","target");
					dist = trQuestVarGet("deckRadius") * trQuestVarGet("p"+p+"spellRange");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("targetx"),0,trQuestVarGet("targetz"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next");
					trUnitChangeProtoUnit("Rocket");
					trUnitSelectClear();
					trUnitSelectByQV("next");
					trMutateSelected(kbGetProtoUnitID("Frost Drift"));
					trSetSelectedScale(0.3 * dist,1,0.4*dist);
					dist = xsPow(dist, 2);
					for(x=xGetDatabaseCount(dEnemies); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies","p"+p+"decktarget") < dist) {
							stunUnit("enemies",2.5,p,false);
						}
					}
				}
				case DECK_RELICS:
				{
					trSoundPlayFN("researchcomplete.wav","1",-1,"","");
					trSoundPlayFN("ageadvance.wav","1",-1,"","");
					target = yGetVar("p"+p+"cards","p"+p+"decktarget");
					trQuestVarSet("player", target);
					hit = trTimeMS() + trQuestVarGet("deckDuration") * trQuestVarGet("p"+p+"spellDuration");
					for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
						if (yGetVar("p"+p+"relics","type") <= NORMAL_RELICS) {
							yAddToDatabase("p"+p+"tempRelics", "player");
							yAddUpdateVar("p"+p+"tempRelics", "type", yGetVar("p"+p+"relics","type"));
							yAddUpdateVar("p"+p+"tempRelics", "timeout", hit);
							relicEffect(1*yGetVar("p"+p+"relics","type"),target,true);
						}
					}
					if (trCurrentPlayer() == target) {
						trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has given you all their relics!");
					}
				}
			}
			yRemoveFromDatabase("p"+p+"cards");
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trVectorSetUnitPos("p"+p+"decktarget", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject");
		trUnitDestroy();
		hit = 0;
		if (trQuestVarGet("p"+p+"deckType") == DECK_RELICS) {
			dist = 100;
			for(x=yGetDatabaseCount("playerCharacters"); >0) {
				if (yDatabaseNext("playerCharacters", true) == -1 || trUnitAlive() == false) {
					removePlayerCharacter();
				} else if (yGetVar("playerCharacters", "player") != p) {
					current = zDistanceToVectorSquared("playerCharacters", "p"+p+"decktarget");
					if (current < dist) {
						hit = trQuestVarGet("playerCharacters");
						target = yGetVar("playerCharacters", "player");
						dist = current;
					}
				}
			}
			if (hit > 0) {
				trVectorQuestVarSet("p"+p+"decktarget", kbGetBlockPosition(""+hit, true));
			}
		} else {
			hit = 1;
			vectorSnapToGrid("p"+p+"decktarget");
		}
		
		if (hit > 0) {
			gainFavor(p, 5.0);
			trSoundPlayFN("ui\scroll.wav","1",-1,"","");
			trSoundPlayFN("skypassagein.wav","1",-1,"","");
			
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters",true) == -1 || trUnitAlive() == false) {
					removeGambler(p);
				} else {
					trVectorSetUnitPos("start", "p"+p+"characters");
					trVectorQuestVarSet("dir", zGetUnitVector("start","p"+p+"decktarget"));
					trMutateSelected(kbGetProtoUnitID("Regent"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitOverrideAnimation(1,0,false,false,-1);
					ySetVar("p"+p+"characters", "animating", 1);
					ySetVar("p"+p+"characters", "timeout", trTimeMS() + 1100);
				}
			}
			
			trQuestVarSet("p"+p+"deckNext", trTimeMS());
			trQuestVarSet("p"+p+"cardsLoaded", trQuestVarGet("p"+p+"gamble"));
			trQuestVarSet("p"+p+"cardsType", trQuestVarGet("p"+p+"deckType"));
			trQuestVarSet("p"+p+"firstCard", 1);
			
			trQuestVarSet("p"+p+"gamble", 1);
			if (trQuestVarGet("p"+p+"gambleStep") == 0) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"numberFlag");
				trUnitSetAnimationPath("0,0,0,0,0,0,0");
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
			
			/* can't give buffs to allies */
			if ((ENEMY_PLAYER == 2) || (Multiplayer == false) || PvP) {
				trQuestVarSet("p"+p+"deckType", 1 + trQuestVarGet("p"+p+"deckType"));
				if (trQuestVarGet("p"+p+"deckType") >= 3) {
					trQuestVarSet("p"+p+"deckType", 1);
				}
			} else {
				trQuestVarSetFromRand("rand", 1, 2, true);
				trQuestVarSet("p"+p+"deckType", trQuestVarGet("p"+p+"deckType") + trQuestVarGet("rand"));
				if (trQuestVarGet("p"+p+"deckType") > 3) {
					trQuestVarSet("p"+p+"deckType", trQuestVarGet("p"+p+"deckType") - 3);
				}
			}
			if (trCurrentPlayer() == p) {
				switch(1*trQuestVarGet("p"+p+"deckType"))
				{
					case DECK_BURN:
					{
						lureName = "(W) Gimmick Deck: BURN";
					}
					case DECK_STUN:
					{
						lureName = "(W) Gimmick Deck: FREEZE";
					}
					case DECK_RELICS:
					{
						lureName = "(W) Gimmick Deck: RELICS";
					}
				}
				trCounterAbort("lure");
				trCounterAddTime("lure",
					trQuestVarGet("p"+p+"lureCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 0, lureName);
			}
		} else {
			trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_COST);
			if (trCurrentPlayer() == p) {
				trCounterAbort("lure");
				trSoundPlayFN("cantdothat.wav","1",-1,"","");
				trChatSend(0, "You must target an allied player!");
			}
		}
	}
	
	if (trQuestVarGet("p"+p+"cardsLoaded") > 0) {
		if (trTimeMS() > trQuestVarGet("p"+p+"deckNext")) {
			trQuestVarSet("p"+p+"deckNext", 200 + trQuestVarGet("p"+p+"deckNext"));
			for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
				if (yDatabaseNext("p"+p+"characters",true) == -1 || trUnitAlive() == false) {
					removeGambler(p);
				} else {
					trVectorSetUnitPos("start", "p"+p+"characters");
					trVectorQuestVarSet("dir", zGetUnitVector("start","p"+p+"decktarget"));
					addGenericProj("p"+p+"cards","start","dir",kbGetProtoUnitID("Statue of Automaton Base"),2,30.0,4,0,p);
					dist = xsMin(1000, zDistanceBetweenVectors("start","p"+p+"decktarget") / 0.03);
					yAddUpdateVar("p"+p+"cards", "timeout", trTimeMS() + dist);
					yAddUpdateVar("p"+p+"cards", "type", trQuestVarGet("p"+p+"cardsType"));
					yAddUpdateVar("p"+p+"cards", "targetx", trQuestVarGet("p"+p+"decktargetx"));
					yAddUpdateVar("p"+p+"cards", "targetz", trQuestVarGet("p"+p+"decktargetz"));
					if (trQuestVarGet("p"+p+"cardsType") == DECK_RELICS) {
						yAddUpdateVar("p"+p+"cards", "target", target);
					} else if (trQuestVarGet("p"+p+"cardsType") == DECK_BURN) {
						if (trQuestVarGet("p"+p+"firstCard") == 0) {
							yAddUpdateVar("p"+p+"cards","type",0);
						} else {
							yAddUpdateVar("p"+p+"cards","count",trQuestVarGet("p"+p+"deckCount"));
							trQuestVarSet("p"+p+"firstCard",0);
						}
					}
				}
			}
			trQuestVarSet("p"+p+"cardsLoaded", trQuestVarGet("p"+p+"cardsLoaded") - 1);
		}
	}
	
	ySetPointer("enemies", index);
	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseGambler(int eventID = -1) {
	int p = eventID - 1000 - 12 * GAMBLER;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Loaded Dice: COPY";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Gamble";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Gimmick Deck: BURN";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"diceType", 1);
	trQuestVarSet("p"+p+"deckType", 1);
	trQuestVarSet("p"+p+"gamble", 1);
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("diceCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("deckCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", trQuestVarGet("gambleCost"));
}


rule gambler_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * GAMBLER + p, "gamblerAlways");
		trEventSetHandler(1000 + 12 * GAMBLER + p, "chooseGambler");
	}
	
	trQuestVarSet("diceCooldown", 6);
	trQuestVarSet("diceRadius", 6);
	trQuestVarSet("diceDamage", 100);
	
	trQuestVarSet("deckCooldown", 12);
	trQuestVarSet("deckRadius", 6);
	trQuestVarSet("deckDuration", 6);
	trQuestVarSet("deckDamage", 20);
	
	trQuestVarSet("gambleCost", 30);
}
