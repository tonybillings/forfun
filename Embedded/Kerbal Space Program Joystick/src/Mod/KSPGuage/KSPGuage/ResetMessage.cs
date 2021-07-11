namespace KSPGuage
{
    public sealed class ResetMessage : Message
    {
        #region Private Constants
        private const byte MESSAGE_TYPE = 0;
        private const byte PAYLOAD_LENGTH = 0;
        #endregion

        #region Constructors
        public ResetMessage()
            : base(PAYLOAD_LENGTH)
        {
            _packet = new byte[] { 
                SYN, SYN, 
                MESSAGE_TYPE, 
                (byte)'0', (byte)'0', (byte)'0', 
                EOT };
        }
        #endregion
    }
}
