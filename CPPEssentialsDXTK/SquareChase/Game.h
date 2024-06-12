#pragma once

#pragma region ...
#include <DirectXMath.h>
#include "SimpleMath.h"
#include "SpriteBatch.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "StepTimer.h"
#pragma endregion

#include <random>
static std::mt19937 s_rand(1974);
inline static int GetNext(int min, int max)
{
    static std::uniform_int_distribution<int> d(min, max);
    return d(s_rand);
}

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
#pragma region ...
public:

    Game() noexcept;
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
#pragma endregion

    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice; // The D3D11 device (GPU object factory)
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext; // The D3D11 rendering context

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain; // Swap chain for legacy flip model

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView; // Back buffer for render
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView; // UV for render

    DX::StepTimer                                   m_timer; // Frame timing and more

    std::unique_ptr<DirectX::Keyboard>              m_keyboard; // Keyboard state
    std::unique_ptr<DirectX::Mouse>                 m_mouse; // Mouse state

    std::unique_ptr<DirectX::SpriteBatch>           spriteBatch;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> squareTexture;
    DirectX::SimpleMath::Rectangle currentSquare;

    int playerScore = 0;
    float timeRemaining = 0.f;
    const float TimePerSquare = 0.75f;

    std::vector<DirectX::XMVECTORF32> colors = { DirectX::Colors::Red, DirectX::Colors::Green, DirectX::Colors::Blue };
};
