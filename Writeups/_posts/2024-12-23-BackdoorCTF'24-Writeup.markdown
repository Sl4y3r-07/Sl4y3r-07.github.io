---
layout: post
title:  "BackdoorCTF'24 Writeup"
date:   2024-12-23 09:41:40 +0530
tags: ["reversing", "Golang"]
categories: 
---


### Reversing/ GoBlind

#### Description: 
This binary hides its secrets deep within. Can you untangle the layers and reveal the secret?

```File``` : [file](/assets/backdoor/public.zip)

#### Writeup 
- This challenge was written in `GoLang`.
- Executing this binary (without debugger), requires a password to reveal some `super secret` as it says. 
- Giving some random input ( less than 16 chars) says: `Invalid Length`.
- Open the binary in the debugger and there we see a lot of `main_*` functions.
- Start analysing from the `main_main` and first see the valid length of the input is `16`

- After length check, we can see the `input[11]` after some operations is being passed to `sync__ptr_WaitGroup_Add()`.
- Let's reverse the operations being performed on `input[11]`
-     ```res= ((ord(input[11]) >> 2)%10)+2```
- this res value is being passed to the function `sync__ptr_WaitGroup_Add()`, it basically keeps the track of how many go_routines are active or need to be completed.
- so, `res` should be equal to no. of go_routines are need to be executed.
- to determine the number of executing goroutines, observe calls to runtime_newproc() and track the respective wrapper functions whose pointers are being passed.
- In the wrapper functions, functions which is going to be executed within that goroutine.
- To clear the check let's find the possible chars for `input[11]`
- Possible chars - ``` '0' '1' '2' '3' 'X' 'Y' 'Z' '[' ```  -> these 8 possibilities

- Let's look into the first go routine:
- `PxIuE72uAaCXX3HmXchd` this function checks the system year and month.
- Year should be `2004` and hash of the month name should be equal to `0xad018838`
- let's look at the hash function `t5gftiAC`:

{% highlight go %}
      func t5gftiAC(input string) uint64 {
         var hash uint64
         for _, c := range input {
            hash = (hash*0x1003F + uint64(c)) & 0xffffffff
         }
         return hash
      }
{% endhighlight %}

- Month name found is `July`.
- Now, since `July` is the 7th month, so `key[7]='N'` `key[9]='$'` `key[10]='G'`
- In this function itself, there is another function `CWlj47zuCalSy1YWM` that returns bool value.
- This seems to be another check. From here, we can see 3 chars of input involved.
- input[5]=='G' is directly observed. 
- let's look for 13th and 14th character. 
- there's another function called `nOsylsKWBryE2UkXS`, in which we can see `runtime.Callers()`-> that calculates the stack depth.
- ```result =((input[13] ^ input[14])>>1)%14 ```
- Understand how runtime.Callers work, get the value equal to result. 
-   1. runtime.Callers()
    2. nOsylsKWBryE2UkXS()
    3. CWlj47zuCalSy1YWM()
    4. PxIuE72uAaCXX3HmXchd()
    5. .go_exit()

- The value here will be `5`
- Again there can be few possibilities, but to pass the check we can proceed further with any possible combination.

- second goroutine involves checking chars of file name with chars of input.
- exeName[8]==input[8] && exeName[5]==input[12] => `key[8]='e'` and `key[12]='.'`

- 3rd goroutine involves the no. of files in the directory in which `chall.exe` is present and being executed.
- No. of files in the directory should be 5 and thus `key[6]=='o'`

- In 4th goroutine, first 5 chars of the key are passed to the function `HrjZ1eDoZCypd2kqH` and its result is compared to `NGZI1mz`.
- upon taking look at this function, it seems to be some kind of custom base64.
- its encoding goes like this:- 
   1. it converts each char to 8-bit representation 
   2. Xor each 8-bit chunk with 0xAA
   3. padding to 6-bit chunks 
   4. bits are reversed in each 6-bit chunk.
   5. order of 6-bit chunks is reversed.
   6. then mapping each 6-bit chunk to the character in the 64 length buffers (like base64).

- Decoding it, we can get `g0n3!` -> these are first five characters of the key.
- The results from these goroutines are collected via channels. If all are `true`, it proceeds; otherwise, it signs out of Windows.

- Till now we have the key: `g0n3!GoNe$GX.XXX` where X are still unknown or unconfirmed.

- If all results collected are `true` then another function `R7iQNypRBtbrpyaNq1z` is encountered. It looks to have some encrypted buffer. This may be the decryption routine. Let's dive into it and see what does it unfold?
- In function `OnkQsAFZuhclKr2sBDT` encrypted buffer is being passed and this decryption routine looks like `rc6`. But, I changed the hardcoded constants used in the standard algo. `pw=0xB7E12163`  `qw= 0x9E3778D9` keeping rest same.
- There is no need to reverse this function, it will decrypt the buffer with the right key. 
- For right key, we still the need rest 4 characters. 
- Since, all `goroutines` checks are passed [setting the  required system time and year, keeping the required no of files in the same directory], `data.txt` file will be created, but it won't be readable since we don't have right key yet. 
- There is a function that should be reversed in depth is `Z2JEqRKSdxHoRaSCjUshuqZUVD`. 
- This function loads some buffer and then goes through some routine (that should be some sort of decoding routine) and we can see very useful stuff here like: `kernel32.dll` `VirtualAlloc` `CreateThread` this is enough for us to conclude that it must be involving shellcode. 
- 3 chars from the input, `input[5]` `input[13]` `input[15]` are involved in the shellcode.
- that decryption routine invoves first 2 chars of the key. `g` and `0`.


{% highlight python %} 
        for i in range(len(sc)):
            sc[i]= sc[i]-2
            sc[i]= sc[i]^ord('g')
            sc[i]= sc[i]-1
            sc[i]= sc[i]^ord('0')
{% endhighlight %}

- we already know input[5]=`G`, we need to find two chars such that our shellcode is fixed. 
- Find the index where these chars are involved in shellcode.

- For key[15]-> `68 42 65 65 70    push 0x70656542` is the correct instruction. Here `0x68` is corresponding to input[15]
- For key[13]-> `8b 40 78     mov    eax,DWORD PTR [rax+0x78]` Here `0x78` is corresponding to key[13].


![image](/assets/backdoor/offset.png)

- Looking at the shellcode carefully, we can find that there is peb_traversal is involved and it finds function with it. Referring to peb_traversal, we can fix key[13].

- For key[15], we can see before this instruction, stack is being set up and `0x70656542` -> `peeB` :- `Beep` It is pushing this string on the stack. Before this instruction there is `xor rsi rsi; push rsi` that suggests it is pushing a null-byte to the stack, followed by this `peeB` -> `Beep\x00` 

- Resources that can be referred to: 
1. https://www.ired.team/offensive-security/code-injection-process-injection/finding-kernel32-base-and-function-addresses-in-shellcode
2. https://defuse.ca/online-x86-assembler.htm#disassembly 
3. Capstone 

{% highlight python %}
            i=0x78
            i= i^ord('0')
            i= i+1
            i= i^ord('g')
            i= i+2
            print(chr(i)) -> It gives '0' 
{% endhighlight %}

{% highlight python %}    
            i=0x68
            i= i^ord('0')
            i= i+1
            i= i^ord('g')
            i= i+2
            print(chr(i)) -> It gives '@' 
{% endhighlight %}

- Our key is now:- `g0n3!GoNe$GX.0X@`

{% highlight python %}  
    str="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/!@#$%^&*."
       for i in str:
          if 5==((48 ^ord(i))>>1)%14:
             print(i)
{% endhighlight %}

  this is possible options for key[14]:=  [`J` `K` `n` `o` `r` `s`]

- With some permutations: `g0n3!GoNe$G0.0n@` [Correct key]
- Now with the correct key and other challenge requirements, we can get our `data.txt` which contains:

        Good Job, Mate! Thank you decrypting this file-- means a lot to me.
        Your secret is here: "ZmxhZ3tnMF9yMHV0MW4zc180cjNfYzAwbCEkIX0=" 
        Enjoy, happy reversing!

- A `Beep` sound is heard at the end, and binary exits. 

- Flag is: `flag{g0_r0ut1n3s_4r3_c00l!$!}`




