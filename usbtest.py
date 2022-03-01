# -*- coding: utf-8 -*-
import sys
import pywinusb.hid as hid
target_vendor_id = 0x0930
all_hids = hid.find_all_hid_devices()
print(all_hids)
for index, device in enumerate(all_hids):
   print(device.vendor_id + device.product_id)
   index += 1
    