#pragma once
#include "json.hpp"

class MsJson : public Json
{
public:
    DINLINE MsJson() { ; }
    DINLINE MsJson(mstr& xJson) { *((Json*)this) = Json::parse(xJson); }

public:
    using ProtobufMsg = ::google::protobuf::Message;
    using ProtobufReflection = ::google::protobuf::Reflection;
    using ProtobufFieldDescriptor = ::google::protobuf::FieldDescriptor;
    using ProtobufDescriptor = ::google::protobuf::Descriptor;

public:
    static Boolean Json2Proto(const Json& json, ProtobufMsg& message, Boolean str2enum = false);
    static void Proto2Json(const ProtobufMsg& message, Json& json, Boolean enum2str = false);

protected:
    static Boolean Json2Repeated(const Json& json, ProtobufMsg& message, const ProtobufFieldDescriptor* field, const ProtobufReflection* reflection, Boolean str2enum);
    static void Repeated2Json(const ProtobufMsg& message, const ProtobufFieldDescriptor* field, const ProtobufReflection* reflection, Json& json, Boolean enum2str);
};
