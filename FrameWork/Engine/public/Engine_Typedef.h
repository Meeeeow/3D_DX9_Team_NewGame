#pragma once
#ifndef __ENGINE_TYPEDEF_H__
#define __ENGINE_TYPEDEF_H__

namespace Engine
{
	typedef unsigned char			_ubyte;
	typedef signed char				_byte;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;
	typedef bool					_bool;

	typedef D3DXVECTOR2				_vec2;
	typedef D3DXVECTOR3				_vec3;
	typedef D3DXVECTOR4				_vec4;
	typedef D3DXMATRIX				_mat;
	typedef D3DXPLANE				_plane;

	typedef wchar_t					_tchar;
	typedef char					_char;

	typedef LPDIRECT3DDEVICE9		_pDevice;
	typedef LPDIRECT3DVERTEXBUFFER9	_pVtxBuf;
	typedef LPDIRECT3DINDEXBUFFER9	_pIdxBuf;
	typedef LPDIRECT3DBASETEXTURE9	_pBaseTexture;
}
#endif // !__ENGINE_TYPEDEF_H__
