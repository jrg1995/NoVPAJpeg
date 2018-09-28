//
//  kern_start.cpp
//  NoVPAJpeg
//
//  Modified from NoTouchID
//  Copyright (C) 2018 Alex James (TheRacerMaster). All rights reserved.
//

#include <Library/LegacyIOService.h>
#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

// Target framework

static const char *binPath = "/System/Library/PrivateFrameworks/AppleVPA.framework/Versions/A/AppleVPA";

// Accompanied processes
static const char *procList[] {
	"/System/Library/Frameworks/LocalAuthentication.framework/Support/DaemonUtils.framework/Versions/A/DaemonUtils"
};

static const uint8_t find[] = "jpeg";
static const uint8_t repl[] = "jpex";

static UserPatcher::BinaryModPatch patch {
	CPU_TYPE_X86_64,
	find,
	repl,
	strlen(reinterpret_cast<const char *>(find)),
	0,
	1,
	UserPatcher::FileSegment::SegmentTextCstring,
	1
};

UserPatcher::BinaryModInfo ADDPR(binaryMod)[] {
	{ binPath, &patch, 1 }
};

const size_t ADDPR(binaryModSize) = 1;

static UserPatcher::ProcInfo ADDPR(procInfo)[] {
	{ procList[0], static_cast<uint32_t>(strlen(procList[0])), 1 }
};

const size_t ADDPR(procInfoSize) = 1;

static void noVpaStart() {
	lilu.onProcLoad(ADDPR(procInfo), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryMod), ADDPR(binaryModSize));
}

static const char *bootargOff[] {
	"-novpaoff"
};

static const char *bootargBeta[] {
	"-novpabeta"
};

PluginConfiguration ADDPR(config) {
	xStringify(PRODUCT_NAME),
	parseModuleVersion(xStringify(MODULE_VERSION)),
	LiluAPI::AllowNormal,
	bootargOff,
	arrsize(bootargOff),
	nullptr,
	0,
	bootargBeta,
	arrsize(bootargBeta),
	KernelVersion::Mojave,
	KernelVersion::Mojave,
	[]() {
		noVpaStart();
	}
};
