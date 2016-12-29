#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "msg.h"


#define   CHANNEL_AWGN    0
#define   CHANNEL_BSC     1



class channel {

public:

   int   type;
   double std_dev;		/* Noise standard deviation for AWGN */
   double expect;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


public:
      channel();
      void init_channel(int t,double ex,double std_dev);
      ~channel();

      double SNR(void);
      double capacity(void);

      void transmit(binary_msg* ,double_msg*  , unsigned int );
      void get_probability( double_msg *msg,unsigned int cnt,double_msg *lra );

private:
      double rand_uniopen (void);
      double rand_uniform (void);
      double rand_gaussian (void);
      double gen_noise();

      
};      

inline double channel::rand_uniform()
{ return (double) rand() /(1.0+ (double) RAND_MAX); }

inline double channel::rand_uniopen()
{
  return (0.5+ (double) rand() ) / (1.0+ (double)RAND_MAX);
}


inline double channel::rand_gaussian ()
{
  double a, b;

  a = rand_uniform();
  b = rand_uniopen();
 
  return cos(2.0*M_PI*a) * sqrt(-2.0*log(b));
}

// Eb/N0 = 1/ sigma^2 ,then converted into dB format
inline double channel::SNR(void)
{
  return  ( -20* log10(std_dev) );

}

// how many bits per symbol
inline double channel::capacity(void)
{
  return  ( log( 1+ ( 1/ (std_dev*std_dev) ) ) ) ;

}

inline double channel::gen_noise()
{
  double  n;
  switch ( type ) {
    case CHANNEL_AWGN :
       n =  std_dev * rand_gaussian();
       break;
       
    case CHANNEL_BSC  :
       n  = 0;
       /*  noise =   rand_uniform  < error_probability  */
       
       break;
    
    default:
       n  = 0;
       break;
  }

  return n;

}



#endif __CHANNEL_H__
