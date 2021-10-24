#pragma once
#include<Windows.h>

typedef unsigned int uint;
typedef unsigned long long address64;

void Throw(HRESULT result)
{
	if (FAILED(result))
	{
		throw std::runtime_error("");
	}
}