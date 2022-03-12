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
		case BLASTMAGE:
		{
			icon = "icons\special e priest icon 64";
		}
		case STORMCUTTER:
		{
			icon = "icons\archer x arcus hero icons 64";
		}
		case ALCHEMIST:
		{
			icon = "icons\special e pharaoh icon 64";
		}
		case SPELLSTEALER:
		{
			icon = "icons\infantry x murmillo hero icons 64";
		}
		case COMMANDO:
		{
			icon = "icons\cavalry x jav cav hero icons 64";
		}
		case THRONESHIELD:
		{
			icon = "icons\infantry x protector hero icons 64";
		}
		case SAVIOR:
		{
			icon = "icons\hero g bellerophon icon 64";
		}
		case GARDENER:
		{
			icon = "icons\hero g chiron icon 64";
		}
		case NIGHTRIDER:
		{
			icon = "icons\hero g achilles icon 64";
		}
		case STARSEER:
		{
			icon = "icons\infantry x oracle hero icons 64";
		}
		case SPARKWITCH:
		{
			icon = "icons\special g circe icon 64";
		}
		case GAMBLER:
		{
			icon = "icons\infantry g hypaspist icon 64";
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
						next = 9;
					}
				}
				case 3:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "(Q) Moonbeam: Create a circle that damages and silences enemies in it for several seconds.";
					if (Multiplayer == false) {
						next = 6;
					}
				}
				case 4:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "(W) Crescent Strikes: Your next 3 attacks deal bonus damage and stun.";
					if (Multiplayer == false) {
						next = 8;
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
					msg = "Cooldown: 18 | Duration: 6 | Radius: 6 | Damage: 30";
					next = 4;
				}
				case 8:
				{
					icon = "icons\improvement swine array icon 64";
					msg = "Cooldown: 12 | Damage: 50 | Lose one stack every 5 seconds if not used";
					next = 5;
				}
				case 9:
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
					msg = "Passive: You can revive allies even when enemies are nearby.";
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
					msg = "Cooldown: 12 | Duration: 6 | Heal: 100 | Fire rate + 0.5x Projectiles";
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
					msg = "Thunderstepper: A speedy assassin that scales off of speed.";
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
					msg = "Passive: Gain Lifesteal equal to your percentage of missing health.";
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
		case BLASTMAGE:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Blastmage: A ranged fighter who commands powerful magic.";
				}
				case 1:
				{
					msg = "Passive: You cannot gain Projectiles. After you cast a spell, your next attack is empowered.";
				}
				case 2:
				{
					msg = "It will grant you 3 favor, deal damage in an area, and create a Mage Fire.";
				}
				case 3:
				{
					msg = "You can store up to three empowered attacks.";
				}
				case 4:
				{
					msg = "Damage: 60 | Radius: 4 (Increased by Projectiles)";
				}
				case 5:
				{
					icon = "sfx a implode lighting flair";
					msg = "Mage Fires remain on the ground for several seconds, damaging nearby enemies.";
				}
				case 6:
				{
					icon = "sfx a implode lighting flair";
					msg = "Duration: 10 | Radius: 4 | Damage: 10 per second";
				}
				case 7:
				{
					icon = "icons\god power meteor icon 64";
					msg = "(Q) Starfall: A star falls from the sky and damages enemies in an area. This creates a Mage Fire.";
				}
				case 8:
				{
					icon = "icons\god power meteor icon 64";
					msg = "Enemies hit by the blast will have their magic resist reduced permanently.";
				}
				case 9:
				{
					icon = "icons\god power meteor icon 64";
					msg = "Cooldown: 8 | Radius: 5 | Damage: 120 | Resist Reduction: 0.1";
				}
				case 10:
				{
					icon = "icons\god power vortex icons 64";
					msg = "(W) Warp: Teleport a short distance towards your cursor and leave behind a Mage Fire.";
				}
				case 11:
				{
					icon = "icons\god power vortex icons 64";
					msg = "Cooldown: 6 | Range: 8";
				}
				case 12:
				{
					icon = "icons\building tower mirror icons 64";
					msg = "(E) Solar Flare: Fire a laser through each of your Mage Fires, dealing damage to enemies.";
				}
				case 13:
				{
					icon = "icons\building tower mirror icons 64";
					msg = "Cost: 40 | Range: 40 | Damage: 90";
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
					icon = "icons\building archery range icon 64";
					msg = "(Q) Shock Arrow: Fire an arrow at your cursor that damages and launches the first enemy hit.";
				}
				case 3:
				{
					icon = "icons\building archery range icon 64";
					msg = "If they are launched into a wall, they take the same amount of damage again.";
				}
				case 4:
				{
					icon = "icons\building archery range icon 64";
					msg = "Cooldown: 10 | Range: 30 | Damage: 100";
				}
				case 5:
				{
					icon = "icons\god power shifting sand icon 64";
					msg = "(W) Disengage: Stun nearby enemies and then teleport a short distance towards your cursor.";
				}
				case 6:
				{
					icon = "icons\god power shifting sand icon 64";
					msg = "Cooldown: 8 | Stun Radius: 4 | Teleport Range: 12";
				}
				case 7:
				{
					icon = "icons\god power lightning icon 64";
					msg = "(E) Rain of Lightning: Your attacks call lightning to strike your foes, dealing damage in an area";
				}
				case 8:
				{
					icon = "icons\god power lightning icon 64";
					msg = "The damage is multiplied by your spell power and it ignores enemy armor.";
				}
				case 9:
				{
					icon = "icons\god power lightning icon 64";
					msg = "Cost: 50 | Duration: 10 | Radius: 5 | Damage: Attack x Spell Power | Fire rate + 0.3x Projectiles";
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
					msg = "Radius: 4 | Duration: 12 | Damage: 12 x Projectiles";
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
					msg = "Passive: When you damage an enemy, the damage is doubled for each status effect on them.";
				}
				case 2:
				{
					msg = "Special Attack (6 attacks): Poison your target and nearby enemies.";
				}
				case 3:
				{
					msg = "Radius: 6 | Duration: 12 | Damage: 12";
				}
				case 4:
				{
					icon = "icons\god power barrage icon";
					msg = "(Q) Spellblades: Throw daggers in a cone that damage enemies.";
				}
				case 5:
				{
					icon = "icons\god power barrage icon";
					msg = "Cooldown: 9 | Damage: 0.5 x Attack x Spell Power | Knives: 5 + 2 x Projectiles";
				}
				case 6:
				{
					icon = "icons\god major leto icons 64";
					msg = "(W) Negation Cloak: For a short duration, you absorb status effects and spell damage that hits you.";
				}
				case 7:
				{
					icon = "icons\god major leto icons 64";
					msg = "During this time, spell damage will heal you instead of damaging you.";
				}
				case 8:
				{
					icon = "icons\god major leto icons 64";
					msg = "When this spell ends, your next spell will apply all the absorbed status effects.";
				}
				case 9:
				{
					icon = "icons\god major leto icons 64";
					msg = "Cooldown: 12 | Duration: 3";
				}
				case 10:
				{
					icon = "icons\improvement greatest of fifty icon 64";
					msg = "(E) Blade Dance: Teleport to an enemy and attack them. Damage ignores their armor.";
				}
				case 11:
				{
					icon = "icons\improvement greatest of fifty icon 64";
					msg = "If the target is affected by a status effect, also attack nearby enemies with the same status";
				}
				case 12:
				{
					icon = "icons\improvement greatest of fifty icon 64";
					msg = "If this hits only one enemy, it costs half.";
				}
				case 13:
				{
					icon = "icons\improvement greatest of fifty icon 64";
					msg = "Cost: 20 | Radius: 15 | Damage: Attack x Spell Power";
					next = 0;
				}
			}
		}
		case COMMANDO:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Commando: A mid-ranged fighter that wields an arsenal of explosive weaponry.";
				}
				case 1:
				{
					msg = "Passive: You wield a shotgun that shoots piercing bullets in a cone.";
				}
				case 2:
				{
					msg = "Restore 1 favor when these bullets hit an enemy.";
				}
				case 3:
				{
					msg = "Damage per bullet: 0.6 x Attack | Bullets: 4 + Projectiles | Damage falls off with distance.";
				}
				case 4:
				{
					icon = "icons\improvement electrum bullets icon 64";
					msg = "(Q) Shrapnel Shot: Fire an explosive at your cursor.";
				}
				case 5:
				{
					icon = "icons\improvement electrum bullets icon 64";
					msg = "If it hits a wall, it explodes and releases a cone of shrapnel, dealing physical damage.";
				}
				case 6:
				{
					icon = "icons\improvement electrum bullets icon 64";
					msg = "Damage per bullet: 0.6 x Attack | Bullets: 8 + 2 x Projectiles | Damage falls off with distance.";
				}
				case 7:
				{
					icon = "icons\special e phoenix egg icon 64";
					msg = "(W) Echo Bomb: Attach a bomb to an enemy that records all the damage it takes for several seconds";
				}
				case 8:
				{
					icon = "icons\special e phoenix egg icon 64";
					msg = "When the bomb explodes, deal all the stored damage in a large radius.";
				}
				case 9:
				{
					icon = "icons\special e phoenix egg icon 64";
					msg = "Cooldown: 20 | Duration: 6 | Radius: 12 | An enemy can only have one Echo Bomb at a time";
				}
				case 10:
				{
					icon = "icons\siege x fire siphon icons 64";
					msg = "(E) Minigun: Switch to your minigun, which continuously fires bullets at longer range.";
				}
				case 11:
				{
					icon = "icons\siege x fire siphon icons 64";
					msg = "These bullets do not pierce through enemies or grant you favor.";
				}
				case 12:
				{
					icon = "icons\siege x fire siphon icons 64";
					msg = "Cost: 1 per bullet | Range: 1.5x | Damage: 1 x Attack | Fire rate: 3 + Projectiles";
					next = 0;
				}
			}
		}
		case THRONESHIELD:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Throne Shield: A frontline fighter that tanks damage for allies.";
				}
				case 1:
				{
					msg = "Passive: When you take damage, reduce it by 2 percent for every 100 health you have.";
				}
				case 2:
				{
					msg = "Special Attack (10 attacks): Stun your target and heal all allies for 5 percent of your health.";
				}
				case 3:
				{
					icon = "icons\god power ragnarok icon 64";
					msg = "(Q) Knight's Vow: Choose an allied player to be your partner.";
				}
				case 4:
				{
					icon = "icons\god power ragnarok icon 64";
					msg = "When they take damage, you take half of it instead. Their lifesteal will heal you as well.";
				}
				case 5:
				{
					icon = "icons\god power ragnarok icon 64";
					msg = "Also, their attacks will advance your special attack cooldown too.";
				}
				case 6:
				{
					icon = "icons\god power ragnarok icon 64";
					msg = "Cooldown: 10 | Cannot target another Throne Shield or a player that already has a Vow.";
				}
				case 7:
				{
					icon = "icons\god power thunder clap icons 64";
					msg = "(W) Justice: Stun nearby enemies that are not attacking you. Silence the ones that are.";
				}
				case 8:
				{
					icon = "icons\god power thunder clap icons 64";
					msg = "Cooldown: 10 | Radius: 8 | Stun Duration: 2 | Silence Duration: 6";
				}
				case 9:
				{
					icon = "icons\god power valor icons 64";
					msg = "(E) Shield of Light: Absorb all the damage you receive for a short duration.";
				}
				case 10:
				{
					icon = "icons\god power valor icons 64";
					msg = "Then, your next attack will fire an enormous laser that deals all the absorbed damage.";
				}
				case 11:
				{
					icon = "icons\god power valor icons 64";
					msg = "Cost: 60 | Duration: 6 | Absorbed damage is calculated before your passive reduction";
					next = 0;
				}
			}
		}
		case SAVIOR:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Savior: A high-mobility fighter that heals allies and grants them favor.";
				}
				case 1:
				{
					msg = "Passive: You can jump towards distant foes. When you land, stun your target.";
				}
				case 2:
				{
					msg = "Special Attack (3 attacks): Your spear pierces your target and hits enemies in a line.";
				}
				case 3:
				{
					msg = "Range: 10 | Apply on-hit effects to each enemy hit";
				}
				case 4:
				{
					icon = "icons\improvement heroic renewal icons 64";
					msg = "(Q) Guardian Angel: Teleport next to an ally and then heal nearby allies.";
				}
				case 5:
				{
					icon = "icons\improvement heroic renewal icons 64";
					msg = "You can target an area near yourself to teleport a short distance.";
				}
				case 6:
				{
					icon = "icons\improvement heroic renewal icons 64";
					msg = "Cooldown: 8 | Heal: 50 + 5 x distance traveled";
				}
				case 7:
				{
					icon = "icons\god power cease fire icon 64";
					msg = "(W) Unity: Create an aura at your cursor. Allies in the aura have 0.5x bonus magic resistance.";
				}
				case 8:
				{
					icon = "icons\god power cease fire icon 64";
					msg = "You have 0.5x bonus attack for each ally in the aura.";
				}
				case 8:
				{
					icon = "icons\god power cease fire icon 64";
					msg = "Each time you attack, heal allies in the aura and grant them 2 favor.";
				}
				case 9:
				{
					icon = "icons\god power cease fire icon 64";
					msg = "Cooldown: 20 | Duration: 8 | Radius: 10 | Heal: 0.5x Attack";
				}
				case 10:
				{
					icon = "icons\god power restoration icon 64";
					msg = "(E) Intervention: Revive all dead allies. If you die while this is ready, it auto-activates.";
				}
				case 11:
				{
					icon = "icons\god power restoration icon 64";
					msg = "Cost: 100 | Cooldown: 10";
					next = 0;
				}
			}
		}
		case GARDENER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Gardener: A supportive archer that grows various plants to assist allies.";
				}
				case 1:
				{
					msg = "Passive: Your arrows drop seeds. A damaged player can consume a seed to restore health.";
				}
				case 2:
				{
					msg = "Heal: 30 | Duration: 15";
				}
				case 3:
				{
					icon = "icons\god power audrey icons 64";
					msg = "(Q) Bloodbloom: Summon a carnivorous plant with your stats at the target location.";
				}
				case 4:
				{
					icon = "icons\god power audrey icons 64";
					msg = "Its attacks will poison enemies and leave behind seeds.";
				}
				case 5:
				{
					icon = "icons\god power audrey icons 64";
					msg = "Cooldown: 15 | Decay: 8 percent per second";
				}
				case 6:
				{
					icon = "icons\god power uproot icon";
					msg = "(W) Stranglethorns: Cause seeds near your cursor to sprout into grasping roots that stun enemies.";
				}
				case 7:
				{
					icon = "icons\god power uproot icon";
					msg = "Cooldown: 12 | Range: 6 | Thorn Radius: 3 | Duration: 3 | Damage: 20 per second";
				}
				case 8:
				{
					icon = "icons\world berry bush icon 64";
					msg = "(E) Nature's Bounty: For a short duration, you plant Bery Bushes instead of seeds.";
				}
				case 9:
				{
					icon = "icons\world berry bush icon 64";
					msg = "Berry Bushes restore 4x health. Casting Stranglethorns on Berry Bushes will bring them to life.";
				}
				case 10:
				{
					icon = "icons\world berry bush icon 64";
					msg = "Walking Berry Bushes have half your attack and health.";
				}
				case 11:
				{
					icon = "icons\world berry bush icon 64";
					msg = "Cost: 60 | Duration: 12 | Heal: 120 | Decay: 4 percent per second";
					next = 0;
				}
			}
		}
		case NIGHTRIDER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Nightrider: A sturdy fighter who thrives upon death of enemies and allies.";
				}
				case 1:
				{
					msg = "Passive: Whenever a unit dies near you, restore 5 percent health and gain 1 favor.";
				}
				case 2:
				{
					msg = "Special Attack (8 attacks): Curse your target with a Death Sentence.";
				}
				case 3:
				{
					msg = "This does not trigger if your target already has a Death Sentence.";
				}
				case 4:
				{
					icon = "icons\special g shade icon 64";
					msg = "Death Sentence: For a duration, the unit is poisoned and silenced and it takes double damage";
				}
				case 5:
				{
					icon = "icons\special g shade icon 64";
					msg = "If the cursed unit dies, the curse jumps to a nearby enemy and a Minion is summoned for you.";
				}
				case 6:
				{
					icon = "icons\special g shade icon 64";
					msg = "Duration: 6 | Damage: 6 | Radius: 8 | Minions have a fifth of your attack and health";
				}
				case 7:
				{
					icon = "icons\improvement mythic rejuvenation icons 64";
					msg = "(Q) Abduct: Summon a dark cloud that drags enemies towards you.";
				}
				case 8:
				{
					icon = "icons\improvement mythic rejuvenation icons 64";
					msg = "The first enemy hit is given a Death Sentence.";
				}
				case 9:
				{
					icon = "icons\improvement mythic rejuvenation icons 64";
					msg = "Cooldown: 15 | Range: 12";
				}
				case 10:
				{
					icon = "icons\god power ancestors icon 64";
					msg = "(W) Arise: Summon 3 minions and an additional one for each active Death Sentence.";
				}
				case 11:
				{
					icon = "icons\god power ancestors icon 64";
					msg = "(W) Arise: When you attack an enemy with a Death Sentence, reduce the cooldown of this spell by 1";
				}
				case 12:
				{
					icon = "icons\god power ancestors icon 64";
					msg = "Cooldown: 16 | Decay: 5 percent health";
				}
				case 13:
				{
					icon = "icons\god power meteor icon 64";
					msg = "(E) Nightfall: Teleport to the target location and release a cloud of darkness.";
				}
				case 14:
				{
					icon = "icons\god power meteor icon 64";
					msg = "Inflict a Death Sentence on all enemies hit and stun them for the same duration.";
				}
				case 15:
				{
					icon = "icons\god power meteor icon 64";
					msg = "Cost: 75 | Radius: 15";
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
					msg = "Passive: Your attack deals zero damage. Instead, three stars orbit around you, damaging enemies.";
				}
				case 2:
				{
					msg = "Damage: 50 | Rotation speed + 0.33x projectiles";
				}
				case 3:
				{
					icon = "icons\god power thunder clap icons 64";
					msg = "(Q) Repel: Launch nearby enemies away from you.";
				}
				case 4:
				{
					icon = "icons\god power thunder clap icons 64";
					msg = "Cooldown: 15 | Range: 15";
				}
				case 5:
				{
					icon = "icons\god power vision icon 64";
					msg = "(W) Realignment: Change the radius of your stars to match your cursor.";
				}
				case 6:
				{
					icon = "icons\god power vision icon 64";
					msg = "The closer they are, the faster you move.";
				}
				case 7:
				{
					icon = "icons\god power vision icon 64";
					msg = "Cooldown: 5 | Range: 2 - 15 | Speed: 1.5x - 0.5x";
				}
				case 8:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "(E) Event Horizon: Your stars rotate at double speed and stun enemies.";
				}
				case 9:
				{
					icon = "icons\god power eclipse icon 64";
					msg = "Cost: 60 | Duration: 10";
					next = 0;
				}
			}
		}
		case SPARKWITCH:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Spark Witch: A powerful spellcaster who excels at destroying mobs of enemies.";
				}
				case 1:
				{
					msg = "Passive: Your attack shoots lightning that deals magic damage and bounces between enemies.";
				}
				case 2:
				{
					msg = "Lightning can bounce to yourself if there are no nearby targets (Doesn't deal damage to yourself)";
				}
				case 3:
				{
					msg = "Bounce range increased by Range. Each time you cast a spell, your next attack gains +1 bounce.";
				}
				case 4:
				{
					msg = "Bounce Range: 10 | Damage: 100 | Bounces: Projectiles + Spells Cast";
				}
				case 5:
				{
					icon = "icons\god power bolt icon 64";
					msg = "(Q) Thunderstrike: Lightning deals damage in an area. Cooldown reduced by 1 for each enemy hit";
				}
				case 6:
				{
					icon = "icons\god power bolt icon 64";
					msg = "Cooldown: 12 | Radius: 6 | Damage: 60";
				}
				case 7:
				{
					icon = "icons\special x lampades icons 64";
					msg = "(W) Hexbolt: Your next attack applies a special effect. It changes each time you cast this spell";
				}
				case 8:
				{
					icon = "sfx g arkantos flash";
					msg = "Electrify: Electrocutes your targets, dealing area damage per second.";
				}
				case 9:
				{
					icon = "icons\god power curse icon 64";
					msg = "Polymorph: Transforms your targets into pigs with 0 attack and armor.";
				}
				case 10:
				{
					icon = "icons\special x lampades icons 64";
					msg = "Cooldown: 18 | Duration: 5 | Electrify Damage: 30 per second";
				}
				case 11:
				{
					icon = "sfx x arkantos birth ring";
					msg = "(E) Vortex: Create a storm that pulls in nearby enemies and stuns them.";
				}
				case 12:
				{
					icon = "sfx x arkantos birth ring";
					msg = "Cost: 80 | Range: 12";
					next = 0;
				}
			}
		}
		case GAMBLER:
		{
			switch(tooltip)
			{
				case 0:
				{
					msg = "Gambler: A luck-based caster with a wide variety of spells.";
				}
				case 1:
				{
					msg = "Passive: You cannot attack. Your basic spells have three variations.";
				}
				case 2:
				{
					msg = "After you cast a basic spell, it will randomly switch to one of its other variations.";
				}
				case 3:
				{
					icon = "icons\improvement engineers icon 64";
					msg = "(Q) Loaded Dice: Throw a dice at your cursor with a special effect.";
				}
				case 4:
				{
					icon = "icons\improvement engineers icon 64";
					msg = "Loaded Dice: COPY: Summon a copy of the first enemy hit by the dice. (excluding bosses)";
				}
				case 5:
				{
					icon = "icons\improvement engineers icon 64";
					msg = "Loaded Dice: EXPLODE: The dice explodes and deals damage in an area.";
				}
				case 6:
				{
					icon = "icons\improvement engineers icon 64";
					msg = "Loaded Dice: HEAL: The dice heals allies that it passes through and grants them favor.";
				}
				case 7:
				{
					icon = "icons\improvement engineers icon 64";
					msg = "Cooldown: 6 | Decay: 4 | Radius: 6 | Damage: 100 | Heal: 100 | Favor: 4";
				}
				case 8:
				{
					icon = "icons\improvement architects icon 64";
					msg = "(W) Gimmick Deck: Play a card with a special effect.";
				}
				case 9:
				{
					icon = "icons\improvement architects icon 64";
					msg = "Gimmick Deck: BURN: Create a circle of fire that damages enemies each second.";
				}
				case 10:
				{
					icon = "icons\improvement architects icon 64";
					msg = "Gimmick Deck: BURN: Damage is increased by the number of allies in the circle.";
				}
				case 11:
				{
					icon = "icons\improvement architects icon 64";
					msg = "Gimmick Deck: FREEZE: Stun enemies at the target location.";
				}
				case 12:
				{
					icon = "icons\improvement architects icon 64";
					msg = "Gimmick Deck: RELICS: Give an allied player bonuses from all your relics.";
				}
				case 13:
				{
					icon = "icons\improvement architects icon 64";
					msg = "Cooldown: 12 | Radius: 6 | Duration: 6 | Damage: 20 + 10 per ally";
				}
				case 14:
				{
					icon = "icons\god power plenty icon 64";
					msg = "(E) Gamble: Get a random number from 1-6. Your next spell is multiplied by that amount.";
				}
				case 15:
				{
					icon = "icons\god power plenty icon 64";
					msg = "Cost: 30";
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
			name = "Thunderstepper";
		}
		case FIREKNIGHT:
		{
			name = "Fire Knight";
		}
		case BLASTMAGE:
		{
			name = "Blastmage";
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
		case COMMANDO:
		{
			name = "Commando";
		}
		case THRONESHIELD:
		{
			name = "Throne Shield";
		}
		case SAVIOR:
		{
			name = "Savior";
		}
		case GARDENER:
		{
			name = "Gardener";
		}
		case NIGHTRIDER:
		{
			name = "Nightrider";
		}
		case STARSEER:
		{
			name = "Starseer";
		}
		case SPARKWITCH:
		{
			name = "Spark Witch";
		}
		case GAMBLER:
		{
			name = "Gambler";
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
