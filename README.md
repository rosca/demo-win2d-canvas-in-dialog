# demo-win2d-canvas-in-dialog

Win2D Canvas Virtual Control does not work properly inside a ContentDialog.

To reproduce:
1) Open sln, build the project and run the application.
2) Click on "Show Dialog" to open the ContentDialog instance. You should see a green circle inside the dialog.
3) Alt Tab a few times to move to other apps, then move back to the demo app. Press Invalidate button a few times. The green circle disappears. It doesn't come back anymore even if you press Invalidate button.
