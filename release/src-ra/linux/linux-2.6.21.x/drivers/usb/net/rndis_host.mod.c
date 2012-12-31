#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=usbnet,usbcore,cdc_ether";

MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic02isc02ipFF*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*icEFisc01ip01*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*icE0isc01ip03*");
