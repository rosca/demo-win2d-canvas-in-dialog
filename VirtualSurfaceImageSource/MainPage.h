#pragma once

#include "MainPage.g.h"

namespace winrt::VirtualSurfaceImageSource::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        fire_and_forget Click_ShowDialog(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::VirtualSurfaceImageSource::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
