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
        trQuestVarSet("p"+p+"lureObject", trGetNextUnitScenarioNameNumber()-1);
        trQuestVarSet("p"+p+"wellObject", trGetNextUnitScenarioNameNumber()-1);
        for(x=10; >0) {
            if (trQuestVarGet("p"+p+"relic"+x) > 0) {
                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                trArmyDispatch(""+p+",0","Dwarf",1,1,0,1,0,true);
                yAddToDatabase("p"+p+"relics", "next");
                yAddUpdateVar("p"+p+"relics", "type", trQuestVarGet("p"+p+"relic"+x));
                trUnitSelectClear();
                trUnitSelectByQV("next", true);
                trMutateSelected(kbGetProtoUnitID("Relic"));
                trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
                trMutateSelected(relicProto(1*trQuestVarGet("p"+p+"relic"+x)));
                trSetSelectedScale(0,0,0);
                trUnitSetAnimationPath("1,0,1,0,0,0,0");
                relicEffect(1*trQuestVarGet("p"+p+"relic"+x), p, true);
            }
        }
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
    yDatabasePointerDefault("stunnedUnits");
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
            float width = 4.0 * (yGetVar("playerLasers", "timeout") - trTimeMS()) / 500;
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

    /* relics dropped */
    trQuestVarSet("relicPlayer", 1 + trQuestVarGet("relicPlayer"));
    if (trQuestVarGet("relicPlayer") >= ENEMY_PLAYER) {
        trQuestVarSet("relicPlayer", 1);
        p = trQuestVarGet("relicPlayer");
        trUnitSelectClear();
        trUnitSelectByQV("p"+p+"unit");
        if (trUnitAlive()) {
            trVectorSetUnitPos("pos", "p"+p+"unit");
            yDatabasePointerDefault("p"+p+"relics");
            for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
                yDatabaseNext("p"+p+"relics", true);
                if ((zDistanceToVectorSquared("p"+p+"relics", "pos") > 1) &&
                    (trUnitGetIsContained("Unit") == false)) {
                    relicEffect(1*yGetVar("p"+p+"relics", "type"), p, false);
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"relics");
                    if (trCurrentPlayer() == p) {
                        trSoundPlayFN("backtowork.wav","1",-1,"","");
                    }
                    if (trPlayerResourceCount(p, "Gold") > 10 &&
                        zDistanceToVectorSquared("p"+p+"relics", "relicTransporterGuyPos") < 36) {
                        trUnitChangeProtoUnit("Conversion SFX");
                        if (trCurrentPlayer() == p) {
                            trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")) + " added to your warehouse");
                            trSoundPlayFN("favordump.wav","1",-1,"","");
                            trQuestVarSet("ownedRelics"+1*yGetVar("p"+p+"relics", "type"), 
                                xsMin(5, 1 + trQuestVarGet("ownedRelics"+1*yGetVar("p"+p+"relics", "type"))));
                        }
                        trPlayerGrantResources(p, "Gold", -10);
                    } else {
                        trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")) + " dropped.");
                        trUnitChangeProtoUnit("Relic");
                        yAddToDatabase("freeRelics", "p"+p+"relics");
                        yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"relics", "type"));
                    }
                    yRemoveFromDatabase("p"+p+"relics");
                    yRemoveUpdateVar("p"+p+"relics", "type");
                }
            }
        }
    }

    if (yGetDatabaseCount("freeRelics") > 0) {
        yDatabaseNext("freeRelics", true);
        if (trUnitGetIsContained("Unit")) {
            trVectorSetUnitPos("pos", "freeRelics");
            for(p=1; < ENEMY_PLAYER) {
                trUnitSelectClear();
                trUnitSelectByQV("p"+p+"unit");
                if (trUnitAlive()) {
                    if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 1) {
                        break;
                    }
                }
            }
            relicEffect(1*yGetVar("freeRelics", "type"), p, true);
            trUnitSelectClear();
            trUnitSelectByQV("freeRelics", true);
            trMutateSelected(relicProto(1*yGetVar("freeRelics", "type")));
            if (yGetVar("freeRelics", "type") < RELIC_KEY_GREEK) {
                trSetSelectedScale(0,0,0);
                trUnitSetAnimationPath("1,0,1,1,0,0,0");
            }
            if (trCurrentPlayer() == p) {
                trChatSend(0, relicName(1*yGetVar("freeRelics", "type")) + " equipped!");
                trSoundPlayFN("researchcomplete.wav","1",-1,"","");
            }
            yAddToDatabase("p"+p+"relics", "freeRelics");
            yAddUpdateVar("p"+p+"relics", "type", yGetVar("freeRelics", "type"));
            yRemoveFromDatabase("freeRelics");
            yRemoveUpdateVar("freeRelics", "type");
            yRemoveUpdateVar("freeRelics", "selected");
            yRemoveUpdateVar("freeRelics", "enemy");
        } else if (trUnitIsSelected()) {
            if (yGetVar("freeRelics", "selected") == 0) {
                ySetVar("freeRelics", "selected", 1);
                relicDescription(1*yGetVar("freeRelics", "type"));
            }
        } else if (yGetVar("freeRelics", "selected") == 1) {
            ySetVar("freeRelics", "selected", 0);
        }
    }

    /* sounds */
    if (trQuestVarGet("stunSound") == 1) {
        trQuestVarSet("stunSound", 0);  
        trQuestVarSetFromRand("sound", 1, 3, true);
        trSoundPlayFN("woodcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
    }

    /* lifesteal */
    for(p=1; < ENEMY_PLAYER) {
        if (trQuestVarGet("p"+p+"lifestealTotal") > 0) {
            trUnitSelectClear();
            trUnitSelectByQV("p"+p+"unit");
            trDamageUnit(0.0 - trQuestVarGet("p"+p+"lifestealTotal"));
            trQuestVarSet("p"+p+"lifestealTotal", 0);
        }
    }

    xsSetContextPlayer(old);
}
