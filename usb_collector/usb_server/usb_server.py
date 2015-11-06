import usb.core
import usb.util
import array
import sys
import struct
import os

def connect():
    # See http://www.beyondlogic.org/usbnutshell/usb1.shtml
    # to learn more about how USB works

    # Vendor and product identifiers for the brick
    ID_VENDOR_LEGO = 0x0694
    ID_PRODUCT_NXT = 0x0002

    # These constants are retrieved from the file
    # ecrobot_usb.edu and they are part of the
    # NXT protocol 

    # Sends a command to the brick and expects a reply
    SYSTEM_COMMAND_REPLY = 0x01
    # Replied command after SYSTEM_COMMAND_REPLY 
    REPLY_COMMAND = 0x02
    # Signals to the brick that the remote is 
    # operating in robot mode
    USB_ECROBOT_MODE = 0xFF
    # Signature from the brick that acknowledges
    # the robo mode
    USB_ECROBOT_SIGNATURE = 'ECROBOT'

    # These constants are define a simple protocol
    # with the USBLoopBack test program

    # Remote wants to disconnect from the brick 
    DISCONNECT_REQ = 0xFF
    # Next bytes belong to the string
    COMM_STRING = 0x01
    # Acknowledgment from USBLoopBack
    ACK_STRING = 0x02
    # find our device
    print ('Seeking for the first NXT block')

    # seek amongst the connected USB devices and picks
    # the first brick
    NXTdevice = None
    for bus in usb.busses():
        for device in bus.devices:
            if device.idVendor == ID_VENDOR_LEGO and device.idProduct == ID_PRODUCT_NXT:

                NXTdevice = device
                break

    # Check if an NXT brick was found
    if NXTdevice is None:
        print ('Device not found')
        return False, None

    # get the first (and only?) configuration for the brick    
    config = NXTdevice.configurations[0]
    # get the the appropriate brick interface
    iface = config.interfaces[0][0]
    # and get the data source/sinks for the brick 
    NXTout, NXTin = iface.endpoints

    # let's open the device
    handle = NXTdevice.open()
    # and get the interface 0 all for us
    handle.claimInterface( 0 )

    # Not sure why this is here.., I do not use Windoz
    # http://code.google.com/p/nxt-python/issues/detail?id=33
    if os.name != 'nt':
        handle.reset()

    # Conform to the protocol to which the funciton ecrobot_process1ms_usb 
    #conforms to, i.e. the one of the LEGO fantom driver
    # Send two bytes: the first one is SYSTEM_COMMAND_REPLY,
    # the second is USB_ECROBOT_MODE.
    data = array.array( 'B', [SYSTEM_COMMAND_REPLY, USB_ECROBOT_MODE] )

    # We will use bulk transfers
    # This type of communication is used to transfer large bursty data.
    # The features of this type of communication are:
    # -Error detection via CRC, with guarantee of delivery.
    # -No guarantee of bandwidth or minimum latency.
    # - Stream Pipe - Unidirectional
    # -Full & high speed modes only.
    handle.bulkWrite( NXTout.address, data )
    
    # read the response from the brick
    data = handle.bulkRead( NXTin.address, len( USB_ECROBOT_SIGNATURE ) + 1 )
    # make sure the response makes sense
    if data[0] != REPLY_COMMAND or data[1:].tostring() != USB_ECROBOT_SIGNATURE:
        print ('Invalid NXT signature (%s)' % (data[1:].tostring(),))
        return False, None;

    return True, lambda size: handle.bulkRead(NXTin.address, size)

def collect_data(read, filename, measure_time):
    start_tick = 0
    tick = 0
    with open("../../motor_data/" + filename, 'w') as my_file:
        print  >> my_file, "Time, Count"
        while tick - start_tick < measure_time:
            data = read(8)
            tick = struct.unpack(">I", data[0:4])[0]
            if start_tick == 0:
                start_tick = tick
            tacho_count = struct.unpack(">i", data[4:8])[0]
            print ("Tick %i, Count %i" % (tick - start_tick, tacho_count))
            print  >> my_file, "%i, %i" % (tick - start_tick, tacho_count)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print ("Ussage python usb_server <filename> <measure_time_ms>")
        sys.exit(1);
    is_conected, read = connect()
    if is_conected:
        collect_data(read, sys.argv[1], int(sys.argv[2]));
