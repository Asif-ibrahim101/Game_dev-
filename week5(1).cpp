// week5(1).cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );

	/**** Set up your scene here ****/
	IMesh * cubeMesh = myEngine->LoadMesh("cube.x");
	IModel* cube = cubeMesh->CreateModel();

	// Load "Comic Sans MS" font at 36 points
	int FontX = 0, FontY = 0;
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, 0, 0, -70);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		myFont->Draw("Hello World", FontX, FontY);

		if (myEngine->KeyHeld(Key_Up)) {
			FontY -= 1;
		}
		else if (myEngine->KeyHeld(Key_Down)) {
			FontY += 1;
		}
		else if (myEngine->KeyHeld(Key_Left)) {
			FontX -= 1;
		}
		else if (myEngine->KeyHeld(Key_Right)) {
			FontX += 1;
		};

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
