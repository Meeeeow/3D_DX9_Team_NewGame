#pragma once
#ifndef __CLIENT_MAGIC_MISSILE_PARTICLE_H__
#define __CLIENT_MAGIC_MISSILE_PARTICLE_H__

BEGIN(Engine)
class CBoxCollider;
END

#include "CubeObject.h"
BEGIN(Client)
class CMagicMissileParticle final:
	public CCubeObject
{
public:
	enum TYPE { TYPE_LEFT, TYPE_RIGHT, TYPE_UP, TYPE_DOWN, TYPE_END};
	typedef struct tagMagicMissileParticleDesc {
		tagMagicMissileParticleDesc()
		{
			ZeroMemory(this, sizeof(tagMagicMissileParticleDesc));

			eType = TYPE_END;
		}

		OBJ_INFO					tObjInfo;
		_float						fSpeed;
		TYPE						eType;
	} DESC;

public:
	explicit CMagicMissileParticle(_pDevice pDevice);
	explicit CMagicMissileParticle(const CMagicMissileParticle& rhs);
	virtual ~CMagicMissileParticle() = default;

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
	TIME	m_tTime;
	TYPE	m_eType = TYPE_END;

public:
	static CMagicMissileParticle* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
