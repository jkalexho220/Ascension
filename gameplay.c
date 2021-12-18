void spawnPlayerClone(int p = 0, string vdb = "") {
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
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
    yAddToDatabase("playerCharacters", "next");
    yAddUpdateVar("playerCharacters", "player", p);
    yAddUpdateVar("playerCharacters", "attacking", 0);
    yAddUpdateVar("playerCharacters", "specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
    yAddToDatabase("playerUnits", "next");
}

void spawnPlayer(int p = 0, string vdb = "") {
    trQuestVarSet("p"+p+"unit", trGetNextUnitScenarioNameNumber());
    spawnPlayerClone(p, vdb);
}

void spawnMinion(int p = 0, string pos = "") {
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    trArmyDispatch(""+p+",0","Minion",1,trQuestVarGet(pos+"x"),0,trQuestVarGet(pos+"z"),0,true);
    yAddToDatabase("playerUnits", "next");
    yAddUpdateVar("playerUnits", "decay", 8 / trQuestVarGet("p"+p+"spellDuration"));
}

rule gameplay_start
inactive
highFrequency
{
    xsDisableSelf();
    trSetFogAndBlackmap(true, true);
    xsEnableRule("gameplay_always");
    xsEnableRule("enemies_always");
    trQuestVarSet("nextProj", trGetNextUnitScenarioNameNumber());

    /*
    TESTING STUFF BELOW THIS LINE
    trVectorQuestVarSet("center", vector(10,0,10));
    spawnPlayer(1, "center");
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    yAddToDatabase("enemies", "next");
    trArmyDispatch(""+ENEMY_PLAYER+",0","Militia",1,20,0,20,0,true);
    */
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
    float angle = 0;
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
                    ySetVar("playerCharacters", "attackNext", yGetVar("playerCharacters", "attackNext") + 1000.0 / trQuestVarGet("p"+p+"attackSpeed"));
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
                            case THUNDER:
                            {
                                /* shoot five bullets in a cone */
                                angle = fModulo(6.283185, angleBetweenVectors("start", "end") - 0.39);
                                for(x=5; >0) {
                                    angle = fModulo(6.283185, angle + 0.13);
                                    if (x == 3) {
                                        continue;
                                    }
                                    trVectorSetFromAngle("end", angle);
                                    trQuestVarSet("endx", trQuestVarGet("endx") + trQuestVarGet("startx"));
                                    trQuestVarSet("endz", trQuestVarGet("endz") + trQuestVarGet("startz"));
                                    shootArrow(p, 1*trQuestVarGet("p"+p+"element0"), "start", "end", 
                                        trQuestVarGet("p"+p+"attack"), 1*trQuestVarGet("p"+p+"element1"));
                                }
                            }
                            case LIGHT:
                            {
                                /* heal all allies */
                                for(x=yGetDatabaseCount("playerUnits"); >0) {
                                    id = yDatabaseNext("playerUnits", true);
                                    if (id == -1 || trUnitAlive() == false) {
                                        removePlayerUnit();
                                    } else {
                                        healUnit(p, trQuestVarGet("p"+p+"attack"));
                                    }
                                }
                            }
                            case DARK:
                            {
                                /* summon zombie minion */
                                spawnMinion(p, "start");
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
                dist = 36;
            } else {
                dist = 9;
            }
            for(x=yGetDatabaseCount("enemies"); >0) {
                id = yDatabaseNext("enemies", true);
                if (id == -1 || trUnitAlive() == false) {
                    removeEnemy();
                } else {
                    if (zDistanceToVectorSquared("enemies", "pos") <= dist) {
                        arrowHitEnemy(p, id, 1*yGetVar("arrowsActive", "element"), 
                            yGetVar("arrowsActive", "damage"), 1*yGetVar("arrowsActive", "special"));
                    }
                }
            }
            arrowHit(p);
            removeArrow();
            if (yGetVar("arrowsActive", "special") == FIRE) {
                trDamageUnitPercent(100);
                trUnitChangeProtoUnit("Meteorite");
            } else {
                trUnitChangeProtoUnit("Lightning Sparks");
            }
        }
    }

    /* 
    maintain stun
    */
    for(x=yGetDatabaseCount("stunnedUnits"); >0) {
        yDatabaseNext("stunnedUnits", true);
        trMutateSelected(1*yGetVar("stunnedUnits", "proto"));
        trUnitOverrideAnimation(2, 0, false, true, -1, 0);
    }

    /*
    Player lasers
    */
    for(x=xsMin(3, yGetDatabaseCount("playerLasers")); >0) {
        yDatabaseNext("playerLasers", true);
        if (trTimeMS() > yGetVar("playerLasers", "timeout")) {
            trUnitDestroy();
            yRemoveFromDatabase("playerLasers");
            yRemoveUpdateVar("playerLasers", "timeout");
            yRemoveUpdateVar("playerLasers", "range");
        } else {
            float width = 3.0 * (yGetVar("playerLasers", "timeout") - trTimeMS()) / 500;
            trSetSelectedScale(width, width, yGetVar("playerLasers", "range"));
        }
    }

    xsSetContextPlayer(old);
}
