import socket
import threading
import re


class Scanner():

    def __init__(self, ip, startport, endport, threadNumbers=1, output=None) -> None:
        self.ip = ip
        self.startport = startport
        self.endport = endport
        self.threadNumbers = threadNumbers
        self.output = output
        self.threads = []  # 线程池
        self.__check_arg()

    def __check_arg(self):
        if self.__check_ip() and self.endport <= 65535 and self.startport >= 1 and self.startport <= self.endport and self.threadNumbers <= 5 and self.threadNumbers >= 0:
            pass
        else:
            raise Exception('invalid argument')

    def __scan(self, startport, endport):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        for port in range(startport, endport+1):
            try:
                s.connect((self.ip, port))
                print('{0} port {1} is open'.format(self.ip, port))
                if self.output is not None:
                    self.output.insertPlainText(
                        '{0} port {1} is open\n'.format(self.ip, port))
                s.close()
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            except Exception as err:
                print('{0} port {1} is not open'.format(self.ip, port))
                if self.output is not None:
                    self.output.insertPlainText(
                        '{0} port {1} is not open\n'.format(self.ip, port))
        s.close()

    def multiScan(self):
        tempstart = self.startport
        tempInternal = int((self.endport-self.startport)/self.threadNumbers)
        for i in range(self.threadNumbers):
            self.threads.append(threading.Thread(target=self.__scan, args=(
                tempstart, self.endport if (tempstart+tempInternal) > self.endport else (tempstart+tempInternal),)))
            self.threads[i].start()
            tempstart = tempstart+tempInternal+1

    def __check_ip(self):
        compile_ip = re.compile(
            '^(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|[1-9])\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)\.(1\d{2}|2[0-4]\d|25[0-5]|[1-9]\d|\d)$')
        if compile_ip.match(self.ip):
            return True
        else:
            return False

    def close(self):
        for i in self.threads:
            i.cancel()


if __name__ == '__main__':
    scanner = Scanner('127.0.0.1', 440, 447, 2)
    scanner.multiScan()
