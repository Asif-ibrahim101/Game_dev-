// Assignment.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

bool CheckAndHandleCollision(Model* model1, Model* model2, float width, float depth, float radius, float marble_speed);

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");

	//States
	bool Ready = true;
	bool Firing = false;
	bool over = false;

	/**** Set up your scene here ****/

	//speed of the objects
	float KGameSpeed = 1.0f;
	float marbleSpeed = 1.0f;

	//Barriars1
	float barrier_xPos = 10.0f;
	float barrier_YPos = 4.0f;
	float barrier_ZPos = 0.0f;
	const int BarrierArray_length = 10;

	//Barrier2
	float barrier_xPos1 = 118.0f;
	float barrier_YPos1 = 4.0f;
	float barrier_ZPos1 = 0.0f;
	IMesh* BarrierMesh = myEngine->LoadMesh("Barrier.x");
	IModel* BarrierModels[BarrierArray_length];
	IModel* BarrierModels1[BarrierArray_length];

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
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	IModel* dummy = dummyMesh->CreateModel(dummy_x, dummy_y, dummy_z);

	//marbels
	const float marble_x = 0.0f;
	const float marble_y = 2.0f;
	const float marble_z = 0.0f;
	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IModel* marble = marbleMesh->CreateModel(marble_x, marble_y, marble_z);
	marble->AttachToParent(dummy);

	//Arrow
	const float Arrow_x = 0.0f;
	const float Arrow_y = 1.0f;
	const float Arrow_z = 0.0f;
	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* Arrow = ArrowMesh->CreateModel(Arrow_x, Arrow_y, Arrow_z);
	Arrow->AttachToParent(dummy);
	Arrow->MoveZ(-10.0f);

	//Blocks
	float xPos = 0.0f;
	float YPos = 4.0f;
	float ZPos = 120.0f;
	const int BlockArray_length = 10;
	IMesh* blockMesh = myEngine->LoadMesh("Block.x");
	IModel* BlockModels[BlockArray_length];

	//Camera setup
	ICamera* myCamera;
	const float Camera_x = 50.0f;
	const float Camera_y = 30.0f;
	const float Camera_z = -60.0f;
	const float Camera_Rotation = 10.0f;
	myCamera = myEngine->CreateCamera(ManualCamera, Camera_x, Camera_y, Camera_z);
	myCamera->RotateX(Camera_Rotation);

	//rotation of the marble
	float rotation = 0.0f;
	const float Rotation_limmit = 50.0f;
	const float Rotation_speed = KGameSpeed / 8;

	//radius of the spheres
	float radius_marble = 2.0f;
	float radius_block = 12.0f;

	//for the blocks
	float bWidth = 12.0f;
	float bDepth = 12.0f;

	//for the barriers
	float BeWidth = 15.0f;
	float BeDepth = 15.0f;

	//for counting amount of hit 
	int Marble_Hit = 0;

	//positioning all the barriers
	for (int i = 0; i < BarrierArray_length; i++)
	{
		BarrierModels[i] = BarrierMesh->CreateModel(-barrier_xPos, barrier_YPos, barrier_ZPos);
		barrier_ZPos += 14;

		BarrierModels[i]->SetSkin(i < 3 ? "barrier1a.BMP" : "barrier1.BMP");
	};

	//positioning all the barriers
	for (int i = 0; i < BarrierArray_length; i++) {
		BarrierModels1[i] = BarrierMesh->CreateModel(barrier_xPos1, barrier_YPos1, barrier_ZPos1);
		barrier_ZPos1 += 14;

		BarrierModels1[i]->SetSkin(i < 3 ? "barrier1a.BMP" : "barrier1.BMP");

	};

	//positioning all the blocks
	for (int i = 0; i < BlockArray_length; i++) {
		BlockModels[i] = blockMesh->CreateModel(xPos, YPos, ZPos);
		xPos += 12;
	};

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//getting location of the marble
		float OldX = marble->GetX();
		float OldZ = marble->GetZ();

		if (Ready) {
			// Rotate based on key presses and limits
			if (myEngine->KeyHeld(Key_Z) && rotation < Rotation_limmit) {
				dummy->RotateY(Rotation_speed);
				rotation += Rotation_speed;
			}
			else if (myEngine->KeyHeld(Key_X) && rotation > -Rotation_limmit) {
				dummy->RotateY(-Rotation_speed);
				rotation -= Rotation_speed;
			}

			// Fire on spacebar press
			if (myEngine->KeyHit(Key_Space)) {
				Firing = true;
			}
		}

		if (Firing == true)
		{
			Ready = false;
			marble->MoveLocalZ(marbleSpeed * KGameSpeed / 3);

			//collision for the blocks
			for (int i = 0; i < BlockArray_length; i++)
			{
				bool collision = false;
				(CheckAndHandleCollision(BlockModels[i], marble, bWidth, bDepth, radius_marble, marbleSpeed) ? collision = true : false);
				//resolving the collision
				if (collision)
				{
					marble->SetX(OldX);
					marble->SetZ(OldZ);
					BlockModels[i]->SetSkin("tiles_red.jpg");
					marbleSpeed = -0.34;
					Marble_Hit++;

					//checking if the hit is more than 5
					if (Marble_Hit > 4)
					{
						Ready = false;
						Firing = false;
						over = true;

						//Changing the marbles colour
						marble->SetSkin("glass_blue.jpg");
					};
				};
			};

			//collision for the barriers
			bool collision = false;
			for (int i = 0; i < BarrierArray_length; i++)
			{
				(CheckAndHandleCollision(BarrierModels[i], marble, BeWidth, BeDepth, radius_marble, marbleSpeed) ? collision = true : false);
				if (collision) {
					marbleSpeed = -0.34;
				};
			}

			for (int i = 0; i < BarrierArray_length; i++)
			{
				(CheckAndHandleCollision(BarrierModels1[i], marble, BeWidth, BeDepth, radius_marble, marbleSpeed) ? collision = true : false);
				if (collision) {
					marbleSpeed = -0.34;
				};
			};

			//Returing the marble
			if (marble->GetZ() <= dummy_z)
			{
				marbleSpeed = 0.33;
				Ready = true;
				Firing = false;
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
};

bool CheckAndHandleCollision(Model* model1, Model* model2, float width, float depth, float radius, float marble_speed) {
	// Calculate collision bounds
	//sphere to box collision
	float minX = model1->GetX() - (width / 2) - radius;
	float maxX = model1->GetX() + (width / 2) + radius;
	float minZ = model1->GetZ() - (depth / 2) - radius;
	float maxZ = model1->GetZ() - (depth / 2) + radius;

	return (
			model2->GetX() > minX && model2->GetX() < maxX
			&& model2->GetZ() > minZ && model2->GetZ() < maxZ
		);
};


