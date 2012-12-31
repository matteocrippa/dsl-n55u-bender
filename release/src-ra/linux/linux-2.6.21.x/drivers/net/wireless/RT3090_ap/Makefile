# If source directory exists, then build from source, otherwise use pre-builts
REBUILD_MODULE=$(shell if [ -d "$(src)/MODULE/common" ]; then echo 1; else echo 0; fi)
ifeq ($(REBUILD_MODULE),0)
# Assume prebuilt object module(s) in prebuilt directory
obj-$(CONFIG_RT3090_AP) += RT3090_ap_util.o
RT3090_ap_util-objs += prebuilt/RT3090_ap_util.o
obj-$(CONFIG_RT3090_AP) += RT3090_ap.o
RT3090_ap-objs += prebuilt/RT3090_ap.o
obj-$(CONFIG_RT3090_AP) += RT3090_ap_net.o
RT3090_ap_net-objs += prebuilt/RT3090_ap_net.o
else

EXTRA_CFLAGS = -Idrivers/net/wireless/RT3090_ap/MODULE/include

obj-$(CONFIG_RT3090_AP) += RT3090_ap_util.o
RT3090_ap_util-objs += UTIL/os/linux/rt_linux.o
RT3090_ap_util-objs += UTIL/os/linux/rt_rbus_pci_util.o

obj-$(CONFIG_RT3090_AP) += RT3090_ap.o
RT3090_ap-objs += MODULE/common/crypt_md5.o
RT3090_ap-objs += MODULE/common/crypt_sha2.o
RT3090_ap-objs += MODULE/common/crypt_biginteger.o
RT3090_ap-objs += MODULE/common/crypt_hmac.o
RT3090_ap-objs += MODULE/common/crypt_aes.o
RT3090_ap-objs += MODULE/common/crypt_arc4.o
RT3090_ap-objs += MODULE/common/mlme.o
RT3090_ap-objs += MODULE/common/cmm_wep.o
RT3090_ap-objs += MODULE/common/action.o
RT3090_ap-objs += MODULE/common/ba_action.o
RT3090_ap-objs += MODULE/common/cmm_data.o
RT3090_ap-objs += MODULE/common/rtmp_init.o
RT3090_ap-objs += MODULE/common/cmm_tkip.o
RT3090_ap-objs += MODULE/common/cmm_aes.o
RT3090_ap-objs += MODULE/common/cmm_sync.o
RT3090_ap-objs += MODULE/common/cmm_sanity.o
RT3090_ap-objs += MODULE/common/cmm_info.o
RT3090_ap-objs += MODULE/common/cmm_cfg.o
RT3090_ap-objs += MODULE/common/cmm_wpa.o
RT3090_ap-objs += MODULE/common/dfs.o
RT3090_ap-objs += MODULE/common/spectrum.o
RT3090_ap-objs += MODULE/common/rtmp_timer.o
RT3090_ap-objs += MODULE/common/rt_channel.o
RT3090_ap-objs += MODULE/common/cmm_profile.o
RT3090_ap-objs += MODULE/common/cmm_asic.o
RT3090_ap-objs += MODULE/common/cmm_cmd.o
RT3090_ap-objs += MODULE/common/rtmp_mcu.o
RT3090_ap-objs += MODULE/common/cmm_mac_pci.o
RT3090_ap-objs += MODULE/common/cmm_data_pci.o
RT3090_ap-objs += MODULE/common/ee_prom.o
RT3090_ap-objs += MODULE/common/ee_efuse.o
RT3090_ap-objs += MODULE/common/ee_flash.o
RT3090_ap-objs += MODULE/common/eeprom.o
RT3090_ap-objs += MODULE/common/rt_ate.o
RT3090_ap-objs += MODULE/common/rt_rf.o

RT3090_ap-objs += MODULE/ap/ap.o
RT3090_ap-objs += MODULE/ap/ap_assoc.o
RT3090_ap-objs += MODULE/ap/ap_auth.o
RT3090_ap-objs += MODULE/ap/ap_connect.o
RT3090_ap-objs += MODULE/ap/ap_mlme.o
RT3090_ap-objs += MODULE/ap/ap_sanity.o
RT3090_ap-objs += MODULE/ap/ap_sync.o
RT3090_ap-objs += MODULE/ap/ap_wpa.o
RT3090_ap-objs += MODULE/ap/ap_data.o
RT3090_ap-objs += MODULE/ap/ap_uapsd.o
RT3090_ap-objs += MODULE/ap/ap_autoChSel.o
RT3090_ap-objs += MODULE/ap/ap_qload.o
RT3090_ap-objs += MODULE/ap/ap_cfg.o

RT3090_ap-objs += MODULE/chips/rt30xx.o
RT3090_ap-objs += MODULE/chips/rt3090.o

obj-$(CONFIG_RT3090_AP) += RT3090_ap_net.o
RT3090_ap_net-objs += NETIF/common/rtmp_init_inf.o
RT3090_ap_net-objs += NETIF/os/linux/ap_ioctl.o
RT3090_ap_net-objs += NETIF/os/linux/rt_main_dev.o
RT3090_ap_net-objs += NETIF/os/linux/pci_main_dev.o
RT3090_ap_net-objs += NETIF/os/linux/rt_pci_rbus.o
RT3090_ap_net-objs += NETIF/os/linux/rt_profile.o

ifeq ($(CONFIG_RT3090_AP_WMM_ACM),y)
RT3090_ap-objs += MODULE/common/acm_edca.o
RT3090_ap-objs += MODULE/common/acm_comm.o
RT3090_ap-objs += MODULE/common/acm_iocl.o
endif

ifeq ($(CONFIG_RT3090_AP_WSC),y)
RT3090_ap-objs += MODULE/common/wsc.o 
RT3090_ap-objs += MODULE/common/wsc_tlv.o
RT3090_ap-objs += MODULE/common/crypt_dh.o
RT3090_ap-objs += MODULE/common/wsc_ufd.o
endif

#ifeq ($(CONFIG_RT3090_AP_NINTENDO),y)
#RT3090_ap-objs += MODULE/ap/ap_nintendo.o
#endif

ifeq ($(CONFIG_RT3090_AP_WDS),y)
RT3090_ap-objs += MODULE/ap/ap_wds.o
RT3090_ap_net-objs += NETIF/ap/ap_wds_inf.o
RT3090_ap-objs += MODULE/common/client_wds.o
endif

ifeq ($(CONFIG_RT3090_AP_MBSS),y)
RT3090_ap_net-objs += NETIF/ap/ap_mbss.o
endif

#ifeq ($(CONFIG_RT3090_AP_APCLI),y)
#RT3090_ap_net-objs += NETIF/ap/ap_apcli_inf.o
#RT3090_ap-objs += MODULE/ap/ap_apcli.o
#RT3090_ap-objs += MODULE/ap/apcli_assoc.o
#RT3090_ap-objs += MODULE/ap/apcli_auth.o
#RT3090_ap-objs += MODULE/ap/apcli_ctrl.o
#RT3090_ap-objs += MODULE/ap/apcli_sync.o
#RT3090_ap-objs += MODULE/common/cmm_mat.o
#RT3090_ap-objs += MODULE/common/cmm_mat_iparp.o
#RT3090_ap-objs += MODULE/common/cmm_mat_pppoe.o
#RT3090_ap-objs += MODULE/common/cmm_mat_ipv6.o
#endif

#ifeq ($(CONFIG_RT3090_80211R_FT),y)
#RT3090_ap-objs += MODULE/common/ft.o
#RT3090_ap-objs += MODULE/common/ft_tlv.o
#RT3090_ap-objs += MODULE/common/ft_ioctl.o
#RT3090_ap-objs += MODULE/common/ft_rc.o
#RT3090_ap-objs += MODULE/ap/ap_ftkd.o
#endif

ifeq ($(CONFIG_RT3090_AP_IGMP_SNOOP),y)
RT3090_ap-objs += MODULE/common/igmp_snoop.o
endif

ifeq ($(CONFIG_RT3090_AP_NETIF_BLOCK),y)
RT3090_ap-objs += MODULE/common/netif_block.o
endif

ifeq ($(CONFIG_RT3090_AP_DLS),y)
RT3090_ap-objs += MODULE/ap/ap_dls.o
endif

#ifeq ($(CONFIG_RT3090_AP_IDS),y)
#RT3090_ap-objs += MODULE/ap/ap_ids.o
#endif

#ifeq ($(CONFIG_RT3090_RT3XXX_AP_ANTENNA_DIVERSITY),y)
#RT3090_ap-objs += MODULE/os/linux/ap_diversity.o
#endif

#ifeq ($(CONFIG_RT3090_80211K_RR),y)
#RT3090_ap-objs += MODULE/common/rrm_tlv.o
#RT3090_ap-objs += MODULE/common/rrm_sanity.o
#RT3090_ap-objs += MODULE/common/rrm.o
#endif

endif # MODULE/common exists

###################
#  CFLAGS
#  ##################
EXTRA_CFLAGS += -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT  -DLINUX \
				-Wall -Wstrict-prototypes -Wno-trigraphs
EXTRA_CFLAGS += -DCONFIG_AP_SUPPORT -DAP_SCAN_SUPPORT -DUAPSD_AP_SUPPORT -DIAPP_SUPPORT -DDBG -DDOT11_N_SUPPORT -DDOT1X_SUPPORT -DSTATS_COUNT_SUPPORT #-DSYSTEM_LOG_SUPPORT
EXTRA_CFLAGS += -DRTMP_MAC_PCI -DRT30xx -DRT3090  -DRTMP_PCI_SUPPORT -DRTMP_RF_RW_SUPPORT -DRTMP_EFUSE_SUPPORT -DRALINK_ATE -DRALINK_QA -DRTMP_FLASH_SUPPORT -DOS_ABL_SUPPORT

# ASUS EXT
EXTRA_CFLAGS += -DLED_CONTROL_SUPPORT
EXTRA_CFLAGS += -DMCAST_RATE_SPECIFIC 
#EXTRA_CFLAGS += -DGREENAP_SUPPORT
ifneq ($(CONFIG_RA_HW_NAT_WIFI),y)
EXTRA_CFLAGS += -DCONFIG_RA_NAT_NONE
endif
EXTRA_CFLAGS += -DAUTO_CH_SELECT_ENHANCE
EXTRA_CFLAGS += -DRSSI_FEEDBACK
EXTRA_CFLAGS += -DSTATS_COUNT_SUPPORT

ifeq ($(CONFIG_RT3090_AP_DLS),y)
EXTRA_CFLAGS += -DQOS_DLS_SUPPORT
endif

#ifeq ($(CONFIG_RT3090_AP_IDS),y)
#EXTRA_CFLAGS += -DIDS_SUPPORT
#endif

#ifeq ($(CONFIG_RT3090_RT3XXX_AP_ANTENNA_DIVERSITY),y)
#EXTRA_CFLAGS   += -DRT3XXX_ANTENNA_DIVERSITY_SUPPORT
#endif

ifeq ($(CONFIG_RT3090_80211N_DRAFT3),y)
EXTRA_CFLAGS += -DDOT11N_DRAFT3
endif

ifeq ($(CONFIG_RT3090_AP_WMM_ACM),y)
EXTRA_CFLAGS += -DWMM_ACM_SUPPORT
endif

ifeq ($(CONFIG_RT3090_AP_WSC),y)
EXTRA_CFLAGS += -DWSC_AP_SUPPORT
endif

#ifeq ($(CONFIG_RT3090_AP_NINTENDO),y)
#EXTRA_CFLAGS += -DNINTENDO_AP
#endif

ifeq ($(CONFIG_RT3090_AP_LLTD),y)
EXTRA_CFLAGS += -DLLTD_SUPPORT
endif

ifeq ($(CONFIG_RT3090_AP_WDS),y)
EXTRA_CFLAGS += -DWDS_SUPPORT
endif

ifeq ($(CONFIG_RT3090_AP_MBSS),y)
EXTRA_CFLAGS += -DMBSS_SUPPORT
endif

#ifeq ($(CONFIG_RT3090_AP_APCLI),y)
#EXTRA_CFLAGS += -DAPCLI_SUPPORT
#EXTRA_CFLAGS += -DMAT_SUPPORT
##EXTRA_CFLAGS += -DMLME_EX
#endif

ifeq ($(CONFIG_RT3090_AP_IGMP_SNOOP),y)
EXTRA_CFLAGS += -DIGMP_SNOOP_SUPPORT
endif

ifeq ($(CONFIG_RT3090_AP_NETIF_BLOCK),y)
EXTRA_CFLAGS += -DBLOCK_NET_IF
endif

clean:
	@rm -f `find ./ -name "*.o.cmd"`
	@rm -f `find ./ -name "*.ko"`
	@rm -f `find ./ -name "*.o"`
