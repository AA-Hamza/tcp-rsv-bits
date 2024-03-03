## Description

This is a simple module that flips the reserved bits in the tcp headers to 1s, Note that the received should ignore these fields and the sender shouldn't set them either. However there is no use for these bits. [Further reading](https://en.wikipedia.org/wiki/Transmission_Control_Protocol#TCP_segment_structure)

## Resources

1. [A similar project](https://infosecwriteups.com/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e)
2. [Netfilter wiki](https://www.netfilter.org/documentation/HOWTO/netfilter-hacking-HOWTO-3.html)
   A Packet Traversing the Netfilter System:

```
--->[1]--->[ROUTE]--->[3]--->[4*]--->
             |            ^
             |            |
             |         [ROUTE]
             v            |
            [2]          [5]
             |            ^
             |            |
             v            |


* We are hooking the at very end of the chain
```
