// week5.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( TL11 );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );

	/**** Set up your scene here ****/
	float sphereSpeed;
	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.fbx");
	IModel* sphere = sphereMesh->CreateModel();
	sphere->SetSkin("Jupiter.jpg");

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, 0, 0, -70);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/


		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
