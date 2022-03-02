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

# Stack Frame states
STATE_NEED_NAME = 0
STATE_NEED_PARENTHESIS = 1
STATE_IN_PARENTHESIS = 2
STATE_WAITING_COMMA = 3
STATE_WAITING_BRACKETS = 4
STATE_IN_BRACKETS = 5
STATE_DONE = 6

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

class Job:
	def __init__(self, name, parent):
		self.name = name
		self.parent = parent
		self.type = ''
		self.datatype = ''
		self.closed = False
		self.expected = []
		self.children = []
	
	def debug(self):
		val = self.name
		if len(self.children) > 0:
			val = val + '('
			first = True
			for c in self.children:
				if not first:
					val = val + ',' + c.debug()
				else:
					val = val + c.debug()
					first = False
			val = val + ')'
		return val

	def resolve(self):
		global CURRENT_JOB
		self.closed = True
		for c in self.children:
			c.resolve()

	def accept(self, token):
		accepted = True
		if len(self.children) > 0:
			# Do you want this token?
			accepted = self.children[-1].accept(token)
		else:
			accepted = False
		return accepted

	def insertAbove(self, frametype, token):
		self.parent.children.remove(self)
		self.parent.children.append(frametype(token, self.parent))
		self.parent = self.parent.children[-1]
		self.parent.children.append(self)

	def parseGeneric(self, token):
		global KNOWN_VARIABLES
		accepted = True
		if token in FUNCTIONS:
			self.children.append(Function(token, self))
		elif token in KNOWN_VARIABLES:
			self.children.append(Variable(token, self))
		elif token == '""':
			self.children.append(Literal(token, self, 'string'))
		elif token.isnumeric():
			self.children.append(Literal(token, self, 'int'))
		elif '.' in token:
			isFloat = True
			for c in token[:token.find('.')]:
				isFloat = isFloat and c.isnumeric()
			for c in token[token.find('.')+1:]:
				isFloat = isFloat and c.isnumeric()
			if isFloat:
				self.children.append(Literal(token, self, 'float'))
			else:
				accepted = False
		elif token in ['true', 'false']:
			self.children.append(Literal(token, self, 'bool'))
		elif token == '(':
			self.children.append(Parenthesis(token, self))
		else:
			accepted = False
		return accepted

# Mathables will check for arithmetic operators and act accordingly
class Mathable(Job):
	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if token in ARITHMETIC:
				if self.parent.type in ['ARITHMETIC', 'BINARY'] and not token in ['*', '/']:
					self.parent.insertAbove(Arithmetic, token)
				else:
					self.insertAbove(Arithmetic, token)
			elif token in BINARY:
				self.insertAbove(Binary, token)
			else:
				accepted = False
		return accepted

class BaseFrame(Job):
	def __init__(self):
		super().__init__("Base", None)

	def debug(self):
		for c in self.children:
			print(c.debug())

	def accept(self, token):
		accepted = True
		# The only things that exist in the BaseFrame are functions and triggers
		if not super().accept(token):
			if token in DATATYPE:
				self.children.append(Declaration(token, self))
			elif token == 'rule':
				self.children.append(Trigger(token, self))
			elif token == ';':
				self.children.pop()
			else:
				error("Unrecognized token: " + token)
				accepted = False
		return accepted

class StackFrame(Job):
	def __init__(self, name, parent):
		global KNOWN_VARIABLES
		super().__init__(name, parent)
		self.depth = len(KNOWN_VARIABLES)
		self.state = 0

	def resolve(self):
		super().resolve()
		print(KNOWN_VARIABLES)

	def accept(self, token):
		global KNOWN_VARIABLES
		global KNOWN_TYPES
		accepted = True
		if not super().accept(token):
			if self.state == STATE_WAITING_BRACKETS:
				if token == '{':
					self.resolve()
					self.state = STATE_IN_BRACKETS
				else:
					accepted = False
			elif self.state == STATE_IN_BRACKETS:
				if token == ';':
					self.children[0].resolve()
					self.children.pop()
				elif token == '}':
					KNOWN_VARIABLES = KNOWN_VARIABLES[:self.depth]
					KNOWN_TYPES = KNOWN_TYPES[:self.depth]
					self.parent.children.pop()
					print(KNOWN_VARIABLES)
				elif self.parseGeneric(token):
					accepted = True
				elif token in DATATYPE:
					if len(self.children) > 0:
						error("Invalid syntax: " + token)
						accepted = False
					else:
						self.children.append(Declaration(token, self))
				else:
					self.resolve()
					accepted = False
			else:
				accepted = False
		return accepted

class Declaration(StackFrame):
	def __init__(self, name, parent):
		super().__init__('', parent)
		self.state = 0
		self.type = 'VARIABLE'
		self.datatype = name

	def resolve(self):
		global KNOWN_VARIABLES
		global KNOWN_TYPES
		if not self.closed:
			self.closed = True
			super().resolve()
			if self.type == 'FUNCTION':
				FUNCTIONS.update({self.name : CustomFunction(self.name, self.datatype)})
				for frame in self.children:
					FUNCTIONS[self.name].add(frame.datatype)
			self.children.clear()

	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if self.state == STATE_NEED_NAME:
				if token in KNOWN_VARIABLES:
					error("Declaring a function or variable name that was already declared in this context: " + token)
					accepted = False
				else:
					self.name = token
					self.state = STATE_NEED_PARENTHESIS
					KNOWN_VARIABLES.append(self.name)
					KNOWN_TYPES.append(self.datatype)
			elif self.state == STATE_NEED_PARENTHESIS:
				if not token in ['=', '(']:
					accepted = False
				elif token == '(':
					# shift depth forward to remember this function
					self.depth = len(KNOWN_VARIABLES)
					self.state = STATE_IN_PARENTHESIS
					self.type = 'FUNCTION'
				else:
					self.resolve()
					self.insertAbove(Assignment, token)
					self.state = STATE_DONE
			elif self.state == STATE_IN_PARENTHESIS:
				if self.type == 'FUNCTION' and token in DATATYPE:
					self.children.append(Declaration(token, self))
					self.state = STATE_WAITING_COMMA
				else:
					accepted = False
			elif self.state == STATE_WAITING_COMMA:
				if not token in [',',')']:
					accepted = False
				else:
					self.children[-1].resolve()
					self.state = STATE_IN_PARENTHESIS
					if token == ')':
						self.state = STATE_WAITING_BRACKETS
			else:
				accepted = False
				
		return accepted

class Assignment(Job):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'ASSIGNMENT'

	def resolve(self):
		if not self.closed:
			super().resolve()
			self.closed = True
			if len(self.children) != 2:
				error("Incorrect number of inputs for assignment operator " + self.name)
			else:
				self.datatype = self.children[0].datatype
				if self.datatype not in ['int', 'float']:
					if self.datatype != self.children[1].datatype:
						error("Cannot assign " + self.children[1].datatype + " to " + self.datatype)
				elif self.children[1].datatype not in ['int', 'float']:
					error("Cannot assign " + self.children[1].datatype + " to " + self.datatype)

	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if self.closed:
				accepted = False
			elif len(self.children) < 2:
				accepted = self.parseGeneric(token)
			else:
				self.resolve()
				accepted = False
		return accepted

class Literal(Mathable):
	def __init__(self, name, parent, datatype):
		super().__init__(name, parent)
		self.type = 'LITERAL'
		self.datatype = datatype
		self.closed = True

class Function(Mathable):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.name = name
		self.type = 'FUNCTION'
		self.datatype = FUNCTIONS[name].datatype
		self.expected = FUNCTIONS[name].parameters
		self.state = 0

	def resolve(self):
		if not self.closed:
			super().resolve()
			self.closed = True
			if len(self.children) > len(self.expected):
				error("Too many inputs for " + self.name + " expected " + str(len(self.expected)) + " but received " + str(len(self.children)))
			else:
				for i in range(len(self.children)):
					if self.expected[i] != self.children[i].datatype:
						if not self.expected[i] in ['int', 'float'] and not self.children[i].datatype in ['int', 'float']:
							error("Incorrect datatype in parameter " + str(i) + "! Expected " + self.expected[i] + " but got " + self.children[i].datatype)
				self.name = self.datatype
				self.children = []

	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if self.closed:
				accepted = False
			elif self.state == 0:
				if token == '(':
					accepted = True
					self.state = 1
				else:
					accepted = False
			elif token == ')':
				self.resolve()
				accepted = True
			elif token == ',':
				self.children[0].resolve()
				accepted = True
			else:
				accepted = self.parseGeneric(token)
		return accepted

class Variable(Mathable):
	def __init__(self, name, parent):
		global KNOWN_VARIABLES
		super().__init__(name, parent)
		self.name = name
		self.type = 'VARIABLE'
		self.datatype = KNOWN_TYPES[KNOWN_VARIABLES.index(name)]
		self.closed = True


class Arithmetic(Mathable):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'ARITHMETIC'

	def resolve(self):
		if not self.closed:
			super().resolve()
			self.closed = True
			if len(self.children) != 2:
				error("Incorrect number of inputs for arithmetic operator " + self.name)
			else:
				self.datatype = self.children[0].datatype
				for i in range(2):
					if self.children[i].datatype in ['bool', 'void']:		
						error("Cannot perform arithmetic operator " + self.name + " on " + self.children[i].name + " of type " + self.children[i].datatype)
				
				if self.datatype == 'string' and self.name in ['-', '/', '*']:
					error("Cannot perform arithmetic operator " + self.name + " on a string!")
				elif self.datatype != self.children[1].datatype and self.children[1].datatype == 'string':
					error("Cannot add a string to a " + self.datatype)

				self.name = self.datatype
				self.children = []

	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if self.closed:
				accepted = False
			elif len(self.children) < 2:
				accepted = self.parseGeneric(token)
			else:
				self.resolve()
				accepted = False
		return accepted



class Binary(Mathable):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'BINARY'
		self.datatype = 'bool'
		if self.name in ['&&', '||']:
			self.expected = ['bool', 'bool']

	def resolve(self):
		if not self.closed:
			super().resolve()
			self.closed = True
			if len(self.children) != 2:
				error("Incorrect number of inputs for boolean operator " + self.name)
			elif len(self.expected) > 0:
				self.datatype = self.expected[0]
				for i in range(2):
					if self.children[i].datatype != self.expected[i]:
						error("Invalid datatype used in logic statement " + self.name + ". Expected boolean but received " + self.children[i].datatype)
			else:
				self.datatype = self.children[0].datatype
				if self.datatype not in ['int', 'float']:
					if self.datatype != self.children[1].datatype:
						error("Cannot perform boolean operator " + self.name + " on data of type " + self.datatype + " and " + self.children[1].datatype)
				elif self.children[1].datatype not in ['int', 'float']:
					error("Cannot perform boolean operator " + self.name + " on data of type " + self.datatype + " and " + self.children[1].datatype)
				self.name = self.datatype
				self.children = []

	def accept(self, token):
		accepted = True
		if not super().accept(token):
			if self.closed:
				accepted = False
			else:
				accepted = self.parseGeneric(token)
				if len(self.children) == 2:
					self.resolve()
		return accepted

class Parenthesis(Mathable):
	def __init__(self, name, parent):
		super().__init__(name, parent)
		self.type = 'PARENTHESIS'

	def resolve(self):
		if not self.closed:
			self.closed = True
			if len(self.children) != 1:
				error("Unresolved operations in parenthesis")
			else:
				self.datatype = self.children[0].datatype
				self.name = self.datatype
				self.children = []

class Trigger(Job):
	def accept(self, token):
		if token == '{':
			self.closed = True
		elif not token in ['active', 'inactive', 'highFrequency'] or 'minInterval' in token:
			error("Unknown syntax for trigger declaration: " + token)
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
											#print(token)
											CURRENT_JOB.accept(token)
											CURRENT_JOB.debug()
								
								templine = reline.strip()
								if '//' in templine:
									templine = templine[:templine.find('//')]

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
								if len(templine) > 0 and not (templine[-1] == ';' or templine[-1] == '{' or templine[-1] == '}' or templine[-2:] == '||' or templine[-2:] == '&&' or templine[-1] == ',' or templine[-4:] == 'else' or templine[0:4] == 'rule' or templine == 'highFrequency' or templine == 'runImmediately' or templine[-1] == '/' or templine[-6:] == 'active' or templine[0:11] == 'minInterval' or templine[0:4] == 'case' or templine[0:7] == 'switch('):
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