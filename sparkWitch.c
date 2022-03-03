void removeSparkWitch(int p = 0) {
	removePlayerSpecific(p);
	yRemoveUpdateVar("p"+p+"characters", "attacking");
}

void sparkWitchAlways(int eventID = -1) {
	int p = eventID - 12 * SPARKWITCH;
	int id = 0;
	int hit = 0;
	int target = 0;
	int index = yGetPointer("enemies");
	float amt = 0;
	float dist = 0;
	float current = 0;
	int old = xsGetContextPlayer();
	xsSetContextPlayer(p);
	for (y=yGetDatabaseCount("p"+p+"characters"); > 0) {
		id = yDatabaseNext("p"+p+"characters", true);
		if (id == -1 || trUnitAlive() == false) {
			removeSparkWitch(p);
		} else {
			if (kbUnitGetAnimationActionType(id) == 59) {
				ySetVar("p"+p+"characters", "attacking", 1);
				ySetVar("p"+p+"characters", "attackTarget", trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id)));
				ySetVar("p"+p+"characters", "attackNext", trTimeMS() + trQuestVarGet("p"+p+"firstDelay"));
				trUnitOverrideAnimation(19,0,false,false,-1);
			}
			switch(1*yGetVar("p"+p+"characters", "attacking"))
			{
				case 1:
				{
					if (trTimeMS() > yGetVar("p"+p+"characters", "attackNext")) {
						ySetVar("p"+p+"characters", "attacking", 2);
						ySetVar("p"+p+"characters", "attackNext",
							yGetVar("p"+p+"characters", "attackNext") + trQuestVarGet("p"+p+"firstDelay"));
							
							trVectorSetUnitPos("pos", "p"+p+"characters");
							trQuestVarSet("next", yGetVar("p"+p+"characters", "attackTarget"));
							trUnitSelectClear();
							trUnitSelectByQV("next");
							if (trUnitAlive()) {
								trVectorSetUnitPos("end", "next");
								yAddToDatabase("p"+p+"zaps", "next");
								yAddUpdateVar("p"+p+"zaps", "startx", trQuestVarGet("posx"));
								yAddUpdateVar("p"+p+"zaps", "startz", trQuestVarGet("posz"));
								yAddUpdateVar("p"+p+"zaps", "endx", trQuestVarGet("endx"));
								yAddUpdateVar("p"+p+"zaps", "endz", trQuestVarGet("endz"));
								yAddUpdateVar("p"+p+"zaps", "startTime", trTimeMS());
								yAddUpdateVar("p"+p+"zaps", "next", 0);
								yAddUpdateVar("p"+p+"zaps", "hex", yGetVar("p"+p+"characters", "hex"));
								yAddUpdateVar("p"+p+"zaps", "bounces", trQuestVarGet("p"+p+"projectiles") + yGetVar("p"+p+"characters", "bounces"));
								if (yGetVar("p"+p+"characters", "hex") == 2) {
									trSoundPlayFN("pigpower.wav","1",-1,"","");
								} else if (yGetVar("p"+p+"characters","hex") == 1) {
									trSoundPlayFN("lightningbirth.wav","1",-1,"","");
								}
								ySetVar("p"+p+"characters", "bounces", 0);
								ySetVar("p"+p+"characters", "hex", 0);
								for(x=yGetDatabaseCount("enemies"); >0) {
									if (yDatabaseNext("enemies") == trQuestVarGet("next")) {
										yAddUpdateVar("p"+p+"zaps", "index", yGetPointer("enemies"));
										break;
									}
								}
								if (trQuestVarGet("p"+p+"characters") == trQuestVarGet("p"+p+"unit")) {
									if (trCurrentPlayer() == p) {
										trClearCounterDisplay();
									}
								}
							}
						} else if (kbUnitGetAnimationActionType(id) != 16) {
							ySetVar("p"+p+"characters", "attacking", 0);
						}
					}
					case 2:
					{
						if (trTimeMS() > yGetVar("p"+p+"characters", "attackNext")) {
							trUnitOverrideAnimation(-1,0,false,true,-1);
							ySetVar("p"+p+"characters", "attacking", 0);
						}
					}
				}
			}
		}
		
		if (yGetDatabaseCount("p"+p+"witchPigs") > 0) {
			id = yDatabaseNext("p"+p+"witchPigs", true);
			if ((yGetVar("p"+p+"witchPigs", "proto") == kbGetProtoUnitID("Hero Boar")) ||
			(yGetVar("p"+p+"witchPigs", "proto") == kbGetProtoUnitID("Hero Boar 2"))) {
				yRemoveFromDatabase("p"+p+"witchPigs");
			} else if (id == -1 || trUnitAlive() == false) {
				trUnitChangeProtoUnit(kbGetProtoUnitName(1*yGetVar("p"+p+"witchPigs", "proto")));
				yRemoveFromDatabase("p"+p+"witchPigs");
			} else if (trTimeMS() > yGetVar("p"+p+"witchPigs", "timeout")) {
				trMutateSelected(1*yGetVar("p"+p+"witchPigs", "proto"));
				if (ySetPointer("enemies", 1*yGetVar("p"+p+"witchPigs", "index"))) {
					ySetVar("enemies", "physicalResist", yGetVar("p"+p+"witchPigs", "physicalResist"));
					ySetVar("enemies", "magicResist", yGetVar("p"+p+"witchPigs", "magicResist"));
				}
				yRemoveFromDatabase("p"+p+"witchPigs");
			}
		}
		
		if(yGetDatabaseCount("p"+p+"hexOrbs") >0) {
			yDatabaseNext("p"+p+"hexOrbs");
			amt = trTimeMS() - yGetVar("p"+p+"hexOrbs", "next");
			if (amt > 500) {
				amt = 0.001 * amt * trQuestVarGet("hexBoltDamage") * trQuestVarGet("p"+p+"spellDamage");
				yVarToVector("p"+p+"hexOrbs", "pos");
				dist = xsPow(trQuestVarGet("hexboltRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
				for(x=yGetDatabaseCount("enemies"); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < dist) {
						damageEnemy(p, amt);
					}
				}
				
				if (trTimeMS() > yGetVar("p"+p+"hexOrbs", "timeout")) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"hexOrbs");
					trUnitDestroy();
					yRemoveFromDatabase("p"+p+"hexOrbs");
				} else {
					ySetVar("p"+p+"hexOrbs", "next", trTimeMS());
				}
			}
		}
		
		if (yGetDatabaseCount("p"+p+"zaps") > 0) {
			yDatabaseNext("p"+p+"zaps");
			current = trTimeMS() - yGetVar("p"+p+"zaps", "startTime");
			if (current > yGetVar("p"+p+"zaps", "next")) {
				trUnitSelectClear();
				trUnitSelectByQV("p"+p+"zaps");
				if (trUnitAlive()) {
					trVectorSetUnitPos("end", "p"+p+"zaps");
					ySetVarFromVector("p"+p+"zaps", "end", "end");
				} else {
					yVarToVector("p"+p+"zaps", "end");
				}
				yVarToVector("p"+p+"zaps", "start");
				trQuestVarSet("dirx", trQuestVarGet("endx") - trQuestVarGet("startx"));
				trQuestVarSet("dirz", trQuestVarGet("endz") - trQuestVarGet("startz"));
				for(y=5; >0) {
					if (current > yGetVar("p"+p+"zaps", "next")) {
						ySetVar("p"+p+"zaps", "next", yGetVar("p"+p+"zaps", "next") + 100);
						amt = yGetVar("p"+p+"zaps", "next") * 0.002;
						trQuestVarSet("posx", trQuestVarGet("startx") + trQuestVarGet("dirx") * amt);
						trQuestVarSet("posz", trQuestVarGet("startz") + trQuestVarGet("dirz") * amt);
						trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
						trArmySelect("1,0");
						if (yGetVar("p"+p+"zaps", "hex") == 0) {
							trUnitChangeProtoUnit("Lightning Sparks Ground");
						} else {
							trUnitChangeProtoUnit("Lightning Sparks");
						}
					} else {
						break;
					}
				}
				if (yGetVar("p"+p+"zaps", "next") >= 500) {
					gainFavor(p, 1.0);
					trQuestVarSetFromRand("sound", 1, 5, true);
					trSoundPlayFN("ui\lightning"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"zaps");
					if (trUnitAlive()) {
						trUnitHighlight(0.2, false);
						if ((1*yGetVar("p"+p+"zaps", "index") > 0) && ySetPointer("enemies", 1*yGetVar("p"+p+"zaps", "index"))) {
							damageEnemy(p, 100.0 * trQuestVarGet("p"+p+"spellDamage"));
							if (yGetVar("p"+p+"zaps", "hex") > 0) {
								trVectorSetUnitPos("pos", "enemies");
								vectorSnapToGrid("pos");
								trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
								if (yGetVar("p"+p+"zaps", "hex") == 2) {
									yAddToDatabase("p"+p+"witchPigs", "p"+p+"zaps");
									yAddUpdateVar("p"+p+"witchPigs", "proto", kbGetUnitBaseTypeID(kbGetBlockID(""+1*trQuestVarGet("enemies"))));
									yAddUpdateVar("p"+p+"witchPigs", "physicalResist", yGetVar("enemies","physicalResist"));
									yAddUpdateVar("p"+p+"witchPigs", "magicResist", yGetVar("enemies","magicResist"));
									yAddUpdateVar("p"+p+"witchPigs", "index", yGetVar("p"+p+"zaps", "index"));
									yAddUpdateVar("p"+p+"witchPigs", "timeout",
										trTimeMS() + 1000 * trQuestVarGet("hexboltDuration") * trQuestVarGet("p"+p+"spellDuration"));
										ySetVar("enemies", "physicalResist", 0);
										ySetVar("enemies", "magicResist", 0);
										if (trQuestVarGet("p"+p+"zaps") == trQuestVarGet("bossUnit")) {
											trMutateSelected(kbGetProtoUnitID("Hero Boar 2"));
										} else {
											trMutateSelected(kbGetProtoUnitID("Hero Boar"));
										}
										ySetVar("enemies", "silenceStatus", 1); // do this to prevent UI Range Indicator from appearing
										silenceEnemy(p, trQuestVarGet("hexboltDuration"));
										trUnitSelectClear();
										trUnitSelectByQV("next");
										trUnitChangeProtoUnit("Curse SFX");
									} else if (yGetVar("p"+p+"zaps", "hex") == 1) {
										trUnitSelectClear();
										trUnitSelectByQV("next");
										trUnitChangeProtoUnit("Spy Eye");
										trUnitSelectClear();
										trUnitSelectByQV("next");
										trMutateSelected(kbGetProtoUnitID("Arkantos God"));
										trSetSelectedScale(0,0,0);
										trUnitOverrideAnimation(26,0,true,false,-1);
										yAddToDatabase("p"+p+"hexOrbs", "next");
										yAddUpdateVar("p"+p+"hexOrbs", "timeout",
											trTimeMS() + 1000 * trQuestVarGet("hexboltDuration") * trQuestVarGet("p"+p+"spellDuration"));
											yAddUpdateVar("p"+p+"hexOrbs", "next", trTimeMS());
											yAddUpdateVar("p"+p+"hexOrbs", "posx", trQuestVarGet("posx"));
											yAddUpdateVar("p"+p+"hexOrbs", "posz", trQuestVarGet("posz"));
										}
									}
								}
							}
							ySetVar("p"+p+"zaps", "bounces", yGetVar("p"+p+"zaps", "bounces") - 1);
							if (yGetVar("p"+p+"zaps", "bounces") > 0) {
								dist = xsPow(trQuestVarGet("p"+p+"Range") - 5, 2);
								hit = 0;
								for(x=yGetDatabaseCount("enemies"); >0) {
									if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
										removeEnemy();
									} else if (trQuestVarGet("p"+p+"zaps") == trQuestVarGet("enemies")) {
										continue;
									} else if (zDistanceToVectorSquared("enemies", "end") < dist) {
										trVectorSetUnitPos("pos", "enemies");
										hit = 1;
										break;
									}
								}
								if (hit == 1) {
									ySetUnit("p"+p+"zaps", trQuestVarGet("enemies"));
									ySetVar("p"+p+"zaps", "index", yGetPointer("enemies"));
									ySetVar("p"+p+"zaps", "next", 0);
									ySetVar("p"+p+"zaps", "startTime", trTimeMS());
									ySetVarFromVector("p"+p+"zaps", "start", "end");
									ySetVarFromVector("p"+p+"zaps", "end", "pos");
								} else if (zDistanceToVectorSquared("p"+p+"unit", "end") < dist) {
									ySetUnit("p"+p+"zaps", trQuestVarGet("p"+p+"unit"));
									ySetVar("p"+p+"zaps", "index", -1);
									ySetVar("p"+p+"zaps", "next", 0);
									ySetVar("p"+p+"zaps", "startTime", trTimeMS());
									ySetVarFromVector("p"+p+"zaps", "start", "end");
									trVectorSetUnitPos("pos", "p"+p+"unit");
									ySetVarFromVector("p"+p+"zaps", "end", "pos");
								} else {
									yRemoveFromDatabase("p"+p+"zaps");
								}
							} else {
								yRemoveFromDatabase("p"+p+"zaps");
							}
						}
					}
				}
				
				if (yGetDatabaseCount("p"+p+"thunderstrikes") >0) {
					yDatabaseNext("p"+p+"thunderstrikes", true);
					trUnitChangeProtoUnit("Tremor");
					yRemoveFromDatabase("p"+p+"thunderstrikes");
				}
				
				if (trQuestVarGet("p"+p+"wellStatus") == ABILITY_ON) {
					trQuestVarSet("p"+p+"wellStatus", ABILITY_OFF);
					for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
						yDatabaseNext("p"+p+"characters");
						ySetVar("p"+p+"characters", "bounces", 1 + yGetVar("p"+p+"characters", "bounces"));
					}
					trQuestVarSetFromRand("sound", 1, 5, true);
					trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
					trChatSetStatus(false);
					trDelayedRuleActivation("enable_chat");
					trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("p"+p+"wellPosx"),0,trQuestVarGet("p"+p+"wellPosz"),0,true);
					trUnitSelectClear();
					trUnitSelectByQV("next");
					trMutateSelected(kbGetProtoUnitID("Militia"));
					trSetSelectedScale(0,0,0);
					trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
					yAddToDatabase("p"+p+"thunderstrikes", "next");
					dist = xsPow(trQuestVarGet("thunderstrikeRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
					amt = trQuestVarGet("thunderstrikeDamage") * trQuestVarGet("p"+p+"spellDamage");
					hit = 0;
					for(x=yGetDatabaseCount("enemies"); >0) {
						if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
							removeEnemy();
						} else if (zDistanceToVectorSquared("enemies", "p"+p+"wellPos") < dist) {
							hit = hit + 1;
							damageEnemy(p, amt);
						}
					}
					gainFavor(p, hit);
					if (hit > 0) {
						trQuestVarSet("p"+p+"wellReadyTime", trQuestVarGet("p"+p+"wellReadyTime") - 1000 * hit);
						if (trQuestVarGet("p"+p+"wellReadyTime") < trTimeMS() + 500) {
							trQuestVarSet("p"+p+"wellReadyTime", trTimeMS() + 500);
						}
						if (trCurrentPlayer() == p) {
							trCounterAbort("well");
							trCounterAddTime("well",
								xsMax(trQuestVarGet("p"+p+"wellCooldown") * trQuestVarGet("p"+p+"cooldownReduction") - hit, 1), 0, wellName);
							}
						}
					}
					
					if (trQuestVarGet("p"+p+"lureStatus") == ABILITY_ON) {
						trQuestVarSet("p"+p+"lureStatus", ABILITY_OFF);
						for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
							yDatabaseNext("p"+p+"characters");
							ySetVar("p"+p+"characters", "bounces", 1 + yGetVar("p"+p+"characters", "bounces"));
						}
						gainFavor(p, 0.0 - trQuestVarGet("vortexCost") * trQuestVarGet("p"+p+"ultimateCost"));
						trVectorSetUnitPos("center", "p"+p+"lureObject");
						trUnitSelectClear();
						trUnitSelectByQV("p"+p+"lureObject", true);
						trUnitDestroy();
						dist = trQuestVarGet("vortexRange") * trQuestVarGet("p"+p+"spellRange");
						trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
						trArmyDispatch(""+p+",0","Dwarf",8,trQuestVarGet("centerx"),0,trQuestVarGet("centerz"),0,true);
						trArmySelect(""+p+",0");
						trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
						trSetSelectedScale(0,0,0);
						trUnitSetAnimationPath("0,0,1,0,0,0,0");
						yAddToDatabase("p"+p+"vortexObjects", "next");
						yAddUpdateVar("p"+p+"vortexObjects", "last", trTimeMS());
						yAddUpdateVar("p"+p+"vortexObjects", "radius", trQuestVarGet("vortexRadius") * trQuestVarGet("p"+p+"spellRange"));
						yAddUpdateVar("p"+p+"vortexObjects", "centerx", trQuestVarGet("centerx"));
						yAddUpdateVar("p"+p+"vortexObjects", "centerz", trQuestVarGet("centerz"));
						yAddUpdateVar("p"+p+"vortexObjects", "dirx", 1);
						yAddUpdateVar("p"+p+"vortexObjects", "dirz", 0);
						yAddUpdateVar("p"+p+"vortexObjects", "step", 0);
						yAddUpdateVar("p"+p+"vortexObjects", "timeout", trTimeMS() + 1000);
						trSoundPlayFN("cinematics\32_out\doorseal.mp3","1",-1,"","");
					}
					
					if (yGetDatabaseCount("p"+p+"vortexObjects") > 0) {
						yDatabaseNext("p"+p+"vortexObjects");
						if (yGetVar("p"+p+"vortexObjects", "step") == 1) {
							dist = 0.015 * (yGetVar("p"+p+"vortexObjects", "timeout") - trTimeMS());
						} else {
							dist = yGetVar("p"+p+"vortexObjects", "radius");
						}
						amt = trTimeMS() - yGetVar("p"+p+"vortexObjects", "last");
						amt = amt * 0.002;
						ySetVar("p"+p+"vortexObjects", "last", trTimeMS());
						trQuestVarSet("cos", xsCos(amt));
						trQuestVarSet("sin", xsSin(amt));
						yVarToVector("p"+p+"vortexObjects", "dir");
						trVectorQuestVarSet("dir", rotationMatrix("dir", trQuestVarGet("cos"), trQuestVarGet("sin")));
						ySetVarFromVector("p"+p+"vortexObjects", "dir", "dir");
						trVectorScale("dir", dist);
						for(x=0; < 8) {
							trUnitSelectClear();
							trUnitSelect(""+(x + trQuestVarGet("p"+p+"vortexObjects")), true);
							trSetSelectedUpVector(trQuestVarGet("dirX"),0,trQuestVarGet("dirZ"));
							trVectorQuestVarSet("dir", rotationMatrix("dir", 0.707107, 0.707107));
						}
						if (trTimeMS() > yGetVar("p"+p+"vortexObjects", "timeout")) {
							if (yGetVar("p"+p+"vortexObjects", "step") == 0) {
								ySetVar("p"+p+"vortexObjects", "step", 1);
								ySetVar("p"+p+"vortexObjects", "timeout",
									trTimeMS() + trQuestVarGet("vortexMoveTime") * trQuestVarGet("p"+p+"spellRange"));
									trSoundPlayFN("suckup1.wav","1",-1,"","");
									trSoundPlayFN("vortexstart.wav","1",-1,"","");
									dist = xsPow(trQuestVarGet("vortexRadius") * trQuestVarGet("p"+p+"spellRange"), 2);
									yVarToVector("p"+p+"vortexObjects", "center");
									for(x=yGetDatabaseCount("enemies"); >0) {
										if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
											removeEnemy();
										} else if (zDistanceToVectorSquared("enemies", "center") < dist) {
											launchUnit("enemies", "center");
										}
									}
								} else {
									for(x=0; < 8) {
										trUnitSelectClear();
										trUnitSelect(""+(x + trQuestVarGet("p"+p+"vortexObjects")), true);
										trUnitChangeProtoUnit("Arkantos Boost SFX");
									}
									yRemoveFromDatabase("p"+p+"vortexObjects");
								}
							}
						}
						
						
						if (trQuestVarGet("p"+p+"rainStatus") == ABILITY_ON) {
							trQuestVarSet("p"+p+"rainStatus", ABILITY_OFF);
							trQuestVarSet("p"+p+"hex", 1 - trQuestVarGet("p"+p+"hex"));
							if (trCurrentPlayer() == p) {
								trSoundPlayFN("skypassageout.wav","1",-1,"","");
								switch(1*trQuestVarGet("p"+p+"hex"))
								{
									case 0:
									{
										trSetCounterDisplay("Next Attack: ELECTRIFY");
										trSoundPlayFN("lapadesconvert.wav","1",-1,"","");
									}
									case 1:
									{
										trSetCounterDisplay("Next Attack: POLYMORPH");
										trSoundPlayFN("pigambient.wav","1",-1,"","");
									}
								}
							}
							for(x=yGetDatabaseCount("p"+p+"characters"); >0) {
								yDatabaseNext("p"+p+"characters");
								ySetVar("p"+p+"characters", "bounces", 1 + yGetVar("p"+p+"characters", "bounces"));
								ySetVar("p"+p+"characters", "hex", 1 + trQuestVarGet("p"+p+"hex"));
							}
						}
						
						ySetPointer("enemies", index);
						poisonKillerBonus(p);
						xsSetContextPlayer(old);
					}
					
					void chooseSparkWitch(int eventID = -1) {
						int p = eventID - 1000 - 12 * SPARKWITCH;
						if (trCurrentPlayer() == p) {
							map("q", "game", "uiSetSpecialPower(133) uiSpecialPowerAtPointer");
							wellName = "(Q) Thunderstrike";
							wellIsUltimate = false;
							map("w", "game", "uiSetSpecialPower(156) uiSpecialPowerAtPointer");
							rainName = "(W) Hexbolt";
							rainIsUltimate = false;
							map("e", "game", "uiSetSpecialPower(227) uiSpecialPowerAtPointer");
							lureName = "(E) Vortex";
							lureIsUltimate = true;
						}
						trQuestVarSet("p"+p+"wellCooldown", trQuestVarGet("thunderstrikeCooldown"));
						trQuestVarSet("p"+p+"wellCost", 0);
						trQuestVarSet("p"+p+"lureCooldown", 1);
						trQuestVarSet("p"+p+"lureCost", trQuestVarGet("vortexCost"));
						trQuestVarSet("p"+p+"rainCooldown", trQuestVarGet("hexboltCooldown"));
						trQuestVarSet("p"+p+"rainCost", 0);
					}
					
					rule sparkWitch_init
					active
					highFrequency
					{
						xsDisableSelf();
						for(p=1; < ENEMY_PLAYER) {
							trEventSetHandler(12 * SPARKWITCH + p, "sparkWitchAlways");
							trEventSetHandler(1000 + 12 * SPARKWITCH + p, "chooseSparkWitch");
						}
						
						trQuestVarSet("thunderstrikeCooldown", 12);
						trQuestVarSet("thunderstrikeRadius", 6);
						trQuestVarSet("thunderstrikeDamage", 60);
						
						trQuestVarSet("hexboltCooldown", 18);
						trQuestVarSet("hexboltDuration", 4);
						trQuestVarSet("hexboltRadius", 3);
						trQuestVarSet("hexboltDamage", 20);
						
						trQuestVarSet("vortexCost", 80);
						trQuestVarSet("vortexRadius", 12);
						trQuestVarSet("vortexMoveTime", 1000 * trQuestVarGet("vortexRadius") / 15);
					}
