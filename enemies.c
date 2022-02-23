void setupProtounitBounty(string proto = "", float armor = 0, int bounty = 2, float relicChance = 0, int relic = -1) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("proto"+p+"bounty", bounty);
    trQuestVarSet("proto"+p+"relicChance", relicChance);
    trQuestVarSet("proto"+p+"relic", relic);
    trQuestVarSet("proto"+p+"armor", armor);
	/* armor */
	trModifyProtounit(proto, ENEMY_PLAYER, 24, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 25, -1);
	trModifyProtounit(proto, ENEMY_PLAYER, 26, -1);
    trModifyProtounit(proto, ENEMY_PLAYER, 24, armor);
    trModifyProtounit(proto, ENEMY_PLAYER, 25, armor);
    trModifyProtounit(proto, ENEMY_PLAYER, 26, armor);
	/* LOS */
	trModifyProtounit(proto, ENEMY_PLAYER, 6, 10);
	trModifyProtounit(proto, 1, 6, 10);

    /* damage bonus vs myth */
    trModifyProtounit(proto, ENEMY_PLAYER, 33, 9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 33, -9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 33, 1.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 44, 9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 44, -9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 44, 1.0);
    /* damage bonus vs hero */
    trModifyProtounit(proto, ENEMY_PLAYER, 34, 9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 34, -9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 34, 1.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 45, 9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 45, -9999999999999999999.0);
    trModifyProtounit(proto, ENEMY_PLAYER, 45, 1.0);
}

bool checkEnemyDeactivated(string db = "") {
    int index = 1*yGetVar(db, "index");
    if ((yGetVarAtIndex("enemies", "xActive", index) == 0) ||
        yGetUnitAtIndex("enemies", index) != trQuestVarGet(db)) {
        return(true);
    }
    return(false);
}

void activateEnemy(string db = "", int bounty = -1, int relic = -1) {
    int id = kbGetBlockID(""+1*trQuestVarGet(db));
    int proto = kbGetUnitBaseTypeID(id);
    if (bounty < 0) {
        bounty = trQuestVarGet("proto"+proto+"bounty");
    }
    if (relic < 0) {
        trQuestVarSetFromRand("relicChance", 0, 1, false);
        if (trQuestVarGet("relicChance") < trQuestVarGet("proto"+proto+"relicChance")) {
            relic = trQuestVarGet("proto"+proto+"relic");
            if (relic == -1) {
                relic = trQuestVarGet("stage");
            }
        }
    }
    
    trVectorSetUnitPos("spawnPos", db);
    yAddToDatabase("enemies", db);
    trQuestVarSetFromRand("bounty", bounty / 2, bounty, true);
    yAddUpdateVar("enemies", "bounty", trQuestVarGet("bounty"));
    yAddUpdateVar("enemies", "relic", relic);
    yAddUpdateVar("enemies", "posX", trQuestVarGet("spawnPosX"));
    yAddUpdateVar("enemies", "posZ", trQuestVarGet("spawnPosZ"));
    yAddUpdateVar("enemies", "stunStatus", 0);
    yAddUpdateVar("enemies", "stunTimeout", 0);
    yAddUpdateVar("enemies", "stunSFX", 0);
    yAddUpdateVar("enemies", "poisonStatus", 0);
    yAddUpdateVar("enemies", "poisonTimeout", 0);
    yAddUpdateVar("enemies", "poisonLast", 0);
    yAddUpdateVar("enemies", "poisonDamage", 0);
    yAddUpdateVar("enemies", "poisonSFX", 0);
    yAddUpdateVar("enemies", "launched", 0);
    yAddUpdateVar("enemies", "magicResist", trQuestVarGet("proto"+proto+"armor"));
    yAddUpdateVar("enemies", "physicalResist", trQuestVarGet("proto"+proto+"armor"));
    yAddUpdateVar("enemies", "silenceStatus", 0);
    yAddUpdateVar("enemies", "silenceTimeout", 0);
    yAddUpdateVar("enemies", "silenceSFX", 0);
    yAddUpdateVar("enemies", "bomb", 0);
    yAddUpdateVar("enemies", "deathSentence", 0);
    yAddUpdateVar("enemies", "missing", 0);
    for(p=1; < ENEMY_PLAYER) {
        if (trQuestVarGet("p"+p+"rideLightning") == 1) {
            yAddToDatabase("p"+p+"rideLightningTargets", db);
            yAddUpdateVar("p"+p+"rideLightningTargets", "index", yGetNewestPointer("enemies"));
        }
    }

    switch(proto)
    {
        case kbGetProtoUnitID("Sphinx"):
        {
            yAddToDatabase("Sphinxes", db);
            yAddUpdateVar("Sphinxes", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Sphinxes", "step", 0);
        }
        case kbGetProtoUnitID("Dryad"):
        {
            yAddToDatabase("Dryads", db);
            yAddUpdateVar("dryads", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Wadjet"):
        {
            yAddToDatabase("Dryads", db);
            yAddUpdateVar("dryads", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Medusa"):
        {
            yAddToDatabase("Medusas", db);
            yAddUpdateVar("Medusas", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Medusas", "step", 0);
        }
        case kbGetProtoUnitID("Mountain Giant"):
        {
            yAddToDatabase("MountainGiants", db);
            yAddUpdateVar("MountainGiants", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("MountainGiants", "step", 0);
        }
        case kbGetProtoUnitID("Frost Giant"):
        {
            yAddToDatabase("FrostGiants", db);
            yAddUpdateVar("FrostGiants", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("FrostGiants", "step", 0);
        }
        case kbGetProtoUnitID("Valkyrie"):
        {
            yAddUpdateVar("enemies", "magicResist", 1);
            yAddToDatabase("Valkyries", db);
            yAddUpdateVar("Valkyries", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Valkyries", "silenced", 0);
            spyEffect(1*trQuestVarGet(db), 
                kbGetProtoUnitID("Vortex Finish Linked"), yGetNewVarName("Valkyries", "sfx"));
        }
        case kbGetProtoUnitID("Ballista"):
        {
            yAddToDatabase("ballistas", db);
            yAddUpdateVar("ballistas", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Colossus"):
        {
            yAddUpdateVar("enemies", "magicResist", 1);
            yAddToDatabase("Valkyries", db);
            yAddUpdateVar("Valkyries", "index", yGetNewestPointer("enemies"));
            spyEffect(1*trQuestVarGet(db), 
                kbGetProtoUnitID("Vortex Finish Linked"), yGetNewVarName("Valkyries", "sfx"));
        }
        case kbGetProtoUnitID("Fire Siphon"):
        {
            yAddToDatabase("siphons", db);
            yAddUpdateVar("siphons", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Battle Boar"):
        {
            yAddToDatabase("battleBoars", db);
            yAddUpdateVar("battleBoars", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("battleBoars", "step", 0);
        }
        case kbGetProtoUnitID("Automaton SPC"):
        {
            yAddToDatabase("automatons", db);
            yAddUpdateVar("automatons", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Behemoth"):
        {
            yAddUpdateVar("enemies", "physicalResist", 1);
            yAddUpdateVar("enemies", "magicResist", -1);
        }
        case kbGetProtoUnitID("Scarab"):
        {
            yAddUpdateVar("enemies", "physicalResist", 1);
            yAddUpdateVar("enemies", "magicResist", -1);
            yAddToDatabase("scarabs", db);
            yAddUpdateVar("scarabs", "index", yGetNewestPointer("enemies"));
        }
        case kbGetProtoUnitID("Satyr"):
        {
            yAddToDatabase("Satyrs", db);
            yAddUpdateVar("Satyrs", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Satyrs", "step", 0);
        }
        case kbGetProtoUnitID("Avenger"):
        {
            yAddToDatabase("Avengers", db);
            yAddUpdateVar("Avengers", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Avengers", "step", 0);
        }
        case kbGetProtoUnitID("Scorpion Man"):
        {
            yAddToDatabase("ScorpionMen", db);
            yAddUpdateVar("ScorpionMen", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("ScorpionMen", "step", 0);
        }
        case kbGetProtoUnitID("Mummy"):
        {
            yAddToDatabase("Mummies", db);
            yAddUpdateVar("Mummies", "index", yGetNewestPointer("enemies"));
            yAddUpdateVar("Mummies", "step", 0);
        }
    }
}


rule setup_enemies
inactive
highFrequency
{
	if (trTime() > cActivationTime) {
		trModifyProtounit("Minion", ENEMY_PLAYER, 8, -99);
		trModifyProtounit("Minion", 1, 8, -99);

        setupProtounitBounty("Nemean Lion", 0, 0, 0);
        setupProtounitBounty("King Folstag", 0, 0, 0);
        setupProtounitBounty("Chimera", 0.25, 0, 0);
        setupProtounitBounty("Shade of Hades", 0.25, 0, 0);
        setupProtounitBounty("Helepolis", 0.25, 0, 0);
        setupProtounitBounty("Scylla", 0.5, 0, 0);
        setupProtounitBounty("Theris", 0.5, 0, 0);
        setupProtounitBounty("Pharaoh of Osiris XP", 0.47, 0, 0);
		
        setupProtounitBounty("Golden Lion", 0.3, 3);
        setupProtounitBounty("Anubite", 0.3, 4, 0.03);
        setupProtounitBounty("Terracotta Soldier", 0.3, 4, 0.03);
		setupProtounitBounty("Sphinx", 0.3, 5, 0.05);
		setupProtounitBounty("Petsuchos", 0.1, 6, 0.1, RELIC_ATTACK_RANGE);

        setupProtounitBounty("Minion", 0.1, 2);
        setupProtounitBounty("Walking Woods Marsh", 0.2, 3);
        setupProtounitBounty("Dryad", 0.2, 4, 0.03);
        setupProtounitBounty("Centaur", 0.1, 4, 0.03);
        setupProtounitBounty("Medusa", 0.1, 8, 0.15, RELIC_SPELL_DURATION);
        setupProtounitBounty("Mountain Giant", 0.3, 10, 0.1);

        setupProtounitBounty("Fenris Wolf", 0.2, 4, 0.03);
        setupProtounitBounty("Valkyrie", 0, 5, 0.05);
        setupProtounitBounty("Ballista", 0, 4, 0.03);
        setupProtounitBounty("Frost Giant", 0.3, 10, 0.1);

        setupProtounitBounty("Automaton SPC", 0.5, 4, 0);
        setupProtounitBounty("Colossus", 0.2, 10, 0.1);
        setupProtounitBounty("Battle Boar", 0.3, 8, 0.08);
        setupProtounitBounty("Fire Siphon", 0, 8, 0.1);

        setupProtounitBounty("Cyclops", 0.2, 6, 0.05);
        setupProtounitBounty("Satyr", 0.1, 4, 0.03);
        setupProtounitBounty("Behemoth", 1, 10, 0.08);
        setupProtounitBounty("Avenger", 0.2, 12, 0.1);

        setupProtounitBounty("Wadjet", 0.2, 4, 0.03);
        setupProtounitBounty("Scorpion Man", 0.3, 6, 0.05);
        setupProtounitBounty("Scarab", 1, 10, 0.08);
        setupProtounitBounty("Mummy", 0.6, 12, 0.1);

        for(class = 1; <= 16) {
            setupProtounitBounty(kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto")), 
                trQuestVarGet("proto"+1*trQuestVarGet("class"+class+"proto")+"armor"), 8, 0);
        }

        /* ballista projectiles */
        trModifyProtounit("Ballista", ENEMY_PLAYER, 13, -3);
        trModifyProtounit("Ballista", ENEMY_PLAYER, 11, -12);
        trModifyProtounit("Ballista Shot", ENEMY_PLAYER, 1, -19);
        trModifyProtounit("Priest Projectile", ENEMY_PLAYER, 1, -20);
        trModifyProtounit("Hero Greek Achilles", ENEMY_PLAYER, 5, 99);
        trModifyProtounit("Lancer Hero", ENEMY_PLAYER, 5, 99);
        trModifyProtounit("Behemoth", ENEMY_PLAYER, 24, 1);
        trModifyProtounit("Behemoth", ENEMY_PLAYER, 25, 1);
        trModifyProtounit("Behemoth", ENEMY_PLAYER, 26, 1);
        trModifyProtounit("Scarab", ENEMY_PLAYER, 24, 1);
        trModifyProtounit("Scarab", ENEMY_PLAYER, 25, 1);
        trModifyProtounit("Scarab", ENEMY_PLAYER, 26, 1);

        trModifyProtounit("Outpost", ENEMY_PLAYER, 0, 9999999999999999999.0);
        trModifyProtounit("Outpost", ENEMY_PLAYER, 0, -9999999999999999999.0);
        trModifyProtounit("Outpost", ENEMY_PLAYER, 0, 47);
        trModifyProtounit("Outpost", ENEMY_PLAYER, 24, 1);
        trModifyProtounit("Outpost", ENEMY_PLAYER, 25, 1);
        trModifyProtounit("Outpost", ENEMY_PLAYER, 26, 1);
		
		xsDisableSelf();
	}
}

void ballistaShotPop() {
    int id = 0;
    for(x=yGetDatabaseCount("playerUnits"); >0) {
        id = yDatabaseNext("playerUnits", true);
        if (id == -1 || trUnitAlive() == false) {
            removePlayerUnit();
        } else if (zDistanceToVectorSquared("playerUnits", "pos") < 16) {
            damagePlayerUnit(200);
        }
    }
    trUnitSelectClear();
    trUnitSelect(""+1*yGetVar("ballistaShots", "next1"), true);
    trUnitSelect(""+1*yGetVar("ballistaShots", "next2"), true);
    trUnitDestroy();
    trUnitSelectClear();
    trUnitSelectByQV("ballistaShots", true);
    trDamageUnitPercent(100);
    trUnitChangeProtoUnit("Meteorite");
    trSoundPlayFN("fireball launch.wav","1",-1,"","");
    yRemoveFromDatabase("ballistaShots");
}

void enemiesAlways() {
    int old = xsGetContextPlayer();
    int p = 0;
    int proto = 0;
    int id = 0;
    int target = 0;
    int action = 0;
    float amt = 0;
    float dist = 0;
    float angle = 0;
    string pName = "";
    /*
    Enemies incoming
    */
    for (x=xsMin(5, yGetDatabaseCount("enemiesIncoming")); > 0) {
        yDatabaseNext("enemiesIncoming", true);
        for(p=1; < ENEMY_PLAYER) {
            if (trUnitHasLOS(p)) {
                activateEnemy("enemiesIncoming");
                yRemoveFromDatabase("enemiesIncoming");
                break;
            }
        }
    }

    if (yGetDatabaseCount("enemies") > 0) {
        id = yDatabaseNext("enemies", true);
        if ((id == -1) || (trUnitAlive() == false)) {
            removeEnemy();
        } else if ((kbUnitGetAnimationActionType(id) == 9) && (yGetVar("enemies", "stunStatus") == 0)) {
            /* if idle and out of LOS, we remove it */
            ySetVar("enemies", "missing", 1 + yGetVar("enemies", "missing"));
            if (yGetVar("enemies", "missing") >= 10) {
                for(p=1; < ENEMY_PLAYER) {
                    if (trUnitHasLOS(p)) {
                        ySetVar("enemies", "missing", 0);
                        break;
                    }
                }
                if (yGetVar("enemies", "missing") >= 10) {
                    yAddToDatabase("enemiesIncoming", "enemies");
                    yRemoveFromDatabase("enemies");
                }
            }
        } else {
            stunsAndPoisons("enemies");
            trVectorSetUnitPos("pos", "enemies");
            ySetVar("enemies", "posX", trQuestVarGet("posX"));
            ySetVar("enemies", "posZ", trQuestVarGet("posZ"));
        }
    }


    for (x=xsMin(5, yGetDatabaseCount("ballistaShots")); >0) {
        yDatabaseNext("ballistaShots", true);
        trVectorSetUnitPos("pos", "ballistaShots");
        vectorToGrid("pos", "loc");
        if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) || trTimeMS() > yGetVar("ballistaShots", "timeout")) {
            ballistaShotPop();
        } else {
            action = 0;
            for(p=1; < ENEMY_PLAYER) {
                action = action + trCountUnitsInArea(""+1*trQuestVarGet("ballistaShots"),p,"Unit",2);
            }
            if (action > 0) {
                ballistaShotPop();
            }
        }
    }

    trQuestVarSet("sound", 0);
    for(x=xsMin(10, yGetDatabaseCount("delayLasers")); >0) {
        id = yDatabaseNext("delayLasers", true);
        if (kbGetUnitBaseTypeID(id) == -1) {
            yRemoveFromDatabase("delayLasers");
        } else if (yGetVar("delayLasers", "phase") == 0) {
            if (trTimeMS() > yGetVar("delayLasers", "next")) {
                ySetVar("delayLasers", "phase", 1);
                ySetVar("delayLasers", "next", trTimeMS() + 500);
                trSetSelectedScale(6.0,6.0,yGetVar("delayLasers", "dist") * 1.3);
                trUnitHighlight(3.0,false);
                trQuestVarSet("sound", 1);
                trVectorSetUnitPos("start", "delayLasers");
                for(y=yGetDatabaseCount("playerUnits"); >0) {
                    if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                        removePlayerUnit();
                    } else {
                        trVectorSetUnitPos("pos", "playerUnits");
                        amt = zDistanceBetweenVectors("pos", "start");
                        if (amt < yGetVar("delayLasers", "dist")) {
                            trQuestVarSet("intersectX", yGetVar("delayLasers", "dirX") * amt + trQuestVarGet("startX"));
                            trQuestVarSet("intersectZ", yGetVar("delayLasers", "dirZ") * amt + trQuestVarGet("startZ"));
                            if (zDistanceBetweenVectorsSquared("pos", "intersect") < 1.0) {
                                damagePlayerUnit(300);
                            }
                        }   
                    }
                }
            }
        } else {
            trQuestVarSet("scale", 0.012 * (yGetVar("delayLasers", "next") - trTimeMS()));
            if (trQuestVarGet("scale") < 0) {
                trUnitDestroy();
                yRemoveFromDatabase("delayLasers");
            } else {
                trSetSelectedScale(trQuestVarGet("scale"), trQuestVarGet("scale"), yGetVar("delayLasers", "dist") * 1.3);
            }
        }
    }

    if (trQuestVarGet("sound") == 1) {
        trSoundPlayFN("sky passage.wav", "1", -1,"","");
        trSoundPlayFN("phoenixattack.wav", "1", -1,"","");
        trQuestVarSet("sound", 0);
    }

    /*
    Projectiles from attacks
    */
    xsSetContextPlayer(ENEMY_PLAYER);
    while(trQuestVarGet("nextProj") < trGetNextUnitScenarioNameNumber()) {
        trUnitSelectClear();
        trUnitSelectByQV("nextProj", true);
        if (trUnitIsOwnedBy(ENEMY_PLAYER)) {
            id = kbGetBlockID(""+1*trQuestVarGet("nextProj"), true);
            proto = kbGetUnitBaseTypeID(id);
            if (proto == kbGetProtoUnitID("Ballista Shot")) {
                trVectorSetUnitPos("pos", "nextProj");
                trQuestVarSet("closest", 0);
                trQuestVarSet("closestName", 0);
                trQuestVarSet("closestDistance", 25);
                for (x=yGetDatabaseCount("ballistas"); >0) {
                    id = yDatabaseNext("ballistas", true);
                    if (trUnitAlive() == false) {
                        yRemoveFromDatabase("ballistas");
                    } else if (kbUnitGetAnimationActionType(id) == 12) {
                        trQuestVarSet("currentDistance", zDistanceToVectorSquared("ballistas", "pos"));
                        if (trQuestVarGet("currentDistance") < trQuestVarGet("closestDistance")) {
                            trQuestVarCopy("closestDistance", "currentDistance");
                            trQuestVarSet("closest", id);
                            trQuestVarCopy("closestName", "ballistas");
                        }
                    }
                }
                trQuestVarSet("target", trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(1*trQuestVarGet("closest"))));
                trVectorSetUnitPos("start", "closestName");
                trVectorSetUnitPos("end", "target");
                vectorSetAsTargetVector("target", "start", "end", 100.0);

                trUnitSelectClear();
                trUnitSelectByQV("nextProj", true);
                trUnitConvert(ENEMY_PLAYER);
                trUnitChangeProtoUnit("Transport Ship Greek");

                trQuestVarSet("next1", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", "Dwarf", 1, 1,0,1,0,true);
                trQuestVarSet("next2", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0", "Dwarf", 1, 1,0,1,0,false);
                trArmySelect("1,0");
                trUnitConvert(ENEMY_PLAYER);
                trImmediateUnitGarrison(""+1*trQuestVarGet("nextProj"));
                trUnitChangeProtoUnit("Dwarf");

                yAddToDatabase("ballistaShots", "next2");
                yAddUpdateVar("ballistaShots", "next1", trQuestVarGet("next1"));
                yAddUpdateVar("ballistaShots", "next2", trQuestVarGet("nextProj"));
                yAddUpdateVar("ballistaShots", "timeout", trTimeMS() + 5000);

                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("next1"), true);
                trUnitSelect(""+1*trQuestVarGet("nextProj"), true);
                trUnitChangeProtoUnit("Relic");

                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("next2"), true);
                trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));

                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("next1"), true);
                trImmediateUnitGarrison(""+1*trQuestVarGet("next2"));

                trMutateSelected(kbGetProtoUnitID("Ballista Shot"));
                trUnitSelectClear();
                trUnitSelectByQV("nextProj", true);
                trImmediateUnitGarrison(""+1*trQuestVarGet("next2"));
                trMutateSelected(kbGetProtoUnitID("Fire Siphon Fire"));
                trUnitConvert(0);
                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("next2"), true);
                trMutateSelected(kbGetProtoUnitID("Priest Projectile"));
                trSetSelectedScale(0,50,0);
                trSetUnitOrientation(zGetUnitVector("start", "end"), xsVectorSet(0,1,0), true);
                trUnitMoveToPoint(trQuestVarGet("targetx"),0,trQuestVarGet("targetz"), -1, false);
                trUnitSelectClear();
                trUnitSelect(""+1*trQuestVarGet("next1"), true);
                trUnitConvert(0);
            } else if (proto == kbGetProtoUnitID("Fire Siphon Fire")) {
                if (yGetDatabaseCount("siphons") > 0) {
                    trVectorSetUnitPos("pos", "nextProj");
                    trUnitChangeProtoUnit("Lightning sparks");
                    trQuestVarSet("closest", -1);
                    trQuestVarSet("closestName", 0);
                    trQuestVarSet("closestDistance", 25);
                    for (x=yGetDatabaseCount("siphons"); >0) {
                        id = yDatabaseNext("siphons", true);
                        if (id == -1 || trUnitAlive() == false) {
                            yRemoveFromDatabase("siphons");
                        } else if (kbUnitGetAnimationActionType(id) == 12) {
                            trQuestVarSet("currentDistance", zDistanceToVectorSquared("siphons", "pos"));
                            if (trQuestVarGet("currentDistance") < trQuestVarGet("closestDistance")) {
                                trQuestVarCopy("closestDistance", "currentDistance");
                                trQuestVarSet("closest", id);
                                trQuestVarCopy("closestName", "siphons");
                            }
                        }
                    }
                    if (trQuestVarGet("closest") >= 0) {
                        trQuestVarSet("target", trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(1*trQuestVarGet("closest"))));
                        trSoundPlayFN("skypassagein.wav","1",-1,"","");
                        trVectorSetUnitPos("start", "closestName");
                        trVectorSetUnitPos("pos", "target");
                        trVectorQuestVarSet("dir", zGetUnitVector("start", "pos"));
                        shootLaser("start", "dir");
                    }
                }
            } else if (proto == kbGetProtoUnitID("Mummy Flies")) {
                if (yGetDatabaseCount("Mummies") > 0) {
                    trUnitChangeProtoUnit("Rocket");
                    trVectorSetUnitPos("pos", "nextProj");
                    trQuestVarSet("closest", -1);
                    trQuestVarSet("closestName", 0);
                    trQuestVarSet("closestDistance", 25);
                    for (x=yGetDatabaseCount("Mummies"); >0) {
                        id = yDatabaseNext("Mummies", true);
                        if (id == -1 || trUnitAlive() == false) {
                            yRemoveFromDatabase("Mummies");
                        } else if (kbUnitGetAnimationActionType(id) == 12) {
                            trQuestVarSet("currentDistance", zDistanceToVectorSquared("Mummies", "pos"));
                            if (trQuestVarGet("currentDistance") < trQuestVarGet("closestDistance")) {
                                trQuestVarCopy("closestDistance", "currentDistance");
                                trQuestVarSet("closest", id);
                                trQuestVarCopy("closestName", "Mummies");
                            }
                        }
                    }
                    if (trQuestVarGet("closest") >= 0) {
                        trQuestVarSet("target", trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(1*trQuestVarGet("closest"))));
                        trVectorSetUnitPos("start", "closestName");
                        trVectorSetUnitPos("pos", "target");
                        trVectorQuestVarSet("dir", zGetUnitVector("start", "pos"));
                        addGenericProj("MummyBalls","start","dir",kbGetProtoUnitID("Kronny Birth SFX"),2,8,4.5);
                        yAddUpdateVar("MummyBalls", "prevX", trQuestVarGet("startx"));
                        yAddUpdateVar("MummyBalls", "prevZ", trQuestVarGet("startz"));
                        yAddUpdateVar("MummyBalls", "dist", 4.0);
                        yAddUpdateVar("MummyBalls", "type", STATUS_SILENCE);
                    }
                }
            }
        }
        trQuestVarSet("nextProj", 1 + trQuestVarGet("nextProj"));
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
                    activateEnemy("next");
                    trQuestVarSet("angle", trQuestVarGet("angle") + angle);
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
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("sphinxes")) {
            if (trUnitAlive()) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            } else {
                trUnitChangeProtoUnit("Sphinx");
            }
            yRemoveFromDatabase("sphinxes");
            yRemoveUpdateVar("sphinxes", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("sphinxes", "index")) == 1) {
            ySetVar("sphinxes", "step", 1);
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
                                if ((trQuestVarGet("p"+p+"nickQuestProgress") == 1) &&
                                    trQuestVarGet("p"+p+"nickEquipped") == 1) {
                                    trQuestVarSet("p"+p+"nickQuestProgress", 2);
                                    if (trCurrentPlayer() == p) {
                                        xsEnableRule("nick_next_dialog");
                                    }
                                }
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
            trQuestVarSet("giantKills", 1 + trQuestVarGet("giantKills"));
            trUnitChangeProtoUnit("Mountain Giant");
            yRemoveFromDatabase("MountainGiants");
            yRemoveUpdateVar("MountainGiants", "step");
        } else if (checkEnemyDeactivated("MountainGiants")) {
            trUnitOverrideAnimation(-1,0,false,true,-1);
            yRemoveFromDatabase("MountainGiants");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("MountainGiants", "index")) == 1) {
            ySetVar("MountainGiants", "step", 2);
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
                            damagePlayerUnit(100 + 100 * trQuestVarGet("stage"));
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
                    ySetVar("MountainGiants", "next", yGetVar("MountainGiants", "next") + 1200);
                    
                }
                case 2:
                {
                    ySetVar("MountainGiants", "step", 0);
                    ySetVar("MountainGiants", "next", trTimeMS() + 15000);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("MountainGiants", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("MountainGiants", "index"));
            if (action > 0 && yGetVar("MountainGiants", "step") == 1) {
                ySetVar("MountainGiants", "step", 0);
                ySetVar("MountainGiants", "next", trTimeMS() + 15000);
            }
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
                stunUnit("playerUnits", 3.0);
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
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Medusas")) {
            trUnitChangeProtoUnit("Medusa");
            yRemoveFromDatabase("Medusas");
            yRemoveUpdateVar("Medusas", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("Medusas", "index")) == 1) {
            trUnitOverrideAnimation(-1,0,false,true,-1);
            ySetVar("Medusas", "step", 2);
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
                    addGenericProj("MedusaBalls","start","dir",kbGetProtoUnitID("Curse SFX"),2,4,4.5);
                    yAddUpdateVar("MedusaBalls", "target", yGetVar("Medusas", "target"));
                    yAddUpdateVar("MedusaBalls", "bounces", 10);
                    ySetVar("Medusas", "step", 2);
                    ySetVar("Medusas", "next", yGetVar("Medusas", "next") + 800);
                }
                case 2:
                {
                    ySetVar("Medusas", "step", 0);
                    ySetVar("Medusas", "next", trTimeMS() + 18000);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("Medusas", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("Medusas", "index"));
            if (action > 0 && yGetVar("Medusas", "step") == 1) {
                ySetVar("Medusas", "step", 0);
                ySetVar("Medusas", "next", trTimeMS() + 18000);
            }
        }
    }

    if (yGetDatabaseCount("Valkyries") > 0) {
        id = yDatabaseNext("Valkyries", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Valkyries")) {
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("Valkyries", "sfx"), true);
            trUnitDestroy();
            yRemoveFromDatabase("Valkyries");
        } else {
            id = yGetPointer("enemies");
            if (ySetPointer("enemies", 1*yGetVar("Valkyries", "index"))) {
                ySetVar("enemies", "magicResist", 1 - yGetVar("enemies", "silenceStatus"));
                if (yGetVar("enemies", "silenceStatus") != yGetVar("Valkyries", "silenced")) {
                    ySetVar("Valkyries", "silenced", yGetVar("enemies", "silenceStatus"));
                    trUnitSelectClear();
                    trUnitSelect(""+1*yGetVar("Valkyries", "sfx"), true);
                    if (yGetVar("enemies", "silenceStatus") == 1) {
                        trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
                    } else {
                        trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
                    }
                }
                ySetPointer("enemies", id);
            }
        }
    }

    if (yGetDatabaseCount("Dryads") > 0) {
        id = yDatabaseNext("Dryads", true);
        if (id == -1 || trUnitAlive() == false) {
            if (yGetVar("dryads", "silenceStatus") == 0) {
                yVarToVector("Dryads", "pos");
                trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
                trArmySelect("1,0");
                trUnitChangeProtoUnit("Lampades Blood");
                for(x=yGetDatabaseCount("playerUnits"); >0) {
                    id = yDatabaseNext("playerUnits", true);
                    if (id == -1 || trUnitAlive() == false) {
                        removePlayerUnit();
                    } else if (zDistanceToVectorSquared("playerUnits", "pos") < 16) {
                        poisonUnit("playerUnits", 10.0, 5.0 * trQuestVarGet("stage"));
                    }
                }
                if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 2) {
                    if (zDistanceToVectorSquared("p"+trCurrentPlayer()+"unit", "pos") < 16) {
                        trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 3);
                        xsEnableRule("nick_next_dialog");
                    }
                }
            }
            yRemoveFromDatabase("Dryads");
        } else if (checkEnemyDeactivated("Dryads")) {
            yRemoveFromDatabase("Dryads");
        } else {
            trVectorSetUnitPos("pos", "Dryads");
            ySetVarFromVector("Dryads", "pos", "pos");
            ySetVar("Dryads", "silenceStatus", 1*yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("Dryads", "index")));
        }
    }

    if (yGetDatabaseCount("scarabs") > 0) {
        id = yDatabaseNext("scarabs", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("scarabs")) {
            if (yGetVar("scarabs", "silenceStatus") == 0) {
                yVarToVector("scarabs", "pos");
                trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
                trArmySelect("1,0");
                trUnitChangeProtoUnit("Pestilence SFX1");
                trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
                trArmySelect("1,0");
                trUnitConvert(ENEMY_PLAYER);
                trUnitChangeProtoUnit("Victory Marker");
            }
            yRemoveFromDatabase("scarabs");
        } else if (checkEnemyDeactivated("scarabs")) {
            yRemoveFromDatabase("scarabs");
        } else {
            trVectorSetUnitPos("pos", "scarabs");
            ySetVarFromVector("scarabs", "pos", "pos");
            ySetVar("scarabs", "silenceStatus", 1*yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("scarabs", "index")));
        }
    }

    if (yGetDatabaseCount("automatonBombs") > 0) {
        yDatabaseNext("automatonBombs");
        if (trTimeMS() > yGetVar("automatonBombs", "timeout")) {
            yVarToVector("automatonBombs", "pos");
            trUnitSelectClear();
            trUnitSelectByQV("automatonBombs", true);
            trDamageUnitPercent(-100);
            trUnitChangeProtoUnit("Tartarian Gate Flame");
            trUnitSelectClear();
            trUnitSelectByQV("automatonBombs", true);
            trDamageUnitPercent(-100);
            trSetSelectedScale(1.0,1.0,0.4);
            yRemoveFromDatabase("automatonBombs");
            for(x=yGetDatabaseCount("playerUnits"); >0) {
                if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                    removePlayerUnit();
                } else if (zDistanceToVectorSquared("playerUnits", "pos") < 16) {
                    damagePlayerUnit(200);
                }
            }
        }
    }

    if (yGetDatabaseCount("automatons") > 0) {
        if (yDatabaseNext("automatons", true) == -1 || trUnitAlive() == false) {
            if (yGetVar("automatons", "silenceStatus") == 0) {
                trUnitHighlight(3.0, true);
                trSoundPlayFN("gatherpoint.wav","1",-1,"","");
                yAddToDatabase("automatonBombs", "automatons");
                yAddUpdateVar("automatonBombs", "timeout", trTimeMS() + 3000);
                yAddUpdateVar("automatonBombs", "posx", yGetVar("automatons", "posx"));
                yAddUpdateVar("automatonBombs", "posz", yGetVar("automatons", "posz"));
            }
            yRemoveFromDatabase("automatons");
        } else if (checkEnemyDeactivated("automatons")) {
            yRemoveFromDatabase("automatons");
        } else {
            trVectorSetUnitPos("pos", "automatons");
            ySetVarFromVector("automatons", "pos", "pos");
            ySetVar("automatons", "silenceStatus", 1*yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("automatons", "index")));
        }
    }

    if(yGetDatabaseCount("frostGiants") >0) {
        id = yDatabaseNext("frostGiants", true);
        if (id == -1 || trUnitAlive() == false) {
            trQuestVarSet("giantKills", 1 + trQuestVarGet("giantKills"));
            if (trQuestVarGet("frostGiantHunt") == 1) {
                trQuestVarSet("frostGiantBones", 1 + trQuestVarGet("frostGiantBones"));
            }
            trUnitChangeProtoUnit("Frost Giant");
            yRemoveFromDatabase("frostGiants");
            yRemoveUpdateVar("frostGiants", "step");
        } else if (checkEnemyDeactivated("frostGiants")) {
            trUnitOverrideAnimation(-1,0,false,true,-1);
            yRemoveFromDatabase("frostGiants");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("frostGiants", "index")) == 1) {
            ySetVar("frostGiants", "step", 2);
        } else if (trTimeMS() > yGetVar("frostGiants", "specialnext")) {
            switch(1*yGetVar("frostGiants", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 6) {
                        target = kbUnitGetTargetUnitID(id);
                        ySetVar("frostGiants", "target", trGetUnitScenarioNameNumber(target));
                        ySetVar("frostGiants", "step", 1);
                        ySetVar("frostGiants", "specialnext", trTimeMS() + 1400);
                        trUnitOverrideAnimation(40,0,false,false,-1);
                    }
                }
                case 1:
                {
                    action = 0;
                    for (x=yGetDatabaseCount("playerUnits"); >0) {
                        if (yGetVar("frostGiants", "target") == yDatabaseNext("playerUnits")) {
                            trUnitSelectClear();
                            trUnitSelectByQV("playerUnits");
                            stunUnit("playerUnits", 3.0);
                            action = 1;
                            break;
                        }
                    }
                    ySetVar("frostGiants", "step", 2);
                    ySetVar("frostGiants", "specialnext", yGetVar("frostGiants", "specialnext") + 600);
                    if (action == 0) {
                        ySetVar("frostGiants", "target", -1);
                    } else if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 3) {
                        if (yGetVar("frostGiants", "target") == trQuestVarGet("p"+trCurrentPlayer()+"unit")) {
                            trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 4);
                            xsEnableRule("nick_next_dialog");
                        }
                    }
                }
                case 2:
                {
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                    ySetVar("frostGiants", "step", 0);
                    if (yGetVar("frostGiants", "target") == -1) {
                        ySetVar("frostGiants", "specialnext", trTimeMS());
                    } else {
                        ySetVar("frostGiants", "specialnext", trTimeMS() + 15000);
                    }
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("frostGiants", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("frostGiants", "index"));
            if (action > 0 && yGetVar("frostGiants", "step") == 1) {
                ySetVar("frostGiants", "step", 0);
                ySetVar("frostGiants", "next", trTimeMS() + 18000);
            }
        }
    }

    if (yGetDatabaseCount("barrages") > 0) {
        yDatabaseNext("barrages");
        if (trTimeMS() > yGetVar("barrages", "next")) {
            ySetVar("barrages", "next", 300 + yGetVar("barrages", "next"));
            /* do the damage */
            yVarToVector("barrages", "pos");
            yVarToVector("barrages", "dir");
            if (yGetVar("barrages", "count") <= 3) {
                for(x=yGetDatabaseCount("playerUnits"); >0) {
                    if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                        removePlayerUnit();
                    } else if (zDistanceToVectorSquared("playerUnits", "pos") < 9.0) {
                        damagePlayerUnit(100);
                    }
                }
            }

            if (yGetVar("barrages", "count") >= 0) {
                trQuestVarSet("endx", trQuestVarGet("posx") + 4.0 * trQuestVarGet("dirx"));
                trQuestVarSet("endz", trQuestVarGet("posz") + 4.0 * trQuestVarGet("dirz"));
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("endx"),0,trQuestVarGet("endz"),0,true);
                trArmySelect("1,0");
                trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
                trUnitConvert(0);
                trUnitChangeProtoUnit("Barrage");
            }
            

            ySetVar("barrages", "count", yGetVar("barrages", "count") - 1);
            trQuestVarSet("posx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
            trQuestVarSet("posz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
            vectorToGrid("pos", "loc");
            if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) || yGetVar("barrages", "count") <= -2) {
                yRemoveFromDatabase("barrages");
            } else {
                ySetVarFromVector("barrages", "pos", "pos");
            }
        }
    }

    if(yGetDatabaseCount("Satyrs") >0) {
        id = yDatabaseNext("Satyrs", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Satyrs")) {
            yRemoveFromDatabase("Satyrs");
            yRemoveUpdateVar("Satyrs", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("Satyrs", "index")) == 1) {
            ySetVar("Satyrs", "specialnext", trTimeMS() + 10000);
        } else if (trTimeMS() > yGetVar("Satyrs", "specialnext")) {
            if (kbUnitGetAnimationActionType(id) == 12) {
                target = kbUnitGetTargetUnitID(id);
                ySetVar("Satyrs", "specialnext", trTimeMS() + 20000);
                trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
                trVectorSetUnitPos("start", "Satyrs");
                trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));

                trQuestVarSetFromRand("sound", 1, 3, true);
                trSoundPlayFN("rainofarrows"+1*trQuestVarGet("sound")+".wav","1",-1,"","");

                yAddToDatabase("barrages", "next");
                yAddUpdateVar("barrages", "dirx", trQuestVarGet("dirx"));
                yAddUpdateVar("barrages", "dirz", trQuestVarGet("dirz"));
                yAddUpdateVar("barrages", "posx", trQuestVarGet("endx") - 8.0 * trQuestVarGet("dirX"));
                yAddUpdateVar("barrages", "posz", trQuestVarGet("endz") - 8.0 * trQuestVarGet("dirZ"));
                yAddUpdateVar("barrages", "next", trTimeMS());
                yAddUpdateVar("barrages", "count", 5);
            }
        }
    }

    if(yGetDatabaseCount("battleBoars") >0) {
        id = yDatabaseNext("battleBoars", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("battleBoars")) {
            if (trUnitAlive()) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            } else {
                trUnitChangeProtoUnit("Battle Boar");
            }
            yRemoveFromDatabase("battleBoars");
            yRemoveUpdateVar("battleBoars", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("battleBoars", "index")) == 1) {
            ySetVar("battleBoars", "step", 2);
        } else if (trTimeMS() > yGetVar("battleBoars", "specialnext")) {
            switch(1*yGetVar("battleBoars", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 6) {
                        target = kbUnitGetTargetUnitID(id);
                        ySetVar("battleBoars", "target", trGetUnitScenarioNameNumber(target));
                        ySetVar("battleBoars", "step", 1);
                        ySetVar("battleBoars", "specialnext", trTimeMS() + 450);
                        trUnitOverrideAnimation(26,0,false,false,-1);
                    }
                }
                case 1:
                {
                    trVectorSetUnitPos("start", "battleBoars");
                    trVectorQuestVarSet("end", kbGetBlockPosition(""+1*yGetVar("battleBoars", "target")));
                    for (x=yGetDatabaseCount("playerUnits"); >0) {
                        if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                            removePlayerUnit();
                        } else if (zDistanceToVectorSquared("playerUnits", "end") < 9.0) {
                            trVectorSetUnitPos("pos", "playerUnits");
                            vectorSetAsTargetVector("target", "start", "pos", 20.0);
                            launchUnit("playerUnits", "target");
                        }
                    }
                    ySetVar("battleBoars", "step", 2);
                    ySetVar("battleBoars", "specialnext", yGetVar("battleBoars", "specialnext") + 300);
                }
                case 2:
                {
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                    ySetVar("battleBoars", "step", 0);
                    if (yGetVar("battleBoars", "target") == -1) {
                        ySetVar("battleBoars", "specialnext", trTimeMS());
                    } else {
                        ySetVar("battleBoars", "specialnext", trTimeMS() + 15000);
                    }
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("battleBoars", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("battleBoars", "index"));
            if (action > 0 && yGetVar("battleBoars", "step") == 1) {
                ySetVar("battleBoars", "step", 0);
                ySetVar("battleBoars", "next", trTimeMS() + 18000);
            }
        }
    }

    if (yGetDatabaseCount("AvengerProj") > 0) {
        if (processGenericProj("AvengerProj") == PROJ_FALLING) {
            yVarToVector("AvengerProj", "prev");
            yVarToVector("AvengerProj", "dir");
            trVectorSetUnitPos("pos", "AvengerProj");
            amt = zDistanceBetweenVectors("pos", "prev");
            if (amt > 2.0) {
                yVarToVector("AvengerProj", "dir");
                ySetVar("AvengerProj", "currentDist", yGetVar("AvengerProj", "currentDist") + amt);
                for(x=yGetDatabaseCount("playerUnits"); >0) {
                    if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                        removePlayerUnit();
                    } else {
                        dist = zDistanceToVector("playerUnits", "prev");
                        if (dist < amt + 1.0) {
                            trQuestVarSet("hitboxX", trQuestVarGet("prevX") + dist * trQuestVarGet("dirX"));
                            trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + dist * trQuestVarGet("dirZ"));
                            if (zDistanceToVectorSquared("playerUnits", "hitbox") < 9.0) {
                                damagePlayerUnit(amt * 20.0);
                                for(p=1; < ENEMY_PLAYER) {
                                    if (trQuestVarGet("playerUnits") == trQuestVarGet("p"+p+"unit")) {
                                        silencePlayer(p, 5.0);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                ySetVarFromVector("AvengerProj", "prev", "pos");
            }
            trQuestVarSet("nextx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
            trQuestVarSet("nextz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
            vectorToGrid("next", "loc");
            if (yGetVar("AvengerProj", "currentDist") >= yGetVar("AvengerProj", "distance") ||
                terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
                trUnitSelectClear();
                trUnitSelectByQV("AvengerProj", true);
                trDamageUnitPercent(-100);
                trUnitChangeProtoUnit("Avenger");
                trUnitSelectClear();
                trUnitSelectByQV("AvengerProj", true);
                trDamageUnitPercent(-100);
                trDamageUnitPercent(yGetVar("AvengerProj", "damaged"));
                activateEnemy("avengerProj");
                yAddUpdateVar("Avengers", "specialnext", trTimeMS() + 10000);
                yRemoveFromDatabase("AvengerProj");
            }
        }
    }

    if(yGetDatabaseCount("Avengers") >0) {
        id = yDatabaseNext("Avengers", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Avengers")) {
            if (trUnitAlive()) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            } else {
                trUnitChangeProtoUnit("Avengers");
            }
            yRemoveFromDatabase("Avengers");
            yRemoveUpdateVar("Avengers", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("Avengers", "index")) == 1) {
            ySetVar("Avengers", "specialnext", trTimeMS() + 10000);
        } else if (trTimeMS() > yGetVar("Avengers", "specialnext")) {
            if (kbUnitGetActionType(id) == 6) {
                target = kbUnitGetTargetUnitID(id);
                trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
                trVectorSetUnitPos("start", "Avengers");
                if (zDistanceBetweenVectorsSquared("start", "end") < 144) {
                    trSoundPlayFN("sphinxspecialattack.wav","1",-1,"","");
                    trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
                    amt = trUnitPercentDamaged();
                    trUnitChangeProtoUnit("Dust Large");
                    addGenericProj("avengerProj","start","dir",kbGetProtoUnitID("Avenger"),39,10.0,4.5,1.0);
                    yAddUpdateVar("avengerProj", "prevX", trQuestVarGet("startx"));
                    yAddUpdateVar("avengerProj", "prevz", trQuestVarGet("startz"));
                    yAddUpdateVar("avengerProj", "damaged", amt);
                    yAddUpdateVar("avengerProj", "distance", zDistanceBetweenVectors("start","end") + 5.0);
                    yAddUpdateVar("avengerProj", "currentDist", 0);
                    if (ySetPointer("enemies", 1*yGetVar("Avengers", "index"))) {
                        yRemoveFromDatabase("enemies");
                    }
                    yRemoveFromDatabase("Avengers");
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("Avengers", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("Avengers", "index"));
            if (action > 0 && yGetVar("Avengers", "step") == 1) {
                ySetVar("Avengers", "step", 0);
                ySetVar("Avengers", "next", trTimeMS() + 10000);
            }
        }
    }

    if (yGetDatabaseCount("ballistas") > 0) {
        id = yDatabaseNext("ballistas", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("ballistas")) {
            yRemoveFromDatabase("ballistas");
        }
    }

    if (yGetDatabaseCount("siphons") > 0) {
        id = yDatabaseNext("siphons", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("siphons")) {
            yRemoveFromDatabase("siphons");
        } else if (kbUnitGetAnimationActionType(id) == 9) {
            if (trTimeMS() > yGetVar("siphons", "next")) {
                ySetVar("siphons", "next", trTimeMS() + 3000);
                yDatabaseNext("playerUnits");
                trVectorSetUnitPos("pos", "playerUnits");
                trUnitMoveToPoint(trQuestVarGet("posx"),0,trQuestVarGet("posz"),-1,true);
            }
        }
    }

    if (yGetDatabaseCount("MummyBalls") > 0) {
        if (processGenericProj("MummyBalls") == PROJ_FALLING) {
            yVarToVector("MummyBalls", "prev");
            yVarToVector("MummyBalls", "dir");
            amt = zDistanceBetweenVectors("pos", "prev");
            ySetVarFromVector("MummyBalls", "prev", "pos");
            for(x=yGetDatabaseCount("playerUnits"); >0) {
                if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
                    removePlayerUnit();
                } else {
                    dist = zDistanceToVector("playerUnits", "prev");
                    if (dist < amt + 4.0) {
                        trQuestVarSet("hitboxX", trQuestVarGet("prevX") + dist * trQuestVarGet("dirX"));
                        trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + dist * trQuestVarGet("dirZ"));
                        if (zDistanceToVectorSquared("playerUnits", "hitbox") < yGetVar("MummyBalls", "dist")) {
                            damagePlayerUnit(xsMin(100.0, amt * 10));
                            switch(1*yGetVar("MummyBalls", "type"))
                            {
                                case STATUS_SILENCE:
                                {
                                    target = yGetVar("playerUnits", "player");
                                    if (trQuestVarGet("p"+target+"unit") == trQuestVarGet("playerUnits")) {
                                        silencePlayer(target, 3.0);
                                    }
                                }
                                case STATUS_POISON:
                                {
                                    poisonUnit("playerUnits", 10, 5.0 * trQuestVarGet("stage"));
                                }
                            }
                        }
                    }
                }
            }
            vectorToGrid("pos", "loc");
            if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
                yRemoveFromDatabase("MummyBalls");
            }
        }
    }

    if (yGetDatabaseCount("Mummies") >0) {
        id = yDatabaseNext("Mummies", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Mummies")) {
            if (trUnitAlive()) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            } else {
                trUnitChangeProtoUnit("Mummy");
            }
            yRemoveFromDatabase("Mummies");
            yRemoveUpdateVar("Mummies", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("Mummies", "index")) == 1) {
            ySetVar("Mummies", "step", 2);
        } else if (trTimeMS() > yGetVar("Mummies", "next")) {
            switch(1*yGetVar("Mummies", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 12) {
                        target = kbUnitGetTargetUnitID(id);
                        trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
                        trVectorSetUnitPos("start", "Mummies");
                        trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
                        ySetVarFromVector("Mummies", "dir", "dir");
                        ySetVarFromVector("Mummies", "start", "start");
                        ySetVar("Mummies", "step", 1);
                        ySetVar("Mummies", "next", trTimeMS() + 1000);
                        trUnitOverrideAnimation(37,0,false,false,-1);
                    }
                }
                case 1:
                {
                    yVarToVector("Mummies", "start");
                    yVarToVector("Mummies", "dir");
                    addGenericProj("MummyBalls","start","dir",kbGetProtoUnitID("Lampades Blood"),2,10,4.0);
                    yAddUpdateVar("MummyBalls", "prevX", trQuestVarGet("startx"));
                    yAddUpdateVar("MummyBalls", "prevZ", trQuestVarGet("startz"));
                    yAddUpdateVar("MummyBalls", "dist", 16);
                    yAddUpdateVar("MummyBalls", "type", STATUS_POISON);
                    ySetVar("Mummies", "step", 2);
                    ySetVar("Mummies", "next", yGetVar("Mummies", "next") + 3000);
                }
                case 2:
                {
                    ySetVar("Mummies", "step", 0);
                    ySetVar("Mummies", "next", trTimeMS() + 18000);
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("Mummies", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("Mummies", "index"));
            if (action > 0 && yGetVar("Mummies", "step") == 1) {
                ySetVar("Mummies", "step", 0);
                ySetVar("Mummies", "next", trTimeMS() + 18000);
            }
        }
    }

    if(yGetDatabaseCount("ScorpionMen") >0) {
        id = yDatabaseNext("ScorpionMen", true);
        if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("ScorpionMen")) {
            if (trUnitAlive()) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            } else {
                trUnitChangeProtoUnit("Scorpion Man");
            }
            yRemoveFromDatabase("ScorpionMen");
            yRemoveUpdateVar("ScorpionMen", "step");
        } else if (yGetVarAtIndex("enemies", "silenceStatus", 1*yGetVar("ScorpionMen", "index")) == 1) {
            ySetVar("ScorpionMen", "step", 2);
        } else if (trTimeMS() > yGetVar("ScorpionMen", "specialnext")) {
            switch(1*yGetVar("ScorpionMen", "step"))
            {
                case 0:
                {
                    if (kbUnitGetAnimationActionType(id) == 6) {
                        target = kbUnitGetTargetUnitID(id);
                        ySetVar("ScorpionMen", "target", trGetUnitScenarioNameNumber(target));
                        ySetVar("ScorpionMen", "step", 1);
                        ySetVar("ScorpionMen", "specialnext", trTimeMS() + 500);
                        trUnitOverrideAnimation(39,0,false,false,-1);
                    }
                }
                case 1:
                {
                    action = 0;
                    for (x=yGetDatabaseCount("playerUnits"); >0) {
                        if (yGetVar("ScorpionMen", "target") == yDatabaseNext("playerUnits")) {
                            trUnitSelectClear();
                            trUnitSelectByQV("playerUnits");
                            poisonUnit("playerUnits", 10, 5.0 * trQuestVarGet("stage"));
                            action = 1;
                            break;
                        }
                    }
                    ySetVar("ScorpionMen", "step", 2);
                    ySetVar("ScorpionMen", "specialnext", yGetVar("ScorpionMen", "specialnext") + 500);
                    if (action == 0) {
                        ySetVar("ScorpionMen", "target", -1);
                    }
                }
                case 2:
                {
                    trUnitOverrideAnimation(-1,0,false,true,-1);
                    ySetVar("ScorpionMen", "step", 0);
                    if (yGetVar("ScorpionMen", "target") == -1) {
                        ySetVar("ScorpionMen", "specialnext", trTimeMS());
                    } else {
                        ySetVar("ScorpionMen", "specialnext", trTimeMS() + 15000);
                    }
                }
            }
        } else {
            action = yGetVarAtIndex("enemies", "stunStatus", 1*yGetVar("ScorpionMen", "index")); 
            action = action + yGetVarAtIndex("enemies", "launched", 1*yGetVar("ScorpionMen", "index"));
            if (action > 0 && yGetVar("ScorpionMen", "step") == 1) {
                ySetVar("ScorpionMen", "step", 0);
                ySetVar("ScorpionMen", "next", trTimeMS() + 18000);
            }
        }
    }

    xsSetContextPlayer(old);
}

