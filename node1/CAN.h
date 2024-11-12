void mcp_reset();
uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t address, uint8_t data);
char mcp_read_status();
void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp_init();
void mcp_request_to_send(int buffer_number);
void mcp_set_mode(uint8_t mode);


typedef struct {
    uint16_t id;
    uint8_t data[8];
    uint8_t length;
} CAN_Message;



typedef struct CanInit CanInit;
__attribute__((packed)) struct CanInit {
    union {
        struct {
            //CNF1 register
            uint8_t brp:6;
            uint8_t sjw:2;
            //CNF2 register
            uint8_t propag:3;
            uint8_t phase1:3;
            uint8_t sam:1;
            uint8_t bltmode:1;
            //CNF3 register
            uint8_t phase2:3;
        };
        uint32_t reg;
    };
};

int can_bus_read(CAN_Message *message);
void can_bus_write(uint8_t tx_buffer, CAN_Message m);
void can_read_print(CAN_Message *message);
void can_init(CanInit init, uint8_t loopback);

void mcp_clear_tx_buffer(int buffer_number);


void can_sensor_data(CAN_Message *message);