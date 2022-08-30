#include "stdafx.h"
#include "UIObj.h"
#include "GameInstance.h"

CUIObj::CUIObj(_pDevice pDevice)
	:CToolObj(pDevice)
{
}

HRESULT CUIObj::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CUIObj NativeConstruct_Prototype - __super");

	//로드한 데이터 부모 클래스에 보관
	if(FAILED(__super::Set_ObjInfo((OBJ_INFO*)pArg)))
		MSG_FAIL("CUIObj::NativeConstruct_Prototype - Set_ObjInfo");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CUIObj NativeConstruct - SetUp_Component");

	return S_OK;
}

HRESULT CUIObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CUIObj Render - __super");

	if (m_pTransform == nullptr
		|| m_pTexture == nullptr
		|| m_pVIBuffer == nullptr)
		MSG_FAIL("CUIObj Render - m_pTransform m_pTexture m_pVIBuffer is null");

	//UI는 직교 투영해야하기 때문에 값을 직접 변경해줘야함
	_mat matOriginalWorld = *m_pTransform->Get_WorldMatrixPtr();
	_vec3 vUIPos = m_pTransform->Get_State(CTransform::STATE_POS);
	vUIPos.x -= _float(g_iWinCX >> 1);
	vUIPos.y -= _float(g_iWinCY >> 1);
	vUIPos.z = 0.f;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vUIPos);
	m_pTransform->Bind_OnDevice();
	m_pTransform->Set_Matrix(matOriginalWorld);

	m_pTexture->Bind_OnDevice(Get_ObjInfo()->iTextureIndex);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CUIObj::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CUIObj::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CUIObj::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CUIObj::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");


	m_pVIBuffer->Render();

	return S_OK;
}

_int CUIObj::Tick(_float fTimeDelta)
{
	return _int();
}

_int CUIObj::LateTick(_float fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_PRIORITY, this);

	return 0;
}

HRESULT CUIObj::Set_Matrix(_mat matrix)
{
	m_pTransform->Set_Matrix(matrix);

	__super::Set_Matrix(matrix);

	return S_OK;
}

HRESULT CUIObj::SetUp_Component()
{
	//텍스쳐 검색할 이름 생성
	CString wstrTextureName = L"PrototypeTextureUI";

	//텍스쳐 검색
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.GetString(), L"ComTexture", (CComponent**)&m_pTexture)))
		MSG_FAIL("CUIObj SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTransform", L"ComTransform", (CComponent**)&m_pTransform)))
		MSG_FAIL("CUIObj SetUp_Component - Add_Component m_pTransform");

	m_pTransform->Set_Matrix(Get_ObjInfo()->matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeRenderer", L"ComRenderer", (CComponent**)&m_pRenderer)))
		MSG_FAIL("CUIObj SetUp_Component - Add_Component m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeVIBufferRect", L"ComVIBuffer", (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CUIObj SetUp_Component - Add_Component PrototypeVIBufferRect");

	return S_OK;
}

void CUIObj::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
}

CUIObj * CUIObj::Create(_pDevice pDevice, void* pArg)
{
	CUIObj* pInstance = new CUIObj(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CUIObj Create Failed");
	}

	return pInstance;
}

_vec3 CUIObj::Get_MinPos()
{
	return _vec3();
}

_vec3 CUIObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 CUIObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 CUIObj::Get_Length()
{
	return _vec3();
}
