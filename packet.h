#define PACKET_SIZE 10
#define FRAME_SIZE_MAX 20 // PACKET_SIZE * 2

/*
  Packet:
  
  +-----------+---------+--------+--------+---------+----------+
  | Operation | ID      | From   | To     | Seat    | Checksum |
  +-----------+---------+--------+--------+---------+----------+
  | 1 byte    | 4 bytes | 1 byte | 1 byte | 2 bytes | 1 byte   |
  +-----------+---------+--------+--------+---------+----------+

  Delimiter: 0xF6
  Escaper: 0xF7

  Operation:
  0x01: query
  0x02: book
  0x03: check
  0x04: refund

  ID: unsigned int
  From, To: unsigned char
  Seat: short

  Checksum algorithm: BSD checksum

 */

class Packet {
public:
	uint8_t bytes_read;
	uint8_t buffer[FRAME_SIZE_MAX];
	uint8_t bytes_to_read();
	void reset();
};
