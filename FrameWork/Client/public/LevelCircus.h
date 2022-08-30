#pragma once
#ifndef __CLIENT_LEVEL_CIRCUS_H__
#define __CLIENT_LEVEL_CIRCUS_H__

#include "Level.h"
BEGIN(Client)
class CLevelCircus final :	public CLevel
{
private:
	explicit CLevelCircus(_pDevice pGraphicDevice);
	virtual ~CLevelCircus() DEFAULT;

public:
	virtual HRESULT NativeConstruct(_uint iLevelID) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_CollisionTag();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Environment(const _tchar* pLayerTag);

	_float	m_fCnt = 0;

public:
	static	CLevelCircus*	Create(_pDevice pGraphicDevice, _uint iLevelID);
	virtual void			Free() override;
};

END
#endif // !__CLIENT_LEVEL_CIRCUS_H__
