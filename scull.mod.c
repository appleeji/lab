#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc776bb3e, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x1f44297f, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb9df1f59, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xf4202cbe, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x1092a52f, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xd8fe7e31, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x4573ee38, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x99951f1e, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x9294e88a, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xca0eff1a, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x19fcc1d7, __VMLINUX_SYMBOL_STR(ex_handler_default) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

