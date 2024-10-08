#ifndef __SOC__
#define __SOC__

// NOTE:
// soc.c/soc.h are used by
// ARM and RISC-V backends

#include "../common/cpu.h"
#include <stdint.h>

#define UNKNOWN -1

typedef int32_t SOC;

enum {
  SOC_VENDOR_UNKNOWN,
  // ARM
  SOC_VENDOR_SNAPDRAGON,
  SOC_VENDOR_MEDIATEK,
  SOC_VENDOR_EXYNOS,
  SOC_VENDOR_KIRIN,
  SOC_VENDOR_KUNPENG,
  SOC_VENDOR_BROADCOM,
  SOC_VENDOR_APPLE,
  SOC_VENDOR_ROCKCHIP,
  SOC_VENDOR_GOOGLE,
  SOC_VENDOR_NVIDIA,
  SOC_VENDOR_AMPERE,
  SOC_VENDOR_NXP,
  SOC_VENDOR_AMLOGIC,
  SOC_VENDOR_MARVELL,
  // RISC-V
  SOC_VENDOR_SIFIVE,
  SOC_VENDOR_STARFIVE,
  SOC_VENDOR_SIPEED,
  SOC_VENDOR_SPACEMIT,
  // ARM & RISC-V
  SOC_VENDOR_ALLWINNER
};

struct system_on_chip {
  SOC model;
  VENDOR vendor;
  int32_t process;
  char* name;
  char* raw_name;
};

struct system_on_chip* get_soc(struct cpuInfo* cpu);
char* get_soc_name(struct system_on_chip* soc);
VENDOR get_soc_vendor(struct system_on_chip* soc);
bool match_soc(struct system_on_chip* soc, char* raw_name, char* expected_name, char* soc_name, SOC soc_model, int32_t process);
char* get_str_process(struct system_on_chip* soc);
void fill_soc(struct system_on_chip* soc, char* soc_name, SOC soc_model, int32_t process);
void fill_soc_raw(struct system_on_chip* soc, char* soc_name, VENDOR vendor);
#ifdef _WIN32
VENDOR try_match_soc_vendor_name(char* vendor_name);
#endif

#define SOC_START if (false) {}
#define SOC_EQ(raw_name, expected_name, soc_name, soc_model, soc, process) \
   else if (match_soc(soc, raw_name, expected_name, soc_name, soc_model, process)) return true;
#define SOC_END else { return false; }

#endif
