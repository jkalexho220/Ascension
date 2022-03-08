/* greek boons */
const int BOON_DOUBLE_FAVOR = 1; // zeus
const int BOON_POISON_STACKS = 2; // poseidon
const int BOON_SPELL_ATTACK = 3; // hades

/* egyptian boons */
const int BOON_SPELL_POISON = 4; // isis
const int BOON_MORE_GOLD = 5; // ra
const int BOON_REGENERATE_HEALTH = 6; // set

/* norse boons */
const int BOON_FROSTY_SPELLS = 7; // odin
const int BOON_DOUBLE_STUN = 8; // thor
const int BOON_DECAY_HALVED = 9; // loki

/* atlantean boons */
const int BOON_EXCESS_FAVOR = 10; // kronos
const int BOON_TWO_RELICS = 11; // oranos
const int BOON_MONSTER_COMPANION = 12; // gaia

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
	string name = "WTF that's not a boon!";
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
		case BOON_DOUBLE_STUN:
		{
			name = "Double the duration of your stuns and silences.";
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
		case BOON_POISON_STACKS:
		{
			name = "Your poison damage stacks.";
		}
		case BOON_SPELL_ATTACK:
		{
			name = "Your attack is multiplied by your spell power.";
		}
		case BOON_FROSTY_SPELLS:
		{
			name = "Your spell damage will stun enemies at full health.";
		}
		case BOON_EXCESS_FAVOR:
		{
			name = "When your favor exceeds your Ultimate cost, your basic spells cost 10 favor and have no cooldown.";
		}
		case BOON_TWO_RELICS:
		{
			name = "Relic Slots +2";
		}
		case BOON_MONSTER_COMPANION:
		{
			name = "Choose a monster from the Monsterpedia to be your pet. Revive it whenever you use your Ultimate.";
		}
	}
	return(name);
}
