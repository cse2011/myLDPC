

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "msg.h"
#include "channel.h"


 channel::channel()
{
   type    = CHANNEL_AWGN;
   std_dev = 1;
   expect  = 0;
   
}


 void channel::init_channel(int t,double e,double sd)
{

  type  = t;
  std_dev=sd;
  expect =e;

}


 channel::~channel()
{

}


void channel::transmit
(binary_msg* inm,
 double_msg* outm,
 unsigned int len
)
{
  int i;
  double v,noise;

  // srand( (unsigned) time(NULL) );

  for ( i=0;i< len;i++){
  
    v = double ( inm->val(i) ? 1: (-1)) ; 
    noise = gen_noise();
    outm->set( i, v + noise );
    
  }

  // no noise touch part
  for ( ; i < (inm->len); i++ )
  {
    v = double ( inm->val(i) ? 1: (-1)) ; 
    outm->set( i, v );
  }
  
 
}


void channel::get_probability
( double_msg *msg,
  unsigned int cnt,
  double_msg *lra )
{
  unsigned int j;
  double x,likelihood;

  /////////////////////////////////////////////////////////////
  //   Lr = P(x=0|r)/ P(x=1|r)   
  //      = Pn( error= r+1 ) /Pn(error =r-1)  Pn~ N(mu,sigma2)
  //      = exp( -2r / sigma^2)
  //   then set initially:   p0=t, p1=1

  //   continue ,there will be:
  //          new_lratio = lratio * product( 1+ product((t-1)/(t+1)) );
  //          
  //
  /////////////////////////////////////////////////////////////
  switch ( type ) {

   case CHANNEL_AWGN :
       for (j = 0; j<cnt; j++)
       { 
          x = (double) msg->val( j );
          likelihood  =  (double)exp( - 2* x / (std_dev * std_dev) );
          // lratio : [0,+inf)
          lra->set(j, likelihood );
       }

   case CHANNEL_BSC  :
         /*  lratio = ( d==1) ?  (1-p)/p : p/(1-p);   */
   
         break;
    
   default:
         break;
  } // end switch


}



  
