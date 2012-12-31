cmd_drivers/net/raeth.dsl/ra_mac.o := /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc -Wp,-MD,drivers/net/raeth.dsl/.ra_mac.o.d  -nostdinc -isystem /root/asuswrt-bender/tools/brcm/K26/hndtools-mipsel-uclibc-4.2.4/bin/../lib/gcc/mipsel-linux-uclibc/4.2.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2  -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips/rt2880 -Iinclude/asm-mips/mach-generic -fomit-frame-pointer -gdwarf-2  -fno-stack-protector -membedded-data -muninit-const-in-rodata -funit-at-a-time -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ra_mac)"  -D"KBUILD_MODNAME=KBUILD_STR(raeth)" -c -o drivers/net/raeth.dsl/ra_mac.o drivers/net/raeth.dsl/ra_mac.c

deps_drivers/net/raeth.dsl/ra_mac.o := \
  drivers/net/raeth.dsl/ra_mac.c \
    $(wildcard include/config/user/snmpd.h) \
    $(wildcard include/config/gigaphy.h) \
    $(wildcard include/config/100phy.h) \
    $(wildcard include/config/p5/mac/to/phy/mode.h) \
    $(wildcard include/config/ralink/rt3052.h) \
    $(wildcard include/config/ralink/rt3352.h) \
    $(wildcard include/config/ge2/rgmii/an.h) \
    $(wildcard include/config/ge2/mii/an.h) \
    $(wildcard include/config/mac/to/gigaphy/mode/addr2.h) \
    $(wildcard include/config/ge1/rgmii/an.h) \
    $(wildcard include/config/ge1/mii/an.h) \
    $(wildcard include/config/mac/to/gigaphy/mode/addr.h) \
    $(wildcard include/config/ethtool.h) \
    $(wildcard include/config/raeth/router.h) \
    $(wildcard include/config/rt/3052/esw.h) \
    $(wildcard include/config/raeth/qos.h) \
    $(wildcard include/config/ralink/rt2883.h) \
    $(wildcard include/config/ralink/rt3883.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/thread_info.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbd.h) \
    $(wildcard include/config/lsf.h) \
    $(wildcard include/config/resources/64bit.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/forced/inlining.h) \
  include/linux/compiler-gcc.h \
  include/asm/posix_types.h \
  include/asm/sgidefs.h \
  include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/64bit.h) \
  include/linux/bitops.h \
  include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/cpu/mips64.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
    $(wildcard include/config/mips/mt/smtc/instant/replay.h) \
  include/asm/hazards.h \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
  include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm/byteorder.h \
    $(wildcard include/config/cpu/mips64/r2.h) \
  include/linux/byteorder/little_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/cpu-features.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  include/asm/cpu.h \
  include/asm/cpu-info.h \
    $(wildcard include/config/sgi/ip27.h) \
    $(wildcard include/config/mips/mt.h) \
  include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  include/asm-mips/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  include/asm-mips/mach-generic/cpu-feature-overrides.h \
  include/asm/war.h \
    $(wildcard include/config/sgi/ip22.h) \
    $(wildcard include/config/sni/rm.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/sb1/pass/1/workarounds.h) \
    $(wildcard include/config/sb1/pass/2/workarounds.h) \
    $(wildcard include/config/mips/malta.h) \
    $(wildcard include/config/mips/atlas.h) \
    $(wildcard include/config/mips/sead.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/momenco/jaguar/atx.h) \
    $(wildcard include/config/pmc/yosemite.h) \
    $(wildcard include/config/basler/excite.h) \
    $(wildcard include/config/momenco/ocelot/3.h) \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/processor.h \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/hotplug/cpu.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/numa.h) \
  /root/asuswrt-bender/tools/brcm/K26/hndtools-mipsel-uclibc-4.2.4/bin/../lib/gcc/mipsel-linux-uclibc/4.2.4/include/stdarg.h \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
  include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  include/asm/cachectl.h \
  include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
  include/asm/prefetch.h \
  include/asm/system.h \
  include/asm/addrspace.h \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/rm7000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/r8000.h) \
    $(wildcard include/config/cpu/sb1a.h) \
  include/asm-mips/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/asm/dsp.h \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/locking.h) \
  include/linux/spinlock_types_up.h \
  include/linux/spinlock_up.h \
  include/linux/spinlock_api_up.h \
  include/asm/atomic.h \
  include/asm-generic/atomic.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/linux/stat.h \
  include/asm/stat.h \
  include/linux/time.h \
  include/linux/seqlock.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kmod.h \
    $(wildcard include/config/kmod.h) \
  include/linux/errno.h \
  include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/elf.h \
  include/linux/auxvec.h \
  include/asm/auxvec.h \
  include/linux/elf-em.h \
  include/asm/elf.h \
    $(wildcard include/config/mips32/n32.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/wait.h \
  include/asm/current.h \
  include/linux/moduleparam.h \
  include/linux/init.h \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/asm/local.h \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
  include/asm/page.h \
    $(wildcard include/config/build/elf64.h) \
    $(wildcard include/config/limited/dma.h) \
  include/linux/pfn.h \
  include/asm/io.h \
  include/asm-generic/iomap.h \
  include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
    $(wildcard include/config/mips/uncached.h) \
  include/asm-mips/mach-generic/ioremap.h \
  include/asm-mips/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
  include/asm-generic/page.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/smp.h \
  include/asm/topology.h \
  include/asm-mips/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/slub_def.h \
  include/linux/workqueue.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/calc64.h \
  include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/timex.h \
    $(wildcard include/config/time/interpolation.h) \
    $(wildcard include/config/no/hz.h) \
  include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/asm/timex.h \
  include/asm-mips/mach-generic/timex.h \
  include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/r6000.h) \
  include/asm/uaccess.h \
  include/asm-generic/uaccess.h \
  include/linux/version.h \
  include/linux/sched.h \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/fault/injection.h) \
  include/linux/capability.h \
  include/linux/rbtree.h \
  include/asm/semaphore.h \
  include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  include/asm/isadep.h \
  include/asm/mmu.h \
  include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
    $(wildcard include/config/ipc/ns.h) \
  include/asm/ipcbuf.h \
  include/asm/sembuf.h \
  include/linux/signal.h \
  include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
    $(wildcard include/config/binfmt/irix.h) \
  include/asm-generic/signal.h \
  include/asm/sigcontext.h \
  include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/securebits.h \
  include/linux/fs_struct.h \
  include/linux/completion.h \
  include/linux/pid.h \
  include/linux/rcupdate.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/futex.h \
    $(wildcard include/config/futex.h) \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/param.h \
  include/linux/resource.h \
  include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/fcntl.h \
  include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/proc/fs.h) \
  include/linux/irqreturn.h \
  include/linux/hardirq.h \
    $(wildcard include/config/preempt/bkl.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/linux/smp_lock.h \
    $(wildcard include/config/lock/kernel.h) \
  include/asm/hardirq.h \
  include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/irq/per/cpu.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/irqbalance.h) \
    $(wildcard include/config/auto/irq/affinity.h) \
    $(wildcard include/config/generic/hardirqs/no//do/irq.h) \
  include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
  include/asm/mipsmtregs.h \
  include/asm-mips/mach-generic/irq.h \
    $(wildcard include/config/irq/cpu/rm7k.h) \
    $(wildcard include/config/irq/cpu/rm9k.h) \
  include/asm/irq_regs.h \
  include/asm/hw_irq.h \
  include/linux/profile.h \
    $(wildcard include/config/profiling.h) \
  include/linux/irq_cpustat.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm.h) \
  include/asm/device.h \
  include/asm-generic/device.h \
  include/linux/ptrace.h \
  include/linux/in.h \
  include/linux/socket.h \
  include/asm/socket.h \
  include/asm/sockios.h \
  include/asm/ioctl.h \
  include/linux/sockios.h \
  include/asm/dma.h \
    $(wildcard include/config/generic/isa/dma/support/broken.h) \
    $(wildcard include/config/pci.h) \
  include/linux/delay.h \
  include/asm/delay.h \
  include/asm/compiler.h \
  include/asm-mips/mipsregs.h \
  include/asm/rt2880/surfboardint.h \
    $(wildcard include/config/ralink/rt2880.h) \
  include/linux/mca.h \
    $(wildcard include/config/mca.h) \
    $(wildcard include/config/mca/proc/fs.h) \
  include/linux/netdevice.h \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/netpoll/trap.h) \
    $(wildcard include/config/net/dma.h) \
  include/linux/if.h \
  include/linux/hdlc/ioctl.h \
  include/linux/if_ether.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/raeth/skb/recycle/2k.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/imq.h) \
  include/linux/net.h \
  include/linux/random.h \
  include/linux/ioctl.h \
  include/linux/sysctl.h \
    $(wildcard include/config/bcm/nat.h) \
  include/linux/textsearch.h \
  include/linux/err.h \
  include/net/checksum.h \
  include/asm/checksum.h \
  include/linux/in6.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/dma/engine.h) \
  include/linux/if_packet.h \
  include/linux/etherdevice.h \
  include/linux/proc_fs.h \
    $(wildcard include/config/proc/devicetree.h) \
    $(wildcard include/config/proc/kcore.h) \
    $(wildcard include/config/mmu.h) \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/namei.h \
  include/linux/radix-tree.h \
  include/linux/prio_tree.h \
  include/linux/quota.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/magic.h \
  drivers/net/raeth.dsl/../../../../config/autoconf.h \
    $(wildcard include/config/lib/libpthread/force.h) \
    $(wildcard include/config/lib/libnvram/force.h) \
    $(wildcard include/config/lib/libntfs3g/force.h) \
    $(wildcard include/config/lib/libbt/force.h) \
    $(wildcard include/config/lib/readline/force.h) \
    $(wildcard include/config/lib/libpcap/force.h) \
    $(wildcard include/config/lib/ncurse/force.h) \
    $(wildcard include/config/lib/flex/force.h) \
    $(wildcard include/config/lib/libupnp/force.h) \
    $(wildcard include/config/lib/libdlna/force.h) \
    $(wildcard include/config/lib/libffmpeg/force.h) \
    $(wildcard include/config/lib/libgdbm/force.h) \
    $(wildcard include/config/lib/libid3tag/force.h) \
    $(wildcard include/config/lib/pcre/force.h) \
    $(wildcard include/config/lib/zlib/force.h) \
    $(wildcard include/config/lib/libstdc/plus/force.h) \
    $(wildcard include/config/user/hello/world.h) \
    $(wildcard include/config/lib/crypt/force.h) \
    $(wildcard include/config/lib/libm/force.h) \
    $(wildcard include/config/lib/nsl/force.h) \
    $(wildcard include/config/lib/util/force.h) \
    $(wildcard include/config/lib/resolv/force.h) \
    $(wildcard include/config/lib/rt/force.h) \
    $(wildcard include/config/lib/usb0112/force.h) \
    $(wildcard include/config/lib/usb100/force.h) \
    $(wildcard include/config/user/mtdutils.h) \
    $(wildcard include/config/user/mtdutils/erase.h) \
    $(wildcard include/config/user/mtdutils/eraseall.h) \
    $(wildcard include/config/user/mtdutils/ftl/check.h) \
    $(wildcard include/config/user/mtdutils/ftl/format.h) \
    $(wildcard include/config/user/mtdutils/mkfsjffs.h) \
    $(wildcard include/config/user/mtdutils/mkfsjffs2.h) \
    $(wildcard include/config/user/mtdutils/nftldump.h) \
    $(wildcard include/config/user/mtdutils/nftl/format.h) \
    $(wildcard include/config/user/mtdutils/nanddump.h) \
    $(wildcard include/config/user/mtdutils/nandtest.h) \
    $(wildcard include/config/user/mtdutils/nandwrite.h) \
    $(wildcard include/config/user/mtdutils/doc/loadbios.h) \
    $(wildcard include/config/user/mtdutils/doc/loadipl.h) \
    $(wildcard include/config/user/3g.h) \
    $(wildcard include/config/user/brctl/brctl.h) \
    $(wildcard include/config/user/bigpond.h) \
    $(wildcard include/config/user/bt/utils.h) \
    $(wildcard include/config/user/bt/avinfo.h) \
    $(wildcard include/config/user/bt/bccmd.h) \
    $(wildcard include/config/user/bt/ciptool.h) \
    $(wildcard include/config/user/bt/hciconfig.h) \
    $(wildcard include/config/user/bt/hciattach.h) \
    $(wildcard include/config/user/bt/hcisecfilter.h) \
    $(wildcard include/config/user/bt/hcitool.h) \
    $(wildcard include/config/user/bt/l2ping.h) \
    $(wildcard include/config/user/bt/ppporc.h) \
    $(wildcard include/config/user/bt/sdptool.h) \
    $(wildcard include/config/user/ctorrent.h) \
    $(wildcard include/config/user/dhcp6.h) \
    $(wildcard include/config/user/dhcp6/server.h) \
    $(wildcard include/config/user/dhcp6/client.h) \
    $(wildcard include/config/user/dnsmasq.h) \
    $(wildcard include/config/user/disktype.h) \
    $(wildcard include/config/user/echo/server.h) \
    $(wildcard include/config/user/storage.h) \
    $(wildcard include/config/user/proftpd.h) \
    $(wildcard include/config/user/samba.h) \
    $(wildcard include/config/user/goahead/hostname.h) \
    $(wildcard include/config/user/mtdaapd.h) \
    $(wildcard include/config/user/dns/responder.h) \
    $(wildcard include/config/user/goahead/httpd.h) \
    $(wildcard include/config/user/goahead/ipv6.h) \
    $(wildcard include/config/user/goahead/ssl.h) \
    $(wildcard include/config/user/goahead/greenap.h) \
    $(wildcard include/config/user/wizard.h) \
    $(wildcard include/config/user/goahead/lang/en.h) \
    $(wildcard include/config/user/goahead/lang/zhtw.h) \
    $(wildcard include/config/user/goahead/lang/zhcn.h) \
    $(wildcard include/config/user/ralinkiappd.h) \
    $(wildcard include/config/user/igmp/proxy.h) \
    $(wildcard include/config/user/inadyn.h) \
    $(wildcard include/config/user/iptables/iptables.h) \
    $(wildcard include/config/user/iptables/ip6tables.h) \
    $(wildcard include/config/user/ipsec/tools.h) \
    $(wildcard include/config/user/iproute2.h) \
    $(wildcard include/config/user/iproute2/ss.h) \
    $(wildcard include/config/user/iproute2/nstat.h) \
    $(wildcard include/config/user/iproute2/ifstat.h) \
    $(wildcard include/config/user/iproute2/rtacct.h) \
    $(wildcard include/config/user/iproute2/lnstat.h) \
    $(wildcard include/config/user/iproute2/ip.h) \
    $(wildcard include/config/user/iproute2/rtmon.h) \
    $(wildcard include/config/user/iproute2/tc.h) \
    $(wildcard include/config/user/matrixssl.h) \
    $(wildcard include/config/user/miniupnpd.h) \
    $(wildcard include/config/user/mkdosfs.h) \
    $(wildcard include/config/user/ntpclient/ntpclient.h) \
    $(wildcard include/config/user/ntpclient/adjtimex.h) \
    $(wildcard include/config/user/openssl.h) \
    $(wildcard include/config/user/openswan.h) \
    $(wildcard include/config/user/p910nd.h) \
    $(wildcard include/config/user/portmap.h) \
    $(wildcard include/config/user/pppd.h) \
    $(wildcard include/config/user/pppd/with/pppoe.h) \
    $(wildcard include/config/user/pppd/pppstats.h) \
    $(wildcard include/config/user/pppd/pppdump.h) \
    $(wildcard include/config/user/pppd/pppchat.h) \
    $(wildcard include/config/user/rppppoe/relay.h) \
    $(wildcard include/config/user/rppppoe/sniff.h) \
    $(wildcard include/config/user/pppoecd.h) \
    $(wildcard include/config/user/pptp/client.h) \
    $(wildcard include/config/user/zebra.h) \
    $(wildcard include/config/lib/libzebra/force.h) \
    $(wildcard include/config/user/rp/l2tp.h) \
    $(wildcard include/config/user/radvd.h) \
    $(wildcard include/config/user/radvd/dump.h) \
    $(wildcard include/config/user/802/1x.h) \
    $(wildcard include/config/user/rt2561/802/1x.h) \
    $(wildcard include/config/user/snort.h) \
    $(wildcard include/config/user/totd.h) \
    $(wildcard include/config/user/uvc/stream.h) \
    $(wildcard include/config/user/upnp/igd.h) \
    $(wildcard include/config/user/wpa/supplicant.h) \
    $(wildcard include/config/user/wsc.h) \
    $(wildcard include/config/user/wireless/tools.h) \
    $(wildcard include/config/user/wireless/tools/iwconfig.h) \
    $(wildcard include/config/user/wireless/tools/iwgetid.h) \
    $(wildcard include/config/user/wireless/tools/iwlist.h) \
    $(wildcard include/config/user/wireless/tools/iwpriv.h) \
    $(wildcard include/config/user/wireless/tools/iwspy.h) \
    $(wildcard include/config/user/wireless/tools/iwevent.h) \
    $(wildcard include/config/user/busybox/busybox.h) \
    $(wildcard include/config/user/bonnie.h) \
    $(wildcard include/config/user/ethtool.h) \
    $(wildcard include/config/dhrystone.h) \
    $(wildcard include/config/dhcpfwd.h) \
    $(wildcard include/config/user/gdb/gdbreplay.h) \
    $(wildcard include/config/user/gdb/gdbserver.h) \
    $(wildcard include/config/user/ixia/endpoint.h) \
    $(wildcard include/config/user/lmbench3.h) \
    $(wildcard include/config/user/mtd/write.h) \
    $(wildcard include/config/user/mpstat.h) \
    $(wildcard include/config/user/netcat.h) \
    $(wildcard include/config/user/netstat/nat.h) \
    $(wildcard include/config/user/ntfs3g.h) \
    $(wildcard include/config/user/ntfsprogs.h) \
    $(wildcard include/config/user/strace/strace.h) \
    $(wildcard include/config/user/tcpdump.h) \
    $(wildcard include/config/user/pciutil/lspci.h) \
    $(wildcard include/config/user/pciutil/setpci.h) \
    $(wildcard include/config/user/lsusb.h) \
    $(wildcard include/config/user/usbmodeswitch.h) \
    $(wildcard include/config/user/comgt.h) \
    $(wildcard include/config/user/sdparm.h) \
    $(wildcard include/config/user/watchdog.h) \
    $(wildcard include/config/ralinkapp.h) \
    $(wildcard include/config/ralinkapp/ated.h) \
    $(wildcard include/config/ralinkapp/reg/rw.h) \
    $(wildcard include/config/ralinkapp/csr.h) \
    $(wildcard include/config/ralinkapp/flash.h) \
    $(wildcard include/config/ralinkapp/hwnat.h) \
    $(wildcard include/config/ralinkapp/mii/mgr.h) \
    $(wildcard include/config/ralinkapp/nvram.h) \
    $(wildcard include/config/ralinkapp/l2/manage.h) \
    $(wildcard include/config/ralinkapp/gpio.h) \
    $(wildcard include/config/ralinkapp/spi.h) \
    $(wildcard include/config/ralinkapp/i2c.h) \
    $(wildcard include/config/ralinkapp/memory/usage.h) \
    $(wildcard include/config/ralinkapp/swqos.h) \
    $(wildcard include/config/ralinkapp/i2s.h) \
    $(wildcard include/config/ralinkapp/pcm.h) \
    $(wildcard include/config/ralinkapp/cls.h) \
    $(wildcard include/config/ralinkapp/wapi.h) \
    $(wildcard include/config/ralinkapp/scripts.h) \
    $(wildcard include/config/user/lltd.h) \
    $(wildcard include/config/asus/shared.h) \
    $(wildcard include/config/asus/nvram/util.h) \
    $(wildcard include/config/asus/libdisk.h) \
    $(wildcard include/config/asus/netconf.h) \
    $(wildcard include/config/asus/rc.h) \
    $(wildcard include/config/dproxy.h) \
    $(wildcard include/config/asus/infosvr.h) \
    $(wildcard include/config/asus/detectwan.h) \
    $(wildcard include/config/asus/httpd.h) \
    $(wildcard include/config/asus/www.h) \
    $(wildcard include/config/pppd.h) \
    $(wildcard include/config/pppoe/relay.h) \
    $(wildcard include/config/bpalogin.h) \
    $(wildcard include/config/tc.h) \
    $(wildcard include/config/udhcpd.h) \
    $(wildcard include/config/asus/ntpclient.h) \
    $(wildcard include/config/ez/ipupdate.h) \
    $(wildcard include/config/vsftpd.h) \
    $(wildcard include/config/asus/samba.h) \
    $(wildcard include/config/asus/lprng.h) \
    $(wildcard include/config/asus/u2ec.h) \
    $(wildcard include/config/asus/wanduck.h) \
    $(wildcard include/config/asus/network/map.h) \
    $(wildcard include/config/asus/ntfs3g.h) \
    $(wildcard include/config/usb/test.h) \
    $(wildcard include/config/vendor.h) \
    $(wildcard include/config/product.h) \
    $(wildcard include/config/linuxdir.h) \
    $(wildcard include/config/libcdir.h) \
    $(wildcard include/config/language.h) \
  drivers/net/raeth.dsl/ra2882ethreg.h \
    $(wildcard include/config/pseudo/support.h) \
    $(wildcard include/config/ralink/rt2880/shuttle.h) \
    $(wildcard include/config/ralink/rt2880/mp.h) \
    $(wildcard include/config/raeth/napi.h) \
  include/linux/mii.h \
  include/asm/rt2880/rt_mmap.h \
  drivers/net/raeth.dsl/raether.h \
  drivers/net/raeth.dsl/ra_mac.h \
  drivers/net/raeth.dsl/ra_ethtool.h \

drivers/net/raeth.dsl/ra_mac.o: $(deps_drivers/net/raeth.dsl/ra_mac.o)

$(deps_drivers/net/raeth.dsl/ra_mac.o):
