from __future__ import print_function
import subprocess 
from itertools import islice


aws = {
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


def next_n_lines(file_opened, N):
    return [x.strip() for x in islice(file_opened, N)]

with open("true_sort", 'r') as sample:
    while True:
        lines = next_n_lines(sample, 4)
        if lines == []:
            exit()
        else:

            encrypted_pass = subprocess.check_output(['./bin/pcap_open', '-p', lines[0]])
            encrypted_pass = encrypted_pass[:-1].strip(' \t\r\n')
            print(subprocess.check_call(['./bin/run.sh', 'true', 'cur/', aws[encrypted_pass], lines[1], lines[2], lines[3]]))

            # aws[encrypted_pass]