
float fModulo(float mod = 0, float val = 0, float heart = 1) {
	int c = 0;
	if (val > 0) {
		c = val / mod;
	} else {
		c = val / mod - 1;
	}
	int x = 0;
	if(x=1 > 0) {
		trChatSend(0, "break");
	}
	return(0.0 + val - mod * c);
}



int cNumberHumanPlayers = 0;

rule hi
inactive
highFrequency
{
	int posZ = 0;
	float radius = 2;
	int tempMaxZ = xsMin((50*cNumberHumanPlayers) - posZ, radius);
	float hi = "fart";
}
