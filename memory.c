const int mInt = 0;
const int mFloat = 1;
const int mString = 2;
const int mVector = 3;
const int mBool = 4;

const int xMetadata = 0; // contains current pointer and database size and numVariables. However, malloc doesn't have metadata so they have to shift down 1
const int xDirtyBit = 1;
const int xNextBlock = 2;
const int xPrevBlock = 3; // for databases, xData is unused and xPrevBlock takes its place
const int xData = 3;

/*
Metadata information
*/
const int mPointer = 0;
const int mCount = 1;
const int mNumVariables = 2;
const int mNextFree = 3;
const int mVariableTypes = 3;
/*
subsequent items in the metadata will determine the datatypes of extra variables for the database
*/

const int NEXTFREE = 0; // the very first block contains the next free pointer and nothing else

int MALLOC = 0;

/*
NOTE: player context must be 0 when calling this! (use xsSetContextPlayer(0))
*/
bool free(int type = -1, int index = -1) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, type * 3 + xDirtyBit - 1, index)) {
		aiPlanSetUserVariableInt(MALLOC, type * 3 + xNextBlock - 1, index,
			aiPlanGetUserVariableInt(MALLOC, type * 3 + xNextBlock - 1, NEXTFREE));
		aiPlanSetUserVariableBool(MALLOC, type * 3 + xDirtyBit - 1, index, false);
		aiPlanSetUserVariableInt(MALLOC, type * 3 + xNextBlock - 1, NEXTFREE, index); // set next free to be the newly added block
		success = true;
	}
	return(success);
}

/*
NOTE: player context must be 0 when calling this! (use xsSetContextPlayer(0))
*/
int malloc(int type = -1) {
	/*
	get next free User variable
	*/
	int next = aiPlanGetUserVariableInt(MALLOC, type * 3 + xNextBlock - 1, NEXTFREE);
	if (next == 0) {
		/*
		if no free buffers, create a new one
		*/
		next = aiPlanGetNumberUserVariableValues(MALLOC,type * 3 + xNextBlock - 1);
		for(i=type * 3; < type * 3 + 3) {
			aiPlanSetNumberUserVariableValues(MALLOC,i,next + 1, false); // resizing seems to be a little expensive?
		}
		aiPlanSetUserVariableInt(MALLOC,type * 3 + xNextBlock - 1, next, 0); // next free block is 0 for a newly created block
	} else {
		/*
		if a free buffer exists, we set our nextFree pointer to the next free buffer
		*/
		aiPlanSetUserVariableInt(MALLOC,type * 3 + xNextBlock - 1,NEXTFREE,
			aiPlanGetUserVariableInt(MALLOC,type * 3 + xNextBlock - 1,next));
	}
	aiPlanSetUserVariableBool(MALLOC,type * 3 + xDirtyBit - 1, next, true); // set dirty bit
	
	return(next);
}

bool mGetBool(int index = 0) {
	bool val = false;
	if (aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xDirtyBit - 1, index)) {
		val = aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xData - 1, index);
	}
	return(val);
}

bool mSetBool(int index = 0, bool val = false) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mBool * 3 + xDirtyBit - 1, index)) {
		success = aiPlanSetUserVariableBool(MALLOC, mBool * 3 + xData - 1, index, val);
	}
	return(success);
}

int mNewBool(bool val = false) {
	int index = malloc(mBool);
	mSetBool(index, val);
	return(index);
}

bool mFreeBool(int index = 0) {
	return(free(mBool, index));
}


string mGetString(int index = 0) {
	string val = "";
	if (aiPlanGetUserVariableBool(MALLOC, mString * 3 + xDirtyBit - 1, index)) {
		val = aiPlanGetUserVariableString(MALLOC, mString * 3 + xData - 1, index);
	}
	return(val);
}

bool mSetString(int index = 0, string val = "") {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mString * 3 + xDirtyBit - 1, index)) {
		success = aiPlanSetUserVariableString(MALLOC, mString * 3 + xData - 1, index, val);
	}
	return(success);
}

int mNewString(string val = "") {
	int index = malloc(mString);
	mSetString(index, val);
	return(index);
}

bool mFreeString(int index = 0) {
	return(free(mString, index));
}

int mGetInt(int index = 0) {
	int val = -1;
	if (aiPlanGetUserVariableBool(MALLOC, mInt * 3 + xDirtyBit - 1, index)) {
		val = aiPlanGetUserVariableInt(MALLOC, mInt * 3 + xData - 1, index);
	}
	return(val);
}

bool mSetInt(int index = 0, int val = 0) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mInt * 3 + xDirtyBit - 1, index)) {
		success = aiPlanSetUserVariableInt(MALLOC, mInt * 3 + xData - 1, index, val);
	}
	return(success);
}

int mNewInt(int val = 0) {
	int index = malloc(mInt);
	mSetInt(index, val);
	return(index);
}

bool mFreeInt(int index = 0) {
	return(free(mInt, index));
}

float mGetFloat(int index = 0) {
	float val = -1;
	if (aiPlanGetUserVariableBool(MALLOC, mFloat * 3 + xDirtyBit - 1, index)) {
		val = aiPlanGetUserVariableFloat(MALLOC, mFloat * 3 + xData - 1, index);
	}
	return(val);
}

bool mSetFloat(int index = 0, float val = 0) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mFloat * 3 + xDirtyBit - 1, index)) {
		success = aiPlanSetUserVariableFloat(MALLOC, mFloat * 3 + xData - 1, index, val);
	}
	return(success);
}

int mNewFloat(float val = 0) {
	int index = malloc(mFloat);
	mSetFloat(index, val);
	return(index);
}

bool mFreeFloat(int index = 0) {
	return(free(mFloat, index));
}

vector mGetVector(int index = 0) {
	vector val = vector(-1,-1,-1);
	if (aiPlanGetUserVariableBool(MALLOC, mVector * 3 + xDirtyBit - 1, index)) {
		val = aiPlanGetUserVariableVector(MALLOC, mVector * 3 + xData - 1, index);
	}
	return(val);
}

bool mSetVector(int index = 0, vector val = vector(0,0,0)) {
	bool success = false;
	if (aiPlanGetUserVariableBool(MALLOC, mVector * 3 + xDirtyBit - 1, index)) {
		success = aiPlanSetUserVariableVector(MALLOC, mVector * 3 + xData - 1, index, val);
	}
	return(success);
}

int mNewVector(vector val = vector(0,0,0)) {
	int index = malloc(mVector);
	mSetVector(index, val);
	return(index);
}

bool mFreeVector(int index = 0) {
	return(free(mVector, index));
}

bool freeAllMemory() {
	return(aiPlanRemoveUserVariables(MALLOC));
}

/*
Size is the starting size of the database, but databases can grow indefinitely
returns the identifier of the database. Use this identifier in other xDatabase triggers
*/
int xInitDatabase(string name = "", int size = 0) {
	xsSetContextPlayer(0);
	int id = aiPlanCreate(name, 0);
	aiPlanAddUserVariableBool(id,xDirtyBit,"DirtyBit",size+1);
	aiPlanAddUserVariableInt(id,xNextBlock,"NextBlock",size+1);
	aiPlanAddUserVariableInt(id,xPrevBlock,"PrevBlock",size+1);
	aiPlanAddUserVariableInt(id,xMetadata,"Metadata",4);
	aiPlanSetUserVariableInt(id,xMetadata,mPointer,0);
	aiPlanSetUserVariableInt(id,xMetadata,mCount,0);
	aiPlanSetUserVariableInt(id,xMetadata,mNumVariables,0);
	
	aiPlanSetUserVariableInt(id,xMetadata,mNextFree,size);
	aiPlanSetUserVariableInt(id,xNextBlock,0,0);
	for(i=1; <= size) { // connect all the free buffers together
		aiPlanSetUserVariableBool(id,xDirtyBit,i,false);
		aiPlanSetUserVariableInt(id,xNextBlock,i,i-1);
	}
}

/*
returns the index of the newly added variable
*/
int xInitAddVar(int id = 0, string name = "", int type = 0) {
	int count = aiPlanGetNumberUserVariableValues(id,xDirtyBit);
	/*
	first, add the type to our list of types in this struct
	*/
	int index = aiPlanGetNumberUserVariableValues(id,xMetadata);
	aiPlanSetNumberUserVariableValues(id,xMetadata,index + 1,false);
	aiPlanSetUserVariableInt(id,xMetadata,index,type);
	
	/*
	next, add a new array of the specified datatype to hold values
	*/
	aiPlanSetUserVariableInt(id,xMetadata,mNumVariables, 1 + aiPlanGetUserVariableInt(id,xMetadata,mNumVariables));
	index = aiPlanGetUserVariableInt(id,xMetadata,mNumVariables) + xData;
	switch(type)
	{
		case mInt:
		{
			aiPlanAddUserVariableInt(id,index,name,count);
		}
		case mFloat:
		{
			aiPlanAddUserVariableFloat(id,index,name,count);
		}
		case mString:
		{
			aiPlanAddUserVariableString(id,index,name,count);
		}
		case mVector:
		{
			aiPlanAddUserVariableVector(id,index,name,count);
		}
		case mBool:
		{
			aiPlanAddUserVariableBool(id,index,name,count);
		}
	}
	return(index);
}

/*
id is the integer name of the database
name is unused but required
defVal = default value for newly created database entries
*/
int xInitAddInt(int id = 0, string name = "", int defVal = 0) {
	int index = xInitAddVar(id,name,mInt);
	aiPlanSetUserVariableInt(id, index, 0, defVal);
	return(index);
}

int xInitAddFloat(int id = 0, string name = "", float defVal = 0) {
	int index = xInitAddVar(id, name, mFloat);
	aiPlanSetUserVariableFloat(id, index, 0, defVal);
	return(index);
}

int xInitAddString(int id = 0, string name = "", string defVal = "") {
	int index = xInitAddVar(id, name, mString);
	aiPlanSetUserVariableString(id, index, 0, defVal);
	return(index);
}

int xInitAddVector(int id = 0, string name = "", vector defVal = vector(0,0,0)) {
	int index = xInitAddVar(id, name, mVector);
	aiPlanSetUserVariableVector(id, index, 0, defVal);
	return(index);
}

int xInitAddBool(int id = 0, string name = "", bool defVal = false) {
	int index = xInitAddVar(id,name,mBool);
	aiPlanSetUserVariableBool(id, index, 0, defVal);
	return(index);
}

int xAddDatabaseBlock(int id = 0) {
	int next = aiPlanGetUserVariableInt(id,xMetadata,mNextFree);
	if (next == 0) {
		/*
		if no available buffers, we extend the total sizes of the arrays
		*/
		next = aiPlanGetNumberUserVariableValues(id,xDirtyBit);
		for(i=aiPlanGetUserVariableInt(id,xMetadata,mNumVariables) + xData; > xMetadata) {
			aiPlanSetNumberUserVariableValues(id,i,next+1,false);
		}
	} else {
		/*
		if a free buffer is available, we grab it and set next free to be the next of the buffer we grabbed
		*/
		aiPlanSetUserVariableInt(id,xMetadata,mNextFree,aiPlanGetUserVariableInt(id,xNextBlock,next));
	}
	aiPlanSetUserVariableBool(id,xDirtyBit,next,true);
	
	if (aiPlanGetUserVariableInt(id,xMetadata,mCount) == 0) {
		/*
		If it's the only thing in the db, point it to itself and also set the database pointer to the new thing
		*/
		aiPlanSetUserVariableInt(id,xNextBlock,next,next);
		aiPlanSetUserVariableInt(id,xPrevBlock,next,next);
		aiPlanSetUserVariableInt(id,xMetadata,mPointer,next);
	} else {
		/*
		otherwise, slide in between two links in the list at mPointer
		*/
		int before = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
		int after = aiPlanGetUserVariableInt(id,xNextBlock,before);
		
		aiPlanSetUserVariableInt(id,xNextBlock,next,after); // next of me is after
		aiPlanSetUserVariableInt(id,xPrevBlock,next,before); // prev of me is before
		aiPlanSetUserVariableInt(id,xNextBlock,before,next); // next of before is me
		aiPlanSetUserVariableInt(id,xPrevBlock,after,next); // prev of after is me
	}
	aiPlanSetUserVariableInt(id,xMetadata,mCount, 1 + aiPlanGetUserVariableInt(id,xMetadata,mCount));
	/*
	finally, initialize all the variables of the struct to their default values (whatever's in index 0 of the array)
	*/
	for(i = 1; <= aiPlanGetUserVariableInt(id,xMetadata,mNumVariables)) {
		switch(aiPlanGetUserVariableInt(id,xMetadata,mVariableTypes + i))
		{
			case mInt:
			{
				aiPlanSetUserVariableInt(id,xData + i,next,aiPlanGetUserVariableInt(id,xData + i,0));
			}
			case mFloat:
			{
				aiPlanSetUserVariableFloat(id,xData + i,next,aiPlanGetUserVariableFloat(id,xData + i,0));
			}
			case mString:
			{
				aiPlanSetUserVariableString(id,xData + i,next,aiPlanGetUserVariableString(id,xData + i,0));
			}
			case mVector:
			{
				aiPlanSetUserVariableVector(id,xData + i,next,aiPlanGetUserVariableVector(id,xData + i,0));
			}
			case mBool:
			{
				aiPlanSetUserVariableBool(id,xData + i,next,aiPlanGetUserVariableBool(id,xData + i,0));
			}
		}
	}
	return(next);
}


bool xFreeDatabaseBlock(int id = 0, int index = -1) {
	bool success = false;
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	if (aiPlanGetUserVariableBool(id,xDirtyBit,index)) {
		/* connect next with prev */
		int after = aiPlanGetUserVariableInt(id,xNextBlock,index);
		int before = aiPlanGetUserVariableInt(id,xPrevBlock,index);
		aiPlanSetUserVariableInt(id,xNextBlock,before,after); // next block of before is after
		aiPlanSetUserVariableInt(id,xPrevBlock,after,before); // prev block of after is before
		
		/* add myself to the top of the free stack */
		aiPlanSetUserVariableInt(id,xNextBlock,index,aiPlanGetUserVariableInt(id,xMetadata,mNextFree));
		aiPlanSetUserVariableInt(id,xMetadata,mNextFree,index);
		aiPlanSetUserVariableBool(id,xDirtyBit,index,false);
		
		/* set mPointer to my previous block and decrement count */
		if (index == aiPlanGetUserVariableInt(id,xMetadata,mPointer)) {
			aiPlanSetUserVariableInt(id,xMetadata,mPointer,aiPlanGetUserVariableInt(id,xPrevBlock,index));
		}
		aiPlanSetUserVariableInt(id,xMetadata,mCount, aiPlanGetUserVariableInt(id,xMetadata,mCount) - 1);
		success = true;
	}
	return(success);
}

int xDatabaseNext(int id = 0) {
	int pointer = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	pointer = aiPlanGetUserVariableInt(id,xNextBlock,pointer);
	if (aiPlanGetUserVariableBool(id,xDirtyBit,pointer)) {
		aiPlanSetUserVariableInt(id,xMetadata,mPointer,pointer);
	} else {
		pointer = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(pointer);
}

void xClearDatabase(int id = 0) {
	int next = aiPlanGetUserVariableInt(id,xMetadata,mNextFree);
	int pointer = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	aiPlanSetUserVariableInt(id,xMetadata,mNextFree,aiPlanGetUserVariableInt(id,xNextBlock,pointer));
	aiPlanSetUserVariableInt(id,xNextBlock,pointer,next);
	
	aiPlanSetUserVariableInt(id,xMetadata,mCount,0);
	aiPlanSetUserVariableInt(id,xMetadata,mPointer,0);
}

int xGetInt(int id = 0, int data = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mInt) {
		return(-1); // if we are trying to get an int from the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanGetUserVariableInt(id,data,index));
}

bool xSetInt(int id = 0, int data = 0, int val = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mInt) {
		return(false); // if we are trying to set the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanSetUserVariableInt(id,data,index,val));
}


float xGetFloat(int id = 0, int data = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mFloat) {
		return(-1.0); // if we are trying to get an int from the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanGetUserVariableFloat(id,data,index));
}

bool xSetFloat(int id = 0, int data = 0, float val = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mFloat) {
		return(false); // if we are trying to set the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanSetUserVariableFloat(id,data,index,val));
}


string xGetString(int id = 0, int data = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mString) {
		return(""); // if we are trying to get an int from the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanGetUserVariableString(id,data,index));
}

bool xSetString(int id = 0, int data = 0, string val = "", int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mString) {
		return(false); // if we are trying to set the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanSetUserVariableString(id,data,index,val));
}


vector xGetVector(int id = 0, int data = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mVector) {
		return(vector(0,0,0)); // if we are trying to get an int from the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanGetUserVariableVector(id,data,index));
}

bool xSetVector(int id = 0, int data = 0, vector val = vector(0,0,0), int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mVector) {
		return(false); // if we are trying to set the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanSetUserVariableVector(id,data,index,val));
}


bool xGetBool(int id = 0, int data = 0, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mBool) {
		return(false); // if we are trying to get an int from the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanGetUserVariableBool(id,data,index));
}

bool xSetBool(int id = 0, int data = 0, bool val = false, int index = -1) {
	if (aiPlanGetUserVariableInt(id,xMetadata,data - xData + mNumVariables) != mBool) {
		return(false); // if we are trying to set the wrong datatype, stop
	}
	if (index == -1) {
		index = aiPlanGetUserVariableInt(id,xMetadata,mPointer);
	}
	return(aiPlanSetUserVariableBool(id,data,index,val));
}

bool xSetPointer(int id = 0, int index = 0) {
	bool success = false;
	if (aiPlanGetUserVariableBool(id,xDirtyBit,index)) {
		aiPlanSetUserVariableInt(id,xMetadata,mPointer,index);
		success = true;
	}
	return(success);
}

int xGetDatabaseCount(int id = 0) {
	return(aiPlanGetUserVariableInt(id,xMetadata,mCount));
}

int xGetPointer(int id = 0) {
	return(aiPlanGetUserVariableInt(id,xMetadata,mPointer));
}

rule mInitializeMemory
active
highFrequency
{
	xsDisableSelf();
	aiSet("NoAI", 0);
	xsSetContextPlayer(0);
	MALLOC = aiPlanCreate("memory",0);
	for(i=0; < 5) {
		aiPlanAddUserVariableBool(MALLOC,i * 3 + xDirtyBit - 1,"DirtyBit"+i,1);
		aiPlanAddUserVariableInt(MALLOC,i * 3 + xNextBlock - 1,"NextBlock"+i,1);
		aiPlanSetUserVariableBool(MALLOC,i * 3 + xDirtyBit - 1, NEXTFREE, true);
		aiPlanSetUserVariableInt(MALLOC,i * 3 + xNextBlock - 1, NEXTFREE, 0);
	}
	aiPlanAddUserVariableInt(MALLOC,mInt * 3 + xData - 1, "intData",1);
	aiPlanAddUserVariableFloat(MALLOC,mFloat * 3 + xData - 1, "floatData",1);
	aiPlanAddUserVariableString(MALLOC,mString * 3 + xData - 1, "stringData",1);
	aiPlanAddUserVariableVector(MALLOC,mVector * 3 + xData - 1, "vectorData",1);
	aiPlanAddUserVariableBool(MALLOC,mBool * 3 + xData - 1, "boolData",1);
}
