/* This ALG just used by Ralink HNAT */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/in.h>
#include <linux/udp.h>
#include <linux/netfilter.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_helper.h>

MODULE_AUTHOR("Steven <steven_liu@ralinktech.com.tw>");
MODULE_DESCRIPTION("connection tracking pseudo helper");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ip_conntrack_hnat");

#define MAX_PORTS 2
#define HTTP_PORT 80
#define TELNET_PORT 23
static unsigned short ports[MAX_PORTS]={HTTP_PORT,TELNET_PORT};

unsigned int (*nf_nat_hnat_hook)(struct sk_buff **pskb,
				 enum ip_conntrack_info ctinfo,
				 struct nf_conntrack_expect *exp) __read_mostly;
EXPORT_SYMBOL_GPL(nf_nat_hnat_hook);

static int hnat_help(struct sk_buff **pskb,
		     unsigned int protoff,
		     struct nf_conn *ct,
		     enum ip_conntrack_info ctinfo)
{
    return NF_ACCEPT;
}

static struct nf_conntrack_helper hnat[MAX_PORTS][2] __read_mostly;
static char hnat_names[MAX_PORTS][2][sizeof("hnat-65535")] __read_mostly;

static void nf_conntrack_hnat_fini(void)
{
	int i, j;

	for (i = 0; i < MAX_PORTS ; i++) {
		for (j = 0; j < 2; j++)
			nf_conntrack_helper_unregister(&hnat[i][j]);
	}
}

static int __init nf_conntrack_hnat_init(void)
{
	int i, j, ret;
	char *tmpname;

	for (i = 0; i < MAX_PORTS; i++) {
		memset(&hnat[i], 0, sizeof(hnat[i]));

		hnat[i][0].tuple.src.l3num = AF_INET;
		hnat[i][1].tuple.src.l3num = AF_INET6;
		for (j = 0; j < 2; j++) {
			hnat[i][j].tuple.dst.protonum = IPPROTO_TCP;
			hnat[i][j].tuple.src.u.udp.port = htons(ports[i]);
			hnat[i][j].mask.src.l3num = 0xFFFF;
			hnat[i][j].mask.dst.protonum = 0xFF;
			hnat[i][j].mask.src.u.udp.port = htons(0xFFFF);
			hnat[i][j].max_expected = 1;
			hnat[i][j].timeout = 5 * 60; /* 5 minutes */
			hnat[i][j].me = THIS_MODULE;
			hnat[i][j].help = hnat_help;

			tmpname = &hnat_names[i][j][0];
			sprintf(tmpname, "hnat");
			hnat[i][j].name = tmpname;

			ret = nf_conntrack_helper_register(&hnat[i][j]);
			if (ret) {
				printk("nf_ct_hnat: failed to register helper "
				       "for pf: %u port: %u\n",
					hnat[i][j].tuple.src.l3num, ports[i]);
				nf_conntrack_hnat_fini();
				return ret;
			}
		}
	}
	return 0;
}

module_init(nf_conntrack_hnat_init);
module_exit(nf_conntrack_hnat_fini);
