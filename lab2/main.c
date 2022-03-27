#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>

/* tcpdump header (ether.h) defines ETHER_HDRLEN) */
#ifndef ETHER_HDRLEN
#define ETHER_HDRLEN 14
#endif

u_int16_t handle_ethernet(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);
u_int8_t handle_IP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);
u_char *handle_TCP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);
u_char *handle_UDP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

/*
 * Structure of an internet header, naked of options.
 *
 * Stolen from tcpdump source (thanks tcpdump people)
 *
 * We declare ip_len and ip_off to be short, rather than u_short
 * pragmatically since otherwise unsigned comparisons can result
 * against negative integers quite easily, and fail in subtle ways.
 */
struct my_ip
{
    u_int8_t ip_vhl; /* header length, version */
#define IP_V(ip) (((ip)->ip_vhl & 0xf0) >> 4)
#define IP_HL(ip) ((ip)->ip_vhl & 0x0f)
    u_int8_t ip_tos;               /* type of service */
    u_int16_t ip_len;              /* total length */
    u_int16_t ip_id;               /* identification */
    u_int16_t ip_off;              /* fragment offset field */
#define IP_DF 0x4000               /* dont fragment flag */
#define IP_MF 0x2000               /* more fragments flag */
#define IP_OFFMASK 0x1fff          /* mask for fragmenting bits */
    u_int8_t ip_ttl;               /* time to live */
    u_int8_t ip_p;                 /* protocol */
    u_int16_t ip_sum;              /* checksum */
    struct in_addr ip_src, ip_dst; /* source and dest address */
};

/* TCP header
 * Stolen from tcpdump source (thanks tcpdump people) huh
 */
typedef u_int tcp_seq;

struct sniff_tcp
{
    u_short th_sport; /* source port */
    u_short th_dport; /* destination port */
    tcp_seq th_seq;   /* sequence number */
    tcp_seq th_ack;   /* acknowledgement number */
    u_char th_offx2;  /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) > 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
    u_short th_win; /* window */
    u_short th_sum; /* checksum */
    u_short th_urp; /* urgent pointer */
};

//udp报文头
typedef struct mudp{
    u_short udp_srcport;//源端口 两字节
    u_short udp_dstport;//目的端口 两字节
    u_short udp_len; //总长度 两字节
    u_short udp_checksum; //校验和 两字节
}udp_t;

/* looking at ethernet headers */
void my_callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    u_int16_t type = handle_ethernet(args, pkthdr, packet);

    if (type == ETHERTYPE_IP)
    { /* handle IP packet */
        u_int8_t transport_layer_type = handle_IP(args, pkthdr, packet);
        if (transport_layer_type == 0x06)
        {
            handle_TCP(args, pkthdr, packet);
        }
        else if (transport_layer_type == 0x11)
        {
            handle_UDP(args, pkthdr, packet);
        }
    }
    else if (type == ETHERTYPE_ARP)
    { /* handle arp packet */
    }
    else if (type == ETHERTYPE_REVARP)
    { /* handle reverse arp packet */
    }
}

u_char *handle_TCP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    const struct sniff_tcp *tcp;

    tcp = (struct sniff_tcp *)(packet + sizeof(struct ether_header) + sizeof(struct my_ip));

    fprintf(stdout, "TCP: ");
    fprintf(stdout, "src Port: %d ",
            tcp->th_sport);
    fprintf(stdout, "dest Port: %d \n",
            tcp->th_dport);

    return NULL;
}

u_char *handle_UDP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{   
    const struct mudp *udp;

    udp = (struct mudp *)(packet + sizeof(struct ether_header) + sizeof(struct my_ip));

    fprintf(stdout, "UDP: ");
    fprintf(stdout, "src Port: %d ",
            udp->udp_srcport);
    fprintf(stdout, "dest Port: %d \n",
            udp->udp_dstport);

    return NULL;
}

u_int8_t handle_IP(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    const struct my_ip *ip;
    u_int length = pkthdr->len;
    u_int hlen, off, version;
    int i;

    int len;

    /* jump pass the ethernet header */
    ip = (struct my_ip *)(packet + sizeof(struct ether_header));
    length -= sizeof(struct ether_header);

    /* check to see we have a packet of valid length */
    if (length < sizeof(struct my_ip))
    {
        printf("truncated ip %d", length);
        return -1;
    }

    len = ntohs(ip->ip_len);
    hlen = IP_HL(ip);   /* header length */
    version = IP_V(ip); /* ip version */

    /* check version */
    if (version == 4)
    {
        /* check header length */
        if (hlen < 5)
        {
            fprintf(stdout, "bad-hlen %d \n", hlen);
        }

        /* see if we have as much packet as we should */
        if (length < len)
            printf("\ntruncated IP - %d bytes missing\n", len - length);

        /* Check to see if we have the first fragment */
        off = ntohs(ip->ip_off);
        if ((off & 0x1fff) == 0) /* aka no 1's in first 13 bits */
        {                        /* print SOURCE DESTINATION hlen version len offset */
            fprintf(stdout, "IP: ");
            fprintf(stdout, "src IP: %s ",
                    inet_ntoa(ip->ip_src));
            fprintf(stdout, "dest IP: %s \n",
                    inet_ntoa(ip->ip_dst));
        }
        return ip->ip_p;
    }
    else
    {
        fprintf(stdout, "Unknown version %d\n", version);
        return -1;
    }
}

/* handle ethernet packets, much of this code gleaned from
 * print-ether.c from tcpdump source
 */
u_int16_t handle_ethernet(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    u_int caplen = pkthdr->caplen;
    u_int length = pkthdr->len;
    struct ether_header *eptr; /* net/ethernet.h */
    u_short ether_type;

    if (caplen < ETHER_HDRLEN)
    {
        fprintf(stdout, "Packet length less than ethernet header length\n");
        return -1;
    }

    /* lets start with the ether header... */
    eptr = (struct ether_header *)packet;
    ether_type = ntohs(eptr->ether_type);

    /* Lets print SOURCE DEST TYPE LENGTH */
    fprintf(stdout, "ETH: ");
    fprintf(stdout, "src MAC: %s ", ether_ntoa((struct ether_addr *)eptr->ether_shost));
    fprintf(stdout, "dest MAC: %s ", ether_ntoa((struct ether_addr *)eptr->ether_dhost));

    /* check to see if we have an ip packet */
    if (ether_type == ETHERTYPE_IP)
    {
        fprintf(stdout, "(IP)");
    }
    else if (ether_type == ETHERTYPE_ARP)
    {
        fprintf(stdout, "(ARP)");
    }
    else if (eptr->ether_type == ETHERTYPE_REVARP)
    {
        fprintf(stdout, "(RARP)");
    }
    else
    {
        fprintf(stdout, "(?)");
    }
    fprintf(stdout, " %d\n", length);

    return ether_type;
}

int main(int argc, char **argv)
{
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *descr;
    struct bpf_program fp; /* hold compiled program     */
    bpf_u_int32 maskp;     /* subnet mask               */
    bpf_u_int32 netp;      /* ip                        */
    u_char *args = NULL;

    /* Options must be passed in as a string because I am lazy */
    if (argc < 2)
    {
        fprintf(stdout, "Usage: %s numpackets \"options\"\n", argv[0]);
        return 0;
    }

    /* grab a device to peak into... */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        printf("%s\n", errbuf);
        exit(1);
    }

    /* ask pcap for the network address and mask of the device */
    pcap_lookupnet(dev, &netp, &maskp, errbuf);

    /* open device for reading. NOTE: defaulting to
     * promiscuous mode*/
    descr = pcap_open_live(dev, BUFSIZ, 1, -1, errbuf);
    if (descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    if (argc > 2)
    {
        /* Lets try and compile the program.. non-optimized */
        if (pcap_compile(descr, &fp, argv[2], 0, netp) == -1)
        {
            fprintf(stderr, "Error calling pcap_compile\n");
            exit(1);
        }

        /* set the compiled program as the filter */
        if (pcap_setfilter(descr, &fp) == -1)
        {
            fprintf(stderr, "Error setting filter\n");
            exit(1);
        }
    }

    printf("%s\n", argv[1]);

    /* ... and loop */
    pcap_loop(descr, atoi(argv[1]), my_callback, args);

    fprintf(stdout, "\nfinished\n");
    return 0;
}
