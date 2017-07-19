#pragma once
#pragma warning(disable:4512)

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4307)
#pragma warning(disable:4125)
#pragma warning(disable:4127)
#include "EnumDef.pb.h"
#include "Server.pb.h"
typedef google::protobuf::Message   BaseProto;
typedef google::protobuf::Message*  LPBASEPROTO;
#pragma warning(pop)

#define TEMP_DEF_TERMINAL_CUSTOM_OBJECT_TYPE
#include "MsBase.h"
