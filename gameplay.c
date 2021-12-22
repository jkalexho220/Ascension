void spawnPlayerClone(int p = 0, string vdb = "") {
    int class = trQuestVarGet("p"+p+"class");
    trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
    yAddToDatabase("p"+p+"characters", "next");
    yAddToDatabase("playerUnits", "next");
    yAddUpdateVar("playerUnits", "player", p);
    yAddToDatabase("playerCharacters", "next");
    yAddUpdateVar("playerCharacters", "player", p);
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
    /* well ability */
    switch(1*trQuestVarGet("p"+p+"wellCooldownStatus"))
    {
        case ABILITY_READY:
        {
            if (trPlayerUnitCountSpecific(p, "Tunnel") == 2) {
                yFindLatest("p"+p+"wellObject", "Tunnel", p);
                trVectorSetUnitPos("p"+p+"wellPos", "p"+p+"wellObject");
                trMutateSelected(kbGetProtoUnitID("Rocket"));
                trUnitDestroy();
                trQuestVarSet("p"+p+"wellStatus", ABILITY_ON);
                trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COOLDOWN);
                trQuestVarSet("p"+p+"wellReadyTime", 
                    trTimeMS() + 1000 * trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction"));
                if (trCurrentPlayer() == p) {
                    trCounterAbort("well");
                    trCounterAddTime("well", 
                        trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 1, wellName);
                }
            } else {
                trQuestVarSet("p"+p+"wellObject", trGetNextUnitScenarioNameNumber() - 1);
            }
        }
        case ABILITY_COOLDOWN:
        {
            if (trTimeMS() > trQuestVarGet("p"+p+"wellReadyTime")) {
                trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COST);
            }
        }
        case ABILITY_COST:
        {
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"wellCost")) {
                trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_READY);
                if (trCurrentPlayer() == p) {
                    trCounterAddTime("well", -1, -99999, wellName);
                    if (wellIsUltimate) {
                        trSoundPlayFN("ui\thunder2.wav","1",-1,"","");
                    }
                }
                if (trQuestVarGet("p"+p+"silenced") == 0) {
                    trTechGodPower(p, "Underworld Passage", 1);
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
                if (trCurrentPlayer() == p) {
                    trCounterAbort("rain");
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
                if (trCurrentPlayer() == p) {
                    trCounterAbort("lure");
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
    trDelayedRuleActivation("gameplay_start_2");
    for(p=1; < ENEMY_PLAYER) {
        chooseClass(p, 1*trQuestVarGet("p"+p+"class"));
    }
}

rule gameplay_start_2
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
        trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COST);
        trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_COST);
        trQuestVarSet("p"+p+"rainCooldownStatus", ABILITY_COST);
        trQuestVarSet("p"+p+"lureObject", trGetNextUnitScenarioNameNumber()-1);
        trQuestVarSet("p"+p+"wellObject", trGetNextUnitScenarioNameNumber()-1);
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
    int p = 0;
    float amt = 0;

    /* player units always */
    if (yGetDatabaseCount("playerUnits") > 0) {
        id = yDatabaseNext("playerUnits", true);
        if ((id == -1) || (trUnitAlive() == false)) {
            removePlayerUnit();
        } else {
            if (yGetVar("playerUnits", "decay") > 0) {
                if (trTimeMS() > yGetVar("playerUnits", "decayNext")) {
                    ySetVar("playerUnits", "decayNext", 1000 + yGetVar("playerUnits", "decayNext"));
                    trDamageUnitPercent(yGetVar("playerUnits", "decay"));
                }
            }

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
            yRemoveUpdateVar("stunnedUnits", "proto");
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

    /* protection */
    if (trQuestVarGet("protectionCount") > 0) {
        for(x=yGetDatabaseCount("playerUnits"); >0) {
            id = yDatabaseNext("playerUnits", true);
            trUnitHighlight(0.2, false);
            xsSetContextPlayer(1*yGetVar("playerUnits", "player"));
            amt = kbUnitGetCurrentHitpoints(id);
            if (amt > yGetVar("playerUnits", "currentHealth")) {
                ySetVar("playerUnits", "currentHealth", amt);
            } else {
                trDamageUnit(amt - yGetVar("playerUnits", "currentHealth"));
            }
        }
    }

    /* sounds */
    if (trQuestVarGet("stunSound") == 1) {
        trQuestVarSet("stunSound", 0);
        trQuestVarSetFromRand("sound", 1, 3, true);
        trSoundPlayFN("woodcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
    }

    xsSetContextPlayer(old);
}
