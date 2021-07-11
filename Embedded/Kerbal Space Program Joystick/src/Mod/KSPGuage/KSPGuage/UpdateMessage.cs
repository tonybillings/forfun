using System;

namespace KSPGuage
{
    public sealed class UpdateMessage : Message
    {
        #region Private Constants
        private const byte MESSAGE_TYPE = 1;
        private const byte PAYLOAD_LENGTH = 25;
        private const int INDEX_FUEL = INDEX_PAYLOAD;
        private const int INDEX_MP = INDEX_PAYLOAD + 3;
        private const int INDEX_EC = INDEX_PAYLOAD + 6;
        private const int INDEX_SPD = INDEX_PAYLOAD + 9;
        private const int INDEX_ALT = INDEX_PAYLOAD + 17;
        private const int INDEX_SAS = INDEX_PAYLOAD + 23;
        private const int INDEX_RCS = INDEX_PAYLOAD + 24;
        private const int INDEX_CHK = 33;
        #endregion

        #region Constructors
        public UpdateMessage() 
            : base(PAYLOAD_LENGTH)
        {
            _packet = new byte[] { 
                SYN, SYN, 
                MESSAGE_TYPE, 
                (byte)'0', (byte)'2', (byte)'5', 
                STX, 
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 
                0,
                0,
                ETX, 
                0, 
                EOT };
        }
        #endregion

        #region Public Methods
        public void SetFuelPercentage(float value)
        {
            WriteValueToPacket((int)(value * 100), INDEX_FUEL, 3);
        }

        public void SetMonoPropellantPercentage(float value)
        {
            WriteValueToPacket((int)(value * 100), INDEX_MP, 3);
        }

        public void SetElectricChargePercentage(float value)
        {
            WriteValueToPacket((int)(value * 100), INDEX_EC, 3);
        }

        public void SetSpeed(int value)
        {
            if (value > 99999999) value *= -1;
            WriteValueToPacket(value, INDEX_SPD, 8);
        }

        public void SetAltitude(int value)
        {
            if (value > 999999) value *= -1;
            WriteValueToPacket(value, INDEX_ALT, 6);
        }

        public void SetSAS(bool enabled)
        {
            WriteValueToPacket(Convert.ToInt32(enabled), INDEX_SAS, 1);
        }

        public void SetRCS(bool enabled)
        {
            WriteValueToPacket(Convert.ToInt32(enabled), INDEX_RCS, 1);
        }
        #endregion
    }
}
