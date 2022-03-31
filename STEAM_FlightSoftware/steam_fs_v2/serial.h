#define MAX_PACKET_SIZE 1024

struct punch_packet {
	char *buf;
	int size;
};

struct h_packet {
	char example;
};

struct s_packet {
	char example;
};

int setup_serial_interfaces();
int close_serial_interfaces();

int h_write(struct h_packet p);
int h_read(struct h_packet *p);

int s_write(struct s_packet p);
int s_read(struct s_packet *p);

int punch_write(struct punch_packet p);
int punch_read(struct punch_packet *p);
