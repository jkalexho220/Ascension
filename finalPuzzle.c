
void turnPuzzleStatue(int index = 0, bool first = false, bool immediate = false) {
    int old = xGetPointer(puzzleStatues);
    if (index > 0) {
        xSetPointer(puzzleStatues, index);
    }
    xSetFloat(puzzleStatues, xStatueAngle, fModulo(6.283185, 1.570796 + xGetFloat(puzzleStatues, xStatueAngle)));
    xSetInt(puzzleStatues, xStatuePosition, 1 + xGetInt(puzzleStatues, xStatuePosition));
    if (xGetInt(puzzleStatues, xStatuePosition) >= 4) {
        xSetInt(puzzleStatues, xStatuePosition, 0);
        trQuestVarSet("puzzleCorrect", trQuestVarGet("puzzleCorrect") + 1);
    } else if (xGetInt(puzzleStatues, xStatuePosition) == 1) {
        trQuestVarSet("puzzleCorrect", trQuestVarGet("puzzleCorrect") - 1);
    }
    xUnitSelect(puzzleStatues, xUnitName);
    trDamageUnitPercent(-100);
    if (immediate) {
        trVectorSetFromAngle("dir", xGetFloat(puzzleStatues, xStatueAngle));
        trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
    } else {
        trUnitConvert(0);
        if (xGetInt(puzzleStatues, xStatueState) == 1) {
            xSetInt(puzzleStatues, xStatueTimeout, 1000 + xGetInt(puzzleStatues, xStatueTimeout));
        } else {
            xSetInt(puzzleStatues, xStatueState, 1);
            xSetInt(puzzleStatues, xStatueTimeout, trTimeMS() + 1000);
            trQuestVarSet("puzzleMoving", 1 + trQuestVarGet("puzzleMoving"));
        }
    }

    if (first) {
        vector pos = xGetVector(puzzleStatues, xStatuePos);
        int x = xsVectorGetX(pos);
        int z = xsVectorGetZ(pos);
        for(i=0; <4) {
            if (i != x) {
                turnPuzzleStatue(aiPlanGetUserVariableInt(puzzleDB, i, z), false, immediate);
            }
            if (i != z) {
                turnPuzzleStatue(aiPlanGetUserVariableInt(puzzleDB, x, i), false, immediate);
            }
        }
        if (immediate == false) {
            trSoundPlayFN("trojangateopen.wav","1",-1,"","");
        }
    }
    
    if (index > 0) {
        xSetPointer(puzzleStatues, old);
    }
}

rule puzzle_room_find
inactive
highFrequency
{
    vector pos = xGetVector(dPlayerUnits, xUnitPos);
    vector center = trVectorQuestVarGet("puzzleRoomCenter");
    if (xsMax(xsAbs(xsVectorGetX(pos) - xsVectorGetX(center)), xsAbs(xsVectorGetZ(pos) - xsVectorGetZ(center))) < 20) {
        trUnitSelectClear();
        trUnitSelectByQV("puzzleRoomRevealer");
        zSetProtoUnitStat("Revealer to Player", 0, 2, 20);
        trUnitChangeProtoUnit("Revealer to Player");
        trUnitSelectClear();
        trUnitSelectByQV("puzzleRoomRevealer");
        trUnitConvert(1);
        xsDisableSelf();
        xsEnableRule("puzzle_room_always");
        trSoundPlayFN("visionswoosh.wav","1",-1,"","");
    }
}

rule puzzle_room_always
inactive
highFrequency
{
    float angle = 0;
    for(x=xGetDatabaseCount(puzzleStatues); >0) {
        xDatabaseNext(puzzleStatues);
        xUnitSelect(puzzleStatues,xUnitName);
        switch(xGetInt(puzzleStatues, xStatueState))
        {
            case 0:
            {
                /* waiting */
                if (trUnitPercentDamaged() > 0) {
                    turnPuzzleStatue(0,true);
                }
            }
            case 1:
            {
                /* turning */
                angle = 0.001 * (xGetInt(puzzleStatues, xStatueTimeout) - trTimeMS());
                if (angle < 0) {
                    angle = xGetFloat(puzzleStatues, xStatueAngle);
                    xSetInt(puzzleStatues, xStatueState, 0);
                    trUnitConvert(ENEMY_PLAYER);
                    trDamageUnitPercent(-100);
                    trQuestVarSet("puzzleMoving", trQuestVarGet("puzzleMoving") - 1);
                } else {
                    angle = fModulo(6.283185, xGetFloat(puzzleStatues, xStatueAngle) - angle * 1.570796);
                }
                trVectorSetFromAngle("dir", angle);
                trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
            }
        }
    }
    if ((trQuestVarGet("puzzleCorrect") == 0) &&
        trQuestVarGet("puzzleMoving") == 0) {
        
        trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
        for(x=xGetDatabaseCount(puzzleStatues); >0) {
            xDatabaseNext(puzzleStatues);
            xUnitSelect(puzzleStatues,xUnitName);
            trDamageUnitPercent(-100);
            trUnitConvert(0);
        }

        if (trQuestVarGet("p"+trCurrentPlayer()+"swordpieceQuest"+SWORD_BLADE) == 1) {
            uiMessageBox("Acquired: Blade of an unknown sword.");
            trQuestVarSet("p"+trCurrentPlayer()+"swordPiece"+SWORD_BLADE, 1);
        }
        xsDisableSelf();
    }
}