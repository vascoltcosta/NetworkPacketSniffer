#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <string>
#include <ifaddrs.h>
#include <net/if.h>
#include <vector>

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

std::string start_sniffing() {
    // Aqui você pode chamar sua lógica real de sniffing
    return "Sniffing started!";
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