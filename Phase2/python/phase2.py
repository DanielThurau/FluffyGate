from __future__ import print_function
import subprocess 
from itertools import islice
from banking_passwords import send_passwords
import os
import time
import sys

password_dict = {
    "itttVNsx5ybFY":"Qheczo",
    "itxLG/AdRAB9A":"Hfgcnw",
    "itcFvZU546v9E":"d9a0us",
    "arWQuYU.HtnyM":"yHMasC",
    "ssXUT7bh1v4uU":"1Bfyzo",
    "pa0TqKk5ohfOQ":"PbUi0s",
    "arWQuYU.HtnyM":"yHMasC",
    "pajniGrnH2oEU":"Yw0sRW",
    "epH.EL7UdlgQU":"ipFCNi",
    "sshxnTdg.XhMI":"KmXcsC",
    "epH.EL7UdlgQU":"ipFCNi",
    "pazDXsLp6GpbI":"c6QhWt",
    "ara4TDwHrtYYc":"yH2w8p",
    "itsR8j9Lw9L5E":"PbecYw",
    "paYgtvUHYt/xg":"At4hLs",
    "pazDXsLp6GpbI":"c6QhWt",
    "toax4uLm8EUEY":"MaPb6t",
    "it9Y.GraxsPW2":"Pbd9Va",
    "ssJztlytKnU62":"NiusKm",
    "arjojbIeZ18E.":"ChFCgc",
    "toyNMygh9aEuY":"2wQhFC",
    "ss.4RGe9jDAJM":"hC0s1B",
    "ss1sixb4OPZG.":"ecLsRW",
    "to1HSc.LWKFHA":"d9Xcb3",
    "epJAnFIiYhHvw":"d91BMa",
    "tov4hvYdzFB/U":"Xc8ptI",
    "topDKsjFITo4g":"6tLsje",
    "itw/CvM787JGg":"Ea8pQh",
    "arzcmLU1XzE.g":"1BVaSt",
    "itWIQLQFRiyGU":"zoQhPb",
    "arQ4Wdof01vZ6":"BBTfqT",
    "arR1obMP2W6xI":"b3Gs5a",
    "pazsSnxoCOAVk":"RWYwrh",
    "toUvCAEN8vtAI":"rhnwZb",
    "paSuKeHLiatu.":"FCgcxC",
    "arjRIcrCUQdjQ":"PbImJa",
    "ssXzmL9XzAAF2":"pbZbzo",
    "itAgPDENcahTQ":"wAWtKm",
    "ssXzmL9XzAAF2":"pbZbzo",
    "toUvCAEN8vtAI":"rhnwZb",
    "to1HSc.LWKFHA":"d9Xcb3",
    "ssKze9pCHOqCs":"ltsCb3",
    "artS8rLv4waJM":"kdyHTf",
    "itcFvZU546v9E":"d9a0us",
    "ss.4RGe9jDAJM":"hC0s1B",
    "ar0rnIkj4bR2o":"HfNiNi",
    "itx..1t3Ya7aA":"Stc6sC",
    "toFZXxjeZX4M.":"UiltQh",
    "arsuXJ5DgG/vg":"Ni3B2w",
    "arwzV5XXzEuDw":"d9EasC",
    "ssdFfnbAeeMx2":"tIGsyH",
    "ep3VuTPzd1iRg":"JaOmGs",
    "arBgcOzBGcYQQ":"Atipmi",
    "ssKMO2nDYHOCk":"ImQh9k",
    "ssYIa0TOeeCJo":"Hf5aXc",
    "arFRUlrfaFvNA":"6t1B5a",
    "ssfOSEXGSCgaE":"HfsCsC",
    "arxJXj7xiTs32":"9kd9Hf",
    "itYvu/LqgsocM":"ZbTfje",
    "epQ77SY3PencM":"Cha01B",
    "ep7TB1c/.JWx6":"rhyHva",
    "arcFdMBo8/YZw":"OmkdDa",
    "itBmIDhpi28TI":"WtjeIm",
    "arfGO7GkxVku2":"ZbvahC",
    "paUr6ut304RAo":"PbhCEa",
}

def populate_passwords(dictionary, path_to_cracked):
    with open(path_to_cracked, "r") as f:
        for line in f:
            line = line.split(" ")
            dictionary[line[0].strip(' \t\r\n')] = line[1].strip(' \t\r\n')

def next_n_lines(file_opened, N):
    return [x.strip() for x in islice(file_opened, N)]

populate_passwords(password_dict, "crackerjacks/cracked_passwords.txt")


if __name__ == '__main__':
    populate_passwords(password_dict, "crackerjacks/cracked_passwords.txt")
    data_dir = "data/P2/T5/"
    files = os.listdir(data_dir)

    files = sorted(files, key=lambda e : int(filter(str.isdigit, e)))
    to_be_cracked = []
    for p in xrange(0,len(files), 6):
        e_pass = subprocess.check_output(['./bin/pcap_open', '-u', data_dir + files[p]]).strip(' \t\r\n')
        if e_pass not in password_dict:
            to_be_cracked.append(e_pass)

    print(to_be_cracked)
    send_passwords(to_be_cracked)
    for i in to_be_cracked:
        while(i not in password_dict):
            print("sleeping for 60")
            time.sleep(60)
            populate_passwords(password_dict, "crackerjacks/cracked_passwords.txt")

    print("All my passwords are there")

    counter = int(sys.argv[1])
    # #https://stackoverflow.com/questions/3415072/pythonic-way-to-iterate-over-sequence-4-items-at-a-time
    for p,k,i,c1,c2,c3 in zip(*[iter(files)]*6):
        if counter == 0:
            break
        e_pass = subprocess.check_output(['./bin/pcap_open', '-u', data_dir+p]).strip(' \t\r\n')
        # execute_string = "./bin/run.sh true cur/ " + password_dict[e_pass] + " " + data_dir+k+ " " + data_dir+i + " " + data_dir+c1
        # print(execute_string)
        print(subprocess.check_call(['./bin/run.sh', 'true', 'cur/', password_dict[e_pass], data_dir+k,\
                                                                     data_dir+i, data_dir+c1, "rot_k"]))
        print(subprocess.check_call(['./bin/run.sh', 'true', 'cur/', password_dict[e_pass], data_dir+k,\
                                                                     data_dir+i, data_dir+c2, "rot_k_perm"]))
        # print(subprocess.check_call(['./bin/run.sh', 'true', 'cur/', password_dict[e_pass], data_dir+k,\
                                                                     # data_dir+i, data_dir+c3]))
        counter = counter - 1