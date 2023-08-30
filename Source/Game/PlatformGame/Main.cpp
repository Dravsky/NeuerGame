#include "PlatformGame.h"

#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"

#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <array>
#include <map>
#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
	lola::MemoryTracker::Initialize();
	lola::seedRandom((unsigned int)time(nullptr));
	lola::setFilePath("Assets");

	lola::g_renderer.Initialize();
	lola::g_renderer.CreateWindow("CSC196", 1215, 600);

	lola::g_inputSystem.Initialize();
	lola::g_audioSystem.Initialize();
	lola::PhysicsSystem::Instance().Initialize();

	lola::g_audioSystem.AddAudio("music", "PlatformGame/Audio/music.mp3");
	lola::g_audioSystem.PlayOneShot("music");

	unique_ptr<PlatformGame> game = make_unique<PlatformGame>();
	game->Initialize();

	// Main Game Loop
	bool quit = false;
	while (!quit)
	{
		// Update Engine
		lola::g_time.Tick();
		lola::g_inputSystem.Update();
		if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		// Update Game
		lola::g_audioSystem.Update();
		lola::g_particleSystem.Update(lola::g_time.GetDeltaTime());
		lola::PhysicsSystem::Instance().Update(lola::g_time.GetDeltaTime());

		game->Update(lola::g_time.GetDeltaTime());

		// Draw Game
		lola::g_renderer.SetColor(0, 0, 0, 0);
		lola::g_renderer.BeginFrame();

		game->Draw(lola::g_renderer);

		lola::g_particleSystem.Draw(lola::g_renderer);
		lola::g_renderer.EndFrame();
	}

	return 0;
}