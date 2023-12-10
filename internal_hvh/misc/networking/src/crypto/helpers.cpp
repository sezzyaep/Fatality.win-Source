#include <string>
#include <math.h>
#include "helpers.h"

//credits: https://math.stackexchange.com/questions/424238/random-primes-between-4000000000-and-4294967291-c
int isprime( int n )
{
/*if((n&1)==0) return n==2;*/
	if ( n % 3 == 0 ) return n == 3;
	/*if(n<25) return n>1;*/
	int p = 5;
	while ( p * p <= n )
	{
		if ( n % p == 0 ) return 0;
		p += 2;
		if ( n % p == 0 ) return 0;
		p += 4;
	}
	return 1;
}
//credits: https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
int gcd( int a, int b )
{
	if ( a == 0 )
		return b;
	return gcd( b % a, a );
}

std::pair<int, int> euclid_extended( int a, int b )
{
	if ( !b )
	{
		return { 1, 0 };
	}

	auto result = euclid_extended( b, a % b );
	return { result.second, result.first - ( a / b ) * result.second };
}

int generateCoprime( int n )
{
	int generated = rand() % 10000;
	while ( gcd( n, generated ) != 1 )
		generated = rand() % 10000;
	return generated;
}

int logPower( int n, int p, int mod )
{
	int result = 1;
	for ( ; p; p >>= 1 )
	{
		if ( p & 1 )
			result = ( 1LL * result * n ) % mod;
		n = ( 1LL * n * n ) % mod;
	}
	return result;
}

int modularInverse( int n, int mod )
{
	int inverse = euclid_extended( n, mod ).first;
	while ( inverse < 0 )
		inverse += mod;
	return inverse;
}

int randInRange( int min, int max )
{
	int range = max - min + 1;
	return rand() % range + min;
}

bool rabin_miller( int n )
{
	bool ok = true;

	for ( int i = 1; i <= 5 && ok; i++ )
	{
		int a = rand() + 1;
		int result = logPower( a, n - 1, n );
		ok &= ( result == 1 );
	}

	return ok;
}

int randPrime( int n )
{
	int generated = rand() % ( int ) pow( 10.0, ( double ) n );
	while ( !rabin_miller( generated ) )
		generated = rand() % ( int ) pow( 10.0, ( double ) n );;
	return generated;
}
