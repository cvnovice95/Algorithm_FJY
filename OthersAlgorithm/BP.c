#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DataCount	820
#define InC		2
#define OutC		1
#define NerveCell	45
#define TrainC		20000
#define Aw		0.2
#define Bw		0.4
#define aw		0.2
#define bw		0.3
double	data_in[DataCount][InC], data_out[DataCount][OutC];  /* DataCount count data item */
double	In_w[NerveCell][InC], ONerve[NerveCell], Out_w[NerveCell][OutC];
double	din_w[NerveCell][InC], dout_w[NerveCell][OutC];
double	MinIn[InC], MaxIn[InC], MinOut[OutC], MaxOut[OutC];
double	OutData[OutC];
double	e;


void GenerateTrainData()
{
	float	a, b, c;
	FILE	* fp = fopen( "data.txt", "w+" );
	if ( fp == NULL )
	{
		printf( "OPEN FILE FAILED!\n" );
	}
	int i;
	for ( i = 0; i < DataCount; i++ )
	{
		a	= rand() % 1000 / 100.0;
		b	= rand() % 1000 / 100.0;
		c	= a + b;
		fprintf( fp, "%lf   %lf   %lf\n", a, b, c );
	}
	fclose( fp );
}


void ReadData()
{
	FILE * fp = fopen( "data.txt", "r+" );
	if ( fp == NULL )
	{
		printf( "OPEN FILE FAILED!\n" );
	}
	int i, j;
	for ( i = 0; i < DataCount; i++ )
	{
		fscanf( fp, "%lf   %lf   %lf", &data_in[i][0], &data_in[i][1], &data_out[i][0] );
	}
	fclose( fp );
}


void InitBP()
{
	int i, j;
	for ( i = 0; i < InC; i++ )
	{
		MinIn[i]	= data_in[0][i];
		MaxIn[i]	= data_in[0][i];
		for ( j = 0; j < DataCount; j++ )
		{
			MinIn[i]	= (data_in[j][i] < MinIn[i]) ? data_in[j][i] : MinIn[i];
			MaxIn[i]	= (data_in[j][i] > MaxIn[i]) ? data_in[j][i] : MaxIn[i];
		}
	}

	for ( i = 0; i < OutC; i++ )
	{
		MinOut[i]	= data_out[0][i];
		MaxOut[i]	= data_out[0][i];
		for ( j = 0; j < DataCount; j++ )
		{
			MinOut[i]	= (data_out[j][i] < MinOut[i]) ? data_out[j][i] : MinOut[i];
			MaxOut[i]	= (data_out[j][i] > MaxOut[i]) ? data_out[j][i] : MaxOut[i];
		}
	}
	for ( i = 0; i < InC; i++ )
	{
		for ( j = 0; j < DataCount; j++ )
		{
			data_in[j][i] = (data_in[j][i] - MinIn[i] + 1) / (MaxIn[i] - MinIn[i] + 1);
		}
	}

	for ( i = 0; i < OutC; i++ )
	{
		for ( j = 0; j < DataCount; j++ )
		{
			data_out[j][i] = (data_out[j][i] - MinOut[i] + 1) / (MaxOut[i] - MinOut[i] + 1);
		}
	}

	for ( i = 0; i < NerveCell; i++ )
	{
		for ( j = 0; j < InC; j++ )
		{
			In_w[i][j]	= rand() * 2.0 / RAND_MAX - 1; /* w[-1,1] */
			din_w[i][j]	= 0;
		}
	}
	for ( i = 0; i < NerveCell; i++ )
	{
		for ( j = 0; j < OutC; j++ )
		{
			Out_w[i][j]	= rand() * 2.0 / RAND_MAX - 1;
			dout_w[i][j]	= 0;
		}
	}
}


void NerveCellOut( int DataItem )
{
	double	sum;
	int	i, j;
	for ( i = 0; i < NerveCell; i++ )
	{
		sum = 0;
		for ( j = 0; j < InC; j++ )
		{
			sum += data_in[DataItem][j] * In_w[i][j];
		}
		ONerve[i] = 1 / (1 + exp( -1 * sum ) );
	}
	for ( i = 0; i < OutC; i++ )
	{
		sum = 0;
		for ( j = 0; j < NerveCell; j++ )
		{
			sum += Out_w[j][i] * ONerve[j];
		}
		OutData[i] = sum;
	}
}


void FeedBack( int DataItem )
{
	int	i, j;
	double	SumD;
	/* for every NerveCell update W */
	for ( i = 0; i < NerveCell; i++ )
	{
		SumD = 0;
		for ( j = 0; j < OutC; j++ )
		{
			SumD += (OutData[j] - data_out[DataItem][j]) * Out_w[i][j];

			dout_w[i][j]	= Aw * dout_w[i][j] + Bw * (OutData[j] - data_out[DataItem][j]) * ONerve[i];
			Out_w[i][j]	-= dout_w[i][j];
		}
		for ( j = 0; j < InC; j++ )
		{
			din_w[i][j]	= aw * din_w[i][j] + bw * SumD * ONerve[i] * (1 - ONerve[i]) * data_in[DataItem][j];
			In_w[i][j]	-= din_w[i][j];
		}
	}
}


/*
 * e==0  stop
 * e/DataCount 训练数据集平均每项误差率
 */
void Train()
{
	int Count = 0, i, j;
	do
	{
		e = 0.0;
		for ( i = 0; i < DataCount; i++ )
		{
			NerveCellOut( i );
			for ( j = 0; j < OutC; j++ )
			{
				e += fabs( (OutData[j] - data_out[i][j]) / data_out[i][j] );
			}
			FeedBack( i );
		}
		Count++;
		printf( "%d  %lf\n", Count, e / DataCount );
	}
	while ( Count<TrainC && e / DataCount>0.01 );
}


double result( double a, double b )
{
	a	= (a - MinIn[0] + 1) / (MaxIn[0] - MinIn[0] + 1);
	b	= (b - MinIn[1] + 1) / (MaxIn[1] - MinIn[1] + 1);
	int	i, j;
	double	sum;
	for ( i = 0; i < NerveCell; i++ )
	{
		sum		= 0;
		sum		= In_w[i][0] * a + In_w[i][1] * b;
		ONerve[i]	= 1 / (1 + exp( -1 * sum ) );
	}
	sum = 0;
	for ( i = 0; i < NerveCell; i++ )
	{
		sum += Out_w[i][0] * ONerve[i];
	}
	return(sum * (MaxOut[0] - MinOut[0] + 1) + MinOut[0] - 1);
}


int main( int argc, char*argv[] )
{
	GenerateTrainData();
	ReadData();
	InitBP();
	Train();
	printf( " result is %lf \n", result( 2.3, 5 ) );
	/* printf("%lf    %lf   %lf\n",data_in[1][0],data_in[1][1],data_out[1][0]); */
	return(0);
}