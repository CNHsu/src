//http://www.anonhack.in/2016/08/program-convert-floating-decimal-binary/
// 2020/10/21

#include<stdio.h>
int main(){
float a;
double c;
int b,k,l=0,m[20],n,i,count=0,p;
printf("Enter any number:");
scanf("%lf",&c);
b=c;   // int part to b
a=c-b; // get floating part to a
printf("%d,%f",b,a);
printf("\n");
while(b!=0){
 n=b%2;
 b/=2;
 m[i]=n;       //int part binary
 count+=1;i++;
 
}
for(k=count-1;k>=0;k--){
printf("%d",m[k]);  // print int part binary
}

printf(".");        // floating part 8bits
for(p=1;p<=8;p++){
a=a*2;         // *2 mov floating part to int part
l=a;           // get int part '1' or '0'
printf("%d",l); // print int part '1' or '0'
if(l==1)
 a=a-l;         // remove int part, 
 
}
printf("\n");

}