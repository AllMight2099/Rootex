#include "rendering_device.h"

#include "common/common.h"
#include "dxgiDebugInterface.h"
#include "d3d11utils.h"

RenderingDevice::RenderingDevice()
{
}

RenderingDevice::~RenderingDevice()
{
	SafeRelease(&m_Device);
	SafeRelease(&m_SwapChain);
	SafeRelease(&m_Context);
}

void RenderingDevice::initialize(HWND hWnd, int width, int height)
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = {};

	HRESULT hr = D3D11CreateDevice(0, // Default adapter
	    D3D_DRIVER_TYPE_HARDWARE,
	    0, // No software device
	    createDeviceFlags,
	    0,
	    0, // Default feature level array
	    D3D11_SDK_VERSION,
	    &m_Device,
	    &featureLevel,
	    &m_Context);

	if (FAILED(hr))
	{
		ERR("D3D11CreateDevice Failed.");
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		ERR("Direct3D Feature Level 11 unsupported.");
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4XMSQuality);
	PANIC(m_4XMSQuality <= 0, "MSAA is not supported on this hardware");

	//if (m_4XMSQuality)
	if (false)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4XMSQuality - 1;
	}
	else // No MSAA
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	
	IDXGIAdapter* dxgiAdapter = 0; 
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0; 
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain);
	
	SafeRelease(&dxgiDevice);
	SafeRelease(&dxgiAdapter);
	SafeRelease(&dxgiFactory);

	ID3D11Resource* backBuffer = nullptr;
	GFX_ERR_CHECK(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer)));
	GFX_ERR_CHECK(m_Device->CreateRenderTargetView(
	    backBuffer,
	    nullptr,
	    &m_RenderTargetView));
	SafeRelease(&backBuffer);

	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	ID3D11DepthStencilState* DSState;
	GFX_ERR_CHECK(m_Device->CreateDepthStencilState(&dsDesc, &DSState));
	m_Context->OMSetDepthStencilState(DSState, 1u);
	SafeRelease(&DSState);

	ID3D11Texture2D* depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC descDepth = { 0 };
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_ERR_CHECK(m_Device->CreateTexture2D(&descDepth, nullptr, &depthStencil));
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSView = {};
	descDSView.Format = DXGI_FORMAT_D32_FLOAT;
	descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSView.Texture2D.MipSlice = 0u;
	GFX_ERR_CHECK(m_Device->CreateDepthStencilView(depthStencil, &descDSView, &m_DepthStencilView));
	SafeRelease(&depthStencil);

	m_Context->OMSetRenderTargets(1u, &m_RenderTargetView, m_DepthStencilView);
}

ID3DBlob* RenderingDevice::createBlob(LPCWSTR path)
{
	ID3DBlob* pBlob = nullptr;
	GFX_ERR_CHECK(D3DReadFileToBlob(path, &pBlob));
	return pBlob;
}

void RenderingDevice::initVertexBuffer(D3D11_BUFFER_DESC* vbd, D3D11_SUBRESOURCE_DATA* vsd, const UINT* stride, const UINT* const offset)
{
	ID3D11Buffer* pVertexBuffer = nullptr;
	GFX_ERR_CHECK(m_Device->CreateBuffer(vbd, vsd, &pVertexBuffer));
	m_Context->IASetVertexBuffers(0u, 1u, &pVertexBuffer, stride, offset);
	SafeRelease(&pVertexBuffer);
}

void RenderingDevice::initIndexBuffer(D3D11_BUFFER_DESC* ibd, D3D11_SUBRESOURCE_DATA* isd, DXGI_FORMAT format)
{
	ID3D11Buffer* pIndexBuffer = nullptr;
	GFX_ERR_CHECK(m_Device->CreateBuffer(ibd, isd, &pIndexBuffer));
	m_Context->IASetIndexBuffer(pIndexBuffer, format, 0u);
	SafeRelease(&pIndexBuffer);
}

void RenderingDevice::initVSConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd)
{
	ID3D11Buffer* pConstantBuffer = nullptr;
	GFX_ERR_CHECK(m_Device->CreateBuffer(cbd, csd, &pConstantBuffer));
	m_Context->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);

	SafeRelease(&pConstantBuffer);
}

void RenderingDevice::initPSConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd)
{
	ID3D11Buffer* pConstantBuffer = nullptr;
	GFX_ERR_CHECK(m_Device->CreateBuffer(cbd, csd, &pConstantBuffer));
	m_Context->PSSetConstantBuffers(0u, 1u, &pConstantBuffer);

	SafeRelease(&pConstantBuffer);
}

void RenderingDevice::initPixelShader(LPCWSTR shader_path)
{
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3DBlob* m_Blob = createBlob(shader_path);
	GFX_ERR_CHECK(m_Device->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &pPixelShader));
	m_Context->PSSetShader(pPixelShader, nullptr, 0u);
	SafeRelease(&m_Blob);
	SafeRelease(&pPixelShader);
}

ID3DBlob* RenderingDevice::initVertexShader(LPCWSTR shader_path)
{
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3DBlob* m_Blob = createBlob(shader_path);
	GFX_ERR_CHECK(m_Device->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader));
	m_Context->VSSetShader(m_VertexShader, nullptr, 0u);
	SafeRelease(&m_VertexShader);
	return m_Blob;
}

void RenderingDevice::initVertexLayout(ID3DBlob* vertexShaderBlob, const D3D11_INPUT_ELEMENT_DESC* ied, UINT size)
{
	ID3D11InputLayout* pInputLayout = nullptr;
	GFX_ERR_CHECK(m_Device->CreateInputLayout(
	    ied, size,
	    vertexShaderBlob->GetBufferPointer(),
	    vertexShaderBlob->GetBufferSize(),
	    &pInputLayout));

	SafeRelease(&vertexShaderBlob);

	//bind vertex layout
	m_Context->IASetInputLayout(pInputLayout);

	SafeRelease(&pInputLayout);
}

void RenderingDevice::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY pt)
{
	m_Context->IASetPrimitiveTopology(pt);
}

void RenderingDevice::setViewports(D3D11_VIEWPORT* vp)
{
	m_Context->RSSetViewports(1u, vp);
}

void RenderingDevice::drawIndexed(UINT number)
{
	m_Context->DrawIndexed(number, 0u, 0u);
}

RenderingDevice* RenderingDevice::GetSingleton()
{
	static RenderingDevice singleton;
	return &singleton;
}

void RenderingDevice::swapBuffers()
{
	GFX_ERR_CHECK(m_SwapChain->Present(1u, 0));
}

void RenderingDevice::clearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	m_Context->ClearRenderTargetView(m_RenderTargetView, color);
	m_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
