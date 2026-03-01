#define _CRT_SECURE_NO_WARNINGS
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <time.h>
#include <glut.h>
#include <cJSON.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define PI 3.14159265358979323846 // pi
#define PI_2 1.57079632679489661923   // pi/2

#define WIDTH 1600
#define INFO_WIDTH 400
#define GAME_WIDTH 1200
#define HEIGHT 900
#define CELLS_Y 15
#define CELLS_X 20

#define CELL_SIZE 60 // GAME_WIDTH / CELLS_X
#define HALF_CELL_SIZE 30
#define TWO_CELLS_SIZE 120
#define RIGHT_WALL 1140 // GAME_WIDTH - CELL_SIZE
#define BOTTOM_WALL 840 // HEIGHT - CELL_SIZE

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
#define MOVE_SPEED 600.0f

int currentLevel = 1; // Текущий уровень
int completedHomes = 0; // Количество заполненных домиков

// максимальное количество объектов - для пользовательских уровней
#define MAX_CUSTOM_OBJS 195
#define MAX_HOMES_COUNT 5  // Количество домиков

#define SCORE_HOME 100
#define SCORE_FORWARD 10

#define HITBOX_LEFT   0
#define HITBOX_RIGHT  1
#define HITBOX_TOP    2
#define HITBOX_BOTTOM 3

#define ALLIGATOR_ANIM_SPEED 2.0f  // Скорость анимации пасти

#define DEBUG_HITBOX_KEY GLFW_KEY_H  // Клавиша для включения/выключения хитбоксов
int showHitboxes = 0;                // Флаг отображения хитбоксов
int hitboxKeyPressed = 0;            // Для отслеживания состояния клавиши H

#define DEBUG_NOCLIP_KEY GLFW_KEY_N  // Клавиша для включения/выключения noclip
int noclipEnabled = 0;         // Флаг режима noclip
int noclipKeyPressed = 0;      // Для отслеживания состояния клавиши

#define DEBUG_LEVELUP_KEY GLFW_KEY_L
int levelupKeyPressed = 0;

int escKeyPressed = 0;

textInputActive = 0;

char levelName[22] = { 0 }; // Буфер для названия уровня

int visitedRows[15] = { 0 }; // Массив посещенных рядов
int lastVisitedRow = -1;        // Последний посещенный ряд

#define TOTAL_TIME 60.0f // Общее время на уровень
float remainingTime = TOTAL_TIME;
float timerWidth = 200.0f; // Ширина полосы таймера

double cursorX, cursorY;
int isCursorInEditorField = 0;

GLuint backgroundTex;
GLuint titleTex;
GLuint playTex;
GLuint exitTex;
GLuint gameOverTex;
GLuint restartTex;
GLuint menuTex;
GLuint waterTex;
GLuint car1Tex;
GLuint car2Tex;
GLuint car3Tex;
GLuint car4Tex;
GLuint truckTex;
GLuint logLTex;
GLuint logMTex;
GLuint logRTex;
GLuint heartTex;
GLuint turtle1Tex;  // Черепаха на поверхности
GLuint turtle2Tex;  // Черепаха наполовину скрыта
GLuint turtle3Tex;  // Черепаха почти под водой
GLuint turtle4Tex;  // Черепаха полностью под водой
GLuint roadTex;      // Текстура дороги
GLuint groundTex;    // Текстура земли
GLuint homeEmptyTex;    // Пустой домик
GLuint homeFill1Tex;    // Анимация заполнения 1
GLuint homeFill2Tex;    // Анимация заполнения 2
GLuint homeFill3Tex;    // Анимация заполнения 3
GLuint homeFullTex;     // Полностью заполненный домик
GLuint infoPanelTex;
GLuint gameOverBackgroundTex;
GLuint menuBackgroundTex;
GLuint lawnmowerTex;
GLuint lawnTex;
GLuint snake1Tex;
GLuint snake2Tex;
GLuint dialogWindowTex;
GLuint yesButtonTex;
GLuint noButtonTex;
GLuint menuPromptTex;
GLuint menuButtonTex;
GLuint editorTex;
GLuint createButtonTex;
GLuint cancelButtonTex;
GLuint alligatorHeadClosedTex;
GLuint alligatorHeadOpenTex;
GLuint alligatorBodyTex;
GLuint alligatorTailTex;
GLuint settingsButtonTex;
GLuint optionButtonTex;
GLuint deleteButtonTex;
GLuint grassButtonTex;
GLuint roadButtonTex;
GLuint lawnButtonTex;
GLuint waterButtonTex;
GLuint homeButtonTex;
GLuint spawnButtonTex;
GLuint invalidPatternTex;
GLuint buttonDirectionLeftTex;
GLuint buttonDirectionRightTex;
GLuint editorOptionsCar1Tex;
GLuint editorOptionsCar2Tex;
GLuint editorOptionsCar3Tex;
GLuint editorOptionsCar4Tex;
GLuint editorOptionsLawnmowerTex;
GLuint editorOptionsLogTex;
GLuint editorOptionsSnakeTex;
GLuint editorOptionsMenuTex;
GLuint editorOptionsTruckTex;
GLuint editorOptionsTurtleTex;
GLuint emptyTex;
GLuint editorOptionsNotAllowedEmpty;
GLuint editorOptionsNotAllowedOneLawnmower;
GLuint editorOptionsNotAllowedTwoLawnmowers;
GLuint editorOptionsNotAllowedRoad;
GLuint editorOptionsNotAllowedWater;
GLuint editorOptionsNotAllowedSnake;
GLuint editorOptionsNotAllowedZeroQuantity;
GLuint exitButtonTex;
GLuint resumeButtonTex;
GLuint saveAndExitButtonTex;
GLuint saveAndPlayButtonTex;
GLuint saveButtonTex;
GLuint levelsTex;
GLuint inputFieldTex;
GLuint loadTex;
GLuint deleteTex;
GLuint frogIdleTex;
GLuint frogJumpTex;
GLuint frogDrownTex[4];
GLuint frogCrashTex[4];
GLuint editorSettings;
GLuint notValidEditorTex;

typedef enum {
    MAIN_MENU,
    GAME_OVER,
    CUSTOM_PLAYING,
    CUSTOM_GAME_OVER,
    CUSTOM_PLAYING_TO_MENU_CONFIRM,
    LEVEL_MENU,
    LEVEL_CREATE_DIALOG,
    LEVEL_CREATE_ERROR,
    LEVEL_LOAD_DIALOG,
    LEVEL_DELETE_CONFIRM,
    LEVEL_LOAD_ERROR,
    EDITOR,
    EDITOR_EXIT_DIALOG,
    EDITOR_SETTINGS_DIALOG,
    EDITOR_NOT_VALID
} GameState;

GameState gameState = MAIN_MENU;

typedef enum {
    OBJ_EMPTY,
    OBJ_CAR1,
    OBJ_CAR2,
    OBJ_CAR3,
    OBJ_CAR4,
    OBJ_TRUCK,
    OBJ_LOG,
    OBJ_TURTLE,
    OBJ_SNAKE,
    OBJ_LAWNMOWER,
    OBJ_COUNT, // Для подсчёта количества типов
    OBJ_ALLIGATOR
} ObjectType;

ObjectType selectedObjectType = OBJ_EMPTY;
GLuint objectPreviewTextures[OBJ_COUNT];
GLuint objectAllowedOptions[OBJ_COUNT];

typedef enum {
    ZONE_NONE,
    ZONE_GRASS,
    ZONE_ROAD,
    ZONE_LAWN,
    ZONE_WATER
} ZoneType;

ZoneType selectedZone = ZONE_NONE;

typedef enum {
    EDITOR_MODE_NONE,
    EDITOR_MODE_ZONE,
    EDITOR_MODE_HOME,
    EDITOR_MODE_SPAWN,
    EDITOR_MODE_DELETE,
    EDITOR_MODE_OPTIONS
} EditorMode;

EditorMode editorMode = EDITOR_MODE_NONE;

typedef enum {
    FROG_IDLE,
    FROG_JUMPING,
    FROG_DROWNING,
    FROG_CRASHING
} FrogState;

float drownProgress = 0.0f;
const float DROWN_DURATION = 0.5f; // Общая длительность анимации
int drownFrame = 0; // Текущий кадр анимации

float crashProgress = 0.0f;
const float CRASH_DURATION = 0.5f;
int crashFrame = 0;

int isCrashing = 0;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} FrogDirection;

FrogState frogState = FROG_IDLE;
FrogDirection frogDirection = DIR_UP;

typedef struct {
    float left;    // % от левого края
    float right;   // % от правого края 
    float top;     // % от верхнего края
    float bottom;  // % от нижнего края
} Hitbox;

typedef struct {
    float x, y;           // Текущие координаты
    float startX, startY; // Начальные позиции (для reset)
    float speed;          // Скорость движения
    int direction;        // Направление (1 = вправо, -1 = влево)
    int row;              // Номер ряда
    GLuint texture;       // Текстура объекта
    float width;          // Ширина объекта
    float rightBoundary;  // Граница для "зацикливания"
    Hitbox hitbox;        // Хитбокс объекта
    int isActive;         // Для змеи/газонокосилки
} CustomObject;

typedef struct {
    CustomObject base;
    int state;          // 0-3: состояния анимации
    float timer;        // Таймер для смены состояний
    int phase;          // Фаза анимации (0-3)
    float phaseTimer;   // Таймер текущей фазы
    int isDiving;       // 1 если черепаха ныряет
} CustomTurtle;

typedef struct {
    CustomObject base;
    float changeRowTimer;
    int isChangingRow;
    float changeRowProgress;
    float targetY;
    float rotationAngle;
    int rotationDirection;
    float rowChangeSpeedFactor;
    int currentRow;
    int isExist;
    int teleport;
} CustomLawnmower;

typedef struct {
    CustomObject base;
    Hitbox headHitbox;      // Хитбокс головы (смертельная зона)
    int mouthState;         // 0 - закрыта, 1 - открыта
    float mouthTimer;       // Таймер анимации
    float mouthProgress;    // Прогресс анимации (0-1)
} CustomAlligator;

typedef struct {
    Hitbox hitbox;
    float x, y;
    int isExist;
    int state;          // 0 - пустой, 1-3 - анимация заполнения, 4 - заполнен
    float animTimer;    // Таймер анимации
    int isFill;
} CustomHome;

typedef struct {
    CustomObject base;
    float animTimer;      // Таймер анимации
    int currentFrame;     // Текущий кадр (0 или 1)
    int teleport;
    GLuint textures[2];   // Массив текстур для анимации
} CustomSnake;

CustomHome customHomes[MAX_HOMES_COUNT]; // Для хранения позиций домиков
int customHomeCount = 0;
float spawnX = -999, spawnY = -999; // Позиция точки спавна

CustomObject customCars[MAX_CUSTOM_OBJS];
CustomObject customLogs[MAX_CUSTOM_OBJS];
CustomTurtle customTurtles[MAX_CUSTOM_OBJS];
CustomSnake customSnakes[MAX_CUSTOM_OBJS];
CustomLawnmower customLawnmowers[MAX_CUSTOM_OBJS];
CustomAlligator customAlligators[MAX_CUSTOM_OBJS];
int customCarCount = 0;
int customLogCount = 0;
int customTurtleCount = 0;
int customSnakeCount = 0;
int customLawnmowerCount = 0;
int customAlligatorCount = 0;

// Структура для хранения звуков
typedef struct {
    Mix_Chunk* click;
    Mix_Chunk* explode;
    Mix_Chunk* jump;
    Mix_Chunk* levelup;
    Mix_Chunk* play;
    Mix_Chunk* quit;
    Mix_Music* gameMusic;
    Mix_Music* menuMusic;
    Mix_Music* gameOver;
    Mix_Chunk* error;
} SoundSystem;

SoundSystem sounds;

typedef struct {
    ObjectType objectType;
    int quantity;
    int length;
    float initialSpeed1;
    float initialSpeed2;
    int speedLevels1;
    int speedLevels2;
    float speedIncrease1;
    float speedIncrease2;
    int countLevels;
    int countIncrease;
    int direction; // 1 - вправо, -1 - влево
} RowSettings;

typedef struct {
    float cellSize;     // Размер ячейки (квадратные ячейки)
    float fieldWidth;   // Ширина всего поля
    float fieldHeight;  // Высота всего поля
    float viewOffsetX;  // Смещение для центрирования
    float viewOffsetY;  // Смещение для центрирования
    float zoomFactor;
    float panX, panY;    // Смещение камеры
    float baseCellSize;  // Базовый размер ячейки без зума
    ZoneType* zoneTypes;
    int isOptionsMenuOpen;      // 1 если окно открыто
    int selectedZoneRow;        // Выбранный ряд для настроек
    float optionsMenuPosX;      // Позиция окна по X
    float optionsMenuPosY;      // Позиция окна по Y
    RowSettings* rowSettings;
    int initialLives;            // Количество жизней (1-10)
    float gameTime;              // Время игры (1-120)
    int alligatorChance;         // Шанс аллигатора (0-100)
} LevelEditor;

LevelEditor editor;
LevelEditor editorBase;

int isDirLeftPressed = 0;
int isDirRightPressed = 0;

typedef struct {
    int focusedField;
    char livesBuffer[3];
    char alligatorBuffer[4];
    char timeBuffer[4];
    int tempLives;          // Временные значения
    int tempAlligatorChance;
    float tempGameTime;
    int originalLives;
    int originalAlligatorChance;
    float originalGameTime;
} EditorSettingsDialog;

EditorSettingsDialog settingsDialog = { 0 };

typedef struct {
    int quantity;      // Значение количества
    int length;        // Значение длины

    // Начальная скорость
    float initialSpeed1;
    float initialSpeed2;

    // Уровней для повышения скорости
    int speedLevels1;
    int speedLevels2;

    // Повышение скорости за n уровней
    float speedIncrease1;
    float speedIncrease2;

    // Уровней для повышения количества объектов
    int countLevels;

    // Повышение количества объектов за n уровней
    int countIncrease;

    // Текущее активное поле ввода
    int focusedInputField; // 0-нет, 1-10 - соответствуют полям
    char inputBuffer[32];
    char prevValue[32];
    int isValueHidden;
} EditorOptions;

EditorOptions editorOptions = {
    .quantity = 0,
    .length = 2,
    .initialSpeed1 = 0,
    .initialSpeed2 = 0,
    .speedLevels1 = 0,
    .speedLevels2 = 0,
    .speedIncrease1 = 0,
    .speedIncrease2 = 0,
    .countLevels = 0,
    .countIncrease = 0,
    .focusedInputField = 0,
    .inputBuffer = {0}
};

typedef struct {
    int keyPressed[GLFW_KEY_LAST + 1]; // Флаги нажатия клавиш
    double lastKeyPressTime[GLFW_KEY_LAST + 1]; // Время последнего нажатия
} KeyState;

KeyState keyStateInput = { 0 };


static double lastMouseX, lastMouseY;
static int isPanning = 0;

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int levelFileExists(const char* filename) {
    char path[256];
    snprintf(path, sizeof(path), "levels/%s.json", filename);
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int getMaxQuantityForObject(ObjectType objType) {
    switch (objType) {
    case OBJ_SNAKE:       return 1;
    case OBJ_LAWNMOWER:   return 2;
    case OBJ_CAR1:
    case OBJ_CAR2:
    case OBJ_CAR3:
    case OBJ_CAR4:
    case OBJ_TRUCK:
    case OBJ_LOG:
    case OBJ_TURTLE:      return 15;
    default:              return 0;
    }
}

void closeOptionsMenu() {
    editor.isOptionsMenuOpen = 0;
    editorOptions.focusedInputField = 0;
    memset(editorOptions.inputBuffer, 0, sizeof(editorOptions.inputBuffer));
    isDirLeftPressed = 0;
    isDirRightPressed = 0;
}

void saveEditorInputValue() {
    if (strlen(editorOptions.inputBuffer) == 0) return;

    if (strlen(editorOptions.inputBuffer) > 0) {
        switch (editorOptions.focusedInputField) {
        case 1: {
            editorOptions.quantity = atoi(editorOptions.inputBuffer);
            int max = getMaxQuantityForObject(selectedObjectType);
            int min = 0;
            if (editorOptions.quantity > max) {
                editorOptions.quantity = max;
            }
            else if (editorOptions.quantity < 0) {
                editorOptions.quantity = 0;
            }
            break;
        }
        case 2: {
            editorOptions.length = atoi(editorOptions.inputBuffer);
            if (editorOptions.length > 5) {
                editorOptions.length = 5;
            }
            else if (selectedObjectType == OBJ_LOG && editorOptions.length < 2) {
                editorOptions.length = 2;
            }
            else if (editorOptions.length < 1) {
                editorOptions.length = 1;
            }
            break;
        }
        case 3: {
            editorOptions.initialSpeed1 = atof(editorOptions.inputBuffer);
            if (editorOptions.initialSpeed1 > 500.0f) {
                editorOptions.initialSpeed1 = 500.0f;
            }
            break;
        }
        case 4: {
            editorOptions.initialSpeed2 = atof(editorOptions.inputBuffer);
            if (editorOptions.initialSpeed2 > 500.0f) {
                editorOptions.initialSpeed2 = 500.0f;
            }
            break;
        }
        case 5: {
            editorOptions.speedLevels1 = atoi(editorOptions.inputBuffer);
            if (editorOptions.speedLevels1 > 20) {
                editorOptions.speedLevels1 = 20;
            }
            break;
        }
        case 6: {
            editorOptions.speedLevels2 = atoi(editorOptions.inputBuffer);
            if (editorOptions.speedLevels2 > 10) {
                editorOptions.speedLevels2 = 10;
            }
            break;
        }
        case 7: {
            editorOptions.speedIncrease1 = atof(editorOptions.inputBuffer);
            if (editorOptions.speedIncrease1 > 250.0f) {
                editorOptions.speedIncrease1 = 250.0f;
            }
            break;
        }
        case 8: {
            editorOptions.speedIncrease2 = atof(editorOptions.inputBuffer);
            if (editorOptions.speedIncrease2 > 250.0f) {
                editorOptions.speedIncrease2 = 250.0f;
            }
            break;
        }
        case 9: {
            editorOptions.countLevels = atoi(editorOptions.inputBuffer);
            if (editorOptions.countLevels > 10) {
                editorOptions.countLevels = 10;
            }
            break;
        }
        case 10: {
            editorOptions.countIncrease = atoi(editorOptions.inputBuffer);
            if (editorOptions.countIncrease > 5) {
                editorOptions.countIncrease = 5;
            }
            break;
        }
        }
    }
    editorOptions.isValueHidden = 0;  // Показываем значение после сохранения
}

void saveRowSettings(int row) {
    if (row < 0 || row >= CELLS_Y) return;

    editor.rowSettings[row].objectType = selectedObjectType;
    if (selectedObjectType == OBJ_LAWNMOWER &&
        editor.rowSettings[row].quantity == 1 && editorOptions.quantity == 2) {
        isDirLeftPressed = isDirRightPressed = 0;
    }
    editor.rowSettings[row].quantity = editorOptions.quantity;
    editor.rowSettings[row].length = editorOptions.length;
    editor.rowSettings[row].initialSpeed1 = editorOptions.initialSpeed1;
    editor.rowSettings[row].initialSpeed2 = editorOptions.initialSpeed2;
    editor.rowSettings[row].speedLevels1 = editorOptions.speedLevels1;
    editor.rowSettings[row].speedLevels2 = editorOptions.speedLevels2;
    editor.rowSettings[row].speedIncrease1 = editorOptions.speedIncrease1;
    editor.rowSettings[row].speedIncrease2 = editorOptions.speedIncrease2;
    editor.rowSettings[row].countLevels = editorOptions.countLevels;
    editor.rowSettings[row].countIncrease = editorOptions.countIncrease;
    editor.rowSettings[row].direction = isDirLeftPressed ? -1 : (isDirRightPressed ? 1 : 0);

    int i = 0;
    if (editor.zoneTypes[row] == ZONE_LAWN) {
        while (editor.zoneTypes[row - i] == ZONE_LAWN) i++;
        if (i % 2 == 0) i = -1;
        else i = 1;

        editor.rowSettings[row + i].objectType = selectedObjectType;
        if (selectedObjectType == OBJ_LAWNMOWER &&
            editor.rowSettings[row + i].quantity == 1 && editorOptions.quantity == 2) {
            isDirLeftPressed = isDirRightPressed = 0;
        }
        editor.rowSettings[row + i].quantity = editorOptions.quantity;
        editor.rowSettings[row + i].length = editorOptions.length;
        editor.rowSettings[row + i].initialSpeed1 = editorOptions.initialSpeed1;
        editor.rowSettings[row + i].initialSpeed2 = editorOptions.initialSpeed2;
        editor.rowSettings[row + i].speedLevels1 = editorOptions.speedLevels1;
        editor.rowSettings[row + i].speedLevels2 = editorOptions.speedLevels2;
        editor.rowSettings[row + i].speedIncrease1 = editorOptions.speedIncrease1;
        editor.rowSettings[row + i].speedIncrease2 = editorOptions.speedIncrease2;
        editor.rowSettings[row + i].countLevels = editorOptions.countLevels;
        editor.rowSettings[row + i].countIncrease = editorOptions.countIncrease;
        editor.rowSettings[row + i].direction = isDirLeftPressed ? -1 : (isDirRightPressed ? 1 : 0);
    }
}

void deleteRowSettings(int row) {
    editor.rowSettings[row].objectType = 0;
    editor.rowSettings[row].quantity = 0;
    editor.rowSettings[row].length = 2;
    editor.rowSettings[row].initialSpeed1 = 0;
    editor.rowSettings[row].initialSpeed2 = 0;
    editor.rowSettings[row].speedLevels1 = 0;
    editor.rowSettings[row].speedLevels2 = 0;
    editor.rowSettings[row].speedIncrease1 = 0;
    editor.rowSettings[row].speedIncrease2 = 0;
    editor.rowSettings[row].countLevels = 0;
    editor.rowSettings[row].countIncrease = 0;
    editor.rowSettings[row].direction = 0;

    int i = 0;
    if (editor.zoneTypes[row] == ZONE_LAWN) {
        while (editor.zoneTypes[row - i] == ZONE_LAWN) i++;
        if (i % 2 == 0) i = -1;
        else i = 1;

        editor.rowSettings[row + i].objectType = 0;
        editor.rowSettings[row + i].quantity = 0;
        editor.rowSettings[row + i].length = 2;
        editor.rowSettings[row + i].initialSpeed1 = 0;
        editor.rowSettings[row + i].initialSpeed2 = 0;
        editor.rowSettings[row + i].speedLevels1 = 0;
        editor.rowSettings[row + i].speedLevels2 = 0;
        editor.rowSettings[row + i].speedIncrease1 = 0;
        editor.rowSettings[row + i].speedIncrease2 = 0;
        editor.rowSettings[row + i].countLevels = 0;
        editor.rowSettings[row + i].countIncrease = 0;
        editor.rowSettings[row + i].direction = 0;
    }

}

void clearEditorOptions() {
    editorOptions.quantity = 0;
    editorOptions.length = 2;
    editorOptions.initialSpeed1 = 0;
    editorOptions.initialSpeed2 = 0;
    editorOptions.speedLevels1 = 0;
    editorOptions.speedLevels2 = 0;
    editorOptions.speedIncrease1 = 0;
    editorOptions.speedIncrease2 = 0;
    editorOptions.countLevels = 0;
    editorOptions.countIncrease = 0;
    editorOptions.focusedInputField = 0;
    isDirLeftPressed = 0;
    isDirRightPressed = 0;
    memset(editorOptions.inputBuffer, 0, sizeof(editorOptions.inputBuffer));
}

void handleZoneSelection(int row) {
    editor.selectedZoneRow = row;

    // Загружаем настройки для этого ряда, если они есть
    if (editor.rowSettings[row].objectType != OBJ_EMPTY) {
        selectedObjectType = editor.rowSettings[row].objectType;
        editorOptions.quantity = editor.rowSettings[row].quantity;
        editorOptions.length = editor.rowSettings[row].length;
        editorOptions.initialSpeed1 = editor.rowSettings[row].initialSpeed1;
        editorOptions.initialSpeed2 = editor.rowSettings[row].initialSpeed2;
        editorOptions.speedLevels1 = editor.rowSettings[row].speedLevels1;
        editorOptions.speedLevels2 = editor.rowSettings[row].speedLevels2;
        editorOptions.speedIncrease1 = editor.rowSettings[row].speedIncrease1;
        editorOptions.speedIncrease2 = editor.rowSettings[row].speedIncrease2;
        editorOptions.countLevels = editor.rowSettings[row].countLevels;
        editorOptions.countIncrease = editor.rowSettings[row].countIncrease;

        isDirLeftPressed = (editor.rowSettings[row].direction == -1);
        isDirRightPressed = (editor.rowSettings[row].direction == 1);
    }
    else {
        selectedObjectType = OBJ_EMPTY;
        clearEditorOptions();
    }

    // Очищаем буфер ввода
    memset(editorOptions.inputBuffer, 0, sizeof(editorOptions.inputBuffer));
    editorOptions.focusedInputField = 0;
    editorOptions.isValueHidden = 0;

    editor.isOptionsMenuOpen = 1;
}

int isObjectAllowedForZone(ObjectType objType, ZoneType zone) {
    if (objType == OBJ_EMPTY) {
        return 1;
    }

    switch (zone) {
    case ZONE_ROAD:
        return (objType >= OBJ_CAR1 && objType <= OBJ_TRUCK);
    case ZONE_WATER:
        return (objType == OBJ_LOG || objType == OBJ_TURTLE);
    case ZONE_LAWN:
        return (objType == OBJ_LAWNMOWER);
    case ZONE_GRASS:
        return (objType == OBJ_SNAKE);
    default:
        return 0;
    }
}

int isFieldEnabledForObject(int fieldId, ObjectType objType) {

    // Проверяем, можно ли разместить объект на текущей зоне
    int isAllowed = isObjectAllowedForZone(selectedObjectType, editor.zoneTypes[editor.selectedZoneRow]);


    if (editorOptions.quantity == 0 &&
        fieldId >= 1 && fieldId <= 10) {
        return 0;
    }

    if (!isAllowed && fieldId >= 0 && fieldId <= 10) {
        return 0;
    }
    switch (objType) {
    case OBJ_SNAKE:
        if (fieldId == 1 || fieldId == 3 ||
            fieldId == 5 || fieldId == 7 || fieldId == 8 || fieldId == 9) {
            return 0;
        }
        break;

    case OBJ_LAWNMOWER:
        if (editorOptions.quantity == 2) {
            if (fieldId == 1 || fieldId == 10 || fieldId == 8 ||
                fieldId == 9) {
                return 0;
            }
        }
        else if (editorOptions.quantity == 1) {
            if (fieldId == 1 || fieldId == 3 || fieldId == 5 ||
                fieldId == 7 || fieldId == 8 || fieldId == 9) {
                return 0;
            }
        }
        break;
    case OBJ_CAR1:
    case OBJ_CAR2:
    case OBJ_CAR3:
    case OBJ_CAR4:
    case OBJ_TRUCK:
        if (fieldId == 1 || fieldId == 3 || fieldId == 5 ||
            fieldId == 7) {
            return 0;
        }
        break;
    case OBJ_LOG:
    case OBJ_TURTLE:
        if (fieldId == 3 || fieldId == 5 || fieldId == 7 ||
            fieldId == 8 || fieldId == 9) {
            return 0;
        }
        break;
    case OBJ_EMPTY:
        if (fieldId >= 0 && fieldId <= 10) {
            return 0;
        }
        break;
    default:
        return 1;
    }
    return 1;
}

void processSettingsDialogInput(GLFWwindow* window) {
    if (settingsDialog.focusedField == 0) return;

    // Обработка Backspace с учетом состояния клавиши
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        if (!keyStateInput.keyPressed[GLFW_KEY_BACKSPACE]) {
            char* buffer = NULL;
            switch (settingsDialog.focusedField) {
            case 1: buffer = settingsDialog.livesBuffer; break;
            case 2: buffer = settingsDialog.alligatorBuffer; break;
            case 3: buffer = settingsDialog.timeBuffer; break;
            }
            if (buffer && strlen(buffer) > 0) {
                buffer[strlen(buffer) - 1] = '\0';
            }
            keyStateInput.keyPressed[GLFW_KEY_BACKSPACE] = 1;
        }
    }
    else {
        keyStateInput.keyPressed[GLFW_KEY_BACKSPACE] = 0;
    }

    // Обработка цифр с учетом состояния
    for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; key++) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            if (!keyStateInput.keyPressed[key]) {
                char* buffer = NULL;
                int maxLen = 0;
                switch (settingsDialog.focusedField) {
                case 1:
                    buffer = settingsDialog.livesBuffer;
                    maxLen = 2;
                    break;
                case 2:
                    buffer = settingsDialog.alligatorBuffer;
                    maxLen = 3;
                    break;
                case 3:
                    buffer = settingsDialog.timeBuffer;
                    maxLen = 3;
                    break;
                }
                if (buffer && strlen(buffer) < maxLen) {
                    char digit = '0' + (key - GLFW_KEY_0);
                    strncat(buffer, &digit, 1);
                }
                keyStateInput.keyPressed[key] = 1;
            }
        }
        else {
            keyStateInput.keyPressed[key] = 0;
        }
    }

    // Обработка Enter с учетом состояния
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (!keyStateInput.keyPressed[GLFW_KEY_ENTER]) {
            switch (settingsDialog.focusedField) {
            case 1: {
                int val = clamp(atoi(settingsDialog.livesBuffer), 1, 10);
                settingsDialog.tempLives = val;
                break;
            }
            case 2: {
                int val = clamp(atoi(settingsDialog.alligatorBuffer), 0, 100);
                settingsDialog.tempAlligatorChance = val;
                break;
            }
            case 3: {
                int val = clamp(atoi(settingsDialog.timeBuffer), 1, 120);
                settingsDialog.tempGameTime = (float)val;
                break;
            }
            }
            settingsDialog.focusedField = 0;
            keyStateInput.keyPressed[GLFW_KEY_ENTER] = 1;
        }
    }
    else {
        keyStateInput.keyPressed[GLFW_KEY_ENTER] = 0;
    }
}

void processEditorTextInput(GLFWwindow* window) {
    if (editorOptions.focusedInputField == 0) return;

    // Как только начинаем ввод - показываем вводимые символы
    editorOptions.isValueHidden = 0;
    if (strlen(editorOptions.inputBuffer) <= 5) {
        // Обработка цифр
        for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; key++) {
            if (glfwGetKey(window, key) == GLFW_PRESS && !keyStateInput.keyPressed[key]) {
                char digit = '0' + (key - GLFW_KEY_0);
                strncat(editorOptions.inputBuffer, &digit, 1);
                keyStateInput.keyPressed[key] = 1;
            }
            else if (glfwGetKey(window, key) == GLFW_RELEASE) {
                keyStateInput.keyPressed[key] = 0;
            }
        }

        // Обработка точки
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS &&
            !keyStateInput.keyPressed[GLFW_KEY_PERIOD] &&
            strchr(editorOptions.inputBuffer, '.') == NULL) {
            strncat(editorOptions.inputBuffer, ".", 1);
            keyStateInput.keyPressed[GLFW_KEY_PERIOD] = 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_RELEASE) {
            keyStateInput.keyPressed[GLFW_KEY_PERIOD] = 0;
        }

    }

    // Обработка Backspace
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS &&
        !keyStateInput.keyPressed[GLFW_KEY_BACKSPACE] &&
        strlen(editorOptions.inputBuffer) > 0) {
        editorOptions.inputBuffer[strlen(editorOptions.inputBuffer) - 1] = '\0';
        keyStateInput.keyPressed[GLFW_KEY_BACKSPACE] = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE) {
        keyStateInput.keyPressed[GLFW_KEY_BACKSPACE] = 0;
    }

    // Обработка Enter
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS &&
        !keyStateInput.keyPressed[GLFW_KEY_ENTER]) {
        saveEditorInputValue();
        editorOptions.focusedInputField = 0;
        memset(editorOptions.inputBuffer, 0, sizeof(editorOptions.inputBuffer));
        keyStateInput.keyPressed[GLFW_KEY_ENTER] = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
        keyStateInput.keyPressed[GLFW_KEY_ENTER] = 0;
    }
}

void handleSettingsDialogClick(double x, double y) {
    int dx = (WIDTH - 500) / 2;
    int dy = (HEIGHT - 500) / 2;

    int prevFocused = settingsDialog.focusedField;
    settingsDialog.focusedField = 0;

    // Проверяем клик по полям
    int fieldY[3] = { 30, 160, 290 };
    for (int i = 0; i < 3; i++) {
        if (x >= dx + 280 && x <= dx + 280 + 190 &&
            y >= dy + fieldY[i] && y <= dy + fieldY[i] + 70 &&
            i != 2 ||
            x >= dx + 280 && x <= dx + 280 + 100 &&
            y >= dy + fieldY[i] && y <= dy + fieldY[i] + 70 &&
            i == 2)
        {
            settingsDialog.focusedField = i + 1;
            // Сохраняем оригинальные значения при первом фокусе
            if (prevFocused != settingsDialog.focusedField) {
                settingsDialog.originalLives = settingsDialog.tempLives;
                settingsDialog.originalAlligatorChance = settingsDialog.tempAlligatorChance;
                settingsDialog.originalGameTime = settingsDialog.tempGameTime;
            }
            // Очищаем буфер при новом фокусе
            memset(settingsDialog.livesBuffer, 0, sizeof(settingsDialog.livesBuffer));
            memset(settingsDialog.alligatorBuffer, 0, sizeof(settingsDialog.alligatorBuffer));
            memset(settingsDialog.timeBuffer, 0, sizeof(settingsDialog.timeBuffer));
            return;
        }
    }

    // Восстанавливаем значения если фокус ушел без сохранения
    if (prevFocused != 0) {
        settingsDialog.tempLives = settingsDialog.originalLives;
        settingsDialog.tempAlligatorChance = settingsDialog.originalAlligatorChance;
        settingsDialog.tempGameTime = settingsDialog.originalGameTime;
    }

    if (x >= dx + 410 && x <= dx + 410 + 50 &&
        y >= dy + 300 && y <= dy + 350) {
        settingsDialog.tempGameTime = -1;
    }
}

void openEditorSettingsDialog() {
    memset(settingsDialog.livesBuffer, 0, sizeof(settingsDialog.livesBuffer));
    memset(settingsDialog.alligatorBuffer, 0, sizeof(settingsDialog.alligatorBuffer));
    memset(settingsDialog.timeBuffer, 0, sizeof(settingsDialog.timeBuffer));
    settingsDialog.focusedField = 0;

    if (editor.initialLives || editor.gameTime) {
        settingsDialog.tempLives = editor.initialLives;
        settingsDialog.tempAlligatorChance = editor.alligatorChance;
        settingsDialog.tempGameTime = editor.gameTime;
    }
    else {
        settingsDialog.tempLives = 3;
        settingsDialog.tempAlligatorChance = 30;
        settingsDialog.tempGameTime = 60.0f;
    }
    gameState = EDITOR_SETTINGS_DIALOG;

}

void handleEditorOptionsMenuClick(double mouseX, double mouseY) {
    const struct {
        int fieldId;
        float x, y, w, h;
    } inputFields[] = {
        {1, 125, 110, 250, 80},  // Количество
        {2, 125, 210, 250, 80},  // Длина
        {3, 120, 410, 120, 80},  // Нач. скорость 1
        {4, 260, 410, 120, 80},  // Нач. скорость 2
        {5, 120, 510, 120, 80},  // Уровни скорости 1
        {6, 260, 510, 120, 80},  // Уровни скорости 2
        {7, 120, 610, 120, 80},  // Пов. скорости 1
        {8, 260, 610, 120, 80},  // Пов. скорости 2
        {9, 190, 710, 120, 80},  // Уровни количества
        {10, 190, 810, 120, 80}  // Пов. количества
    };

    int isAllowed = isObjectAllowedForZone(selectedObjectType, editor.zoneTypes[editor.selectedZoneRow]);

    // Проверка кнопки "вставить" (справа)
    if (editor.optionsMenuPosX < GAME_WIDTH / 2 &&
        mouseX >= editor.optionsMenuPosX + 415 && mouseX <= editor.optionsMenuPosX + 485 &&
        mouseY >= 815 && mouseY <= 885) {
        if (isAllowed &&
            (selectedObjectType == OBJ_EMPTY || isDirLeftPressed || isDirRightPressed || selectedObjectType == OBJ_LAWNMOWER && editorOptions.quantity == 2)) { // нужно выбрать направление
            saveRowSettings(editor.selectedZoneRow);
            Mix_PlayChannel(-1, sounds.click, 0);
            closeOptionsMenu();
        }
        else {
            Mix_PlayChannel(-1, sounds.error, 0);
        }
        return;
    }

    // Проверка кнопки "вставить" (слева)
    if (editor.optionsMenuPosX > GAME_WIDTH / 2 &&
        mouseX >= editor.optionsMenuPosX - 85 && mouseX <= editor.optionsMenuPosX - 15 &&
        mouseY >= 815 && mouseY <= 885) {
        if (isAllowed &&
            (selectedObjectType == OBJ_EMPTY || isDirLeftPressed || isDirRightPressed || selectedObjectType == OBJ_LAWNMOWER && editorOptions.quantity == 2)) { // нужно выбрать направление
            saveRowSettings(editor.selectedZoneRow);
            Mix_PlayChannel(-1, sounds.click, 0);
            closeOptionsMenu();
        }
        else {
            Mix_PlayChannel(-1, sounds.error, 0);
        }
        return;
    }

    // Проверяем, находится ли курсор в пределах меню Options
    if (mouseX >= editor.optionsMenuPosX &&
        mouseX <= editor.optionsMenuPosX + 400 &&
        mouseY >= 0 &&
        mouseY <= 900) {

        // Обработка кнопок выбора типа объекта
        if (mouseX >= editor.optionsMenuPosX + 125 && mouseX <= editor.optionsMenuPosX + 195 &&
            mouseY >= 15 && mouseY <= 85) {
            // Кнопка "тип->влево" 
            if (selectedObjectType == OBJ_LAWNMOWER || selectedObjectType == OBJ_SNAKE ||
                selectedObjectType == OBJ_LOG || selectedObjectType == OBJ_CAR1) {
                clearEditorOptions();
            }
            selectedObjectType = (selectedObjectType - 1 + OBJ_COUNT) % OBJ_COUNT;
            Mix_PlayChannel(-1, sounds.click, 0);
        }
        else if (mouseX >= editor.optionsMenuPosX + 305 && mouseX <= editor.optionsMenuPosX + 375 &&
            mouseY >= 15 && mouseY <= 85) {
            // Кнопка "тип->вправо"
            if (selectedObjectType == OBJ_TRUCK || selectedObjectType == OBJ_TURTLE ||
                selectedObjectType == OBJ_SNAKE || selectedObjectType == OBJ_LAWNMOWER) {
                clearEditorOptions();
            }
            selectedObjectType = (selectedObjectType + 1) % OBJ_COUNT;
            Mix_PlayChannel(-1, sounds.click, 0);
        }

        for (size_t i = 0; i < sizeof(inputFields) / sizeof(inputFields[0]); i++) {
            if (mouseX >= editor.optionsMenuPosX + inputFields[i].x &&
                mouseX <= editor.optionsMenuPosX + inputFields[i].x + inputFields[i].w &&
                mouseY >= inputFields[i].y &&
                mouseY <= inputFields[i].y + inputFields[i].h) {

                // Проверяем, активно ли поле для текущего объекта (текстового)
                if (!isFieldEnabledForObject(inputFields[i].fieldId - 1, selectedObjectType)) {
                    return;
                }

                if (editorOptions.focusedInputField != inputFields[i].fieldId) {

                    // Заполняем буфер текущим значением
                    switch (inputFields[i].fieldId) {
                    case 1: sprintf(editorOptions.inputBuffer, "%d", editorOptions.quantity); break;
                    case 2: sprintf(editorOptions.inputBuffer, "%d", editorOptions.length); break;
                    case 3: sprintf(editorOptions.inputBuffer, "%.1f", editorOptions.initialSpeed1); break;
                    case 4: sprintf(editorOptions.inputBuffer, "%.1f", editorOptions.initialSpeed2); break;
                    case 5: sprintf(editorOptions.inputBuffer, "%d", editorOptions.speedLevels1); break;
                    case 6: sprintf(editorOptions.inputBuffer, "%d", editorOptions.speedLevels2); break;
                    case 7: sprintf(editorOptions.inputBuffer, "%.1f", editorOptions.speedIncrease1); break;
                    case 8: sprintf(editorOptions.inputBuffer, "%.1f", editorOptions.speedIncrease2); break;
                    case 9: sprintf(editorOptions.inputBuffer, "%d", editorOptions.countLevels); break;
                    case 10: sprintf(editorOptions.inputBuffer, "%d", editorOptions.countIncrease); break;
                    }

                    editorOptions.focusedInputField = inputFields[i].fieldId;
                    editorOptions.isValueHidden = 1;  // Скрываем значение
                    editorOptions.inputBuffer[0] = '\0'; // Очищаем буфер ввода
                }
                return;
            }
        }

        // Проверяем, активно ли поле для текущего объекта (текстового)
        if (!isFieldEnabledForObject(10, selectedObjectType)) {
            return;
        }

        if (mouseX >= editor.optionsMenuPosX + 170 && mouseX <= editor.optionsMenuPosX + 240 &&
            mouseY >= editor.optionsMenuPosY + 315 && mouseY <= editor.optionsMenuPosY + 385) {
            isDirLeftPressed = 1;
            isDirRightPressed = 0;
        }
        else if (mouseX >= editor.optionsMenuPosX + 260 && mouseX <= editor.optionsMenuPosX + 330 &&
            mouseY >= editor.optionsMenuPosY + 315 && mouseY <= editor.optionsMenuPosY + 385) {
            isDirLeftPressed = 0;
            isDirRightPressed = 1;
        }

        // Клик вне полей ввода - снимаем фокус
        if (editorOptions.focusedInputField != 0) {
            // Если ничего не ввели, восстанавливаем предыдущее значение
            if (strlen(editorOptions.inputBuffer) == 0) {
                strcpy(editorOptions.inputBuffer, editorOptions.prevValue);
            }
            saveEditorInputValue();
            editorOptions.focusedInputField = 0;
            editorOptions.isValueHidden = 0;  // Показываем значение снова
            memset(editorOptions.inputBuffer, 0, sizeof(editorOptions.inputBuffer));
        }
    }
}

void editorMouseButton(GLFWwindow* window, int button, int action, int mods) {
    int isInOptionsMenu = 0;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (editor.isOptionsMenuOpen &&
            cursorX >= editor.optionsMenuPosX &&
            cursorX <= editor.optionsMenuPosX + 400 &&
            cursorY >= 0 &&
            cursorY <= 900 ||
            editor.optionsMenuPosX < GAME_WIDTH / 2 &&
            cursorX >= editor.optionsMenuPosX + 415 && cursorX <= editor.optionsMenuPosX + 485 &&
            cursorY >= 815 && cursorY <= 885 ||
            editor.optionsMenuPosX > GAME_WIDTH / 2 &&
            cursorX >= editor.optionsMenuPosX - 85 && cursorX <= editor.optionsMenuPosX - 15 &&
            cursorY >= 815 && cursorY <= 885 && gameState == EDITOR) {

            isInOptionsMenu = 1;

        }
        if (!isInOptionsMenu) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Если зона не выбрана - разрешаем панорамирование
            if (mouseX >= 0 && mouseX < GAME_WIDTH &&
                mouseY >= 0 && mouseY < HEIGHT) {
                isPanning = 1;
                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

        if (editor.isOptionsMenuOpen &&
            cursorX >= editor.optionsMenuPosX &&
            cursorX <= editor.optionsMenuPosX + 400 &&
            cursorY >= 0 &&
            cursorY <= 900 ||
            editor.optionsMenuPosX < GAME_WIDTH / 2 &&
            cursorX >= editor.optionsMenuPosX + 415 && cursorX <= editor.optionsMenuPosX + 485 &&
            cursorY >= 815 && cursorY <= 885 ||
            editor.optionsMenuPosX > GAME_WIDTH / 2 &&
            cursorX >= editor.optionsMenuPosX - 85 && cursorX <= editor.optionsMenuPosX - 15 &&
            cursorY >= 815 && cursorY <= 885 && gameState == EDITOR) {

            handleEditorOptionsMenuClick(cursorX, cursorY);

        }
        isPanning = 0;
    }
}


void editorMouseMove(GLFWwindow* window, double xpos, double ypos) {
    if (gameState == EDITOR) {
        // Сохраняем позицию курсора
        cursorX = xpos;
        cursorY = ypos;

        // Проверяем находится ли курсор в пределах меню параметров
        int isInOptionsMenu = 0;
        if (editor.isOptionsMenuOpen) {
            if (xpos >= editor.optionsMenuPosX &&
                xpos <= editor.optionsMenuPosX + 400 &&
                ypos >= 0 &&
                ypos <= 900 ||
                editor.optionsMenuPosX < GAME_WIDTH / 2 &&
                xpos >= editor.optionsMenuPosX + 415 && xpos <= editor.optionsMenuPosX + 485 &&
                ypos >= 815 && ypos <= 885 ||
                editor.optionsMenuPosX > GAME_WIDTH / 2 &&
                xpos >= editor.optionsMenuPosX - 85 && xpos <= editor.optionsMenuPosX - 15 &&
                ypos >= 815 && ypos <= 885) {
                isInOptionsMenu = 1;
            }
        }

        // Проверяем находится ли курсор в пределах игрового поля
        isCursorInEditorField = !isInOptionsMenu &&
            (xpos >= 0 && xpos < GAME_WIDTH &&
                ypos >= 0 && ypos < HEIGHT);

        if (isPanning) {
            xpos = fmax(0, fmin(xpos, GAME_WIDTH - 1));
            ypos = fmax(0, fmin(ypos, HEIGHT - 1));
            editor.panX += (xpos - lastMouseX);
            editor.panY += (ypos - lastMouseY);
            lastMouseX = xpos;
            lastMouseY = ypos;
        }
    }
}

void drawHitbox(float x, float y, float w, float h, Hitbox hb) {
    // Рассчитываем реальные границы хитбокса
    float left = x + GAME_WIDTH / CELLS_X * hb.left;
    float right = x + w - GAME_WIDTH / CELLS_X * hb.right;
    float top = y + h * hb.top;
    float bottom = y + h - h * hb.bottom;
    float width = right - left;
    float height = bottom - top;

    glDisable(GL_TEXTURE_2D);

    // Полупрозрачная заливка
    glColor4f(1.0f, 0.0f, 0.5f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();

    // Контур
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}


// Функция для проверки столкновения двух объектов с учетом их хитбоксов
int checkCollision(float x1, float y1, float w1, float h1, Hitbox hb1,
    float x2, float y2, float w2, float h2, Hitbox hb2) {
    // Рассчитываем реальные границы с учетом хитбоксов
    float left1 = x1 + GAME_WIDTH / CELLS_X * hb1.left;
    float right1 = x1 + w1 - GAME_WIDTH / CELLS_X * hb1.right;
    float top1 = y1 + h1 * hb1.top;
    float bottom1 = y1 + h1 - h1 * hb1.bottom;

    float left2 = x2 + GAME_WIDTH / CELLS_X * hb2.left;
    float right2 = x2 + w2 - GAME_WIDTH / CELLS_X * hb2.right;
    float top2 = y2 + h2 * hb2.top;
    float bottom2 = y2 + h2 - h2 * hb2.bottom;

    // Проверяем пересечение
    return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
}

// Хелпер-функция для проверки столкновения с игроком
int checkPlayerCollision(float objX, float objY, float objW, float objH, Hitbox objHitbox,
    float playerX, float playerY) {
    if (noclipEnabled) return 0;  // В режиме noclip столкновений нет

    float cellW = GAME_WIDTH / CELLS_X;
    float cellH = HEIGHT / CELLS_Y;
    Hitbox frogHitbox = { .left = 0.1f, .right = 0.1f, .top = 0.1f, .bottom = 0.1f };

    return checkCollision(objX, objY, objW, cellH, objHitbox,
        playerX, playerY, cellW, cellH, frogHitbox);
}


GLuint loadTexture(const char* path) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 4);
    if (!data) {
        printf("[DEBUG] Failed to load texture: %s\n", path);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void drawText(const char* text, float x, float y) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Чёрный цвет текста

    glRasterPos2f(x, y);
    for (const char* c = text; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_TEXTURE_2D);
}

int calculateTextWidth(const char* text) {
    int width = 0;
    for (const char* c = text; *c; c++) {
        width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
    }
    return width;
}

void drawTextCentered(const char* text, float y) {
    int textWidth = calculateTextWidth(text);
    float x = (WIDTH - textWidth) / 2.0f; // Центрирование по горизонтали

    glRasterPos2f(x, y);
    for (const char* c = text; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawQuad(float x, float y, float w, float h, int flipX) {
    glBegin(GL_QUADS);
    glEnable(GL_TEXTURE_2D);
    if (flipX) {
        glTexCoord2f(1, 0); glVertex2f(x, y);
        glTexCoord2f(0, 0); glVertex2f(x + w, y);
        glTexCoord2f(0, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(1, 1); glVertex2f(x, y + h);
    }
    else {
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x, y + h);
    }
    glEnd();
}


void cleanupSoundSystem() {
    Mix_FreeChunk(sounds.click);
    Mix_FreeChunk(sounds.explode);
    Mix_FreeChunk(sounds.jump);
    Mix_FreeChunk(sounds.levelup);
    Mix_FreeChunk(sounds.play);
    Mix_FreeMusic(sounds.gameMusic);
    Mix_FreeMusic(sounds.menuMusic);

    Mix_CloseAudio();
}

void updateEditorTransforms() {
    // Обновляем размеры с учетом зума
    editor.cellSize = editor.baseCellSize * editor.zoomFactor;
    editor.fieldWidth = editor.cellSize * CELLS_X;
    editor.fieldHeight = editor.cellSize * CELLS_Y;
}


void editorMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
    if (!(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) || gameState != EDITOR) {
        return;
    }

    // Получаем позицию мыши в мировых координатах
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Преобразуем в координаты мира до зума
    float worldX = (mouseX - editor.panX - editor.viewOffsetX) / editor.zoomFactor;
    float worldY = (mouseY - editor.panY - editor.viewOffsetY) / editor.zoomFactor;

    // Изменяем масштаб
    float oldZoom = editor.zoomFactor;
    editor.zoomFactor += yoffset * 0.1f;
    editor.zoomFactor = fmaxf(0.2f, fminf(2.0f, editor.zoomFactor));

    // Вычисляем новую позицию для сохранения точки под курсором
    editor.panX = mouseX - worldX * editor.zoomFactor - editor.viewOffsetX;
    editor.panY = mouseY - worldY * editor.zoomFactor - editor.viewOffsetY;

    // Обновляем трансформации
    updateEditorTransforms();
}

int isZonePlacementValid(ZoneType zone, int startRow) {
    if (startRow < 0 || startRow >= CELLS_Y)
        return 0;

    // Для газона проверяем 2 ряда
    int rowsNeeded = (zone == ZONE_LAWN) ? 2 : 1;
    if (startRow + rowsNeeded > CELLS_Y)
        return 0;

    // Проверяем конфликты с существующими зонами
    for (int i = startRow; i < startRow + rowsNeeded; i++) {
        // Не ставим никакю зону поверх другой
        if (editor.zoneTypes[i] != ZONE_NONE) {
            return 0;
        }

        // Другие зоны нельзя ставить поверх газона
        if (zone != ZONE_LAWN && editor.zoneTypes[i] == ZONE_LAWN) {
            return 0;
        }
    }

    return 1;
}

void initEditor() {

    // Фиксированный размер ячейки (как в оригинальной игре)
    editor.cellSize = GAME_WIDTH / 20.0f;
    // Размеры всего поля
    editor.fieldWidth = editor.cellSize * CELLS_X;
    editor.fieldHeight = editor.cellSize * CELLS_Y;

    // Центрирование поля (центр в GAME_WIDTH/2, HEIGHT/2)
    editor.viewOffsetX = (GAME_WIDTH - editor.fieldWidth) / 2;
    editor.viewOffsetY = (HEIGHT - editor.fieldHeight) / 2;

    editor.zoomFactor = 1.0f;
    editor.panX = editor.panY = 0.0f;
    editor.baseCellSize = GAME_WIDTH / 20.0f; // 20 клеток по ширине

    editor.initialLives = 3;
    editor.gameTime = 60.0f;
    editor.alligatorChance = 30;

    // Инициализация массива зон
    editor.zoneTypes = (ZoneType*)malloc(CELLS_Y * sizeof(int));
    for (int i = 0; i < CELLS_Y; i++) {
        editor.zoneTypes[i] = ZONE_NONE;
    }

    editorMode = EDITOR_MODE_NONE;
    memset(customHomes, 0, sizeof(customHomes));
    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        customHomes[i].x = customHomes[i].y = -999;
    }
    spawnX = spawnY = -999;

    // Выделяем память для настроек рядов
    editor.rowSettings = (RowSettings*)malloc(CELLS_Y * sizeof(RowSettings));
    if (editor.rowSettings == NULL) {
        exit(1);
    }

    // Инициализируем настройки рядов
    for (int i = 0; i < CELLS_Y; i++) {
        memset(&editor.rowSettings[i], 0, sizeof(RowSettings));
        editor.rowSettings[i].objectType = OBJ_EMPTY;
    }

    updateEditorTransforms();
}

int initSoundSystem() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return 0;
    }

    // Загрузка звуков
    sounds.click = Mix_LoadWAV("sounds/click1.ogg");
    sounds.explode = Mix_LoadWAV("sounds/explode.ogg");
    sounds.jump = Mix_LoadWAV("sounds/jump.ogg");
    sounds.levelup = Mix_LoadWAV("sounds/levelup.ogg");
    sounds.play = Mix_LoadWAV("sounds/play.ogg");
    sounds.quit = Mix_LoadWAV("sounds/quit.ogg");
    sounds.gameOver = (Mix_Music*)Mix_LoadWAV("sounds/gameOver.ogg");
    sounds.error = Mix_LoadWAV("sounds/error.ogg");

    // Загрузка музыки
    sounds.gameMusic = (Mix_Music*)Mix_LoadMUS("sounds/gameloop.mp3");
    sounds.menuMusic = (Mix_Music*)Mix_LoadMUS("sounds/menuloop.mp3");

    if (!sounds.click || !sounds.explode || !sounds.jump ||
        !sounds.levelup || !sounds.play || !sounds.quit ||
        !sounds.gameMusic || !sounds.menuMusic) {
        printf("[DEBUG] Failed to load some sounds! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }

    return 1;
}

GLuint getTextureForObject(ObjectType type) {
    switch (type) {
    case OBJ_CAR1: return car1Tex;
    case OBJ_CAR2: return car2Tex;
    case OBJ_CAR3: return car3Tex;
    case OBJ_CAR4: return car4Tex;
    case OBJ_TRUCK: return truckTex;
    case OBJ_LOG: return logMTex;
    case OBJ_TURTLE: return turtle1Tex;
    default: return 0;
    }
}

// для рекорда
void initCustomScoreSystem() {
    // Определяем стартовый ряд
    int startRow = (int)(spawnY / (HEIGHT / CELLS_Y));

    // Сбрасываем массив посещений
    memset(visitedRows, 0, sizeof(visitedRows));

    // Отмечаем стартовый ряд как посещенный
    visitedRows[startRow] = 1;
    lastVisitedRow = startRow;
}

void initCustomCars() {
    customCarCount = 0;
    int i_offset = 0;
    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType < OBJ_CAR1 || rs->objectType > OBJ_TRUCK) continue;

        float cellW = GAME_WIDTH / CELLS_X;
        float y = row * (HEIGHT / CELLS_Y);
        float carWidth = (rs->objectType == OBJ_TRUCK) ? cellW * 2 : cellW;
        float rightBoundary = GAME_WIDTH;

        // Вычисляем базовое расстояние между машинами
        float baseSpace = (float)GAME_WIDTH / (float)rs->quantity;

        // Размещаем машины
        float prevX = -baseSpace;
        for (int i = 0; i < rs->quantity; i++) {
            CustomObject* car = &customCars[customCarCount];

            // Вычисляем позицию с рандомным смещением
            float randomOffset = (i != 0) ? (rand() % (int)(baseSpace * 100)) / 100.0f - baseSpace / 2 : 0;

            car->x = prevX + baseSpace + ((i != 0) ? carWidth : 0) + randomOffset;
            prevX = car->x;
            car->y = y;
            car->startX = car->x;
            car->startY = car->y;
            car->speed = rs->initialSpeed1;
            car->direction = rs->direction;
            car->row = row;
            car->width = carWidth;
            car->texture = getTextureForObject(rs->objectType);
            car->hitbox = (rs->objectType == truckTex) ?
                (Hitbox) {
                .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f
            } :
                (Hitbox) {
                .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f
            };

                rightBoundary = (rightBoundary > car->x) ? rightBoundary : car->x;

                customCarCount++;
        }
        // присваиваем каждой машине rightBoundary
        for (int i = 0; i < rs->quantity; i++) {
            CustomObject* car = &customCars[i + i_offset];
            car->rightBoundary = rightBoundary;
        }
        i_offset = customCarCount;
    }
}

void initCustomSnakes() {
    customSnakeCount = 0;

    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType != OBJ_SNAKE) continue;

        float cellW = GAME_WIDTH / CELLS_X;
        float y = row * (HEIGHT / CELLS_Y);
        float snakeWidth = cellW * 2; // Змея занимает 2 клетки

        // Для змеи всегда только 1 объект в ряду
        if (customSnakeCount < CELLS_Y - 2) {
            CustomSnake* snake = &customSnakes[customSnakeCount];

            snake->base.x = (rs->direction == 1) ? 0 : GAME_WIDTH - snakeWidth;
            snake->base.y = y;
            snake->base.startX = snake->base.x;
            snake->base.startY = snake->base.y;
            snake->base.speed = rs->initialSpeed1;
            snake->base.direction = rs->direction;
            snake->base.row = row;
            snake->base.width = snakeWidth;
            snake->textures[0] = snake1Tex;
            snake->textures[1] = snake2Tex;
            snake->base.hitbox = (Hitbox){ .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f };
            snake->base.rightBoundary = GAME_WIDTH + GAME_WIDTH / CELLS_X * 2;
            snake->base.isActive = 1;
            snake->teleport = 0;

            customSnakeCount++;
        }
    }
}

void initCustomLogs() {
    customLogCount = 0;
    int i_offset = 0;

    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType != OBJ_LOG) continue;

        float cellW = GAME_WIDTH / CELLS_X;
        float y = row * (HEIGHT / CELLS_Y);
        float logWidth = cellW * rs->length; // ширина бревна в клетках
        float rightBoundary = GAME_WIDTH;

        // вычисляем базовое расстояние между брёвнами
        float baseSpace = (float)GAME_WIDTH / (float)rs->quantity;

        // размещаем брёвна
        float prevX = -baseSpace;
        for (int j = 0; j < rs->quantity; j++) {
            CustomObject* log = &customLogs[customLogCount];

            // вычисляем позицию с рандомным смещением
            float randomOffset = (j != 0) ? (rand() % (int)(baseSpace * 100)) / 100.0f - baseSpace / 2 : 0;

            log->x = prevX + baseSpace + ((j != 0) ? logWidth : 0) + randomOffset;
            prevX = log->x;
            log->y = y;
            log->startX = log->x;
            log->startY = log->y;
            log->speed = rs->initialSpeed1;
            log->direction = rs->direction;
            log->row = row;
            log->width = logWidth;
            log->texture = logMTex; // основная текстура бревна
            log->hitbox = (Hitbox){ .left = 0.05f, .right = 0.05f, .top = 0.05f, .bottom = 0.05f };

            rightBoundary = (rightBoundary > log->x) ? rightBoundary : log->x;

            customLogCount++;
        }
        // присваиваем каждому бревну rightBoundary
        for (int i = 0; i < rs->quantity; i++) {
            CustomObject* log = &customLogs[i + i_offset];
            log->rightBoundary = rightBoundary;
        }
        i_offset = customLogCount;
    }
}

void initCustomTurtles() {
    customTurtleCount = 0;
    int i_offset = 0;

    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType != OBJ_TURTLE) continue;

        float cellW = GAME_WIDTH / CELLS_X;
        float y = row * (HEIGHT / CELLS_Y);
        float turtleWidth = cellW * rs->length;

        // Определяем сколько черепах будут нырять (примерно 30%)
        int divingCount = (rs->quantity * 3) / 10;
        int* divingIndices = malloc(rs->quantity * sizeof(int));
        for (int k = 0; k < rs->quantity; k++) {
            divingIndices[k] = (k < divingCount) ? 1 : 0;
        }

        // Перемешиваем массив
        for (int k = 0; k < rs->quantity; k++) {
            int swapWith = rand() % rs->quantity;
            int temp = divingIndices[k];
            divingIndices[k] = divingIndices[swapWith];
            divingIndices[swapWith] = temp;
        }

        float rightBoundary = GAME_WIDTH;

        // Размещаем черепах
        float baseSpace = (float)GAME_WIDTH / (float)rs->quantity;
        float prevX = -baseSpace;

        for (int j = 0; j < rs->quantity; j++) {
            CustomTurtle* turtle = &customTurtles[customTurtleCount];

            float randomOffset = (j != 0) ? (rand() % (int)(baseSpace * 100)) / 100.0f - baseSpace / 2 : 0;

            turtle->base.x = prevX + baseSpace + ((j != 0) ? turtleWidth : 0) + randomOffset;
            prevX = turtle->base.x;
            turtle->base.y = y;
            turtle->base.startX = turtle->base.x;
            turtle->base.startY = turtle->base.y;
            turtle->base.speed = rs->initialSpeed1;
            turtle->base.direction = rs->direction;
            turtle->base.row = row;
            turtle->base.width = turtleWidth;
            turtle->base.texture = turtle1Tex;
            turtle->base.hitbox = (Hitbox){ .left = 0.1f, .right = 0.1f, .top = 0.1f, .bottom = 0.1f };

            turtle->state = 0;
            turtle->timer = 0.0f;
            turtle->phase = 0;
            turtle->phaseTimer = (float)(rand() % 2000) / 1000.0f;
            turtle->isDiving = divingIndices[j];

            rightBoundary = (rightBoundary > turtle->base.x) ? rightBoundary : turtle->base.x;

            customTurtleCount++;
        }
        // присваиваем rightBoundary
        for (int i = 0; i < rs->quantity; i++) {
            CustomTurtle* turtle = &customTurtles[i + i_offset];
            turtle->base.rightBoundary = rightBoundary;
        }
        i_offset = customTurtleCount;

        free(divingIndices);
    }
}

void initCustomLawnmowers() {
    customLawnmowerCount = 0;

    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType != OBJ_LAWNMOWER) continue;

        float cellW = GAME_WIDTH / CELLS_X;
        float y = row * (HEIGHT / CELLS_Y);

        // Для газонокосилок quantity может быть 1 или 2
        int lawnmowersToCreate = (rs->quantity == 2) ? 2 : 1;

        if (rs->quantity == 1) {
            customLawnmowers[customLawnmowerCount].isExist = 1;
            if (customLawnmowerCount % 2) {
                customLawnmowers[customLawnmowerCount].isExist = 0;
            }
        }

        for (int j = 0; j < lawnmowersToCreate; j++) {
            CustomLawnmower* lm = &customLawnmowers[customLawnmowerCount];

            int i = 0;
            while (editor.zoneTypes[row - i] == ZONE_LAWN) i++;

            if (j == 0) {
                if (i % 2) {
                    lm->isExist = 1;
                }
                else {
                    lm->isExist = 0;
                }
            }
            else {
                if (i % 2) {
                    lm->isExist = 0;
                }
                else {
                    lm->isExist = 1;
                }
            }

            // Первая косилка - слева, движется вправо
            if (j == 0) {
                lm->base.x = 0;
                lm->base.direction = 1;
            }
            // Вторая косилка - справа, движется влево
            else {
                lm->base.x = GAME_WIDTH - cellW;
                lm->base.direction = -1;
            }

            lm->base.y = y;
            lm->base.startX = lm->base.x;
            lm->base.startY = lm->base.y;
            lm->base.speed = (j == 0) ? rs->initialSpeed1 : rs->initialSpeed2;
            lm->base.row = row;
            lm->currentRow = row;
            lm->base.width = cellW;
            lm->base.texture = lawnmowerTex;
            lm->base.hitbox = (Hitbox){ .left = 0.2f, .right = 0.2f, .top = 0.2f, .bottom = 0.2f };
            lm->base.rightBoundary = GAME_WIDTH;
            lm->base.isActive = 1;

            // Специфичные для газонокосилки параметры
            lm->changeRowTimer = 0.0f;
            lm->isChangingRow = 0;
            lm->changeRowProgress = 0.0f;
            lm->targetY = y;
            lm->rotationAngle = 0.0f;
            lm->rotationDirection = (rand() % 2) ? 1 : -1; // Случайное направление вращения
            lm->rowChangeSpeedFactor = 0.3f;
            lm->teleport = 0;

            customLawnmowerCount++;
        }
    }
}

void initCustomAlligators() {
    customAlligatorCount = 0;

    // Проходим по всем кастомным бревнам
    for (int i = 0; i < customLogCount; i++) {
        CustomObject* log = &customLogs[i];
        RowSettings* rs = &editor.rowSettings[i];

        // Проверяем длину бревна (должно быть минимум 3 клетки)
        if (log->width < (GAME_WIDTH / CELLS_X) * 3) continue;

        // 30% шанс заменить это бревно на аллигатора
        if (rand() % 100 >= editor.alligatorChance) {
            i += rs->quantity;
            continue;
        }

        // Проверяем, нет ли уже аллигатора в этом ряду
        int alreadyHasAlligator = 0;
        for (int j = 0; j < customAlligatorCount; j++) {
            if (customAlligators[j].base.row == log->row) {
                alreadyHasAlligator = 1;
                break;
            }
        }
        if (alreadyHasAlligator) continue;

        // Создаем аллигатора на месте этого бревна
        CustomAlligator* alligator = &customAlligators[customAlligatorCount];

        // Копируем базовые параметры из бревна
        alligator->base = *log;
        alligator->base.texture = 0; // У аллигатора своя система рендера

        // Настраиваем хитбоксы
        alligator->base.hitbox = (Hitbox){ .left = 0.05f, .right = 0.05f, .top = 0.05f, .bottom = 0.05f };

        // Хитбокс головы зависит от направления
        float segmentWidth = GAME_WIDTH / CELLS_X;
        if (alligator->base.direction == 1) {
            alligator->headHitbox = (Hitbox){
                .left = alligator->base.width / GAME_WIDTH * CELLS_X - 0.5f - 0.05f,
                .right = 0.05f,
                .top = 0.3f,
                .bottom = 0.3f
            };
        }
        else {
            alligator->headHitbox = (Hitbox){
                .left = 0.05f,
                .right = alligator->base.width / GAME_WIDTH * CELLS_X - 0.5f - 0.05f,
                .top = 0.3f,
                .bottom = 0.3f
            };
        }

        // Настройки анимации
        alligator->mouthState = 0;
        alligator->mouthTimer = (float)(rand() % 1000) / 1000.0f; // Случайное начальное время
        alligator->mouthProgress = 0.0f;

        // Удаляем замененное бревно (помечаем как неактивное)
        log->width = 0;

        customAlligatorCount++;
    }
}

void initLevelRenderFBO();
void renderingFBO();

void initCustomObjects() {
    initCustomCars();
    initCustomSnakes();
    initCustomLogs();
    initCustomTurtles();
    initCustomLawnmowers();
    initCustomAlligators();

    initLevelRenderFBO();
    renderingFBO();
}

void drawZonePreview(int startRow, ZoneType zone) {
    int rowsToDraw = (zone == ZONE_LAWN) ? 2 : 1;

    for (int r = 0; r < rowsToDraw; r++) {
        int row = startRow + r;
        if (row >= CELLS_Y) break;

        for (int col = 0; col < CELLS_X; col++) {
            float x = col * editor.baseCellSize;
            float y = row * editor.baseCellSize;

            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(1, 0); glVertex2f(x + editor.baseCellSize, y);
            glTexCoord2f(1, 1); glVertex2f(x + editor.baseCellSize, y + editor.baseCellSize);
            glTexCoord2f(0, 1); glVertex2f(x, y + editor.baseCellSize);
            glEnd();
        }
    }
}

void drawTimer() {
    // Позиция и размеры
    float x = GAME_WIDTH + 50;
    float y = 274.0f;
    float height = 20.0f;

    glDisable(GL_TEXTURE_2D);

    // Белая обводка
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y - 2);
    glVertex2f(x + timerWidth + 2, y - 2);
    glVertex2f(x + timerWidth + 2, y + height + 2);
    glVertex2f(x - 2, y + height + 2);
    glEnd();

    // Фоновая подложка
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + timerWidth, y);
    glVertex2f(x + timerWidth, y + height);
    glVertex2f(x, y + height);
    glEnd();

    if (remainingTime >= 0) {
        // Зелёный прогресс
        float progressWidth = (remainingTime / editor.gameTime) * timerWidth;
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + progressWidth, y);
        glVertex2f(x + progressWidth, y + height);
        glVertex2f(x, y + height);
        glEnd();

        // Текст времени
        char timeText[32];
        snprintf(timeText, sizeof(timeText), "Time: %.1f", remainingTime);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(timeText, x + timerWidth + 10, y + height - 5);
    }
    else {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + timerWidth, y);
        glVertex2f(x + timerWidth, y + height);
        glVertex2f(x, y + height);
        glEnd();

        // Текст inf
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Time: inf", x + timerWidth + 10, y + height - 5);
    }

    glEnable(GL_TEXTURE_2D);
}

void renderGameUI(int lives, int score) {

    if (infoPanelTex) {
        glBindTexture(GL_TEXTURE_2D, infoPanelTex);
        drawQuad(GAME_WIDTH, 0, INFO_WIDTH, HEIGHT, 0);
    }

    // жизни
    if (lives <= 5) {
        for (int i = 0; i < lives; i++) {
            glBindTexture(GL_TEXTURE_2D, heartTex);
            drawQuad(GAME_WIDTH + 50 + i * 62, 50, 52, 52, 0);
        }
    }
    else {
        for (int i = 0; i < 5; i++) {
            glBindTexture(GL_TEXTURE_2D, heartTex);
            drawQuad(GAME_WIDTH + 50 + i * 62, 50, 52, 52, 0);
        }
        for (int i = 0; i < lives - 5; i++) {
            glBindTexture(GL_TEXTURE_2D, heartTex);
            drawQuad(GAME_WIDTH + 50 + i * 62, 112, 52, 52, 0);
        }
    }

    drawTimer();

    if (noclipEnabled) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2i(1250, 354);
        char* noclipText = "NOCLIP ON";
        for (char* c = noclipText; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glEnable(GL_TEXTURE_2D);
    }

    if (showHitboxes) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2i(1250, 324);
        char* hitboxText = "HITBOXES ON";
        for (char* c = hitboxText; *c; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glEnable(GL_TEXTURE_2D);
    }

    // счет
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(1250, 214);

    for (char* c = scoreText; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // текущий лвл
    char levelText[50];
    snprintf(levelText, sizeof(levelText), "Level: %d", currentLevel);

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(1250, 244);

    for (char* c = levelText; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_TEXTURE_2D);

    if (menuButtonTex) {
        glBindTexture(GL_TEXTURE_2D, menuButtonTex);
        drawQuad(WIDTH - CELL_SIZE, 20, 40, 40, 0);
    }
}

void renderCustomCar(CustomObject* car) {
    if (!car->texture) return;

    glBindTexture(GL_TEXTURE_2D, car->texture);
    drawQuad(car->x, car->y, car->width, CELL_SIZE, car->direction == 1);

    if (showHitboxes) {
        drawHitbox(car->x, car->y, car->width, CELL_SIZE, car->hitbox);
    }
}

void renderCustomSnake(CustomSnake* snake) {
    if (!snake->textures[0] || !snake->textures[1]) return;

    GLuint currentTex = snake->textures[snake->currentFrame];
    glBindTexture(GL_TEXTURE_2D, currentTex);
    drawQuad(snake->base.x, snake->base.y, snake->base.width, CELL_SIZE, snake->base.direction == 1);

    if (showHitboxes) {
        drawHitbox(snake->base.x, snake->base.y, snake->base.width, CELL_SIZE, snake->base.hitbox);
    }
}

void renderCustomLog(CustomObject* log) {
    if (!log->texture) return;

    float x = log->x;

    // левый конец бревна
    glBindTexture(GL_TEXTURE_2D, logLTex);
    drawQuad(x, log->y, CELL_SIZE, CELL_SIZE, 0);
    x += CELL_SIZE;

    // середина бревна
    glBindTexture(GL_TEXTURE_2D, logMTex);
    for (int i = 0; i < (log->width / CELL_SIZE) - 2; i++) {
        drawQuad(x, log->y, CELL_SIZE, CELL_SIZE, 0);
        x += CELL_SIZE;
    }

    // правый конец бревна
    if (x < log->x + log->width - CELL_SIZE / 2) {
        glBindTexture(GL_TEXTURE_2D, logRTex);
        drawQuad(x, log->y, CELL_SIZE, CELL_SIZE, 0);
    }

    if (showHitboxes) {
        drawHitbox(log->x, log->y, log->width, CELL_SIZE, log->hitbox);
    }
}

void renderCustomTurtle(CustomTurtle* turtle) {
    if (!turtle->base.texture) return;

    GLuint tex;
    switch (turtle->state) {
    case 0: tex = turtle1Tex; break;
    case 1: tex = turtle2Tex; break;
    case 2: tex = turtle3Tex; break;
    case 3: tex = turtle4Tex; break;
    default: tex = turtle1Tex; break;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    for (int i = 0; i < turtle->base.width / CELL_SIZE; i++) {
        drawQuad(turtle->base.x + i * CELL_SIZE,
            turtle->base.y,
            CELL_SIZE,
            CELL_SIZE,
            turtle->base.direction == 1);
    }

    if (showHitboxes && turtle->state < 3) { // Не показываем хитбокс, когда черепаха под водой
        drawHitbox(turtle->base.x, turtle->base.y,
            turtle->base.width, CELL_SIZE,
            turtle->base.hitbox);
    }
}

void renderCustomLawnmower(CustomLawnmower* lm) {
    if (!lm->base.texture || !lm->isExist) return;

    glPushMatrix();
    // Центр вращения - центр газонокосилки
    glTranslatef(lm->base.x + lm->base.width / 2,
        lm->base.y + HALF_CELL_SIZE,
        0);

    // Вращение только при смене ряда
    if (lm->isChangingRow) {
        glRotatef(lm->rotationAngle, 0, 0, 1);
    }

    // Возвращаем обратно
    glTranslatef(-(lm->base.x + lm->base.width / 2),
        -(lm->base.y + HALF_CELL_SIZE),
        0);

    glBindTexture(GL_TEXTURE_2D, lm->base.texture);
    drawQuad(lm->base.x, lm->base.y,
        lm->base.width, CELL_SIZE,
        lm->base.direction == 1);

    glPopMatrix();

    if (showHitboxes) {
        drawHitbox(lm->base.x, lm->base.y,
            lm->base.width, CELL_SIZE,
            lm->base.hitbox);
    }
}

void renderCustomAlligator(CustomAlligator* alligator) {

    int segments = (int)(alligator->base.width / CELL_SIZE);
    segments = segments < 2 ? 2 : segments; // Минимум 2 сегмента
    float x = alligator->base.x;
    int flipX = (alligator->base.direction == 1);

    if (flipX) {
        // Хвост (первый сегмент)
        glBindTexture(GL_TEXTURE_2D, alligatorTailTex);
        drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);
        x += CELL_SIZE;

        // Тело (средние сегменты)
        if (segments >= 3) {
            glBindTexture(GL_TEXTURE_2D, alligatorBodyTex);
            for (int i = 0; i < segments - 2; i++) {
                drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);
                x += CELL_SIZE;
            }
        }

        // Голова с анимацией пасти
        GLuint headTex = alligator->mouthProgress > 0.5f ?
            alligatorHeadOpenTex : alligatorHeadClosedTex;
        glBindTexture(GL_TEXTURE_2D, headTex);
        drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);

        if (showHitboxes) {
            // Основной хитбокс (зеленый)
            drawHitbox(alligator->base.x, alligator->base.y,
                alligator->base.width, CELL_SIZE,
                alligator->base.hitbox);

            // Хитбокс головы (красный)
            glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
            drawHitbox(alligator->base.x, alligator->base.y,
                alligator->base.width, CELL_SIZE,
                alligator->headHitbox);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
    }
    else {
        // Голова с анимацией пасти
        GLuint headTex = alligator->mouthProgress > 0.5f ?
            alligatorHeadOpenTex : alligatorHeadClosedTex;
        glBindTexture(GL_TEXTURE_2D, headTex);
        drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);
        x += CELL_SIZE;

        // Тело (средние сегменты)
        if (segments >= 3) {
            glBindTexture(GL_TEXTURE_2D, alligatorBodyTex);
            for (int i = 0; i < segments - 2; i++) {
                drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);
                x += CELL_SIZE;
            }
        }

        // Хвост (первый сегмент)
        glBindTexture(GL_TEXTURE_2D, alligatorTailTex);
        drawQuad(x, alligator->base.y, CELL_SIZE, CELL_SIZE, flipX);

        if (showHitboxes) {
            // Основной хитбокс (зеленый)
            drawHitbox(alligator->base.x, alligator->base.y,
                alligator->base.width, CELL_SIZE,
                alligator->base.hitbox);

            // Хитбокс головы (красный)
            glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
            drawHitbox(alligator->base.x, alligator->base.y,
                alligator->base.width, CELL_SIZE,
                alligator->headHitbox);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
    }
}

GLuint levelTex = 0;
GLuint levelFBO = 0;
int levelBaked = 0;

void drawQuadUV(float x, float y, float w, float h,
    float u0, float v0, float u1, float v1)
{
    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0);
    glVertex2f(x, y);

    glTexCoord2f(u1, v0);
    glVertex2f(x + w, y);

    glTexCoord2f(u1, v1);
    glVertex2f(x + w, y + h);

    glTexCoord2f(u0, v1);
    glVertex2f(x, y + h);
    glEnd();
}


void initLevelRenderFBO()
{
    glGenTextures(1, &levelTex);
    glBindTexture(GL_TEXTURE_2D, levelTex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        GAME_WIDTH,
        HEIGHT,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &levelFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, levelFBO);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        levelTex,
        0
    );

    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderingFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, levelFBO);
    glViewport(0, 0, GAME_WIDTH, HEIGHT);

    // Важно: сохранить матрицы проекции
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, GAME_WIDTH, HEIGHT, 0, -1, 1); // Y перевёрнут для FBO

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    for (int row = 0; row < CELLS_Y; row++) {
        GLuint tex = 0;

        switch (editor.zoneTypes[CELLS_Y - row - 1]) {
        case ZONE_WATER:  tex = waterTex;  break;
        case ZONE_ROAD:   tex = roadTex;   break;
        case ZONE_LAWN:   tex = lawnTex;   break;
        case ZONE_GRASS:  tex = groundTex; break;
        default: continue;
        }

        glBindTexture(GL_TEXTURE_2D, tex);

        // ПРАВИЛЬНЫЕ текстурные координаты:
        // u: от 0 до CELLS_X (повторение текстуры)
        // v: от 0 до 1 (одна текстура по вертикали)
        drawQuadUV(
            0.0f,
            row * CELL_SIZE,
            GAME_WIDTH,
            CELL_SIZE,
            0.0f, 0.0f,      // u0, v0
            (float)CELLS_X,  // u1 = CELLS_X (повторить текстуру CELLS_X раз)
            1.0f            // v1
        );
    }

    glDisable(GL_TEXTURE_2D); // Сетка без текстуры
    glColor3f(0.0f, 0.0f, 0.0f); // Чёрный цвет сетки
    glLineWidth(1.0f);

    // Вертикальные линии
    for (int i = 0; i <= CELLS_X; i++) {
        float lineX = i * CELL_SIZE;
        glBegin(GL_LINES);
        glVertex2f(lineX, 0);
        glVertex2f(lineX, HEIGHT);
        glEnd();
    }

    // Горизонтальные линии
    for (int i = 0; i <= CELLS_Y; i++) {
        float lineY = i * CELL_SIZE;
        glBegin(GL_LINES);
        glVertex2f(0, lineY);
        glVertex2f(GAME_WIDTH, lineY);
        glEnd();
    }

    glEnable(GL_TEXTURE_2D); // Включаем обратно для остального рендера

    // Восстанавливаем матрицы
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Восстанавливаем viewport (если у вас другой основной viewport)
    glViewport(0, 0, WIDTH, HEIGHT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    levelBaked = 1;
}


void renderCustomGame(float playerX, float playerY, int lives, int score) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glColor3f(1, 1, 1);

    if (levelTex && levelBaked) {
        glBindTexture(GL_TEXTURE_2D, levelTex);
        drawQuad(0, 0, GAME_WIDTH, HEIGHT, 0);
    }

    // car
    for (int i = 0; i < customCarCount; i++) {
        renderCustomCar(&customCars[i]);
    }

    // snake
    for (int i = 0; i < customSnakeCount; i++) {
        renderCustomSnake(&customSnakes[i]);
    }

    // log
    for (int i = 0; i < customLogCount; i++) {
        if (customLogs[i].width > 0) {
            renderCustomLog(&customLogs[i]);
        }
    }

    // turtle
    for (int i = 0; i < customTurtleCount; i++) {
        renderCustomTurtle(&customTurtles[i]);
    }

    // lawnmower
    for (int i = 0; i < customLawnmowerCount; i++) {
        renderCustomLawnmower(&customLawnmowers[i]);
    }

    // alligators
    for (int i = 0; i < customAlligatorCount; i++) {
        renderCustomAlligator(&customAlligators[i]);
    }

    if (homeEmptyTex && homeFill1Tex && homeFill2Tex && homeFill3Tex && homeFullTex) {
        for (int i = 0; i < MAX_HOMES_COUNT; i++) {
            if (customHomes[i].isExist) {
                GLuint tex;
                switch (customHomes[i].state) {
                case 0: tex = homeEmptyTex; break;
                case 1: tex = homeFill1Tex; break;
                case 2: tex = homeFill2Tex; break;
                case 3: tex = homeFill3Tex; break;
                case 4: tex = homeFullTex; break;
                default: tex = homeEmptyTex; break;
                }
                glBindTexture(GL_TEXTURE_2D, tex);
                drawQuad(customHomes[i].x, customHomes[i].y, CELL_SIZE, CELL_SIZE, 0);

                if (showHitboxes) {
                    Hitbox homeHitbox = { .left = 0.0f, .right = 0.0f, .top = 0.0f, .bottom = 0.0f };
                    drawHitbox(customHomes[i].x, customHomes[i].y,
                        CELL_SIZE, CELL_SIZE,
                        homeHitbox);
                }
            }
        }
    }

    int isFillingHome = 0;
    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        if (customHomes[i].state > 0 && customHomes[i].state < 4) {
            isFillingHome = 1;
            break;
        }
    }

    // Отрисовка лягушки
    if (frogIdleTex && frogJumpTex && !isFillingHome) {
        if (noclipEnabled) {
            glColor3f(1.0f, 0.5f, 0.5f); // Розоватый оттенок в режиме noclip
        }
        else {
            glColor3f(1.0f, 1.0f, 1.0f); // Нормальный цвет
        }

        GLuint tex = frogState == FROG_IDLE ? frogIdleTex : frogJumpTex;
        float rotation = 0.0f;

        // Определяем поворот на основе направления
        switch (frogDirection) {
        case DIR_UP: rotation = 0.0f; break;
        case DIR_DOWN: rotation = 180.0f; break;
        case DIR_LEFT: rotation = -90.0f; break;
        case DIR_RIGHT: rotation = 90.0f; break;
        }

        if (frogState == FROG_DROWNING) {
            if (drownFrame < 3) {
                tex = frogDrownTex[drownFrame];
            }
        }

        if (frogState == FROG_CRASHING) {
            if (crashFrame < 3) {
                tex = frogCrashTex[crashFrame];
            }
        }

        glPushMatrix();
        glTranslatef(playerX + HALF_CELL_SIZE, playerY + HALF_CELL_SIZE, 0);
        glRotatef(rotation, 0, 0, 1);
        glTranslatef(-HALF_CELL_SIZE, -HALF_CELL_SIZE, 0);

        glBindTexture(GL_TEXTURE_2D, tex);
        drawQuad(0, 0, CELL_SIZE, CELL_SIZE, 0);

        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); // Восстанавливаем белый цвет

        if (showHitboxes) {
            Hitbox frogHitbox = { .left = 0.1f, .right = 0.1f, .top = 0.1f, .bottom = 0.1f };
            drawHitbox(playerX, playerY, CELL_SIZE, CELL_SIZE, frogHitbox);
        }
    }

    // Отрисовка UI (жизни, счет и т.д.)
    renderGameUI(lives, score);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderEditorObjects() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glColor3f(1, 1, 1);

    for (int row = 0; row < CELLS_Y; row++) {
        if (editor.rowSettings[row].objectType == OBJ_EMPTY) continue;

        float cellSize = editor.baseCellSize;
        float yPos = row * cellSize + cellSize / 2 - 30; // Центрируем по вертикали
        float fieldWidth = CELLS_X * cellSize;

        int direction = editor.rowSettings[row].direction;
        int quantity = editor.rowSettings[row].quantity;
        int length = editor.rowSettings[row].length;
        float objectWidth = cellSize * length;

        // Определяем позицию для отрисовки (лево/право в зависимости от направления)
        float xPos;
        if (editor.rowSettings[row].objectType == OBJ_LOG || editor.rowSettings[row].objectType == OBJ_TURTLE) {
            xPos = (fieldWidth - objectWidth) / 2;
        }
        else {
            xPos = (direction == 1) ? 0 : (fieldWidth - objectWidth);
        }

        // Рисуем объекты в зависимости от типа
        switch (editor.rowSettings[row].objectType) {
        case OBJ_SNAKE:
            glBindTexture(GL_TEXTURE_2D, snake1Tex);
            drawQuad((xPos - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                cellSize * 2 / editor.zoomFactor, cellSize / editor.zoomFactor,
                direction == 1);
            break;

        case OBJ_LAWNMOWER: {
            int i = 0;
            while (editor.zoneTypes[row - i] == ZONE_LAWN) i++;

            if (i % 2 != 0) {
                if (quantity == 1) {
                    glBindTexture(GL_TEXTURE_2D, lawnmowerTex);
                    drawQuad((xPos + ((direction == 1) ? 0 : cellSize) - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                        (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                        cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                        direction == 1);
                }
                else if (quantity == 2) {
                    // Первая газонокосилка слева
                    glBindTexture(GL_TEXTURE_2D, lawnmowerTex);
                    drawQuad((0 - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                        (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                        cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                        1);

                    // Вторая газонокосилка справа
                    drawQuad((fieldWidth - cellSize - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                        (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                        cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                        0);
                }
            }
            break;
        }
        case OBJ_LOG: {
            // Рендеринг бревна с сегментами
            float segmentX = xPos;

            // Левый конец
            glBindTexture(GL_TEXTURE_2D, logLTex);
            drawQuad((segmentX - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                0);
            segmentX += cellSize;

            // Средние части (если длина > 2)
            if (length > 2) {
                glBindTexture(GL_TEXTURE_2D, logMTex);
                for (int i = 0; i < length - 2; i++) {
                    drawQuad((segmentX - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                        (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                        cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                        0);
                    segmentX += cellSize;
                }
            }

            // Правый конец
            glBindTexture(GL_TEXTURE_2D, logRTex);
            drawQuad((segmentX - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                0);
            break;
        }
        case OBJ_TURTLE: {
            // Рендерим N черепах подряд
            glBindTexture(GL_TEXTURE_2D, turtle1Tex);
            for (int i = 0; i < length; i++) {
                float offset = i * cellSize;
                drawQuad((xPos + offset - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                    (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                    cellSize / editor.zoomFactor, cellSize / editor.zoomFactor,
                    direction == 1);
            }
            break;
        }
        default: // Для машин, бревен и черепах
            glBindTexture(GL_TEXTURE_2D, getTextureForObject(editor.rowSettings[row].objectType));
            float width = (editor.rowSettings[row].objectType == OBJ_TRUCK) ?
                cellSize * 2 : cellSize;

            // Рисуем по центру
            drawQuad((fieldWidth / 2 - width / 2 - editor.panX - editor.viewOffsetX) / editor.zoomFactor,
                (yPos - editor.panY - editor.viewOffsetY) / editor.zoomFactor,
                width / editor.zoomFactor, cellSize / editor.zoomFactor,
                direction == 1);
            break;
        }
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderEditorPreview() {
    if (!isCursorInEditorField || editorMode == EDITOR_MODE_NONE) return;

    // Преобразуем координаты с учетом панорамирования и зума
    float worldX = (cursorX - editor.panX - editor.viewOffsetX) / editor.zoomFactor;
    float worldY = (cursorY - editor.panY - editor.viewOffsetY) / editor.zoomFactor;

    int hoveredCol = (int)(worldX / editor.baseCellSize);
    int hoveredRow = (int)(worldY / editor.baseCellSize);

    if (hoveredCol < 0 || hoveredCol >= CELLS_X ||
        hoveredRow < 0 || hoveredRow >= CELLS_Y) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (editorMode) {
    case EDITOR_MODE_HOME: {
        // Проверяем, можно ли разместить домик в этой позиции (верхний ряд)
        int isValid = (editor.zoneTypes[hoveredRow] == ZONE_WATER &&
            editor.rowSettings[hoveredRow].objectType == OBJ_EMPTY);

        // Рисуем полупрозрачный домик
        if (homeEmptyTex) {
            glBindTexture(GL_TEXTURE_2D, homeEmptyTex);
            glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
            float x = hoveredCol * editor.baseCellSize;
            float y = hoveredRow * editor.baseCellSize;
            drawQuad(x, y, editor.baseCellSize, editor.baseCellSize, 0);

            // Если позиция невалидна - рисуем красный поверх
            if (!isValid) {
                glDisable(GL_TEXTURE_2D);
                glColor4f(1.0f, 0.0f, 0.0f, 0.4f);
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + editor.baseCellSize, y);
                glVertex2f(x + editor.baseCellSize, y + editor.baseCellSize);
                glVertex2f(x, y + editor.baseCellSize);
                glEnd();
                glEnable(GL_TEXTURE_2D);
            }
        }
        break;
    }

    case EDITOR_MODE_SPAWN: {
        // Проверяем валидность позиции (нижний ряд)
        int isValid = (editor.zoneTypes[hoveredRow] == ZONE_GRASS &&
            editor.rowSettings[hoveredRow].objectType == OBJ_EMPTY);

        // Рисуем полупрозрачную лягушку
        if (frogIdleTex) {
            glBindTexture(GL_TEXTURE_2D, frogIdleTex);
            glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
            float x = hoveredCol * editor.baseCellSize;
            float y = hoveredRow * editor.baseCellSize;
            drawQuad(x + 4, y + 4, editor.baseCellSize - 8, editor.baseCellSize - 8, 0);

            if (!isValid) {
                glDisable(GL_TEXTURE_2D);
                glColor4f(1.0f, 0.0f, 0.0f, 0.4f);
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + editor.baseCellSize, y);
                glVertex2f(x + editor.baseCellSize, y + editor.baseCellSize);
                glVertex2f(x, y + editor.baseCellSize);
                glEnd();
                glEnable(GL_TEXTURE_2D);
            }
        }
        break;
    }

    case EDITOR_MODE_DELETE: {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

        // 1. Проверка домиков
        int hoveredHome = -1;
        for (int i = 0; i < MAX_HOMES_COUNT; i++) {
            if (!customHomes[i].isExist) continue;

            // Проверяем попадание курсора
            if (worldX - customHomes[i].x < editor.baseCellSize &&
                worldY - customHomes[i].y < editor.baseCellSize &&
                worldX - customHomes[i].x > 0 &&
                worldY - customHomes[i].y > 0) {
                hoveredHome = i;
                break;
            }
        }

        // 2. Проверка спавна
        int hoveredSpawn = 0;
        if (spawnX > 0 && spawnY > 0) {
            if (worldX - spawnX < editor.baseCellSize &&
                worldY - spawnY < editor.baseCellSize &&
                worldX - spawnX > 0 &&
                worldY - spawnY > 0) {
                hoveredSpawn = 1;
            }
        }

        // 3. Приоритет объектов над зонами
        if (hoveredHome >= 0 || hoveredSpawn) {
            // Подсветка объекта
            if (hoveredHome >= 0) {
                // Рассчитываем координаты домика
                float x = customHomes[hoveredHome].x;
                float y = customHomes[hoveredHome].y;
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + editor.baseCellSize, y);
                glVertex2f(x + editor.baseCellSize, y + editor.baseCellSize);
                glVertex2f(x, y + editor.baseCellSize);
                glEnd();
            }
            else {
                // Подсветка спавна
                float x = spawnX;
                float y = spawnY;
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + editor.baseCellSize, y);
                glVertex2f(x + editor.baseCellSize, y + editor.baseCellSize);
                glVertex2f(x, y + editor.baseCellSize);
                glEnd();
            }
        }
        else {
            // Подсветка зоны
            int rowsToHighlight = 1;
            int i = 0;
            if (editor.zoneTypes[hoveredRow] == ZONE_LAWN) {
                rowsToHighlight = 2;
                while (editor.zoneTypes[hoveredRow - i] == ZONE_LAWN) i++;
            }


            for (int r = 0; r < rowsToHighlight; r++) {
                int row = hoveredRow + r - 2 * r * (i % 2 == 0);
                if (row >= CELLS_Y) break;

                float y = row * editor.baseCellSize;
                glBegin(GL_QUADS);
                glVertex2f(0, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y + editor.baseCellSize);
                glVertex2f(0, y + editor.baseCellSize);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        break;
    }

    case EDITOR_MODE_OPTIONS: {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);

        int isExistObj = 0;
        for (int i = 0; i < CELLS_X; i++) {
            for (int j = 0; j < MAX_HOMES_COUNT; j++) {
                if (!customHomes[j].isExist) {
                    continue;
                }
                if (fabs(customHomes[j].x - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                    fabs(customHomes[j].y - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                    isExistObj = 1;
                    break;
                }
            }
            if (fabs(spawnX - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                fabs(spawnY - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                isExistObj = 1;
                break;
            }
        }

        // Подсветка зоны
        int rowsToHighlight = 1;
        int i = 0;
        if (editor.zoneTypes[hoveredRow] == ZONE_LAWN) {
            rowsToHighlight = 2;
            while (editor.zoneTypes[hoveredRow - i] == ZONE_LAWN) i++;
        }

        if (editor.zoneTypes[hoveredRow] != ZONE_NONE && !isExistObj) {
            for (int r = 0; r < rowsToHighlight; r++) {
                int row = hoveredRow + r - 2 * r * (i % 2 == 0);;
                if (row >= CELLS_Y) break;

                float y = row * editor.baseCellSize;
                glBegin(GL_QUADS);
                glVertex2f(0, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y + editor.baseCellSize);
                glVertex2f(0, y + editor.baseCellSize);
                glEnd();
            }
        }
        else if (isExistObj || editor.zoneTypes[hoveredRow] == ZONE_NONE) { // нельзя
            glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
            for (int r = 0; r < rowsToHighlight; r++) {
                int row = hoveredRow + r - 2 * r * (i % 2 == 0);;
                if (row >= CELLS_Y) break;

                float y = row * editor.baseCellSize;
                glBegin(GL_QUADS);
                glVertex2f(0, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y);
                glVertex2f(editor.fieldWidth / editor.zoomFactor, y + editor.baseCellSize);
                glVertex2f(0, y + editor.baseCellSize);
                glEnd();
            }
        }

        glDisable(GL_BLEND);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        break;
    }

    case EDITOR_MODE_ZONE: {
        glEnable(GL_TEXTURE_2D);
        int isValid = isZonePlacementValid(selectedZone, hoveredRow);
        GLuint previewTex = 0;

        // Основная текстура зоны
        switch (selectedZone) {
        case ZONE_GRASS: previewTex = groundTex; break;
        case ZONE_ROAD: previewTex = roadTex; break;
        case ZONE_LAWN: previewTex = lawnTex; break;
        case ZONE_WATER: previewTex = waterTex; break;
        }

        if (previewTex && invalidPatternTex) {
            glEnable(GL_BLEND);

            // 1. Отрисовка основной полупрозрачной текстуры
            glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
            glBindTexture(GL_TEXTURE_2D, previewTex);
            drawZonePreview(hoveredRow, selectedZone);

            // 2. Отрисовка штриховки для невалидных зон
            if (!isValid) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(1.0f, 0.0f, 0.0f, 1); // Красный цвет
                glBindTexture(GL_TEXTURE_2D, invalidPatternTex);
                drawZonePreview(hoveredRow, selectedZone);
            }

            glDisable(GL_BLEND);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
        break;
    }
    }

    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void renderEditorField() {
    // 1. Очистка буфера
    glClear(GL_COLOR_BUFFER_BIT);

    // 2. Отрисовка статичного фона (только игровая область)
    if (backgroundTex) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backgroundTex);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(GAME_WIDTH, 0);
        glTexCoord2f(1, 1); glVertex2f(GAME_WIDTH, HEIGHT);
        glTexCoord2f(0, 1); glVertex2f(0, HEIGHT);
        glEnd();
    }

    // 4. Применяем трансформации только к игровому полю
    glTranslatef(editor.viewOffsetX + editor.panX, editor.viewOffsetY + editor.panY, 0);
    glScalef(editor.zoomFactor, editor.zoomFactor, 1.0f);


    // 5. Отрисовка белого фона ячеек
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(editor.fieldWidth / editor.zoomFactor, 0);
    glVertex2f(editor.fieldWidth / editor.zoomFactor, editor.fieldHeight / editor.zoomFactor);
    glVertex2f(0, editor.fieldHeight / editor.zoomFactor);
    glEnd();

    glEnable(GL_TEXTURE_2D);

    for (int row = 0; row < CELLS_Y; row++) {
        GLuint tex = 0;
        switch (editor.zoneTypes[row]) {
        case ZONE_GRASS: tex = groundTex; break;
        case ZONE_ROAD: tex = roadTex; break;
        case ZONE_LAWN: tex = lawnTex; break;
        case ZONE_WATER: tex = waterTex; break;
        }

        if (tex) {
            glBindTexture(GL_TEXTURE_2D, tex);
            for (int col = 0; col < CELLS_X; col++) {
                float x = col * editor.baseCellSize;
                float y = row * editor.baseCellSize;
                drawQuad(x, y, editor.baseCellSize, editor.baseCellSize, 0);
            }
        }
    }
    renderEditorPreview();

    glEnable(GL_BLEND);

    // Отрисовка домиков
    if (homeEmptyTex) {
        glBindTexture(GL_TEXTURE_2D, homeEmptyTex);
        for (int i = 0; i < MAX_HOMES_COUNT; i++) {
            if (customHomes[i].x >= 0 && customHomes[i].isExist) {
                drawQuad(customHomes[i].x,
                    customHomes[i].y,
                    editor.baseCellSize,
                    editor.baseCellSize, 0);
            }
        }
    }

    // Отрисовка точки спавна
    if (frogIdleTex && spawnX >= 0 && spawnY >= 0) {
        glBindTexture(GL_TEXTURE_2D, frogIdleTex);
        drawQuad(spawnX + 4,
            spawnY + 4,
            editor.baseCellSize - 8,
            editor.baseCellSize - 8, 0);
    }

    glDisable(GL_BLEND);

    glDisable(GL_TEXTURE_2D);
    // 6. Отрисовка сетки
    glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    float lineWidth = 1.0f / editor.zoomFactor;
    lineWidth = fmaxf(0.25f, fminf(2.0f, lineWidth)); // Ограничиваем толщину
    glLineWidth(lineWidth);

    // Вертикальные линии
    for (int i = 0; i <= CELLS_X; i++) {
        float x = i * editor.baseCellSize;
        glBegin(GL_LINES);
        glVertex2f(x, 0);
        glVertex2f(x, editor.fieldHeight / editor.zoomFactor);
        glEnd();
    }

    // Горизонтальные линии
    for (int i = 0; i <= CELLS_Y; i++) {
        float y = i * editor.baseCellSize;
        glBegin(GL_LINES);
        glVertex2f(0, y);
        glVertex2f(editor.fieldWidth / editor.zoomFactor, y);
        glEnd();
    }

    glEnable(GL_TEXTURE_2D);

    // Сохраняем текущую матрицу
    glPushMatrix();

    // Применяем трансформации редактора
    glTranslatef(editor.viewOffsetX + editor.panX, editor.viewOffsetY + editor.panY, 0);
    glScalef(editor.zoomFactor, editor.zoomFactor, 1.0f);

    // Отрисовываем объекты
    renderEditorObjects();

    // Восстанавливаем матрицу
    glPopMatrix();


    // 8. Отрисовка UI панели (с отдельной проекцией)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1); // На весь экран
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    if (infoPanelTex) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, infoPanelTex);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(GAME_WIDTH, 0);
        glTexCoord2f(1, 0); glVertex2f(WIDTH, 0);
        glTexCoord2f(1, 1); glVertex2f(WIDTH, HEIGHT);
        glTexCoord2f(0, 1); glVertex2f(GAME_WIDTH, HEIGHT);
        glEnd();
    }
    glDisable(GL_BLEND);

}

void renderEditorUI() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    // Отображение информации о размере уровня
    char sizeText[50];
    snprintf(sizeText, sizeof(sizeText), "Level size: %dx%d", CELLS_X, CELLS_Y);

    // Отображение текущего масштаба
    char zoomText[32];
    snprintf(zoomText, sizeof(zoomText), "Zoom: %.0f%%", editor.zoomFactor * 100);

    // 1. Верхняя часть (300px) - информация
    // Размер уровня
    glRasterPos2i(GAME_WIDTH + 50, 50);
    for (char* c = sizeText; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Масштаб
    glRasterPos2i(GAME_WIDTH + 50, 90);
    for (char* c = zoomText; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    // 2. Средняя часть (600px) - кнопки (2x3)
    int buttonWidth = 140;
    int buttonHeight = 140;
    int startY = 180;
    int margin = 40;

    if (settingsButtonTex) {
        glBindTexture(GL_TEXTURE_2D, settingsButtonTex);
        drawQuad(GAME_WIDTH + margin * 2 + buttonWidth + 20, startY - (buttonHeight + margin) + 20, buttonWidth - 40, buttonHeight - 40, 0);
    }

    // Первый ряд: Copy-Paste и Delete (подняты на buttonHeight + margin)
    if (optionButtonTex) {
        if (editorMode == EDITOR_MODE_OPTIONS) {
            glColor3f(0.7f, 1.0f, 0.7f); // Подсветка активной кнопки
        }
        glBindTexture(GL_TEXTURE_2D, optionButtonTex);
        drawQuad(GAME_WIDTH + margin, startY, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f); // Восстанавливаем цвет
    }

    if (deleteButtonTex) {
        if (editorMode == EDITOR_MODE_DELETE) {
            glColor3f(0.7f, 1.0f, 0.7f); // Подсветка активной кнопки
        }
        glBindTexture(GL_TEXTURE_2D, deleteButtonTex);
        drawQuad(GAME_WIDTH + margin * 2 + buttonWidth, startY, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f); // Восстанавливаем цвет
    }

    // Второй ряд: Home и Spawn (новые кнопки)
    if (homeButtonTex) {
        glBindTexture(GL_TEXTURE_2D, homeButtonTex);
        if (editorMode == EDITOR_MODE_HOME) {
            glColor3f(0.7f, 1.0f, 0.7f); // Подсветка активной кнопки
        }
        drawQuad(GAME_WIDTH + margin, startY + buttonHeight + margin, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    if (spawnButtonTex) {
        glBindTexture(GL_TEXTURE_2D, spawnButtonTex);
        if (editorMode == EDITOR_MODE_SPAWN) {
            glColor3f(0.7f, 1.0f, 0.7f); // Подсветка активной кнопки
        }
        drawQuad(GAME_WIDTH + margin * 2 + buttonWidth, startY + buttonHeight + margin, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // Третий ряд: Grass и Road
    if (grassButtonTex) {
        if (selectedZone == ZONE_GRASS) {
            glColor3f(0.7f, 1.0f, 0.7f); // Подсветка выбранной кнопки
        }
        glBindTexture(GL_TEXTURE_2D, grassButtonTex);
        drawQuad(GAME_WIDTH + margin, startY + (buttonHeight + margin) * 2, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    if (roadButtonTex) {
        if (selectedZone == ZONE_ROAD) {
            glColor3f(0.7f, 1.0f, 0.7f);
        }
        glBindTexture(GL_TEXTURE_2D, roadButtonTex);
        drawQuad(GAME_WIDTH + margin * 2 + buttonWidth, startY + (buttonHeight + margin) * 2, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // Четвертый ряд: Lawn и Water
    if (lawnButtonTex) {
        if (selectedZone == ZONE_LAWN) {
            glColor3f(0.7f, 1.0f, 0.7f);
        }
        glBindTexture(GL_TEXTURE_2D, lawnButtonTex);
        drawQuad(GAME_WIDTH + margin, startY + (buttonHeight + margin) * 3, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    if (waterButtonTex) {
        if (selectedZone == ZONE_WATER) {
            glColor3f(0.7f, 1.0f, 0.7f);
        }
        glBindTexture(GL_TEXTURE_2D, waterButtonTex);
        drawQuad(GAME_WIDTH + margin * 2 + buttonWidth, startY + (buttonHeight + margin) * 3, buttonWidth, buttonHeight, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // options-меню
    if (editor.isOptionsMenuOpen && editorOptionsMenuTex) {

        if (editor.optionsMenuPosX > GAME_WIDTH / 2) {
            glBindTexture(GL_TEXTURE_2D, spawnButtonTex);
            drawQuad(editor.optionsMenuPosX - 85, editor.optionsMenuPosY + 815, 70, 70, 0);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, spawnButtonTex);
            drawQuad(editor.optionsMenuPosX + 415, editor.optionsMenuPosY + 815, 70, 70, 0);
        }

        glBindTexture(GL_TEXTURE_2D, editorOptionsMenuTex);
        drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY, 400, 900, 0);

        if (editor.isOptionsMenuOpen && editorOptionsMenuTex) {
            glBindTexture(GL_TEXTURE_2D, editorOptionsMenuTex);
            drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY, 400, 900, 0);

            // Массив полей для отрисовки
            const struct {
                int fieldId;
                float x, y;
                const char* value;
            } fieldsToDraw[] = {
                {1, 125, 110, editorOptions.focusedInputField == 1 ? editorOptions.inputBuffer : ""},  // Количество
                {2, 125, 210, editorOptions.focusedInputField == 2 ? editorOptions.inputBuffer : ""},  // Длина
                {3, 120, 410, editorOptions.focusedInputField == 3 ? editorOptions.inputBuffer : ""},  // Нач. скорость 1
                {4, 260, 410, editorOptions.focusedInputField == 4 ? editorOptions.inputBuffer : ""},  // Нач. скорость 2
                {5, 120, 510, editorOptions.focusedInputField == 5 ? editorOptions.inputBuffer : ""},  // Уровни скорости 1
                {6, 260, 510, editorOptions.focusedInputField == 6 ? editorOptions.inputBuffer : ""},  // Уровни скорости 2
                {7, 120, 610, editorOptions.focusedInputField == 7 ? editorOptions.inputBuffer : ""},  // Пов. скорости 1
                {8, 260, 610, editorOptions.focusedInputField == 8 ? editorOptions.inputBuffer : ""},  // Пов. скорости 2
                {9, 190, 710, editorOptions.focusedInputField == 9 ? editorOptions.inputBuffer : ""},  // Уровни количества
                {10, 190, 810, editorOptions.focusedInputField == 10 ? editorOptions.inputBuffer : ""} // Пов. количества
            };

            for (size_t i = 0; i < sizeof(fieldsToDraw) / sizeof(fieldsToDraw[0]); i++) {
                char displayText[32] = "";

                if (editorOptions.focusedInputField == fieldsToDraw[i].fieldId) {
                    if (editorOptions.isValueHidden) {
                        // Поле в фокусе, но значение скрыто - ничего не показываем
                        displayText[0] = '\0';
                    }
                    else {
                        // Показываем вводимый текст
                        strcpy(displayText, editorOptions.inputBuffer);
                    }
                }
                else {
                    // Получаем сохранённое значение
                    switch (fieldsToDraw[i].fieldId) {
                    case 1: sprintf(displayText, "%d", editorOptions.quantity); break;
                    case 2: sprintf(displayText, "%d", editorOptions.length); break;
                    case 3: sprintf(displayText, "%.1f", editorOptions.initialSpeed1); break;
                    case 4: sprintf(displayText, "%.1f", editorOptions.initialSpeed2); break;
                    case 5: sprintf(displayText, "%d", editorOptions.speedLevels1); break;
                    case 6: sprintf(displayText, "%d", editorOptions.speedLevels2); break;
                    case 7: sprintf(displayText, "%.1f", editorOptions.speedIncrease1); break;
                    case 8: sprintf(displayText, "%.1f", editorOptions.speedIncrease2); break;
                    case 9: sprintf(displayText, "%d", editorOptions.countLevels); break;
                    case 10: sprintf(displayText, "%d", editorOptions.countIncrease); break;
                    }
                }

                drawText(displayText,
                    editor.optionsMenuPosX + fieldsToDraw[i].x + 10,
                    fieldsToDraw[i].y + 40);
            }

            glEnable(GL_TEXTURE_2D);

            if (isDirLeftPressed && buttonDirectionLeftTex) {
                glBindTexture(GL_TEXTURE_2D, buttonDirectionLeftTex);
                drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY, 400, 900, 0);
            }

            if (isDirRightPressed && buttonDirectionRightTex) {
                glBindTexture(GL_TEXTURE_2D, buttonDirectionRightTex);
                drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY, 400, 900, 0);
            }

            // Отрисовка предпросмотра выбранного объекта
            if (objectPreviewTextures[selectedObjectType]) {
                glBindTexture(GL_TEXTURE_2D, objectPreviewTextures[selectedObjectType]);
                drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY,
                    400, 900, 0);

                if (editorOptions.quantity || selectedObjectType == OBJ_EMPTY && !editorOptions.quantity) {
                    if (objectAllowedOptions[selectedObjectType] == editorOptionsNotAllowedOneLawnmower && editorOptions.quantity == 2) {
                        objectAllowedOptions[selectedObjectType] = editorOptionsNotAllowedTwoLawnmowers;
                    }
                    else if (objectAllowedOptions[selectedObjectType] == editorOptionsNotAllowedTwoLawnmowers && editorOptions.quantity == 1) {
                        objectAllowedOptions[selectedObjectType] = editorOptionsNotAllowedOneLawnmower;
                    }


                    // отрисовка не разрешаемых кнопок
                    glBindTexture(GL_TEXTURE_2D, objectAllowedOptions[selectedObjectType]);
                    drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY,
                        400, 900, 0);
                }
                else {

                    // Проверяем, можно ли разместить объект на текущей зоне
                    int isAllowed = isObjectAllowedForZone(selectedObjectType, editor.zoneTypes[editor.selectedZoneRow]);

                    // отрисовка не разрешаемых кнопок при нулевом количестве
                    if (!isAllowed) {
                        glBindTexture(GL_TEXTURE_2D, editorOptionsNotAllowedEmpty);
                        drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY,
                            400, 900, 0);
                    }
                    else {
                        glBindTexture(GL_TEXTURE_2D, editorOptionsNotAllowedZeroQuantity);
                        drawQuad(editor.optionsMenuPosX, editor.optionsMenuPosY,
                            400, 900, 0);
                    }
                }
            }

            // Проверяем, можно ли разместить объект на текущей зоне
            int isAllowed = isObjectAllowedForZone(selectedObjectType, editor.zoneTypes[editor.selectedZoneRow]);

            if (invalidPatternTex) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, invalidPatternTex);
                // Если объект нельзя разместить - рисуем поверх invalid_pattern + над кнопкой сохранения
                if (!isAllowed) {
                    drawQuad(editor.optionsMenuPosX + 205, editor.optionsMenuPosY + 5, 90, 90, 0);
                }
                if (!isAllowed ||
                    editorOptions.quantity == 0 && selectedObjectType != OBJ_EMPTY ||
                    !isDirLeftPressed && !isDirRightPressed && !(selectedObjectType == OBJ_LAWNMOWER && editorOptions.quantity == 2)) {
                    if (editor.optionsMenuPosX > GAME_WIDTH / 2) {
                        drawQuad(editor.optionsMenuPosX - 85, editor.optionsMenuPosY + 815, 70, 70, 0);
                    }
                    else {
                        drawQuad(editor.optionsMenuPosX + 415, editor.optionsMenuPosY + 815, 70, 70, 0);
                    }
                }
                glDisable(GL_BLEND);
            }

            // Отрисовка названия типа
            char* typeName = "Empty";

            glDisable(GL_TEXTURE_2D);
            glColor3f(1.0f, 1.0f, 1.0f);
            if (isAllowed && selectedObjectType == OBJ_EMPTY) {
                glRasterPos2i(editor.optionsMenuPosX + 225, editor.optionsMenuPosY + 55);
                for (typeName; *typeName; typeName++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *typeName);
                }
            }
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}

void renderLevelCreateDialog() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending с правильными параметрами для полупрозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Полупрозрачный темный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Теперь точно черный с прозрачностью 0.5
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1); // Восстанавливаем белый цвет для текстур

    // Окно диалога (центрированное)
    int dialogWidth = 500;
    int dialogHeight = 150;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2 - 50, dialogWidth, dialogHeight + 50, 0);
    }

    // Отрисовка кнопок Create и Cancel (симметрично)
    int dialogX = (WIDTH - 500) / 2;
    int dialogY = (HEIGHT - 150) / 2;

    if (createButtonTex) {
        glBindTexture(GL_TEXTURE_2D, createButtonTex);
        drawQuad(dialogX + 100, dialogY + 75, 120, 50, 0);  // Create слева
    }

    if (invalidPatternTex && !strlen(levelName)) {
        glBindTexture(GL_TEXTURE_2D, invalidPatternTex);
        drawQuad(dialogX + 100, dialogY + 75, 120, 50, 0);
    }

    if (cancelButtonTex) {
        glBindTexture(GL_TEXTURE_2D, cancelButtonTex);
        drawQuad(dialogX + 280, dialogY + 75, 120, 50, 0);  // Cancel справа
    }

    if (strlen(levelName) > 0) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        char selectedLevelNameText[24];
        drawTextCentered("A level with that name alredy exist", (HEIGHT - dialogHeight) / 2 - 10);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawTextCentered("Do you want to OVERWRITE it?", (HEIGHT - dialogHeight) / 2 + 20);
        glColor3f(1.0f, 1.0f, 1.0f);
        sprintf(selectedLevelNameText, "\"%s\"", levelName);
        drawTextCentered(selectedLevelNameText, (HEIGHT - dialogHeight) / 2 + 50);
        glEnable(GL_TEXTURE_2D);
    }

    // Восстанавливаем предыдущие настройки blending
    if (!blendEnabled) {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blendSrc, blendDst);
}

void renderLevelLoadDialog() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending для полупрозрачного фона
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Темный полупрозрачный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    // Отрисовка окна диалога
    int dialogWidth = 500;
    int dialogHeight = 400;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }

    // Позиционирование элементов
    int dialogX = (WIDTH - dialogWidth) / 2;
    int dialogY = (HEIGHT - dialogHeight) / 2;
    int yOffset = dialogY + 50;

    // Отрисовка названия уровня
    glDisable(GL_TEXTURE_2D);
    char titleText[64];
    snprintf(titleText, sizeof(titleText), "\"%s\"", levelName);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawTextCentered("The level has been SUCCESSFULLY loaded:", yOffset - 8);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextCentered(titleText, yOffset + 27);
    glEnable(GL_TEXTURE_2D);

    // Кнопки (располагаем по центру с отступом)
    yOffset += 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 20;

    // Play button
    if (playTex) {
        glBindTexture(GL_TEXTURE_2D, playTex);
        drawQuad((WIDTH - buttonWidth) / 2, yOffset, buttonWidth, buttonHeight, 0);
    }
    yOffset += buttonHeight + buttonSpacing;

    // Editor button
    if (editorTex) {
        glBindTexture(GL_TEXTURE_2D, editorTex);
        drawQuad((WIDTH - buttonWidth) / 2, yOffset, buttonWidth, buttonHeight, 0);
    }
    yOffset += buttonHeight + buttonSpacing;

    // Delete button
    if (deleteTex) {
        glBindTexture(GL_TEXTURE_2D, deleteTex);
        drawQuad((WIDTH - buttonWidth) / 2, yOffset, buttonWidth, buttonHeight, 0);
    }
    yOffset += buttonHeight + buttonSpacing;

    // Cancel button
    if (cancelButtonTex) {
        glBindTexture(GL_TEXTURE_2D, cancelButtonTex);
        drawQuad((WIDTH - buttonWidth) / 2, yOffset, buttonWidth, buttonHeight, 0);
    }

    // Восстанавливаем настройки blending
    if (!blendEnabled) glDisable(GL_BLEND);
    glBlendFunc(blendSrc, blendDst);
}

void renderNotValidEditor() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending для полупрозрачного фона
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Темный полупрозрачный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    // Отрисовка окна диалога
    int dialogWidth = 500;
    int dialogHeight = 400;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }
    if (notValidEditorTex) {
        glBindTexture(GL_TEXTURE_2D, notValidEditorTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }
    // Восстанавливаем настройки blending
    if (!blendEnabled) glDisable(GL_BLEND);
    glBlendFunc(blendSrc, blendDst);
}

void renderLevelError() {
    // Фон
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    // Окно
    int dialogWidth = 500;
    int dialogHeight = 200;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }

    // Текст
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 0.0f, 0.0f);
    char* mode = (gameState == LEVEL_LOAD_ERROR) ? "LOAD" : "CREATE";
    char value[20];
    sprintf(value, "LEVEL %s ERROR!", mode);
    drawTextCentered(value, (HEIGHT - dialogHeight) / 2 + 50);
    glColor3f(1.0f, 1.0f, 1.0f);
    char* reason = (gameState == LEVEL_LOAD_ERROR) ? "Level doesn't exist or invalid name" : "Invalid name";
    drawTextCentered(reason, (HEIGHT - dialogHeight) / 2 + 90);
    glEnable(GL_TEXTURE_2D);

    // Кнопка Cancel
    if (cancelButtonTex) {
        glBindTexture(GL_TEXTURE_2D, cancelButtonTex);
        drawQuad((WIDTH - 200) / 2, (HEIGHT + dialogHeight) / 2 - 70, 200, 50, 0);
    }
}

void renderLevelDeleteConfirm() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending для полупрозрачного фона
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Полупрозрачный темный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Отрисовка диалога
    int dialogWidth = 500;
    int dialogHeight = 150;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2 - 50, dialogWidth, dialogHeight + 50, 0);
    }

    // Текст подтверждения
    glDisable(GL_TEXTURE_2D);
    char levelNameQuotes[25];
    sprintf(levelNameQuotes, "\"%s\"?", levelName);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawTextCentered("Are you sure you want to DELETE level", (HEIGHT - dialogHeight) / 2 - 10);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextCentered(levelNameQuotes, (HEIGHT - dialogHeight) / 2 + 20);
    glEnable(GL_TEXTURE_2D);

    // Кнопки
    int buttonY = (HEIGHT - dialogHeight) / 2 + 75;
    if (deleteTex) {
        glBindTexture(GL_TEXTURE_2D, deleteTex);
        drawQuad((WIDTH - 200) / 2 - 110, buttonY, 200, 50, 0);
    }
    if (cancelButtonTex) {
        glBindTexture(GL_TEXTURE_2D, cancelButtonTex);
        drawQuad((WIDTH - 200) / 2 + 110, buttonY, 200, 50, 0);
    }

    // Восстанавливаем настройки
    if (!blendEnabled) glDisable(GL_BLEND);
    glBlendFunc(blendSrc, blendDst);
}

void renderEditorExitDialog() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending с правильными параметрами для полупрозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Полупрозрачный темный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Теперь точно черный с прозрачностью 0.5
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1); // Восстанавливаем белый цвет для текстур

    // Окно диалога (центрированное)
    int dialogWidth = 300;
    int dialogHeight = 550;

    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }

    int dialogX = (WIDTH - 300) / 2;
    int dialogY = (HEIGHT - 550) / 2;

    if (resumeButtonTex) {
        glBindTexture(GL_TEXTURE_2D, resumeButtonTex);
        drawQuad(dialogX + 90, dialogY + 50, 120, 50, 0);
    }

    if (saveButtonTex) {
        glBindTexture(GL_TEXTURE_2D, saveButtonTex);
        drawQuad(dialogX + 90, dialogY + 150, 120, 50, 0);
    }

    if (saveAndPlayButtonTex) {
        glBindTexture(GL_TEXTURE_2D, saveAndPlayButtonTex);
        drawQuad(dialogX + 90, dialogY + 250, 120, 50, 0);
    }

    if (saveAndExitButtonTex) {
        glBindTexture(GL_TEXTURE_2D, saveAndExitButtonTex);
        drawQuad(dialogX + 90, dialogY + 350, 120, 50, 0);
    }

    if (exitButtonTex) {
        glBindTexture(GL_TEXTURE_2D, exitButtonTex);
        drawQuad(dialogX + 90, dialogY + 450, 120, 50, 0);
    }

    // Восстанавливаем предыдущие настройки blending
    if (!blendEnabled) {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blendSrc, blendDst);
}

void renderEditorSettingsDialog() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending для полупрозрачного фона
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Полупрозрачный темный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Отрисовка диалога
    int dialogWidth = 500;
    int dialogHeight = 500;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2 - 50, dialogWidth, dialogHeight + 100, 0);
    }

    if (editorSettings) {
        glBindTexture(GL_TEXTURE_2D, editorSettings);
        drawQuad((WIDTH - dialogWidth) / 2, (HEIGHT - dialogHeight) / 2, dialogWidth, dialogHeight, 0);
    }

    const int fieldY[3] = { 70, 200, 330 };
    const int fieldX[3] = { 370, 365, 320 };
    for (int i = 0; i < 3; i++) {
        const char* text = NULL;
        char tempBuffer[16];

        if (settingsDialog.focusedField == i + 1) {
            switch (i) {
            case 0: text = settingsDialog.livesBuffer; break;
            case 1: text = settingsDialog.alligatorBuffer; break;
            case 2: text = settingsDialog.timeBuffer; break;
            }
        }
        else {
            switch (i) {
            case 0: snprintf(tempBuffer, 16, "%d", settingsDialog.tempLives); break;
            case 1: snprintf(tempBuffer, 16, "%d", settingsDialog.tempAlligatorChance); break;
            case 2: snprintf(tempBuffer, 16, "%d", (int)settingsDialog.tempGameTime); break;
            }
            text = tempBuffer;
        }

        // Отрисовка текста
        if (text) {
            glDisable(GL_TEXTURE_2D);
            glColor3f(1, 1, 1);
            drawText(text, (WIDTH - 500) / 2 + fieldX[i], (HEIGHT - 500) / 2 + fieldY[i]);
            glEnable(GL_TEXTURE_2D);
        }
    }

    // Восстанавливаем настройки
    if (!blendEnabled) glDisable(GL_BLEND);
    glBlendFunc(blendSrc, blendDst);
}

void renderMenu() {
    glEnable(GL_BLEND);
    glColor3f(1, 1, 1);
    if (menuBackgroundTex) {
        glBindTexture(GL_TEXTURE_2D, menuBackgroundTex);
        drawQuad(0, 0, WIDTH, HEIGHT, 0);
    }

    if (titleTex) {
        glBindTexture(GL_TEXTURE_2D, titleTex);
        drawQuad((WIDTH - 522) / 2, 150, 522, 166, 0);
    }
    if (playTex) {
        glBindTexture(GL_TEXTURE_2D, playTex);
        drawQuad((WIDTH - 200) / 2, 400, 200, 50, 0);
    }
    if (levelsTex) {
        glBindTexture(GL_TEXTURE_2D, levelsTex);
        drawQuad((WIDTH - 200) / 2, 500, 200, 50, 0);
    }
    if (exitTex) {
        glBindTexture(GL_TEXTURE_2D, exitTex);
        drawQuad((WIDTH - 200) / 2, 600, 200, 50, 0);
    }
}

void renderLevelsMenu() {
    glColor3f(1, 1, 1);
    if (menuBackgroundTex) {
        glBindTexture(GL_TEXTURE_2D, menuBackgroundTex);
        drawQuad(0, 0, WIDTH, HEIGHT, 0);
    }

    glDisable(GL_TEXTURE_2D);
    glRasterPos2i((WIDTH - 225) / 2, 260);
    for (char* c = "Enter the name of the level:"; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_TEXTURE_2D);

    // Поле ввода названия уровня
    if (inputFieldTex) {
        glBindTexture(GL_TEXTURE_2D, inputFieldTex);
        drawQuad((WIDTH - 400) / 2, 275, 400, 100, 0);
    }

    // Отображение введённого текста
    glDisable(GL_TEXTURE_2D);
    glRasterPos2i((WIDTH - 355) / 2, 335);
    for (char* c = levelName; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_TEXTURE_2D);

    // Кнопки
    float buttonY = 400;
    float buttonSpacing = 100;

    if (createButtonTex) {
        glBindTexture(GL_TEXTURE_2D, createButtonTex);
        drawQuad((WIDTH - 200) / 2, buttonY, 200, 50, 0); // Editor
    }

    if (loadTex) {
        glBindTexture(GL_TEXTURE_2D, loadTex);
        drawQuad((WIDTH - 200) / 2, buttonY + buttonSpacing, 200, 50, 0); // Load
    }

    if (cancelButtonTex) {
        glBindTexture(GL_TEXTURE_2D, cancelButtonTex);
        drawQuad((WIDTH - 200) / 2, buttonY + buttonSpacing * 2, 200, 50, 0); // cancel
    }
}

void renderMenuConfirm() {
    // Сохраняем текущие настройки blending
    GLboolean blendEnabled;
    glGetBooleanv(GL_BLEND, &blendEnabled);
    GLint blendSrc, blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    // Включаем blending с правильными параметрами для полупрозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Полупрозрачный темный фон
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(WIDTH, 0);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(0, HEIGHT);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1); // Восстанавливаем белый цвет для текстур

    // Окно подтверждения (центрированное)
    int confirmWidth = 500;
    int confirmHeight = 300;
    if (dialogWindowTex) {
        glBindTexture(GL_TEXTURE_2D, dialogWindowTex);
        drawQuad((WIDTH - confirmWidth) / 2, (HEIGHT - confirmHeight) / 2, confirmWidth, confirmHeight, 0);
    }

    // Текст вопроса
    if (menuPromptTex) {
        glBindTexture(GL_TEXTURE_2D, menuPromptTex);
        drawQuad((WIDTH - 400) / 2, (HEIGHT - confirmHeight) / 2 + 50, 400, 100, 0);
    }

    // Кнопка Да
    if (yesButtonTex) {
        glBindTexture(GL_TEXTURE_2D, yesButtonTex);
        drawQuad((WIDTH - confirmWidth) / 2 + 100, (HEIGHT - confirmHeight) / 2 + 180, 120, 50, 0);
    }

    // Кнопка Нет
    if (noButtonTex) {
        glBindTexture(GL_TEXTURE_2D, noButtonTex);
        drawQuad((WIDTH - confirmWidth) / 2 + 280, (HEIGHT - confirmHeight) / 2 + 180, 120, 50, 0);
    }

    // Восстанавливаем предыдущие настройки blending
    if (!blendEnabled) {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blendSrc, blendDst);
}

void renderGameOver(int score) {  // Добавляем параметр score для отображения
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);

    if (gameOverBackgroundTex) {
        glBindTexture(GL_TEXTURE_2D, gameOverBackgroundTex);
        drawQuad(0, 0, WIDTH, HEIGHT, 0);
    }

    // Отрисовка Game Over (центрируем)
    if (gameOverTex) {
        glBindTexture(GL_TEXTURE_2D, gameOverTex);
        drawQuad((WIDTH - 1500) / 2, 300, 640, 360, 0);  // Увеличиваем размер текстуры
    }

    // Кнопка Restart (центрируем)
    if (restartTex) {
        glBindTexture(GL_TEXTURE_2D, restartTex);
        drawQuad((WIDTH - 200) / 2, 400, 200, 50, 0);  // Поднимаем выше
    }

    // Кнопка Menu (центрируем)
    if (menuTex) {
        glBindTexture(GL_TEXTURE_2D, menuTex);
        drawQuad((WIDTH - 200) / 2, 500, 200, 50, 0);  // Поднимаем выше
    }

    // Отображение счета (добавляем)
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Final Score: %d", score);

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i((WIDTH - 200) / 2, 350);  // Позиция под надписью Game Over

    for (char* c = scoreText; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void updateCustomObjectsForNewLevel() {
    for (int row = 0; row < CELLS_Y; row++) {
        RowSettings* rs = &editor.rowSettings[row];
        if (rs->objectType == OBJ_EMPTY) continue;

        // Обновление скоростей
        if (rs->speedLevels1 > 0 && currentLevel % rs->speedLevels1 == 0) {
            rs->initialSpeed1 += rs->speedIncrease1;
            rs->initialSpeed1 = fminf(rs->initialSpeed1, 500.0f); // Максимальная скорость 500
        }
        if (rs->speedLevels2 > 0 && currentLevel % rs->speedLevels2 == 0) {
            rs->initialSpeed2 += rs->speedIncrease2;
            rs->initialSpeed2 = fminf(rs->initialSpeed2, 500.0f);
        }

        // Обновление количества объектов
        if (rs->countLevels > 0 && currentLevel % rs->countLevels == 0) {
            int maxQuantity = getMaxQuantityForObject(rs->objectType);
            rs->quantity = clamp(rs->quantity + rs->countIncrease, 0, maxQuantity);
        }
    }
}

void deleteAllTextures() {
    glDeleteTextures(1, &backgroundTex);
    glDeleteTextures(1, &titleTex);
    glDeleteTextures(1, &playTex);
    glDeleteTextures(1, &exitTex);
    glDeleteTextures(1, &gameOverTex);
    glDeleteTextures(1, &restartTex);
    glDeleteTextures(1, &menuTex);
    glDeleteTextures(1, &waterTex);
    glDeleteTextures(1, &car1Tex);
    glDeleteTextures(1, &car2Tex);
    glDeleteTextures(1, &car3Tex);
    glDeleteTextures(1, &car4Tex);
    glDeleteTextures(1, &truckTex);
    glDeleteTextures(1, &logLTex);
    glDeleteTextures(1, &logMTex);
    glDeleteTextures(1, &logRTex);
    glDeleteTextures(1, &heartTex);
    glDeleteTextures(1, &turtle1Tex);
    glDeleteTextures(1, &turtle2Tex);
    glDeleteTextures(1, &turtle3Tex);
    glDeleteTextures(1, &turtle4Tex);
    glDeleteTextures(1, &roadTex);
    glDeleteTextures(1, &groundTex);
    glDeleteTextures(1, &homeEmptyTex);
    glDeleteTextures(1, &homeFill1Tex);
    glDeleteTextures(1, &homeFill2Tex);
    glDeleteTextures(1, &homeFill3Tex);
    glDeleteTextures(1, &homeFullTex);
    glDeleteTextures(1, &infoPanelTex);
    glDeleteTextures(1, &gameOverBackgroundTex);
    glDeleteTextures(1, &menuBackgroundTex);
    glDeleteTextures(1, &lawnmowerTex);
    glDeleteTextures(1, &lawnTex);
    glDeleteTextures(1, &snake1Tex);
    glDeleteTextures(1, &snake2Tex);
    glDeleteTextures(1, &dialogWindowTex);
    glDeleteTextures(1, &yesButtonTex);
    glDeleteTextures(1, &noButtonTex);
    glDeleteTextures(1, &menuPromptTex);
    glDeleteTextures(1, &menuButtonTex);
    glDeleteTextures(1, &editorTex);
    glDeleteTextures(1, &createButtonTex);
    glDeleteTextures(1, &cancelButtonTex);
    glDeleteTextures(1, &alligatorHeadClosedTex);
    glDeleteTextures(1, &alligatorHeadOpenTex);
    glDeleteTextures(1, &alligatorBodyTex);
    glDeleteTextures(1, &alligatorTailTex);
    glDeleteTextures(1, &settingsButtonTex);
    glDeleteTextures(1, &optionButtonTex);
    glDeleteTextures(1, &deleteButtonTex);
    glDeleteTextures(1, &grassButtonTex);
    glDeleteTextures(1, &roadButtonTex);
    glDeleteTextures(1, &lawnButtonTex);
    glDeleteTextures(1, &waterButtonTex);
    glDeleteTextures(1, &homeButtonTex);
    glDeleteTextures(1, &spawnButtonTex);
    glDeleteTextures(1, &invalidPatternTex);
    glDeleteTextures(1, &buttonDirectionLeftTex);
    glDeleteTextures(1, &buttonDirectionRightTex);
    glDeleteTextures(1, &editorOptionsCar1Tex);
    glDeleteTextures(1, &editorOptionsCar2Tex);
    glDeleteTextures(1, &editorOptionsCar3Tex);
    glDeleteTextures(1, &editorOptionsCar4Tex);
    glDeleteTextures(1, &editorOptionsLawnmowerTex);
    glDeleteTextures(1, &editorOptionsLogTex);
    glDeleteTextures(1, &editorOptionsSnakeTex);
    glDeleteTextures(1, &editorOptionsMenuTex);
    glDeleteTextures(1, &editorOptionsTruckTex);
    glDeleteTextures(1, &editorOptionsTurtleTex);
    glDeleteTextures(1, &emptyTex);
    glDeleteTextures(1, &editorOptionsNotAllowedEmpty);
    glDeleteTextures(1, &editorOptionsNotAllowedOneLawnmower);
    glDeleteTextures(1, &editorOptionsNotAllowedTwoLawnmowers);
    glDeleteTextures(1, &editorOptionsNotAllowedRoad);
    glDeleteTextures(1, &editorOptionsNotAllowedWater);
    glDeleteTextures(1, &editorOptionsNotAllowedSnake);
    glDeleteTextures(1, &editorOptionsNotAllowedZeroQuantity);
    glDeleteTextures(1, &exitButtonTex);
    glDeleteTextures(1, &resumeButtonTex);
    glDeleteTextures(1, &saveAndExitButtonTex);
    glDeleteTextures(1, &saveAndPlayButtonTex);
    glDeleteTextures(1, &saveButtonTex);
    glDeleteTextures(1, &levelsTex);
    glDeleteTextures(1, &inputFieldTex);
    glDeleteTextures(1, &loadTex);
    glDeleteTextures(1, &deleteTex);
    glDeleteTextures(1, &frogIdleTex);
    glDeleteTextures(1, &frogJumpTex);

    // Удаляем текстуры анимаций
    for (int i = 0; i < 4; i++) {
        glDeleteTextures(1, &frogDrownTex[i]);
        glDeleteTextures(1, &frogCrashTex[i]);
    }

    glDeleteTextures(1, &editorSettings);
    glDeleteTextures(1, &notValidEditorTex);
}

void customSpawn(float* playerX, float* playerY, float* targetX, float* targetY, int* isAnimating) {
    *playerX = spawnX;
    *playerY = spawnY;
    *targetX = *playerX;
    *targetY = *playerY;
    *isAnimating = 0;
    frogState = FROG_IDLE;
    frogDirection = DIR_UP;
    initCustomScoreSystem();
}

typedef struct {
    int isDeadly;
    int objectIndex;
    ObjectType objectType;
} ObjectInfoField;

ObjectInfoField objectInfoField[CELLS_Y + 1][CELLS_X + 1] = { 0 }; // хранение данных об объектах

void customRestartGame(float* playerX, float* playerY, float* targetX, float* targetY, int* isAnimating,
    int* lives, int* score, int* currentLevel, int* completedHomes) {

    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        customHomes[i].state = 0;
        customHomes[i].animTimer = 0.0f;
        customHomes[i].isFill = 0;
    }

    *currentLevel = 1;
    *completedHomes = 0;
    *score = 0;
    *lives = editor.initialLives;
    remainingTime = editor.gameTime;
    customSpawn(playerX, playerY, targetX, targetY, isAnimating);

    initCustomObjects();

    Mix_HaltMusic();
    Mix_PlayMusic(sounds.gameMusic, -1);

    memset(objectInfoField, 0, sizeof objectInfoField);
}

// Сброс параметров редактора
void exitEditor() {
    editor.zoomFactor = 1.0f;
    editor.panX = 0;
    editor.panY = 0;
    editor.baseCellSize = GAME_WIDTH / 20.0f;
    editor.isOptionsMenuOpen = 0;
    editor.selectedZoneRow = 0;
    editor.optionsMenuPosX = 0;
    editor.optionsMenuPosY = 0;
    editor.initialLives = 0;
    editor.alligatorChance = 0;
    editor.gameTime = 0;
    editorMode = EDITOR_MODE_NONE;
    selectedZone = ZONE_NONE;

    // Очищаем настройки рядов
    if (editor.rowSettings) {
        for (int i = 0; i < CELLS_Y; i++) {
            memset(&editor.rowSettings[i], 0, sizeof(RowSettings));
            editor.rowSettings[i].objectType = OBJ_EMPTY;
        }
    }

    // Очищаем зоны
    if (editor.zoneTypes) {
        for (int i = 0; i < CELLS_Y; i++) {
            editor.zoneTypes[i] = ZONE_NONE;
        }
    }

    // Сбрасываем позиции домиков и точки спавна
    customHomeCount = 0;
    memset(customHomes, 0, sizeof(customHomes));
    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        customHomes[i].x = customHomes[i].y = -999;
    }
    spawnX = -999;
    spawnY = -999;

    // Сбрасываем параметры редактора
    clearEditorOptions();
}

void saveLevel(const char* filename) {
    // Создаём JSON-объект уровня
    cJSON* levelJson = cJSON_CreateObject();

    // 1. Сохраняем правила уровня
    cJSON* rulesJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(rulesJson, "lives", editor.initialLives);
    cJSON_AddNumberToObject(rulesJson, "gameTime", editor.gameTime);
    cJSON_AddNumberToObject(rulesJson, "alligatorChance", editor.alligatorChance);
    cJSON_AddItemToObject(levelJson, "rules", rulesJson);

    // 2. Сохраняем точку спавна
    cJSON* spawnJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(spawnJson, "x", spawnX); // сохраняем в координатах
    cJSON_AddNumberToObject(spawnJson, "y", spawnY);
    cJSON_AddItemToObject(levelJson, "spawn", spawnJson);

    // 3. Сохраняем домики
    cJSON* homesJson = cJSON_CreateArray();
    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        if (customHomes[i].isExist) {
            cJSON* homeJson = cJSON_CreateObject();
            cJSON_AddNumberToObject(homeJson, "x", customHomes[i].x);
            cJSON_AddNumberToObject(homeJson, "y", customHomes[i].y);
            cJSON_AddItemToArray(homesJson, homeJson);
        }
    }
    cJSON_AddItemToObject(levelJson, "homes", homesJson);

    // 4. Сохраняем зоны
    cJSON* zonesJson = cJSON_CreateArray();
    for (int i = 0; i < CELLS_Y; i++) {
        const char* zoneName = "NONE";
        switch (editor.zoneTypes[i]) {
        case ZONE_GRASS: zoneName = "GRASS"; break;
        case ZONE_ROAD:  zoneName = "ROAD";  break;
        case ZONE_LAWN:  zoneName = "LAWN";  break;
        case ZONE_WATER: zoneName = "WATER"; break;
        }
        cJSON_AddItemToArray(zonesJson, cJSON_CreateString(zoneName));
    }
    cJSON_AddItemToObject(levelJson, "zones", zonesJson);

    // 5. Сохраняем настройки рядов
    cJSON* rowsJson = cJSON_CreateArray();
    for (int i = 0; i < CELLS_Y; i++) {
        RowSettings* row = &editor.rowSettings[i];
        if (row->objectType == OBJ_EMPTY) continue;

        cJSON* rowJson = cJSON_CreateObject();
        cJSON_AddNumberToObject(rowJson, "row", i); // индекс ряда
        const char* typeName = "EMPTY";
        switch (row->objectType) {
        case OBJ_CAR1:      typeName = "CAR1"; break;
        case OBJ_CAR2:      typeName = "CAR2"; break;
        case OBJ_CAR3:      typeName = "CAR3"; break;
        case OBJ_CAR4:      typeName = "CAR4"; break;
        case OBJ_TRUCK:     typeName = "TRUCK"; break;
        case OBJ_LOG:       typeName = "LOG"; break;
        case OBJ_TURTLE:    typeName = "TURTLE"; break;
        case OBJ_SNAKE:     typeName = "SNAKE"; break;
        case OBJ_LAWNMOWER: typeName = "LAWNMOWER"; break;
        }
        cJSON_AddStringToObject(rowJson, "type", typeName);
        cJSON_AddNumberToObject(rowJson, "quantity", row->quantity);
        cJSON_AddNumberToObject(rowJson, "direction", row->direction);
        cJSON_AddNumberToObject(rowJson, "length", row->length);
        cJSON_AddNumberToObject(rowJson, "initialSpeed1", row->initialSpeed1);
        cJSON_AddNumberToObject(rowJson, "initialSpeed2", row->initialSpeed2);
        cJSON_AddNumberToObject(rowJson, "speedLevels1", row->speedLevels1);
        cJSON_AddNumberToObject(rowJson, "speedLevels2", row->speedLevels1);
        cJSON_AddNumberToObject(rowJson, "speedIncrease1", row->speedIncrease1);
        cJSON_AddNumberToObject(rowJson, "speedIncrease2", row->speedIncrease2);
        cJSON_AddNumberToObject(rowJson, "countLevels", row->countLevels);
        cJSON_AddNumberToObject(rowJson, "countIncrease", row->countIncrease);
        cJSON_AddItemToArray(rowsJson, rowJson);
    }
    cJSON_AddItemToObject(levelJson, "rows", rowsJson);

    // Записываем JSON в файл
    char* jsonStr = cJSON_Print(levelJson);
    FILE* file = fopen(filename, "w");
    if (file) {
        fputs(jsonStr, file);
        fclose(file);
    }
    else {
        printf("[DEBUG] Failed to save level!\n");
    }

    // Очищаем память
    cJSON_Delete(levelJson);
    free(jsonStr);
}

int loadLevel(const char* filename) {
    // 1. Читаем файл
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[DEBUG] Failed to open level file: %s\n", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(fileSize + 1);
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    // 2. Парсим JSON
    cJSON* levelJson = cJSON_Parse(buffer);
    if (!levelJson) {
        printf("[DEBUG] Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return 0;
    }

    // 3. Очищаем текущий уровень
    exitEditor(); // Сбрасываем все настройки редактора

    // 4. Базовые параметры и правила
    cJSON* rulesJson = cJSON_GetObjectItem(levelJson, "rules");
    if (rulesJson) {
        cJSON* livesJson = cJSON_GetObjectItem(rulesJson, "lives");
        cJSON* gameTimeJson = cJSON_GetObjectItem(rulesJson, "gameTime");
        cJSON* alligatorChanceJson = cJSON_GetObjectItem(rulesJson, "alligatorChance");
        if (livesJson && gameTimeJson && alligatorChanceJson) {
            editor.initialLives = livesJson->valuedouble;
            editor.gameTime = gameTimeJson->valuedouble;
            editor.alligatorChance = alligatorChanceJson->valuedouble;

            editorBase.initialLives = livesJson->valuedouble;
            editorBase.gameTime = gameTimeJson->valuedouble;
            editorBase.alligatorChance = alligatorChanceJson->valuedouble;
        }
    }
    editor.baseCellSize = GAME_WIDTH / 20.0f; // Стандартный размер клетки
    editorBase.baseCellSize = GAME_WIDTH / 20.0f;
    updateEditorTransforms();

    // 5. Загружаем точку спавна
    cJSON* spawnJson = cJSON_GetObjectItem(levelJson, "spawn");
    if (spawnJson) {
        cJSON* spawnXJson = cJSON_GetObjectItem(spawnJson, "x");
        cJSON* spawnYJson = cJSON_GetObjectItem(spawnJson, "y");
        if (spawnXJson && spawnYJson) {
            spawnX = spawnXJson->valuedouble;
            spawnY = spawnYJson->valuedouble;
        }
    }

    // 6. Загружаем домики
    customHomeCount = 0;
    cJSON* homesJson = cJSON_GetObjectItem(levelJson, "homes");
    if (homesJson && cJSON_IsArray(homesJson)) {
        int homeCount = cJSON_GetArraySize(homesJson);
        for (int i = 0; i < homeCount && i < MAX_HOMES_COUNT; i++) {
            cJSON* homeJson = cJSON_GetArrayItem(homesJson, i);
            cJSON* homeXJson = cJSON_GetObjectItem(homeJson, "x");
            cJSON* homeYJson = cJSON_GetObjectItem(homeJson, "y");
            if (homeXJson && homeYJson) {
                customHomes[i].x = homeXJson->valuedouble;
                customHomes[i].y = homeYJson->valuedouble;
                customHomes[i].isExist = 1;
                customHomeCount++;
            }
        }
    }

    // 7. Загружаем зоны
    cJSON* zonesJson = cJSON_GetObjectItem(levelJson, "zones");
    if (zonesJson && cJSON_IsArray(zonesJson)) {
        editor.zoneTypes = malloc(CELLS_Y * sizeof(int));
        editorBase.zoneTypes = malloc(CELLS_Y * sizeof(int));
        for (int i = 0; i < CELLS_Y && i < cJSON_GetArraySize(zonesJson); i++) {
            cJSON* zoneJson = cJSON_GetArrayItem(zonesJson, i);
            const char* zoneName = zoneJson->valuestring;
            if (strcmp(zoneName, "GRASS") == 0) {
                editor.zoneTypes[i] = ZONE_GRASS;
                editorBase.zoneTypes[i] = ZONE_GRASS;
            }
            else if (strcmp(zoneName, "ROAD") == 0) {
                editor.zoneTypes[i] = ZONE_ROAD;
                editorBase.zoneTypes[i] = ZONE_ROAD;
            }
            else if (strcmp(zoneName, "LAWN") == 0) {
                editor.zoneTypes[i] = ZONE_LAWN;
                editorBase.zoneTypes[i] = ZONE_LAWN;
            }
            else if (strcmp(zoneName, "WATER") == 0) {
                editor.zoneTypes[i] = ZONE_WATER;
                editorBase.zoneTypes[i] = ZONE_WATER;
            }
            else {
                editor.zoneTypes[i] = ZONE_NONE;
                editorBase.zoneTypes[i] = ZONE_NONE;
            }
        }
    }

    // 8. Загружаем настройки рядов
    cJSON* rowsJson = cJSON_GetObjectItem(levelJson, "rows");
    if (rowsJson && cJSON_IsArray(rowsJson)) {
        editor.rowSettings = malloc(CELLS_Y * sizeof(RowSettings));
        editorBase.rowSettings = malloc(CELLS_Y * sizeof(RowSettings));

        memset(editor.rowSettings, 0, CELLS_Y * sizeof(RowSettings));
        memset(editorBase.rowSettings, 0, CELLS_Y * sizeof(RowSettings));

        for (int i = 0; i < CELLS_Y && i < cJSON_GetArraySize(rowsJson); i++) {
            cJSON* rowJson = cJSON_GetArrayItem(rowsJson, i);

            // Получаем индекс ряда
            cJSON* rowIndexJson = cJSON_GetObjectItem(rowJson, "row");
            if (!rowIndexJson) continue;
            int rowIndex = rowIndexJson->valueint;

            // Проверяем валидность индекса
            if (rowIndex < 0 || rowIndex >= CELLS_Y) continue;

            RowSettings* row = &editor.rowSettings[rowIndex];
            RowSettings* rowBase = &editorBase.rowSettings[rowIndex];

            // Тип объекта
            cJSON* typeJson = cJSON_GetObjectItem(rowJson, "type");
            if (typeJson) {
                const char* typeName = typeJson->valuestring;
                if (strcmp(typeName, "CAR1") == 0) {
                    row->objectType = OBJ_CAR1;
                    rowBase->objectType = OBJ_CAR1;
                }
                else if (strcmp(typeName, "CAR2") == 0) {
                    row->objectType = OBJ_CAR2;
                    rowBase->objectType = OBJ_CAR2;
                }
                else if (strcmp(typeName, "CAR3") == 0) {
                    row->objectType = OBJ_CAR3;
                    rowBase->objectType = OBJ_CAR3;
                }
                else if (strcmp(typeName, "CAR4") == 0) {
                    row->objectType = OBJ_CAR4;
                    rowBase->objectType = OBJ_CAR4;
                }
                else if (strcmp(typeName, "TRUCK") == 0) {
                    row->objectType = OBJ_TRUCK;
                    rowBase->objectType = OBJ_TRUCK;
                }
                else if (strcmp(typeName, "LOG") == 0) {
                    row->objectType = OBJ_LOG;
                    rowBase->objectType = OBJ_LOG;
                }
                else if (strcmp(typeName, "TURTLE") == 0) {
                    row->objectType = OBJ_TURTLE;
                    rowBase->objectType = OBJ_TURTLE;
                }
                else if (strcmp(typeName, "SNAKE") == 0) {
                    row->objectType = OBJ_SNAKE;
                    rowBase->objectType = OBJ_SNAKE;
                }
                else if (strcmp(typeName, "LAWNMOWER") == 0) {
                    row->objectType = OBJ_LAWNMOWER;
                    rowBase->objectType = OBJ_LAWNMOWER;
                }
                else {
                    row->objectType = OBJ_EMPTY;
                    rowBase->objectType = OBJ_EMPTY;
                }
            }

            // Параметры
            cJSON* quantityJson = cJSON_GetObjectItem(rowJson, "quantity");
            if (quantityJson) {
                row->quantity = quantityJson->valueint;
                rowBase->quantity = quantityJson->valueint;
            }

            cJSON* lengthJson = cJSON_GetObjectItem(rowJson, "length");
            if (lengthJson) {
                row->length = lengthJson->valueint;
                rowBase->length = lengthJson->valueint;
            }

            cJSON* directionJson = cJSON_GetObjectItem(rowJson, "direction");
            if (directionJson) {
                row->direction = directionJson->valueint;
                rowBase->direction = directionJson->valueint;
            }

            // Скорости
            cJSON* speed1Json = cJSON_GetObjectItem(rowJson, "initialSpeed1");
            if (speed1Json) {
                row->initialSpeed1 = speed1Json->valuedouble;
                rowBase->initialSpeed1 = speed1Json->valuedouble;
            }

            cJSON* speed2Json = cJSON_GetObjectItem(rowJson, "initialSpeed2");
            if (speed2Json) {
                row->initialSpeed2 = speed2Json->valuedouble;
                rowBase->initialSpeed2 = speed2Json->valuedouble;
            }

            // Уровни для повышения скорости
            cJSON* speedLevels1Json = cJSON_GetObjectItem(rowJson, "speedLevels1");
            if (speedLevels1Json) {
                row->speedLevels1 = speedLevels1Json->valueint;
                rowBase->speedLevels1 = speedLevels1Json->valueint;
            }

            cJSON* speedLevels2Json = cJSON_GetObjectItem(rowJson, "speedLevels2");
            if (speedLevels2Json) {
                row->speedLevels2 = speedLevels2Json->valueint;
                rowBase->speedLevels2 = speedLevels2Json->valueint;
            }

            // Повышение скорости
            cJSON* speedIncrease1Json = cJSON_GetObjectItem(rowJson, "speedIncrease1");
            if (speedIncrease1Json) {
                row->speedIncrease1 = speedIncrease1Json->valuedouble;
                rowBase->speedIncrease1 = speedIncrease1Json->valuedouble;
            }

            cJSON* speedIncrease2Json = cJSON_GetObjectItem(rowJson, "speedIncrease2");
            if (speedIncrease2Json) {
                row->speedIncrease2 = speedIncrease2Json->valuedouble;
                rowBase->speedIncrease2 = speedIncrease2Json->valuedouble;
            }

            // Уровни для повышения количества
            cJSON* countLevelsJson = cJSON_GetObjectItem(rowJson, "countLevels");
            if (countLevelsJson) {
                row->countLevels = countLevelsJson->valueint;
                rowBase->countLevels = countLevelsJson->valueint;
            }

            // Повышение количества
            cJSON* countIncreaseJson = cJSON_GetObjectItem(rowJson, "countIncrease");
            if (countIncreaseJson) {
                row->countIncrease = countIncreaseJson->valueint;
                rowBase->countIncrease = countIncreaseJson->valueint;
            }
        }
    }

    editor.panX = 0;
    editor.panY = 0;
    editor.zoomFactor = 1.0f;

    editorBase.panX = 0;
    editorBase.panY = 0;
    editorBase.zoomFactor = 1.0f;

    // Обновляем трансформации
    updateEditorTransforms();

    // Пересчитываем смещения для центрирования
    editor.viewOffsetX = (GAME_WIDTH - editor.fieldWidth) / 2;
    editor.viewOffsetY = (HEIGHT - editor.fieldHeight) / 2;

    editorBase.viewOffsetX = (GAME_WIDTH - editorBase.fieldWidth) / 2;
    editorBase.viewOffsetY = (HEIGHT - editorBase.fieldHeight) / 2;

    // 9. Очищаем ресурсы
    cJSON_Delete(levelJson);
    free(buffer);

    return 1;
}

void restoreEditorFromBase(void) {
    // rules
    editor.initialLives = editorBase.initialLives;
    editor.gameTime = editorBase.gameTime;
    editor.alligatorChance = editorBase.alligatorChance;

    // baseCellSize
    editor.baseCellSize = editorBase.baseCellSize;

    // pan и zoom
    editor.panX = editorBase.panX;
    editor.panY = editorBase.panY;
    editor.zoomFactor = editorBase.zoomFactor;

    // viewOffset
    editor.viewOffsetX = editorBase.viewOffsetX;
    editor.viewOffsetY = editorBase.viewOffsetY;

    // zoneTypes
    if (editor.zoneTypes && editorBase.zoneTypes) {
        memcpy(editor.zoneTypes, editorBase.zoneTypes, CELLS_Y * sizeof(ZoneType));
    }

    // rowSettings
    if (editor.rowSettings && editorBase.rowSettings) {
        memcpy(editor.rowSettings, editorBase.rowSettings, CELLS_Y * sizeof(RowSettings));
    }

    updateEditorTransforms();
}

int deleteLevel(const char* levelName) {
    char path[256];
    snprintf(path, sizeof(path), "levels/%s.json", levelName);

    if (!levelFileExists(levelName)) {
        printf("[DEBUG] Level file does not exist: %s\n", path);
        return 0;
    }

    if (remove(path) == 0) {
        printf("[DEBUG] Successfully deleted level: %s\n", levelName);
        return 1;
    }
    else {
        printf("[DEBUG] Failed to delete level: %s\n", path);
        return 0;
    }
}

void addObjectToOIF(CustomObject* obj,
    const int objectIndex, const ObjectType objectType, const int isDeadly) {


    int leftCellX = (obj->x >= 0 || !((int)obj->x % CELL_SIZE)) ? (int)obj->x / CELL_SIZE : (int)obj->x / CELL_SIZE - 1;
    // если X больше 0, то целая часть находится нормально, но если X отрицателен и не делится нацело на CELL_SIZE 
    // (т.е. мы не получим координату ячейки, где по факту находится X из-за дибильной делимости нацело, 
    // поэтому мы просто к целой части отрицательного числа прибавляем -1)
    // 
    // ПРИМЕР: [хороший случай] Если obj->x == 600, то cellX = 600 / CELL_SIZE (60) = 10 и width = 2 (truck / snake), тогда все условия выполнены, и все ячейки заполнятся
    // [плохой случай 1] Если obj->x == 1140, cellX = 1140 / 60 = 19 и width = 2, тогда у нас 19-ая ячейка по X заполнится, а вот 20-ая нет, потому что ограничение
    // [плохой случай 2] Если obj->x == -65, cellX = -65 / 60 = -1, НО так как не выполняется ни одно из условий:
    //                                                                             (obj->x >= 0 || !((int)obj->x % CELL_SIZE))
    //                                                   , то cellX -= 1 -> cellX == -2; width == 3. Тогда отрицательные ячейки скипаются, а заполняется только 0-вая по X

    int rightCellX = ((int)obj->x % CELL_SIZE) ? ((int)obj->width + (int)obj->x) / CELL_SIZE + 1 : ((int)obj->width + (int)obj->x) / CELL_SIZE;
    int cellY = (int)obj->y / CELL_SIZE;

    int x = leftCellX;
    for (x; x < rightCellX; x++) {
        if (x < 20 && x >= 0) {
            if (objectInfoField[cellY][x].isDeadly == -1) break;
            if (x == rightCellX - 1 || x == 19) return;
        }
    }

    int height = ((int)obj->y % CELL_SIZE) ? 2 : 1;

    x = leftCellX;
    for (x; x < rightCellX; x++) {
        if (x < 20 && x >= 0) {
            objectInfoField[cellY][x].isDeadly = isDeadly;
            objectInfoField[cellY][x].objectIndex = objectIndex;
            objectInfoField[cellY][x].objectType = objectType;
            if (height == 2) {
                objectInfoField[cellY + 1][x].isDeadly = isDeadly;
                objectInfoField[cellY + 1][x].objectIndex = objectIndex;
                objectInfoField[cellY + 1][x].objectType = objectType;
            }
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int lawnmower1Teleport = 0;
    int lawnmower2Teleport = 0;
    int snakeTeleport = 0;

    int score = 0;
    int lastRow = CELLS_Y - 1;
    int lives = 3;

    memset(customHomes, 0, sizeof(customHomes));
    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
        customHomes[i].x = customHomes[i].y = -999;
    }

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Frogger", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetScrollCallback(window, editorMouseWheel);
    glfwSetCursorPosCallback(window, editorMouseMove);
    glfwSetMouseButtonCallback(window, editorMouseButton);

    glfwSetScrollCallback(window, editorMouseWheel);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float playerX;
    float playerY;
    float targetX;
    float targetY;
    float animProgress = 0.0f;
    int isAnimating = 0;
    int keyState[GLFW_KEY_LAST + 1] = { 0 };

    Mix_FadeOutMusic(500); // Плавное затухание музыки за 500мс
    if (!initSoundSystem()) {
        printf("[DEBUG] Failed to initialize sound system! Continuing without sound...\n");
    }
    else {
        // Запускаем музыку меню
        Mix_PlayMusic(sounds.menuMusic, -1); // -1 для зацикливания
    }

    backgroundTex = loadTexture("texture/background.png");
    titleTex = loadTexture("texture/title.png");
    playTex = loadTexture("texture/button_play.png");
    exitTex = loadTexture("texture/button_quit.png");
    gameOverTex = loadTexture("texture/game_over.png");
    restartTex = loadTexture("texture/button_restart.png");
    menuTex = loadTexture("texture/button_to_menu.png");
    waterTex = loadTexture("texture/water.png");
    car1Tex = loadTexture("texture/car1.png");
    car2Tex = loadTexture("texture/car2.png");
    car3Tex = loadTexture("texture/car3.png");
    car4Tex = loadTexture("texture/car4.png");
    truckTex = loadTexture("texture/truck.png");
    logLTex = loadTexture("texture/log_l.png");
    logMTex = loadTexture("texture/log_m.png");
    logRTex = loadTexture("texture/log_r.png");
    heartTex = loadTexture("texture/heart.png");
    turtle1Tex = loadTexture("texture/turtle1.png");
    turtle2Tex = loadTexture("texture/turtle2.png");
    turtle3Tex = loadTexture("texture/turtle3.png");
    turtle4Tex = loadTexture("texture/turtle4.png");
    roadTex = loadTexture("texture/road.png");
    groundTex = loadTexture("texture/ground.png");
    homeEmptyTex = loadTexture("texture/home_empty.png");
    homeFill1Tex = loadTexture("texture/home_fill1.png");
    homeFill2Tex = loadTexture("texture/home_fill2.png");
    homeFill3Tex = loadTexture("texture/home_fill3.png");
    homeFullTex = loadTexture("texture/home_full.png");
    infoPanelTex = loadTexture("texture/info_panel.png");
    gameOverBackgroundTex = loadTexture("texture/game_over_background.png");
    menuBackgroundTex = loadTexture("texture/menu_background.png");
    lawnmowerTex = loadTexture("texture/lawnmower.png");
    lawnTex = loadTexture("texture/lawn.png");
    snake1Tex = loadTexture("texture/snake1.png");
    snake2Tex = loadTexture("texture/snake2.png");
    dialogWindowTex = loadTexture("texture/dialog_window.png");
    yesButtonTex = loadTexture("texture/button_yes.png");
    noButtonTex = loadTexture("texture/button_no.png");
    menuPromptTex = loadTexture("texture/menu_prompt.png");
    menuButtonTex = loadTexture("texture/button_pause.png");
    editorTex = loadTexture("texture/button_editor.png");
    createButtonTex = loadTexture("texture/button_create.png");
    cancelButtonTex = loadTexture("texture/button_cancel.png");
    alligatorHeadClosedTex = loadTexture("texture/alligator_head_closed.png");
    alligatorHeadOpenTex = loadTexture("texture/alligator_head_open.png");
    alligatorBodyTex = loadTexture("texture/alligator_body.png");
    alligatorTailTex = loadTexture("texture/alligator_tail.png");
    settingsButtonTex = loadTexture("texture/button_settings.png");
    optionButtonTex = loadTexture("texture/button_option.png");
    deleteButtonTex = loadTexture("texture/button_delete.png");
    grassButtonTex = loadTexture("texture/button_grass.png");
    roadButtonTex = loadTexture("texture/button_road.png");
    lawnButtonTex = loadTexture("texture/button_lawn.png");
    waterButtonTex = loadTexture("texture/button_water.png");
    homeButtonTex = loadTexture("texture/button_home.png");
    spawnButtonTex = loadTexture("texture/button_spawn.png");
    invalidPatternTex = loadTexture("texture/invalid_pattern.png");
    buttonDirectionLeftTex = loadTexture("texture/button_direction_left.png");
    buttonDirectionRightTex = loadTexture("texture/button_direction_right.png");
    editorOptionsCar1Tex = loadTexture("texture/editor_options_car1.png");
    editorOptionsCar2Tex = loadTexture("texture/editor_options_car2.png");
    editorOptionsCar3Tex = loadTexture("texture/editor_options_car3.png");
    editorOptionsCar4Tex = loadTexture("texture/editor_options_car4.png");
    editorOptionsLawnmowerTex = loadTexture("texture/editor_options_lawnmower.png");
    editorOptionsLogTex = loadTexture("texture/editor_options_log.png");
    editorOptionsSnakeTex = loadTexture("texture/editor_options_snake.png");
    editorOptionsMenuTex = loadTexture("texture/editor_options_menu.png");
    editorOptionsTruckTex = loadTexture("texture/editor_options_truck.png");
    editorOptionsTurtleTex = loadTexture("texture/editor_options_turtle.png");
    emptyTex = loadTexture("texture/empty.png");
    editorOptionsNotAllowedEmpty = loadTexture("texture/editor_options_notallowed_empty.png");
    editorOptionsNotAllowedOneLawnmower = loadTexture("texture/editor_options_notallowed_onelawnmower.png");
    editorOptionsNotAllowedTwoLawnmowers = loadTexture("texture/editor_options_notallowed_twolawnmowers.png");
    editorOptionsNotAllowedWater = loadTexture("texture/editor_options_notallowed_water.png");
    editorOptionsNotAllowedSnake = loadTexture("texture/editor_options_notallowed_snake.png");
    editorOptionsNotAllowedRoad = loadTexture("texture/editor_options_notallowed_road.png");
    editorOptionsNotAllowedZeroQuantity = loadTexture("texture/editor_options_notallowed_zeroquantity.png");
    exitButtonTex = loadTexture("texture/button_exit.png");
    resumeButtonTex = loadTexture("texture/button_resume.png");
    saveAndExitButtonTex = loadTexture("texture/button_saveandexit.png");
    saveAndPlayButtonTex = loadTexture("texture/button_saveandplay.png");
    saveButtonTex = loadTexture("texture/button_save.png");
    levelsTex = loadTexture("texture/button_levels.png");
    inputFieldTex = loadTexture("texture/input_field.png");
    loadTex = loadTexture("texture/button_load.png");
    deleteTex = loadTexture("texture/menu_button_delete.png");
    frogIdleTex = loadTexture("texture/frog_state.png");
    frogJumpTex = loadTexture("texture/frog_jump.png");
    frogDrownTex[0] = loadTexture("texture/frog_sink1.png");
    frogDrownTex[1] = loadTexture("texture/frog_sink2.png");
    frogDrownTex[2] = loadTexture("texture/frog_sink3.png");
    frogDrownTex[3] = loadTexture("texture/empty.png");
    frogCrashTex[0] = loadTexture("texture/frog_crash1.png");
    frogCrashTex[1] = loadTexture("texture/frog_crash2.png");
    frogCrashTex[2] = loadTexture("texture/frog_crash3.png");
    frogCrashTex[3] = loadTexture("texture/empty.png");
    editorSettings = loadTexture("texture/editor_settings.png");
    notValidEditorTex = loadTexture("texture/not_valid_editor.png");

    Hitbox frogHitbox = { .left = 0.1f, .right = 0.1f, .top = 0.1f, .bottom = 0.1f };
    Hitbox carHitbox = { .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f };
    Hitbox truckHitbox = { .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f };
    Hitbox logHitbox = { .left = 0.05f, .right = 0.05f, .top = 0.05f, .bottom = 0.05f };
    Hitbox turtleHitbox = { .left = 0.1f, .right = 0.1f, .top = 0.1f, .bottom = 0.1f };
    Hitbox lawnmowerHitbox = { .left = 0.2f, .right = 0.2f, .top = 0.2f, .bottom = 0.2f };
    Hitbox snakeHitbox = { .left = 0.2f, .right = 0.2f, .top = 0.25f, .bottom = 0.25f };

    objectPreviewTextures[OBJ_EMPTY] = emptyTex;
    objectPreviewTextures[OBJ_CAR1] = editorOptionsCar1Tex;
    objectPreviewTextures[OBJ_CAR2] = editorOptionsCar2Tex;
    objectPreviewTextures[OBJ_CAR3] = editorOptionsCar3Tex;
    objectPreviewTextures[OBJ_CAR4] = editorOptionsCar4Tex;
    objectPreviewTextures[OBJ_TRUCK] = editorOptionsTruckTex;
    objectPreviewTextures[OBJ_LOG] = editorOptionsLogTex;
    objectPreviewTextures[OBJ_TURTLE] = editorOptionsTurtleTex;
    objectPreviewTextures[OBJ_SNAKE] = editorOptionsSnakeTex;
    objectPreviewTextures[OBJ_LAWNMOWER] = editorOptionsLawnmowerTex;

    objectAllowedOptions[OBJ_EMPTY] = editorOptionsNotAllowedEmpty;
    objectAllowedOptions[OBJ_CAR1] = editorOptionsNotAllowedRoad;
    objectAllowedOptions[OBJ_CAR2] = editorOptionsNotAllowedRoad;
    objectAllowedOptions[OBJ_CAR3] = editorOptionsNotAllowedRoad;
    objectAllowedOptions[OBJ_CAR4] = editorOptionsNotAllowedRoad;
    objectAllowedOptions[OBJ_TRUCK] = editorOptionsNotAllowedRoad;
    objectAllowedOptions[OBJ_LOG] = editorOptionsNotAllowedWater;
    objectAllowedOptions[OBJ_TURTLE] = editorOptionsNotAllowedWater;
    objectAllowedOptions[OBJ_SNAKE] = editorOptionsNotAllowedSnake;
    objectAllowedOptions[OBJ_LAWNMOWER] = editorOptionsNotAllowedOneLawnmower;

    int mousePressed = 0;
    double pressX, pressY, releaseX, releaseY;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    float* sin_arr = malloc(1001 * sizeof(sin_arr));
    for (int el = 0; el <= 100; el++) {
        sin_arr[el] = sin(el / 50.0f * PI_2);
    }

    // флаг для play.json
    short main_level = 0;

    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClear(GL_COLOR_BUFFER_BIT);

        if (editor.isOptionsMenuOpen) {
            processEditorTextInput(window);
        }

        if (gameState == MAIN_MENU) {
            renderMenu();
        }
        else if (gameState == CUSTOM_PLAYING) {
            renderCustomGame(playerX, playerY, lives, score);

            // Обновление времени
            remainingTime -= deltaTime;
            if (remainingTime <= 0 && remainingTime > -1) {
                // Обработка окончания времени
                remainingTime = 0;
                frogState = FROG_CRASHING;
            }

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = CUSTOM_PLAYING_TO_MENU_CONFIRM;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }

            // Обработка включения/выключения noclip
            int nKeyState = glfwGetKey(window, DEBUG_NOCLIP_KEY);
            if (nKeyState == GLFW_PRESS && !noclipKeyPressed) {
                noclipEnabled = !noclipEnabled;
                noclipKeyPressed = 1;
                printf("[DEBUG] Noclip %s\n", noclipEnabled ? "ENABLED" : "DISABLED");
            }
            if (nKeyState == GLFW_RELEASE) {
                noclipKeyPressed = 0;
            }

            // Обработка включения/выключения хитбоксов
            int hKeyState = glfwGetKey(window, DEBUG_HITBOX_KEY);
            if (hKeyState == GLFW_PRESS && !hitboxKeyPressed) {
                showHitboxes = !showHitboxes;
                hitboxKeyPressed = 1;
                printf("[DEBUG] Hitboxes %s\n", showHitboxes ? "SHOWN" : "HIDDEN");
            }
            if (hKeyState == GLFW_RELEASE) {
                hitboxKeyPressed = 0;
            }

            // Обработка увеличения уровня на L
            int lKeyState = glfwGetKey(window, DEBUG_LEVELUP_KEY);
            if (lKeyState == GLFW_PRESS && !levelupKeyPressed) {
                currentLevel++;
                updateCustomObjectsForNewLevel();
                initCustomObjects();
                customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                levelupKeyPressed = 1;
                remainingTime = editor.gameTime;
            }
            if (lKeyState == GLFW_RELEASE) {
                levelupKeyPressed = 0;
            }

            if (frogState == FROG_CRASHING) {
                if (!isCrashing) {
                    Mix_PlayChannel(-1, sounds.explode, 0);
                    isCrashing = 1;
                }

                crashProgress += deltaTime;

                // Смена кадров каждые 0.5 секунд
                if (crashProgress >= CRASH_DURATION / 3 * (crashFrame + 1)) {
                    crashFrame = (crashFrame + 1) % 3;

                    // После завершения анимации
                    if (crashProgress >= CRASH_DURATION) {
                        remainingTime = editor.gameTime;
                        lives--;
                        if (lives <= 0) {
                            Mix_PlayChannel(-1, (Mix_Chunk*)sounds.gameOver, 0);
                            Mix_FadeOutMusic(500);
                            gameState = CUSTOM_GAME_OVER;
                        }
                        customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                        isCrashing = 0;
                        crashProgress = 0.0f;
                        crashFrame = 0;
                    }
                }
            }

            int isFillingHome = 0;
            if (customHomes[0].state > 0 && customHomes[0].state < 4) {
                isFillingHome = 1;
            }
            else if (customHomes[1].state > 0 && customHomes[1].state < 4) {
                isFillingHome = 1;
            }
            else if (customHomes[2].state > 0 && customHomes[2].state < 4) {
                isFillingHome = 1;
            }
            else if (customHomes[3].state > 0 && customHomes[3].state < 4) {
                isFillingHome = 1;
            }
            else if (customHomes[4].state > 0 && customHomes[4].state < 4) {
                isFillingHome = 1;
            }

            int keys[] = { GLFW_KEY_LEFT, GLFW_KEY_A, GLFW_KEY_RIGHT, GLFW_KEY_D,
                          GLFW_KEY_UP, GLFW_KEY_W, GLFW_KEY_DOWN, GLFW_KEY_S };
            for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
                int state = glfwGetKey(window, keys[i]);
                if (state == GLFW_PRESS && !keyState[keys[i]] && !isAnimating &&
                    frogState != FROG_DROWNING && frogState != FROG_CRASHING && !isFillingHome) {
                    int dx = 0, dy = 0;
                    switch (keys[i]) {
                    case GLFW_KEY_LEFT:
                    case GLFW_KEY_A: {
                        frogDirection = DIR_LEFT;
                        dx = -1;
                        break;
                    }
                    case GLFW_KEY_RIGHT:
                    case GLFW_KEY_D: {
                        frogDirection = DIR_RIGHT;
                        dx = 1;
                        break;
                    }
                    case GLFW_KEY_UP:
                    case GLFW_KEY_W: {
                        frogDirection = DIR_UP;
                        dy = -1;
                        break;
                    }
                    case GLFW_KEY_DOWN:
                    case GLFW_KEY_S: {
                        frogDirection = DIR_DOWN;
                        dy = 1;
                        break;
                    }
                    }

                    float newX = targetX + dx * CELL_SIZE;
                    float newY = targetY + dy * CELL_SIZE;

                    int playerCellY = (int)playerY / CELL_SIZE;
                    int playerCellX = (int)playerX / CELL_SIZE;

                    // проверка, может ли лягушка прыгнуть на бревне в сторону
                    ObjectInfoField oIF = objectInfoField[playerCellY][playerCellX];
                    switch (oIF.objectType) {

                    case OBJ_LOG: {
                        CustomObject* log = &customLogs[oIF.objectIndex];
                        if (playerY == log->y) {
                            float frogWidth = 56.0f; // ширина лягушки
                            float logLeft = log->x;
                            float logRight = log->x + log->width;

                            // проверка, находится ли лягушка на бревне (с учётом её ширины)
                            if (playerX >= logLeft && playerX + frogWidth <= logRight) {
                                float futureX = newX;

                                // блокировка слева: futureX не может быть меньше левой границы бревна
                                if (futureX < logLeft + 16 && playerX != logLeft) {
                                    newX = logLeft;
                                }
                                // блокировка справа: futureX + ширина лягушки не может превышать правую границу
                                else if (futureX + frogWidth > logRight - 16 && playerX + frogWidth != logRight) {
                                    newX = logRight - frogWidth;
                                }
                            }
                        }
                        break;
                    }
                    case OBJ_TURTLE: {
                        CustomTurtle* turtle = &customTurtles[oIF.objectIndex];
                        if (playerY == turtle->base.y) {
                            float frogWidth = 56.0f; // ширина лягушки
                            float logLeft = turtle->base.x;
                            float logRight = turtle->base.x + turtle->base.width;

                            // проверка, находится ли лягушка на черепахах (с учётом её ширины)
                            if (playerX >= logLeft && playerX + frogWidth <= logRight) {
                                float futureX = newX;

                                // блокировка слева: futureX не может быть меньше левой границы черепах
                                if (futureX < logLeft + 16 && playerX != logLeft) {
                                    newX = logLeft;
                                }
                                // блокировка справа: futureX + ширина лягушки не может превышать правую границу
                                else if (futureX + frogWidth > logRight - 16 && playerX + frogWidth != logRight) {
                                    newX = logRight - frogWidth;
                                }
                            }
                        }
                        break;
                    }
                    case OBJ_ALLIGATOR: {
                        CustomAlligator* alligator = &customAlligators[oIF.objectIndex];
                        if (playerY == alligator->base.y) {
                            float frogWidth = 56.0f; // ширина лягушки
                            float alligatorLeft = alligator->base.x;
                            float alligatorRight = alligator->base.x + alligator->base.width;

                            // проверка, находится ли лягушка на бревне (с учётом её ширины)
                            if (playerX >= alligatorLeft && playerX + frogWidth <= alligatorRight) {
                                float futureX = newX;

                                // блокировка слева: futureX не может быть меньше левой границы бревна
                                if (futureX < alligatorLeft + 16 && playerX != alligatorLeft) {
                                    newX = alligatorLeft;
                                }
                                // блокировка справа: futureX + ширина лягушки не может превышать правую границу
                                else if (futureX + frogWidth > alligatorRight - 16 && playerX + frogWidth != alligatorRight) {
                                    newX = alligatorRight - frogWidth;
                                }
                            }
                        }
                        break;
                    }
                    }

                    // проверяем, будет ли прыжок успешным (не в стену)
                    if (newX >= 0 && newX <= RIGHT_WALL &&
                        newY >= 0 && newY <= BOTTOM_WALL) {

                        Mix_PlayChannel(-1, sounds.jump, 0);

                        targetX = newX;
                        targetY = newY;
                        isAnimating = 1;
                        animProgress = 0.0f;
                    }
                    else if (newX < 0) {
                        if (targetX > 0) {
                            Mix_PlayChannel(-1, sounds.jump, 0);
                            targetX = 0;
                            isAnimating = 1;
                            animProgress = 0.0f;
                        }
                    }
                    else if (newX > RIGHT_WALL) {
                        // частичный прыжок к стене (вправо)
                        if (targetX < RIGHT_WALL) {
                            Mix_PlayChannel(-1, sounds.jump, 0);
                            targetX = RIGHT_WALL;
                            isAnimating = 1;
                            animProgress = 0.0f;
                        }
                    }
                    // для вертикальных границ аналогично
                    else if (newY < 0) {
                        if (targetY > 0) {
                            Mix_PlayChannel(-1, sounds.jump, 0);
                            targetY = 0;
                            isAnimating = 1;
                            animProgress = 0.0f;
                        }
                    }
                    else if (newY > BOTTOM_WALL) {
                        if (targetY < BOTTOM_WALL) {
                            Mix_PlayChannel(-1, sounds.jump, 0);
                            targetY = BOTTOM_WALL;
                            isAnimating = 1;
                            animProgress = 0.0f;
                        }
                    }
                }
                keyState[keys[i]] = state;
            }

            if (frogState != FROG_DROWNING && frogState != FROG_CRASHING && !isFillingHome) {
                if (!isAnimating) {
                    frogState = FROG_IDLE;
                    int currentRow = (int)(playerY) / CELL_SIZE;
                    currentRow = clamp(currentRow, 0, CELLS_Y - 1);

                    // если ряд новый и выше предыдущего
                    if (!visitedRows[currentRow]) {
                        score += SCORE_FORWARD;
                        visitedRows[currentRow] = 1;
                        lastVisitedRow = currentRow;
                    }
                }

                if (isAnimating) {
                    frogState = FROG_JUMPING;
                    float step = MOVE_SPEED * deltaTime;

                    float dx = targetX - playerX;
                    float dy = targetY - playerY;
                    float distance = sqrtf(dx * dx + dy * dy);

                    if (distance <= step) {
                        playerX = targetX;
                        playerY = targetY;
                        isAnimating = 0;
                    }
                    else {
                        playerX += dx / distance * step;
                        playerY += dy / distance * step;
                    }
                }
            }


            // "обнуление" ближних к игроку клеток
            int playerRow = (int)playerY / CELL_SIZE;
            int playerColumn = (int)playerX / CELL_SIZE;

            objectInfoField[playerRow][playerColumn].isDeadly =
                objectInfoField[playerRow][playerColumn + 1].isDeadly =
                objectInfoField[playerRow + 1][playerColumn].isDeadly = -1;

            // Обновление позиции змеи
            for (int i = 0; i < customSnakeCount; i++) {
                CustomSnake* snake = &customSnakes[i];
                if (snake->base.isActive) {
                    float animSpeed = 0.5f / snake->base.speed * 100;

                    snake->animTimer += deltaTime;
                    if (snake->animTimer >= animSpeed) {
                        snake->animTimer = 0.0f;
                        snake->currentFrame = (snake->currentFrame + 1) % 2;
                    }
                    snake->base.x += snake->base.speed * deltaTime * snake->base.direction;

                    if ((snake->base.direction == 1 && snake->base.x > GAME_WIDTH - TWO_CELLS_SIZE) ||
                        (snake->base.direction == -1 && snake->base.x < 0)) {

                        if (rand() % 2 == 0 && !snake->teleport) { // 50% шанс развернуться
                            snake->base.direction *= -1;
                            // Корректируем позицию точно к границе
                            snake->base.x = (snake->base.direction == 1) ?
                                0 :
                                GAME_WIDTH - TWO_CELLS_SIZE;
                        }
                        else {
                            snake->teleport = 1; // "Пропуск" к телепорту, чтобы не работало прошлое условие, пока она ползёт к GAME_WIDTH + GAME_WIDTH / CELLS
                            if (snake->base.x > GAME_WIDTH || snake->base.x < -TWO_CELLS_SIZE) {
                                // Телепортация на другую сторону
                                snake->base.x = (snake->base.direction == 1) ?
                                    -TWO_CELLS_SIZE - CELL_SIZE * (rand() % 6) :
                                    GAME_WIDTH + CELL_SIZE * (rand() % 6);
                                snake->teleport = 0;
                            }
                        }
                    }

                    addObjectToOIF(&snake->base, i, OBJ_SNAKE, 1);

                }
            }

            // Обновление позиций машин
            for (int i = 0; i < customCarCount; i++) {
                CustomObject* car = &customCars[i];

                car->x += car->speed * deltaTime * car->direction;

                // Перемещение машины на начало ряда при выходе за границы
                if (car->direction == 1 && car->x > car->rightBoundary + CELL_SIZE) {
                    car->x = -((car->texture == truckTex) ? TWO_CELLS_SIZE : CELL_SIZE);
                }
                else if (car->direction == -1 && car->x < -((car->texture == truckTex) ? TWO_CELLS_SIZE : CELL_SIZE)) {
                    car->x = car->rightBoundary + CELL_SIZE;
                }

                addObjectToOIF(car, i, editor.rowSettings[car->row].objectType, 1);
            }

            // Обновление позиций бревен
            for (int i = 0; i < customLogCount; i++) {
                CustomObject* log = &customLogs[i];
                if (log->width) {
                    log->x += log->speed * deltaTime * log->direction;

                    // Перезапуск при выходе за границы
                    if (log->direction == 1 && log->x > log->rightBoundary + CELL_SIZE) {
                        log->x = -log->width;
                    }
                    else if (log->direction == -1 && log->x < -log->width) {
                        log->x = log->rightBoundary + CELL_SIZE;
                    }
                    addObjectToOIF(log, i, OBJ_LOG, 2);
                }
            }

            // Обновление позиций аллигаторов
            for (int i = 0; i < customAlligatorCount; i++) {
                CustomAlligator* alligator = &customAlligators[i];
                // Движение
                alligator->base.x += alligator->base.speed * deltaTime * alligator->base.direction;

                // Границы
                if (alligator->base.direction == 1 && alligator->base.x > alligator->base.rightBoundary + CELL_SIZE) {
                    alligator->base.x = -alligator->base.width;
                }
                else if (alligator->base.direction == -1 && alligator->base.x < -alligator->base.width) {
                    alligator->base.x = alligator->base.rightBoundary + CELL_SIZE;
                }

                // Анимация пасти
                alligator->mouthTimer += deltaTime;

                if (alligator->mouthTimer > ALLIGATOR_ANIM_SPEED) {
                    alligator->mouthState = !alligator->mouthState;
                    alligator->mouthTimer = 0.0f;
                }

                // Плавное изменение состояния
                if (alligator->mouthState == 1) {
                    alligator->mouthProgress = fminf(1.0f, alligator->mouthProgress + deltaTime * 2.0f);
                }
                else {
                    alligator->mouthProgress = fmaxf(0.0f, alligator->mouthProgress - deltaTime * 2.0f);
                }

                addObjectToOIF(&alligator->base, i, OBJ_ALLIGATOR, 2);

            }

            // Обновление позиций черепах
            for (int i = 0; i < customTurtleCount; i++) {
                CustomTurtle* turtle = &customTurtles[i];
                turtle->base.x += turtle->base.speed * deltaTime * turtle->base.direction;

                // Только для ныряющих черепах
                if (turtle->isDiving) {
                    turtle->phaseTimer += deltaTime;

                    float speedFactor = 1.0f + (turtle->base.speed / 100.0f);
                    float surfacePause = 3.0f / speedFactor;
                    float diveDuration = 2.0f / speedFactor;
                    float underwaterPause = 0.75f / speedFactor;
                    float riseDuration = 2.0f / speedFactor;

                    switch (turtle->phase) {
                    case 0: // Пауза на поверхности
                        turtle->state = 0;
                        if (turtle->phaseTimer > surfacePause) {
                            turtle->phase = 1;
                            turtle->phaseTimer = 0.0f;
                        }
                        break;

                    case 1: // Погружение
                        if (turtle->phaseTimer > diveDuration / 3) {
                            turtle->state = 1;
                        }
                        if (turtle->phaseTimer > diveDuration * 2 / 3) {
                            turtle->state = 2;
                        }
                        if (turtle->phaseTimer > diveDuration) {
                            turtle->state = 3;
                            turtle->phase = 2;
                            turtle->phaseTimer = 0.0f;
                        }
                        break;

                    case 2: // Пауза под водой
                        turtle->state = 3;
                        if (turtle->phaseTimer > underwaterPause) {
                            turtle->phase = 3;
                            turtle->phaseTimer = 0.0f;
                        }
                        break;

                    case 3: // Всплытие
                        if (turtle->phaseTimer > riseDuration / 3) {
                            turtle->state = 2;
                        }
                        if (turtle->phaseTimer > riseDuration * 2 / 3) {
                            turtle->state = 1;
                        }
                        if (turtle->phaseTimer > riseDuration) {
                            turtle->state = 0;
                            turtle->phase = 0;
                            turtle->phaseTimer = 0.0f;
                        }
                        break;
                    }
                }
                else {
                    // Для не-ныряющих черепах - всегда состояние 0
                    turtle->state = 0;
                }

                // Перезапуск при выходе за границы
                if (turtle->base.direction == 1 && turtle->base.x > turtle->base.rightBoundary + CELL_SIZE) {
                    turtle->base.x = -turtle->base.width;
                    if (rand() % 2 == 0) turtle->isDiving = !turtle->isDiving;
                }
                else if (turtle->base.direction == -1 && turtle->base.x < -turtle->base.width) {
                    turtle->base.x = turtle->base.rightBoundary + CELL_SIZE;
                    if (rand() % 2 == 0) turtle->isDiving = !turtle->isDiving;
                }

                addObjectToOIF(&turtle->base, i, OBJ_TURTLE, 2);
            }

            // Обновление позиций газонокосилки (/-ок)
            for (int i = 0; i < customLawnmowerCount; i++) {
                CustomLawnmower* lawnmower = &customLawnmowers[i];
                CustomLawnmower* lawnmower2 = NULL;
                RowSettings* rs = &editor.rowSettings[lawnmower->base.row];
                if (rs->quantity == 2) {
                    lawnmower2 = &customLawnmowers[i + 3];
                }

                if (lawnmower->isExist && i % 2 == 0) {
                    int secondRow = lawnmower->base.row + 1;
                    int secondRow2 = lawnmower->base.row;

                    // первая
                    lawnmower->base.x += lawnmower->base.speed * deltaTime * lawnmower->base.direction;
                    lawnmower->changeRowTimer += deltaTime;

                    if ((lawnmower->base.direction == 1 && lawnmower->base.x > RIGHT_WALL) ||
                        (lawnmower->base.direction == -1 && lawnmower->base.x < 0)) {

                        if (rand() % 2 == 0 && !lawnmower->teleport || (rs->quantity == 1) ? 0 : !lawnmower2->teleport) { // 50% шанс развернуться
                            lawnmower->base.direction *= -1;

                            lawnmower->base.x = (lawnmower->base.direction == 1) ?
                                0 :
                                RIGHT_WALL;
                        }
                        else {
                            lawnmower->teleport = 1; // "Пропуск" к телепорту, чтобы не работало прошлое условие, пока она едет к GAME_WIDTH + GAME_WIDTH / CELLS
                            if (lawnmower->base.x > GAME_WIDTH || lawnmower->base.x < -CELL_SIZE) {

                                lawnmower->base.x = (lawnmower->base.direction == 1) ?
                                    -CELL_SIZE :
                                    GAME_WIDTH;
                                lawnmower->teleport = 0;
                            }
                        }
                    }


                    // Смена ряда только если не у границы
                    if (lawnmower->changeRowTimer > 400.0f / lawnmower->base.speed + (rand() % 5) &&
                        lawnmower->base.x > CELL_SIZE * 2.5f &&
                        lawnmower->base.x < GAME_WIDTH - CELL_SIZE * 3.5f &&
                        ((lawnmower2 == NULL) ? 1 : (fabs(lawnmower->base.x - lawnmower2->base.x) > CELL_SIZE * 2.5f))) {
                        lawnmower->isChangingRow = 1;
                        lawnmower->changeRowProgress = 0.0f;
                        lawnmower->targetY = (lawnmower->currentRow == lawnmower->base.row) ?
                            secondRow * CELL_SIZE :
                            lawnmower->base.row * CELL_SIZE;
                        lawnmower->changeRowTimer = 0.0f;

                        // Определяем направление поворота текстуры
                        if ((lawnmower->currentRow == lawnmower->base.row && lawnmower->base.direction == 1) ||
                            (lawnmower->currentRow == secondRow && lawnmower->base.direction == -1)) {
                            if (lawnmower->base.row < secondRow) {
                                lawnmower->rotationDirection = 1;
                            }
                            else {
                                lawnmower->rotationDirection = -1;
                            }
                        }
                        else {
                            if (lawnmower->base.row < secondRow) {
                                lawnmower->rotationDirection = -1;
                            }
                            else {
                                lawnmower->rotationDirection = 1;
                            }
                        }
                    }

                    // Обработка смены ряда
                    if (lawnmower->isChangingRow) {

                        float rowChangeSpeed = lawnmower->rowChangeSpeedFactor * (lawnmower->base.speed / 30.0f);

                        lawnmower->changeRowProgress += deltaTime * rowChangeSpeed;

                        lawnmower->base.y = lawnmower->base.startY +
                            (lawnmower->targetY - lawnmower->base.startY) * sin_arr[(int)(lawnmower->changeRowProgress * 50)];

                        lawnmower->rotationAngle = 120.0f / lawnmower->base.speed * 20.0f * sin_arr[(int)(lawnmower->changeRowProgress * 100)] * lawnmower->rotationDirection;

                        if (lawnmower->changeRowProgress >= 1.0f) {
                            lawnmower->isChangingRow = 0;
                            lawnmower->currentRow = (lawnmower->currentRow == lawnmower->base.row) ? secondRow : lawnmower->base.row;
                            lawnmower->base.startY = lawnmower->base.y = lawnmower->targetY;
                            lawnmower->rotationAngle = 0.0f;
                        }
                    }

                    addObjectToOIF(&lawnmower->base, i, OBJ_LAWNMOWER, 1);

                    // если существует вторая - обрабатываем
                    if (rs->quantity == 2) {

                        if (lawnmower2->base.isActive) {
                            lawnmower2->base.x += lawnmower2->base.speed * deltaTime * lawnmower2->base.direction;
                            lawnmower2->changeRowTimer += deltaTime;

                            // Проверка столкновения между газонокосилками
                            if (checkCollision(lawnmower->base.x, lawnmower->base.y, CELL_SIZE, CELL_SIZE, lawnmower->base.hitbox,
                                lawnmower2->base.x, lawnmower2->base.y, CELL_SIZE, CELL_SIZE, lawnmower2->base.hitbox)) {

                                // Определяем, кто сзади (по направлению движения)
                                if (lawnmower->base.direction != lawnmower2->base.direction) {
                                    lawnmower->base.direction *= -1;
                                    lawnmower2->base.direction *= -1;
                                }
                                else {
                                    // Разворачиваем ту, которая сзади
                                    if ((lawnmower->base.direction == 1 && lawnmower->base.x < lawnmower2->base.x) ||
                                        (lawnmower->base.direction == -1 && lawnmower->base.x > lawnmower2->base.x)) {
                                        lawnmower->base.direction *= -1;
                                    }
                                    else {
                                        lawnmower2->base.direction *= -1;
                                    }
                                }

                                // Корректируем позиции, чтобы они не застревали
                                lawnmower->base.x += lawnmower->base.speed * deltaTime * lawnmower->base.direction;
                                lawnmower2->base.x += lawnmower2->base.speed * deltaTime * lawnmower2->base.direction;

                                if (lawnmower->base.direction == 1 && lawnmower->base.x > GAME_WIDTH - CELL_SIZE * 3 ||
                                    lawnmower->base.direction == -1 && lawnmower->base.x < TWO_CELLS_SIZE) {
                                    lawnmower->teleport = 1;
                                }
                                if (lawnmower2->base.direction == 1 && lawnmower2->base.x > GAME_WIDTH - CELL_SIZE * 3 ||
                                    lawnmower2->base.direction == -1 && lawnmower2->base.x < TWO_CELLS_SIZE) {
                                    lawnmower2->teleport = 1;
                                }
                            }

                            // Обновление 2ой газонокосилки
                            if ((lawnmower2->base.direction == 1 && lawnmower2->base.x > RIGHT_WALL) ||
                                (lawnmower2->base.direction == -1 && lawnmower2->base.x < 0)) {

                                if (rand() % 2 == 0 && !lawnmower2->teleport || !lawnmower->teleport) { // 50% шанс развернуться
                                    lawnmower2->base.direction *= -1;
                                    lawnmower2->base.x = (lawnmower2->base.direction == 1) ?
                                        0 :
                                        RIGHT_WALL;
                                }
                                else {
                                    lawnmower2->teleport = 1; // "Пропуск" к телепорту, чтобы не работало прошлое условие, пока она едет к GAME_WIDTH + GAME_WIDTH / CELLS
                                    if (lawnmower2->base.x > GAME_WIDTH || lawnmower2->base.x < -CELL_SIZE) {

                                        lawnmower2->base.x = (lawnmower2->base.direction == 1) ?
                                            -CELL_SIZE :
                                            GAME_WIDTH;
                                        lawnmower2->teleport = 0;
                                    }
                                }
                            }

                            if (lawnmower2->changeRowTimer > 200.0f / lawnmower2->base.speed + (rand() % 9) &&
                                lawnmower2->base.x > CELL_SIZE * 2.5f &&
                                lawnmower2->base.x < GAME_WIDTH - CELL_SIZE * 3.5f && fabs(lawnmower->base.x - lawnmower2->base.x) > CELL_SIZE * 2.5f) {
                                lawnmower2->isChangingRow = 1;
                                lawnmower2->changeRowProgress = 0.0f;
                                lawnmower2->targetY = (lawnmower2->currentRow == lawnmower2->base.row) ?
                                    secondRow2 * CELL_SIZE :
                                    lawnmower2->base.row * CELL_SIZE;
                                lawnmower2->changeRowTimer = 0.0f;

                                if ((lawnmower2->currentRow == lawnmower2->base.row && lawnmower2->base.direction == 1) ||
                                    (lawnmower2->currentRow == secondRow2 && lawnmower2->base.direction == -1)) {
                                    if (lawnmower2->base.row < secondRow2) {
                                        lawnmower2->rotationDirection = 1;
                                    }
                                    else {
                                        lawnmower2->rotationDirection = -1;
                                    }
                                }
                                else {
                                    if (lawnmower2->base.row < secondRow2) {
                                        lawnmower2->rotationDirection = -1;
                                    }
                                    else {
                                        lawnmower2->rotationDirection = 1;
                                    }
                                }
                            }

                            if (lawnmower2->isChangingRow) {

                                float rowChangeSpeed = lawnmower2->rowChangeSpeedFactor * (lawnmower2->base.speed / 30.0f);

                                lawnmower2->changeRowProgress += deltaTime * rowChangeSpeed;

                                lawnmower2->base.y = lawnmower2->base.startY +
                                    (lawnmower2->targetY - lawnmower2->base.startY) * sin_arr[(int)(lawnmower2->changeRowProgress * 50)];

                                lawnmower2->rotationAngle = 120.0f / lawnmower2->base.speed * 20.0f * sin_arr[(int)(lawnmower2->changeRowProgress * 100)] * lawnmower2->rotationDirection;

                                if (lawnmower2->changeRowProgress >= 1.0f) {
                                    lawnmower2->isChangingRow = 0;
                                    lawnmower2->currentRow = (lawnmower2->currentRow == lawnmower2->base.row) ? secondRow2 : lawnmower2->base.row;
                                    lawnmower2->base.startY = lawnmower2->base.y = lawnmower2->targetY;
                                    lawnmower2->rotationAngle = 0.0f;
                                }
                            }

                            addObjectToOIF(&lawnmower2->base, i + 3, OBJ_LAWNMOWER, 1);
                        }
                    }
                }
            }


            // Проверка нахождения на бревне, черепахах и аллигаторе
            int onPlatform = 0;
            if (frogState != FROG_DROWNING && frogState != FROG_CRASHING) {

                int playerCellY = (int)playerY / CELL_SIZE;
                int playerCellX = (int)playerX / CELL_SIZE;

                ObjectInfoField oIF1 = objectInfoField[playerCellY][playerCellX];
                int objectIndex = oIF1.objectIndex;

                ObjectInfoField oIF2 = objectInfoField[playerCellY][playerCellX + 1];
                int objectIndex2 = oIF2.objectIndex;

                switch (oIF1.objectType) {

                case OBJ_LOG: {
                    CustomObject* log = &customLogs[objectIndex];
                    if ((int)playerY == (int)log->y) {
                        if (checkPlayerCollision(log->x, log->y, log->width, CELL_SIZE, log->hitbox, playerX, playerY)) {
                            if ((playerX <= 2 && log->direction == -1) ||
                                (playerX >= RIGHT_WALL - 2 && log->direction == 1)) {
                                onPlatform = 1;
                            }
                            else {
                                playerX += log->speed * deltaTime * log->direction;
                                targetX += log->speed * deltaTime * log->direction;
                                onPlatform = 1;
                            }
                        }
                    }
                    break;
                }
                case OBJ_TURTLE: {
                    CustomTurtle* turtle = &customTurtles[objectIndex];
                    if ((int)playerY == (int)turtle->base.y &&
                        ((turtle->isDiving && turtle->state != 3) || !turtle->isDiving)) {

                        if (checkPlayerCollision(turtle->base.x, turtle->base.y, turtle->base.width, CELL_SIZE, turtle->base.hitbox, playerX, playerY)) {
                            if ((playerX <= 2 && turtle->base.direction == -1) ||
                                (playerX >= RIGHT_WALL - 2 && turtle->base.direction == 1)) {
                                onPlatform = 1;
                            }
                            else {
                                playerX += turtle->base.speed * deltaTime * turtle->base.direction;
                                targetX += turtle->base.speed * deltaTime * turtle->base.direction;
                                onPlatform = 1;
                            }

                        }
                    }
                    break;
                }
                case OBJ_ALLIGATOR: {
                    CustomAlligator* alligator = &customAlligators[objectIndex];

                    if ((int)playerY == (int)alligator->base.y) {

                        // Проверка основного хитбокса аллигатора (как бревно)
                        if (checkPlayerCollision(alligator->base.x, alligator->base.y, alligator->base.width, CELL_SIZE, alligator->base.hitbox, playerX, playerY)) {

                            // Если лягушка в опасной зоне и пасть открыта
                            if (checkPlayerCollision(alligator->base.x, alligator->base.y, alligator->base.width, CELL_SIZE, alligator->headHitbox, playerX, playerY) &&
                                alligator->mouthProgress > 0.7f && !noclipEnabled && !isAnimating) {
                                frogState = FROG_CRASHING;
                            }
                            else {
                                // Если не умерли, обрабатываем как платформу
                                if ((playerX <= 2 && alligator->base.direction == -1) ||
                                    (playerX >= RIGHT_WALL - 2 && alligator->base.direction == 1)) {
                                    onPlatform = 1;
                                }
                                else {
                                    playerX += alligator->base.speed * deltaTime * alligator->base.direction;
                                    targetX += alligator->base.speed * deltaTime * alligator->base.direction;
                                    onPlatform = 1;
                                }
                            }
                        }
                    }
                    break;
                }
                }
                if (!onPlatform) {
                    switch (oIF2.objectType) {

                    case OBJ_LOG: {
                        CustomObject* log = &customLogs[objectIndex2];
                        if ((int)playerY == (int)log->y) {
                            if (checkPlayerCollision(log->x, log->y, log->width, CELL_SIZE, log->hitbox, playerX, playerY)) {
                                if ((playerX <= 2 && log->direction == -1) ||
                                    (playerX >= RIGHT_WALL - 2 && log->direction == 1)) {
                                    onPlatform = 1;
                                }
                                else {
                                    playerX += log->speed * deltaTime * log->direction;
                                    targetX += log->speed * deltaTime * log->direction;
                                    onPlatform = 1;
                                }
                            }
                        }
                        break;
                    }
                    case OBJ_TURTLE: {
                        CustomTurtle* turtle = &customTurtles[objectIndex2];
                        if ((int)playerY == (int)turtle->base.y &&
                            ((turtle->isDiving && turtle->state != 3) || !turtle->isDiving)) {

                            if (checkPlayerCollision(turtle->base.x, turtle->base.y, turtle->base.width, CELL_SIZE, turtle->base.hitbox, playerX, playerY)) {
                                if ((playerX <= 2 && turtle->base.direction == -1) ||
                                    (playerX >= RIGHT_WALL - 2 && turtle->base.direction == 1)) {
                                    onPlatform = 1;
                                }
                                else {
                                    playerX += turtle->base.speed * deltaTime * turtle->base.direction;
                                    targetX += turtle->base.speed * deltaTime * turtle->base.direction;
                                    onPlatform = 1;
                                }

                            }
                        }
                        break;
                    }
                    case OBJ_ALLIGATOR: {
                        CustomAlligator* alligator = &customAlligators[objectIndex2];

                        if ((int)playerY == (int)alligator->base.y) {

                            // Проверка основного хитбокса аллигатора (как бревно)
                            if (checkPlayerCollision(alligator->base.x, alligator->base.y, alligator->base.width, CELL_SIZE, alligator->base.hitbox, playerX, playerY)) {

                                // Если лягушка в опасной зоне и пасть открыта
                                if (checkPlayerCollision(alligator->base.x, alligator->base.y, alligator->base.width, CELL_SIZE, alligator->headHitbox, playerX, playerY) &&
                                    alligator->mouthProgress > 0.7f && !noclipEnabled && !isAnimating) {
                                    frogState = FROG_CRASHING;
                                }
                                else {
                                    // Если не умерли, обрабатываем как платформу
                                    if ((playerX <= 2 && alligator->base.direction == -1) ||
                                        (playerX >= RIGHT_WALL - 2 && alligator->base.direction == 1)) {
                                        onPlatform = 1;
                                    }
                                    else {
                                        playerX += alligator->base.speed * deltaTime * alligator->base.direction;
                                        targetX += alligator->base.speed * deltaTime * alligator->base.direction;
                                        onPlatform = 1;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    }
                }
            }

            short index = 5;

            if (customHomes[0].state > 0 && customHomes[0].state < 4) {
                customHomes[0].animTimer += deltaTime;
                index = 0;
            }
            else if (customHomes[1].state > 0 && customHomes[1].state < 4) {
                customHomes[1].animTimer += deltaTime;
                index = 1;
            }
            else if (customHomes[2].state > 0 && customHomes[2].state < 4) {
                customHomes[2].animTimer += deltaTime;
                index = 2;
            }
            else if (customHomes[3].state > 0 && customHomes[3].state < 4) {
                customHomes[3].animTimer += deltaTime;
                index = 3;
            }
            else if (customHomes[4].state > 0 && customHomes[4].state < 4) {
                customHomes[4].animTimer += deltaTime;
                index = 4;
            }
            if (index != 5) {
                if (customHomes[index].animTimer > 0.15f) { // Скорость анимации
                    customHomes[index].state++;
                    customHomes[index].animTimer = 0.0f;

                    // Если анимация только что завершилась
                    if (customHomes[index].state == 4) {
                        Mix_PlayChannel(-1, sounds.levelup, 0);
                        score += SCORE_HOME;
                        completedHomes++;

                        // Проверяем, все ли домики заняты
                        if (completedHomes >= customHomeCount) {
                            currentLevel++;
                            completedHomes = 0;

                            customHomes[0].state =
                                customHomes[1].state =
                                customHomes[2].state =
                                customHomes[3].state =
                                customHomes[4].state = 0;

                            customHomes[0].animTimer =
                                customHomes[1].animTimer =
                                customHomes[2].animTimer =
                                customHomes[3].animTimer =
                                customHomes[4].animTimer = 0.0f;

                            customHomes[0].isFill =
                                customHomes[1].isFill =
                                customHomes[2].isFill =
                                customHomes[3].isFill =
                                customHomes[4].isFill = 0;


                            updateCustomObjectsForNewLevel();
                            initCustomObjects();
                            customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                            remainingTime = editor.gameTime;
                        }
                    }
                }
            }

            int currentRow = (int)playerY / CELL_SIZE;

            // проверка попадания в воду
            if (!isAnimating && !onPlatform && editor.zoneTypes[currentRow] == ZONE_WATER &&
                !noclipEnabled && frogState != FROG_CRASHING) {

                int cantDrow = 0;

                for (int i = 0; i < MAX_HOMES_COUNT; i++) {
                    if (customHomes[i].state > 0 && customHomes[i].state < 4) {
                        cantDrow = 1;
                    }
                    else if (checkPlayerCollision(customHomes[i].x, customHomes[i].y, CELL_SIZE, CELL_SIZE, customHomes[i].hitbox, playerX, playerY)) {
                        cantDrow = 1;
                        if (customHomes[i].state == 0) {
                            customHomes[i].state = 1;
                        }
                        else if (!customHomes[i].isFill) {
                            customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                            remainingTime = editor.gameTime;
                            customHomes[i].isFill = 1;
                            break;
                        }
                    }
                }

                if (!cantDrow) {
                    frogState = FROG_DROWNING;
                    drownProgress += deltaTime;

                    // Смена кадров каждые 0.5 секунд
                    if (drownProgress >= DROWN_DURATION / 3 * (drownFrame + 1)) {
                        drownFrame = (drownFrame + 1) % 3;
                        // После завершения анимации
                        if (drownProgress >= DROWN_DURATION) {
                            lives--;
                            if (lives <= 0) {
                                Mix_PlayChannel(-1, (Mix_Chunk*)sounds.gameOver, 0);
                                Mix_FadeOutMusic(500);
                                gameState = CUSTOM_GAME_OVER;
                            }
                            else {
                                customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                                remainingTime = editor.gameTime;
                            }
                            drownProgress = 0.0f;
                            drownFrame = 0;
                        }
                    }
                }
            }

            /* рендеринг для отладки */
            if (0) {
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(0.0f, 1.0f, 0.0f, 0.8f);

                for (int y = 0; y < 15; y++) {
                    for (int x = 0; x < 20; x++) {
                        float posX = x * CELL_SIZE + 2;
                        float posY = y * CELL_SIZE + CELL_SIZE - 2;

                        Hitbox hitbox = { 0 };
                        float width = 0;
                        float x1 = 0;
                        float y1 = 0;
                        switch (objectInfoField[y][x].objectType) {
                        case OBJ_CAR1:
                        case OBJ_CAR2:
                        case OBJ_CAR3:
                        case OBJ_CAR4:
                        case OBJ_TRUCK: {
                            x1 = customCars[objectInfoField[y][x].objectIndex].x;
                            y1 = customCars[objectInfoField[y][x].objectIndex].y;
                            hitbox = customCars[objectInfoField[y][x].objectIndex].hitbox;
                            width = customCars[objectInfoField[y][x].objectIndex].width;
                            break;
                        }
                        case OBJ_LAWNMOWER: {
                            x1 = customLawnmowers[objectInfoField[y][x].objectIndex].base.x;
                            y1 = customLawnmowers[objectInfoField[y][x].objectIndex].base.y;
                            hitbox = customLawnmowers[objectInfoField[y][x].objectIndex].base.hitbox;
                            width = customLawnmowers[objectInfoField[y][x].objectIndex].base.width;
                            break;
                        }
                        case OBJ_SNAKE: {
                            x1 = customSnakes[objectInfoField[y][x].objectIndex].base.x;
                            y1 = customSnakes[objectInfoField[y][x].objectIndex].base.y;
                            hitbox = customSnakes[objectInfoField[y][x].objectIndex].base.hitbox;
                            width = customSnakes[objectInfoField[y][x].objectIndex].base.width;
                            break;
                        }
                        case OBJ_LOG: {
                            x1 = customLogs[objectInfoField[y][x].objectIndex].x;
                            y1 = customLogs[objectInfoField[y][x].objectIndex].y;
                            hitbox = customLogs[objectInfoField[y][x].objectIndex].hitbox;
                            width = customLogs[objectInfoField[y][x].objectIndex].width;
                            break;
                        }
                        case OBJ_TURTLE: {
                            x1 = customTurtles[objectInfoField[y][x].objectIndex].base.x;
                            y1 = customTurtles[objectInfoField[y][x].objectIndex].base.y;
                            hitbox = customTurtles[objectInfoField[y][x].objectIndex].base.hitbox;
                            width = customTurtles[objectInfoField[y][x].objectIndex].base.width;
                            break;
                        }
                        case OBJ_ALLIGATOR: {
                            x1 = customAlligators[objectInfoField[y][x].objectIndex].base.x;
                            y1 = customAlligators[objectInfoField[y][x].objectIndex].base.y;
                            hitbox = customAlligators[objectInfoField[y][x].objectIndex].base.hitbox;
                            width = customAlligators[objectInfoField[y][x].objectIndex].base.width;
                            break;
                        }
                        }

                        if (objectInfoField[y][x].isDeadly) {
                            glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
                            char widthStr[8];
                            if (0) {
                                if (width >= 100) {
                                    snprintf(widthStr, sizeof(widthStr), "%.0f", width);
                                }
                                else if (width >= 10) {
                                    snprintf(widthStr, sizeof(widthStr), "%.1f", width);
                                }
                                else {
                                    snprintf(widthStr, sizeof(widthStr), "%.2f", width);
                                }
                            }
                            if (1) {
                                snprintf(widthStr, sizeof(widthStr), "%d", objectInfoField[y][x].objectIndex);
                            }
                            if (0) {
                                snprintf(widthStr, sizeof(widthStr), "%f", x1);
                            }

                            glRasterPos2f(posX, posY);
                            for (char* c = widthStr; *c; c++) {
                                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
                            }
                        }
                        else {
                            glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
                            glRasterPos2f(posX, posY);
                            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '-');
                        }

                    }
                }

                glDisable(GL_BLEND);
                glEnable(GL_TEXTURE_2D);
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            /* рендеринг для отладки */

            // проверка на столкновение
            int playerCellY = (int)playerY / CELL_SIZE;
            int playerCellX = (int)playerX / CELL_SIZE;
            if (!noclipEnabled) {

                Hitbox hitbox = { 0 };
                float x = 0;
                float y = 0;
                float width = 0;
                ObjectInfoField oIF = objectInfoField[playerCellY][playerCellX];

                // в основной клетке игрока (всегда левая)
                if (oIF.isDeadly == 1) {

                    int index = oIF.objectIndex;

                    switch (oIF.objectType) {
                    case OBJ_CAR1:
                    case OBJ_CAR2:
                    case OBJ_CAR3:
                    case OBJ_CAR4:
                    case OBJ_TRUCK: {
                        y = customCars[index].y;
                        x = customCars[index].x;
                        hitbox = customCars[index].hitbox;
                        width = customCars[index].width;
                        break;
                    }
                    case OBJ_LAWNMOWER: {
                        y = customLawnmowers[index].base.y;
                        x = customLawnmowers[index].base.x;
                        hitbox = customLawnmowers[index].base.hitbox;
                        width = customLawnmowers[index].base.width;
                        break;
                    }
                    case OBJ_SNAKE: {
                        y = customSnakes[index].base.y;
                        x = customSnakes[index].base.x;
                        hitbox = customSnakes[index].base.hitbox;
                        width = customSnakes[index].base.width;
                        break;
                    }
                    }

                    if (checkPlayerCollision(x, y, width, CELL_SIZE,
                        hitbox, playerX, playerY)) {
                        frogState = FROG_CRASHING;
                    }
                }

                // в смежной клетке по X (всегда справа), при пермещении по X
                if (((int)playerX % CELL_SIZE)) {

                    oIF = objectInfoField[playerCellY][playerCellX + 1];

                    if (oIF.isDeadly == 1) {

                        int index = oIF.objectIndex;

                        switch (oIF.objectType) {
                        case OBJ_CAR1:
                        case OBJ_CAR2:
                        case OBJ_CAR3:
                        case OBJ_CAR4:
                        case OBJ_TRUCK: {
                            y = customCars[index].y;
                            x = customCars[index].x;
                            hitbox = customCars[index].hitbox;
                            width = customCars[index].width;
                            break;
                        }
                        case OBJ_LAWNMOWER: {
                            y = customLawnmowers[index].base.y;
                            x = customLawnmowers[index].base.x;
                            hitbox = customLawnmowers[index].base.hitbox;
                            width = customLawnmowers[index].base.width;
                            break;
                        }
                        case OBJ_SNAKE: {
                            y = customSnakes[index].base.y;
                            x = customSnakes[index].base.x;
                            hitbox = customSnakes[index].base.hitbox;
                            width = customSnakes[index].base.width;
                            break;
                        }
                        }

                        if (checkPlayerCollision(x, y, width, CELL_SIZE,
                            hitbox, playerX, playerY)) {
                            frogState = FROG_CRASHING;
                        }
                    }
                }

                // в смежной клетке по Y (всегда снизу по полю, но выше по координате), при перемещении по Y
                if (((int)playerY % CELL_SIZE)) {

                    oIF = objectInfoField[playerCellY + 1][playerCellX];

                    if (oIF.isDeadly == 1) {

                        int index = oIF.objectIndex;

                        switch (oIF.objectType) {
                        case OBJ_CAR1:
                        case OBJ_CAR2:
                        case OBJ_CAR3:
                        case OBJ_CAR4:
                        case OBJ_TRUCK: {
                            y = customCars[index].y;
                            x = customCars[index].x;
                            hitbox = customCars[index].hitbox;
                            width = customCars[index].width;
                            break;
                        }
                        case OBJ_LAWNMOWER: {
                            y = customLawnmowers[index].base.y;
                            x = customLawnmowers[index].base.x;
                            hitbox = customLawnmowers[index].base.hitbox;
                            width = customLawnmowers[index].base.width;
                            break;
                        }
                        case OBJ_SNAKE: {
                            y = customSnakes[index].base.y;
                            x = customSnakes[index].base.x;
                            hitbox = customSnakes[index].base.hitbox;
                            width = customSnakes[index].base.width;
                            break;
                        }
                        }

                        if (checkPlayerCollision(x, y, width, CELL_SIZE,
                            hitbox, playerX, playerY)) {
                            frogState = FROG_CRASHING;
                        }
                    }
                }
            }

        }
        else if (gameState == CUSTOM_PLAYING_TO_MENU_CONFIRM) {
            renderCustomGame(playerX, playerY, lives, score);
            renderMenuConfirm();

            // Обработка ESC для возврата в игру
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                if (lives <= 0) {
                    gameState = CUSTOM_GAME_OVER;
                }
                else {
                    gameState = CUSTOM_PLAYING;
                }
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }

        }
        else if (gameState == GAME_OVER || gameState == CUSTOM_GAME_OVER) {
            renderGameOver(score);
        }
        else if (gameState == EDITOR_EXIT_DIALOG) {
            renderEditorField(); // Включает фон, сетку и UI
            renderEditorUI();    // Дополнительные элементы интерфейса
            renderEditorExitDialog();

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = EDITOR;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == EDITOR_SETTINGS_DIALOG) {
            renderEditorField(); // Включает фон, сетку и UI
            renderEditorUI();    // Дополнительные элементы интерфейса
            renderEditorSettingsDialog();

            processSettingsDialogInput(window);

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = EDITOR;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == LEVEL_CREATE_DIALOG) {
            renderLevelsMenu();
            renderLevelCreateDialog(); // Отрисовываем диалог поверх

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = LEVEL_MENU;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == LEVEL_LOAD_DIALOG) {
            renderLevelsMenu();
            renderLevelLoadDialog(); // Отрисовываем диалог поверх

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = LEVEL_MENU;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == LEVEL_LOAD_ERROR || gameState == LEVEL_CREATE_ERROR) {
            renderLevelsMenu();
            renderLevelError();

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = LEVEL_MENU;
                memset(levelName, 0, sizeof(levelName));
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == EDITOR_NOT_VALID) {
            renderEditorField(); // Включает фон, сетку и UI
            renderEditorUI();    // Дополнительные элементы интерфейса
            renderEditorExitDialog();
            renderNotValidEditor();

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = EDITOR_EXIT_DIALOG;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == LEVEL_DELETE_CONFIRM) {
            renderLevelsMenu();
            renderLevelDeleteConfirm();

            // Обработка ESC для выхода из диалога
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = LEVEL_LOAD_DIALOG;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == EDITOR) {
            glClear(GL_COLOR_BUFFER_BIT);

            // Устанавливаем проекцию на все окно
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Рендерим все компоненты
            renderEditorField(); // Включает фон, сетку и UI
            renderEditorUI();    // Дополнительные элементы интерфейса

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = EDITOR_EXIT_DIALOG;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }
        }
        else if (gameState == LEVEL_MENU) {
            renderLevelsMenu();

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escKeyPressed) {
                gameState = MAIN_MENU;
                escKeyPressed = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
                escKeyPressed = 0;
            }

            if (textInputActive) {
                static int keyStates[GLFW_KEY_LAST + 1] = { 0 };

                // Определяем состояние модификаторов
                int shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ||
                    glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
                static int capsLockActive = 0;

                // Обработка Caps Lock (только при нажатии, не зависит от состояния)
                static int capsLockState = 0;
                if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS && !capsLockState) {
                    capsLockActive = !capsLockActive;
                    capsLockState = 1;
                }
                else if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE) {
                    capsLockState = 0;
                }

                // Функция для определения текущего регистра
                int isUpperCase = (shiftPressed != capsLockActive); // XOR - если один активен

                // Обработка букв A-Z
                for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; key++) {
                    if (glfwGetKey(window, key) == GLFW_PRESS && !keyStates[key] && strlen(levelName) < 20) {
                        char letter = 'a' + (key - GLFW_KEY_A);
                        if (isUpperCase) letter = toupper(letter);
                        levelName[strlen(levelName)] = letter;
                        levelName[strlen(levelName) + 1] = '\0';
                        keyStates[key] = 1;
                    }
                    else if (glfwGetKey(window, key) == GLFW_RELEASE) {
                        keyStates[key] = 0;
                    }
                }

                // Обработка цифр 0-9 (верхний ряд клавиш)
                for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; key++) {
                    if (glfwGetKey(window, key) == GLFW_PRESS && !keyStates[key] && strlen(levelName) < 20) {
                        char number = '0' + (key - GLFW_KEY_0);
                        levelName[strlen(levelName)] = number;
                        levelName[strlen(levelName) + 1] = '\0';
                        keyStates[key] = 1;
                    }
                    else if (glfwGetKey(window, key) == GLFW_RELEASE) {
                        keyStates[key] = 0;
                    }
                }

                // Обработка цифр (numpad)
                for (int key = GLFW_KEY_KP_0; key <= GLFW_KEY_KP_9; key++) {
                    if (glfwGetKey(window, key) == GLFW_PRESS && !keyStates[key] && strlen(levelName) < 20) {
                        char number = '0' + (key - GLFW_KEY_KP_0);
                        levelName[strlen(levelName)] = number;
                        levelName[strlen(levelName) + 1] = '\0';
                        keyStates[key] = 1;
                    }
                    else if (glfwGetKey(window, key) == GLFW_RELEASE) {
                        keyStates[key] = 0;
                    }
                }

                // Обработка подчёркивания (Shift + '-')
                static int underscoreState = 0;
                if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && shiftPressed && !underscoreState && strlen(levelName) < 20) {
                    levelName[strlen(levelName)] = '_';
                    levelName[strlen(levelName) + 1] = '\0';
                    underscoreState = 1;
                }
                else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE) {
                    underscoreState = 0;
                }

                // Обработка Backspace
                static int backspaceState = 0;
                if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && !backspaceState && strlen(levelName) > 0) {
                    levelName[strlen(levelName) - 1] = '\0';
                    backspaceState = 1;
                }
                else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE) {
                    backspaceState = 0;
                }
            }
        }

        int currentState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (currentState == GLFW_PRESS && !mousePressed) {
            mousePressed = 1;
            glfwGetCursorPos(window, &pressX, &pressY);
        }
        if (currentState == GLFW_RELEASE && mousePressed) {
            mousePressed = 0;
            glfwGetCursorPos(window, &releaseX, &releaseY);

            if (fabs(pressX - releaseX) < 10 && fabs(pressY - releaseY) < 10) {
                switch (gameState) {
                case MAIN_MENU: {
                    if (releaseX >= (WIDTH - 200) / 2 && releaseX <= (WIDTH - 200) / 2 + 200) {
                        if (releaseY >= 400 && releaseY <= 450) {  // Start button
                            Mix_PlayChannel(-1, sounds.play, 0);

                            if (!loadLevel("frogger")) {
                                for (int z = 0; z < 5; z++) printf("CRITICAL ERROR: why did you delete the main level? :( Put it back!\n");
                            }
                            customRestartGame(&playerX, &playerY, &targetX, &targetY, &isAnimating,
                                &lives, &score, &currentLevel, &completedHomes);

                            gameState = CUSTOM_PLAYING;

                            main_level = 1;
                        }
                        else if (releaseY >= 500 && releaseY <= 550) {  // Levels button
                            Mix_PlayChannel(-1, sounds.click, 0);
                            gameState = LEVEL_MENU;
                            memset(levelName, 0, sizeof(levelName));
                        }
                        else if (releaseY >= 600 && releaseY <= 650) {  // Exit button
                            glfwSetWindowShouldClose(window, GL_TRUE);
                        }
                    }
                    break;
                }
                case LEVEL_CREATE_ERROR:
                case LEVEL_LOAD_ERROR: {
                    if (releaseX >= (WIDTH - 200) / 2 && releaseX <= (WIDTH + 200) / 2 &&
                        releaseY >= (HEIGHT + 200) / 2 - 70 && releaseY <= (HEIGHT + 200) / 2 - 20) {
                        gameState = LEVEL_MENU;
                        memset(levelName, 0, sizeof(levelName));
                    }
                    break;
                }
                case LEVEL_LOAD_DIALOG: {
                    int buttonWidth = 200;
                    int buttonHeight = 50;
                    int dialogY = (HEIGHT - 400) / 2 + 100; // Начало кнопок

                    // Play
                    if (releaseX >= (WIDTH - buttonWidth) / 2 && releaseX <= (WIDTH + buttonWidth) / 2 &&
                        releaseY >= dialogY && releaseY <= dialogY + buttonHeight) {
                        Mix_PlayChannel(-1, sounds.play, 0);
                        customRestartGame(&playerX, &playerY, &targetX, &targetY, &isAnimating,
                            &lives, &score, &currentLevel, &completedHomes);
                        gameState = CUSTOM_PLAYING;

                        main_level = 0;
                    }
                    // Editor
                    else if (releaseY >= dialogY + 70 && releaseY <= dialogY + 70 + buttonHeight) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        Mix_FadeOutMusic(500);
                        gameState = EDITOR; // Переход в редактор
                    }
                    // Delete
                    else if (releaseY >= dialogY + 140 && releaseY <= dialogY + 140 + buttonHeight) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        gameState = LEVEL_DELETE_CONFIRM;
                    }
                    // Cancel
                    else if (releaseY >= dialogY + 210 && releaseY <= dialogY + 210 + buttonHeight) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        gameState = LEVEL_MENU;
                    }
                    break;
                }
                case LEVEL_DELETE_CONFIRM: {
                    int buttonY = (HEIGHT - 150) / 2 + 75;

                    // Delete button
                    if (releaseX >= (WIDTH - 200) / 2 - 110 && releaseX <= (WIDTH - 200) / 2 - 110 + 200 &&
                        releaseY >= buttonY && releaseY <= buttonY + 50) {
                        if (deleteLevel(levelName)) {
                            Mix_PlayChannel(-1, sounds.click, 0);
                            gameState = LEVEL_MENU;
                            memset(levelName, 0, sizeof(levelName));
                        }
                        else {
                            Mix_PlayChannel(-1, sounds.error, 0);
                        }
                    }
                    // Cancel button
                    else if (releaseX >= (WIDTH - 200) / 2 + 110 && releaseX <= (WIDTH - 200) / 2 + 110 + 200 &&
                        releaseY >= buttonY && releaseY <= buttonY + 50) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        gameState = LEVEL_LOAD_DIALOG;
                    }
                    break;
                }
                case LEVEL_CREATE_DIALOG: {
                    int dialogX = (WIDTH - 500) / 2;
                    int dialogY = (HEIGHT - 150) / 2;

                    // Проверка клика по кнопке Create
                    if (strlen(levelName)) {
                        if (releaseX >= dialogX + 100 && releaseX <= dialogX + 220 &&
                            releaseY >= dialogY + 75 && releaseY <= dialogY + 125) {
                            initEditor(CELLS_X, CELLS_Y);
                            Mix_FadeOutMusic(1000);
                            Mix_PlayChannel(-1, sounds.click, 0);
                            gameState = EDITOR; // Переходим в режим редактора
                        }
                    }
                    // Проверка клика по кнопке Cancel
                    if (releaseX >= dialogX + 280 && releaseX <= dialogX + 400 &&
                        releaseY >= dialogY + 75 && releaseY <= dialogY + 125) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        gameState = LEVEL_MENU;
                    }

                    break;
                }
                case CUSTOM_PLAYING: {
                    // Проверяем клик по кнопке выхода (в правом верхнем углу инфо-панели)
                    if (releaseX >= WIDTH - 60 && releaseX <= WIDTH - 20 &&
                        releaseY >= 20 && releaseY <= 60) {
                        gameState = CUSTOM_PLAYING_TO_MENU_CONFIRM;
                    }
                    break;
                }
                case CUSTOM_PLAYING_TO_MENU_CONFIRM: {
                    int confirmWidth = 500;
                    int confirmHeight = 300;
                    int confirmX = (WIDTH - confirmWidth) / 2;
                    int confirmY = (HEIGHT - confirmHeight) / 2;

                    // Проверка клика по кнопке Да
                    if (releaseX >= confirmX + 100 && releaseX <= confirmX + 220 &&
                        releaseY >= confirmY + 180 && releaseY <= confirmY + 230) {
                        Mix_PlayChannel(-1, sounds.quit, 0); // Звук выхода из уровня
                        Mix_HaltMusic();
                        Mix_PlayMusic(sounds.menuMusic, -1); // Музыка меню
                        if (main_level) {
                            gameState = MAIN_MENU;
                        }
                        else gameState = LEVEL_MENU;
                    }
                    // Проверка клика по кнопке Нет
                    else if (releaseX >= confirmX + 280 && releaseX <= confirmX + 400 &&
                        releaseY >= confirmY + 180 && releaseY <= confirmY + 230) {
                        // Возвращаемся в предыдущее состояние (MENU или PLAYING)
                        if (lives <= 0) {
                            gameState = CUSTOM_GAME_OVER;
                        }
                        else {
                            gameState = CUSTOM_PLAYING;
                        }
                    }
                    break;
                }
                case CUSTOM_GAME_OVER: {
                    if (releaseX >= (WIDTH - 200) / 2 && releaseX <= (WIDTH - 200) / 2 + 200) {
                        if (releaseY >= 400 && releaseY <= 450) {  // Restart
                            Mix_PlayChannel(-1, sounds.play, 0);
                            restoreEditorFromBase();
                            customRestartGame(&playerX, &playerY, &targetX, &targetY, &isAnimating,
                                &lives, &score, &currentLevel, &completedHomes);
                            gameState = CUSTOM_PLAYING;
                        }
                        else if (releaseY >= 500 && releaseY <= 550) {  // Menu
                            Mix_PlayChannel(-1, sounds.quit, 0); // Звук выхода из уровня
                            Mix_HaltMusic();
                            Mix_PlayMusic(sounds.menuMusic, -1); // Музыка меню
                            if (main_level) {
                                gameState = MAIN_MENU;
                            }
                            else gameState = LEVEL_MENU;
                        }
                    }
                    break;
                }
                case EDITOR: {

                    // Проверяем клик по кнопкам редактора
                    if (releaseX >= GAME_WIDTH) {
                        int buttonWidth = 140;
                        int buttonHeight = 140;
                        int startY = 180;
                        int margin = 40;


                        // Проверяем, какая кнопка была нажата
                        if (releaseX >= GAME_WIDTH + margin * 2 + buttonWidth + 20 && releaseX <= GAME_WIDTH + margin * 2 + buttonWidth * 2 - 20 &&
                            releaseY >= startY - (buttonHeight + margin) + 20 && releaseY <= startY - margin - 20) {
                            closeOptionsMenu();
                            openEditorSettingsDialog();
                            gameState = EDITOR_SETTINGS_DIALOG;
                        }
                        else if (releaseY >= startY && releaseY <= startY + buttonHeight) {
                            // Первый ряд
                            if (releaseX >= GAME_WIDTH + margin && releaseX <= GAME_WIDTH + margin + buttonWidth) {
                                // option button
                                if (editorMode == EDITOR_MODE_OPTIONS) {
                                    editorMode = EDITOR_MODE_NONE;
                                    closeOptionsMenu();
                                }
                                else {
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_OPTIONS;
                                }
                            }
                            else if (releaseX >= GAME_WIDTH + margin * 2 + buttonWidth &&
                                releaseX <= GAME_WIDTH + margin * 3 + buttonWidth * 2) {
                                // delete button
                                if (editorMode == EDITOR_MODE_DELETE) {
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_DELETE;
                                }
                            }
                        }
                        else if (releaseY >= startY + buttonHeight + margin &&
                            releaseY <= startY + buttonHeight * 2 + margin) {
                            // Второй ряд
                            if (releaseX >= GAME_WIDTH + margin && releaseX <= GAME_WIDTH + margin + buttonWidth) {
                                // home
                                if (editorMode == EDITOR_MODE_HOME) {
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_HOME;
                                }
                            }
                            else if (releaseX >= GAME_WIDTH + margin * 2 + buttonWidth &&
                                releaseX <= GAME_WIDTH + margin * 3 + buttonWidth * 2) {
                                // spawn
                                if (editorMode == EDITOR_MODE_SPAWN) {
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_SPAWN;
                                }
                            }
                        }
                        else if (releaseY >= startY + buttonHeight * 2 + margin * 2 &&
                            releaseY <= startY + buttonHeight * 3 + margin * 2) {
                            if (releaseX >= GAME_WIDTH + margin && releaseX <= GAME_WIDTH + margin + buttonWidth) {
                                // Grass button - переключаем выбор
                                if (selectedZone == ZONE_GRASS) {
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_GRASS;
                                    editorMode = EDITOR_MODE_ZONE;
                                }
                            }
                            else if (releaseX >= GAME_WIDTH + margin * 2 + buttonWidth &&
                                releaseX <= GAME_WIDTH + margin * 3 + buttonWidth * 2) {
                                // Road button - переключаем выбор
                                if (selectedZone == ZONE_ROAD) {
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_ROAD;
                                    editorMode = EDITOR_MODE_ZONE;
                                }
                            }
                        }
                        else if (releaseY >= startY + buttonHeight * 3 + margin * 3 &&
                            releaseY <= startY + buttonHeight * 4 + margin * 3) {
                            if (releaseX >= GAME_WIDTH + margin && releaseX <= GAME_WIDTH + margin + buttonWidth) {
                                // Lawn button - переключаем выбор
                                if (selectedZone == ZONE_LAWN) {
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_LAWN;
                                    editorMode = EDITOR_MODE_ZONE;
                                }
                            }
                            else if (releaseX >= GAME_WIDTH + margin * 2 + buttonWidth &&
                                releaseX <= GAME_WIDTH + margin * 3 + buttonWidth * 2) {
                                // Water button - переключаем выбор
                                if (selectedZone == ZONE_WATER) {
                                    selectedZone = ZONE_NONE;
                                    editorMode = EDITOR_MODE_NONE;
                                }
                                else {
                                    closeOptionsMenu();
                                    selectedZone = ZONE_WATER;
                                    editorMode = EDITOR_MODE_ZONE;
                                }
                            }
                        }
                    }
                    else if (isCursorInEditorField) {
                        float worldX = (releaseX - editor.panX - editor.viewOffsetX) / editor.zoomFactor;
                        float worldY = (releaseY - editor.panY - editor.viewOffsetY) / editor.zoomFactor;

                        int hoveredCol = (int)(worldX / editor.baseCellSize);
                        int hoveredRow = (int)(worldY / editor.baseCellSize);

                        if (hoveredCol >= 0 && hoveredCol < CELLS_X && hoveredRow >= 0 && hoveredRow < CELLS_Y) {
                            switch (editorMode) {
                            case EDITOR_MODE_HOME:

                                if (editor.zoneTypes[hoveredRow] == ZONE_WATER &&
                                    customHomeCount < MAX_HOMES_COUNT && editor.rowSettings[hoveredRow].objectType == OBJ_EMPTY) { // Домики только в пустой воде
                                    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
                                        int isAlreadyExist = 0;
                                        for (int j = 0; j < MAX_HOMES_COUNT; j++) {
                                            if (customHomes[j].x == hoveredCol * editor.baseCellSize &&
                                                customHomes[j].y == hoveredRow * editor.baseCellSize) {
                                                isAlreadyExist = 1;
                                            }
                                        }
                                        if (customHomes[i].isExist == 0 && !isAlreadyExist) {
                                            customHomes[i].x = hoveredCol * editor.baseCellSize;
                                            customHomes[i].y = hoveredRow * editor.baseCellSize;
                                            customHomes[i].isExist = 1;
                                            customHomeCount++;
                                            Mix_PlayChannel(-1, sounds.click, 0);
                                            break;
                                        }
                                    }

                                }
                                else {
                                    Mix_PlayChannel(-1, sounds.error, 0);
                                }
                                break;

                            case EDITOR_MODE_SPAWN:
                                if (editor.zoneTypes[hoveredRow] == ZONE_GRASS &&
                                    editor.rowSettings[hoveredRow].objectType == OBJ_EMPTY) { // Спавн только на траве
                                    spawnX = hoveredCol * editor.baseCellSize;
                                    spawnY = hoveredRow * editor.baseCellSize;
                                    Mix_PlayChannel(-1, sounds.click, 0);
                                }
                                else {
                                    Mix_PlayChannel(-1, sounds.error, 0);
                                }
                                break;

                            case EDITOR_MODE_DELETE: {
                                int isDeleteObj = 0;
                                for (int i = 0; i < MAX_HOMES_COUNT; i++) {
                                    if (worldX - customHomes[i].x < editor.baseCellSize &&
                                        worldY - customHomes[i].y < editor.baseCellSize &&
                                        worldX - customHomes[i].x > 0 &&
                                        worldY - customHomes[i].y > 0 &&
                                        customHomes[i].isExist == 1) {
                                        customHomes[i].y = customHomes[i].y = -999;
                                        customHomes[i].isExist = 0;
                                        customHomeCount--;
                                        isDeleteObj = 1;
                                        Mix_PlayChannel(-1, sounds.click, 0);
                                    }
                                }

                                if (worldX - spawnX < editor.baseCellSize &&
                                    worldY - spawnY < editor.baseCellSize &&
                                    worldX - spawnX > 0 &&
                                    worldY - spawnY > 0) {
                                    spawnX = spawnY = -999;
                                    isDeleteObj = 1;
                                    Mix_PlayChannel(-1, sounds.click, 0);
                                }
                                if (!isDeleteObj && hoveredRow >= 0 && hoveredRow < CELLS_Y) {
                                    if (editor.zoneTypes[hoveredRow] != ZONE_NONE) {
                                        deleteRowSettings(hoveredRow);
                                        if (editor.zoneTypes[hoveredRow] == ZONE_LAWN) {
                                            int i = 0;
                                            while (editor.zoneTypes[hoveredRow - i] == ZONE_LAWN) i++;
                                            editor.zoneTypes[hoveredRow] = ZONE_NONE;
                                            if (i % 2 == 0) {
                                                editor.zoneTypes[hoveredRow - 1] = ZONE_NONE;
                                            }
                                            else {
                                                editor.zoneTypes[hoveredRow + 1] = ZONE_NONE;
                                            }
                                        }
                                        else {
                                            editor.zoneTypes[hoveredRow] = ZONE_NONE;
                                            for (int i = 0; i < CELLS_X; i++) {
                                                for (int j = 0; j < MAX_HOMES_COUNT; j++) {
                                                    if (!customHomes[j].isExist) {
                                                        continue;
                                                    }
                                                    if (fabs(customHomes[j].x - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                                                        fabs(customHomes[j].y - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                                                        customHomes[j].x = customHomes[j].y = -999;
                                                        customHomes[j].isExist = 0;
                                                        customHomeCount--;
                                                        break;
                                                    }
                                                }
                                                if (fabs(spawnX - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                                                    fabs(spawnY - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                                                    spawnX = spawnY = -999;
                                                }
                                            }
                                        }

                                        Mix_PlayChannel(-1, sounds.click, 0);
                                    }
                                }
                                else if (!isDeleteObj) {
                                    Mix_PlayChannel(-1, sounds.error, 0);
                                }
                                break;
                            }

                            case EDITOR_MODE_OPTIONS: {
                                if (editor.zoneTypes[hoveredRow] == ZONE_NONE) break;
                                int isExistObj = 0;
                                for (int i = 0; i < CELLS_X; i++) {
                                    for (int j = 0; j < MAX_HOMES_COUNT; j++) {
                                        if (!customHomes[j].isExist) {
                                            continue;
                                        }
                                        if (fabs(customHomes[j].x - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                                            fabs(customHomes[j].y - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                                            isExistObj = 1;
                                            break;
                                        }
                                    }
                                    if (fabs(spawnX - i * editor.baseCellSize - 1) < editor.baseCellSize / 2 &&
                                        fabs(spawnY - hoveredRow * editor.baseCellSize - 1) < editor.baseCellSize / 2) {
                                        isExistObj = 1;
                                        break;
                                    }
                                }

                                if (!isExistObj) {
                                    // Определяем позицию окна
                                    float optionsX;
                                    if (releaseX < GAME_WIDTH / 2) {
                                        optionsX = GAME_WIDTH - GAME_WIDTH / 3; // Справа от игрового поля
                                    }
                                    else {
                                        optionsX = 0; // Слева от игрового поля
                                    }

                                    // Сохраняем контекст выбранной зоны
                                    editor.selectedZoneRow = hoveredRow;

                                    // Активируем окно настроек
                                    editor.isOptionsMenuOpen = 1;
                                    editor.optionsMenuPosX = optionsX;
                                    editor.optionsMenuPosY = 0; // Верхний край
                                    handleZoneSelection(hoveredRow);
                                }
                                break;
                            }

                            case EDITOR_MODE_ZONE:
                                if (hoveredRow >= 0) {
                                    if (isZonePlacementValid(selectedZone, hoveredRow)) {
                                        // Размещаем зону
                                        int rowsToPlace = (selectedZone == ZONE_LAWN) ? 2 : 1;
                                        for (int r = 0; r < rowsToPlace; r++) {
                                            int row = hoveredRow + r;
                                            if (row >= CELLS_Y) break;
                                            editor.zoneTypes[row] = selectedZone;
                                        }
                                        Mix_PlayChannel(-1, sounds.click, 0); // Звук успешного размещения
                                    }
                                    else {
                                        Mix_PlayChannel(-1, sounds.error, 0); // Звук ошибки
                                    }
                                }
                                break;
                            }
                        }
                    }
                    break;

                }
                case LEVEL_MENU: {
                    // Проверка клика на поле ввода
                    if (releaseX >= (WIDTH - 400) / 2 && releaseX <= (WIDTH + 400) / 2 &&
                        releaseY >= 275 && releaseY <= 375) {
                        textInputActive = 1;
                    }
                    else {
                        textInputActive = 0;
                    }

                    if (releaseX >= (WIDTH - 200) / 2 && releaseX <= (WIDTH + 200) / 2) {
                        // create
                        if (releaseY >= 400 && releaseY <= 450) {
                            Mix_PlayChannel(-1, sounds.click, 0);
                            if (levelFileExists(levelName)) {
                                gameState = LEVEL_CREATE_DIALOG;
                            }
                            else if (strlen(levelName) == 0) {
                                gameState = LEVEL_CREATE_ERROR;
                            }
                            else {
                                initEditor(CELLS_X, CELLS_Y);
                                Mix_FadeOutMusic(1000);
                                gameState = EDITOR;
                            }
                        }

                        // load
                        if (releaseY >= 500 && releaseY <= 550) {
                            Mix_PlayChannel(-1, sounds.click, 0);
                            if (strlen(levelName) == 0) {
                                gameState = LEVEL_LOAD_ERROR;
                            }
                            else if (!levelFileExists(levelName)) {
                                gameState = LEVEL_LOAD_ERROR;
                            }
                            else {
                                char path[256];
                                snprintf(path, sizeof(path), "levels/%s.json", levelName);
                                if (loadLevel(path)) {
                                    gameState = LEVEL_LOAD_DIALOG;
                                }
                                else {
                                    gameState = LEVEL_LOAD_ERROR;
                                }
                            }
                        }

                        //cancel
                        if (releaseY >= 600 && releaseY <= 650) {
                            Mix_PlayChannel(-1, sounds.click, 0);
                            gameState = MAIN_MENU;
                            memset(levelName, 0, sizeof(levelName));
                        }
                    }
                    break;
                }
                case EDITOR_NOT_VALID: {
                    int dialogX = (WIDTH - 500) / 2;
                    int dialogY = (HEIGHT - 400) / 2;

                    if (releaseX >= dialogX + 150 && releaseX <= dialogX + 150 + 200 &&
                        releaseY >= dialogY + 310 && releaseY <= dialogY + 310 + 50) {
                        gameState = EDITOR_EXIT_DIALOG;
                    }
                    break;
                }
                case EDITOR_EXIT_DIALOG: {
                    int dialogX = (WIDTH - 300) / 2;
                    int dialogY = (HEIGHT - 550) / 2;

                    int isLevelValid = 0;
                    for (int i = 0; i < MAX_HOMES_COUNT; i++) {
                        if (customHomes[i].isExist) {
                            isLevelValid = 1;
                        }
                    }
                    for (int row = 0; row < CELLS_Y; row++) {
                        if (editor.zoneTypes[row] == ZONE_NONE) {
                            isLevelValid = 0;
                        }
                    }

                    if (releaseX >= dialogX + 90 && releaseX <= dialogX + 210) {
                        // resume
                        if (releaseY >= dialogY + 50 && releaseY < dialogY + 100) {
                            gameState = EDITOR;
                        }

                        char direction[33];
                        sprintf(direction, "levels/%s.json", levelName);

                        // save
                        if (releaseY >= dialogY + 150 && releaseY <= dialogY + 200) {
                            if (spawnX < 0 || spawnY < 0 || !isLevelValid) {
                                gameState = EDITOR_NOT_VALID;
                            }
                            else {
                                saveLevel(direction);
                                loadLevel(direction);
                            }
                        }

                        // save and play
                        if (releaseY >= dialogY + 250 && releaseY <= dialogY + 300) {
                            if (spawnX < 0 || spawnY < 0 || !isLevelValid) {
                                gameState = EDITOR_NOT_VALID;
                            }
                            else {
                                saveLevel(direction);
                                loadLevel(direction);
                                Mix_PlayChannel(-1, sounds.play, 0);
                                Mix_PlayMusic(sounds.gameMusic, -1);
                                gameState = CUSTOM_PLAYING;

                                main_level = 0;

                                remainingTime = editor.gameTime;
                                lives = editor.initialLives;
                                score = 0;
                                customSpawn(&playerX, &playerY, &targetX, &targetY, &isAnimating);
                                initCustomObjects();
                            }
                        }

                        // save and exit
                        if (releaseY >= dialogY + 350 && releaseY <= dialogY + 400) {
                            if (spawnX < 0 || spawnY < 0 || !isLevelValid) {
                                gameState = EDITOR_NOT_VALID;
                            }
                            else {
                                saveLevel(direction);
                                Mix_PlayMusic(sounds.menuMusic, -1);
                                gameState = LEVEL_MENU;
                                memset(levelName, 0, sizeof(levelName));
                                exitEditor();
                            }
                        }

                        // exit
                        if (releaseY >= dialogY + 450 && releaseY <= dialogY + 500) {
                            Mix_PlayMusic(sounds.menuMusic, -1);
                            gameState = LEVEL_MENU;
                            memset(levelName, 0, sizeof(levelName));
                            exitEditor();
                        }
                    }
                    break;
                }
                case EDITOR_SETTINGS_DIALOG: {
                    handleSettingsDialogClick(releaseX, releaseY);
                    int buttonY = (HEIGHT) / 2 + 170;

                    // Save button
                    if (releaseX >= (WIDTH - 500) / 2 + 25 && releaseX <= (WIDTH - 500) / 2 + 25 + 200 &&
                        releaseY >= buttonY && releaseY <= buttonY + 50) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        if (settingsDialog.tempLives && settingsDialog.tempGameTime) {
                            editor.initialLives = settingsDialog.tempLives;
                            editor.alligatorChance = settingsDialog.tempAlligatorChance;
                            editor.gameTime = settingsDialog.tempGameTime;
                        }
                        gameState = EDITOR;
                    }
                    // Cancel button
                    else if (releaseX >= (WIDTH - 500) / 2 + 275 && releaseX <= (WIDTH - 500) / 2 + 275 + 200 &&
                        releaseY >= buttonY && releaseY <= buttonY + 50) {
                        Mix_PlayChannel(-1, sounds.click, 0);
                        gameState = EDITOR;
                    }
                    break;
                }
                }

            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(editor.zoneTypes);
    free(editor.rowSettings);

    deleteAllTextures();
    cleanupSoundSystem();
    glfwTerminate();
    return 0;
}