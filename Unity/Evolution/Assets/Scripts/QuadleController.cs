using UnityEngine;

public class QuadleController : MonoBehaviour
{
    public float DirectionChangeFrequency;
    public float MultiplyTimeInterval;
    public float MultiplyFrequency;
    public float MutateFrequency;
    public float GrowthFrequency;
    public float TimeToLive;
    public float KillRewardTimeToLiveSeconds;

    public GameObject ArmTop;
    public GameObject ArmRight;
    public GameObject ArmBottom;
    public GameObject ArmLeft;

    private Vector2 _directionMovement;
    private Vector2 _newDirection;
    private Rigidbody2D _rigidBody2D;
    private float _lastDirectionChange;
    private bool _isDying;
    private float _lastMultiplyTime;
    private float _startTime;
    private QuadleDna _dna;

    void Start()
    {
        _directionMovement = _newDirection = new Vector2(Random.Range(-1f, 1f), Random.Range(-1f, 1f)).normalized;
        _rigidBody2D = GetComponent<Rigidbody2D>();
        _lastDirectionChange = Time.time;
        _rigidBody2D.rotation = Random.Range(.5f, 6f);
        _lastMultiplyTime = Time.time;
        _startTime = Time.time;
    }

    public void SetDna(QuadleDna dna)
    {
        _dna = dna;
        int rnd = Random.Range(0, 100);

        if (rnd <= (int)(MutateFrequency * 100))
            Mutate();

        SetArms();

        float armTop = _dna.ArmTop * 0.1f;
        float armRight = _dna.ArmRight * 0.1f;
        float armBottom = _dna.ArmBottom * 0.1f;
        float armLeft = _dna.ArmLeft * 0.1f;
        GetComponent<SpriteRenderer>().color = new Color(1 - (armTop * armRight), .01f, 1 - (armBottom * armLeft));
    }

    private void Mutate()
    {
        int armTop = MutateArm(_dna.ArmTop);
        int armRight = MutateArm(_dna.ArmRight);
        int armBottom = MutateArm(_dna.ArmBottom);
        int armLeft = MutateArm(_dna.ArmLeft);

        _dna = new QuadleDna(armTop, armRight, armBottom, armLeft);
    }

    private int MutateArm(int currentArmLength)
    {
        int gf = (int)(GrowthFrequency * 100);
        int rnd = Random.Range(0, 100);

        if (rnd <= gf)
            return Mathf.Clamp(currentArmLength + 1, 0, 9);
        else
            return Mathf.Clamp(currentArmLength - 1, 0, 9);
    }

    private void SetArms()
    {
        ArmTop.transform.localPosition = new Vector3(0, 0.01f + (0.001f * _dna.ArmTop), 0);
        ArmTop.transform.localScale = new Vector3((_dna.ArmTop == 0) ? 0 : 0.2f, _dna.ArmTop * 0.1f, 1);

        ArmRight.transform.localPosition = new Vector3(0.01f + (0.001f * _dna.ArmRight), 0, 0);
        ArmRight.transform.localScale = new Vector3(_dna.ArmRight * 0.1f, (_dna.ArmRight == 0) ? 0 : 0.2f, 1);

        ArmBottom.transform.localPosition = new Vector3(0, -0.01f + (-0.001f * _dna.ArmBottom), 0);
        ArmBottom.transform.localScale = new Vector3((_dna.ArmBottom == 0) ? 0 : 0.2f, _dna.ArmBottom * 0.1f, 1);

        ArmLeft.transform.localPosition = new Vector3(-0.01f + (-0.001f * _dna.ArmLeft), 0, 0);
        ArmLeft.transform.localScale = new Vector3(_dna.ArmLeft * 0.1f, (_dna.ArmLeft == 0) ? 0 : 0.2f, 1);
    }

    void FixedUpdate()
    {
        if (Time.time - _startTime > TimeToLive)
            _isDying = true;

        if (_isDying)
        {
            transform.localScale = Vector3.Lerp(transform.localScale, Vector3.zero, 0.1f);

            if (transform.localScale.x < 0.05)
                Destroy(gameObject);

            return;
        }

        if (Time.time - _lastDirectionChange > DirectionChangeFrequency)
        {
            _newDirection = new Vector2(Random.Range(-1f, 1f), Random.Range(-1f, 1f)).normalized;
            _lastDirectionChange = Time.time;
        }

        _directionMovement = Vector2.Lerp(_directionMovement, _newDirection, 0.2f);
        _rigidBody2D.velocity = _directionMovement;

        if (Time.time - _lastMultiplyTime > MultiplyTimeInterval)
        {
            int mf = (int)(MultiplyFrequency * 100);
            int rnd = Random.Range(0, 100);

            if (rnd <= mf)
            {
                GameObject newQuadle = Instantiate(gameObject);
                newQuadle.transform.parent = transform.parent;
                QuadleController qc = newQuadle.GetComponent<QuadleController>();
                qc._startTime = Time.time;
                qc.SetDna(_dna);
            }

            _lastMultiplyTime = Time.time;
        }
    }

    public void OnArmCollision(QuadleController otherQuadle)
    {
        if (otherQuadle._dna != _dna)
        {
            if (Time.time - otherQuadle._startTime > 2 && Time.time - _startTime > 2)
            {
                _isDying = true;
                otherQuadle._startTime += KillRewardTimeToLiveSeconds;
            }
        }
    }

}
