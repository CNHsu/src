# 2021 10 15 NCYU Rachel
#Assume that two variables, varA and varB , are assigned values, either numbers or strings.
#Write a piece of Python code that evaluates varA and varB , and then prints out one of the following messages:


import sys

while 1:
 varA=input("A : ")
 varB=input("B : ")
 
 if varA.isdigit() ==True or varA[0]=='-':
     print('varA is digit')
     if varA[0]=='-':
          varA=-1*int(varA)
     else:
         varA=int(varA);
 if varB.isdigit() ==True or varB[0]=='-':
     print('varB is digit')
     if varB[0]=='-':
          varB=-1*int(varB)
     else:
         varB=int(varB);         

 if type(varA) == str or type(varB) == str:
  print('string involved')  
 elif int(varA) > int(varB):
     print('bigger')
 elif int(varA) == int(varB):
     print('equal')
 else:
# If none of the above conditions are true,
# it must be the case that varA < varB
   print('smaller')