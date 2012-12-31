/* Quake3 extension for UDP NAT alteration.
 * (C) 2002 by Filip Sneppe <filip.sneppe@cronos.be>
 * (C) 2005 by Harald Welte <laforge@netfilter.org>
 * based on ip_nat_ftp.c and ip_nat_tftp.c
 *
 * ip_nat_quake3.c v0.0.3 2002-08-31
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *      Module load syntax:
 *      insmod ip_nat_quake3.o ports=port1,port2,...port<MAX_PORTS>
 *
 *      please give the ports of all Quake3 master servers You wish to
 *      connect to. If you don't specify ports, the default will be UDP
 *      port 27950.
 *
 *      Thanks to the Ethereal folks for their analysis of the Quake3 protocol.
 *
 *      Notes: 
 *      - If you're one of those people who would try anything to lower
 *        latency while playing Quake (and who isn't :-) ), you may want to
 *        consider not loading ip_nat_quake3 at all and just MASQUERADE all
 *        outgoing UDP traffic.
 *        This will make ip_conntrack_quake3 add the necessary expectations,
 *        but there will be no overhead for client->server UDP streams. If
 *        ip_nat_quake3 is loaded, quake3_nat_expected will be called per NAT
 *        hook for every packet in the client->server UDP stream.
 *      - Only SNAT/MASQUEARDE targets are useful for ip_nat_quake3.
 *        The IP addresses in the master connection payload (=IP addresses
 *        of Quake servers) have no relation with the master server so
 *        DNAT'ing the master connection to a server should not change the
 *        expected connections.
 *      - Not tested due to lack of equipment:
 *        - multiple Quake3 clients behind one MASQUERADE gateway
 *        - what if Quake3 client is running on router too
 */

#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/udp.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include <linux/netfilter_ipv4/ip_conntrack_quake3.h>
#include <linux/netfilter_ipv4/ip_nat_helper.h>

MODULE_AUTHOR("Filip Sneppe <filip.sneppe@cronos.be>");
MODULE_DESCRIPTION("Netfilter NAT helper for Quake III Arena");
MODULE_LICENSE("GPL");

/* Quake3 master server reply will add > 100 expectations per reply packet; when
   doing lots of printk's, klogd may not be able to read /proc/kmsg fast enough */
#if 0 
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

static unsigned int 
quake3_nat_help(struct ip_conntrack_expect *exp)
{
	struct ip_conntrack *ct = exp->master;

	/* What is this?  Why don't we try to alter the port? -HW */
	exp->tuple.src.ip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
	exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
	exp->expectfn = ip_nat_follow_master;
	//exp->dir = !dir;

	if (ip_conntrack_expect_related(exp) != 0) {
		ip_conntrack_expect_free(exp);
		return NF_DROP;
	}

	return NF_ACCEPT;
}

static void fini(void)
{
	ip_nat_quake3_hook = NULL;
	synchronize_net();
}

static int __init init(void)
{
	BUG_ON(ip_nat_quake3_hook);
	ip_nat_quake3_hook = quake3_nat_help;
	return 0;
}
	
module_init(init);
module_exit(fini);
