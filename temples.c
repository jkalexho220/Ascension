
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
			trQuestVarSet("boonUnlocked"+BOON_MORE_GOLD, 1);
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
					trShowImageDialog(boonIcon(BOON_MORE_GOLD), boonName(BOON_MORE_GOLD));
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