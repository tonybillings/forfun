using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace RobotMapper.Objects
{
    public class Robot
    {
        #region Constants
        private const float DEFAULT_SCALE = 0.75f;
        private readonly Vector3 DEFAULT_COLOR = new Vector3(1f, 0f, 0f);
        #endregion

        #region Public Variables
        public static Model Model;
        public Vector3 Position;
        public Vector3 TargetPosition;
        public float Rotation;
        public float TargetRotation;
        public float Scale;
        public Vector3 Color;
        #endregion

        #region Constructors
        public Robot()
        {
            Scale = DEFAULT_SCALE;
            Color = DEFAULT_COLOR;
        }
        #endregion
    }
}
