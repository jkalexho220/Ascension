const int mInt = 0;
const int mFloat = 1;
const int mString = 2;
const int mVector = 3;
const int mBool = 4;

const int xDirtyBit = 0;
const int xNextBlock = 1;
const int xData = 2;
const int xPrevBlock = 2; // for databases, xData is unused and xPrevBlock takes its place

const int NEXTFREE = 0; // the very first block contains the next free pointer and nothing else

int MALLOC = 0;

/*
NOTE: player context must be 0 when calling this! (use xsSetContextPlayer(0))
*/
bool free(int type = -1, int index = -1) {
	bool success = false;
	if (type >= 0 && type < 5) {
		if (aiPlanGetUserVariableBool(MALLOC, type * 3 + xDirtyBit, index)) {
			aiPlanSetUserVariableInt(MALLOC, type * 3 + xNextBlock, index,
				aiPlanGetUserVariableInt(MALLOC, type * 3 + xNextBlock, NEXTFREE));
			aiPlanSetUserVariableBool(MALLOC, type * 3 + xDirtyBit, index, false);
			aiPlanSetUserVariableInt(MALLOC, type * 3 + xNextBlock, NEXTFREE, index); // set next free to be the newly added block
			success = true;
		}
	}
	return(success);
}

/*
NOTE: player context must be 0 when calling this! (use xsSetContextPlayer(0))
*/
int malloc(int type = -1) {
	if (type < 0 || type > 5) {
		return(-1);
	}
	/*
	get next free User variable
	*/
	int next = aiPlanGetUserVariableInt(MALLOC, type * 3 + xNextBlock, NEXTFREE);
	if (next == 0) {
		/*
		if no free buffers, create a new one
		*/
		next = aiPlanGetNumberUserVariableValues(MALLOC,type * 3 + xNextBlock);
		for(i=type * 3; < type * 3 + 3) {
			aiPlanSetNumberUserVariableValues(MALLOC,i,next + 1, false);
		}
		aiPlanSetUserVariableInt(MALLOC,type * 3 + xNextBlock, next, 0); // next free block is 0 for a newly created block
	} else {
		/*
		if a free buffer exists, we set our nextFree pointer to the next free buffer
		*/
		aiPlanSetUserVariableInt(MALLOC,type * 3 + xNextBlock,NEXTFREE,
			aiPlanGetUserVariableInt(MALLOC,type * 3 + xNextBlock,next));
	}
	aiPlanSetUserVariableBool(MALLOC,type * 3 + xDirtyBit, next, true); // set dirty bit
	switch(type) // initialize data
	{
		case mInt:
		{
			aiPlanSetUserVariableInt(MALLOC, mInt * 3 + xData, next, 0);
		}
		case mFloat:
		{
			aiPlanSetUserVariableFloat(MALLOC, mFloat * 3 + xData, next, 0);
		}
		case mString:
		{
			aiPlanSetUserVariableString(MALLOC, mString * 3 + xData, next, "");
		}
		case mVector:
		{
			aiPlanSetUserVariableVector(MALLOC, mVector * 3 + xData, next, vector(0,0,0));
		}
		case mBool:
		{
			aiPlanSetUserVariableBool(MALLOC, mVector * 2 + xData, next, false);
		}
	}
	
	return(next);
}

bool mGetBool(int index = 0) {
	bool val = false;
	if (aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xDirtyBit, index)) {
		val = aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xData, index);
	}
	return(val);
}

bool mSetBool(int index = 0, bool val = false) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xDirtyBit, index)) {
		success = aiPlanSetUserVariableBool(MALLOC, mBool * 3 + xData, index, val);
	}
	return(success);
}

string mGetString(int index = 0) {
	string val = "";
	if (aiPlanGetUserVariableBool(MALLOC, mString * 3 + xDirtyBit, index)) {
		val = aiPlanGetUserVariableString(MALLOC, mString * 3 + xData, index);
	}
	return(val);
}

bool mSetString(int index = 0, string val = "") {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mString * 3 + xDirtyBit, index)) {
		success = aiPlanSetUserVariableString(MALLOC, mString * 3 + xData, index, val);
	}
	return(success);
}

int mGetInt(int index = 0) {
	int val = -1;
	if (aiPlanGetUserVariableBool(MALLOC, mInt * 3 + xDirtyBit, index)) {
		val = aiPlanGetUserVariableInt(MALLOC, mInt * 3 + xData, index);
	}
	return(val);
}

bool mSetInt(int index = 0, int val = 0) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mInt * 3 + xDirtyBit, index)) {
		success = aiPlanSetUserVariableInt(MALLOC, mInt * 3 + xData, index, val);
	}
	return(success);
}

float mGetFloat(int index = 0) {
	float val = -1;
	if (aiPlanGetUserVariableBool(MALLOC, mFloat * 3 + xDirtyBit, index)) {
		val = aiPlanGetUserVariableFloat(MALLOC, mFloat * 3 + xData, index);
	}
	return(val);
}

bool mSetFloat(int index = 0, float val = 0) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mFloat * 3 + xDirtyBit, index)) {
		success = aiPlanSetUserVariableFloat(MALLOC, mFloat * 3 + xData, index, val);
	}
	return(success);
}

vector mGetVector(int index = 0) {
	vector val = vector(-1,-1,-1);
	if (aiPlanGetUserVariableBool(MALLOC, mVector * 3 + xDirtyBit, index)) {
		val = aiPlanGetUserVariableVector(MALLOC, mVector * 3 + xData, index);
	}
	return(val);
}

bool mSetVector(int index = 0, vector val = vector(0,0,0)) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mVector * 3 + xDirtyBit, index)) {
		success = aiPlanSetUserVariableVector(MALLOC, mVector * 3 + xData, index, val);
	}
	return(success);
}

rule mInitializeDatabase
active
highFrequency
{
	xsDisableSelf();
	aiSet("NoAI", 0);
	xsSetContextPlayer(0);
	MALLOC = aiPlanCreate("memory",0);
	for(i=0; < 5) {
		aiPlanAddUserVariableBool(MALLOC,i * 3 + xDirtyBit,"DirtyBit"+i,1);
		aiPlanAddUserVariableInt(MALLOC,i * 3 + xNextBlock,"NextBlock"+i,1);
		aiPlanSetUserVariableBool(MALLOC,i * 3 + xDirtyBit, NEXTFREE, true);
		aiPlanSetUserVariableInt(MALLOC,i * 3 + xNextBlock, NEXTFREE, 0);
	}
	aiPlanAddUserVariableInt(MALLOC,mInt * 3 + xData, "intData",1);
	aiPlanAddUserVariableFloat(MALLOC,mFloat * 3 + xData, "floatData",1);
	aiPlanAddUserVariableString(MALLOC,mString * 3 + xData, "stringData",1);
	aiPlanAddUserVariableVector(MALLOC,mVector * 3 + xData, "vectorData",1);
	aiPlanAddUserVariableBool(MALLOC,mBool * 3 + xData, "boolData",1);
}
