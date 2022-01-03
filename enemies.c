void setupProtounitBounty(string proto = "", int bounty = 2, float relicChance = 0, int relic = -1) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("proto"+p+"bounty", bounty);
    trQuestVarSet("proto"+p+"relicChance", relicChance);
    trQuestVarSet("proto"+p+"relic", relic);
	/* armor */
	trModifyProtounit(proto, ENEMY_PLAYER, 24, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 25, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 26, -1);
	trModifyProtounit(proto, 1, 24, -1);
	trModifyProtounit(proto, 1, 25, -1);
	trModifyProtounit(proto, 1, 26, -1);
	/* LOS */
	trModifyProtounit(proto, ENEMY_PLAYER, 6, 10);
	trModifyProtounit(proto, 1, 6, 10);
}

void activateEnemy(int id = 0) {
    int proto = kbGetUnitBaseTypeID(id);
    int bounty = trQuestVarGet("proto"+proto+"bounty");
    int relic = 0;
    trQuestVarSetFromRand("relicChance", 0, 1, false);
    if (trQuestVarGet("relicChance") < trQuestVarGet("proto"+proto+"relicChance")) {
        relic = trQuestVarGet("proto"+proto+"relic");
        if (relic == -1) {
            relic = trQuestVarGet("stage");
        }
    }

    yAddToDatabase("enemies", "enemiesIncoming");
    trQuestVarSetFromRand("bounty", bounty / 2, bounty, true);
    yAddUpdateVar("enemies", "bounty", trQuestVarGet("bounty"));
    yAddUpdateVar("enemies", "relic", relic);
    for(p=1; < ENEMY_PLAYER) {
        if (trQuestVarGet("p"+p+"rideLightning") == 1) {
            yAddToDatabase("p"+p+"rideLightningTargets", "enemiesIncoming");
            yAddUpdateVar("p"+p+"rideLightningTargets", "index", yGetNewestPointer("enemies"));
        }
    }

    switch(proto)
    {
        case kbGetProtoUnitID("Sphinx"):
        {
            yAddToDatabase("Sphinxes", "enemiesIncoming");
        }
        case kbGetProtoUnitID("Dryad"):
        {
            yAddToDatabase("Dryads", "enemiesIncoming");
        }
        case kbGetProtoUnitID("Medusa"):
        {
            yAddToDatabase("Medusas", "enemiesIncoming");
        }
        case kbGetProtoUnitID("Mountain Giant"):
        {
            yAddToDatabase("MountainGiants", "enemiesIncoming");
        }
    }

    yRemoveFromDatabase("enemiesIncoming");
}

rule setup_enemies
inactive
highFrequency
{
	if (trTime() > cActivationTime) {
		trModifyProtounit("Minion", ENEMY_PLAYER, 8, -99);
		trModifyProtounit("Minion", 1, 8, -99);
		
        setupProtounitBounty("Golden Lion", 4);
        setupProtounitBounty("Anubite", 6, 0.03);
        setupProtounitBounty("Terracotta Soldier", 6, 0.03);
		setupProtounitBounty("Sphinx", 8, 0.05);
		setupProtounitBounty("Petsuchos", 8, 0.1, RELIC_ATTACK_RANGE);

        setupProtounitBounty("Minion", 4);
        setupProtounitBounty("Walking Marsh", 5);
        setupProtounitBounty("Dryad", 7, 0.03);
        setupProtounitBounty("Centaur", 6, 0.03);
        setupProtounitBounty("Medusa", 14, 0.15, RELIC_SPELL_DURATION);
        setupProtounitBounty("Mountain Giant", 12, 0.1);
		
		xsDisableSelf();
	}
}

void enemiesAlways() {
    int old = xsGetContextPlayer();
    int proto = 0;
    int id = 0;
    int target = 0;
    float amt = 0;
    float angle = 0;
    string pName = "";
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
            }
        }
        trQuestVarSet("nextProj", 1 + trQuestVarGet("nextProj"));
    }

    if (yGetDatabaseCount("enemies") > 0) {
    	id = yDatabaseNext("enemies", true);
	    if ((id == -1) || (trUnitAlive() == false)) {
	    	removeEnemy();
	    } else {
	    	stunsAndPoisons("enemies");
            trVectorSetUnitPos("pos", "enemies");
            ySetVar("enemies", "posX", trQuestVarGet("posX"));
            ySetVar("enemies", "posZ", trQuestVarGet("posZ"));
	    }
    }

    /* ambush rooms */
    if (yGetDatabaseCount("ambushRooms") > 0) {
        yDatabaseNext("ambushRooms");
        trQuestVarSet("posX", yGetVar("ambushRooms", "posX"));
        trQuestVarSet("posZ", yGetVar("ambushRooms", "posZ"));
        for(p=1; < ENEMY_PLAYER) {
            if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 100) {
                pName = trStringQuestVarGet("enemyProto"+1*yGetVar("ambushRooms", "type"));
                trQuestVarSetFromRand("count", trQuestVarGet("stage"), 11, true);
                angle = 6.283185 / trQuestVarGet("count");
                amt = 0;
                trQuestVarSet("angle",0);
                for(x=trQuestVarGet("count"); >0) {
                    trQuestVarSetFromRand("dist", 6, 18, true);
                    trVectorSetFromAngle("dir", trQuestVarGet("angle"));
                    trVectorScale("dir", trQuestVarGet("dist"));
                    trQuestVarSet("dirX", trQuestVarGet("posX") + trQuestVarGet("dirX"));
                    trQuestVarSet("dirZ", trQuestVarGet("posZ") + trQuestVarGet("dirZ"));
                    amt = fModulo(360.0, trQuestVarGet("angle") * 180.0 / 3.141592 - 180.0);
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trArmyDispatch("1,0",pName,1,trQuestVarGet("dirX"),0,trQuestVarGet("dirZ"),amt,true);
                    trUnitSelectClear();
                    trUnitSelectByQV("next", true);
                    trUnitConvert(ENEMY_PLAYER);
                    yAddToDatabase("enemiesIncoming", "next");
                    trQuestVarSet("angle", trQuestVarGet("angle") + angle);
                }
                for(x=trQuestVarGet("count"); >0) {
                    id = yDatabaseNext("enemiesIncoming", true);
                    activateEnemy(id);
                }
                trSoundPlayFN("attackwarning.wav","1",-1,"","");
                trSoundPlayFN("wild.wav","1",-1,"","");
                yRemoveFromDatabase("ambushRooms");
                yRemoveUpdateVar("ambushRooms", "posX");
                yRemoveUpdateVar("ambushRooms", "posZ");
                yRemoveUpdateVar("ambushRooms", "type");
                break;
            }
        }
    }

    if (yGetDatabaseCount("sphinxes") > 0) {
        id = yDatabaseNext("sphinxes", true);
        if (id == -1 || trUnitAlive() == false) {
            trUnitOverrideAnimation(-1,0,false,true,-1);
            yRemoveFromDatabase("sphinxes");
            yRemoveUpdateVar("sphinxes", "step");
        } else if (trTimeMS() > yGetVar("sphinxes", "next")) {
            switch(1*yGetVar("sphinxes", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 6) {
                        ySetVar("sphinxes", "next", trTimeMS() + 1600);
                        ySetVar("sphinxes", "step", 1);
                        trUnitOverrideAnimation(39,0,false,false,-1);
                        trVectorSetUnitPos("pos", "sphinxes");
                        for(p=1; < ENEMY_PLAYER) {
                            if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 16) {
                                silencePlayer(p, 5);
                            }
                        }
                    }
                }
                case 1:
                {
                    ySetVar("sphinxes", "next", trTimeMS() + 10000);
                    ySetVar("sphinxes", "step", 0);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        }
    }

    if (yGetDatabaseCount("MountainGiants") > 0) {
        id = yDatabaseNext("MountainGiants", true);
        if (id == -1 || trUnitAlive() == false) {
            trUnitOverrideAnimation(-1,0,false,true,-1);
            yRemoveFromDatabase("MountainGiants");
            yRemoveUpdateVar("MountainGiants", "step");
        } else if (trTimeMS() > yGetVar("MountainGiants", "next")) {
            switch(1*yGetVar("MountainGiants", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 6) {
                        target = kbUnitGetTargetUnitID(id);
                        trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
                        trVectorSetUnitPos("start", "MountainGiants");
                        trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
                        ySetVarFromVector("MountainGiants", "dir", "dir");

                        ySetVar("MountainGiants", "next", trTimeMS() + 1800);
                        ySetVar("MountainGiants", "step", 1);
                        trUnitOverrideAnimation(39,0,false,false,-1);
                    }
                }
                case 1:
                {
                    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                    trVectorSetUnitPos("pos", "MountainGiants");
                    yVarToVector("MountainGiants", "dir");
                }
            }
        }
    }

    xsSetContextPlayer(old);
}

