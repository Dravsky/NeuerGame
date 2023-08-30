#include "PlatformGame/Player.h"
#include "Audio/AudioSystem.h"

#include "Input/InputSystem.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

namespace lola
{
    CLASS_DEFINITION(Player);

    bool Player::Initialize()
    {
        Actor::Initialize();

        m_physicsComponent = GetComponent<lola::PhysicsComponent>();
        m_spriteAnimComponent = GetComponent<lola::SpriteAnimComponent>();

        lola::g_audioSystem.AddAudio("win", "PlatformGame/Audio/win.mp3");
        lola::g_audioSystem.AddAudio("slime", "PlatformGame/Audio/slime.mp3");

        return true;
    }

    void Player::Update(float dt)
    {
        Actor::Update(dt);

        bool onGround = (groundCount > 0);
        
        // Movement
        float dir = 0;
        if (g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
        if (g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;

        vec2 forward = lola::vec2{ 1, 0 };
        m_physicsComponent->ApplyForce(forward * speed * dir * ((onGround) ? 1 : 0.5f));

        // Jump

        if (onGround && g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
        {
            int mouseY = (int)(g_inputSystem.GetMousePosition().y);

            mouseY = (mouseY / 75);

            (mouseY < 2) ? (mouseY = 2) : mouseY;
            (mouseY > 5) ? (mouseY = 5) : mouseY;

            vec2 up = lola::vec2{ 0, -1 };
            m_physicsComponent->SetVelocity(up * (float)mouseY);

            //std::cout << "Jump: " << mouseY << '\n';

            lola::g_audioSystem.PlayOneShot("slime");

        }

        // Animation

        if (g_inputSystem.GetKeyDown(SDL_SCANCODE_1)) {
            m_spriteAnimComponent->SetSequence("blueIdle");
        }

        if (g_inputSystem.GetKeyDown(SDL_SCANCODE_2)) {
            m_spriteAnimComponent->SetSequence("greenIdle");
        }

        if (g_inputSystem.GetKeyDown(SDL_SCANCODE_3)) {
            m_spriteAnimComponent->SetSequence("redIdle");
        }

        //vec2 velocity = m_physicsComponent->velocity;

        // Check if moving
        /* if (std::fabs(velocity.x) > 0.2f)
        {
           //if (dir != 0) m_spriteAnimComponent->flipH = (dir < 0);
            m_spriteAnimComponent->SetSequence("idle");
        }
        else
        {
            m_spriteAnimComponent->SetSequence("idle");
        } */

    }

    void Player::OnCollisionEnter(Actor* actor)
    {
        if (actor->tag == "Ground") groundCount++;

        if (actor->tag == "Orb")
        {
            lola::g_audioSystem.PlayOneShot("win");
            EventManager::Instance().DispatchEvent("OnPlayerWin", 0);
        }

    }

    void Player::OnCollisionExit(Actor* actor)
    {
        if (actor->tag == "Ground") groundCount--;
    }

    void Player::Read(const json_t& value)
    {
        Actor::Read(value);

        READ_NAME_DATA(value, "speed", speed);
        READ_NAME_DATA(value, "jump", jump);
    }
}