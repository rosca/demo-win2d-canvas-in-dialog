#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "DialogContent.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace winrt::VirtualSurfaceImageSource::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    fire_and_forget MainPage::Click_ShowDialog(IInspectable const&, RoutedEventArgs const&)
    {
        auto dialog = ContentDialog{};
        dialog.Content(VirtualSurfaceImageSource::DialogContent{});
        co_await dialog.ShowAsync();
    }
}
