#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <string>
#include <ifaddrs.h>
#include <net/if.h>
#include <vector>
#include <pcap.h> //lib that allows packet capturing
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <ctime>


namespace py = pybind11;

std::vector<std::string> get_interfaces() {
    std::vector<std::string> interfaces;
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == 0) { // Gets the list of all the network interfaces
        for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_name && (ifa->ifa_flags & IFF_UP) && !(ifa->ifa_flags & IFF_LOOPBACK)) {
                std::string name(ifa->ifa_name);
                if (std::find(interfaces.begin(), interfaces.end(), name) == interfaces.end()) {
                    interfaces.push_back(name);
                }
            }
        }
        freeifaddrs(ifaddr);
    }else{
        throw std::runtime_error("Failed to get network interfaces");
    }
    return interfaces;
}

py::dict capture_packet(const std::string& interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        throw std::runtime_error(std::string("pcap_open_live failed: ") + errbuf);
    }

    struct pcap_pkthdr header;
    const u_char* packet = pcap_next(handle, &header);
    if (!packet) {
        pcap_close(handle);
        throw std::runtime_error("No packet captured");
    }

    py::dict pkt;

    // Time
    char timebuf[64];
    std::time_t t = header.ts.tv_sec;
    std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    pkt["Time"] = std::string(timebuf);

    // Length
    pkt["Length"] = header.len;

    // Payload (hex)
    std::string payload;
    char buf[4];
    for (u_int i = 0; i < header.caplen; ++i) {
        snprintf(buf, sizeof(buf), "%02x", packet[i]);
        payload += buf;
        if (i != header.caplen - 1) payload += " ";
    }
    pkt["Payload"] = payload;

    // Tenta extrair IPs e protocolo (assumindo Ethernet + IP)
    if (header.caplen > sizeof(ether_header)) {
        const struct ether_header* eth = (struct ether_header*)packet;
        if (ntohs(eth->ether_type) == ETHERTYPE_IP) {
            const struct ip* iph = (struct ip*)(packet + sizeof(ether_header));
            pkt["Source"] = std::string(inet_ntoa(iph->ip_src));
            pkt["Destination"] = std::string(inet_ntoa(iph->ip_dst));
            if (iph->ip_p == IPPROTO_TCP)
                pkt["Protocol"] = "TCP";
            else if (iph->ip_p == IPPROTO_UDP)
                pkt["Protocol"] = "UDP";
            else if (iph->ip_p == IPPROTO_ICMP)
                pkt["Protocol"] = "ICMP";
            else
                pkt["Protocol"] = "Other";
        } else {
            pkt["Source"] = "";
            pkt["Destination"] = "";
            pkt["Protocol"] = "Non-IP";
        }
    } else {
        pkt["Source"] = "";
        pkt["Destination"] = "";
        pkt["Protocol"] = "";
    }

    pcap_close(handle);
    return pkt;
}

py::dict start_sniffing() {
    return capture_packet("wlan0");
}


std::string stop_sniffing() {
    // Aqui você pode parar o sniffing
    return "Sniffing stopped!";
}

PYBIND11_MODULE(sniffer, m) {
    m.def("start_sniffing", &start_sniffing, "Inicia o sniffing");
    m.def("stop_sniffing", &stop_sniffing, "Para o sniffing");
    m.def("get_interfaces", &get_interfaces, "All active network interfaces");
}