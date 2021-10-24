# Write up for Hack@AC Intra-School CTF

## Crypto

## Forensics

## Misc

## OSINT

## Pwn

## RE

## Scripting

## Steganography

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

### [Kool Kids Klub](http://159.223.66.67:5003/)

This is the main page of the Kool Kids Klub website.

![Screenshot of page](images/koolkids1.png "kool")

On clicking the button, it leads to this. claiming I am a normie.

![Screenshot of page](images/kookids2.png "normie")

The verification seems to be some sort of JWT authentication, as shown by the cookie `auth:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJrb29sIjoiRmFsc2UifQ.vit2I6fcXICDOGMWD_jOA9eOtBPlGVnCA4l0HYcVQ4U` in the client session.

![Screenshot of page](images/koolkids3.png "JWT")

Putting this in [jwt.io](jwt.io), we notice that `kool:False`.

![Screenshot of page](images/koolkids4.png)

To become kool, we simply change it to `kool:True` and `alg:none` (though this was the intended solution, it actually isn't necessary; the shortest solution is just to change `kool:True`), and replace the JWT in the Kool kids website with our new JWT. Reloading the page, we get this:

![Screenshot](images/koolkids5.png)

as well as the flag `ACSI{wh3re_4re_th3_k00l_kid5???}`.

### [Quote](http://159.223.66.67:5001/)

The first thing we see is a quote, as follows:

![Screenshot](images/quote1.png)

One can attempt to endlessly click the button provided, as a means of escape from reality and the cruel fact that one is utterly hopeless at CTFs. Or one can google the quote provided to realize that it is a SingPost quote.

![SingPost](images/quote2.png)

Sending a POST request to the site via [https://reqbin.com/](https://reqbin.com/), we get the flag `ACSI{P0STm4n}`.

![reqbin](images/quote3.png)

### [Red Flag](http://159.223.66.67:5002/)

This one appears to be a file viewer, with many *red flags*. This is the page that greets us on startup:

![Screenshot](images/redflag1.png)

Following the prompt, we can enter `image_1.png` to view the image, a red flag.

![Red Flag](images/redflag2.png)

~~If you haven't noticed any file that has the extension .png will lead to this image lmao~~

Anyway, this is an obvious form of directory traversal, and we can obtain our flag via `../../../../../../../../flag.txt`, which gives us our flag, `ACSI{ed1bl3}`.
