export LINUXDIR := $(SRCBASE)/linux/linux-2.6.21.x

EXTRA_CFLAGS := -DLINUX26 -DCONFIG_RALINK -pipe -DDEBUG_NOISY -DDEBUG_RCTEST

export CONFIG_LINUX26=y
export CONFIG_RALINK=y

define platformRouterOptions
	@( \
	if [ "$(RALINK)" = "y" ]; then \
		sed -i "/RTCONFIG_RALINK/d" $(1); \
		echo "RTCONFIG_RALINK=y" >>$(1); \
		if [ "$(RT3883)" = "y" ]; then \
			sed -i "/RTCONFIG_RALINK_RT3883/d" $(1); \
			echo "RTCONFIG_RALINK_RT3883=y" >>$(1); \
		else \
			sed -i "/RTCONFIG_RALINK_RT3883/d" $(1); \
			echo "# RTCONFIG_RALINK_RT3883 is not set" >>$(1); \
		fi; \
		if [ "$(RT3052)" = "y" ]; then \
			sed -i "/RTCONFIG_RALINK_RT3052/d" $(1); \
			echo "RTCONFIG_RALINK_RT3052=y" >>$(1); \
		else \
			sed -i "/RTCONFIG_RALINK_RT3052/d" $(1); \
			echo "# RTCONFIG_RALINK_RT3052 is not set" >>$(1); \
		fi; \
	fi; \
	)
endef

define platformBusyboxOptions
endef

define platformKernelConfig
	@( \
	if [ "$(RALINK)" = "y" ]; then \
		if [ "$(RT3052)" = "y" ]; then \
			sed -i "/CONFIG_RALINK_RT3052_MP2/d" $(1); \
			echo "CONFIG_RALINK_RT3052_MP2=y" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3052/d" $(1); \
			echo "CONFIG_RALINK_RT3052=y" >>$(1); \
			sed -i "/CONFIG_MTD_PHYSMAP_START/d" $(1); \
			echo "CONFIG_MTD_PHYSMAP_START=0xBF000000" >>$(1); \
			sed -i "/CONFIG_RT3052_ASIC/d" $(1); \
			echo "CONFIG_RT3052_ASIC=y" >>$(1); \
			sed -i "/CONFIG_RT2880_DRAM_128M/d" $(1); \
			sed -i "/CONFIG_RT2880_DRAM_32M/d" $(1); \
			echo "CONFIG_RT2880_DRAM_32M=y" >>$(1); \
			sed -i "/CONFIG_RALINK_RAM_SIZE/d" $(1); \
			echo "CONFIG_RALINK_RAM_SIZE=32" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3883_MP/d" $(1); \
			echo "# CONFIG_RALINK_RT3883_MP is not set" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3883/d" $(1); \
			echo "# CONFIG_RALINK_RT3883 is not set" >>$(1); \
			sed -i "/CONFIG_RT3883_ASIC/d" $(1); \
			echo "# CONFIG_RT3883_ASIC is not set" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3883_MP/d" $(1); \
			echo "# CONFIG_RALINK_RT3883_MP is not set" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3883_MP/d" $(1); \
			echo "# CONFIG_RALINK_RT3883_MP is not set" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3662_2T2R/d" $(1); \
			echo "# CONFIG_RALINK_RT3662_2T2R is not set" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3052_2T2R/d" $(1); \
			echo "CONFIG_RALINK_RT3052_2T2R=y" >>$(1); \
			sed -i "/CONFIG_RALINK_RT3352/d" $(1); \
			echo "# CONFIG_RALINK_RT3352 is not set" >>$(1); \
			sed -i "/CONFIG_LAN_WAN_SUPPORT/d" $(1); \
			echo "CONFIG_LAN_WAN_SUPPORT=y" >>$(1); \
			sed -i "/CONFIG_RT_3052_ESW/d" $(1); \
			echo "CONFIG_RT_3052_ESW=y" >>$(1); \
		fi; \
	fi; \
	if [ "$(DSL)" = "y" ]; then \
		sed -i "/CONFIG_RTL8367M/d" $(1); \
		echo "# CONFIG_RTL8367M is not set" >>$(1); \
		sed -i "/CONFIG_RTL8367R/d" $(1); \
		echo "CONFIG_RTL8367R=y" >>$(1); \
		sed -i "/CONFIG_RAETH_GMAC2/d" $(1); \
		echo "# CONFIG_RAETH_GMAC2 is not set" >>$(1); \
		sed -i "/CONFIG_GE2_RGMII_FORCE_1000/d" $(1); \
		echo "# CONFIG_GE2_RGMII_FORCE_1000 is not set" >>$(1); \
		sed -i "/CONFIG_RAETH_DSL/d" $(1); \
		echo "CONFIG_RAETH_DSL=y" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT/d" $(1); \
		echo "# CONFIG_RA_HW_NAT is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_AUTO_BIND/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_AUTO_BIND is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_MANUAL_BIND/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_MANUAL_BIND is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_SPEEDUP_UPSTREAM/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_SPEEDUP_UPSTREAM is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_SPEEDUP_DOWNSTREAM/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_SPEEDUP_DOWNSTREAM is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_SPEEDUP_BIDIRECTION/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_SPEEDUP_BIDIRECTION is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_LAN_VLANID/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_LAN_VLANID is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_WAN_VLANID/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_WAN_VLANID is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_BINDING_THRESHOLD/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_BINDING_THRESHOLD is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_QURT_LMT/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_QURT_LMT is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_HALF_LMT/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_HALF_LMT is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_FULL_LMT/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_FULL_LMT is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TBL_1K/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TBL_1K is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TBL_2K/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TBL_2K is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TBL_4K/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TBL_4K is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TBL_8K/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TBL_8K is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TBL_16K/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TBL_16K is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_HASH0/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_HASH0 is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_HASH1/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_HASH1 is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_PRE_ACL_SIZE/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_PRE_ACL_SIZE is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_PRE_MTR_SIZE/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_PRE_MTR_SIZE is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_PRE_AC_SIZE/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_PRE_AC_SIZE is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_POST_MTR_SIZE/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_POST_MTR_SIZE is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_POST_AC_SIZE/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_POST_AC_SIZE is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TCP_KA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TCP_KA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_UDP_KA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_UDP_KA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_ACL_DLTA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_ACL_DLTA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_UNB_DLTA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_UNB_DLTA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_UNB_MNP/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_UNB_MNP is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_UDP_DLTA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_UDP_DLTA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_TCP_DLTA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_TCP_DLTA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_FIN_DLTA/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_FIN_DLTA is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_IPV6/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_IPV6 is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_ACL2UP_HELPER/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_ACL2UP_HELPER is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_DSCP2UP_HELPER/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_DSCP2UP_HELPER is not set" >>$(1); \
		sed -i "/CONFIG_RA_HW_NAT_NONE2UP/d" $(1); \
		echo "# CONFIG_RA_HW_NAT_NONE2UP is not set" >>$(1); \
	fi; \
	)
endef
