const int NPC_RELIC_TRANSPORTER = 0;
const int NPC_RELIC_TRANSPORTER_WTF = 1;

const int NPC_EXPLAIN_SINGLEPLAYER = 2;

const int NPC_ZENO_NEXT_QUESTION = 3;
const int NPC_ZENO_QUIZ_END = 4;

const int NPC_BOSS_ENTRANCE = 5;

const int NPC_MONSTERPEDIA = 6;

const int NPC_NOTTUD = 7;

const int NPC_ZENOS_PARADOX = 8;

const int NPC_EXPLAIN_BOONS = 9;

const int NPC_KASTOR = 10;
/*

RESERVED TO 40

*/

const int NPC_QUEST = 100;
/*

RESERVED TO 139

*/
const int NPC_QUEST_COMPLETE = 140;
/*

RESERVED TO 169

*/

const int NPC_TEMPLE = 200;
/*
RESERVED TO 300
*/

const int NPC_TEMPLE_COMPLETE = 300;
/* 
RESERVED TO 400
*/

const int NPC_ATE_CORPSE = 401;
const int NPC_ATE_BERRIES = 402;

const int NPC_NICK_NO = 403;
const int NPC_NICK_START = 404;
/* reserved to 307 */
const int NPC_NICK_DROP = 408;
const int NPC_NICK_NEXT = 409;
/* reserved to 312 */
const int NPC_NICK_QUEST_COMPLETE = 413;

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
	string extra = "";
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
		case NPC_EXPLAIN_BOONS:
		{
			switch(dialog)
			{
				case 1:
				{
					uiLookAtUnitByName(""+1*yDatabaseNext("boons"));
					uiMessageBox("You have acquired a Blessing! A Blessing is a powerful passive effect.");
				}
				case 2:
				{
					uiMessageBox("However, you can only have one Blessing at a time. Choose your Blessing by selecting it.");
				}
				case 3:
				{
					uiMessageBox("There are more Blessings to be found in the Tower, and each one requires a unique unlock condition.");
					trQuestVarSet("boonUnlocked0", 1);
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

		case NPC_NICK_NO:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Ahhh!! I hate donkeys!");
				}
				case 2:
				{
					uiMessageBox("Get away from me you filthy animal!");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_START:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Oh thank goodness! Finally a human being!");
				}
				case 2:
				{
					uiMessageBox("Please help me! I accidentally spilled a quantum potion on myself!");
				}
				case 3:
				{
					uiMessageBox("I need to absorb some magical energies in order to return to my original form!");
				}
				case 4:
				{
					uiMessageBox("You're my only hope! First, I need to absorb a Sphinx's sandstorm!");
				}
				case 5:
				{
					trMessageSetText("Touch a Sphinx's sandstorm while holding the talking relic.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_START + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Remember: I need a Sphinx's sandstorm!");
				}
				case 2:
				{
					uiMessageBox("There should be some Sphinxes on the first floor of the tower.");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_START + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I need to absorb a Dryad's Blood.");
				}
				case 2:
				{
					uiMessageBox("We can find Dryads on the second floor of the tower.");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_START + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I need to absorb a Frost Giant's Breath.");
				}
				case 2:
				{
					uiMessageBox("There are Frost Giants on the third floor.");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_NEXT:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("*cough* *cough* Oh dear, some of that got in my nose.");
				}
				case 2:
				{
					uiMessageBox("Wait, I don't have a nose! Oh please, I just want my body back.");
				}
				case 3:
				{
					uiMessageBox("Let's hurry up and get the next ingredient! I need a Dryad's Blood!");
				}
				case 4:
				{
					trMessageSetText("Absorb Dryad's Blood while holding the talking relic.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_NEXT + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Gross! Tastes awful every time.");
				}
				case 2:
				{
					uiMessageBox("Anyway, there's one more ingredient that I need to return to my original body.");
				}
				case 3:
				{
					uiMessageBox("I need the Breath of a Frost Giant. Please help me out one more time!");
				}
				case 4:
				{
					uiMessageBox("I promise I'll reward you with something good!");
				}
				case 5:
				{
					trMessageSetText("Absorb a Frost Giant's breath attack while holding the talking relic.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_NEXT + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Alright! Feeling better already!");
				}
				case 2:
				{
					uiMessageBox("Let's return to the Guild to complete my transformation!");
					dialog = 0;
				}
			}
		}

		case NPC_NICK_DROP:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey! Don't leave me!");
				}
				case 2:
				{
					uiMessageBox("Where are you going?!");
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
					uiMessageBox("Where am I supposed to find five Frosted Bones to repair this axe?");
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

		case FETCH_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The creatures on this floor are my creations. They escaped from their facility.");
				}
				case 2:
				{
					uiMessageBox("Try not to aggravate them. They are quite ferocious.");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Greetings! I am the great wizard Akard!");
				}
				case 2:
				{
					uiMessageBox("Would you like to be my test subjects?");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Very interesting. What drives people to climb this tower?");
				}
				case 2:
				{
					uiMessageBox("They don't even know what's at the top and yet they enter without hesitation.");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Eek! Put on your masks! There's a deadly plague on this floor!");
				}
				case 2:
				{
					uiMessageBox("It deals 25 damage per second!");
					dialog = 0;
				}
			}
		}

		case FETCH_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I'm running low on rations...");
				}
				case 2:
				{
					uiMessageBox("This is my last piece of fresh pork. The rest of it rotted away.");
				}
				case 3:
				{
					uiMessageBox("Oh, I hope the next floor has something edible.");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Mmm, the meat on this floor is absolutely delicious!");
				}
				case 2:
				{
					uiMessageBox("Much better than the junk I had back at home.");
				}
				case 3:
				{
					uiMessageBox("Y'all got any more of that decaying meat?");
					dialog = 0;
				}
			}
		}

		case FETCH_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Alright, when I laugh maniacally, you quiver in fear, got it?");
				}
				case 2:
				{
					uiMessageBox("3...");
				}
				case 3:
				{
					uiMessageBox("2...");
				}
				case 4:
				{
					uiMessageBox("1...");
				}
				case 5:
				{
					uiMessageBox("MWUAHAHAHA!!!");
					trSoundPlayFN("xpack\xdialog\xkri075.mp3","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case BOUNTY_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("What do you want?");
				}
				case 2:
				{
					uiMessageBox("Stop bothering me. I have important business to tend to.");
					dialog = 0;
				}
			}
		}

		case SHOP_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Once I attain the power of RMS, I shall be unstoppable!");
				}
				case 2:
				{
					uiMessageBox("Ahem...");
				}
				case 3:
				{
					uiMessageBox("What? Stop staring or I'll vaporize you.");
					dialog = 0;
				}
			}
		}

		case NPC_KASTOR:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Damn, to think I could only make it up to the fifth floor...");
				}
				case 2:
				{
					uiMessageBox("Now how am I going to prove my old man wrong?!");
				}
				case 3:
				{
					uiMessageBox("...Huh?");
				}
				case 4:
				{
					uiMessageBox("What the hell are you looking at?");
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

		case NPC_QUEST + SHOP_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey you! Wanna buy this relic?");
				}
				case 2:
				{
					trShowImageDialog(relicIcon(1*trQuestVarGet("shopRelic")),relicName(1*trQuestVarGet("shopRelic")));
				}
				case 3:
				{
					uiMessageBox("It's nice and shiny! Only 200 gold!");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + SHOP_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Behold! My latest product! I worked for weeks on this!");
				}
				case 2:
				{
					trShowImageDialog(relicIcon(1*trQuestVarGet("shopRelic")),relicName(1*trQuestVarGet("shopRelic")));
				}
				case 3:
				{
					uiMessageBox("Dwarven quality goods! Expect the best! For a small sum of 200 gold, of course.");
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
					uiMessageBox("Please, help me find my precious pigs! There are three of them in total!");
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
					trShowImageDialog("icons\improvement tax collectors icon 64", "300 Gold");
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
					uiMessageBox("I'm sorry. I was wrong. If we ever meet again, I'll be a proper father this time.");
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
					trShowImageDialog("icons\improvement tax collectors icon 64", "200 Gold");
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
					trShowImageDialog("icons\improvement tax collectors icon 64", "300 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Oh, adventurers! I'm at my wit's end! I need that dwarf to repair my axe!");
				}
				case 2:
				{
					uiMessageBox("But in order to repair it, he needs five Frosted Bones! They can be collected from Frost Giants.");
				}
				case 3:
				{
					uiMessageBox("But I can't kill Frost Giants without this axe! You're my only hope!");
				}
				case 4:
				{
					uiMessageBox("There are Frost Giants in this canyon. Please kill them and bring their bones to me!");
				}
				case 5:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Kill five Frost Giants.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Frosted Bones! Thank you so much!");
				}
				case 2:
				{
					uiMessageBox("Now I'm going to go on a massacre! Mwuahaha!");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(gemstoneIcon(MANASTONE),gemstoneName(MANASTONE) + " x1");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("This ancient tree will occasionally shed fruits of knowledge.");
				}
				case 2:
				{
					uiMessageBox("Bring me the fruit that solves this math problem:");
				}
				case 3:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText(""+1*trQuestVarGet("operand1") + " + " + 1*trQuestVarGet("operand2") + " = ?", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + FETCH_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Impressive. If you had brought the wrong fruit...");
				}
				case 2:
				{
					uiMessageBox("Nevermind. Here's your reward.");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(gemstoneIcon(STARSTONE),gemstoneName(STARSTONE) + " x1");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Congratulations! You have been randomly selected to be my test subjects!");
				}
				case 2:
				{
					uiMessageBox("Please do not resist.");
				}
				case 3:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Defeat the monsters.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("This has been some interesting data.");
				}
				case 2:
				{
					uiMessageBox("Thank you for volunteering. Here is your reward.");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\improvement tax collectors icon 64", "300 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_ZENOS_PARADOX:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Oh no, not like this! We've entered Zeno's Paradox!");
				}
				case 2:
				{
					extra = "" + 1*trQuestVarGet("operand1") + " + " + 1*trQuestVarGet("operand2") + " = " + 1*trQuestVarGet("wrongAnswer");
					uiMessageBox("You fools! We're now trapped in a world where " + extra + "!");
				}
				case 3:
				{
					uiMessageBox("We're doomed! We're doomed!");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey... please help me... I'm starving...");
				}
				case 2:
				{
					uiMessageBox("There must be... something edible... on this floor...");
				}
				case 3:
				{
					uiMessageBox("I'll pay you anything... I just need some food...");
				}
				case 4:
				{
					trMessageSetText("Find something edible for the adventurer.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_ATE_CORPSE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Is this all there is? Well, here goes nothing.");
				}
				case 2:
				{
					uiMessageBox("Hrrrk! Oh god, what is this?!");
				}
				case 3:
				{
					uiMessageBox("I'm dying!!!!!");
					dialog = 0;
				}
			}
		}

		case NPC_ATE_BERRIES:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Berries? Thank you so much! I'll never forget this!");
				}
				case 2:
				{
					uiMessageBox("Wait, what's going on? What's happening to my body?");
				}
				case 3:
				{
					uiMessageBox("AaaAaAhHhhHH!!1!");
					dialog = 0;
				}
			}
		}

		/*
		literal feces
		mysterious berry
		a fucking corpse
		*/
		case NPC_QUEST_COMPLETE + FETCH_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Oh thank goodness! I thought I was going to starve!");
				}
				case 2:
				{
					uiMessageBox("This is just what I needed! Thank you so much! You're a lifesaver!");
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\improvement tax collectors icon 64", "300 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey you! I can't get enough of this marvelous meat!");
				}
				case 2:
				{
					uiMessageBox("But it needs a bit more seasoning.");
				}
				case 3:
				{
					uiMessageBox("Please collect some poison using this bucket and bring it back to me.");
				}
				case 4:
				{
					trMessageSetText("Equip the Bucket and collect some Poison.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Simply exquisite! This is luxury! This is true cuisine!");
				}
				case 2:
				{
					uiMessageBox("Would you like to have a bite?");
				}
				case 3:
				{
					uiMessageBox("No? Okay fine. Have this instead.");
				}
				case 4:
				{
					trShowImageDialog(gemstoneIcon(SOULSTONE),gemstoneName(SOULSTONE) + " x1");
					trSoundPlayFN("favordump.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + FETCH_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Adventurers. I have an important task for you.");
				}
				case 2:
				{
					uiMessageBox("One of my special relics has been stolen.");
				}
				case 3:
				{
					uiMessageBox("Fortunately, this magic detector can tell you how far the relic is from you.");
				}
				case 4:
				{
					uiMessageBox("However, this magic detector drains your favor in order to function.");
				}
				case 5:
				{
					uiMessageBox("If your favor reaches zero while you hold it, it will explode!");
				}
				case 6:
				{
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					trMessageSetText("Use the Magic Detector to find Yeebaagooon's missing relic and return it to him.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + FETCH_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Excellent work. You are far more useful than those dwarves I enslaved.");
				}
				case 2:
				{
					uiMessageBox("One of them betrayed me. That's his smoldering corpse over there.");
				}
				case 3:
				{
					uiMessageBox("Anyway, here is your reward.");
				}
				case 4:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog("icons\improvement tax collectors icon 64", "300 Gold");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST + BOUNTY_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Ah, right on time, adventurers.");
				}
				case 2:
				{
					uiMessageBox("There is a lot of useless junk in my storage. Please get rid of it.");
				}
				case 3:
				{
					uiMessageBox("Don't touch anything else, however.");
				}
				case 4:
				{
					trMessageSetText("Move the Worthless Junk out of Yeebaagooon's storage room.", -1);
					trSoundPlayFN("new_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_QUEST_COMPLETE + BOUNTY_NPC + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Excellent work. Now I can fill this place with other worthless junk.");
				}
				case 2:
				{
					uiMessageBox("Here is your reward.");
				}
				case 3:
				{
					trShowImageDialog(gemstoneIcon(MANASTONE), gemstoneName(MANASTONE));
					trSoundPlayFN("favordump.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}

		case NPC_TEMPLE + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("A trial of thunder and lightning. Only the worthy may succeed.");
				}
				case 2:
				{
					uiMessageBox("If you can survive 47 seconds, my power is yours.");
				}
				case 3:
				{
					uiMessageBox("To begin the trial, drop a relic at my feet.");
					dialog = 0;
				}
			}
		}

		case NPC_TEMPLE_COMPLETE + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You have proven yourself worthy, and as such, I grant you this power.");
				}
				case 2:
				{
					trShowImageDialog(boonIcon(BOON_SPELL_ATTACK), boonName(BOON_SPELL_ATTACK));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}

		case NPC_TEMPLE + 11:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("A single question, yet infinite possibilities.");
				}
				case 2:
				{
					uiMessageBox("Show thy mettle, and be rewarded with glorious treasure.");
				}
				case 3:
				{
					uiMessageBox("What is " + 1*trQuestVarGet("operand1") + " + " + 1*trQuestVarGet("operand2") + "?");
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
			trUnitConvert(0);
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
			trUnitConvert(0);
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
	if ((trQuestVarGet("currentNPCProgress") > 0) && 
		(trIsGadgetVisible("ingame-messagedialog") == false) &&
		(trIsGadgetVisible("ShowImageBox") == false)) {
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
			trQuestVarSet("p"+p+"gold", gold + trQuestVarGet("p"+p+"gold"));
			trPlayerGrantResources(p, "gold", gold);
		}
	}
	trQuestVarSet("questCount", 1 + trQuestVarGet("questCount"));
	startNPCDialog(NPC_QUEST_COMPLETE + trQuestVarGet("stage") + 10 * trQuestVarGet("localQuest"));
	trQuestVarSet("questActive", -1);
	uiLookAtUnitByName(""+1*trQuestVarGet("questGuy"));
}

rule town_always
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	int p = 0;
	int room = 0;
	int action = 0;
	float amt = 0;
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
		} else if (trQuestVarGet("questActive") > 0) {
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
									trQuestVarSet("questActive", -1);
									trCounterAbort("shadecountdown");
									trMessageSetText("Quest failed!", -1);
									trQuestVarSet("portalsActive", 1);
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
								trQuestVarSet("questActive", -1);
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
								questComplete(-1, 300);
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
							trQuestVarSet("questActive", 2);
							trQuestVarSet("frostGiantBones", 0);
							trQuestVarSet("frostGiantBonesLast", 0);
							trQuestVarSet("frostGiantHunt", 1);
							for(i=1; < 14) {
								if (i == trQuestVarGet("village")) {
									continue;
								} else {
									z = i / 4;
									x = i - z * 4;
									trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
									trQuestVarSetFromRand("heading", 0, 360, true);
									trArmyDispatch("1,0", "Dwarf", 1, 70*x+40,0,70*z+40,trQuestVarGet("heading"),true);
									trArmySelect("1,0");
									trUnitConvert(ENEMY_PLAYER);
									trUnitChangeProtoUnit("Frost Giant");
									yAddToDatabase("enemiesIncoming", "next");
								}
							}
						}
						case 2:
						{
							if (trQuestVarGet("frostGiantBones") > trQuestVarGet("frostGiantBonesLast")) {
								if (trQuestVarGet("frostGiantBones") >= 5) {
									trQuestVarSet("frostGiantHunt", 0);
									trMessageSetText("Return to the axeman for your reward.", -1);
									trQuestVarSet("questActive", 3);
								} else {
									trQuestVarSet("frostGiantBonesLast", trQuestVarGet("frostGiantBones"));
									trChatSend(0, "<color=1,1,1>Frosted Bone collected! ("+1*trQuestVarGet("frostGiantBones")+"/5)");
								}
							}
						}
						case 3:
						{
							trVectorSetUnitPos("bountyGuyPos", "questguy");
							for(p=1; < ENEMY_PLAYER) {
								if (zDistanceToVectorSquared("p"+p+"unit", "questGuyPos") < 16) {
									questComplete(MANASTONE, 0);
									break;
								}
							}
						}
					}
				}
				case BOUNTY_NPC + 4:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSet("questSpawns", 12);
							trQuestVarSetFromRand("questAngle", 0, 3.14, false);
							trVectorSetFromAngle("questDir", trQuestVarGet("questAngle"));
							trQuestVarSet("questNext", trTimeMS());
							trSoundPlayFN("changeunit.wav","1",-1,"","");
							trBlockAllSounds(true);
						}
						case 2:
						{
							if (yGetDatabaseCount("questLeaves") > 0) {
								action = processGenericProj("questLeaves");
								if (action == PROJ_GROUND) {
									trUnitSelectClear();
									trUnitSelectByQV("questLeaves");
									trUnitChangeProtoUnit("Dryad");
									trUnitSelectClear();
									trUnitSelectByQV("questLeaves");
									trDamageUnitPercent(-100);
									activateEnemy("questLeaves",-1,0);
									yAddToDatabase("questTargets", "questLeaves");
									yRemoveFromDatabase("questLeaves");
									if (trQuestVarGet("questSpawns") == 0) {
										trQuestVarSet("questActive", 3);
										trUnblockAllSounds();
									}
								}
							}
							if (trQuestVarGet("questSpawns") > 0) {
								if (trTimeMS() > trQuestVarGet("questNext")) {
									for(i=trQuestVarGet("akardTower"); < trQuestVarGet("akardTowerEnd")) {
										trUnitSelectClear();
										trUnitSelect(""+i, true);
										trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
									}
									trQuestVarSet("questSpawns", trQuestVarGet("questSpawns") - 1);
									trQuestVarSet("questNext", trQuestVarGet("questNext") + 500);
									z = trQuestVarGet("village") / 4;
									x = trQuestVarGet("village") - 4 * z;
									trVectorQuestVarSet("start", xsVectorSet(70*x+40,0,70*z+40));
									trQuestVarSetFromRand("rand", 8, 20, false);
									addGenericProj("questLeaves","start","questDir",kbGetProtoUnitID("Kronny Birth SFX"),2,trQuestVarGet("rand"),1);
									trVectorQuestVarSet("questDir", rotationMatrix("questDir", -0.757322, 0.653041));
									trUnitSelectClear();
									trUnitSelectByQV("akardTower", true);
									trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
									trUnitSelectClear();
									trUnitSelectByQV("akardGuild1", true);
									trMutateSelected(kbGetProtoUnitID("Guild"));
									trUnitSelectClear();
									trUnitSelectByQV("akardGuild2", true);
									trMutateSelected(kbGetProtoUnitID("Guild"));
									trUnitSelectClear();
									trUnitSelectByQV("akardTree", true);
									trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
								}
							}
						}
						case 3:
						{
							if (yGetDatabaseCount("questTargets") > 0) {
								if (yDatabaseNext("questTargets", true) == -1 || trUnitAlive() == false) {
									yRemoveFromDatabase("questTargets");
								}
							} else {
								questComplete(-1, 300);
							}
						}
					}
				}
				case FETCH_NPC + 4:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSetFromRand("operand1", 1, 10, true);
							trQuestVarSetFromRand("operand2", 1, 10, true);
							trQuestVarSet("questAnswer", trQuestVarGet("operand1") + trQuestVarGet("operand2"));
							trQuestVarSet("questActive", 2);
							trQuestVarSet("questSpawns", 20);
							trQuestVarSetFromRand("questAngle", 0, 3.14, false);
							trVectorSetFromAngle("questDir", trQuestVarGet("questAngle"));
							trQuestVarSet("questNext", trTimeMS());
							trSoundPlayFN("changeunit.wav","1",-1,"","");
							trBlockAllSounds(true);
						}
						case 2:
						{
							if (yGetDatabaseCount("questLeaves") > 0) {
								action = processGenericProj("questLeaves");
								if (action == PROJ_GROUND) {
									trUnitSelectClear();
									trUnitSelectByQV("questLeaves");
									trUnitChangeProtoUnit("Relic");
									trUnitSelectClear();
									trUnitSelectByQV("questLeaves");
									trDamageUnitPercent(-100);
									yAddToDatabase("freeRelics", "questLeaves");
									yAddUpdateVar("freeRelics", "type", yGetVar("questLeaves", "type"));
									yAddToDatabase("questApples", "questLeaves");
									yAddUpdateVar("questApples", "type", yGetVar("questLeaves", "type"));
									yRemoveFromDatabase("questLeaves");
									if (yGetDatabaseCount("questLeaves") + trQuestVarGet("questSpawns") == 0) {
										trQuestVarSet("questActive", 3);
										trUnblockAllSounds();
									}
								}
							}
							if (trQuestVarGet("questSpawns") > 0) {
								if (trTimeMS() > trQuestVarGet("questNext")) {
									
									for(i=trQuestVarGet("akardTower"); < trQuestVarGet("akardTowerEnd")) {
										trUnitSelectClear();
										trUnitSelect(""+i, true);
										trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
									}
									
									trQuestVarSet("questNext", trQuestVarGet("questNext") + 200);
									z = trQuestVarGet("village") / 4;
									x = trQuestVarGet("village") - 4 * z;
									trVectorQuestVarSet("start", xsVectorSet(70*x+36,0,70*z+36));
									trQuestVarSetFromRand("rand", 12, 16, false);
									addGenericProj("questLeaves","start","questDir",kbGetProtoUnitID("Einheriar"),18,trQuestVarGet("rand"),1);
									yAddUpdateVar("questLeaves", "type", RELIC_MATH_PROBLEM + trQuestVarGet("questSpawns"));
									trQuestVarSet("questSpawns", trQuestVarGet("questSpawns") - 1);
									trVectorQuestVarSet("questDir", rotationMatrix("questDir", -0.757322, 0.653041));
									trUnitSelectClear();
									trUnitSelectByQV("akardTower", true);
									trMutateSelected(kbGetProtoUnitID("Tower Mirror"));
									trUnitSelectClear();
									trUnitSelectByQV("akardGuild1", true);
									trMutateSelected(kbGetProtoUnitID("Guild"));
									trUnitSelectClear();
									trUnitSelectByQV("akardGuild2", true);
									trMutateSelected(kbGetProtoUnitID("Guild"));
									trUnitSelectClear();
									trUnitSelectByQV("akardTree", true);
									trMutateSelected(kbGetProtoUnitID("Tamarisk Tree"));
									
								}
							}
						}
						case 3:
						{
							yDatabaseNext("questApples");
							if (zDistanceToVectorSquared("questApples", "questGuyPos") < 9) {
								trUnitSelectClear();
								trUnitSelectByQV("questApples", true);
								if (yGetVar("questApples", "type") - RELIC_MATH_PROBLEM == trQuestVarGet("questAnswer")) {
									trUnitChangeProtoUnit("Osiris Box Glow");
									questComplete(STARSTONE, -1);
								} else {
									trQuestVarSet("wrongAnswer", yGetVar("questApples", "type") - RELIC_MATH_PROBLEM);
									yClearDatabase("questApples");
									xsDisableRule("gameplay_always");
									xsDisableRule("town_always");
									xsDisableRule("boss_entrance_always");
									xsDisableRule("enter_boss_room");
									xsDisableRule("nottud_always");
									xsDisableRule("class_shop_always");
									xsDisableRule("relic_transporter_guy_always");
									xsDisableRule("relic_transporter_guy_found");
									trUnitChangeProtoUnit("Dwarf");
									trUnitSelectClear();
									trUnitSelectByQV("questApples", true);
									trUnitConvert(0);
									trUnitChangeProtoUnit("Implode Sphere Effect");
									trSoundPlayFN("wonderdeath.wav","1",-1,"","");
									trSoundPlayFN("changeunit.wav","1",-1,"","");
									trChatSendSpoofed(0, "Akard: OH GOD! WHAT HAVE YOU DONE?! THAT'S THE WRONG-");
									xsEnableRule("zenos_paradox_00");
									trQuestVarSet("cinNext", 0);
									trQuestVarSet("cinStep", 0);
								}
							}
						}
					}
				}
				case FETCH_NPC + 5:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("questActive", 2);
							trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("basicRooms") - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								yDatabaseNext("basicRooms");
							}
							room = trQuestVarGet("basicRooms");
							trVectorQuestVarSet("pos", randomNearEdgeOfRoom(room));
							trQuestVarSet("fecesRelic", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
							trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("basicRooms") - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								yDatabaseNext("basicRooms");
							}
							room = trQuestVarGet("basicRooms");
							trVectorQuestVarSet("pos", randomNearEdgeOfRoom(room));
							trQuestVarSet("berryRelic", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,false);
							trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("basicRooms") - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								yDatabaseNext("basicRooms");
							}
							room = trQuestVarGet("basicRooms");
							trVectorQuestVarSet("pos", randomNearEdgeOfRoom(room));
							trQuestVarSet("corpseRelic", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,false);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Relic");
							yAddToDatabase("freeRelics", "fecesRelic");
							yAddUpdateVar("freeRelics", "type", RELIC_LITERAL_FECES);
							yAddToDatabase("freeRelics", "berryRelic");
							yAddUpdateVar("freeRelics", "type", RELIC_BERRY_BUSH);
							yAddToDatabase("freeRelics", "corpseRelic");
							yAddUpdateVar("freeRelics", "type", RELIC_A_FUCKING_CORPSE);
						}
						case 2:
						{
							if (zDistanceToVectorSquared("berryRelic", "questGuyPos") < 16) {
								trUnitSelectClear();
								trUnitSelectByQV("berryRelic", true);
								trUnitChangeProtoUnit("Regeneration SFX");
								trQuestVarSet("questActive", 4);
								trQuestVarSet("questNext", trTime() + 5);
								trSoundPlayFN("colossuseat.wav","1",-1,"","");
								startNPCDialog(NPC_ATE_BERRIES);
							} else if (zDistanceToVectorSquared("corpseRelic", "questGuyPos") < 16) {
								trUnitSelectClear();
								trUnitSelectByQV("corpseRelic", true);
								trUnitChangeProtoUnit("Scarab Blood");
								trQuestVarSet("questActive", 5);
								trQuestVarSet("questNext", trTime() + 5);
								trSoundPlayFN("colossuseat.wav","1",-1,"","");
								startNPCDialog(NPC_ATE_CORPSE);
							} else if (zDistanceToVectorSquared("fecesRelic", "questGuyPos") < 16) {
								trUnitSelectClear();
								trUnitSelectByQV("fecesRelic", true);
								trUnitChangeProtoUnit("Osiris Box Glow");
								trSoundPlayFN("colossuseat.wav","1",-1,"","");
								trSoundPlayFN("heal.wav","1",-1,"","");
								questComplete(-1, 300);
							}
						}
						case 4:
						{
							if (trTime() > trQuestVarGet("questNext")) {
								trQuestVarSet("questActive", -1);
								trMessageSetText("Quest failed!", -1);
								trSoundPlayFN("changeunit.wav","1",-1,"","");
								trUnitSelectClear();
								trUnitSelectByQV("questGuy", true);
								trUnitDestroy();
								trQuestVarSet("next", trGetNextUnitScenarioNameNumber());
								trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("questGuyPosX"),0,trQuestVarGet("questGuyPosZ"),180,true);
								trUnitSelectClear();
								trUnitSelectByQV("next");
								trUnitConvert(ENEMY_PLAYER);
								trUnitChangeProtoUnit("Walking Berry Bush");
								activateEnemy("next", 10);
							}
						}
						case 5:
						{
							if (trTime() > trQuestVarGet("questNext")) {
								trQuestVarSet("questActive", -1);
								trMessageSetText("Quest failed!", -1);
								trSoundPlayFN("argusfreezeattack.wav","1",-1,"","");
								trUnitSelectClear();
								trUnitSelectByQV("questGuy", true);
								trUnitDelete(false);
							}
						}
					}
				}
				case BOUNTY_NPC + 5:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSet("poisonBucket", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("questGuyPosX") - 2,0,trQuestVarGet("questGuyPosZ"),0,true);
							trUnitSelectClear();
							trUnitSelectByQV("poisonBucket", true);
							trUnitChangeProtoUnit("Relic");
							yAddToDatabase("freeRelics", "poisonBucket");
							yAddUpdateVar("freeRelics", "type", RELIC_POISON_BUCKET);
							trQuestVarSet("questActive", 2);
						}
						case 2:
						{
							p = trQuestVarGet("poisonBucketHolder");
							if (p > 0) {
								if (yGetVarAtIndex("playerUnits", "poisonStatus", 1*trQuestVarGet("p"+p+"index")) > 0) {
									if (trTime() > trQuestVarGet("poisonBucketNext")) {
										trQuestVarSet("poisonBucketNext", trTime());
										trQuestVarSet("poisonBucketFill", trQuestVarGet("poisonBucketFill") + 3);
										if (trQuestVarGet("poisonBucketFill") >= 100) {
											trQuestVarSet("poisonBucketFill", 100);
											trMessageSetText("Poison bucket has been filled. Return to the quest giver.", -1);
											trQuestVarSet("questActive", 3);
										}
										trChatSend(0, "<color=0.3,1,0.5>Poison bucket: " + 1*trQuestVarGet("poisonBucketFill") + "/100");
									}
								}
							}
						}
						case 3:
						{
							if (zDistanceToVectorSquared("poisonBucket", "questGuyPos") < 16) {
								trUnitSelectClear();
								trUnitSelectByQV("poisonBucket", true);
								trUnitChangeProtoUnit("Lampades Blood");
								trSoundPlayFN("lampadesblood.wav","1",-1,"","");
								questComplete(SOULSTONE, -1);
							}
						}
					}
				}
				case FETCH_NPC + 6:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							trQuestVarSetFromRand("rand", 1, yGetDatabaseCount("basicRooms"), true);
							for(i=0; < trQuestVarGet("rand")) {
								yDatabaseNext("basicRooms");
							}
							room = trQuestVarGet("basicRooms");
							trVectorQuestVarSet("pos", randomNearEdgeOfRoom(room));
							trQuestVarSet("yeebRelic", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Relic");
							trQuestVarSet("questActive", 2);
							trQuestVarSet("magicDetectorRelic", trGetNextUnitScenarioNameNumber());
							trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("questGuyPosX") - 2,0,trQuestVarGet("questguyposz")-2,0,true);
							trArmySelect("1,0");
							trUnitChangeProtoUnit("Relic");
							yAddToDatabase("freeRelics", "yeebRelic");
							yAddUpdateVar("freeRelics", "type", RELIC_YEEBAAGOOON);
							yAddToDatabase("freeRelics", "magicDetectorRelic");
							yAddUpdateVar("freeRelics", "type", RELIC_MAGIC_DETECTOR);
							xsEnableRule("yeeb_hit_list");
						}
						case 2:
						{
							p = trQuestVarGet("magicDetectorHolder");
							trVectorSetUnitPos("pos", "yeebRelic");
							if (zDistanceBetweenVectorsSquared("pos", "questGuyPos") < 16) {
								trUnitSelectClear();
								trUnitSelectByQV("yeebRelic", true);
								trUnitChangeProtoUnit("Osiris Box Glow");
								questComplete(-1, 300);
								xsDisableRule("yeeb_hit_list");
								trQuestVarSet("yeebHit", 0);
							} else if (p > 0) {
								if (trTime() > trQuestVarGet("magicDetectorNext")) {
									trQuestVarSet("magicDetectorNext", trTime());
									amt = zDistanceToVector("magicDetectorRelic", "pos");
									if (trCurrentPlayer() == p) {
										if (trPlayerResourceCount(p, "favor") < 10) {
											trChatSend(0, "<color=1,0,0>Distance: " + 1*amt);
											trSoundPlayFN("attackwarning.wav","1",-1,"","");
										} else {
											trChatSend(0, "<color=1,1,1>Distance: " + 1*amt);
										}
									}
								}
								if (trPlayerResourceCount(p, "favor") == 0) {
									trMessageSetText("Quest failed!");
									trQuestVarSet("questActive", -1);
									trUnitSelectClear();
									trUnitSelectByQV("yeebRelic", true);
									trUnitDestroy();
									trUnitSelectClear();
									trUnitSelectByQV("magicDetectorRelic", true);
									trUnitDestroy();
									trVectorSetUnitPos("pos", "p"+p+"unit");
									trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
									trArmySelect("1,0");
									trDamageUnitPercent(100);
									trUnitChangeProtoUnit("Implode Sphere Effect");
									trUnitSelectClear();
									trUnitSelectByQV("p"+p+"unit");
									trUnitDelete(false);
									trSoundPlayFN("cinematics\32_out\explosion.mp3","1",-1,"","");
									trCameraShake(1.0, 0.5);
								}
							}							
						}
					}
				}
				case BOUNTY_NPC + 6:
				{
					switch(1*trQuestVarGet("questActive"))
					{
						case 1:
						{
							xsEnableRule("yeeb_hit_list");
							trQuestVarSet("questActive", 2);
						}
						case 2:
						{
							if (yGetDatabaseCount("worthlessJunk") > 0) {
								yDatabaseNext("worthlessJunk");
								trVectorSetUnitPos("pos", "worthlessJunk");
								if (vectorInRectangle("pos", "yeebRoomBottom", "yeebRoomTop") == false) {
									trUnitSelectClear();
									trUnitSelectByQV("worthlessJunk", true);
									trUnitChangeProtoUnit("Dust Large");
									trSoundPlayFN("swing2.wav","1",-1,"","");
									trChatSend(0, "<color=1,1,1>Worthless junk removed.");
									yRemoveFromDatabase("worthlessJunk");
								}
							} else {
								questComplete(MANASTONE, -1);
							}
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

rule zenos_paradox_00
inactive
highFrequency
{
	if (trTime() > trQuestVarGet("cinNext")) {
		trQuestVarSet("cinStep", 1 + trQuestVarGet("cinStep"));
		switch(1*trQuestVarGet("cinStep"))
		{
			case 1:
			{
				trUIFadeToColor(255,255,255,5000,0,true);
				trCameraShake(5.0, 0.3);
				trQuestVarSet("cinNext", trTime() + 6);
			}
			case 2:
			{
				trChatHistoryClear();
				trQuestVarSet("cinNext", trTime() + 3);
				trQuestVarSet("stage", 11);
				xsEnableRule("rebuild_map");
				trOverlayText("Zeno's Paradox",3,-1,-1,-1);
			}
			case 3:
			{
				xsDisableRule("enter_boss_room");
				xsDisableSelf();
			}
		}
	}
}

rule yeeb_hit_list
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("yeebRelic", true);
	if (trUnitIsOwnedBy(0)) {
		trQuestVarSet("yeebHit", 0);
		trQuestVarSet("yeebBossFight", 0);
	} else {
		trQuestVarSet("yeebBossFight", 1);
		if (trUnitIsOwnedBy(trCurrentPlayer())) {
			trQuestVarSet("yeebHit", 1);
		}
	}
}

rule nick_dialog
inactive
highFrequency
{
	int p = trCurrentPlayer();
	if (trQuestVarGet("nickEquippedLocal") != trQuestVarGet("p"+p+"nickEquipped")) {
		trQuestVarSet("nickEquippedLocal", trQuestVarGet("p"+p+"nickEquipped"));
		if (trQuestVarGet("nickEquippedLocal") == 0) {
			if (Multiplayer) {
				startNPCDialog(NPC_NICK_DROP);
			}
		} else if (trQuestVarGet("nickQuestProgressLocal") < 5) {
			startNPCDialog(NPC_NICK_START + trQuestVarGet("nickQuestProgressLocal"));
			if (trQuestVarGet("nickQuestProgressLocal") == 0) {
				trQuestVarSet("nickQuestProgressLocal", 1);
			}
		}
	}
}

rule nick_next_dialog
inactive
highFrequency
{
	int p = trCurrentPlayer();
	if (trCountUnitsInArea(""+1*trQuestVarGet("p"+p+"unit"),ENEMY_PLAYER,"Unit",20) == 0) {
		xsDisableSelf();
		startNPCDialog(NPC_NICK_NEXT + trQuestVarGet("p"+p+"nickQuestProgress") - 2);
		trQuestVarSet("nickQuestProgressLocal", trQuestVarGet("p"+p+"nickQuestProgress"));
	}
}