void removeSavior(int p = 0) {
	removePlayerSpecific(p);
	
}

void saviorAlways(int eventID = -1) {
	int p = eventID - 12 * SAVIOR;
	int id = 0;
	int hit = 0;
	int target = 0;
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trVectorSetUnitPos("pos", "p"+p+"lureObject");
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"lureObject", true);
		trMutateSelected(kbGetProtoUnitID("Rocket"));
		trUnitDestroy();
		
	}

	if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
		
	}

	xsSetContextPlayer(p);
	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeSavior(p);
		} else {
			hit = CheckOnHit(p, id);
			if (hit == ON_HIT_JUMP) {
				if (yGetVar("p"+p+"characters", "jumping") == 0) {
					ySetVar("p"+p+"characters", "jumping", 1);
					ySetVar("p"+p+"characters", "attackTarget", kbUnitGetTargetUnitID(id));
					for(x=yGetDatabaseCount("enemies"); >0) {
                		yDatabaseNext("enemies");
                		if (kbGetBlockID(""+1*trQuestVarGet("enemies")) == yGetVar("p"+p+"characters", "attackTarget")) {
                			ySetVar("p"+p+"characters", "attackTargetIndex", yGetPointer("enemies"));
                			trQuestVarSet("p"+p+"poisonKillerActive", yGetVar("enemies", "poisonStatus"));
                			break;
                		}
                	}
				}
			} else if (yGetVar("p"+p+"characters", "jumping") == 1) {

			} else if (hit >= ON_HIT_NORMAL) {
				if (hit == ON_HIT_SPECIAL) {
					if (ySetPointer("enemies", 1*yGetVar("p"+p+"characters", "attackTargetIndex"))) {
						trPlayerGrantResources(p, "favor", 3);
						
					}
				}
			}
		}
	}


	poisonKillerBonus(p);
	xsSetContextPlayer(old);
}

void chooseSavior(int eventID = -1) {
	int p = eventID - 1000 - 12 * SAVIOR;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
		wellName = "(Q) Guardian Angel";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Intervention";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Unity";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("guardianAngelCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("unityCooldown"));
	trQuestVarSet("p"+p+"lureCost", trQuestVarGet("unityCost"));
	trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("interventionCooldown"));
	trQuestVarSet("p"+p+"rainCost", trQuestVarGet("interventionCost"));
}


rule savior_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * SAVIOR + p, "saviorAlways");
		trEventSetHandler(1000 + 12 * SAVIOR + p, "chooseSavior");
	}

	trQuestVarSet("guardianAngelCooldown", 8);

	trQuestVarSet("unityCooldown", 16);
	trQuestVarSet("unityRadius", 8);
	trQuestVarSet("unityDuration", 8);
	trQuestVarSet("unityBonus", 0.2);

	trQuestVarSet("interventionCost", 100);
	trQuestVarSet("interventionCooldown", 30);
}