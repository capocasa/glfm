// GLFM
// https://github.com/brackeen/glfm

#ifndef GLFM_H
#define GLFM_H

#define GLFM_VERSION_MAJOR 0
#define GLFM_VERSION_MINOR 10
#define GLFM_VERSION_REVISION 0

#if !defined(__APPLE__) && !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
#  error Unsupported platform
#endif

#if defined(__APPLE__)
#  include <TargetConditionals.h>
#  if !(TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_OSX)
#    error Unsupported Apple platform
#  endif
#  if !defined(GL_SILENCE_DEPRECATION)
#    define GL_SILENCE_DEPRECATION
#  endif
#endif

// OpenGL ES includes

#if defined(GLFM_INCLUDE_ES32)
#  if defined(__ANDROID__)
#    include <GLES3/gl32.h>
#    include <GLES3/gl3ext.h>
#  else
#    error OpenGL ES 3.2 only supported on Android
#  endif
#elif defined(GLFM_INCLUDE_ES31)
#  if defined(__ANDROID__)
#    include <GLES3/gl31.h>
#    include <GLES3/gl3ext.h>
#  else
#    error OpenGL ES 3.1 only supported on Android
#  endif
#elif defined(GLFM_INCLUDE_ES3)
#  if defined(__APPLE__)
#    if TARGET_OS_OSX
#      error OpenGL ES unavailable on macOS
#    else
#      include <OpenGLES/ES3/gl.h>
#      include <OpenGLES/ES3/glext.h>
#    endif
#  elif defined(__EMSCRIPTEN__)
#    include <GLES3/gl3.h>
#    include <GLES3/gl2ext.h>
#  else
#    include <GLES3/gl3.h>
#    include <GLES3/gl3ext.h>
#  endif
#elif !defined(GLFM_INCLUDE_NONE)
#  if defined(__APPLE__)
#    if TARGET_OS_OSX
#      include <OpenGL/gl3.h>
#    else
#      include <OpenGLES/ES2/gl.h>
#      include <OpenGLES/ES2/glext.h>
#    endif
#  else
#    include <GLES2/gl2.h>
#    include <GLES2/gl2ext.h>
#  endif
#endif

#ifdef __GNUC__
#  define GLFM_DEPRECATED(message) __attribute__((deprecated(message)))
#else
#  define GLFM_DEPRECATED(message)
#endif

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Enums

typedef enum {
    GLFMRenderingAPIOpenGLES2,
    GLFMRenderingAPIOpenGLES3,
    GLFMRenderingAPIOpenGLES31,
    GLFMRenderingAPIOpenGLES32,
    GLFMRenderingAPIMetal,
} GLFMRenderingAPI;

typedef enum {
    GLFMColorFormatRGBA8888,
    GLFMColorFormatRGB565,
} GLFMColorFormat;

typedef enum {
    GLFMDepthFormatNone,
    GLFMDepthFormat16,
    GLFMDepthFormat24,
} GLFMDepthFormat;

typedef enum {
    GLFMStencilFormatNone,
    GLFMStencilFormat8,
} GLFMStencilFormat;

typedef enum {
    GLFMMultisampleNone,
    GLFMMultisample4X,
} GLFMMultisample;

typedef enum {
    GLFMSwapBehaviorPlatformDefault,
    GLFMSwapBehaviorBufferDestroyed,
    GLFMSwapBehaviorBufferPreserved,
} GLFMSwapBehavior;

/// Defines whether system UI chrome (status bar, navigation bar) is shown.
typedef enum {
    /// Displays the app with the navigation bar.
    ///  - Android: Show the navigation bar.
    ///  - iOS: Show the home indicator on iPhone X.
    ///  - Emscripten: Display the browser normally.
    GLFMUserInterfaceChromeNavigation,
    /// Displays the app with both the navigation bar and the status bar.
    ///  - Android: Show the navigation bar and status bar.
    ///  - iOS: Show status bar, and show the home indicator on iPhone X.
    ///  - Emscripten: Display the browser normally.
    GLFMUserInterfaceChromeNavigationAndStatusBar,
    /// Displays the app fullscreen.
    ///  - Android 2.3: Fullscreen.
    ///  - Android 4.0 - 4.3: Navigation bar dimmed.
    ///  - Android 4.4: Fullscreen immersive mode.
    ///  - iOS: Fullscreen.
    ///  - Emscripten: Requests fullscreen display for the browser window.
    GLFMUserInterfaceChromeFullscreen,
} GLFMUserInterfaceChrome;

typedef enum {
    GLFMInterfaceOrientationUnknown = 0,
    GLFMInterfaceOrientationPortrait = (1 << 0),
    GLFMInterfaceOrientationPortraitUpsideDown = (1 << 1),
    GLFMInterfaceOrientationLandscapeLeft = (1 << 2),
    GLFMInterfaceOrientationLandscapeRight = (1 << 3),
    GLFMInterfaceOrientationLandscape = (GLFMInterfaceOrientationLandscapeLeft |
                                         GLFMInterfaceOrientationLandscapeRight),
    GLFMInterfaceOrientationAll = (GLFMInterfaceOrientationPortrait |
                                   GLFMInterfaceOrientationPortraitUpsideDown |
                                   GLFMInterfaceOrientationLandscapeLeft |
                                   GLFMInterfaceOrientationLandscapeRight),
    GLFMInterfaceOrientationAllButUpsideDown = (GLFMInterfaceOrientationPortrait |
                                                GLFMInterfaceOrientationLandscapeLeft |
                                                GLFMInterfaceOrientationLandscapeRight),
} GLFMInterfaceOrientation;

/// *Deprecated:* See ``GLFMInterfaceOrientation``.
typedef enum {
    GLFMUserInterfaceOrientationAny GLFM_DEPRECATED("Replaced with GLFMInterfaceOrientationAll") = GLFMInterfaceOrientationAll,
    GLFMUserInterfaceOrientationPortrait GLFM_DEPRECATED("Replaced with GLFMInterfaceOrientationPortrait") = GLFMInterfaceOrientationPortrait,
    GLFMUserInterfaceOrientationLandscape GLFM_DEPRECATED("Replaced with GLFMInterfaceOrientationLandscape") = GLFMInterfaceOrientationLandscape,
} GLFMUserInterfaceOrientation GLFM_DEPRECATED("Replaced with GLFMInterfaceOrientation");

typedef enum {
    GLFMTouchPhaseHover,
    GLFMTouchPhaseBegan,
    GLFMTouchPhaseMoved,
    GLFMTouchPhaseEnded,
    GLFMTouchPhaseCancelled,
} GLFMTouchPhase;

typedef enum {
    GLFMMouseCursorAuto,
    GLFMMouseCursorNone,
    GLFMMouseCursorDefault,
    GLFMMouseCursorPointer,
    GLFMMouseCursorCrosshair,
    GLFMMouseCursorText
} GLFMMouseCursor;

typedef enum {
    GLFMMouseWheelDeltaPixel,
    GLFMMouseWheelDeltaLine,
    GLFMMouseWheelDeltaPage
} GLFMMouseWheelDeltaType;

typedef enum {
    GLFMKeyUnknown           = 0x00,

    // The key codes below have the same values as ASCII codes (uppercase)

    GLFMKeyBackspace         = 0x08, // Apple: Delete
    GLFMKeyTab               = 0x09,
    GLFMKeyEnter             = 0x0D, // Apple: Return
    GLFMKeyEscape            = 0x1B,
    GLFMKeySpace             = 0x20,
    GLFMKeyQuote             = 0x27,
    GLFMKeyComma             = 0x2C,
    GLFMKeyMinus             = 0x2D,
    GLFMKeyPeriod            = 0x2E,
    GLFMKeySlash             = 0x2F,
    GLFMKeyDigit0            = 0x30,
    GLFMKeyDigit1            = 0x31,
    GLFMKeyDigit2            = 0x32,
    GLFMKeyDigit3            = 0x33,
    GLFMKeyDigit4            = 0x34,
    GLFMKeyDigit5            = 0x35,
    GLFMKeyDigit6            = 0x36,
    GLFMKeyDigit7            = 0x37,
    GLFMKeyDigit8            = 0x38,
    GLFMKeyDigit9            = 0x39,
    GLFMKeySemicolon         = 0x3B,
    GLFMKeyEqual             = 0x3D,
    GLFMKeyA                 = 0x41,
    GLFMKeyB                 = 0x42,
    GLFMKeyC                 = 0x43,
    GLFMKeyD                 = 0x44,
    GLFMKeyE                 = 0x45,
    GLFMKeyF                 = 0x46,
    GLFMKeyG                 = 0x47,
    GLFMKeyH                 = 0x48,
    GLFMKeyI                 = 0x49,
    GLFMKeyJ                 = 0x4A,
    GLFMKeyK                 = 0x4B,
    GLFMKeyL                 = 0x4C,
    GLFMKeyM                 = 0x4D,
    GLFMKeyN                 = 0x4E,
    GLFMKeyO                 = 0x4F,
    GLFMKeyP                 = 0x50,
    GLFMKeyQ                 = 0x51,
    GLFMKeyR                 = 0x52,
    GLFMKeyS                 = 0x53,
    GLFMKeyT                 = 0x54,
    GLFMKeyU                 = 0x55,
    GLFMKeyV                 = 0x56,
    GLFMKeyW                 = 0x57,
    GLFMKeyX                 = 0x58,
    GLFMKeyY                 = 0x59,
    GLFMKeyZ                 = 0x5A,
    GLFMKeyBracketLeft       = 0x5B,
    GLFMKeyBackslash         = 0x5C,
    GLFMKeyBracketRight      = 0x5D,
    GLFMKeyBackquote         = 0x60, // Grave
    GLFMKeyDelete            = 0x7F, // Apple: Forward Delete

    // Non-ASCII keys. These values may change in the future.

    GLFMKeyCapsLock          = 0x80,
    GLFMKeyShiftLeft         = 0x81,
    GLFMKeyShiftRight        = 0x82,
    GLFMKeyControlLeft       = 0x83,
    GLFMKeyControlRight      = 0x84,
    GLFMKeyAltLeft           = 0x85, // Apple: Option
    GLFMKeyAltRight          = 0x86, // Apple: Option
    GLFMKeyMetaLeft          = 0x87, // Apple: Command
    GLFMKeyMetaRight         = 0x88, // Apple: Command
    GLFMKeyMenu              = 0x89, // Context menu

    GLFMKeyInsert            = 0x90,
    GLFMKeyPageUp            = 0x91,
    GLFMKeyPageDown          = 0x92,
    GLFMKeyEnd               = 0x93,
    GLFMKeyHome              = 0x94,
    GLFMKeyArrowLeft         = 0x95,
    GLFMKeyArrowUp           = 0x96,
    GLFMKeyArrowRight        = 0x97,
    GLFMKeyArrowDown         = 0x98,

    GLFMKeyPower             = 0x99,
    GLFMKeyFunction          = 0x9A, // Apple: Fn
    GLFMKeyPrintScreen       = 0x9B, // System Request
    GLFMKeyScrollLock        = 0x9C,
    GLFMKeyPause             = 0x9D, // Break

    GLFMKeyNumLock           = 0xA0, // Apple: Clear
    GLFMKeyNumpadDecimal     = 0xA1,
    GLFMKeyNumpadMultiply    = 0xA2,
    GLFMKeyNumpadAdd         = 0xA3,
    GLFMKeyNumpadDivide      = 0xA4,
    GLFMKeyNumpadEnter       = 0xA5,
    GLFMKeyNumpadSubtract    = 0xA6,
    GLFMKeyNumpadEqual       = 0xA7,

    GLFMKeyNumpad0           = 0xB0,
    GLFMKeyNumpad1           = 0xB1,
    GLFMKeyNumpad2           = 0xB2,
    GLFMKeyNumpad3           = 0xB3,
    GLFMKeyNumpad4           = 0xB4,
    GLFMKeyNumpad5           = 0xB5,
    GLFMKeyNumpad6           = 0xB6,
    GLFMKeyNumpad7           = 0xB7,
    GLFMKeyNumpad8           = 0xB8,
    GLFMKeyNumpad9           = 0xB9,

    GLFMKeyF1                = 0xC1,
    GLFMKeyF2                = 0xC2,
    GLFMKeyF3                = 0xC3,
    GLFMKeyF4                = 0xC4,
    GLFMKeyF5                = 0xC5,
    GLFMKeyF6                = 0xC6,
    GLFMKeyF7                = 0xC7,
    GLFMKeyF8                = 0xC8,
    GLFMKeyF9                = 0xC9,
    GLFMKeyF10               = 0xD0,
    GLFMKeyF11               = 0xD1,
    GLFMKeyF12               = 0xD2,
    GLFMKeyF13               = 0xD3,
    GLFMKeyF14               = 0xD4,
    GLFMKeyF15               = 0xD5,
    GLFMKeyF16               = 0xD6,
    GLFMKeyF17               = 0xD7,
    GLFMKeyF18               = 0xD8,
    GLFMKeyF19               = 0xD9,
    GLFMKeyF20               = 0xDA,
    GLFMKeyF21               = 0xDB,
    GLFMKeyF22               = 0xDC,
    GLFMKeyF23               = 0xDD,
    GLFMKeyF24               = 0xDE,

    GLFMKeyNavigationBack    = 0xE0, // Android (soft) back button, tvOS menu/back button.
    GLFMKeyMediaSelect       = 0xE1, // tvOS
    GLFMKeyMediaPlayPause    = 0xE2, // tvOS

} GLFMKey;

enum {
    GLFMKeyLeft GLFM_DEPRECATED("Replaced with GLFMKeyArrowLeft") = GLFMKeyArrowLeft,
    GLFMKeyUp GLFM_DEPRECATED("Replaced with GLFMKeyArrowUp") = GLFMKeyArrowUp,
    GLFMKeyRight GLFM_DEPRECATED("Replaced with GLFMKeyArrowRight") = GLFMKeyArrowRight,
    GLFMKeyDown GLFM_DEPRECATED("Replaced with GLFMKeyArrowDown") = GLFMKeyArrowDown,
    GLFMKeyPlayPause GLFM_DEPRECATED("Replaced with GLFMKeyMediaPlayPause") = GLFMKeyMediaPlayPause,
    GLFMKeyNavBack GLFM_DEPRECATED("Replaced with GLFMKeyNavigationBack") = GLFMKeyNavigationBack,
    GLFMKeyNavSelect GLFM_DEPRECATED("Replaced with GLFMKeyMediaSelect") = GLFMKeyMediaSelect,
};

typedef enum {
    GLFMKeyModifierShift    = (1 << 0),
    GLFMKeyModifierCtrl     = (1 << 1),
    GLFMKeyModifierAlt      = (1 << 2), // Apple: Option
    GLFMKeyModifierMeta     = (1 << 3), // Apple: Command
    GLFMKeyModifierFunction = (1 << 4), // Apple: Fn
} GLFMKeyModifier;

typedef enum {
    GLFMKeyActionPressed,
    GLFMKeyActionRepeated,
    GLFMKeyActionReleased,
} GLFMKeyAction;

/// The hardware sensor type. See ``glfmIsSensorAvailable`` and ``glfmSetSensorFunc``.
typedef enum {
    /// Accelerometer sensor.
    /// In ``GLFMSensorFunc``, the `GLFMSensorEvent` vector is the acceleration in G's.
    GLFMSensorAccelerometer,
    /// Magnetometer sensor.
    /// In ``GLFMSensorFunc``, the `GLFMSensorEvent` vector is the magnetic field in microteslas.
    GLFMSensorMagnetometer,
    /// Gyroscope sensor.
    /// In ``GLFMSensorFunc``, the `GLFMSensorEvent` vector is the rotation rate in radians/second.
    GLFMSensorGyroscope,
    /// Rotation sensor.
    /// In ``GLFMSensorFunc``, the `GLFMSensorEvent` matrix is the rotation matrix where the
    /// X axis points North and the Z axis is vertical.
    GLFMSensorRotationMatrix,
} GLFMSensor;

typedef enum {
    GLFMHapticFeedbackLight,
    GLFMHapticFeedbackMedium,
    GLFMHapticFeedbackHeavy,
} GLFMHapticFeedbackStyle;

// MARK: - Structs and function pointers

typedef struct GLFMDisplay GLFMDisplay;

/// Function pointer returned from ``glfmGetProcAddress``.
typedef void (*GLFMProc)(void);

/// Render callback function. See ``glfmSetRenderFunc``.
typedef void (*GLFMRenderFunc)(GLFMDisplay *display);

/// *Deprecated:* Use ``GLFMRenderFunc``.
typedef void (*GLFMMainLoopFunc)(GLFMDisplay *display, double frameTime)
GLFM_DEPRECATED("See glfmSetRenderFunc and glfmSwapBuffers");

/// Callback function when mouse or touch events occur. See ``glfmSetTouchFunc``.
///
/// - Parameters:
///   - touch: The touch number (zero for primary touch, 1+ for multitouch), or
///            the mouse button number (zero for the primary button, one for secondary, etc.).
///   - phase: The touch phase.
///   - x: The x location of the event, in pixels.
///   - y: The y location of the event, in pixels.
/// - Returns: `true` if the event was handled, `false` otherwise.
typedef bool (*GLFMTouchFunc)(GLFMDisplay *display, int touch, GLFMTouchPhase phase,
                              double x, double y);

/// Callback function when key events occur. See ``glfmSetKeyFunc``.
///
/// - Android and tvOS: When the user presses the back button (`GLFMKeyNavigationBack`), this
/// function should return `false` to allow the user to exit the app, or return `true` if the back
/// button was handled in-app.
/// - Returns: `true` if the event was handled, `false` otherwise.
typedef bool (*GLFMKeyFunc)(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action,
                            int modifiers);

/// Callback function when character input events occur. See ``glfmSetCharFunc``.
typedef void (*GLFMCharFunc)(GLFMDisplay *display, const char *utf8, int modifiers);

/// Callback function when mouse wheel input events occur. See ``glfmSetMouseWheelFunc``.
/// - Parameters:
///   - x: The x location of the event, in pixels.
///   - y: The y location of the event, in pixels.
/// - Returns: `true` if the event was handled, `false` otherwise.
typedef bool (*GLFMMouseWheelFunc)(GLFMDisplay *display, double x, double y,
                                   GLFMMouseWheelDeltaType deltaType,
                                   double deltaX, double deltaY, double deltaZ);

/// Callback function when the virtual keyboard visibility changes.
/// See ``glfmSetKeyboardVisibilityChangedFunc``.
typedef void (*GLFMKeyboardVisibilityChangedFunc)(GLFMDisplay *display, bool visible,
                                                  double x, double y, double width, double height);

/// Callback function when the app interface orientation changes.
/// See ``glfmSetOrientationChangedFunc``.
typedef void (*GLFMOrientationChangedFunc)(GLFMDisplay *display,
                                           GLFMInterfaceOrientation orientation);

/// Callback function when the surface could not be created.
/// See ``glfmSetSurfaceErrorFunc``.
typedef void (*GLFMSurfaceErrorFunc)(GLFMDisplay *display, const char *message);

/// Callback function when the OpenGL surface was created.
/// See ``glfmSetSurfaceCreatedFunc``.
typedef void (*GLFMSurfaceCreatedFunc)(GLFMDisplay *display, int width, int height);

/// Callback function when the OpenGL surface was resized (or rotated).
/// See ``glfmSetSurfaceResizedFunc``.
typedef void (*GLFMSurfaceResizedFunc)(GLFMDisplay *display, int width, int height);

/// Callback function to notify that the surface needs to be redrawn.
/// See ``glfmSetSurfaceRefreshFunc``.
typedef void (*GLFMSurfaceRefreshFunc)(GLFMDisplay *display);

/// Callback function when the OpenGL surface was destroyed.
/// See ``glfmSetSurfaceDestroyedFunc``.
typedef void (*GLFMSurfaceDestroyedFunc)(GLFMDisplay *display);

/// Callback function when the system receives a low memory warning.
/// See ``glfmSetMemoryWarningFunc``.
typedef void (*GLFMMemoryWarningFunc)(GLFMDisplay *display);

/// Callback function when the app loses or gains focus. See ``glfmSetAppFocusFunc``.
typedef void (*GLFMAppFocusFunc)(GLFMDisplay *display, bool focused);

/// The result used in the hardware sensor callback. See ``glfmSetSensorFunc``.
///
/// The `vector` is used for all sensor types except for `GLFMSensorRotationMatrix`,
/// which uses `matrix`.
typedef struct {
    /// The sensor type
    GLFMSensor sensor;
    /// The timestamp of the event, which may not be related to wall-clock time.
    double timestamp;
    union {
        /// A three-dimensional vector.
        struct {
            double x, y, z;
        } vector;
        /// A 3x3 matrix.
        struct {
            double m00, m01, m02;
            double m10, m11, m12;
            double m20, m21, m22;
        } matrix;
    };
} GLFMSensorEvent;

/// Callback function when sensor events occur. See ``glfmSetSensorFunc``.
typedef void (*GLFMSensorFunc)(GLFMDisplay *display, GLFMSensorEvent event);

// MARK: - Functions

/// Main entry point for a GLFM app.
///
/// In this function, call ``glfmSetDisplayConfig`` and ``glfmSetRenderFunc``.
extern void glfmMain(GLFMDisplay *display);

/// Sets the requested display configuration.
///
/// This function should only be called in ``glfmMain``.
///
/// If the device does not support the preferred rendering API, the next available rendering API is
/// used (OpenGL ES 2.0 if OpenGL ES 3.0 is not available, for example).
/// Call ``glfmGetRenderingAPI`` in the ``GLFMSurfaceCreatedFunc`` to check which rendering API is
/// used.
void glfmSetDisplayConfig(GLFMDisplay *display,
                          GLFMRenderingAPI preferredAPI,
                          GLFMColorFormat colorFormat,
                          GLFMDepthFormat depthFormat,
                          GLFMStencilFormat stencilFormat,
                          GLFMMultisample multisample);

/// Sets the user data pointer.
///
/// The data is neither read nor modified. See ``glfmGetUserData``.
void glfmSetUserData(GLFMDisplay *display, void *userData);

/// Gets the user data pointer.
///
/// See ``glfmSetUserData``.
void *glfmGetUserData(const GLFMDisplay *display);

/// Swap buffers.
///
/// This function should be called at the end of the ``GLFMRenderFunc`` if any content was rendered.
///
/// - Emscripten: Rhis function does nothing. Buffer swapping happens automatically if any
/// OpenGL calls were made.
///
/// - Apple platforms: When using the Metal rendering API, this function does nothing.
/// Presenting the Metal drawable must happen in application code.
void glfmSwapBuffers(GLFMDisplay *display);

/// *Deprecated:* Use ``glfmGetSupportedInterfaceOrientation``.
GLFMUserInterfaceOrientation glfmGetUserInterfaceOrientation(GLFMDisplay *display)
GLFM_DEPRECATED("Replaced with glfmGetSupportedInterfaceOrientation");

/// *Deprecated:* Use ``glfmSetSupportedInterfaceOrientation``.
void glfmSetUserInterfaceOrientation(GLFMDisplay *display,
                                     GLFMUserInterfaceOrientation supportedOrientations)
GLFM_DEPRECATED("Replaced with glfmSetSupportedInterfaceOrientation");

/// Returns the supported user interface orientations. Default is `GLFMInterfaceOrientationAll`.
///
/// Actual support may be limited by the device or platform.
GLFMInterfaceOrientation glfmGetSupportedInterfaceOrientation(const GLFMDisplay *display);

/// Sets the supported user interface orientations.
///
/// Typical values are `GLFMInterfaceOrientationAll`, `GLFMInterfaceOrientationPortrait`, or
/// `GLFMInterfaceOrientationLandscape.`
///
/// Actual support may be limited by the device or platform.
void glfmSetSupportedInterfaceOrientation(GLFMDisplay *display,
                                          GLFMInterfaceOrientation supportedOrientations);

/// Gets the current user interface orientation.
///
/// - Returns: Either `GLFMInterfaceOrientationPortrait`, `GLFMInterfaceOrientationLandscapeLeft`,
///   `GLFMInterfaceOrientationLandscapeRight`, `GLFMInterfaceOrientationPortraitUpsideDown`, or
///   `GLFMInterfaceOrientationUnknown`.
GLFMInterfaceOrientation glfmGetInterfaceOrientation(const GLFMDisplay *display);

/// Gets the display size, in pixels.
void glfmGetDisplaySize(const GLFMDisplay *display, int *width, int *height);

/// Gets the display scale.
///
/// On Apple platforms, the value will be 1.0 for non-retina displays and 2.0
/// for retina. Similar values will be returned for Android and Emscripten.
double glfmGetDisplayScale(const GLFMDisplay *display);

/// Gets the chrome insets, in pixels (AKA "safe area insets" in iOS).
///
/// The "insets" are the space taken on the outer edges of the display by status bars,
/// navigation bars, and other UI elements.
void glfmGetDisplayChromeInsets(const GLFMDisplay *display, double *top, double *right,
                                double *bottom, double *left);

/// Gets the user interface chrome.
GLFMUserInterfaceChrome glfmGetDisplayChrome(const GLFMDisplay *display);

/// Sets the user interface chrome.
///
/// - Emscripten: To switch to fullscreen, this function must be called from an user-generated
/// event handler.
void glfmSetDisplayChrome(GLFMDisplay *display, GLFMUserInterfaceChrome uiChrome);

/// Gets the rendering API of the display.
///
/// Defaults to `GLFMRenderingAPIOpenGLES2`.
///
/// The return value is not valid until the surface is created.
GLFMRenderingAPI glfmGetRenderingAPI(const GLFMDisplay *display);

/// Sets the swap behavior for newly created surfaces (Android only).
///
/// In order to take effect, the behavior should be set before the surface
/// is created, preferable at the very beginning of the ``glfmMain`` function.
void glfmSetSwapBehavior(GLFMDisplay *display, GLFMSwapBehavior behavior);

/// Returns the swap buffer behavior.
GLFMSwapBehavior glfmGetSwapBehavior(const GLFMDisplay *display);

/// Gets the address of the specified function.
GLFMProc glfmGetProcAddress(const char *functionName);

/// Gets the value of the highest precision time available, in seconds.
///
/// The time should not be considered related to wall-clock time.
double glfmGetTime(void);

// MARK: - Callback functions

/// Sets the function to call before each frame is displayed.
///
/// This function is called at regular intervals (typically 60fps).
/// Applications will typically render in this callback. If the application rendered any content,
/// the application should call ``glfmSwapBuffers`` before returning. If the application did
/// not render content, it should return without calling ``glfmSwapBuffers``.
GLFMRenderFunc glfmSetRenderFunc(GLFMDisplay *display, GLFMRenderFunc renderFunc);

/// *Deprecated:* Use ``glfmSetRenderFunc``.
///
/// If this function is set, ``glfmSwapBuffers`` is called after calling the `GLFMMainLoopFunc`.
GLFMMainLoopFunc glfmSetMainLoopFunc(GLFMDisplay *display, GLFMMainLoopFunc mainLoopFunc)
GLFM_DEPRECATED("See glfmSetRenderFunc and glfmSwapBuffers");

/// Sets the function to call when the surface could not be created.
///
/// For example, the browser does not support WebGL.
GLFMSurfaceErrorFunc glfmSetSurfaceErrorFunc(GLFMDisplay *display,
                                             GLFMSurfaceErrorFunc surfaceErrorFunc);

/// Sets the function to call when the surface was created.
GLFMSurfaceCreatedFunc glfmSetSurfaceCreatedFunc(GLFMDisplay *display,
                                                 GLFMSurfaceCreatedFunc surfaceCreatedFunc);

/// Sets the function to call when the surface was resized (or rotated).
GLFMSurfaceResizedFunc glfmSetSurfaceResizedFunc(GLFMDisplay *display,
                                                 GLFMSurfaceResizedFunc surfaceResizedFunc);

/// Sets the function to call to notify that the surface needs to be redrawn.
///
/// This callback is called when returning from the background, or when the device was rotated.
/// The `GLFMRenderFunc` is called immediately after `GLFMSurfaceRefreshFunc`.
GLFMSurfaceRefreshFunc glfmSetSurfaceRefreshFunc(GLFMDisplay *display,
                                                 GLFMSurfaceRefreshFunc surfaceRefreshFunc);

/// Sets the function to call when the surface was destroyed.
///
/// The surface may be destroyed during OpenGL context loss.
/// All OpenGL resources should be deleted in this call.
GLFMSurfaceDestroyedFunc glfmSetSurfaceDestroyedFunc(GLFMDisplay *display,
                                                     GLFMSurfaceDestroyedFunc surfaceDestroyedFunc);

/// Sets the function to call when app interface orientation changes.
GLFMOrientationChangedFunc
glfmSetOrientationChangedFunc(GLFMDisplay *display,
                              GLFMOrientationChangedFunc orientationChangedFunc);

/// Sets the function to call when the system sends a "low memory" warning.
GLFMMemoryWarningFunc glfmSetMemoryWarningFunc(GLFMDisplay *display,
                                               GLFMMemoryWarningFunc lowMemoryFunc);

/// Sets the function to call when the app loses or gains focus (goes into the background or returns
/// from the background).
///
/// - Emscripten: This function is called when switching browser tabs and
/// before the page is unloaded.
GLFMAppFocusFunc glfmSetAppFocusFunc(GLFMDisplay *display, GLFMAppFocusFunc focusFunc);

// MARK: - Input functions

/// Sets whether multitouch input is enabled. By default, multitouch is disabled.
void glfmSetMultitouchEnabled(GLFMDisplay *display, bool multitouchEnabled);

/// Gets whether multitouch input is enabled. By default, multitouch is disabled.
bool glfmGetMultitouchEnabled(const GLFMDisplay *display);

/// Gets whether the display has touch capabilities.
bool glfmHasTouch(const GLFMDisplay *display);

/// Checks if a hardware sensor is available.
///
/// - Emscripten: Always returns `false`.
bool glfmIsSensorAvailable(const GLFMDisplay *display, GLFMSensor sensor);

/// Sets the mouse cursor (only on platforms with a mouse).
void glfmSetMouseCursor(GLFMDisplay *display, GLFMMouseCursor mouseCursor);

/// Requests to show or hide the onscreen virtual keyboard.
///
/// - Emscripten: this function does nothing.
void glfmSetKeyboardVisible(GLFMDisplay *display, bool visible);

/// Returns `true` if the virtual keyboard is currently visible.
bool glfmIsKeyboardVisible(const GLFMDisplay *display);

/// Sets the function to call when the virtual keyboard changes visibility or changes bounds.
GLFMKeyboardVisibilityChangedFunc
glfmSetKeyboardVisibilityChangedFunc(GLFMDisplay *display,
                                     GLFMKeyboardVisibilityChangedFunc visibilityChangedFunc);

/// Sets the function to call when a mouse or touch event occurs.
GLFMTouchFunc glfmSetTouchFunc(GLFMDisplay *display, GLFMTouchFunc touchFunc);

/// Sets the function to call when a key event occurs.
///
/// - iOS: Only pressed events are sent (no repeated or released events) and with no modifiers.
///
/// - Android and tvOS: When the user presses the back button (`GLFMKeyNavigationBack`), the
/// `GLFMKeyFunc` function should return `false` to allow the user to exit the app, or return `true`
/// if the back button was handled in-app.
GLFMKeyFunc glfmSetKeyFunc(GLFMDisplay *display, GLFMKeyFunc keyFunc);

/// Sets the function to call when character input events occur.
GLFMCharFunc glfmSetCharFunc(GLFMDisplay *display, GLFMCharFunc charFunc);

/// Sets the function to call when the mouse wheel is moved.
///
/// Only enabled on Emscripten.
GLFMMouseWheelFunc glfmSetMouseWheelFunc(GLFMDisplay *display, GLFMMouseWheelFunc mouseWheelFunc);

/// Sets the function to call when the hardware sensor events occur for a particular sensor.
///
/// If the hardware sensor is not available, this function does nothing.
/// See ``glfmIsSensorAvailable``.
///
/// Each ``GLFMSensor`` type can have it's own ``GLFMSensorFunc``.
///
/// The hardware sensor is enabled when the `sensorFunc` is not `NULL`.
///
/// Sensor events can drain battery. To save battery, when sensor events are not needed,
/// set the `sensorFunc` to `NULL` to disable the sensor.
///
/// Sensors are automatically disabled when the app is inactive, and re-enabled when active again.
GLFMSensorFunc glfmSetSensorFunc(GLFMDisplay *display, GLFMSensor sensor, GLFMSensorFunc sensorFunc);

// MARK: - Haptics

/// Returns true if the device supports haptic feedback.
///
/// - iOS: Returns `true` if the device supports haptic feedback (iPhone 7 or newer) and
///   the device is running iOS 13 or newer.
/// - Emscripten: Always returns `false`.
bool glfmIsHapticFeedbackSupported(const GLFMDisplay *display);

/// Performs haptic feedback.
///
/// - Emscripten: This function does nothing.
void glfmPerformHapticFeedback(GLFMDisplay *display, GLFMHapticFeedbackStyle style);

// MARK: - Platform-specific functions

/// Returns `true` if this is an Apple platform that supports Metal, `false` otherwise.
bool glfmIsMetalSupported(const GLFMDisplay *display);

#if defined(__APPLE__) || defined(GLFM_EXPOSE_NATIVE_APPLE)

/// *Apple platforms only*: Returns a pointer to an `MTKView` instance, or `NULL` if Metal is not
/// available.
///
/// This will only return a valid reference after the surface was created.
void *glfmGetMetalView(const GLFMDisplay *display);

/// *Apple platforms only*: Returns a pointer to the `UIViewController` (iOS, tvOS) or the
/// `NSViewController` (macOS) used to display content.
void *glfmGetViewController(const GLFMDisplay *display);

#endif // GLFM_EXPOSE_NATIVE_APPLE

#if defined(__ANDROID__) || defined(GLFM_EXPOSE_NATIVE_ANDROID)

#if defined(__ANDROID__)
#  include <android/native_activity.h>
#else
typedef struct ANativeActivity ANativeActivity;
#endif

/// *Android only*: Returns a pointer to GLFM's `ANativeActivity` instance.
ANativeActivity *glfmAndroidGetActivity(void);

#endif // GLFM_EXPOSE_NATIVE_ANDROID

#ifdef __cplusplus
}
#endif

#endif // GLFM_H
