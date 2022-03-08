
rule find_temple
inactive
highFrequency
{
	if (trQuestVarGet("play") == 1) {
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
				trVectorSetUnitPos("pos", "templeRevealer");
				trArmyDispatch("1,0","Dwarf",2,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
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

rule yeebaagooon_temple_always
inactive
highFrequency
{
	bool hit = false;
	float amt = 0;
	int p = 0;
	if (trQuestVarGet("templeChallengeActive") == 1) {
		trSoundPlayFN("attackwarning.wav","1",-1,"","");
		trMessageSetText("Stay in the room and survive for 47 seconds.", -1);
		trCounterAddTime("countdown47",,48,1,"Survive",-1);
		trQuestVarSet("templeChallengeActive", 2);
		trQuestVarSet("templeChallengeEnd", trTimeMS() + 47000);
		trQuestVarSet("templeChallengeNext", trTimeMS());
		trQuestVarSet("templeChallengeTimeout", trTimeMS() + 6400);
		trMusicPlay("music\fight\rot loaf.mp3","1",0.0);
		for(p=1; < ENEMY_PLAYER) {
			if (trQuestVarGet("p"+p+"dead") == 0) {
				yAddToDatabase("applicants", "p"+p+"unit");
			}
		}
		trSoundPlayFN("lightningbirth.wav","1",-1,"","");
		trQuestVarSet("lightningClouds", trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("templePosX"),0,trQuestVarGet("templePosZ"),0,true);
		trArmySelect("1,0");
		trUnitChangeProtoUnit("Lightning Cloud");
	} else {
		if (yGetDatabaseCount("applicants") > 0) {
			yDatabaseNext("applicants", true);
			if (trUnitAlive() == false) {
				yRemoveFromDatabase("applicants");
			} else {
				trVectorSetUnitPos("pos", "applicants");
				if (vectorInRectangle("pos", "templeRoomLower", "templeRoomUpper") == false) {
					if (trUnitIsOwnedBy(trCurrentPlayer())) {
						uiMessageBox("You have left the room and have been disqualified.");
					}
					yRemoveFromDatabase("applicants");
				}
			}
		}
		
		if (yGetDatabaseCount("yeebLightningEnd") > 0) {
			trQuestVarSetFromRand("sound", 1, 5, true);
			hit = false;
			for(y=yGetDatabaseCount("yeebLightningEnd"); >0) {
				yDatabaseNext("yeebLightningEnd", true);
				if (trUnitVisToPlayer()) {
					hit = true;
				}
				trVectorSetUnitPos("pos", "yeebLightningEnd");
				trUnitChangeProtoUnit("Lightning sparks");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 0.75) {
						trUnitDelete(false);
					}
				}
			}
			yClearDatabase("yeebLightningEnd");
			if (hit) {
				trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			}
		}
		
		for(x=xsMin(12, yGetDatabaseCount("yeebLightning")); >0) {
			yDatabaseNext("yeebLightning");
			if (trTimeMS() > yGetVar("yeebLightning", "timeout")) {
				hit = true;
				trChatSetStatus(false);
				trDelayedRuleActivation("enable_chat");
				yAddToDatabase("yeebLightningEnd", "yeebLightning");
				trUnitSelectClear();
				trUnitSelectByQV("yeebLightning", true);
				trUnitChangeProtoUnit("Militia");
				trUnitSelectClear();
				trUnitSelectByQV("yeebLightning", true);
				trSetSelectedScale(0,0,0);
				trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
				yRemoveFromDatabase("yeebLightning");
			}
		}
		
		
		if (trQuestVarGet("templeChallengeActive") == 2) {
			if (trTimeMS() > trQuestVarGet("templeChallengeNext")) {
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 1000);
				for(x=yGetDatabaseCount("applicants"); >0) {
					yDatabaseNext("applicants");
					trVectorSetUnitPos("pos", "applicants");
					spawnLightning("pos");
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
					trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
					trQuestVarSet("templeChallengeActive", 3);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 3) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posz", trQuestVarGet("posz") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartX", trQuestVarGet("lightningStartX") - 2);
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
				for(x=yGetDatabaseCount("applicants"); >0) {
					yDatabaseNext("applicants");
					trVectorSetUnitPos("pos", "applicants");
					spawnLightning("pos");
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
					trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
					trQuestVarSet("templeChallengeActive", 6);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 6) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posx", trQuestVarGet("posx") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartZ", trQuestVarGet("lightningStartZ") - 2);
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
				for(y=yGetDatabaseCount("applicants"); >0) {
					yDatabaseNext("applicants");
					trVectorSetUnitPos("center", "applicants");
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
					spawnLightning("center");
					for(x=4; >0) {
						trQuestVarSet("posx", trQuestVarGet("centerx") + trQuestVarGet("dir1x"));
						trQuestVarSet("posz", trQuestVarGet("centerz") + trQuestVarGet("dir1z"));
						spawnLightning("pos");
						trQuestVarSet("posx", trQuestVarGet("centerx") + trQuestVarGet("dir2x"));
						trQuestVarSet("posz", trQuestVarGet("centerz") + trQuestVarGet("dir2z"));
						spawnLightning("pos");
						trVectorQuestVarSet("dir1", rotationMatrix("dir1", 0, 1));
						trVectorQuestVarSet("dir2", rotationMatrix("dir2", 0, 1));
					}
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
					trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
					trQuestVarSet("templeChallengeActive", 9);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 9) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posz", trQuestVarGet("posz") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartx", trQuestVarGet("lightningStartx") - 2);
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
				for(y=yGetDatabaseCount("applicants"); >0) {
					yDatabaseNext("applicants");
					trVectorSetUnitPos("center", "applicants");
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
					spawnLightning("center");
					for(x=4; >0) {
						trQuestVarSet("posx", trQuestVarGet("centerx") + trQuestVarGet("dir1x"));
						trQuestVarSet("posz", trQuestVarGet("centerz") + trQuestVarGet("dir1z"));
						spawnLightning("pos");
						trQuestVarSet("posx", trQuestVarGet("centerx") + trQuestVarGet("dir2x"));
						trQuestVarSet("posz", trQuestVarGet("centerz") + trQuestVarGet("dir2z"));
						spawnLightning("pos");
						trVectorQuestVarSet("dir1", rotationMatrix("dir1", 0, 1));
						trVectorQuestVarSet("dir2", rotationMatrix("dir2", 0, 1));
					}
				}
				
				if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
					trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
					trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
					trQuestVarSet("templeChallengeActive", 12);
					trQuestVarSet("templeChallengeNext", 0);
					trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
				}
			}
		} else if (trQuestVarGet("templeChallengeActive") == 12) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posx", trQuestVarGet("posx") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartz", trQuestVarGet("lightningStartz") - 2);
			}
			if (amt > 1600) {
				trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
				trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
				trQuestVarSet("templeChallengeActive", 13);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 13) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posx", trQuestVarGet("posx") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartz", trQuestVarGet("lightningStartz") - 2);
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 14);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 14) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
				trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
				trQuestVarSet("templeChallengeActive", 15);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 15) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posz", trQuestVarGet("posz") - 4);
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartx", trQuestVarGet("lightningStartx") - 2);
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 16);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 2000);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 16) {
			if (trTimeMS() > trQuestVarGet("templeChallengeTimeout")) {
				trQuestVarSet("lightningStartx", trQuestVarGet("templeRoomUpperx") - 3);
				trQuestVarSet("lightningStartz", trQuestVarGet("templeRoomUpperz") - 3);
				trQuestVarSet("templeChallengeActive", 17);
				trQuestVarSet("templeChallengeNext", 0);
				trQuestVarSet("templeChallengeTimeout", trQuestVarGet("templeChallengeTimeout") + 1600);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 17) {
			amt = trTimeMS() - trQuestVarGet("templeChallengeTimeout") + 1600;
			while(trQuestVarGet("templeChallengeNext") < amt) {
				trQuestVarSet("posx", trQuestVarGet("lightningStartX"));
				trQuestVarSet("posz", trQuestVarGet("lightningStartZ"));
				for(x=0; < 8) {
					spawnLightning("pos");
					trQuestVarSet("posz", trQuestVarGet("posz") - 4);
					if (trQuestVarGet("posz") < trQuestVarGet("templeRoomLowerZ")) {
						trQuestVarSet("posZ", trQuestVarGet("posZ") + 32);
					}
				}
				trQuestVarSet("templeChallengeNext", trQuestVarGet("templeChallengeNext") + 100);
				trQuestVarSet("lightningStartx", trQuestVarGet("lightningStartx") - 2);
				trQuestVarSet("lightningStartz", trQuestVarGet("lightningStartz") - 2);
			}
			if (amt > 1600) {
				trQuestVarSet("templeChallengeActive", 18);
			}
		} else if (trQuestVarGet("templeChallengeActive") == 18) {
			if (yGetDatabaseCount("yeebLightningEnd") + yGetDatabaseCount("yeebLightning") == 0) {
				for(x=yGetDatabaseCount("applicants"); >0) {
					yDatabaseNext("applicants", true);
					if (trUnitAlive() && trUnitIsOwnedBy(trCurrentPlayer())) {
						startNPCDialog(NPC_TEMPLE_COMPLETE + 6);
						trQuestVarSet("boonUnlocked"+1*trQuestVarGet("stageTemple"), 1);
					}
				}
			}
		}
	}
}

rule zeno_temple_always
inactive
highFrequency
{
	float amt = 0;
	trUnitSelectClear();
	trUnitSelectByQV("akard", true);
	if (trUnitIsSelected()) {
		reselectMyself();
		startNPCDialog(NPC_ZENOS_PARADOX);
	}
	if (yGetDatabaseCount("zenoRelicsIncoming") >0) {
		yDatabaseNext("zenoRelicsIncoming", true);
		if (trUnitGetIsContained("Unit") == false) {
			yAddToDatabase("freeRelics", "zenoRelicsIncoming");
			yAddUpdateVar("freeRelics", "type", yGetVar("zenoRelicsIncoming", "type"));
			if (yGetVar("zenoRelicsIncoming", "type") - RELIC_MATH_PROBLEM == trQuestVarGet("questAnswer")) {
				trQuestVarSet("correctRelic", trQuestVarGet("zenoRelicsIncoming"));
			} else if (yGetVar("zenoRelicsIncoming", "type") - RELIC_MATH_PROBLEM == trQuestVarGet("wrongAnswer")) {
				trQuestVarSet("wrongRelic", trQuestVarGet("zenoRelicsIncoming"));
			} else {
				yAddToDatabase("zenoRelics", "zenoRelicsIncoming");
				yAddUpdateVar("zenoRelics", "type", yGetVar("zenoRelicsIncoming", "type"));
			}
			yRemoveFromDatabase("zenoRelicsIncoming");
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
		if (yGetDatabaseCount("zenoRelics") > 0) {
			yDatabaseNext("zenoRelics");
			if (zDistanceToVectorSquared("zenoRelics", "templePos") < 400) {
				trChatSendSpoofed(0, "Statue of Zeno: WRONG!");
				trVectorSetUnitPos("pos", "zenoRelics");
				vectorSnapToGrid("pos");
				trUnitSelectClear();
				trUnitSelectByQV("zenoRelics", true);
				if (trUnitVisToPlayer()) {
					trCameraShake(0.5,0.5);
				}
				trUnitChangeProtoUnit("Meteor Impact Ground");
				trVectorQuestVarSet("left", zGetUnitVector("pos", "templePos"));
				trVectorQuestVarSet("left", rotationMatrix("left", 0.0, 1.0));
				trVectorQuestVarSet("forward", zGetUnitVector3d("templePos", "pos"));
				trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0", "Dwarf", 1, trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				trUnitSelectClear();
				trUnitSelectByQV("next", true);
				trUnitSetStance("Passive");
				trMutateSelected(kbGetProtoUnitID("Petosuchus Projectile"));
				trSetUnitOrientation(trVectorQuestVarGet("forward"),trVectorQuestVarGet("left"),true);
				trUnitHighlight(3.0, false);
				yAddToDatabase("mirrorTowerLasers", "next");
				yAddUpdateVar("mirrorTowerLasers", "length", zDistanceBetweenVectors3d("pos", "templePos") * 1.25);
				yAddUpdateVar("mirrorTowerLasers", "timeout", trTimeMS() + 1500);
				
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Meteor Impact Ground");
				trSoundPlayFN("cinematics\35_out\strike.mp3","1",-1,"","");
				trSoundPlayFN("meteordustcloud.wav","1",-1,"","");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 25.0) {
						trUnitDelete(false);
					}
				}
				for(x=yGetDatabaseCount("enemies"); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < 25.0) {
						trUnitDelete(false);
					}
				}
				yRemoveFromDatabase("zenoRelics");
			}
		}
		if (yGetDatabaseCount("mirrorTowerLasers") > 0) {
			yDatabaseNext("mirrorTowerLasers", true);
			amt = yGetVar("mirrorTowerLasers", "timeout") - trTimeMS();
			if (amt < 0) {
				trUnitDestroy();
				yRemoveFromDatabase("mirrorTowerLasers");
			} else {
				amt = amt / 75;
				trSetSelectedScale(amt, amt, yGetVar("mirrorTowerLasers", "length"));
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
