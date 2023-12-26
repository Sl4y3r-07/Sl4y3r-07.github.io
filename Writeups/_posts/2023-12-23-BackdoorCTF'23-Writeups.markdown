---
layout: post
title:  "BackdoorCTF'23 Writeups"
date:   2023-12-23 09:41:40 +0530
tags: ["reversing","Forensics"]
categories: 
---
I'm sharing writeups for the challenges I  created for Backdoor CTF'23, hosted by InfoSecIITR. Here they are :-

### Forensics/Headache


 ```Description:-```
I've had a headache since last evening. Is there a magic spell that can cure it immediately?

 ```Challenge File:```  [file](/assets/backdoor/chal.png)

This challenge was beginner level forensics challenge. A png image is given as the challenge file, which seems to be corrupted. As the name of the chall suggests, we should check the header of the png. Using hexedit, first 8 bytes seems incorrect.

![header](/assets/backdoor/ss.png)

Edit correct header bytes ```89 50 4E 47 0D 0A 1A 0A``` and save the file, that reveals the flag.

![flag](/assets/backdoor/flag.png)

That was 's' not '8', I am sorry, it was bit cursive :)

Flag:- ```flag{sp3ll_15_89_50_4E_47}```

### Forensics/Drunk

 ```Description:-``` 
I was offered a famous Italian drink along with a message, but it affected me in such a way that I couldn't decipher it. Need your help and please don't make excuse if you can't.

 ```Challenge File:``` [file1](/assets/backdoor/encoded.bin) [file2](/assets/backdoor/something.txt)

In this challenge, ```something.txt``` and ```encoded.bin``` are provided. In something.txt file, there seems to be some type of encoding, so to figure out this, I use cyberchef. It is octal encoding followed by hex and then base64 encoded. We get ```zlMg5K3TobbFh_8l7doDT_408rH7Md_W3Oc1yKX1FrA=```. 

Now, the real challenge starts to figure out what ```encoded.bin``` is all about. In encoded.bin, data seems to be encrypted. The string we got from something.txt may be the key to decrypt it. Let's try to get the encryption method. From the description, ```Italian Drink```. Try to google about Italian drinks and some cryptographic encryption methods, ```Fernet``` will be the result (Fernet-Branca is an Italian armaro). Though you have to google it hard. Now, it becomes super easy to decrypt the data. This is the following script I used to decrypt the data.

{% highlight python %}
 from cryptography.fernet import Fernet
 from PIL import Image
 import io
 def decrypt_image(encrypted_image, key):
    
    with open(encrypted_image, 'rb') as file:
        encrypted_data = file.read()
    
    f = Fernet(key)
    decrypted_data = f.decrypt(encrypted_data)

    decrypted_image = Image.open(io.BytesIO(decrypted_data))
    decrypted_image.save('decrypted_image.png')
    
 if __name__ == "__main__":
    encrypted_image = "encoded.bin" 
    encryption_key ='zlMg5K3TobbFh_8l7doDT_408rH7Md_W3Oc1yKX1FrA='
    decrypt_image(encrypted_image, encryption_key)

{% endhighlight %}

Though this challenge involves cryptographic part but forensics is incomplete without cryptographic as all encryption methods use it. Fernet is one of the most common encryption methods used in forensics.

![flag](/assets/backdoor/f.png)

Flag:- ```flag{Th1s_time_n0t_cURs1v3}```


### Rev/Open Sesame


 ```Description:-``` Whisper the phrase, unveil with 'Open Sesame'.  

 ```Challenge File:``` [file](/assests/backdoor/open_sesame.apk)
 
 
An apk file is provided as the challenge file. First open this apk in  ```JADX``` tool to see its source code. After opening the apk in Jadx, navigate to ```com.example.open_sesame.MainActivity```. This app requires UserID and Key, and if they are invalid, ```Invalid credentials. Please try again."``` will be displayed, but valid UserID and key will not display the flag. For this, understanding the code logic is must. We have few functions here : ```n4ut1lus, sh4dy, it4chi, sl4y3r and flag```.The  ```valid_password``` is ```4l1baba``` (from the given ascii codes), and ```AppCompatDelegate.FEATURE_SUPPORT_ACTION_BAR``` has a constant value 108. Now, ```it4chi``` function extracts digits from the password (4,1), ```sh4dy``` function appends the digits and return as string (41) and ```sl4y3r``` function subtracts 1 from the number (40). Now, in ```flag``` function, ```U|]rURuoU^PoR_FDMo@X]uBUg``` is xored with ```40```. This will give us the flag.
 
 ![open](/assets/backdoor/open.png)


 

Flag:- ```flag{aLiBabA_and_forty_thiEveS}```

### Rev/Sl4ydroid


 ```Description:-``` This challenge has got some Rizz !!

 ```Challenge File:``` [file](/assests/backdoor/Sl4ydroid.apk)

This challenge is based on dynamic debugging of Android Application. Open the app on JADX, and naviagte to ```com.backdoor.sl4ydroid.MainActivity``` to see its source code. This app loads a native library named ```sl4ydroid``` using ```System.loadLibrary()```. The class declares several native methods ```damn, k2, kim, nim``` that are implemented in native code, not in Java. These native methods are invoked within ```onCreate``` method of ```MainActivity``` class with strings as parameters which are retrieved from the string resource using identifiers, like ```(R.string.k1)```.
   ![sl4y](/assets/backdoor/sl4y.png)

So, the implementation of these methods are in native library. Extract the library by either unzipping it or using ```apktool```. So ```libsl4ydroid.so``` will be extracted in lib/ folder. Open it in IDA or any other debugging software (I personally prefer IDA) and try to reverse the code. To get the flag, there are more than one approaches that can be followed. I'll discuss few of them here. First, flag can be found by ```static analysis``` but that can be bit complex and time consuming process. If we look at the pseudo code, we can clearly see that it involves a ```RC4``` function: ```ring```. So this hints, we should debug it dynamically. To do dyanmic analysis, we have to set up ```Remote Android Debugger```. For this, either use emulator or your own android device (provided developer mode is on). Through ```adb```, install the apk and start android server (There are many blogs explaining this process, though it is applicable for IDA pro). After successfully setting up the debugger, attach the process (of this apk) and debug the code dynamically now. 

This process seems bit lengthy and sometimes it can be bit uncertain (as I have gone through it often:) ). Interesting thing to notice in the code is ```System.out.println(message)``` under the functions ```sh4dy, sl4y3r, it4chi and n4ut1lus```. It might be possible that these methods are being called from native methods using JNI (can be viewed in IDA). Let's try to log these results. For this, we use JADX debugger, launch the app on emulator or own device and click ```Launch App```. In ```logcat```, we have our flag. This challenge has surely got some Rizz :)
 ![adb](/assets/backdoor/adb.png)
 
 ![flag](/assets/backdoor/jadx.png)


Flag:- ```flag{RizZZ! Rc4_R3v3r51Ngg_RrR!:}```



Hope !! You all enjoyed the challenges. 

