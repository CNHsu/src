def is_perfect(i):
    if i >1:
        sum =0
        mylist=[" "]
        for j in range(1,i):
            if i != j and i % j ==0:
                sum = sum + j                
                mylist.append(str(j))
            #    print("*",sum,j)
        if sum == i:
            #print("True: ")
            print(sum,"=","+".join(mylist[1:]))
            return True
        else:
            return False
count=0
N = int(input())
for i in range(N):
    if is_perfect(i):
 #       num_list =[j for j in range(1,i) if i % j == 0]
 #       numn= [str(x) for x in num_list]
        count=count+1
 #       print(i, "=", "+".join(numn))
print(count)        
            