/*20030709 -fanghui */
#ifndef __GF2MATRIX_H__
#define __GF2MATRIX_H__

#include "msg.h"

class matrix  
{
public:
	unsigned int row_cnt;
	unsigned int col_cnt;
	
	matrix();
	matrix(int row, int col );
    ~matrix();

 public:
    // void reset(int v=0);
    int  create(int row,int col);

    int  gen_sparse (unsigned int algorithm, unsigned int maxi, unsigned int maxj );
    int  no4cycle();

    int  add(matrix &a, matrix &b );
    //int  minus( matrix a );
    int  multiple( matrix &a,matrix &b);
    int  transpose( matrix &g);
    int  invert( matrix &a);
    int  is_eye( );
    int  equal( matrix &a);

    int  gen_msg( message* in_m,  message* out_m );

    int  val(int i, int j);
    int  set(int i, int j, int value);
    int  show(void);

 private:
    binary_msg  matrix_buf;

};


 // i,j start from 0
inline  int matrix::val(int i,int j) {  return   matrix_buf.val( i*col_cnt + j); } 

 // start from 0
inline int matrix::set(int i, int j, int value) { return matrix_buf.set(i*col_cnt +j,value );}




#endif  __GF2MATRIX_H__
