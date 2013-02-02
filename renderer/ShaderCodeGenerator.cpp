/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or 
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once
#include "precompiled.h"
#include "Renderer.h"
#include "ShaderCodeGenerator.h"

namespace P3D
{

// -----------------------------------
CShaderCodeGenerator::CShaderCodeGenerator()
{
	m_uniqueId = 0;
}

// -----------------------------------
CShaderCodeGenerator::~CShaderCodeGenerator()
{

}

// -----------------------------------
const Str CShaderCodeGenerator::GetUniqueName(const wchar* operationName)
{
	assert( operationName && *operationName && wstrlen(operationName)<=MAX_SHADER_VAR_LEN);

	Str out;

	out.FormatEx( _W("var%d%s"), m_uniqueId, operationName);
	m_uniqueId++;

	return out;
}

const Str CShaderCodeGenerator::GetTypeString(eShaderVariableType vtype, const int vecDim)
{
	wchar dimStr[8]=_W("");
	if (vecDim>1) wsprintf(dimStr, 7, _W("%d"), vecDim);

	Str out;

	switch(vtype)
	{
	case SV_FLOAT: out.FormatEx( _W("float%s"), dimStr); break;
	case SV_UFLOAT: out.FormatEx( _W("unsigned float%s"), dimStr); break;
	case SV_HALF: out.FormatEx( _W("half%s"), dimStr); break;
	case SV_UHALF: out.FormatEx( _W("unsigned half%s"), dimStr); break;
	case SV_INT: out.FormatEx( _W("int%s"), dimStr); break;
	case SV_UINT: out.FormatEx( _W("unsigned int%s"), dimStr); break;
	case SV_CHAR: out.FormatEx( _W("char%s"), dimStr); break;
	case SV_UCHAR: out.FormatEx( _W("unsigned char%s"), dimStr); break;
	default:
		assertd(false, "Unknown shader variable type!");
		out.FormatEx( _W("unknown_type")); break;
	}

	return out;
}

// -----------------------------------
const Str CShaderCodeGenerator::GetVarDim(sShaderVariable &var, const int vecDim )
{
	Str out;

	switch(vecDim)
	{
	case 0:
	case 1:
		out.FormatEx( _W("%s.x"), var.variableName.Get() );
		break;
	case 2: // target is 2 dimmens.
		switch(var.varVecDim)
		{
		case 0:
		case 1:
			out.FormatEx( _W("%s.xx"), var.variableName.Get() );
			break;
		default:
			out.FormatEx( _W("%s.xy"), var.variableName.Get() );
			break;
		}
		break;
	case 3: // target is 3 dimmens.
		switch(var.varVecDim)
		{
		case 0:
		case 1:
			out.FormatEx( _W("%s.xxx"), var.variableName.Get() );
			break;
		case 2:
			out.FormatEx( _W("%s.xyx"), var.variableName.Get() ); //
			break;
		default:
			out.FormatEx( _W("%s.xyz"), var.variableName.Get() );
			break;
		}
		break;
	case 4: // target is 4 dimmens.
		switch(var.varVecDim)
		{
		case 0:
		case 1:
			out.FormatEx( _W("%s.xxxx"), var.variableName.Get() );
			break;
		case 2:
			out.FormatEx( _W("%s.xyxy"), var.variableName.Get() ); //
			break;
		case 3:
			out.FormatEx( _W("%s.xyzz"), var.variableName.Get() ); //
			break;
		default:
			out.FormatEx( _W("%s"), var.variableName.Get() );
			break;
		}
		break;
	}
	return out;
}

// -----------------------------------
sShaderCodeOutput CShaderCodeGenerator::CreateValue( eShaderVariableType vtype, const int vecDim )
{
	sShaderCodeOutput out;

	out.outputVar.variableName.SetEx( GetUniqueName(_W("Value")) );
	out.outputVar.varType = vtype;
	out.outputVar.varVecDim = vecDim;
	out.shaderCode.FormatEx( _W("%s %s;\r\n"), GetTypeString(vtype, vecDim).Get(), out.outputVar.variableName.Get() ); 

	return out;
}

// -----------------------------------
sShaderCodeOutput CShaderCodeGenerator::SetFloat( sShaderVariable a, const float* param, const int vecDim )
{
	sShaderCodeOutput res;

	res.outputVar = a;

	if (vecDim!=a.varVecDim)
	{
		CON(MSG_ERR, _W("ShaderGenerator: Trying to set %d dim. vector value to %d dim. vector shader variable!"), vecDim, a.varVecDim );
		return res;
	}

	res.shaderCode.Reserve(128);
	res.shaderCode	<< res.outputVar.variableName.Get()
	<< _W(" = ")	<< GetTypeString(res.outputVar.varType, res.outputVar.varVecDim)
	<< _W("(")
	;

	switch(vecDim)
	{
	case 0:
	case 1:
		res.shaderCode << param[0] << _W("f");
		break;
	case 2:
		res.shaderCode << param[0] << _W("f, ") << param[1] << _W("f");
		break;
	case 3:
		res.shaderCode << param[0] << _W("f, ") << param[1] << _W("f, ") << param[2] << _W("f");
		break;
	case 4:
		res.shaderCode << param[0] << _W("f, ") << param[1] << _W("f, ") << param[2] << _W("f, ") << param[3] << _W("f");
		break;
	}

	res.shaderCode << _W(");\r\n");

	return res;
}

// -----------------------------------
bool CShaderCodeGenerator::GenerateCode()
{
	return false;
}

// -----------------------------------
sShaderCodeOutput CShaderCodeGenerator::Add(sShaderVariable a, sShaderVariable b)
{
	sShaderCodeOutput res;

	// initialize output variable
	res.outputVar.variableName = GetUniqueName( _W("Add") );
	res.outputVar.varVecDim = max(a.varVecDim, b.varVecDim);
	res.outputVar.varType = a.varType;
	
	// check for same types
	if (a.varType != b.varType)
		CON(MSG_ERR, _W("ShaderGenerator: Operations on different input variable types are not allowed!") );

	// generate shader code
	/*res.shaderCode.FormatEx(_W("%s %s = %s ++ %s;\r\n"), 
		GetTypeString(res.outputVar.varType, res.outputVar.varVecDim), 
		res.outputVar.variableName.Get(),
		GetVarDim(a, res.outputVar.varVecDim),
		GetVarDim(b, res.outputVar.varVecDim)  
	);*/

	res.shaderCode.Reserve(128);
	res.shaderCode	<< GetTypeString(res.outputVar.varType, res.outputVar.varVecDim)
	<< _W(" ")		<< res.outputVar.variableName.Get()
	<< _W(" = ")	<< GetVarDim(a, res.outputVar.varVecDim)
	<< _W(" + ")	<< GetVarDim(b, res.outputVar.varVecDim)  
	<< _W(";\r\n")
	;

	/*wchar shaderCode[256];
	wsprintf(shaderCode, 255, _W("%s %s = %s + %s;\r\n"),
		GetTypeString(a.varType, 3).Get(), 
		res.outputVar.variableName.Get(), 
		GetVarDim(a, res.outputVar.varVecDim),
		GetVarDim(b, res.outputVar.varVecDim) 
	); 
	res.shaderCode.SetEx(shaderCode);*/

	return res;
}



}; // ns