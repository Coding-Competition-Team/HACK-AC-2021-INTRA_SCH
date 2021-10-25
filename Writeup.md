# Write up for Hack@AC Intra-School CTF

## Table of contents

[Crypto](#crypto)

[Forensics](#forensics)

[Misc](#misc)

[OSINT](#osint)

[Pwn](#pwn)

[RE](#re)

[Scripting](#scripting)

[Steganography](#steganography)

[Web](#web)

---

## Crypto

### 154

The encoded flag is as follows: `HBJDG5TIM4ZWITCVK5RTCWDJOVGWOUKUIFREEVKII5CEMY2NOJYGG===`

Notice the `===` at the back of the encoded text. This should be immediately recognized as Basex encoding. If you didn't notice this, it's okay, just paste it in Cyberchef and press the magic wand to automatically decode it.

![Cyberchef](images/1541.png)

This gets us the flag, `ACSI{k1nd4_b45ed}`.

---

### Big Mac

A chunk of text is provided, possibly encrypted. From the description, it can be deduced that the Mono-Alphabetic Cipher is being used. To decrypt it, one can just dump the ciphertext provided into a [MAC bruteforcer](https://www.dcode.fr/monoalphabetic-substitution), and scan through the plaintext for the flag, `ACSI{g3tt1ng_t0_m4c5}`.

---

### ezpz

The description tells us to use Cyberchef, so let's be good and do exactly that. Cyberchef's magic wand helpfully identifies Base64 and we get this: `NPFV{1ap3cgvba}`.

![Cyberchef](images/ezpz1.png)

As one can probably tell, it's already in the flag format. Possibly it's been shifted similar to a Caesar cipher or ROTx method, so counting the shift from `ACSI --> NPFV`, we can deduce that it is ROT13-encoded. Thus, using Cyberchef's built-in ROT13 recipe, we can obtain the flag, `ACSI{1nc3ption}`.

![Cyberchef](images/ezpz2.png)

---

### ilovemath

We are given a text file and 7z file. In the text file, the public parameters of a Diffie-Hellman key exchange are given. The 7z fle we have is password encrypted. We can assume that the secret key of the Diffie-Hellman key exchange would be the password of the 7z file.

If you are unfamiliar with Diffie-Hellman key exchange, this [Wikipedia page](https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange) provides a good explanation.

![](images/diffie-hellman.png)

> p= 104711  
> g= 104642  
> g^a= 1975106301245017026503289083852911126512096109792117074551298200678459015168  
> g^b= 1504368685604858602888119532110545611422351872

We need to find out the secret values of a and b, before we can find the secret/shared key. As our modulus is relatively small, we can use discrete logarithm to find a and b easily. To do this, you can use this [online discrete logarithm calculator](https://www.alpertron.com.ar/DILOG.HTM). Alternatively, you can use [SageMath](https://www.sagemath.org/).

![](images/a.jpg)
![](images/b.jpg)

> a: 15  
> b: 9

After this, we can just use the power operator in python to find the secret key.

> pow(g, a*b, p)

![](images/pow.jpg)

We can now key in the password for the 7z file, which opens a text file titled `howdoesxorwork.txt` which contains the following text.

> B@PJxfuf\jp\wkf\njwn~

The title of the text file implies that the text had been encrypted with XOR. As no other clues are given, we can try to brute force. Cyberchef XOR bruteforce can be used in this case.

![](images/xorbruteforce.jpg)

Hence, the flag is revealed. `ACSI{eve_is_the_mitm}`

---

### wEak

This seems to be an RSA challenge, and we are given c, e and n. Since c = m<sup>e</sup> mod n, it also holds true that m = c<sup>1/e</sup> if m<sup>e</sup> < n. In English, this means that you can simply take the third root of c to get m, if m<sup>e</sup> is smaller than n. The challenge seems to hint towards that, what with the capital Es everywhere. Thus, let's try exactly that. However, Python doesn't give us the exact value of large integers, so we will have to use the built-in `Decimal` module in Python. [PyCryptodome](https://pycryptodome.readthedocs.io/en/latest/) is used to convert long to bytes. Below is the script (same directory as `wEak.txt`) to perform the cube-root attack:

```python
from Crypto.Util.number import long_to_bytes, bytes_to_long
from decimal import *

# Specifies the precision
getcontext().prec = 2000

with open('wEak.txt') as file:
  a = file.readlines()

# Parse c from wEak.txt
c = a[0][4:]

# Perform the calculations
m = long_to_bytes(Decimal(c) ** (Decimal(1) / Decimal(3)))

print(m)
```

This gives us the output `b'ACSI{wE4k_Exp0nEn7|'`, which is poorly formatted due to some obscure Python bug (I think). Just fix the vertical bar to a '}' and all should be fine. Flag: `ACSI{wE4k_Exp0nEn7}`

---

## Forensics

### Corrupted PNG

This is a giveaway challenge. Opening the png in a hex editor, we can see that the file signature and IHDR image header is corrupted.

A normal png header would look like this.

![](images/header2.jpg)

The header of the corrupted png we were given looks like this.

![](images/header.jpg)

Manually fix the bytes, and the picture can be viewed.

![](images/headerflag.jpg)

Hence, the flag is `ACSI{headerswillbreakyourhead}`

---

### Ice Cream

We have a pcapng file. Open using Wireshark. We can see that a file called message for ice cream vendor.docx is being transferred over FTP.

![](images/ft1.jpg)

FTP-DATA indicates that it is the packet that contains the transferred document.  
Following the TCP stream, we can view the raw data and save it as a .docx file.

![](images/ft2.jpg)

The message in the docx file gives us a hint to look at the metadata of the file, and that there is a Instagram profile involved.

![](images/igpage.jpg)

And then searching for this name on Instagram gives us a profile with the flag.

![](images/igpage2.jpg)

`ACSI{shark_eats_icecream}`

---

## Misc

### 0_##.zip

This appears to be a zip bomb challenge. We are given a zip file that seems to be recursively zipped. Script to unzip as follows:

```Python
import zipfile
import os
for i in range(1, 100):
  with zipfile.ZipFile('0_'+str(i)+'.zip', 'r') as zip:
    if i != 99:
      zip.extract('0_'+str(i+1)+'.zip')
    else:
      zip.extract('flag.txt')
    os.remove('0_'+str(i-1)+'.zip')

with open('flag.txt') as file:
  a = file.read()

print(a)
```

This yields the flag, `ACSI{wh00sh}`.

---

### Hashcat 100

This is just a simple hashcat challenge. Command for hashcat as follows:

`hashcat.exe -m 0 -a 3 hash.txt ?a?a?a?a?a`

Breakdown: `-m 0` specifies the hash type as MD5. `-a 3` specifies the attack mode to be bruteforce/mask. `hash.txt` loads the hash, depending on where your hash is stored. `?a?a?a?a?a` is the mask that we will use to crack the hash, which tries all lowercase, uppercase, decimal and symbol characters. This yields the plaintext `CraSh`. Wrap this with ACSI{} to get `ACSI{CraSh}`.

---

### Sanity Check

Open a ticket in the discord. Flag: `ACSI{w3lcom3_to_H4CK@AC}`

---

### Spam

This challenge is simple. Just extract the message from the EML file and input it into the decoder on [Spam Mimic](https://www.spammimic.com/index.cgi).  
Most people have not come across this thing before and needed prompting to Google.

Inputting in to decoder will get `ACSI{thisflagisnotspam}`.

---

## OSINT

### Sneaky Part 1

A social media handle is given. Searching for the account on Instagram, we find this account. In one of the posts, we see a boarding pass, with destination WSSS, which is Singapore Changi Airport. Kind of guessy, but flag is `ACSI{Changi}`.

---

### Sneaky Part 2

Now we have to find the postal code of the place of interest. From the captions on his last post, we know that he is trying to find a gym. Now, we can start to mark out the places that he has visited based on his posts. So we have National Art Gallery, Old Hill Street Police Station, St. Andrews Cathedral, Raffles Terrence, and Clark Quay. Now, we can use the sacred circle method to try to define our area of search.

It would look something like this.  
![](images/downtown.jpg)

Drawing circles around the places the John visited, we can see that most of the circles overlap in the area defined by the green circle. This allows us guess that Funan is the place that has a gym.  
Alternatively, we also know that John is a professional rock climber. If we know that there is a rock climbing gym in Funan, we can also guess that the building of interest is Funan.

Funan's postal code is 179105, hence our flag is `ACSI{179105}`.

---

## Pwn

## RE

### Fleck Chag

This is just another flag checker challenge. We know that the flag format is `ACSI{...}`. Let's go through if-statement by if-statement.

`if len(flag) !=  int(flag[8]) * 6:` This tells us that the length of the flag is a multiple of 6. 12 and 18 are feasible lengths, 24 seems a little too long. 6 is just way too short.

`elif flag[4] != '{':` This confirms that the flag format is part of the flag checking service.

`elif flag[0] != flag[-3]:` This tells us that `flag[-3] == 'A'`, which will be useful later on once we can confirm the length.

`elif flag[6] != flag[-8]:` Another wildcard, we can't confirm anything yet.

`elif flag[2].lower() != flag[-5] and flag[13] != flag[11]:` This tells us that `flag[-5] == 's'`. More importantly, it basically confirms that the flag is 18 chars long. Also, `flag[8] == '3'`.

`elif flag.index("f") != 7:` Our first letter given, the 8th character is 'f'. So far, our flag is `ACSI{-*f3-*s-s-A-}`. (* marks characters which are the same but unconfirmed as of yet)

`elif flag[9] != "_" and flag[-6] != "_":` This provides the positions of the 2 underscores in the flag. `ACSI{-*f3_*s_s-A-}`

`elif flag[-2] != 'g':` --> `ACSI{-*f3_*s_s-Ag}`

`elif int(flag[8]) + int(flag[-8]) != 4:` Since we know `int(flag[8]) = 3`, `flag[-8] == flag[6] == '1'`. `ACSI{-1f3_1s_s-Ag}`

`elif flag.index('s') != flag.index('f') + flag.index('{'):` This just serves as confirmation that you are on the right track. Since 11 (index of 1st occurrence of 's') == 7 + 4, we are correct.

Incomplete flag: `ACSI{-1f3_1s_s-Ag}`. No other information is given on the last 2 letters. Since flags should resemble words, the first letter can either be 'l' or 'w'. The second letter can be 'h', 'l', 'n' or 'w'. Sorry for the guessy challenge, but you have a total of 8 different flags to guess.

---

## Scripting

### Random SHA512 Algorithm

We are provided with some numbers as well as a list of 10,000 hashes. The script used to generate the hashes is as follows:

``` Python
import random
import hashlib

h=''
random.seed(3)

for i in range(1, 10000):
    if i == 87:
        h += 'p962353971624266972455319213868077726366609538581374822006171q371530745565212682103475064601401364910938176445787832647171e65537\n'
        break
    h += hashlib.sha512(bytes(random.randint(1, i))).hexdigest()+'\n'
with open("random.txt", "w") as f:
    f.write(h)
```

However, participants are not given this script. Thus, to parse this, they are required to write their own script. An example script that can solve the challenge is as follows:

```Python
import hashlib
import random
random.seed(3)
with open("random_sha512_anomaly.txt") as f:
    lst = f.read().splitlines()
for i in range(1, 10000):
    h = hashlib.sha512(bytes(random.randint(1, i))).hexdigest()
    if str(h) != str(lst[i-1]):
        print(str(h))
        print(lst[i-1])
        break
```

This yields the string `p962353971624266972455319213868077726366609538581374822006171q371530745565212682103475064601401364910938176445787832647171e65537`, which is our anomaly. As the name suggests, RSA might be a key component here, and this is confirmed by the occurrence of p, q and e. Afterwards, it's just simple textbook RSA as we are given p and q already. To get the flag, one might write a script as follows:

```python
from Crypto.Util.number import long_to_bytes

# Initial declaration
c = 165119199922105794768978164576731828113693801657072359161278043838210629526501851742829838893770341337395913834665783424
p = 962353971624266972455319213868077726366609538581374822006171
q = 371530745565212682103475064601401364910938176445787832647171
e = 65537

# Math
n = p*q
t = (p-1) * (q-1)
d = pow(e, -1, t)
m = long_to_bytes(pow(c, d, n))

print(m)
```
This gives us the flag, `Y12021{really_sneaky_asym}`. Note that the flag format is different cos I reused this apprently *really hard* chall that no one solved in our internal CTF... but that shouldn't be an issue.

---

## Steganography

### Mario

We are given a png file with the description, `Mario feels rather insignificant today`. This is a clue that information is hidden in the seemingly normal image by using the LSB technique.

Participants can write their own python script to solve this challenge, ~~or just download a script from github or use an online tool like [this](https://stylesuxx.github.io/steganography/)~~.

---

### Mad World

One of the first things that we can do when we have an audio file for a steganography challenge, we open in sonic visualiser/audacity and view the spectrogram. True enough, we have some text that is encoded in base64. Dump it in Cyberchef and we get this pastebin link. `https://pastebin.com/cXsdQyP9`.

> ++++++++++[>+>+++>+++++++>++++++++++<<<<-]>>>>++++++++++++++++.------------.+.++++++++++.----------.++++++++++.-----.+.+++++..------------.---.+.++++++.-----------.++++++.-----.+++++++++++++++++++.-.+++.---------------.-------.+++++++++++++++++++.-----------.++++++++++.----------.+++++++++++.<-------.

We get some code in Brainf*ck. Throwing it into a decoder, we get this.

>thisisnottheflagbutwhatisit?

It seems like we have to try something else now. The challenge description makes a reference to Xiao steganography. Loading the wav file into the application, we can see that there is indeed a hidden file named `madworldflag.txt`. However, a password is required to successfully extract the file. The previous string that we have, `thisisnottheflagbutwhatisit?`, seems to be the password. However, there seems to be a limit to the length of the password. Hence, we try `thisisnottheflag`, and it is successful.  

The test file reveals more Brainf*ck code. Decoding it, we get our flag, `ACSI{in_between_funny_and_sad}`.

---

## Web

### [Deserted](http://159.223.66.67:5000/)

Upon loading the page, it says `Everyone is gone, it's just you and me...`, as well as a picture of a desert.

![Screenshot of page](images/deserted1.png "Deserted")

This should be a hint to check `robots.txt`, and indeed, these are the contents of `robots.txt`:

```
User-agent: *
Disallow: /
Disallow: /n0-bot5.txt
```

Next, one should attempt to navigate to `n0-bot5.txt`, where the flag, `ACSI{re4lly_l0n3ly...}`, can be found.

---

### [Kool Kids Klub](http://159.223.66.67:5003/)

This is the main page of the Kool Kids Klub website.

![Screenshot of page](images/koolkids1.png "kool")

On clicking the button, it leads to this. claiming I am a normie.

![Screenshot of page](images/koolkids2.png "normie")

The verification seems to be some sort of JWT authentication, as shown by the cookie `auth:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJrb29sIjoiRmFsc2UifQ.vit2I6fcXICDOGMWD_jOA9eOtBPlGVnCA4l0HYcVQ4U` in the client session.

![Screenshot of page](images/koolkids3.png "JWT")

Putting this in [jwt.io](jwt.io), we notice that `kool:False`.

![Screenshot of page](images/koolkids4.png)

To become kool, we simply change it to `kool:True` and `alg:none` (though this was the intended solution, it actually isn't necessary; the shortest solution is just to change `kool:True`), and replace the JWT in the Kool kids website with our new JWT. Reloading the page, we get this:

![Screenshot](images/koolkids5.png)

as well as the flag `ACSI{wh3re_4re_th3_k00l_kid5???}`.

---

### [Quote](http://159.223.66.67:5001/)

The first thing we see is a quote, as follows:

![Screenshot](images/quote1.png)

One can attempt to endlessly click the button provided, as a means of escape from reality and the cruel fact that one is utterly hopeless at CTFs. Or one can google the quote provided to realize that it is a SingPost quote.

![SingPost](images/quote2.png)

Sending a POST request to the site via [https://reqbin.com/](https://reqbin.com/), we get the flag `ACSI{P0STm4n}`.

![reqbin](images/quote3.png)

---

### [Red Flag](http://159.223.66.67:5002/)

This one appears to be a file viewer, with many *red flags*. This is the page that greets us on startup:

![Screenshot](images/redflag1.png)

Following the prompt, we can enter `image_1.png` to view the image, a red flag.

![Red Flag](images/redflag2.png)

~~If you haven't noticed any file that has the extension .png will lead to this image lmao~~

Anyway, this is an obvious form of directory traversal, and we can obtain our flag via `../../../../../../../../flag.txt`, which gives us our flag, `ACSI{ed1bl3}`.

---
