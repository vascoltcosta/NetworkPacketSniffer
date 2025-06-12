#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

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
}