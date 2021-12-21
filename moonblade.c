
void moonbladeAlways(int eventID = -1) {
	int p = eventID - 12*MOONBLADE;
	if (trQuestVarGet("p"+p+"visionStatus") == ABILITY_ON) {
		trQuestVarSet("p"+p+"visionStatus", ABILITY_OFF);
	}
}

void chooseMoonblade(int eventID = -1) {
	int p = eventID - 1000 - 12 * MOONBLADE;
	if (trCurrentPlayer() == p) {
		map("q", "game", "uiSetSpecialPower(220) uiSpecialPowerAtPointer");
		visionName = "(Q) Moonbeam";
		visionIsUltimate = false;
		map("e", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
		rainName = "(E) Protection";
		rainIsUltimate = true;
		map("w", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
		lureName = "(W) Crescent Strikes";
		lureIsUltimate = false;
	}
	trQuestVarSet("p"+p+"visionCooldown", trQuestVarGet("moonbeamCooldown"));
	trQuestVarSet("p"+p+"visionCost", 0);
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