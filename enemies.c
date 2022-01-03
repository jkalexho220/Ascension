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
            yAddUpdateVar("MountainGiants", "index", yGetNewestPointer("enemies"));
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
        setupProtounitBounty("Walking Woods Marsh", 5);
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
    int action = 0;
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
            trUnitChangeProtoUnit("Sphinx");
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
            trUnitChangeProtoUnit("Mountain Giant");
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
                        ySetVarFromVector("MountainGiants", "end", "end");

                        ySetVar("MountainGiants", "next", trTimeMS() + 1800);
                        ySetVar("MountainGiants", "step", 1);
                        trUnitOverrideAnimation(39,0,false,false,-1);
                    }
                }
                case 1:
                {
                    trVectorSetUnitPos("pos", "MountainGiants");
                    yVarToVector("MountainGiants", "end");
                    for(x=yGetDatabaseCount("playerUnits"); >0) {
                        id = yDatabaseNext("playerUnits", true);
                        if (id == -1 || trUnitAlive() == false) {
                            removePlayerUnit();
                        } else if (zDistanceToVectorSquared("playerUnits", "end") < 4) {
                            damagePlayerUnit(300);
                            if (yGetVar("playerUnits", "hero") == 1 && trCurrentPlayer() == yGetVar("playerUnits", "player")) {
                                trCameraShake(0.7, 0.7);
                            }
                        }
                    }
                    trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("endx"),0,trQuestVarGet("endz"),45,true);
                    trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("endx"),0,trQuestVarGet("endz"),135,false);
                    trArmySelect("1,0");
                    trUnitChangeProtoUnit("Tartarian Gate Flame");
                    ySetVar("MountainGiants", "step", 2);
                    ySetVar("MountainGiants", "next", trTimeMS() + 1200);
                    
                }
                case 2:
                {
                    ySetVar("MountainGiants", "step", 0);
                    ySetVar("MountainGiants", "next", trTimeMS() + 15000);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        } else if (yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("MountainGiants", "index")) > 0 &&
            yGetVar("MountainGiants", "step") == 1) {
            ySetVar("MountainGiants", "step", 0);
            ySetVar("MountainGiants", "next", trTimeMS() + 15000);
        }
    }

    for(x=xsMin(4, yGetDatabaseCount("MedusaBalls")); >0) {
        action = processGenericProj("MedusaBalls");
        trVectorSetUnitPos("pos", "MedusaBalls");
        vectorToGrid("pos", "loc");
        for(y=yGetDatabaseCount("playerUnits"); >0) {
            id = yDatabaseNext("playerUnits", true);
            if (id == -1 || trUnitAlive() == false) {
                removePlayerUnit();
            } else if (zDistanceToVectorSquared("playerUnits", "pos") < 2.0) {
                stunUnit("playerUnits", 3.0, 0 - yGetVar("playerUnits", "player"));
                action = PROJ_REMOVE;
                break;
            }
        }
        if (action == PROJ_FALLING) {
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("MedusaBalls", "target"));
            if (trUnitAlive()) {
                trVectorQuestVarSet("end", kbGetBlockPosition(""+1*yGetVar("MedusaBalls", "target")));
                trVectorQuestVarSet("dir", zGetUnitVector("pos", "end"));
                ySetVarFromVector("MedusaBalls", "dir", "dir");
            }
        } else if (action == PROJ_BOUNCE) {
            ySetVar("MedusaBalls", "bounces", yGetVar("MedusaBalls", "bounces") - 1);
        } else if (action == PROJ_REMOVE ||
            terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) ||
            yGetVar("MedusaBalls", "bounces") <= 0) {
            trUnitSelectClear();
            trUnitSelectByQV("MedusaBalls");
            trUnitChangeProtoUnit("Conversion SFX");
            yRemoveFromDatabase("MedusaBalls");
        }
    }

    if (yGetDatabaseCount("Medusas") >0) {
        id = yDatabaseNext("Medusas", true);
        if (id == -1 || trUnitAlive() == false) {
            trUnitChangeProtoUnit("Medusa");
            yRemoveFromDatabase("Medusas");
            yRemoveUpdateVar("Medusas", "step");
        } else if (trTimeMS() > yGetVar("Medusas", "next")) {
            switch(1*yGetVar("Medusas", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 12) {
                        target = kbUnitGetTargetUnitID(id);
                        ySetVar("Medusas", "target", trGetUnitScenarioNameNumber(target));

                        ySetVar("Medusas", "step", 1);
                        ySetVar("Medusas", "next", trTimeMS() + 1200);
                        trUnitOverrideAnimation(40,0,false,false,-1);
                    }
                }
                case 1:
                {
                    trVectorQuestVarSet("end", kbGetBlockPosition(""+1*yGetVar("Medusas", "target")));
                    trVectorSetUnitPos("start", "Medusas");
                    trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
                    addGenericProj("MedusaBalls","start","dir",kbGetProtoUnitID("Curse SFX"),2,3,5);
                    yAddUpdateVar("MedusaBalls", "target", yGetVar("Medusas", "target"));
                    yAddUpdateVar("MedusaBalls", "bounces", 12);
                    ySetVar("Medusas", "step", 2);
                    ySetVar("Medusas", "next", trTimeMS() + 800);
                }
                case 2:
                {
                    ySetVar("Medusas", "step", 0);
                    ySetVar("Medusas", "next", trTimeMS() + 18000);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        } else if (yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("Medusas", "index")) > 0 &&
            yGetVar("Medusas", "step") == 1) {
            ySetVar("Medusas", "step", 0);
            ySetVar("Medusas", "next", trTimeMS() + 18000);
        }
    }

    if (yGetDatabaseCount("Dryads") > 0) {
        id = yDatabaseNext("Dryads", true);
        if (id == -1 || trUnitAlive() == false) {
            yVarToVector("Dryads", "pos");
            trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
            trArmySelect("1,0");
            trUnitChangeProtoUnit("Lampades Blood");
            for(x=yGetDatabaseCount("playerUnits"); >0) {
                id = yDatabaseNext("playerUnits", true);
                if (id == -1 || trUnitAlive() == false) {
                    removePlayerUnit();
                } else if (zDistanceToVectorSquared("playerUnits", "pos") < 16) {
                    poisonUnit("playerUnits", 12.0, 10.0);
                }
            }
            yRemoveFromDatabase("Dryads");
        } else {
            trVectorSetUnitPos("pos", "Dryads");
            ySetVarFromVector("Dryads", "pos", "pos");
        }
    }

    xsSetContextPlayer(old);
}

