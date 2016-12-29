#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "matrix.h"

matrix::matrix()
{
	row_cnt=0;
	col_cnt=0;
}

matrix::~matrix()
{}

matrix::matrix(int row, int col)
{
	//  if (row < 0 || col < 0)	return;
    row_cnt=row;
    col_cnt=col;

    if( !matrix_buf.create(row*col)) return ;
    
   
}

int matrix::create(int row, int col)
{
   //if ( row <0 || col <0 ) return 0;
   row_cnt =row;
   col_cnt =col;

   return matrix_buf.create(row*col) ;
}


// create  a sparse parity-check matrix 
int  matrix::gen_sparse
(unsigned int algorithm,
 unsigned int maxi,
 unsigned int maxj )
{
 unsigned int i,j, choose;

 //maxj = (col_cnt / row_cnt) +1 ;
 //maxj = 5;

 switch(algorithm) {
   case 0:
      // example:  6 *  9
      set(0,0,1);set(0,1,1);set(0,2,1);
      set(1,3,1);set(1,4,1);set(1,5,1);
      set(2,6,1);set(2,7,1);set(2,8,1);
      set(3,0,1);set(3,3,1);set(3,6,1);
      set(4,1,1);set(4,4,1);set(4,7,1);
      set(5,2,1);set(5,5,1);set(5,8,1);  
      break;

   case 1:
     break;
     
   default:
     // srand( (unsigned) time(NULL) );
     for (i=0; i< row_cnt; i++) {
       for(j=0;j< maxj;j++) {
         choose = ( rand() * col_cnt ) / RAND_MAX;
         set( i,choose, 1);
       }
     }

     no4cycle();
     
     break;
  }

 return 1;
}

//////////////////////////////////////////////////
/* Eliminate cycles of length four if possible. */
/*  4-cycle definition:   if two code bits are both checked by the */
/*  same pair of parity-check equations */
//             
//             2(k,j)             (k,m)
//               ^
//               |      ====>        3
//               |
//      i-->   1(i,j)             (i,m)
////////////////////////////////////////////////////
int matrix::no4cycle()
{

  int i,j,k,r,m,found;

  found =0;
  // srand( (unsigned) time(NULL) );

  for ( i= 1; i < row_cnt ; i++ ) {
         for ( j =0 ; j < col_cnt ; j ++ ) {
              if ( val(i,j) == 1 ) /* locate (i,j) =1 */
              
                for ( k =0; k < i ; k++ ){
                    if ( val(k,j) == 1 ) {
                        do {
                          for ( m=0; m < col_cnt;m++ ){
                           found = 0;
                           if (  (m!=j) && (val(i,m)==1) && (val(k,m)==1) ) {
                              found =1;

                              do r = ( rand() *  col_cnt ) /RAND_MAX;
                              while ( r ==m || r==j || ((val(i,r)==1) && (val(k,r)==1)));

                              // move to (i,r)
                              set(i,m,0);set(i,r,1);
                              

                           }
                          }
                        } while ( found );

                    }
                    
                }

         }
  
  }
  return 1;     

}

int  matrix::add(matrix &a, matrix &b )
{

return 1;
}

// use reference to avoid writing
int  matrix::multiple( matrix &a,matrix &b)
{
  int i,j,k,c;

  if ( a.col_cnt != b.row_cnt )  return 0;
  if ( ! create( a.row_cnt,b.col_cnt ) ) return 0;

  for ( i=0;i< row_cnt ; i++ )
    for ( j=0; j < col_cnt; j++ )
    {
       c = 0;
       for ( k=0 ; k < a.col_cnt; k ++ )
       {
         c +=  (a.val(i,k)* b.val(k,j)) ;
       }

       set( i,j, c%2);
    }

  return 1;
}

int  matrix::transpose( matrix &g)
{
   int i,j;

   if ( !create( g.col_cnt, g.row_cnt ) ) return 0;
   
   for (i=0;i < row_cnt; i ++ )
    for ( j=0 ;j <col_cnt ;j ++ )
         set( i,j , g.val(j,i) );

   return 1;         
}

int  matrix::is_eye( )
{
  int i,j;
  
  if ( row_cnt != col_cnt ) return 0;
 
  for (i =0 ; i< row_cnt; i++)
     for ( j=0 ; j< row_cnt ; j++ )
          if ( ( (i != j) && val(i,j) ==1 ) ||
               ( (i == j) && val(i,j) !=1 ) ) 
           return 0;

  return 1;           
}

int  matrix::equal( matrix &a)
{
  int i,j;
  
  if ( (row_cnt != a.row_cnt) ||
       (col_cnt != a.col_cnt) ) return 0;
 
  for (i =0 ; i< row_cnt; i++)
     for ( j=0 ; j< row_cnt ; j++ )
          if ( val(i,j) !=a.val(i,j) )
          return 0;

  return 1;           

}

/* Use gauss elimination by row */
int  matrix::invert( matrix &aa) 
{ 
  int i,j,k,m,n,t,k0;
  int c,d,e,f;
  matrix a;

  #define ROW_MINUS(i,j)     for ( t = 0; t <k0; t++ ){                          \
                                 a.set( j,t , (a.val(j,t) + a.val(i,t) ) %2 ); \
                                   set( j,t , (  val(j,t) +   val(i,t) ) %2);  }

  #define ROW_EXCHANGE(j,i)  for ( t =0;t < k0;t++ ) {                              \
                              c = a.val(j,t); a.set(j,t, a.val(i,t)); a.set(i,t,c); \
                              c =   val(j,t);   set(j,t,   val(i,t));   set(i,t,c); }

  if ( aa.row_cnt != aa.col_cnt ) return 0;

  if ( ! create( aa.row_cnt,aa.col_cnt) ) return 0;
  if ( ! a.create( aa.row_cnt,aa.col_cnt) ) return 0;
  /* in order not to modify the msg buf in aa,another copy: a is created */

  k0 = a.row_cnt;

  /*  C itself is k0 x k0 idnetity matrix ,and a is copy of aa  */
  for (i =0 ; i< k0; i++)
     for ( j=0 ; j<k0; j++ )
     {
          set(i,j, (i==j) ? 1:0 );
          a.set(i,j, aa.val(i,j));

     }          
  

  // start from column j, assume the <j is identity matrix already
  for ( j= 0; j < k0 ; j++ )
  {
     for ( i =j ; i < k0 ; i++) 
     {
         if  ( a.val(i,j) == 1 )  break;
     }
     if ( i>=k0 ) /* No new 1 in column j found */
         return 0;

     //  exchange the row i to row j
     if ( i  != j )
     {
         ROW_EXCHANGE(j,i);
     }

     //eliminate the rest 1 in column j
     for ( m = j+1; m <k0; m++)
     {
         if ( a.val(m,j) == 1 )
         {
            ROW_MINUS( j , m);
         }
     }

   }

   // now got upper triangle matrix
   // and still eliminate the upper 1's
   for ( j = k0-1; j >= 0; j-- )
   {
      for ( i =0;i < j ;i++ )
      {
          if ( a.val( i,j) == 1 )
          {
            ROW_MINUS(j,i);
          }
      }

   }


 return 1; 
}

int  matrix::gen_msg( message* in_m,  message* out_m )  { return 1; }


int matrix::show(void)
{

   int i,j;
   printf( "Matrix (%2d,%2d):\r\n",row_cnt,col_cnt);

  if( /*0*/  col_cnt > MAX_MSG_SHOW  ) {
     printf("...\r\n");
     return 0;
  }

  for  ( i=0;i < row_cnt; i++)
  {
      for (j=0;j < col_cnt; j++)
       {
          printf( "%d ",val(i,j));
   
       }
      printf("\r\n");
  }
   
  return 1;
}       
