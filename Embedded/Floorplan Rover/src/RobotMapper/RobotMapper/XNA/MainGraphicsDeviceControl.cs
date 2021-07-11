using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using RobotMapper.Objects;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using INPUT = Microsoft.Xna.Framework.Input;

namespace RobotMapper
{
    public partial class MainGraphicsDeviceControl : GraphicsDeviceControl
    {
        #region Settings
        public static float MARKER_CONNECT_RADIUS = 10f;
        #endregion

        #region Constants
        public const float GRID_UNIT_SIZE = 32f;
        #endregion

        #region Private Variables
        private Stopwatch _timer;
        private TimeSpan _lastTime;
        private TimeSpan _deltaTime;
        private ContentManager _content;
        private SpriteBatch _spriteBatch;
        private BasicEffect _effect;
        private SpriteFont _font;
        private float _height;
        private float _width;
        private float _halfHeight;
        private float _halfWidth;
        private Robot _robot;
        private Beacon _beacon;
        private List<Path> _path;
        private List<Marker> _markers;
        private List<VertexPositionColor> _lines;
        private List<VertexPositionColor> _grid;
        private KeyboardState _prevKeyState;
        private int _markerConnRad;
        #endregion

        #region Public Variables
        public Mutex _mutex;
        public Color BackgroundColor;
        public bool GridEnabled;
        public bool MarkersEnabled;
        public bool LinesEnabled;
        public bool RobotEnabled;
        public bool PathEnabled;
        #endregion

        #region Properties
        public int MarkerConnectionRadius
        {
            get { return _markerConnRad; }
            set 
            { 
                _markerConnRad = value;
                UpdateLines();
            }
        }
        #endregion

        #region Constructors
        public MainGraphicsDeviceControl()
        {
            BackgroundColor = Color.FromNonPremultiplied(63, 72, 204, 255);
        }
        #endregion

        #region Initialization
        protected override void Initialize()
        {
            InitGraphics();
            InitTime();
            LoadContent();
            InitObjects();
            InitGrid();
            InitSettings();
        }

        private void InitGraphics()
        {
            Application.Idle += delegate { Invalidate(); };
            _spriteBatch = new SpriteBatch(GraphicsDevice);
            
            _effect = new BasicEffect(GraphicsDevice);
            _effect.VertexColorEnabled = true;
            _effect.View = Matrix.CreateLookAt(new Vector3(0, 0, 1), Vector3.Zero, Vector3.Up);
            _effect.Projection = Matrix.CreateOrthographic(GraphicsDevice.Viewport.Width, GraphicsDevice.Viewport.Height, 100, -100);

            _height = GraphicsDevice.Viewport.Height;
            _width = GraphicsDevice.Viewport.Width;
            _halfHeight = GraphicsDevice.Viewport.Height / 2;
            _halfWidth = GraphicsDevice.Viewport.Width / 2;
        }

        private void InitTime()
        {
            _timer = Stopwatch.StartNew();
            _lastTime = new TimeSpan();
            _deltaTime = new TimeSpan();
        }

        private void LoadContent()
        {
            _content = new ContentManager(Services, "Content");
            _font = _content.Load<SpriteFont>("Fonts\\Arial");
            Beacon.Model = _content.Load<Model>("Models\\Beacon");
            Path.Model = _content.Load<Model>("Models\\Path");
            Robot.Model = _content.Load<Model>("Models\\Robot");
        }

        private void InitObjects()
        {
            _grid = new List<VertexPositionColor>();
            _markers = new List<Marker>();
            _lines = new List<VertexPositionColor>();
            _robot = new Robot();
            _beacon = new Beacon();
            _path = new List<Path>();
            _mutex = new Mutex();
        }

        private void InitGrid()
        {
            Color color = Color.FromNonPremultiplied(101, 109, 204, 255);

            for (int i = 0; i < 13; i++)
            {
                _grid.Add(new VertexPositionColor(new Vector3(i * GRID_UNIT_SIZE, 400, 0), color));
                _grid.Add(new VertexPositionColor(new Vector3(i * GRID_UNIT_SIZE, -400, 0), color));
            }

            for (int i = 0; i < 13; i++)
            {
                _grid.Add(new VertexPositionColor(new Vector3(-i * GRID_UNIT_SIZE, 400, 0), color));
                _grid.Add(new VertexPositionColor(new Vector3(-i * GRID_UNIT_SIZE, -400, 0), color));
            }

            for (int i = 0; i < 13; i++)
            {
                _grid.Add(new VertexPositionColor(new Vector3(-400, i * GRID_UNIT_SIZE, 0), color));
                _grid.Add(new VertexPositionColor(new Vector3(400, i * GRID_UNIT_SIZE, 0), color));
            }

            for (int i = 0; i < 13; i++)
            {
                _grid.Add(new VertexPositionColor(new Vector3(-400, -i * GRID_UNIT_SIZE, 0), color));
                _grid.Add(new VertexPositionColor(new Vector3(400, -i * GRID_UNIT_SIZE, 0), color));
            }
        }

        private void InitSettings()
        {
            GridEnabled = MarkersEnabled = LinesEnabled = RobotEnabled = PathEnabled = true;
            _markerConnRad = 20;
        }
        #endregion

        #region Misc Functions
        public void Reset()
        {
            _markers = new List<Marker>();
            _lines = new List<VertexPositionColor>();
        }

        Vector3 VectorLerp(Vector3 current, Vector3 target, float amount)
        {
            Vector3 result = current;
            result.X = result.X + (target.X - result.X) * amount;
            result.Y = result.Y + (target.Y - result.Y) * amount;
            result.Z = result.Z + (target.Z - result.Z) * amount;
            return result;
        }

        private void UpdateLines()
        {
            if (_markers == null || _markers.Count < 2) { return; }
            _lines = new List<VertexPositionColor>();
            float maxDistance = MARKER_CONNECT_RADIUS * GRID_UNIT_SIZE * (_markerConnRad / 100f);

            for (int i = 0; i < _markers.Count; i++)
            {
                Vector3 pos1 = _markers[i].Position;

                for (int j = 0; j < _markers.Count; j++)
                {
                    Vector3 pos2 = _markers[j].Position;
                    Vector3 delta = pos1 - pos2;
                    float distance = delta.Length();

                    if (distance <= maxDistance)
                    {
                        _lines.Add(new VertexPositionColor(pos1, Color.White));
                        _lines.Add(new VertexPositionColor(pos2, Color.White));
                    }
                }
            }
        }

        private void HandleInput()
        {
            KeyboardState keyState = Keyboard.GetState();

            // TEST CODE
            if (_prevKeyState.IsKeyDown(INPUT.Keys.W) && keyState.IsKeyUp(INPUT.Keys.W))
            {
                _robot.TargetPosition = _beacon.TargetPosition = _robot.Position + new Vector3(0, 1 * GRID_UNIT_SIZE, 0);
                _path.Add(new Path() { Position = _robot.Position });
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.S) && keyState.IsKeyUp(INPUT.Keys.S))
            {
                _robot.TargetPosition = _beacon.TargetPosition = _robot.Position + new Vector3(0, -1 * GRID_UNIT_SIZE, 0);
                _path.Add(new Path() { Position = _robot.Position });
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.A) && keyState.IsKeyUp(INPUT.Keys.A))
            {
                _robot.TargetPosition = _beacon.TargetPosition = _robot.Position + new Vector3(-1 * GRID_UNIT_SIZE, 0, 0);
                _path.Add(new Path() { Position = _robot.Position });
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.D) && keyState.IsKeyUp(INPUT.Keys.D))
            {
                _robot.TargetPosition = _beacon.TargetPosition = _robot.Position + new Vector3(1 * GRID_UNIT_SIZE, 0, 0);
                _path.Add(new Path() { Position = _robot.Position });
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.Q) && keyState.IsKeyUp(INPUT.Keys.Q))
            {
                _robot.TargetRotation = _beacon.TargetRotation = _robot.Rotation + MathHelper.PiOver4;
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.E) && keyState.IsKeyUp(INPUT.Keys.E))
            {
                _robot.TargetRotation = _beacon.TargetRotation = _robot.Rotation - MathHelper.PiOver4;
                _beacon.Pulsing = true;
            }

            if (_prevKeyState.IsKeyDown(INPUT.Keys.F) && keyState.IsKeyUp(INPUT.Keys.F))
            {
                Marker m = new Marker();
                Vector3 position = Vector3.Transform(new Vector3(0, 1 * GRID_UNIT_SIZE, 0), Matrix.CreateRotationZ(_robot.Rotation));
                position += _robot.Position;
                m.Translate(position);
                _markers.Add(m);
                UpdateLines();
            }
        
            _prevKeyState = keyState;
        }
        #endregion

        #region Robot Updates
        public void UpdateRobotPosition(double x, double y)
        {
            _mutex.WaitOne();
            _robot.TargetPosition = _beacon.TargetPosition = new Vector3((float)x * GRID_UNIT_SIZE, (float)y * GRID_UNIT_SIZE, 0);
            _path.Add(new Path() { Position = _robot.Position });
            _beacon.Pulsing = true;
            _mutex.ReleaseMutex();
        }

        public void UpdateRobotRotation(double rotation)
        {
            _mutex.WaitOne();
            _robot.TargetRotation = _beacon.TargetRotation = -MathHelper.ToRadians((float)rotation);
            _beacon.Pulsing = true;
            _mutex.ReleaseMutex();
        }
        #endregion

        #region Rendering
        private void UpdateTime()
        {
            _deltaTime = _timer.Elapsed - _lastTime;
            _lastTime = _timer.Elapsed;
        }

        private void ResetGraphics()
        {
            GraphicsDevice.Clear(BackgroundColor);
            GraphicsDevice.BlendState = BlendState.AlphaBlend;
            GraphicsDevice.DepthStencilState = DepthStencilState.DepthRead;

            _effect.World = Matrix.Identity;
            _effect.CurrentTechnique.Passes[0].Apply();
        }

        private void DrawGrid()
        {
            GraphicsDevice.DrawUserPrimitives(PrimitiveType.LineList, _grid.ToArray(), 0, _grid.Count / 2);
        }

        private void DrawMarkers()
        {
            foreach (Marker m in _markers)
                GraphicsDevice.DrawUserPrimitives(Marker.PrimitiveType, m.Vertices, 0, Marker.PrimitiveCount);
        }

        private void DrawLines()
        {
            if (_lines.Count > 1 && _lines.Count % 2 == 0)
                GraphicsDevice.DrawUserPrimitives(PrimitiveType.LineList, _lines.ToArray(), 0, _lines.Count / 2);
        }

        private void DrawRobot()
        {
            HandleInput();

            float lerpAmount = 0.01f * (float)_deltaTime.TotalMilliseconds;
            _robot.Position = VectorLerp(_robot.Position, _robot.TargetPosition, lerpAmount);
            _beacon.Position = VectorLerp(_beacon.Position, _beacon.TargetPosition, lerpAmount);
            _robot.Rotation = MathHelper.Lerp(_robot.Rotation, _robot.TargetRotation, lerpAmount);
            _beacon.Rotation = MathHelper.Lerp(_beacon.Rotation, _beacon.TargetRotation, lerpAmount);
            _beacon.Animate((float)_deltaTime.TotalMilliseconds);

            foreach (ModelMesh mesh in Beacon.Model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.DiffuseColor = _beacon.Color;
                    effect.Alpha = _beacon.Alpha;
                    effect.World = Matrix.CreateScale(_beacon.Scale) * Matrix.CreateRotationZ(_beacon.Rotation) * Matrix.CreateTranslation(_beacon.Position);
                    effect.View = _effect.View;
                    effect.Projection = _effect.Projection;
                }

                mesh.Draw();
            }

            foreach (ModelMesh mesh in Robot.Model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.DiffuseColor = _robot.Color;
                    effect.World = Matrix.CreateScale(_robot.Scale) * Matrix.CreateRotationZ(_robot.Rotation) * Matrix.CreateTranslation(_robot.Position);
                    effect.View = _effect.View;
                    effect.Projection = _effect.Projection;
                }

                mesh.Draw();
            }
        }

        private void DrawPath()
        {
            foreach (Path path in _path)
            {
                foreach (ModelMesh mesh in Path.Model.Meshes)
                {
                    foreach (BasicEffect effect in mesh.Effects)
                    {
                        effect.DiffuseColor = path.Color;
                        effect.Alpha = path.Alpha;
                        effect.World = Matrix.CreateScale(path.Scale) * Matrix.CreateTranslation(path.Position);
                        effect.View = _effect.View;
                        effect.Projection = _effect.Projection;
                    }

                    mesh.Draw();
                }
            }
        }

        protected override void Draw()
        {
            UpdateTime();
            ResetGraphics();

            _mutex.WaitOne();
            if (GridEnabled) { DrawGrid(); }
            if (MarkersEnabled) { DrawMarkers(); }
            if (LinesEnabled) { DrawLines(); }
            if (PathEnabled) { DrawPath(); }
            if (RobotEnabled) { DrawRobot(); }
            _mutex.ReleaseMutex();
        }
        #endregion
    }
}
