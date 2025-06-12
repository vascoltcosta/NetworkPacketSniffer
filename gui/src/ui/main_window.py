import sys
import os
sys.path.append(os.path.expanduser('~/Documents/Projects/NetworkPacketSniffer/sniffer/build'))
import sniffer

from PyQt5.QtWidgets import QMainWindow, QPushButton, QLabel, QHBoxLayout, QVBoxLayout, QWidget

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Network Packet Sniffer")
        self.setGeometry(200, 200, 1000, 800)

        self.initUI()

    def initUI(self):
        vlayout = QVBoxLayout() #Creating a global vertical layout 
        hlayout = QHBoxLayout()
        vlayout.addLayout(hlayout)

        self.label = QLabel("Welcome to the Network Packet Sniffer", self)
        vlayout.addWidget(self.label)

        self.stopsniffbutton = QPushButton("Stop Sniffing", self)
        self.stopsniffbutton.clicked.connect(self.on_stopsniffbutton_click)
        hlayout.addWidget(self.stopsniffbutton)

        self.startsniffbutton = QPushButton("Start Sniffing", self)
        self.startsniffbutton.clicked.connect(self.on_startsniffbutton_click)
        hlayout.addWidget(self.startsniffbutton)

        container = QWidget()
        container.setLayout(vlayout)
        container.setLayout(hlayout)
        self.setCentralWidget(container)


    def on_startsniffbutton_click(self):
        result = sniffer.start_sniffing()
        self.label.setText(result)

    def on_stopsniffbutton_click(self):
        result = sniffer.stop_sniffing()
        self.label.setText(result)