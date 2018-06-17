#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BIT_COUNT 8
typedef struct _operator {
	unsigned char	max_bit;
	unsigned char	bit[9];
}operator;
void InitOperator( operator * in )
{
	int i;
	for ( i = 0; i <= BIT_COUNT; i++ )
	{
		in->bit[i] = 0;
	}
}


void Encode( operator * in, unsigned char Source )
{
	int i, temp;
	for ( i = 0; i < BIT_COUNT; i++ )
	{
		temp				= (Source << i);
		temp				= (temp & 0x80);
		in->bit[BIT_COUNT - 1 - i]	= (temp > 0) ? 1 : 0;
	}
	/* DEBUG */
	printf( "[Encode]:Source is %x Encode is ", Source );
	for ( i = BIT_COUNT - 1; i >= 0; i-- )
	{
		printf( "%d", in->bit[i] );
	}
	printf( "\n" );
}


void printf_code( operator* in )
{
	int i;
	/* DEBUG */
	printf( "[printf_code]:Encode is " );
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		printf( "%d", in->bit[i] );
	}
	printf( "\n" );
}


unsigned char Decode( operator * in )
{
	int		i;
	unsigned char	result;
	for ( i = 0; i < BIT_COUNT; i++ )
	{
		if ( in->bit[i] == 1 )
		{
			result += pow( 2, i );
		}
	}
	return(result);
}


void  Check_Max_bit( operator * in )
{
	int i;
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		if ( in->bit[i] == 1 )
		{
			in->max_bit = i;
			break;
		}
	}
	printf( "[Check_Max_bit]%d\n", in->max_bit );
}


void copy_r( operator *in, operator *out )
{
	int i;
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		out->bit[i] = in->bit[i];
	}
	/* DEBUG */
	printf( "[copy_r] out is" );
	printf_code( out );
}


void offset_q( operator * in, int offset )
{
	in->bit[offset] = 1;
}


void offsetbit( operator *in, operator * out, int offset )
{
	int i;
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		out->bit[i] = 0;
	}
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		if ( in->bit[i] == 1 )
		{
			out->bit[i + offset] = 1;
		}
	}
	/* DEBUG */
	printf( "[offsetbit] Out is" );
	printf_code( out );
}


void xor (operator * in, operator * out) {
	int i;
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		in->bit[i] = (in->bit[i]) ^ (out->bit[i]);
	}
	/* DEBUG */
	printf( "[xor] In is" );
	printf_code( in );
}
int Div( operator * dsor, operator *dend, operator * q, operator*r )
{
	int		dsor_max_bit, dend_max_bit, offset;
	operator	*temp = (operator *) malloc( sizeof(operator) );
	/* InitOperator(temp); */

	while ( 1 )
	{
		Check_Max_bit( dsor );
		Check_Max_bit( dend );
		dsor_max_bit	= dsor->max_bit; /* 除数 */
		dend_max_bit	= dend->max_bit;
		if ( dsor_max_bit < dend_max_bit )
		{
			copy_r( dsor, r );
			free( temp );
			temp = NULL;
			break;
		}
		offset = dsor_max_bit - dend_max_bit;
		offset_q( q, offset );
		printf_code( q );
		offsetbit( dend, temp, offset );
		xor (dsor, temp);
	}
	return(0);
}


void copy( operator * a, operator *b )
{
	int i;
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		a->bit[i] = b->bit[i];
	}
}


void Mul( operator *A, operator *q, operator *B, operator * Out )
{
	int		i;
	operator	tempA;
	operator	tempB;
	InitOperator( &tempA );
	InitOperator( &tempB );
	for ( i = BIT_COUNT; i >= 0; i-- )
	{
		if ( q->bit[i] == 1 )
		{
			offsetbit( B, &tempB, i );
			xor (&tempA, &tempB);
		}
	}
	xor (&tempA, A);
	copy( Out, &tempA );
	/* DEBUG */
	printf( "[Mul] Out" );
	printf_code( Out );
}


int  result_opppsite( unsigned char input )
{
	/* Init */
	unsigned char	result;
	operator	A1;
	operator	A2;
	operator	A3;
	operator	tA1;
	operator	tA2;
	operator	tA3;
	operator	B1;
	operator	B2;
	operator	B3;
	operator	Q;
	operator	R;
	operator	SaveA3;
	unsigned char	Ro;
	InitOperator( &A1 );
	InitOperator( &A2 );
	InitOperator( &A3 );
	InitOperator( &B1 );
	InitOperator( &B2 );
	InitOperator( &B3 );
	InitOperator( &tA1 );
	InitOperator( &tA2 );
	InitOperator( &tA3 );
	InitOperator( &Q );
	InitOperator( &R );
	InitOperator( &SaveA3 );

	Encode( &A1, 0x01 );
	Encode( &A2, 0x00 );
	Encode( &A3, 0x1B );
	A3.bit[8] = 1;

	Encode( &B1, 0x00 );
	Encode( &B2, 0x01 );
	Encode( &B3, input );

	while ( 1 )
	{
		copy( &SaveA3, &A3 );
		InitOperator( &Q );
		Div( &A3, &B3, &Q, &R );
		copy( &tA1, &B1 );
		copy( &tA2, &B2 );
		copy( &tA3, &B3 );
		Mul( &A1, &Q, &B1, &B1 );
		Mul( &A2, &Q, &B2, &B2 );
		Mul( &SaveA3, &Q, &B3, &B3 );
		if ( Decode( &B3 ) == 0x01 )
		{
			printf( "[result_opppsite]: B2 is" );
			printf_code( &B2 );
			result = Decode( &B2 );
			printf( "[result_opppsite]: B2 Code is %x \n", result );
			break;
		}
		copy( &A1, &tA1 );
		copy( &A2, &tA2 );
		copy( &A3, &tA3 );
	}
	return(0);
}


int main( int argc, char *argv[] )
{
	result_opppsite( 0x95 );
	return(0);
}