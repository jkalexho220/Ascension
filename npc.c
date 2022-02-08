const int NPC_RELIC_TRANSPORTER = 0;
const int NPC_RELIC_TRANSPORTER_WTF = 1;

const int NPC_EXPLAIN_SINGLEPLAYER = 2;

const int NPC_ZENO_NEXT_QUESTION = 3;
const int NPC_ZENO_QUIZ_END = 4;

const int NPC_BOSS_ENTRANCE = 5;

const int NPC_MONSTERPEDIA = 6;

const int NPC_NOTTUD = 7;

const int NPC_QUEST = 100;
/*

RESERVED TO 139

*/
const int NPC_QUEST_COMPLETE = 140;
/*

RESERVED TO 169

*/

const int FETCH_NPC = 10;
const int BOUNTY_NPC = 20;
const int SHOP_NPC = 30;
const int FETCH_GUY = 1;
const int BOUNTY_GUY = 2;
const int SHOP_GUY = 3;

void startNPCDialog(int npc = 0) {
	xsEnableRule("npc_talk_01");
	trQuestVarSet("currentNPC", npc);
	trQuestVarSet("currentNPCProgress", 0);
}

int npcDiag(int npc = 0, int dialog = 0) {
	dialog = dialog + 1;
	int gem = 0;
	switch(npc)
	{
		case NPC_RELIC_TRANSPORTER:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Relic Carrying Company, at your service! You can hire me for just 100 gold!");
				}
				case 2:
				{
					uiMessageBox("I can carry up to 3 relics. After you clear this floor, I will send them to your warehouse.");
				}
				case 3:
				{
					uiMessageBox("You can visit your warehouse by playing this map in singleplayer.");
				}
				case 4:
				{
					uiMessageBox("To hire me, simply drop a relic next to me. I will charge you 100 gold. (The relic will be re-equipped)");
					dialog = -1;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_WTF:
		{
			dialog = -1;
			uiMessageBox("You want to hire another? What happened to the last guy?!");
		}
		case NPC_BOSS_ENTRANCE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Find the relic matching my symbol and bring it to me to open the portal to the boss room.");
					dialog = 0;
				}
			}
		}
		case NPC_MONSTERPEDIA:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The Monsterpedia is now open! You can see monster stats and abilities by clicking on them!");
					dialog = 0;
				}
			}
		}
		case NPC_EXPLAIN_SINGLEPLAYER:
		{
			switch(dialog)
			{
				case 1:
				{
					trCameraCut(vector(90,70.710701,90), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
					uiMessageBox("Welcome to the Ascension Guild! There are many things you can do here!");
				}
				case 2:
				{
					trCameraCut(vector(60,70.710701,90), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
					uiMessageBox("This is your relic warehouse. This is where all your spare relics are stored.");
				}
				case 3:
				{
					uiMessageBox("If you drop a relic here, it will automatically be kept in the warehouse.");
				}
				case 4:
				{
					trCameraCut(vector(90,70.710701,60), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
					uiMessageBox("This is your class selection. Click on a class to see its details or switch to it.");
				}
				case 5:
				{
					uiMessageBox("Some classes are locked. Each class has a unique unlock condition.");
				}
				case 6:
				{
					trUnitSelectClear();
					trUnitSelectByQV("levelupObelisk");
					trUnitHighlight(15.0, true);
					uiMessageBox("This is your Ascension Obelisk. Click on this obelisk to level up your current hero.");
				}
				case 7:
				{
					uiMessageBox("Level-ups cost gold and gemstones, which are dropped by bosses.");
				}
				case 8:
				{
					uiMessageBox("Each level-up increases your hero's relic capacity by 1.");
				}
				case 9:
				{
					uiMessageBox("There are more facilities, but they are locked for now. Play the game some more to unlock them!");
				}
				case 10:
				{
					trCameraCut(vector(90,70.710701,90), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
					uiMessageBox("When you are ready to save your configuration, enter the sky passage in the center.");
					trUnitSelectClear();
					trUnitSelectByQV("skypassage");
					trUnitHighlight(5.0, true);
				}
				case 11:
				{
					dialog = 0;
					xsEnableRule("gameplay_start_2");
				}
			}
		}
		case NPC_ZENO_NEXT_QUESTION:
		{
			if (dialog > trQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"explainations")) {
				dialog = 0;
				trShowChoiceDialog(trStringQuestVarGet("question"+1*trQuestVarGet("currentQuestion")),
					trStringQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"first"), 6001,
					trStringQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"second"), 6002);
			} else {
				uiMessageBox(trStringQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"explain"+dialog));
			}
		}

		case NPC_ZENO_QUIZ_END:
		{
			if (dialog > trQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"explainations")) {
				dialog = 0;
				uiMessageBox("That's all for today! I'll be back after you clear another floor!");
				trUnitSelectClear();
				trUnitSelectByQV("zenoUnit");
				trUnitChangeProtoUnit("Arkantos God Out");
				trSoundPlayFN("arrkantosleave.wav","1",-1,"","");
			} else {
				uiMessageBox(trStringQuestVarGet("question"+1*trQuestVarGet("currentQuestion")+"explain"+dialog));
			}
		}

		case NPC_NOTTUD:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Why hello there! It is I, nottud!");
				}
				case 2:
				{
					uiMessageBox("Welcome to my humble shop. Everything here costs 300 gold.");
				}
				case 3:
				{
					uiMessageBox("To purchase an item, simply walk next to it and drop a relic. (The relic will be re-equipped)");
				}
				case 4:
				{
					uiMessageBox("Your purchased item will be delivered to your warehouse.");
					dialog = 0;
				}
			}
		}

		case FETCH_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Good day! Welcome to Foothill Camp!");
				}
				case 2:
				{
					uiMessageBox("They call us Foothill Camp because we're at the first floor of the Tower!");
				}
				case 3:
				{
					uiMessageBox("Oh these pigs? They're not for sale. Don't touch them.");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Welcome, adventurers! Are you here for some training?");
				}
				case 2:
				{
					uiMessageBox("I don't have any bounties for you today though! Check again later!");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("They say there are ten floors in this tower, but no one knows what's at the top.");
				}
				case 2:
				{
					uiMessageBox("No adventurer has made it up there and came back down to tell the tale.");
				}
				case 3:
				{
					uiMessageBox("Maybe they've been abducted by aliens!");
					dialog = 0;
				}
			}
		}

		case FETCH_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Dammit! To think I could only reach the second floor!");
				}
				case 2:
				{
					uiMessageBox("I'm such a useless father!");
				}
				case 3:
				{
					uiMessageBox("Kastor! Please be safe!");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Who am I? Who are you?");
				}
				case 2:
				{
					uiMessageBox("Timmy! Don't run that way! No!");
				}
				case 3:
				{
					uiMessageBox("Wait, who's Timmy?");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("We're lost souls. As long as that Wraithwood stands, we're trapped here.");
				}
				case 2:
				{
					uiMessageBox("But I'm hoping an adventurer will come by and sell me a Resurrection Stone.");
				}
				case 3:
				{
					uiMessageBox("Hey, do you have a Resurrection Stone? I'll buy it for 9000 gold.");
					dialog = 0;
				}
			}
		}

		case FETCH_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I never knew there could be native peoples living in this tower.");
				}
				case 2:
				{
					uiMessageBox("Turns out there are dwarves living on this floor.");
				}
				case 3:
				{
					uiMessageBox("Have you checked out that dwarf's wares yet? He sells some impressive stuff!");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("These damn dwarves charge such high prices!");
				}
				case 2:
				{
					uiMessageBox("Where am I supposed to find a Frosted Bone to repair this axe?");
				}
				case 3:
				{
					uiMessageBox("Do they expect me to take down a Frost Giant with my bare fists?!");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Welcome, travelers! Come and rest a while. The ascent up this tower can be quite arduous.");
				}
				case 2:
				{
					uiMessageBox("I'm forging something at the moment, but if you come back later, there might be something for sale.");
				}
				case 3:
				{
					uiMessageBox("Us dwarves pride ourselves on our craftsmanship. Quality guaranteed!");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + SHOP_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey! It's dangerous to go alone! Buy this!");
				}
				case 2:
				{
					uiMessageBox("If you want this relic, you can buy it for 200 gold. Just drop a relic here to confirm your purchase.");
				}
				case 3:
				{
					uiMessageBox("It will be sent straight to your warehouse.");
				}
				case 4:
				{
					trShowImageDialog(relicIcon(1*trQuestVarGet("shopRelic")),relicName(1*trQuestVarGet("shopRelic")));
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + SHOP_NPC:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The price is 200 gold. This is what's for sale:");
				}
				case 2:
				{
					trShowImageDialog(relicIcon(1*trQuestVarGet("shopRelic")),relicName(1*trQuestVarGet("shopRelic")));
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Adventurers! Help! My pigs have ran off from this impenetrable enclosure!");
				}
				case 2:
				{
					uiMessageBox("Please, help me find my pigs! There are three of them in total!");
				}
				case 3:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Find three pigs and bring them to the enclosure.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + FETCH_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("My pigs! Thank you so much!");
				}
				case 2:
				{
					uiMessageBox("I didn't know what I would be eating tonight.");
				}
				case 3:
				{
					uiMessageBox("Here, take this as your reward!");
				}
				case 4:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\icon resource gold", "300 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Adventurers! I have an urgent request!");
				}
				case 2:
				{
					uiMessageBox("A Mummy has been spotted! Such a high-level enemy should not be on the first floor.");
				}
				case 3:
				{
					uiMessageBox("Eliminate it at once! I will reward you handsomely.");
				}
				case 4:
				{
					trMinimapFlare(trCurrentPlayer(), 10, trVectorQuestVarGet("bountyTargetPos"), true);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Defeat the Mummy.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Good work defeating the Mummy. There could have been many casualties.");
				}
				case 2:
				{
					uiMessageBox("Here is your reward.");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(gemstoneIcon(STARSTONE),gemstoneName(STARSTONE) + " x1");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Adventurers. I feel like my time is coming. I just want one last look at my family.");
				}
				case 2:
				{
					uiMessageBox("I don't remember where I died, but there should be a picture of my son there. Can you retrieve it?");
				}
				case 3:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Bring back the ghost's picture.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + FETCH_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Thank you, adventurers. This is what I was looking for.");
				}
				case 2:
				{
					uiMessageBox("I don't have hands. Can you do one last thing for me? Please write this down on the picture:");
				}
				case 3:
				{
					uiMessageBox("I'm sorry, I was wrong. If we ever meet again, I'll be a proper father this time.");
				}
				case 4:
				{
					uiMessageBox("...");
				}
				case 5:
				{
					uiMessageBox("Thank you.");
				}
				case 6:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(gemstoneIcon(SOULSTONE),gemstoneName(SOULSTONE) + " x1");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Jimmy? Jimmy is that you?");
				}
				case 2:
				{
					uiMessageBox("Jimmy you bastard! It's your fault we all died!");
				}
				case 3:
				{
					uiMessageBox("I'm going to kill you Jimmy!");
				}
				case 4:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Run!", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I'm sorry... my memory isn't what it used to be. I know you're not Jimmy.");
				}
				case 2:
				{
					uiMessageBox("Please take this as compensation... and kill Jimmy if you get the chance.");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\icon resource gold", "200 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Excuse me! You there! I need your help! I'll pay you handsomely!");
				}
				case 2:
				{
					uiMessageBox("I have a shipment of goods but it needs an escort. Can you bring it here?");
				}
				case 3:
				{
					uiMessageBox("Be warned! The goods smell delicious, so wolves may come and attack it!");
				}
				case 4:
				{
					trMinimapFlare(trCurrentPlayer(), 10, trVectorQuestVarGet("fetchTargetPos"), true);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Escort the goods to the axeman.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + FETCH_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Thank you, adventurers. Looks like everything's intact.");
				}
				case 2:
				{
					uiMessageBox("Here is your reward.");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\icon resource gold", "500 Gold");
					dialog = 0;
				}
			}
		}
	}
	return(dialog);
}



rule relic_transporter_guy_found
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("relicTransporterGuyName");
	for(p=1; <ENEMY_PLAYER) {
		if (trUnitHasLOS(p)) {
			xsDisableSelf();
			xsEnableRule("relic_transporter_guy_always");
			trArmyDispatch("1,0","Dwarf",1,
				trQuestVarGet("relicTransporterGuyPosx"),0,trQuestVarGet("relicTransporterGuyPosz"),0,true);
			trArmySelect("1,0");
			trUnitChangeProtoUnit("Revealer");
			trSoundPlayFN("villagercreate.wav","1",-1,"","");
			break;
		}
	}
}

rule boss_entrance_found
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("bossEntranceStatue");
	for(p=1; <ENEMY_PLAYER) {
		if (trUnitHasLOS(p)) {
			xsDisableSelf();
			xsEnableRule("boss_entrance_always");
			trVectorSetUnitPos("pos", "bossEntranceStatue");
			trArmyDispatch("1,0","Dwarf",1,
				trQuestVarGet("Posx"),0,trQuestVarGet("Posz"),0,true);
			trArmySelect("1,0");
			trUnitChangeProtoUnit("Revealer");
			trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
			trMessageSetText("The boss entrance has been found!", -1);
			break;
		}
	}
}


rule relic_transporter_guy_always
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("relicTransporterGuyName");
	if (trUnitIsSelected()) {
		trUnitHighlight(5.0, true);
		int p = trCurrentPlayer();
		if (trQuestVarGet("p"+p+"transporterPurchased") == 0) {
			startNPCDialog(NPC_RELIC_TRANSPORTER);
		} else {
			startNPCDialog(NPC_RELIC_TRANSPORTER_WTF);
		}
		reselectMyself();
	}
}

rule npc_talk_01
inactive
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("currentNPCProgress", 
		npcDiag(1*trQuestVarGet("currentNPC"), 1*trQuestVarGet("currentNPCProgress")));
	if (trQuestVarGet("currentNPCProgress") > 0) {
		trDelayedRuleActivation("npc_talk_02");
	}
}

rule npc_talk_02
inactive
highFrequency
{
	if ((trQuestVarGet("currentNPCProgress") > 0) && (trIsGadgetVisible("ingame-messagedialog") == false)) {
		trDelayedRuleActivation("npc_talk_01");
		xsDisableSelf();
	}
}

void questComplete(int gem = -1, int gold = -1) {
	if (gem >= 0) {
		trQuestVarSet("gemstone"+gem, 1 + trQuestVarGet("gemstone"+gem));
	}
	if (gold > 0) {
		for(p=1; < ENEMY_PLAYER) {
			trQuestVarSet("p"+p+"gold", 300 + trQuestVarGet("p"+p+"gold"));
			trPlayerGrantResources(p, "gold", 300);
		}
	}
	trQuestVarSet("questCount", 1 + trQuestVarGet("questCount"));
	startNPCDialog(NPC_QUEST_COMPLETE + trQuestVarGet("stage") + 10 * trQuestVarGet("localQuest"));
	trQuestVarSet("questActive", 1 + trQuestVarGet("questActive"));
	uiLookAtUnitByName(""+1*trQuestVarGet("questGuy"));
}

rule town_always
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	if (trQuestVarGet("townFound") == 0) {
		trUnitSelectClear();
		trUnitSelectByQV("guy1");
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				trQuestVarSet("townFound", 1);
				break;
			}
		}
		if (trQuestVarGet("townFound") == 1) {
			x = trQuestVarGet("village");
			z = x / 4;
			x = trQuestVarGet("village") - 4 * z;
			trVectorQuestVarSet("townCenter", xsVectorSet(70*x + 40, 0, 70*z + 40));
			trQuestVarSet("townCenter", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Revealer to Player",1,70*x+40,0,70*z+40,225,true);
			trSoundPlayFN("settlement.wav","1",-1,"","");
		}
	} else {
		yDatabaseNext("npcTalk", true);
		if (trUnitIsSelected()) {
			startNPCDialog(1*yGetVar("npcTalk", "dialog"));
			reselectMyself();
		}
		
		if (trQuestVarGet("questActive") == 0) {
			trVectorSetUnitPos("questGuyPos", "questGuy");
			for(p=1; < ENEMY_PLAYER) {
				if (zDistanceToVectorSquared("p"+p+"unit", "questGuyPos") < 16) {
					uiLookAtUnitByName(""+1*trQuestVarGet("questGuy"));
					trQuestVarSet("questActive", 1);
					startNPCDialog(NPC_QUEST + trQuestVarGet("stage") + 10 * trQuestVarGet("localQuest"));
					break;
				}
			}
		} else if (trQuestVarGet("localQuest") == SHOP_GUY) {
			switch(1*trQuestVarGet("questActive"))
			{
				case 1:
				{
					trQuestVarSet("questActive", 2);
					trQuestVarSetFromRand("shopRelic", 5, 10, true);
					trQuestVarSet("shopRelic", trQuestVarGet("shopRelic") + trQuestVarGet("stage"));
					trQuestVarSet("shopGuyActive", 1);
					trVectorSetUnitPos("shopGuyPos", "questGuy");
				}
				case 2:
				{
					trUnitSelectClear();
					trUnitSelectByQV("questGuy");
					if (trUnitIsSelected()) {
						startNPCDialog(NPC_QUEST_COMPLETE + SHOP_NPC);
						reselectMyself();
					}
				}
			}
		} else {
			/* start the quest */
			switch(1*trQuestVarGet("stage") + 10 * trQuestVarGet("localQuest"))
			{
				case FETCH_NPC + 1:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSetFromRand("rand", 1, 8, true);
							trQuestVarSet("pig", trQuestVarGet("rand") + trQuestVarGet("village"));
							if (trQuestVarGet("pig") > 14) {
								trQuestVarSet("pig", trQuestVarGet("pig") - 14);
							}
							trQuestVarSet("pig1", trGetNextUnitScenarioNameNumber());
							z = trQuestVarGet("pig") / 4;
							x = trQuestVarGet("pig") - 4 * z;
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 40,0,70 * z + 40,0,true);
							trUnitSelectClear();
							trUnitSelectByQV("pig1", true);
							trUnitConvert(0);
							trUnitChangeProtoUnit("Pig");
							trQuestVarSetFromRand("rand2", 1, 11 - trQuestVarGet("rand"));
							trQuestVarSet("pig", trQuestVarGet("rand2") + trQuestVarGet("pig"));
							if (trQuestVarGet("pig") > 14) {
								trQuestVarSet("pig", trQuestVarGet("pig") - 14);
							}
							trQuestVarSet("pig2", trGetNextUnitScenarioNameNumber());
							z = trQuestVarGet("pig") / 4;
							x = trQuestVarGet("pig") - 4 * z;
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 40,0,70 * z + 40,0,true);
							trUnitSelectClear();
							trUnitSelectByQV("pig2", true);
							trUnitConvert(0);
							trUnitChangeProtoUnit("Pig");
							trQuestVarSetFromRand("rand3", 1, 15 - trQuestVarGet("rand") - trQuestVarGet("rand2"));
							trQuestVarSet("pig", trQuestVarGet("rand") + trQuestVarGet("pig"));
							if (trQuestVarGet("pig") > 14) {
								trQuestVarSet("pig", trQuestVarGet("pig") - 14);
							}
							trQuestVarSet("pig3", trGetNextUnitScenarioNameNumber());
							z = trQuestVarGet("pig") / 4;
							x = trQuestVarGet("pig") - 4 * z;
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 40,0,70 * z + 40,0,true);
							trUnitSelectClear();
							trUnitSelectByQV("pig3", true);
							trUnitConvert(0);
							trUnitChangeProtoUnit("Pig");
						}
						case 2:
						{
							for(i=1; <4) {
								if (trQuestVarGet("pigReturned"+i) == 0) {
									trVectorSetUnitPos("pos", "pig"+i);
									if (vectorInRectangle("pos", "pigpenLower", "pigpenUpper")) {
										trQuestVarSet("pigReturned"+i, 1);
										trQuestVarSet("pigReturnCount", 1 + trQuestVarGet("pigReturnCount"));
										trChatSend(0, "<color=1,1,1>Pig returned!</color>");
										trSoundPlayFN("pigambient.wav","1",-1,"","");
										if (trQuestVarGet("pigReturnCount") == 3) {
											questComplete(-1, 300);
										}
									}
								}
							}
						}
					}
				}
				case BOUNTY_NPC + 1:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSetFromRand("rand", 6, 9, true);
							trQuestVarSet("rand", trQuestVarGet("village") + trQuestVarGet("rand"));
							if (trQuestVarGet("rand") > 14) {
								trQuestVarSet("rand", trQuestVarGet("rand") - 14);
							}
							z = trQuestVarGet("rand") / 4;
							x = trQuestVarGet("rand") - 4 * z;
							trVectorQuestVarSet("bountyTargetPos", xsVectorSet(70*x+40,0,70*z+40));
							trQuestVarSet("bountyTarget", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 40,0,70 * z + 40,225,true);
							trUnitSelectClear();
							trUnitSelectByQV("bountyTarget", true);
							trUnitConvert(ENEMY_PLAYER);
							trUnitChangeProtoUnit("Mummy");
							activateEnemy("bountyTarget", 100);
							trModifyProtounit("Mummy", ENEMY_PLAYER, 0, 700);
						}
						case 2:
						{
							trUnitSelectClear();
							trUnitSelectByQV("bountyTarget", true);
							if (trUnitAlive() == false) {
								trMessageSetText("Bounty target defeated! Return to the quest giver for your reward!", -1);
								trQuestVarSet("questActive", 3);
							}
						}
						case 3:
						{
							trVectorSetUnitPos("bountyGuyPos", "questguy");
							for(p=1; < ENEMY_PLAYER) {
								if (zDistanceToVectorSquared("p"+p+"unit", "questGuyPos") < 16) {
									questComplete(STARSTONE, 0);
									break;
								}
							}
						}
					}
				}
				case FETCH_NPC + 2:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSetFromRand("rand", 6, 9, true);
							trQuestVarSet("rand", trQuestVarGet("village") + trQuestVarGet("rand"));
							if (trQuestVarGet("rand") > 14) {
								trQuestVarSet("rand", trQuestVarGet("rand") - 14);
							}
							z = trQuestVarGet("rand") / 4;
							x = trQuestVarGet("rand") - 4 * z;
							trVectorQuestVarSet("fetchTargetPos", xsVectorSet(70*x+40,0,70*z+40));
							trQuestVarSet("fetchTarget", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 36,0,70 * z + 36,225,true);
							trUnitSelectClear();
							trUnitSelectByQV("fetchTarget", true);
							trUnitChangeProtoUnit("Relic");
							yAddToDatabase("freeRelics", "fetchTarget");
							yAddUpdateVar("freeRelics", "type", RELIC_GHOST_PICTURE);
							debugLog("position is " + 1*trQuestVarGet("rand"));
						}
						case 2:
						{
							trVectorSetUnitPos("pos", "fetchTarget");
							if (zDistanceBetweenVectorsSquared("pos", "questGuyPos") < 16.0) {
								questComplete(SOULSTONE, 0);
								trUnitSelectClear();
								trUnitSelectByQV("fetchTarget", true);
								trUnitChangeProtoUnit("Osiris Box Glow");
							}
						}
					}
				}
				case BOUNTY_NPC + 2:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("portalsActive", 0);
							trCounterAddTime("countdown", 15, 1, "The shade attacks", -1);
							trQuestVarSet("shadeAttackTime", trTime() + 15);
							trQuestVarSet("questActive", 2);
							trModifyProtounit("Shade", 0, 2, 30);
							trModifyProtounit("Shade", 0, 27, 3000);
						}
						case 2:
						{
							if (trTime() >= trQuestVarGet("shadeAttackTime")) {
								trUnitSelectClear();
								trUnitSelectByQV("questGuy", true);
								trUnitMoveToPoint(0,0,0,-1,true);
								trSoundPlayFN("shadeofhadesacknowledge2.wav","1",-1,"","");
								trSoundPlayFN("attackwarning.wav","1",-1,"","");
								trChatSendSpoofed(0, "Shade: JIMMY!!!!");
								trQuestVarSet("questActive", 3);
								trQuestVarSet("shadeAttackTime", trTime() + 30);
								trCounterAddTime("shadecountdown", 30, 1, "Survive", -1);
							}
						}
						case 3:
						{
							if (trTime() >= trQuestVarGet("shadeAttackTime")) {
								trQuestVarSet("portalsActive", 1);
								trQuestVarSet("questActive", 4);
								trUnitSelectClear();
								trUnitSelectByQV("questGuy", true);
								if (trUnitAlive()) {
									trUnitDoWorkOnUnit(""+1*trQuestVarGet("questGuy"));
									questComplete(-1, 200);
								}
							} else {
								trUnitSelectClear();
								trUnitSelectByQV("questGuy", true);
								if (trUnitAlive() == false) {
									trSoundPlayFN("pestilencebirth.wav","1",-1,"","");
									trQuestVarSet("questActive", 4);
									trCounterAbort("shadecountdown");
									trMessageSetText("Quest failed!", -1);
								}
							}
						}
					}
				}
				case FETCH_NPC + 3:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSetFromRand("rand", 6, 9, true);
							trQuestVarSet("rand", trQuestVarGet("village") + trQuestVarGet("rand"));
							if (trQuestVarGet("rand") > 14) {
								trQuestVarSet("rand", trQuestVarGet("rand") - 14);
							}
							z = trQuestVarGet("rand") / 4;
							x = trQuestVarGet("rand") - 4 * z;
							trVectorQuestVarSet("fetchTargetPos", xsVectorSet(70*x+40,0,70*z+40));
							trQuestVarSet("fetchTarget", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0", "Dwarf",1,70 * x + 36,0,70 * z + 36,225,true);
							trUnitSelectClear();
							trUnitSelectByQV("fetchTarget", true);
							trUnitConvert(0);
							trUnitChangeProtoUnit("Ox Cart");
							trQuestVarSet("fetchWolvesSpawn", zDistanceBetweenVectors("fetchTargetPos", "townCenter") - 20);
						}
						case 2:
						{
							trUnitSelectClear();
							trUnitSelectByQV("fetchTarget", true);
							for(p=1; < ENEMY_PLAYER) {
								if (trUnitHasLOS(p)) {
									trSoundPlayFN("find.wav","1",-1,"","");
									trUnitConvert(p);
									trQuestVarSet("questActive", 3);
									trMessageSetText("Bring the cart back to the town.");
									break;
								}
							}
						}
						case 3:
						{
							trUnitSelectClear();
							trUnitSelectByQV("fetchTarget", true);
							if (trUnitAlive() == false) {
								trSoundPlayFN("pestilencebirth.wav","1",-1,"","");
								trMessageSetText("Quest failed!");
								trQuestVarSet("questActive", 4);
							} else if (zDistanceToVector("fetchTarget", "townCenter") < trQuestVarGet("fetchWolvesSpawn")) {
								trQuestVarSet("fetchWolvesSpawn", trQuestVarGet("fetchWolvesSpawn") - 20);
								trVectorSetUnitPos("center", "fetchTarget");
								trQuestVarSetFromRand("count", 1, ENEMY_PLAYER, true);
								for(x=trQuestVarGet("count"); >0) {
									trQuestVarSetFromRand("angle", 0, 6.283185, false);
									trVectorSetFromAngle("dir", trQuestVarGet("angle"));
									trQuestVarSet("heading", 57.29578 * trQuestVarGet("angle") - 180);
									trQuestVarSet("posx", 20.0 * trQuestVarGet("dirx") + trQuestVarGet("centerx"));
									trQuestVarSet("posz", 20.0 * trQuestVarGet("dirz") + trQuestVarGet("centerz"));
									trQuestVarSet("next" , trGetNextUnitScenarioNameNumber());
									vectorToGrid("pos", "loc");
									if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
										trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),trQuestVarGet("heading"),true);
										trUnitSelectClear();
										trUnitSelectByQV("next", true);
										trUnitConvert(ENEMY_PLAYER);
										trUnitChangeProtoUnit("Fenris Wolf");
										trUnitSelectClear();
										trUnitSelectByQV("next", true);
										trUnitMoveToPoint(trQuestVarGet("centerx"),0,trQuestVarGet("centerz"),-1,true);
										activateEnemy("next");
									}
								}
							} else if (zDistanceToVectorSquared("fetchTarget", "townCenter") < 400) {
								questComplete(-1, 500);
								trUnitSelectClear();
								trUnitSelectByQV("fetchTarget", true);
								trUnitConvert(0);
							}
						}
					}
				}
				case BOUNTY_NPC + 3:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							
						}
					}
				}
			}
		}


		if ((trTime() > trQuestVarGet("townHealNext")) && (trQuestVarGet("boss") == 0)) {
			trQuestVarSet("townHealNext", trTime());
			for(p=1; < ENEMY_PLAYER) {
				if (zDistanceToVectorSquared("p"+p+"unit", "townCenter") < 400) {
					trUnitSelectClear();
					trUnitSelectByQV("p"+p+"unit");
					if (trUnitAlive()) {
						trDamageUnitPercent(-1);
					}
				}
			}
		}
	}
}

rule nottud_always
inactive
highFrequency
{
	int relic = 0;
	trUnitSelectClear();
	trUnitSelectByQV("nottud");
	if (trQuestVarGet("nottudFound") == 0) {
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				trQuestVarSet("nottudFound", 1);
				break;
			}
		}
		if (trQuestVarGet("nottudFound") == 1) {
			trVectorSetUnitPos("pos", "nottud");
			trArmyDispatch("1,0","Revealer to Player",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),225,true);
			trSoundPlayFN("oracledone.wav","1",-1,"","");
		}
	} else {
		if (trUnitIsSelected()) {
			startNPCDialog(NPC_NOTTUD);
			reselectMyself();
		}
		yDatabaseNext("nottudShop", true);
		if (trUnitIsSelected()) {
			relic = yGetVar("nottudShop", "relic");
			trShowImageDialog(relicIcon(relic), relicName(relic));
			reselectMyself();
		}
	}
}