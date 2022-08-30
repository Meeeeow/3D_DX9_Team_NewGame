#include "..\public\CamMatrix.h"

IMPLEMENT_SINGLETON(CCamMatrix)

CCamMatrix::CCamMatrix()
{
}

_int CCamMatrix::Save_CamMatrix(_float fTimeDelte)
{
	if (m_pDevice == nullptr)
		MSG_MINUSONE("CCamMatrix::Save_CamMatrix - m_pDevice == nullptr");

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matCamView);
	D3DXMatrixInverse(&m_matCamWorld, nullptr, &m_matCamView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matCamProj);

	return 0;
}

HRESULT CCamMatrix::Set_Device(_pDevice pDevice)
{
	if (pDevice == nullptr)
		MSG_FAIL("CCamMatrix::Set_Device - pDevice == nullptr");

	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	return S_OK;
}

void CCamMatrix::Free()
{
	Safe_Release(m_pDevice);
}
