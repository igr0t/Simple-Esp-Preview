#include "window.hpp"
#include "../../esp.h"
#include <dwmapi.h>
#include <stdio.h>
#include "../../aim.h"
#pragma comment(lib, "dxgi.lib")



static void DrawESPPreview(ImDrawList* draw,
	ImVec2 pos,
	ImVec2 size,
	bool showName,
	bool showBox,
	bool showHealth,
	bool filled,
	bool showWeapon,
	bool showSkeleton,
	float scale = 1.0f)
{

	const float pad = 6.0f * scale;
	ImVec2 boxTL = ImVec2(pos.x + pad, pos.y + pad);
	ImVec2 boxBR = ImVec2(pos.x + size.x - pad, pos.y + size.y - pad);

	float boxW = (boxBR.x - boxTL.x) * 0.5f * scale;
	float boxH = (boxBR.y - boxTL.y) * 0.75f * scale;
	ImVec2 pCenter = ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);

	ImVec2 playerTL = ImVec2(pCenter.x - boxW * 0.5f, pCenter.y - boxH * 0.5f);
	ImVec2 playerBR = ImVec2(pCenter.x + boxW * 0.5f, pCenter.y + boxH * 0.5f);

	ImU32 boxCol = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // branco
	ImU32 textCol = ImGui::GetColorU32(ImVec4(1, 1, 1, 1));
	ImU32 bgCol = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.25f));

	
	draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgCol, 6.0f * scale);

	
	if (showBox) {
		
		float height = (playerBR.y - playerTL.y);
		float width = (playerBR.x - playerTL.x);
		float x = playerTL.x;
		float y = playerTL.y;

		float lineLength = width * 0.2f;
		float thickness = 2.0f * scale;

		
		draw->AddLine(ImVec2(x, y), ImVec2(x + lineLength, y), boxCol, thickness);
		draw->AddLine(ImVec2(x, y), ImVec2(x, y + lineLength), boxCol, thickness);

		
		draw->AddLine(ImVec2(x + width, y), ImVec2(x + width - lineLength, y), boxCol, thickness);
		draw->AddLine(ImVec2(x + width, y), ImVec2(x + width, y + lineLength), boxCol, thickness);

	
		draw->AddLine(ImVec2(x, y + height), ImVec2(x + lineLength, y + height), boxCol, thickness);
		draw->AddLine(ImVec2(x, y + height), ImVec2(x, y + height - lineLength), boxCol, thickness);

	
		draw->AddLine(ImVec2(x + width, y + height), ImVec2(x + width - lineLength, y + height), boxCol, thickness);
		draw->AddLine(ImVec2(x + width, y + height), ImVec2(x + width, y + height - lineLength), boxCol, thickness);
	}


	if (showHealth) {
		float hbW = 8.0f * scale;
		ImVec2 hbTL = ImVec2(playerBR.x + 10.0f * scale, playerTL.y);
		ImVec2 hbBR = ImVec2(hbTL.x + hbW, playerBR.y);
		float hpPercent = 0.75f; // sample value
		ImVec2 hpFillTL = ImVec2(hbTL.x, hbBR.y - (hbBR.y - hbTL.y) * hpPercent);
		draw->AddRect(hbTL, hbBR, ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.8f)), 3.0f * scale);
		draw->AddRectFilled(hpFillTL, hbBR, ImGui::GetColorU32(ImVec4(1.0f - hpPercent, hpPercent, 0.0f, 1.0f)), 3.0f * scale);

		
		char hpbuf[8];
		int hp = static_cast<int>(hpPercent * 100.0f);
		snprintf(hpbuf, sizeof(hpbuf), "%d", hp);
		draw->AddText(ImVec2(hbTL.x - 6.0f * scale, hbBR.y + 2.0f * scale), textCol, hpbuf);
	}

	
	if (showName) {
		const char* sampleName = "Player Name";
		ImVec2 textSize = ImGui::CalcTextSize(sampleName);
		ImVec2 textPos = ImVec2(pCenter.x - textSize.x * 0.5f, playerTL.y - textSize.y - 6.0f * scale);
		draw->AddText(textPos, textCol, sampleName);
	}

	
	if (showWeapon) {
		const char* weapon = "Weapon";
		
		ImVec2 weaponPos = ImVec2(playerTL.x, playerBR.y + 6.0f * scale);
		
		draw->AddText(weaponPos, textCol, weapon);
		
	}

	
	//if (showSkeleton) {
	//	
	//	ImU32 skelCol = ImGui::GetColorU32(ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
	//	ImVec2 head = ImVec2(pCenter.x, playerTL.y + 10.0f * scale);
	//	ImVec2 chest = ImVec2(pCenter.x, playerTL.y + 30.0f * scale);
	//	ImVec2 leftHand = ImVec2(pCenter.x - 20.0f * scale, playerTL.y + 40.0f * scale);
	//	ImVec2 rightHand = ImVec2(pCenter.x + 20.0f * scale, playerTL.y + 40.0f * scale);
	//	ImVec2 hip = ImVec2(pCenter.x, playerTL.y + 60.0f * scale);
	//	ImVec2 lLeg = ImVec2(pCenter.x - 12.0f * scale, playerTL.y + 90.0f * scale);
	//	ImVec2 rLeg = ImVec2(pCenter.x + 12.0f * scale, playerTL.y + 90.0f * scale);

	//	draw->AddLine(head, chest, skelCol, 2.0f * scale);
	//	draw->AddLine(chest, leftHand, skelCol, 2.0f * scale);
	//	draw->AddLine(chest, rightHand, skelCol, 2.0f * scale);
	//	draw->AddLine(chest, hip, skelCol, 2.0f * scale);
	//	draw->AddLine(hip, lLeg, skelCol, 2.0f * scale);
	//	draw->AddLine(hip, rLeg, skelCol, 2.0f * scale);
	//	draw->AddCircleFilled(head, 4.0f * scale, skelCol);
	//}
}


void Overlay::SetCaptureBypass(bool CaptureBypassOn) {
	SetWindowDisplayAffinity(overlay, CaptureBypassOn ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
}

inline bool CaptureBypass = false;


// declaration of the ImGui_ImplWin32_WndProcHandler function
// basically integrates ImGui with the Windows message loop so ImGui can process input and events
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// set up ImGui window procedure handler
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	// switch that disables alt application and checks for if the user tries to close the window.
	switch (msg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu (imgui uses it in their example :shrug:)
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		return 0;
	}

	// define the window procedure
	return DefWindowProc(window, msg, wParam, lParam);
}

float Overlay::GetRefreshRate()
{
	// get dxgi variables
	IDXGIFactory* dxgiFactory = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIOutput* dxgiOutput = nullptr;
	DXGI_MODE_DESC modeDesc;

	// create DXGI factory
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&dxgiFactory))))
		return 60;

	// get the adapter (aka GPU)
	if (FAILED(dxgiFactory->EnumAdapters(0, &dxgiAdapter))) {
		dxgiAdapter->Release();
		return 60;
	}

	// get the MAIN monitor - to add multiple, you should use "dxgiAdapter->EnumOutputs" to loop through each monitor, and save it.
	// then, you can access the refresh rate of each one and save the highest one, then set the refreshrate to that.
	// i haven't had any issues just using the main one though.
	if (FAILED(dxgiAdapter->EnumOutputs(0, &dxgiOutput))) {
		dxgiAdapter->Release();
		dxgiFactory->Release();
		return 60;
	}

	// iterate through display modes
	UINT numModes = 0;
	if (FAILED(dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr))) {
		dxgiOutput->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();
		return 60;
	}

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (FAILED(dxgiOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, displayModeList))) {
		delete[] displayModeList;
		dxgiOutput->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();
		return 60;
	}

	float refreshRate = 60;
	// next, find the refresh rate
	for (int i = 0; i < numModes; ++i) {
		// check 
		float hz = static_cast<float>(displayModeList[i].RefreshRate.Numerator) /
			static_cast<float>(displayModeList[i].RefreshRate.Denominator);
		
		// make sure hz didn't return 0, and is more or the same.
		if (hz != 0 && hz >= refreshRate)
			refreshRate = hz;
	}

	delete[] displayModeList;
	dxgiOutput->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	printf("[>>] Refresh rate: %f", refreshRate);
	printf("\n"); // i genuinely do not care anymore

	return refreshRate;
}

bool Overlay::CreateDevice()
{
	// First we setup our swap chain, this basically just holds a bunch of descriptors for the swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// set number of back buffers (this is double buffering)
	sd.BufferCount = 2;

	// width + height of buffer, (0 is automatic sizing)
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;

	// set the pixel format
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// get the fps from GetRefreshRate(). If anything fails it just returns 60 anyways.
	sd.BufferDesc.RefreshRate.Numerator = GetRefreshRate(); 
	sd.BufferDesc.RefreshRate.Denominator = 1;

	// allow mode switch (changing display modes)
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// set how the bbuffer will be used
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.OutputWindow = overlay;

	// setup the multi-sampling
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// specify what Direct3D feature levels to use
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	
	// create device and swap chain
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		featureLevelArray,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&featureLevel,
		&device_context);

	// if the hardware isn't supported create with WARP (basically just a different renderer)
	if (result == DXGI_ERROR_UNSUPPORTED) {
		result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			0U,
			featureLevelArray,
			2, D3D11_SDK_VERSION,
			&sd,
			&swap_chain,
			&device,
			&featureLevel,
			&device_context);

		printf("[>>] DXGI_ERROR | Created with D3D_DRIVER_TYPE_WARP\n");
	}

	// can't do much more, if the hardware still isn't supported just return false.
	if (result != S_OK) {
		printf("[>>] Device Not Okay\n");
		return false;
	}

	// retrieve back_buffer, im defining it here since it isn't being used at any other point in time.
	ID3D11Texture2D* back_buffer{ nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	// if back buffer is obtained then we can create render target view and release the back buffer again
	if (back_buffer) 
	{
		device->CreateRenderTargetView(back_buffer, nullptr, &render_targetview);
		back_buffer->Release();

		printf("[>>] Created Device\n");
		return true;
	}

	// if we reach this point then it failed to create the back buffer
	printf("[>>] Failed to create Device\n");
	return false;
}

void Overlay::DestroyDevice()
{
	// release everything that has to do with the device.
	if (device)
	{
		device->Release();
		device_context->Release();
		swap_chain->Release();
		render_targetview->Release();

		printf("[>>] Released Device\n");
	}
	else
		printf("[>>] Device Not Found when Exiting.\n");
}

void Overlay::CreateOverlay(const char* window_name)
{
	// holds descriptors for the window, called a WindowClass
	// set up window class
	wc.cbSize = sizeof(wc);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = GetModuleHandleA(0);
	wc.lpszClassName = "bolagato";

	// register our class
	RegisterClassEx(&wc);

	// create window (the actual one that shows up in your taskbar)
	// WS_EX_TOOLWINDOW hides the new window that shows up in your taskbar and attaches it to any already existing windows instead.
	// (in this case the console)
	overlay = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		wc.lpszClassName,
		window_name,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN), // 1920
		GetSystemMetrics(SM_CYSCREEN), // 1080
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	if (overlay == NULL)
		printf("[>>] Failed to create Overlay\n");

	// set overlay window attributes to make the overlay transparent
	SetLayeredWindowAttributes(overlay, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	// set up the DWM frame extension for client area
	{
		// first we define our RECT structures that hold our client and window area
		RECT client_area{};
		RECT window_area{};

		// get the client and window area
		GetClientRect(overlay, &client_area);
		GetWindowRect(overlay, &window_area);

		// calculate the difference between the screen and window coordinates
		POINT diff{};
		ClientToScreen(overlay, &diff);

		// calculate the margins for DWM frame extension
		const MARGINS margins{
			window_area.left + (diff.x - window_area.left),
			window_area.top + (diff.y - window_area.top),
			client_area.right,
			client_area.bottom
		};

		// then we extend the frame into the client area
		DwmExtendFrameIntoClientArea(overlay, &margins);
	}

	// show + update overlay
	ShowWindow(overlay, SW_SHOW);
	UpdateWindow(overlay);

	printf("[>>] Overlay Created\n");
}

void Overlay::DestroyOverlay()
{
	DestroyWindow(overlay);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool Overlay::CreateImGui()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Initalize ImGui for the Win32 library
	if (!ImGui_ImplWin32_Init(overlay)) {
		printf("[>>] Failed ImGui_ImplWin32_Init\n");
		return false;
	}
	
	// Initalize ImGui for DirectX 11.
	if (!ImGui_ImplDX11_Init(device, device_context)) {
		printf("[>>] Failed ImGui_ImplDX11_Init\n");
		return false;
	}

	printf("[>>] ImGui Initialized\n");
	return true;
}

void Overlay::DestroyImGui()
{
	// Cleanup ImGui by shutting down DirectX11, the Win32 Platform and Destroying the ImGui context.
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}



void Overlay::StartRender()
{
	// handle windows messages
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// begin a new frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	// if the user presses Insert then enable the menu.
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		RenderMenu = !RenderMenu;

		// If we are rendering the menu set the window styles to be able to clicked on.
		if (RenderMenu) {
			SetWindowLong(overlay, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
		}
		else {
			SetWindowLong(overlay, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED);
		}
	}
}

void Overlay::EndRender()
{
	// Render ImGui
	ImGui::Render();

	// Make a color that's clear / transparent
	float color[4]{ 0, 0, 0, 0 };

	// Set the render target and then clear it
	device_context->OMSetRenderTargets(1, &render_targetview, nullptr);
	device_context->ClearRenderTargetView(render_targetview, color);

	// Render ImGui draw data.
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present rendered frame with V-Sync
	swap_chain->Present(1U, 0U);

	// Present rendered frame without V-Sync
	//swap_chain->Present(0U, 0U);
}

void Overlay::Render()
{
	ImGui::SetNextWindowSize({ 420, 520 });
	ImGui::Begin("Bola Gato | AssaultClub", &RenderMenu, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::BeginTabBar("Hacks")) {
		if (ImGui::BeginTabItem("Aim")) {
			ImGui::Checkbox("Aimbot", &AimbotActive);
			if (AimbotActive) {
				ImGui::SliderFloat("FOV", &FieldOfView, 50.0f, 300.0f);
			}
			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem("Visuals")) {
			ImGui::Checkbox("Esp Line", &espLineActive);
			ImGui::Checkbox("Esp Name", &EspNameActive);
			ImGui::Checkbox("Esp Box", &EspBoxActive);
			ImGui::Checkbox("Esp Health", &EspHealthActive);
			ImGui::Checkbox("Filled Box", &FilledBoxActive);
			

			ImGui::Separator();

			
			ImGui::Text("Preview:");
			ImGui::BeginChild("ESPPreview", ImVec2(360, 320), true, ImGuiWindowFlags_NoScrollbar);
			{
				ImDrawList* draw = ImGui::GetWindowDrawList();
				ImVec2 childPos = ImGui::GetCursorScreenPos();
				ImVec2 childSize = ImGui::GetContentRegionAvail();
				ImGui::Dummy(childSize); 

				
				float previewScale = 1.0f;

				DrawESPPreview(draw, childPos, childSize,
					EspNameActive,   
					EspBoxActive,    
					EspHealthActive, 
					FilledBoxActive, 
					EspWeaponActive, 
					EspSkeletonActive,
					previewScale);
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::End();
	}

}
void Overlay::SetForeground(HWND window)
{
	if (!IsWindowInForeground(window))
		BringToForeground(window);
}