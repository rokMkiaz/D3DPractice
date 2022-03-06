#pragma once

#include<Windows.h>
#include<assert.h>


//STL
#include<string>
#include<vector>
#include <functional>

using namespace std;

//Direct3D

#include <d3dcompiler.h> 
#include <d3d11.h> 
#include <d3dx10math.h> 
#include <d3dx11async.h> 
#include <d3dx11effect.h>


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib") 
#pragma comment(lib, "d3dx11.lib") 
#pragma comment(lib, "d3dcompiler.lib") 
#pragma comment(lib, "Effects11d.lib")


#define Check(hr) { assert(SUCCEEDED(hr)); }

#define SafeRelease(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SafeDelete(p){ if(p){ delete (p); (p) = NULL; } }
#define SafeDeleteArray(p){ if(p){ delete [] (p); (p) = NULL; } }

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;
typedef D3DXPLANE Plane;

#include"Systems/D3D.h"


#include "Utilities/String.h"
#include "Utilities/Path.h"