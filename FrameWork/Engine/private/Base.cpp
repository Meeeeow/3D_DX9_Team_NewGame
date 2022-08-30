#include "..\public\Base.h"

using namespace Engine;

CBase::CBase()
{

}


unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

/* 레퍼런스 카운트가 0일경우 삭제한다.  */
/* 레퍼런스 카운트 감소. */
unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;
		 
		return 0;
	}
	else
		return m_dwRefCnt--;	
}

