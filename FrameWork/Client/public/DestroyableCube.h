#pragma once
#ifndef __CLIENT_DESTROYABLE_CUBE_H__
#define __CLIENT_DESTROYABLE_CUBE_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
END

#include "CollisionCubeObject.h"
BEGIN(Client)
class CDestroyableCube :
	public CCollisionCubeObject
{
public:
	typedef struct tagDestroyablCubeDesc {
		OBJ_INFO* pObjInfo;
	}DESC;

public:
	explicit CDestroyableCube(_pDevice pDevice);
	explicit CDestroyableCube(const CDestroyableCube& rhs);
	virtual ~CDestroyableCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

public:
	virtual void Collision(CGameObject* pColObj) override;

private:

public:
	static CDestroyableCube* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
