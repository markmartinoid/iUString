#ifndef   __IDEVICESLIST_H__
#define   __IDEVICESLIST_H__

//
// dev ID structure
//
typedef struct { 
    USHORT  usVendorID;
    USHORT  usDeviceID;
    PCHAR   szDevice;
} IUSBDEVICESID, *PUSBDEVICESID;


IUSBDEVICESID USBDevicesIDs[] =
{ 
 { 0x13BA,0x0018, "Barcode Scanner M-3100"  }

};


#define	USBDevicesIDs_COUNT	(sizeof(USBDevicesIDs)/sizeof(IUSBDEVICESID))


#endif /* __IDEVICESLIST_H__ */