#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

int main() {

    // Hash para armazenar as lestras do texto cifrado
    std::unordered_map<char, int> hash;

    // Vetor para armazenar as letras mais frequentes do português
    std::vector<char> letrasMaisFrequentes = {'a', 'e', 'o', 's', 'r', 'i', 'n', 'd', 'm', 'u', 't',
         'c', 'l', 'p', 'v', 'g', 'h', 'q', 'b', 'f', 'z', 'j', 'x', 'k', 'w', 'y'};

    // Recebe o texto cifrado
    std::string textoCifrado;
    textoCifrado = "D TXDOLGDGH GR FRGLJR YHP FRP SUDWLFD H UHYLVDR";

    // Testar todas as 26 possibilidades de deslocamento
    for (int deslocamento = 0; deslocamento < 26; deslocamento++) {
        std::string textoDecifrado;
        for (char c : textoCifrado) {
            if (std::isalpha(c)) {
                char base = std::isupper(c) ? 'A' : 'a';
                textoDecifrado += (c - base - deslocamento + 26) % 26 + base;
            } else {
                textoDecifrado += c;
            }
        }
        std::cout << "Deslocamento " << deslocamento << ": " << textoDecifrado << std::endl;
    }

    // Conta a frequência de cada letra
    for (char c : textoCifrado) {
        if (std::isalpha(c)) {
            hash[c]++;
        }
    }

    // Coloca as letras e suas frequências em um vetor para ordenação
    std::vector<std::pair<char, int>> frequencias(hash.begin(), hash.end());

    // Ordena o vetor de frequências em ordem decrescente
    std::sort(frequencias.begin(), frequencias.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
        return a.second > b.second;
    });

    // Imprime as letras mais frequentes
    std::cout << "\nLetras mais frequentes no texto cifrado:\n";
    for (const auto& par : frequencias) {
        std::cout << par.first << ": " << par.second << std::endl;
    }

    // Trocar as letras mais frequentes do texto cifrado pelas letras mais frequentes do português
    std::string textoDecifrado = textoCifrado;
    for (size_t i = 0; i < frequencias.size() && i < letrasMaisFrequentes.size(); i++) {
        char letraCifrada = frequencias[i].first;
        char letraDecifrada = letrasMaisFrequentes[i];
        std::replace(textoDecifrado.begin(), textoDecifrado.end(), letraCifrada, letraDecifrada);
    }

    std::cout << "\nTexto decifrado usando a frequência das letras:\n" << textoDecifrado << std::endl;


    return 0;

}