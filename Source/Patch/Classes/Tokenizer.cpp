#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	// Purpose: The tokenizer that splits the string into arguments. (Thanks to Crazycarpet for this Tokenizer.)
	Tokenizer::Tokenizer(const std::string& pInput, const bool pIgnoreFirst)
	{
		_tokenCount = pIgnoreFirst ? -1 : 0;

		// Parse input.
		bool inToken = false;
		bool inContainer = false;
		bool escaped = false;
		char containerStart = 0, ch;
		for (size_t i = 0, len = pInput.length(); i < len; ++i) {
			ch = pInput[i];
			switch (ch) {
			case ' ':
			case '\t':
			case '\n':
				if (!inToken) continue;
				if (inContainer) {
					AddChar(ch);
					continue;
				}
				if (escaped) {
					escaped = false;
					AddChar(ch);
					continue;
				}
				inToken = false;
				FinishToken();
				break;
			case '\'':
			case '\"':
				if (escaped) {
					AddChar(ch);
					escaped = false;
					continue;
				}
				if (!inToken) {
					inToken = true; inContainer = true;
					containerStart = ch;
					continue;
				}
				if (inContainer) {
					if (ch == containerStart) {
						inContainer = false;
						inToken = false;
						FinishToken();
						continue;
					}
					else {
						AddChar(ch);
						continue;
					}
				}
				break;
			case '\\':
				if (inContainer && pInput[i + 1] != containerStart) {
					AddChar(ch);
					continue;
				}
				else { escaped = true; }
				break;
			default:
				if (!inToken) {
					inToken = true;
				}
				AddChar(ch);
			}

			if (i == (len - 1)) {
				inToken = false;
				FinishToken();
				break;
			}
		}
	}

	void Tokenizer::AddChar(char pChar)
	{
		m_Token.push_back(pChar);
	}

	void Tokenizer::FinishToken()
	{
		if (_tokenCount != -1)
		{
			m_Tokens.push_back(m_Token);
		}
		else { m_First = m_Token; }

		++_tokenCount;
		m_Token.clear();
	}

	const bool Tokenizer::Empty()
	{
		return m_Tokens.empty();
	}
}