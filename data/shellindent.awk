#!/usr/bin/awk -f

# SOLARIS USERS: Change "awk" to "nawk", above!!!

# This is part of Phil's AWK tutorial at http://www.bolthole.com/AWK.html
# This program adjusts the indentation level based on which keywords are
# found in each line it encounters.
#
# THIS IS A (relatively) COMPLEX PROGRAM. If you're an AWK rookie,
# go back and read the tutorial before trying to understand this program!
# This program shows off awk functions, variables, and its ability to
# perform multiple actions for the same line


function doindent(){
	tmpindent=indent;
	if(indent<0){
		print "ERROR; indent level == " indent
	}
	while(tmpindent >0){
		printf("    ");
		tmpindent-=1;
	}
}

$1 == "done" 	{ indent -=1; }
$1 == "fi" 	{ indent -=1; }
$0 ~ /}/	{ if(indent!=0) indent-=1;  }

# This is the 'default' action, that actually prints a line out.
# This gets called AS WELL AS any other matching clause, in the
# order they appear in this program.
# An "if" match is run AFTER we run this clause.
# A "done" match is run BEFORE we run this clause.
		{ 
		  doindent();
		  print $0;
		}

$0 ~ /if.*;[ ]*then/	{ indent+=1; }
$0 ~ /for.*;[ ]*do/	{ indent+=1; }
$0 ~ /while.*;[ ]*do/	{ indent+=1; }

$1 == "then"		{ indent+=1; }
$1 == "do"		{ indent+=1; }
$0 ~ /{$/		{ indent+=1; }
