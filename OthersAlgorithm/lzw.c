#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_TABLE	100
#define MAX_CHARCODE	100
typedef struct _table_item {
	char	CharCode[MAX_CHARCODE];
	int	CharNum;
}table_item;
typedef struct _table {
	table_item	ti[MAX_TABLE];
	int		table_size;
	int		table_top;
}table;
table tb;
void InitTable( table *tb )
{
	int i;
	for ( i = 0; i < MAX_TABLE; i++ )
	{
		tb->ti[i].CharNum = i;
	}
	tb->table_size	= 0;
	tb->table_top	= 0;
}


void push( char input[], table * tb )
{
	/* printf("[push] input is %s size is %d \n",input,strlen(input)); */
	strncpy( tb->ti[(tb->table_top)++].CharCode, input, strlen( input ) );
	tb->table_size++;
}


void pop( char * output, table *tb )
{
	strcpy( output, tb->ti[--(tb->table_top)].CharCode );
	tb->table_size--;
}


int print_table( table *tb )
{
	int i;
	for ( i = 0; i < tb->table_size; i++ )
	{
		printf( "[print_table] %d  is CharCode is %s \n", i, tb->ti[i].CharCode );
		printf( "[print_table] %d  is CharNum  is %d \n", i, tb->ti[i].CharNum );
	}
}


int SearchChar( char input[], table *tb, int size )
{
	int	i;
	int	flag = 0;
	for ( i = 0; i < size; i++ )
	{
		if ( strncmp( input, (tb->ti[i]).CharCode, strlen( input ) ) == 0 )
		{
			return(tb->ti[i].CharNum);
		}
	}
	return(-1);
}


int Init( table *tb )
{
	char	* initChar[5] = { "A", "B", "C", "D", "E" };
	int	i;
	InitTable( tb );
	for ( i = 0; i < 5; i++ )
	{
		push( initChar[i], tb );
	}
	print_table( tb );
}


void MyClear( char input[] )
{
	int i;
	for ( i = 0; i < MAX_CHARCODE; i++ )
	{
		input[i] = '\0';
	}
}


void Encode( char input[], table * tb )
{
	int	num;
	char	prefix[MAX_CHARCODE]	= { '\0' };
	char	bakprefix[MAX_CHARCODE] = { '\0' };
	char	inputchar[1];
	int	i;
	for ( i = 0; i <= strlen( input ); i++ )
	{
		inputchar[0] = input[i];
		MyClear( bakprefix );
		strcpy( bakprefix, prefix );
		strncat( prefix, inputchar, 1 );
		num = SearchChar( prefix, tb, tb->table_size );
		if ( num != -1 )
		{
			if ( i == strlen( input ) )
			{
				printf( "[Encode] Code is %d\n", num );
			}
			continue;
		}else{
			num = SearchChar( bakprefix, tb, tb->table_size );
			if ( num != -1 )
			{
				printf( "[Encode] Code is %d\n", num );
			}
			push( prefix, tb );
			MyClear( prefix );
			strncpy( prefix, inputchar, 1 );
		}
	}
}


int main( int argc, char * argv[] )
{
	char in[] = "ABABA";
	Init( &tb );
	Encode( in, &tb );
	return(0);
}