#include <stdio.h>
#include <math.h>

#define DataCount	120
#define TestCount	30
#define CategoryCount	3
#define Pi		3.14159265358979323846
typedef struct TrainItem
{
	float	AeigenValue;
	float	BeigenValue;
	float	CeigenValue;
	float	DeigenValue;
	int	Category;
}_TrainItem;
typedef struct Statisticalparameter
{
	int	Category;
	float	AmeanValue;
	float	Avariance;
	float	BmeanValue;
	float	Bvariance;
	float	CmeanValue;
	float	Cvariance;
	float	DmeanValue;
	float	Dvariance;
	int	CategoryTotalNum;
}_SP;
_SP		sp[CategoryCount];
_TrainItem	TI[DataCount];
_TrainItem	TestI[TestCount];
int		Category_0_DataItem;
int		Category_1_DataItem;
int		Category_2_DataItem;
void ReadTrainData()
{
	FILE * fp = fopen( "IRISTrain.txt", "r+" );
	if ( fp == NULL )
	{
		printf( "OPEN FILE FAILED!\n" );
	}
	int i;
	for ( i = 0; i < DataCount; i++ )
	{
		fscanf( fp, "%f,%f,%f,%f,%d\n", &TI[i].AeigenValue, &TI[i].BeigenValue, &TI[i].CeigenValue, &TI[i].DeigenValue, &TI[i].Category );
	}
	fclose( fp );
}


void ReadTestData()
{
	FILE * fp = fopen( "IRISTest.txt", "r+" );
	if ( fp == NULL )
	{
		printf( "OPEN FILE FAILED!\n" );
	}
	int i;
	for ( i = 0; i < DataCount; i++ )
	{
		fscanf( fp, "%f,%f,%f,%f,%d\n", &TestI[i].AeigenValue, &TestI[i].BeigenValue, &TestI[i].CeigenValue, &TestI[i].DeigenValue, &TestI[i].Category );
	}
	fclose( fp );
}


void CountEveryCategoryC_DataItem()
{
	int i;
	for ( i = 0; i < DataCount; i++ )
	{
		if ( TI[i].Category == 0 )
		{
			Category_0_DataItem++;
		}
		if ( TI[i].Category == 1 )
		{
			Category_1_DataItem++;
		}
		if ( TI[i].Category == 2 )
		{
			Category_2_DataItem++;
		}
	}
	sp[0].Category		= 0;
	sp[0].CategoryTotalNum	= Category_0_DataItem;
	sp[1].Category		= 1;
	sp[1].CategoryTotalNum	= Category_1_DataItem;
	sp[2].Category		= 2;
	sp[2].CategoryTotalNum	= Category_2_DataItem;
}


void CalculateMeanValue()
{
	int	i, j;
	float	sumA	= 0.0f;
	float	sumB	= 0.0f;
	float	sumC	= 0.0f;
	float	sumD	= 0.0f;
	for ( j = 0; j < CategoryCount; j++ )
	{
		sumA	= 0.0f;
		sumB	= 0.0f;
		sumC	= 0.0f;
		sumD	= 0.0f;
		for ( i = 0; i < DataCount; i++ )
		{
			if ( TI[i].Category == j )
			{
				sumA	+= TI[i].AeigenValue;
				sumB	+= TI[i].BeigenValue;
				sumC	+= TI[i].CeigenValue;
				sumD	+= TI[i].DeigenValue;
			}
		}
		sp[j].AmeanValue	= sumA / (sp[j].CategoryTotalNum);
		sp[j].BmeanValue	= sumB / (sp[j].CategoryTotalNum);
		sp[j].CmeanValue	= sumC / (sp[j].CategoryTotalNum);
		sp[j].DmeanValue	= sumD / (sp[j].CategoryTotalNum);
	}
}


void CalculateVariance()
{
	int	i, j;
	float	sumA	= 0.0f;
	float	sumB	= 0.0f;
	float	sumC	= 0.0f;
	float	sumD	= 0.0f;
	for ( j = 0; j < CategoryCount; j++ )
	{
		sumA	= 0.0f;
		sumB	= 0.0f;
		sumC	= 0.0f;
		sumD	= 0.0f;
		for ( i = 0; i < DataCount; i++ )
		{
			if ( TI[i].Category == j )
			{
				sumA	+= (TI[i].AeigenValue - sp[j].AmeanValue) * (TI[i].AeigenValue - sp[j].AmeanValue);
				sumB	+= (TI[i].BeigenValue - sp[j].BmeanValue) * (TI[i].BeigenValue - sp[j].BmeanValue);
				sumC	+= (TI[i].CeigenValue - sp[j].CmeanValue) * (TI[i].CeigenValue - sp[j].CmeanValue);
				sumD	+= (TI[i].DeigenValue - sp[j].DmeanValue) * (TI[i].DeigenValue - sp[j].DmeanValue);
			}
		}
		sp[j].Avariance = sumA / (sp[j].CategoryTotalNum);
		sp[j].Bvariance = sumB / (sp[j].CategoryTotalNum);
		sp[j].Cvariance = sumC / (sp[j].CategoryTotalNum);
		sp[j].Dvariance = sumD / (sp[j].CategoryTotalNum);
	}
}


void Calculatedprobability( float Ae, float Be, float Ce, float De, float re[] )
{
	int	i;
	double	operator, operatorA, operatorB, result[4], probability;
	double	MAXp = 0.0;
	for ( i = 0; i < CategoryCount; i++ )
	{
		operatorB	= -1 * (pow( (double) (Ae - sp[i].AmeanValue), 2 ) / (2 * sp[i].Avariance) );
		operatorA	= 1 / (sqrt( 2 * Pi * (double) sp[0].Avariance ) );
		result[0]	= operatorA * exp( operatorB );

		operatorB	= -1 * (pow( (double) (Be - sp[i].BmeanValue), 2 ) / (2 * sp[i].Bvariance) );
		operatorA	= 1 / (sqrt( 2 * Pi * (double) sp[i].Bvariance ) );
		result[1]	= operatorA * exp( operatorB );

		operatorB	= -1 * (pow( (double) (Ce - sp[i].CmeanValue), 2 ) / (2 * sp[i].Cvariance) );
		operatorA	= 1 / (sqrt( 2 * Pi * (double) sp[i].Cvariance ) );
		result[2]	= operatorA * exp( operatorB );

		operatorB	= -1 * (pow( (double) (De - sp[i].DmeanValue), 2 ) / (2 * sp[i].Dvariance) );
		operatorA	= 1 / (sqrt( 2 * Pi * (double) sp[i].Dvariance ) );
		result[3]	= operatorA * exp( operatorB );

		operator	= (double) sp[i].CategoryTotalNum / DataCount;
		probability	= result[0] * result[1] * result[2] * result[3] * operator;
		printf( "%d   P is %lf \n", i, probability );
		MAXp = (probability - MAXp > 0.0) ? probability : MAXp;
		if ( MAXp == probability )
		{
			re[0]	= i;
			re[1]	= (float) probability;
		}
	}
}


int main( int argc, char*argv[] )
{
	float	Category[2];
	int	i;
	ReadTrainData();
	ReadTestData();
	/*
	 * float Ae =4.8;
	 * float Be =3.0;
	 * float Ce =1.4;
	 * float De =0.3;
	 */
	CountEveryCategoryC_DataItem();
	CalculateMeanValue();
	CalculateVariance();
	for ( i = 0; i < TestCount; i++ )
	{
		Calculatedprobability( TestI[i].AeigenValue, TestI[i].BeigenValue, TestI[i].CeigenValue, TestI[i].DeigenValue, Category );
		printf( " Count %d Test Category %d  CP is %lf  result is %s P is %lf\n", i, TestI[i].Category, Category[0], ( ( (TestI[i].Category - Category[0]) == 0) ? "true" : "false"), Category[1] );
	}
	/*
	 * int i=119;
	 * printf("%f %f %f %f %d\n",TI[i].AeigenValue,TI[i].BeigenValue,TI[i].CeigenValue,TI[i].DeigenValue,TI[i].Category);
	 * CountEveryCategoryC_DataItem();
	 * printf("%d %d %d \n",Category_0_DataItem,Category_1_DataItem,Category_2_DataItem);
	 */

	return(0);
}