const int BOON_REGENERATE_HEALTH = 1;
const int BOON_MORE_GOLD = 2;
const int BOON_IMMUNE_TO_SILENCE = 3;
const int BOON_DECAY_HALVED = 4;
const int BOON_SPELL_POISON = 5;
const int BOON_REGENERATE_FAVOR = 6;

const int BOON_SPELL_ATTACK = 8;
const int BOON_EXCESS_FAVOR = 9;
const int BOON_TWO_RELICS = 11;

void overrideGodStatue(int boon = 0) {
	switch(boon)
	{
		case BOON_REGENERATE_HEALTH:
		{
			trUnitSetAnimationPath("");
		}
	}
}

string boonIcon(int boon = 0) {
	string icon = "icons\infantry g hoplite icon 64";
	return(icon);
}

string boonName(int boon = 0) {
	string name = "WTF that's not a boon!";
	return(name);
}