#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>

/* dump memory data in the hex format */
static void dump_data(const void* data, size_t len)
{
    unsigned int i = 0;
    unsigned int j = 0;
    const unsigned char *pdata = (const unsigned char *)data;
    const int max_ch_of_line = 16;
    for (i = 0; i < len; i += max_ch_of_line) {
        char buf_0x[128] = {0};
        char buf_ch[128] = {0};
        for (j = 0; j < max_ch_of_line; j++) {
            snprintf(buf_0x + strlen(buf_0x), sizeof(buf_0x) - strlen(buf_0x) - 1, 
                "%02x ", (*pdata & 0xff)); /* avoid 0xffffff09 */
            snprintf(buf_ch + strlen(buf_ch), sizeof(buf_ch) - strlen(buf_ch) - 1, 
                "%c", *pdata > 31 && *pdata <= 'z' ? *pdata : '.');
            pdata++;
        }
        printf("0x%x: %s %s\n", (unsigned int)(pdata - 16), buf_0x, buf_ch);
        if (0 != i && i % 512 == 0) {
            printf("\n");
        }
    }
}

static int tun_create(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    if ((fd = open(clonedev, O_RDWR)) < 0) {
        perror("Open /dev/net/tun failed");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;
    if (*dev != '\0') {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name); // return the actual dev name
    printf("Open tun/tap device: %s success...\n", ifr.ifr_name);

    return fd;
}

static void process_arp(char *buf, int nread)
{
    struct ether_header *ether = (struct ether_header *)buf;
    struct arphdr *arph = (struct arphdr *)(ether + 1);
    struct ether_arp *arp = (struct ether_arp *)(arph + 1);
    u_int8_t  ether_self[ETH_ALEN] = {0};

    printf("Please set static arp address!\n");
    printf("# arp -s 10.10.10.1 <tun/tap MAC address>\n");
}

static void process_icmp(char *buf, int offset)
{
    unsigned char ip[4];
    memcpy(ip, &buf[12 + offset], 4);
    memcpy(&buf[12 + offset], &buf[16 + offset], 4);
    memcpy(&buf[16 + offset], ip, 4);
    buf[20 + offset] = 0; // ICMP reply
    *((unsigned short*)&buf[22 + offset]) += 8; // checksum
}

int main(int argc, const char **argv)
{
    int tun_fd, nread, nwrite;
    char tun_name[IFNAMSIZ] = "tun0";
    int flag = 0;
    char buf[1500];

    if (argc > 1) {
        strncpy(tun_name, argv[1], IFNAMSIZ);
    }
    if (strstr(tun_name, "tap")) {
        flag = IFF_TAP;
    } else {
        flag = IFF_TUN;
    }

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *        IFF_NO_PI - Do not provide packet information
     */
    tun_fd = tun_create(tun_name, flag | IFF_NO_PI);

    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }
    printf("TUN name is %s\n", tun_name);

    while (1) {
        nread = read(tun_fd, buf, sizeof(buf));
        if (nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }
        printf("Read %d bytes from tun/tap device\n", nread);
				dump_data(buf, nread);

        if (flag == IFF_TUN) {
            process_icmp(buf, 0);
        } else {
            unsigned short type = htons((unsigned short)buf[12]);
            printf("Ether type: 0x%04x\n", type);
            if (type == ETHERTYPE_ARP) { // ARP, 0x0806
              process_arp(buf, nread);
            } else if (type == 0x0800) { // IP, 0x0800, only support ICMP
              unsigned char mac[6];
              memcpy(mac, buf, 6);
              memcpy(buf, &buf[6], 6);
              memcpy(&buf[6], mac, 6);
              process_icmp(buf, 14); // sizeof(ether_head) == 14
            }
        }
        nwrite = write(tun_fd, buf, nread);
        printf("Write %d bytes to tun/tap device\n", nwrite);
    }
    return 0;
}

