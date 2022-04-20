/** @file i2c.c
 * 
 *  @brief steam virtual i2c/smbus driver. 
 *
 *  @author Bharath Raj       
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/* linux kernel i2c */
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/* file handler */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct i2c_steam_dev_module 
{
    char* dev_name;
    uint8_t dev_addr;

    int fd;
    bool status;
}i2c_steam_dev_module_t;

enum i2c_steam_dev_id
{
    /* add more I2C as per requirement */
    I2C_STEAM_ADC_1 = 0,
    I2C_STEAM_MAX
};

i2c_steam_dev_module_t g_i2c_module[I2C_STEAM_MAX] = {
    {"/dev/i2c-1", 0x03, -1, 0}\
    /* add more I2C info as per requirement */
};
 
static int i2c_open(void)
{
    unsigned long funcs;

    for(int i = 0; i < I2C_STEAM_MAX; i++)
    {
        g_i2c_module[i].fd = open(g_i2c_module[i].dev_name, O_RDWR);
        if(g_i2c_module[i].fd < 0)
        {
            printf("%s : error with id: %d\n", __func__, i);
            perror("error");
            return -1;
        }
        printf("%s : id = %d, fd = %d, dev_name  = %s\n", __func__, i, g_i2c_module[i].fd, g_i2c_module[i].dev_name);
        g_i2c_module[i].status = true;

        /* check for supported functions */
        if(ioctl(g_i2c_module[i].fd, I2C_FUNCS, &funcs) < 0)
        {
            printf("%s : support 1 error with id: %d\n", __func__, i);
            perror("error");
            return -1;
        }
#if 1
        if(!(funcs & I2C_FUNC_I2C))
        {
            printf("%s : support 2 error with id: %d | 0x%lx\n", __func__, i, funcs);
            perror("error");
            //return -1;
        } 
#endif
    }
    return 0;
}

static int i2c_set_addr(void)
{
    for(int i = 0; i < I2C_STEAM_MAX; i++)
    {
        if (ioctl(g_i2c_module[i].fd, I2C_SLAVE, g_i2c_module[i].dev_addr) < 0) 
        {
            printf("%s : error with id: %d\n", __func__, i);
            perror("error");
            return -1;
        }
        printf("%s : id = %d, dev_name  = %s, slave_addr = 0x%x\n", __func__, i, g_i2c_module[i].dev_name, g_i2c_module[i].dev_addr);
    }

    return 0;
}

static void i2c_close(void)
{
    for(int i = 0; i < I2C_STEAM_MAX; i++)
    {
        if(!(g_i2c_module[i].fd < 0))
        {
            close(g_i2c_module[i].fd);
        }
        g_i2c_module[i].status = false;
    }
}

int i2c_init(void)
{
    // Open all the i2c dev handles
    if(i2c_open() < 0)
    {
        return -1;
    }

    // set the i2c slave address for all subsequent i2c device transfers
    if(i2c_set_addr() < 0)
    {
        return -1;
    }

    printf("%s : successful \n", __func__);
    return 0;
}

void i2c_deinit(void)
{
    i2c_close();
    printf("%s : successful \n", __func__);
}

#if 0
int i2c_read_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_rd_buf)
{
    if(id >= I2C_STEAM_MAX)
    {
        printf("%s : invalid dev_id %d\n", __func__, id);
        return -1;
    }

    if(write(g_i2c_module[id].fd, 0x00, 1) != 1)
    {
        printf("%s : writing read_ptr failed | id: %d\n", __func__, id);
        return -1;
    }

    if(read(g_i2c_module[id].fd, p_rd_buf, len) != len)
    {
        printf("%s : reading read_ptr failed | id: %d\n", __func__, id);
        return -1;
    }

    return 0;
}

int i2c_write_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_wr_buf)
{
    uint8_t temp_buf[UINT8_MAX + 1];
    temp_buf[0] = reg_addr;
    memcpy(&temp_buf[1], p_wr_buf, len);

    if(write(g_i2c_module[id].fd, &reg_addr, len + 1) != len + 1)
    {
        printf("%s : writing write_ptr failed | id: %d\n", __func__, id);
        return -1;
    }
}
#elif 0

static int i2c_transfer(enum i2c_steam_dev_id id, struct i2c_msg *msgs, size_t count) 
{
    struct i2c_rdwr_ioctl_data i2c_rdwr_data;

    /* Prepare I2C transfer structure */
    memset(&i2c_rdwr_data, 0, sizeof(struct i2c_rdwr_ioctl_data));

    i2c_rdwr_data.msgs = msgs;
    i2c_rdwr_data.nmsgs = count;

    /* Transfer */
    if (ioctl(g_i2c_module[id].fd, I2C_RDWR, &i2c_rdwr_data) < 0)
    {
        printf("%s : I2C_RDWR failed | id: %d\n", __func__, id);
        return -1;         
    }

    return 0;
}

// int i2c_read_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_rd_buf)
// {
//     struct i2c_smbus_ioctl_data args;
//     union i2c_smbus_data data;
//     args.read_write = I2C_SMBUS_READ;
//     args.command = reg_addr;
//     args.size = len;
//     args.data = &data;
    
//     for(int i = 0; i <  len; i++)
//     {
//         args.command = reg_addr + i;
//         if(ioctl(g_i2c_module[id].fd, I2C_SMBUS, &args) < 0)
//         {
//             printf("%s : reading read_ptr failed | id: %d\n", __func__, id);
//             return -1;
//         }

//         p_rd_buf[i] = data.byte;
//         printf("%s : [%d] : 0x%x\n", __func__, i, p_rd_buf[i]);
//     }
//     return 0;
// }    

int i2c_read_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_rd_buf)
{
    struct i2c_msg msgs[2] = {
            { .addr = g_i2c_module[id].dev_addr, .flags = 0,        .len = 1,   .buf = &reg_addr}, //Write the read pointer 
            { .addr = g_i2c_module[id].dev_addr, .flags = I2C_M_RD, .len = len, .buf = p_rd_buf}}; //Read the data from read pointer

    return i2c_transfer(id, msgs, 2);
} 

int i2c_write_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_wr_buf)
{

}

#else

int i2c_smbus_access(int file, char read_write, uint8_t command,
		       int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	if(ioctl(file, I2C_SMBUS, &args) <0)
    {
        return -1;
    }

	return 0;
}

int i2c_write_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_wr_buf)
{
    union i2c_smbus_data data;
    for(int i = 0; i < len; i++)
    {
        data.byte = p_wr_buf[i];
       if(i2c_smbus_access(g_i2c_module[id].fd, I2C_SMBUS_WRITE, reg_addr + i, I2C_SMBUS_BYTE_DATA, &data) < 0)
       {
           return -1;
       }
    }

    return 0;
}
int i2c_read_data(enum i2c_steam_dev_id id, uint8_t reg_addr, uint8_t len, uint8_t* p_rd_buf)
{
    union i2c_smbus_data data;
    for(int i = 0; i < len; i++)
    {
       if(i2c_smbus_access(g_i2c_module[id].fd, I2C_SMBUS_READ, reg_addr + i, I2C_SMBUS_BYTE_DATA, &data) < 0)
       {
           return -1;
       }
       p_rd_buf[i] = data.byte & 0xFF;
    }

    return 0;
}

#endif

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

uint8_t g_test_rd_buf[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t g_rd_buf[UINT8_MAX] = { 0,};

uint8_t g_test_wr_buf[]= "Hello from I2C device\n";

int main(void)
{
    if(i2c_init() < 0)
    {
        goto exit;
    }

    /* test script */
    /* read val */
    if(i2c_read_data(I2C_STEAM_ADC_1, 0x00, 8, g_rd_buf) < 0)
    {
        printf("i2c_read_data  failed \n");
    }
    DumpHex(g_test_rd_buf, 8);
    DumpHex(g_rd_buf, 8);
    if(strncmp(g_rd_buf, g_test_rd_buf, 8) !=0)
    {
        printf("Read test failed \n");
        goto exit;
    }

    printf("Read test passed\n");

#if 1
    /* write val */
    if(i2c_write_data(I2C_STEAM_ADC_1, 0x10, sizeof(g_test_wr_buf), g_test_wr_buf) < 0)
    {

    }
    /* read val */
    if(i2c_read_data(I2C_STEAM_ADC_1, 0x10, sizeof(g_test_wr_buf), g_rd_buf) < 0)
    {

    }
    DumpHex(g_test_wr_buf, sizeof(g_test_wr_buf));
    DumpHex(g_rd_buf, sizeof(g_test_wr_buf));
    if(strncmp(g_rd_buf, g_test_wr_buf, sizeof(g_test_wr_buf)) != 0)
    {
        printf("Write test failed \n");
        goto exit;
    }

    printf("Read test passed\n");
#endif
exit:
    i2c_deinit();
    return 1;
}