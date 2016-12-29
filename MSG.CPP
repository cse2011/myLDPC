
#include "msg.h"

/////////////////////  parent class //////////////
message::message() { len =0;}

message::~message() {}

int message::create(unsigned int n) {return n;}

void message::show(unsigned int start,unsigned length) 
{ 
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
////////// binary_msg class//////////////////////

binary_msg::binary_msg() { len=0; mbuf=0;word_cnt=0; }
   
int binary_msg::create(unsigned int n)
{   
      int k1,k2;
      k1 = n / 32;
      k2 = n % 32; if (k2) k1++;
      
      word_cnt = k1;

      if (mbuf)  free(mbuf);
      mbuf = (unsigned int*)  calloc( word_cnt,sizeof(unsigned int));

      return (len= ((mbuf)? n:0));
      //printf("error:msg allocation!");
}
   
binary_msg::binary_msg(unsigned int n) { create(n); }
  
binary_msg::~binary_msg() { if (mbuf) free(mbuf); }




int binary_msg::gen_random()
{
  int i;
  //  srand( ( unsigned ) time (NULL) );
  
  // randomly set a in_bstream
  
  for (i =0;i<len;i++) {
     set( i,  ( rand()*2 / RAND_MAX) % 2 );
  }
  

  // for (i=0;i<len;i++) set(i,i%2);

  return 1;
}

void binary_msg::show(unsigned int start,unsigned length)
{
  if(length > MAX_MSG_SHOW) {
     printf("...");
     return;
  }
  for (int i=start;i< start+length ;i++)
     printf("%d",val(i));
  
}

/////////////////////////////////////////////////
//////////////class double_msg /////////////////

double_msg::double_msg() { len=0; mbuf=0; }

double_msg::double_msg(unsigned int n) { create(n);}

int double_msg::create(unsigned int n)
{
      if(mbuf) free(mbuf);
      mbuf = (double*)  calloc( n,sizeof( double));
      return (len= ((mbuf)? n:0));
}

double_msg::~double_msg() { if(mbuf) free(mbuf); }


void double_msg::show(unsigned int start,unsigned length)
{
  if(length > MAX_MSG_SHOW) {
     printf("...");
     return;
  }
  
  for (int i=start;i< start+length ;i++) 
      printf("%+4.2f ",val(i));
  
}
  
