
#include <stdio.h>
#include <math.h>

#include "ldpc.h"
//#include "matrix.h"



//demo
void demo_msg(void)
{
  binary_msg  a(11);
  binary_msg  b;
  double_msg  c(12);

  b.create(33);
  a.gen_random();
  b.set(8,1);b.set(9,1);b.set(32,1);
  
  printf("A:");
  a.show(0,a.len); printf("\r\nB:");
  b.show(0,b.len);printf("\r\n");
  b.show(7,12);

  c.set( 10,0.11) ; c.set(11,0.12);
  printf("\r\n"); c.show(0,c.len); 
  printf("\r\n"); c.show(9,3);

}

 void demo_rand()
{
  double a, b,g;
  int i;
  printf("\r\n Rand 1:");  
  for (i=0; i< 10; i++) {
  
    srand( (unsigned) time(NULL) );
    a = (double) rand() / (double) RAND_MAX;
    b = (0.5+ (double) rand() ) / (1.0+ (double)RAND_MAX);
    g= cos(2.0*M_PI*a) * sqrt(-2.0*log(b));

    printf("%4.2f ", g);
    
  }

  printf("\r\n Rand 2:");
  srand( (unsigned) time(NULL) );
  for (i=0; i< 10; i++) {
 
    a = (double) rand() / (double) RAND_MAX;
    b = (0.5+ (double) rand() ) / (1.0+ (double)RAND_MAX);
    g= cos(2.0*M_PI*a) * sqrt(-2.0*log(b));

    printf("%4.2f ", g);
    
  }

 
}

void demo_matrix( )
{
 int k,m,i,j,v;
 matrix X1,X2,X3,X4,X5,Z,W,Y1,Y2;

 X2.create( 3,3);
 X2.set(0,0,1); 
                X2.set(1,1,1); X2.set(1,2,1);
 X2.set(2,0,1);                X2.set(2,2,1);
 printf("X2=\r\n"); X2.show();

 X3.create( 3,3);
 X3.set(0,0,1);
 X3.set(1,0,1);  X3.set(1,1,1); X3.set(1,2,1);
 X3.set(2,0,1);                 X3.set(2,2,1);
 printf("X3=\r\n"); X3.show();

 W.multiple(X2,X3);printf("X2 * X3 =\r\n");W.show();
 
 printf("Invert X2\r\n");
 if ( ! Y1.invert(X2) ) printf("det(X2)=0!");
 else  Y1.show();


 k = 505;m=505;
 X1.create( m,k );
 for (i=0; i< X1.row_cnt; i++) {
       X1.set(i,i,1);
       for(j=i+1;j< X1.col_cnt;j++) {
         v = (( rand() * 2 ) / RAND_MAX) % 2;
         X1.set(j,i, v);
       }
 }
 printf("X1 =");X1.show();

 printf("inv(X1) =");
 if ( ! Z.invert(X1) ) printf("det(X1)=0!");
 else {
   Z.show();
   X2.invert(Z); printf("new X1"); X2.show();
   if ( X2.equal(X1) ) printf("OKOKOK");
   Y2.multiple(X1,Z); printf("X1* inv(X1)=");Y2.show();
   if( Y2.is_eye() ) printf("OK!!!!!");
 }

}



int main(void)
{

 
 int N,K,M;
 int sparse_row,sparse_col,sparse_algo;
 double   sigma,mu;
 ldpc_codes  sys;
 int   chan_type;
 int   max_iteration;

 K=8000;
 M=2000;
 N=K+M;
 
 chan_type  = 0; //awgn
 mu = 0;
 sigma =0.65;

 sparse_row = 4;
 sparse_col = 10;
 sparse_algo = 3;

 max_iteration = 20;
 
 srand( (unsigned) time(NULL) );

 sys.init_ldpc(K,M);   // set the check matrix

 sys.P.gen_sparse(sparse_algo,sparse_row,sparse_col);
 sys.P.show(); 

 printf("Raw    : ");
 sys.in_msg.show(0,sys.x_len);

 sys.encode(); //add check bits
 printf("\r\nEncoded: ");
 sys.in_msg.show(0,sys.x_len);

  
 sys.Ch.init_channel(chan_type,mu,sigma);  // set the channel
 sys.Ch.transmit( &sys.in_msg,&sys.rcv_msg,sys.x_len);
 printf("\r\nTransmit:");
 sys.rcv_msg.show(0,sys.x_len);
 printf("\r\n");


 sys.decode();



 //demo_rand();
 
 // demo_matrix();


 
 return 1;
}



