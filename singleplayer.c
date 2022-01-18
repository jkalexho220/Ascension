void spSwitchToClass(int class = -1) {
	chooseClass(1, class - 3000);
}

void spExplainClass(int class = -1) {
	class = class - 4000;
	explainClass(class);
}

void spAscendClass(int class = -1) {
	class = class - 5000;
	int gemstone = trQuestVarGet("class"+class+"gemstone");
	if (trPlayerResourceCount(1, "Gold") < trQuestVarGet("goldCost")) {
		trSoundPlayFN("cantdothat.wav","1",-1,"","");
		uiMessageBox("You don't have enough gold! You need " + 1*trQuestVarGet("goldCost"));
	} else if (trQuestVarGet("gemstone"+gemstone) < trQuestVarGet("gemstoneCost")) {
		trSoundPlayFN("cantdothat.wav","1",-1,"","");
		string noMessage = "You don't have enough " + gemstoneName(gemstone) + "! (";
		noMessage = noMessage + 1*trQuestVarGet("gemstone"+gemstone)+"/"+1*trQuestVarGet("gemstoneCost")+")";
		uiMessageBox(noMessage);
	} else {
		trPlayerGrantResources(1, "Gold", 0-trQuestVarGet("goldCost"));
		trQuestVarSet("gemstone"+gemstone, trQuestVarGet("gemstone"+gemstone) - trQuestVarGet("gemstoneCost"));
		trSoundPlayFN("ageadvance.wav","1",-1,"","");
		trQuestVarSet("class"+class+"level", 1 + trQuestVarGet("class"+class+"level"));
		trQuestVarSet("p1level", 1 + trQuestVarGet("p1level"));
		uiMessageBox(className(class) + " ascended to level " + 1*trQuestVarGet("class"+class+"level") + "! +1 relic slot!");
		trModifyProtounit(kbGetProtoUnitName(1*trQuestVarGet("class"+class+"proto")),1,5,1);
		trSetCivilizationNameOverride(1, "Level " + (1+trQuestVarGet("p1level")));
		if (trQuestVarGet("class"+class+"level") == 5) {
			if (trQuestVarGet("class"+ALCHEMIST+"level") == 0) {
				trQuestVarSet("class"+ALCHEMIST+"level", 1);
				trModifyProtounit(kbGetProtoUnitName(1*trQuestVarGet("class"+ALCHEMIST+"proto")),1,5,1);
				trQuestVarSet("newClasses", trQuestVarGet("newClasses") + 1);
				trQuestVarSet("newClass"+1*trQuestVarGet("newClasses"), ALCHEMIST);
				xsEnableRule("singleplayer_unlocks");
				trVectorSetUnitPos("pos", "class"+ALCHEMIST+"unit");
				vectorToGrid("pos", "loc");
				trPaintTerrain(trQuestVarGet("locx"),trQuestVarGet("locz"),trQuestVarGet("locx"),trQuestVarGet("locz"),4,15,false);
			}
		}
		trChatHistoryClear();
		trChatSend(0, "<u><color=1,1,1>Gemstones</color></u>");
		for(x=0; <3) {
			trChatSend(0, gemstoneName(x) + " x" + 1*trQuestVarGet("gemstone"+x));
		}
	}
}


void answerQuestion(int eventID = -1) {
	int answer = eventID - 6000;
	int question = trQuestVarGet("currentQuestion");
	string result = "Incorrect! ";
	
	if (answer == 1*trQuestVarGet("question"+question+"answer")) {
		result = "Correct! ";
		trQuestVarSet("gemstone"+1*trQuestVarGet("zenoReward"), 1 + trQuestVarGet("gemstone"+1*trQuestVarGet("zenoReward")));
		trSoundPlayFN("favordump.wav","1",-1,"","");
	} else {
		trSoundPlayFN("cantdothat.wav","1",-1,"","");
	}
	question = question + 1;
	trQuestVarSet("currentQuestion", question);
	trStringQuestVarSet("question"+question+"explain1", result + trStringQuestVarGet("question"+question+"explain1"));
	if (trQuestVarGet("currentQuestion") <= trQuestVarGet("zenoQuestions")) {
		startNPCDialog(NPC_ZENO_NEXT_QUESTION);
	} else {
		startNPCDialog(NPC_ZENO_QUIZ_END);
		trQuestVarSet("zenoQuiz", 1 + trQuestVarGet("zenoQuiz"));
	}
}

void classNewUnlock(int class = 0) {
	bool unlocked = false;
	if (trQuestVarGet("class"+class+"level") == 0) {
		switch(class)
		{
			case FIREKNIGHT:
			{
				if (trQuestVarGet("playerHasHosted") == 1) {
					unlocked = true;
				}
			}
			case FROSTKNIGHT:
			{
				if (trQuestVarGet("bossKills") >= 5) {
					unlocked = true;
				}
			}
			case STORMCUTTER:
			{
				unlocked = true;
			}
			case STARSEER:
			{
				if (trQuestVarGet("questCount") >= 3) {
					unlocked = true;
				}
			}
			case COMMANDO:
			{
				trQuestVarSet("relicCount", yGetDatabaseCount("p1relics") + yGetDatabaseCount("freeRelics"));
				if (trQuestVarGet("relicCount") >= 100) {
					unlocked = true;
				}
			}
			case THUNDERRIDER:
			{
				if (trQuestVarGet("giantKills") >= 100) {
					unlocked = true;
				}
			}
		}
	}
	if (unlocked) {
		trQuestVarSet("class"+class+"level", 1);
		trQuestVarSet("newClasses", 1 + trQuestVarGet("newClasses"));
		trQuestVarSet("newClass"+1*trQuestVarGet("newClasses"), class);
	}
}

rule singleplayer_unlocks
inactive
highFrequency
{
	if ((trIsGadgetVisible("ShowImageBox") == false) &&
		(trIsGadgetVisible("ingame-messagedialog") == false)) {
		xsDisableSelf();
		if (trQuestVarGet("newClasses") > 0) {
			trQuestVarSetFromRand("sound", 1, 5, true);
			int class = trQuestVarGet("newClass"+1*trQuestVarGet("newClasses"));
			trShowImageDialog(classIcon(class), "New class unlocked! " + className(class));
			trSoundPlayFN("ui\thunder"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
			trDelayedRuleActivation("singleplayer_unlocks");
			trQuestVarSet("newClasses", trQuestVarGet("newClasses") - 1);
		}
	}
}

rule singleplayer_init
inactive
highFrequency
{
	if (trTime() > cActivationTime + 2) {
		int proto = 0;
		xsDisableSelf();
		trLetterBox(false);
	    trUIFadeToColor(0,0,0,1000,0,false);
		trMusicPlayCurrent();
	    trPlayNextMusicTrack();

	    trVectorQuestVarSet("startPosition", vector(135,0,135));

	    trPaintTerrain(0,0,145,145,2,13,false);
	    
	    TERRAIN_WALL = 2;
	    TERRAIN_SUB_WALL = 13;

	    trSetUnitIdleProcessing(false);
	    trPlayerSetDiplomacy(1, 2, "Enemy");
	    trPlayerSetDiplomacy(2, 1, "Enemy");
	    trArmyDispatch("1,0","Victory Marker",1,1,0,1,0,true);
	    trArmyDispatch("2,0","Victory Marker",1,1,0,1,0,true);

	    int x = 0;
	    int z = 15;
	    /* center island */
	    for(x=0; <15) {
	    	for(b=0; < 15) {
	    		if (x*x + z*z <= 225) {
	    			trPaintTerrain(72-x,72-z,72+x,72+z,0,50,false);
	    			break;
	    		} else {
	    			z = z - 1;
	    		}
	    	}
	    }

	    trPaintTerrain(57,71,73,73,0,53,false);
	    trPaintTerrain(71,57,73,73,0,53,false);

	    trQuestVarSet("levelupObelisk", trGetNextUnitScenarioNameNumber());
	    trArmyDispatch("1,0","Dwarf",1,145,0,118,0,true);
	    trArmySelect("1,0");
	    trUnitConvert(0);
	    trMutateSelected(kbGetProtoUnitID("Outpost"));

	    /* relic warehouse */
	    trPaintTerrain(45,66, 57,78, 0,70, false);
	    x = 114;
	    for(a=1; <= 10) {
	    	z = 154;
	    	if (trQuestVarGet("ownedRelics"+a) > 0) {
	    		trArmyDispatch("1,0","Dwarf",1,x,0,z+2,180,true);
		    	trArmySelect("1,0");
		    	trUnitConvert(0);
		    	trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(a)));
		    	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		    	trArmyDispatch("1,0", "Dwarf",trQuestVarGet("ownedRelics"+a),x,0,z,0,true);
		    	trArmySelect("1,0");
		    	trUnitChangeProtoUnit("Relic");
		    	for(b=0; <trQuestVarGet("ownedRelics"+a)) {
		    		yAddToDatabase("freeRelics", "next");
		    		yAddUpdateVar("freeRelics", "type", a);
		    		trQuestVarSet("next", 1 + trQuestVarGet("next"));
		    	}
	    	}
	    	
	    	z = 134;
	    	if (trQuestVarGet("ownedRelics"+(a+10)) > 0) {
	    		trArmyDispatch("1,0","Dwarf",1,x,0,z-2,0,true);
		    	trArmySelect("1,0");
		    	trUnitConvert(0);
		    	trUnitChangeProtoUnit(kbGetProtoUnitName(relicProto(a+10)));
		    	trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
		    	trArmyDispatch("1,0", "Dwarf",trQuestVarGet("ownedRelics"+(a+10)),x,0,z,0,true);
		    	trArmySelect("1,0");
		    	trUnitChangeProtoUnit("Relic");
		    	for(b=0; <trQuestVarGet("ownedRelics"+(a+10))) {
		    		yAddToDatabase("freeRelics", "next");
		    		yAddUpdateVar("freeRelics", "type", a+10);
		    		trQuestVarSet("next", 1 + trQuestVarGet("next"));
		    	}
	    	}
	    	
	    	x = x - 2;
	    }

	    /* if player is new */
	    if (trQuestVarGet("class1level") == 0) {
	    	xsEnableRule("singleplayer_cin");
	    	trQuestVarSet("class1level", 1);
	    	trQuestVarSet("class2level", 1);
	    	trQuestVarSet("gemstone"+STARSTONE, 1 + trQuestVarGet("gemstone"+STARSTONE));
	    	startNPCDialog(NPC_EXPLAIN_SINGLEPLAYER);
	    } else {
	    	xsEnableRule("gameplay_start_2");
	    	xsEnableRule("singleplayer_unlocks");
	    	trEventSetHandler(6001, "answerQuestion");
	    	trEventSetHandler(6002, "answerQuestion");
	    	for(a=4 * (1 + xsFloor(trQuestVarGet("p1progress") / 2)); >2) {
	    		classNewUnlock(a);
	    	}
	    	if (trQuestVarGet("p1progress") > trQuestVarGet("zenoQuiz")) {
	    		trQuestVarSet("zenoUnit", trGetNextUnitScenarioNameNumber());
	    		trArmyDispatch("1,0", "Hoplite", 1, 130, 0, 160, 225, false);
	    		trUnitSelectClear();
	    		trUnitSelectByQV("zenoUnit", true);
	    		trUnitConvert(0);
	    		xsEnableRule("zeno_quiz_start");
	    	}
	    }

	    if (trQuestVarGet("p1class") == 0) {
	    	trQuestVarSet("p1class", MOONBLADE);
	    }
	    chooseClass(1, 1*trQuestVarGet("p1class"));

	    /* class selection */
	    trPaintTerrain(69,49, 75,56, 4,15, false);
	    x = 138;
	    z = 98;
	    for(a=1; <= CLASS_COUNT) {
			proto = trQuestVarGet("class"+a+"proto");
			trModifyProtounit(kbGetProtoUnitName(proto),1,5,trQuestVarGet("class"+a+"level")-1);
	    	trQuestVarSet("class"+a+"unit", trGetNextUnitScenarioNameNumber());
	    	trArmyDispatch("1,0","Dwarf",1,x,0,z,180,true);
	    	trArmySelect("1,0");
	    	trUnitConvert(0);
	    	if (a <= 4 * (1 + xsFloor(trQuestVarGet("p1progress") / 2))) {
	    		trMutateSelected(1*trQuestVarGet("class"+a+"proto"));
	    	} else {
	    		trUnitChangeProtoUnit("Atlantis Wall Connector");
	    	}
	    	if (trQuestVarGet("class"+a+"level") == 0) {
	    		trPaintTerrain(x/2,z/2,x/2,z/2,2,12,false);
	    	}
	    	x = x + 4;
	    	if (x > 150) {
	    		x = 138;
	    		z = z + 4;
	    	}
	    	trEventSetHandler(3000+a, "spSwitchToClass");
	    	trEventSetHandler(4000+a, "spExplainClass");
	    	trEventSetHandler(5000+a, "spAscendClass");
	    }

	    /* monsterpedia */
	    if (trQuestVarGet("p1progress") >= 3) {
	    	/*
	    	MONSERPEDIA
	    	*/
	    }

	    trSetCounterDisplay("To save and exit, enter the Sky Passage.");
	    
	    trSetFogAndBlackmap(false, false);
	    trSetLighting("default", 0);

	    trQuestVarSet("skyPassage", trGetNextUnitScenarioNameNumber());
	    trArmyDispatch("1,0","Dwarf",1,145,0,145,0,true);
	    trArmySelect("1,0");
	    trMutateSelected(kbGetProtoUnitID("Sky Passage"));

	    /* paint deployment square at the bottom of the map */
		trPaintTerrain(0,0,5,5,0,70,true);
        trPaintTerrain(0,0,5,5,2,13,false);

        xsEnableRule("singleplayer_always");
	}
}

rule singleplayer_always
inactive
highFrequency
{
	int class = 0;
	trUnitSelectClear();
	trUnitSelectByQV("p1unit");
	if (trUnitGetIsContained("Sky Passage")) {
		xsDisableSelf();
		xsDisableRule("gameplay_always");

		/* free relics go into ownedRelics */
		for(x=31; >0) {
			trQuestVarSet("ownedRelics"+x, 0);
		}
		for(x=yGetDatabaseCount("freeRelics"); >0) {
			yDatabaseNext("freeRelics");
			class = yGetVar("freeRelics", "type");
			trQuestVarSet("ownedRelics"+class, xsMin(10, 1 + trQuestVarGet("ownedRelics"+class)));
		}
		saveAllData();
		
		trUIFadeToColor(255,255,255,1000,0,true);
		xsEnableRule("singleplayer_end");
		
	}

	for(x=CLASS_COUNT; >0) {
		trUnitSelectClear();
		trUnitSelectByQV("class"+x+"unit");
		if (trUnitIsSelected()) {
			reselectMyself();
			if (x > 4 * (1 + xsFloor(trQuestVarGet("p1progress") / 2))) {
				explainClass(x + 16);
			} else if (trQuestVarGet("p1class") == x || trQuestVarGet("class"+x+"level") == 0) {
				explainClass(x);
				if (trQuestVarGet("class"+x+"level") == 0) {
					trDelayedRuleActivation("singleplayer_explain_class");
				}
			} else if (trQuestVarGet("class"+x+"level") > 0) {
				trShowChoiceDialog(className(x) + " (Level " + 1*trQuestVarGet("class"+x+"level")+")",
				"Switch to this class", 3000 + x, "View class details", 4000 + x);
			}
		}
	}

	trUnitSelectClear();
	trUnitSelectByQV("levelupObelisk");
	if (trUnitIsSelected()) {
		reselectMyself();
		class = trQuestVarGet("p1class");
		trQuestVarSet("goldCost", 100 * trQuestVarGet("class"+class+"level"));
		trQuestVarSet("gemstoneCost", (1 + trQuestVarGet("class"+class+"level")) / 2);
		string yesPrompt = "Yes (" + 1*trQuestVarGet("goldCost") + " gold + " + 1*trQuestVarGet("gemstoneCost") + " ";
		yesPrompt = yesPrompt + gemstoneName(1*trQuestVarGet("class"+class+"gemstone")) + ")";
		if (trQuestVarGet("p1level") < 9) {
			trShowChoiceDialog("Ascend " + className(class) + "? (Increases relic capacity by 1)",
				yesPrompt, 5000+class, "No", -1);
			trChatHistoryClear();
			trChatSend(0, "<u><color=1,1,1>Gemstones</color></u>");
			for(x=0; <3) {
				trChatSend(0, gemstoneName(x) + " x" + 1*trQuestVarGet("gemstone"+x));
			}
		} else {
			uiMessageBox("You have reached the max level for " + className(class) + "!");
		}
	}
}

rule singleplayer_end
inactive
highFrequency
{
	if (trTime() > 1 + cActivationTime) {
		trModeEnter("Pregame");
	}
}


rule singleplayer_explain_class
inactive
highFrequency
{
	if (trQuestVarGet("explain") == 0 && trIsGadgetVisible("ShowImageBox") == false) {
		xsDisableSelf();
		int class = trQuestVarGet("pleaseExplain");
		switch(class)
		{
			case THUNDERRIDER:
			{
				uiMessageBox("To unlock this class, kill 100 Giants. Current: " + 1*trQuestVarGet("giantKills"));
			}
			case FIREKNIGHT:
			{
				uiMessageBox("To unlock this class, host this map in multiplayer once.");
			}
			case FROSTKNIGHT:
			{
				uiMessageBox("To unlock this class, defeat 5 bosses. Current: " + 1*trQuestVarGet("bossKills"));
			}
			case INVENTOR:
			{
				uiMessageBox("To unlock this class, IDK");
			}
			case ALCHEMIST:
			{
				uiMessageBox("To unlock this class, ascend a character to level 5.");
			}
			case STARSEER:
			{
				uiMessageBox("To unlock this class, complete three quests. Current: " + 1*trQuestVarGet("questCount"));
			}
			case STORMCUTTER:
			{
				uiMessageBox("To unlock this class, clear stage 2.");
			}
			case SPELLSTEALER:
			{
				uiMessageBox("To unlock this class, open ten chests. Current: " + 1*trQuestVarGet("chestCount"));
			}
			case COMMANDO:
			{
				uiMessageBox("To unlock this class, collect 50 relics. Current: " + 1*trQuestVarGet("relicCount"));
			}
		}
	}
}

void setupQuestion(string question = "", string first = "", string second = "", int answer = 1) {
	int count = 1 + trQuestVarGet("zenoQuestions");
	trQuestVarSet("zenoQuestions", count);
	trStringQuestVarSet("question"+count, question);
	trStringQuestVarSet("question"+count+"first", first);
	trStringQuestVarSet("question"+count+"second", second);
	trQuestVarSet("question"+count+"answer", answer);
}

void setupExplain(string explain = "", int question = -1) {
	if (question < 0) {
		question = 1 + trQuestVarGet("zenoQuestions");
	}
	int count = 1 + trQuestVarGet("question"+question+"explainations");
	trQuestVarSet("question"+question+"explainations", count);
	trStringQuestVarSet("question"+question+"explain"+count, explain);
}


rule zeno_quiz_start
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("zenoUnit", true);
	if (trUnitIsSelected()) {
		xsDisableSelf();
		reselectMyself();
		trQuestVarSet("currentQuestion", 1);
		for(x=trQuestVarGet("zenoQuestions"); >0) {
			trQuestVarSet("question"+x+"explainations", 0);
		}
		trQuestVarSet("zenoQuestions", 0);

		if (trQuestVarGet("zenoQuiz") == 0) {
			trQuestVarSet("zenoReward", STARSTONE);

			setupExplain("Hey it's me, Zenophobia! I've got a fun little quiz for you!");
			setupExplain("For each question you answer correctly, I'll give you a Starstone! Ready?");
			setupQuestion("What buttons do you press to cast a spell?", "Q, W, and E", "Click on the god power", 1);
			setupExplain("In this game, you can cast your spells by casting Q, W, and E on your keyboard.");

			setupQuestion("Which spells cost favor to cast?", "All spells cost favor.", "Only my ultimate spell costs favor.",2);
			setupExplain("Only your ultimate costs favor! The other spells are free!");
			setupExplain("In fact, your basic spells generate favor when they hit enemies.");

			setupQuestion("The Spell Power stat affects both spell damage and healing.", "True", "False", 1);
			setupExplain("Spell Power affects the healing and damage of your spells.");
		} else {
			int gem = 0;
			switch(1*trQuestVarGet("zenoQuiz"))
			{
				case 1:
				{
					gem = MANASTONE;
					
					setupQuestion("You cannot cast spells when stunned.", "True", "False", 2);
					setupExplain("You can still cast spells when stunned.");

					setupQuestion("Enemies cannot cast spells when stunned.", "True", "False", 1);
					setupExplain("Enemies cannot cast spells when stunned. In fact, you can interrupt an enemy's spell with a stun!");
						
					setupQuestion("What does the Stun Resistance stat give you?", "Grants a percentage chance to ignore a stun.",
						"Reduces the duration of stuns on you by a percentage", 2);
					setupExplain("Stun Resistance will reduce the duration of stuns on you. It stacks multiplicatively.");
				}
				case 2:
				{
					gem = SOULSTONE;
					setupQuestion("Being poisoned prevents you from healing.", "True", "False", 1);
					setupExplain("In addition to dealing damage over time, the poison status effect prevents you from healing.");

					setupQuestion("Spell Duration affects the duration of Stuns and Poisons that you inflict on enemies.",
						"True", "False", 1);
					setupExplain("Spell Duration does increase the duration of your Stuns and Poisons.");

					setupQuestion("Spell Power affects the damage of Poisons that you inflict on enemies.",
						"True", "False", 1);
					setupExplain("Your poison damage is increased by Spell Power.");
				}
				case 3:
				{
					gem = MANASTONE;
					setupExplain("First Question: Each cooldown reduction relic grants 0.1x cooldown reduction.");
					setupQuestion("If I have 10 cooldown reduction relics, how long are my cooldowns?",
						"Cooldowns are 0", "Cooldowns are 0.35x as long", 2);
					setupExplain("Cooldown reduction stacks multiplicatively, not additively. You cannot reach 0 cooldowns.");

					setupQuestion("Inflicting Silence on an enemy will prevent them from using their special attack.",
						"True", "False", 1);
					setupExplain("Not only will Silence prevent an enemy's special attacks, their passive abilities are disabled!");
					setupExplain("For example, silencing a Dryad will prevent them from bleeding poison on death!");
					setupExplain("However, Silences do not interrupt a special attack that is already being cast.");

					setupQuestion("Inflicting Silence on a boss will prevent it from casting spells.", "True", "False", 2);
					setupExplain("Silences will only extend the duration of a boss's cooldowns.");
				}
				case 4:
				{
					gem = SOULSTONE;
					setupQuestion("All players must be present to start the boss battle.","True","False",2);
					setupExplain("Only the living players need to be present. Dead players will be automatically revived.");

					setupQuestion("If a unit is afflicted with two different poisons, how is the damage calculated?",
						"The poison damage adds up.","Only the strongest poison deals damage.",2);
					setupQuestion("Only the strongest poison deals damage. Poison damage does not stack.");

					setupQuestion("Launching an enemy into a wall will stun them.", "True", "False",1);
					setupExplain("Some abilities will launch enemies. Launching an enemy into a wall will stun them!");
				}
				case 5:
				{
					gem = STARSTONE;
					setupQuestion("Ultimate Cost Reduction does not work on toggled Ultimates","True","False",2);
					setupExplain("Ultimate Cost Reduction will decrease the favor drain of toggled Ultimates.");

					setupQuestion("Only melee characters have Special Attacks.","True","False",1);
					setupExplain("Melee characters have Special Attacks, while ranged characters have Projectiles.");
				}
			}
			trQuestVarSet("zenoReward", gem);
			setupExplain("Hey it's me again! Here comes another quiz!", 1);
			setupExplain("This time, I'll give you a "+gemstoneName(gem)+" for each correct answer!", 1);
		}
		startNPCDialog(NPC_ZENO_NEXT_QUESTION);
	}
}
