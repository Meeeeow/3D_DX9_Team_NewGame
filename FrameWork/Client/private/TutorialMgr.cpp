#include "stdafx.h"
#include "..\public\TutorialMgr.h"
#include "DialogMgr.h"
#include "TipObject.h"
#include "TipMgr.h"
#include "TipText.h"


IMPLEMENT_SINGLETON(CTutorialMgr)

CTutorialMgr::CTutorialMgr()
{
}


CTutorialMgr::~CTutorialMgr()
{
}

HRESULT CTutorialMgr::SetUp_Mgr()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_pDevice pDevice = pGameInstance->Get_Device();

	//물 튜토리얼
	Create_TutorialWet();
	//마그마 튜토리얼
	Create_TutorialMagma();
	//부숴지는 객체 튜토리얼
	Create_TutorialBreak();
	//해골보스 튜토리얼
	Create_TutorialBossSkeleton();
	//광대보스 회피 튜토리얼
	Create_TutorialClown();
	//서커스 시점 변환 튜토리얼
	Create_TutorialCamera();
	//광대보스 염동력 튜토리얼
	Create_TutorialTelekinesis();
	//낙하 튜토리얼
	Create_TutorialFall();
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CTutorialMgr::Set_Dialog(TUTORIAL eTuto)
{
	if (eTuto == TUTORIAL_END)
		return;

	if (m_tTutoArr[eTuto].bEnd)
		return;

	m_tTutoArr[eTuto].bEnd = true;
	for (auto& tDialogDec : m_tTutoArr[eTuto].listDialogDesc)
	{
		CDialogMgr::Get_Instance()->SetUp_Dialog(true, tDialogDec);
	}
}

void CTutorialMgr::Create_TipObject(_bool bBlend, LEVEL eTextureLevel, const _tchar* pTagPrototypeVIBuffer, _uint iTextureIndex
	, const _tchar * pTextureName, _vec3 vPos, _vec3 vScale, TUTORIAL eTuto, _bool bText /*= false*/
	, D3DXCOLOR dwColor /*= 0xffffffff*/, const _tchar* pText /*= nullptr*/)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_pDevice pDevice = pGameInstance->Get_Device();

	CTipObject* pTipObj = nullptr;
	CTipObject::DESC tTipObjDesc;
	tTipObjDesc.bBlend = bBlend;
	tTipObjDesc.eTextureLevel = eTextureLevel;
	tTipObjDesc.pTagPrototypeVIBuffer = pTagPrototypeVIBuffer;
	tTipObjDesc.tObjInfo.iTextureIndex = iTextureIndex;
	lstrcpy(tTipObjDesc.tObjInfo.szTextureName, pTextureName);
	D3DXMatrixIdentity(&tTipObjDesc.tObjInfo.matWorld);
	tTipObjDesc.tObjInfo.SetUp_Scale(vScale.x, vScale.y, 1.f);

	if (bText)
	{
		_vec3 vTextPos = vPos;
		vTextPos.y = g_iWinCY - vTextPos.y;
		tTipObjDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(vTextPos.x, vTextPos.y - _float(g_iWinCY), 0.f));
		CTipText::DESC tTipTextDesc;
		tTipTextDesc.FontColor = dwColor;
		tTipTextDesc.pText = pText;
		tTipTextDesc.tSuperDesc = tTipObjDesc;
		pTipObj = CTipText::Create(pDevice, &tTipTextDesc);
	}
	else
	{
		tTipObjDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(vPos.x, vPos.y + _float(g_iWinCY), 0.f));
		pTipObj = CTipObject::Create(pDevice, &tTipObjDesc);
	}


	if (pTipObj == nullptr)
		MSG_VOID("CTutorialMgr::Create_TipObject - pTipObj == nullptr");

	m_listTipObjArr[eTuto].emplace_back(pTipObj);

	RELEASE_INSTANCE(CGameInstance);
}

void CTutorialMgr::Create_TutorialWet()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_WET);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"기믹 가이드");

	//물타일 + 마법사1스 = 얼음
	//물
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 1, pTagPrototypeTextureTipObject
		, _vec3(440.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//+
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	//마법사1스
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
		, _vec3(640.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//=
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	//얼음
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureTipObject
		, _vec3(840.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"물속성 큐브는 마법사의 매직 미사일로 얼릴 수 있습니다");

	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialMagma()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_MAGMA);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"기믹 가이드");

	//마그마 + 마법사1스 = 일반
	//마그마
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 2, pTagPrototypeTextureTipObject
		, _vec3(440.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//+
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	//마법사1스
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
		, _vec3(640.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//=
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	//일반
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureTipObject
		, _vec3(840.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"불속성 큐브는 마법사의 매직 미사일로 식힐 수 있습니다");

	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialBreak()
{
	////툴팁
	////블랙스크린
	//Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
	//	, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_MAGMA);

	////글씨 기믹 가이드
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"기믹 가이드");

	////금 간 타일 + 전사1스 / 마법사 3스 = 부숴짐
	////마그마
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 4, pTagPrototypeTextureTipObject
	//	, _vec3(440.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////+
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
	//	, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	////마법사1스
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
	//	, _vec3(640.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////=
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
	//	, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	////일반
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureTipObject
	//	, _vec3(840.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////글씨 물속성 큐브는 어쩌고
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"불속성 큐브는 마법사의 매직 미사일로 식힐 수 있습니다");

	////글씨 그만보기 : 엔터
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialBossSkeleton()
{
}

void CTutorialMgr::Create_TutorialClown()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CLWON_DODGE);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"즐거운 서커스 관람을 위한 가이드");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 2, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CLWON_DODGE);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"주인공은 관객의 방해에도 굴하지 않고 연극을 이어나가네요!");
	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"물건을 던지는 정도로는 방해할 수 없나봐요!");

	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialCamera()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CIRCUS_CAMERA);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"즐거운 서커스 관람을 위한 가이드");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 1, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CIRCUS_CAMERA);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"때로는 우리의");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(467.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"관점");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(537.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"을 달리 할 필요가 있어요!");
	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 52.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"정말, 어른들은 언제나 설명이 필요하네요!");

	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialTelekinesis()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CLOWN_TELEKINESIS);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"즐거운 서커스 관람을 위한 가이드");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"이번에는 한번 밀어서 방해를 해보도록 할까요?");
	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"요새 마술사들은 다 ");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(387.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"염동력");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(447.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"정도는 쓰잖아요?");
	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Create_TutorialFall()
{
	//툴팁
	//블랙스크린
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_FALL);

	//글씨 기믹 가이드
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"즐거운 서커스 관람을 위한 가이드");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_FALL);

	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"떨어지면 다시 시작점으로 돌아갈 수 있어요!");
	//글씨 물속성 큐브는 어쩌고
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"모든 사람들이 다 ");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(387.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"저 처럼");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(467.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"게임을 잘하진 않네요..");
	//글씨 그만보기 : 엔터
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"그만 보기 : Enter");
}

void CTutorialMgr::Set_Tutorial(TUTORIAL eTuto)
{
	if (eTuto == TUTORIAL_END || m_tTutoArr[eTuto].bEnd)
		return;

	Set_Dialog(eTuto);
}

void CTutorialMgr::Set_TutorialToolTip(TUTORIAL eTuto)
{
	if (eTuto == TUTORIAL_END || m_listTipObjArr[eTuto].size() <= 0)
		return;

	for (auto& pTipObj : m_listTipObjArr[eTuto])
	{
		CTipMgr::Get_Instance()->Add_Obj(pTipObj);
		Safe_Release(pTipObj);
	}
	m_listTipObjArr[eTuto].clear();
}

void CTutorialMgr::Free()
{
	for (auto& list : m_listTipObjArr)
	{
		for (auto& pTipObj : list)
		{
			Safe_Release(pTipObj);
		}
		list.clear();
	}
}
