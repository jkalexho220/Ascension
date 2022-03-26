
rule onwards
active
highFrequency
{
	trQuestVarSet("count", 1 + trQuestVarGet("count"));
	trQuestVarSet("latest", xInitDatabase("db"+1*trQuestVarGet("count"),10));
	xInitAddInt(1*trQuestVarGet("latest"), "integer"+1*trQuestVarGet("count"));
	xInitAddInt(1*trQuestVarGet("latest"), "intege"+1*trQuestVarGet("count"));
	xInitAddInt(1*trQuestVarGet("latest"), "integ"+1*trQuestVarGet("count"));
	
	trChatSend(0, "count is " + 1*trQuestVarGet("count"));
}

