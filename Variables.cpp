

//ImGui
static int width = 550;
static int height = 400;
static int step = 0;
static int tab = 1;
static int GameType;
static int Game;
static int EmulatorType;
static int Emulator;
inline bool InfWindow = false;
static float color_edit4[4] = { 1.00f, 1.00f, 1.00f, 1.000f };
static int size_pixels = 10;

//Auth
static float progress = -0.5f, progress_dir = -1.0f;
inline bool LoginCheck = false;
inline bool animate = false;

//ImGui Animated Text
static float Alpha = 255;
static bool Tick = false;
static float Speed = 1.5f;

//Auto-Update
static int LoaderVersion = 1;
static int CheckedVersion;
static int IsChecked = 0;


//Miscs
static int selected_emulator;
static int selected_game;

static int choices;
static int choice;
static int selectedEmu;

inline bool lessRecoil = false;
inline bool noHead = false;
inline bool ultraHD = false;
inline bool fps = false;
inline bool ipadView = true;
static float iPad = 100.0f;
static int status = 0;
