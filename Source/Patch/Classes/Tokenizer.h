#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class Tokenizer
	{
	public:
		// Constructors/Destructors
		Tokenizer(const std::string& pInput, const bool pIgnoreFirst = false);

		// Variables
		std::vector<std::string> m_Tokens;
		std::string m_Token;
		std::string m_First;

		const bool Empty();

	private:
		short _tokenCount;

		//Internal Methods
		void AddChar(char pChar);
		void FinishToken();
	};
}