# FluffyGate

FluffyGate (play on WaterGate) was the third project of my Computer Security class (CMPS-122 @ UCSC). This project
was a test of sutdents automation skills, and their ability to scale solutions. 

There were several phases to this project, the final phase, which encapsulated the previous phases, is what is located
in this git repo. The goal of the project was to intercept several (can scale to thousands) of PCAP packets, and depending 
on their use, complete the puzzle.

The steps included

  1. Intercepting and extracting an Initialization Vector from specifically identifiable IP packet.
  2. Intercepting an encrypted password, and brute force decrypting it (through several methods)
  3. Intercepting and unlocking a password protected zip file, that provides an obfuscated key.
  4. Discover the key, to be used on an intercepted encrypted message along with the IV.
  
# Tools I used

I created a mix of scripting and binary files to achieve these steps. I used C to do most of the interception since
it interfaced with the OS, ports, and network stack easier. The PCAP library available in C was instrumental in
achieving a filtering status (https://www.tcpdump.org/pcap.html). Futhermore, I used python to deal with most 
decryption options. There were several different encryption methods we needed to overcome, and python was easiest
to use to whip up a solution to each. These different methods included ROT-K, Caesar-Cipher, and Polybius Square. Python
was additionally used since passing of data between modules became significantly easier instead of using Linux's pipelines.
When I needed to automate I turned to bash scripting, and this was especially helpful when I needed to scale to intercept 
to 60000+ packets I needed to intercept and decrypt in 1 hour. 
