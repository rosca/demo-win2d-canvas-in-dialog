#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "DialogContent.g.h"

namespace winrt::VirtualSurfaceImageSource::implementation
{
    struct DialogContent : DialogContentT<DialogContent>
    {
        DialogContent();

        void ButtonClose(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void ButtonInvalidate(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::VirtualSurfaceImageSource::factory_implementation
{
    struct DialogContent : DialogContentT<DialogContent, implementation::DialogContent>
    {
    };
}
