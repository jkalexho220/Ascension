import os
import sys

FILE_1 = None
FILE_2 = 'Ascension MMORPG.xs'
NAME = None
named = False
comment = False


def checkQuestVarSet(templine, ln):
	tokens = templine.replace('(', ' ( ').replace(')', ' ) ').replace(',', ' , ').split(' ')
	findComma = False
	foundComma = False
	depth = 0
	for x in tokens:
		if (x == 'trQuestVarSet'):
			findComma = True
		if findComma:
			if (depth == 1):
				if x == ',':
					if not foundComma:
						foundComma = True
					else:
						print("trQuestVarSet instead of trQuestVarSetFromRand")
						print("Line " + str(ln) + ":\n    " + line)
				elif x == ')' and not foundComma:
					print("trQuestVarSet instead of trQuestVarGet")
					print("Line " + str(ln) + ":\n    " + line)
			if (x == '('):
				depth = depth + 1
			elif (x == ')'):
				depth = depth - 1

def checkQuestVarGet(templine, ln):
	tokens = templine.replace('(', ' ( ').replace(')', ' ) ').replace(',', ' , ').split(' ')
	findComma = False
	foundComma = False
	depth = 0
	for x in tokens:
		if (x == 'trQuestVarGet'):
			findComma = True
		if findComma:
			if (depth == 1):
				if x == ',':
					foundComma = True
				elif x == ')':
					if (foundComma):
						print("trQuestVarGet instead of trQuestVarSet")
						print("Line " + str(ln) + ":\n    " + line)
					else:
						break
			if (x == '('):
				depth = depth + 1
			elif (x == ')'):
				depth = depth - 1

def checkUnknownFunctions(templine, ln):
	tokens = templine.replace('=', ' ').replace(';', ' ').replace('(', ' ( ').replace(')', ' ) ').replace('*', ' ').replace('+', ' ').replace(',', ' ').replace('"', ' " ').replace('-', ' ').replace('/', ' ').replace('<', ' ').replace('>', ' ').split(' ')
	if (tokens[0] == 'void' or tokens[0] == 'int' or tokens[0] == 'string' or tokens[0] == 'vector' or tokens[0] == 'float' or tokens[0] == 'bool') and tokens[2] == '(':
		functions.add(tokens[1])
	else:
		ignore = False

		for t in range(len(tokens)):
			if '"' in tokens[t]:
				ignore = not ignore
			if not ignore:
				if t > 0 and tokens[t] == '(':
					if (not tokens[t-1] in functions) and (not tokens[t-1].isspace()):
						print("Unknown function")
						print("Line " + str(ln) + ":\n    " + tokens[t-1])
						if not tokens[t-1] in unknowns:
							unknowns.add(tokens[t-1])


print("rmsification start!")

functions = {' ', 'xsPow', 'trQuestVarGet', 'trQuestVarSet', '', 'trSetDisableGPBlocking', 'kbIsPlayerHuman', 'trPlayNextMusicTrack',
			'trDamageUnitsInArea', 'trCameraCut', 'trUnitSetAnimation', 'trPlayerTechTreeEnabledGodPowers', 'uiLookAtUnitByName',
			'trGetUnitScenarioNameNumber', 'trUnitIsOwnedBy', 'trCounterAddTime', 'trMutateSelected', 'xsMin', 'trSetSelectedUpVector',
			'trChatSend', 'trUnitIsSelected', 'kbGetProtoUnitName', 'trUnitSetStance', 'trForceNonCinematicModels', 'trackGotoSelectedWaypoint',
			'trLetterBox', 'trUnitDead', 'trUnitPercentDamaged', 'trDamageUnitPercent', 'trCamTrackLoad', 'trUnitVisToPlayer',
			'trTechGodPower', 'xsCos', 'trModifyProtounit', 'while', 'trPlayerSetDiplomacy', 'trIsGadgetVisible', 'trackRemove',
			'trSetUnitOrientation', 'trMessageSetText', 'kbUnitGetAnimationActionType', 'trUnitHighlight', 'trUnitConvert', 
			'xsVectorSet', 'trVectorQuestVarGetZ', 'map', 'if', 'trSetUnitIdleProcessing', 'configUndef', 'uiMessageBox',
			'trCountUnitsInArea', 'trArmyDispatch', 'trTime', 'xsEnableRule', 'trUnitDestroy', 'trPlayerUnitCountSpecific', 
			'xsSetContextPlayer', 'trVectorQuestVarGetX', 'trSetCivAndCulture', 'xsAbs', 'xsAtan', 'trUnitGetIsContained',
			'trUnitSelect', 'kbUnitGetActionType', 'trSoundPlayFN', 'trUnitTeleport', 'trUnitOverrideAnimation', 'xsVectorNormalize', 'xsVectorSetZ', 'xsVectorSetX',
			'trCameraShake', 'trCamTrackPlay', 'kbGetProtoUnitID', 'trDamageUnit', 'trGetStatValue', 'xsDisableRule', 'trRenderSnow',
			'xsSqrt', 'trSetLighting', 'for', 'trVectorQuestVarSet', 'trTechInvokeGodPower', 'aiSet', 'trCheckGPActive', 
			'trGetNextUnitScenarioNameNumber', 'trMusicPlay', 'xsSin', 'trPlayerGetPopulation', 'trUnitMoveToUnit', 'trPlayerModifyLOS',
			'trDelayedRuleActivation', 'trStringQuestVarGet', 'kbGetBlockID', 'trCurrentPlayer', 'trPlayerGrantResources', 
			'trPaintTerrain', 'trPlayerResourceCount', 'trCounterAbort', 'return', 'unitTransform', 'trPlayerKillAllGodPowers', 
			'sunColor', 'trSetCounterDisplay', 'trUnitSelectClear', 'trArmySelect', 'kbGetBlockPosition', 'trPlayerResetBlackMapForAllPlayers',
			'trQuestVarSetFromRand', 'vector', 'trTechSetStatus', 'trRevealEntireMap', 'trSetObscuredUnits', 'trEventFire',
			'kbUnitGetTargetUnitID', 'ambientColor', 'trUIFadeToColor', 'trClearCounterDisplay', 'trUnitChangeProtoUnit', 
			'switch', 'trVectorQuestVarGetY', 'trQuestVarCopy', 'trUnitAlive', 'trSetSelectedScale', 'trUnitDoWorkOnUnit', 
			'kbGetUnitBaseTypeID', 'trImmediateUnitGarrison', 'trStringQuestVarSet', 'trOverlayTextColour', 'trUnitSelectByID', 
			'trOverlayText', 'trUnitMoveToPoint', 'trSetFogAndBlackmap', 'xsMax', 'trForbidProtounit', 'xsGetContextPlayer', 
			'trSetPlayerWon', 'trEndGame', 'trSetPlayerDefeated', 'trUnitChangeName', 'trChatSendSpoofed', 'trUnitSetAnimationPath',
			'trMusicPlayCurrent', 'trVectorQuestVarGet', 'trUnitEjectContained', 'trObjectiveSetID', 'trUIFlash', 'kbUnitGetCurrentHitpoints', 
			'trMusicStop', 'uiFindType', 'xsRound', 'xsFloor', 'trChatHistoryClear', 'trSetCivilizationNameOverride', 'kbIsPlayerResigned',
			'trSoundPlayDialog', 'trChangeTerrainHeight', 'trRenderSky', 'trFadeOutAllSounds', 'aiIsMultiplayer', 'trShowWinLose', 
			'gadgetUnreal', 'trGetScenarioUserData', 'uiLookAtUnit', 'uiClearSelection', 'uiCreateNumberGroup', 'kbLookAtAllUnitsOnMap',
			'trTimeMS', 'trChatSendToPlayer', 'uiTransformSelectedUnit', 'trBlockAllSounds', 'xsDisableSelf', 'trUnblockAllSounds', 
			'trSetCurrentScenarioUserData','trModeEnter', 'exit', 'musicToggleBattleMode', 'trEventSetHandler', 'kbProtoUnitIsUnitType',
			'uiCopyToClipboard', 'uiPasteFromClipboard', 'trGetTerrainType', 'trGetTerrainSubType', 'trUnitHasLOS', 'trUnitDistanceToPoint',
			'trShowImageDialog', 'trShowChoiceDialog', 'trGameLoadScenario', 'xsVectorGetX', 'xsVectorGetY', 'xsVectorGetZ', 'kbArmyCreate', 
			'kbArmyGetName', 'kbArmyDestroy', 'trackEditWaypoint', 'trackInsert', 'trackAddWaypoint', 'trackPlay', 'trPlayerKillAllUnits', 
			'trChangeTerrainHeight'}
unknowns = {''}
ln = 1
files = ['main.c', 'shared.c', 'relics.c', 'setup.c', 'dataLoad.c', 'npc.c', 'chooseClass.c', 'gameplayHelpers.c',
		'walls.c', 'chests.c',  'traps.c', 'buildMap.c', 'moonblade.c', 'sunbow.c', 'stormcutter.c', 'alchemist.c',
		'thunderrider.c', 'fireknight.c', 'frostknight.c', 'starseer.c', 'enemies.c', 'bosses.c', 'gameplay.c', 'singleplayer.c']
first = True
try:
	with open('XS/' + FILE_2, 'w') as file_data_2:
		file_data_2.write('void code(string xs="") {\n')
		file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
		file_data_2.write('rmSetTriggerEffectParam("IdleProc",");*/"+xs+"/*");}\n')
		file_data_2.write('void main(void) {\n')
		for f in files:
			FILE_1 = f
			ln = 1
			print("parsing " + FILE_1 + "...")
			with open(FILE_1, 'r') as file_data_1:
				line = file_data_1.readline()
				while line:
					if not line.isspace():
						if ('/*' in line):
							comment = True
						
						if (not comment):
							templine = line.strip()
							checkQuestVarSet(templine, ln)
							checkQuestVarGet(templine, ln)
							if not first:
								checkUnknownFunctions(templine, ln)
							if ('//' in templine):
								print("Invalid // comment found!")
								print("Line " + str(ln) + ":\n    " + line)
							if (len(templine) > 120):
								print("Line length greater than 120! Length is " + str(len(templine)))
								print("Line " + str(ln) + ":\n    " + line)
							if ('if ' in templine or 'if(' in templine) and not 'ySetPointer' in templine and ('yGetVar' in templine or 'trQuestVarGet' in templine) and not ('=' in templine or '>' in templine or '<' in templine or 'ySetContains' in templine or 'trUnitIsOwnedBy' in templine or 'cWatchActive' in templine or 'yDatabaseContains' in templine or 'HasKeyword' in templine or 'trCheckGPActive' in templine):
								print("Missing equality statement")
								print("Line " + str(ln) + ":\n    " + line)
							if not (templine[-1] == ';' or '//' in templine or templine[-1] == '{' or templine[-1] == '}' or templine[-2:] == '||' or templine[-2:] == '&&' or templine[-1] == ',' or templine[-4:] == 'else' or templine[0:4] == 'rule' or templine == 'highFrequency' or templine == 'runImmediately' or templine[-1] == '/' or templine[-6:] == 'active' or templine[0:11] == 'minInterval' or templine[0:4] == 'case' or templine[0:7] == 'switch('):
								print("Missing semicolon")
								print("Line " + str(ln) + ":\n    " + line)
							if '{' in templine and '(' in templine and not 'else' in templine and not 'if' in templine and not 'for' in templine and not 'while' in templine and ')' in templine and not '{P' in templine:
								equalCount = templine.count('string', templine.index('(')) + templine.count('int ', templine.index('(')) + templine.count('float', templine.index('(')) + templine.count('bool', templine.index('('))
								if equalCount > templine.count('='):
									print("Needs equals sign")
									print("Line " + str(ln) + ":\n    " + line)
							if 'return' in templine and not '(' in templine:
								print("Needs parenthesis")
								print("Line " + str(ln) + ":\n    " + line)
							if ('for(' in templine or 'for (' in templine) and '";' in templine:
								print("Wrong parenthesis")
								print("Line " + str(ln) + ":\n    " + line)
							if 'for' in templine and not ';' in templine and not '//':
								print("Missing semicolon in for statement")
								print("Line " + str(ln) + ":\n    " + line)
							if 'trMutateSelected("' in templine:
								print("Needs kbGetProtoUnitID()")
								print("Line " + str(ln) + ":\n    " + line)
							if first:
								file_data_2.write(templine + '\n')
							else:
								file_data_2.write('code("' + templine.replace('"', '\\"') + '");\n')
						if ('*/' in line):
							comment = False
					else:
						file_data_2.write('\n')
					line = file_data_1.readline()
					ln = ln + 1
			if first:
				file_data_2.write('rmSwitchToTrigger(rmCreateTrigger("zenowashere"));\n')
				file_data_2.write('rmSetTriggerPriority(4);\n')
				file_data_2.write('rmSetTriggerActive(false);\n')
				file_data_2.write('rmSetTriggerLoop(false);\n')
				file_data_2.write('rmSetTriggerRunImmediately(true);\n')
				file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
				file_data_2.write('rmSetTriggerEffectParam("IdleProc",");}}/*");\n')
				first = False
		file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
		file_data_2.write('rmSetTriggerEffectParam("IdleProc",");*/rule _zenowashereagain inactive {if(true){xsDisableSelf();//");\n')
		file_data_2.write('rmSwitchToTrigger(rmCreateTrigger("get_player_names"));\n')
		file_data_2.write('rmSetTriggerPriority(4);\n')
		file_data_2.write('rmSetTriggerActive(true);\n')
		file_data_2.write('rmSetTriggerLoop(false);\n')
		file_data_2.write('rmSetTriggerRunImmediately(true);\n')
		file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
		file_data_2.write('rmSetTriggerEffectParam("IdleProc","false);/*");\n')
		file_data_2.write('for(p=1; < cNumberNonGaiaPlayers) {\n')
		file_data_2.write('code("trStringQuestVarSet(\\"p"+p+"name\\", \\""+rmGetPlayerName(p)+"\\");");\n')
		file_data_2.write('}\n');
		file_data_2.write('string quote = "\\\\";\n')
		file_data_2.write('code("map(\\"esc\\", \\"ShowImageBox\\", \\"gadgetUnreal("+quote+"ShowImageBox"+quote+")\\");");\n')
		file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
		file_data_2.write('rmSetTriggerEffectParam("IdleProc",");*///");\n')
		file_data_2.write('rmSetStatusText("", 0.99);')
		file_data_2.write('}')
except IOError:
	sys.exit("Files not found!")

print("Done!")
if (len(unknowns) > 1):
	print("Unknowns: ")
	print(unknowns)
	"""
	#"""