import os
import sys

FILENAME = 'Ascension MMORPG.xs'
files = ['main.c', 'test.c']


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

def removeStrings(line):
	retline = ""
	isString = False
	for token in line:
		if token == '"':
			isString = not isString
		if not isString:
			retline = retline + token
	if "//" in retline:
		retline = retline[:retline.find("//")]
	return retline

def resolveArithmetic(linelist, depth):
	resolved = ""
	

print("rmsification start!")

functions = {''}
unknowns = {''}
ln = 1
FILE_1 = None
comment = False
first = True
escape = False
try:
	with open('XS/' + FILENAME, 'w') as file_data_2:
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
					# Rewrite history
					reline = line.strip()
					nostrings = removeStrings(reline)
					if '}' in nostrings:
						thedepth = thedepth - 1
					reline = "\t" * thedepth + reline
					rewrite.append(reline)
					if '{' in nostrings:
						thedepth = thedepth + 1
					if '(' in nostrings:
						thedepth = thedepth + 1
					if ')' in nostrings:
						thedepth = thedepth - 1
					pcount = pcount + nostrings.count('(') - nostrings.count(')')
					bcount = bcount + nostrings.count('{') - nostrings.count('}')
					
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
								if not first:
									checkUnknownFunctions(templine, ln)
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

								# reWrite the line
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
			with open(FILE_1, 'w') as file_data_1:
				for line in rewrite:
					file_data_1.write(line + '\n')
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