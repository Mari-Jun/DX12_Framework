#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
    class Pawn;

    class Controller : public Actor
    {
    protected:
        Controller(const std::string& name = "controller");
        virtual ~Controller() = default;

        virtual void Update(float delta_time) override;
        
    private:
        void UpdateYaw(float delta_time);
        void UpdatePitch(float delta_time);
        void UpdateRoll(float delta_time);

    protected:
        SPtr<Pawn> m_controlled_pawn;
        float m_pitch_speed = 0.0f;
        float m_yaw_speed = 0.0f;
        float m_roll_speed = 0.0f;
        float m_max_pitch = math::ToRadian(89.f);
        float m_max_roll = math::ToRadian(89.f);

    private:
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;
        float m_roll = 0.0f;

    public:
        void SetMaxPitch(float pitch) { m_max_pitch = pitch; }
        void SetMaxRoll(float roll) { m_max_roll = roll; }

    public:
        const SPtr<Pawn>& GetControlledPawn() const { return m_controlled_pawn; }
        virtual void Possess(const SPtr<Pawn>& pawn);
        virtual void UnPossess();
    };
}

