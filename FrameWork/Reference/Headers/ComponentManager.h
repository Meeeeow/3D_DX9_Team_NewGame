#pragma once

#ifndef __ENGINE_COMPONENT_MANAGER_H__
#define	__ENGINE_COMPONENT_MANAGER_H__ 

#include "Renderer.h"
#include "VIBufferRect.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBufferTerrain.h"
#include "VIBufferCube.h"
#include "VIBufferRectXY.h"
#include "VIBufferRectYZ.h"
#include "VIBufferRectColor.h"
#include "BoxCollider.h"
#include "Subject.h"
#include "Frustum.h"
#include "VIBufferThorn.h"
#include "VIBufferCubeColor.h"

BEGIN(Engine)

class CComponentManager final : public CBase
{
	DECLARE_SINGLETON(CComponentManager)
public:
	explicit CComponentManager();
	virtual ~CComponentManager() = default;

public:
	HRESULT		Reserve_Container(_uint iNumLevels);
	HRESULT		Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

	HRESULT		Clear_ComponentContainer(_uint iNumLevels);

private:
	unordered_map<const _tchar*, CComponent*>*			m_mapComponentArr = nullptr;
	typedef unordered_map<const _tchar*, CComponent*>	PROTOTYPES;

	_uint		m_iNumLevels = 0;

public:
	virtual void Free() override;
};

END

#endif // !__ENGINE_COMPONENT_MANAGER_H__
