//
//https://www.geeksforgeeks.org/booths-multiplication-algorithm/
// CPP code to implement booth's algorithm 

// CNHsu 2020/10/6
// modified the original, it has bug
//
#include <bits/stdc++.h> 
  
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
    int temp = 0; 
    cout << "qn\tq[n+1]\tBR\t\tAC\t\tQR\t\tsc\n"; 
    cout << "\t\tinitial\t\t"; 
      
    display(ac, qr, qrn); 
    cout << "\t\t\t" << sc << "\n"; 
      
    while (sc != 0) { 
        cout << qr[0] << "\t" << qn; 
          
        // SECOND CONDITION 
        if ((qn + qr[0]) == 1) 
        { 
            if (temp == 0) { 
                  
                // subtract BR from accumulator 
                add(ac, mt, qrn); 
                cout << "\tA = A - BR\t"; 
                  
                for (int i = qrn - 1; i >= 0; i--) 
                    cout << ac[i]; 
                temp = 1; 
            } 
              
            // THIRD CONDITION 
            else if (temp == 1) 
            { 
                // add BR to accumulator 
                add(ac, br, qrn); 
                cout << "\tA = A + BR\t"; 
                  
                for (int i = qrn - 1; i >= 0; i--) 
                    cout << ac[i]; 
                temp = 0; 
            } 
            cout << "\n\t"; 
            rightShift(ac, qr, qn, qrn); 
        } 
          
        // FIRST CONDITION 
        else if (qn - qr[0] == 0) 
            rightShift(ac, qr, qn, qrn); 
         
        display(ac, qr, qrn); 
         
        cout << "\t"; 
          
        // decrement counter 
        sc--; 
        cout << "\t" << sc << "\n"; 
    } 
} 

int dec2bin(char** arg,int *br, int *qr, int *num){
	int i;
	int j;
	int n;
	int *a;
	
	if (arg[1]==NULL || arg[2]==NULL) {
		printf(" no input! \n");
		printf("example for A*B (BR*QR)\n A,B max 0-9\n");
		printf("example: a.exe A B\n");
		return 1;
	}		
	else{ // conver dec to bin
	for(j=1;j<=2;j++){
		n=(int)(*arg[j]-48); // asicc to dec
		printf("argu[%d]=%d %d\n",j,*arg[j],n);		
		if(j==1) a=br;
		else a=qr;
		printvalue(br,4);	
		printvalue(qr,4);
		printvalue(a,4);
		printf("num=%d",*num);
		
	for(i=0; n>0; i++)    
	{    
	a[i]=n%2;    
	n= n/2;  
	} 
	*num=8;   
	cout<<"Binary of the given number= ";    
	for(i=i-1 ;i>=0 ;i--)    
	{   		 
	cout<<a[i];   
	} 
	cout << endl; 
	printf("*num=%d",*num);  	
	reverse(a, a + (*num)); 
	}
	}
return 0;	
}

  
// driver code 
int main(int argc, char** arg) 
{ 
  
    int mt[16], sc;                              // mt=(-BR) is 2's of BR for A-BR
    int br[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // change br, qr array for calculation quantize
    int qr[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
    int brn, qrn;                               // quantize number
      
    // Number of multiplicand bit 
    brn = 8; 
      
    // multiplicand 
    if (dec2bin(arg, br, qr,&brn) !=0){  // cmd line input 0-9 for test
    brn=4;                              // default for demo
    br[0]=0;                            // msb 
    br[1]=1;
    br[2]=1;
    br[3]=0;                            // array[msb] is value lsb, need reverse,  br=0110;
    
    qr[0]=0;
    qr[1]=0;
    qr[2]=1;
    qr[3]=0;                           //qr=0010
   }
    
     cout << "br * qr :"; printvalue(br,brn); 
    // copy multiplier to temp array mt[] 
    for (int i = brn - 1; i >= 0; i--) 
        mt[i] = br[i];  
          
    reverse(br, br + brn);            // reverse  br[msb] to br[lsb]
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
           
    for (int i = qrn - 1; i >= 0; i--) 
        cout << qr[i]; 
} 