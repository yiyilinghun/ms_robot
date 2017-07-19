#pragma once
#define TIXML_USE_STL
//#include <string>  
//#include "google/protobuf/message.h"  

template<typename T>
T from_string(std::string s)
{
    istringstream is(s);
    T t;
    is >> t;
    return t;
}

std::string string_trim(std::string s, std::string t)
{
    return s;
}

//string convertToString(double d) {
//    ostringstream os;
//    if (os << d)
//        return os.str();
//    return "invalid conversion";
//}

bool tinyxml2proto(const std::string& xml, google::protobuf::Message& msg);

enum tinyxml_type
{
    TINYXML_GB2312,
    TINYXML_UTF8,
};

bool proto2tinyxml(const google::protobuf::Message& msg, std::string& xml, tinyxml_type type = TINYXML_UTF8);



//#include "InterProtoTinyXml.h"  
#include "xml/tinyxml.h"  
#include "google/protobuf/descriptor.h"  
//#include "utils.h"  

#define INTER_PROTO_TINYXML_NAME        "_name"  
#define INTER_PROTO_TINYXML_VALUE       "_value"  
#define INTER_PROTO_TINYXML_ATTR        "_attr_"  
#define INTER_PROTO_TINYXML_ATTR_SIZE   6  

using namespace google::protobuf;

template<typename T>
static T get_xml_field(TiXmlNode* node, bool& get_ok)
{
    for (TiXmlNode* child_node = node->FirstChild(); child_node != NULL; child_node = node->NextSibling())
    {
        if (child_node->Type() == TiXmlNode::TINYXML_TEXT)
        {
            get_ok = true;
            return from_string<T>(string_trim(child_node->ValueStr(), " \r\n\t\""));
        }
    }

    get_ok = false;
    return T();
}

template<>
static std::string get_xml_field<std::string>(TiXmlNode* node, bool& get_ok)
{
    for (TiXmlNode* child_node = node->FirstChild(); child_node != NULL; child_node = node->NextSibling())
    {
        if (child_node->Type() == TiXmlNode::TINYXML_TEXT)
        {
            get_ok = true;
            return string_trim(child_node->ValueStr(), " \r\n\t\"");
        }
    }

    get_ok = false;
    return "";
}

template<>
static bool get_xml_field<bool>(TiXmlNode* node, bool& get_ok)
{
    for (TiXmlNode* child_node = node->FirstChild(); child_node != NULL; child_node = node->NextSibling())
    {
        if (child_node->Type() == TiXmlNode::TINYXML_TEXT)
        {
            get_ok = true;
            return (bool)from_string<int>(string_trim(child_node->ValueStr(), " \r\n\t\""));
        }
    }

    get_ok = false;
    return false;
}

static bool parse_xmlattr2proto(TiXmlElement* element, google::protobuf::Message& msg)
{
    if (!element)
    {
        assert(0);
        return false;
    }

    //解析XML文件  
    const google::protobuf::Reflection* reflection = msg.GetReflection();
    const google::protobuf::Descriptor* des = msg.GetDescriptor();
    for (TiXmlAttribute* attr = element->FirstAttribute(); attr != NULL; attr = attr->Next())
    {
        std::string name = attr->Name();
        std::string value = attr->Value();
        const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_TINYXML_ATTR + name);
        if (fd)
        {
            switch (fd->type())
            {
                // double, exactly eight bytes on the wire.  
            case FieldDescriptor::TYPE_DOUBLE:
                if (fd->is_repeated())
                {
                    reflection->AddDouble(&msg, fd, from_string<double>(value));
                }
                else
                {
                    reflection->SetDouble(&msg, fd, from_string<double>(value));
                }

                break;

                // float, exactly four bytes on the wire.  
            case FieldDescriptor::TYPE_FLOAT:
                if (fd->is_repeated())
                {
                    reflection->AddFloat(&msg, fd, from_string<float>(value));
                }
                else
                {
                    reflection->SetFloat(&msg, fd, from_string<float>(value));
                }

                break;

                // int32, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT32 if negative  
                // values are likely.  
            case FieldDescriptor::TYPE_INT32:
                // int32, exactly four bytes on the wire  
            case FieldDescriptor::TYPE_SFIXED32:
                // int32, ZigZag-encoded varint on the wire  
            case FieldDescriptor::TYPE_SINT32:
                if (fd->is_repeated())
                {
                    reflection->AddInt32(&msg, fd, from_string<google::protobuf::int32>(value));
                }
                else
                {
                    reflection->SetInt32(&msg, fd, from_string<google::protobuf::int32>(value));
                }

                break;

                // uint32, varint on the wire  
            case FieldDescriptor::TYPE_UINT32:
                // uint32, exactly four bytes on the wire.  
            case FieldDescriptor::TYPE_FIXED32:
                if (fd->is_repeated())
                {
                    reflection->AddUInt32(&msg, fd, from_string<google::protobuf::uint32>(value));
                }
                else
                {
                    reflection->SetUInt32(&msg, fd, from_string<google::protobuf::uint32>(value));
                }

                break;

                // int64, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT64 if negative  
                // values are likely.  
            case FieldDescriptor::TYPE_INT64:
                // int64, exactly eight bytes on the wire  
            case FieldDescriptor::TYPE_SFIXED64:
                // int64, ZigZag-encoded varint on the wire  
            case FieldDescriptor::TYPE_SINT64:
                if (fd->is_repeated())
                {
                    reflection->AddInt64(&msg, fd, from_string<google::protobuf::int64>(value));
                }
                else
                {
                    reflection->SetInt64(&msg, fd, from_string<google::protobuf::int64>(value));
                }
                break;

                // uint64, varint on the wire.  
            case FieldDescriptor::TYPE_UINT64:
                // uint64, exactly eight bytes on the wire.  
            case FieldDescriptor::TYPE_FIXED64:
                if (fd->is_repeated())
                {
                    reflection->AddUInt64(&msg, fd, from_string<google::protobuf::uint64>(value));
                }
                else
                {
                    reflection->SetUInt64(&msg, fd, from_string<google::protobuf::uint64>(value));
                }

                break;

                // bool, varint on the wire.  
            case FieldDescriptor::TYPE_BOOL:
                if (fd->is_repeated())
                {
                    reflection->AddBool(&msg, fd, from_string<bool>(value));
                }
                else
                {
                    reflection->SetBool(&msg, fd, from_string<bool>(value));
                }

                break;

                // UTF-8 text.  
            case FieldDescriptor::TYPE_STRING:
                // Arbitrary byte array.  
            case FieldDescriptor::TYPE_BYTES:
                if (fd->is_repeated())
                {
                    reflection->AddString(&msg, fd, value);
                }
                else
                {
                    reflection->SetString(&msg, fd, value);
                }

                break;

                // Tag-delimited message.  Deprecated.  
            case FieldDescriptor::TYPE_GROUP:
                // Length-delimited message.  
            case FieldDescriptor::TYPE_MESSAGE:
                assert(0);
                return false;
                break;

                // Enum, varint on the wire  
            case FieldDescriptor::TYPE_ENUM:
            {
                const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(from_string<int>(value));
                if (!evd)
                {
                    return false;
                }

                if (fd->is_repeated())
                {
                    reflection->AddEnum(&msg, fd, evd);
                }
                else
                {
                    reflection->SetEnum(&msg, fd, evd);
                }
            }

            break;
            default:
                assert(0);
                return false;
                break;
            }
        }
    }

    return true;
}

static bool parse_xml2proto_msg(TiXmlNode* node, google::protobuf::Message& msg)
{
    assert(node && node->Type() == TiXmlNode::TINYXML_ELEMENT);
    //解析XML文件  
    const google::protobuf::Reflection* reflection = msg.GetReflection();
    const google::protobuf::Descriptor* des = msg.GetDescriptor();
    for (TiXmlNode* child_node = node->FirstChild(); child_node != NULL; child_node = child_node->NextSibling())
    {
        std::string child_value = child_node->ValueStr();
        switch (child_node->Type())
        {
        case TiXmlNode::TINYXML_ELEMENT:
        {
            const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(child_value);
            if (fd)
            {
                switch (fd->type())
                {
                    // double, exactly eight bytes on the wire.  
                case FieldDescriptor::TYPE_DOUBLE:
                {
                    bool get_ok = false;
                    double temp_value = get_xml_field<double>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddDouble(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetDouble(&msg, fd, temp_value);
                    }
                }

                break;

                // float, exactly four bytes on the wire.  
                case FieldDescriptor::TYPE_FLOAT:
                {
                    bool get_ok = false;
                    float temp_value = get_xml_field<float>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddFloat(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetFloat(&msg, fd, temp_value);
                    }
                }
                break;

                // int32, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT32 if negative  
                // values are likely.  
                case FieldDescriptor::TYPE_INT32:
                    // int32, exactly four bytes on the wire  
                case FieldDescriptor::TYPE_SFIXED32:
                    // int32, ZigZag-encoded varint on the wire  
                case FieldDescriptor::TYPE_SINT32:
                {
                    bool get_ok = false;
                    google::protobuf::int32 temp_value = get_xml_field<google::protobuf::int32>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddInt32(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetInt32(&msg, fd, temp_value);
                    }
                }

                break;

                // uint32, varint on the wire  
                case FieldDescriptor::TYPE_UINT32:
                    // uint32, exactly four bytes on the wire.  
                case FieldDescriptor::TYPE_FIXED32:
                {
                    bool get_ok = false;
                    google::protobuf::uint32 temp_value = get_xml_field<google::protobuf::uint32>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddUInt32(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetUInt32(&msg, fd, temp_value);
                    }
                }

                break;

                // int64, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT64 if negative  
                // values are likely.  
                case FieldDescriptor::TYPE_INT64:
                    // int64, exactly eight bytes on the wire  
                case FieldDescriptor::TYPE_SFIXED64:
                    // int64, ZigZag-encoded varint on the wire  
                case FieldDescriptor::TYPE_SINT64:
                {
                    bool get_ok = false;
                    google::protobuf::int64 temp_value = get_xml_field<google::protobuf::int64>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddInt64(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetInt64(&msg, fd, temp_value);
                    }
                }

                break;

                // uint64, varint on the wire.  
                case FieldDescriptor::TYPE_UINT64:
                    // uint64, exactly eight bytes on the wire.  
                case FieldDescriptor::TYPE_FIXED64:
                {
                    bool get_ok = false;
                    google::protobuf::uint64 temp_value = get_xml_field<google::protobuf::uint64>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddUInt64(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetUInt64(&msg, fd, temp_value);
                    }
                }

                break;

                // bool, varint on the wire.  
                case FieldDescriptor::TYPE_BOOL:
                {
                    bool get_ok = false;
                    bool temp_value = get_xml_field<bool>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddBool(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetBool(&msg, fd, temp_value);
                    }
                }

                break;

                // UTF-8 text.  
                case FieldDescriptor::TYPE_STRING:
                    // Arbitrary byte array.  
                case FieldDescriptor::TYPE_BYTES:
                {
                    bool get_ok = false;
                    std::string temp_value = get_xml_field<std::string>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddString(&msg, fd, temp_value);
                    }
                    else
                    {
                        reflection->SetString(&msg, fd, temp_value);
                    }
                }

                break;

                // Tag-delimited message.  Deprecated.  
                case FieldDescriptor::TYPE_GROUP:
                    // Length-delimited message.  
                case FieldDescriptor::TYPE_MESSAGE:
                    if (fd->is_repeated())
                    {
                        if (!parse_xml2proto_msg(child_node, *reflection->AddMessage(&msg, fd)))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (!parse_xml2proto_msg(child_node, *reflection->MutableMessage(&msg, fd)))
                        {
                            return false;
                        }
                    }

                    break;

                    // Enum, varint on the wire  
                case FieldDescriptor::TYPE_ENUM:
                {
                    bool get_ok = false;
                    int temp_value = get_xml_field<int>(child_node, get_ok);
                    if (!get_ok)
                    {
                        return false;
                    }

                    const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(temp_value);
                    if (!evd)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddEnum(&msg, fd, evd);
                    }
                    else
                    {
                        reflection->SetEnum(&msg, fd, evd);
                    }
                }

                break;
                default:
                    assert(0);
                    return false;
                    break;
                }
            }
        }

        break;
        case TiXmlNode::TINYXML_TEXT:
        {
            const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_TINYXML_VALUE);
            if (fd)
            {
                child_value = string_trim(child_value, " \r\n\t\"");
                switch (fd->type())
                {
                    // double, exactly eight bytes on the wire.  
                case FieldDescriptor::TYPE_DOUBLE:
                    if (fd->is_repeated())
                    {
                        reflection->AddDouble(&msg, fd, from_string<double>(child_value));
                    }
                    else
                    {
                        reflection->SetDouble(&msg, fd, from_string<double>(child_value));
                    }

                    break;

                    // float, exactly four bytes on the wire.  
                case FieldDescriptor::TYPE_FLOAT:
                    if (fd->is_repeated())
                    {
                        reflection->AddFloat(&msg, fd, from_string<float>(child_value));
                    }
                    else
                    {
                        reflection->SetFloat(&msg, fd, from_string<float>(child_value));
                    }

                    break;

                    // int32, varint on the wire.  Negative numbers  
                    // take 10 bytes.  Use TYPE_SINT32 if negative  
                    // values are likely.  
                case FieldDescriptor::TYPE_INT32:
                    // int32, exactly four bytes on the wire  
                case FieldDescriptor::TYPE_SFIXED32:
                    // int32, ZigZag-encoded varint on the wire  
                case FieldDescriptor::TYPE_SINT32:
                    if (fd->is_repeated())
                    {
                        reflection->AddInt32(&msg, fd, from_string<google::protobuf::int32>(child_value));
                    }
                    else
                    {
                        reflection->SetInt32(&msg, fd, from_string<google::protobuf::int32>(child_value));
                    }

                    break;

                    // uint32, varint on the wire  
                case FieldDescriptor::TYPE_UINT32:
                    // uint32, exactly four bytes on the wire.  
                case FieldDescriptor::TYPE_FIXED32:
                    if (fd->is_repeated())
                    {
                        reflection->AddUInt32(&msg, fd, from_string<google::protobuf::uint32>(child_value));
                    }
                    else
                    {
                        reflection->SetUInt32(&msg, fd, from_string<google::protobuf::uint32>(child_value));
                    }

                    break;

                    // int64, varint on the wire.  Negative numbers  
                    // take 10 bytes.  Use TYPE_SINT64 if negative  
                    // values are likely.  
                case FieldDescriptor::TYPE_INT64:
                    // int64, exactly eight bytes on the wire  
                case FieldDescriptor::TYPE_SFIXED64:
                    // int64, ZigZag-encoded varint on the wire  
                case FieldDescriptor::TYPE_SINT64:
                    if (fd->is_repeated())
                    {
                        reflection->AddInt64(&msg, fd, from_string<google::protobuf::int64>(child_value));
                    }
                    else
                    {
                        reflection->SetInt64(&msg, fd, from_string<google::protobuf::int64>(child_value));
                    }
                    break;

                    // uint64, varint on the wire.  
                case FieldDescriptor::TYPE_UINT64:
                    // uint64, exactly eight bytes on the wire.  
                case FieldDescriptor::TYPE_FIXED64:
                    if (fd->is_repeated())
                    {
                        reflection->AddUInt64(&msg, fd, from_string<google::protobuf::uint64>(child_value));
                    }
                    else
                    {
                        reflection->SetUInt64(&msg, fd, from_string<google::protobuf::uint64>(child_value));
                    }

                    break;

                    // bool, varint on the wire.  
                case FieldDescriptor::TYPE_BOOL:
                    if (fd->is_repeated())
                    {
                        reflection->AddBool(&msg, fd, from_string<bool>(child_value));
                    }
                    else
                    {
                        reflection->SetBool(&msg, fd, from_string<bool>(child_value));
                    }

                    break;

                    // UTF-8 text.  
                case FieldDescriptor::TYPE_STRING:
                    // Arbitrary byte array.  
                case FieldDescriptor::TYPE_BYTES:
                    if (fd->is_repeated())
                    {
                        reflection->AddString(&msg, fd, child_value);
                    }
                    else
                    {
                        reflection->SetString(&msg, fd, child_value);
                    }

                    break;

                    // Tag-delimited message.  Deprecated.  
                case FieldDescriptor::TYPE_GROUP:
                    // Length-delimited message.  
                case FieldDescriptor::TYPE_MESSAGE:
                    assert(0);
                    return false;
                    break;

                    // Enum, varint on the wire  
                case FieldDescriptor::TYPE_ENUM:
                {
                    const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(from_string<int>(child_value));
                    if (!evd)
                    {
                        return false;
                    }

                    if (fd->is_repeated())
                    {
                        reflection->AddEnum(&msg, fd, evd);
                    }
                    else
                    {
                        reflection->SetEnum(&msg, fd, evd);
                    }
                }

                break;
                default:
                    assert(0);
                    return false;
                    break;
                }
            }
        }

        break;
        case TiXmlNode::TINYXML_DECLARATION:
        case TiXmlNode::TINYXML_COMMENT:
            //ignore  
            break;
        case TiXmlNode::TINYXML_UNKNOWN:
        case TiXmlNode::TINYXML_DOCUMENT:
        case TiXmlNode::TINYXML_TYPECOUNT:
        default:
            assert(0);
            return false;
            break;
        }
    }

    //解析XML属性  
    if (!parse_xmlattr2proto(node->ToElement(), msg))
    {
        return false;
    }

    return true;
}

bool tinyxml2proto(const std::string& xml, google::protobuf::Message& msg)
{
    TiXmlDocument doc;
    doc.Parse(xml.c_str());
    TiXmlElement* root_ele = doc.RootElement();
    if (!root_ele)
    {
        return false;
    }

    const google::protobuf::Reflection* reflection = msg.GetReflection();
    const google::protobuf::Descriptor* des = msg.GetDescriptor();
    const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_TINYXML_NAME);
    if (fd && !fd->is_repeated() && (fd->type() == FieldDescriptor::TYPE_STRING || fd->type() == FieldDescriptor::TYPE_BYTES))
    {
        reflection->SetString(&msg, fd, root_ele->ValueStr());
    }

    if (!parse_xml2proto_msg(root_ele, msg))
    {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------  
static inline bool is_attribute(const std::string& str)
{
    if (str.size() > INTER_PROTO_TINYXML_ATTR_SIZE && str.substr(0, INTER_PROTO_TINYXML_ATTR_SIZE) == INTER_PROTO_TINYXML_ATTR)
    {
        return true;
    }

    return false;
}

static inline std::string get_attribute_name(const std::string& str)
{
    assert(is_attribute(str));
    return str.substr(INTER_PROTO_TINYXML_ATTR_SIZE);
}

static inline bool is_text(const std::string& str)
{
    return str == INTER_PROTO_TINYXML_VALUE;
}

static bool proto_fd2xml_attr(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor* fd, TiXmlElement* element, const std::string& name)
{
    const google::protobuf::Reflection* reflection = msg.GetReflection();
    switch (fd->type())
    {
        // double, exactly eight bytes on the wire.  
    case FieldDescriptor::TYPE_DOUBLE:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedDouble(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetDouble(msg, fd)));
        }

        break;

        // float, exactly four bytes on the wire.  
    case FieldDescriptor::TYPE_FLOAT:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedFloat(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetFloat(msg, fd)));
        }

        break;

        // int32, varint on the wire.  Negative numbers  
        // take 10 bytes.  Use TYPE_SINT32 if negative  
        // values are likely.  
    case FieldDescriptor::TYPE_INT32:
        // int32, exactly four bytes on the wire  
    case FieldDescriptor::TYPE_SFIXED32:
        // int32, ZigZag-encoded varint on the wire  
    case FieldDescriptor::TYPE_SINT32:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedInt32(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetInt32(msg, fd)));
        }

        break;

        // uint32, varint on the wire  
    case FieldDescriptor::TYPE_UINT32:
        // uint32, exactly four bytes on the wire.  
    case FieldDescriptor::TYPE_FIXED32:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedUInt32(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetUInt32(msg, fd)));
        }

        break;

        // int64, varint on the wire.  Negative numbers  
        // take 10 bytes.  Use TYPE_SINT64 if negative  
        // values are likely.  
    case FieldDescriptor::TYPE_INT64:
        // int64, exactly eight bytes on the wire  
    case FieldDescriptor::TYPE_SFIXED64:
        // int64, ZigZag-encoded varint on the wire  
    case FieldDescriptor::TYPE_SINT64:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedInt64(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetInt64(msg, fd)));
        }

        break;

        // uint64, varint on the wire.  
    case FieldDescriptor::TYPE_UINT64:
        // uint64, exactly eight bytes on the wire.  
    case FieldDescriptor::TYPE_FIXED64:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedUInt64(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetUInt64(msg, fd)));
        }

        break;

        // bool, varint on the wire.  
    case FieldDescriptor::TYPE_BOOL:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedBool(msg, fd, 0)));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetBool(msg, fd)));
        }

        break;

        // UTF-8 text.  
    case FieldDescriptor::TYPE_STRING:
        // Arbitrary byte array.  
    case FieldDescriptor::TYPE_BYTES:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, reflection->GetRepeatedString(msg, fd, 0));
            }
        }
        else
        {
            element->SetAttribute(name, reflection->GetString(msg, fd));
        }

        break;

        // Tag-delimited message.  Deprecated.  
    case FieldDescriptor::TYPE_GROUP:
        // Length-delimited message.  
    case FieldDescriptor::TYPE_MESSAGE:
        assert(0);
        return false;
        break;

        // Enum, varint on the wire  
    case FieldDescriptor::TYPE_ENUM:
        if (fd->is_repeated())
        {
            if (reflection->FieldSize(msg, fd) > 0)
            {
                element->SetAttribute(name, to_string(reflection->GetRepeatedEnum(msg, fd, 0)->number()));
            }
        }
        else
        {
            element->SetAttribute(name, to_string(reflection->GetEnum(msg, fd)->number()));
        }

        break;
    default:
        assert(0);
        return false;
        break;
    }

    return true;
}

static bool proto_fd2xml_text(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor* fd, TiXmlElement* element)
{
    const google::protobuf::Reflection* reflection = msg.GetReflection();
    switch (fd->type())
    {
        // double, exactly eight bytes on the wire.  
    case FieldDescriptor::TYPE_DOUBLE:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedDouble(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetDouble(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // float, exactly four bytes on the wire.  
    case FieldDescriptor::TYPE_FLOAT:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedFloat(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetFloat(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // int32, varint on the wire.  Negative numbers  
        // take 10 bytes.  Use TYPE_SINT32 if negative  
        // values are likely.  
    case FieldDescriptor::TYPE_INT32:
        // int32, exactly four bytes on the wire  
    case FieldDescriptor::TYPE_SFIXED32:
        // int32, ZigZag-encoded varint on the wire  
    case FieldDescriptor::TYPE_SINT32:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedInt32(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetInt32(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // uint32, varint on the wire  
    case FieldDescriptor::TYPE_UINT32:
        // uint32, exactly four bytes on the wire.  
    case FieldDescriptor::TYPE_FIXED32:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedUInt32(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetUInt32(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // int64, varint on the wire.  Negative numbers  
        // take 10 bytes.  Use TYPE_SINT64 if negative  
        // values are likely.  
    case FieldDescriptor::TYPE_INT64:
        // int64, exactly eight bytes on the wire  
    case FieldDescriptor::TYPE_SFIXED64:
        // int64, ZigZag-encoded varint on the wire  
    case FieldDescriptor::TYPE_SINT64:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedInt64(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetInt64(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // uint64, varint on the wire.  
    case FieldDescriptor::TYPE_UINT64:
        // uint64, exactly eight bytes on the wire.  
    case FieldDescriptor::TYPE_FIXED64:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedUInt64(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetUInt64(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // bool, varint on the wire.  
    case FieldDescriptor::TYPE_BOOL:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedBool(msg, fd, count)));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetBool(msg, fd)));
            element->LinkEndChild(temp_text);
        }

        break;

        // UTF-8 text.  
    case FieldDescriptor::TYPE_STRING:
        // Arbitrary byte array.  
    case FieldDescriptor::TYPE_BYTES:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(reflection->GetRepeatedString(msg, fd, count));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(reflection->GetString(msg, fd));
            element->LinkEndChild(temp_text);
        }

        break;

        // Tag-delimited message.  Deprecated.  
    case FieldDescriptor::TYPE_GROUP:
        // Length-delimited message.  
    case FieldDescriptor::TYPE_MESSAGE:
        assert(0);
        return false;
        break;

        // Enum, varint on the wire  
    case FieldDescriptor::TYPE_ENUM:
        if (fd->is_repeated())
        {
            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
            {
                TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedEnum(msg, fd, count)->number()));
                element->LinkEndChild(temp_text);
            }
        }
        else
        {
            TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetEnum(msg, fd)->number()));
            element->LinkEndChild(temp_text);
        }

        break;
    default:
        assert(0);
        return false;
        break;
    }

    return true;
}

static bool proto_msg2xml(const google::protobuf::Message& msg, TiXmlElement* element)
{
    assert(element);
    const google::protobuf::Reflection* reflection = msg.GetReflection();
    assert(reflection);
    const google::protobuf::Descriptor* des = msg.GetDescriptor();
    assert(des);
    for (int count = 0; count < des->field_count(); ++count)
    {
        const google::protobuf::FieldDescriptor* fd = des->field(count);
        assert(fd);
        std::string key = fd->name();
        //没有设置的非repeated字段不组装进xml  
        if (key == INTER_PROTO_TINYXML_NAME || (!fd->is_repeated() && !reflection->HasField(msg, fd)))
        {
            continue;
        }

        if (is_attribute(key))
        {
            if (!proto_fd2xml_attr(msg, fd, element, get_attribute_name(key)))
            {
                return false;
            }
        }
        else if (is_text(key))
        {
            if (!proto_fd2xml_text(msg, fd, element))
            {
                return false;
            }
        }
        else
        {
            switch (fd->type())
            {
                // double, exactly eight bytes on the wire.  
            case FieldDescriptor::TYPE_DOUBLE:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedDouble(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetDouble(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // float, exactly four bytes on the wire.  
            case FieldDescriptor::TYPE_FLOAT:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedFloat(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetFloat(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // int32, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT32 if negative  
                // values are likely.  
            case FieldDescriptor::TYPE_INT32:
                // int32, exactly four bytes on the wire  
            case FieldDescriptor::TYPE_SFIXED32:
                // int32, ZigZag-encoded varint on the wire  
            case FieldDescriptor::TYPE_SINT32:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedInt32(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetInt32(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // uint32, varint on the wire  
            case FieldDescriptor::TYPE_UINT32:
                // uint32, exactly four bytes on the wire.  
            case FieldDescriptor::TYPE_FIXED32:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedUInt32(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetUInt32(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // int64, varint on the wire.  Negative numbers  
                // take 10 bytes.  Use TYPE_SINT64 if negative  
                // values are likely.  
            case FieldDescriptor::TYPE_INT64:
                // int64, exactly eight bytes on the wire  
            case FieldDescriptor::TYPE_SFIXED64:
                // int64, ZigZag-encoded varint on the wire  
            case FieldDescriptor::TYPE_SINT64:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedInt64(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetInt64(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // uint64, varint on the wire.  
            case FieldDescriptor::TYPE_UINT64:
                // uint64, exactly eight bytes on the wire.  
            case FieldDescriptor::TYPE_FIXED64:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedUInt64(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetUInt64(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // bool, varint on the wire.  
            case FieldDescriptor::TYPE_BOOL:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedBool(msg, fd, count)));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetBool(msg, fd)));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // UTF-8 text.  
            case FieldDescriptor::TYPE_STRING:
                // Arbitrary byte array.  
            case FieldDescriptor::TYPE_BYTES:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(reflection->GetRepeatedString(msg, fd, count));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(reflection->GetString(msg, fd));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;

                // Tag-delimited message.  Deprecated.  
            case FieldDescriptor::TYPE_GROUP:
                // Length-delimited message.  
            case FieldDescriptor::TYPE_MESSAGE:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        if (!proto_msg2xml(reflection->GetRepeatedMessage(msg, fd, count), temp_ele))
                        {
                            delete temp_ele;
                            return false;
                        }

                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    //宏用于解决宏定义与函数名冲突  
#pragma push_macro("GetMessage")  
#undef GetMessage  
                    if (!proto_msg2xml(reflection->GetMessage(msg, fd), temp_ele))
                    {
                        delete temp_ele;
                        return false;
                    }

#pragma pop_macro("GetMessage")  
                    element->LinkEndChild(temp_ele);
                }

                break;

                // Enum, varint on the wire  
            case FieldDescriptor::TYPE_ENUM:
                if (fd->is_repeated())
                {
                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
                    {
                        TiXmlElement* temp_ele = new TiXmlElement(key);
                        TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetRepeatedEnum(msg, fd, count)->number()));
                        temp_ele->LinkEndChild(temp_text);
                        element->LinkEndChild(temp_ele);
                    }
                }
                else
                {
                    TiXmlElement* temp_ele = new TiXmlElement(key);
                    TiXmlText* temp_text = new TiXmlText(to_string(reflection->GetEnum(msg, fd)->number()));
                    temp_ele->LinkEndChild(temp_text);
                    element->LinkEndChild(temp_ele);
                }

                break;
            default:
                assert(0);
                return false;
                break;
            }
        }
    }

    return true;
}

bool proto2tinyxml(const google::protobuf::Message& msg, std::string& xml, tinyxml_type type/* = TINYXML_UTF8*/)
{
    TiXmlDocument doc;
    TiXmlDeclaration *declaration = NULL;
    if (type == TINYXML_GB2312)
    {
        declaration = new TiXmlDeclaration("1.0", "gb2312", "yes");
    }
    else if (type == TINYXML_UTF8)
    {
        declaration = new TiXmlDeclaration("1.0", "utf-8", "yes");
    }

    if (declaration)
    {
        doc.LinkEndChild(declaration);
    }

    const google::protobuf::Descriptor* des = msg.GetDescriptor();
    const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_TINYXML_NAME);
    if (!fd || fd->is_repeated() || (fd->type() != FieldDescriptor::TYPE_STRING && fd->type() != FieldDescriptor::TYPE_BYTES))
    {
        assert(0);
        return false;
    }

    const google::protobuf::Reflection* reflection = msg.GetReflection();
    TiXmlElement* root_ele = new TiXmlElement(reflection->GetString(msg, fd));
    doc.LinkEndChild(root_ele);
    if (!proto_msg2xml(msg, root_ele))
    {
        return false;
    }

    // Declare a printer      
    TiXmlPrinter printer;
    // attach it to the document you want to convert in to a std::string   
    doc.Accept(&printer);
    // Create a std::string and copy your document data in to the string      
    xml = printer.Str();
    return true;
}
//[cpp] view plain copy
#ifndef _INTER_PROTO_CMARKUP_H_  
#define _INTER_PROTO_CMARKUP_H_  

#include <string>  
#include "google/protobuf/message.h"  

bool cmarkup2proto(const std::string& xml, google::protobuf::Message& msg);

enum cmarkup_type
{
    CMARKUP_GB2312,
    CMARKUP_UTF8,
};

bool proto2cmarkup(const google::protobuf::Message& msg, std::string& xml, cmarkup_type type = CMARKUP_GB2312);

#endif //_INTER_PROTO_CMARKUP_H_  

//[cpp] view plain copy
//#include "InterProtoCmarkup.h"  
//#include "Markup.h"  
#include "google/protobuf/descriptor.h"  
//#include "utils/utils.h"  

#define INTER_PROTO_CMARKUP_NAME        "_name"  
#define INTER_PROTO_CMARKUP_VALUE       "_value"  
#define INTER_PROTO_CMARKUP_ATTR        "_attr_"  
#define INTER_PROTO_CMARKUP_ATTR_SIZE   6  

using namespace google::protobuf;

//template<typename T>
//static T get_xml_field(CMarkup& doc, bool& get_ok)
//{
//    if (!doc.IntoElem())
//    {
//        get_ok = false;
//        return T();
//    }
//
//    while (doc.FindNode())
//    {
//        if (doc.GetNodeType() == CMarkup::MNT_TEXT)
//        {
//            get_ok = true;
//            T ret = from_string<T>(string_trim(doc.GetData(), " \r\n\t\""));
//            doc.OutOfElem();
//            return ret;
//        }
//    }
//
//    doc.OutOfElem();
//    get_ok = false;
//    return T();
//}

//template<>
//static std::string get_xml_field<std::string>(CMarkup& doc, bool& get_ok)
//{
//    if (!doc.IntoElem())
//    {
//        get_ok = false;
//        return "";
//    }
//
//    while (doc.FindNode())
//    {
//        if (doc.GetNodeType() == CMarkup::MNT_TEXT)
//        {
//            get_ok = true;
//            std::string ret = string_trim(doc.GetData(), " \r\n\t\"");
//            doc.OutOfElem();
//            return ret;
//        }
//    }
//
//    doc.OutOfElem();
//    get_ok = false;
//    return "";
//}

//template<>
//static bool get_xml_field<bool>(CMarkup& doc, bool& get_ok)
//{
//    if (!doc.IntoElem())
//    {
//        get_ok = false;
//        return false;
//    }
//
//    while (doc.FindNode())
//    {
//        if (doc.GetNodeType() == CMarkup::MNT_TEXT)
//        {
//            get_ok = true;
//            bool ret = (bool)from_string<int>(string_trim(doc.GetData(), " \r\n\t\""));
//            doc.OutOfElem();
//            return ret;
//        }
//    }
//
//    doc.OutOfElem();
//    get_ok = false;
//    return false;
//}

//static bool parse_xmlattr2proto(CMarkup& doc, google::protobuf::Message& msg)
//{
//    //解析XML文件  
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    const google::protobuf::Descriptor* des = msg.GetDescriptor();
//    int attr_count = 0;
//#pragma warning(push)  
//#pragma warning(disable:4127)  
//    while (true)
//    {
//#pragma warning(pop)  
//        std::string name = doc.GetAttribName(attr_count);
//        if (name.empty())
//        {
//            break;
//        }
//        std::string value = doc.GetAttrib(name);
//        const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_CMARKUP_ATTR + name);
//        if (fd)
//        {
//            switch (fd->type())
//            {
//                // double, exactly eight bytes on the wire.  
//            case FieldDescriptor::TYPE_DOUBLE:
//                if (fd->is_repeated())
//                {
//                    reflection->AddDouble(&msg, fd, from_string<double>(value));
//                }
//                else
//                {
//                    reflection->SetDouble(&msg, fd, from_string<double>(value));
//                }
//
//                break;
//
//                // float, exactly four bytes on the wire.  
//            case FieldDescriptor::TYPE_FLOAT:
//                if (fd->is_repeated())
//                {
//                    reflection->AddFloat(&msg, fd, from_string<float>(value));
//                }
//                else
//                {
//                    reflection->SetFloat(&msg, fd, from_string<float>(value));
//                }
//
//                break;
//
//                // int32, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT32 if negative  
//                // values are likely.  
//            case FieldDescriptor::TYPE_INT32:
//                // int32, exactly four bytes on the wire  
//            case FieldDescriptor::TYPE_SFIXED32:
//                // int32, ZigZag-encoded varint on the wire  
//            case FieldDescriptor::TYPE_SINT32:
//                if (fd->is_repeated())
//                {
//                    reflection->AddInt32(&msg, fd, from_string<google::protobuf::int32>(value));
//                }
//                else
//                {
//                    reflection->SetInt32(&msg, fd, from_string<google::protobuf::int32>(value));
//                }
//
//                break;
//
//                // uint32, varint on the wire  
//            case FieldDescriptor::TYPE_UINT32:
//                // uint32, exactly four bytes on the wire.  
//            case FieldDescriptor::TYPE_FIXED32:
//                if (fd->is_repeated())
//                {
//                    reflection->AddUInt32(&msg, fd, from_string<google::protobuf::uint32>(value));
//                }
//                else
//                {
//                    reflection->SetUInt32(&msg, fd, from_string<google::protobuf::uint32>(value));
//                }
//
//                break;
//
//                // int64, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT64 if negative  
//                // values are likely.  
//            case FieldDescriptor::TYPE_INT64:
//                // int64, exactly eight bytes on the wire  
//            case FieldDescriptor::TYPE_SFIXED64:
//                // int64, ZigZag-encoded varint on the wire  
//            case FieldDescriptor::TYPE_SINT64:
//                if (fd->is_repeated())
//                {
//                    reflection->AddInt64(&msg, fd, from_string<google::protobuf::int64>(value));
//                }
//                else
//                {
//                    reflection->SetInt64(&msg, fd, from_string<google::protobuf::int64>(value));
//                }
//                break;
//
//                // uint64, varint on the wire.  
//            case FieldDescriptor::TYPE_UINT64:
//                // uint64, exactly eight bytes on the wire.  
//            case FieldDescriptor::TYPE_FIXED64:
//                if (fd->is_repeated())
//                {
//                    reflection->AddUInt64(&msg, fd, from_string<google::protobuf::uint64>(value));
//                }
//                else
//                {
//                    reflection->SetUInt64(&msg, fd, from_string<google::protobuf::uint64>(value));
//                }
//
//                break;
//
//                // bool, varint on the wire.  
//            case FieldDescriptor::TYPE_BOOL:
//                if (fd->is_repeated())
//                {
//                    reflection->AddBool(&msg, fd, from_string<bool>(value));
//                }
//                else
//                {
//                    reflection->SetBool(&msg, fd, from_string<bool>(value));
//                }
//
//                break;
//
//                // UTF-8 text.  
//            case FieldDescriptor::TYPE_STRING:
//                // Arbitrary byte array.  
//            case FieldDescriptor::TYPE_BYTES:
//                if (fd->is_repeated())
//                {
//                    reflection->AddString(&msg, fd, value);
//                }
//                else
//                {
//                    reflection->SetString(&msg, fd, value);
//                }
//
//                break;
//
//                // Tag-delimited message.  Deprecated.  
//            case FieldDescriptor::TYPE_GROUP:
//                // Length-delimited message.  
//            case FieldDescriptor::TYPE_MESSAGE:
//                assert(0);
//                return false;
//                break;
//
//                // Enum, varint on the wire  
//            case FieldDescriptor::TYPE_ENUM:
//            {
//                const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(from_string<int>(value));
//                if (!evd)
//                {
//                    return false;
//                }
//
//                if (fd->is_repeated())
//                {
//                    reflection->AddEnum(&msg, fd, evd);
//                }
//                else
//                {
//                    reflection->SetEnum(&msg, fd, evd);
//                }
//            }
//
//            break;
//            default:
//                assert(0);
//                return false;
//                break;
//            }
//        }
//
//        ++attr_count;
//    }
//
//    return true;
//}

//static bool parse_xml2proto_msg(CMarkup& doc, google::protobuf::Message& msg)
//{
//    assert(doc.GetNodeType() == CMarkup::MNT_ELEMENT);
//    if (!doc.IntoElem())
//    {
//        assert(0);
//        return false;
//    }
//
//    //解析XML文件  
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    const google::protobuf::Descriptor* des = msg.GetDescriptor();
//    while (doc.FindNode())
//    {
//        switch (doc.GetNodeType())
//        {
//        case CMarkup::MNT_ELEMENT:
//        {
//            std::string tag_name = doc.GetTagName();
//            const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(tag_name);
//            if (fd)
//            {
//                switch (fd->type())
//                {
//                    // double, exactly eight bytes on the wire.  
//                case FieldDescriptor::TYPE_DOUBLE:
//                {
//                    bool get_ok = false;
//                    double temp_value = get_xml_field<double>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddDouble(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetDouble(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // float, exactly four bytes on the wire.  
//                case FieldDescriptor::TYPE_FLOAT:
//                {
//                    bool get_ok = false;
//                    float temp_value = get_xml_field<float>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddFloat(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetFloat(&msg, fd, temp_value);
//                    }
//                }
//                break;
//
//                // int32, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT32 if negative  
//                // values are likely.  
//                case FieldDescriptor::TYPE_INT32:
//                    // int32, exactly four bytes on the wire  
//                case FieldDescriptor::TYPE_SFIXED32:
//                    // int32, ZigZag-encoded varint on the wire  
//                case FieldDescriptor::TYPE_SINT32:
//                {
//                    bool get_ok = false;
//                    google::protobuf::int32 temp_value = get_xml_field<google::protobuf::int32>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddInt32(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetInt32(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // uint32, varint on the wire  
//                case FieldDescriptor::TYPE_UINT32:
//                    // uint32, exactly four bytes on the wire.  
//                case FieldDescriptor::TYPE_FIXED32:
//                {
//                    bool get_ok = false;
//                    google::protobuf::uint32 temp_value = get_xml_field<google::protobuf::uint32>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddUInt32(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetUInt32(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // int64, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT64 if negative  
//                // values are likely.  
//                case FieldDescriptor::TYPE_INT64:
//                    // int64, exactly eight bytes on the wire  
//                case FieldDescriptor::TYPE_SFIXED64:
//                    // int64, ZigZag-encoded varint on the wire  
//                case FieldDescriptor::TYPE_SINT64:
//                {
//                    bool get_ok = false;
//                    google::protobuf::int64 temp_value = get_xml_field<google::protobuf::int64>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddInt64(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetInt64(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // uint64, varint on the wire.  
//                case FieldDescriptor::TYPE_UINT64:
//                    // uint64, exactly eight bytes on the wire.  
//                case FieldDescriptor::TYPE_FIXED64:
//                {
//                    bool get_ok = false;
//                    google::protobuf::uint64 temp_value = get_xml_field<google::protobuf::uint64>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddUInt64(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetUInt64(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // bool, varint on the wire.  
//                case FieldDescriptor::TYPE_BOOL:
//                {
//                    bool get_ok = false;
//                    bool temp_value = get_xml_field<bool>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddBool(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetBool(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // UTF-8 text.  
//                case FieldDescriptor::TYPE_STRING:
//                    // Arbitrary byte array.  
//                case FieldDescriptor::TYPE_BYTES:
//                {
//                    bool get_ok = false;
//                    std::string temp_value = get_xml_field<std::string>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddString(&msg, fd, temp_value);
//                    }
//                    else
//                    {
//                        reflection->SetString(&msg, fd, temp_value);
//                    }
//                }
//
//                break;
//
//                // Tag-delimited message.  Deprecated.  
//                case FieldDescriptor::TYPE_GROUP:
//                    // Length-delimited message.  
//                case FieldDescriptor::TYPE_MESSAGE:
//                    if (fd->is_repeated())
//                    {
//                        if (!parse_xml2proto_msg(doc, *reflection->AddMessage(&msg, fd)))
//                        {
//                            return false;
//                        }
//                    }
//                    else
//                    {
//                        if (!parse_xml2proto_msg(doc, *reflection->MutableMessage(&msg, fd)))
//                        {
//                            return false;
//                        }
//                    }
//
//                    break;
//
//                    // Enum, varint on the wire  
//                case FieldDescriptor::TYPE_ENUM:
//                {
//                    bool get_ok = false;
//                    int temp_value = get_xml_field<int>(doc, get_ok);
//                    if (!get_ok)
//                    {
//                        return false;
//                    }
//
//                    const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(temp_value);
//                    if (!evd)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddEnum(&msg, fd, evd);
//                    }
//                    else
//                    {
//                        reflection->SetEnum(&msg, fd, evd);
//                    }
//                }
//
//                break;
//                default:
//                    assert(0);
//                    return false;
//                    break;
//                }
//            }
//        }
//
//        break;
//        case CMarkup::MNT_TEXT:
//        {
//            const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_CMARKUP_VALUE);
//            if (fd)
//            {
//                std::string child_value = string_trim(doc.GetData(), " \r\n\t\"");
//                switch (fd->type())
//                {
//                    // double, exactly eight bytes on the wire.  
//                case FieldDescriptor::TYPE_DOUBLE:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddDouble(&msg, fd, from_string<double>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetDouble(&msg, fd, from_string<double>(child_value));
//                    }
//
//                    break;
//
//                    // float, exactly four bytes on the wire.  
//                case FieldDescriptor::TYPE_FLOAT:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddFloat(&msg, fd, from_string<float>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetFloat(&msg, fd, from_string<float>(child_value));
//                    }
//
//                    break;
//
//                    // int32, varint on the wire.  Negative numbers  
//                    // take 10 bytes.  Use TYPE_SINT32 if negative  
//                    // values are likely.  
//                case FieldDescriptor::TYPE_INT32:
//                    // int32, exactly four bytes on the wire  
//                case FieldDescriptor::TYPE_SFIXED32:
//                    // int32, ZigZag-encoded varint on the wire  
//                case FieldDescriptor::TYPE_SINT32:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddInt32(&msg, fd, from_string<google::protobuf::int32>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetInt32(&msg, fd, from_string<google::protobuf::int32>(child_value));
//                    }
//
//                    break;
//
//                    // uint32, varint on the wire  
//                case FieldDescriptor::TYPE_UINT32:
//                    // uint32, exactly four bytes on the wire.  
//                case FieldDescriptor::TYPE_FIXED32:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddUInt32(&msg, fd, from_string<google::protobuf::uint32>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetUInt32(&msg, fd, from_string<google::protobuf::uint32>(child_value));
//                    }
//
//                    break;
//
//                    // int64, varint on the wire.  Negative numbers  
//                    // take 10 bytes.  Use TYPE_SINT64 if negative  
//                    // values are likely.  
//                case FieldDescriptor::TYPE_INT64:
//                    // int64, exactly eight bytes on the wire  
//                case FieldDescriptor::TYPE_SFIXED64:
//                    // int64, ZigZag-encoded varint on the wire  
//                case FieldDescriptor::TYPE_SINT64:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddInt64(&msg, fd, from_string<google::protobuf::int64>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetInt64(&msg, fd, from_string<google::protobuf::int64>(child_value));
//                    }
//                    break;
//
//                    // uint64, varint on the wire.  
//                case FieldDescriptor::TYPE_UINT64:
//                    // uint64, exactly eight bytes on the wire.  
//                case FieldDescriptor::TYPE_FIXED64:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddUInt64(&msg, fd, from_string<google::protobuf::uint64>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetUInt64(&msg, fd, from_string<google::protobuf::uint64>(child_value));
//                    }
//
//                    break;
//
//                    // bool, varint on the wire.  
//                case FieldDescriptor::TYPE_BOOL:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddBool(&msg, fd, from_string<bool>(child_value));
//                    }
//                    else
//                    {
//                        reflection->SetBool(&msg, fd, from_string<bool>(child_value));
//                    }
//
//                    break;
//
//                    // UTF-8 text.  
//                case FieldDescriptor::TYPE_STRING:
//                    // Arbitrary byte array.  
//                case FieldDescriptor::TYPE_BYTES:
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddString(&msg, fd, child_value);
//                    }
//                    else
//                    {
//                        reflection->SetString(&msg, fd, child_value);
//                    }
//
//                    break;
//
//                    // Tag-delimited message.  Deprecated.  
//                case FieldDescriptor::TYPE_GROUP:
//                    // Length-delimited message.  
//                case FieldDescriptor::TYPE_MESSAGE:
//                    assert(0);
//                    return false;
//                    break;
//
//                    // Enum, varint on the wire  
//                case FieldDescriptor::TYPE_ENUM:
//                {
//                    const google::protobuf::EnumValueDescriptor* evd = fd->enum_type()->FindValueByNumber(from_string<int>(child_value));
//                    if (!evd)
//                    {
//                        return false;
//                    }
//
//                    if (fd->is_repeated())
//                    {
//                        reflection->AddEnum(&msg, fd, evd);
//                    }
//                    else
//                    {
//                        reflection->SetEnum(&msg, fd, evd);
//                    }
//                }
//
//                break;
//                default:
//                    assert(0);
//                    return false;
//                    break;
//                }
//            }
//        }
//
//        break;
//        case CMarkup::MNT_TEXT_AND_WHITESPACE:
//        case CMarkup::MNT_CDATA_SECTION:
//        case CMarkup::MNT_PROCESSING_INSTRUCTION:
//            assert(0);
//            return false;
//            break;
//        case CMarkup::MNT_WHITESPACE:
//        case CMarkup::MNT_COMMENT:
//            //ignore  
//            break;
//        case CMarkup::MNT_DOCUMENT_TYPE:
//        case CMarkup::MNT_EXCLUDE_WHITESPACE:
//        case CMarkup::MNT_LONE_END_TAG:
//        case CMarkup::MNT_NODE_ERROR:
//        default:
//            assert(0);
//            return false;
//            break;
//        }
//    }
//
//    doc.OutOfElem();
//    //解析XML属性  
//    if (!parse_xmlattr2proto(doc, msg))
//    {
//        return false;
//    }
//
//    return true;
//}

//bool cmarkup2proto(const std::string& xml, google::protobuf::Message& msg)
//{
//    CMarkup doc(xml);
//    bool find_ok = doc.FindElem();
//    if (!find_ok)
//    {
//        return false;
//    }
//
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    const google::protobuf::Descriptor* des = msg.GetDescriptor();
//    const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_CMARKUP_NAME);
//    if (fd && !fd->is_repeated() && (fd->type() == FieldDescriptor::TYPE_STRING || fd->type() == FieldDescriptor::TYPE_BYTES))
//    {
//        reflection->SetString(&msg, fd, doc.GetTagName());
//    }
//
//    if (!parse_xml2proto_msg(doc, msg))
//    {
//        return false;
//    }
//
//    return true;
//}
//
////---------------------------------------------------------------------------------------------------------  
//static inline bool is_attribute(const std::string& str)
//{
//    if (str.size() > INTER_PROTO_CMARKUP_ATTR_SIZE && str.substr(0, INTER_PROTO_CMARKUP_ATTR_SIZE) == INTER_PROTO_CMARKUP_ATTR)
//    {
//        return true;
//    }
//
//    return false;
//}
//
//static inline std::string get_attribute_name(const std::string& str)
//{
//    assert(is_attribute(str));
//    return str.substr(INTER_PROTO_CMARKUP_ATTR_SIZE);
//}
//
//static inline bool is_text(const std::string& str)
//{
//    return str == INTER_PROTO_CMARKUP_VALUE;
//}
//
//static bool proto_fd2xml_attr(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor* fd, CMarkup& doc, const std::string& name)
//{
//    //先定位主元素才可以进行属性操作  
//    if (!doc.OutOfElem())
//    {
//        return false;
//    }
//
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    switch (fd->type())
//    {
//        // double, exactly eight bytes on the wire.  
//    case FieldDescriptor::TYPE_DOUBLE:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedDouble(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetDouble(msg, fd)));
//        }
//
//        break;
//
//        // float, exactly four bytes on the wire.  
//    case FieldDescriptor::TYPE_FLOAT:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedFloat(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetFloat(msg, fd)));
//        }
//
//        break;
//
//        // int32, varint on the wire.  Negative numbers  
//        // take 10 bytes.  Use TYPE_SINT32 if negative  
//        // values are likely.  
//    case FieldDescriptor::TYPE_INT32:
//        // int32, exactly four bytes on the wire  
//    case FieldDescriptor::TYPE_SFIXED32:
//        // int32, ZigZag-encoded varint on the wire  
//    case FieldDescriptor::TYPE_SINT32:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedInt32(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetInt32(msg, fd)));
//        }
//
//        break;
//
//        // uint32, varint on the wire  
//    case FieldDescriptor::TYPE_UINT32:
//        // uint32, exactly four bytes on the wire.  
//    case FieldDescriptor::TYPE_FIXED32:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedUInt32(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetUInt32(msg, fd)));
//        }
//
//        break;
//
//        // int64, varint on the wire.  Negative numbers  
//        // take 10 bytes.  Use TYPE_SINT64 if negative  
//        // values are likely.  
//    case FieldDescriptor::TYPE_INT64:
//        // int64, exactly eight bytes on the wire  
//    case FieldDescriptor::TYPE_SFIXED64:
//        // int64, ZigZag-encoded varint on the wire  
//    case FieldDescriptor::TYPE_SINT64:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedInt64(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetInt64(msg, fd)));
//        }
//
//        break;
//
//        // uint64, varint on the wire.  
//    case FieldDescriptor::TYPE_UINT64:
//        // uint64, exactly eight bytes on the wire.  
//    case FieldDescriptor::TYPE_FIXED64:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedUInt64(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetUInt64(msg, fd)));
//        }
//
//        break;
//
//        // bool, varint on the wire.  
//    case FieldDescriptor::TYPE_BOOL:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, to_string(reflection->GetRepeatedBool(msg, fd, 0)));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, to_string(reflection->GetBool(msg, fd)));
//        }
//
//        break;
//
//        // UTF-8 text.  
//    case FieldDescriptor::TYPE_STRING:
//        // Arbitrary byte array.  
//    case FieldDescriptor::TYPE_BYTES:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, reflection->GetRepeatedString(msg, fd, 0));
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, reflection->GetString(msg, fd));
//        }
//
//        break;
//
//        // Tag-delimited message.  Deprecated.  
//    case FieldDescriptor::TYPE_GROUP:
//        // Length-delimited message.  
//    case FieldDescriptor::TYPE_MESSAGE:
//        assert(0);
//        return false;
//        break;
//
//        // Enum, varint on the wire  
//    case FieldDescriptor::TYPE_ENUM:
//        if (fd->is_repeated())
//        {
//            if (reflection->FieldSize(msg, fd) > 0)
//            {
//                doc.AddAttrib(name, reflection->GetRepeatedEnum(msg, fd, 0)->number());
//            }
//        }
//        else
//        {
//            doc.AddAttrib(name, reflection->GetEnum(msg, fd)->number());
//        }
//
//        break;
//    default:
//        assert(0);
//        return false;
//        break;
//    }
//
//    //重新进入tag  
//    doc.IntoElem();
//    return true;
//}
//
//static bool proto_fd2xml_text(const google::protobuf::Message& msg, const google::protobuf::FieldDescriptor* fd, CMarkup& doc)
//{
//    //先定位主元素才可以进行数据操作  
//    if (!doc.OutOfElem())
//    {
//        return false;
//    }
//
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    switch (fd->type())
//    {
//        // double, exactly eight bytes on the wire.  
//    case FieldDescriptor::TYPE_DOUBLE:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedDouble(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetDouble(msg, fd)));
//        }
//
//        break;
//
//        // float, exactly four bytes on the wire.  
//    case FieldDescriptor::TYPE_FLOAT:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedFloat(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetFloat(msg, fd)));
//        }
//
//        break;
//
//        // int32, varint on the wire.  Negative numbers  
//        // take 10 bytes.  Use TYPE_SINT32 if negative  
//        // values are likely.  
//    case FieldDescriptor::TYPE_INT32:
//        // int32, exactly four bytes on the wire  
//    case FieldDescriptor::TYPE_SFIXED32:
//        // int32, ZigZag-encoded varint on the wire  
//    case FieldDescriptor::TYPE_SINT32:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedInt32(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetInt32(msg, fd)));
//        }
//
//        break;
//
//        // uint32, varint on the wire  
//    case FieldDescriptor::TYPE_UINT32:
//        // uint32, exactly four bytes on the wire.  
//    case FieldDescriptor::TYPE_FIXED32:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedUInt32(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetUInt32(msg, fd)));
//        }
//
//        break;
//
//        // int64, varint on the wire.  Negative numbers  
//        // take 10 bytes.  Use TYPE_SINT64 if negative  
//        // values are likely.  
//    case FieldDescriptor::TYPE_INT64:
//        // int64, exactly eight bytes on the wire  
//    case FieldDescriptor::TYPE_SFIXED64:
//        // int64, ZigZag-encoded varint on the wire  
//    case FieldDescriptor::TYPE_SINT64:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedInt64(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetInt64(msg, fd)));
//        }
//
//        break;
//
//        // uint64, varint on the wire.  
//    case FieldDescriptor::TYPE_UINT64:
//        // uint64, exactly eight bytes on the wire.  
//    case FieldDescriptor::TYPE_FIXED64:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedUInt64(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetUInt64(msg, fd)));
//        }
//
//        break;
//
//        // bool, varint on the wire.  
//    case FieldDescriptor::TYPE_BOOL:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(to_string(reflection->GetRepeatedBool(msg, fd, count)));
//            }
//        }
//        else
//        {
//            doc.SetData(to_string(reflection->GetBool(msg, fd)));
//        }
//
//        break;
//
//        // UTF-8 text.  
//    case FieldDescriptor::TYPE_STRING:
//        // Arbitrary byte array.  
//    case FieldDescriptor::TYPE_BYTES:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(reflection->GetRepeatedString(msg, fd, count));
//            }
//        }
//        else
//        {
//            doc.SetData(reflection->GetString(msg, fd));
//        }
//
//        break;
//
//        // Tag-delimited message.  Deprecated.  
//    case FieldDescriptor::TYPE_GROUP:
//        // Length-delimited message.  
//    case FieldDescriptor::TYPE_MESSAGE:
//        assert(0);
//        return false;
//        break;
//
//        // Enum, varint on the wire  
//    case FieldDescriptor::TYPE_ENUM:
//        if (fd->is_repeated())
//        {
//            for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//            {
//                doc.SetData(reflection->GetRepeatedEnum(msg, fd, count)->number());
//            }
//        }
//        else
//        {
//            doc.SetData(reflection->GetEnum(msg, fd)->number());
//        }
//
//        break;
//    default:
//        assert(0);
//        return false;
//        break;
//    }
//
//    //重新进入tag  
//    doc.IntoElem();
//    return true;
//}
//
//static bool proto_msg2xml(const google::protobuf::Message& msg, CMarkup& doc, const std::string& name)
//{
//    //增加tag  
//    doc.AddElem(name);
//    //进入新增tag  
//    doc.IntoElem();
//
//    const google::protobuf::Reflection* reflection = msg.GetReflection();
//    assert(reflection);
//    const google::protobuf::Descriptor* des = msg.GetDescriptor();
//    assert(des);
//    for (int count = 0; count < des->field_count(); ++count)
//    {
//        const google::protobuf::FieldDescriptor* fd = des->field(count);
//        assert(fd);
//        std::string key = fd->name();
//        //没有设置的非repeated字段不组装进xml  
//        if (key == INTER_PROTO_CMARKUP_NAME || (!fd->is_repeated() && !reflection->HasField(msg, fd)))
//        {
//            continue;
//        }
//
//        if (is_attribute(key))
//        {
//            if (!proto_fd2xml_attr(msg, fd, doc, get_attribute_name(key)))
//            {
//                return false;
//            }
//        }
//        else if (is_text(key))
//        {
//            if (!proto_fd2xml_text(msg, fd, doc))
//            {
//                return false;
//            }
//        }
//        else
//        {
//            switch (fd->type())
//            {
//                // double, exactly eight bytes on the wire.  
//            case FieldDescriptor::TYPE_DOUBLE:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedDouble(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetDouble(msg, fd)));
//                }
//
//                break;
//
//                // float, exactly four bytes on the wire.  
//            case FieldDescriptor::TYPE_FLOAT:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedFloat(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetFloat(msg, fd)));
//                }
//
//                break;
//
//                // int32, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT32 if negative  
//                // values are likely.  
//            case FieldDescriptor::TYPE_INT32:
//                // int32, exactly four bytes on the wire  
//            case FieldDescriptor::TYPE_SFIXED32:
//                // int32, ZigZag-encoded varint on the wire  
//            case FieldDescriptor::TYPE_SINT32:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedInt32(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetInt32(msg, fd)));
//                }
//
//                break;
//
//                // uint32, varint on the wire  
//            case FieldDescriptor::TYPE_UINT32:
//                // uint32, exactly four bytes on the wire.  
//            case FieldDescriptor::TYPE_FIXED32:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedUInt32(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetUInt32(msg, fd)));
//                }
//
//                break;
//
//                // int64, varint on the wire.  Negative numbers  
//                // take 10 bytes.  Use TYPE_SINT64 if negative  
//                // values are likely.  
//            case FieldDescriptor::TYPE_INT64:
//                // int64, exactly eight bytes on the wire  
//            case FieldDescriptor::TYPE_SFIXED64:
//                // int64, ZigZag-encoded varint on the wire  
//            case FieldDescriptor::TYPE_SINT64:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedInt64(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetInt64(msg, fd)));
//                }
//
//                break;
//
//                // uint64, varint on the wire.  
//            case FieldDescriptor::TYPE_UINT64:
//                // uint64, exactly eight bytes on the wire.  
//            case FieldDescriptor::TYPE_FIXED64:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedUInt64(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetUInt64(msg, fd)));
//                }
//
//                break;
//
//                // bool, varint on the wire.  
//            case FieldDescriptor::TYPE_BOOL:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, to_string(reflection->GetRepeatedBool(msg, fd, count)));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, to_string(reflection->GetBool(msg, fd)));
//                }
//
//                break;
//
//                // UTF-8 text.  
//            case FieldDescriptor::TYPE_STRING:
//                // Arbitrary byte array.  
//            case FieldDescriptor::TYPE_BYTES:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, reflection->GetRepeatedString(msg, fd, count));
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, reflection->GetString(msg, fd));
//                }
//
//                break;
//
//                // Tag-delimited message.  Deprecated.  
//            case FieldDescriptor::TYPE_GROUP:
//                // Length-delimited message.  
//            case FieldDescriptor::TYPE_MESSAGE:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        if (!proto_msg2xml(reflection->GetRepeatedMessage(msg, fd, count), doc, key))
//                        {
//                            return false;
//                        }
//                    }
//                }
//                else
//                {
//                    //宏用于解决宏定义与函数名冲突  
//#pragma push_macro("GetMessage")  
//#undef GetMessage  
//                    if (!proto_msg2xml(reflection->GetMessage(msg, fd), doc, key))
//                    {
//                        return false;
//                    }
//
//#pragma pop_macro("GetMessage")  
//                }
//
//                break;
//
//                // Enum, varint on the wire  
//            case FieldDescriptor::TYPE_ENUM:
//                if (fd->is_repeated())
//                {
//                    for (int count = 0; count < reflection->FieldSize(msg, fd); ++count)
//                    {
//                        doc.AddElem(key, reflection->GetRepeatedEnum(msg, fd, count)->number());
//                    }
//                }
//                else
//                {
//                    doc.AddElem(key, reflection->GetEnum(msg, fd)->number());
//                }
//
//                break;
//            default:
//                assert(0);
//                return false;
//                break;
//            }
//        }
//    }
//
//    doc.OutOfElem();
//    return true;
//}
//
////bool proto2cmarkup(const google::protobuf::Message& msg, std::string& xml, cmarkup_type type/* = CMARKUP_GB2312*/)
////{
////    CMarkup doc;
////    if (type == CMARKUP_GB2312)
////    {
////        doc.SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");            //设置xml头  
////    }
////    else if (type == CMARKUP_UTF8)
////    {
////        doc.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");         //设置xml头  
////    }
////
////    const google::protobuf::Descriptor* des = msg.GetDescriptor();
////    const google::protobuf::FieldDescriptor* fd = des->FindFieldByName(INTER_PROTO_CMARKUP_NAME);
////    if (!fd || fd->is_repeated() || (fd->type() != FieldDescriptor::TYPE_STRING && fd->type() != FieldDescriptor::TYPE_BYTES))
////    {
////        assert(0);
////        return false;
////    }
////
////    const google::protobuf::Reflection* reflection = msg.GetReflection();
////    if (!proto_msg2xml(msg, doc, reflection->GetString(msg, fd)))
////    {
////        return false;
////    }
////
////    xml = doc.GetDoc();
////    return true;
////}
