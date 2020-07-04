#include "pch.h"
#include "DialogContent.h"
#if __has_include("DialogContent.g.cpp")
#include "DialogContent.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Media.h>
#include "VirtualImages.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::VirtualSurfaceImageSource::implementation
{
    DialogContent::DialogContent()
    {
        InitializeComponent();
    }

    void DialogContent::ButtonClose(IInspectable const&, RoutedEventArgs const&)
    {
        auto parent = Media::VisualTreeHelper::GetParent(*this);
        while (parent && !parent.try_as<Controls::ContentDialog>()) {
            parent = Media::VisualTreeHelper::GetParent(parent);
        }
        if (parent) {
            parent.as<Controls::ContentDialog>().Hide();
        }
    }

    void DialogContent::ButtonInvalidate(IInspectable const&, RoutedEventArgs const&)
    {
        get_self<VirtualImages>(images())->invalidate();
    }
}
