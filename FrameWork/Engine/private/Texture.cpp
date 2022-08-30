#include "..\public\Texture.h"

CTexture::CTexture(_pDevice pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

HRESULT CTexture::NativeConstruct_Prototype(const _tchar* szFilePath, _uint iNum, TYPE eType)
{
	if (__super::NativeConstruct_Prototype())
		MSG_FAIL("CTexture NativeConstruct_Prototype - super");

	m_iNumTextures = iNum;

	m_vectorTexture.reserve(iNum);

	_tchar szBuf[MAX_PATH]{};
	_pBaseTexture pTexture = nullptr;

	for (size_t i = 0; i < iNum; ++i)
	{
		swprintf_s(szBuf, szFilePath, i);
		if (eType == TYPE_CUBE)
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szBuf, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				MSG_FAIL("CTexture NativeConstruct_Prototype - D3DXCreateTextureFromFile");
		}
		else
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDevice, szBuf, (LPDIRECT3DTEXTURE9*)&pTexture)))
				MSG_FAIL("CTexture NativeConstruct_Prototype - D3DXCreateTextureFromFile");
		}

		m_vectorTexture.emplace_back(pTexture);
	}

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	if (__super::NativeConstruct(pArg))
		MSG_FAIL("CTexture NativeConstruct - super");

	return S_OK;
}

HRESULT CTexture::Bind_OnDevice(_uint iIndex)
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTexture Bind_OnDevice - m_pGraphicDevice is null");

	if (iIndex >= m_iNumTextures)
		MSG_FAIL("CTexture Bind_OnDevice - iIndex is too Big");

	m_pGraphicDevice->SetTexture(0, m_vectorTexture[iIndex]);
	
	return S_OK;
}

CTexture * CTexture::Create(_pDevice pGraphicDevice, const _tchar * szFilePath, _uint iNum, TYPE eType)
{
	CTexture* pInstance = new CTexture(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(szFilePath, iNum, eType)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTexture Create Failed");
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	AddRef();
	return this;
}

void CTexture::Free()
{

	__super::Free();

	for (auto& pTexture : m_vectorTexture)
		Safe_Release(pTexture);
	m_vectorTexture.clear();
	m_vectorTexture.shrink_to_fit();
}
