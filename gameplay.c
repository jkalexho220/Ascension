void removeCamTracks(int eventID = -1) {
    trackRemove();
    trUnblockAllSounds();
}


void fixAnimations(int p = 0) {
    /*
    Prevent hip-thrusting heroes from invisible relic holding
    */
    int action = 0;
    int id = kbGetBlockID(""+1*trQuestVarGet("p"+p+"unit"));
    if (id > 0) {
        trUnitSelectClear();
        trUnitSelectByQV("p"+p+"unit");
        action = kbUnitGetAnimationActionType(id);
        if (trQuestVarGet("p"+p+"animation") == action) {
            if (action == -1) {
                trUnitOverrideAnimation(-1,0,false,true,-1);
            }
        } else {
            trQuestVarSet("p"+p+"animation", action);
            if ((action == 11) && xsAbs(trQuestVarGet("p"+p+"action") - 12) > 0) {
                trUnitSetAnimationPath("0,0,0,0,0,0,0");
            } else if (action == -1) {
                xsSetContextPlayer(p);
                trQuestVarSet("p"+p+"action", kbUnitGetActionType(id));
            }
        }
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
                trDamageUnitPercent(100);
                yFindLatest("p"+p+"wellObject", "Tunnel", p);
                trMutateSelected(kbGetProtoUnitID("Rocket"));
                trDamageUnitPercent(100);
                trQuestVarSet("p"+p+"wellStatus", ABILITY_ON);
                trQuestVarSet("p"+p+"wellCooldownStatus", ABILITY_COOLDOWN);
                trQuestVarSet("p"+p+"wellReadyTime", 
                    trTimeMS() + 1000 * trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction"));
                if (trCurrentPlayer() == p) {
                    trCounterAbort("well");
                    trCounterAddTime("well", 
                        trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 0, wellName);
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
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"wellCost") * trQuestVarGet("p"+p+"ultimateCost")) {
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
                        trQuestVarGet("p"+p+"rainCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 0, rainName);
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
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"rainCost") * trQuestVarGet("p"+p+"ultimateCost")) {
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
                        trQuestVarGet("p"+p+"lureCooldown") * trQuestVarGet("p"+p+"cooldownReduction"), 0, lureName);
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
            if (trPlayerResourceCount(p, "favor") >= trQuestVarGet("p"+p+"lureCost") * trQuestVarGet("p"+p+"ultimateCost")) {
                trQuestVarSet("p"+p+"lureCooldownStatus", ABILITY_READY);
                if (trCurrentPlayer() == p) {
                    trCounterAbort("lure");
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
    trMusicPlayCurrent();
    trPlayNextMusicTrack();
    trSetFogAndBlackmap(true, true);
    trPlayerResetBlackMapForAllPlayers();
}

rule gameplay_start_2
inactive
highFrequency
{
    xsDisableSelf();
    trEventSetHandler(999, "removeCamTracks");
    
    trCameraCut(vector(0,70.710701,0), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
    xsEnableRule("gameplay_always");
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
                trSetSelectedScale(0,0,-1);
                trUnitSetAnimationPath("1,0,1,1,0,0,0");
                relicEffect(1*trQuestVarGet("p"+p+"relic"+x), p, true);
                trQuestVarSet("p"+p+"relic"+x, 0);
            }
        }
        trSetCivilizationNameOverride(p, "Level " + (1+trQuestVarGet("p"+p+"level")));
    }
    trQuestVarSet("nextProj", trGetNextUnitScenarioNameNumber());

    if (Multiplayer) {
        trSetUnitIdleProcessing(true);
    }
    /*
    TESTING STUFF BELOW THIS LINE
    
    
    if (Multiplayer) {
        trUnitSelectClear();
        trUnitSelectByQV("p1unit");
        trUnitTeleport(trQuestVarGet("bossRoomCenterX")-10,0,trQuestVarGet("bossRoomCenterZ")-10);
    }
    */
}

rule gameplay_always
inactive
highFrequency
{
    int old = xsGetContextPlayer();
    int id = 0;
    int p = 0;
    int simp = 0;
    int count = 0;
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
            trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("stunnedUnits", "proto")));
            yRemoveFromDatabase("stunnedUnits");
            yRemoveUpdateVar("stunnedUnits", "proto");
        } else {
            if (xsAbs(trQuestVarGet("stunnedUnits") - trQuestVarGet("bossUnit")) > 0 ||
                trQuestVarGet("bossAnim") == 0) {
                trMutateSelected(1*yGetVar("stunnedUnits", "proto"));
                trUnitOverrideAnimation(2, 0, false, false, -1, 0);
            }
        }
    }

    enemiesAlways();

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
        if (trQuestVarGet("protectionWas") == 0) {
            trQuestVarSet("protectionWas", 1);
            for(p=1; < ENEMY_PLAYER) {
                id = trQuestVarGet("p"+p+"class");
                id = trQuestVarGet("class"+id+"proto");
                trModifyProtounit(kbGetProtoUnitName(id),p,24,1);
                trModifyProtounit(kbGetProtoUnitName(id),p,25,1);
                trModifyProtounit(kbGetProtoUnitName(id),p,26,1);
            }
        }
    } else if (trQuestVarGet("protectionWas") == 1) {
        trQuestVarSet("protectionWas", 0);
        for(p=1; < ENEMY_PLAYER) {
            id = trQuestVarGet("p"+p+"class");
            id = trQuestVarGet("class"+id+"proto");
            trModifyProtounit(kbGetProtoUnitName(id),p,24,-1);
            trModifyProtounit(kbGetProtoUnitName(id),p,25,-1);
            trModifyProtounit(kbGetProtoUnitName(id),p,26,-1);
        }
    }

    /* relics dropped */
    trQuestVarSet("relicPlayer", 1 + trQuestVarGet("relicPlayer"));
    if (trQuestVarGet("relicPlayer") >= ENEMY_PLAYER) {
        trQuestVarSet("relicPlayer", 1);
    }
    p = trQuestVarGet("relicPlayer");
    trUnitSelectClear();
    trUnitSelectByQV("p"+p+"unit");
    if (trUnitAlive() && trQuestVarGet("p"+p+"launched") == 0 && trQuestVarGet("p"+p+"dead") <= 0) {
        trVectorSetUnitPos("pos", "p"+p+"unit");
        for(x=yGetDatabaseCount("p"+p+"relics"); >0) {
            yDatabaseNext("p"+p+"relics", true);
            if (trUnitGetIsContained("Unit") == false) {
                if (yGetVar("p"+p+"relics", "type") < RELIC_KEY_GREEK) {
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"relics");
                    if (trCurrentPlayer() == p) {
                        trSoundPlayFN("backtowork.wav","1",-1,"","");
                    }
                    if (zDistanceBetweenVectorsSquared("pos", "relicTransporterGuyPos") < 36) {
                        trUnitChangeProtoUnit("Relic");
                        trUnitSelectClear();
                        trUnitSelectByQV("p"+p+"relics", true);
                        trImmediateUnitGarrison(""+1*trQuestVarGet("p"+p+"unit"));
                        trMutateSelected(relicProto(1*yGetVar("p"+p+"relics", "type")));
                        trSetSelectedScale(0,0,-1);
                        trUnitSetAnimationPath("1,0,1,1,0,0,0");
                        if (trQuestVarGet("p"+p+"transporterPurchased") == 0) {
                            if (trPlayerResourceCount(p, "gold") >= 100) {
                                trPlayerGrantResources(p, "gold", -100);
                                trQuestVarSet("p"+p+"transporterPurchased", 1);
                                trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
                                yAddToDatabase("playerUnits", "next");
                                yAddUpdateVar("playerUnits", "player", p);
                                trArmyDispatch(""+p+",0","Villager Atlantean Hero",1,
                                    trQuestVarGet("posx"),0,trQuestVarGet("posz"),180,true);
                                if (trCurrentPlayer() == p) {
                                    trChatSend(0, "A Relic Transporter has been hired!");
                                    trSoundPlayFN("favordump.wav","1",-1,"","");
                                    trSoundPlayFN("villagercreate.wav","1",-1,"","");
                                }
                            } else if (trCurrentPlayer() == p) {
                                trChatSend(0, "You don't have enough gold! You need 100!");
                                trSoundPlayFN("cantdothat.wav","1",-1,"","");
                            }
                        }
                    } else {
                        if (trCurrentPlayer() == p) {
                            trChatSend(0, relicName(1*yGetVar("p"+p+"relics", "type")) + " dropped.");
                        }
                        relicEffect(1*yGetVar("p"+p+"relics", "type"), p, false);
                        trUnitChangeProtoUnit("Relic");
                        yAddToDatabase("freeRelics", "p"+p+"relics");
                        yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"relics", "type"));
                        yRemoveFromDatabase("p"+p+"relics");
                        yRemoveUpdateVar("p"+p+"relics", "type");
                    }
                } else {
                    /* RELIC_KEY_GREEK */
                    trSoundPlayFN("storehouse.wav","1",-1,"","");
                    yRemoveFromDatabase("p"+p+"relics");
                    yRemoveUpdateVar("p"+p+"relics", "type");
                }
            }
        }
    }

    if (yGetDatabaseCount("p"+p+"warehouse") > 0) {
        yDatabaseNext("p"+p+"warehouse", true);
        if ((trUnitGetIsContained("Villager Atlantean Hero") || trUnitGetIsContained("Cinematic Block")) == false) {
            if (yGetVar("p"+p+"warehouse", "type") < RELIC_KEY_GREEK ||
                trPlayerUnitCountSpecific(p, "Villager Atlantean Hero") == 0) {
                trUnitChangeProtoUnit("Relic");
                yAddToDatabase("freeRelics", "p"+p+"warehouse");
                yAddUpdateVar("freeRelics", "type", yGetVar("p"+p+"warehouse", "type"));
            } else {
                trSoundPlayFN("storehouse.wav","1",-1,"","");
            }
            yRemoveFromDatabase("p"+p+"warehouse");
        }
    }

    /* free relics */
    if (Multiplayer) {
        count = 1;
    } else {
        count = 30;
    }
    for (x=xsMin(count, yGetDatabaseCount("freeRelics")); > 0) {
        amt = 0;
        yDatabaseNext("freeRelics", true);
        if (trUnitGetIsContained("Unit")) {
            if (trUnitGetIsContained("Villager Atlantean Hero")) {
                for(p=1; < ENEMY_PLAYER) {
                    if (trUnitIsOwnedBy(p)) {
                        trSetSelectedScale(0,0,-1);
                        trMutateSelected(relicProto(1*yGetVar("freeRelics", "type")));
                        if (yGetVar("freeRelics", "type") < RELIC_KEY_GREEK) {
                            trUnitSetAnimationPath("1,0,1,1,0,0,0");
                        }
                        yAddToDatabase("p"+p+"warehouse", "freeRelics");
                        yAddUpdateVar("p"+p+"warehouse", "type", yGetVar("freeRelics", "type"));
                        yRemoveFromDatabase("freeRelics");
                        yRemoveUpdateVar("freeRelics", "type");
                        break;
                    }
                }
            } else {
                trVectorSetUnitPos("pos", "freeRelics");
                for(p=1; < ENEMY_PLAYER) {
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"unit");
                    if (trUnitAlive()) {
                        if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 1.5) {
                            amt = 1;
                            break;
                        }
                    }
                }
                if (amt == 1) {
                    relicEffect(1*yGetVar("freeRelics", "type"), p, true);
                    trUnitSelectClear();
                    trUnitSelectByQV("freeRelics", true);
                    trSetSelectedScale(0,0,-1);
                    trMutateSelected(relicProto(1*yGetVar("freeRelics", "type")));
                    if (yGetVar("freeRelics", "type") < RELIC_KEY_GREEK) {
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
                }
            }
        } else if (trUnitIsSelected()) {
            relicDescription(1*yGetVar("freeRelics", "type"));
            reselectMyself();
        }
    }

    /* sounds */
    if (trQuestVarGet("stunSound") == 1) {
        trQuestVarSet("stunSound", 0);  
        trQuestVarSetFromRand("sound", 1, 3, true);
        trSoundPlayFN("woodcrush"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
    }

    if (trQuestVarGet("poisonSound") == 1) {
        trQuestVarSet("poisonSound", 0);
        trSoundPlayFN("lampadesblood.wav","1",-1,"","");
        trSoundPlayFN("carnivorabirth.wav","1",-1,"","");
    }

    /* misc */
    for(p=1; < ENEMY_PLAYER) {
        if (trQuestVarGet("p"+p+"dead") == 0) {
            trUnitSelectClear();
            trUnitSelectByQV("p"+p+"unit");
            /* lifesteal */
            if (trQuestVarGet("p"+p+"lifestealTotal") > 0) {
                healUnit(p, trQuestVarGet("p"+p+"lifestealTotal"), 1*trQuestVarGet("p"+p+"index"));
                /* simp benefits */
                if (trQuestVarGet("p"+p+"simp") > 0) {
                    simp = trQuestVarGet("p"+p+"simp");
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+simp+"unit");
                    healUnit(p, trQuestVarGet("p"+p+"lifestealTotal"), 1*trQuestVarGet("p"+simp+"index"));
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"unit");
                }
                trQuestVarSet("p"+p+"lifestealTotal", 0);
            }
            if (Multiplayer) {
                fixAnimations(p);
            }
            /* undo silence */
            if (trQuestVarGet("p"+p+"silenced") == 1) {
                if (trTimeMS() > trQuestVarGet("p"+p+"silenceTimeout")) {
                    trQuestVarSet("p"+p+"silenced", 0);
                    if (trQuestVarGet("p"+p+"wellCooldownStatus") == ABILITY_READY) {
                        trTechGodPower(p, "Underworld Passage", 1);
                    }
                    if (trQuestVarGet("p"+p+"lureCooldownStatus") == ABILITY_READY) {
                        trTechGodPower(p, "Animal magnetism", 1);
                    }
                    if (trQuestVarGet("p"+p+"rainCooldownStatus") == ABILITY_READY) {
                        trTechGodPower(p, "rain", 1);
                    }
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"silenceSFX");
                    trUnitChangeProtoUnit("Cinematic Block");
                }
            }
        } else if (trTimeMS() > trQuestVarGet("p"+p+"reviveNext")) {
            count = 0;
            for(x=yGetDatabaseCount("enemies"); >0) {
                id = yDatabaseNext("enemies", true);
                if (id == -1 || trUnitAlive() == false) {
                    removeEnemy();
                } else if (zDistanceToVectorSquared("enemies", "dead"+p+"pos") < 225) {
                    count = count - 1;
                }
            }
            trQuestVarSet("p"+p+"reviveNext", trTimeMS() + 1000);
            trQuestVarSet("playersReviving", 0);
            for(x=yGetDatabaseCount("playerCharacters"); >0) {
                id = yDatabaseNext("playerCharacters", true);
                if (id == -1 || trUnitAlive() == false) {
                    removePlayerCharacter();
                } else if (zDistanceToVectorSquared("playerCharacters", "dead"+p+"pos") < 100) {
                    count = count + 1;
                    trQuestVarSet("playersReviving", 1);
                }
            }
            if (Multiplayer == false) {
                trQuestVarSet("playersReviving", 1);
                count = 1;
            }
            if (trQuestVarGet("playersReviving") == 1) {
                if (count <= 0) {
                    trChatSend(0, "<color={Playercolor("+p+")}>{Playername("+p+")}</color> can't be revived. Too many enemies nearby!");
                } else {
                    trQuestVarSet("p"+p+"dead", xsMax(0, trQuestVarGet("p"+p+"dead") - count));
                    trChatSend(0, 
                        "<color={Playercolor("+p+")}>{Playername("+p+")}</color> is being revived: " + 1*trQuestVarGet("p"+p+"dead"));
                }
                if (trQuestVarGet("p"+p+"dead") <= 0) {
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"reviveBeam");
                    trUnitChangeProtoUnit("Rocket");
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"unit");
                    trUnitDestroy();
                    spawnPlayer(p, "dead"+p+"pos");
                    trSoundPlayFN("herorevived.wav","1",-1,"","");
                    trUnitSelectClear();
                    trUnitSelectByQV("p"+p+"unit");
                    trDamageUnitPercent(50);
                    trQuestVarSet("deadPlayerCount", trQuestVarGet("deadPlayerCount") - 1);
                    equipRelicsAgain(p);
                    if (trCurrentPlayer() == p) {
                        uiFindType(kbGetProtoUnitName(id));
                    }
                }
            }
        }
    }

    /* sky passages */
    if (yGetDatabaseCount("skyPassages") > 0) {
        yDatabaseNext("skyPassages");
        trVectorSetUnitPos("pos", "skyPassages");
        trQuestVarSet("sound", 0);
        for(x=yGetDatabaseCount("playerUnits"); >0) {
            yDatabaseNext("playerUnits");
            if (zDistanceToVectorSquared("playerUnits", "pos") < 6) {
                yAddToDatabase("magicalJourney", "playerUnits");
                if (yGetVar("playerUnits", "hero") == 1) {
                    if (trCurrentPlayer() == yGetVar("playerUnits", "player")) {
                        uiLookAtUnitByName(""+1*yGetVar("skyPassages", "exit"));
                        trQuestVarSet("sound", 1);
                    }
                }
            }
        }

        if (yGetDatabaseCount("magicalJourney") > 0) {
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
            trUnitChangeProtoUnit("Dwarf");
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
            trMutateSelected(kbGetProtoUnitID("Sky Passage"));
            for(x=yGetDatabaseCount("magicalJourney"); >0) {
                yDatabaseNext("magicalJourney", true);
                trImmediateUnitGarrison(""+1*yGetVar("skyPassages", "exit"));
            }
            yClearDatabase("magicalJourney");
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
            trUnitEjectContained();
            trUnitChangeProtoUnit("Spy Eye");
            trUnitSelectClear();
            trUnitSelect(""+1*yGetVar("skyPassages", "exit"));
            trMutateSelected(kbGetProtoUnitID("Sky Passage"));
            trSetSelectedScale(0, 0.2, 0);
        }
        if (trQuestVarGet("sound") == 1) {
            trSoundPlayFN("skypassageout.wav","1",-1,"","");
        }
    }

    for(x=xsMin(5, yGetDatabaseCount("launchedUnits")); >0) {
        processLaunchedUnit();
    }

    if (yGetDatabaseCount("decayingWolves") > 0) {
        yDatabaseNext("decayingWolves");
        if (trTimeMS() > yGetVar("decayingWolves", "timeout")) {
            trUnitSelectClear();
            trUnitSelectByQV("decayingWolves", true);
            trUnitChangeProtoUnit("Dust Small");
            yRemoveFromDatabase("decayingWolves");
        }
    }

    processChests();
    processWalls();

    xsSetContextPlayer(old);

    /* GAME OVER */
    if (trQuestVarGet("deadPlayerCount") == trQuestVarGet("activePlayerCount") && Multiplayer) {
        xsDisableSelf();
        trSoundPlayFN("lose.wav","1",-1,"","");
        trUIFadeToColor(0,0,0,1500,0,true);
        trLetterBox(true);
        trQuestVarSet("gameOverNext", trTime() + 2);
        xsEnableRule("game_over");
    }

    /* No specials */
    if (trTime() > trQuestVarGet("noSpecialsNext")) {
        trQuestVarSet("noSpecialsNext", trTime());
        for(p=ENEMY_PLAYER; >0) {
            trModifyProtounit("Frost Giant", p, 9, -99990);
            trModifyProtounit("Frost Giant", p, 9, 99999);
            trModifyProtounit("Medusa", p, 9, -99990);
            trModifyProtounit("Medusa", p, 9, 99999);
            trModifyProtounit("Mummy", p, 9, -99990);
            trModifyProtounit("Mummy", p, 9, 99999);
        }
    }
}

rule game_over
inactive
highFrequency
{
    if (trTime() > trQuestVarGet("gameOverNext")) {
        trQuestVarSet("gameOverStep", 1 + trQuestVarGet("gameOverStep"));
        switch(1*trQuestVarGet("gameOverStep"))
        {
            case 1:
            {
                trSoundPlayFN("default","1",-1,"Zenophobia: Your journey is not over! This map remembers your progress!",
                    "icons\infantry g hoplite icon 64");
                trQuestVarSet("gameOverNext", trTime() + 4);
            }
            case 2:
            {
                trSoundPlayFN("default", "1",-1,
                    "Zenophobia: Your level, gold, and relics have been saved! The next time you play, you will keep them!",
                    "icons\infantry g hoplite icon 64");
                trQuestVarSet("gameOverNext", trTime() + 6);
            }
            case 3:
            {
                trSoundPlayFN("default", "1",-1,
                    "Zenophobia: Be sure to also play this map in singleplayer to switch classes, level up, and more!",
                    "icons\infantry g hoplite icon 64");
                trQuestVarSet("gameOverNext", trTime() + 6);
            }
            case 4:
            {
                trQuestVarSet("gameOverNext", trTime() + 1);
                trUIFadeToColor(0,0,0,1000,0,false);
                trLetterBox(false);
                saveAllData();
            }
            case 5:
            {
                xsDisableSelf();
                for(p=1; < ENEMY_PLAYER) {
                    if (trQuestVarGet("playersWon") == 1) {
                        trSetPlayerWon(p);
                    } else {
                        trSetPlayerDefeated(p);
                    }
                }
                trEndGame();
            }
        }
    }
}
