#include <FlexCAN_T4.h>
#include <stdint.h>

class Benewake_CAN {
	
	public:
	Benewake_CAN(FlexCAN_T4FD& canbus, uint32_t baud);
	
	// Functions
	void init(void);

	void getVersion(uint32_t target_id);
	void set_txID(uint32_t target_id, uint32_t new_id);
	void set_rxID(uint32_t target_id, uint32_t new_id);
	void saveConfig(uint32_t target_id);

	private:
	
	FlexCAN_T4FD& _canbus
	
	// Functions
	uint8_t calc_checksum(CAN_message_t msg);
	
	// Variables
	uint32_t _baud;
	char _msg_buf[50];
	uint8_t _debug = 1;

};