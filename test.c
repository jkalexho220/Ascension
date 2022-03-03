const int DIMENSION_X = 290;
const int DIMENSION_Z = 290;
const float PI = 3.141592;


void trVectorQuestVarSet(string VQVname = "", vector QVv = vector(-1,-1,-1)) {
	if (VQVname == "") return;
	trQuestVarSet(""+VQVname+"X", xsVectorGetX(QVv));
	trQuestVarSet(""+VQVname+"Y", xsVectorGetY(QVv));
	trQuestVarSet(""+VQVname+"Z", xsVectorGetZ(QVv));
}

void debugLog(string msg = "") {
	if (trCurrentPlayer() == 1) {
		trChatSend(0, "<color=1,0,0>" + msg);
	}
}
