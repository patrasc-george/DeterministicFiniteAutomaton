#include <algorithm>

#include "DeterministicFiniteAutomaton.h"

std::string DeterministicFiniteAutomaton::GetSigma() const
{
	return Sigma;
}

std::string DeterministicFiniteAutomaton::GetS() const
{
	return S;
}

std::vector<std::pair<std::string, std::string>> DeterministicFiniteAutomaton::GetTranzitie() const
{
	return Tranzitie;
}

void DeterministicFiniteAutomaton::SetQ(const std::string& character)
{
	Q.clear();
	char aux = 65;
	while (int(aux) <= int(character[0]))
	{
		Q = Q + aux;
		aux++;
	}
}

void DeterministicFiniteAutomaton::SetSigma(const std::string& expression)
{
	for (auto it : expression)
		if (it != '(' && it != ')' && it != '|' && it != '*')
			if (Sigma.find(it) == std::string::npos)
				Sigma = Sigma + it;
	sort(Sigma.begin(), Sigma.end());
}

void DeterministicFiniteAutomaton::SetDelta(const std::string& Delta)
{
	this->Delta = Delta;
}

void DeterministicFiniteAutomaton::SetS(const std::string& S)
{
	this->S = S;
}

void DeterministicFiniteAutomaton::SetF(const std::string& F)
{
	this->F = F;
}

void DeterministicFiniteAutomaton::SetTranzitie(const std::vector<std::pair<std::string, std::string>>& aux)
{
	Tranzitie.clear();
	int k = 0, index;
	Sigma.insert(0, "_");
	int x = Q.size() * Sigma.size();
	Tranzitie.resize(x);
	for (int i = 0; i < Q.size(); i++)
	{
		index = 0;
		for (int j = k; j < k + Sigma.size(); j++)
		{
			Tranzitie[j].first = std::string(1, Q[i]) + Sigma[index];
			index++;
		}
		k = k + Sigma.size();
	}
	Sigma.erase(Sigma.begin());
	for (int i = 0; i < aux.size(); i++)
		for (int j = 0; j < Tranzitie.size(); j++)
			if (aux[i].first == Tranzitie[j].first)
				Tranzitie[j].second = Tranzitie[j].second + aux[i].second;
}

bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
	if (Q.find(S) == std::string::npos)
		return false;
	for (auto it : F)
		if (Q.find(it) == std::string::npos)
			return false;
	for (auto i : Tranzitie)
	{
		if (Q.find(i.first[0]) == std::string::npos || Sigma.find(i.first[1]) == std::string::npos)
			return false;
		for (auto j : i.second)
			if (Q.find(j) == std::string::npos)
				return false;
	}
	return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const
{
	std::cout << "M = ({";
	for (int i = 0; i < Q.size(); i++)
	{
		std::cout << Q[i];
		if (i < Q.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, {";
	}
	for (int i = 0; i < Sigma.size(); i++)
	{
		std::cout << Sigma[i];
		if (i < Sigma.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, " << Delta << ", " << S << ", {";
	}
	for (int i = 0; i < F.size(); i++)
	{
		std::cout << F[i];
		if (i < F.size() - 1)
			std::cout << ", ";
		else
			std::cout << "})" << std::endl;
	}
	for (auto it : Tranzitie)
		if (it.second.size() > 0)
		{
			std::cout << Delta << "(" << it.first[0] << ", " << it.first[1] << ") = {";
			for (int j = 0; j < it.second.size(); j++)
			{
				std::cout << it.second[j];
				if (j < it.second.size() - 1)
					std::cout << ", ";
				else
					std::cout << "}" << std::endl;
			}
		}
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word, const std::string& start) const
{
	int ok = 0;
	for (int i = 0; i < Tranzitie.size(); i++)
	{
		if (Tranzitie[i].first[0] == start[0] && Tranzitie[i].first[1] == word[0])
		{
			ok = 1;
			for (int j = 0; j < Tranzitie[i].second.size(); j++)
			{
				std::string copyWord = word;
				std::string copyWordErase = word.erase(0, 1);
				word = copyWord;
				if (copyWordErase.size() == 0)
				{
					for (auto k : F)
						if (Tranzitie[i].second[j] == k)
							return true;
				}
				else
				{
					if (CheckWord(copyWordErase, std::string(1, Tranzitie[i].second[j])))
						return true;
				}
			}
		}
		if (ok)
			break;
	}
	return false;

}

bool DeterministicFiniteAutomaton::IsDeterministic() const
{
	for (auto it : Tranzitie)
		if (it.second.size() > 1)
			return false;
	return true;
}