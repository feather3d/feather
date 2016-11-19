#!/bin/python
import sys
import pyvulture 
'''
from PyQt5.QtCore import pyqtSignal, Qt, QDir, QRect, QThread, QTimer
from PyQt5.QtWidgets import QMainWindow, QAbstractScrollArea, QScrollArea, QSlider, QCheckBox, QMenuBar, QMenu, QAction, QDialog, QGroupBox, QStackedWidget, QStackedLayout, QGridLayout, QComboBox, QListWidget, QTabWidget, QLineEdit, QStatusBar, QFrame, QWidget, QPushButton, QLabel, QHBoxLayout, QVBoxLayout, QDockWidget, QFileDialog, QColorDialog, QApplication
from PyQt5.QtGui import QImage, QPalette, QIcon, QPainter, QFont, QColor, QPen, QIntValidator, QDoubleValidator

class MainWindow(QMainWindow):
    
    def __init__(self):
        super().__init__()
        window = pyvulture.QVulkanView()
        self.vbox = QVBoxLayout()
        self.vbox.addWidget(window)
        self.layout().setLayout(vbox)
        #self.setWidth(1200)
        #self.setHeight(600)
        #self.setCentralWidget(window)
        self.setWindowTitle('VulkanQtTest')
        self.show()

    def keyPressEvent(self, e):
        if e.key() == Qt.Key_Escape:
            app.quit()
'''

if __name__ == '__main__':
    window = pyvulture.Window()
    #app = QApplication(sys.argv)
    #ex = MainWindow()
    #window = pyvulture.QVulkanView()
    #sys.exit(app.exec_())
    sys.exit(0)       
