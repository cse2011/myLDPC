#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sunmath.h>

#include "ldpc.h"



ldpc_codes::ldpc_codes()
{
   c_len = 0;
   s_len = 0;
   x_len = 0;

}

void ldpc_codes::init_ldpc(int data_len,int check_len)
{
  int i,j;
 
  if( ! P.create(check_len,data_len) ) { printf("P MemErr");return ; }
  c_len=check_len;
  s_len=data_len;
  x_len = c_len + s_len;

  if( ! in_msg.create(x_len)  ) { printf("in_msg MemErr");return ; }
  if( ! out_msg.create(x_len) ) { printf("out_msg MemErr");return ; }

  if ( !rcv_msg.create(x_len) ) { printf("rcv_msg MemErr");return ; }
  if ( !lratio.create(x_len) ) { printf("lratio MemErr");return ; }
    
  in_msg.gen_random();
  
  
}

ldpc_codes::~ldpc_codes()
{}

void ldpc_codes::encode()
{
 int i,j;

 // add check bit value to in_msg
 for(j=0;j< P.row_cnt; j++) {
    unsigned int cj=0;
    for (i=0; i< P.col_cnt; i++) {
        
      cj+= (P.val(j,i) *  in_msg.val(i)) ;

    }

    in_msg.set( s_len +j ,cj%2 );
   
 }

}

int ldpc_codes::adjust_ratio()
{
   
   int i;
   /*
   for (i=0;i<x_len;i++) {
      if ( isinf( lratio.val(i) ) || 
           isnan( lratio.val(i) ) )
      {  
        printf("NaN!!\r\n");
        lratio.set(i,1);
      }
   }
   
   */
   return 0;
}   
   

//used in decoding procedure
void  ldpc_codes::init_probability()
{

  unsigned int j,c;
  double x,y,lr;

  Ch.get_probability( &rcv_msg,x_len, &lratio );
  //adjust_ratio();

  y =expected_error();
  printf("Block length =%d,Code rate =%3.2f\r\n",x_len,double((double) s_len/ (double) x_len) );
  printf("Error standard deviation=%3.2f,SNR=%+3.2f dB\r\n", Ch.std_dev,Ch.SNR());
  printf("Channel capacity =%3.2f\r\n",Ch.capacity());
  printf("Expected errors =%5.1f,     Expected BER =%7.6f\r\n", y,(y/(double)x_len) );

  // first time decode a binary out_msg from double rcv_msg
  prob_decode();
  
  printf("Initial: ");
  out_msg.show(0,out_msg.len);
  c=parity_check();
  printf(" Error %4d   BER %7.6f Parity %d  ",bit_changed(),bit_error_rate(),c);
  if ( c==0 )  printf("<--Valid");  printf("\r\n");

}


/* Returns the number of parity checks violated by dblk. */
int ldpc_codes::parity_check()
{
  unsigned int i,j,violation;

  violation =0;
  
  for(j=0;j< P.row_cnt; j++) {
    unsigned int cj=0;
    for (i=0; i< P.col_cnt; i++) {
        
      cj += (P.val(j,i) *  out_msg.val(i)) ;

    }

    if ( (cj%2) !=  out_msg.val( s_len +j ))  /*Not satisfy the parity-check equation */
            violation ++;
  }
  return  violation;
}


int ldpc_codes::ldpc_iteration()
{
  int N, M,K;
  int i, j,k;
  double Pc,Pd,Pext;

  double_msg  tmp_lr;

  #define M   (c_len)
  #define K   (s_len)
  #define N   (x_len)

  if( ! tmp_lr.create( N) ) { printf("MemErr!!"); return 0; }

  //recompute the data probability
  for (j=0; j<K;j++) {
     // select bit Sj from x =(s,c)
     
     Pext = 1;

     for( i=0;i<M;i++) {
       if ( P.val(i,j) ==1 ) // find a parity-check equation(i)
       {
         // must multiply the single i check prob also.
         Pc = ( lratio.val(K+i)  - 1 )/ ( lratio.val( K+i) + 1 );
         
         for (k=0; k < K ; k++)
         {
           if ( (k != j) && (P.val(i,k) ==1) )  // data k in equation
           {
               Pc *= ( lratio.val(k) - 1 )/ ( lratio.val(k) + 1 );
           }
          
         }

         Pext  *= ( 1+Pc) /(1-Pc);
         if ( isnan(Pext) ) Pext =1;
       
       } //end if
     } //end for i

    Pd = lratio.val(j) * Pext ; 
    if ( isnan(Pd) )    Pd = 1;
    
    // save the new ratio value for j data
    tmp_lr.set(j,Pd);
           
  } // end for j

  //recompute the check digit probability
  // note:since a simple parity-check equation is used, 
  // there's only one equation related to one check bit.
  for ( i=0;i <M ;i++){
  
       // likelihood_ratio (check i);
       Pc =1;
       for ( j=0;j< K; j ++) {
         if ( P.val(i,j) == 1 )
            Pc *= ( lratio.val(j) - 1 ) / ( lratio.val(j) + 1 );
       }

       Pc = ( 1+ Pc ) /( 1-Pc );
       // Pc is between [0,+inf)
       if ( isnan(Pc) ) Pc=1;

       Pd = lratio.val(K+i) * Pc;  // Pc is also Pext
       if ( isnan(Pd) ) Pd=1;

       //save the new prob ratio for check (i)
       tmp_lr.set(K+i,Pd);

  }

  // now the whole new Lratio is saved in tmp_lratio
  // replace the old one.
  for ( i=0;i<N;i++)  lratio.set(i, tmp_lr.val(i) );

 //  adjust_ratio( );


  return 1;

}

void  ldpc_codes::prob_decode()
{
   double  t;
   unsigned int k;

   for (int i=0;i< x_len;i++) {
      t = (lratio.val(i) );
      k = (  (t >=1) ? 0:1 ); // not sure yet how to do when t=1
      
      out_msg.set( i,  k);
   }

}

unsigned ldpc_codes::decode()
{ 
  unsigned int  n,c;

  init_probability();

  printf("\r\n");
  
  for (n = 1;n< MAX_ITERATION ; n++)
  { 
    c = parity_check();
 //   if ( c==0)  // all parity check equations satisfied 
 //        break; 
    ldpc_iteration();

    prob_decode();  //according to lratio ,set the new decode result

    printf("Iter%2d : ",n);
    out_msg.show(0,out_msg.len);
    c= parity_check();    
    printf(" Error %4d   BER %7.6f Parity %d  ",bit_changed(),bit_error_rate(),c);
    if ( c==0 )  printf("<--Valid"); printf("\r\n");
   
  }

  printf("\r\nRepeat %d times\r\n",n);
  return n;
}

unsigned int ldpc_codes::bit_changed()
{
 unsigned int i,ber;

 ber =0;
 for (i=0; i< x_len;i++ ){
     if ( in_msg.val(i) != out_msg.val(i) ) ber++;
 }

 return ( ber );

}


double   ldpc_codes::bit_error_rate()
{
 return ( (double) bit_changed() / (double) x_len );
}



void  ldpc_codes::show( message m)
{
  m.show(0,m.len);

}




/* Compute expected number of parity check errors with respect to the distribution
   given by the bit probabilities passed, with bits assumed to be independent. 
 */

double ldpc_codes::expected_error()
{ 
  double ee, p;
  int i, j;

  //   E(e) = sum( E(ei) ) 
  //        = sum (  (1-p)*P(inbit i =0) + p* P(inbit i =1) )
  //
  ee = 0;
  for (i = 0; i< x_len; i++)
  { 
    if(  in_msg.val(i) ==0 )  
        p = 1 / ( lratio.val(i) + 1 );
    else  p = lratio.val(i) / ( lratio.val(i) + 1 );

    ee += p;
  }

  return ee;
}



