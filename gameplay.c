void spawnPlayerClone(int p = 0, string vdb = "") {
    int class = trQuestVarGet("p"+p+"class");
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    yAddToDatabase("playerUnits", "next");
    yAddToDatabase("playerCharacters", "next");
    yAddUpdateVar("playerCharacters", "player", p);
    yAddUpdateVar("playerCharacters", "attacking", 0);
    yAddUpdateVar("playerCharacters", "specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
    yAddUpdateVar("playerCharacters", "firstDelay", trQuestVarGet("class"+class+"firstDelay"));
    yAddUpdateVar("playerCharacters", "nextDelay", trQuestVarGet("class"+class+"nextDelay"));
    string proto = kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto"));
    trArmyDispatch(""+p+",0",proto,1,trQuestVarGet(vdb+"x"),0,trQuestVarGet(vdb+"z"),0,true);
}

void spawnPlayer(int p = 0, string vdb = "") {
    trQuestVarSet("p"+p+"unit", trGetNextUnitScenarioNameNumber());
    spawnPlayerClone(p, vdb);
    if (trCurrentPlayer() == p) {
        int class = trQuestVarGet("p"+p+"class");
        string proto = kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto"));
        uiFindType(proto);
    }
}

void checkGodPowers(int p = 0) {
    /* vision ability */
    switch(1*trQuestVarGet("p"+p+"visionCooldownStatus"))
    {
        case ABILITY_READY:
        {
            if (trPlayerGetPopulation(p) >= 9999) {
                yFindLatestReverse("p"+p+"visionObject", "Vision Revealer", p);
                trMutateSelected(kbGetProtoUnitID("Rocket"));
                trQuestVarSet("p"+p+"visionStatus", ABILITY_ON);
                trQuestVarSet("p"+p+"visionCooldownStatus", ABILITY_COOLDOWN);
                trQuestVarSet("p"+p+"visionReadyTime", 
                    trTimeMS() + 1000 * trQuestVarGet("p"+p+"visionCooldown") * trQuestVarGet("p"+p+"cooldownReduction"));
                if (trQuestVarGet("p"+p+"visionIsUltimate") == 1) {
                    trCounterAddTime("vision", 
                        trQuestVarGet("p"+p+"visionCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 1, visionName);
                }
            }
        }
        case ABILITY_COOLDOWN:
        {
            if (trTimeMS() > trQuestVarGet("p"+p+"visionReadyTime")) {
                trQuestVarSet("p"+p+"visionCooldownStatus", ABILITY_COST);
            }
        }
        case ABILITY_COST:
        {
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"visionCost")) {
                trQuestVarSet("p"+p+"visionCooldownStatus", ABILITY_READY);
                if (trCurrentPlayer() == p) {
                    trCounterAddTime("vision", -1, -99999, visionName);
                    if (visionIsUltimate) {
                        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
                    }
                }
                if (trQuestVarGet("p"+p+"silenced") == 0) {
                    trTechGodPower(p, "vision", 1);
                }
            }
        }
    }

    /* rain ability */
    switch(1*trQuestVarGet("p"+p+"rainCooldownStatus"))
    {
        case ABILITY_READY:
        {
            if (trCheckGPActive("rain", p)) {
                trQuestVarSet("p"+p+"rainStatus", ABILITY_ON);
                trQuestVarSet("p"+p+"rainCooldownStatus", ABILITY_COOLDOWN);
                trQuestVarSet("p"+p+"rainReadyTime", 
                    trTimeMS() + 1000 * trQuestVarGet("p"+p+"rainCooldown") * trQuestVarGet("p"+p+"cooldownReduction"));
                if (trQuestVarGet("p"+p+"rainIsUltimate") == 1) {
                    trCounterAddTime("rain", 
                        trQuestVarGet("p"+p+"rainCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 1, rainName);
                }
            }
        }
        case ABILITY_COOLDOWN:
        {
            if (trTimeMS() > trQuestVarGet("p"+p+"rainReadyTime")) {
                trQuestVarSet("p"+p+"rainCooldownStatus", ABILITY_COST);
            }
        }
        case ABILITY_COST:
        {
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"rainCost")) {
                trQuestVarSet("p"+p+"rainCooldownStatus", ABILITY_READY);
                if (trCurrentPlayer() == p) {
                    trCounterAddTime("rain", -1, -99999, rainName);
                    if (rainIsUltimate) {
                        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
                    }
                }
                if (trQuestVarGet("p"+p+"silenced") == 0) {
                    trTechGodPower(p, "rain", 1);
                }
            }
        }
    }

    /* lure ability */
    switch(1*trQuestVarGet("p"+p+"lureCooldownStatus"))
    {
        case ABILITY_READY:
        {
            if (trPlayerUnitCountSpecific(p, "Animal Attractor") == 1) {
                yFindLatestReverse("p"+p+"lureObject", "Animal Attractor", p);
                trMutateSelected(kbGetProtoUnitID("Rocket"));
                trQuestVarSet("p"+p+"lureStatus", ABILITY_ON);
                trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_COOLDOWN);
                trQuestVarSet("p"+p+"lureReadyTime", 
                    trTimeMS() + 1000 * trQuestVarGet("p"+p+"lureCooldown") * trQuestVarGet("p"+p+"cooldownReduction"));
                if (trQuestVarGet("p"+p+"lureIsUltimate") == 1) {
                    trCounterAddTime("lure", 
                        trQuestVarGet("p"+p+"lureCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 1, lureName);
                }
            }
        }
        case ABILITY_COOLDOWN:
        {
            if (trTimeMS() > trQuestVarGet("p"+p+"lureReadyTime")) {
                trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_COST);
            }
        }
        case ABILITY_COST:
        {
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"lureCost")) {
                trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_READY);
                if (trCurrentPlayer() == p) {
                    trCounterAddTime("lure", -1, -99999, lureName);
                    if (lureIsUltimate) {
                        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
                    }
                }
                if (trQuestVarGet("p"+p+"silenced") == 0) {
                    trTechGodPower(p, "animal magnetism", 1);
                }
            }
        }
    }
}


rule gameplay_start
inactive
highFrequency
{
    xsDisableSelf();
    trSetUnitIdleProcessing(true);
    trSetFogAndBlackmap(true, true);
    trPlayerResetBlackMapForAllPlayers();
    trCameraCut(vector(0,70.710701,0), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
    xsEnableRule("gameplay_always");
    xsEnableRule("enemies_always");
    for(p=1; < ENEMY_PLAYER) {
        spawnPlayer(p, "startPosition");
    }
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
    int action = 0;
    int p = 0;
    for (i = 0; < xsMin(yGetDatabaseCount("playerCharacters"), 1 + ENEMY_PLAYER / 3)) {
        id = yDatabaseNext("playerCharacters", true);
        p = yGetVar("playerCharacters", "player");
        action = kbUnitGetAnimationActionType(id);
        if (yGetVar("playerCharacters", "attacking") == 0) {
            if ((action == 12) || (action == 6)) {
                ySetVar("playerCharacters", "attacking", 1);
                ySetVar("playerCharacters", "attackNext", trTimeMS() + yGetVar("playerCharacters", "firstDelay"));
            }
        } else {
            if ((action == 12) || (action == 6)) {
                if (trTimeMS() > yGetVar("playerCharacters", "attackNext")) {
                    ySetVar("playerCharacters", "attackNext", 
                        yGetVar("playerCharacters", "attackNext") + yGetVar("playerCharacters", "nextDelay"));
                    xsSetContextPlayer(p);
                    target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
                    OnHitEffects(p, 1*trQuestVarGet("playerCharacters"), target);
                    /* only melee characters have special attacks */
                    if (action == 6) {
                        ySetVar("playerCharacters", "specialAttack", yGetVar("playerCharacters", "specialAttack") - 1);
                        if (yGetVar("playerCharacters", "specialAttack") <= 0) {
                            ySetVar("playerCharacters", "specialAttack", trQuestVarGet("p"+p+"specialAttackCooldown"));
                        }
                    }
                }
            } else {
                ySetVar("playerCharacters", "attacking", 0);
            }
        }
    }

    /* player units always */
    if (yGetDatabaseCount("playerUnits") > 0) {
        id = yDatabaseNext("playerUnits", true);
        if ((id == -1) || (trUnitAlive() == false)) {
            removePlayerUnit();
        } else {
            stunsAndPoisons("playerUnits");
        }
    }

    /* 
    maintain stun
    */
    for(x=yGetDatabaseCount("stunnedUnits"); >0) {
        id = yDatabaseNext("stunnedUnits", true);
        if (id == -1 || trUnitAlive() == false) {
            yRemoveFromDatabase("stunnedUnits");
        } else {
            trMutateSelected(1*yGetVar("stunnedUnits", "proto"));
            trUnitOverrideAnimation(2, 0, false, true, -1, 0);
        }
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

    /* class functions */
    for(p=1; < ENEMY_PLAYER) {
        trEventFire(12*trQuestVarGet("p"+p+"class") + p);
        checkGodPowers(p);
    }

    xsSetContextPlayer(old);
}
