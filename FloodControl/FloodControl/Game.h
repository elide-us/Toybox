//
// Game.h
//

#pragma once

#include "DeviceResources.h"	// Template
#include "StepTimer.h"			// Template
#include "GamePiece.h"			// Custom

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();
    void Render();

    // Rendering helpers
    void Clear();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_playingPieces;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_backgroundScreen;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_titleScreen;
};