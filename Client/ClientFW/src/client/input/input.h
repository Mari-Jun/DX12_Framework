#pragma once

namespace client_fw
{
	class IVec2;
	class EventSystem;

	enum class eKey
	{
		kLButton = 0x01,
		kRButton = 0x02,
		kMButton = 0x04,
		kXButton1 = 0x05,
		kXButton2 = 0x06,
		kXMove = 0xEA,
		kYMove = 0xEB,

		kBackspace = 0x08,
		kTab = 0x09,
		kEnter = 0x0D,
		kShift = 0x10,
		kControl = 0x11,
		kALT = 0x12,
		kPause = 0x13,
		kCapsLock = 0x14,
		kEscape = 0x1B,

		kSpace = 0x20,
		kPageUp = 0x21,
		kPageDown = 0x22,
		kEnd = 0x23,
		kHome = 0x24,
		kLArrow = 0x25,
		kUArrow = 0x26,
		kRArrow = 0x27,
		kDArrow = 0x28,
		kSelect = 0x29,
		kPrint = 0x2A,
		kExecute = 0x2B,
		kSnapshot = 0x2C,
		kInsert = 0x2D,
		kDelete = 0x2E,
		kHelp = 0x2F,

		k0 = 0x30,
		k1 = 0x31,
		k2 = 0x32,
		k3 = 0x33,
		k4 = 0x34,
		k5 = 0x35,
		k6 = 0x36,
		k7 = 0x37,
		k8 = 0x38,
		k9 = 0x39,

		kA = 0x41,
		kB = 0x42,
		kC = 0x43,
		kD = 0x44,
		kE = 0x45,
		kF = 0x46,
		kG = 0x47,
		kH = 0x48,
		kI = 0x49,
		kJ = 0x4A,
		kK = 0x4B,
		kL = 0x4C,
		kM = 0x4D,
		kN = 0x4E,
		kO = 0x4F,
		kP = 0x50,
		kQ = 0x51,
		kR = 0x52,
		kS = 0x53,
		kT = 0x54,
		kU = 0x55,
		kV = 0x56,
		kW = 0x57,
		kX = 0x58,
		kY = 0x59,
		kZ = 0x5A,

		kNumPad0 = 0x60,
		kNumPad1 = 0x61,
		kNumPad2 = 0x62,
		kNumPad3 = 0x63,
		kNumPad4 = 0x64,
		kNumPad5 = 0x65,
		kNumPad6 = 0x66,
		kNumPad7 = 0x67,
		kNumPad8 = 0x68,
		kNumPad9 = 0x69,
		kMultiply = 0x6A,
		kAdd = 0x6B,
		kSeparator = 0x6C,
		kSubtract = 0x6D,
		kDecimal = 0x6E,
		kDevide = 0x6F,

		kF1 = 0x70,
		kF2 = 0x71,
		kF3 = 0x72,
		kF4 = 0x73,
		kF5 = 0x74,
		kF6 = 0x75,
		kF7 = 0x76,
		kF8 = 0x77,
		kF9 = 0x78,
		kF10 = 0x79,
		kF11 = 0x7A,
		kF12 = 0x7B,

		kLShift = 0xA0,
		kRShift = 0xA1,
		kLControl = 0xA2,
		kRControl = 0xA3,

		kBackTick = 0xC0,
	};

	enum class eAdditionalKey
	{
		kShift = 0x10,
		kControl = 0x11,
		kAlt = 0x12,
	};

	struct EventKeyInfo
	{
		eKey key;
		std::vector<eAdditionalKey> additional_keys;
	}; 

	struct AxisEventKeyInfo
	{
		eKey key;
		float scale;
	};

	enum class eInputMode
	{
		kEditor, kUIOnly, kUIAndGame, kGameOnly, kInActive,
	};

	enum class eInputOwnerType
	{
		kApplication, kLevel, kActor, kPawn, kPlayerController,
	};

	class Input final
	{
	public:
		static bool IsKeyHoldDown(eKey key);
		static bool IsKeyHoldDown(eAdditionalKey key);
		static bool IsKeyPressed(eKey key);
		static bool IsKeyReleased(eKey key);
		static bool IsNotKeyHoldDown(eKey key);
		static const IVec2& GetMousePosition();
		static const IVec2 GetRelativeMousePosition();

		static void ConsumeKey(eKey key);
		static bool IsConsumedKey(eKey key);

		static void SetHideCursor(bool hide);
		static bool IsHideCursor();

		static void SetClipCursor(bool clip);
		static bool IsClipCursor();

		static void StartInputMethodEditor();
		static void EndInputMethodEditor();
		static void OnChangeTextFromIME(const std::function<void(wchar_t)>& function);

		static bool RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption, eInputOwnerType type);
		static bool RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption, eInputOwnerType type);
		static bool RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption, eInputOwnerType type);
		static void UnregisterInputEvent(const std::string& name);

		static eInputMode GetInputMode();
		static void SetInputMode(eInputMode input_mode);		
		static void SetOnlyInputMode(eInputMode input_mode);		
	};
}