#pragma once
#define __PJ_COMMON__

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4307)
#pragma warning(disable:4127)
#pragma warning(disable:4996)
#include "EnumDef.pb.h"
#include "Common.pb.h"
#include "Server.pb.h"
#include "NetPacket.pb.h"
#include "Database.pb.h"
#include "Mail.pb.h"
typedef google::protobuf::Message   BaseProto;
typedef google::protobuf::Message*  LPBASEPROTO;
#pragma warning(pop)

#include "MsBase.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "CommonResDef.h"

#ifndef var
#define var auto
#endif

#include "PJConfig.h"
#include "CommonBattle.h"

//#include "StaticData.h"
//#include "CenterService.h"

//#include "CenterNodeManager.h"
//#include "CenterMailManager.h"
//#include "CenterSceneManager.h"
//#include "CenterRoleDataManager.h"

//#include "CenterServer.h"

//#define AUTO_REG_FORWARD_PACKET(proto, gw_player, gw_factory, real_object, real_factory) ARCB(gw_player, gw_factory, Msg_##proto){proto xPacket; RET_TERMINAL_ANALYTIC_PROTO(xPacket);RetAssertMsg(m_IsLoginSuccessfully, "流程非法!");if (this->m_RoleIndex == xPacket.role_index()){this->SendPacketToCT(Msg_##proto, xPacket);return True;}return False;}ARCB(real_object, real_factory, Msg_##proto)
