using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace RobotMapper.Objects
{
    public class Path
    {
        #region Constants
        private const float DEFAULT_SCALE = 0.66f;
        private const float DEFAULT_ALPHA = 0.5f;
        private readonly Vector3 DEFAULT_COLOR = new Vector3(1f, 0.4f, 0.4f);
        #endregion

        #region Public Variables
        public static Model Model;
        public Vector3 Position;
        public float Scale;
        public float Alpha;
        public Vector3 Color;
        #endregion

        #region Constructors
        public Path()
        {
            Scale = DEFAULT_SCALE;
            Alpha = DEFAULT_ALPHA;
            Color = DEFAULT_COLOR;
        }
        #endregion
    }
}
