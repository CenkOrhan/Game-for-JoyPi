#ifndef MFRC522_H
#define MFRC522_H

#include <stdint.h>

// MFRC522 Commands
#define PCD_IDLE                0x00
#define PCD_AUTHENT             0x0E
#define PCD_RECEIVE             0x08
#define PCD_TRANSMIT            0x04
#define PCD_TRANSCEIVE          0x0C
#define PCD_RESETPHASE          0x0F
#define PCD_CALCCRC             0x03

// PICC Commands
#define PICC_REQIDL             0x26
#define PICC_REQALL             0x52
#define PICC_ANTICOLL           0x93
#define PICC_SELECTTAG          0x93
#define PICC_AUTHENT1A          0x60
#define PICC_AUTHENT1B          0x61
#define PICC_READ               0x30
#define PICC_WRITE              0xA0
#define PICC_DECREMENT          0xC0
#define PICC_INCREMENT          0xC1
#define PICC_RESTORE            0xC2
#define PICC_TRANSFER           0xB0
#define PICC_HALT               0x50

// Status Codes
#define MI_OK                   0
#define MI_NOTAGERR             1
#define MI_ERR                  2

// MFRC522 Registers
#define CommandReg              0x01
#define ComIEnReg               0x02
#define DivIEnReg               0x03
#define ComIrqReg               0x04
#define DivIrqReg               0x05
#define ErrorReg                0x06
#define Status1Reg              0x07
#define Status2Reg              0x08
#define FIFODataReg             0x09
#define FIFOLevelReg            0x0A
#define WaterLevelReg           0x0B
#define ControlReg              0x0C
#define BitFramingReg           0x0D
#define CollReg                 0x0E
#define TModeReg                0x2A
#define TPrescalerReg           0x2B
#define TReloadRegH             0x2C
#define TReloadRegL             0x2D
#define TxAutoReg               0x15
#define ModeReg                 0x11
#define TxControlReg            0x14
#define CRCResultRegM           0x21
#define CRCResultRegL           0x22

// Function Prototypes
void mfrc522_init(void);
void mfrc522_reset(void);
void mfrc522_write_register(uint8_t reg, uint8_t val);
uint8_t mfrc522_read_register(uint8_t reg);
void mfrc522_set_bit_mask(uint8_t reg, uint8_t mask);
void mfrc522_clear_bit_mask(uint8_t reg, uint8_t mask);
void mfrc522_antenna_on(void);
void mfrc522_antenna_off(void);
uint8_t mfrc522_to_card(uint8_t command, uint8_t *sendData, int sendLen, uint8_t *backData, int *backLen);
uint8_t mfrc522_request(uint8_t reqMode, uint8_t *TagType);
uint8_t mfrc522_anticoll(uint8_t *serNum);
void calculate_crc(uint8_t *data, int length, uint8_t *result);
uint8_t mfrc522_select_tag(uint8_t *serNum);
uint8_t mfrc522_auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum);
void mfrc522_read(uint8_t blockAddr, uint8_t *recvData);
void mfrc522_write(uint8_t blockAddr, uint8_t *writeData);
void mfrc522_halt(void);
void mfrc522_stop_crypto1(void);

#endif /* MFRC522_H */
