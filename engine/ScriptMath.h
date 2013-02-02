/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/

namespace P3D{

static int script_imod(int a, int m){ return a % m; }

static int script_rand(){ return rand(); }
static int script_rand(int max){ return rand()%max; }
static int script_rand(int min, int max){ return min + (rand()%(max-min)); }

static float script_frand(){ return 1.0f / RAND_MAX * rand(); }
static float script_frand(float max){ return max / RAND_MAX * rand(); }
static float script_frand(float min, float max){ return min + (max-min)/RAND_MAX*rand(); }

void RegisterMath(asIScriptEngine * VM)
{
	int r;
	// goniometric
	r = VM->RegisterGlobalFunction( "float sin(float)", asFUNCTIONPR(sin, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float asin(float)", asFUNCTIONPR(asin, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float cos(float)", asFUNCTIONPR(cos, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float acos(float)", asFUNCTIONPR(acos, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float tan(float)", asFUNCTIONPR(tan, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float atan(float)", asFUNCTIONPR(atan, (float), float), asCALL_CDECL ); assert( r >= 0 );

	// standard
	r = VM->RegisterGlobalFunction( "float abs(float)", asFUNCTIONPR(abs, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float sqrt(float)", asFUNCTIONPR(sqrt, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float pow(float, float)", asFUNCTIONPR(pow, (float, float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float floor(float)", asFUNCTIONPR(floor, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float ceil(float)", asFUNCTIONPR(ceil, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float log(float)", asFUNCTIONPR(log, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float log10(float)", asFUNCTIONPR(log10, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float exp(float)", asFUNCTIONPR(exp, (float), float), asCALL_CDECL ); assert( r >= 0 );

	// extra
	r = VM->RegisterGlobalFunction( "float mod(float, float)", asFUNCTIONPR(fmod, (float, float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "int mod(int, int)", asFUNCTIONPR(script_imod, (int, int), int), asCALL_CDECL ); assert( r >= 0 );

	// random
	r = VM->RegisterGlobalFunction( "int rand()", asFUNCTIONPR(script_rand, (void), int), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "int rand(int)", asFUNCTIONPR(script_rand, (int), int), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "int rand(int, int)", asFUNCTIONPR(script_rand, (int, int), int), asCALL_CDECL ); assert( r >= 0 );

	r = VM->RegisterGlobalFunction( "float rand()", asFUNCTIONPR(script_frand, (void), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float rand(float)", asFUNCTIONPR(script_frand, (float), float), asCALL_CDECL ); assert( r >= 0 );
	r = VM->RegisterGlobalFunction( "float rand(float, float)", asFUNCTIONPR(script_frand, (float, float), float), asCALL_CDECL ); assert( r >= 0 );
}

}; // ns