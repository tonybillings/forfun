using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace RobotMapper.Objects
{
    public class Beacon
    {
        #region Constants
        private const float DEFAULT_SCALE = 0.33f;
        private const float SCALE_MAX = 5f;
        private const float SCALING_SPEED = 0.0035f;
        private const float BLENDING_SPEED = 0.001f;
        private readonly Vector3 DEFAULT_COLOR = new Vector3(1f, 0f, 0f);
        #endregion

        #region Public Variables
        public static Model Model;   
        public Vector3 Position;
        public Vector3 TargetPosition;
        public float Rotation;
        public float TargetRotation;
        public float Scale;
        public float Alpha;
        public Vector3 Color;
        public bool Pulsing;
        #endregion

        #region Constructors
        public Beacon()
        {
            Scale = DEFAULT_SCALE;
            Alpha = 1f;
            Color = DEFAULT_COLOR;
        }
        #endregion

        #region Methods
        public void Animate(float time)
        {
            if (Pulsing)
            {
                if (Scale < SCALE_MAX)
                {
                    Scale += SCALING_SPEED * time;
                    Alpha = MathHelper.Clamp(Alpha - BLENDING_SPEED * time, 0f, 1f);
                }
                else
                {
                    Scale = DEFAULT_SCALE;
                    Alpha = 1f;
                    Pulsing = false;
                }
            }
        }
        #endregion
    }
}
