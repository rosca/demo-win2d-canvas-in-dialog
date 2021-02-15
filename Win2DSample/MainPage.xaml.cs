using Microsoft.Graphics.Canvas.UI.Xaml;
using Microsoft.Graphics.Canvas.Geometry;
using Windows.UI;
using Windows.UI.Xaml.Controls;
using System.Windows;
using Windows.UI.Xaml;
using System.Threading.Tasks;


namespace Win2DSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        CanvasVirtualControl control;
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void virtualControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            control.Invalidate();
        }

        async Task PutTaskDelay()
        {
            await Task.Delay(5000);
        }

        public async void Click_Me(object sender, RoutedEventArgs e)
        {
            await PutTaskDelay();
            control = new CanvasVirtualControl();
            control.Width = 100;
            control.Height = 100;
            MyCanvas.Children.Add(control);
            Canvas.SetLeft(control, 0);
            Canvas.SetTop(control, 0);
            control.RegionsInvalidated += OnRegionsInvalidated;
            control.SizeChanged += virtualControl_SizeChanged;
        }

        void OnRegionsInvalidated(CanvasVirtualControl sender, CanvasRegionsInvalidatedEventArgs args)
        {
            var rectangle = CanvasGeometry.CreateRectangle(sender, 10, 10, 100, 100);
            using (var ds = sender.CreateDrawingSession(args.VisibleRegion))
            {
                ds.FillGeometry(rectangle, Color.FromArgb(128, 0, 0, 255));
            }
        }
    }
}
