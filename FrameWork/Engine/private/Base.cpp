#include "..\public\Base.h"

using namespace Engine;

CBase::CBase()
{

}


unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

/* ���۷��� ī��Ʈ�� 0�ϰ�� �����Ѵ�.  */
/* ���۷��� ī��Ʈ ����. */
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

