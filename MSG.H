
#ifndef __LDPC_MSG_H__
#define __LDPC_MSG_H__


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PACKWORD_LEN    ( sizeof( unsigned int) * 8 )

#define MAX_MSG_SHOW   20

//////////////////////////////////////////////
class  message {
  public:
        message();
        virtual int create(unsigned int n);
        virtual ~message();
        virtual void show(unsigned int start,unsigned length);
          
  public:
        unsigned int len;

        
};

///////////////////////////////////////////////
class   binary_msg:public message {
   public:
         binary_msg();
         binary_msg(unsigned int n);
         int create(unsigned int n);
         ~binary_msg();

         unsigned val(unsigned i);
         int set(unsigned i, int v);
         int gen_random();
         virtual void show(unsigned int start,unsigned length);
         
   private:
         unsigned int * mbuf;
         unsigned int word_cnt;

};


// i starts from 0
inline unsigned binary_msg::val(unsigned i)  {   return   (((unsigned(*((mbuf)+ (i/32))))>>(32-(i%32)-1))&0x1); }

// i starts from 0
inline int binary_msg::set(unsigned i,int v)
{
     int  k1,k2;
     int *ptr;
    //no check on i +1 >=len    

    k1= i / 32;
    k2= i % 32;
    
    ptr = (int*) (mbuf+k1);
    return (*ptr = ( (v==1) ? ((*ptr) | (0x1 << (32-k2-1))): ((*ptr)&(~ (0x1 << (32-k2-1)) )) ));
 
}



//////////////////////////////////////

class  double_msg: public message {
   public:
       double_msg();
       double_msg(unsigned int n);
       int create(unsigned int n);
       ~double_msg();

       double val(unsigned i);
       double set(unsigned i, double v);
       virtual void show(unsigned int start,unsigned length);
         
   private:
         double     *mbuf;

};

inline double double_msg::val(unsigned i){ return  (double (*(mbuf+i)));  }
  
inline double double_msg::set(unsigned i, double v) { return (*(mbuf+i) = v); }



#endif  __LDPC_MSG_H__

