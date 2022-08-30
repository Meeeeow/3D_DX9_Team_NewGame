#pragma once
#ifndef __ENGINE_RENDERER_H__
#define	__ENGINE_RENDERER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final:
	public CComponent
{
public:
	enum RENDER_GROUP {GROUP_PRIORITY, GROUP_NONALPHA, GROUP_ALPHA, GROUP_SCREENEFFECT, GROUP_UI, GROUP_UI_ALPHA,
		GROUP_UI_LATE, GROUP_END};

public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CRenderer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDER_GROUP eGroup, class CGameObject* pGameObject);
	HRESULT Draw_RenderGroup();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_ScreenEffect();
	HRESULT Render_UI();
	HRESULT Render_UI_Alpha();
	HRESULT Render_UI_Late();
	HRESULT Render(RENDER_GROUP eGroup);

private:
	list<class CGameObject*>		m_listRender[GROUP_END];
	typedef list<class CGameObject*> RENDERS;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent * Clone(void * pArg) override;
	virtual void Free() override;
};

END

#endif // !__ENGINE_RENDERER_H__
