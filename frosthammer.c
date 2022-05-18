int icicleCooldown = 5;
float icicleDuration = 8;
float icicleDamage = 80;
float icicleRadius = 1.5;
float icicleHealRange = 4;
float icicleRange = 4;

int xIciclePos = 0;
int xIcicleTimeout = 0;
int xIcicleLast = 0;
int xIcicleStep = 0;
int xIcicleRadius = 0;

int titanicImpactCooldown = 15;
float smashRange = 6;
float smashWidth = 2;

int xSmashAngle = 0;
int xSmashVelocity = 0;
int xSmashScale = 0;
int xSmashDir = 0;

int blizzardCost = 80;
float blizzardDuration = 12;
float blizzardRadius = 10;

void removeFrostHammer(int p = 0) {
	removePlayerSpecific(p);
	
}

void frosthammerAlways(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 12 * FROSTHAMMER;
	pvpDetachPlayer(p);
	int id = 0;
	int hit = 0;
	int target = 0;
	int next = 0;
	int index = xGetPointer(dEnemies);
	int db = getCharactersDB(p);
	int icicles = trQuestVarGet("p"+p+"icicles");
	int smashes = trQuestVarGet("p"+p+"titanicSmashes");
	float amt = 0;
	float dist = 0;
	float current = 0;
	xSetPointer(dPlayerData, p);
	
	vector start = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);

	for (x=xsMin(3, xGetDatabaseCount(icicles)); > 0) {
		xDatabaseNext(icicles);
		if (xGetBool(icicles, xIcicleStep)) {
			amt = (1.0 * xGetInt(icicles, xIcicleTimeout) - trTimeMS()) / icicleDuration / 1000;
			amt = 1.0 + 3.0 * amt;
			xUnitSelect(icicles, xUnitName);
			if (amt <= 1.0) {
				trUnitSelect(""+(xGetInt(icicles, xUnitName) + 1), true);
				trUnitDestroy();
				xFreeDatabaseBlock(icicles);
			} else {
				trSetSelectedScale(xGetFloat(icicles, xIcicleRadius),amt,xGetFloat(icicles, xIcicleRadius));
				amt = trTimeMS() - xGetInt(icicles, xIcicleLast);
				if (amt > 500) {
					xSetInt(icicles, xIcicleLast, trTimeMS());
					amt = amt * 0.000005 * xGetFloat(dPlayerData, xPlayerHealth);
					dist = icicleHealRange * xGetFloat(icicles, xIcicleRadius);
					pos = xGetVector(icicles, xIciclePos);
					for(y=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db, xUnitID);
						if (trUnitAlive() == false) {
							removeFrostHammer();
						} else if (unitDistanceToVector(xGetInt(db, xUnitName), pos) < dist) {
							healUnit(p, amt);
						}
					}
				}
			}
		} else {
			xUnitSelect(icicles, xUnitName);
			amt = (150.0 - xGetInt(icicles, xIcicleTimeout) + trTimeMS()) / 150.0;
			if (amt >= 1.0) {
				xSetBool(icicles, xIcicleStep, true);
				xSetInt(icicles, xIcicleTimeout, xGetInt(icicles, xIcicleTimeout) + icicleDuration * xGetFloat(dPlayerData, xPlayerSpellDuration) * 1000);
				xSetInt(icicles, xIcicleLast, trTimeMS());
				amt = 1.0;
			}
			amt = 1.0 + 3.0 * amt;
			trSetSelectedScale(xGetFloat(icicles, xIcicleRadius),amt,xGetFloat(icicles, xIcicleRadius));
		}
	}
	
	if (xGetBool(dPlayerData, xPlayerWellActivated)) {
		xSetBool(dPlayerData, xPlayerWellActivated, false);
		trQuestVarSetFromRand("heading", 0, 360, true);
		next = trGetNextUnitScenarioNameNumber();
		pos = vectorSnapToGrid(xGetVector(dPlayerData, xPlayerWellPos));
		trArmyDispatch(""+p+",0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
		trUnitSelectClear();
		trUnitSelect(""+(next+1),true);
		trMutateSelected(kbGetProtoUnitID("Frost Drift"));
		trUnitOverrideAnimation(2,0,true,true,-1);
		trSetSelectedScale(1.25 * xGetFloat(dPlayerData, xPlayerSpellRange),1,1.6*xGetFloat(dPlayerData, xPlayerSpellRange));

		trUnitSelectClear();
		trUnitSelect(""+next, true);
		trMutateSelected(kbGetProtoUnitID("Ice Block"));
		trUnitSetAnimationPath("0,0,0,0,0,0,0");
		trSetSelectedScale(icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange),0,icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange));

		dist = xsPow(icicleRadius * xGetFloat(dPlayerData, xPlayerSpellRange), 2);
		amt = dist;
		hit = 0;
		for(x=xGetDatabaseCount(dEnemies); >0) {
			xDatabaseNext(dEnemies);
			xUnitSelectByID(dEnemies, xUnitID);
			if (trUnitAlive() == false) {
				removeEnemy();
			} else {
				current = unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos);
				if (current < dist) {
					hit = hit + 1;
					damageEnemy(p, icicleDamage * xGetFloat(dPlayerData, xPlayerSpellDamage));
					if (current < amt) {
						amt = current;
						target = xGetPointer(dEnemies);
					}
				}
			}
		}
		if (hit >= 1) {
			gainFavor(p, hit);
			xSetPointer(dEnemies, target);
			stunUnit(dEnemies, 2, p);
			trQuestVarSetFromRand("sound", 1, 2, true);
			trSoundPlayFN("titanpunch"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}

		xAddDatabaseBlock(icicles, true);
		xSetInt(icicles, xUnitName, next);
		xSetInt(icicles, xIcicleTimeout, trTimeMS() + 150);
		xSetFloat(icicles, xIcicleRadius, xGetFloat(dPlayerData, xPlayerSpellRange));
		xSetVector(icicles, xIciclePos, pos);
	}
	
	if (xGetBool(dPlayerData, xPlayerLureActivated)) {
		xSetBool(dPlayerData, xPlayerLureActivated, false);
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		
		
	}
	
	if (xGetBool(dPlayerData, xPlayerRainActivated)) {
		xSetBool(dPlayerData, xPlayerRainActivated, false);
		
	}
	
	
	if (xGetDatabaseCount(db) > 0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			removeFrostHammer(p);
		} else {
			hit = CheckOnHit(p);
			if (hit == ON_HIT_SPECIAL) {

			}
		}
	}
	
	
	xSetPointer(dEnemies, index);
	poisonKillerBonus(p);
	pvpReattachPlayer();
}

void chooseFrostHammer(int eventID = -1) {
	xsSetContextPlayer(0);
	int p = eventID - 1000 - 12 * FROSTHAMMER;
	int db = getCharactersDB(p);
	resetCharacterCustomVars(p);
	xSetPointer(dPlayerData, p);
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Icicle";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Blizzard";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Titanic Impact";
		lureIsUltimate = false;
	}
	
	xSetInt(dPlayerData,xPlayerWellCooldown, icicleCooldown);
	xSetFloat(dPlayerData,xPlayerWellCost,0);
	xSetInt(dPlayerData,xPlayerLureCooldown, titanicImpactCooldown);
	xSetFloat(dPlayerData,xPlayerLureCost, 0);
	xSetInt(dPlayerData,xPlayerRainCooldown,1);
	xSetFloat(dPlayerData,xPlayerRainCost, blizzardCost);

	if (trQuestVarGet("p"+p+"icicles") == 0) {
		db = xInitDatabase("p"+p+"icicles");
		trQuestVarSet("p"+p+"icicles", db);
		xInitAddInt(db, "name");
		xIcicleTimeout = xInitAddInt(db, "timeout");
		xIcicleLast = xInitAddInt(db, "last");
		xIciclePos = xInitAddVector(db, "pos");
		xIcicleStep = xInitAddBool(db, "step", false);
		xIcicleRadius = xInitAddFloat(db, "radius");
	}

	if (trQuestVarGet("p"+p+"titanicSmashes") == 0) {
		db = xInitDatabase("p"+p+"titanicSmashes");
		trQuestVarSet("p"+p+"titanicSmashes", db);
		xInitAddInt(db, "name");
		xSmashAngle = xInitAddFloat(db, "angle");
		xSmashVelocity = xInitAddFloat(db, "velocity");
		xSmashScale = xInitAddFloat(db, "scale");
		xSmashDir = xInitAddVector(db, "dir");
	}
}


rule frosthammer_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * FROSTHAMMER + p, "frosthammerAlways");
		trEventSetHandler(1000 + 12 * FROSTHAMMER + p, "chooseFrostHammer");
	}
}
