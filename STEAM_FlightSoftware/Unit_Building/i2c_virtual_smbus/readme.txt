/* #1 create a virtual i2c/smbus port */

$ modprobe i2c-dev
$ modprobe i2c-stub chip_addr=0x03

/* #2 Load test data to the registers */
$ i2c-stub-from-dump 0x03 test_i2c.dump


/* #3 run the i2c_test object file */

$ sudo ./i2c_test