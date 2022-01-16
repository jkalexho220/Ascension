const int NPC_RELIC_TRANSPORTER = 0;
const int NPC_RELIC_TRANSPORTER_WTF = 1;

const int NPC_EXPLAIN_SINGLEPLAYER = 2;

const int NPC_ZENO_NEXT_QUESTION = 3;
const int NPC_ZENO_QUIZ_END = 4;


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
				case 33:
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
		case NPC_EXPLAIN_SINGLEPLAYER:
		{
			switch(dialog)
			{
				case 1:
				{
					trCameraCut(vector(90,70.710701,90), vector(0.5,-0.707107,0.5), vector(0.5,0.707107,0.5), vector(0.707107,0,-0.707107));
					uiMessageBox("Welcome to the Seeker's Guild! There are many things you can do here!");
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
					uiMessageBox("Why did I come to this tower? I should've never listened to those bastards!");
				}
				case 2:
				{
					uiMessageBox("What's so great about the Tower of Ascension anyway? It's not like anyone makes it to the top!");
				}
				case 3:
				{
					uiMessageBox("You should leave too! Before you're killed!");
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
			zSetProtoUnitStat("Revealer", 1, 2, 32);
			trQuestVarSet("townCenter", trGetNextUnitScenarioNameNumber());
			trArmyDispatch("1,0","Revealer",1,70*x+40,0,70*z+40,225,true);
			trArmySelect("1,0");
			trUnitConvert(0);
			trSoundPlayFN("settlement.wav","1",-1,"","");
		}
	} else {
		yDatabaseNext("npcTalk", true);
		if (trUnitIsSelected()) {
			startNPCDialog(1*yGetVar("npcTalk", "dialog"));
			reselectMyself();
		}
		trUnitSelectClear();
		trUnitSelectByQV("questGuy");
		if (false) {
			uiLookAtUnitByName(""+1*trQuestVarGet("questGuy"));
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