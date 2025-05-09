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

#pragma once

#include "../Core/Attribute.h"
#include "../Core/Object.h"
#include <cstddef>

namespace Urho3D
{

    class Archive;
    class ArchiveBlock;
    class Deserializer;
    class Serializer;
    class XMLElement;
    class JSONValue;
    class ObjectReflection;

    /// Base class for objects with automatic serialization through attributes.
    class URHO3D_API Serializable : public Object
    {
        URHO3D_OBJECT( Serializable, Object );
    public:
        /// Construct.
        explicit Serializable( Context* context );
        /// Destruct.
        ~Serializable() override;

        /// Handle attribute write access. Default implementation writes to the variable at offset, or invokes the set accessor.
        virtual void OnSetAttribute( const AttributeInfo& attr, const Variant& src );
        /// Handle attribute read access. Default implementation reads the variable at offset, or invokes the get accessor.
        virtual void OnGetAttribute( const AttributeInfo& attr, Variant& dest ) const;
        /// Return reflection used for serialization.
        virtual ObjectReflection* GetReflection() const;
        /// Return attribute descriptions, or null if none defined.
        virtual const ea::vector< AttributeInfo >* GetAttributes() const;

        /// Serialize content from/to archive. May throw ArchiveException.
        void SerializeInBlock( Archive& archive ) override;
        void SerializeInBlock( Archive& archive, bool serializeTemporary );

        /// Load from binary data. Return true if successful.
        virtual bool Load( Deserializer& source );
        /// Save as binary data. Return true if successful.
        virtual bool Save( Serializer& dest ) const;
        /// Load from XML data. Return true if successful.
        virtual bool LoadXML( const XMLElement& source );
        /// Save as XML data. Return true if successful.
        virtual bool SaveXML( XMLElement& dest ) const;
        /// Load from JSON data. Return true if successful.
        virtual bool LoadJSON( const JSONValue& source );
        /// Save as JSON data. Return true if successful.
        virtual bool SaveJSON( JSONValue& dest ) const;
        /// Load from binary resource.
        virtual bool Load( const ea::string& resourceName );
        /// Load from XML resource.
        virtual bool LoadXML( const ea::string& resourceName );
        /// Load from JSON resource.
        virtual bool LoadJSON( const ea::string& resourceName );
        /// Load from resource of automatically detected type.
        virtual bool LoadFile( const ea::string& resourceName );

        /// Apply attribute changes that can not be applied immediately. Called after scene load or a network update.
        virtual void ApplyAttributes() {}

        /// Return whether should save default-valued attributes into XML. Default false.
        virtual bool SaveDefaultAttributes( const AttributeInfo& attr ) const
        {
            return false;
        }

        /// Set attribute by index. Return true if successfully set.
        /// @property{set_attributes}
        bool SetAttribute( unsigned index, const Variant& value );
        /// Set attribute by name. Return true if successfully set.
        bool SetAttribute( const ea::string& name, const Variant& value );
        /// (Internal use) Set instance-level default flag.
        void SetInstanceDefault( bool enable )
        {
            setInstanceDefault_ = enable;
        }
        /// (Internal use) Set instance-level default value. Allocate the internal data structure as necessary.
        void SetInstanceDefault( const ea::string& name, const Variant& defaultValue );
        /// (Internal use) Get instance-level default value.
        virtual Variant GetInstanceDefault( const ea::string& name ) const;
        /// Reset all editable attributes to their default values.
        void ResetToDefault();
        /// Remove instance's default values if they are set previously.
        void RemoveInstanceDefault();
        /// Set temporary flag. Temporary objects will not be saved.
        /// @property
        void SetTemporary( bool enable );

        /// Return attribute value by index. Return empty if illegal index.
        /// @property{get_attributes}
        Variant GetAttribute( unsigned index ) const;
        /// Return attribute value by name. Return empty if not found.
        Variant GetAttribute( const ea::string& name ) const;
        /// Return attribute default value by index. Return empty if illegal index.
        /// @property{get_attributeDefaults}
        Variant GetAttributeDefault( unsigned index ) const;
        /// Return attribute default value by name. Return empty if not found.
        Variant GetAttributeDefault( const ea::string& name ) const;
        /// Return number of attributes.
        /// @property
        unsigned GetNumAttributes() const;

        /// Copy all attributes from another serializable.
        void CopyAttributes( const Serializable* source, bool resetToDefault = true );

        /// Return whether is temporary.
        /// @property
        bool IsTemporary() const
        {
            return temporary_;
        }
    protected:
        /// Attribute default value at each instance level.
        ea::unique_ptr< VariantMap > instanceDefaultValues_;
        /// When true, store the attribute value as instance's default value (internal use only).
        bool setInstanceDefault_;
        /// Temporary flag.
        bool temporary_;
    };

    using WeakSerializableVector = ea::vector< WeakPtr< Serializable > >;

    /// Template implementation of the variant attribute accessor.
    template < class TClassType, class TGetFunction, class TSetFunction > class VariantAttributeAccessorImpl : public AttributeAccessor
    {
    public:
        /// Construct.
        VariantAttributeAccessorImpl( TGetFunction getFunction, TSetFunction setFunction ) : getFunction_( getFunction ), setFunction_( setFunction ) {}

        /// Invoke getter function.
        void Get( const Serializable* ptr, Variant& value ) const override
        {
            assert( ptr );
            const auto classPtr = static_cast< const TClassType* >( ptr );
            getFunction_( *classPtr, value );
        }

        /// Invoke setter function.
        void Set( Serializable* ptr, const Variant& value ) override
        {
            assert( ptr );
            auto classPtr = static_cast< TClassType* >( ptr );
            setFunction_( *classPtr, value );
        }
    private:
        /// Get functor.
        TGetFunction getFunction_;
        /// Set functor.
        TSetFunction setFunction_;
    };

    /// Make variant attribute accessor implementation.
    /// \tparam TClassType Serializable class type.
    /// \tparam TGetFunction Functional object with call signature `void getFunction(const TClassType& self, Variant& value)`
    /// \tparam TSetFunction Functional object with call signature `void setFunction(TClassType& self, const Variant& value)`
    template < class TClassType, class TGetFunction, class TSetFunction > SharedPtr< AttributeAccessor > MakeVariantAttributeAccessor( TGetFunction getFunction, TSetFunction setFunction )
    {
        return SharedPtr< AttributeAccessor >( new VariantAttributeAccessorImpl< TClassType, TGetFunction, TSetFunction >( getFunction, setFunction ) );
    }

/// Make member attribute accessor.
#define URHO3D_MAKE_MEMBER_ATTRIBUTE_ACCESSOR( typeName, variable ) \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(              \
        []( const ClassName& self, Urho3D::Variant& value )         \
        {                                                           \
            value = self.variable;                                  \
        },                                                          \
        []( ClassName& self, const Urho3D::Variant& value )         \
        {                                                           \
            self.variable = value.Get< typeName >();                \
        } )

/// Make member attribute accessor with custom post-set callback.
#define URHO3D_MAKE_MEMBER_ATTRIBUTE_ACCESSOR_EX( typeName, variable, postSetCallback ) \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                                  \
        []( const ClassName& self, Urho3D::Variant& value )                             \
        {                                                                               \
            value = self.variable;                                                      \
        },                                                                              \
        []( ClassName& self, const Urho3D::Variant& value )                             \
        {                                                                               \
            self.variable = value.Get< typeName >();                                    \
            self.postSetCallback();                                                     \
        } )

/// Make custom member attribute accessor.
#define URHO3D_MAKE_CUSTOM_MEMBER_ATTRIBUTE_ACCESSOR( typeName, variable ) \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                     \
        []( const ClassName& self, Urho3D::Variant& value )                \
        {                                                                  \
            value.SetCustom( static_cast< typeName >( self.variable ) );   \
        },                                                                 \
        []( ClassName& self, const Urho3D::Variant& value )                \
        {                                                                  \
            self.variable = value.GetCustom< typeName >();                 \
        } )

/// Make get/set attribute accessor.
#define URHO3D_MAKE_GET_SET_ATTRIBUTE_ACCESSOR( getFunction, setFunction, typeName ) \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                               \
        []( const ClassName& self, Urho3D::Variant& value )                          \
        {                                                                            \
            value = self.getFunction();                                              \
        },                                                                           \
        []( ClassName& self, const Urho3D::Variant& value )                          \
        {                                                                            \
            self.setFunction( value.Get< typeName >() );                             \
        } )

/// Make member enum attribute accessor.
#define URHO3D_MAKE_MEMBER_ENUM_ATTRIBUTE_ACCESSOR( variable )                              \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                                      \
        []( const ClassName& self, Urho3D::Variant& value )                                 \
        {                                                                                   \
            value = static_cast< int >( self.variable );                                    \
        },                                                                                  \
        []( ClassName& self, const Urho3D::Variant& value )                                 \
        {                                                                                   \
            self.variable = static_cast< decltype( self.variable ) >( value.Get< int >() ); \
        } )

/// Make member enum attribute accessor with custom post-set callback.
#define URHO3D_MAKE_MEMBER_ENUM_ATTRIBUTE_ACCESSOR_EX( variable, postSetCallback )          \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                                      \
        []( const ClassName& self, Urho3D::Variant& value )                                 \
        {                                                                                   \
            value = static_cast< int >( self.variable );                                    \
        },                                                                                  \
        []( ClassName& self, const Urho3D::Variant& value )                                 \
        {                                                                                   \
            self.variable = static_cast< decltype( self.variable ) >( value.Get< int >() ); \
            self.postSetCallback();                                                         \
        } )

/// Make get/set enum attribute accessor.
#define URHO3D_MAKE_GET_SET_ENUM_ATTRIBUTE_ACCESSOR( getFunction, setFunction, typeName ) \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(                                    \
        []( const ClassName& self, Urho3D::Variant& value )                               \
        {                                                                                 \
            value = static_cast< int >( self.getFunction() );                             \
        },                                                                                \
        []( ClassName& self, const Urho3D::Variant& value )                               \
        {                                                                                 \
            self.setFunction( static_cast< typeName >( value.Get< int >() ) );            \
        } )

/// Make fake accessor for an action.
#define URHO3D_MAKE_ACTION_LABEL_ACCESSOR( action, label )  \
    Urho3D::MakeVariantAttributeAccessor< ClassName >(      \
        []( const ClassName& self, Urho3D::Variant& value ) \
        {                                                   \
            value = ( label );                              \
        },                                                  \
        []( ClassName& self, const Urho3D::Variant& value ) \
        {                                                   \
            if ( value.GetBool() )                          \
                ( action );                                 \
        } )

    /// Attribute metadata.
    namespace AttributeMetadata
    {
        /// Names of vector struct elements. StringVector.
        URHO3D_GLOBAL_CONSTANT( ConstString VectorStructElements{ "VectorStructElements" } );
        /// Flag that indicates that attribute is an action.
        URHO3D_GLOBAL_CONSTANT( ConstString IsAction{ "IsAction" } );
        /// Flag that indicates that the attribute should be resizable by user. Applies only to ResourceRefList.
        URHO3D_GLOBAL_CONSTANT( ConstString AllowResize{ "AllowResize" } );
    }

// The following macros need to be used within a class member function such as ClassName::RegisterObject().
// A variable called "context" needs to exist in the current scope and point to a valid Context object.

/// Copy attributes from a base class.
#define URHO3D_COPY_BASE_ATTRIBUTES( sourceClassName )                   context->Reflect< ClassName >()->CopyAttributesFrom( context->GetReflection< sourceClassName >() )
/// Update the default value of an already registered attribute.
#define URHO3D_UPDATE_ATTRIBUTE_DEFAULT_VALUE( name, defaultValue )      context->UpdateAttributeDefaultValue< ClassName >( name, defaultValue )
/// Remove attribute by name.
#define URHO3D_REMOVE_ATTRIBUTE( name )                                  context->RemoveAttribute< ClassName >( name )

/// Define an object member attribute.
#define URHO3D_ATTRIBUTE( name, typeName, variable, defaultValue, mode ) context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::GetVariantType< typeName >(), name, URHO3D_MAKE_MEMBER_ATTRIBUTE_ACCESSOR( typeName, variable ), nullptr, defaultValue, mode ) )
/// Define an object member attribute. Post-set member function callback is called when attribute set.
#define URHO3D_ATTRIBUTE_EX( name, typeName, variable, postSetCallback, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::GetVariantType< typeName >(), name, URHO3D_MAKE_MEMBER_ATTRIBUTE_ACCESSOR_EX( typeName, variable, postSetCallback ), nullptr, defaultValue, mode ) )
/// Define an attribute that uses get and set functions.
#define URHO3D_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::GetVariantType< typeName >(), name, URHO3D_MAKE_GET_SET_ATTRIBUTE_ACCESSOR( getFunction, setFunction, typeName ), nullptr, defaultValue, mode ) )

/// Define an object member attribute. Zero-based enum values are mapped to names through an array of C string pointers.
#define URHO3D_ENUM_ATTRIBUTE( name, variable, enumNames, defaultValue, mode ) context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_INT, name, URHO3D_MAKE_MEMBER_ENUM_ATTRIBUTE_ACCESSOR( variable ), enumNames, static_cast< int >( defaultValue ), mode ) )
/// Define an object member attribute. Zero-based enum values are mapped to names through an array of C string pointers. Post-set member function callback is called when attribute set.
#define URHO3D_ENUM_ATTRIBUTE_EX( name, variable, postSetCallback, enumNames, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_INT, name, URHO3D_MAKE_MEMBER_ENUM_ATTRIBUTE_ACCESSOR_EX( variable, postSetCallback ), enumNames, static_cast< int >( defaultValue ), mode ) )
/// Define an attribute that uses get and set functions. Zero-based enum values are mapped to names through an array of C string pointers.
#define URHO3D_ENUM_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, enumNames, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_INT, name, URHO3D_MAKE_GET_SET_ENUM_ATTRIBUTE_ACCESSOR( getFunction, setFunction, typeName ), enumNames, static_cast< int >( defaultValue ), mode ) )

/// Define an attribute with custom setter and getter.
#define URHO3D_CUSTOM_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::GetVariantType< typeName >(), name, Urho3D::MakeVariantAttributeAccessor< ClassName >( getFunction, setFunction ), nullptr, defaultValue, mode ) )
/// Define an enum attribute with custom setter and getter. Zero-based enum values are mapped to names through an array of C string pointers.
#define URHO3D_CUSTOM_ENUM_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, enumNames, defaultValue, mode ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_INT, name, Urho3D::MakeVariantAttributeAccessor< ClassName >( getFunction, setFunction ), enumNames, static_cast< int >( defaultValue ), mode ) )

/// Define an object member attribute of any type.
#define URHO3D_ATTRIBUTE_CUSTOM( name, typeName, variable, defaultValue, mode ) context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_CUSTOM, name, URHO3D_MAKE_CUSTOM_MEMBER_ATTRIBUTE_ACCESSOR( typeName, variable ), nullptr, defaultValue, mode ) )

/// Define an action as fake attribute with static label.
#define URHO3D_ACTION_STATIC_LABEL( name, action, label )                       context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_BOOL, name, URHO3D_MAKE_ACTION_LABEL_ACCESSOR( self.action(), label ), nullptr, false, AM_EDIT ) ).SetMetadata( Urho3D::AttributeMetadata::IsAction, true )
/// Define an action as fake attribute with dynamic label.
#define URHO3D_ACTION_DYNAMIC_LABEL( name, action, label ) \
    context->Reflect< ClassName >()->AddAttribute( Urho3D::AttributeInfo( Urho3D::VAR_BOOL, name, URHO3D_MAKE_ACTION_LABEL_ACCESSOR( self.action(), self.label() ), nullptr, false, AM_EDIT ) ).SetMetadata( Urho3D::AttributeMetadata::IsAction, true )

/// Deprecated. Use URHO3D_ACCESSOR_ATTRIBUTE instead.
#define URHO3D_MIXED_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode ) URHO3D_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode )
/// Deprecated. Use URHO3D_CUSTOM_ACCESSOR_ATTRIBUTE instead.
#define URHO3D_CUSTOM_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode )         URHO3D_CUSTOM_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode )
/// Deprecated. Use URHO3D_CUSTOM_ENUM_ACCESSOR_ATTRIBUTE instead.
#define URHO3D_CUSTOM_ENUM_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode )    URHO3D_CUSTOM_ENUM_ACCESSOR_ATTRIBUTE( name, getFunction, setFunction, typeName, defaultValue, mode )

}
