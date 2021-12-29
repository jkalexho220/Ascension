int displayNextTooltip(int class = 0, int tooltip = 0) {
	string icon = "";
	string msg = "";
	int next = tooltip + 1;
	switch(class)
	{
		case MOONBLADE:
		{
			icon = "icons\hero g theseus icon 64";
			switch(tooltip)
			{
				case 0:
				{
					msg = "Moonblade: A jack-of-all-trades good for beginners.";
				}
				case 1:
				{
					msg = "Passive: Each time you attack, restore health to all allies. Heal = 1 percent of your max health";
				}
				case 2:
				{
					msg = "Special Attack (7 attacks): Summon a wolf companion that fights for you.";
				}
				case 3:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "(Q) Moonbeam: Create a circle that damages enemies. Damage increases for each ally in the circle";
				}
				case 4:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "(W) Crescent Strikes: Your next 3 attacks deal bonus damage and stun.";
				}
				case 5:
				{
					icon = "icons\improvement rheias gift icons 64";
					msg = "(E) Protection (Toggle): Grant all allies invulnerability. Costs 12 favor per second.";
					next = 0;
				}
			}
		}
		case SUNBOW:
		{
			icon = "icons\hero g hyppolyta icon 64";
			switch(tooltip)
			{
				case 0:
				{
					msg = "Sunbow: A ranged healer who can also deal good damage.";
				}
				case 1:
				{
					msg = "Passive: All allies regenerate 1 health each second.";
				}
				case 2:
				{
					icon = "icons\god power restoration icon 64";
					msg = "(Q) Sunlight: Create a circle that heals allies for several seconds.";
				}
				case 3:
				{
					icon = "icons\improvement sun ray icon 64";
					msg = "(W) Healing Rays: For a short duration, your attacks fire rays that heal allies.";
				}
				case 4:
				{
					icon = "icons\improvement flames of typhon icon";
					msg = "(E) Searing Light (Toggle): Your heals also deal damage to enemies. Costs 5 favor per second.";
					next = 0;
				}
			}
		}
	}
	trShowImageDialog(icon, msg);
	return(next);
}

string className(int class = 0) {
	string name = "N/A";
	switch(class)
	{
		case MOONBLADE:
		{
			name = "Moonblade";
		}
		case SUNBOW:
		{
			name = "Sunbow";
		}
		case THUNDERRIDER:
		{
			name = "Thunder Rider";
		}
	}
	return(name);
}

rule class_shop_always
inactive
highFrequency
{
	int class = 0;
	for(i=2; >0) {
		trUnitSelectClear();
		trUnitSelectByQV("choice"+i+"unit");
		if (trUnitIsSelected()) {
			trQuestVarSet("choice"+i+"selected", 1);
			trQuestVarSet("pleaseExplain", i);
			trQuestVarSet("choice"+i+"explain", 0);
			trDelayedRuleActivation("class_shop_explain_01");
			uiClearSelection();
		}
		trVectorSetUnitPos("pos", "choice"+i+"unit");
		for (p=1; < ENEMY_PLAYER) {
			if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 5) {
				if (trQuestVarGet("p"+p+"buy"+i) < 4 &&
					trTimeMS() > trQuestVarGet("p"+p+"buyNext") &&
					trQuestVarGet("p"+p+"noob") == 1) {
					trQuestVarSet("p"+p+"buy"+i, trQuestVarGet("p"+p+"buy"+i) + 1);
					trQuestVarSet("p"+p+"buyNext", trTimeMS() + 1000);
					class = i + 2 * trQuestVarGet("stage") - 2;
					if (trQuestVarGet("p"+p+"buy"+i) == 4) {
						/* attempt to buy the class */
						trQuestVarSet("p"+p+"buy"+i, 5);
						chooseClass(p, class);
						if (trCurrentPlayer() == p) {
							trChatSend(0, "You have switched to the " + className(class) + " class!");
						}
						trSoundPlayFN("ui\thunder5.wav","1",-1,"","");
					} else if (trCurrentPlayer() == p) {
						trChatSend(0,"<color=1,1,1>Switching to "+className(class)+" in "+(4-trQuestVarGet("p"+p+"buy"+i))+"...");
					}
				}
			} else {
				trQuestVarSet("p"+p+"buy"+i, 0);
			}
		}
	}
}

rule class_shop_explain_01
inactive
highFrequency
{
	xsDisableSelf();
	int i = trQuestVarGet("pleaseExplain");
	trQuestVarSet("choice"+i+"explain", 
		displayNextTooltip(i + 2 * trQuestVarGet("stage") - 2, 1*trQuestVarGet("choice"+i+"explain")));
	trDelayedRuleActivation("class_shop_explain_02");
}

rule class_shop_explain_02
inactive
highFrequency
{
	int i = trQuestVarGet("pleaseExplain");
	if (trIsGadgetVisible("ShowImageBox") == false) {
		if (trQuestVarGet("choice"+i+"explain") > 0) {
			trDelayedRuleActivation("class_shop_explain_01");
		}
		xsDisableSelf();
	}
}