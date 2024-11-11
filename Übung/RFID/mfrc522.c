#include "mfrc522.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdlib.h>
#include <string.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000
// Constants for the MFRC522
#define MFRC522_MAX_LEN 16

void mfrc522_init(void) {
    wiringPiSetup();
    wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    mfrc522_reset();
    mfrc522_write_register(TModeReg, 0x8D);
    mfrc522_write_register(TPrescalerReg, 0x3E);
    mfrc522_write_register(TReloadRegL, 30);
    mfrc522_write_register(TReloadRegH, 0);
    mfrc522_write_register(TxAutoReg, 0x40);
    mfrc522_write_register(ModeReg, 0x3D);
    mfrc522_antenna_on();
}

void mfrc522_reset(void) {
    mfrc522_write_register(CommandReg, PCD_RESETPHASE);
}

void mfrc522_write_register(uint8_t reg, uint8_t val) {
    uint8_t data[] = { (reg << 1) & 0x7E, val };
    wiringPiSPIDataRW(SPI_CHANNEL, data, 2);
}

uint8_t mfrc522_read_register(uint8_t reg) {
    uint8_t data[] = { ((reg << 1) & 0x7E) | 0x80, 0 };
    wiringPiSPIDataRW(SPI_CHANNEL, data, 2);
    return data[1];
}

void mfrc522_set_bit_mask(uint8_t reg, uint8_t mask) {
    uint8_t tmp = mfrc522_read_register(reg);
    mfrc522_write_register(reg, tmp | mask);
}

void mfrc522_clear_bit_mask(uint8_t reg, uint8_t mask) {
    uint8_t tmp = mfrc522_read_register(reg);
    mfrc522_write_register(reg, tmp & (~mask));
}

void mfrc522_antenna_on(void) {
    uint8_t value = mfrc522_read_register(TxControlReg);
    if ((value & 0x03) != 0x03) {
        mfrc522_set_bit_mask(TxControlReg, 0x03);
    }
}

void mfrc522_antenna_off(void) {
    mfrc522_clear_bit_mask(TxControlReg, 0x03);
}

uint8_t mfrc522_to_card(uint8_t command, uint8_t *sendData, int sendLen, uint8_t *backData, int *backLen) {
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    int i = 2000;

    if (command == PCD_AUTHENT) {
        irqEn = 0x12;
        waitIRq = 0x10;
    } else if (command == PCD_TRANSCEIVE) {
        irqEn = 0x77;
        waitIRq = 0x30;
    }

    mfrc522_write_register(ComIEnReg, irqEn | 0x80);
    mfrc522_clear_bit_mask(ComIrqReg, 0x80);
    mfrc522_set_bit_mask(FIFOLevelReg, 0x80);

    mfrc522_write_register(CommandReg, PCD_IDLE);

    // Writing data to the FIFO
    for (int j = 0; j < sendLen; j++) {
        mfrc522_write_register(FIFODataReg, sendData[j]);
    }

    // Execute the command
    mfrc522_write_register(CommandReg, command);
    if (command == PCD_TRANSCEIVE) {
        mfrc522_set_bit_mask(BitFramingReg, 0x80);
    }

    // Waiting for completion of command execution
    while (i--) {
        n = mfrc522_read_register(ComIrqReg);
        if (n & waitIRq) {
            break;
        }
        if (n & 0x01) { // Error
            status = MI_ERR;
            break;
        }
    }

    mfrc522_clear_bit_mask(BitFramingReg, 0x80);

    if (i != 0) {
        if ((mfrc522_read_register(ErrorReg) & 0x1B) == 0) { // BufferOvfl CollErr CRCErr ProtocolErr
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }

            if (command == PCD_TRANSCEIVE) {
                n = mfrc522_read_register(FIFOLevelReg);
                lastBits = mfrc522_read_register(ControlReg) & 0x07;
                if (lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }

                if (n == 0) {
                    n = 1;
                }
                if (n > MFRC522_MAX_LEN) {
                    n = MFRC522_MAX_LEN;
                }

                // Reading the received data from FIFO
                for (int k = 0; k < n; k++) {
                    backData[k] = mfrc522_read_register(FIFODataReg);
                }
            }
        } else {
            status = MI_ERR;
        }
    }

    return status;
}

// Requests RFID card presence
uint8_t mfrc522_request(uint8_t reqMode, uint8_t *TagType) {
    uint8_t status;
    int backBits; // Received data bits

    mfrc522_write_register(BitFramingReg, 0x07);  // TxLastBits = BitFramingReg[2..0]

    TagType[0] = reqMode;
    status = mfrc522_to_card(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

    if ((status != MI_OK) || (backBits != 0x10)) {
        status = MI_ERR;
    }

    return status;
}

// Anti-collision, return card serial number 4 bytes
uint8_t mfrc522_anticoll(uint8_t *serNum) {
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck = 0;
    int serNumLength = 5;

    mfrc522_write_register(BitFramingReg, 0x00);  // TxLastBits = 0

    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    
    status = mfrc522_to_card(PCD_TRANSCEIVE, serNum, 2, serNum, &serNumLength);
    if (status == MI_OK) {
        // Check card serial number
        for (i = 0; i < 4; i++) {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) {
            status = MI_ERR;
        }
    }

    return status;
}

// Calculate CRC
void calculate_crc(uint8_t *data, int length, uint8_t *result) {
    mfrc522_clear_bit_mask(DivIrqReg, 0x04);  // CRCIrq = 0
    mfrc522_set_bit_mask(FIFOLevelReg, 0x80); // Clear the FIFO pointer

    // Write data to the FIFO
    for (int i = 0; i < length; i++) {
        mfrc522_write_register(FIFODataReg, data[i]);
    }
    mfrc522_write_register(CommandReg, PCD_CALCCRC);

    // Wait CRC calculation is complete
    int i = 0xFF;
    while (1) {
        uint8_t n = mfrc522_read_register(DivIrqReg);
        if (n & 0x04) { // CRCIrq = 1
            break;
        }
        if (--i == 0) {
            break;
        }
    }

    // Read CRC calculation result
    result[0] = mfrc522_read_register(CRCResultRegL);
    result[1] = mfrc522_read_register(CRCResultRegM);
}

// Select card, return capacity
uint8_t mfrc522_select_tag(uint8_t *serNum) {
    uint8_t i;
    uint8_t status;
    int size;
    uint8_t recvData[2];
    uint8_t buffer[9];

    // Frame format of Select Command
    buffer[0] = PICC_SELECTTAG;
    buffer[1] = 0x70;
    for (i = 0; i < 5; i++) {
        buffer[i+2] = *(serNum+i);
    }
    calculate_crc(buffer, 7, &buffer[7]);  // CRC for Select Command, last two bytes

    status = mfrc522_to_card(PCD_TRANSCEIVE, buffer, 9, recvData, &size);

    if ((status == MI_OK) && (size == 0x18)) {
        size = recvData[0]; // Capacity
    } else {
        size = 0;
    }

    return size;
}
// Verify card password
uint8_t mfrc522_auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum) {
    uint8_t status;
    int recvBits;
    uint8_t i;
    uint8_t buff[12];

    // Verify command + block address + sector key + card serial number
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i = 0; i < 6; i++) {
        buff[i+2] = *(Sectorkey+i);
    }
    for (i = 0; i < 4; i++) {
        buff[i+8] = *(serNum+i);
    }
    status = mfrc522_to_card(PCD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(mfrc522_read_register(Status2Reg) & 0x08))) {
        status = MI_ERR;
    }

    return status;
}

// Read block data
void mfrc522_read(uint8_t blockAddr, uint8_t *recvData) {
    uint8_t status;
    int unLen;
    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    calculate_crc(recvData, 2, &recvData[2]);
    status = mfrc522_to_card(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
    if (!(status == MI_OK)) {
        // Handle error
    }
}

// Write block data
void mfrc522_write(uint8_t blockAddr, uint8_t *writeData) {
    uint8_t buf[18];
    uint8_t status;
    int recvBits;
    buf[0] = PICC_WRITE;
    buf[1] = blockAddr;
    calculate_crc(buf, 2, &buf[2]);
    status = mfrc522_to_card(PCD_TRANSCEIVE, buf, 4, buf, &recvBits);

    if ((status == MI_OK) && (recvBits == 4) && ((buf[0] & 0x0F) == 0x0A)) {
        // Transfer 16 bytes to the FIFO
        memcpy(buf, writeData, 16);
        calculate_crc(buf, 16, &buf[16]);
        status = mfrc522_to_card(PCD_TRANSCEIVE, buf, 18, buf, &recvBits);
    }

    if (!(status == MI_OK) || !(recvBits == 4) || ((buf[0] & 0x0F) != 0x0A)) {
        // Handle error
    }
}

// Halt the card
void mfrc522_halt(void) {
    uint8_t status;
    int unLen;
    uint8_t buff[4];

    buff[0] = PICC_HALT;
    buff[1] = 0;
    calculate_crc(buff, 2, &buff[2]);

    mfrc522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}


void mfrc522_stop_crypto1(void) {
    // Clear the bit that enables crypto1 - usually bit 3 (0x08) in Status2Reg
    mfrc522_clear_bit_mask(Status2Reg, 0x08);
}