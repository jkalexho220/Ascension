rule gameplay_shoot
inactive
highFrequency
{
    int old = xsGetContextPlayer();
    int id = 0;
    int target = 0;
    int p = 0;
    for (x = 1; < ENEMY_PLAYER / 4) {
        trQuestVarSet("searchplayer", trQuestVarGet("searchplayer") + 1);
        if (trQuestVarGet("searchplayer") >= ENEMY_PLAYER) {
            trQuestVarSet("searchplayer", 1);
        }
        p = trQuestVarGet("searchplayer");
        id = kbGetBlockID(""+1*trQuestVarGet("p"+p+"unit"), true);
        if (trQuestVarGet("p"+p+"attacking") == false) {
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
                        /*
                        TODO: Activate special attack
                        */
                        switch(1*trQuestVarGet("p"+p+"element2"))
                        {
                            case NONE:
                            {

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