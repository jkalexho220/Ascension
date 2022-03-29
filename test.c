
rule onwards
active
highFrequency
{
	
	%
	for(p=1; < cNumberNonGaiaPlayers) {
		code("trQuestVarCopy(\"p"+p+"name\", \""+rmGetPlayerName(p)+"\");");
	}
	code("map(\"esc\", \"ShowImageBox\", \"gadgetUnreal("+quote+"ShowImageBox"+quote+")\");");
	%
}

