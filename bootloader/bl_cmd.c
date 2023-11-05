/**
 * @Command Name: BL_GET_HELP
 * @Command Format:
 *      +------------------+--------------+----------------------------------+
 *      | Length to follow | Command Code |                CRC               |
 *      +------------------+--------------+----------------------------------+
 *      |     1 byte       |    1 byte    |              4 bytes             |
 *      +------------------+--------------+----------------------------------+
 * 
 * @Total bytes of the packet = 6
 * @Length to follow field will contain the value: 5
 * @Command Code: 0x52
 * 
 * @Command Replies:
 * 
 * 
 */