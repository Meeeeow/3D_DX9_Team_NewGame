#pragma once
#ifndef __CLIENT_POISON_CUBE_HIT_H__
#define __CLIENT_POISON_CUBE_HIT_H__

BEGIN(Engine)
class CBoxCollider;
END

#include "CubeObject.h"
BEGIN(Client)
class CPoisonCubeHit final:
	public CCubeObject
{
public:
	typedef struct tagPoisonCubeHitDesc {
		tagPoisonCubeHitDesc()
		{
			ZeroMemory(this, sizeof(tagPoisonCubeHitDesc));
		}

		OBJ_INFO					tObjInfo;
	} DESC;

public:
	explicit CPoisonCubeHit(_pDevice pDevice);
	explicit CPoisonCubeHit(const CPoisonCubeHit& rhs);
	virtual ~CPoisonCubeHit() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	void	Tick_Move(_float fTimeDelta, CGameInstance* pGameInstance);

	void	Set_RenderState();
	void	Release_RenderState();

private:
	TIME		m_tTimeRemain;
	_vec3		m_vOriginalPos = _vec3(0.f, 0.f, 0.f);

public:
	static CPoisonCubeHit* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
