//http://www.anonhack.in/2016/08/program-convert-floating-decimal-binary/
// 2020/10/21

#include<stdio.h>
#include <cstdlib> // Needed to use the exit function
int main(){
FILE *fp,*fout;
char decfile[25];
int i;
printf("input log filename:");
scanf("%s",decfile);
fp=fopen(decfile,"r");
fout=fopen("out.bin","wb");
 if(!fp) {printf("open file error\n\r"); exit(1);}
 while(!feof(fp)){
 fscanf(fp,"%d\n\r",&i);
 fwrite(&i,2,1,fout);
}
printf("file output done!! \n\r");
 fclose(fp);
 fclose(fout);
}
 
