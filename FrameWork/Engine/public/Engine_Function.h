#pragma once
#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

template <typename T>
void Safe_Delete(T& pPointer)
{
	if (pPointer != nullptr)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Arr(T& pPointer)
{
	if (pPointer != nullptr)
	{
		delete[] pPointer;
		pPointer = nullptr;
	}
}


template <typename T>
unsigned long Safe_Release(T& pBase)
{
	unsigned long dwRefCnt = 0;
	if (pBase != nullptr)
	{
		dwRefCnt = pBase->Release();
		if (dwRefCnt == 0)
			pBase = nullptr;
	}

	return dwRefCnt;
}

template <typename T>
unsigned long Safe_AddRef(T& pBase)
{
	unsigned long dwRefCnt = 0;

	if (pBase != nullptr)
		dwRefCnt = pBase->AddRef();

	return dwRefCnt;
}

#endif // !__ENGINE_FUNCTION_H__
