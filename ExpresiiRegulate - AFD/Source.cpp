#include <iostream>
#include <stack>
#include <algorithm>

#include "DeterministicFiniteAutomaton.h"
#include "RegularExpression.h"

DeterministicFiniteAutomaton GetAFN(const RegularExpression& r)
{
	DeterministicFiniteAutomaton f;
	std::vector<std::pair<std::string, std::string>> Tranzitie;
	char anterior = 65;
	std::stack<char> parantezaDeschisa;
	std::stack<char> parantezaInchisa;
	std::string aux;
	for (int i = 0; i < r.GetExpression().size(); i++)
	{
		if (r.GetExpression()[i] == '*')
		{
			continue;
		}
		if (r.GetExpression()[i] == '(')
		{
			aux = std::string(1, anterior) + "_";
			parantezaDeschisa.push(anterior);
		}
		else
			if (r.GetExpression()[i] == '|')
			{
				aux = std::string(1, parantezaDeschisa.top()) + "_";
				parantezaInchisa.push(anterior);
			}
			else
				if (r.GetExpression()[i] == ')')
				{
					aux = std::string(1, anterior) + '_';
					if (parantezaInchisa.size() == 0 && r.GetExpression()[i + 1] == '*')
					{
						Tranzitie.push_back(std::make_pair(aux, std::string(1, parantezaDeschisa.top() + 1)));
						aux = std::string(1, parantezaDeschisa.top()) + '_';
						Tranzitie.push_back(std::make_pair(aux, std::string(1, anterior + 1)));
						aux = std::string(1, anterior) + '_';
					}
					else
					{
						Tranzitie.push_back(std::make_pair(aux, std::string(1, anterior + 1)));
						aux = std::string(1, parantezaInchisa.top()) + "_";
						if (parantezaInchisa.size() > 0 && r.GetExpression()[i + 1] == '*')
						{
							Tranzitie.push_back(std::make_pair(aux, std::string(1, anterior + 1)));
							if (r.GetExpression()[i + 1] == '*')
							{
								aux = std::string(1, anterior + 1) + "_";
								Tranzitie.push_back(std::make_pair(aux, std::string(1, parantezaDeschisa.top())));
								for (int i = 0; i < Tranzitie.size(); i++)
									if (Tranzitie[i].first[0] >= parantezaDeschisa.top())
									{
										Tranzitie[i].first[0]++;
										Tranzitie[i].second[0]++;
									}
								aux = std::string(1, parantezaDeschisa.top()) + "_";
								Tranzitie.push_back(std::make_pair(aux, std::string(1, parantezaDeschisa.top() + 1)));
								anterior = anterior + 2;
								Tranzitie.push_back(std::make_pair(aux, std::string(1, anterior + 1)));
								aux = std::string(1, anterior) + '_';
							}
							parantezaDeschisa.pop();
							parantezaInchisa.pop();
						}
					}
				}
				else
					aux = std::string(1, anterior) + r.GetExpression()[i];
		Tranzitie.push_back(std::make_pair(aux, std::string(1, anterior + 1)));
		anterior++;
	}
	f.SetQ(std::string(1, anterior));
	f.SetSigma(r.GetExpression());
	f.SetDelta("d");
	f.SetS("A");
	f.SetF(std::string(1, anterior));
	f.SetTranzitie(Tranzitie);
	return f;
}
std::string Stare(const DeterministicFiniteAutomaton& f, const int& index)
{
	int k;
	std::string result;
	std::vector<std::pair<std::string, std::string>> Tranzitie = f.GetTranzitie();
	for (int i = 0; i < Tranzitie[index].second.size(); i++)
	{
		k = (Tranzitie[index].second[i] - 65) * (f.GetSigma().size() + 1);
		for (int j = k; j < k + f.GetSigma().size() + 1; j++)
			if (Tranzitie[j].second.size())
				if (Tranzitie[j].first[1] == '_')
					result = result + Stare(f, j);
				else
					result = result + std::string(1, Tranzitie[j].first[0]);
		if (Tranzitie[index].second[i] == Tranzitie[Tranzitie.size() - 1].first[0] && result[result.size() - 1] != Tranzitie[Tranzitie.size() - 1].first[0])
			result = result + Tranzitie[Tranzitie.size() - 1].first[0];
	}
	result.push_back(Tranzitie[index].first[0]);
	return result;
}
DeterministicFiniteAutomaton GetAFD(DeterministicFiniteAutomaton f)
{
	bool ok = false;
	char first = 65;
	std::string aux;
	std::string aux2;
	std::vector<std::pair<std::string, std::string>> Tranzitie;
	std::vector<std::string> stari;
	stari.push_back(Stare(f, 0));
	for (int i = 0; i < stari.size(); i++)
	{
		if (i > stari.size() - 1)
			break;
		for (int j = 0; j < f.GetSigma().size(); j++)
		{
			for (int k = 0; k < stari[i].size(); k++)
			{
				int l = (stari[i][k] - 65) * (f.GetSigma().size() + 1) + j + 1;
				aux = aux + f.GetTranzitie()[l].second;
			}
			for (auto k : aux)
			{
				int x = (k - 65) * (f.GetSigma().size() + 1);
				for (int l = x; l < x + f.GetSigma().size() + 1; l++)
					if (f.GetTranzitie()[l].first[1] == '_' && f.GetTranzitie()[l].second.size())
					{
						ok = true;
						aux2 = aux2 + Stare(f, l);
					}
				if (!ok)
					aux2 = aux2 + k;
				ok = false;
			}
			std::sort(aux2.begin(), aux2.end());
			if (aux2.size())
			{
				if (std::find(stari.begin(), stari.end(), aux2) == stari.end())
					stari.push_back(aux2);
				int z = std::find(stari.begin(), stari.end(), aux2) - stari.begin();
				Tranzitie.push_back(std::make_pair((std::string(1, 65 + i) + f.GetSigma()[j]), std::string(1, 65 + z)));
			}
			aux.clear();
			aux2.clear();
		}
	}
	f.SetQ(std::string(1, 65 + stari.size() - 1));
	f.SetF(std::string(1, 65 + stari.size() - 1));
	f.SetTranzitie(Tranzitie);
	return f;
}
int main()
{
	DeterministicFiniteAutomaton f;
	RegularExpression e;
	int x = 0;
	std::string word;
	if (e.IsValid())
	{
		f = GetAFN(e);
		f = GetAFD(f);
		while (x != 4)
		{
			std::cout << "1. Afisarea automatului." << std::endl;
			std::cout << "2. Afisarea expresiei regulate." << std::endl;
			std::cout << "3. Verificarea daca un cuvant este acceptata de automat." << std::endl;
			std::cout << "4. Exit" << std::endl;
			std::cin >> x;
			switch (x)
			{
			case 1:
				f.PrintAutomaton();
				break;
			case 2:
				std::cout << e.GetExpression() << std::endl;
				break;
			case 3:
				std::cout << "Cuvant: ";
				std::cin >> word;
				if (f.CheckWord(word, f.GetS()))
					std::cout << "Cuvantul este acceptat in automat." << std::endl;
				else
					std::cout << "Cuvantul nu este acceptat in automat." << std::endl;
				break;
			}
			std::cout << std::endl;
		}
	}
	return 0;
}