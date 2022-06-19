const int NPC_RELIC_TRANSPORTER = 0;
const int NPC_RELIC_TRANSPORTER_WTF = 1;

const int NPC_EXPLAIN_SINGLEPLAYER = 2;

const int NPC_ZENO_NEXT_QUESTION = 3;
const int NPC_ZENO_QUIZ_END = 4;

const int NPC_BOSS_ENTRANCE = 5;

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
/* reserved to 408 */
const int NPC_NICK_DROP = 409;
const int NPC_NICK_NEXT = 410;
/* reserved to 413 */
const int NPC_NICK_QUEST_COMPLETE = 414;
const int NPC_NICK_SLOT_MACHINE = 415;

const int NPC_EXPLAIN_DEEP = 416;
const int NPC_EXPLAIN_CLOUDS = 417;
const int NPC_EXPLAIN_PIT = 418;
const int NPC_EXPLAIN_SPACE = 419;

const int NPC_MONSTER_TAMER_START = 420;
const int NPC_MONSTER_TAMER_NEXT = 421;
const int MONSTERPEDIA_NPC = 421;
/*
reserved to 424
*/
const int NPC_DEVIL_DIE = 425;
const int NPC_MONSTERPEDIA_COMPLETE = 426;

const int NPC_HIPPOCAMPUS_QUEST = 427;
const int NPC_HIPPOCAMPUS_QUEST_AGAIN = 428;
const int NPC_HIPPOCAMPUS_QUEST_DONE = 429;

const int NPC_VENLESH_START = 430;
const int NPC_VENLESH_NEXT = 431;
const int NPC_VENLESH_FIRST = 432;
const int NPC_VENLESH_SECOND = 433;
const int NPC_VENLESH_FINAL = 434;

const int NPC_RUNESTONE_FIRST = 435;
const int NPC_RUNESTONE_SECOND = 436;

const int NPC_RUNESTONE_FINAL = 437;

const int NPC_RUNESTONE_DWARF = 438;
const int NPC_RUNESTONE_AKARD = 439;

const int NPC_EXCALIBUR_START = 444;
const int NPC_EXCALIBUR_NEXT = 445;
const int NPC_EXCALIBUR_END = 446;

const int NPC_EXCALIBUR_BYSTANDER = 447;
/*
RESERVED TO 457
*/
const int NPC_EXCALIBUR_BREAK = 458;

const int NPC_POISON_TRIAL_START = 459;
const int NPC_POISON_TRIAL_NEXT = 460;

const int NPC_HUNT_THE_TIGER_KILL = 461;

const int NPC_RELIC_TRANSPORTER_QUEST = 462;
/*
RESERVED TO 468
*/

const int FETCH_NPC = 10;
const int BOUNTY_NPC = 20;
const int SHOP_NPC = 30;
const int FETCH_GUY = 1;
const int BOUNTY_GUY = 2;
const int SHOP_GUY = 3;

void startNPCDialog(int npc = 0) {
	if (trQuestVarGet("currentNPCProgress") == 0) {
		xsEnableRule("npc_talk_01");
		trQuestVarSet("currentNPC", npc);
		trQuestVarSet("currentNPCProgress", 0);
	}
}

int npcDiag(int npc = 0, int dialog = 0) {
	dialog = dialog + 1;
	int gem = 0;
	string extra = "";
	switch(npc)
	{
		case NPC_RELIC_TRANSPORTER_QUEST:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Relic Transporting Company here! Did you like our services while in the Tower?");
				}
				case 2:
				{
					uiMessageBox("Well today, we've got an important task for you!");
				}
				case 3:
				{
					uiMessageBox("We've witnessed your prowess in battle first-hand, and would like your help.");
				}
				case 4:
				{
					uiMessageBox("We need some cargo delivered to various people within the Tower.");
				}
				case 5:
				{
					uiMessageBox("However, the cargo attracts monsters.");
				}
				case 6:
				{
					uiMessageBox("For each successful delivery, we'll give you some pretty awesome rewards.");
					xSetInt(dPlayerData, xPlayerRelicTransporterLevel, 1);
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Ready for your first delivery?");
				}
				case 2:
				{
					uiMessageBox("Please escort our goods to Foothill Camp on the first floor.");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Thanks for your help last time!");
				}
				case 2:
				{
					uiMessageBox("Next, I need your help delivering goods to the town on the third floor.");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Good work on the deliveries!");
				}
				case 2:
				{
					uiMessageBox("Next up, the camp on the fifth floor is in desperate need of supplies.");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("We've got an unusual request today.");
				}
				case 2:
				{
					uiMessageBox("The wizard Akard needs some goods delivered to him.");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Yeebaagooon has submitted a request for some supplies.");
				}
				case 2:
				{
					uiMessageBox("He said not to look at them though, or else he would fry us alive.");
				}
				case 3:
				{
					uiMessageBox("Anyway, you'll be the one at risk, not me. Have fun!");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 6:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Thank you for all that you have done for us, adventurer.");
				}
				case 2:
				{
					uiMessageBox("Our journey together has been an amazing one. For your help, we offer this reward:");
				}
				case 3:
				{
					uiMessageBox("Bring this relic with you whenever you want one of us to accompany you.");
					spawnRelicSpecific(vector(163,0,151), RELIC_TRANSPORTER_TICKET);
					xSetInt(dPlayerData, xPlayerRelicTransporterLevel, 7);
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 7:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I can offer you my assistance anytime.");
				}
				case 2:
				{
					uiMessageBox("Bring this relic with you into the Tower and one of us will join your adventure.");
				}
				case 3:
				{
					uiMessageBox("Completely free of charge!");
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_QUEST + 8:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Well then, off you go.");
				}
				case 2:
				{
					uiMessageBox("Bring back a souvenir!");
					dialog = 0;
				}
			}
		}
		case NPC_EXCALIBUR_START:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey you, have you heard? They found something incredible on the first floor.");
				}
				case 2:
				{
					uiMessageBox("There's a sword stuck in a rock! All sorts of adventurers have been trying to pull it out!");
				}
				case 3:
				{
					uiMessageBox("But so far, no one's been able to. The stone around it regenerates too.");
				}
				case 4:
				{
					uiMessageBox("Maybe if we damage the stone faster than it can regenerate, we can pull the sword out?");
				}
				case 5:
				{
					trMessageSetText("Uncover the sword on the first floor.", -1);
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
					trQuestVarSet("p1swordpieceQuest"+SWORD_HANDLE, 1);
					dialog = 0;
				}
			}
		}
		case NPC_EXCALIBUR_NEXT:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("People have been trying to pull out the sword in the first floor.");
				}
				case 2:
				{
					uiMessageBox("Maybe we can damage the stone around it enough to free it.");
				}
				case 3:
				{
					uiMessageBox("But the stone regenerates, so our damage needs to surpass the regeneration.");
					dialog = 0;
				}
			}
		}
		case NPC_EXCALIBUR_BREAK:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You uncovered the sword, only to discover...");
				}
				case 2:
				{
					uiMessageBox("...it was actually just the handle of a sword!");
				}
				case 3:
				{
					uiMessageBox("Acquired: Handle of an unknown sword");
					dialog = 0;
				}
			}
		}
		case NPC_EXCALIBUR_END:
		{
			uiMessageBox("What? So it was just the handle of a sword? Haha! Sucks to be you!");
			xsDisableRule("rogers_always");
			trUnitSelectClear();
			trUnitSelectByQV("phdorogers4");
			trUnitChangeProtoUnit("Hero Birth");
			trQuestVarSet("p1swordpieceQuest"+SWORD_HANDLE, 0);
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER:
		{
			uiMessageBox("Come on! Put some back into it!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 1:
		{
			uiMessageBox("Man, can anyone pull this sword out?");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 2:
		{
			uiMessageBox("Ha! Back in my day, we used to pull swords out of rocks all the time!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 3:
		{
			uiMessageBox("Whoever pulls it out first gets a kiss from me!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 4:
		{
			uiMessageBox("Have you tried morbing it?");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 5:
		{
			uiMessageBox("What? That's it?");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 6:
		{
			uiMessageBox("Lame!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 7:
		{
			uiMessageBox("Hahahaha! You young'uns couldn't even pull out a real sword! Ahahahaha!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 8:
		{
			uiMessageBox("Ew, just a hilt won't satisfy me! I'm not kissing you!");
			dialog = 0;
		}
		case NPC_EXCALIBUR_BYSTANDER + 9:
		{
			uiMessageBox("Looks like you didn't morb hard enough.");
			dialog = 0;
		}
		case NPC_HUNT_THE_TIGER_KILL:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Boss defeated! Here are the rewards!");
				}
				case 2:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(relicIcon(1*trQuestVarGet("bossReward")), relicName(1*trQuestVarGet("bossReward")));
				}
				case 3:
				{
					trSoundPlayFN("favordump.wav","1",-1,"","");
					trShowImageDialog(boonIcon(BOON_HEALTH_ATTACK), boonName(BOON_HEALTH_ATTACK));
				}
				case 4:
				{
					uiMessageBox("You can equip this blessing in singleplayer.");
					dialog = 0;
				}
			}
		}
		case NPC_RUNESTONE_FIRST:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Once upon a time, there was a horrific entity known as The Void that threatened the world.");
				}
				case 2:
				{
					uiMessageBox("The gods fought against The Void, but found that their attacks could not touch it.");
				}
				case 3:
				{
					uiMessageBox("They asked the legendary blacksmith nottud to craft them a mighty weapon that could harm The Void.");
				}
				case 4:
				{
					uiMessageBox("He granted their wish, and thus, the Starsword was born.");
					if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") > 1 || trQuestVarGet("runestoneComplete") == 1) {
						dialog = 0;
					}
				}
				case 5:
				{
					uiMessageBox("...");
				}
				case 6:
				{
					uiMessageBox("You feel a shiver run down your spine.");
				}
				case 7:
				{
					uiMessageBox("Something has taken notice of you.");
					trSoundPlayFN("spybirth.wav","1",-1,"","");
					trQuestVarSet("p"+trCurrentPlayer()+"runestoneQuest", 2);
					dialog = 0;
				}
			}
		}
		case NPC_VENLESH_FIRST:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("So it's a tale of something that happened in the past.");
				}
				case 2:
				{
					uiMessageBox("The Starsword... The Void... a blacksmith named nottud... Hmm...");
				}
				case 3:
				{
					uiMessageBox("Thank you for your help, adventurer. The second runestone has also long since been discovered.");
				}
				case 4:
				{
					uiMessageBox("It's on the fifth floor of the Tower.");
					dialog = 0;
				}
			}
		}
		case NPC_RUNESTONE_SECOND:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("With the Starsword, the gods were able to drive off The Void, but at a terrible price.");
				}
				case 2:
				{
					uiMessageBox("The Starsword was far too powerful for any god to handle alone.");
				}
				case 3:
				{
					uiMessageBox("One by one, they wielded the sword and fought until its power overwhelmed them.");
				}
				case 4:
				{
					uiMessageBox("As one god fell, the next would take up the blade to continue the fight.");
				}
				case 5:
				{
					uiMessageBox("In the end, only one god remained. The rest had succumbed to madness.");
					if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") > 2 || trQuestVarGet("runestoneComplete") == 1) {
						dialog = 0;
					}
				}
				case 6:
				{
					uiMessageBox("...");
				}
				case 7:
				{
					uiMessageBox("You sense a presence from beyond. Something knows where you are.");
					trSoundPlayFN("spybirth.wav","1",-1,"","");
					trQuestVarSet("p"+trCurrentPlayer()+"runestoneQuest", 3);
					dialog = 0;
				}
			}
		}
		case NPC_VENLESH_SECOND:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Oh my, what a horrific tale. Just who is that last god?");
				}
				case 2:
				{
					uiMessageBox("Thank you for your help, adventurer. I know you are taking on a terrible risk.");
				}
				case 3:
				{
					uiMessageBox("Unfortunately, no one knows where the final Runestone is.");
				}
				case 4:
				{
					uiMessageBox("Perhaps the dwarves know something?");
				}
				case 5:
				{
					uiMessageBox("They're indigenous to the Tower.");
					dialog = 0;
				}
			}
		}
		case NPC_VENLESH_FINAL:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Amazing. So this is the true tale...");
				}
				case 2:
				{
					uiMessageBox("Once upon a time, there was a horrific entity known as The Void that threatened the world.");
				}
				case 3:
				{
					uiMessageBox("The gods fought against The Void, but found that their attacks could not touch it.");
				}
				case 4:
				{
					uiMessageBox("They asked the legendary blacksmith nottud to craft them a mighty weapon that could harm The Void.");
				}
				case 5:
				{
					uiMessageBox("He granted their wish, and thus, the Starsword was born.");
				}
				case 6:
				{
					uiMessageBox("With the Starsword, the gods were able to drive off The Void, but at a terrible price.");
				}
				case 7:
				{
					uiMessageBox("The Starsword was far too powerful for any god to handle alone.");
				}
				case 8:
				{
					uiMessageBox("One by one, they wielded the sword and fought until its power overwhelmed them.");
				}
				case 9:
				{
					uiMessageBox("As one god fell, the next would take up the blade to continue the fight.");
				}
				case 10:
				{
					uiMessageBox("In the end, only one god remained. The rest had succumbed to madness.");
				}
				case 11:
				{
					uiMessageBox("With all his compatriots going mad, the last god knew what he had to do.");
				}
				case 12:
				{
					uiMessageBox("He sealed them all within a massive tower and sunk it into the ocean depths.");
				}
				case 13:
				{
					uiMessageBox("Each god was imprisoned on a separate floor to prevent them from hurting the others.");
				}
				case 14:
				{
					uiMessageBox("As for the Starsword, it was split into three pieces and sealed within the tower as well.");
				}
				case 15:
				{
					uiMessageBox("Should The Void ever return, the tower would rise from the ground and beckon challengers to come forth.");
				}
				case 16:
				{
					uiMessageBox("Perhaps this time, a worthy champion can wield the Starsword and defeat The Void for good...");
				}
				case 17:
				{
					uiMessageBox("So I guess you've gotta find these three sword pieces now, huh?");
				}
				case 18:
				{
					uiMessageBox("Wait, what's that?! OH GOD!");
					dialog = 0;
					xsEnableRule("the_keeper_hunt_start");
					xsEnableRule("keeper_grab_singleplayer");
				}
			}
		}
		case NPC_RUNESTONE_DWARF:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You're looking for the final Runestone? How are you still alive after reading the first two?!");
				}
				case 2:
				{
					uiMessageBox("Anyway, it is dangerous to speak of it, for it resides in a place of madness.");
				}
				case 3:
				{
					uiMessageBox("It's in a fractured dimension filled with logical fallacies. Most terrifying.");
				}
				case 4:
				{
					uiMessageBox("If you wish to know more, then speak with the wizard Akard on the fourth floor.");
					dialog = 0;
				}
			}
		}
		case NPC_RUNESTONE_AKARD:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("What?! You've read the Runestones?! Get away from me! Before that thing strikes!");
				}
				case 2:
				{
					uiMessageBox("Wait, perhaps... If you read the final one... Fine! I'll tell you where it is!");
				}
				case 3:
				{
					uiMessageBox("It's locked away in Zeno's Paradox! If you go there, you will find it!");
				}
				case 4:
				{
					uiMessageBox("How to get to that dimension? I'm not saying!");
				}
				case 5:
				{
					uiMessageBox("It's a land of mathematical errors, and I hate math errors!");
					dialog = 0;
				}
			}
		}
		case NPC_RUNESTONE_FINAL:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("With all his compatriots going mad, the last god knew what he had to do.");
				}
				case 2:
				{
					uiMessageBox("He sealed them all within a massive tower and sunk it into the ocean depths.");
				}
				case 3:
				{
					uiMessageBox("Each god was imprisoned on a separate floor to prevent them from hurting the others.");
				}
				case 4:
				{
					uiMessageBox("As for the Starsword, it was split into three pieces and sealed within the tower as well.");
				}
				case 5:
				{
					uiMessageBox("Should The Void ever return, the tower would rise from the ground and beckon challengers to come forth.");
				}
				case 6:
				{
					uiMessageBox("Perhaps this time, a worthy champion can wield the Starsword and defeat The Void for good...");
					dialog = 0;
				}
			}
		}
		case NPC_VENLESH_START:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hello! Any high-level adventurers here? I need help!");
				}
				case 2:
				{
					uiMessageBox("Ah, you! I need your help! I'm Venlesh, an archaeologist researching the Tower.");
				}
				case 3:
				{
					uiMessageBox("Inside the Tower, there's fabled to be three Runestones that hold incredible truths.");
				}
				case 4:
				{
					uiMessageBox("However, anyone who reads these Runestones will be hunted by an unknown horror.");
				}
				case 5:
				{
					uiMessageBox("Many have tried and failed, which is why I'm looking for a high-level adventurer to help.");
				}
				case 6:
				{
					uiMessageBox("Can you help me read the Runestones?");
				}
				case 7:
				{
					trMessageSetText("Read the first Runestone.", -1);
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
					trQuestVarSet("p1runestoneQuest", 1);
					dialog = 0;
				}
			}
		}
		case NPC_VENLESH_NEXT:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The first Runestone has long since been discovered on the second floor of the Tower.");
				}
				case 2:
				{
					uiMessageBox("It's surrounded by ghosts and should be pretty hard to miss.");
				}
				case 3:
				{
					trMessageSetText("Read the first Runestone.", -1);
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
					dialog = 0;
				}
			}
		}
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
					dialog = 0;
				}
			}
		}
		case NPC_RELIC_TRANSPORTER_WTF:
		{
			dialog = 0;
			uiMessageBox("You want to hire another? What happened to the last guy?!");
		}
		case NPC_BOSS_ENTRANCE:
		{
			uiMessageBox("Find the relic matching my symbol and bring it to me to open the portal to the boss room.");
			dialog = 0;
		}
		case NPC_MONSTERPEDIA_COMPLETE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Thank you for saving the Monsterpedia, adventurer!");
				}
				case 2:
				{
					uiMessageBox("The Monsterpedia will reveal to you detailed information about monsters you have encountered.");
				}
				case 3:
				{
					uiMessageBox("In addition, I grant you this power as a reward...");
				}
				case 4:
				{
					trShowImageDialog(boonIcon(BOON_MONSTER_COMPANION), boonName(BOON_MONSTER_COMPANION));
				}
				case 5:
				{
					uiMessageBox("Thank you again, and good luck on your journey, adventurer!");
					xsEnableRule("monsterpedia_complete");
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
					uiLookAtUnitByName(""+xGetInt(dBoons,xUnitName));
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
				if (xGetInt(dPlayerData, xPlayerProgress, 1) < 6) {
					uiMessageBox("That's all for today! I'll be back after you clear another floor!");
				}
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
					uiMessageBox("Please help me! I've fallen victim to a bug and turned into a relic!");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					uiMessageBox("There should be some Sphinxes on the first floor of the Tower.");
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
					uiMessageBox("We can find Dryads on the second floor of the Tower.");
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
		
		case NPC_NICK_START + 4:
		{
			if (Multiplayer == false) {
				uiMessageBox("Alright, just set me down over there at the glowing spotlight.");
				trMinimapFlare(1, 3.0, trVectorQuestVarGet("nickPos"), true);
			} else {
				uiMessageBox("Let's return to the Guild to complete my transformation!");
			}
			dialog = 0;
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
		
		case NPC_NICK_QUEST_COMPLETE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Ahh, back to my sexy body. Thank you for rescuing me, friend.");
				}
				case 2:
				{
					uiMessageBox("As a reward, I'll let you use my Quantum Relic Machine.");
				}
				case 3:
				{
					uiMessageBox("To use it, drop three relics in front of me and then select me to confirm.");
				}
				case 4:
				{
					uiMessageBox("The three relics will be consumed and a new random relic will be generated.");
				}
				case 5:
				{
					uiMessageBox("First roll is free. Let's give it a whirl!");
				}
				case 6:
				{
					trQuestVarSet("p1nickQuestProgress", 5);
					dialog = 0;
					trQuestVarSet("quantumSlotMachine", 2);
					trQuestVarSet("quantumSlotMachineNext", trTimeMS() + 3000);
					trQuestVarSetFromRand("quantumRelic", 10, 26, true);
					xsEnableRule("quantum_slot_machine");
					trSoundPlayFN("plentybirth.wav","1",-1,"","");
					trSoundPlayFN("skypassageout.wav","1",-1,"","");
				}
			}
		}
		
		case NPC_NICK_SLOT_MACHINE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Want to spin the Quantum Slot Machine? Drop three relics here to start the machine!");
				}
				case 2:
				{
					uiMessageBox("You will get a new random relic that is guaranteed to not be one of the ones you dropped.");
					dialog = 0;
				}
			}
		}
		
		case NPC_EXPLAIN_DEEP:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Here in the ocean depths, the pressure of water will crush you into a pancake.");
				}
				case 2:
				{
					uiMessageBox("However, you are protected in a bubble created by your Hippocampus.");
				}
				case 3:
				{
					uiMessageBox("If you exit the bubble, you take damage! (The bubble is the Line of Sight of the Hippocampus)");
				}
				case 4:
				{
					uiMessageBox("You only get one Hippocampus, so keep her safe!");
				}
				case 5:
				{
					uiMessageBox("In addition, the Palace of the Deep will constantly spawn hunters to attack you.");
				}
				case 6:
				{
					uiMessageBox("Find and destroy the Palace to make the attacks stop.");
					dialog = 0;
				}
			}
		}
		
		case NPC_EXPLAIN_CLOUDS:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The thin atmosphere gives you limited resources.");
				}
				case 2:
				{
					uiMessageBox("All players lose 0.5 favor per second on this floor.");
				}
				case 3:
				{
					uiMessageBox("In addition, dangerous tornados wander the skies, dealing high damage.");
				}
				case 4:
				{
					uiMessageBox("If you find and destroy the Sky Palace, the tornados will stop.");
					dialog = 0;
				}
			}
		}
		
		case NPC_EXPLAIN_PIT:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The flames of the Pits are hostile to the living.");
				}
				case 2:
				{
					uiMessageBox("Healing is reduced to 20 percent effectiveness on this floor.");
				}
				case 3:
				{
					uiMessageBox("In addition, the Flame Palace will occasionally summon Tartarian Gates to attack you.");
				}
				case 4:
				{
					uiMessageBox("Destroy the Flame Palace to make the attacks stop.");
					dialog = 0;
				}
			}
		}

		case NPC_MONSTER_TAMER_START:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("This is terrible! The Monsterpedia has been stolen!");
				}
				case 2:
				{
					uiMessageBox("That nefarious thief devil_do1 stole it and escaped into the Tower!");
				}
				case 3:
				{
					uiMessageBox("Adventurer! Please retrieve the Monsterpedia! It is a dangerous artifact in the wrong hands!");
				}
				case 4:
				{
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
					trMessageSetText("Find devil_do1 and retrieve the Monsterpedia.", -1);
					dialog = 0;
				}
			}
		}

		case NPC_MONSTER_TAMER_NEXT:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Please catch devil_do1 and retrieve our lost artifact!");
				}
				case 2:
				{
					trShowImageDialog("icons\scenario kemsyt icon 64", "This is his last known appearance.");
				}
				case 3:
				{
					uiMessageBox("Try talking to the people in the Tower. They might have seen something.");
					dialog = 0;
				}
			}
		}

		case MONSTERPEDIA_NPC + 1:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You're looking for a criminal who stole something from the Guild? Do you have a picture?");
				}
				case 2:
				{
					uiMessageBox("Ooh, who's this handsome devil? Unfortunately, I haven't seen him on this floor.");
					dialog = 0;
				}
			}
		}

		case MONSTERPEDIA_NPC + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You're looking for the dastardly thief devil_do1?");
				}
				case 2:
				{
					uiMessageBox("Too bad, haven't seen him... Wait a minute!");
				}
				case 3:
				{
					uiMessageBox("Dammit! I should've charged you for that information!");
					dialog = 0;
				}
			}
		}

		case MONSTERPEDIA_NPC + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You're looking for a thief? Oh, this guy!");
				}
				case 2:
				{
					uiMessageBox("This bastard stole from me too! Fortunately, I place a tracker in all my goods.");
				}
				case 3:
				{
					uiMessageBox("Here, use this device to find him. He's not on this floor anymore.");
				}
				case 4:
				{
					if (trQuestVarGet("p"+trCurrentPlayer()+"monsterpediaQuest") == 1) {
						trQuestVarSet("p"+trCurrentPlayer()+"monsterpediaQuest", 2);
						trMessageSetText("Rehost the map and catch devil_do1.",-1);
						trSoundPlayFN("xnew_objective.wav","1",-1,"","");
					}
					dialog = 0;
				}
			}
		}

		case NPC_DEVIL_DIE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("devil_do1 has been defeated. You have retrieved the Monsterpedia");
				}
				case 2:
				{
					uiMessageBox("Return to singleplayer to complete the quest.");
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
					uiMessageBox("They say there are ten floors in this Tower, but no one knows what's at the top.");
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
					uiMessageBox("Such horrific beings reside within the Tower. If you're not ready, turn back now.");
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
					uiMessageBox("I never knew there could be native peoples living in the Tower.");
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
					uiMessageBox("Welcome, travelers! Come and rest a while. The ascent up this Tower can be quite arduous.");
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
					uiMessageBox("Very interesting. What drives people to climb this Tower?");
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

		case NPC_QUEST + SHOP_NPC + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("I don't think I can make it any further than this. Want to buy my equipment?");
				}
				case 2:
				{
					trShowImageDialog(relicIcon(1*trQuestVarGet("shopRelic")),relicName(1*trQuestVarGet("shopRelic")));
				}
				case 3:
				{
					uiMessageBox("Maybe it will be better off in your hands.");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					if (trQuestVarGet("p"+trCurrentPlayer()+"runestoneQuest") < 3) {
						uiMessageBox("Impressive. If you had brought the wrong fruit...");
					} else {
						uiMessageBox("Oh thank goodness. If you had brought the wrong fruit, something catastrophic-");
					}
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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

		case NPC_HIPPOCAMPUS_QUEST:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Hey! Are you headed to the seventh floor? You'll need something special!");
				}
				case 2:
				{
					uiMessageBox("The seventh floor resembles the bottom of the ocean! Without oxygen, you will suffocate!");
				}
				case 3:
				{
					uiMessageBox("You will need the help of a Hippocampus, but they are elusive and difficult to catch!");
				}
				case 4:
				{
					uiMessageBox("They reside within the Tower. Catch one to have it assist you!");
					dialog = 0;
				}
			}
		}

		case NPC_HIPPOCAMPUS_QUEST_AGAIN:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You can likely find a Hippocampus on any floor below 6");
				}
				case 2:
				{
					uiMessageBox("You must stun it in order to capture it!");
					dialog = 0;
				}
			}
		}

		case NPC_HIPPOCAMPUS_QUEST_DONE:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You have captured the Hippocampus. It will assist you on floor 7");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
					trSoundPlayFN("xnew_objective.wav","1",-1,"","");
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
		
		case NPC_TEMPLE + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Which is the lie? Which is the truth?");
				}
				case 2:
				{
					uiMessageBox("Pierce the darkness and be rewarded with power!");
				}
				case 3:
				{
					uiMessageBox("To begin the trial, drop a relic at my feet.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE_COMPLETE + 2:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The wheel of time turns ever infinitely.");
				}
				case 2:
				{
					uiMessageBox("But for now, I grant you this power.");
				}
				case 3:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")), boonName(1*trQuestVarGet("stageTemple")));
				}
				case 4:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE + 3:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Those who endure the harshest winters shall be rewarded.");
				}
				case 2:
				{
					uiMessageBox("To begin the trial, drop a relic at my feet.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Wealth is power! Power absolute!");
				}
				case 2:
				{
					uiMessageBox("Sacrifice your wealth to me, and be rewarded with ever greater power!");
				}
				case 3:
				{
					dialog = trQuestVarGet("p"+trCurrentPlayer()+"relicsSacrificed") + trQuestVarGet("boonUnlocked"+BOON_MORE_GOLD);
					uiMessageBox("Relics sacrificed: ("+dialog+"/10)");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE_COMPLETE + 4:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("A most expensive sacrifice. You have proven yourself. I grant you this reward.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Do you have the courage to face death?");
				}
				case 2:
				{
					uiMessageBox("In each of these cups, there is a deadly poison. Drink too much of it, and you die.");
				}
				case 3:
				{
					uiMessageBox("But one cup contains a most powerful lifeblood. Drink it to attain my power.");
				}
				case 4:
				{
					uiMessageBox("You have at most three guesses. Find the lifeblood or perish. Do you dare to take the gamble?");
				}
				case 5:
				{
					trMessageSetText("Find the correct goblet in three guesses. (Guesses are shared for the whole party)",-1);
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE_COMPLETE + 5:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Fate has smiled upon you today. And for that, I grant you this power.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
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
					uiMessageBox("If you can survive 47 seconds within this room, my power is yours.");
				}
				case 3:
				{
					uiMessageBox("But step one foot outside, and you will be deemed unworthy.");
				}
				case 4:
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
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")), boonName(1*trQuestVarGet("stageTemple")));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE + 7:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("The tides cannot be stopped, but they can be weathered.");
				}
				case 2:
				{
					uiMessageBox("Defeat these hordes and be granted their strength.");
				}
				case 3:
				{
					uiMessageBox("Drop a relic at my feet to begin the trial.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE_COMPLETE + 7:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Your strength has been acknowledged. Accept this power as your reward.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE + 8:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("LONG HAVE PLAYERS IGNORED THE DREADED STATUE PUZZLE.");
				}
				case 2:
				{
					uiMessageBox("BUT YOUR TIME IS NIGH. COMPLETE THIS PUZZLE TO RECEIVE MY BLESSING.");
				}
				case 3:
				{
					uiMessageBox("IT'S A REALLY GOOD BLESSING I PROMISE.");
					dialog = 0;
				}
			}
		}
		
		case NPC_TEMPLE_COMPLETE + 8:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Wow you actually did it. Touch some grass y'all.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
					dialog = 0;
				}
			}
		}

		case NPC_TEMPLE + 9:
		{
			switch(dialog)
			{
				case 1:
				{
					if (trQuestVarGet("templefailed") == 0) {
						uiMessageBox("Your journey has been long. You have learned much in reaching here.");
					} else {
						uiMessageBox("Better luck next time, foolish one.");
						dialog = 0;
					}
				}
				case 2:
				{
					uiMessageBox("It is time for your ultimate test. Ready? Let's begin.");
				}
				case 3:
				{
					dialog = 0;
					xsEnableRule("status_effect_temple_always");
					trQuestVarSet("currentQuestion", 0);
					trQuestVarSet("currentAnswer", 1);
					trStringQuestVarSet("question", "You cannot cast spells when stunned.");
					trStringQuestVarSet("questionfirst", "True");
					trStringQuestVarSet("questionsecond", "False");
					trEventSetHandler(6001, "quizTempleQuestion");
					trEventSetHandler(6002, "quizTempleQuestion");
				}
			}
		}

		case NPC_TEMPLE_COMPLETE + 9:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("You have successfully passed the exam. Here is your reward.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(1*trQuestVarGet("stageTemple")),boonName(1*trQuestVarGet("stageTemple")));
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
					uiMessageBox("Show thy mettle, and be rewarded with glorious power.");
				}
				case 3:
				{
					uiMessageBox("What is " + 1*trQuestVarGet("operand1") + " + " + 1*trQuestVarGet("operand2") + "?");
					dialog = 0;
				}
			}
		}

		case NPC_TEMPLE_COMPLETE + 11:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("So you solved the math problem. Congratulations. Here is your reward.");
				}
				case 2:
				{
					trSoundPlayFN("sentinelbirth.wav","1",-1,"","");
					trShowImageDialog(boonIcon(BOON_SPELL_ATTACK),boonName(BOON_SPELL_ATTACK));
				}
				case 3:
				{
					uiMessageBox("You can equip this Blessing in singleplayer.");
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
			vector pos = trVectorQuestVarGet("relicTransporterGuyPos");
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
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
			vector pos = kbGetBlockPosition(""+1*trQuestVarGet("bossEntranceStatue"));
			trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
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
			xSetInt(dPlayerData, xPlayerGold, gold + xGetInt(dPlayerData, xPlayerGold), p);
			trPlayerGrantResources(p, "gold", gold);
		}
	}
	trQuestVarSet("questCount", 1 + trQuestVarGet("questCount"));
	startNPCDialog(NPC_QUEST_COMPLETE + trQuestVarGet("stage") + 10 * trQuestVarGet("localQuest"));
	trQuestVarSet("questActive", -1);
	uiLookAtUnitByName(""+1*trQuestVarGet("questGuy"));
}

int dQuestLeaves = 0;
int dQuestTargets = 0;
int xLeafType = 0;

int dWorthlessJunk = 0;

int dNottudShop = 0;
int xNottudShopPos = 0;

int dNpcTalk = 0;
int xNpcDialog = 0;

rule initialize_town_database
active
highFrequency
{
	xsDisableSelf();
	dQuestLeaves = initGenericProj("questLeaves",kbGetProtoUnitID("Einheriar"),18,10.0,1.0);
	xLeafType = xInitAddInt(dQuestLeaves,"type");
	
	dQuestTargets = xInitDatabase("questTargets");
	xInitAddInt(dQuestTargets,"name");
	xInitAddInt(dQuestTargets,"type");
	
	dWorthlessJunk = xInitDatabase("worthlessJunk");
	xInitAddInt(dWorthlessJunk,"name");
	
	dNpcTalk = xInitDatabase("npcTalk",3);
	xInitAddInt(dNpcTalk,"name");
	xNpcDialog = xInitAddInt(dNpcTalk,"dialog");
}

rule runestone_dwarf_talk
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelectByQV("guy"+SHOP_GUY);
	if (trUnitIsSelected()) {
		startNPCDialog(NPC_RUNESTONE_DWARF);
		reselectMyself();
	}
}

rule akard_runestone_dialog
inactive
highFrequency
{
	int p = trCurrentPlayer();
	trUnitSelectClear();
	trUnitSelectByQV("guy"+FETCH_GUY);
	if (trQuestVarGet("p"+p+"runestoneQuest") == 3) {
		if (trUnitIsSelected()) {
			startNPCDialog(NPC_RUNESTONE_AKARD);
			reselectMyself();
		}
	}
}

rule town_always
inactive
highFrequency
{
	int x = 0;
	int z = 0;
	int p = 0;
	int next = 0;
	int room = 0;
	int action = 0;
	float amt = 0;
	float angle = 0;
	vector center = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector loc = vector(0,0,0);
	vector dir = vector(0,0,0);

	xDatabaseNext(dNpcTalk);
	xUnitSelect(dNpcTalk,xUnitName);
	if (trUnitIsSelected()) {
		startNPCDialog(xGetInt(dNpcTalk,xNpcDialog));
		reselectMyself();
	}

	if (trQuestVarGet("townFound") == 0) {
		trUnitSelectClear();
		trUnitSelectByQV("guy1");
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				trQuestVarSet("townFound", 1);
				trVectorSetUnitPos("questGuyPos", "questGuy");
				break;
			}
		}
		if (trQuestVarGet("townFound") == 1) {
			x = trQuestVarGet("village");
			z = x / 4;
			x = trQuestVarGet("village") - 4 * z;
			trVectorQuestVarSet("townCenter", xsVectorSet(70*x + 40, 0, 70*z + 40));
			trArmyDispatch("1,0","Revealer to Player",1,70*x+40,0,70*z+40,225,true);
			trSoundPlayFN("settlement.wav","1",-1,"","");
		}
	} else {
		
		if (trQuestVarGet("questActive") == 0) {
			for(p=1; < ENEMY_PLAYER) {
				if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), trVectorQuestVarGet("questGuyPos")) < 16) {
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
									pos = kbGetBlockPosition(""+1*trQuestVarGet("pig"+i));
									if (vectorInRectangle(pos, trVectorQuestVarGet("pigpenLower"), trVectorQuestVarGet("pigpenUpper"))) {
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
							activateEnemy(1*trQuestVarGet("bountyTarget"), 100);
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
								if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), trVectorQuestVarGet("questGuyPos")) < 16) {
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
							xSetPointer(dFreeRelics,xAddDatabaseBlock(dFreeRelics));
							xSetInt(dFreeRelics,xRelicName,1*trQuestVarGet("fetchTarget"));
							xSetInt(dFreeRelics,xRelicType, RELIC_GHOST_PICTURE);
						}
						case 2:
						{
							pos = kbGetBlockPosition(""+1*trQuestVarGet("fetchTarget"),true);
							if (distanceBetweenVectors(pos, trVectorQuestVarGet("questGuyPos")) < 16.0) {
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
							trCounterAddTime("countdown", 10, 1, "The shade attacks", -1);
							trQuestVarSet("shadeAttackTime", trTime() + 10);
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
								center = kbGetBlockPosition(""+1*trQuestVarGet("fetchTarget"),true);
								trQuestVarSetFromRand("count", 1, ENEMY_PLAYER, true);
								for(x=trQuestVarGet("count"); >0) {
									trQuestVarSetFromRand("angle", 0, 6.283185, false);
									angle = trQuestVarGet("angle");
									dir = xsVectorSet(xsSin(angle),0,xsCos(angle));
									trQuestVarSet("heading", 57.29578 * angle - 180);
									pos = xsVectorSet(20.0 * xsVectorGetX(dir) + xsVectorGetX(center),0,20.0 * xsVectorGetZ(dir) + xsVectorGetZ(center));
									next = trGetNextUnitScenarioNameNumber();
									loc = vectorToGrid(pos);
									if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
										trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),trQuestVarGet("heading"),true);
										trUnitSelectClear();
										trUnitSelect(""+next,true);
										trUnitConvert(ENEMY_PLAYER);
										trUnitChangeProtoUnit("Fenris Wolf");
										trUnitSelectClear();
										trUnitSelect(""+next,true);
										trUnitMoveToPoint(xsVectorGetX(center),0,xsVectorGetZ(center),-1,true);
										activateEnemy(next);
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
									next = trGetNextUnitScenarioNameNumber();
									trQuestVarSetFromRand("heading", 0, 360, true);
									trArmyDispatch("1,0", "Dwarf", 1, 70*x+40,0,70*z+40,trQuestVarGet("heading"),true);
									trArmySelect("1,0");
									trUnitConvert(ENEMY_PLAYER);
									trUnitChangeProtoUnit("Frost Giant");
									xSetPointer(dEnemiesIncoming,xAddDatabaseBlock(dEnemiesIncoming));
									xSetInt(dEnemiesIncoming,xUnitName,next);
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
							pos = kbGetBlockPosition(""+1*trQuestVarGet("questguy"),true);
							for(p=1; < ENEMY_PLAYER) {
								if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), pos) < 16) {
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
							if (xGetDatabaseCount(dQuestLeaves) > 0) {
								action = processGenericProj(dQuestLeaves);
								if (action == PROJ_GROUND) {
									xUnitSelectByID(dQuestLeaves,xUnitID);
									trUnitChangeProtoUnit("Dryad");
									xUnitSelectByID(dQuestLeaves,xUnitID);
									trDamageUnitPercent(-100);
									activateEnemy(xGetInt(dQuestLeaves,xUnitName),-1,0);
									xSetPointer(dQuestTargets,xAddDatabaseBlock(dQuestTargets));
									xSetInt(dQuestTargets,xUnitName,xGetInt(dQuestLeaves,xUnitName));
									xFreeDatabaseBlock(dQuestLeaves);
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
									trQuestVarSetFromRand("rand", 8, 20, false);
									addGenericProj(dQuestLeaves,xsVectorSet(70*x+40,0,70*z+40),trVectorQuestVarGet("questDir"),
										ENEMY_PLAYER,trQuestVarGet("rand"),1);
									trVectorQuestVarSet("questDir", rotationMatrix(trVectorQuestVarGet("questDir"), -0.757322, 0.653041));
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
							if (xGetDatabaseCount(dQuestTargets) > 0) {
								xDatabaseNext(dQuestTargets);
								xUnitSelect(dQuestTargets,xUnitName);
								if (trUnitAlive() == false) {
									xFreeDatabaseBlock(dQuestTargets);
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
							if (xGetDatabaseCount(dQuestLeaves) > 0) {
								action = processGenericProj(dQuestLeaves);
								if (action == PROJ_GROUND) {
									xUnitSelectByID(dQuestLeaves,xUnitID);
									trUnitChangeProtoUnit("Relic");
									xUnitSelectByID(dQuestLeaves,xUnitID);
									trDamageUnitPercent(-100);
									xSetPointer(dFreeRelics,xAddDatabaseBlock(dFreeRelics));
									xSetInt(dFreeRelics,xRelicName,xGetInt(dQuestLeaves,xUnitName));
									xSetInt(dFreeRelics,xRelicType,xGetInt(dQuestLeaves,xLeafType));
									
									xSetPointer(dQuestTargets,xAddDatabaseBlock(dQuestTargets));
									xSetInt(dQuestTargets,xUnitName,xGetInt(dQuestLeaves,xUnitName));
									xSetInt(dQuestTargets,xRelicType,xGetInt(dQuestLeaves,xLeafType));
									
									xFreeDatabaseBlock(dQuestLeaves);
									if (xGetDatabaseCount(dQuestLeaves) + trQuestVarGet("questSpawns") == 0) {
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
									trQuestVarSetFromRand("rand", 12, 16, false);
									addGenericProj(dQuestLeaves,xsVectorSet(70*x+36,0,70*z+36),trVectorQuestVarGet("questDir"),
										ENEMY_PLAYER,trQuestVarGet("rand"));
									xSetInt(dQuestLeaves,xLeafType, RELIC_MATH_PROBLEM + trQuestVarGet("questSpawns"));
									trQuestVarSet("questSpawns", trQuestVarGet("questSpawns") - 1);
									trVectorQuestVarSet("questDir", rotationMatrix(trVectorQuestVarGet("questDir"), -0.757322, 0.653041));
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
							xDatabaseNext(dQuestTargets);
							if (unitDistanceToVector(xGetInt(dQuestTargets,xUnitName), trVectorQuestVarGet("questGuyPos")) < 9) {
								xUnitSelect(dQuestTargets,xUnitName);
								if (xGetInt(dQuestTargets, xRelicType) - RELIC_MATH_PROBLEM == trQuestVarGet("questAnswer")) {
									trUnitChangeProtoUnit("Osiris Box Glow");
									questComplete(STARSTONE, -1);
								} else {
									trQuestVarSet("wrongAnswer", xGetInt(dQuestTargets, xRelicType) - RELIC_MATH_PROBLEM);
									xsDisableRule("gameplay_always");
									xsDisableRule("town_always");
									xsDisableRule("boss_entrance_always");
									xsDisableRule("enter_boss_room");
									xsDisableRule("nottud_always");
									xsDisableRule("class_shop_always");
									xsDisableRule("relic_transporter_guy_always");
									xsDisableRule("relic_transporter_guy_found");
									xsDisableRule("the_keeper_hunt");
									xClearDatabase(dKeeperPaint);
									trUnitChangeProtoUnit("Dwarf");
									xUnitSelect(dQuestTargets,xUnitName);
									trUnitConvert(0);
									trUnitChangeProtoUnit("Implode Sphere Effect");
									trSoundPlayFN("wonderdeath.wav","1",-1,"","");
									trSoundPlayFN("changeunit.wav","1",-1,"","");
									trChatSendSpoofed(0, "Akard: OH GOD! WHAT HAVE YOU DONE?! THAT'S THE WRONG-");
									xsEnableRule("zenos_paradox_00");
									trQuestVarSet("cinNext", 0);
									trQuestVarSet("cinStep", 0);
									xClearDatabase(dQuestTargets);
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
							trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms) - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								xDatabaseNext(dBasicRooms);
							}
							room = xGetInt(dBasicRooms,xRoomNumber);
							pos = randomNearEdgeOfRoom(room);
							trQuestVarSet("fecesRelic", trGetNextUnitScenarioNameNumber());
							spawnRelicSpecific(pos,RELIC_LITERAL_FECES);
							trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms) - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								xDatabaseNext(dBasicRooms);
							}
							room = xGetInt(dBasicRooms,xRoomNumber);
							pos = randomNearEdgeOfRoom(room);
							trQuestVarSet("berryRelic", trGetNextUnitScenarioNameNumber());
							spawnRelicSpecific(pos,RELIC_BERRY_BUSH);
							trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms) - 1, true);
							for(i=0; < trQuestVarGet("rand")) {
								xDatabaseNext(dBasicRooms);
							}
							room = xGetInt(dBasicRooms,xRoomNumber);
							pos = randomNearEdgeOfRoom(room);
							trQuestVarSet("corpseRelic", trGetNextUnitScenarioNameNumber());
							spawnRelicSpecific(pos,RELIC_A_FUCKING_CORPSE);
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
								pos = trVectorQuestVarGet("questGuyPos");
								next = trGetNextUnitScenarioNameNumber();
								trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),180,true);
								trUnitSelectClear();
								trUnitSelect(""+next,true);
								trUnitConvert(ENEMY_PLAYER);
								trUnitChangeProtoUnit("Walking Berry Bush");
								activateEnemy(next, 10);
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
							pos = trVectorQuestVarGet("questGuyPos");
							pos = xsVectorSet(xsVectorGetX(pos) - 2, 0, xsVectorGetZ(pos) - 2);
							spawnRelicSpecific(pos,RELIC_POISON_BUCKET);
							trQuestVarSet("questActive", 2);
						}
						case 2:
						{
							p = trQuestVarGet("poisonBucketHolder");
							if (p > 0) {
								if (xGetInt(dPlayerUnits, xPoisonStatus, xGetInt(dPlayerData,xPlayerIndex,p)) > 0) {
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
							trQuestVarSetFromRand("rand", 1, xGetDatabaseCount(dBasicRooms), true);
							for(i=0; < trQuestVarGet("rand")) {
								xDatabaseNext(dBasicRooms);
							}
							room = xGetInt(dBasicRooms,xRoomNumber);
							pos = randomNearEdgeOfRoom(room);
							trQuestVarSet("yeebRelic", trGetNextUnitScenarioNameNumber());
							spawnRelicSpecific(pos, RELIC_YEEBAAGOOON);
							trQuestVarSet("questActive", 2);
							trQuestVarSet("magicDetectorRelic", trGetNextUnitScenarioNameNumber());
							pos = trVectorQuestVarGet("questGuyPos");
							pos = xsVectorSet(xsVectorGetX(pos) - 2, 0, xsVectorGetZ(pos) - 2);
							spawnRelicSpecific(pos, RELIC_MAGIC_DETECTOR);
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
								trQuestVarSet("yeebBossFight", 0);
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
									pos = kbGetBlockPosition(""+xGetInt(dPlayerData,xPlayerUnit,p));
									trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
									trArmySelect("1,0");
									trDamageUnitPercent(100);
									trUnitChangeProtoUnit("Implode Sphere Effect");
									trUnitSelectClear();
									trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
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
							if (xGetDatabaseCount(dWorthlessJunk) > 0) {
								xDatabaseNext(dWorthlessJunk);
								pos = kbGetBlockPosition(""+xGetInt(dWorthlessJunk,xUnitName));
								if (vectorInRectangle(pos, trVectorQuestVarGet("yeebRoomBottom"), trVectorQuestVarGet("yeebRoomTop")) == false) {
									xUnitSelect(dWorthlessJunk,xUnitName);
									trUnitChangeProtoUnit("Dust Large");
									trSoundPlayFN("swing2.wav","1",-1,"","");
									trChatSend(0, "<color=1,1,1>Worthless junk removed.");
									xFreeDatabaseBlock(dWorthlessJunk);
								}
							} else {
								xsDisableRule("yeeb_hit_list");
								trQuestVarSet("yeebHit", 0);
								trQuestVarSet("yeebBossFight", 0);
								questComplete(MANASTONE, -1);
							}
						}
					}
				}
			}
		}
		
		
		if ((trTime() > trQuestVarGet("townHealNext")) && (boss == 0)) {
			trQuestVarSet("townHealNext", trTime());
			pos = trVectorQuestVarGet("townCenter");
			for(p=1; < ENEMY_PLAYER) {
				if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p),pos) < 400) {
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dPlayerData,xPlayerUnit,p),true);
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
			vector pos = kbGetBlockPosition(""+1*trQuestVarGet("nottud"), true);
			trArmyDispatch("1,0","Revealer to Player",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),225,true);
			trSoundPlayFN("oracledone.wav","1",-1,"","");
		}
	} else {
		if (trUnitIsSelected()) {
			startNPCDialog(NPC_NOTTUD);
			reselectMyself();
		}
		xDatabaseNext(dNottudShop);
		xUnitSelect(dNottudShop,xUnitName);
		if (trUnitIsSelected()) {
			relic = xGetInt(dNottudShop, xRelicType);
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
				xsDisableSelf();
				trQuestVarSet("stage", 11);
				xsEnableRule("rebuild_map");
				trOverlayText("Zeno's Paradox",3,-1,-1,-1);
				for(p=1; < ENEMY_PLAYER) {
					xSetInt(dPlayerData, xPlayerDead, 0, p);
				}
				trQuestVarSet("deadPlayerCount", 0);
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
	if (trUnitAlive()) {
		trQuestVarSet("yeebTarget", kbUnitGetOwner(kbGetBlockID(""+1*trQuestVarGet("yeebRelic"))));
		if (trQuestVarGet("yeebTarget") == 0) {
			trQuestVarSet("yeebHit", 0);
			trQuestVarSet("yeebBossFight", 0);
		} else {
			trQuestVarSet("yeebBossFight", 1);
			if (trUnitIsOwnedBy(trCurrentPlayer())) {
				trQuestVarSet("yeebHit", 1);
			}
		}
	} else {
		int p = trQuestVarGet("yeebTarget");
		if (xGetInt(dPlayerData, xPlayerDead, p) == 0) {
			int relics = getRelicsDB(p);
			for(x=xGetDatabaseCount(relics); >0) {
				xDatabaseNext(relics);
				if (xGetInt(relics, xRelicType) == RELIC_YEEBAAGOOON) {
					trQuestVarSet("yeebRelic", xGetInt(relics, xUnitName));
					break;
				}
			}
		}
	}
}

rule nick_dialog
inactive
minInterval 3
{
	int p = trCurrentPlayer();
	if (trQuestVarGet("nickEquippedLocal") != trQuestVarGet("p"+p+"nickEquipped")) {
		trQuestVarSet("nickEquippedLocal", trQuestVarGet("p"+p+"nickEquipped"));
		if (trQuestVarGet("nickEquippedLocal") == 0) {
			if (Multiplayer) {
				startNPCDialog(NPC_NICK_DROP);
			} else if (trQuestVarGet("nickQuestProgressLocal") == 4) {
				if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), trVectorQuestVarGet("nickPos")) < 4) {
					xSetPointer(dFreeRelics,xGetNewestPointer(dFreeRelics));
					for(i=xGetDatabaseCount(dFreeRelics); >0) {
						if (xGetInt(dFreeRelics,xRelicType) != RELIC_NICKONHAWK) {
							xDatabaseNext(dFreeRelics);
						} else {
							break;
						}
					}
					xUnitSelect(dFreeRelics,xUnitName);
					trUnitChangeProtoUnit("Cinematic Block");
					trQuestVarSet("nickonhawk", xGetInt(dFreeRelics,xUnitName));
					xFreeDatabaseBlock(dFreeRelics);
					trQuestVarSet("cinTime", trTimeMS() + 5000);
					trQuestVarSet("cinStep", 0);
					xsEnableRule("nick_transform");
					trUnitSelectClear();
					trUnitSelectByQV("nickSpotlight", true);
					trUnitChangeProtoUnit("Osiris Birth");
					trUnitSelectClear();
					trUnitSelectByQV("nickSpotlight", true);
					trSetSelectedScale(0,0,0);
					xsDisableSelf();
					trSoundPlayFN("cinematics\24_in\magic.mp3", "5", -1, "","");
				}
			}
		} else {
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
	if (trCountUnitsInArea(""+xGetInt(dPlayerData,xPlayerUnit,p),ENEMY_PLAYER,"Unit",20) == 0) {
		xsDisableSelf();
		startNPCDialog(NPC_NICK_NEXT + trQuestVarGet("p"+p+"nickQuestProgress") - 2);
		trQuestVarSet("nickQuestProgressLocal", trQuestVarGet("p"+p+"nickQuestProgress"));
	}
}

void readTheRunestone(int eventID = -1) {
	switch(1*trQuestVarGet("stage"))
	{
		case 2:
		{
			startNPCDialog(NPC_RUNESTONE_FIRST);
		}
		case 5:
		{
			startNPCDialog(NPC_RUNESTONE_SECOND);
		}
		case 11:
		{
			startNPCDialog(NPC_RUNESTONE_FINAL);
		}
	}
}

rule runestone_read
inactive
minInterval 3
{
	if (boss == 0) {
		int p = trCurrentPlayer();
		switch(1*trQuestVarGet("readRunestone"))
		{
			case 0:
			{
				if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), trVectorQuestVarGet("runestonePos")) < 36.0) {
					trShowChoiceDialog("The Runestone stands before you. Do you read it?", "Yes", 10002, "No", -1);
					trQuestVarSet("readRunestone", 1);
				}
			}
			case 1:
			{
				if (unitDistanceToVector(xGetInt(dPlayerData, xPlayerUnit, p), trVectorQuestVarGet("runestonePos")) > 36.0) {
					trQuestVarSet("readRunestone", 0);
				}
			}
		}
	}
}

rule guardian_dialog
inactive
highFrequency
{
	trUnitSelectClear();
	trUnitSelect(""+bossUnit, true);
	if (trUnitIsSelected()) {
		uiMessageBox("The statue stands alone on the platform. Something is missing from his hand.");
		reselectMyself();
	}
}