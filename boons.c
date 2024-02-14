/* greek boons */
const int BOON_DOUBLE_FAVOR = 1; // zeus
const int BOON_MORE_GOLD = 2; // poseidon
const int BOON_SPELL_ATTACK = 3; // hades

/* egyptian boons */
const int BOON_SPELL_POISON = 4; // isis
const int BOON_STATUS_COOLDOWNS = 5; // ra
const int BOON_HEALTH_ATTACK = 6; // set

/* norse boons */
const int BOON_MONSTER_COMPANION = 7; // odin
const int BOON_REGENERATE_HEALTH = 8; // thor
const int BOON_DECAY_HALVED = 9; // loki

/* atlantean boons */
const int BOON_ATTACK_PROLONGS_STUN = 10; // kronos
const int BOON_TWO_RELICS = 11; // oranos
const int BOON_HEAL_FAVOR = 12; // gaia

const int SYMPHONY_DOUBLE_SPECIAL = 0;
const int SYMPHONY_FAVOR_CLONE = 1;
const int SYMPHONY_LASER_ATTACKS = 2;
const int SYMPHONY_STUN_HEAL = 3;
const int SYMPHONY_HEAL_DEAL = 4;
const int SYMPHONY_MORE_PHYSICAL = 5;
const int SYMPHONY_MORE_MAGICAL = 6;
const int SYMPHONY_STATUS_DAMAGE = 7;
const int SYMPHONY_SHARE_STATUS = 8;
const int SYMPHONY_REGENERATE_FAVOR = 9;
const int SYMPHONY_GAIN_LEVELS = 10;
const int SYMPHONY_GAIN_ATTACK = 11;
const int SYMPHONY_GAIN_POWER = 12;
const int SYMPHONY_EXPLODING_MINIONS = 13;
const int SYMPHONY_MAGIC_SHRED = 14;
const int SYMPHONY_POISON_STACKS = 15;

int statueCiv(int boon = 0) {
	return(boon - 1);
}

int statueCulture(int boon = 0) {
	return(statueCiv(boon) / 3);
}

void overrideStatue(int boon = 0) {
	int g = 0;
	switch(boon)
	{
		case 1:
		{
			g = 5;
		}
		case 2:
		{
			g = 4;
		}
		case 3:
		{
			g = 3;
		}
		case 4:
		{
			g = 0;
		}
		case 5:
		{
			g = 1;
		}
		case 6:
		{
			g = 2;
		}
		case 7:
		{
			g = 7;
		}
		case 8:
		{
			g = 8;
		}
		case 9:
		{
			g = 6;
		}
		case 10:
		{
			g = 9;
		}
		case 11:
		{
			g = 11;
		}
		case 12:
		{
			g = 10;
		}
	}
	trUnitSetAnimationPath(""+g+",0,0,0,0,0,0");
}

string boonIcon(int boon = 0) {
	string icon = "icons\infantry g hoplite icon 64";
	switch(boon)
	{
		case 1:
		{
			icon = "icons\god major zeus icon 128";
		}
		case 2:
		{
			icon = "icons\god major poseidon icon 128";
		}
		case 3:
		{
			icon = "icons\god major hades icon 128";
		}
		case 4:
		{
			icon = "icons\god major isis icon 128";
		}
		case 5:
		{
			icon = "icons\god major ra icon 128";
		}
		case 6:
		{
			icon = "icons\god major set icon 128";
		}
		case 7:
		{
			icon = "icons\god major odin icon 128";
		}
		case 8:
		{
			icon = "icons\god major thor icon 128";
		}
		case 9:
		{
			icon = "icons\god major loki icon 128";
		}
		case 10:
		{
			icon = "icons\god major kronos icons 64";
		}
		case 11:
		{
			icon = "icons\god major ouranos icons 64";
		}
		case 12:
		{
			icon = "icons\god major gaia icons 64";
		}
	}
	return(icon);
}

string boonName(int boon = 0) {
	string name = "(This blessing has not been implemented yet)";
	switch(boon)
	{
		case BOON_REGENERATE_HEALTH:
		{
			name = "Regenerate 3 percent health per second.";
		}
		case BOON_MORE_GOLD:
		{
			name = "Gain double gold from enemy kills.";
		}
		case BOON_DECAY_HALVED:
		{
			name = "The decay rate of your summoned allies is halved.";
		}
		case BOON_SPELL_POISON:
		{
			name = "Your damage spells will poison enemies.";
		}
		case BOON_DOUBLE_FAVOR:
		{
			name = "Your maximum favor is doubled.";
		}
		case BOON_SPELL_ATTACK:
		{
			name = "Your attack is multiplied by your spell power.";
		}
		case BOON_TWO_RELICS:
		{
			name = "Relic Slots +2";
		}
		case BOON_MONSTER_COMPANION:
		{
			name = "Choose a monster from the Monsterpedia to be your pet companion. (Cannot choose bosses)";
		}
		case BOON_STATUS_COOLDOWNS:
		{
			name = "When you inflict a status effect, advance your spell cooldowns by 1 second.";
		}
		case BOON_HEAL_FAVOR:
		{
			name = "Each time you heal an ally, gain 1 favor. (Maximum of 3 per second)";
		}
		case BOON_HEALTH_ATTACK:
		{
			name = "Add 3 percent of your health to your attack.";
		}
		case BOON_ATTACK_PROLONGS_STUN:
		{
			name = "When you attack an enemy with a status effect, increase its duration by 1 second.";
		}
	}
	return(name);
}

string symphonyName(int boon = 0) {
	string name = "(This Symphony has not been implemented yet)";
	switch(boon)
	{
		case SYMPHONY_DOUBLE_SPECIAL:
		{
			name = "Your special attack triggers twice.";
		}
		case SYMPHONY_FAVOR_CLONE:
		{
			name = "Each time you spend 50 favor, summon a decaying copy of yourself.";
		}
		case SYMPHONY_LASER_ATTACKS:
		{
			name = "Your attacks fire lasers that deal 100 magic damage in a line.";
		}
		case SYMPHONY_STUN_HEAL:
		{
			name = "Each time you stun an enemy, restore 10 percent of your health.";
		}
		case SYMPHONY_HEAL_DEAL:
		{
			name = "Whenever you heal, deal the same amount of damage to enemies within 5 meters.";
		}
		case SYMPHONY_MORE_PHYSICAL:
		{
			name = "Increase health, attack, and armor by 20 percent of current value.";
		}
		case SYMPHONY_MORE_MAGICAL:
		{
			name = "Increase spell power, duration, and range by 20 percent of current value.";
		}
		case SYMPHONY_STATUS_DAMAGE:
		{
			name = "Whenever you apply a status effect, the target loses 1 percent health.";
		}
		case SYMPHONY_SHARE_STATUS:
		{
			name = "Whenever you are inflicted by a status effect, inflict it on nearby enemies.";
		}
		case SYMPHONY_REGENERATE_FAVOR:
		{
			name = "Regenerate 1 favor per second.";
		}
		case SYMPHONY_GAIN_LEVELS:
		{
			name = "Gain 3 levels.";
		}
		case SYMPHONY_GAIN_ATTACK:
		{
			name = "+50 Attack";
		}
		case SYMPHONY_GAIN_POWER:
		{
			name = "+1.0 Spell Power";
		}
		case SYMPHONY_EXPLODING_MINIONS:
		{
			name = "When your summoned allies die, they explode and deal 300 damage.";
		}
		case SYMPHONY_MAGIC_SHRED:
		{
			name = "When you deal magic damage to an enemy, reduce their magic resist by 1.";
		}
		case SYMPHONY_POISON_STACKS:
		{
			name = "When you attack a poisoned enemy, increase the poison damage by 10.";
		}
	}
	return(name);
}

bool playerHasSymphony(int p = 0, int symphony = 0) {
	for(i=trQuestVarGet("symphonyUpgradeNumber"); > 0) {
		if (1*trQuestVarGet("p"+p+"upgrade"+i) == symphony) {
			return(true);
		}
	}
	return(false);
}