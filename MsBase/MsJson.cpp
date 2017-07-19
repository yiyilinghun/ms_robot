#include "Precompiled.h"

Boolean MsJson::Json2Proto(const Json& json, ProtobufMsg& message, Boolean str2enum)
{
    auto descriptor = message.GetDescriptor();
    auto reflection = message.GetReflection();
    if (nullptr == descriptor || nullptr == reflection)
    {
        return False;
    }

    auto count = descriptor->field_count();
    for (auto i = 0; i < count; ++i)
    {
        const auto field = descriptor->field(i);
        if (nullptr == field) continue;

        auto& value = json[field->name()];
        if (value.is_null()) continue;

        if (field->is_repeated())
        {
            if (!value.is_array())
            {
                return False;
            }
            else
            {
                Json2Repeated(value, message, field, reflection, str2enum);
                continue;
            }
        }

        switch (field->type()) {
            case ProtobufFieldDescriptor::TYPE_BOOL:
            {
                if (value.is_boolean())
                    reflection->SetBool(&message, field, value.get<Boolean>());
                else if (value.is_number_integer())
                    reflection->SetBool(&message, field, value.get<uint32_t>() != 0);
                else if (value.is_string()) {
                    if (value.get<std::string>() == "true")
                        reflection->SetBool(&message, field, true);
                    else if (value.get<std::string>() == "false")
                        reflection->SetBool(&message, field, false);
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_ENUM:
            {
                auto const* pedesc = field->enum_type();
                const ::google::protobuf::EnumValueDescriptor* pevdesc = nullptr;

                if (str2enum) {
                    pevdesc = pedesc->FindValueByName(value.get<std::string>());
                }
                else {
                    pevdesc = pedesc->FindValueByNumber(value.get<int>());
                }

                if (nullptr != pevdesc) {
                    reflection->SetEnum(&message, field, pevdesc);
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_INT32:
            case ProtobufFieldDescriptor::TYPE_SINT32:
            case ProtobufFieldDescriptor::TYPE_SFIXED32:
            {
                if (value.is_number()) reflection->SetInt32(&message, field, value.get<int32_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_UINT32:
            case ProtobufFieldDescriptor::TYPE_FIXED32:
            {
                if (value.is_number()) reflection->SetUInt32(&message, field, value.get<uint32_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_INT64:
            case ProtobufFieldDescriptor::TYPE_SINT64:
            case ProtobufFieldDescriptor::TYPE_SFIXED64:
            {
                if (value.is_number()) reflection->SetInt64(&message, field, value.get<int64_t>());
            } break;
            case ProtobufFieldDescriptor::TYPE_UINT64:
            case ProtobufFieldDescriptor::TYPE_FIXED64:
            {
                if (value.is_number()) reflection->SetUInt64(&message, field, value.get<uint64_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_FLOAT:
            {
                if (value.is_number()) reflection->SetFloat(&message, field, value.get<float>());
            } break;

            case ProtobufFieldDescriptor::TYPE_DOUBLE:
            {
                if (value.is_number()) reflection->SetDouble(&message, field, value.get<double>());
            } break;

            case ProtobufFieldDescriptor::TYPE_STRING:
            case ProtobufFieldDescriptor::TYPE_BYTES:
            {
                if (value.is_string()) reflection->SetString(&message, field, value.get<std::string>());
            } break;

            case ProtobufFieldDescriptor::TYPE_MESSAGE:
            {
                if (value.is_object()) Json2Proto(value, *reflection->MutableMessage(&message, field));
            } break;

            default:
                break;
        }
    }
    return True;
}

Boolean MsJson::Json2Repeated(const Json& json, ProtobufMsg& message, const ProtobufFieldDescriptor* field, const ProtobufReflection* reflection, Boolean str2enum)
{
    size_t count = json.size();
    for (size_t j = 0; j < count; ++j)
    {
        switch (field->type())
        {
            case ProtobufFieldDescriptor::TYPE_BOOL:
            {
                if (json[j].is_boolean())
                    reflection->AddBool(&message, field, json[j].get<Boolean>());
                else if (json[j].is_number_integer())
                    reflection->AddBool(&message, field, json[j].get<uint32_t>() != 0);
                else if (json[j].is_string()) {
                    if (json[j].get<std::string>() == "true")
                        reflection->AddBool(&message, field, true);
                    else if (json[j].get<std::string>() == "false")
                        reflection->AddBool(&message, field, false);
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_ENUM:
            {
                auto const* pedesc = field->enum_type();
                const ::google::protobuf::EnumValueDescriptor* pevdesc = nullptr;
                if (str2enum) {
                    pevdesc = pedesc->FindValueByName(json[j].get<std::string>());

                }
                else {
                    pevdesc = pedesc->FindValueByNumber(json[j].get<int32_t>());
                }
                if (nullptr != pevdesc) {
                    reflection->AddEnum(&message, field, pevdesc);
                }

            } break;

            case ProtobufFieldDescriptor::TYPE_INT32:
            case ProtobufFieldDescriptor::TYPE_SINT32:
            case ProtobufFieldDescriptor::TYPE_SFIXED32:
            {
                if (json[j].is_number()) reflection->AddInt32(&message, field, json[j].get<int32_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_UINT32:
            case ProtobufFieldDescriptor::TYPE_FIXED32:
            {
                if (json[j].is_number()) reflection->AddUInt32(&message, field, json[j].get<uint32_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_INT64:
            case ProtobufFieldDescriptor::TYPE_SINT64:
            case ProtobufFieldDescriptor::TYPE_SFIXED64:
            {
                if (json[j].is_number()) reflection->AddInt64(&message, field, json[j].get<int64_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_UINT64:
            case ProtobufFieldDescriptor::TYPE_FIXED64:
            {
                if (json[j].is_number()) reflection->AddUInt64(&message, field, json[j].get<uint64_t>());
            } break;

            case ProtobufFieldDescriptor::TYPE_FLOAT:
            {
                if (json[j].is_number()) reflection->AddFloat(&message, field, json[j].get<float>());
            } break;

            case ProtobufFieldDescriptor::TYPE_DOUBLE:
            {
                if (json[j].is_number()) reflection->AddDouble(&message, field, json[j].get<double>());
            } break;

            case ProtobufFieldDescriptor::TYPE_MESSAGE:
            {
                if (json[j].is_object()) Json2Proto(json[j], *reflection->AddMessage(&message, field));
            } break;

            case ProtobufFieldDescriptor::TYPE_STRING:
            case ProtobufFieldDescriptor::TYPE_BYTES:
            {
                if (json[j].is_string()) reflection->AddString(&message, field, json[j].get<std::string>());
            } break;

            default:
                break;
        }
    }
    return True;
}

void MsJson::Proto2Json(const ProtobufMsg& message, Json& json, Boolean enum2str)
{
    auto descriptor = message.GetDescriptor();
    auto reflection = message.GetReflection();
    if (nullptr == descriptor || nullptr == descriptor) return;

    auto count = descriptor->field_count();

    for (auto i = 0; i < count; ++i)
    {
        const auto field = descriptor->field(i);

        if (field->is_repeated())
        {
            if (reflection->FieldSize(message, field) > 0)
                Repeated2Json(message, field, reflection, json[field->name()], enum2str);

            continue;
        }

        if (!reflection->HasField(message, field))
        {
            continue;
        }

        switch (field->type())
        {
            case ProtobufFieldDescriptor::TYPE_MESSAGE:
            {
                const ProtobufMsg& tmp_message = reflection->GetMessage(message, field);
                if (0 != tmp_message.ByteSize()) Proto2Json(tmp_message, json[field->name()]);
            } break;

            case ProtobufFieldDescriptor::TYPE_BOOL:
                json[field->name()] = reflection->GetBool(message, field) ? true : false;
                break;

            case ProtobufFieldDescriptor::TYPE_ENUM:
            {
                auto* enum_value_desc = reflection->GetEnum(message, field);
                if (enum2str)
                {
                    json[field->name()] = enum_value_desc->name();
                }
                else {
                    json[field->name()] = enum_value_desc->number();
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_INT32:
            case ProtobufFieldDescriptor::TYPE_SINT32:
            case ProtobufFieldDescriptor::TYPE_SFIXED32:
                json[field->name()] = reflection->GetInt32(message, field);
                break;

            case ProtobufFieldDescriptor::TYPE_UINT32:
            case ProtobufFieldDescriptor::TYPE_FIXED32:
                json[field->name()] = reflection->GetUInt32(message, field);
                break;

            case ProtobufFieldDescriptor::TYPE_INT64:
            case ProtobufFieldDescriptor::TYPE_SINT64:
            case ProtobufFieldDescriptor::TYPE_SFIXED64:
                json[field->name()] = reflection->GetInt64(message, field);
                break;

            case ProtobufFieldDescriptor::TYPE_UINT64:
            case ProtobufFieldDescriptor::TYPE_FIXED64:
                json[field->name()] = reflection->GetUInt64(message, field);
                break;

            case ProtobufFieldDescriptor::TYPE_FLOAT:
                json[field->name()] = reflection->GetFloat(message, field);
                break;

            case ProtobufFieldDescriptor::TYPE_STRING:
            case ProtobufFieldDescriptor::TYPE_BYTES:
                json[field->name()] = reflection->GetString(message, field);
                break;

            default:
                break;
        }
    }
}

void MsJson::Repeated2Json(const ProtobufMsg& message, const ProtobufFieldDescriptor* field, const ProtobufReflection* reflection, Json& json, Boolean enum2str)
{
    if (nullptr == field || nullptr == reflection)
    {
        Proto2Json(message, json);
    }

    for (auto i = 0; i < reflection->FieldSize(message, field); ++i)
    {
        Json tmp_json;
        switch (field->type()) {
            case ProtobufFieldDescriptor::TYPE_MESSAGE:
            {
                const ProtobufMsg& tmp_message = reflection->GetRepeatedMessage(message, field, i);
                if (0 != tmp_message.ByteSize()) {
                    Proto2Json(tmp_message, tmp_json);
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_BOOL:
                tmp_json = reflection->GetRepeatedBool(message, field, i) ? true : false;
                break;

            case ProtobufFieldDescriptor::TYPE_ENUM:
            {
                auto* enum_value_desc = reflection->GetRepeatedEnum(message, field, i);
                if (enum2str) {
                    tmp_json = enum_value_desc->name();
                }
                else {
                    tmp_json = enum_value_desc->number();
                }
            } break;

            case ProtobufFieldDescriptor::TYPE_INT32:
            case ProtobufFieldDescriptor::TYPE_SINT32:
            case ProtobufFieldDescriptor::TYPE_SFIXED32:
                tmp_json = reflection->GetRepeatedInt32(message, field, i);
                break;

            case ProtobufFieldDescriptor::TYPE_UINT32:
            case ProtobufFieldDescriptor::TYPE_FIXED32:
                tmp_json = reflection->GetRepeatedUInt32(message, field, i);
                break;

            case ProtobufFieldDescriptor::TYPE_INT64:
            case ProtobufFieldDescriptor::TYPE_SINT64:
            case ProtobufFieldDescriptor::TYPE_SFIXED64:
                tmp_json = reflection->GetRepeatedInt64(message, field, i);
                break;

            case ProtobufFieldDescriptor::TYPE_UINT64:
            case ProtobufFieldDescriptor::TYPE_FIXED64:
                tmp_json = reflection->GetRepeatedUInt64(message, field, i);
                break;

            case ProtobufFieldDescriptor::TYPE_FLOAT:
                tmp_json = reflection->GetRepeatedFloat(message, field, i);
                break;

            case ProtobufFieldDescriptor::TYPE_STRING:
            case ProtobufFieldDescriptor::TYPE_BYTES:
                tmp_json = reflection->GetRepeatedString(message, field, i);
                break;

            default:
                break;
        }
        json += tmp_json;
    }
}

