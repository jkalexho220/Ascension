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
					if (Multiplayer == false) {
						next = 6;
					}
				}
				case 4:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "(W) Crescent Strikes: Your next 3 attacks deal bonus damage and stun.";
					if (Multiplayer == false) {
						next = 7;
					}
				}
				case 5:
				{
					icon = "icons\improvement rheias gift icons 64";
					msg = "(E) Protection (Toggle): Grant all allies invulnerability. Costs 12 favor per second.";
					next = 0;
				}
				case 6:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "Cooldown: 18 | Duration: 6 | Radius: 6 | Damage: 20 + 10 per ally";
					next = 4;
				}
				case 7:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "Cooldown: 12 | Damage: 50";
					next = 5;
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
					if (Multiplayer == false) {
						next = 5;
					}
				}
				case 3:
				{
					icon = "icons\improvement sun ray icon 64";
					msg = "(W) Healing Rays: For a short duration, your attacks fire rays that heal allies.";
					if (Multiplayer == false) {
						next = 6;
					}
				}
				case 4:
				{
					icon = "icons\improvement flames of typhon icon";
					msg = "(E) Searing Light (Toggle): Your heals also deal damage to enemies. Costs 5 favor per second.";
					next = 0;
				}
				case 5:
				{
					icon = "icons\god power restoration icon 64";
					msg = "Cooldown: 18 | Duration: 6 | Radius: 6 | Heal: 60";
					next = 3;
				}
				case 6:
				{
					icon = "icons\improvement sun ray icon 64";
					msg = "Cooldown: 12 | Duration: 5 | Heal: 100 | Fire rate scales with projectile count.";
					next = 4;
				}
			}
		}
		case THUNDERRIDER:
		{
			icon = "icons\hero g atalanta icon 64";
			switch(tooltip)
			{
				case 0:
				{
					msg = "Thunderwalker: A speedy assassin that scales off of speed.";
				}
				case 1:
				{
					msg = "Passive: You gain bonus attack by continuously moving. This bonus decays by 5 percent each second.";
				}
				case 2:
				{
					msg = "Special Attack (5 attacks): Your attack chains to nearby enemies, damaging all of them.";
				}
				case 3:
				{
					icon = "icons\god power bolt icon 64";
					msg = "(Q) Blitz: Teleport towards your cursor, stunning and damaging enemies hit.";
				}
				case 4:
				{
					icon = "icons\god power bolt icon 64";
					msg = "If Ride the Lightning is active, this changes your direction of travel instead.";
				}
				case 5:
				{
					icon = "icons\god power bolt icon 64";
					msg = "Cooldown: 8 | Range: 12 | Damage: 50";
				}
				case 6:
				{
					icon = "icons\improvement heart of the titans icons 64";
					msg = "(W) Recharge: Consume all your bonus attack and convert it into healing and favor for yourself.";
				}
				case 7:
				{
					icon = "icons\improvement heart of the titans icons 64";
					msg = "Cooldown: 22 | Heal: 1x Attack | Favor Regen: 0.1x Attack";
				}
				case 8:
				{
					icon = "icons\god power implode icons 64";
					msg = "(E) Ride the Lightning: Become a fast-moving ball of lightning that bounces off walls";
				}
				case 9:
				{
					icon = "icons\god power implode icons 64";
					msg = "Damage enemies that you pass through. Damage increases with each bounce.";
				}
				case 10:
				{
					icon = "icons\god power implode icons 64";
					msg = "Cost: 8 favor per second | Radius: 5 | Damage: 100 + 50 per bounce";
					next = 0;
				}
			}
		}
		case FIREKNIGHT:
		{
			icon = "icons\cavalry x lancer hero icons 64";
			switch(tooltip)
			{
				case 0:
				{
					msg = "Fire Knight: A diving character that sacrifices health for damage.";
				}
				case 1:
				{
					msg = "Passive: Gain spell lifesteal equal to your percentage of missing health.";
				}
				case 2:
				{
					msg = "Special Attack (5 attacks): Deal 80 damage to nearby enemies and yourself.";
				}
				case 3:
				{
					icon = "icons\special e phoenix icon 64";
					msg = "(Q) Flaming Impact: Dash to the target location, damaging all enemies hit.";
				}
				case 4:
				{
					icon = "icons\special e phoenix icon 64";
					msg = "Cooldown: 12 | Radius: 4 | Damage: 20 per meter";
				}
				case 5:
				{
					icon = "icons\improvement flames of typhon icon";
					msg = "(W) Overheat: Set yourself on fire, damaging yourself each second.";
				}
				case 6:
				{
					icon = "icons\improvement flames of typhon icon";
					msg = "During this time, reflect all damage you receive onto nearby enemies.";
				}
				case 7:
				{
					icon = "icons\improvement flames of typhon icon";
					msg = "Cooldown: 14 | Radius: 4 | Duration: 5 | Damage: 40 per second";
				}
				case 8:
				{
					icon = "icons\god power firestorm icon 64";
					msg = "(E) Inferno: Unleash a ring of flames that damages enemies and heals allies for several seconds";
				}
				case 9:
				{
					icon = "icons\god power firestorm icon 64";
					msg = "Cost: 60 | Radius: 12 | Duration: 8 | Damage: 120 per second | Heal: 60 per second";
					next = 0;
				}
			}
		}
		case FROSTKNIGHT:
		{
			icon = "icons\hero g achilles icon 64";
			switch(tooltip)
			{
				case 0:
				{
					msg = "Frost Knight: A fighter that brings a lot of crowd control.";
				}
				case 1:
				{
					msg = "Passive: Each time you stun an enemy, deal 10 percent of your max health to them.";
				}
				case 2:
				{
					msg = "Special Attack (5 attacks): Your attack stuns its target.";
				}
				case 3:
				{
					icon = "";
					msg = "(Q) Icicle: Summon an icicle at the target location, dealing damage.";
				}
				case 4:
				{
					icon = "";
					msg = "If this hits an enemy at full health, stun them.";
				}
				case 5:
				{
					icon = "";
					msg = "Cooldown: 5 | Radius: 1.5 | Duration: 6 | Damage: 80";
				}
				case 6:
				{
					icon = "icons\god power frost icon 64";
					msg = "(W) Blizzard: For a short duration, you and your summoned allies release frost, dealing damage.";
				}
				case 7:
				{
					icon = "icons\god power frost icon 64";
					msg = "Cooldown: 16 | Duration: 6 | Radius: 5 | Damage: 20 per second";
				}
				case 8:
				{
					icon = "icons\special n frost giant icon 64";
					msg = "(E) Frost Giant: Summon a Frost Giant familiar to fight for you. It copies your attack and health.";
				}
				case 9:
				{
					icon = "icons\special n frost giant icon 64";
					msg = "Cost: 60 | Decay: 3 percent health per second.";
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
			name = "Thunderwalker";
		}
		case FIREKNIGHT:
		{
			name = "Fire Knight";
		}
		case FROSTKNIGHT:
		{
			name = "Frost Knight";
		}
	}
	return(name);
}

void explainClass(int class = 0) {
	trQuestVarSet("explain", 0);
	trQuestVarSet("pleaseExplain", class);
	xsEnableRule("class_shop_explain_01");
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
			trQuestVarSet("explain", 0);
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
					class = i;
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
	trQuestVarSet("explain", 
		displayNextTooltip(i, 1*trQuestVarGet("explain")));
	trDelayedRuleActivation("class_shop_explain_02");
}

rule class_shop_explain_02
inactive
highFrequency
{
	int i = trQuestVarGet("pleaseExplain");
	if (trIsGadgetVisible("ShowImageBox") == false) {
		if (trQuestVarGet("explain") > 0) {
			trDelayedRuleActivation("class_shop_explain_01");
		}
		xsDisableSelf();
	}
}