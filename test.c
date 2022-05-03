int mydb = 0;
int myname = 0;

rule init
active
highFrequency
{
	xsDisableSelf();
	mydb = xInitDatabase("testing");
	myname = xInitAddInt(mydb, "name");

	int x = 4;
	for(i=0; < 10) {
		xAddDatabaseBlock(mydb, true);
		xSetInt(mydb,myname,trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Militia",1,x,0,4,180,true);
		trArmySelect("1,0");
		trDamageUnit(i);
		x = x + 4;
	}

	
}

rule highlight
active
highFrequency
{
	xDatabaseNext(mydb);
	xUnitSelect(mydb, myname);
	trUnitHighlight(0.1, true);
}


rule _Trigger_273
highFrequency
active
{
	if (trTime() > trQuestVarGet("next")) {
		trQuestVarSetFromRand("rand", 1, 10);
		trQuestVarSet("next", trTime());
		vector pos = kbGetBlockPosition(""+xGetInt(mydb,myname,1*trQuestVarGet("rand"))) + vector(0,0,2);
		trArmyDispatch("1,0","Conversion SFX",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		if (aiPlanGetUserVariableBool(mydb,xDirtyBit,1*trQuestVarGet("rand"))) {
			xDetachDatabaseBlock(mydb, 1*trQuestVarGet("rand"));
			trChatSend(0, "Detach " + 1*trQuestVarGet("rand"));
		} else {
			xRestoreDatabaseBlock(mydb, 1*trQuestVarGet("rand"));
			trChatSend(0, "<color=1,1,1>Attach " + 1*trQuestVarGet("rand"));
		}
	} 
}