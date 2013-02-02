/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
#include "interface.h"

namespace P3D{

enum eShaderVariableType
{
	SV_FLOAT,
	SV_UFLOAT,
	SV_HALF,
	SV_UHALF,
	SV_INT,
	SV_UINT,
	SV_CHAR,
	SV_UCHAR
};

struct sShaderVariable
{
	/// Function shader code
	Str variableName;
	/// variable type
	eShaderVariableType varType;
	/// vector dimension
	unsigned char varVecDim;
	/// array dimension
	unsigned int varArrayDim;
};

/// Result of shader function code generation
struct sShaderCodeOutput
{
	/// Function shader code
	Str shaderCode;
	/// Output variable desc.
	sShaderVariable outputVar;
};

#define MAX_SHADER_VAR_LEN 32

class CShaderCodeGenerator
{
public:
	CShaderCodeGenerator();
	~CShaderCodeGenerator();
	bool GenerateCode();

	const Str GetUniqueName(const wchar* operationName); // should not deallocate result
	const Str GetTypeString(eShaderVariableType vtype, const int vecDim ); // should not deallocate result
	/// Get variable of desired target dimension
	const Str GetVarDim(sShaderVariable &var, const int vecDim ); // should not deallocate result

	sShaderCodeOutput CreateValue( eShaderVariableType vtype, const int vecDim );
	sShaderCodeOutput SetFloat( sShaderVariable a, const float* param, const int vecDim=1 );

	sShaderCodeOutput Add(sShaderVariable a, sShaderVariable b);

private:
	unsigned int m_uniqueId;
};

}; //ns