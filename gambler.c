const int DICE_COPY = 1;
const int DICE_EXPLODE = 2;
const int DICE_HEAL = 3;

const int DECK_BURN = 1;
const int DECK_STUN = 2;
const int DECK_RELICS = 3;

int diceCooldown = 6;
float diceRadius = 6;
float diceDamage = 100;

int deckCooldown = 12;
float deckRadius = 6;
float deckDuration = 6;
float deckDamage = 20;

float gambleCost = 30;

int xGamblerAnimating = 0;
int xGamblerTimeout = 0;
int xGamblerSpy = 0;

int GamblerNumberFlags = 0;
int GamblerGambleSFX = 0;

int xDicePrev = 0;
int xDiceUnit = 0;
int xDiceSFX = 0;
int xDiceDir = 0;
int xDiceType = 0;
int xDiceCount = 0;

void removeGambler(int p = 0) {
	removePlayerSpecific(p);
}

void gamblerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * GAMBLER;
	int id = 0;
	int action = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int dice = trQuestVarGet("p"+p+"dice");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);

	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		if (trUnitAlive() == false) {
			removeGambler(p);
		} else if (xGetBool(db, xGamblerAnimating)) {
			if (trTimeMS() > xGetInt(db, xGamblerTimeout)) {
				trUnitOverrideAnimation(-1, 0, false, true, -1);
				xSetBool(db, xGamblerAnimating, false);
			}
		} else {
			switch(xGetInt(db, xGamblerSpy))
			{
				case 0:
				{
					if (xGetInt(dPlayerUnits, xStunSFX, xGetInt(db, xCharIndex)) == 0) {
						xSetPointer(dPlayerUnits, xGetInt(db, xCharIndex));
						xUnitSelectByID(db, xUnitID);
						trMutateSelected(kbGetProtoUnitID("Hypaspist"));
						spyEffect(xGetInt(db, xUnitName),
							kbGetProtoUnitID("Cinematic Block"),xsVectorSet(dPlayerUnits,xStunSFX,xGetPointer(dPlayerUnits)));
						spyEffect(xGetInt(db, xUnitName),
							kbGetProtoUnitID("Cinematic Block"),xsVectorSet(dPlayerUnits,xPoisonSFX,xGetPointer(dPlayerUnits)));
						spyEffect(xGetInt(db, xUnitName),
							kbGetProtoUnitID("Cinematic Block"),xsVectorSet(dPlayerUnits,xSilenceSFX,xGetPointer(dPlayerUnits)));
						xSetInt(db, xGamblerSpy, 1);
					}
				}
				case 1:
				{
					if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
						xUnitSelectByID(db, xUnitID);
						trMutateSelected(kbGetProtoUnitID("Regent"));
						xSetInt(db, xGamblerSpy, 2);
					}
				}
			}
		}
	}
	
	switch(1*trQuestVarGet("p"+p+"gambleSpy"))
	{
		case 0:
		{
			xUnitSelect(dPlayerData, xPlayerUnit);
			if (trUnitAlive()) {
				target = aiPlanGetUserVariableInt(ARRAYS, GamblerNumberFlags, p);
				if ((target <= 0) || (kbGetBlockID(""+target) == -1)) {
					xUnitSelect(dPlayerData, xPlayerUnit);
					trMutateSelected(kbGetProtoUnitID("Hypaspist"));
					spyEffect(xGetInt(dPlayerData, xPlayerUnit),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS, GamblerNumberFlags, p));
					spyEffect(xGetInt(dPlayerData, xPlayerUnit),kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS, GamblerGambleSFX, p));
					trQuestVarSet("p"+p+"gambleSpy", 1);
				}
			}
		}
		case 1:
		{
			if (trQuestVarGet("spyfind") == trQuestVarGet("spyfound")) {
				xUnitSelect(dPlayerData, xPlayerUnit);
				trMutateSelected(kbGetProtoUnitID("Regent"));
				trQuestVarSet("p"+p+"gambleSpy", 2);
			}
		}
		case 2:
		{
			target = aiPlanGetUserVariableInt(ARRAYS, GamblerNumberFlags, p);
			if ((target <= 0) || (kbGetBlockID(""+target) == -1)) {
				trQuestVarSet("p"+p+"gambleSpy", 0);
			}
		}
	}
	
	if (xGetDatabaseCount(dice) > 0) {
		xDatabaseNext(dice);
		prev = xGetVector(dice, xDicePrev);
		pos = kbGetBlockPosition(""+xGetInt(dice, xUnitName), true);
		dist = distanceBetweenVectors(prev, pos, false);
		if (dist > 2.0) {
			dir = xGetVector(dice, xDiceDir);
			if (xGetInt(dice, xDiceType) == DICE_HEAL) {
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (rayCollision(dPlayerUnits,prev,dir,dist + 2.0,4.0)) {
						healUnit(p, dist * diceDamage * 0.25 * xGetFloat(dPlayerData, xPlayerSpellDamage));
						gainFavor(p, dist);
					}
				}
			} else {
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision(dEnemies,prev,dir,dist + 2.0, 4.0)) {
						hit = 1;
						break;
					}
				}
			}
			
			if (hit == 1) {
				gainFavor(p, 3.0);
				switch(xGetInt(dice, xDiceType))
				{
					case DICE_COPY:
					{
						if (bossUnit != xGetInt(dEnemies, xUnitName)) {
							id = kbGetUnitBaseTypeID(xGetInt(dEnemies, xUnitID));
							trSoundPlayFN("favordump.wav","1",-1,"","");
							trSoundPlayFN("changeunit.wav","1",-1,"","");
							for(x=xGetInt(dice, xDiceCount); >0) {
								trQuestVarSetFromRand("heading",1,360,true);
								next = trGetNextUnitScenarioNameNumber();
								trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
								trUnitSelectClear();
								trUnitSelect(""+next, true);
								trMutateSelected(id);
								activatePlayerUnit(next, p, id, calculateDecay(p, 4.0));
							}
							xUnitSelect(dice, xDiceUnit);
							trUnitChangeProtoUnit("Osiris Box Glow");
							xUnitSelect(dice, xDiceSFX);
							trUnitChangeProtoUnit("Tremor");
							xUnitSelect(dice, xUnitName);
							trUnitChangeProtoUnit("Dust Large");
						}
					}
					case DICE_EXPLODE:
					{
						trSoundPlayFN("favordump.wav","1",-1,"","");
						trSoundPlayFN("cinematics\32_out\explosion.mp3","1",-1,"","");
						xUnitSelect(dice, xDiceUnit);
						trUnitChangeProtoUnit("Meteor Impact Ground");
						xUnitSelect(dice, xDiceSFX);
						trUnitChangeProtoUnit("Qilin Heal");
						xUnitSelect(dice, xUnitName);
						trDamageUnitPercent(100);
						trUnitChangeProtoUnit("Meteorite");
						
						dist = xsPow(diceRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
						amt = diceDamage * xGetFloat(dPlayerData, xPlayerSpellDamage);
						for(x=xGetDatabaseCount(dEnemies); >0) {
							xDatabaseNext(dEnemies);
							xUnitSelectByID(dEnemies, xUnitID);
							if (trUnitAlive() == false) {
								removeEnemy();
							} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
								damageEnemy(p, amt);
							}
						}
					}
				}
				xFreeDatabaseBlock(dice);
			} else if (terrainIsType(vectorToGrid(pos), TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				xUnitSelect(dice, xDiceUnit);
				trUnitChangeProtoUnit("Dust Small");
				xUnitSelect(dice, xDiceSFX);
				trUnitChangeProtoUnit("Dust Small");
				xUnitSelect(dice, xUnitName);
				trUnitChangeProtoUnit("Dust Small");
				xFreeDatabaseBlock(dice);
			} else {
				xSetVector(dice, xDicePrev, pos);
				trQuestVarSetFromRand("sound", 1, 5, true);
				if (trQuestVarGet("sound") <= 3) {
					xUnitSelect(dice, xUnitName);
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
		for(x=xGetDatabaseCount(db); >0) {
			if (xDatabaseNext(db,true) == -1 || trUnitAlive() == false) {
				removeGambler(p);
			} else {
				trVectorSetUnitPos("start", db);
				trVectorQuestVarSet("dir", zGetUnitVector("start","p"+p+"wellPos"));
				trMutateSelected(kbGetProtoUnitID("Regent"));
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trUnitOverrideAnimation(1,0,false,false,-1);
				ySetVar(db, "animating", 1);
				ySetVar(db, "timeout", trTimeMS() + 1100);
				
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
				
				yAddToDatabase(dice, "next");
				yAddUpdateVar(dice,"prevx",trQuestVarGet("startx"));
				yAddUpdateVar(dice,"prevz",trQuestVarGet("startz"));
				yAddUpdateVar(dice, "dice", trQuestVarGet("dice"));
				yAddUpdateVar(dice, "sfx", trQuestVarGet("sfx"));
				yAddUpdateVar(dice, "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar(dice, "dirz", trQuestVarGet("dirz"));
				yAddUpdateVar(dice, "type", trQuestVarGet("p"+p+"diceType"));
				yAddUpdateVar(dice, "count", trQuestVarGet("p"+p+"gamble"));
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
		gainFavor(p, 0.0 - trQuestVarGet("gambleCost") * xGetFloat(dPlayerData, xPlayerUltimateCost));
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
	
	if (xGetDatabaseCount("p"+p+"deckBurns") > 0) {
		xDatabaseNext("p"+p+"deckBurns");
		if (trTimeMS() > yGetVar("p"+p+"deckBurns","next")) {
			ySetVar("p"+p+"deckBurns","next", trTimeMS() + 500);
			yVarToVector("p"+p+"deckBurns","pos");
			amt = 2;
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				if (xDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
					removePlayerUnit();
				} else if (zDistanceToVectorSquared("playerUnits", "pos") < yGetVar("p"+p+"deckBurns", "radius")) {
					amt = 1 + amt;
				}
			}
			amt = amt * 0.5 * yGetVar("p"+p+"deckBurns","damage");
			for(x=xGetDatabaseCount(dEnemies); >0) {
				if (xDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
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
	
	if (xGetDatabaseCount("p"+p+"tempRelics") > 0) {
		target = xDatabaseNext("p"+p+"tempRelics");
		if (trTimeMS() > yGetVar("p"+p+"tempRelics", "timeout")) {
			relicEffect(1*yGetVar("p"+p+"tempRelics","type"),target,false);
			yRemoveFromDatabase("p"+p+"tempRelics");
		}
	}
	
	for (y=xsMin(3, xGetDatabaseCount("p"+p+"cards")); > 0) {
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
					dist = trQuestVarGet("deckRadius") * xGetFloat(dPlayerData, xPlayerSpellRange);
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
						trQuestVarGet("deckDamage") * xGetFloat(dPlayerData, xPlayerSpellDamage) * yGetVar("p"+p+"cards","count"));
					yAddUpdateVar("p"+p+"deckBurns", "next", trTimeMS());
					yAddUpdateVar("p"+p+"deckBurns", "timeout",
						trTimeMS() + 1000 * trQuestVarGet("deckDuration") * xGetFloat(dPlayerData, xPlayerSpellDuration));
				}
				case DECK_STUN:
				{
					trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
					yVarToVector("p"+p+"cards","target");
					dist = trQuestVarGet("deckRadius") * xGetFloat(dPlayerData, xPlayerSpellRange);
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
						if (xDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
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
					hit = trTimeMS() + trQuestVarGet("deckDuration") * xGetFloat(dPlayerData, xPlayerSpellDuration);
					for(x=xGetDatabaseCount("p"+p+"relics"); >0) {
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
			for(x=xGetDatabaseCount("playerCharacters"); >0) {
				if (xDatabaseNext("playerCharacters", true) == -1 || trUnitAlive() == false) {
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
			
			for(x=xGetDatabaseCount(db); >0) {
				if (xDatabaseNext(db,true) == -1 || trUnitAlive() == false) {
					removeGambler(p);
				} else {
					trVectorSetUnitPos("start", db);
					trVectorQuestVarSet("dir", zGetUnitVector("start","p"+p+"decktarget"));
					trMutateSelected(kbGetProtoUnitID("Regent"));
					trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
					trUnitOverrideAnimation(1,0,false,false,-1);
					ySetVar(db, "animating", 1);
					ySetVar(db, "timeout", trTimeMS() + 1100);
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
			for(x=xGetDatabaseCount(db); >0) {
				if (xDatabaseNext(db,true) == -1 || trUnitAlive() == false) {
					removeGambler(p);
				} else {
					trVectorSetUnitPos("start", db);
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
	
	xSetPointer(dEnemies, index);
}

void chooseGambler(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * GAMBLER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
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
	xGamblerAnimating = xInitAddBool(db, "animating");
	xGamblerTimeout = xInitAddInt(db, "timeout");
	xGamblerSpy = xInitAddInt(db, "spy");
	trQuestVarSet("p"+p+"diceType", 1);
	trQuestVarSet("p"+p+"deckType", 1);
	trQuestVarSet("p"+p+"gamble", 1);

	xSetInt(dPlayerData,xPlayerWellCooldown, diceCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, deckCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, gambleCost);

	if (trQuestVarGet("p"+p+"dice") == 0) {
		db = xInitDatabase("p"+p+"dice");
		trQuestVarSet("p"+p+"dice", db);
		xInitAddInt(db, "name");
		xDicePrev = xInitAddVector(db, "prev");
		xDiceUnit = xInitAddInt(db, "diceUnit");
		xDiceSFX = xInitAddInt(db, "sfx");
		xDiceDir = xInitAddVector(db, "dir");
		xDiceType = xInitAddInt(db, "type");
		xDiceCount = xInitAddInt(db, "count");
	}
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

	GamblerNumberFlags = zNewArray(mInt, ENEMY_PLAYER, "GamblerNumberFlags");
	GamblerGambleSFX = zNewArray(mInt, ENEMY_PLAYER, "GamblerGambleSFX");
}
