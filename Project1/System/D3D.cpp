#include"Framework.h"
#include"D3D.h"

//정적 변수 초기화
vector<D3DEnumAdapterInfo*> D3D::adapterInfos;
int D3D::selectedAdapterIndex = -1;

D3D* D3D::instance = nullptr;

D3DDesc D3D::d3dDesc;
ID3D11Device* D3D::device = nullptr;
ID3D11DeviceContext* D3D::deviceContext = nullptr;
IDXGISwapChain* D3D::swapChain = nullptr;

D3D* D3D::Get()
{
	return instance;
}

void D3D::Create()
{
	assert(instance == nullptr);

	instance = new D3D();
}

void D3D::Delete()
{
	SafeDelete(instance);
}


void D3D::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	if (rtv == nullptr)
		rtv = renderTargetView;
	if (dsv = nullptr)
		dsv = depthStencilView;

	D3D::GetDC()->OMSetRenderTargets(1, &rtv, dsv);
}

void D3D::Clear(D3DXCOLOR color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	if (rtv == nullptr)
		rtv = renderTargetView;

	if (dsv == nullptr)
		dsv = depthStencilView;

	deviceContext->ClearRenderTargetView(rtv, color);
	deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void D3D::Present()
{
	swapChain->Present(d3dDesc.bVsync == true ? 1 : 0, 0);
}

void D3D::ResizeScreen(float width, float height) //백버퍼삭제, 창조절 이후 백버퍼 재형성
{
	if (width < 1 || height < 1)
		return;
	d3dDesc.Width = width;
	d3dDesc.Height = height;

	DeleteBackBuffer(); 
	{
		HRESULT hr = swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
	}
	CreateBackBuffer(width, height);
}	

D3D::D3D()
	:numerator(0),denominator(1)
{
	SetGpuInfo();
	CreateSwapChainAndDevic();
	CreateBackBuffer(d3dDesc.Width, d3dDesc.Height);
}

D3D::~D3D()
{
	DeleteBackBuffer();

	if (swapChain != nullptr)
		swapChain->SetFullscreenState(false, NULL);

	SafeRelease(deviceContext);
	SafeRelease(device);
	SafeRelease(swapChain);
}

void D3D::EnumerateAdapters() //어댑터 찾기
{
	IDXGIFactory1* factory;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
	{
		return;
	}

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter;
		HRESULT hr = factory->EnumAdapters1(index, &adapter);

		if (hr == DXGI_ERROR_NOT_FOUND)
			break;

		Check(hr);

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		adapterInfo->AdapterOrdinal = index; //0번은 주사용 어댑터
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->adapter = adapter;

		EnumerateAdapterOutput(adapterInfo);

		adapterInfos.push_back(adapterInfo);

		++index;
	}
	SafeRelease(factory);
}

bool D3D::EnumerateAdapterOutput(D3DEnumAdapterInfo* adapterInfo)
{
	HRESULT hr;
	IDXGIOutput* output = NULL;

	hr = adapterInfo->adapter->EnumOutputs(0, &output);
	if (DXGI_ERROR_NOT_FOUND == hr)
		return false;

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();
	ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));

	hr = output->GetDesc(&outputInfo->outputDesc);
	Check(hr);

	outputInfo->output = output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);


	for (UINT i = 0; i < numModes; i++)
	{
		bool isCheck = true;
		isCheck &= displayModes[i].Width == d3dDesc.Width;
		isCheck &= displayModes[i].Height == d3dDesc.Height;

		if (isCheck == true)
		{
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}

	SafeDeleteArray(displayModes);

	return true;
}

void D3D::SetGpuInfo()
{
}

void D3D::CreateSwapChainAndDevic()
{
}

void D3D::CreateBackBuffer(float width, float height)
{
}

void D3D::DeleteBackBuffer()
{
}

D3DEnumAdapterInfo::D3DEnumAdapterInfo()
{
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
}

D3DEnumOutputInfo::D3DEnumOutputInfo()
{
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
}

