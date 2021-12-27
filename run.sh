#!/bin/bash

sudo rmmod usbserial
sudo rmmod ftdi_sio

sudo ~/dev/N64-UNFLoader/UNFLoader/UNFLoader -f 3 -c 0 -s 1 -net -r "gacha64.z64"