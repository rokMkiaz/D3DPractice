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
	if (rtv == nullptr)//널이들어오면 시스템, 아니면 사용자
		rtv = renderTargetView;
	
	if (dsv == nullptr)
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

	d3dDesc.width = width;
	d3dDesc.height = height;

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


	CreateSwapChainAndDevice();
	CreateBackBuffer(d3dDesc.width, d3dDesc.height);
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
	Check(hr);

	displayModes = new DXGI_MODE_DESC[numModes];
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	Check(hr);


	for (UINT i = 0; i < numModes; i++)
	{
		bool isCheck = true;
		isCheck &= displayModes[i].Width == d3dDesc.width;
		isCheck &= displayModes[i].Height == d3dDesc.height;

		if (isCheck == true)
		{
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}
	adapterInfo->outputInfo = outputInfo;

	SafeDeleteArray(displayModes);

	return true;
}

void D3D::SetGpuInfo()
{
	EnumerateAdapters();
}

void D3D::CreateSwapChainAndDevice()
{
	SafeRelease(device);
	SafeRelease(deviceContext);
	SafeRelease(swapChain);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (d3dDesc.bVsync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = d3dDesc.handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !d3dDesc.bFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	creationFlags = D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
	};

	UINT maxVideoMemory = 0;  //그래픽카드 메모리 비교
	for (UINT i = 0; i < adapterInfos.size(); ++i)
	{
		if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory)
		{
			selectedAdapterIndex = i;
			maxVideoMemory = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
		}
	}

	numerator = adapterInfos[0]->outputInfo->numerator;
	denominator = adapterInfos[0]->outputInfo->denominator;

	gpuMemorySize = adapterInfos[selectedAdapterIndex]->adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterInfos[selectedAdapterIndex]->adapterDesc.Description;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		adapterInfos[selectedAdapterIndex]->adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		creationFlags,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		NULL,
		&deviceContext
	);
	assert(SUCCEEDED(hr));

}

void D3D::CreateBackBuffer(float width, float height)
{
	HRESULT hr;

	//Create RenderTargetView
	{
		ID3D11Texture2D* backBufferPointer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPointer);
		Check(hr)

		hr = D3D:: GetDevice()->CreateRenderTargetView(backBufferPointer, NULL, &renderTargetView);
		Check(hr)

			SafeRelease(backBufferPointer);
	}

	//Create Depth Stencil View
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //깊이에 대해 24비트, 스텐실에 8비트 지원하는 z버퍼
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;


		hr = D3D::GetDevice()->CreateTexture2D(&desc, NULL, &backBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = D3D::GetDevice()->CreateDepthStencilView(backBuffer, &desc, &depthStencilView);
		assert(SUCCEEDED(hr));

		SetRenderTarget(renderTargetView, depthStencilView);
	}
}

void D3D::DeleteBackBuffer()
{
	SafeRelease(depthStencilView);
	SafeRelease(renderTargetView);
	SafeRelease(backBuffer);
}

D3DEnumAdapterInfo::D3DEnumAdapterInfo()
	:adapter(nullptr), outputInfo(nullptr)
{
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
	SafeRelease(adapter);
	SafeDelete(outputInfo);
}

D3DEnumOutputInfo::D3DEnumOutputInfo()
	:output(nullptr), numerator(0), denominator(1)
{
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
	SafeRelease(output);
}

