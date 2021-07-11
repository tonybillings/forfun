using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Remote
{
    public enum CommandCode
    {
        Invalid = 0,

        RecPowerToggle = 100,
        RecOk,
        RecBack,
        RecMenu,
        RecUp,
        RecDown,
        RecLeft,
        RecRight,
        RecVolumeUp,
        RecVolumeDown,
        RecInputSelectGame,
        RecInputSelectMediaPlayer,
        RecMovieSoundMode,
        RecMusicSoundMode,

        ProPowerOn = 114,
        ProPowerOff,
        ProOk,
        ProBack,
        ProMenu,
        ProUp,
        ProDown,
        ProLeft,
        ProRight,
        ProCycleAdvMenu,
        ProCycleColorProfile,
        ProCyclePictureMode,

        RokuOk = 126,
        RokuBack,
        RokuMenu,
        RokuUp,
        RokuDown,
        RokuLeft,
        RokuRight,
        RokuStar,
        RokuPlayPause,
        RokuRewind,
        RokuForward
    }

    public static class RemoteController
    {
        private const string ROKU_TRANSMITTER = "192.168.1.61";
        private const string REC_PRO_TRANSMITTER = "192.168.1.62";
        private const int PORT = 6163;

        private static UdpClient _udpClient1;
        private static UdpClient _udpClient2;

        static RemoteController()
        {
            _udpClient1 = new UdpClient();
            _udpClient1.Connect(new IPEndPoint(IPAddress.Parse(REC_PRO_TRANSMITTER), PORT));

            _udpClient2 = new UdpClient();
            _udpClient2.Connect(new IPEndPoint(IPAddress.Parse(ROKU_TRANSMITTER), PORT));
        }

        public static void SendCommand(CommandCode command)
        {
            byte[] data = { 22, 22, (byte)command, 23 };

            if ((int)command < 126)
                _udpClient1.Send(data, data.Length);
            else
                _udpClient2.Send(data, data.Length);
        }
    }

}