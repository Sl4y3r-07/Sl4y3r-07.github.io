---
layout: post
title:  "Reversing LummaC2 Stealer"
date:   2025-01-19 06:21:40 +0530
tags: ["Malware Analysis","reversing"]
categories: Malware Analysis
---

Hi again! In this blog, I have reversed a malware containing `LummaC2 stealer`. 

### Basic Analysis
- First, let's analyse the malware on VirusTotal 
  ![image](../../assets/lumma/2.png)
  ![image](../../assets/lumma/3.png)
  ![image](../../assets/lumma/4.png)

- SHA-256 of the file is: `c5fd4ee7fef2655d5340221a2fe4990d0b744720fdb0b0530599b376c913bf7e`
- The original name of the binary is `Handler.exe` and it's a `.NET executable`
- Looking at the strings, lot of base64 encoded strings are present, it suggests there may be some kind of string obfuscation.

### Static Analysis
- For analysing the malware, I used `dnSpy` as our malware is `.NET executable`.
  ![image](../../assets/lumma/10.png) 

- WinAPIs like `VirtualProtect` and `CallWindowProcA` are present: it suggests the presence of `shellcode`. 
  So, it leverages the `VirtualProtect` call to make the shellcode memory region executable and abuses the `CallWindowProc API` to execute the shellcode.
- `Program.VirtualProtect(ref Program.inputData[0], Program.inputData.Length, 64U, ref num);`
   Taking references from Microsoft docs, third param is `flNewProtect`: memory protection option, and here `64` is being passed that is `PAGE_EXECUTE_READWRITE`. It confirms `inputData` is the shellcode. 
- Let's look at `NormalizeListing`
  ![image](../../assets/lumma/rc4.png) 

- This function implements RC4 encryption algorithm.
- `DataKey` is : `{ 92, 102, 121, 128, 113, 104, 212, 200, 111, 37, 50, 69, 96, 76, 234, 96, 208, 253, 98, 100, 81, 137, 30, 48, 163, 184, 164, 191, 123, 212, 74, 195, 119, 36, 4, 61}` -> The key for RC4 decryption of shellcode
- Similarly, `inputData` (or shellcode) and key for decrypting `sectionContent` are also hardcoded.
 ![image](../../assets/lumma/main.png)

This above code is the part of the `Main` function. It extracts the `binary path` and then checks for `MZ` and `PE` headers for a valid PE file. 
- There is a loop where a string is encoded as `base64` and comapared. 
- From `Program.SectionContent`, it seems program sections names like `.text`, `.reloc`, `.rsrc` and `.idata` are being compared and if the check passes, data is copied of that section. 
- It seems, this is a packed malware, which will be decrypted with RC4 and then executed.
- We can verify this using the debugger. 



### Dynamic Analysis
- Now, let's try to analyse the behaviour of the malware.
- Run the binary in the sandboxed environment only, to avoid any undesirable consequences.
- So, first it checked for `s = ".idata"` (base64 string was `LmlkYXRh`) and then it copied the data from `0xf000` to `0x5bbff` (offsets of the binary), data length is `0x4cc00`
  ![image](../../assets/lumma/11.png)
- Now, reversing `SecureData`, in first call of `NormalizeListing`, `.idata` data is being passed along with the key (length: `0xA`) 
- Key is:- [0xA, 0xB, 0x1, 0x12, 0x0C, 0x57, 0x88, 0x0B, 0xFF, 0x0C]
- In second call of `NormalizeListing`, now `inputData` (shellcode) is being passed along with the key (length: `0x24`) [Key already mentioned in above block]
- Our both data are decrypted with RC4 now and another PE file (packed binary) comes out. Let's try to analyse it.

- Running `floss` on this PE file, gives some interesting results:

{% highlight plaintext %}
    Content-Disposition: form-data; name="
    Content-Type: attachment/x-object
    Content-Disposition: form-data; name="file"; filename="
    hwid
    send_message
    Eact
    file
    Content-Type: multipart/form-data; boundary=
    Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36
    Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36
    \Last Version
    %Program
    pplication\
    advapi32.dll
    SeImpersonatePrivilege
    winc
    json
    http://localhost:
    \Local Extension Settings\
    /Extensions/
    key4.db
    cert9.db
    cookies.sqlite
    logins.json
    formhistory.sqlite
    \key4.db
    ole32.dll
    LOCK
    \Packages
    Thunderb
    \LocalState\Indexed\LiveComm\
    .eml
    Mails/Windows Mail
    DiscordCanary
    DiscordPTB
    \Local State
    os_crypt.encrypted_key
    /dp.txt
    Path: 
    Configuration: 
    Buy now: TG @lummanowork
    Buy&Sell logs: @lummamarketplace_bot
    LummaC2 Build: Jan 15 2025
    LID: 
    %SystemDrive%
    ROOT\CIMV2
    SELECT * FROM Win32_BIOS
    SerialNumber
{% endhighlight %}

- Debugging it in IDA, 
 ![image](../../assets/lumma/17.png) 
*API resolving*

- Here is the function which decrypts `base64` strings (e.g. `Bm1NE4MDSBkzp0zZyP6i4LdYDOjfYRjN+sZUfIYz8lxvACB872I8fFDPLbWkm8zO1TdijA==`) to the domains:
  ![image](../../assets/lumma/16.png) 

- Malicious Domains captured in `Wireshark`:

{% highlight bash %}
   crookedfoshe.bond
   strivehelpeu.bond
   immolatechallen.bond
   growthselec.bond
   jarry-deatile.bond
   pain-temper.bond
   stripedre-lot.bond
   jarry-fixxer.bond
   culltereddirtys.click
{% endhighlight %}

![image](../../assets/lumma/12.png) 
![image](../../assets/lumma/13.png) 
*Setting up and communicating with C2 Server*

- Malicious IP addr: `172.67.131.36` `104.21.3.197` `172.67.199.224`

 ![image](../../assets/lumma/18.png) 
 *`https://steamcommunity.com/profiles/76561199724331900`*
 ![image](../../assets/lumma/19.png) 
 

![image](../../assets/lumma/14.png)
 *Collecting data of Edge Browser*


![image](../../assets/lumma/15.png)
 *Scanning for 'wallet' file*

![image](../../assets/lumma/20.png)
*Scanning for the extensions*

### Conclusion 

- Binary Packing
- Abusing `CallWindowsProcA()` for shellcode execution.
- `PEB traversal and API hashing` 
- Setting up C2 server
- Using `DDR (Dead Drop Resolver)` to hide its C2 details on the steam community website.
- Contacting malicious domains 
- Stealing sensitive files like: `key4.db` `logins.json` `cert9.db` `os_crypt.encrypted_key` and many more.
- It targets Browsers, CryptoWallets, Extensions, Messaging Apps (like Telegram, Discord), Email Clients (like Thunderbird, PMAIL, The BAT!)


![image](../../assets/lumma/tele.png) 
*Telegram channel associated with LummaC2.*


### References 
- https://osandamalith.com/2021/04/01/executing-shellcode-via-callbacks/


