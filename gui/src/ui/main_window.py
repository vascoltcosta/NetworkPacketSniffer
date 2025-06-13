import sys
import os
sys.path.append(os.path.expanduser('~/Documents/Projects/NetworkPacketSniffer/sniffer/build'))
import sniffer

from PyQt5.QtWidgets import QTableWidgetItem,QHeaderView, QTableWidget ,QMainWindow,QComboBox, QPushButton, QLabel, QHBoxLayout, QVBoxLayout, QWidget

class MainWindow(QMainWindow):
    def __init__(self):
        
        super().__init__()

        self.setWindowTitle("Network Packet Sniffer")
        self.setGeometry(200, 200, 1000, 800)

        self.startsniffingcounter = False

        self.initUI()

    def initUI(self):
        vlayout = QVBoxLayout() #Creating a global vertical layout 
        hlayout = QHBoxLayout()
        vlayout.addLayout(hlayout)

        #Horizontal Header
        self.packet_table = QTableWidget(self)
        self.packet_table.setColumnCount(6)
        self.packet_table.setHorizontalHeaderLabels(["Time", "Source", "Destination", "Protocol", "Length", "Payload"])
        self.packet_table.horizontalHeader()
        for c in range(self.packet_table.columnCount()):
            self.packet_table.horizontalHeader().resizeSection(c, 150)  # Defines the starting width of each column
        self.packet_table.horizontalHeader().setStretchLastSection(True)
        vlayout.addWidget(self.packet_table)

        # Combo box with all the Network Interfaces
        self.interface_combo = QComboBox(self)
        self.get_interfaces()
        hlayout.addWidget(self.interface_combo)

        self.sniffbutton = QPushButton("Start Sniffing", self)
        self.sniffbutton.clicked.connect(self.on_sniffbutton_click)
        hlayout.addWidget(self.sniffbutton)

        container = QWidget()
        vlayout.addLayoutz(hlayout)
        container.setLayout(vlayout)
        self.setCentralWidget(container)


    def get_interfaces(self):
        interfaces = sniffer.get_interfaces()
        self.interface_combo.clear()
        self.interface_combo.addItems(interfaces)
        if interfaces:
            self.interface_combo.setCurrentIndex(0)
        else:
            self.label.setText("No network interfaces found.")

    def on_sniffbutton_click(self):
        self.startsniffingcounter = not self.startsniffingcounter
        if self.startsniffingcounter:
            self.sniffbutton.setText("Stop Sniffing")
            self.sniffbutton.setStyleSheet("background-color: green")
            pkt = sniffer.start_sniffing()
            row = self.packet_table.rowCount()
            self.packet_table.insertRow(row)
            self.packet_table.setItem(row, 0, QTableWidgetItem(str(pkt.get("Time", ""))))
            self.packet_table.setItem(row, 1, QTableWidgetItem(str(pkt.get("Source", ""))))
            self.packet_table.setItem(row, 2, QTableWidgetItem(str(pkt.get("Destination", ""))))
            self.packet_table.setItem(row, 3, QTableWidgetItem(str(pkt.get("Protocol", ""))))
            self.packet_table.setItem(row, 4, QTableWidgetItem(str(pkt.get("Length", ""))))
            self.packet_table.setItem(row, 5, QTableWidgetItem(str(pkt.get("Payload", ""))))

        else:
            self.sniffbutton.setText("Start Sniffing")
            self.sniffbutton.setStyleSheet("background-color: red")