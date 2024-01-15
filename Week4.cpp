// Week4.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );

	/**** Set up your scene here ****/
	IMesh* starsMesh = myEngine->LoadMesh("Stars.x");
	IModel* stars = starsMesh->CreateModel();

	//for the particles
	IMesh* particleMesh = myEngine->LoadMesh("particle.x");
	IModel* particle = particleMesh->CreateModel(0, 20, 0);

	//camera settings
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, 0, 0, -50);

	int particleState = 0;

	//vectors
	float VectorX = -0.005f, VectorY = -0.005f, VectorZ = 0;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//for moving the model
		if (myEngine->KeyHeld(Key_I)) {
			particle->MoveY(VectorY);
		}
		else if (myEngine->KeyHeld(Key_K))
		{
			particle->MoveX(VectorX);
		}
		else if (myEngine->KeyHeld(Key_L))
		{
			particle->MoveZ(VectorZ);
		}

		//for removig the model
		if (myEngine->KeyHit(Key_Return)) {
			if (particleState == 0)
			{
				particleMesh->RemoveModel(particle);
				particleState = 1;
			}
			else
			{
				particle = particleMesh->CreateModel();
				particleState = 0;
			}
		}

		//for changing the texture/colors
		if (myEngine->KeyHit(Key_Y)) {
			if (particleState == 0) {
				particleState = 1;
				particle->SetSkin("006_tlxadd.tga");
			}
			else if (particleState == 1)
			{
				particleState = 2;
				particle->SetSkin("007_tlxadd.tga");
			}
			else if (particleState == 2)
			{
				particleState = 3;
				particle->SetSkin("008_tlxadd.tga");
			}
			else if (particleState == 3)
			{
				particleState = 4;
				particle->SetSkin("009_tlxadd.tga");
			}
			else if (particleState == 4)
			{
				particleState = 5;
				particle->SetSkin("010_tlxadd.tga");
			}
			else if (particleState == 5)
			{
				particleState = 6;
				particle->SetSkin("011_tlxadd.tga");
			}
			else if (particleState == 6)
			{
				particleState = 7;
				particle->SetSkin("012_tlxadd.tga");
			}
		}

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
