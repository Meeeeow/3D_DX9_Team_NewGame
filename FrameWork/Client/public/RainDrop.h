#pragma once
#ifndef __CLIENT_RAIN_DROP_H__
#define __CLIENT_RAIN_DROP_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
END

#include "CubeObject.h"
BEGIN(Client)
class CRainDrop :
	public CCubeObject
{
public:
	typedef struct tagRainDrop {
		_float		fDegree;
		OBJ_INFO*	pObjInfo;
	} DESC;

public:
	explicit CRainDrop(_pDevice pDevice);
	explicit CRainDrop(const CRainDrop& rhs);
	virtual ~CRainDrop() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	_float	m_fDegree;

public:
	static CRainDrop* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
