#include "Input.h"

void InputController::Update()
{
	for (std::pair<const std::string, InputKey>& inputKey : m_inputKeys)
	{
		inputKey.second.Update();
	}

	for (std::pair<const std::string, InputAxis>& inputAxis : m_inputAxes)
	{
		inputAxis.second.Update();
	}
}

const Math::Vector2& InputController::GetAxis(const std::string& axisName) const
{
	auto foundItr = m_inputAxes.find(axisName);

#ifdef _DEBUG
	// 配列の中にない
	if (foundItr == m_inputAxes.end())
	{
		assert(0 && "登録されていない軸の検索です");

		return Math::Vector2::Zero;
	}
#endif // DEBUG

	return foundItr->second.GetAxis();
}

char InputController::GetKeyState(const std::string& keyName) const
{
	auto foundItr = m_inputKeys.find(keyName);

#ifdef _DEBUG
	// 配列の中にない
	if (foundItr == m_inputKeys.end())
	{
		assert(0 && "登録されていないキーの検索です");

		return eKeyFree;
	}
#endif // DEBUG

	return foundItr->second.GetKeyState();
}

void InputKey::Update()
{
	if (GetAsyncKeyState(m_keyCode))
	{
		// キーホールドが既にONの場合
		if (m_keyState & eKeyHold)
		{
			// プレスフラグのみをOFFにする
			m_keyState &= ~eKeyPress;
		}
		else
		{
			m_keyState |= eKeyPress | eKeyHold;
			m_keyState &= ~eKeyRelease;
		}
	}
	else
	{
		// 離された瞬間を判定
		if (m_keyState & eKeyHold)
		{
			m_keyState |= eKeyRelease;
			m_keyState &= ~eKeyPress;
			m_keyState &= ~eKeyHold;
		}
		else
		{
			m_keyState &= ~eKeyRelease;
		}
	}
}

void InputAxis::Update()
{
	for (InputKey& key : m_inputAxisKeys)
	{
		key.Update();
	}

	m_axis = Math::Vector2::Zero;

	if (m_inputAxisKeys[eUp].GetKeyState())
	{
		m_axis.y += 1.0f;
	}
	if (m_inputAxisKeys[eRight].GetKeyState())
	{
		m_axis.x += 1.0f;
	}
	if (m_inputAxisKeys[eDown].GetKeyState())
	{
		m_axis.y -= 1.0f;
	}
	if (m_inputAxisKeys[eLeft].GetKeyState())
	{
		m_axis.x -= 1.0f;
	}
}

void InputAxis::SetKeyCode(UINT up, UINT right, UINT down, UINT left)
{
	m_inputAxisKeys[eUp].SetKeyCode(up);
	m_inputAxisKeys[eRight].SetKeyCode(right);
	m_inputAxisKeys[eDown].SetKeyCode(down);
	m_inputAxisKeys[eLeft].SetKeyCode(left);
}
