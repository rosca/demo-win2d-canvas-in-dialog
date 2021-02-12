#pragma once
#include "App.xaml.g.h"

namespace winrt::VirtualSurfaceImageSource::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

        void onEnteringBackground(IInspectable const&, Windows::ApplicationModel::EnteredBackgroundEventArgs const&);
        void onLeavingBackground(IInspectable const&, Windows::ApplicationModel::LeavingBackgroundEventArgs const&);
        void onApplicationResuming(IInspectable const&, IInspectable const&);

    private:
       bool  enteredBackGround_ = false;
    };
}
