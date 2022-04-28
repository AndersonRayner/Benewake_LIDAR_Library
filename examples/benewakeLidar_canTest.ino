

#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;  // CAN2 : Pin  0,  1

CAN_message_t msg;

char _msg_buf[50];

uint32_t _next_msg = 3 * 1000;  // Time when we can send the next messsage
uint8_t _msg_sent = 0;
uint32_t _sendTo_id = 0x4004 - 10; //   - 10;//3003 - 0x100;

// it would appear maybe 0x7ff might be a broadcast address?

void setup(void) {
  can2.begin();
  can2.setBaudRate(1000000);

}

void loop() {

  if (_msg_sent == 0)
  {
    if (millis() > _next_msg)
    {

      if (0)
      {
        _sendTo_id++;

        // Skip anything that ends in 0xFF, as these appear to be broadcast addresses
        uint32_t x = _sendTo_id & 0xFF;
        if (x == 0xFF)
        {
          _sendTo_id++;
        }

        // Attempt to get the version
        send_getVersion_msg(_sendTo_id);

        _msg_sent = 0;

      }

      else if (0)
      {
        //set_txID(0x2fff, 0x04);
        set_rxID(0x3fff, 0x4004);

        _msg_sent = 1;
      }

      // Work out when the next message should be send
      _next_msg = millis() + 50;
    }
  }


  // Read out whatever we have
  while ( can2.read(msg) ) {
    if (1)//(msg.buf[0] == 0x5A)
    {
      Serial.print("CAN2 ");
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


    // Print out if we found someone on this address
    if (msg.buf[0] == 0x5A)
    {
      sprintf(_msg_buf, "\tFound configuration message from ID 0x%x!\n", msg.id);
      Serial.print(_msg_buf);

      //send_saveConfig(0x3fff);
      //send_saveConfig(0x4004);

      // halt the program
      //while (1) {};
    }
  }

}

void send_getVersion_msg(uint32_t target_id)
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
  can2.write(msg);

  return;
}


void set_txID(uint32_t target_id, uint32_t new_id)
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

  sprintf(_msg_buf, "Setting ID of 0x%X to ID 0x%X\n", msg.id, new_id);
  Serial.print(_msg_buf);
  can2.write(msg);

  return;

}

void set_rxID(uint32_t target_id, uint32_t new_id)
{
	
	CAN_message_t msg;

  // Hardcoded to set to 0x4004 for now
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

  sprintf(_msg_buf, "Setting ID of 0x%X to ID 0x%X\n", msg.id, new_id);
  Serial.print(_msg_buf);
  can2.write(msg);

  return;

}

void send_saveConfig(uint32_t target_id)
{
	
	CAN_message_t msg;
	
  msg.id = target_id;
  msg.flags.extended = 0;
  msg.len = 4;
  msg.buf[0] = 0x5A;  // Header
  msg.buf[1] = 0x04; // Save configuration (1)
  msg.buf[2] = 0x11; // Save configuration (2)
  msg.buf[3] = calc_checksum(msg); // checksum

  sprintf(_msg_buf, "Saving configuration of ID 0x%X\n", msg.id);
  Serial.print(_msg_buf);
  can2.write(msg);
  
}

uint8_t calc_checksum(CAN_message_t msg)
{
  // Set the checksum value to 0 for the start
  msg.buf[msg.len - 1] = 0;

  // Simply add everything together
  for (uint8_t ii = 0; ii < msg.len - 1; ii++) // calculate the checksum
  {
    msg.buf[msg.len - 1] += msg.buf[ii];
  }

  return (msg.buf[msg.len - 1]);
}
