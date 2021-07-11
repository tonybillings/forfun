namespace KSPGuage
{
    public abstract class Message
    {
        #region Constants
        protected const byte SYN = 22;
        protected const byte STX = 2;
        protected const byte ETX = 3;
        protected const byte EOT = 4;
        protected const int INDEX_PAYLOAD = 7;
        #endregion

        #region Protected Variables
        protected byte[] _packet;
        protected int _payloadLength;
        #endregion

        #region Constructors
        protected Message(int payloadLength)
        {
            _payloadLength = payloadLength;
        }
        #endregion

        #region Private Methods
        protected byte GetChecksum()
        {
            byte chk = ETX;

            for (int i = 0; i < _payloadLength; i++)
                chk ^= _packet[INDEX_PAYLOAD + i];

            return chk;
        }

        protected void WriteValueToPacket(int value, int startIndex, int length)
        {
            if (value < 0)
            {
                for (int i = 0; i < length; i++)
                    _packet[startIndex + i] = (byte)'-';
            }
            else
            {
                string valueStr = value.ToString().PadLeft(length, '0');

                for (int i = 0; i < length; i++)
                    _packet[startIndex + i] = (byte)valueStr[i];
            }
        }
        #endregion

        #region Public Methods
        public byte[] GetBytes()
        {
            if (_payloadLength > 0)
                _packet[INDEX_PAYLOAD + _payloadLength + 1] = GetChecksum();

            return _packet;
        }
        #endregion
    }
}
