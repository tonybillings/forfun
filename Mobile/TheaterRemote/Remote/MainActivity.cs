using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Support.V7.App;
using Android.Views;
using Android.Widget;
using System;
using System.Timers;

namespace Remote
{
    [Activity(Icon = "@drawable/icon", Label = "@string/app_name", Theme = "@style/AppTheme", MainLauncher = true)]
    public class MainActivity : AppCompatActivity
    {
        #region Enums
        private enum ControllerMode { Roku, Projector, Receiver }
        #endregion

        #region Private Variables
        private View _rootView;
        private ControllerMode _controllerMode;
        private LinearLayout _powerMenu;
        private LinearLayout _inputSourceMenu;
        private LinearLayout _audioVideoOptionsMenu;
        private LinearLayout _mainMenu;
        private Button _starButton;
        private Button _rewindButton;
        private Button _playPauseButton;
        private Button _forwardButton;
        private Button _menuButton;
        private Color _redColor;
        private Color _greenColor;
        private Color _blueColor;
        private Color _orangeColor;
        private Color _lightGrayColor;
        private Color _darkGrayColor;
        private bool _volumeChanging;
        private Timer _volumeUpTimer;
        private Timer _volumeDownTimer;
        #endregion

        #region Override Methods
        protected override void OnCreate(Bundle savedInstanceState)
        {
            RequestWindowFeature(WindowFeatures.NoTitle);
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.activity_main);

            InitVariables();
            InitButtons();
            SetBackgroundColor();
        }
        #endregion

        #region Private Methods
        private void InitVariables()
        {
            _controllerMode = ControllerMode.Roku;

            _powerMenu = FindViewById<LinearLayout>(Resource.Id.PowerMenuView);
            _inputSourceMenu = FindViewById<LinearLayout>(Resource.Id.InputSourceMenuView);
            _audioVideoOptionsMenu = FindViewById<LinearLayout>(Resource.Id.AudioVideoOptionsMenuView);
            _mainMenu = FindViewById<LinearLayout>(Resource.Id.MainMenuView);

            _starButton = FindViewById<Button>(Resource.Id.ControllerStarButton);
            _rewindButton = FindViewById<Button>(Resource.Id.ControllerRewindButton);
            _playPauseButton = FindViewById<Button>(Resource.Id.ControllerPlayPauseButton);
            _forwardButton = FindViewById<Button>(Resource.Id.ControllerForwardButton);
            _menuButton = FindViewById<Button>(Resource.Id.ControllerMenuButton);

            _redColor = Color.ParseColor("#FF7373");
            _greenColor = Color.ParseColor("#00D900");
            _blueColor = Color.ParseColor("#6C03FC");
            _orangeColor = Color.ParseColor("#FFA64D");
            _lightGrayColor = Color.ParseColor("#CCCCCC");
            _darkGrayColor = Color.ParseColor("#555555");

            _volumeChanging = false;
            _volumeUpTimer = new Timer();
            _volumeDownTimer = new Timer();
            _volumeUpTimer.Interval = _volumeDownTimer.Interval = 50;
            _volumeUpTimer.Elapsed += (sender, e) =>
            {
                ProcessReceiverCommand("VolumeUp");
            };
            _volumeDownTimer.Elapsed += (sender, e) =>
            {
                ProcessReceiverCommand("VolumeDown");
            };
        }

        private void InitButtons()
        {
            View mainContainer = FindViewById(Resource.Id.MainContainer);

            void SetButtonEvents(Button b)
            {
                b.Click += ButtonClicked;

                if (!b.Tag.ToString().StartsWith("MENU"))
                    b.Touch += ButtonTouched;
            }

            void IterateChildren(View view)
            {
                if (view is RelativeLayout relativeLayout)
                {
                    for (int i = 0; i < relativeLayout.ChildCount; i++)
                    {
                        if (relativeLayout.GetChildAt(i) is Button button)
                            SetButtonEvents(button);
                        else
                            IterateChildren(relativeLayout.GetChildAt(i));
                    }
                }
                else if (view is LinearLayout linearLayout)
                {
                    for (int i = 0; i < linearLayout.ChildCount; i++)
                    {
                        if (linearLayout.GetChildAt(i) is Button button)
                            SetButtonEvents(button);
                        else
                            IterateChildren(linearLayout.GetChildAt(i));
                    }
                }

            }

            IterateChildren(mainContainer);

            void RemoveClickedHandler(Button volUpButton, Button volDownButton)
            {
                if (volUpButton != null && volDownButton != null)
                {
                    volUpButton.Click -= ButtonClicked;
                    volDownButton.Click -= ButtonClicked;
                }
            }

            RemoveClickedHandler(FindViewById<Button>(Resource.Id.VolumeUpButton), FindViewById<Button>(Resource.Id.VolumeDownButton));
            RemoveClickedHandler(FindViewById<Button>(Resource.Id.ControllerVolumeUpButton), FindViewById<Button>(Resource.Id.ControllerVolumeDownButton));
        }

        private void SetBackgroundColor()
        {
            View mainContainer = FindViewById(Resource.Id.MainContainer);
            _rootView = mainContainer.RootView;
            _rootView.SetBackgroundColor(Color.White);

            Window.SetStatusBarColor(Color.Argb(255, 0, 0, 0));
        }

        private void ButtonTouched(object sender, View.TouchEventArgs e)
        {
            Button button = sender as Button;

            if (e.Event.Action == MotionEventActions.Down)
                button.Background.Alpha = 128;
            else if (e.Event.Action == MotionEventActions.Up)
                button.Background.Alpha = 255;

            if (button.Tag.ToString().Contains("Volume"))
            {
                if (e.Event.Action == MotionEventActions.Down)
                    ButtonPressed(button);
                else if (e.Event.Action == MotionEventActions.Up)
                    ButtonReleased(button);

                e.Handled = true;
            }

            e.Handled = false;
        }

        private void ButtonPressed(Button button)
        {
            if (!_volumeChanging)
            {
                _volumeChanging = true;

                if (button.Tag.ToString().Contains("VolumeUp"))
                    _volumeUpTimer.Start();
                else if (button.Tag.ToString().Contains("VolumeDown"))
                    _volumeDownTimer.Start();
            }
        }

        private void ButtonReleased(Button button)
        {
            _volumeChanging = false;
            _volumeUpTimer.Stop();
            _volumeDownTimer.Stop();
        }
        
        private void ButtonClicked(object sender, EventArgs e)
        {
            Button button = sender as Button; 
            string[] cmdArray = button.Tag.ToString().Split(':');
            string cmdType = cmdArray[0];
            string cmdName = cmdArray[1];

            switch (cmdType)
            {
                case "MENU":
                    ProcessMenuCommand(cmdName);
                    break;
                case "REC":
                    ProcessReceiverCommand(cmdName);
                    break;
                case "PRO":
                    ProcessProjectorCommand(cmdName);
                    break;
                case "CTRL":
                    ProcessControllerCommand(cmdName);
                    break;
            }
        }

        private void ProcessMenuCommand(string cmdName)
        {
            switch (cmdName)
            {
                case "Power":
                    _mainMenu.Visibility = _inputSourceMenu.Visibility = _audioVideoOptionsMenu.Visibility = ViewStates.Invisible;
                    _powerMenu.Visibility = ViewStates.Visible;
                    _rootView.SetBackgroundColor(_redColor);
                    break;
                case "InputSource":
                    _mainMenu.Visibility = _powerMenu.Visibility = _audioVideoOptionsMenu.Visibility = ViewStates.Invisible;
                    _inputSourceMenu.Visibility = ViewStates.Visible;
                    _rootView.SetBackgroundColor(_greenColor);
                    break;
                case "AudioVideoOptions":
                    _mainMenu.Visibility = _powerMenu.Visibility = _inputSourceMenu.Visibility = ViewStates.Invisible;
                    _audioVideoOptionsMenu.Visibility = ViewStates.Visible;
                    _rootView.SetBackgroundColor(_blueColor);
                    break;
                case "Main":
                    _powerMenu.Visibility = _inputSourceMenu.Visibility = _audioVideoOptionsMenu.Visibility = ViewStates.Invisible;
                    _mainMenu.Visibility = ViewStates.Visible;
                    _rootView.SetBackgroundColor(Color.White);
                    break;
            }
        }

        private void ProcessRokuCommand(string cmdName)
        {
            Enum.TryParse($"Roku{cmdName}", out CommandCode command);
            RemoteController.SendCommand(command);
        }

        private void ProcessProjectorCommand(string cmdName)
        {
            if (cmdName == "LowLatencyToggle")
            {
                RemoteController.SendCommand(CommandCode.ProLeft);
            }
            else
            {
                Enum.TryParse($"Pro{cmdName}", out CommandCode command);
                RemoteController.SendCommand(command);
            }
        }

        private void ProcessReceiverCommand(string cmdName)
        {
            Enum.TryParse($"Rec{cmdName}", out CommandCode command);
            RemoteController.SendCommand(command);
        }

        private void ProcessControllerCommand(string cmdName)
        {
            switch (cmdName)
            {
                case "Device":
                    _controllerMode = (ControllerMode)(((int)_controllerMode + 1) % 3);
                    TextView selectedDeviceText = FindViewById<TextView>(Resource.Id.ControllerSelectedDeviceText);
                    selectedDeviceText.Text = $"Device: {_controllerMode}";

                    if (_controllerMode == ControllerMode.Roku)
                    {
                        _starButton.SetTextColor(_orangeColor);
                        _rewindButton.SetTextColor(_orangeColor);
                        _playPauseButton.SetTextColor(_orangeColor);
                        _forwardButton.SetTextColor(_orangeColor);
                        _starButton.Enabled = true;
                        _rewindButton.Enabled = true;
                        _playPauseButton.Enabled = true;
                        _forwardButton.Enabled = true;
                        _menuButton.Text = "Home";
                    }
                    else
                    {
                        _starButton.SetTextColor(Color.Black);
                        _rewindButton.SetTextColor(Color.Black);
                        _playPauseButton.SetTextColor(Color.Black);
                        _forwardButton.SetTextColor(Color.Black);
                        _starButton.Enabled = false;
                        _rewindButton.Enabled = false;
                        _playPauseButton.Enabled = false;
                        _forwardButton.Enabled = false;
                        _menuButton.Text = "Menu";
                    }
                    break;
                default:
                    switch (_controllerMode)
                    {
                        case ControllerMode.Roku:
                            ProcessRokuCommand(cmdName);
                            break;
                        case ControllerMode.Projector:
                            ProcessProjectorCommand(cmdName);
                            break;
                        case ControllerMode.Receiver:
                            ProcessReceiverCommand(cmdName);
                            break;
                    }
                    break;
            }
        }
        #endregion
    }
}