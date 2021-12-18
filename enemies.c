void setupProtounitBounty(string proto = "", int bounty = 2) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("proto"+p+"bounty", bounty);
	trModifyProtounit(proto, ENEMY_PLAYER, 24, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 25, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 26, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 6, 6);
}

void setupProtounitResist(string proto = "", int element = 0, float resist = 0) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("proto"+p+"resist"+element, resist);
}

void activateEnemy(int id = 0) {
    int proto = kbGetUnitBaseTypeID(id);
    int bounty = trQuestVarGet("proto"+proto+"bounty");

    yAddToDatabase("enemies", "enemiesIncoming");
    trQuestVarSetFromRand("bounty", bounty / 2, bounty, true);
    yAddUpdateVar("enemies", "bounty", trQuestVarGet("bounty"));
    for(i = NONE; <= DARK) {
    	yAddUpdateVar("enemies", "resist"+i, trQuestVarGet("proto"+proto+"resist"+i));
    }
}

rule setup_enemies
inactive
highFrequency
{
	setupProtounitBounty("Anubite", 5);
	setupProtounitResist("Anubite", ICE, -0.5);

	setupProtounitBounty("Shade of Hades", 5);
	setupProtounitResist("Shade of Hades", LIGHT, -1);
	setupProtounitResist("Shade of Hades", DARK, 1);
	xsDisableSelf();
}

rule enemies_always
inactive
highFrequency
{
    int old = xsGetContextPlayer();
    int proto = 0;
    int id = 0;
    /*
    Enemies incoming
    */
    id = yDatabaseNext("enemiesIncoming", true);
    for(p=1; < ENEMY_PLAYER) {
        if (trUnitHasLOS(p)) {
            activateEnemy(id);
            break;
        }
    }

    /*
    Projectiles from attacks
    */
    xsSetContextPlayer(ENEMY_PLAYER);
    while(trQuestVarGet("nextProj") < trGetNextUnitScenarioNameNumber()) {
        id = kbGetBlockID(""+1*trQuestVarGet("nextProj"), true);
        proto = kbGetUnitBaseTypeID(id);
        if ((proto == kbGetProtoUnitID("Sling Stone")) ||
            (proto == kbGetProtoUnitID("Arrow Flaming")) ||
            (proto == kbGetProtoUnitID("Javelin Flaming")) ||
            (proto == kbGetProtoUnitID("Axe"))) {
            trUnitSelectClear();
            trUnitSelectByQV("nextProj");
            if (trUnitIsOwnedBy(ENEMY_PLAYER)) {
                /*
                Enemy projectiles.
                Do something fancy
                */
            } else {
                trUnitDestroy();
            }
        }
        trQuestVarSet("nextProj", 1 + trQuestVarGet("nextProj"));
    }

    xsSetContextPlayer(old);
}