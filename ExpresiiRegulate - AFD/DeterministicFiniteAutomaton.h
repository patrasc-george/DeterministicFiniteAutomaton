#pragma once
#include <iostream>
#include <vector>

class DeterministicFiniteAutomaton
{
public:
	std::string GetSigma() const;
	std::string GetS() const;
	std::vector<std::pair<std::string, std::string>> GetTranzitie() const;

	void SetQ(const std::string& character);
	void SetSigma(const std::string& expression);
	void SetDelta(const std::string& Delta);
	void SetS(const std::string& S);
	void SetF(const std::string& F);
	void SetTranzitie(const std::vector<std::pair<std::string, std::string>>& aux);

	bool VerifyAutomaton() const;
	void PrintAutomaton() const;
	bool CheckWord(std::string word, const std::string& start) const;
	bool IsDeterministic() const;

private:
	std::string Q;
	std::string Sigma;
	std::string Delta;
	std::string S;
	std::string F;
	std::vector<std::pair<std::string, std::string>> Tranzitie;
};