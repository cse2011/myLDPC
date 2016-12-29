#include "channel.h"
#include "matrix.h"
#include "msg.h"

#define MAX_ITERATION 20

class ldpc_codes
{
  public:
      matrix  P;  /*
        Hx=0 ,where H=(P,I) ,x=transpose(s,c)
        then   c= PS mod2.

        let G= transpose(I,P) ,then  x= Gs.
        obviously , HG=0.

        order :   Pm,k    Hm,k+m   Gk+m,k
        k is data bit len (s) , m is check bit length (c).
        x length = n =k+m
        

      */
      

      unsigned c_len; // check  length  M
      unsigned s_len; // symbol length  K
      unsigned x_len; // whole length   N


      binary_msg    in_msg;  // length =N ,store raw/encoded data

      binary_msg    out_msg; // length =N, store decoded data

      double_msg    rcv_msg;   // use double type to deal with noise

      double_msg    lratio;  /* length=N, likelihood ratio:  */
                             /* probability of 0 conditional on recv */
                             /* divided by probability of 1 conditional on recv */
                             /* liklihood_ratio = p0/p1,[0,+inf ) */

      channel Ch;

  public:
      ldpc_codes();
  void  init_ldpc(int data_len,int check_len);
       ~ldpc_codes();


  public:
      void encode();
      unsigned  decode();

      unsigned int bit_changed();
      double  bit_error_rate();
      double  expected_error();
      int   parity_check();
      int   ldpc_iteration();
      void  init_probability();
      void  show( message m );

  private:
      void  prob_decode();
      int   adjust_ratio();
      double phi( double beta );


      

 };


inline double ldpc_codes::phi( double beta )
{
  return  log( (exp(beta) +1) / (exp(beta) - 1) ) ;
}

