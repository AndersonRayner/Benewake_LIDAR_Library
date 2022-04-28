
#include "Benewake_CAN.h"

Benewake_CAN::Benewake_CAN(FlexCAN_T4FD& canbus, uint32_t baud) :
  _canbus(canbus),
  _baud(baud);  // This constructor might be wrongly formed

void Benewake_CAN::setup(void) {
	
	// Initialise the CAN bus
  _canbus.begin();
  _canbus.setBaudRate(_baud);
  
  return;

}

void Benewake_CAN::readMessage()
{
	
	// Read the latest message in the inbox
	_canbus.read(msg);
	
	// Debugging
	if (_debug)
	{
		 Serial.print("CAN | ");
      Serial.print("MB: "); Serial.print(msg.mb);
      Serial.print("  ID: 0x"); Serial.print(msg.id, HEX );
      Serial.print("  EXT: "); Serial.print(msg.flags.extended );
      Serial.print("  LEN: "); Serial.print(msg.len);
      Serial.print(" DATA: ");
      for ( uint8_t i = 0; i < 8; i++ ) {
        Serial.print(msg.buf[i]); Serial.print(" ");
      }
      Serial.print("  TS: "); Serial.println(msg.timestamp);
    }
	
	return;
}

void Benewake_CAN::getVersion(uint32_t target_id)
{
	
	CAN_message_t msg;
	
  // Gets the firmware version
  msg.id = target_id;
  msg.flags.extended = 0;
  msg.len = 4;
  msg.buf[0] = 0x5A;
  msg.buf[1] = 0x04;
  msg.buf[2] = 0x01;
  msg.buf[3] = calc_checksum(msg);  // checksum

  sprintf(_msg_buf, "Getting version from ID 0x%x\n", msg.id);
  Serial.print(_msg_buf);
  _canbus.write(msg);

  return;
}


void Benewake_CAN::set_txID(uint32_t target_id, uint32_t new_id)
{

	CAN_message_t msg;

  // Hardcoded to set to 0x04 for now
  msg.id = target_id;
  msg.flags.extended = 0;
  msg.len = 8;
  msg.buf[0] = 0x5A;  // Header
  msg.buf[1] = 0x08; // Modify CAN Arbitation ID (1)
  msg.buf[2] = 0x50; // Modify CAN Arbitration ID (2)
  msg.buf[3] = (new_id >> 0) & 0xFF; // Addr H1
  msg.buf[4] = (new_id >> 8) & 0xFF; // Addr H2
  msg.buf[5] = (new_id >> 16) & 0xFF; // Addr H3
  msg.buf[6] = (new_id >> 24) & 0xFF; // Addr H4
  msg.buf[7] = calc_checksum(msg);   // checksum

	// Debugging
	if (_debug)
	{
  sprintf(_msg_buf, "Setting ID of 0x%X to ID 0x%X\n", msg.id, new_id);
  Serial.print(_msg_buf);
	}
	
	// Send the message
  _canbus.write(msg);

  return;

}

void Benewake_CAN::set_rxID(uint32_t target_id, uint32_t new_id)
{
	
	CAN_message_t msg;
  
  // Compile the message
  msg.id = target_id;
  msg.flags.extended = 0;
  msg.len = 8;
  msg.buf[0] = 0x5A;  // Header
  msg.buf[1] = 0x08; // Modify CAN Arbitation ID (1)
  msg.buf[2] = 0x51; // Modify CAN Arbitration ID (2)
  msg.buf[3] = (new_id >> 0) & 0xFF; // Addr H1
  msg.buf[4] = (new_id >> 8) & 0xFF; // Addr H2
  msg.buf[5] = (new_id >> 16) & 0xFF; // Addr H3
  msg.buf[6] = (new_id >> 24) & 0xFF; // Addr H4
  msg.buf[7] = calc_checksum(msg);   // checksum

	// Debugging
	if (_debug)
	{
  sprintf(_msg_buf, "Setting ID of 0x%X to ID 0x%X\n", msg.id, new_id);
  Serial.print(_msg_buf);
	}
	
	// Write the message
  _canbus.write(msg);

  return;

}

void Benewake_CAN::saveConfig(uint32_t target_id)
{
	
	CAN_message_t msg;
	
	// Compile the message
  msg.id = target_id;
  msg.flags.extended = 0;
  msg.len = 4;
  msg.buf[0] = 0x5A;  // Header
  msg.buf[1] = 0x04; // Save configuration (1)
  msg.buf[2] = 0x11; // Save configuration (2)
  msg.buf[3] = calc_checksum(msg); // checksum

	if (_debug)
	{
  sprintf(_msg_buf, "Saving configuration of ID 0x%X\n", msg.id);
  Serial.print(_msg_buf);
	}
	
	// Send the message
  _canbus.write(msg);
  
  
  // Really should have a wait for reply in here...
  
  return;
  
}

uint8_t Benewake_CAN::calc_checksum(CAN_message_t msg)
{
  // Set the checksum value to 0 to initialise
  msg.buf[msg.len - 1] = 0;

  // Simply add everything together
  for (uint8_t ii = 0; ii < msg.len - 1; ii++) // calculate the checksum
  {
    msg.buf[msg.len - 1] += msg.buf[ii];
  }

	// Return the checksum value
  return (msg.buf[msg.len - 1]);
  
}
