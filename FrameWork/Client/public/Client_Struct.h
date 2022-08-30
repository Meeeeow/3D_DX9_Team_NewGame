#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__

#include "GameInstance.h"

namespace Client
{
#define OBJ_NAME 256
	typedef struct tagObjectInfo
	{
		tagObjectInfo() { 
			ZeroMemory(this, sizeof(*this)); 
			D3DXMatrixIdentity(&matWorld);
		}

		enum VIBufferType {
			VI_BUFFER_TYPE_RECT, VI_BUFFER_TYPE_RECT_YZ, VI_BUFFER_TYPE_RECT_XY
			, VI_BUFFER_TYPE_CUBE, VI_BUFFER_TYPE_TERRAIN, VI_BUFFER_TYPE_UI,
			VI_BUFFER_TYPE_END
		};

		enum State {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POS, STATE_END};

		VIBufferType eVIBufferType;
		wchar_t szObjName[OBJ_NAME];
		wchar_t szTextureName[OBJ_NAME];

		D3DXMATRIX matWorld;
		_vec3 vRot;

		int iTextureIndex;

		//�ͷ��� ���� ����
		int iNumVerticesX;
		int iNumVerticesY;
		int iNumVerticesZ;

		//�ͷ��� ���� ũ��
		float fSizeX;
		float fSizeY;
		float fSizeZ;

		//���� ��ǥ
		_vec3 vVertexPos;

		//�ͷ��� U,V ��ǥ
		float fU;
		float fV;
		wchar_t szHeightMap[OBJ_NAME] = L"None";

		// Rect YZ ��
		CVIBufferRectYZ::WALLSIDE_YZ	eWallSide = CVIBufferRectYZ::WS_END;
		CVIBufferRectXY::WALLLOOK_XY    eWallLook = CVIBufferRectXY::WL_END;

		// Collider Info
		//enum ColliderType { BOX_COLLIDER, SPHERE_COLLIDER, END_COLLIDER};
		_vec3 vColliderCenter;
		_vec3 vColliderSize;
		DWORD dwColliderColor;

		// �� ��ȯ
		_vec3 Get_State(State eState) const {
			return *(_vec3*)(&matWorld.m[eState][0]);
		}

		//  ������ ��ȯ
		_vec3 Get_Scale() const {
			return _vec3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
				D3DXVec3Length(&Get_State(STATE_UP)),
				D3DXVec3Length(&Get_State(STATE_LOOK)));
		}

		// �� ����
		void SetUp_State(State eState, const _vec3& vState) {
			memcpy(&matWorld.m[eState][0], &vState, sizeof(_vec3));
		}

		// ������ ����
		void SetUp_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
		{
			_vec3 vRight = Get_State(STATE_RIGHT);
			_vec3 vUp = Get_State(STATE_UP);
			_vec3 vLook = Get_State(STATE_LOOK);

			SetUp_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScaleX);
			SetUp_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
			SetUp_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
		}

		void SetUp_Rotation(const _vec3& vAxis, _float fDegree)
		{
			_mat mat;
			D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(fDegree));

			_vec3 vRight(1.f, 0.f, 0.f);
			_vec3 vLook(0.f, 0.f, 1.f);

			_vec3 vScale = Get_Scale();
			SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &mat)) * vScale.x);
			SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &mat)) * vScale.z);
		}

		// X�� ȸ��
		void SetUp_XRotation(const _vec3& vAxis, _float fDegree)
		{
			_mat mat;
			D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(fDegree));

			_vec3 vUp(0.f, 1.f, 0.f);
			_vec3 vLook(0.f, 0.f, 1.f);

			_vec3 vScale = Get_Scale();
			SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &mat)) * vScale.y);
			SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &mat)) * vScale.z);
		}

		// Z�� ȸ��
		void SetUp_ZRotation(const _vec3& vAxis, _float fDegree)
		{
			_mat mat;
			D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(fDegree));

			_vec3 vRight(1.f, 0.f, 0.f);
			_vec3 vUp(0.f, 1.f, 0.f);

			_vec3 vScale = Get_Scale();
			SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &mat)) * vScale.x);
			SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &mat)) * vScale.y);
		}

		// ������ ���� �������� ȸ��
		void Rotation_Axis(_vec3 vAxis, _float fDegree)
		{
			_mat mat;
			D3DXMatrixRotationAxis(&mat, &vAxis, D3DXToRadian(fDegree));

			_vec3 vRight = Get_State(STATE_RIGHT);
			_vec3 vUp = Get_State(STATE_UP);
			_vec3 vLook = Get_State(STATE_LOOK);

			SetUp_State(STATE_RIGHT, (*D3DXVec3TransformNormal(&vRight, &vRight, &mat)));
			SetUp_State(STATE_UP, (*D3DXVec3TransformNormal(&vUp, &vUp, &mat)));
			SetUp_State(STATE_LOOK, (*D3DXVec3TransformNormal(&vLook, &vLook, &mat)));
		}

	}OBJ_INFO;

	typedef struct tagSceneInfo
	{
		vector<OBJ_INFO*> vectorObj;
	}SCENE_INFO;

	typedef struct tagFrame
	{
		tagFrame() {
			fDelay = 0.1f; fTime = 0.f; iStart = 0; iEnd = 0;
		}

		_float	fDelay;
		_float	fTime;
		_int	iStart;
		_int	iEnd;

		void Set_Value(_int iEnd, _int iStart = 0, _float fDelay = 0.1f, _float fTime = 0.f)
		{
			this->iEnd = iEnd;
			this->iStart = iStart;
			this->fDelay = fDelay;
			this->fTime = fTime;
		}

	}FRAME;

}

using namespace Client;


#endif // !__CLIENT_DEFINES_H__
