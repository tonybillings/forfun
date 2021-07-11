using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class MatManager : MonoBehaviour 
{
    public int RowCount = 5;
    public int ColCount = 5;
    public float TileGap = 0.04f;
    public float TileSize = 0.32f;
    public GameObject TileSensorPrefab;
    public GameObject TileModelPrefab;
    public Rect GUILabelRect;
    public string AreaName;
    public Font GUIFont;

    private List<TileSensor> _sensors;
    private List<Texture> _tileTextures;
    private TileHDS _tileHds;
    private int _activeSensorCount;
    private bool _prevTileHdsIsActive;

    private void InitMat()
    {
        _sensors = new List<TileSensor>();
        _tileHds = new TileHDS(RowCount, ColCount);
        GameObject mat = this.gameObject;

        for (int x = 0; x < RowCount; x++)
        {
            for (int y = 0; y < ColCount; y++)
            {
                GameObject tileSensor = Instantiate(TileSensorPrefab);
                tileSensor.transform.parent = mat.transform;
                tileSensor.transform.localPosition = new Vector3(x * TileSize + (x * TileGap), 0, y * TileSize + (y * TileGap));
                TileSensor sensor = tileSensor.GetComponent<TileSensor>();
                sensor.Location = new Vector2(x, y);
                _tileHds.AddSensor(sensor);
                _sensors.Add(sensor);

                GameObject tileModel = Instantiate(TileModelPrefab);
                tileModel.transform.parent = mat.transform;
                tileModel.transform.localPosition = new Vector3(x * TileSize + (x * TileGap), -10.0f, y * TileSize + (y * TileGap));
                tileModel.GetComponent<Renderer>().material.mainTexture = _tileTextures[0];
                sensor.TileModel = tileModel;
            }
        }
    }

    private void LoadTextures()
    {
        _tileTextures = new List<Texture>();
        _tileTextures.Add((Texture)Resources.Load("Models/TileSensor/Textures/TileTexture_Black", typeof(Texture)));

        for (int i = 1; i <= 45; i++)
            _tileTextures.Add((Texture)Resources.Load("Models/TileSensor/Textures/TileTexture_" + i, typeof(Texture)));
    }

    private void UpdateTileModels()
    {
        if (_tileHds.IsActive)
        {
            foreach (TileSensor sensor in _sensors.Where(s => s.IsActive == true))
            {
                Material mat = sensor.TileModel.GetComponent<Renderer>().material;

                if (mat.mainTexture == _tileTextures[0])
                {
                    _activeSensorCount++;
                    mat.mainTexture = _tileTextures[_activeSensorCount];
                }
            }
        }
        else if (_prevTileHdsIsActive && !_tileHds.IsActive)
        {
            foreach (TileSensor sensor in _sensors)
            {
                Material mat = sensor.TileModel.GetComponent<Renderer>().material;

                if (mat.mainTexture != _tileTextures[0])
                    mat.mainTexture = _tileTextures[0];
            }

            _activeSensorCount = 0;
        }

        _prevTileHdsIsActive = _tileHds.IsActive;
    }

	private void Start() 
	{
        LoadTextures();
        InitMat();
	}
	
	private void Update() 
	{
        _tileHds.Update(Time.timeSinceLevelLoad);
        UpdateTileModels();
	}

    void OnGUI()
    {
        GUIStyle style = new GUIStyle();
        style.font = GUIFont;
        style.normal.textColor = Color.white;
        GUI.Label(GUILabelRect, "Room " + AreaName + ": " + _tileHds.OccupancyCount, style);
    }
}