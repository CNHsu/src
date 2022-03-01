// 2022/2/17
// uart send data transfer
// data format
// start 1bit '0'
// data  8bit
// parity 1bit
// stop 1bit '1'
// read data add start/parity/stop bit

#include <stdio.h>

char outfilename[200];

main()
{	
char a1,a2;
char a[10];
unsigned char b,c;
int i;
int j,k,l;
unsigned char parity;

FILE *infile=fopen("senddata.txt","r");

FILE *outfile;
	printf("read input file senddata.txt\n\r");
	if( infile==NULL) {
		printf(" cannot open senddata.txt file!!");
	        return -1;
	        }
        printf("output filename:   ");
        scanf("%s",outfilename);	        
        if((outfile=fopen(outfilename,"w"))==NULL){
        	printf(" cannot open output file %s",outfilename);
        	return -1;
        }
i=0;        
 while (!feof(infile)){
 	fscanf(infile,"%c",a);
 	for(l=0;l<1;l++){
 	a1=a[l]; printf("(%x)",a[l]);
 	if (a1 >=65 && a1<=70) b=a1-65+10;
 	else 
 	if( a1 >=48 && a1<=57) b=a1-48;
 	else
 	if( a1>=97 && a1 <=102) b=a1-97+10; 	
 	else { b=0; continue; 	}
 	printf("%d:%02d [%02x]\n\r",i,b,b);
 	i++;
 	// start bit
 	if(l==0) {fputc('0',outfile);fputc('\n',outfile);}
 	for(k=0,j=0;j<7;j++){ 		
            c=(b>>j)&0x01;
            if(c) k++;
            // data bit
            if(c==1)
              {
              	fputc('1',outfile);            
              	fputc('\n',outfile);
             }
            else { fputc('0',outfile); fputc('\n',outfile);}
 	}
 	// parity bit
 	//if(l==1){
 	if(k%2) {fputc('0',outfile); fputc('\n',outfile);}
 	else {fputc('1',outfile);fputc('\n',outfile);}
 	//end bit
 	fputc('1',outfile);
 	fprintf(outfile,"\n");
        //}   
 	//fprintf(outfile,"%02x",b);
       }
        
}        
        
fclose(infile);
fclose(outfile);       
}