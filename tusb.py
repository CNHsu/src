import usb
import usb.core
import usb.util

#dev = usb.core.find(idVendor=0x930, idProduct=0x1408)
#dev=list(usb.core.find(find_all=True))


# find our device
#dev = usb.core.find(idVendor=0x930, idProduct=0x1408)
#dev=usb.core.find(find_all=True)

#dev=usb.core.show_devices(find_all=True)
# was it found?
if dev is None:
    print (ValueError('Device not found'))
print ("#"+dev)