const int CHEST_STATE_CLOSED = 0;
const int CHEST_STATE_UNLOCKED = 1;
const int CHEST_STATE_REWARDING = 2;
const int CHEST_STATE_COUNTDOWN = 3;

int xStatuePosition = 0;
int xStatueAngle = 0;
int xStatueState = 0;
int xStatueTimeout = 0;
int xStatueConnections = 0;
int xStatueArray = 0;
int xStatuePos = 0;

int dRainingRelics = 0;
int xRainingRelicMorphed = 0;

int dChestHitbox = 0;
int xChestHitboxCenter = 0;
int xChestHitboxStart = 0;
int xChestHitboxDist = 0;

int dChests = 0;
int xChestType = 0;
int xChestState = 0;
int xChestRoom = 0;
int xChestSFX = 0;

int xChestKey = 0;
int xChestCount = 0;

int xChestBegin = 0;
int xChestEnd = 0;

rule initialize_chest_database
active
highFrequency
{
	xsDisableSelf();
	dRainingRelics = xInitDatabase("rainingRelics");
	xInitAddInt(dRainingRelics,"name");
	xRainingRelicMorphed = xInitAddInt(dRainingRelics,"morphed");
	
	dChestHitbox = xInitDatabase("chestHitboxes");
	xChestHitboxCenter = xInitAddVector(dChestHitbox,"center");
	xChestHitboxStart = xInitAddInt(dChestHitbox,"start");
	xChestHitboxDist = xInitAddFloat(dChestHitbox,"dist");
	
	dChests = xInitDatabase("chests");
	xInitAddInt(dChests,"name");
	xChestType = xInitAddInt(dChests,"type");
	xChestState = xInitAddInt(dChests,"state");
	xChestRoom = xInitAddInt(dChests,"room");
	xChestSFX = xInitAddInt(dChests,"sfx"); // also doubles as enemies begin
	xChestKey = xInitAddInt(dChests,"key"); // also doubles as temple indicator and count and enemies end
	xChestCount = xChestKey;
	xChestBegin = xChestSFX;
	xChestEnd = xChestKey;
}

void initializeStatuePuzzle(int room = 0) {
	int db = xInitDatabase("statuesIn"+room);
	trQuestVarSet("statuesIn"+room, db);
	xInitAddInt(db,"name");
	xStatueState = xInitAddInt(db,"state");
	xStatuePosition = xInitAddInt(db,"position");
	xStatueAngle = xInitAddFloat(db,"angle");
	xStatueTimeout = xInitAddInt(db,"timeout");
	xStatueConnections = xInitAddInt(db,"connections");
	xStatueArray = xInitAddInt(db,"array");
	xStatuePos = xInitAddVector(db,"pos");
}

void turnStatue(int room = 0, int index = 0, bool first = false, bool immediate = false) {
	int db = trQuestVarGet("statuesIn"+room);
	int old = xGetPointer(db);
	if (index > 0) {
		xSetPointer(db, index);
	}
	xSetFloat(db, xStatueAngle, fModulo(6.283185, 1.570796 + xGetFloat(db, xStatueAngle)));
	xSetInt(db, xStatuePosition, 1 + xGetInt(db, xStatuePosition));
	if (xGetInt(db, xStatuePosition) >= 4) {
		xSetInt(db, xStatuePosition, 0);
		trQuestVarSet("correctStatuesIn"+room, 1 + trQuestVarGet("correctStatuesIn"+room));
	} else if (xGetInt(db, xStatuePosition) == 1) {
		trQuestVarSet("correctStatuesIn"+room, trQuestVarGet("correctStatuesIn"+room) - 1);
	}
	xUnitSelect(db, xUnitName);
	trDamageUnitPercent(-100);
	if (immediate) {
		trVectorSetFromAngle("dir", xGetFloat(db, xStatueAngle));
		trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
	} else {
		trUnitConvert(0);
		if (xGetInt(db, xStatueState) == 1) {
			xSetInt(db, xStatueTimeout, 1000 + xGetInt(db, xStatueTimeout));
		} else {
			xSetInt(db, xStatueState, 1);
			xSetInt(db, xStatueTimeout, trTimeMS() + 1000);
			trQuestVarSet("movingStatuesIn"+room, 1 + trQuestVarGet("movingStatuesIn"+room));
		}
	}
	
	if (first) {
		for(x=0; < xGetInt(db, xStatueConnections)) {
			turnStatue(room, aiPlanGetUserVariableInt(ARRAYS,xGetInt(db,xStatueArray),x), false, immediate);
		}
		if (immediate == false) {
			trSoundPlayFN("trojangateopen.wav","1",-1,"","");
		}
	}
	
	if (index > 0) {
		xSetPointer(db, old);
	}
}

void processChests() {
	int room = 0;
	int db = 0;
	float dist = 0;
	float angle = 0;
	float amt = 0;
	vector pos = vector(0,0,0);
	vector loc = vector(0,0,0);
	if (xGetDatabaseCount(dChestHitbox) > 0) {
		xDatabaseNext(dChestHitbox);
		pos = xGetVector(dChestHitbox,xChestHitboxCenter);
		dist = xsPow(0.015 * (trTimeMS() - xGetInt(dChestHitbox,xChestHitboxStart)), 2);
		bool hit = false;
		amt = xGetFloat(dChestHitbox,xChestHitboxDist);
		for(i=xGetDatabaseCount(dPlayerUnits); >0) {
			xDatabaseNext(dPlayerUnits);
			xUnitSelectByID(dPlayerUnits,xUnitID);
			if (trUnitAlive() == false) {
				removePlayerUnit();
			} else {
				angle = unitDistanceToVector(xGetInt(dPlayerUnits,xUnitName),pos);
				if (angle > xGetFloat(dChestHitbox,xChestHitboxDist) && angle < dist) {
					damagePlayerUnit(1000.0);
					if (angle > amt) {
						amt = angle;
					}
				}
			}
		}
		for(i=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies,xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				angle = unitDistanceToVector(xGetInt(dEnemies,xUnitName),pos);
				if (angle > xGetFloat(dChestHitbox,xChestHitboxDist) && angle < dist) {
					trDamageUnit(1000.0);
					if (angle > amt) {
						amt = angle;
					}
				}
			}
		}
		xSetFloat(dChestHitbox,xChestHitboxDist,amt);
		if (trTimeMS() - xGetInt(dChestHitbox,xChestHitboxStart) > 1500) {
			xFreeDatabaseBlock(dChestHitbox);
		}
	}
	for(x=xGetDatabaseCount(dRainingRelics); > 0) {
		xDatabaseNext(dRainingRelics);
		xUnitSelect(dRainingRelics,xUnitName);
		if (xGetInt(dRainingRelics, xRainingRelicMorphed) == 1) {
			xSetInt(dRainingRelics, xRainingRelicMorphed, 0);
			trMutateSelected(kbGetProtoUnitID("Curse SFX"));
			trUnitSetAnimationPath("0,0,0,0,0,0,0");
		} else if (xGetInt(dRainingRelics, xRainingRelicMorphed) == 2) {
			xSetInt(dRainingRelics, xRainingRelicMorphed, 3);
			trMutateSelected(kbGetProtoUnitID("Meteorite death"));
		} else if (xGetInt(dRainingRelics, xRainingRelicMorphed) == 3) {
			pos = kbGetBlockPosition(""+xGetInt(dRainingRelics,xUnitName));
			if (xsVectorGetY(pos) <= 0) {
				trUnitChangeProtoUnit("Cinematic Scorch");
				xUnitSelect(dRainingRelics,xUnitName);
				trDamageUnitPercent(-100);
				trUnitSetAnimationPath("2,0,0,0,0,0,0");
				xFreeDatabaseBlock(dRainingRelics);
			}
		} else {
			pos = kbGetBlockPosition(""+xGetInt(dRainingRelics,xUnitName));
			if (xsVectorGetY(pos) <= worldHeight + 0.3) {
				trUnitChangeProtoUnit("Relic");
				xUnitSelect(dRainingRelics,xUnitName);
				trDamageUnitPercent(-100);
				xSetPointer(dFreeRelics,xAddDatabaseBlock(dFreeRelics));
				xSetInt(dFreeRelics,xRelicName,xGetInt(dRainingRelics,xUnitName));
				xSetInt(dFreeRelics,xRelicType,randomStageClosest(20));
				xFreeDatabaseBlock(dRainingRelics);
			}
		}
	}
	if (xGetDatabaseCount(dChests) > 0) {
		xDatabaseNext(dChests);
		xUnitSelect(dChests,xUnitName);
		switch(xGetInt(dChests,xChestState))
		{
			case CHEST_STATE_CLOSED:
			{
				switch(xGetInt(dChests,xChestType))
				{
					case CHEST_KEY:
					{
						if (trUnitIsSelected()) {
							reselectMyself();
							uiMessageBox("Find a relic with a matching symbol and bring it here to open this chest.");
						}
						pos = kbGetBlockPosition(""+xGetInt(dChests,xChestKey));
						if (unitDistanceToVector(xGetInt(dChests,xUnitName), pos) < 16) {
							xSetInt(dChests,xChestState,CHEST_STATE_UNLOCKED);
							xUnitSelect(dChests,xChestKey);
							trUnitChangeProtoUnit("Osiris Box Glow");
							xUnitSelect(dChests,xChestSFX);
							trUnitChangeProtoUnit("Rocket");
						}
					}
					case CHEST_STATUES:
					{
						if (trUnitIsSelected()) {
							reselectMyself();
							uiMessageBox("Make all the statues face the chest to open it.");
						}
						room = xGetInt(dChests,xChestRoom);
						db = trQuestVarGet("statuesIn"+room);
						for(x=xGetDatabaseCount(db); >0) {
							xDatabaseNext(db);
							xUnitSelect(db,xUnitName);
							switch(xGetInt(db, xStatueState))
							{
								case 0:
								{
									/* waiting */
									if (trUnitPercentDamaged() > 0) {
										turnStatue(room,0,true,false);
									}
								}
								case 1:
								{
									/* turning */
									angle = 0.001 * (xGetInt(db, xStatueTimeout) - trTimeMS());
									if (angle < 0) {
										angle = xGetFloat(db, xStatueAngle);
										xSetInt(db, xStatueState, 0);
										trUnitConvert(ENEMY_PLAYER);
										trDamageUnitPercent(-100);
										trQuestVarSet("movingStatuesIn"+room, trQuestVarGet("movingStatuesIn"+room) - 1);
									} else {
										angle = fModulo(6.283185, xGetFloat(db, xStatueAngle) - angle * 1.570796);
									}
									trVectorSetFromAngle("dir", angle);
									trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
								}
							}
						}
						if ((trQuestVarGet("correctStatuesIn"+room) == xGetDatabaseCount(db)) &&
							trQuestVarGet("movingStatuesIn"+room) == 0) {
							if (xGetInt(dChests,xChestCount) == 0) { // temple?
								xSetInt(dChests,xChestState,CHEST_STATE_UNLOCKED);
							} else {
								trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"),1);
								startNPCDialog(NPC_TEMPLE_COMPLETE + 8);
								xFreeDatabaseBlock(dChests);
							}
							trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
							for(x=xGetDatabaseCount(db); >0) {
								xDatabaseNext(db);
								xUnitSelect(db,xUnitName);
								trDamageUnitPercent(-100);
								trUnitConvert(0);
							}
						}
					}
					case CHEST_ENCOUNTER:
					{
						if (trUnitIsSelected()) {
							reselectMyself();
							uiMessageBox("You must defeat all the enemies in this room to open this chest.");
						}
						bool allDead = true;
						for(x=xGetInt(dChests, xChestSFX); < xGetInt(dChests, xChestKey)) {
							trUnitSelectClear();
							trUnitSelect(""+x);
							if (trUnitAlive()) {
								allDead = false;
								break;
							}
						}
						if (allDead) {
							xSetInt(dChests,xChestState,CHEST_STATE_UNLOCKED);
						}
					}
				}
			}
			case CHEST_STATE_UNLOCKED:
			{
				trQuestVarSet("chestCount", 1 + trQuestVarGet("chestCount"));
				xUnitSelect(dChests,xUnitName);
				trUnitSetAnimation("SE_Great_Box_Opening",false,-1);
				trSoundPlayFN("siegetowerdeath.wav","1",-1,"","");
				trQuestVarSetFromRand("rand", 1, 10, true);
				if (trQuestVarGet("rand") == 1) {
					xSetInt(dChests, xChestState, CHEST_STATE_COUNTDOWN);
					trUnitHighlight(4.0, true);
					trUnitChangeProtoUnit("Phoenix Egg");
					xSetInt(dChests,xChestSFX, trTimeMS() + 1000);
					xSetInt(dChests,xChestCount, 3);
					trSoundPlayFN("attackwarning.wav","1",-1,"","");
					trMessageSetText("The chest was a bomb! Run!",-1);
				} else {
					room = xGetInt(dChests,xChestRoom);
					trQuestVarSetFromRand("rand", 1, 1*trQuestVarGet("rand"), true);
					trQuestVarSet("rand", trQuestVarGet("rand") + trQuestVarGet("correctStatuesIn"+room));
					if (trQuestVarGet("rand") < ENEMY_PLAYER) {
						trQuestVarSet("rand", ENEMY_PLAYER);
					}
					xSetInt(dChests,xChestState,CHEST_STATE_REWARDING);
					xSetInt(dChests,xChestSFX, trTimeMS());
					xSetInt(dChests,xChestCount,1*trQuestVarGet("rand"));
					trSoundPlayFN("plentybirth.wav","1",-1,"","");
				}
			}
			case CHEST_STATE_REWARDING:
			{
				if (trTimeMS() > xGetInt(dChests, xChestSFX)) {
					if (xGetInt(dChests, xChestCount) == 0) {
						xFreeDatabaseBlock(dChests);
					} else {
						trSoundPlayFN("tributereceived.wav","1",-1,"","");
						xSetInt(dChests,xChestSFX,trTimeMS() + 500);
						pos = kbGetBlockPosition(""+xGetInt(dChests,xUnitName));
						xSetPointer(dRainingRelics, xAddDatabaseBlock(dRainingRelics));
						xSetInt(dRainingRelics,xUnitName,trGetNextUnitScenarioNameNumber());
						xSetInt(dRainingRelics,xRainingRelicMorphed,1);
						trQuestVarSetFromRand("heading",1,360,true);
						trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
						trQuestVarSetFromRand("speed", 2, 10);
						zSetProtoUnitStat("Kronny Flying", 0, 1, trQuestVarGet("speed"));
						trArmySelect("0,0");
						trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
						trDamageUnitPercent(100);
						trSetSelectedScale(0,-3,0);
						xSetInt(dChests,xChestCount,xGetInt(dChests,xChestCount) - 1);
					}
				}
			}
			case CHEST_STATE_COUNTDOWN:
			{
				xUnitSelect(dChests,xUnitName);
				float scale = 4.0 - xGetInt(dChests, xChestCount) + 0.001 * (trTimeMS() - xGetInt(dChests, xChestSFX) + 1000);
				trSetSelectedScale(scale, scale, scale);
				if (trTimeMS() > xGetInt(dChests, xChestSFX)) {
					xSetInt(dChests, xChestSFX, trTimeMS() + 1000);
					xSetInt(dChests, xChestCount, xGetInt(dChests, xChestCount) - 1);
					if (xGetInt(dChests, xChestCount) == 0) {
						pos = kbGetBlockPosition(""+xGetInt(dChests,xUnitName));
						trQuestVarSet("heading", 0);
						zSetProtoUnitStat("Kronny Flying", 0, 1, 15);
						for(x=18; >0) {
							xSetPointer(dRainingRelics,xAddDatabaseBlock(dRainingRelics));
							xSetInt(dRainingRelics,xUnitName,trGetNextUnitScenarioNameNumber());
							xSetInt(dRainingRelics,xRainingRelicMorphed,2);
							trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
							trArmySelect("0,0");
							trDamageUnitPercent(100);
							trMutateSelected(kbGetProtoUnitID("Kronny Flying"));
							trSetSelectedScale(0,-2.0,0);
							trQuestVarSet("heading", trQuestVarGet("heading") + 20);
						}
						
						xSetPointer(dChestHitbox,xAddDatabaseBlock(dChestHitbox));
						xSetVector(dChestHitbox,xChestHitboxCenter,pos);
						xSetInt(dChestHitbox,xChestHitboxStart,trTimeMS());
						
						xUnitSelect(dChests,xUnitName);
						trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
						if (trUnitVisToPlayer()) {
							trUIFadeToColor(255,255,255,1000,0,false);
							trCameraShake(3.0, 0.25);
						}
						trDamageUnitPercent(100);
						xFreeDatabaseBlock(dChests);
					} else {
						trSoundPlayFN("attackwarning.wav","1",-1,"","");
					}
				}
			}
		}
	}
}
