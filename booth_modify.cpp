//
//https://www.geeksforgeeks.org/booths-multiplication-algorithm/
// CPP code to implement booth's algorithm 
// https://web.archive.org/web/20120429071825/http://fourier.eng.hmc.edu/e85/lectures/arithmetic_html/node10.html
//http://www.ecs.umass.edu/ece/koren/arith/simulator/Booth/
//https://web.archive.org/web/20120304074323/http://hdlsnippets.com/verilog_booth_multiplier
//http://philosophyforprogrammers.blogspot.com/2011/05/booths-multiplication-algorithm-in.html
// CNHsu 2020/10/6
// modified the original, it has bug
// 2020/10/21
// modify to fit verilog code?? not sure it is ok or not
//
//
/*
  binary of fraction part
  0.0 = 0
  0.1 =0.5
  0.01=0.25
  0.001=0.125
  (8bit)0.00000001=0.00390625
  (12bits) =0.00024414
  (16bits) =0.00001526
  
  caculation consider
  (1) shift by 2**n for praction part 0.00x precision need shift 8bits: 2**8
  (2) int part 8bit, fractin part 8bit (total 16bits) max.  128.xx
  (3) for filter coeficition most bit for fraction part says sign(1bit)int(3bits)fraction(12bits)
  
  calculation flow
  (1)16bits: sign(1bit)int(3bits)fraction(12bits)
  (2)qr x 2**12
  (3) br x qr= max (6.999..)
 

*/
#include <bits/stdc++.h> 
#include <math.h>
  
using namespace std; 
  
void printvalue( int *value, int n)
{
	int i;
	for (i=0;i<n;i++){
		cout << value[i];
	}
}   
  
// function to perform adding in the accumulator 
void add(int ac[], int x[], int qrn) 
{ 
    int i, c = 0; 
 // cout<<" ac: "; printvalue(ac,qrn); cout << "+"; printvalue(x,qrn);   
    for (i = 0; i < qrn; i++) { 
          
        // updating accumulator with A = A + BR 
        ac[i] = ac[i] + x[i] + c; 
          
        if (ac[i] > 1) { 
            ac[i] = ac[i] % 2; 
            c = 1; 
        } 
        else
            c = 0; 
    } 
// cout << "result :"; printvalue(ac,qrn);  cout << endl;   
} 
  
// function to find the number's complement 
void complement(int a[], int n) 
{ 
    int i; 
    int x[16] = { 0}; 
    x[0] = 1; 
 //     cout<<"complement"<< endl;
    for (i = 0; i < n; i++) { 
        a[i] = (a[i] + 1) % 2; 
 //       cout << a[i];
    } 
    
    add(a, x, n); 
} 
  
// function ro perform right shift 
void rightShift(int ac[], int qr[], int& qn, int qrn) 
{ 
    int temp, i; 
    temp = ac[0]; 
    qn = qr[0]; 
      
    cout << "\trightShift\t"; 
      
    for (i = 0; i < qrn - 1; i++) { 
        ac[i] = ac[i + 1]; 
        qr[i] = qr[i + 1]; 
    } 
    qr[qrn - 1] = temp; 
} 


  
// function to display oprations 
void display(int ac[], int qr[], int qrn) 
{ 
    int i; 
      
    // accumulator content 
    for (i = qrn - 1; i >= 0; i--) 
        cout << ac[i]; 
    cout << "\t"; 
      
    // multiplier content 
    for (i = qrn - 1; i >= 0; i--) 
        cout << qr[i]; 
} 
  
// Function to implement booth's algo 
void boothAlgorithm(int br[], int qr[], int mt[], int qrn, int sc) 
{ 
  
    int qn = 0, ac[16] = { 0 }; 
    int i;
    int temp = 0; 
    char booth[200],bt;
    cout << "qn\tq[n+1]\tBR\t\tAC\t\tQR\t\tsc\n"; 
    cout << "\t\tinitial\t\t"; 
      
    display(ac, qr, qrn); 
    cout << "\t\t\t" << sc << "\n"; 
    bt=0;  
    while (sc != 0) { 
        cout << qr[0] << "\t" << qn; 

        // SECOND CONDITION 
        if ((qn + qr[0]) == 1) 
        { 
            if (temp == 0) { 
                  
                // subtract BR from accumulator 
                add(ac, mt, qrn); 
                cout <<'{'<<sc<<')'<< "\tA = A - BR\t"; 
                booth[bt]='-';  bt++;
                for (int i = qrn - 1; i >= 0; i--) 
                    cout << ac[i]; 
                temp = 1; 

                }                          
              
            // THIRD CONDITION 
            else if (temp == 1) 
            { 
                // add BR to accumulator 
                add(ac, br, qrn); 
                cout <<'{'<<sc<<')'<< "\tA = A + BR\t"; 
                booth[bt]='+';  bt++;
                for (int i = qrn - 1; i >= 0; i--) 
                    cout << ac[i]; 
                temp = 0; 

            } 
            cout << "\n\t"; 
            rightShift(ac, qr, qn, qrn); 
            booth[bt]='0'; bt++;
        } 
          
        // FIRST CONDITION 
        else if (qn - qr[0] == 0) 
           { rightShift(ac, qr, qn, qrn); booth[bt]='0';bt++;}
             
        display(ac, qr, qrn); 
         
        cout << "\t"; 
          
        // decrement counter 
        sc--; 
        cout << "\t" << sc << "\n"; 
    } 
// dump int part
    cout << " int part: ";    
    for (temp=0,i = qrn - 1; i >= 0; i--) {
         temp=temp+ac[i];         
        }
    if(temp==8) cout << '1';
    else    
    for (i = qrn - 1; i >= 0; i--){                 
    	if(ac[qrn-1]==1) cout << 1-ac[i];
    	else
        cout << ac[i];       
       }
    cout << "\t"; 
    booth[bt]=0x0; // string end
    printf("booth=%s",booth);    
} 

int dec2bin(int *br, int *qr, int *num){

float a;
double c;
int b,k,l=0,mt[20],n,i,count=0,p;
printf(" booth multiplication br x qr:\n\r");
printf("Enter binary bits number(16):");
scanf("%d",num);
printf("bits num=%d\n\r",*num); 
printf("Enter int only number(br):");
scanf("%lf",&c);
b=c;   // int part to b
a=c-b; // get fraction part to a
printf("%d,%f",b,a);
printf("\n");
i=0;
while(b!=0){
 n=b%2;
 b/=2;
 br[i]=n;       //int part binary
 count+=1;i++; 
}

for(k=count-1;k>=0;k--){
printf("%d",br[k]);  // print int part binary
}

printf(".");        // fractioni part 8bits
for(p=1;p<=*num;p++){
a=a*2; 
l=a;        // *2 mov floating part to int part
mt[p-1]=l;           // get int part '1' or '0'
printf("%d",mt[p-1]); // print int part '1' or '0'
if(l==1)
 a=a-l;         // remove int part, 
}
reverse(br, br + *num); // bor int part, br reverse
printf("\n");
//
l=0; count=0;i=0;
printf("Enter any number(qr):");
scanf("%lf",&c);
// value shift 2**12 and check int part only
//c=c*4096;
b=c;   // int part to b
a=c-b; // get floating part to a
printf("%d,%f",b,a);
printf("\n");

c=c*4096;
b=c;   // int part to b
a=c-b; // get floating part to a
printf("x 2^12 = %d,%f",b,a);
printf("\n");
while(b!=0){
 n=b%2;
 b/=2;
 qr[i]=n;       //int part binary
 count+=1;i++;
 
}
for(k=count-1;k>=0;k--){
printf("%d",qr[k]);  // print int part binary
}

printf(".");        // floating part 8bits
for(p=1;p<=*num;p++){
a=a*2; 
l=a;        // *2 mov floating part to int part
mt[p-1]=l;           // get int part '1' or '0'
printf("%d",mt[p-1]); // print int part '1' or '0'
if(l==1)
 a=a-l;         // remove int part, 
 
}
printf("\n");
reverse(qr, qr + *num); // bor int part, qr reverse

return 0;	
}

  
// driver code 
int main() 
{ 
  
    int mt[16], sc;                              // mt=(-BR) is 2's of BR for A-BR
    int br[20]; // change br, qr array for calculation quantize
    int qr[20];
    int brn, qrn;                               // quantize number
    float v;  
    int i;
    // Number of multiplicand bit 
    brn = 20;
    for(i=0;i<brn;i++) { br[i]=0,qr[i]=0;}
      
    // multiplicand 
    if (dec2bin(br, qr,&brn) !=0){  // cmd line input 0-9 for test
    brn=9;                              // default for demo
    br[0]=0;                            // msb 
    br[1]=0;
    br[2]=0;
    br[3]=0;                            // array[msb] is value lsb, need reverse,  br=0110;
    br[4]=0;
    br[5]=0;
    br[6]=0;
    br[7]=1;
    br[8]=0;
    
    qr[0]=0;
    qr[1]=0;
    qr[2]=1;
    qr[3]=0;
    qr[4]=0;                           //qr=10011, 19
    qr[5]=0;
    qr[6]=0;
    qr[7]=1;
    qr[8]=1;
   }
    
     cout << "br * qr :"; printvalue(br,brn); 
    // copy multiplier to temp array mt[] 
    for (int i = brn - 1; i >= 0; i--) 
        mt[i] = br[i];  
          
    reverse(br, br + brn);            // reverse  br[msb] to br[lsb]
   // reverse(qr, qr + brn); // cn + for booth_modify?
    reverse(mt, mt + brn); 
    complement(mt, brn); 
  //cout << "BR complement mt: ";printvalue(mt,brn);cout << endl; 
    // No. of multiplier bit 
    qrn = brn; 
      
    // sequence counter 
    sc = qrn; 
      
    // multiplier 
 //   int qr[] = {  0,0,0,1,1,0}; 
    cout <<" * " ;printvalue(qr,brn); cout << endl;
    cout << "qr : "; printvalue(qr,brn); cout << endl;   
    reverse(qr, qr + qrn); 
    cout << "QR reverse: ";printvalue(qr,brn);cout << endl;
    cout <<"BR reverse : "; printvalue(br,brn);cout << endl;
    cout <<"mt(~BR) : "; printvalue(mt,brn);cout << endl;
    cout << "qrn: "<< qrn<< endl;
    cout << "sc: "<< sc<< endl;    
  
    boothAlgorithm(br, qr, mt, qrn, sc); 
  
    cout << endl 
         << "Result = "; 
           
    for (i = qrn - 1; i >= 0; i--)       
        cout << qr[i]; 
    for (i=0,v=0;i<qrn;i++){
    	 v=v+qr[i]*pow(2,i);
         }  
         cout << "=" << v;
         printf(" final value (/2^12)= %f", v/4096);
} 