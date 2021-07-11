using UnityEngine;
using XInputDotNetPure;

public class PlayerController : MonoBehaviour 
{
    public GameObject Basketballs;
    public GameObject Basketball;
    public GameObject Player;
    public GameObject ShotIndicator;
    public GameObject PowerIndicator;
    public GameObject SpinIndicator;
    public GameObject Reticle;
    public GameObject MainCameraGameObject;
    public GameObject CloseUpCameraGameObject;
    public GameObject BasketBallCameraContainerGameObject;

    private bool _isShooting;
    private float _powerScaleIncrement;
    private float _powerScale;
    private float _power;

    private Camera _mainCamera;
    private Camera _closeUpCamera;
    private Camera _ballCamera;

    private BallCameraController _ballCameraController;

    private Rigidbody _ballRigidbody;

	void Start() 
	{
        _mainCamera = MainCameraGameObject.GetComponent<Camera>();
        _closeUpCamera = CloseUpCameraGameObject.GetComponent<Camera>();
        _ballCamera = BasketBallCameraContainerGameObject.GetComponentInChildren<Camera>();
        _ballCameraController = BasketBallCameraContainerGameObject.GetComponent<BallCameraController>();
    }
	
	void Update() 
	{
        GamePadState gps = GamePad.GetState(PlayerIndex.One);

        if (gps.Buttons.Y == ButtonState.Pressed)
        {
            _mainCamera.enabled = false;
            _ballCamera.enabled = false;
            _closeUpCamera.enabled = true;
            return;
        }
        else if (gps.Buttons.B == ButtonState.Pressed)
        {
            _mainCamera.enabled = false;
            _closeUpCamera.enabled = false;
            _ballCamera.enabled = true;
            return;
        }
        else
        {
            _closeUpCamera.enabled = false;
            _ballCamera.enabled = false;
            _mainCamera.enabled = true;
        }

        float rt = gps.Triggers.Right;
        float lt = gps.Triggers.Left;

        bool isShooting = rt > 0.05f;
        ShotIndicator.SetActive(isShooting);
        Reticle.SetActive(isShooting);

        SpinIndicator.transform.Rotate(0, (50f + (lt * 600f)) * Time.deltaTime, 0);
        float spin = lt;

        float distance = Vector3.Distance(ShotIndicator.transform.position - new Vector3(0, ShotIndicator.transform.position.y, 0), transform.position - new Vector3(0, transform.position.y, 0));

        if (_isShooting)
        {
            if (isShooting)
            {
                _powerScaleIncrement += (lt > 0.05f) ? 25f * (1 / Mathf.Min(distance, 12.5f)) * Time.deltaTime : 17f * (1 / Mathf.Min(distance, 12.5f)) * Time.deltaTime;
                _powerScale = 0.005f + (_powerScaleIncrement * 0.065f);
                float scale = Mathf.Min(_powerScale, 0.07f);
                PowerIndicator.transform.localScale = Vector3.Lerp(new Vector3(scale, scale, scale), PowerIndicator.transform.localScale, 0.75f);
                _power = _powerScaleIncrement / 1.0f;
            }
            else
            {
                GameObject ball = null;

                if (_ballRigidbody == null)
                {
                    ball = GameObject.Instantiate(Basketball);
                    ball.transform.parent = Basketballs.transform;
                    _ballRigidbody = ball.GetComponent<Rigidbody>();
                }
                else
                {
                    ball = _ballRigidbody.gameObject;
                }

                _ballRigidbody.velocity = new Vector3();
                _ballRigidbody.angularVelocity = new Vector3();
                _ballRigidbody.maxAngularVelocity = float.MaxValue;

                Vector3 moveDirection = Vector3.Normalize(Camera.main.transform.forward);

                GamePadState gp = GamePad.GetState(PlayerIndex.One);
                moveDirection = Vector3.Normalize(Vector3.RotateTowards(moveDirection, Vector3.up, Mathf.Deg2Rad * 30, 0));

                ball.transform.position = Player.transform.position + moveDirection + new Vector3(0, 1, 0);

                if (distance < 7)
                    moveDirection *= 7.0f;
                else if (distance < 9)
                    moveDirection *= 8.0f;
                else if (distance < 11)
                    moveDirection *= 8.5f;
                else if (distance < 13)
                    moveDirection *= 8.75f;
                else if (distance < 15)
                    moveDirection *= 9.0f;
                else if (distance < 17)
                    moveDirection *= 9.25f;
                else if (distance < 19)
                    moveDirection *= 9.5f;
                else if (distance < 21)
                    moveDirection *= 9.9f;
                else if (distance < 23)
                    moveDirection *= 10.2f;
                else if (distance < 25)
                    moveDirection *= 10.4f;
                else if (distance < 27)
                    moveDirection *= 10.65f;
                else if (distance < 29)
                    moveDirection *= 10.8f;
                else
                    moveDirection *= 12.0f;

                moveDirection *= Mathf.Min(_power, 1.2f);

                float rotationForce = Mathf.Max(0.1f, spin);

                _ballRigidbody.AddForce(moveDirection, ForceMode.Impulse);
                _ballRigidbody.AddTorque(Vector3.Cross(moveDirection, Vector3.up) * rotationForce * .4f, ForceMode.Impulse);

                _isShooting = false;
                _powerScaleIncrement = _powerScale = _power = 0;
                PowerIndicator.transform.localScale = new Vector3(.005f, .005f, .005f);

                _ballCameraController.SetBasketBall(ref _ballRigidbody);
            }
        }
        else
        {
            if (isShooting) _isShooting = true;
        }

        if (Input.GetKeyUp(KeyCode.Escape))
            Application.Quit();
    }

}
