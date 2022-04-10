
rule find_temple
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
		vector pos = vector(0,0,0);
		trUnitSelect();
		trUnitSelectByQV("temple");
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				xsDisableSelf();
				zSetProtoUnitStat("Revealer", 1, 2, trQuestVarGet("templeLOS"));
				trUnitSelectClear();
				trUnitSelectByQV("templeRevealer");
				trUnitChangeProtoUnit("Revealer");
				trUnitSelectClear();
				trUnitSelectByQV("templeRevealer");
				trUnitConvert(0);
				trQuestVarSet("templeFound", 1);
				trSoundPlayFN("temple.wav","1",-1,"","");
				pos = kbGetBlockPosition(""+1*trQuestVarGet("templeRevealer"));
				trArmyDispatch("1,0","Dwarf",2,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Vision SFX");
				xsEnableRule("temple_always");
				break;
			}
		}
	}
}

rule temple_always
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("temple", true);
	if (trUnitIsSelected()) {
		reselectMyself();
		startNPCDialog(NPC_TEMPLE + trQuestVarGet("stage"));
	}
}

rule shade_temple_init
inactive
highFrequency
{
	if (trTime() > cActivationTime) {
		xsDisableSelf();
		for(x=trQuestVarGet("templeShadesStart"); < trQuestVarGet("templeShadesEnd")) {
			trUnitSelectClear();
			trUnitSelect(""+x);
			trMutateSelected(kbGetProtoUnitID("Columns"));
		}
	}
}

rule shade_temple_always
inactive
highFrequency
{
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trMessageSetText("The Statue's challenge will begin soon! Stay in the room if you wish to participate.",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeNext", trTime() + 10);
		trCounterAddTime("countdown",10,1,"Challenge begins",-1);
	} else if (trQuestVarGet("templeChallengeActive") == 2) {
		if (trTime() >= trQuestVarGet("templeChallengeNext")) {
			trModifyProtounit("Shade XP", 0, 2, 20);
			trModifyProtounit("Shade XP", ENEMY_PLAYER, 2, 20);
			trMessageSetText("Defeat the truth.", -1);
			for(x=trQuestVarGet("templeShadesStart"); < trQuestVarGet("templeShadesEnd")) {
				trUnitSelectClear();
				trUnitSelect(""+x);
				if (trQuestVarGet("templeShadeTrue") == x) {
					trUnitConvert(ENEMY_PLAYER);
				}
				trUnitChangeProtoUnit("Shade XP");
			}
			activateEnemy(1*trQuestVarGet("templeShadeTrue"));
			trQuestVarSet("templeChallengeActive", 3);
			for(p=1;<ENEMY_PLAYER) {
				if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
					if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), trVectorQuestVarGet("templePos")) < 400) {
						xAddDatabaseBlock(dApplicants, true);
						xSetInt(dApplicants, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
						xSetInt(dApplicants, xPlayerOwner, p);
					}
				}
			}
		}
	} else if (trQuestVarGet("templeChallengeActive") == 3) {
		if (trTime() > trQuestVarGet("shadeNoSpecials")) {
			trQuestVarSet("shadeNoSpecials", trTime());
			trModifyProtounit("Shade XP", ENEMY_PLAYER, 9, -99990);
			trModifyProtounit("Shade XP", ENEMY_PLAYER, 9, 99999);
		}
		if (xGetDatabaseCount(dApplicants) > 0) {
			xDatabaseNext(dApplicants, true);
			xUnitSelect(dApplicants, xUnitName);
			if (trUnitAlive() == false) {
				xFreeDatabaseBlock(dApplicants);
			} else if (unitDistanceToVector(xGetInt(dApplicants, xUnitName), trVectorQuestVarGet("templePos")) > 400) {
				if (trCurrentPlayer() == xGetInt(dApplicants, xPlayerOwner)) {
					uiMessageBox("You have left the room and failed the challenge.");
				}
				xFreeDatabaseBlock(dApplicants);
			}
			trUnitSelectClear();
			trUnitSelectByQV("templeShadeTrue");
			if (trUnitAlive() == false) {
				for(x=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					xUnitSelect(dApplicants, xUnitName);
					if (trUnitAlive() && trUnitIsOwnedBy(trCurrentPlayer())) {
						startNPCDialog(NPC_TEMPLE_COMPLETE + 2);
						trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
					}
				}
				for(x=trQuestVarGet("templeShadesStart"); < trQuestVarGet("templeShadesEnd")) {
					trUnitSelectClear();
					trUnitSelect(""+x);
					trUnitDelete(false);
				}
				xsDisableSelf();
			}
		} else {
			trQuestVarSet("templeChallengeActive", 4);
			xsDisableSelf();
			for(x=trQuestVarGet("templeShadesStart"); < trQuestVarGet("templeShadesEnd")) {
				trUnitSelectClear();
				trUnitSelect(""+x);
				trUnitChangeProtoUnit("Hero Death");
			}
		}
	}
}

rule snow_temple_always
inactive
highFrequency
{
	int p = 0;
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trMessageSetText("The Statue's challenge will begin soon! Stay in the room if you wish to participate.",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeNext", trTime() + 10);
		trCounterAddTime("countdown",10,1,"Challenge begins", -1);
	} else if (trQuestVarGet("templeChallengeActive") == 2) {
		if (trTime() > trQuestVarGet("templeChallengeNext")) {
			trUnitSelectClear();
			trUnitSelectByQV("frostDriftSFX");
			trMutateSelected(kbGetProtoUnitID("Ice Sheet"));
			trRenderSnow(0.2);
			trSoundPlayFN("wind.wav","1",-1,"","");
			for(p=1; < ENEMY_PLAYER) {
				if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
					if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), trVectorQuestVarGet("templePos")) < 256) {
						xAddDatabaseBlock(dApplicants, true);
						xSetInt(dApplicants, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
						xSetInt(dApplicants, xPlayerOwner, p);
						if (trCurrentPlayer() == p) {
							trMessageSetText("You may now exit the room. Complete the rest of the stage without dying.",-1);
							trChatSend(0, "<color=1,0,0>You are losing 1 percent of your health each second!</color>");
						}
					}
				}
			}
			trQuestVarSet("templeChallengeActive", 3);
		}
	} else if (trQuestVarGet("templeChallengeActive") == 3) {
		if (trQuestVarGet("playersWon") == 1) {
			trQuestVarSet("templeChallengeActive", 4);
		}
		if (xGetDatabaseCount(dApplicants) > 0) {
			if (trTime() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trTime());
				for(x=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					p = xGetInt(dApplicants, xPlayerOwner);
					if (xGetInt(dPlayerData, xPlayerDead, p) > 0) {
						xFreeDatabaseBlock(dApplicants);
						if (trCurrentPlayer() == p) {
							uiMessageBox("You have died and failed the Statue's challenge.");
						}
					} else {
						xUnitSelect(dApplicants, xUnitName);
						trDamageUnitPercent(1);
					}
				}
			}
		} else {
			xsDisableSelf();
		}
	} else if (trQuestVarGet("templeChallengeActive") == 4) {
		for(x=xGetDatabaseCount(dApplicants); >0) {
			xDatabaseNext(dApplicants);
			p = xGetInt(dApplicants, xPlayerOwner);
			if (trCurrentPlayer() == p) {
				trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
				trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
			}
		}
		xsDisableSelf();
	}
}

rule poison_temple_always
inactive
highFrequency
{
	xDatabaseNext(dPoisonRelics);
	int id = kbGetBlockID(""+xGetInt(dPoisonRelics, xUnitName));
	if (kbGetUnitBaseTypeID(id) == kbGetProtoUnitID("Jiangshi")) {
		if (xGetInt(dPoisonRelics, xUnitName) == trQuestVarGet("correctGoblet")) {
			xUnitSelect(dPoisonRelics, xUnitName);
			trUnitChangeProtoUnit("Recreation");
			trSoundPlayFN("recreation.wav","1",-1,"","");
			xsDisableSelf();
			for(x=xGetDatabaseCount(dPoisonRelics); >1) {
				xDatabaseNext(dPoisonRelics);
				xUnitSelect(dPoisonRelics, xUnitName);
				trUnitChangeProtoUnit("Hero Death");
				xSetPointer(dFreeRelics, xGetInt(dPoisonRelics,xPoisonRelicIndex));
				xFreeDatabaseBlock(dFreeRelics);
			}
			startNPCDialog(NPC_TEMPLE_COMPLETE + trQuestVarGet("stage"));
			trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
		} else {
			xUnitSelect(dPoisonRelics, xUnitName);
			trSoundPlayFN("argusfreezeattack.wav","1",-1,"","");
			trSoundPlayFN("spybirth.wav","1",-1,"","");
			trQuestVarSet("poisonGuesses", trQuestVarGet("poisonGuesses") - 1);
			if (trQuestVarGet("poisonGuesses") > 0) {
				trChatSend(0,"<color=0.8,0.2,0.8>Incorrect! Remaining guesses: " + 1*trQuestVarGet("poisonGuesses"));
				if (trUnitIsOwnedBy(trCurrentPlayer())) {
					if (xGetInt(dPoisonRelics, xUnitName) < trQuestVarGet("correctGoblet")) {
						uiMessageBox("Incorrect. The lifeblood is somewhere to the right of this goblet.");
					} else {
						uiMessageBox("Incorrect. The lifeblood is somewhere to the left of this goblet.");
					}
				}
				xFreeDatabaseBlock(dPoisonRelics);
				trUnitChangeProtoUnit("Lampades Blood");
			} else {
				trSoundPlayFN("tartariangateselect.wav","1",-1,"","");
				trMessageSetText("The death knell tolls. Thus, you have failed the challenge.",-1);
				for(x=xGetDatabaseCount(dPoisonRelics); >1) {
					xDatabaseNext(dPoisonRelics);
					xUnitSelect(dPoisonRelics, xUnitName);
					trUnitChangeProtoUnit("Lampades Blood");
					if (xSetPointer(dFreeRelics, xGetInt(dPoisonRelics,xPoisonRelicIndex))) {
						xFreeDatabaseBlock(dFreeRelics);
					}
				}
				int doomed = trQuestVarGet("doomedPlayers");
				for(x=0; < trQuestVarGet("doomedPlayerCount")) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData, xPlayerUnit, aiPlanGetUserVariableInt(ARRAYS,doomed,x)), true);
					trUnitDelete(false);
				}
				xsDisableSelf();
			}
		}
	}
}

rule greedy_temple_always
inactive
highFrequency
{
	int p = trCurrentPlayer();
	if (trQuestVarGet("p"+p+"relicsSacrificed") == 10) {
		startNPCDialog(NPC_TEMPLE_COMPLETE + 4);
		trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
		xsDisableSelf();
	}
}

rule yeebaagooon_temple_always
inactive
highFrequency
{
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trMessageSetText("The Statue's challenge will begin soon! Stay in the room if you wish to participate.",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeNext", trTime() + 10);
		trCounterAddTime("countdown",10,1,"Challenge begins", -1);
	} else if ((trQuestVarGet("templeChallengeActive") == 2) && (trTime() > trQuestVarGet("templeChallengeNext"))) {
		trQuestVarSet("templeChallengeActive", 1);
		xsEnableRule("yeebaagooon_temple_active");
		xsDisableSelf();
	}
}

rule yeebaagooon_temple_active
inactive
highFrequency
{
	bool hit = false;
	float amt = 0;
	int p = 0;
	vector pos = vector(0,0,0);
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trSoundPlayFN("attackwarning.wav","1",-1,"","");
		trMessageSetText("Stay in the room and survive for 47 seconds.", -1);
		trCounterAddTime("countdown47",48,1,"Survive",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeEnd", trTimeMS() + 47000);
		trQuestVarSet("templeChallengeNext", trTimeMS());
		trQuestVarSet("templeChallengeTimeout", trTimeMS() + 6400);
		trMusicPlay("music\fight\rot loaf.mp3","1",0.0);
		for(p=1; < ENEMY_PLAYER) {
			if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
				xAddDatabaseBlock(dApplicants, true);
				xSetInt(dApplicants, xUnitName, xGetInt(dPlayerData, xPlayerUnit, p));
				xSetInt(dApplicants, xPlayerOwner, p);
			}
		}
		pos = trVectorQuestVarGet("templePos");
		trSoundPlayFN("lightningbirth.wav","1",-1,"","");
		trQuestVarSet("lightningClouds", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Lightning Cloud");
	} else {
		if (xGetDatabaseCount(dApplicants) > 0) {
			xDatabaseNext(dApplicants);
			xUnitSelect(dApplicants, xUnitName);
			if (trUnitAlive() == false) {
				xFreeDatabaseBlock(dApplicants);
			} else {
				pos = kbGetBlockPosition(""+xGetInt(dApplicants, xUnitName), true);
				if (vectorInRectangle(pos, trVectorQuestVarGet("templeRoomLower"), trVectorQuestVarGet("templeRoomUpper")) == false) {
					if (trUnitIsOwnedBy(trCurrentPlayer())) {
						uiMessageBox("You have left the room and have been disqualified.");
					}
					xFreeDatabaseBlock(dApplicants);
				}
			}
		}
		
		if (xGetDatabaseCount(dYeebLightningEnd) > 0) {
			trQuestVarSetFromRand("sound", 1, 5, true);
			hit = false;
			for(y=xGetDatabaseCount(dYeebLightningEnd); >0) {
				xDatabaseNext(dYeebLightningEnd);
				xUnitSelect(dYeebLightningEnd, xUnitName);
				if (trUnitVisToPlayer()) {
					hit = true;
				}
				pos = kbGetBlockPosition(""+xGetInt(dYeebLightningEnd, xUnitName), true);
				trUnitChangeProtoUnit("Lightning sparks");
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 0.75) {
						trUnitDelete(false);
					}
				}
			}
			xClearDatabase(dYeebLightningEnd);
			if (hit) {
				trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
		
		for(x=xsMin(10, xGetDatabaseCount(dYeebLightning)); >0) {
			xDatabaseNext(dYeebLightning);
			if (trTimeMS() > xGetInt(dYeebLightning, xTimeout)) {
				hit = true;
				trChatSetStatus(false);
				trDelayedRuleActivation("enable_chat");
				xSetPointer(dYeebLightningEnd,xAddDatabaseBlock(dYeebLightningEnd));
				xSetInt(dYeebLightningEnd,xUnitName,xGetInt(dYeebLightning,xUnitName));
				xUnitSelect(dYeebLightning,xUnitName);
				trUnitChangeProtoUnit("Militia");
				xUnitSelect(dYeebLightning,xUnitName);
				trSetSelectedScale(0,0,0);
				trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
				xFreeDatabaseBlock(dYeebLightning);
			}
		}
		
		
		if (trQuestVarGet("templeChallengeActive") == 2) {
			if (trTimeMS() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 1000);
				for(x=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					spawnLightning(kbGetBlockPosition(""+xGetInt(dApplicants, xUnitName)));
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
					trQuestVarSet("templeChallengeActive", 3);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 3) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(0,0,4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(2,0,0));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 4);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 4) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trQuestVarSet("templeChallengeActive", 5);
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeTimeout"));
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 6400);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 5) {
			if (trTimeMS() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 500);
				for(x=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					spawnLightning(kbGetBlockPosition(""+xGetInt(dApplicants, xUnitName)));
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
					trQuestVarSet("templeChallengeActive", 6);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 6) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(4,0,0);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(0,0,2));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 7);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 7) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trQuestVarSet("templeChallengeActive", 8);
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeTimeout"));
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 6400);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 8) {
			if (trTimeMS() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 1000);
				trQuestVarSetFromRand("shape", 0, 2, true);
				for(y=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					pos = kbGetBlockPosition(""+xGetInt(dApplicants, xUnitName), true);
					switch(1*trQuestVarGet("shape"))
					{
						case 0:
						{
							trVectorQuestVarSet("dir1", vector(2,0,0));
							trVectorQuestVarSet("dir2", vector(4,0,0));
						}
						case 1:
						{
							trVectorQuestVarSet("dir1", vector(2,0,2));
							trVectorQuestVarSet("dir2", vector(4,0,4));
						}
						case 2:
						{
							trVectorQuestVarSet("dir1", vector(2,0,2));
							trVectorQuestVarSet("dir2", vector(2,0,0));
						}
					}
					spawnLightning(pos);
					for(x=4; >0) {
						spawnLightning(pos + trVectorQuestVarGet("dir1"));
						spawnLightning(pos + trVectorQuestVarGet("dir2"));
						trVectorQuestVarSet("dir1", rotationMatrix(trVectorQuestVarGet("dir1"), 0, 1));
						trVectorQuestVarSet("dir2", rotationMatrix(trVectorQuestVarGet("dir2"), 0, 1));
					}
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
					trQuestVarSet("templeChallengeActive", 9);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 9) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(0,0,4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(2,0,0));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 10);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 10) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trQuestVarSet("templeChallengeActive", 11);
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeTimeout"));
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 6400);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 11) {
			if (trTimeMS() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 500);
				trQuestVarSetFromRand("shape", 0, 2, true);
				for(y=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants);
					pos = kbGetBlockPosition(""+xGetInt(dApplicants, xUnitName), true);
					switch(1*trQuestVarGet("shape"))
					{
						case 0:
						{
							trVectorQuestVarSet("dir1", vector(2,0,0));
							trVectorQuestVarSet("dir2", vector(4,0,0));
						}
						case 1:
						{
							trVectorQuestVarSet("dir1", vector(2,0,2));
							trVectorQuestVarSet("dir2", vector(4,0,4));
						}
						case 2:
						{
							trVectorQuestVarSet("dir1", vector(2,0,2));
							trVectorQuestVarSet("dir2", vector(2,0,0));
						}
					}
					spawnLightning(pos);
					for(x=4; >0) {
						spawnLightning(pos + trVectorQuestVarGet("dir1"));
						spawnLightning(pos + trVectorQuestVarGet("dir2"));
						trVectorQuestVarSet("dir1", rotationMatrix(trVectorQuestVarGet("dir1"), 0, 1));
						trVectorQuestVarSet("dir2", rotationMatrix(trVectorQuestVarGet("dir2"), 0, 1));
					}
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
					trQuestVarSet("templeChallengeActive", 12);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 12) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(4,0,0);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(0,0,2));
			}
			if (amt > 1600) {
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
				trQuestVarSet("templeChallengeActive", 13);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 13) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(0,0,4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(2,0,0));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 14);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 14) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
				trQuestVarSet("templeChallengeActive", 15);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 15) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(4,0,0);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(0,0,2));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 16);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 16) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("templeRoomUpper") - vector(3,0,3));
				trQuestVarSet("templeChallengeActive", 17);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 17) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				pos = trVectorQuestVarGet("lightningStart");
				for(x=0; < 8) {
					spawnLightning(pos);
					pos = pos - vector(0,0,4);
					if (xsVectorGetZ(pos) < trVectorQuestVarGetZ("templeRoomLower")) {
						pos = pos + vector(0,0,32);
					}
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trVectorQuestVarSet("lightningStart", trVectorQuestVarGet("lightningStart") - vector(2,0,2));
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 18);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 18) {
			if (xGetDatabaseCount(dYeebLightningEnd) + xGetDatabaseCount(dYeebLightning) == 0) {
				for(x=xGetDatabaseCount(dApplicants); >0) {
					xDatabaseNext(dApplicants, true);
					if (trUnitAlive() && trUnitIsOwnedBy(trCurrentPlayer())) {
						startNPCDialog(NPC_TEMPLE_COMPLETE + 6);
						trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
					}
				}
				xsDisableSelf();
			}
		}
	}
}


rule monster_temple_init
inactive
highFrequency
{
	if (trTime() > cActivationTime) {
		xsDisableSelf();
		for(x=trQuestVarGet("columnsStart"); < trQuestVarGet("columnsEnd")) {
			trUnitSelectClear();
			trUnitSelect(""+x);
			trUnitChangeProtoUnit("Atlantis Wall Connector");
		}
	}
}

rule monster_temple_always
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trMessageSetText("The Statue's challenge will begin soon! Stay in the room if you wish to participate.",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeNext", trTime() + 10);
		trCounterAddTime("countdown",10,1,"Challenge begins", -1);
	} else if (trQuestVarGet("templeChallengeActive") == 2) {
		if (trTime() > trQuestVarGet("templeChallengeNext")) {
			trQuestVarSet("templeChallengeActive", 3);
		}
	} else if (trQuestVarGet("templeChallengeActive") > 2) {
		bool done = true;
		for(y=trQuestVarGet("monsterChallengeStart"); < trQuestVarGet("monsterChallengeEnd")) {
			trUnitSelectClear();
			trUnitSelect(""+y,true);
			if (trUnitAlive()) {
				done = false;
				break; // no need to check the rest
			}
		}
		if (done) {
			trQuestVarSet("templeChallengeActive", 1 + trQuestVarGet("templeChallengeActive"));
			if (trQuestVarGet("templeChallengeActive") == 9) {
				trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
				xsDisableSelf();
				startNPCDialog(NPC_TEMPLE_COMPLETE + trQuestVarGet("stage"));
				trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
			} else {
				trQuestVarSet("monsterChallengeStart", trGetNextUnitScenarioNameNumber());
				dir = vector(1,0,0);
				pos = trVectorQuestVarGet("templePos");
				for(y=8; >0) {
					x = xsVectorGetX(pos) - xsVectorGetX(dir) * 14;
					z = xsVectorGetZ(pos) - xsVectorGetZ(dir) * 14;
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,x,0,z,0,true);
					trArmySelect(""+ENEMY_PLAYER+",0");
					trSetUnitOrientation(dir,vector(0,1,0),true);
					trUnitChangeProtoUnit(trStringQuestVarGet("enemyProto"+(1*trQuestVarGet("templeChallengeActive")-2)));
					activateEnemy(1*trQuestVarGet("next"));
					dir = rotationMatrix(dir, 0.707107, 0.707107);
				}
				trQuestVarSet("monsterChallengeEnd", trGetNextUnitScenarioNameNumber());
			}
		}
	}
}


rule zeno_temple_always
inactive
highFrequency
{
	float amt = 0;
	vector pos = vector(0,0,0);
	trUnitSelectClear();
	trUnitSelectByQV("akard", true);
	if (trUnitIsSelected()) {
		reselectMyself();
		startNPCDialog(NPC_ZENOS_PARADOX);
	}
	int db = trQuestVarGet("zenoRelicsIncoming");
	if (xGetDatabaseCount(db) >0) {
		xDatabaseNext(db);
		xUnitSelect(db, xUnitName);
		if (trUnitGetIsContained("Unit") == false) {
			xAddDatabaseBlock(dFreeRelics, true);
			xSetInt(dFreeRelics, xUnitName, xGetInt(db, xUnitName));
			xSetInt(dFreeRelics, xRelicType, xGetInt(db, xRelicType));
			if (xGetInt(db, xRelicType) - RELIC_MATH_PROBLEM == trQuestVarGet("questAnswer")) {
				trQuestVarSet("correctRelic", xGetInt(db, xUnitName));
			} else if (xGetInt(db, xRelicType) - RELIC_MATH_PROBLEM == trQuestVarGet("wrongAnswer")) {
				trQuestVarSet("wrongRelic", xGetInt(db, xUnitName));
			} else {
				xAddDatabaseBlock(dZenoRelics, true);
				xSetInt(dZenoRelics, xUnitName, xGetInt(db, xUnitName));
			}
			xFreeDatabaseBlock(db);
		}
	}
	if (trQuestVarGet("templeFound") == 1) {
		if (zDistanceToVectorSquared("correctRelic", "templePos") < 16) {
			trUnitSelectClear();
			trUnitSelectByQV("correctRelic", true);
			trUnitChangeProtoUnit("Implode Sphere Effect");
			trQuestVarSet("templeFound", 2);
			trQuestVarSet("templeNext", trTime() + 4);
			trSoundPlayFN("wonder.wav","1",-1,"","");
			trSoundPlayFN("xpack\xcinematics\8_in\pyramidscrape.mp3","1",-1,"","");
			uiLookAtUnitByName(""+1*trQuestVarGet("temple"));
			trUIFadeToColor(0,0,0,3000,0,true);
			trQuestVarSet("boonUnlocked"+BOON_SPELL_ATTACK, 1);
		}
		if (xGetDatabaseCount(dZenoRelics) > 0) {
			xDatabaseNext(dZenoRelics);
			if (unitDistanceToVector(xGetInt(dZenoRelics, xUnitName), trVectorQuestVarGet("templePos")) < 400) {
				trChatSendSpoofed(0, "Statue of Zeno: WRONG!");
				pos = vectorSnapToGrid(kbGetBlockPosition(""+xGetInt(dZenoRelics, xUnitName)));
				xUnitSelect(dZenoRelics, xUnitName);
				if (trUnitVisToPlayer()) {
					trCameraShake(0.5,0.5);
				}
				trUnitChangeProtoUnit("Meteor Impact Ground");
				trVectorQuestVarSet("left", rotationMatrix(getUnitVector(pos, trVectorQuestVarGet("templePos")), 0.0, 1.0));
				trVectorQuestVarSet("forward", getUnitVector3d(trVectorQuestVarGet("templePos"), pos));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0", "Dwarf", 1, xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitSetStance("Passive");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(trVectorQuestVarGet("forward"),trVectorQuestVarGet("left"),true);
				trUnitHighlight(3.0, false);
				xAddDatabaseBlock(dMirrorTowerLasers, true);
				xSetInt(dMirrorTowerLasers, xUnitName, 1*trQuestVarGet("next"));
				xSetFloat(dMirrorTowerLasers, xMirrorTowerLaserLength,
					distanceBetweenVectors3d(pos, trVectorQuestVarGet("templePos"), false) * 1.25);
				xSetInt(dMirrorTowerLasers, xMirrorTowerLaserTimeout, trTimeMS() + 1500);
				
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Meteor Impact Ground");
				trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
				trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 25.0) {
						trUnitDelete(false);
					}
				}
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies, xUnitName), pos) < 25.0) {
						trUnitDelete(false);
					}
				}
				xFreeDatabaseBlock(dZenoRelics);
			}
		}
		if (xGetDatabaseCount(dMirrorTowerLasers) > 0) {
			xDatabaseNext(dMirrorTowerLasers);
			xUnitSelect(dMirrorTowerLasers, xUnitName);
			amt = xGetInt(dMirrorTowerLasers, xMirrorTowerLaserTimeout) - trTimeMS();
			if (amt < 0) {
				trUnitDestroy();
				xFreeDatabaseBlock(dMirrorTowerLasers);
			} else {
				amt = amt / 75;
				trSetSelectedScale(amt, amt, xGetFloat(dMirrorTowerLasers, xMirrorTowerLaserLength));
			}
		}
	} else if (trQuestVarGet("templeFound") > 1) {
		if (trTime() > trQuestVarGet("templeNext")) {
			switch(1*trQuestVarGet("templeFound"))
			{
				case 2:
				{
					trLetterBox(true);
					trSoundPlayFN("","1",-1,
						"Zenophobia:So you've solved the math problem. Congratulations!",
						"icons\infantry g hoplite icon 64");
					trQuestVarSet("templeNext", trTime() + 4);
				}
				case 3:
				{
					trSoundPlayFN("","1",-1,
						"Zenophobia:Here is your reward. The Blessing of Zenophobia.",
						"icons\infantry g hoplite icon 64");
					trQuestVarSet("templeNext", trTime() + 5);
					trShowImageDialog(boonIcon(BOON_SPELL_ATTACK), boonName(BOON_SPELL_ATTACK));
					gadgetUnreal("ShowImageBox-CloseButton");
				}
				case 4:
				{
					trSoundPlayFN("","1",-1,
						"Zenophobia:You can equip this Blessing in singleplayer.",
						"icons\infantry g hoplite icon 64");
					trQuestVarSet("templeNext", trTime() + 5);
				}
				case 5:
				{
					xsDisableSelf();
					gadgetUnreal("ShowImageBox");
					xsEnableRule("game_over");
					trQuestVarSet("gameOverStep", 4);
					trQuestVarSet("gameOverNext", trTime());
					trQuestVarSet("playersWon", 1);
				}
			}
			trQuestVarSet("templeFound", 1 + trQuestVarGet("templeFound"));
		}
	}
}
