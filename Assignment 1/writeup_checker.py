from __future__ import print_function
from sys import argv,exit
status = 0
try:
	f = open(argv[1],'r')
	compiler_id = f.readline().rstrip()
	if compiler_id=="":
		print("Error: writeup.txt is empty")
		status = -1
	elif compiler_id not in ("1","2","3"):
		print("%s is not a valid option for choice of programming languages. It has to be 1,2 or 3."%(compiler_id))
		status = -1
	else:
		collaborator = f.readline().rstrip()
		if collaborator =="":
			print("You need to specify 'None' on second line if no collaborators are mentioned.")
			status = -1
		else :
			print("Correct : writeup.txt is in correct format")
except:	
	print("Error : Could not read writeup.txt")
	status = -1
finally:
	exit(status)
