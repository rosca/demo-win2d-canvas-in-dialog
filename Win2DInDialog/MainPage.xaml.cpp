//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Win2DInDialog;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Microsoft::Graphics::Canvas::UI::Xaml;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}


void Win2DInDialog::MainPage::Click_ShowDialog(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	dialog_ = ref new ContentDialog();
	auto grid = ref new Grid();

	auto row1 = ref new RowDefinition();
	auto row2 = ref new RowDefinition();
	row2->Height = 40;
	row1->Height = 300;
	grid->RowDefinitions->Append(row1);
	grid->RowDefinitions->Append(row2);

	auto closeButton = ref new Button();
	closeButton->Content = ref new Platform::String(L"Close");
	closeButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &Win2DInDialog::MainPage::Click_CloseDialog);
	Grid::SetRow(closeButton, 1);

	auto invalidateButton = ref new Button();
	invalidateButton->Content = ref new Platform::String(L"Invalidate");
	invalidateButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &Win2DInDialog::MainPage::Click_Invalidate);
	Grid::SetRow(invalidateButton, 1);
	invalidateButton->Margin = Thickness(100, 0, 0, 0);

	auto virtualControl = ref new CanvasVirtualControl();
	virtualControl->RegionsInvalidated += ref new Windows::Foundation::TypedEventHandler<Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl^, Microsoft::Graphics::Canvas::UI::Xaml::CanvasRegionsInvalidatedEventArgs^>(this, &Win2DInDialog::MainPage::OnRegionsInvalidated);
	Grid::SetRow(virtualControl, 0);

	grid->Children->Append(closeButton);
	grid->Children->Append(invalidateButton);
	grid->Children->Append(virtualControl);
	dialog_->Content = grid;

	canvas_ = virtualControl;

	dialog_->ShowAsync();
}


void Win2DInDialog::MainPage::Click_CloseDialog(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	dialog_->Hide();
	dialog_ = nullptr;
}
void Win2DInDialog::MainPage::Click_Invalidate(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	canvas_->Invalidate();
}


void Win2DInDialog::MainPage::OnRegionsInvalidated(Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl^ control, Microsoft::Graphics::Canvas::UI::Xaml::CanvasRegionsInvalidatedEventArgs^ args)
{
	for (const auto& region : args->InvalidatedRegions) {
		if (auto drawingSession = control->CreateDrawingSession(region)) {
			Windows::Foundation::Numerics::float2 center;
			center.x = static_cast<float>(control->ActualWidth) / 2;
			center.y = static_cast<float>(control->ActualHeight) / 2;
			auto radius = min(center.x, center.y);
			drawingSession->FillCircle(center, radius, Windows::UI::Colors::Green);
			delete drawingSession;
			drawingSession = nullptr;
		}
	}
}
