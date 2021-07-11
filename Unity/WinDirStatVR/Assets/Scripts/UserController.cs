using UnityEngine;
using XInputDotNetPure;

public class UserController : MonoBehaviour
{
    #region Private Enums
    private enum ViewState { Folders, Files }
    #endregion

    #region Public Variables
    public Camera MainCamera;
    public GameObject FolderInfo;
    public TextMesh DisplayText;
    public GameObject DisplayTextFrame;
    public GameObject SelectionBlock;
    public Skybox SunnyDaySkybox;
    public GameObject SpaceSkybox;
    public Renderer SceneChangePaneRenderer;
    public GameObject FilesView;
    public GameObject Blocks;
    #endregion

    #region Private Variables
    private ViewState _viewState;
    private GamePadState _prevGamePadState;
    private MeshRenderer _lastSelectedMesh;
    private Renderer _displayTextRenderer;
    private bool _slowMode;
    private Vector3 _targetFolderPosition;
    private bool _openingFolder;
    private float _sceneChangeAlpha;
    private bool _lockSelection;
    private GameObject _selectedFolder;
    private FilesViewManager _filesViewManager;
    private Vector3 _lastPosition;
    private Quaternion _lastRotation;
    #endregion

    #region Private Methods
    private void Start()
    {
        _viewState = ViewState.Folders;
        _displayTextRenderer = DisplayText.GetComponent<TextMesh>().GetComponent<Renderer>();
        _filesViewManager = FilesView.GetComponent<FilesViewManager>();
    }

    private void Update()
    {
        GamePadState gamePadState = GamePad.GetState(PlayerIndex.One);

        switch (_viewState)
        {
            case ViewState.Folders:
                HandleInputFolderView(gamePadState);
                break;
            case ViewState.Files:
                HandleInputFileView(gamePadState);
                break;
        }

        HandleInputGlobal(gamePadState);

        _prevGamePadState = gamePadState;
    }

    private void HandleInputFolderView(GamePadState gamePadState)
    {
        if (_openingFolder)
        {
            TransitionToFilesView();
            return;
        }

        UpdateSkybox(gamePadState);
        UpdatePositionFolderView(gamePadState);
        UpdateRotationFolderView(gamePadState);
        UpdateSelectionFolderView(gamePadState);
    }

    private void HandleInputFileView(GamePadState gamePadState)
    {
        if (gamePadState.Buttons.LeftShoulder == ButtonState.Pressed && _prevGamePadState.Buttons.LeftShoulder == ButtonState.Released)
        {
            this.transform.position = _lastPosition;
            this.transform.rotation = _lastRotation;
            _viewState = ViewState.Folders;
            Blocks.SetActive(true);
            FilesView.SetActive(false);
        }
    }

    private void HandleInputGlobal(GamePadState gamePadState)
    {
        if (gamePadState.Buttons.Back == ButtonState.Pressed && _prevGamePadState.Buttons.Back == ButtonState.Released)
            Application.Quit();
    }

    private bool GetSlowMode(GamePadState gamePadState)
    {
        bool slowMode = gamePadState.Buttons.LeftStick == ButtonState.Pressed || gamePadState.Buttons.RightStick == ButtonState.Pressed;

        if (gamePadState.Buttons.Y == ButtonState.Pressed && _prevGamePadState.Buttons.Y == ButtonState.Released)
            _slowMode = !_slowMode;

        return (slowMode || _slowMode);
    }

    private void TransitionToFilesView()
    {
        FolderInfo.SetActive(false);
        this.transform.position = Vector3.Lerp(this.transform.position, _targetFolderPosition, 2.0f * Time.deltaTime);
        float distance = (_targetFolderPosition - this.transform.position).magnitude;

        if (distance < 2)
        {
            _openingFolder = false;

            this.transform.position = new Vector3(0, -495, 0);
            this.transform.rotation = Quaternion.LookRotation((FilesView.transform.position + new Vector3(0, 5, 0)) - this.transform.position, this.transform.up);

            FolderMetaData fmd = _selectedFolder.GetComponent<FolderMetaData>();
            _filesViewManager.SetRoomMaterial(fmd.MaterialIndex);

            Blocks.SetActive(false);
            FilesView.SetActive(true);
            _viewState = ViewState.Files;

            _sceneChangeAlpha = 0f;
            SceneChangePaneRenderer.material.color = new Color(1f, 1f, 1f, _sceneChangeAlpha);
        }
        else if (distance < 50f)
        {
            _sceneChangeAlpha += 1f * Time.deltaTime;
            SceneChangePaneRenderer.material.color = new Color(1f, 1f, 1f, _sceneChangeAlpha);
        }
    }

    private void UpdateSkybox(GamePadState gamePadState)
    {
        if (gamePadState.DPad.Down == ButtonState.Pressed)
        {
            SunnyDaySkybox.enabled = true;
            SpaceSkybox.SetActive(false);
            MainCamera.clearFlags = CameraClearFlags.Skybox;
        }
        else if (gamePadState.DPad.Up == ButtonState.Pressed)
        {
            SunnyDaySkybox.enabled = false;
            SpaceSkybox.SetActive(true);
            MainCamera.clearFlags = CameraClearFlags.Skybox;
        }
        else if (gamePadState.DPad.Left == ButtonState.Pressed)
        {
            SunnyDaySkybox.enabled = false;
            SpaceSkybox.SetActive(false);
            MainCamera.clearFlags = CameraClearFlags.Color;
            MainCamera.backgroundColor = Color.white;
        }
        else if (gamePadState.DPad.Right == ButtonState.Pressed)
        {
            SunnyDaySkybox.enabled = false;
            SpaceSkybox.SetActive(false);
            MainCamera.clearFlags = CameraClearFlags.Color;
            MainCamera.backgroundColor = Color.black;
        }
    }

    private void UpdatePositionFolderView(GamePadState gamePadState)
    {
        bool slowMode = GetSlowMode(gamePadState);

        float moveSpeed = (slowMode || _slowMode) ? 2f : 20f;

        float forwardSpeed = gamePadState.ThumbSticks.Left.Y * moveSpeed * Time.deltaTime;
        float strafeSpeed = gamePadState.ThumbSticks.Left.X * moveSpeed * Time.deltaTime;
        float upSpeed = gamePadState.Triggers.Right * moveSpeed * Time.deltaTime;
        float downSpeed = gamePadState.Triggers.Left * moveSpeed * Time.deltaTime;

        this.transform.position += this.transform.forward * forwardSpeed;
        this.transform.position += this.transform.right * strafeSpeed;
        this.transform.position += Vector3.up * upSpeed;
        this.transform.position -= Vector3.up * downSpeed;
    }

    private void UpdateRotationFolderView(GamePadState gamePadState)
    {
        float rotateSpeed = gamePadState.ThumbSticks.Right.X * 100f * Time.deltaTime;
        this.transform.Rotate(Vector3.up, rotateSpeed);
    }

    private void UpdateSelectionFolderView(GamePadState gamePadState)
    {
        if (gamePadState.Buttons.A == ButtonState.Pressed)
        {
            RaycastHit hit;
            Ray ray = MainCamera.ViewportPointToRay(new Vector3(0.5f, 0.5f, 0));

            if (Physics.Raycast(ray, out hit))
            {
                Transform objectHit = hit.transform;

                if (objectHit.tag == "B")
                {
                    _selectedFolder = objectHit.gameObject;

                    if (_lastSelectedMesh != null) _lastSelectedMesh.enabled = true;
                    _lastSelectedMesh = objectHit.gameObject.GetComponent<MeshRenderer>();
                    _lastSelectedMesh.enabled = false;

                    DisplayText.text = objectHit.gameObject.name;

                    SelectionBlock.transform.position = objectHit.position;
                    SelectionBlock.transform.localScale = objectHit.localScale;

                    float distance = Mathf.Abs(Vector3.Magnitude(transform.position - objectHit.position));
                    float pct = (distance - 0.3f) / 69.7f;
                    float scale = Mathf.Clamp((pct * 0.99f) + 0.01f, 0.01f, 1f);

                    FolderInfo.transform.localScale = new Vector3(scale, scale, scale);
                    FolderInfo.transform.position = objectHit.position + new Vector3(0, (objectHit.transform.localScale.y * 0.5f) + FolderInfo.transform.localScale.y + 0.1f, 0);
                    DisplayTextFrame.transform.localScale = new Vector3((DisplayText.text.Length / 6f) * 0.5f, DisplayTextFrame.transform.localScale.y, DisplayTextFrame.transform.localScale.z);
                    FolderInfo.transform.rotation = MainCamera.transform.rotation;

                    _targetFolderPosition = objectHit.position;
                    
                    SelectionBlock.SetActive(true);
                    FolderInfo.SetActive(true);
                }
                else
                {
                    _selectedFolder = null;
                }
            }
            else
            {
                _selectedFolder = null;
            }
        }
        else
        {
            if (!_lockSelection)
            {
                _selectedFolder = null;
                DisplayText.text = string.Empty;
                SelectionBlock.SetActive(false);
                FolderInfo.SetActive(false);
                if (_lastSelectedMesh != null) _lastSelectedMesh.enabled = true;
            }
        }

        if (gamePadState.Buttons.RightShoulder == ButtonState.Pressed && _prevGamePadState.Buttons.RightShoulder == ButtonState.Released)
        {
            _lockSelection = !_lockSelection;
        }

        if (gamePadState.Buttons.LeftShoulder == ButtonState.Pressed && _selectedFolder != null)
        {
            _openingFolder = true;
            _lastPosition = this.transform.position;
            _lastRotation = this.transform.rotation;
        }
    }
    #endregion
}
