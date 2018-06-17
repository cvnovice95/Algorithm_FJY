#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* [0,7]f=x^2+y^2; */
#define Pc		0.9     /* 交叉的概率 */
#define Pv		0.1     /* 变异概率 */
#define ChromosomeNum	4       /* 染色体个数 */
#define CodeWidth	6
#define CountTf		500
#define AIM_VALUE	98
typedef struct ChromosomeInformation {
	unsigned char	Code[CodeWidth];
	unsigned char	X;
	unsigned char	Y;
	float		fitValue;
}CI;
CI	ci[ChromosomeNum];
int	A, B;
float	MAX	= 0;
int	MAX_I	= 0;
int RouletteWheelSelection()
{
	int	i;
	float	m, r;
	m	= 0;
	r	= rand() % 11 / 10.0;
	printf( "Debug[RouletteWheelSelection]:r is %f \n", r );
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		m = m + ci[i].fitValue;
		printf( "Debug[RouletteWheelSelection]:<%d>  m is  %f  {%d} is %f\n", i, m, i, ci[i].fitValue );
		if ( r <= m )
		{
			return(i);
		}
	}
}


void select()
{
	A = RouletteWheelSelection();
	printf( "Debug[select]: A is %d \n", A );
	B = RouletteWheelSelection();
	printf( "Debug[select]: B{0} is %d \n", B );
	if ( A == B )
		B = (B + 1) % ChromosomeNum;  /* 防止自交 */
	printf( "Debug[select]: B{1} is %d \n", B );
}


/* 单点交叉 */
void cross()
{
	unsigned char crossPoint, i, t;
	crossPoint = rand() % (CodeWidth);
	printf( "Debug[cross]: randcrossPoint is %d \n", crossPoint );
	for ( i = crossPoint; i > 0; i-- )
	{
		t		= ci[A].Code[i];
		ci[A].Code[i]	= ci[B].Code[i];
		ci[B].Code[i]	= t;
		printf( "Debug[cross]: randcrossPoint is %d \n", crossPoint );
		printf( "Debug[cross]: CrossCount id <%d> \n", i );
	}
}


void variation()
{
	unsigned char crossPoint;
	crossPoint		= rand() % CodeWidth;
	ci[A].Code[crossPoint]	= 1 - ci[A].Code[crossPoint];
	crossPoint		= rand() % CodeWidth;
	ci[B].Code[crossPoint]	= 1 - ci[B].Code[crossPoint];
	printf( "Debug[variation]:variation \n" );
}


void encoding()
{
	int		i, j, k;
	unsigned char	rnum;
	unsigned char	bit;
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			rnum = rand() % 8;
			/* printf("Debug[encoding]:Number is %d \n",rnum); */
			for ( k = 0; k < 3; k++ )
			{
				bit			= (rnum << (5 + k) ) & 0x80;
				bit			= (bit) ? 1 : 0;
				ci[i].Code[j * 3 + k]	= bit;
			}
		}
		/* printf("Debug[encoding]:Count is <%d> %d %d %d %d %d %d\n",i,ci[i].Code[0],ci[i].Code[1],ci[i].Code[2],ci[i].Code[3],ci[i].Code[4],ci[i].Code[5]); */
	}
}


void decoding()
{
	int		i;
	unsigned char	X, Y;
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		X	= pow( 2, 2 ) * ci[i].Code[0] + pow( 2, 1 ) * ci[i].Code[1] + pow( 2, 0 ) * ci[i].Code[2];
		Y	= pow( 2, 2 ) * ci[i].Code[3] + pow( 2, 1 ) * ci[i].Code[4] + pow( 2, 0 ) * ci[i].Code[5];
		ci[i].X = X;
		ci[i].Y = Y;
		printf( "Debug[decoding]:Count <%d> X is %d Y is %d \n", i, X, Y );
	}
}


void CalculateP()
{
	float	sum;
	int	i;
	sum = 0.0f;
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		sum += pow( ci[i].X, 2 ) + pow( ci[i].Y, 2 );
		printf( "Debug[CalculateP]:sum is %f \n", sum );
	}
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		ci[i].fitValue = (pow( ci[i].X, 2 ) + pow( ci[i].Y, 2 ) ) / sum;
		printf( "Debug[CalculateP]:<%d> fitValue is %f\n", i, ci[i].fitValue );
	}
	for ( i = 0; i < ChromosomeNum; i++ )
	{
		MAX = ( (pow( ci[i].X, 2 ) + pow( ci[i].Y, 2 ) ) > MAX) ? (pow( ci[i].X, 2 ) + pow( ci[i].Y, 2 ) ) : MAX;
		if ( MAX == (pow( ci[i].X, 2 ) + pow( ci[i].Y, 2 ) ) )
			MAX_I = i;
	}
	printf( "Debug[CalculateP]:MAX_I is <%d>\n", MAX_I );
}


int main( int argc, char*argv[] )
{
	int	i, count;
	float	Rc;
	float	Rv;
	srand( (unsigned) time( NULL ) );
	encoding();
	decoding();
	CalculateP();
	count = 0;
	do
	{
		decoding();
		CalculateP();
		select();
		Rc = (rand() % 11) / 10.0;
		if ( Rc < Pc )
		{
			cross();
		}
		Rv = (rand() % 11) / 10.0;
		if ( Rv < Pv )
		{
			variation();
		}
		count++;
		printf( "Debug[main]:<%d>  %f\n", count, MAX );
	}
	while ( count < CountTf && MAX != AIM_VALUE );

	return(0);
}