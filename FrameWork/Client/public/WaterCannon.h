#pragma once
#ifndef __CLIENT_WATER_CANNON_H__
#define __CLIENT_WATER_CANNON_H__

#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)
class CWaterCannon final:
	public CGameObject
{
public:
	typedef struct tagWaterCannonDesc {
		tagWaterCannonDesc() {
			ZeroMemory(this, sizeof(tagWaterCannonDesc));
		}

		_vec3 vPos;
	} DESC;
public:
	explicit CWaterCannon(_pDevice pDevice);
	explicit CWaterCannon(const CWaterCannon& rhs);
	virtual ~CWaterCannon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:

private:
	_vec3			m_vPos = _vec3(0.f, 0.f, 0.f);
	TIME			m_tTimeShoot;

public:
	static CWaterCannon* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;


	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_Length() override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
