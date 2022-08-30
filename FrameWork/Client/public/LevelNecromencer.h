#pragma once

#ifndef __LEVELNECROMENCER_H__
#define __LEVELNECROMENCER_H__

#include "Level.h"

BEGIN(Client)
class CLevelNecromencer final
	: public CLevel
{
private:
	explicit CLevelNecromencer(_pDevice pGraphicDevice);
	virtual ~CLevelNecromencer() = default;

public:
	virtual HRESULT NativeConstruct(_uint iLevelID) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_CollisionTag();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

	_float	m_fCnt = 0;

public:
	static	CLevelNecromencer*	Create(_pDevice pGraphicDevice, _uint iLevelID);
	virtual void			Free() override;
};
END
#endif // !__LEVELNECROMENCER_H__
