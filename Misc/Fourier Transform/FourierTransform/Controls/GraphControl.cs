using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Diagnostics;

namespace FourierTransform
{
    public partial class GraphControl : GraphicsDeviceControl
    {
        #region Events
        public delegate void InitializedHandler(GraphControl sender);
        public event InitializedHandler Initialized;
        public delegate void DataChangedHandler(GraphControl sender);
        public event DataChangedHandler DataChanged;
        #endregion

        #region Declarations
        Stopwatch _timer;
        SpriteBatch _spriteBatch;
        BasicEffect _effect;
        VertexPositionColor[] _axes;
        float _height;
        float _width;
        float _halfHeight;
        float _halfWidth;
        List<WaveControl> _waves;
        bool _compositeVisible;
        VertexPositionColor[] _compositeDisplayPoints;
        VertexPositionColor[] _compositePoints;
        #endregion

        #region Properties
        public bool CompositeVisible
        {
            get { return _compositeVisible; }
            set { _compositeVisible = value; }
        }

        public VertexPositionColor[] CompositePoints
        {
            get { return _compositePoints; }
        }
        #endregion

        #region Constructors
        public GraphControl()
        {
            _waves = new List<WaveControl>();
            _compositeVisible = true;
        }
        #endregion

        #region Initialization
        protected override void Initialize()
        {
            _timer = Stopwatch.StartNew();
            _spriteBatch = new SpriteBatch(GraphicsDevice);
            _compositePoints = new VertexPositionColor[WaveControl.PRIMITIVE_COUNT + 1];

            Application.Idle += delegate { Invalidate(); };
            
            _effect = new BasicEffect(GraphicsDevice);
            _effect.VertexColorEnabled = true;
            _effect.Projection = Matrix.CreateOrthographicOffCenter(0, GraphicsDevice.Viewport.Width, GraphicsDevice.Viewport.Height, 0, 0, 1);                                         // near, far plane

            _height = GraphicsDevice.Viewport.Height;
            _width = GraphicsDevice.Viewport.Width;
            _halfHeight = GraphicsDevice.Viewport.Height / 2;
            _halfWidth = GraphicsDevice.Viewport.Width / 2;

            InitializeAxes();

            if (Initialized != null)
                Initialized(this);
        }

        private void InitializeAxes()
        {
            _axes = new VertexPositionColor[164];

            _axes[0].Position = new Vector3(_halfWidth, 0, 0);
            _axes[0].Color = Color.DarkOrange;
            _axes[1].Position = new Vector3(_halfWidth, _height, 0);
            _axes[1].Color = Color.DarkOrange;

            _axes[2].Position = new Vector3(0, _halfHeight, 0);
            _axes[2].Color = Color.DarkOrange;
            _axes[3].Position = new Vector3(_width, _halfHeight, 0);
            _axes[3].Color = Color.DarkOrange;

            float offset = 10;
            for (int i = 0; i < 40; i += 2)
            {
                _axes[4 + i].Position = new Vector3(_halfWidth - 1.5f, _halfHeight, 0) - new Vector3(0, offset, 0);
                _axes[4 + i].Color = Color.Orange;

                _axes[4 + i + 1].Position = _axes[4 + i].Position + new Vector3(3, 0, 0);
                _axes[4 + i + 1].Color = Color.Orange;

                offset += 10;
            }

            offset = 10;
            for (int i = 0; i < 40; i += 2)
            {
                _axes[44 + i].Position = new Vector3(_halfWidth - 1.5f, _halfHeight, 0) + new Vector3(0, offset, 0);
                _axes[44 + i].Color = Color.Orange;

                _axes[44 + i + 1].Position = _axes[44 + i].Position + new Vector3(3, 0, 0);
                _axes[44 + i + 1].Color = Color.Orange;

                offset += 10;
            }

            offset = 10;
            for (int i = 0; i < 40; i += 2)
            {
                _axes[84 + i].Position = new Vector3(_halfWidth, _halfHeight - 1.5f, 0) + new Vector3(offset, 0, 0);
                _axes[84 + i].Color = Color.Orange;

                _axes[84 + i + 1].Position = _axes[84 + i].Position + new Vector3(0, 3, 0);
                _axes[84 + i + 1].Color = Color.Orange;

                offset += 10;
            }

            offset = 10;
            for (int i = 0; i < 40; i += 2)
            {
                _axes[124 + i].Position = new Vector3(_halfWidth, _halfHeight - 1.5f, 0) - new Vector3(offset, 0, 0);
                _axes[124 + i].Color = Color.Orange;

                _axes[124 + i + 1].Position = _axes[124 + i].Position + new Vector3(0, 3, 0);
                _axes[124 + i + 1].Color = Color.Orange;

                offset += 10;
            }
        }
        #endregion

        #region Waves
        public void AddWave(WaveControl wave)
        {
            _waves.Add(wave);
            Random rnd = new Random((int)DateTime.Now.Ticks);
            wave.BackColor = System.Drawing.Color.FromArgb(rnd.Next(0, 256), rnd.Next(0, 256), rnd.Next(0, 256));
            wave.DeleteClicked += WaveControl_DeleteClicked;
            wave.DataChanged += WaveControl_DataChanged;
            wave.WaveEnabledChanged += WaveControl_EnabledChanged;
            wave.Initialize(_width, _height);
        }

        private void WaveControl_DeleteClicked(WaveControl wave)
        {
            for (int i = _waves.Count - 1; i >= 0; i--)
            {
                if (_waves[i] == wave)
                {
                    _waves.Remove(wave);
                    UpdateCompositeWave();
                    return;
                }
            }
        }

        public void WaveControl_DataChanged(WaveControl wave)
        {
            UpdateCompositeWave();
        }

        private void WaveControl_EnabledChanged(WaveControl wave)
        {
            UpdateCompositeWave();
        }

        private void UpdateCompositeWave()
        {
            _compositePoints = new VertexPositionColor[WaveControl.PRIMITIVE_COUNT + 1];
            foreach (WaveControl w in _waves.Where(w => w.WaveEnabled))
            {
                for (int i = 0; i < w.Points.Length; i++)
                {
                    _compositePoints[i].Position.X = w.Points[i].Position.X;
                    _compositePoints[i].Position.Y += w.Points[i].Position.Y;
                    _compositePoints[i].Color = Color.White;
                }
            }

            _compositeDisplayPoints = new VertexPositionColor[_compositePoints.Length];
            for (int i = 0; i < _compositePoints.Length; i++)
            {
                _compositeDisplayPoints[i].Position = _compositePoints[i].Position + new Vector3(-51.5f, _halfHeight, 0);
                _compositeDisplayPoints[i].Color = Color.White;
            }

            if (DataChanged != null)
                DataChanged(this);
        }
        #endregion

        #region Graphics
        protected override void Draw()
        {
            GraphicsDevice.Clear(Color.Gray);
            float time = (float)_timer.Elapsed.TotalSeconds;
            _effect.CurrentTechnique.Passes[0].Apply();
            GraphicsDevice.DrawUserPrimitives<VertexPositionColor>(PrimitiveType.LineList, _axes, 0, 82);
            
            foreach (WaveControl wave in _waves.Where(w => w.WaveVisible))
                GraphicsDevice.DrawUserPrimitives<VertexPositionColor>(PrimitiveType.LineStrip, wave.DisplayPoints, 0, WaveControl.PRIMITIVE_COUNT);

            if (_compositeVisible)
                GraphicsDevice.DrawUserPrimitives<VertexPositionColor>(PrimitiveType.LineStrip, _compositeDisplayPoints, 0, WaveControl.PRIMITIVE_COUNT);
        }
        #endregion

    }
}
