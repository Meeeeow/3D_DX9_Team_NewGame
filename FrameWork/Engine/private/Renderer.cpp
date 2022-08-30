#include "..\public\Renderer.h"
#include  "GameObject.h"
#include "Effect.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER_GROUP eGroup, CGameObject * pGameObject)
{
	if (pGameObject == nullptr
		|| eGroup >= GROUP_END)
		MSG_FAIL("CRenderer - Add_RenderGroup");

	m_listRender[eGroup].emplace_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{

	if (FAILED(Render_Priority()))
		MSG_FAIL("CRenderer - Render_Priority");

	if (FAILED(Render_NonAlpha()))
		MSG_FAIL("CRenderer - Render_NonAlpha");

	if (FAILED(Render_Alpha()))
		MSG_FAIL("CRenderer - Render_Alpha");

	if(FAILED(Render_ScreenEffect()))
		MSG_FAIL("CRenderer - Render_ScreenEffect");

	if (FAILED(Render_UI()))
		MSG_FAIL("CRenderer - Render_UI");

	if (FAILED(Render_UI_Alpha()))
		MSG_FAIL("CRenderer - Render_UI_Alpha");

	if (FAILED(Render_UI_Late()))
		MSG_FAIL("CRenderer - Render_UI_Late");

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	return Render(GROUP_PRIORITY);
}

HRESULT CRenderer::Render_NonAlpha()
{
	return Render(GROUP_NONALPHA);
}

HRESULT CRenderer::Render_Alpha()
{
	m_listRender[GROUP_ALPHA].sort(
		[](CGameObject* pGameSrc, CGameObject* pGameDst)->bool {
		CEffect* pSrc = dynamic_cast<CEffect*>(pGameSrc);
		CEffect* pDst = dynamic_cast<CEffect*>(pGameDst);

		if (pSrc == nullptr || pDst == nullptr)
			return false;

		return pSrc->Get_CamDistance() > pDst->Get_CamDistance();
	}
	);
	return Render(GROUP_ALPHA);
}

HRESULT CRenderer::Render_ScreenEffect()
{
	return Render(GROUP_SCREENEFFECT);
}

HRESULT CRenderer::Render_UI()
{
	return Render(GROUP_UI);
}

HRESULT CRenderer::Render_UI_Alpha()
{
	return Render(GROUP_UI_ALPHA);
}

HRESULT CRenderer::Render_UI_Late()
{
	return Render(GROUP_UI_LATE);
}

HRESULT CRenderer::Render(RENDER_GROUP eGroup)
{
	for (auto& pObj : m_listRender[eGroup])
	{
		if (pObj != nullptr)
		{
			if (FAILED(pObj->Render()))
				MSG_FAIL("CRenderer - Render");

			Safe_Release(pObj);
		}
	}
	m_listRender[eGroup].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CRenderer Create Failed");
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();
	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for(auto& list : m_listRender)
		for (auto& pObj : list)
			Safe_Release(pObj);
}
