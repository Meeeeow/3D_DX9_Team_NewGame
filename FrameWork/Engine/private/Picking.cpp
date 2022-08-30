#include "..\public\Picking.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CPicking)


CPicking::CPicking()
{
}

HRESULT CPicking::PreConvert()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_pDevice pDevice = CGraphic_Device::Get_Instance()->Get_Device();
	
	D3DVIEWPORT9 Viewport;
	pDevice->GetViewport(&Viewport);

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(pInstance->Get_Wnd(), &pt);

	_vec3 vMousePos;
	vMousePos.x = _float(pt.x / (Viewport.Width >> 1)) - 1;
	vMousePos.y = _float(pt.y / (Viewport.Height >> 1) * -1.f) + 1;
	vMousePos.z = 0.f;

	_mat matProj;
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	m_vMousePivot = _vec3(0.f, 0.f, 0.f);
	m_vMouseRay = vMousePos - m_vMousePivot;

	_mat matView;
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &matView);
	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &matView);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CPicking::Picking(const _tchar * szTransformTag, const _tchar * szBufferTag, CGameObject * pTarget, _vec3 * pOut)
{
	CTransform* pTransform = dynamic_cast<CTransform*>(pTarget->Get_Component(szTransformTag));
	if (pTransform == nullptr)
		MSG_FALSE("CPicking Picking - pTransform is null");

	CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pTarget->Get_Component(szBufferTag));
	if (pVIBuffer == nullptr)
		MSG_FALSE("CPicking Picking - pVIBuffer is null");

	_mat matWorld, matInverse;
	matWorld = *pTransform->Get_WorldMatrixPtr();
	D3DXMatrixInverse(&matInverse, nullptr, &matWorld);
	D3DXVec3TransformCoord(&m_vMousePivot, &m_vMousePivot, &matInverse);
	D3DXVec3TransformNormal(&m_vMouseRay, &m_vMouseRay, &matInverse);
	D3DXVec3Normalize(&m_vMouseRay, &m_vMouseRay);

	if (pVIBuffer->Picking(m_vMouseRay, m_vMousePivot, pOut))
	{
		D3DXVec3TransformCoord(pOut, pOut, &matWorld);
		return true;
	}

	return false;
}

void CPicking::Free()
{

}
