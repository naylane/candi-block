#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define HPS_PHYS_BASE 0xFF000000
#define HPS_SPAN 0x01000000   
#define SYSMGR_BASE 0xFFD08000
#define I2C0_BASE 0xFFC04000 

#define SYSMGR_I2C0USEFPGA (SYSMGR_BASE + 0x704)
#define SYSMGR_GENERALIO7 (SYSMGR_BASE + 0x49C)
#define SYSMGR_GENERALIO8 (SYSMGR_BASE + 0x4A0)

#define I2C0_CON (I2C0_BASE + 0x00) 
#define I2C0_TAR (I2C0_BASE + 0x04)
#define I2C0_DATA_CMD (I2C0_BASE + 0x10)
#define I2C0_FS_SCL_HCNT (I2C0_BASE + 0x1C)
#define I2C0_FS_SCL_LCNT (I2C0_BASE + 0x20)
#define I2C0_ENABLE (I2C0_BASE + 0x6C) 
#define I2C0_RXFLR (I2C0_BASE + 0x78)   
#define I2C0_TXFLR (I2C0_BASE + 0x74)
#define I2C0_CLR_INTR (I2C0_BASE + 0x40)

#define ADXL345_ADDR 0x53   
#define ADXL345_DEVID 0x00 
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32 
#define ADXL345_DATAX1 0x33 
#define ADXL345_INT_ENABLE 0x2E
#define ADXL345_INT_MAP 0x2F
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_BW_RATE 0x2C



volatile uint32_t *base_hps;
int mg_por_lsb = 4; 
int16_t offset_x = 0;
int fd;

void escrever_registro(uint32_t endereco, uint32_t valor) {
   *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4) = valor;
}

uint32_t ler_registro(uint32_t endereco) {
   return *(volatile uint32_t*)(base_hps + (endereco - HPS_PHYS_BASE) / 4);
}


void inicializar_i2c() {
   escrever_registro(SYSMGR_I2C0USEFPGA, 0);
   escrever_registro(SYSMGR_GENERALIO7, 1);
   escrever_registro(SYSMGR_GENERALIO8, 1);

   escrever_registro(I2C0_ENABLE, 0x0);
   escrever_registro(I2C0_CON, 0x65);
   escrever_registro(I2C0_TAR, ADXL345_ADDR);

   escrever_registro(I2C0_FS_SCL_HCNT, 60 + 30);
   escrever_registro(I2C0_FS_SCL_LCNT, 130 + 30);
  
   escrever_registro(I2C0_ENABLE, 0x1);
   usleep(10000);
}

void verificar_status_i2c() {
   uint32_t status = ler_registro(I2C0_ENABLE);
   printf("Status I2C: 0x%08X\n", status);
   if (status & 0x1) {
       printf("I2C habilitado\n");
   } else {
       printf("I2C nao habilitado\n");
   }
}

void ler_reg_acel(uint8_t address, uint8_t *value) {
   escrever_registro(I2C0_DATA_CMD, address + 0x400);
   escrever_registro(I2C0_DATA_CMD, 0x100);
   while (ler_registro(I2C0_RXFLR) == 0) {}
   *value = ler_registro(I2C0_DATA_CMD) & 0xFF;
}

void escrever_reg_acel(uint8_t address, uint8_t value) {
   escrever_registro(I2C0_DATA_CMD, address + 0x400);
   escrever_registro(I2C0_DATA_CMD, value);
}
