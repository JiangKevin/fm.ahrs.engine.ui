//
// Copyright (c) 2008-2022 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/// \file

#pragma once

#include "../Core/ThreadSafeCache.h"
#include "../Graphics/GraphicsDefs.h"
#include "../Math/Frustum.h"
#include "../Math/Ray.h"
#include "../Math/Rect.h"
#include "../Scene/Component.h"

namespace Urho3D
{

    class Zone;

    static const float DEFAULT_NEARCLIP   = 0.1f;
    static const float DEFAULT_FARCLIP    = 1000.0f;
    static const float DEFAULT_CAMERA_FOV = 45.0f;
    static const float DEFAULT_ORTHOSIZE  = 20.0f;

    enum ViewOverride : unsigned
    {
        VO_NONE                 = 0x0,
        VO_LOW_MATERIAL_QUALITY = 0x1,
        VO_DISABLE_SHADOWS      = 0x2,
        VO_DISABLE_OCCLUSION    = 0x4,
    };
    URHO3D_FLAGSET( ViewOverride, ViewOverrideFlags );

    /// %Camera component.
    class URHO3D_API Camera : public Component
    {
        URHO3D_OBJECT( Camera, Component );
    public:
        /// Construct.
        explicit Camera( Context* context );
        /// Destruct.
        ~Camera() override;
        /// Register object factory.
        /// @nobind
        static void RegisterObject( Context* context );

        /// Visualize the component as debug geometry.
        void DrawDebugGeometry( DebugRenderer* debug, bool depthTest ) override;

        /// Set current mouse position in normalized coordinates.
        void SetMousePosition( const Vector2& pos )
        {
            mousePosition_ = pos;
        }
        const Vector2& GetMousePosition() const
        {
            return mousePosition_;
        }
        bool HasMousePosition() const
        {
            return Rect::POSITIVE.IsInside( mousePosition_ ) != OUTSIDE;
        }

        /// Set near clip distance.
        /// @property
        void SetNearClip( float nearClip );
        /// Set far clip distance.
        /// @property
        void SetFarClip( float farClip );
        /// Set vertical field of view in degrees.
        /// @property
        void SetFov( float fov );
        /// Set orthographic mode view uniform size.
        /// @property
        void SetOrthoSize( float orthoSize );
        /// Set orthographic mode view non-uniform size. Disables the auto aspect ratio -mode.
        void SetOrthoSize( const Vector2& orthoSize );
        /// Set aspect ratio manually. Disables the auto aspect ratio -mode.
        /// @property
        void SetAspectRatio( float aspectRatio );
        /// Set polygon fill mode to use when rendering a scene.
        /// @property
        void SetFillMode( FillMode mode );
        /// Set zoom.
        /// @property
        void SetZoom( float zoom );
        /// Set LOD bias.
        /// @property
        void SetLodBias( float bias );
        /// Set view mask for primary rendering.
        void SetPrimaryViewMask( unsigned mask )
        {
            primaryViewMask_ = mask;
        }
        /// Set view mask for shadow casters.
        void SetShadowViewMask( unsigned mask )
        {
            shadowViewMask_ = mask;
        }
        /// Set view mask for everything.
        void SetViewMask( unsigned mask );
        /// Set zone mask.
        void SetZoneMask( unsigned mask );
        /// Set view override flags.
        /// @property
        void SetViewOverrideFlags( ViewOverrideFlags flags );
        /// Set orthographic mode enabled/disabled.
        /// @property
        void SetOrthographic( bool enable );
        /// Set automatic aspect ratio based on viewport dimensions. Enabled by default.
        /// @property
        void SetAutoAspectRatio( bool enable );
        /// Set projection offset. It needs to be calculated as (offset in pixels) / (viewport dimensions).
        /// @property
        void SetProjectionOffset( const Vector2& offset );
        /// Set reflection mode.
        /// @property
        void SetUseReflection( bool enable );
        /// Set reflection plane in world space for reflection mode.
        /// @property
        void SetReflectionPlane( const Plane& plane );
        /// Set whether to use a custom clip plane.
        /// @property
        void SetUseClipping( bool enable );
        /// Set custom clipping plane in world space.
        /// @property
        void SetClipPlane( const Plane& plane );
        /// Set vertical flipping mode. Called internally by View to resolve OpenGL rendertarget sampling differences.
        void SetFlipVertical( bool enable );
        /// Set custom projection matrix, which should be specified in D3D convention with depth range 0 - 1. Disables auto aspect ratio.
        /// @property
        /** Change any of the standard view parameters (FOV, far clip, zoom, etc.) to revert to the standard projection.
            Note that the custom projection is not serialized or replicated through the network.
         */
        void SetProjection( const Matrix4& projection );
        /// Set whether to draw the debug geometry when rendering the scene from this camera.
        void SetDrawDebugGeometry( bool enable )
        {
            drawDebugGeometry_ = enable;
        }

        /// Return far clip distance. If a custom projection matrix is in use, is calculated from it instead of the value assigned with SetFarClip().
        /// @property
        float GetFarClip() const;

        /// Return near clip distance. If a custom projection matrix is in use, is calculated from it instead of the value assigned with SetNearClip().
        /// @property
        float GetNearClip() const;

        /// Return vertical field of view in degrees.
        /// @property
        float GetFov() const
        {
            return fov_;
        }

        /// Return orthographic mode size.
        /// @property
        float GetOrthoSize() const
        {
            return orthoSize_;
        }

        /// Return aspect ratio.
        /// @property
        float GetAspectRatio() const
        {
            return aspectRatio_;
        }

        /// Return zoom.
        /// @property
        float GetZoom() const
        {
            return zoom_;
        }

        /// Return LOD bias.
        /// @property
        float GetLodBias() const
        {
            return lodBias_;
        }

        /// Return view mask.
        /// @property
        unsigned GetViewMask() const
        {
            return primaryViewMask_ | shadowViewMask_;
        }

        /// Return view mask for primary rendering.
        unsigned GetPrimaryViewMask() const
        {
            return primaryViewMask_;
        }

        /// Return view mask for shadow casters.
        unsigned GetShadowViewMask() const
        {
            return shadowViewMask_;
        }

        /// Return zone mask.
        unsigned GetZoneMask() const
        {
            return zoneMask_;
        }

        /// Return view override flags.
        /// @property
        ViewOverrideFlags GetViewOverrideFlags() const
        {
            return viewOverrideFlags_;
        }

        /// Return fill mode.
        /// @property
        FillMode GetFillMode() const
        {
            return fillMode_;
        }

        /// Return orthographic flag.
        /// @property
        bool IsOrthographic() const
        {
            return orthographic_;
        }

        /// Return auto aspect ratio flag.
        /// @property
        bool GetAutoAspectRatio() const
        {
            return autoAspectRatio_;
        }

        /// Return whether to draw the debug geometry when rendering the scene from this camera.
        bool GetDrawDebugGeometry() const
        {
            return drawDebugGeometry_;
        }

        /// Return frustum in world space.
        /// @property
        const Frustum& GetFrustum() const;
        /// Return projection matrix. It's in D3D convention with depth range 0 - 1.
        /// @property
        Matrix4 GetProjection( bool ignoreFlip = false ) const;
        /// Return projection matrix converted to API-specific format for use as a shader parameter.
        /// @property
        Matrix4 GetGPUProjection( bool ignoreFlip = false ) const;
        /// Return effective view-projection matrix with optionally applied depth bias.
        Matrix4 GetEffectiveGPUViewProjection( float constantDepthBias ) const;
        /// Return view matrix.
        /// @property
        const Matrix3x4& GetView() const;
        /// Return view-projection matrix.
        const Matrix4& GetViewProj() const;
        /// Return inverted view-projection matrix.
        const Matrix4& GetInverseViewProj() const;
        /// Return frustum near and far sizes.
        void GetFrustumSize( Vector3& nearSize, Vector3& farSize ) const;
        /// Return half view size.
        /// @property
        float GetHalfViewSize() const;
        /// Return dimensions of camera frustum at given distance.
        Vector2 GetViewSizeAt( float z ) const;
        /// Return frustum split by custom near and far clip distances.
        Frustum GetSplitFrustum( float nearClip, float farClip ) const;
        /// Return frustum in view space.
        /// @property
        Frustum GetViewSpaceFrustum() const;
        /// Return split frustum in view space.
        Frustum GetViewSpaceSplitFrustum( float nearClip, float farClip ) const;
        /// Return ray corresponding to normalized screen coordinates (0 - 1), with origin on the near clip plane.
        Ray GetScreenRay( float x, float y ) const;
        /// Return ray corresponding to current mouse position, with origin on the near clip plane.
        Ray GetScreenRayFromMouse() const;
        /// Convert a world space point to normalized screen coordinates (0 - 1).
        Vector2 WorldToScreenPoint( const Vector3& worldPos ) const;
        /// Convert normalized screen coordinates (0 - 1) and distance along view Z axis (in Z coordinate) to a world space point. The distance can not be closer than the near clip plane.
        /** Note that a HitDistance() from the camera screen ray is not the same as distance along the view Z axis, as under a perspective projection the ray is likely to not be Z-aligned.
         */
        Vector3 ScreenToWorldPoint( const Vector3& screenPos ) const;

        /// Return projection offset.
        /// @property
        const Vector2& GetProjectionOffset() const
        {
            return projectionOffset_;
        }

        /// Return whether is using reflection.
        /// @property
        bool GetUseReflection() const
        {
            return useReflection_;
        }

        /// Return the reflection plane.
        /// @property
        const Plane& GetReflectionPlane() const
        {
            return reflectionPlane_;
        }

        /// Return whether is using a custom clipping plane.
        /// @property
        bool GetUseClipping() const
        {
            return useClipping_;
        }

        /// Return the custom clipping plane.
        /// @property
        const Plane& GetClipPlane() const
        {
            return clipPlane_;
        }

        /// Return vertical flipping mode.
        bool GetFlipVertical() const
        {
            return flipVertical_;
        }

        /// Return whether to reverse culling; affected by vertical flipping and reflection.
        bool GetReverseCulling() const
        {
            return flipVertical_ ^ useReflection_;
        }

        /// Return distance to position. In orthographic mode uses only Z coordinate.
        float GetDistance( const Vector3& worldPos ) const;
        /// Return squared distance to position. In orthographic mode uses only Z coordinate.
        float GetDistanceSquared( const Vector3& worldPos ) const;
        /// Return a scene node's LOD scaled distance.
        float GetLodDistance( float distance, float scale, float bias ) const;
        /// Return a world rotation for facing a camera on certain axes based on the existing world rotation.
        Quaternion GetFaceCameraRotation( const Vector3& position, const Quaternion& rotation, FaceCameraMode mode, float minAngle = 0.0f );
        /// Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
        /// @property
        Matrix3x4 GetEffectiveWorldTransform() const;
        /// Return if projection parameters are valid for rendering and raycasting.
        bool IsProjectionValid() const;

        /// Set aspect ratio without disabling the "auto aspect ratio" mode. Called internally by View.
        void SetAspectRatioInternal( float aspectRatio );
        /// Set orthographic size attribute without forcing the aspect ratio.
        void SetOrthoSizeAttr( float orthoSize );
        /// Set reflection plane attribute.
        void SetReflectionPlaneAttr( const Vector4& value );
        /// Return reflection plane attribute.
        Vector4 GetReflectionPlaneAttr() const;
        /// Set clipping plane attribute.
        void SetClipPlaneAttr( const Vector4& value );
        /// Return clipping plane attribute.
        Vector4 GetClipPlaneAttr() const;

        /// Set current zone.
        void SetZone( Zone* zone )
        {
            zone_ = zone;
        }
        /// Return current zone.
        Zone* GetZone() const
        {
            return zone_;
        }

        /// Return effective ambient light color.
        const Color& GetEffectiveAmbientColor() const;
        /// Return effective ambient light brightness.
        float GetEffectiveAmbientBrightness() const;
        /// Return effective fog color considering current zone.
        const Color& GetEffectiveFogColor() const;
        /// Return effective fog start distance considering current zone.
        float GetEffectiveFogStart() const;
        /// Return effective fog end distance considering current zone.
        float GetEffectiveFogEnd() const;
    protected:
        /// Handle node being assigned.
        void OnNodeSet( Node* previousNode, Node* currentNode ) override;
        /// Handle node transform being dirtied.
        void OnMarkedDirty( Node* node ) override;
    private:
        /// Recalculate projection matrix.
        void UpdateProjection() const;
        /// Recalculate view-projection matrices.
        void UpdateViewProjectionMatrices() const;

        /// Cached projection data.
        struct CachedProjection
        {
            /// Cached projection matrix.
            Matrix4 projection_;
            /// Cached actual near clip distance.
            float projNearClip_{};
            /// Cached actual far clip distance.
            float projFarClip_{};
            /// Use custom projection matrix flag. Used internally.
            bool customProjection_{};
        };

        /// Cached view-projection matrix.
        struct CachedViewProj
        {
            /// Cached view-projection matrix.
            Matrix4 viewProj_;
            /// Cached inverse view-projection matrix.
            Matrix4 inverseViewProj_;
        };

        /// Cached view matrix.
        mutable ThreadSafeCache< Matrix3x4 > cachedView_;
        /// Cached projection data.
        mutable ThreadSafeCache< CachedProjection > cachedProjection_;
        /// Cached view-projection matrices.
        mutable ThreadSafeCache< CachedViewProj > cachedViewProj_;
        /// Cached world space frustum.
        mutable ThreadSafeCache< Frustum > cachedFrustum_;
        /// Orthographic mode flag.
        bool orthographic_;
        /// Near clip distance.
        float nearClip_;
        /// Far clip distance.
        float farClip_;
        /// Field of view.
        float fov_;
        /// Orthographic view size.
        float orthoSize_;
        /// Aspect ratio.
        float aspectRatio_;
        /// Zoom.
        float zoom_;
        /// LOD bias.
        float lodBias_;
        /// View mask for primary rendering.
        unsigned primaryViewMask_{};
        /// View mask for shadow casters.
        unsigned shadowViewMask_{};
        /// Zone mask.
        unsigned zoneMask_{};
        /// Current zone containing camera.
        Zone* zone_{};
        /// View override flags.
        ViewOverrideFlags viewOverrideFlags_;
        /// Fill mode.
        FillMode fillMode_;
        /// Projection offset.
        Vector2 projectionOffset_;
        /// Reflection plane.
        Plane reflectionPlane_;
        /// Clipping plane.
        Plane clipPlane_;
        /// Reflection matrix calculated from the plane.
        Matrix3x4 reflectionMatrix_;
        /// Auto aspect ratio flag.
        bool autoAspectRatio_;
        /// Flip vertical flag.
        bool flipVertical_;
        /// Reflection mode enabled flag.
        bool useReflection_;
        /// Use custom clip plane flag.
        bool useClipping_;
        /// Whether to draw debug geometry when rendering from this camera.
        bool drawDebugGeometry_{ true };

        /// Current normalized mouse position supplied externally.
        Vector2 mousePosition_;

        WeakPtr< Graphics > graphics_;
    };

}
