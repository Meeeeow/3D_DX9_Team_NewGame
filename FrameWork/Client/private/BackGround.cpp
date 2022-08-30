#include "stdafx.h"
#include "..\public\BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
	D3DXMatrixIdentity(&m_matProject);
}

CBackGround::CBackGround(const CBackGround & rhs)
	:CGameObject(rhs)/*, m_pRendererCom(rhs.m_pRendererCom), m_pVIBufferCom(rhs.m_pVIBufferCom)*/
{
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBackGround NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBackGround NativeConstruct - __super");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = (float)g_iWinCX;
	m_fSizeY = (float)g_iWinCY;
	m_fX = float(g_iWinCX >> 1);
	m_fY = float(g_iWinCY >> 1);

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CBackGround NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(m_fX - (m_fSizeX * 0.5f), - m_fY + (m_fSizeY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	return S_OK;
}

_int CBackGround::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBackGround Tick - __super");

	return _int();
}

_int CBackGround::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBackGround LateTick - __super");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_PRIORITY, this)))
		MSG_MINUSONE("CBackGround LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBackGround::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBackGround Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pTextureCom is null");

	m_pTransformCom->Bind_OnDevice();

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);
	m_pTextureCom->Bind_OnDevice();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBackGround::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//구조체 _tchar의 크기를 안다고 가정할때
	// _tchar szName[256];
	//typedef struct tagBuf {
	//	_tchar szName[256];
	//	_tchar szPath[256];
	//}BUF;

	//BUF tBuf;
	//lstrcpy(tBuf.szName, L"VIBufferRect");
	//lstrcpy(tBuf.szPath, L"Renderer");
	//void* pArg = &tBuf;
	//
	//BUF tTest;
	//_tchar Name[256], Path[256];
	//memcpy(&tTest, pArg, sizeof(BUF));
	//memcpy(Name, pArg, sizeof(_tchar) * 256);
	//memcpy(Path, ((_tchar*)pArg + 256), sizeof(_tchar) * 256);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBackGround SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBackGround SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBufferRect"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CBackGround SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureBackGround"), TEXT("ComTextureBackGround"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBackGround SetUp_Component - Add_Component m_pTextureCom");
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CBackGround* pInstance = new CBackGround(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("BackGround Create Failed");
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBackGround::Clone(void *pArg /*= (void *)nullptr*/)
{
	CGameObject* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("BackGround Clone Failed");
	}

	return pInstance;
}

_vec3 CBackGround::Get_MinPos()
{
	return _vec3();
}

_vec3 CBackGround::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBackGround::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBackGround::Get_Length()
{
	return _vec3();
}

