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

def checkStringConcatenation(templine, ln):
	tokens = templine.replace('(', ' ( ').replace(')', ' ) ').replace(',', ' , ').replace('+', ' + ').replace('=', ' = ').replace('"', ' " ').split(' ')
	tokens = [token for token in tokens if token != '']
	inString = False
	prev = ''
	for x in tokens:
		if not inString and prev == '"':
			if not (x == ')' or x == ';' or x == '+' or x == ',' or x == ''):
				print("Error in string concatenation. Subsequent token was " + x)
				print("Line " + str(ln) + ":\n    " + line)
				break
		if (x == '"'):
			inString = not inString
			if inString:
				if not (prev == '(' or prev == '+' or prev == '=' or prev == ',' or prev == ''):
					print("Error in string concatenation. Previous token was " + prev)
					print("Line " + str(ln) + ":\n    " + line)
					break
		prev = x

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
			'xsVectorSet', 'trVectorQuestVarGetZ', 'map', 'if', 'trSetUnitIdleProcessing', 'configUndef', 'uiMessageBox', 'trUnitDelete',
			'trCountUnitsInArea', 'trArmyDispatch', 'trTime', 'xsEnableRule', 'trUnitDestroy', 'trPlayerUnitCountSpecific', 
			'xsSetContextPlayer', 'trVectorQuestVarGetX', 'trSetCivAndCulture', 'xsAbs', 'xsAtan', 'trUnitGetIsContained',
			'trUnitSelect', 'kbUnitGetActionType', 'trSoundPlayFN', 'trUnitTeleport', 'trUnitOverrideAnimation', 'xsVectorNormalize', 'xsVectorSetZ', 'xsVectorSetX',
			'trCameraShake', 'trCamTrackPlay', 'kbGetProtoUnitID', 'trDamageUnit', 'trGetStatValue', 'xsDisableRule', 'trRenderSnow',
			'xsSqrt', 'trSetLighting', 'for', 'trVectorQuestVarSet', 'trTechInvokeGodPower', 'aiSet', 'trCheckGPActive', 'trMinimapFlare',
			'trGetNextUnitScenarioNameNumber', 'trMusicPlay', 'xsSin', 'trPlayerGetPopulation', 'trUnitMoveToUnit', 'trPlayerModifyLOS',
			'trDelayedRuleActivation', 'trStringQuestVarGet', 'kbGetBlockID', 'trCurrentPlayer', 'trPlayerGrantResources', 'aiSetAttackResponseDistance',
			'trPaintTerrain', 'trPlayerResourceCount', 'trCounterAbort', 'return', 'unitTransform', 'trPlayerKillAllGodPowers', 
			'sunColor', 'trSetCounterDisplay', 'trUnitSelectClear', 'trArmySelect', 'kbGetBlockPosition', 'trPlayerResetBlackMapForAllPlayers',
			'trQuestVarSetFromRand', 'vector', 'trTechSetStatus', 'trRevealEntireMap', 'trSetObscuredUnits', 'trEventFire',
			'kbUnitGetTargetUnitID', 'ambientColor', 'trUIFadeToColor', 'trClearCounterDisplay', 'trUnitChangeProtoUnit', 'trSetCloudData',
			'switch', 'trVectorQuestVarGetY', 'trQuestVarCopy', 'trUnitAlive', 'trSetSelectedScale', 'trUnitDoWorkOnUnit', 'trSetCloudMap',
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
			'trChangeTerrainHeight', 'trChatSetStatus', 'trUnitSetHeading'}
unknowns = {''}
ln = 1
files = ['main.c', 'shared.c', 'boons.c', 'relics.c', 'setup.c', 'dataLoad.c', 'chooseClass.c', 'gameplayHelpers.c', 'enemies.c', 'mapHelpers.c', 'npc.c', 'walls.c', 'chests.c', 'traps.c',
		'buildMap.c', 'moonblade.c', 'sunbow.c', 'stormcutter.c', 'alchemist.c', 'spellstealer.c', 'commando.c', 'savior.c', 'gardener.c', 'nightrider.c', 'sparkwitch.c',
		'starseer.c', 'throneShield.c', 'thunderrider.c', 'fireknight.c', 'frostknight.c', 'blastmage.c', 'bosses.c', 'temples.c', 'gameplay.c', 'singleplayer.c']

first = True
escape = False
try:
	with open('XS/' + FILE_2, 'w') as file_data_2:
		file_data_2.write('void code(string xs="") {\n')
		file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
		file_data_2.write('rmSetTriggerEffectParam("IdleProc",");*/"+xs+"/*");}\n')
		file_data_2.write('void main(void) {\n')
		for f in files:
			FILE_1 = f
			ln = 1
			pcount = 0 # parenthesis
			bcount = 0 # brackets
			print("parsing " + FILE_1 + "...")
			rewrite = []
			thedepth = 0
			with open(FILE_1, 'r') as file_data_1:
				line = file_data_1.readline()
				while line:
					reline = line.strip()
					if '(' in reline:
						thedepth = thedepth + 1
					if ')' in reline:
						thedepth = thedepth - 1
					if '}' in reline:
						thedepth = thedepth - 1
					reline = "\t" * thedepth + reline
					rewrite.append(reline)
					if '{' in reline:
						thedepth = thedepth + 1
					
					if not line.isspace():
						if ('/*' in line):
							comment = True

						if not comment:
							if ('%' in line):
								escape = not escape
							elif escape:
								file_data_2.write(line)
							else:
								templine = line.strip()
								checkStringConcatenation(templine, ln)
								checkQuestVarSet(templine, ln)
								checkQuestVarGet(templine, ln)
								if not first:
									checkUnknownFunctions(templine, ln)
								pcount = pcount + line.count('(') - line.count(')')
								bcount = bcount + line.count('{') - line.count('}')
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
									if ('//' in templine):
										templine = templine[:templine.find('//')]
									file_data_2.write('code("' + templine.replace('"', '\\"') + '");\n')
						if ('*/' in line):
							comment = False
					else:
						file_data_2.write('\n')
					line = file_data_1.readline()
					ln = ln + 1
			#with open(FILE_1, 'w') as file_data_1:
			#	for line in rewrite:
			#		file_data_1.write(line + '\n')
			if pcount < 0:
				print("ERROR: Extra close parenthesis detected!\n")
			elif pcount > 0:
				print("ERROR: Missing close parenthesis detected!\n")
			if bcount < 0:
				print("ERROR: Extra close brackets detected!\n")
			elif bcount > 0:
				print("ERROR: Missing close brackets detected!\n")
			if first:
				file_data_2.write('rmAddTriggerEffect("SetIdleProcessing");\n')
				file_data_2.write('rmSetTriggerEffectParam("IdleProc",");*/rule _zenowasherefirstagain inactive {if(true){xsDisableSelf();//");\n')
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