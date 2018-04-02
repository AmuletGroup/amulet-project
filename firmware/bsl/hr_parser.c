#include "stdio.h"

// Parser for BLE Heart Rate Measurement radio packets
//
// Any values not present in the packet are filled with zeroes in the returned data
// This parser is based on the Bluetooth.org Heart Rate Measurement spec at:
// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml&u=org.bluetooth.characteristic.heart_rate_measurement.xml
// It is assumed that the CRC on the packet has already been checked.
// It is assumed that there are no more than 20 bytes of returned data.  Usually it
// will be less than that.  Caller must supply the 20 byte return data array.
// The function returns the size of the returned data.
//
// The returned data is always:
// uint16_t heartrate;
// uint16_t energyExpended;
// uint16_t RRI1;
// uint16_t RRI2;
// uint16_t RRI3;
// uint16_t RRI4;
// uint16_t RRI5;
// uint16_t RRI6;
// uint16_t RRI7;
// uint16_t Contact;
//   Any values that are zero can be ignored.
//   

// Uncomment to turn on debugging
//#define DEBUG
// Uncomment to turn on detailed debugging messages
//#define DEBUG1
//#define DEBUG2

#ifdef DEBUG 
typedef char uint8_t;
typedef unsigned int uint16_t;
#else
#include "stdlib.h"
#include "stdint.h"
#endif

uint8_t parseHeartRateAndRRI(uint8_t *packet, uint8_t length, uint16_t data[10])
// packet is the raw BLE packet
// length is the length of the BLE packet
// data is the heart rate and RRI values from the packet
// returns the size of the returned data
{
// Define where the HRM data starts in the packet
#define HRM_OFFSET 0
uint8_t hrmValueFormat = 0;
uint8_t sensorContact = 0;
uint8_t energyExpendedStatus = 0;
uint8_t rrIntervalBit = 0;
//uint8_t hrv8=0;
uint16_t hrv16=0;
uint16_t energyExpended=0;
#define NRRI 7
uint16_t RRI[NRRI] = {0, 0, 0, 0, 0, 0, 0}; // I assume there are never more than NRRI values
uint8_t nRRI=0; // Number of RRI values found
uint8_t it=0;
uint8_t dataCount=0; // Number of values returned

#ifdef DEBUG2
   printf("\nDEBUG: ===========================");
   for(it=0; it<length; it++)
   {
      printf("\npacket[%d] = %x (%d)", it, packet[it], packet[it]);
   }
#endif

   // Decode the first byte which contains flags that described how to
   // intepret the rest of the packet.

   hrmValueFormat = (packet[0+HRM_OFFSET]) & 0x01; // Bit 0
   sensorContact = ((packet[0+HRM_OFFSET]) & 0x06)>>1; // Bits 1+2
   energyExpendedStatus = ((packet[0+HRM_OFFSET]) & 0x80)>>7; // Bit 3
   rrIntervalBit = ((packet[0+HRM_OFFSET]) & 0x10)>>1; // Bit 4
#ifdef DEBUG1
   printf("\nDEBUG: flags packet is: %x", packet[0+HRM_OFFSET]);
#endif

   // Decode the HR value

   if(hrmValueFormat == 0)
   {
      // Heart rate value in a UINT8

      hrv16 = packet[1+HRM_OFFSET];
   } else
   {
      // Heart rate value in a UINT16
      // I'm assuming low byte, then high byte in packet.

      hrv16 = (((uint16_t)(packet[2+HRM_OFFSET]))<<8) + packet[1+HRM_OFFSET];
#ifdef DEBUG1
//      printf("\nDEBUG: packet[2] = %d", (packet[2+HRM_OFFSET])<<8);
//      printf("\nDEBUG: packet[1] = %d", packet[1+HRM_OFFSET]);
//      printf("\nDEBUG: hrv16 = %d", hrv16);
#endif
   }

   // Decode the Energy Expended value
   // Don't ask me why the "0xff &" is necessary, but if it is not there then the
   // compiler extends the uppermost bit to all bits to the left.
   
   if(energyExpendedStatus == 1)
   {
      if(hrmValueFormat == 0) // HRV was 8 bit
      {
         //energyExpended = ((((uint16_t)(packet[3+HRM_OFFSET]))<<8)&0xffff) + 0xff&(uint16_t)(packet[2+HRM_OFFSET]);
         energyExpended = (packet[3+HRM_OFFSET]<<8) + (0xff & (packet[2+HRM_OFFSET]));
      } else // HRV was 16 bit
      {
         energyExpended = (packet[4+HRM_OFFSET]<<8) + (0xff & packet[3+HRM_OFFSET]);
      }
   }

   // Decode the RR-Intervals
   // There can be one or more.  Use the packet length to figure out how many
   // there are.  The Flags byte is always present.
   // No need to check the rrIntervalBit, the size tells us what we need to know.

   if(hrmValueFormat == 0)
   {
      // Heart rate value used one byte

      if(energyExpendedStatus == 0)
      {
         // Energy Expended value was not present
         // So only two bytes used and the rest are RRI values

         if((length - 2) >= 2)
         {
            // There's at least one RRI value.

            nRRI = (length-2)/2;
#ifdef DEBUG1
            printf("\nDEBUG: nRRI *** = %d", nRRI);
#endif
            if(nRRI > NRRI)nRRI = NRRI; // Error check
            for(it=0; it<nRRI; it++)
            {
               RRI[it] = (packet[it*2+3+HRM_OFFSET]<<8) + packet[it*2+2+HRM_OFFSET];
            }
         }
      } else
      {
         // Energy Expended value was present and used two bytes
         // So three bytes used and the rest are RRI values

         if((length - 4) >= 2)
         {
            nRRI = (length-4)/2;
            if(nRRI > NRRI)nRRI = NRRI; // Error check
            for(it=0; it<nRRI; it++)
            {
               RRI[it] = (packet[it*2+5+HRM_OFFSET]<<8) + packet[it*2+4+HRM_OFFSET];
            }
         }
      }
   } else
   {
      // Heart rate value used two bytes

      if(energyExpendedStatus == 0)
      {
         // Energy Expended value was not present
         // So three bytes used and the rest are RRI values

         if((length - 3) >= 2)
         {
            nRRI = (length-3)/2;
            if(nRRI > NRRI)nRRI = NRRI; // Error check
            for(it=0; it<nRRI; it++)
            {
               RRI[it] = (packet[it*2+4+HRM_OFFSET]<<8) + packet[it*2+3+HRM_OFFSET];
            }
         }
      } else
      {
         // Energy Expended value was present and used two bytes
         // So five bytes used and the rest are RRI values

         if((length - 5) >= 2)
         {
            nRRI = (length-5)/2;
            if(nRRI > NRRI)nRRI = NRRI; // Error check
            for(it=0; it<nRRI; it++)
            {
               RRI[it] = (packet[it*2+6+HRM_OFFSET]<<8) + packet[it*2+5+HRM_OFFSET];
            }
         }
      }
   }

   // Copy HRV, energyExpended, and RRI into return array.
   // To make decoding easier HRV will always be two bytes, followed by
   // energyExpensed followed by RRI values.  Any RRI values that are zero 
   // should not be used.
   // If energyExpended is always zero it should not be used.

   if(data != NULL) // Error check, don't write to nonexistent storage
   {
      for(it=0; it<10; it++)data[it] = 0; // Zero the return array.

      data[0] = hrv16;
      data[1] = energyExpended;
      for(it=0; it<nRRI; it++)
      {
         data[2+it] = RRI[it];
#ifdef DEBUG1
         printf("\n here RRI[%d] = %x", it, RRI[it]);
#endif
      }
      data[9] = sensorContact;  // Last byte is always sensor contact yes/no bit.
                                 // This assumes we never get that many RRI values
                                 // but seems likely to work.  We'll see what values
                                 // we actually get.

      //dataCount = 2 + nRRI; // Add 2 for HR and energy
      dataCount = nRRI; // Changed to nRRI, more useful to caller of function.
   } // if(data != NULL)...

#ifdef DEBUG1
   printf("\nDEBUG: hrmValueFormat = %d", hrmValueFormat);
   printf("\nDEBUG: sensorContact = %d", sensorContact);
   printf("\nDEBUG: energyExpendedStatus = %d", energyExpendedStatus);
   printf("\nDEBUG: rrIntervalBit = %d", rrIntervalBit);
   printf("\nDEBUG: length = %d", length);
   printf("\nDEBUG: HRV = %d", hrv16);
   printf("\nDEBUG: nRRI = %d", nRRI);
   printf("\nDEBUG: dataCount = %d", dataCount);
#endif

   return dataCount;
}

// Test
#ifdef DEBUG

int main(int argc, char**argv)
{
   // Flags:
   // Bit0: 0=HRV is uint8, 1=HRV is uint16
   // Bit1,2: 00 = Sensor Contact not supported
   //         01 = Sensor Contact not supported
   //         10 = Sensor Contact supported, no contact detected
   //         11 = Sensor Contact supported, contact detected
   // Bit3: 0 = Energy Expended field not present
   //       1 = Energy Expended field is present
   // Bit4: 0 = RRI not present
   //       1 = RRI is present
   // Bit5: reserved
#define HRV8 0x00
#define HRV16 0x01
#define CONTACTNOTSUPPORTED 0x00
#define CONTACTNOTSUPPORTED2 0x02
#define NOCONTACT 0x04
#define CONTACT 0x06
#define NOENERGY 0x00
#define ENERGY 0x80
#define NORRI 0x00
#define RRI 0x10
// 0x3C = 60
#define HEARTRATE8 0x3C
#define HEARTRATE16_LO 0x10
#define HEARTRATE16_HI 0x01
#define RRI_LO 0x10
#define RRI_HI 0x02
#define ENERGY_LO 0xCC
#define ENERGY_HI 0x01

   uint8_t packet1[2] = {HRV8+CONTACT, HEARTRATE8}; // Flags and uint8 HR
   uint8_t packet2[3] = {HRV16+CONTACT, HEARTRATE16_LO, HEARTRATE16_HI}; // Flags and uint16 HR
   uint8_t packet3[4] = {HRV8+RRI+CONTACT, HEARTRATE8, RRI_LO, RRI_HI}; // Flags and uint8 HR and one RRI
   uint8_t packet4[5] = {HRV16+RRI+CONTACT, HEARTRATE16_LO, HEARTRATE16_HI, RRI_LO, RRI_HI}; // Flags and uint16 HR and one RRI
   uint8_t packet5[6] = {HRV8+RRI+CONTACT, HEARTRATE8, RRI_LO, RRI_HI, RRI_LO, RRI_HI}; // Flags and uint8 HR and two RRI
   uint8_t packet6[7] = {HRV16+RRI+CONTACT, HEARTRATE16_LO, HEARTRATE16_HI, RRI_LO, RRI_HI, RRI_LO, RRI_HI}; // Flags and uint16 HR and two RRI
   uint8_t packet7[8] = {HRV8+ENERGY+RRI+CONTACT, HEARTRATE8, ENERGY_LO, ENERGY_HI, RRI_LO, RRI_HI, RRI_LO, RRI_HI}; // Flags and uint8 HR and energy and two RRI
   uint8_t packet8[9] = {HRV16+ENERGY+RRI+NOCONTACT, HEARTRATE16_LO, HEARTRATE16_HI, ENERGY_LO, ENERGY_HI, RRI_LO, RRI_HI, RRI_LO, RRI_HI}; // Flags and uint16 HR and energy and two RRI

   uint16_t data[10];
   uint8_t dataCount;
   uint8_t it;

   printf("\n Running Test 1");
   dataCount = parseHeartRateAndRRI(packet1, sizeof(packet1), data);
   if(dataCount != 2)printf("\nERROR: dataCount = %d instead of %d", dataCount, 2);
   if(data[0] != HEARTRATE8)printf("\nERROR: heart rate = %d instead of %d", data[0], HEARTRATE8);
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   printf("\n Test Done");

   printf("\n\n Running Test 2");
   dataCount = parseHeartRateAndRRI(packet2, sizeof(packet2), data);
   if(dataCount != 2)printf("\nERROR: dataCount = %d instead of %d", dataCount, 2);
   if(data[0] != ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)) )printf("\nERROR: heart rate = %d instead of %d", data[0], ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)));
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   printf("\n Test Done");

   printf("\n\n Running Test 3");
   dataCount = parseHeartRateAndRRI(packet3, sizeof(packet3), data);
   if(dataCount != 3)printf("\nERROR: dataCount = %d instead of %d", dataCount, 3);
   if(data[0] != HEARTRATE8)printf("\nERROR: heart rate = %d instead of %d", data[0], HEARTRATE8);
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   printf("\n Test Done");

   printf("\n\n Running Test 4");
   dataCount = parseHeartRateAndRRI(packet4, sizeof(packet4), data);
   if(dataCount != 3)printf("\nERROR: dataCount = %d instead of %d", dataCount, 3);
   if(data[0] != ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)) )printf("\nERROR: heart rate = %d instead of %d", data[0], ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)));
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   printf("\n Test Done");

   printf("\n\n Running Test 5");
   dataCount = parseHeartRateAndRRI(packet5, sizeof(packet5), data);
   if(dataCount != 4)printf("\nERROR: dataCount = %d instead of %d", dataCount, 4);
   if(data[0] != HEARTRATE8)printf("\nERROR: heart rate = %d instead of %d", data[0], HEARTRATE8);
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   printf("\n Test Done");

   printf("\n\n Running Test 6");
   dataCount = parseHeartRateAndRRI(packet6, sizeof(packet6), data);
   if(dataCount != 4)printf("\nERROR: dataCount = %d instead of %d", dataCount, 4);
   if(data[0] != ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)) )printf("\nERROR: heart rate = %d instead of %d", data[0], ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)));
   if(data[1] != 0x0000)printf("\nERROR: energyExpended = %d instead of zero", data[1]);
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   printf("\n Test Done");

   printf("\n\n Running Test 7");
   dataCount = parseHeartRateAndRRI(packet7, sizeof(packet7), data);
   if(dataCount != 4)printf("\nERROR: dataCount = %d instead of %d", dataCount, 4);
   if(data[0] != HEARTRATE8)printf("\nERROR: heart rate = %d instead of %d", data[0], HEARTRATE8);
   if(data[1] != ((ENERGY_HI << 8) + (ENERGY_LO)) )printf("\nERROR: energyExpended = %d instead of %d", data[1], ((ENERGY_HI << 8) + (ENERGY_LO)));
   if(data[9] != 0x0003)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
   {
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   }
   printf("\n Test Done");

   printf("\n\n Running Test 8");
   dataCount = parseHeartRateAndRRI(packet8, sizeof(packet8), data);
   if(dataCount != 4)printf("\nERROR: dataCount = %d instead of %d", dataCount, 4);
   if(data[0] != ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)) )printf("\nERROR: heart rate = %d instead of %d", data[0], ((HEARTRATE16_HI << 8) + (HEARTRATE16_LO)));
   if(data[1] != ((ENERGY_HI << 8) + (ENERGY_LO)) )printf("\nERROR: energyExpended = %d instead of %d", data[1], ((ENERGY_HI << 8) + (ENERGY_LO)));
   if(data[9] != 0x0002)printf("\nERROR: Contact = %d instead of %d\n", data[9], 0x0003);
   for(it=2; it<dataCount; it++)
   {
      if(data[it] != ((RRI_HI << 8) + (RRI_LO)) )
         printf("\nERROR  RRI data[%d] = %d instead of %d", it, data[it], ((RRI_HI << 8) + (RRI_LO)));
   }
   printf("\n Test Done");
   printf("\n");
   
}
#endif


