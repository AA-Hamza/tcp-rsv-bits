#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

// Network libraries
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/tcp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ahmed Anwar Hamza <ahmed.rk30@gmail.com");
MODULE_DESCRIPTION(
    "A kernel module that flips every tcp packet reserved bits to 1s");
MODULE_VERSION("1.0");

static struct nf_hook_ops *nfho = NULL;

static unsigned int hfunc(void *priv, struct sk_buff *skb,
                          const struct nf_hook_state *state) {
  if (!skb)
    return NF_ACCEPT;

  struct iphdr *ipHeader = ip_hdr(skb);
  if (ipHeader->protocol == IPPROTO_TCP) {
    struct tcphdr *tcpHeader = tcp_hdr(skb);
    tcpHeader->res1 = 0xF;
  }

  return NF_ACCEPT;
}

static int __init tcp_rsv_bits_init(void) {
  printk(KERN_INFO "TCP reserve bits will be flipped now\n");
  nfho = (struct nf_hook_ops *)kmalloc(sizeof(struct nf_hook_ops), GFP_KERNEL);

  /* Initialize netfilter hook */
  nfho->hook = (nf_hookfn *)hfunc;      /* hook function */
  nfho->hooknum = NF_INET_POST_ROUTING; /* received packets */
  nfho->pf = PF_INET;                   /* IPv4 */
  nfho->priority = NF_IP_PRI_FIRST;     /* max hook priority */

  nf_register_net_hook(&init_net, nfho);
  return 0;
}

static void __exit tcp_rsv_bits_exit(void) {
  nf_unregister_net_hook(&init_net, nfho);
  kfree(nfho);
}

module_init(tcp_rsv_bits_init);
module_exit(tcp_rsv_bits_exit);
