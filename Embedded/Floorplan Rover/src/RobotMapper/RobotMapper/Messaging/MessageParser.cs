using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotMapper.Messaging
{
    public static class MessageParser
    {
        #region Constants
        private static readonly byte SYN_BYTE = 22;    // Synchronous idle byte
        private static readonly byte STX_BYTE = 2;     // Start of text byte
        private static readonly byte ETX_BYTE = 3;     // End of text byte
        private static readonly byte EOT_BYTE = 4;     // End of transmission byte
        #endregion

        #region Type Definitions
        enum MessageState
        {
            SYN,	// Synchronization Idle
            MT,		// Message Type
            PL,		// Payload Length
            STX,	// Start of Text
            PAY,	// Payload
            ETX,	// End of Text
            CHK,	// Checksum (exclusive-or of PAY and ETX)
            EOT		// End of Transmission
        }

        public delegate void MessageReceivedHandler(byte messageType, byte[] payload);
        #endregion

        #region Events
        public static event MessageReceivedHandler MessageReceived;
        #endregion

        #region Private Variables
        private static MessageState _state;
        private static byte _currentByte;
        private static byte _syncCount;
        private static byte _messageType;
        private static byte[] _payloadLengthBytes;
        private static byte _payloadLength;
        private static byte[] _payload;
        private static byte _payloadIndex;
        #endregion

        #region Private Methods
        private static void Reset()
        {
            _state = MessageState.SYN;
            _currentByte = 0;
            _syncCount = 0;
            _messageType = 0;
            _payloadLengthBytes = new byte[2];
            _payloadLength = 0;
            _payload = new byte[100];
            _payloadIndex = 0;
        }

        private static bool IsPayloadValid(byte checksum)
        {
            byte chk = ETX_BYTE;

            for (int i = 0; i < _payloadLength; i++)
                chk ^= _payload[i];

            return checksum == chk;
        }

        public static void ProcessByte(byte value)
        {
            _currentByte = value;

            switch (_state)
            {
                case MessageState.SYN:
                    if (_currentByte == SYN_BYTE)
                    {
                        if (++_syncCount == 2) { _state = MessageState.MT; }
                    }
                    else
                    {
                        _syncCount = 0;
                    }
                    break;
                case MessageState.MT:
                    _messageType = _currentByte;
                    _state = MessageState.PL;
                    break;
                case MessageState.PL:
                    if (_payloadLengthBytes[0] == 0)
                    {
                        _payloadLengthBytes[0] = _currentByte;
                    }
                    else if (_payloadLengthBytes[1] == 0)
                    {
                        _payloadLengthBytes[1] = _currentByte;
                        _payloadLength = Convert.ToByte(ASCIIEncoding.ASCII.GetString(_payloadLengthBytes));
                        _state = (_payloadLength > 0) ? MessageState.STX : MessageState.EOT;
                    }
                    break;
                case MessageState.STX:
                    if (_currentByte == STX_BYTE)
                        _state = MessageState.PAY;
                    else
                        Reset();
                    break;
                case MessageState.PAY:
                    _payload[_payloadIndex++] = _currentByte;
                    if (_payloadIndex == _payloadLength) { _state = MessageState.ETX; }
                    break;
                case MessageState.ETX:
                    if (_currentByte == ETX_BYTE)
                        _state = MessageState.CHK;
                    else
                        Reset();
                    break;
                case MessageState.CHK:
                    if (IsPayloadValid(_currentByte))
                        _state = MessageState.EOT;
                    else
                        Reset();
                    break;
                case MessageState.EOT:
                    if (_currentByte == EOT_BYTE)
                    {
                        if (MessageReceived != null) { MessageReceived(_messageType, _payload); }
                        Reset();
                    }
                    else
                    {
                        Reset();
                    }
                    break;
            }
        }
        #endregion

        #region Constructor
        static MessageParser()
        {
            Reset();
        }
        #endregion
    }
}
