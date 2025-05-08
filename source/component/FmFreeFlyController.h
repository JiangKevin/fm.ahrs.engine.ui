//
#pragma once

#include "Urho3D/Input/AxisAdapter.h"
#include "Urho3D/Input/Input.h"
#include "Urho3D/Input/MultitouchAdapter.h"
#include "Urho3D/Scene/Component.h"

namespace Urho3D
{

    class URHO3D_API FmFreeFlyController : public Component
    {
        URHO3D_OBJECT( FmFreeFlyController, Component )
    private:
        struct Movement
        {
            Vector3   rotation_{ Vector3::ZERO };
            Vector3   translation_{ Vector3::ZERO };
            Movement& operator+=( const Movement& rhs )
            {
                rotation_ += rhs.rotation_;
                translation_ += rhs.translation_;
                return *this;
            }
        };
    public:
        /// Construct.
        explicit FmFreeFlyController( Context* context );
        /// Destruct.
        ~FmFreeFlyController() override;

        /// Register object factory and attributes.
        static void RegisterObject( Context* context );

        /// Handle enabled/disabled state change. Changes update event subscription.
        void OnSetEnabled() override;

        /// Attributes
        /// @{
        void SetSpeed( float value )
        {
            speed_ = value;
        }
        float GetSpeed() const
        {
            return speed_;
        }
        void SetMouseEnable( bool value )
        {
            mouse_active = value;
        }
        bool GetMouseEnable() const
        {
            return mouse_active;
        }
        void SetAcceleratedSpeed( float value )
        {
            acceleratedSpeed_ = value;
        }
        float GetAcceleratedSpeed() const
        {
            return acceleratedSpeed_;
        }
        float GetMinPitch() const
        {
            return minPitch_;
        }
        void SetMinPitch( float value )
        {
            minPitch_ = value;
        }
        float GetMaxPitch() const
        {
            return maxPitch_;
        }
        void SetMaxPitch( float value )
        {
            maxPitch_ = value;
        }
        void SetSecondMouseEnable( bool value )
        {
            isActive_ = value;
        }
        bool GetSecondMouseEnable() const
        {
            return isActive_;
        }
        /// @}
    private:
        /// Handle scene node being assigned at creation.
        void OnNodeSet( Node* previousNode, Node* currentNode ) override;

        /// Subscribe/unsubscribe to update events based on current enabled state and update event mask.
        void UpdateEventSubscription();

        /// Handle scene update event.
        void HandleUpdate( StringHash eventType, VariantMap& eventData );
        /// Handle scene update. Called by LogicComponent base class.
        void Update( float timeStep );
        /// Handle mouse input.
        Movement HandleMouse() const;
        /// Handle keyboard input.
        Movement HandleKeyboard( float timeStep ) const;
        /// Handle controller input.
        Movement HandleController( const JoystickState* state, float timeStep );
        /// Handle controller input.
        Movement HandleGenericJoystick( const JoystickState* state, float timeStep );
        /// Handle wheel input.
        Movement HandleWheel( const JoystickState* state, float timeStep );
        /// Handle flight stick input.
        Movement HandleFlightStick( const JoystickState* state, float timeStep );
        /// Handle keyboard and mouse input.
        void HandleKeyboardMouseAndJoysticks( float timeStep );
        /// Handle multitouch input event.
        void HandleMultitouch( StringHash eventType, VariantMap& eventData );
        /// Detect camera angles if camera has changed.
        void UpdateCameraAngles();
        /// Set camera rotation.
        void SetCameraRotation( Quaternion quaternion );
        /// Update camera rotation.
        void SetCameraAngles( Vector3 eulerAngles );
    private:
        /// Camera speed.
        float speed_{ 20.0f };
        /// Camera accelerated speed.
        float acceleratedSpeed_{ 100.0f };
        /// Mouse sensitivity
        float mouseSensitivity_{ 0.1f };
        /// Touch sensitivity
        float touchSensitivity_{ 1.0f };
        /// Axis sensitivity
        float axisSensitivity_{ 100.0f };
        /// Pitch range
        float minPitch_{ -90.0f };
        float maxPitch_{ 90.0f };
        ///
        bool mouse_active{ false };
        /// Gamepad default axis adapter
        AxisAdapter axisAdapter_{};
        /// Is subscribed to update
        bool subscribed_{ false };
        /// Multitouch input adapter
        MultitouchAdapter multitouchAdapter_;
        /// Last known camera rotation to keep track of yaw and pitch.
        ea::optional< Quaternion > lastKnownCameraRotation_;
        /// Last known yaw, pitch and roll to prevent gimbal lock.
        Vector3 lastKnownEulerAngles_;
        /// Joystick to ignore (SDL gyroscope virtual joystick)
        int ignoreJoystickId_{ -1 };

        /// Whether the rotation is performing now.
        bool      isActive_{ false };
        bool      oldMouseVisible_{};
        MouseMode oldMouseMode_{};
    };

}  // namespace Urho3D
