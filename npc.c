int NPC_RELIC_TRANSPORTER = 0;

int npcDiag(int npc = 0, int dialog = 0) {
	dialog = dialog + 1;
	switch(npc)
	{
		case NPC_RELIC_TRANSPORTER:
		{
			switch(dialog)
			{
				case 1:
				{
					uiMessageBox("Greetings! I am the Relic Transporter!");
				}
				case 2:
				{
					uiMessageBox("Drop a relic in front of me and I will teleport it to your warehouse for 10 gold.");
				}
				case 3:
				{
					uiMessageBox("You can visit your warehouse by playing this map in singleplayer.");
				}
			}
			if (dialog == 4) {
				dialog = -1;
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
		trQuestVarSet("relicTransporterGuySelected", 1);
		xsEnableRule("relic_transporter_guy_explain_01");
		trQuestVarSet("relicTransporterGuyExplain", 0);
		uiClearSelection();
	}
}

rule relic_transporter_guy_explain_01
inactive
highFrequency
{
	xsDisableSelf();
	trQuestVarSet("relicTransporterGuyExplain", 
		npcDiag(NPC_RELIC_TRANSPORTER, 1*trQuestVarGet("relicTransporterGuyExplain")));
	if (trQuestVarGet("relicTransporterGuyExplain") > 0) {
		trDelayedRuleActivation("relic_transporter_guy_explain_02");
	}
}

rule relic_transporter_guy_explain_02
inactive
highFrequency
{
	if ((trQuestVarGet("relicTransporterGuyExplain") > 0) && (trIsGadgetVisible("ingame-messagedialog") == false)) {
		trDelayedRuleActivation("relic_transporter_guy_explain_01");
		xsDisableSelf();
	}
}