#include "PlatformGame.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"

#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Core/Core.h"

#include "Framework/Components/TextRenderComponent.h"


bool PlatformGame::Initialize()
{

    // Create scene
    m_scene = std::make_unique<lola::Scene>();
    m_scene->Load("PlatformGame/Scenes/PlatformScene.json");
    m_scene->Initialize();

    lola::g_audioSystem.AddAudio("gamestart", "PlatformGame/Audio/gamestart.mp3");

    EVENT_SUBSCRIBE("OnPlayerWin", PlatformGame::OnPlayerWin);

    return true;
}

void PlatformGame::Shutdown()
{

}

void PlatformGame::Update(float dt)
{
    switch (m_state)
    {
    case PlatformGame::eState::Title:
    {
        if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) || victoryLap)
        {

            if (!victoryLap) lola::g_audioSystem.PlayOneShot("gamestart");

            m_state = eState::StartLevel;
            m_scene->Load("PlatformGame/Scenes/tilemap.json");
            m_scene->Initialize();

            m_scene->GetActorByName("Title07")->active = false;

            auto player = INSTANTIATE(Actor, "Player");
            player->Initialize();
            m_scene->Add(std::move(player));

            auto orb = INSTANTIATE(Actor, "Orb");
            orb->Initialize();
            m_scene->Add(std::move(orb));
        }
    }
    break;
    case PlatformGame::eState::StartGame:

        break;

    case PlatformGame::eState::StartLevel:

        break;
    case PlatformGame::eState::Game:

        break;
    case PlatformGame::eState::GameOver:

        if (!m_scene->GetActorByName("Title07")->active) m_scene->GetActorByName("Title07")->active = true;

        if (lola::g_inputSystem.GetKeyDown(SDL_SCANCODE_4)) {

            m_scene->GetActorByName("Title07")->active = false;

            m_scene->RemoveAll();

            m_scene->GetActorByName("Title07")->destroyed = true;

            victoryLap = true;

            m_state = eState::Title;

        }

        break;
    default:

        break;
    }

    m_scene->Update(dt);
}

void PlatformGame::Draw(lola::Renderer& renderer)
{
    m_scene->Draw(renderer);
}

void PlatformGame::OnPlayerWin(const lola::Event& event)
{
    m_state = eState::GameOver;
}