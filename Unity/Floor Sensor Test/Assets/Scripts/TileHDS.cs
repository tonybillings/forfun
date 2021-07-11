using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class TileHDS
{
    private const float INACTIVE_TIME_THRESHOLD = 1f;

    public int OccupancyCount { get; private set; }
    public bool IsActive { get; private set; }

    private List<TileSensor> _sensors;
    private int _rowCount;
    private int _colCount;
    private int _firstActiveZone;
    private int _lastActiveZone;
    private bool _isGoingInactive;
    private float _lastActiveTime;

    public TileHDS(int rowCount, int colCount)
    {
        _sensors = new List<TileSensor>();
        _rowCount = rowCount;
        _colCount = colCount;
    }

    public void AddSensor(TileSensor sensor)
    {
        _sensors.Add(sensor);
    }

    public void Update(float time)
    {
        if (!IsActive)
        {
            foreach (TileSensor sensor in _sensors.Where(s => s.IsActive == true))
            {
                int centerLine = _rowCount / 2;

                if (sensor.Location.x < centerLine)
                {
                    _firstActiveZone = 2;
                    IsActive = true;
                    break;
                }
                else if (sensor.Location.x > centerLine)
                {
                    _firstActiveZone = 1;
                    IsActive = true;
                    break;
                }
                else if (sensor.Location.x == centerLine)
                {
                    _firstActiveZone = 3;
                    IsActive = true;
                    break;
                }
            }
        }
        else
        {
            if (_sensors.Where(s => s.IsActive == true).Count() == 0)
            {
                if (_isGoingInactive)
                {
                    if (time - _lastActiveTime > INACTIVE_TIME_THRESHOLD)
                    {
                        if ((_firstActiveZone == 1 || _firstActiveZone == 3) && _lastActiveZone == 2)
                            OccupancyCount++;
                        else if ((_firstActiveZone == 2 || _firstActiveZone == 3) && _lastActiveZone == 1)
                            OccupancyCount--;

                        _firstActiveZone = _lastActiveZone = 0;
                        _isGoingInactive = false;
                        _lastActiveTime = 0;
                        IsActive = false;
                    }
                }
                else
                {
                    _lastActiveTime = time;
                    _isGoingInactive = true;
                }
            }
            else
            {
                foreach (TileSensor sensor in _sensors.Where(s => s.IsActive == true))
                {
                    int centerLine = _rowCount / 2;

                    if (sensor.Location.x < centerLine)
                    {
                        _lastActiveZone = 2;
                        break;
                    }
                    else if (sensor.Location.x > centerLine)
                    {
                        _lastActiveZone = 1;
                        break;
                    }
                }

                _isGoingInactive = false;
                _lastActiveTime = 0;
            }
        }
    }
}
