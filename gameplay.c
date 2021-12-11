rule gameplay_always
inactive
highFrequency
{
    int old = xsGetContextPlayer();
    int id = 0;
    int target = 0;
    int p = 0;
    float dist = 0;
    for (i = 1; < ENEMY_PLAYER / 4) {
        trQuestVarSet("searchplayer", 1 + trQuestVarGet("searchplayer"));
        if (trQuestVarGet("searchplayer") >= ENEMY_PLAYER) {
            trQuestVarSet("searchplayer", 1);
        }
        p = trQuestVarGet("searchplayer");
        id = kbGetBlockID(""+1*trQuestVarGet("p"+p+"unit"), true);
        if (trQuestVarGet("p"+p+"attacking") == 0) {
            if (kbUnitGetAnimationActionType(id) == 12) {
                trQuestVarSet("p"+p+"attacking", 1);
                trQuestVarSet("p"+p+"attackNext", trTimeMS() + 500);
            }
        } else {
            if (kbUnitGetAnimationActionType(id) == 12) {
                if (trTimeMS() > trQuestVarGet("p"+p+"attackNext")) {
                    trQuestVarSet("p"+p+"attackNext", trTimeMS() + 1000 / trQuestVarGet("p"+p+"attackSpeed"));
                    xsSetContextPlayer(p);
                    target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
                    trVectorSetUnitPos("start", "p"+p+"unit");
                    trVectorQuestVarSet("end", kbGetBlockPosition(""+target, true));

                    trQuestVarSet("p"+p+"specialAttack", trQuestVarGet("p"+p+"specialAttack") - 1);
                    if (trQuestVarGet("p"+p+"specialAttack") == 0) {
                        trQuestVarSet("p"+p+"specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
                        shootArrow(p, 1*trQuestVarGet("p"+p+"element1"), "start", "end", 
                            trQuestVarGet("p"+p+"attack"), 1*trQuestVarGet("p"+p+"element2"));
                        switch(1*trQuestVarGet("p"+p+"element2"))
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
                        shootArrow(p, 1*trQuestVarGet("p"+p+"element1"), "start", "end", trQuestVarGet("p"+p+"attack"));
                    }
                }
            } else {
                trQuestVarSet("p"+p+"attacking", 0);
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
        } else if (trCountUnitsInArea(""+1*trQuestVarGet("arrowsActive"),ENEMY_PLAYER,"Unit",2) > 0) {
            p = yGetVar("arrowsActive", "player");
            trVectorSetUnitPos("pos", "arrowsActive");
            if (yGetVar("arrowsActive", "special") == FIRE) {
                dist = 16;
                trDamageUnitPercent(100);
                trUnitChangeProtoUnit("Meteorite");
            } else {
                dist = 4;
                trUnitChangeProtoUnit("Lightning Sparks");
            }
            for(x=yGetDatabaseCount("enemies"); >0) {
                id = yDatabaseNext("enemies", true);
                if (id == -1 || trUnitAlive() == false) {
                    removeEnemy();
                } else {
                    if (zDistanceToVectorSquared("enemies", "pos") <= dist) {
                        trDamageUnit(yGetVar("arrowsActive", "damage"));
                        if (yGetVar("arrowsActive", "special") == ICE && kbUnitGetCurrentHitpoints(id) > 0) {
                            stunUnit("enemies", 1.5 * trQuestVarGet("p"+p+"spellDuration"));
                        }
                    }
                }
            }
            removeArrow();
        }
    }

    xsSetContextPlayer(old);
}