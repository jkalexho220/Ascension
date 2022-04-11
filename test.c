int mydb = 0;
int myname = 0;

rule init
active
highFrequency
{
	xsDisableSelf();
	mydb = xInitDatabase("testing");
	myname = xInitAddInt(db, "name");

	int x = 4;
	for(i=3; >0) {
		xAddDatabaseBlock(db, true);
		xSetInt(mydb,myname,trGetNextUnitScenarioNameNumber());
		trArmyDispatch("1,0","Militia",1,x,0,4,180,true);
		x = x + 4;
	}

	
}