void removeMoonblade(int p = 0) {
	removePlayerCharacter(p);
	yRemoveUpdateVar("p"+p+"characters", "crescentCount");
	yRemoveUpdateVar("p"+p+"characters", "crescentDamage");
	yRemoveUpdateVar("p"+p+"characters", "crescentTimeout");
	yRemoveUpdateVar("p"+p+"characters", "crescentSFX");
	yRemoveUpdateVar("p"+p+"characters", "crescentOn");
}

void moonbladeAlways(int eventID = -1) {
	int p = eventID - 12 * MOONBLADE;
	int id = 0;
	int hit = 0;
	float angle = 0;

	if (yGetDatabaseCount("p"+p+"characters") > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeMoonblade(p);
		} else {
			hit = CheckOnHit(p, id);
			if ((yGetVar("p"+p+"characters", "crescentCount") > 0) && 
				(trTimeMS() > yGetVar("p"+p+"characters", "crescentTimeout"))) {
				ySetVar("p"+p+"characters", "crescentTimeout", trTimeMS() + 5000);
				ySetVar("p"+p+"characters", "crescentCount", yGetVar("p"+p+"characters", "crescentCount") - 1);
			}
		}
	}

	if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
		vectorSnapToGrid("p"+p+"wellPos");
		trUnitSelectClear();
		yAddToDatabase("moonbeams", "next");
		yAddUpdateVar("moonbeams", "player", p);
		yAddUpdateVar("moonbeams", "radius", trQuestVarGet("moonbeamRadius") * trQuestVarGet("p"+p+"spellRange"));
		yAddUpdateVar("moonbeams", "damage", trQuestVarGet("moonbeamDamage") * trQuestVarGet("p"+p+"spellDamage"));
		yAddUpdateVar("moonbeams", "damageBoost", trQuestVarGet("moonbeamDamageBoost") * trQuestVarGet("p"+p+"spellDamage"));
		yAddUpdateVar("moonbeams", "timeout", 
			trTimeMS() + 1000 * trQuestVarGet("moonbeamDuration") * trQuestVarGet("p"+p+"spellDuration"));
		yAddUpdateVar("moonbeams", "start", trGetNextUnitScenarioNameNumber());
		yAddUpdateVar("moonbeams", "posx", trQuestVarGet("p"+p+"wellPosx"));
		yAddUpdateVar("moonbeams", "posz", trQuestVarGet("p"+p+"wellPosz"));
		trUnitSelectClear();
		trUnitSelectByQV("p"+p+"wellObject", true);
		trUnitDestroy();
		trSoundPlayFN("eclipsebirth.wav","1",-1,"","");
		for(x=0; <16) {
			trVectorSetFromAngle("dir", angle);
			angle = angle + 6.283185 / 16;
			trQuestVarSet("dirx", 
				trQuestVarGet("dirx") * trQuestVarGet("moonbeamRadius") * trQuestVarGet("p"+p+"spellRange") + trQuestVarGet("posx"));
			trQuestVarSet("dirz", 
				trQuestVarGet("dirz") * trQuestVarGet("moonbeamRadius") * trQuestVarGet("p"+p+"spellRange") + trQuestVarGet("posz"));
			trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
			trArmyDispatch(""+p+",0","Dwarf",1,trQuestVarGet("dirx"),0,trQuestVarGet("dirz"),0,true);
			trUnitSelectClear();
			trUnitSelectByQV("next", true);
			trMutateSelected(kbGetProtoUnitID("Healing SFX"));
		}
	}

	if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
		trQuestVarSet("crescentStarted", 1);
		for(x=yGetDatabaseCount("p"+p+"Characters"); >0) {
			yDatabaseNext("p"+p+"Characters");
			if (yGetVar("p"+p+"Characters", "player") == p) {
				ySetVar("p"+p+"Characters", "crescentCount", yGetVar("p"+p+"Characters", "data1") + 3);
				ySetVar("p"+p+"Characters", "crescentDamage", trQuestVarGet("crescentDamage") * trQuestVarGet("p"+p+"spellDamage"));
				ySetVar("p"+p+"Characters", "crescentTimeout", trTimeMS() + 5000);
				ySetVar("p"+p+"characters", "crescentOn", 1);
				if (yGetVar("p"+p+"Characters", "crescentSFX") == 0) {
					ySetVar("p"+p+"Characters", "crescentSFX", 
						0 - spyEffect(1*trQuestVarGet("p"+p+"Characters"), kbGetProtoUnitID("Rocket")));
				}
			}
		}
	}

	if ((trQuestVarGet("crescentStarted") == 1) && (trQuestVarGet("spyFind") == trQuestVarGet("spyFound"))) {
		for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
			yDatabaseNext("p"+p+"characters");
			if (yGetVar("p"+p+"characters", "crescentSFX") < 0) {
				ySetVar("p"+p+"characters", "crescentSFX", trQuestVarGet("spyEye"+(1-yGetVar("p"+p+"characters", "crescentSFX"))));
			}
			trUnitSelectClear();
			trUnitSelect(""+1*yGetVar("p"+p+"characters", "crescentSFX"), true);
			trMutateSelected(kbGetProtoUnitID("Outpost"));
			trSetSelectedScale(0.0,0.0,0.0);
		}
	}
}

void chooseMoonblade(int eventID = -1) {
	int p = eventID - 1000 - 12 * MOONBLADE;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(366) uiSpecialPowerAtPointer uiSpecialPowerAtPointer");
		wellName = "(Q) Moonbeam";
		wellIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Protection";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Crescent Strikes";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("moonbeamCooldown"));
	trQuestVarSet("p"+p+"wellCost", 0);
	trQuestVarSet("p"+p+"lureCooldown", trQuestVarGet("crescentCooldown"));
	trQuestVarSet("p"+p+"lureCost", 0);
	trQuestVarSet("p"+p+"rainCooldown", 1);
	trQuestVarSet("p"+p+"rainCost", 0);
}

rule moonblade_init
active
highFrequency
{
	xsDisableSelf();
	for(p=1; < ENEMY_PLAYER) {
		trEventSetHandler(12 * MOONBLADE + p, "moonbladeAlways");
		trEventSetHandler(1000 + 12 * MOONBLADE + p, "chooseMoonblade");
	}
	trQuestVarSet("moonbeamCooldown", 18);
	trQuestVarSet("moonbeamDamage", 20);
	trQuestVarSet("moonbeamDamageBoost", 10);
	trQuestVarSet("moonbeamDuration", 6);
	trQuestVarSet("moonbeamRadius", 6);

	trQuestVarSet("crescentCooldown", 12);
	trQuestVarSet("crescentCount", 3);
	trQuestVarSet("crescentDamage", 50);

	trQuestVarSet("protectionCost", 12);
}