string classIcon(int class = 0) {
	string icon = "icons\infantry g hoplite icon 64";
	switch(class)
	{
		case MOONBLADE:
		{
			icon = "icons\hero g theseus icon 64";
		}
		case SUNBOW:
		{
			icon = "icons\hero g hyppolyta icon 64";
		}
		case THUNDERRIDER:
		{
			icon = "icons\hero g atalanta icon 64";
		}
		case FIREKNIGHT:
		{
			icon = "icons\cavalry x lancer hero icons 64";
		}
		case FROSTKNIGHT:
		{
			icon = "icons\hero g achilles icon 64";
		}
		case STARSEER:
		{
			icon = "icons\infantry x oracle hero icons 64";
		}
		case STORMCUTTER:
		{
			icon = "icons\archer x arcus hero icons 64";
		}
		case ALCHEMIST:
		{
			icon = "icons\special e pharaoh icon 64";
		}
	}
	return(icon);
}

int displayNextTooltip(int class = 0, int tooltip = 0) {
	string icon = classIcon(class);
	string msg = "";
	int next = tooltip + 1;
	if (class > 16) {
		next = 0;
		int revealed = ((class - 17) / 4) * 2;
		icon = "icons\building outpost icon 64";
		msg = "This class will be revealed when you clear Stage " + revealed;
	}
	switch(class)
	{
		case MOONBLADE:
		{
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
					if (Multiplayer == false) {
						next = 8;
					}
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
					msg = "(E) Protection (Toggle): Grant all allies invulnerability. Costs 15 favor per second.";
					next = 0;
				}
				case 6:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "Cooldown: 18 | Duration: 6 | Radius: 6 | Damage: 30 + 15 per ally";
					next = 4;
				}
				case 7:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "Cooldown: 12 | Damage: 50 | Lose one stack every 5 seconds if not used";
					next = 5;
				}
				case 8:
				{
					msg = "It has a fifth of your attack and health. Decay: 5 percent health per second";
					next = 3;
				}
			}
		}
		case SUNBOW:
		{
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
					msg = "Bonus: 0.1 x Attack for every meter traveled | Decay doubles when standing still";
				}
				case 3:
				{
					msg = "Special Attack (5 attacks): Your attack chains to nearby enemies, damaging all of them.";
				}
				case 4:
				{
					icon = "icons\god power bolt icon 64";
					msg = "(Q) Blitz: Teleport towards your cursor, stunning and damaging enemies hit.";
				}
				case 5:
				{
					icon = "icons\god power bolt icon 64";
					msg = "If Ride the Lightning is active, this changes your direction of travel instead.";
				}
				case 6:
				{
					icon = "icons\god power bolt icon 64";
					msg = "Cooldown: 8 | Range: 12 | Damage: 50";
				}
				case 7:
				{
					icon = "icons\improvement heart of the titans icons 64";
					msg = "(W) Recharge: Consume all your bonus attack and convert it into healing and favor for yourself.";
				}
				case 8:
				{
					icon = "icons\improvement heart of the titans icons 64";
					msg = "Cooldown: 22 | Heal: 1x Attack | Favor Regen: 0.1x Attack";
				}
				case 9:
				{
					icon = "icons\god power implode icons 64";
					msg = "(E) Ride the Lightning: Become a fast-moving ball of lightning that bounces off walls";
				}
				case 10:
				{
					icon = "icons\god power implode icons 64";
					msg = "Damage enemies that you pass through. Damage increases with each bounce.";
				}
				case 11:
				{
					icon = "icons\god power implode icons 64";
					msg = "Cost: 8 favor per second | Radius: 5 | Damage: 100 + 50 per bounce";
					next = 0;
				}
			}
		}
		case FIREKNIGHT:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Fire Knight: A character that charges in and sacrifices health for damage.";
				}
				case 1:
				{
					msg = "Passive: Gain Spell Lifesteal equal to your percentage of missing health.";
				}
				case 2:
				{
					msg = "Special Attack (5 attacks): Deal 50 damage to nearby enemies and yourself.";
				}
				case 3:
				{
					icon = "icons\special e phoenix icon 64";
					msg = "(Q) Flaming Impact: Dash to the target location, damaging enemies and dragging them with you";
				}
				case 4:
				{
					icon = "icons\special e phoenix icon 64";
					msg = "Cooldown: 12 | Radius: 4 | Duration: 2 | Damage: 90";
				}
				case 5:
				{
					icon = "icons\god power chaos icons 64";
					msg = "(W) Overheat: Set yourself on fire, damaging yourself each second.";
				}
				case 6:
				{
					icon = "icons\god power chaos icons 64";
					msg = "During this time, reflect all damage you receive onto nearby enemies.";
				}
				case 7:
				{
					icon = "icons\god power chaos icons 64";
					msg = "Cooldown: 18 | Radius: 4 | Duration: 5 | Damage: 40 per second";
				}
				case 8:
				{
					icon = "icons\god power firestorm icon 64";
					msg = "(E) Inferno: Unleash a ring of flames that damages enemies and heals allies for several seconds";
				}
				case 9:
				{
					icon = "icons\god power firestorm icon 64";
					msg = "Cost: 70 | Radius: 12 | Duration: 8 | Damage: 120 per second | Heal: 60 per second";
					next = 0;
				}
			}
		}
		case FROSTKNIGHT:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Frost Knight: A sturdy fighter that brings a lot of crowd control.";
				}
				case 1:
				{
					msg = "Passive: Each time you stun an enemy, restore 5 pecent of your health.";
				}
				case 2:
				{
					msg = "Special Attack (5 attacks): Your attack stuns its target.";
				}
				case 3:
				{
					icon = "icons\improvement bite of the shark icons 64";
					msg = "(Q) Icicle: Summon an icicle at the target location, dealing damage.";
				}
				case 4:
				{
					icon = "icons\improvement bite of the shark icons 64";
					msg = "If this hits an enemy at full health, stun them.";
				}
				case 5:
				{
					icon = "icons\improvement bite of the shark icons 64";
					msg = "Cooldown: 4 | Radius: 1.5 | Duration: 6 | Damage: 80";
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
		case STARSEER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Starseer: A ranged fighter with a unique attack mechanism.";
				}
				case 1:
				{
					msg = "Passive: Your normal attack deals 0 damage. Instead, 3 stars orbit around you, damaging enemies.";
				}
				case 2:
				{
					msg = "Damage: 30 | Star speed scales with Projectiles";
				}
				case 3:
				{
					msg = "Special Attack (3 attacks): Knock your target away from yourself.";
				}
				case 4:
				{
					icon = "icons\improvement halo of the sun icons 64";
					msg = "(Q) Solar Flare: Your stars explode, damaging and stunning nearby enemies.";
				}
				case 5:
				{
					icon = "icons\improvement halo of the sun icons 64";
					msg = "Cooldown: 15 | Damage: 50 | Radius: 4";
				}
				case 6:
				{
					icon = "icons\improvement eyes of atlas icons 64";
					msg = "(W) Realign: Change the radius of your stars to match the distance of your cursor from your hero";
				}
				case 7:
				{
					icon = "icons\improvement eyes of atlas icons 64";
					msg = "Cooldown: 5 | Max Range: 15";
				}
				case 8:
				{
					icon = "icons\improvement celerity icons 64";
					msg = "(E) The Stars Align: Your stars stop spinning and form a line in the target direction.";
				}
				case 9:
				{
					icon = "icons\improvement celerity icons 64";
					msg = "Consume all your favor over several seconds. When you have none left, fire a massive laser.";
				}
				case 10:
				{
					icon = "icons\improvement celerity icons 64";
					msg = "Cost: 50-100 | Damage: 30 x favor spent";
					next = 0;
				}
			}
		}
		case STORMCUTTER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Stormcutter: An archer who commands the power of storms.";
				}
				case 1:
				{
					msg = "Passive: You deal more damage the further your target is. 10 percent for every 5 meters.";
				}
				case 2:
				{
					icon = "icons\god power lightning icon 64";
					msg = "(Q) Rain of Lightning: Your attacks call lightning to strike your foes, dealing damage in an area";
				}
				case 3:
				{
					icon = "icons\god power lightning icon 64";
					msg = "Cooldown: 12 | Duration: 4 | Damage: 2 x Attack x Spell Power | Fire rate scales with projectiles";
				}
				case 4:
				{
					icon = "icons\god power shifting sand icon 64";
					msg = "(W) Disengage: Stun nearby enemies and then teleport a short distance towards your cursor.";
				}
				case 5:
				{
					icon = "icons\god power shifting sand icon 64";
					msg = "Cooldown: 8 | Stun Radius: 4 | Teleport Range: 12";
				}
				case 6:
				{
					icon = "icons\god power tornado icon 64";
					msg = "(E) Whirlwind: Create a storm that instantly pulls in nearby enemies.";
				}
				case 7:
				{
					icon = "icons\god power tornado icon 64";
					msg = "Cost: 70 | Range: 18";
					next = 0;
				}
			}
		}
		case ALCHEMIST:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Alchemist: A versatile support with the right tool for every situation.";
				}
				case 1:
				{
					msg = "Passive: Every third attack, you throw a potion with a special effect.";
				}
				case 2:
				{
					icon = "icons\god power frost icon 64";
					msg = "Freeze: The potion stuns your target. Additional projectiles will stun nearby enemies.";
				}
				case 3:
				{
					icon = "icons\god power pestilence icon 64";
					msg = "Poison: The potion spills poison on the ground, poisoning enemies in an area.";
				}
				case 4:
				{
					icon = "icons\god power pestilence icon 64";
					msg = "Duration: 12 | Damage: 12 x Projectiles";
				}
				case 5:
				{
					icon = "icons\god power restoration icon 64";
					msg = "Heal: When the potion is thrown, all allies restore 5 percent of their health";
				}
				case 6:
				{
					icon = "icons\improvement temple of healing icon 64";
					msg = "(Q) Elixir: Restore health to an ally and remove all status effects from them.";
				}
				case 7:
				{
					icon = "icons\improvement temple of healing icon 64";
					msg = "Cooldown: 12 | Heal: 200";
				}
				case 8:
				{
					icon = "icons\improvement salt amphora icon 64";
					msg = "(W) Swap Potion: Swap the type of potion you throw, cycling between the three options.";
				}
				case 9:
				{
					icon = "icons\god power call to arms icon";
					msg = "(E) Duplicate: Summon a perfect copy of an allied hero. It also casts spells with them.";
				}
				case 10:
				{
					icon = "icons\god power call to arms icon";
					msg = "Cost: 50 | Decay: 1 + 1 per second (percent max health)";
					next = 0;
				}
			}
		}
		case SPELLSTEALER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Spellstealer: An assassin who reflects status effects onto enemies.";
				}
				case 1:
				{
					msg = "Passive: Every third attack, you throw a potion with a special effect.";
				}
				case 2:
				{
					icon = "icons\god power frost icon 64";
					msg = "Freeze: The potion stuns your target. Additional projectiles will stun nearby enemies.";
				}
				case 3:
				{
					icon = "icons\god power pestilence icon 64";
					msg = "Poison: The potion spills poison on the ground, poisoning enemies in an area.";
				}
				case 4:
				{
					icon = "icons\god power pestilence icon 64";
					msg = "Duration: 12 | Damage: 12 x Projectiles";
				}
				case 5:
				{
					icon = "icons\god power restoration icon 64";
					msg = "Heal: When the potion is thrown, all allies restore 5 percent of their health";
				}
				case 6:
				{
					icon = "icons\improvement temple of healing icon 64";
					msg = "(Q) Elixir: Restore health to an ally and remove all status effects from them.";
				}
				case 7:
				{
					icon = "icons\improvement temple of healing icon 64";
					msg = "Cooldown: 12 | Heal: 200";
				}
				case 8:
				{
					icon = "icons\improvement salt amphora icon 64";
					msg = "(W) Swap Potion: Swap the type of potion you throw, cycling between the three options.";
				}
				case 9:
				{
					icon = "icons\god power call to arms icon";
					msg = "(E) Duplicate: Summon a perfect copy of an allied hero. It also casts spells with them.";
				}
				case 10:
				{
					icon = "icons\god power call to arms icon";
					msg = "Cost: 50 | Decay: 1 + 1 per second (percent max health)";
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
		case STARSEER:
		{
			name = "Starseer";
		}
		case STORMCUTTER:
		{
			name = "Stormcutter";
		}
		case ALCHEMIST:
		{
			name = "Alchemist";
		}
		case SPELLSTEALER:
		{
			name = "Spellstealer";
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
			reselectMyself();
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