#include <iostream>
#include <string>
#include <algorithm>
#include <exception>

using std::cout;
using std::endl;
using std::string;

class Valid {
private:
	string cpf;

public:
	void setCpf(string cpf) {
		this->cpf = cpf;
	}

	string getCpf() {
		return this->cpf;
	}

	string limparCpf(string cpf) {
		string cpfLimpo = cpf;
		char remover[] = ".-";
		for (unsigned int i = 0; i < strlen(remover); ++i)
			cpfLimpo.erase(	std::remove(cpfLimpo.begin(), cpfLimpo.end(), remover[i]) , cpfLimpo.end() );
		this->cpf = cpfLimpo;
		return cpfLimpo;
	}

	int primeiroDigito(int iCpf) {
		/* primeiro digito */
		int total = 0;
		int peso = 10;
		int resultado = 0;

		int verificador = 0;

		for (auto & n : this->cpf) {
			resultado =  (n - '0') * peso;
			cout << "Digito: " << n << " Resultado * peso: " << resultado << endl;
			peso--;
			total += resultado;
		}	

		cout << "Soma digito 1: " << total << endl;

		total = total % 11;
		resultado = 11 - total;

		if (resultado > 9)
			verificador = 0;
		else
			verificador = resultado;

		cout << verificador;

		return verificador;
	}

	string segundoDigito(int iCpf, int digito) {
		/* segundo digito */
		int total = 0;
		int peso = 11;
		int resultado = 0;

		int verificador = 0;

		for (auto & n : this->cpf) {
			resultado = (n - '0') * peso;
			cout << "Digito: " << n << " Resultado * peso: " << resultado << endl;
			peso--;
			total += resultado;
		}

		total = total % 11;
		resultado = 11 - total;

		if (resultado > 9)
			verificador = 0;
		else
			verificador = resultado;

		string digitos = std::to_string(digito);
		digitos.append(std::to_string(verificador));

		return digitos;
	}

	bool validar(int iCpf) {
		if( this->cpf.substr(this->cpf.length() -2) == this->segundoDigito(iCpf, this->primeiroDigito(iCpf)) )
			return true;
		return false;
	}

};

int main() {

	Valid* v = new Valid();

	string cpf;

	cout << "Digite um cpf: ";
	std::cin >> cpf;

	try {
		v->setCpf(cpf);

		if (v->getCpf().length() == 14)
			v->limparCpf(v->getCpf());		

		if (v->validar(std::stoll(v->limparCpf(v->getCpf()).c_str())))
			cout << "Valido." << endl;
		else
			cout << "Nao valido." << endl;
		
	}	
	catch (std::exception e) {
		cout << e.what();
	}

	delete v;

	return 0;
}
