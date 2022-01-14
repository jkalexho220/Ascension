const int NPC_RELIC_TRANSPORTER = 0;
const int NPC_RELIC_TRANSPORTER_WTF = 1;

const int NPC_EXPLAIN_SINGLEPLAYER = 2;

const int NPC_ZENO_NEXT_QUESTION = 3;
const int NPC_ZENO_QUIZ_END = 4;

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