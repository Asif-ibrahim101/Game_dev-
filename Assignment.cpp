// Assignment.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");

	//States
	bool Ready = false;
	bool Firing = false;
	bool Collision_state = false;
	bool over = false;

    /**** Set up your scene here ****/


	//speed of the objects
	const float KGameSpeed = 1.0f;

	//floor
	IMesh* floorMesh = myEngine->LoadMesh("Floor.X");
	IModel* floor = floorMesh->CreateModel();

	//sky
	const float sky_X = 0.0f;
	const float sky_Y = -1000.0f;
	const float sky_Z = 0.0f;

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox_Hell.x");
	IModel* skyBox = skyBoxMesh->CreateModel(sky_X, sky_Y, sky_Z);

	//Dummy
	const float dummy_x = 50.0f;
	const float dummy_y = 1.0f;
	const float dummy_z = 0.0f;

	//IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	//IModel* dummy = dummyMesh->CreateModel(dummy_x, dummy_y, dummy_z);

	//marbels
	const float marble_x = 50.0f;
	const float marble_y = 2.0f;
	const float marble_z = 0.0f;

	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IModel* marble = marbleMesh->CreateModel(marble_x, marble_y, marble_z);
	//marble->AttachToParent(dummy);

	//Arrow
	const float Arrow_x = 0.0f;
	const float Arrow_y = 1.0f;
	const float Arrow_z = 0.0f;

	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* Arrow = ArrowMesh->CreateModel(Arrow_x, Arrow_y, Arrow_z);
	Arrow->AttachToParent(marble);
	Arrow->MoveZ(-10.0f);

	//Blocks
	float xPos = 0.0f;
	float YPos = 4.0f;
	float ZPos = 120.0f;
	const int BlockArray_length = 10;
	const int Colllision_lenght = 10;

	IMesh* blockMesh = myEngine->LoadMesh("Block.x");
	IModel* BlockModels[BlockArray_length];
	//IModel* BlockCol[Colllision_lenght];

	//positioning all the models
	for (int i = 0; i < BlockArray_length; i++) {
		BlockModels[i] = blockMesh->CreateModel(xPos, YPos, ZPos);
		xPos += 12;
	};

	//Camera setup
	const float Camera_x = 50.0f;
	const float Camera_y = 30.0f;
	const float Camera_z = -60.0f;
	const float Camera_Rotation = 10.0f;

	ICamera* mycamera;
	mycamera = myEngine->CreateCamera(ManualCamera, Camera_x, Camera_y, Camera_z);
	mycamera->RotateX(Camera_Rotation);

	//rotation of the marble
	float rotation = 0.0f;
	const float Rotation_limmit = 50.0f;
	const float Rotation_speed = KGameSpeed / 4;


	//radius of the spheres
	float radius_marble = 2.00;
	float radius_block = 4.00;

	//maximum distance of the marble
	const float Max  = 200.0f;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//getting location of the marble
		float OldX = marble->GetX();
		float OldZ = marble->GetZ();

		 
		//moving the marble and starting Ready State
		if (myEngine->KeyHeld(Key_Z) && rotation < Rotation_limmit)
		{
			if (Ready == false) {
				marble->RotateY(Rotation_speed);
				rotation += Rotation_speed;
			};
		};

		if (myEngine->KeyHeld(Key_X) && rotation > -Rotation_limmit ) {

			if (Ready == false) {
				marble->RotateY(-Rotation_speed);
				rotation -= Rotation_speed;
			};
		}; 

		//Hiting State
		if (myEngine->KeyHeld(Key_Space) && marble->GetLocalZ() <= Max) {
			if (Firing == false && over == false) {
				marble->MoveLocalZ(KGameSpeed);
				Ready = true;
			};
		};

		//looking for collision
		if (!Firing) {

			for (int i = 0; i < BlockArray_length; i++) {
				float distx = BlockModels[i]->GetX() - marble->GetX();
				float distz = BlockModels[i]->GetZ() - marble->GetZ();

				float distance = sqrt((distx * distx) + (distz * distz));
				bool collision = distance < (radius_marble + radius_block);

				//resolving the collision
					if (collision) {
						marble->SetX(OldX);
						marble->SetZ(OldZ);

						//changing Blocks colour after collision
						BlockModels[i]->SetSkin("tiles_red.jpg");
						Ready = true;
						Firing = true;
						Collision_state = true;
					};
			};

		};

		//Returing the marble
		if (Collision_state == true) {
			//Returing the marble
			if (myEngine->KeyHit(Key_R)) {
				marble->SetX(marble_x);
				marble->SetZ(marble_z);
				Ready = false;
				Firing = false;
				Collision_state = false;
			};
		};
		
		// Stop if the Escape key is pressed
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		};
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
