import os
import sys

###############################
####### CUSTOMIZE THESE #######
###############################
FILENAME = 'Ascension MMORPG.xs'
files = ['main.c', 'test.c']


#########################################
####### CODE BELOW (DO NOT TOUCH) #######
#########################################

DATATYPE = ['int', 'float', 'string', 'void', 'vector', 'bool']
ARITHMETIC = ['+', '-', '/', '*']
BINARY = ['==', '!=', '<=', '>=', '>', '<', '&&', '||']
FLOW = ['(', ')', '{', '}', ',', ';']

DELIMITER = [',', ';']
IGNORE = ['const']

SYMBOLS = [ARITHMETIC, BINARY, FLOW]

class CustomFunction:
	def __init__(self, name, datatype):
		self.name = name
		self.datatype = datatype
		self.parameters = []

	def add(self, datatype):
		self.parameters.append(datatype)

CURRENT_DEPTH = 0
FUNCTIONS = {}
CURRENT_JOB = None

KNOWN_VARIABLES = []
KNOWN_TYPES = []
STACK_FRAMES = []

def error(msg):
	global ln
	global line
	print(msg)
	print("Line " + str(ln) + ":\n    " + line)

class StackFrame:
	name = ''
	parent = None
	type = ''
	datatype = ''
	closed = False
	expected = []

	def __init__(self, name, parent):
		self.name = name
		self.parent = parent
	
	def debug(self):
		if self.parent and not self.parent == self:
			return self.parent.debug() + ' ' + self.name
		else:
			return self.name

	def resolve(self, inputs):
		global CURRENT_JOB
		if self.closed:
			CURRENT_JOB = self.parent
			inputs = [self] + inputs
			self.parent.resolve(inputs)
		self.closed = True

	def accept(self, token):
		global CURRENT_JOB
		if token in ARITHMETIC or token in BINARY:
			lastOpen = CURRENT_JOB
			if not token in ['*', '/']:
				while lastOpen.parent.closed:
					lastOpen = lastOpen.parent
				if lastOpen.parent.type == 'ARITHMETIC':
					CURRENT_JOB.resolve([])
			if token in ARITHMETIC:
				CURRENT_JOB.parent = Arithmetic(token, CURRENT_JOB.parent)
			elif token in BINARY:
				CURRENT_JOB.parent = Binary(token, CURRENT_JOB.parent)
		elif token in DATATYPE:
			CURRENT_JOB = Declaration(token, CURRENT_JOB)
		elif token in FUNCTIONS:
			CURRENT_JOB = Function(token, CURRENT_JOB)
		elif token in KNOWN_VARIABLES:
			CURRENT_JOB = Variable(token, CURRENT_JOB)
		elif token == ';':
			self.closed = True
			self.resolve([])
			CURRENT_JOB = BaseFrame()
		elif token == ')':
			self.closed = True
			self.resolve([])
			while CURRENT_JOB.type == 'ARITHMETIC' or CURRENT_JOB.type == 'BINARY':
				CURRENT_JOB.resolve([])
		elif token == ',' and not self.closed:
			self.resolve([])
		elif token == '""':
			CURRENT_JOB = Literal(token, CURRENT_JOB, 'string')
		elif token.isnumeric():
			CURRENT_JOB = Literal(token, CURRENT_JOB, 'int')
		elif '.' in token:
			isFloat = True
			for c in token[:token.find('.')]:
				isFloat = isFloat and c.isnumeric()
			for c in token[token.find('.')+1:]:
				isFloat = isFloat and c.isnumeric()
			if isFloat:
				CURRENT_JOB = Literal(token, CURRENT_JOB, 'float')
			else:
				error("Unrecognized symbol: " + token)
		elif token in ['true', 'false']:
			CURRENT_JOB = Literal(token, CURRENT_JOB, 'bool')
		elif token == '=':
			if not CURRENT_JOB.type == 'VARIABLE':
				error("Cannot use assignment operator on this token: " + self.name)
			else:
				CURRENT_JOB.resolve([])
				CURRENT_JOB.parent = Assignment(token, CURRENT_JOB.parent)
		elif token == '(':
			if self.closed:
				CURRENT_JOB = Parenthesis(token, CURRENT_JOB)

class Literal(StackFrame):
	def __init__(self, name, parent, datatype):
		super().__init__(name, parent)
		self.type = 'LITERAL'
		self.datatype = datatype
		self.closed = True

class Function(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.name = name
		self.type = 'FUNCTION'
		self.datatype = FUNCTIONS[name].datatype
		self.expected = FUNCTIONS[name].parameters

	def resolve(self, inputs):
		if self.closed == True:
			super().resolve(inputs)
		else:
			self.closed = True
			if len(inputs) > len(self.expected):
				error("Too many inputs for " + self.name)
			for i in range(len(inputs)):
				if self.expected[i] != inputs[i].datatype:
					if not self.expected[i] in ['int', 'float'] and not inputs[i].datatype in ['int', 'float']:
						error("Incorrect datatype in parameter " + i + "! Expected " + self.expected[i] + " but got " + inputs[i].datatype)

class Variable(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.name = name
		self.type = 'VARIABLE'
		self.datatype = KNOWN_TYPES[KNOWN_VARIABLES.find(name)]
		self.closed = True

class BaseFrame(StackFrame):
	def __init__(self):
		self.name = "Base"
		self.parent = self

	def resolve(self, inputs):
		pass

	def debug(self):
		return 'Base'

class Trigger(StackFrame):
	def accept(self, token):
		if token == '{':
			self.closed = True
		elif not token in ['active', 'inactive', 'highFrequency'] or 'minInterval' in token:
			error("Unknown syntax for trigger declaration: " + token)
		else:
			super().accept(token)

class Arithmetic(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'ARITHMETIC'

	def resolve(self, inputs):
		if self.closed:
			super().resolve(inputs)
		else:
			self.closed = True
			if len(inputs) != 2:
				error("Incorrect number of inputs for arithmetic operator " + self.name)
			else:
				self.datatype = inputs[0].datatype
				self.name = self.datatype
				for i in range(2):
					if inputs[i].datatype in ['bool', 'void']:		
						error("Cannot perform arithmetic operator " + self.name + " on " + inputs[i].name + " of type " + inputs[i].datatype)
				
				if self.datatype != inputs[1].datatype:
					if self.datatype == 'string' and self.name in ['-', '/', '*']:
						error("Cannot perform arithmetic operator " + self.name + " on a string!")
					elif inputs[1].datatype == 'string':
						error("Cannot add a string to a " + self.datatype)


class Binary(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'BINARY'
		self.datatype = 'bool'
		if self.name in ['&&', '||']:
			self.expected = ['bool', 'bool']

	def resolve(self, inputs):
		if self.closed:
			super().resolve(inputs)
		else:
			self.closed = True
			if len(inputs) != 2:
				error("Incorrect number of inputs for boolean operator " + self.name)
			elif len(self.expected) > 0:
				self.datatype = self.expected[0]
				for i in range(2):
					if inputs[i].datatype != self.expected[i]:
						error("Invalid datatype used in logic statement " + self.name + ". Expected boolean but received " + inputs[i].datatype)
			else:
				self.datatype = inputs[0].datatype
				self.name = self.datatype
				if self.datatype not in ['int', 'float']:
					if self.datatype != inputs[1].datatype:
						error("Cannot perform boolean operator " + self.name + " on data of type " + self.datatype + " and " + inputs[1].datatype)
				elif inputs[1].datatype not in ['int', 'float']:
					error("Cannot perform boolean operator " + self.name + " on data of type " + self.datatype + " and " + inputs[1].datatype)

class Assignment(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'ASSIGNMENT'

	def resolve(self, inputs):
		if self.closed:
			super().resolve(inputs)
		else:
			self.closed = True
			if len(inputs) != 2:
				error("Incorrect number of inputs for assignment operator " + self.name)
			else:
				self.datatype = inputs[0].datatype
				if self.datatype not in ['int', 'float']:
					if self.datatype != inputs[1].datatype:
						error("Cannot assign " + inputs[1].datatype + " to " + self.datatype)
				elif inputs[1].datatype not in ['int', 'float']:
					error("Cannot assign " + inputs[1].datatype + " to " + self.datatype)

class Parenthesis(StackFrame):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'PARENTHESIS'

	def resolve(self, inputs):
		if self.closed:
			super().resolve(inputs)
		else:
			self.closed = True
			if len(inputs) != 1:
				error("Unresolved operations in parenthesis")
			else:
				self.datatype = inputs[0].datatype
				self.name = self.datatype

class Declaration(StackFrame):
	def __init__(self, name, parent):
		super().__init__('', parent)
		self.state = 0
		self.type = 'VARIABLE'
		self.datatype = name

	def resolve(self, inputs):
		KNOWN_VARIABLES.append(self.name)
		KNOWN_TYPES.append(self.datatype)
		if self.type == 'FUNCTION':
			self.state = 2
			FUNCTIONS.update({self.name : CustomFunction(self.name, self.datatype)})
			for frame in inputs:
				FUNCTIONS[self.name].add(frame.datatype)
		super().resolve(inputs)

	def accept(self, token):
		if self.state == 0:
			self.name = token
			self.state = 1
		elif self.state == 1:
			if token == '(':
				self.type = 'FUNCTION'
				self.state = 2
			else:
				self.state = 2
				super().accept(token)
		else:
			super().accept(token)


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
		if not isString or token == '"':
			retline = retline + token
	if "//" in retline:
		retline = retline[:retline.find("//")]
	return retline

print("Reading Command Viewer")
with open('Commands.xml', 'r') as fd:
	line = fd.readline()
	while line:
		if '<method' in line:
			fq = line.find('"') + 1
			lq = line.rfind('"')
			name = line[line.rfind('"',0,lq-1)+1:lq] # The thing between the last two quotes
			FUNCTIONS.update({name: CustomFunction(name, line[fq:line.find('"',fq+1)])})
			while not '</method' in line:
				if '<parameters' in line:
					fq = line.find('"') + 1
					FUNCTIONS[name].add(line[fq:line.find('"',fq+1)])
				line = fd.readline()
		line = fd.readline()

print("rmsification start!")

CURRENT_JOB = BaseFrame()
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
			CURRENT_DEPTH = 0
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
								if not first:
									templine = nostrings
									templine = templine.replace('=', ' = ').replace(' =  = ', '==').replace(' !  = ', '!=').replace(' >  = ', '>=').replace(' <  = ', '<=')
									for s in SYMBOLS:
										for n in s:
											templine = templine.replace(n, ' ' + n + ' ')
									tokens = [token for token in templine.split(' ') if token != '']

									for token in tokens:
										if not token in IGNORE:
											CURRENT_JOB.accept(token)
											print(token)
											print(CURRENT_JOB.debug())
								
								templine = reline.strip()

								# Obsolete Sanity Checks
								checkStringConcatenation(templine, ln)
								#if not first:
									#checkUnknownFunctions(templine, ln)
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