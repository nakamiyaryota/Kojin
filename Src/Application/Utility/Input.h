#pragma once

enum KeyState
{
	eKeyFree = 0x0, // ビットフラグが1つも立っていない状態
	eKeyPress = 0x1, // ビットフラグの1番目が立っている
	eKeyHold = 0x2, // ビットフラグの2番目が立っている
	eKeyRelease = 0x4  // ビットフラグの3番目が立っている
};

class InputKey
{
public:
	InputKey() {}
	~InputKey() {}

	void Update();

	// 自身が管理するキーの状態を返す関数
	char GetKeyState() const { return m_keyState; }

	void SetKeyCode(UINT code) { m_keyCode = code; }

private:
	char m_keyState = eKeyFree;

	UINT m_keyCode = 0;
};

class InputAxis
{
public:
	void Update();

	const Math::Vector2& GetAxis() const { return m_axis; }

	void SetKeyCode(UINT up, UINT right, UINT down, UINT left);

private:
	enum AxisKeys
	{
		eUp,
		eRight,
		eDown,
		eLeft,
		eMax
	};

	Math::Vector2 m_axis;

	InputKey m_inputAxisKeys[eMax];
};

class InputController
{
public:
	InputController()
	{
		m_inputKeys["Jump"].SetKeyCode(VK_SPACE);
		m_inputKeys["ChangeScene"].SetKeyCode(VK_RETURN);
		m_inputKeys["ChangeWorld"].SetKeyCode(VK_SHIFT);
		m_inputKeys["End"].SetKeyCode(VK_ESCAPE);
		m_inputKeys["Ride"].SetKeyCode('R');

		// 軸の登録
		m_inputAxes["Move"].SetKeyCode('W', 'D', 'S', 'A');
	}

	~InputController() {}

	void Update();

	const Math::Vector2& GetAxis(const std::string& axisName) const;

	char GetKeyState(const std::string& keyName) const;

private:

	std::unordered_map<std::string, InputKey> m_inputKeys;

	// 軸クラス
	std::unordered_map<std::string, InputAxis> m_inputAxes;
};

