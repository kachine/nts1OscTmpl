# nts1OscTmpl
KORG NTS-1 user oscillator template


## What's this
This project is KORG NTS-1 user oscillator template for developer.
There are no special feature, but many comments to help implementation.


## Prerequisite
[logue-sdk](https://github.com/korginc/logue-sdk)


## How to build
You have to change PLATFORMDIR variable in Makefile, it should correspond to where you installed logue-sdk (something like below).
```Makefile
PLATFORMDIR = $(PATHTO}/logue-sdk/platform/nutekt-digital
```

Then, simply type "make" to build this project.
```sh
$ make
```


---
The names of products or companies are the trademarks or registered trademarks of their respective companies. These companies have no relation to this project.
