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

int xDeckBurnEnd = 0;
int xDeckBurnRadius = 0;
int xDeckBurnPos = 0;
int xDeckBurnDamage = 0;
int xDeckBurnNext = 0;
int xDeckBurnTimeout = 0;

int xTempRelicPlayer = 0;
int xTempRelicTimeout = 0;

int xCardTimeout = 0;
int xCardType = 0;
int xCardDest = 0;
int xCardCount = 0;

void removeGambler(int p = 0) {
	removePlayerSpecific(p);
}

void gamblerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * GAMBLER;
	pvpDetachPlayer(p);
	int id = 0;
	int action = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int dice = trQuestVarGet("p"+p+"dice");
	int deckburns = trQuestVarGet("p"+p+"deckBurns");
	int relics = trQuestVarGet("p"+p+"tempRelics");
	int cards = trQuestVarGet("p"+p+"cards");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector pos = vector(0,0,0);
	vector prev = vector(0,0,0);
	vector dir = vector(0,0,0);
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		xUnitSelectByID(db, xUnitID);
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
					spyEffect(xGetInt(dPlayerData, xPlayerUnit),
						kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS, GamblerNumberFlags, p));
					spyEffect(xGetInt(dPlayerData, xPlayerUnit),
						kbGetProtoUnitID("Cinematic Block"),xsVectorSet(ARRAYS, GamblerGambleSFX, p));
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
			xDatabaseNext(db);
			xUnitSelectByID(db, xUnitID);
			if (trUnitAlive() == false) {
				removeGambler(p);
			} else {
				prev = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
				dir = getUnitVector(prev, xGetVector(dPlayerData, xPlayerWellPos));
				trMutateSelected(kbGetProtoUnitID("Regent"));
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trUnitOverrideAnimation(1,0,false,false,-1);
				xSetBool(db, xGamblerAnimating, true);
				xSetInt(db, xGamblerTimeout, trTimeMS() + 1100);
				
				pos = vectorSetAsTargetVector(prev, xGetVector(dPlayerData, xPlayerWellPos), 300.0);
				next = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,xsVectorGetX(prev),0,xsVectorGetZ(prev),0,true);
				trArmySelect(""+p+",0");
				trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
				trSetUnitOrientation(dir,vector(0,1,0),true);
				
				target = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				hit = trGetNextUnitScenarioNameNumber();
				trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
				trUnitSelectClear();
				trUnitSelect(""+target, true);
				trUnitChangeProtoUnit("Revealer");
				trUnitSelectClear();
				trUnitSelect(""+target, true);
				trUnitSetAnimationPath("1,0,0,0,0,0,0");
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+next);
				trMutateSelected(kbGetProtoUnitID("Revealer"));
				trSetSelectedScale(amt,amt,amt);
				trUnitSelectClear();
				trUnitSelect(""+hit, true);
				trMutateSelected(kbGetProtoUnitID("Relic"));
				trImmediateUnitGarrison(""+next);
				trUnitSelectClear();
				trUnitSelect(""+hit, true);
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
				trUnitSelect(""+next, true);
				trMutateSelected(kbGetProtoUnitID("Axe"));
				trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,false);
				trSetSelectedScale(0.5,0.01,0.5);
				
				xAddDatabaseBlock(dice, true);
				xSetInt(dice, xUnitName, next);
				xSetVector(dice, xDicePrev, prev);
				xSetInt(dice, xDiceUnit, target);
				xSetInt(dice, xDiceSFX, hit);
				xSetVector(dice, xDiceDir, dir);
				xSetInt(dice, xDiceType, 1*trQuestVarGet("p"+p+"diceType"));
				xSetInt(dice, xDiceCount, 1*trQuestVarGet("p"+p+"gamble"));
			}
		}
		trModifyProtounit("Hero Greek Achilles", p, 5, -2);
		trQuestVarSet("p"+p+"gamble", 1);
		if (trQuestVarGet("p"+p+"gambleStep") == 0) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerNumberFlags,p));
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
				xGetInt(dPlayerData, xPlayerWellCooldown) * xGetFloat(dPlayerData, xPlayerCooldownReduction), 0, wellName);
		}
	}
	
	if (trQuestVarGet("p"+p+"gambleStep") == 1) {
		if (trTimeMS() > trQuestVarGet("p"+p+"gambleNext")) {
			trQuestVarSet("p"+p+"gambleNext", trQuestVarGet("p"+p+"gambleNext") + 100);
			trQuestVarSetFromRand("rand", 0, 5, true);
			if (kbGetBlockID(""+aiPlanGetUserVariableInt(ARRAYS,GamblerNumberFlags,p)) != -1) {
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerNumberFlags,p), true);
				trUnitSetAnimationPath(""+1*trQuestVarGet("rand")+",0,0,0,0,0,0");
			}
			if (trTimeMS() > trQuestVarGet("p"+p+"gambleTimeout")) {
				trQuestVarSet("p"+p+"gambleStep", 0);
				trQuestVarSet("p"+p+"gamble", trQuestVarGet("rand") + 1);
				trSoundPlayFN("favordump.wav","1",-1,"","");
				trUnitHighlight(0.3,false);
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerGambleSFX,p), true);
				trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
			}
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		gainFavor(p, 0.0 - gambleCost * xGetFloat(dPlayerData, xPlayerUltimateCost));
		trSoundPlayFN("plentybirth.wav","1",-1,"","");
		xUnitSelect(dPlayerData, xPlayerUnit);
		if (trUnitAlive()) {
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerNumberFlags,p), true);
			trMutateSelected(kbGetProtoUnitID("Flag Numbered"));
			trUnitSelectClear();
			trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerGambleSFX,p), true);
			trMutateSelected(kbGetProtoUnitID("Curse SFX"));
		}
		trQuestVarSet("p"+p+"gambleNext", trTimeMS());
		trQuestVarSet("p"+p+"gambleTimeout", trTimeMS() + 1500);
		trQuestVarSet("p"+p+"gambleStep", 1);
	}
	
	if (xGetDatabaseCount(deckburns) > 0) {
		xDatabaseNext(deckburns);
		if (trTimeMS() > xGetInt(deckburns, xDeckBurnNext)) {
			dist = xGetFloat(deckburns, xDeckBurnRadius);
			xSetInt(deckburns, xDeckBurnNext, xGetInt(deckburns, xDeckBurnNext) + 500);
			pos = xGetVector(deckburns, xDeckBurnPos);
			amt = 2;
			for(x=xGetDatabaseCount(dPlayerUnits); >0) {
				xDatabaseNext(dPlayerUnits);
				xUnitSelectByID(dPlayerUnits, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < dist) {
					amt = 1 + amt;
				}
			}
			amt = amt * 0.5 * xGetFloat(deckburns, xDeckBurnDamage);
			for(x=xGetDatabaseCount(dEnemies); >0) {
				xDatabaseNext(dEnemies);
				xUnitSelectByID(dEnemies, xUnitID);
				if (trUnitAlive() == false) {
					removeEnemy();
				} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < dist) {
					damageEnemy(p, amt);
				}
			}
			if (trTimeMS() > xGetInt(deckburns, xDeckBurnTimeout)) {
				for(x=xGetInt(deckburns, xUnitName); < xGetInt(deckburns, xDeckBurnEnd)) {
					trUnitSelectClear();
					trUnitSelect(""+x, true);
					trUnitDestroy();
				}
				xFreeDatabaseBlock(deckburns);
			}
		}
	}
	
	if (xGetDatabaseCount(relics) > 0) {
		xDatabaseNext(relics);
		target = xGetInt(relics, xTempRelicPlayer);
		if (trTimeMS() > xGetInt(relics, xTempRelicTimeout)) {
			relicEffect(xGetInt(relics, xRelicType),target,false);
			xFreeDatabaseBlock(relics);
		}
	}
	
	for (y=xsMin(3, xGetDatabaseCount(cards)); > 0) {
		action = processGenericProj(cards);
		if (action == PROJ_BOUNCE) {
			trSetSelectedScale(0.5,0.1,0.8);
		}
		if (trTimeMS() > xGetInt(cards, xCardTimeout)) {
			xUnitSelect(cards, xUnitName);
			trUnitChangeProtoUnit("Fimbulwinter SFX");
			xUnitSelect(cards, xUnitName);
			trDamageUnitPercent(-100);
			switch(xGetInt(cards, xCardType))
			{
				case DECK_BURN:
				{
					trSoundPlayFN("flamingweapons.wav","1",-1,"","");
					pos = xGetVector(cards, xCardDest);
					dist = deckRadius * xGetFloat(dPlayerData, xPlayerSpellRange);
					dir = xsVectorSet(dist, 0, 0);
					next = trGetNextUnitScenarioNameNumber();
					for(x=16; >0) {
						prev = pos + dir;
						trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(prev),0,xsVectorGetZ(prev),0,true);
						trArmySelect("1,0");
						trMutateSelected(kbGetProtoUnitID("Hades Fire"));
						trUnitSetAnimationPath("1,1,0,0,0,0,0");
						dir = rotationMatrix(dir, 0.923879, 0.382683);
					}
					xAddDatabaseBlock(deckburns, true);
					xSetInt(deckburns, xUnitName, next);
					xSetInt(deckburns, xDeckBurnEnd, trGetNextUnitScenarioNameNumber());
					xSetFloat(deckburns, xDeckBurnRadius, xsPow(dist, 2));
					xSetVector(deckburns, xDeckBurnPos, pos);
					xSetFloat(deckburns, xDeckBurnDamage,
						deckDamage * xGetFloat(dPlayerData, xPlayerSpellDamage) * xGetInt(cards,xCardCount));
					xSetInt(deckburns, xDeckBurnNext, trTimeMS());
					xSetInt(deckburns, xDeckBurnTimeout,
						trTimeMS() + 1000 * deckDuration * xGetFloat(dPlayerData, xPlayerSpellDuration));
				}
				case DECK_STUN:
				{
					trSoundPlayFN("frostgiantattack.wav","1",-1,"","");
					pos = xGetVector(cards, xCardDest);
					dist = deckRadius * xGetFloat(dPlayerData, xPlayerSpellRange);
					next = trGetNextUnitScenarioNameNumber();
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trUnitChangeProtoUnit("Rocket");
					trUnitSelectClear();
					trUnitSelect(""+next, true);
					trMutateSelected(kbGetProtoUnitID("Frost Drift"));
					trSetSelectedScale(0.3 * dist,1,0.4*dist);
					dist = xsPow(dist, 2);
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies, xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName),pos) < dist) {
							stunUnit(dEnemies,2.5,p,false);
						}
					}
				}
				case DECK_RELICS:
				{
					trSoundPlayFN("researchcomplete.wav","1",-1,"","");
					trSoundPlayFN("ageadvance.wav","1",-1,"","");
					target = xGetInt(cards, xCardCount);
					hit = trTimeMS() + deckDuration * xGetFloat(dPlayerData, xPlayerSpellDuration);
					next = getRelicsDB(p);
					for(x=xGetDatabaseCount(next); >0) {
						xDatabaseNext(next);
						if (xGetInt(next, xRelicType) <= NORMAL_RELICS) {
							xAddDatabaseBlock(relics, true);
							xSetInt(relics, xTempRelicPlayer, target);
							xSetInt(relics, xTempRelicTimeout, hit);
							xSetInt(relics, xRelicType, xGetInt(next, xRelicType));
							relicEffect(xGetInt(next, xRelicType),target,true);
						}
					}
					if (trCurrentPlayer() == target) {
						trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> has given you all their relics!");
					}
				}
			}
			xFreeDatabaseBlock(cards);
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		pos = xGetVector(dPlayerData, xPlayerLurePos);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject");
		trUnitDestroy();
		hit = 0;
		if (trQuestVarGet("p"+p+"deckType") == DECK_RELICS) {
			dist = 100;
			for(x=xGetDatabaseCount(dPlayerCharacters); >0) {
				xDatabaseNext(dPlayerCharacters);
				xUnitSelectByID(dPlayerCharacters, xUnitID);
				if (trUnitAlive() == false) {
					removePlayerCharacter();
				} else if (xGetInt(dPlayerCharacters, xPlayerOwner) != p) {
					current = unitDistanceToVector(xGetInt(dPlayerCharacters, xUnitName), pos);
					if (current < dist) {
						hit = xGetInt(dPlayerCharacters, xUnitName);
						target = xGetInt(dPlayerCharacters, xPlayerOwner);
						dist = current;
					}
				}
			}
			if (hit > 0) {
				pos = kbGetBlockPosition(""+hit, true);
			}
		} else {
			hit = 1;
			pos = vectorSnapToGrid(pos);
		}
		xSetVector(dPlayerData, xPlayerLurePos, pos);
		
		if (hit > 0) {
			gainFavor(p, 5.0);
			trSoundPlayFN("ui\scroll.wav","1",-1,"","");
			trSoundPlayFN("skypassagein.wav","1",-1,"","");
			
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeGambler(p);
				} else {
					prev = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dir = getUnitVector(prev, pos);
					trMutateSelected(kbGetProtoUnitID("Regent"));
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitOverrideAnimation(1,0,false,false,-1);
					xSetBool(db, xGamblerAnimating, true);
					xSetInt(db, xGamblerTimeout, trTimeMS() + 1100);
				}
			}
			
			trQuestVarSet("p"+p+"deckNext", trTimeMS());
			trQuestVarSet("p"+p+"cardsLoaded", trQuestVarGet("p"+p+"gamble"));
			trQuestVarSet("p"+p+"cardsType", trQuestVarGet("p"+p+"deckType"));
			trQuestVarSet("p"+p+"firstCard", 1);
			
			trQuestVarSet("p"+p+"gamble", 1);
			if (trQuestVarGet("p"+p+"gambleStep") == 0) {
				trUnitSelectClear();
				trUnitSelect(""+aiPlanGetUserVariableInt(ARRAYS,GamblerNumberFlags,p), true);
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
					xGetInt(dPlayerData, xPlayerLureCooldown) * xGetFloat(dPlayerData, xPlayerCooldownReduction), 0, lureName);
			}
		} else {
			xSetInt(dPlayerData, xPlayerLureCooldownStatus, ABILITY_COST);
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
			pos = xGetVector(dPlayerData, xPlayerLurePos);
			trQuestVarSet("p"+p+"deckNext", 200 + trQuestVarGet("p"+p+"deckNext"));
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db, xUnitID);
				if (trUnitAlive() == false) {
					removeGambler(p);
				} else {
					prev = kbGetBlockPosition(""+xGetInt(db, xUnitName), true);
					dir = getUnitVector(prev, pos);
					addGenericProj(cards,prev, dir);
					dist = xsMin(1000, distanceBetweenVectors(prev,pos, false) / 0.03);
					xSetInt(cards, xCardTimeout, trTimeMS() + dist);
					xSetInt(cards, xCardType, 1*trQuestVarGet("p"+p+"cardsType"));
					xSetVector(cards, xCardDest, pos);
					if (trQuestVarGet("p"+p+"cardsType") == DECK_RELICS) {
						xSetInt(cards, xCardCount, target);
					} else if (trQuestVarGet("p"+p+"cardsType") == DECK_BURN) {
						if (trQuestVarGet("p"+p+"firstCard") == 0) {
							xSetInt(cards, xCardType, 0);
						} else {
							xSetInt(cards, xCardCount,trQuestVarGet("p"+p+"deckCount"));
							trQuestVarSet("p"+p+"firstCard",0);
						}
					}
				}
			}
			trQuestVarSet("p"+p+"cardsLoaded", trQuestVarGet("p"+p+"cardsLoaded") - 1);
		}
	}
	
	xSetPointer(dEnemies, index);
	pvpReattachPlayer();
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
	
	if (trQuestVarGet("p"+p+"deckBurns") == 0) {
		db = xInitDatabase("p"+p+"deckBurns");
		trQuestVarSet("p"+p+"deckBurns", db);
		xInitAddInt(db, "name");
		xDeckBurnEnd = xInitAddInt(db,"end");
		xDeckBurnRadius = xInitAddFloat(db,"radius");
		xDeckBurnPos = xInitAddVector(db,"pos");
		xDeckBurnDamage = xInitAddFloat(db, "damage");
		xDeckBurnNext = xInitAddInt(db, "next");
		xDeckBurnTimeout = xInitAddInt(db, "timeout");
	}
	
	if (trQuestVarGet("p"+p+"tempRelics") == 0) {
		db = xInitDatabase("p"+p+"tempRelics");
		trQuestVarSet("p"+p+"tempRelics", db);
		xTempRelicPlayer = xInitAddInt(db, "player");
		xInitAddInt(db, "type");
		xTempRelicTimeout = xInitAddInt(db, "type");
	}
	
	if (trQuestVarGet("p"+p+"cards") == 0) {
		db = initGenericProj("p"+p+"cards",kbGetProtoUnitID("Statue of Automaton Base"),2,30.0,4,0,p);
		xCardTimeout = xInitAddInt(db, "timeout");
		xCardType = xInitAddInt(db, "type");
		xCardDest = xInitAddVector(db, "dest");
		xCardCount = xInitAddInt(db, "count");
		trQuestVarSet("p"+p+"cards", db);
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
