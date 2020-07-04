#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#include "winrt/Windows.UI.Xaml.Media.Imaging.h"
#include <winrt/Windows.Graphics.Display.h>
#include "VirtualImages.g.h"
#include <D3d11.h>
#include <D2d1_1.h>
#include <windows.ui.xaml.media.dxinterop.h>

namespace winrt::VirtualSurfaceImageSource::implementation
{

    struct VirtualSurfaceImage : implements<VirtualSurfaceImage, IVirtualSurfaceUpdatesCallbackNative> {

        VirtualSurfaceImage(winrt::Windows::Foundation::Size const&, ID2D1Device*);

        HRESULT UpdatesNeeded();

        void invalidate();

        winrt::Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource const& source() { return m_source; }

    private:
        winrt::Windows::Foundation::Size m_size;
        float m_dpi;
        winrt::Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource m_source{ nullptr };
    };

    struct VirtualImages : VirtualImagesT<VirtualImages>
    {
        VirtualImages();

        void invalidate();

    private:
        com_ptr<VirtualSurfaceImage> m_bigSource;
        com_ptr<VirtualSurfaceImage> m_smallSource;

        winrt::com_ptr<ID3D11Device> m_d3dDevice;
        winrt::com_ptr<ID2D1Device> m_d2dDevice;

        void createDevice();

    public:
        void smallImageContainer_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e);
    };
}

namespace winrt::VirtualSurfaceImageSource::factory_implementation
{
    struct VirtualImages : VirtualImagesT<VirtualImages, implementation::VirtualImages>
    {
    };
}
