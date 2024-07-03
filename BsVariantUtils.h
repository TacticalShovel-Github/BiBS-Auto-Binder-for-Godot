#ifndef BS_VARIANT_UTILS_H
#define BS_VARIANT_UTILS_H

namespace Bs
{
    namespace Gd
    {
        #define BS_GD_VARIANT_XLATE(FROM,TO) \
            template<> struct VariantCodeFromType<FROM> \
            { \
                constexpr const static godot::Variant::Type value = TO; \
            }; \
            template<> struct VariantCodeFromType<FROM &> \
            { \
                constexpr const static godot::Variant::Type value = TO; \
            }; \
            template<> struct VariantCodeFromType<const FROM &> \
            { \
                constexpr const static godot::Variant::Type value = TO; \
            };

        template<typename T> struct VariantCodeFromType;

        //Special Case - godot::Variant::NIL
        template<> struct VariantCodeFromType<void> {constexpr const static godot::Variant::Type value = godot::Variant::NIL;};

        //Special Case - godot::Variant::OBJECT
        template<typename T> struct VariantCodeFromType<const godot::Ref<T> &>{constexpr const static godot::Variant::Type value = VariantCodeFromType<T>::value;};
        template<typename T> struct VariantCodeFromType<      godot::Ref<T> &>{constexpr const static godot::Variant::Type value = VariantCodeFromType<T>::value;};
        template<typename T> struct VariantCodeFromType<      godot::Ref<T>  >{constexpr const static godot::Variant::Type value = VariantCodeFromType<T>::value;};
        template<typename T> struct VariantCodeFromType<const godot::Ref<T>  >{constexpr const static godot::Variant::Type value = VariantCodeFromType<T>::value;};
        template<typename T> requires (std::derived_from<T,godot::Object> == true) struct VariantCodeFromType<T *>{constexpr const static godot::Variant::Type value = godot::Variant::OBJECT;};
        template<typename T> requires (std::derived_from<T,godot::Object> == true) struct VariantCodeFromType<T  >{constexpr const static godot::Variant::Type value = godot::Variant::OBJECT;};

        //Special Case - godot::Variant::INT
        template<typename T> requires (std::is_integral_v<T>) struct VariantCodeFromType<T>{constexpr const static godot::Variant::Type value = godot::Variant::INT;};
        template<typename T> requires (std::is_enum_v<T>) struct VariantCodeFromType<T>{constexpr const static godot::Variant::Type value = godot::Variant::INT;};
        template<typename T> requires (CBsEnumType<T>) struct VariantCodeFromType<T>{constexpr const static godot::Variant::Type value = godot::Variant::INT;};

        //Special Case - godot::Variant::FLOAT
        template<typename T> requires (std::is_floating_point_v<T>) struct VariantCodeFromType<T>{constexpr const static godot::Variant::Type value = godot::Variant::FLOAT;};

        //Special Case - godot::Variant::ARRAY
        template <> struct VariantCodeFromType<godot::Array>{constexpr const static godot::Variant::Type value = godot::Variant::ARRAY;};
        template<typename T> struct VariantCodeFromType<godot::TypedArray<T>>{constexpr const static godot::Variant::Type value = godot::Variant::ARRAY;};


      /*BS_GD_VARIANT_XLATE(void                      ,godot::Variant::NIL                 ); - Special Case -*/
        BS_GD_VARIANT_XLATE(bool                      ,godot::Variant::BOOL                );
      /*BS_GD_VARIANT_XLATE(int                       ,godot::Variant::INT                 ); - Special Case*/
      /*BS_GD_VARIANT_XLATE(real_t                    ,godot::Variant::FLOAT               ); - Special Case*/
        BS_GD_VARIANT_XLATE(godot::String             ,godot::Variant::STRING              );
        BS_GD_VARIANT_XLATE(godot::Vector2            ,godot::Variant::VECTOR2             );
        BS_GD_VARIANT_XLATE(godot::Vector2i           ,godot::Variant::VECTOR2I            );
        BS_GD_VARIANT_XLATE(godot::Rect2              ,godot::Variant::RECT2               );
        BS_GD_VARIANT_XLATE(godot::Rect2i             ,godot::Variant::RECT2I              );
        BS_GD_VARIANT_XLATE(godot::Vector3            ,godot::Variant::VECTOR3             );
        BS_GD_VARIANT_XLATE(godot::Vector3i           ,godot::Variant::VECTOR3I            );
        BS_GD_VARIANT_XLATE(godot::Transform3D        ,godot::Variant::TRANSFORM3D         );
        BS_GD_VARIANT_XLATE(godot::Projection         ,godot::Variant::PROJECTION          );
        BS_GD_VARIANT_XLATE(godot::Color              ,godot::Variant::COLOR               );
        BS_GD_VARIANT_XLATE(godot::StringName         ,godot::Variant::STRING_NAME         );
		//RID,
        BS_GD_VARIANT_XLATE(godot::NodePath           ,godot::Variant::NODE_PATH           );
      /*BS_GD_VARIANT_XLATE(godot::Object             ,godot::Variant::OBJECT              ); - Special Case*/
        BS_GD_VARIANT_XLATE(godot::Callable           ,godot::Variant::CALLABLE            );
        BS_GD_VARIANT_XLATE(godot::Signal             ,godot::Variant::SIGNAL              );
        BS_GD_VARIANT_XLATE(godot::Dictionary         ,godot::Variant::DICTIONARY          );
      /*BS_GD_VARIANT_XLATE(godot::ARRAY              ,godot::Variant::ARRAY               );*/
        
        BS_GD_VARIANT_XLATE(godot::PackedByteArray    ,godot::Variant::PACKED_BYTE_ARRAY   );
        BS_GD_VARIANT_XLATE(godot::PackedInt32Array   ,godot::Variant::PACKED_INT32_ARRAY  );
        BS_GD_VARIANT_XLATE(godot::PackedInt64Array   ,godot::Variant::PACKED_INT64_ARRAY  );
        BS_GD_VARIANT_XLATE(godot::PackedFloat32Array ,godot::Variant::PACKED_FLOAT32_ARRAY);
        BS_GD_VARIANT_XLATE(godot::PackedFloat64Array ,godot::Variant::PACKED_FLOAT64_ARRAY);
        BS_GD_VARIANT_XLATE(godot::PackedStringArray  ,godot::Variant::PACKED_STRING_ARRAY );
        BS_GD_VARIANT_XLATE(godot::PackedVector2Array ,godot::Variant::PACKED_VECTOR2_ARRAY);
        BS_GD_VARIANT_XLATE(godot::PackedVector3Array ,godot::Variant::PACKED_VECTOR3_ARRAY);
        BS_GD_VARIANT_XLATE(godot::PackedColorArray   ,godot::Variant::PACKED_COLOR_ARRAY  );

        template<typename T>
        constexpr const godot::Variant::Type VcGet=VariantCodeFromType<T>::value;
    }
}

#endif