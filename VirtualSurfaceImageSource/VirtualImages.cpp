#include "pch.h"
#include "VirtualImages.h"
#if __has_include("VirtualImages.g.cpp")
#include "VirtualImages.g.cpp"
#endif

#include <functional>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::Display;

namespace {

constexpr float kDefaultDpi = 96.0f;

float pixelsToDips(int32_t pixels, float dpi);

enum class DpiRounding { kFloor, kRound, kCeiling };
float pixelsToDips(int32_t pixels, float dpi) {
    return pixels * kDefaultDpi / dpi;
}

int32_t dipsToPixels(float dips, float dpi, DpiRounding dpiRounding) {
    float scaled = dips * dpi / kDefaultDpi;
    switch (dpiRounding) {
    case DpiRounding::kFloor: scaled = floorf(scaled); break;
    case DpiRounding::kRound: scaled = roundf(scaled); break;
    case DpiRounding::kCeiling: scaled = ceilf(scaled); break;
    }

    return static_cast<int32_t>(scaled);
}

int32_t sizeDipsToPixels(float dips, float dpi) {
    int32_t result = dipsToPixels(dips, dpi, DpiRounding::kRound);

    // Zero versus non-zero is pretty important for things like control sizes, so we want
    // to avoid ever rounding non-zero input sizes down to zero during conversion to pixels.
    // If the input value was small but positive, it's safer to round up to one instead.
    if (result == 0 && dips > 0) {
        return 1;
    }

    return result;
}

RECT toRECT(const winrt::Windows::Foundation::Rect& rect, float dpi) {
    auto left = dipsToPixels(rect.X, dpi, DpiRounding::kRound);
    auto top = dipsToPixels(rect.Y, dpi, DpiRounding::kRound);
    auto right = dipsToPixels(rect.X + rect.Width, dpi, DpiRounding::kRound);
    auto bottom = dipsToPixels(rect.Y + rect.Height, dpi, DpiRounding::kRound);

    if (right == left && rect.Width > 0)
        right++;

    if (bottom == top && rect.Height > 0)
        bottom++;

    return RECT{left, top, right, bottom};
}

winrt::Windows::Foundation::Rect toRect(RECT const& rect, float dpi) {
    auto x = pixelsToDips(rect.left, dpi);
    auto y = pixelsToDips(rect.top, dpi);
    auto width = pixelsToDips(rect.right - rect.left, dpi);
    auto height = pixelsToDips(rect.bottom - rect.top, dpi);

    winrt::Windows::Foundation::Rect rc;
    rc.X = x;
    rc.Y = y;
    rc.Width = width;
    rc.Height = height;
    return rc;
}

inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        throw winrt::hresult_error(hr);
;    }
}

}

namespace winrt::VirtualSurfaceImageSource::implementation
{
    VirtualImages::VirtualImages()
    {
        InitializeComponent();

        createDevice();

        // The big source has a fixed size.
        m_bigSource = make_self<VirtualSurfaceImage>(bigImageContainer().ActualSize(), m_d2dDevice.get());
        bigImageContainer().Source(m_bigSource->source());

        m_smallSource = make_self<VirtualSurfaceImage>(smallImageContainer().ActualSize(), m_d2dDevice.get());
        ImageBrush brush;
        brush.ImageSource(m_smallSource->source());
        smallImageContainer().Background(brush);
    }

    void VirtualImages::invalidate() {
        m_bigSource->invalidate();
        m_smallSource->invalidate();
    }

    void VirtualImages::smallImageContainer_SizeChanged(::IInspectable const&, SizeChangedEventArgs const& e) {
        auto dpi = DisplayInformation::GetForCurrentView().LogicalDpi();
        // The big source has a fixed size.
        auto width = sizeDipsToPixels(dpi, e.NewSize().Width);
        auto height = sizeDipsToPixels(dpi, e.NewSize().Height);
        com_ptr<IVirtualSurfaceImageSourceNative> native(m_smallSource->source().as<IVirtualSurfaceImageSourceNative>());
        native->Resize(width, height);
    }

    void VirtualImages::createDevice() {
        D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
        UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        ThrowIfFailed(D3D11CreateDevice(NULL,  // adapter
            driverType,
            NULL,  // software handle
            deviceFlags,
            nullptr,  // feature levels default
            0,        // feature level count
            D3D11_SDK_VERSION,
            m_d3dDevice.put(),
            NULL,
            NULL));

        auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();

        // Create the D2D device
        ThrowIfFailed(D2D1CreateDevice(dxgiDevice.get(), NULL, m_d2dDevice.put()));
    }

    VirtualSurfaceImage::VirtualSurfaceImage(winrt::Windows::Foundation::Size const& size, ID2D1Device* device) : m_size(size) {
        m_dpi = DisplayInformation::GetForCurrentView().LogicalDpi();
        // The big source has a fixed size.
        auto width = sizeDipsToPixels(m_dpi, size.Width);
        auto height = sizeDipsToPixels(m_dpi, size.Height);
        m_source = Imaging::VirtualSurfaceImageSource(width, height);
        auto nativeD2D(m_source.as<ISurfaceImageSourceNativeWithD2D>());
        ThrowIfFailed(nativeD2D->SetDevice(device));
        auto native = m_source.as<IVirtualSurfaceImageSourceNative>();
        ThrowIfFailed(native->RegisterForUpdatesNeeded(this));
    }

    HRESULT VirtualSurfaceImage::UpdatesNeeded() {
        auto native = m_source.as<IVirtualSurfaceImageSourceNative>();
        DWORD rectCount = 0;
        ThrowIfFailed(native->GetUpdateRectCount(&rectCount));
        if (!rectCount) {
            return S_OK;
        }
        auto rects = std::make_unique<RECT[]>(rectCount);
        ThrowIfFailed(native->GetUpdateRects(rects.get(), rectCount));
        
        auto nativeD2D(m_source.as<ISurfaceImageSourceNativeWithD2D>());

        for (size_t i = 0; i < rectCount; ++i) {
            auto& rect = rects[i];
            com_ptr<ID2D1DeviceContext> context;
            POINT offset;
            ThrowIfFailed(nativeD2D->BeginDraw(rect, __uuidof(ID2D1DeviceContext), context.put_void(), &offset));
            D2D1_COLOR_F color{ 0, 1, 0, 1 };  // green
            context->Clear(color);
            nativeD2D->EndDraw();
        }
        return S_OK;
    }

    void VirtualSurfaceImage::invalidate() {
        auto native = m_source.as<IVirtualSurfaceImageSourceNative>();
        auto width = sizeDipsToPixels(m_dpi, m_size.Width);
        auto height = sizeDipsToPixels(m_dpi, m_size.Height);
        RECT rc{ 0, 0, width, height };
        ThrowIfFailed(native->Invalidate(rc));
    }
}
