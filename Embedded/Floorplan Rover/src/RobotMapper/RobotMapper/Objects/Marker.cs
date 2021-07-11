using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace RobotMapper.Objects
{
    public class Marker
    {
        #region Constants
        private const float DEFAULT_SCALE = 32f;
        #endregion

        #region Private Variables
        private float _scale;
        private Vector3 _position;
        private VertexPositionColor[] _vertices;
        #endregion

        #region Public Variables
        public readonly static PrimitiveType PrimitiveType = PrimitiveType.LineList;
        public readonly static int PrimitiveCount = 2;
        #endregion

        #region Properties
        public Vector3 Position
        {
            get { return _position; }
        }        
        
        public VertexPositionColor[] Vertices
        {
            get { return _vertices; }
        }
        #endregion

        #region Constructors
        public Marker()
        {
            _scale = DEFAULT_SCALE;
            _vertices = new VertexPositionColor[4] {
                new VertexPositionColor(new Vector3(-.5f, .5f, 0), Color.Red),
                new VertexPositionColor(new Vector3(.5f, -.5f, 0), Color.Red),
                new VertexPositionColor(new Vector3(.5f, .5f, 0), Color.Red),
                new VertexPositionColor(new Vector3(-.5f, -.5f, 0), Color.Red)
            };

            Scale(DEFAULT_SCALE);
        }
        #endregion

        #region Public Methods
        public void Scale(float scale)
        {
            _scale = scale;

            for (int i = 0; i < Vertices.Length; i++)
                _vertices[i].Position *= scale * 0.5f;
        }

        public void Translate(Vector3 translate)
        {
            _position = translate;

            for (int i = 0; i < Vertices.Length; i++)
                _vertices[i].Position += _position;
        }
        #endregion
    }
}
