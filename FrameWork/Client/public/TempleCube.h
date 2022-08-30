#pragma once
#ifndef __CLIENT_TEMPLE_CUBE_H__
#define __CLIENT_TEMPLE_CUBE_H__

#include "TileCube.h"

BEGIN(Client)
class CTempleCube final : public CTileCube
{
public:
	explicit CTempleCube(_pDevice pGraphicDevice);
	explicit CTempleCube(const CTempleCube& rhs);
	virtual ~CTempleCube() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	
private:
	HRESULT			SetUp_Component(void* pArg);


public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Collision(CGameObject * pColObj) override;
	static CTempleCube* Create(_pDevice pGraphicDevice);
	virtual void Free() override;;
};

END
#endif // !__CLIENT_TEMPLE_CUBE_H__
