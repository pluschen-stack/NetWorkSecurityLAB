from re import A
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from start_ui import *
from scan import Scanner
from display_result import *


class MyWindow(StartUI, QMainWindow):
    def __init__(self) -> None:
        super(MyWindow, self).__init__()
        self.setupUi(self)
        self.pushButton.pressed.connect(self.get_args)

    def get_args(self):

        self.display = Display()
        try:
            self.scanner = Scanner(self.lineEdit.text(), int(self.lineEdit_2.text()),
                                   int(self.lineEdit_3.text()), int(self.lineEdit_4.text()), self.display.textEdit)
            self.display.show()
            self.scanner.multiScan()

        except Exception as err:
            print(err)


class Display(Ui_MainWindow, QMainWindow):
    def __init__(self) -> None:
        super(Display, self).__init__()
        self.setupUi(self)


def main():
    app = QApplication(sys.argv)
    my_windows = MyWindow()
    my_windows.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
