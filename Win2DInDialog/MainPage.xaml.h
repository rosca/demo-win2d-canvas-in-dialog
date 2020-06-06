//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace Win2DInDialog
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void Click_ShowDialog(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Click_CloseDialog(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Click_Invalidate(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		Windows::UI::Xaml::Controls::ContentDialog^ dialog_;
		Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl^ canvas_;
		void OnRegionsInvalidated(Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasRegionsInvalidatedEventArgs^ args);
	};
}
