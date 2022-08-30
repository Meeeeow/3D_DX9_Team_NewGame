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

	//�� Ʃ�丮��
	Create_TutorialWet();
	//���׸� Ʃ�丮��
	Create_TutorialMagma();
	//�ν����� ��ü Ʃ�丮��
	Create_TutorialBreak();
	//�ذ񺸽� Ʃ�丮��
	Create_TutorialBossSkeleton();
	//���뺸�� ȸ�� Ʃ�丮��
	Create_TutorialClown();
	//��Ŀ�� ���� ��ȯ Ʃ�丮��
	Create_TutorialCamera();
	//���뺸�� ������ Ʃ�丮��
	Create_TutorialTelekinesis();
	//���� Ʃ�丮��
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
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_WET);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��� ���̵�");

	//��Ÿ�� + ������1�� = ����
	//��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 1, pTagPrototypeTextureTipObject
		, _vec3(440.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//+
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	//������1��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
		, _vec3(640.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//=
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	//����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureTipObject
		, _vec3(840.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_WET);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"���Ӽ� ť��� �������� ���� �̻��Ϸ� �� �� �ֽ��ϴ�");

	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_WET, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialMagma()
{
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_MAGMA);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��� ���̵�");

	//���׸� + ������1�� = �Ϲ�
	//���׸�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 2, pTagPrototypeTextureTipObject
		, _vec3(440.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//+
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	//������1��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
		, _vec3(640.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//=
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
		, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	//�Ϲ�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureTipObject
		, _vec3(840.f, 500.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�ҼӼ� ť��� �������� ���� �̻��Ϸ� ���� �� �ֽ��ϴ�");

	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialBreak()
{
	////����
	////����ũ��
	//Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
	//	, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_MAGMA);

	////�۾� ��� ���̵�
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(640.f - 62.5f - 35.f, 600.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��� ���̵�");

	////�� �� Ÿ�� + ����1�� / ������ 3�� = �ν���
	////���׸�
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 4, pTagPrototypeTextureTipObject
	//	, _vec3(440.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////+
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 25.f, 500.f + 25.f, 0.f)
	//	, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"+");

	////������1��
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, L"PrototypeTextureMageSkill"
	//	, _vec3(640.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////=
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(740.f - 25.f, 500.f + 25.f, 0.f)
	//	, _vec3(2.f, 2.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"=");

	////�Ϲ�
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureTipObject
	//	, _vec3(840.f, 500.f, 0.f)
	//	, _vec3(100.f, 100.f, 1.f), TUTORIAL_MAGMA);

	////�۾� ���Ӽ� ť��� ��¼��
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�ҼӼ� ť��� �������� ���� �̻��Ϸ� ���� �� �ֽ��ϴ�");

	////�۾� �׸����� : ����
	//Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
	//	, _vec3(1.f, 1.f, 1.f), TUTORIAL_MAGMA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialBossSkeleton()
{
}

void CTutorialMgr::Create_TutorialClown()
{
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CLWON_DODGE);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��ſ� ��Ŀ�� ������ ���� ���̵�");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 2, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CLWON_DODGE);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"���ΰ��� ������ ���ؿ��� ������ �ʰ� ������ �̾���׿�!");
	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"������ ������ �����δ� ������ �� ��������!");

	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLWON_DODGE, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialCamera()
{
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CIRCUS_CAMERA);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��ſ� ��Ŀ�� ������ ���� ���̵�");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 1, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CIRCUS_CAMERA);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"���δ� �츮��");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(467.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"����");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(537.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�� �޸� �� �ʿ䰡 �־��!");
	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 52.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"����, ����� ������ ������ �ʿ��ϳ׿�!");

	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CIRCUS_CAMERA, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialTelekinesis()
{
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_CLOWN_TELEKINESIS);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��ſ� ��Ŀ�� ������ ���� ���̵�");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�̹����� �ѹ� �о ���ظ� �غ����� �ұ��?");
	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��� ��������� �� ");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(387.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"������");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(447.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"������ ���ݾƿ�?");
	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_CLOWN_TELEKINESIS, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
}

void CTutorialMgr::Create_TutorialFall()
{
	//����
	//����ũ��
	Create_TipObject(true, LEVEL_STATIC, pTagPrototypeVIBufferRect, 7, pTagPrototypeTextureBlackAlpha, _vec3(_float(g_iWinCX >> 1), _float(g_iWinCY >> 1), 0.f)
		, _vec3(_float(g_iWinCX), _float(g_iWinCY), 1.f), TUTORIAL_FALL);

	//�۾� ��� ���̵�
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(540.f - 122.5f - 35.f, 600.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��ſ� ��Ŀ�� ������ ���� ���̵�");

	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 3, pTagPrototypeTextureCircusTip
		, _vec3(640.f, 480.f, 0.f)
		, _vec3(100.f, 100.f, 1.f), TUTORIAL_FALL);

	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�������� �ٽ� ���������� ���ư� �� �־��!");
	//�۾� ���Ӽ� ť��� ��¼��
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(217.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"��� ������� �� ");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(387.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), L"�� ó��");
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(467.f, 400.f - 42.5f, 0.f)
		, _vec3(0.5f, 0.5f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"������ ������ �ʳ׿�..");
	//�۾� �׸����� : ����
	Create_TipObject(false, LEVEL_STATIC, pTagPrototypeVIBufferRect, 0, pTagPrototypeTextureBlackAlpha, _vec3(100.f + 387.f, 100.f + 12.5f, 0.f)
		, _vec3(1.f, 1.f, 1.f), TUTORIAL_FALL, true, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), L"�׸� ���� : Enter");
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
