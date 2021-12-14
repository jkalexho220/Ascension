void spawnPlayer(int p = 0, string vdb = "") {
    trQuestVarSet("p"+p+"unit", trGetNextUnitScenarioNameNumber());
    string proto = "Slinger";
    switch(1*trQuestVarGet("p"+p+"class"))
    {
        case 1:
        {
            proto = "Archer Atlantean";
        }
        case 2:
        {
            proto = "Javelin Cavalry";
        }
        case 3:
        {
            proto = "Throwing Axeman";
        }
    }
    trArmyDispatch(""+p+",0",proto,1,trQuestVarGet(vdb+"x"),0,trQuestVarGet(vdb+"z"),0,true);
    yAddToDatabase("playerCharacters", "p"+p+"unit");
    yAddUpdateVar("playerCharacters", "player", p);
    yAddUpdateVar("playerCharacters", "attacking", 0);
}

rule gameplay_start
inactive
highFrequency
{
    xsDisableSelf();
    trSetFogAndBlackmap(true, true);
    xsEnableRule("gameplay_always");
    trQuestVarSet("nextProj", trGetNextUnitScenarioNameNumber());



    trVectorQuestVarSet("center", vector(10,0,10));
    spawnPlayer(1, "center");
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    yAddToDatabase("enemies", "next");
    trArmyDispatch(""+ENEMY_PLAYER+",0","Militia",1,20,0,20,0,true);
}

rule gameplay_always
inactive
highFrequency
{
    int old = xsGetContextPlayer();
    int id = 0;
    int target = 0;
    int p = 0;
    float dist = 0;
    float dmg = 0;
    for (i = 0; < xsMin(yGetDatabaseCount("playerCharacters"), 1 + ENEMY_PLAYER / 3)) {
        id = yDatabaseNext("playerCharacters", true);
        p = yGetVar("playerCharacters", "player");
        if (yGetVar("playerCharacters", "attacking") == 0) {
            if (kbUnitGetAnimationActionType(id) == 12) {
                ySetVar("playerCharacters", "attacking", 1);
                ySetVar("playerCharacters", "attackNext", trTimeMS() + 500);
            }
        } else {
            if (kbUnitGetAnimationActionType(id) == 12) {
                if (trTimeMS() > yGetVar("playerCharacters", "attackNext")) {
                    ySetVar("playerCharacters", "attackNext", trTimeMS() + 1000.0 / trQuestVarGet("p"+p+"attackSpeed"));
                    xsSetContextPlayer(p);
                    target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
                    trVectorSetUnitPos("start", "playerCharacters");
                    trVectorQuestVarSet("end", kbGetBlockPosition(""+target, true));

                    ySetVar("playerCharacters", "specialAttack", yGetVar("playerCharacters", "specialAttack") - 1);
                    if (yGetVar("playerCharacters", "specialAttack") <= 0) {
                        ySetVar("playerCharacters", "specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
                        shootArrow(p, 1*trQuestVarGet("p"+p+"element0"), "start", "end", 
                            trQuestVarGet("p"+p+"attack"), 1*trQuestVarGet("p"+p+"element1"));
                        switch(1*trQuestVarGet("p"+p+"element1"))
                        {
                            case LIGHT:
                            {
                                /* heal all allies */
                            }
                            case DARK:
                            {
                                /* summon zombie minion */
                            }
                        }
                    } else {
                        shootArrow(p, 1*trQuestVarGet("p"+p+"element0"), "start", "end", trQuestVarGet("p"+p+"attack"));
                    }
                }
            } else {
                ySetVar("playerCharacters", "attacking", 0);
            }
        }
    }

    /*
    Projectiles from attacks
    */
    xsSetContextPlayer(ENEMY_PLAYER);
    while(trQuestVarGet("nextProj") < trGetNextUnitScenarioNameNumber()) {
        id = kbGetBlockID(""+1*trQuestVarGet("nextProj"), true);
        target = kbGetUnitBaseTypeID(id);
        if ((target == kbGetProtoUnitID("Sling Stone")) ||
            (target == kbGetProtoUnitID("Arrow Flaming")) ||
            (target == kbGetProtoUnitID("Javelin Flaming")) ||
            (target == kbGetProtoUnitID("Axe"))) {
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

    /*
    Arrows
    */
    for(i=0; < xsMin(cNumberPlayers, yGetDatabaseCount("arrowsActive"))) {
        id = yDatabaseNext("arrowsActive", true);
        if (id == -1) {
            removeArrow();
        } else if (checkArrowDie()) {
            removeArrow();
            trUnitChangeProtoUnit("Dust Small");
        } else if (trCountUnitsInArea(""+1*trQuestVarGet("arrowsActive"),ENEMY_PLAYER,"Unit",2.5) > 0) {
            p = yGetVar("arrowsActive", "player");
            trVectorSetUnitPos("pos", "arrowsActive");
            if (yGetVar("arrowsActive", "special") == FIRE) {
                dist = 25;
            } else {
                dist = 6.25;
            }
            for(x=yGetDatabaseCount("enemies"); >0) {
                id = yDatabaseNext("enemies", true);
                if (id == -1 || trUnitAlive() == false) {
                    removeEnemy();
                } else {
                    if (zDistanceToVectorSquared("enemies", "pos") <= dist) {
                        elementalDamage(p, 1*yGetVar("arrowsActive", "element"), yGetVar("arrowsActive", "damage"));
                        if (yGetVar("arrowsActive", "special") == ICE && kbUnitGetCurrentHitpoints(id) > 0) {
                            stunUnit("enemies", 1.5 * trQuestVarGet("p"+p+"spellDuration"));
                        }
                    }
                }
            }
            removeArrow();
            if (yGetVar("arrowsActive", "special") == FIRE) {
                trDamageUnitPercent(100);
                trUnitChangeProtoUnit("Meteorite");
            } else {
                trUnitChangeProtoUnit("Lightning Sparks");
            }
        }
    }

    xsSetContextPlayer(old);
}